#include "stdafx.h"
#include <curl/curl.h>
#include "BusiLogin.h"
#include "ErrorCode.h"
#include "Function.h"
#include "RouterBusiness.h"
#include "time.h"
#include "AppManager.h"
#include "NotifyProtocol.h"
#include "ConnBusiControl.h"
#include "../DataBaseLib/MysqlLib.h"
#include "json/json.h"
#include <iconv.h>
#include "hessian/hessianLogi.h"

int HexStr2BYTEBCDArray(const char *chBuf, int len, BYTE *chBYTE)
{
	int i;

	char  chtmp;
	BYTE itmp;

	//check PN length
	if(len != (int)strlen(chBuf))
	{
		return -1;	
	}

	//Transter Hex string(chBuf) to BCD BYTE Array(chBYTE)
	for(i=0 ;i<len;i++)
	{
		chtmp = chBuf[i];
		if( chtmp<='9' && chtmp >= '0')
		{
			itmp = chtmp-'0';
		}
		else
		{
			itmp = chtmp-'a' + 10;;
		}

		if(i%2 == 0)
			chBYTE[i/2] = (itmp&0x0f)<<4;
		else
			chBYTE[i/2] += (itmp&0x0f);
	}


	return 0;
}

static long curl_writer(void *data, int size, int nmemb, string *content)
{
    long sizes = size * nmemb;
    *content += string((char*)data, sizes);
    return sizes;
}


/////////////////////// �汾����Ĵ��� //////////////////////////
#define  IMVERSION_LOW      (unsigned char)0x02
#define  IMVERSION_CANUSE   (unsigned char)0x01
#define  IMVERSION_AGREE    (unsigned char)0x00
//////////////////////////////////////////////////////////////////

CBusiLogin::CBusiLogin(CRouterBusiness* pRouter):
CBusiness(pRouter,CMD_LOGIN_REQ,CMD_LOGIN_ACK)
{
	m_uid = 0;
	m_pDataBaseWeb = m_pRouter->GetDataBaseWeb();
	m_pAppNetwork = &(CAppManager::GetInstance().GetAppNetWork());
	m_szError[0] = '\0';

	m_IsOverLoad = false;
}

CBusiLogin::~CBusiLogin(void)
{
}


//��½ҵ����Ϊһ�������ҵ����,�ǻ��������Ĵ�����ҪӦ�ò����мӽ���
int CBusiLogin::DoWork()
{
	//��ʼ����������
	RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
	LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)m_pSendData->data;
	m_szError[0] = '\0';
	m_IsOverLoad = false;
	uint32 uRet;
	XT_LOGIN_REQ req;
	
	if(lpRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
	{
		CRecvPacket rp(pRecvData->data + sizeof(XT_HEAD),pRecvData->data_len - sizeof(XT_HEAD));
		//���
		rp>>req.login_flag;
		switch (req.login_flag & 0x3)
		{
		case XT_LOGIN_REQ::LOGINBYID:
		{
			rp >> req.uLoginKey.id;
			CLog::Log("CBusiLogin", CLog::TYPE_DEBUG, "receive user\'%d\'login packet",
					req.uLoginKey.id);
			break;
		}

		case XT_LOGIN_REQ::LOGINBYNAME:
		{
			rp >> req.uLoginKey.username;
			CLog::Log("CBusiLogin", CLog::TYPE_DEBUG, "receive user\'%s\'login packet",
					req.uLoginKey.username);
			break;
		}
		case XT_LOGIN_REQ::LOGINBYMOBILENO:
		{
			rp >> req.uLoginKey.mobileno;
			CLog::Log("CBusiLogin", CLog::TYPE_DEBUG, "receive user\'%s\'login packet",
					req.uLoginKey.mobileno);
			break;
		}
		default:
			break;
		}

		rp.ReadPacket(req.random_key,sizeof(req.random_key));
		CLog::Log("CBusiLogin", CLog::TYPE_DEBUG, "receive user random_key %s",req.random_key);

		char plain[MAX_RECV_BUFFERS] = {0};
		int dwPlainLen = 0;
		dwPlainLen = rp.ReadData(plain,sizeof(plain));
		if( !xTEADecryptWithKey(plain,dwPlainLen,req.random_key, plain ,&dwPlainLen) ) 
		{
			return RESULT_FAILED;	//����ʧ��
		}

		rp.SetData(plain,dwPlainLen);

		rp.ReadPacket(req.password,sizeof(req.password));
		rp>>req.status>>req.lanip>>req.lanport;
		CLog::Log("CBusiLogin", CLog::TYPE_DEBUG, "receive user %s pwd %s status %d ip %u port %d",
			req.uLoginKey.username,req.password,req.status,req.lanip,req.lanport);

		uRet = HandleReq(&req, lpRecvHead,pRecvData->vlink);

	}
	else
	{
		XT_LOGIN_REQ*  req = (XT_LOGIN_REQ*)(pRecvData->data + sizeof(XT_HEAD));
		uRet = HandleReq(req, lpRecvHead,pRecvData->vlink);
	}

	CLog::Log("Login", CLog::TYPE_IMPORTANT, "sid:%u, did:%u, flag:%x, login_flag:%u, username:%s, status:%u, lanip:%u, lanport:%u, sessionID:%s" , 
		lpRecvHead->sid, lpRecvHead->did, lpRecvHead->flag, req.login_flag, req.uLoginKey.username, req.status, req.lanip, req.lanport, req.sessionID);	


	if(uRet != RESULT_SUCCESS)
	{
		//��װ���ش���ԭ���
		if(lpRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
		{
			if(uRet == BUSIERR_INTERNAL)
				return BUSIERR_INTERNAL;
			CSendPacket sp(m_pSendData->data + sizeof(XT_HEAD));	
			sp<<(uint8)1;
			if( m_szError[0] != '\0' )
			{
				sp<<m_szError;
			}
			else
			{
				sp<<"��¼ʧ��";
			}
			m_pSendData->data_len = static_cast<uint16>(sizeof(XT_HEAD) + sp.GetSize());
		}
		else
		{
			int len;
			XT_LOGIN_ERROR* ack = (XT_LOGIN_ERROR*)(m_pSendData->data + sizeof(XT_HEAD));
			*(uint8*)ack = 1;
			if( m_szError[0] != '\0' )
			{
				len = strlen(m_szError);
				strncpy((char*)ack + sizeof(uint8),m_szError,len);
				*((char*)ack + sizeof(uint8) + len) = 0;
			}
			else
			{
				len = strlen("��¼ʧ��");
				strncpy((char*)ack + sizeof(uint8),"��¼ʧ��",len);
				*((char*)ack + sizeof(uint8) + len) = 0;
			}
			m_pSendData->data_len = static_cast<uint16>(sizeof(XT_HEAD) + len);
		}
	}
	if(lpRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
	{
		if(lpRecvHead->flag == XT_HEAD::FLAG_XMPPREQ || lpRecvHead->flag == XT_HEAD::FLAG_XMPPSASL)
			InitHead(XT_HEAD::FLAG_XMPPACK,lpSendHead,m_nBusinessIDAck,lpRecvHead->seq,m_pSendData->data_len - sizeof(XT_HEAD),XT_HEAD::DEST_TYPE_USER,lpRecvHead->sid);
		else
			InitHead(XT_HEAD::FLAG_SRVACK,lpSendHead,m_nBusinessIDAck,lpRecvHead->seq,m_pSendData->data_len - sizeof(XT_HEAD),XT_HEAD::DEST_TYPE_USER,lpRecvHead->sid);
	}
	else
		InitHead(XT_HEAD::FLAG_MOBSRVACK,lpSendHead,m_nBusinessIDAck,lpRecvHead->seq,m_pSendData->data_len - sizeof(XT_HEAD),XT_HEAD::DEST_TYPE_USER,lpRecvHead->sid);

	if( m_IsOverLoad )
	{
	//��ʱ�����û�����(�������ͬʱ�����û�����)��zhangmin 20150425�����
	#ifdef USE_CONNECT_SERVER
		CSendPacket sp(m_pSendData->data + sizeof(XT_HEAD));
		SRVSTATUSINFO* pBestSvsInfo;
		pBestSvsInfo = CAppManager::GetInstance().GetConnBusiControl()->GetBestSrvInfo();
		uint32 serverip = pBestSvsInfo == NULL ? 0 : pBestSvsInfo->SrvStatus.srvBaseInfo.dnsIP;
		if(serverip == inet_addr(CAppManager::GetInstance().GetConfig().GetListenAddr()) )
		{
			serverip = 0;
		}
		sp << (uint8)2
		   << serverip;
		m_pSendData->data_len = (uint16)(sizeof(XT_HEAD) + sp.GetSize());
	#endif
	}

	if(lpRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
	{
		//���ܣ��������ݣ�����retѡ����ܷ�ʽ,ret == 0 ����md5(md5(password)) ���ܣ�ret == 1 ����randomkey ����
		uRet = (*(uint8 *)(m_pSendData->data + sizeof(XT_HEAD)));
		char szKey[16] = {0};
		if(uRet == 0)
		{
			memcpy(szKey,m_digest,sizeof(szKey));
		}
		else
		{
			memcpy(szKey,req.random_key,sizeof(szKey));
		}

		//�����û���¼��ʽ�ж��Ƿ񽫷���ֵ���͸�jabberdIM�߳�
		if(lpRecvHead->flag == XT_HEAD::FLAG_XMPPREQ || lpRecvHead->flag == XT_HEAD::FLAG_XMPPSASL)
		{
			CLog::Log("CBusiLogin::DoWork",CLog::TYPE_IMPORTANT,"send to xmpp, length:%d", m_pSendData->data_len);
			CAppManager::GetInstance().SendToJabberdIM((BYTE*)m_pSendData->data, m_pSendData->data_len, pRecvData->vlink);
		}

		int tlen = 0;
		if(!xTEAEncryptWithKey(m_pSendData->data + sizeof(XT_HEAD),m_pSendData->data_len - sizeof(XT_HEAD),szKey,m_pSendData->data + sizeof(XT_HEAD),&tlen))
		{
			CLog::Log("CBusiLogin::DoWork",CLog::TYPE_IMPORTANT,"%d xTEAEncryptWithKey failed", m_uid);
			return BUSIERR_INTERNAL;
		}
	 
		m_pSendData->data_len = (uint16)tlen + sizeof(XT_HEAD);
	}

	//�ظ���½����,���ô����ֱ�ӷ���
	m_pSendData->retry_times = 0;
	m_pSendData->dest_ip = pRecvData->src_ip;
	m_pSendData->port = pRecvData->port;

	
	if(lpRecvHead->flag != XT_HEAD::FLAG_XMPPREQ && lpRecvHead->flag != XT_HEAD::FLAG_XMPPSASL)
	{
		CLog::Log("CBusiLogin::DoWork",CLog::TYPE_IMPORTANT,"send to other");
		m_pAppNetwork->SendDataTo(*m_pSendData);
	}
	
	return RESULT_SUCCESS;
}
uint32 CBusiLogin::HandleReq(const LPXT_LOGIN_REQ lpReq, LPXT_HEAD lphead,uint32 fd)
{
	//�����
	uint32 uRet = 0;

#if 0

	bool isline = false;
	
	char cPass[32 + 1];
	if(lpReq->login_flag == XT_LOGIN_REQ::LOGINBYID)
	{
		m_uid = lpReq->uLoginKey.id;
		uRet = m_pDataBaseWeb->Get_PWD_ByID(lpReq->uLoginKey.id,cPass,lpReq->uLoginKey.username,lpReq->uLoginKey.mobileno);
	}
	else if(lpReq->login_flag == XT_LOGIN_REQ::LOGINBYNAME)
	{
		char *ret = strchr(lpReq->uLoginKey.username,'_');
		if ( ret != NULL)
		{
			char repname[MAX_USERNAME_LEN + 1];
			char *p = ret;
			while(*p)
			{
				if (*p == '_')
				{
					*p=0;
					++p;
					sprintf_s(repname, "%s--%s", lpReq->uLoginKey.username, p);
					strcpy_s(lpReq->uLoginKey.username, repname);
					isline = true;
				}
				else
					++p;
			}
		}
		
#ifndef _USE_MYSQL
		uRet = m_pDataBaseWeb->Get_UserID_PWD(lpReq->uLoginKey.username,(uint32 &)uid,cPass);
#else
		uRet = MYSQL_LIB->getPwdByName(lpReq->uLoginKey.username, uid, cPass);
#endif

		if(uRet == DB_SUCCESS)
		{
			m_uid = uid;
		}
		else
		{			
			if(uRet == DB_NOACTIVE)
				sprintf_s(m_szError, "%s", "�����˺���δ����");
			else
				CError::DB_Error(uRet,m_szError,true);

			
			CLog::Log("CBusiLogin",CLog::TYPE_IMPORTANT,"ȡ����ʧ�ܣ�%s",m_szError);
			return RESULT_FAILED;
		}
	}
	else if(lpReq->login_flag == XT_LOGIN_REQ::LOGINBYMOBILENO)
	{
		uRet = m_pDataBaseWeb->Get_UserID_PWD_ByMobileNo(lpReq->uLoginKey.mobileno,(uint32 &)uid,cPass,lpReq->uLoginKey.username);
		if(uRet == DB_SUCCESS)
		{
			m_uid = uid;
		}
		else
		{
			CError::DB_Error(uRet,m_szError,true);
			CLog::Log("CBusiLogin",CLog::TYPE_IMPORTANT,"ȡ����ʧ��2��%s",m_szError);
			return RESULT_FAILED;
		}
	}

	cPass[sizeof(cPass)-1]='\0';
	if (strcmp(cPass, "") == 0)
	{
		sprintf_s(m_szError, "%s", "�������");
		CLog::Log("CBusiLogin",CLog::TYPE_IMPORTANT,"����Ƚϴ���Ϊ�գ���%s",m_szError);
		return RESULT_FAILED;
	}
		
	unsigned char digest[16];

	HexStr2BYTEBCDArray(cPass,sizeof(digest)*2,digest);

	memcpy(m_digest,digest,sizeof(m_digest));
	
	unsigned char userdigest[16];
	m_md5.MD5Init();
	m_md5.MD5Update ((unsigned char*)lpReq->password, static_cast<uint32>(strlen(lpReq->password)));
	m_md5.MD5Final ((unsigned char*)userdigest);	

	if( memcmp(userdigest,digest,sizeof(digest)) != 0 )
	{
		sprintf_s(m_szError, "%s", "�������");
		CLog::Log("CBusiLogin",CLog::TYPE_IMPORTANT,"��������ƱȽϴ���%s",m_szError);
		return RESULT_FAILED;
	}
#endif
	char cPass[40 + 1];
	unsigned char digest[16];
	
	strcpy(cPass, lpReq->password);
	MD5Password(cPass);

	//�жϵ�½�������һ������
	char szGuid[MAX_GUID_LEN] = "";
	char szUserName[MAX_USERNAME_LEN + 1] = "";
    char szMobileNo[MAX_PHONE_LEN + 1]="";
    char szEmail[MAX_EMAIL_LEN+1]="";
	char szNickName[61]="";
	char szHeadImgUrl[MAX_HEADURL_LEN+1] ="";
	switch ((lpReq->login_flag & 0x4) >> 2)
	{
		case XT_LOGIN_REQ::BUYER:
		{
			//web�����
		  	if(lpReq->sessionID[0] != 0 )
			{
				memcpy(szGuid, lpReq->sessionID + 36, 36);
				strcpy(szUserName, lpReq->uLoginKey.username);
                strcpy(szMobileNo, lpReq->mobilephoneno);
                strcpy(szEmail,lpReq->email);
				//strcpy(szNickname,lpReq->uLoginKey.username);
				szGuid[36] = '\0';				
			}
			//�ͻ������
			else if(CheckBuyer(lpReq->uLoginKey.username, lpReq->password,szGuid, szUserName))
			{
				CLog::Log("CheckBuyer",CLog::TYPE_DEBUG,"username:%s",szUserName);
			}	
			else
			{
				sprintf_s(m_szError, "%s", "�û������������");
				CLog::Log("CBusiLogin SaveAccountBuyer", CLog::TYPE_ERROR, "Buyer:%s pwsError", szUserName);
				return RESULT_FAILED;
			}
			//��ȡ�����Ϣ
			#if 0
			if(GetBuyerInfo(lpReq->uLoginKey.username, szMobileNo, szEmail, szNickName, szHeadImgUrl))
			{
				CLog::Log("GetBuyerInfo",CLog::TYPE_IMPORTANT,
						"username:%s, email:%s, mobileno:%s, szNickname:%s, szHeadImgFile:%s",
						szUserName, szEmail, szMobileNo, szNickName, szHeadImgUrl);
			}
			else
			#endif
			{
				strncpy(szNickName,szUserName,MAX_USERNAME_LEN);
				CLog::Log("GetBuyerInfo",CLog::TYPE_IMPORTANT,"Buyer:%s, getInfoSelf",szUserName);
			}


			//�ڲ��������Ա��ϵͳ¼��
#ifdef PRE_INTERNAL			
			uint32 uid = 0;
			char cPass2[60];
			uRet = m_pDataBaseWeb->Get_UserID_PWD(lpReq->uLoginKey.username,(uint32 &)uid,cPass2);			
			if(uRet != DB_SUCCESS)
			{
				sprintf_s(m_szError, "%s", "�������ʺ���δ�����輤��");
				return RESULT_FAILED;
			}			
#endif		
			
			
			//��֤ͨ��,�洢�ʺ�
			if(strlen(szGuid)== 36 )
			{
				if(strlen(szUserName) == 0)
				{
					sprintf_s(m_szError, "%s", "�û�������");
					CLog::Log("CBusiLogin SaveAccountBuyer",CLog::TYPE_ERROR,"Buyer: %s GetUserName Failed!",lpReq->uLoginKey.username);
					return RESULT_FAILED;
				}
				else if(m_pDataBaseWeb->SaveAccountBuyer(szUserName, cPass, szGuid, m_uid, szMobileNo, szEmail, szNickName, szHeadImgUrl) != DB_SUCCESS)
				{
					sprintf_s(m_szError, "%s", "�����û������������");
					CLog::Log("CBusiLogin SaveAccountBuyer",CLog::TYPE_ERROR,"Buyer:%s Save Account Failed!",lpReq->uLoginKey.username);
					return RESULT_FAILED;
				}
			}
			else
			{
				sprintf_s(m_szError, "%s", "���û������������");
				CLog::Log("CBusiLogin SaveAccountBuyer",CLog::TYPE_ERROR,"Buyer:%s, GUID:%s error!",lpReq->uLoginKey.username,szGuid);
				return RESULT_FAILED;
			}

			break;
		}
		case XT_LOGIN_REQ::SELLER:
		{
			uint32_t state = 0;
			//if( CheckSellerMysql(lpReq->uLoginKey.username, lpReq->password, m_ack.user_id, m_ack.merchant_id, state) )
			if( CheckSeller(lpReq->uLoginKey.username, lpReq->password, m_ack.user_id, m_ack.merchant_id, state) )
			{
				CLog::Log("CheckSeller",CLog::TYPE_IMPORTANT,"CheckSeller ok! username:%s, id:%d, state:%d",
					lpReq->uLoginKey.username, m_ack.user_id, state);
				if(state == 2)
				{
					sprintf_s(m_szError, "%s", "�����ʺ���ͣ��");
					return RESULT_FAILED;
				}
				if(m_pDataBaseWeb->SaveAccount(lpReq->uLoginKey.username,cPass,m_ack.user_id,lpReq->uLoginKey.username) == DB_SUCCESS)
				{
					m_uid = m_ack.user_id;
					if(m_ack.merchant_id == 0)
					{
						sprintf_s(m_szError, "%s", "����δ��פ��ţ������פ���ٵ�¼����");
						return RESULT_FAILED;
					}
					/*
					else if(m_ack.merchant_id != m_ack.user_id)
					{	
						CLog::Log("CBusiLogin SaveSubAccount",CLog::TYPE_DEBUG,"Save Account %d %d",m_ack.merchant_id,m_ack.user_id);
						//������Ϊ���˺Ŵ洢���ҵ����˺�
						if(m_pDataBaseWeb->SaveSubAccount(m_ack.merchant_id,m_ack.user_id,MYGROUP_ID,
						cPass,lpReq->uLoginKey.username,lpReq->uLoginKey.username) != DB_SUCCESS)
						{
							CLog::Log("CBusiLogin SaveSubAccount",CLog::TYPE_ERROR,"Save Account Failed");
						}
					}
					*/
				}
				else
				{
					sprintf_s(m_szError, "%s", "�����û������������");
					CLog::Log("CBusiLogin SaveAccount",CLog::TYPE_ERROR,"Seller SaveAccount Failed! username:%s, user_id:%d, merchant_id:%d",
						lpReq->uLoginKey.username, m_ack.user_id, m_ack.merchant_id);
					return RESULT_FAILED;
				}

			}
			else
			{
				sprintf_s(m_szError, "%s", "�û������������");
				CLog::Log("CBusiLogin SaveAccount",CLog::TYPE_ERROR,"Seller PSW error! username:%s, user_id:%d, merchant_id:%d", 
					lpReq->uLoginKey.username,  m_ack.user_id, m_ack.merchant_id);
				return RESULT_FAILED;
			}
			break;
		}
		default:
			return RESULT_FAILED;
	}

	HexStr2BYTEBCDArray(cPass,sizeof(digest)*2,digest);
	memcpy(m_digest,digest,sizeof(m_digest));
	
	CAppManager& app = CAppManager::GetInstance();
	CUserManager& UserManager = app.GetUserManager();
	if( UserManager.GetElemCount() >= app.GetConfig().GetMaxUserCount() )
	{
		//������
		m_IsOverLoad = true;
		//��ʱ������������û����ƣ�zhangmin 20140425
		//return RESULT_SUCCESS;
	}
	
	RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
	LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;
	//������Ч��У��...
	//�����û�session key
	unsigned char bRandomStr[4];
	srand( (unsigned)time( NULL ) ); 
	*(unsigned char*)bRandomStr = rand();
	int dwSessionLen;
	if(!xTEAEncryptWithKey((const char*)bRandomStr, sizeof(bRandomStr), (char  *)digest,(char *)m_ack.session_key,&dwSessionLen))
	{
		CLog::Log("CBusiLogin::HandleReq",CLog::TYPE_IMPORTANT,"%u xTEAEncryptWithKey failed", lpRecvHead->sid);
		return BUSIERR_INTERNAL;
	}

	m_ack.ret = 0;
	m_ack.user_id = m_uid;
	//���汾
	bool bLogin = true;
	int canUseVersion = CAppManager::GetInstance().GetCanUserver();
	int agreeVersion = CAppManager::GetInstance().GetAgreever();
	//int updateOption = CAppManager::GetInstance().GetUpdateOption();
	char szUpdateinfo[256] = {0}, szUpdateConfigBuyer[256] = {0}, szUpdateConfigSeller[256] = {0};
	strcpy(szUpdateConfigBuyer, CAppManager::GetInstance().GetUpdateConfigBuyer());
	strcpy(szUpdateConfigSeller, CAppManager::GetInstance().GetUpdateConfigSeller());

	strcpy(szUpdateinfo, CAppManager::GetInstance().GetUpdateInfo());
//	strcpy_s(m_ack.version_url,szUpdateUrl);
	if(m_uid > 5000000)
	{
		strcpy(m_ack.update_config,szUpdateConfigBuyer);
	}
	else
	{
		strcpy(m_ack.update_config,szUpdateConfigSeller);
	}
	if(lpRecvHead->from >= agreeVersion)
	{
		m_ack.version_flag = (uint8)IMVERSION_AGREE;
	}
	else if(lpRecvHead->from >= canUseVersion)
	{
		m_ack.version_flag = (uint8)CAppManager::GetInstance().GetUpdateOption(lpRecvHead->from);
		strcpy(m_ack.update_info,szUpdateinfo);		
	}
	else
	{
		bLogin = false;
		m_ack.version_flag = (uint8)IMVERSION_LOW;
		strcpy(m_ack.update_info,szUpdateinfo);
	}
	
	if(bLogin)	
	{
		uRet = LoginSucess(lpReq,lphead,fd);
		if(uRet != RESULT_SUCCESS)
		{
			CLog::Log("CBusiLogin::HandleReq",CLog::TYPE_IMPORTANT,"LoginSucess init fail !");
			return uRet;
		}
	}

	m_ack.publicip = pRecvData->src_ip;

	if(lphead->flag != XT_HEAD::FLAG_MOBREQ)
	{
		
		time_t tm;
		time(&tm);
		//��װ��ȷ�Ļ�Ӧ��
		CSendPacket sp(m_pSendData->data + sizeof(XT_HEAD));
		sp<<m_ack.ret;
		sp.WritePacket((const char *)m_ack.session_key,sizeof(m_ack.session_key));
		sp	<<m_ack.user_id
			<<m_ack.merchant_id
			<<m_ack.publicip
			<<m_ack.version_flag
			<<""
			<<m_ack.update_config
			<<m_ack.update_info
			<<uint8(0)
			<<szGuid;
		
		if(lpRecvHead->flag == XT_HEAD::FLAG_XMPPREQ || lpRecvHead->flag == XT_HEAD::FLAG_XMPPSASL)
		{
			sp << lpReq->uLoginKey.username;
		}
		else
		{
			sp<<szUserName;
		}
		
			sp<<(uint32)tm;
		CLog::Log("CBusiLogin",CLog::TYPE_DEBUG,"id:%d, LOGIN TIME %u", m_ack.user_id, tm);

		m_pSendData->data_len = static_cast<uint16>(sizeof(XT_HEAD) + sp.GetSize());
	}
	else
	{
		//strcpy(m_ack.sessionID,lpReq->sessionID);
		memcpy(m_ack.sessionID,lpReq->sessionID,36);
		m_ack.sessionID[36] = '\0'; 		
		CLog::Log("CBusiLogin",CLog::TYPE_DEBUG,"ack.sessionID:%s", m_ack.sessionID);
		memcpy(m_pSendData->data + sizeof(XT_HEAD), (char*)&m_ack, sizeof(XT_LOGIN_ACK));
		m_pSendData->data_len = static_cast<uint16>(sizeof(XT_HEAD) + sizeof(XT_LOGIN_ACK));
	}


	CLog::Log("CBusiLogin::HandleReq", CLog::TYPE_IMPORTANT, "sid:%u, ret:%u, user_id:%u, merchant_id:%u, publicip:%u, version_flag:%u, guid:%s, username:%s, sessionid:%s", 
		lpRecvHead->sid, m_ack.ret, m_ack.user_id, m_ack.merchant_id, m_ack.publicip, m_ack.version_flag, szGuid, lpReq->uLoginKey.username, m_ack.sessionID);
	return RESULT_SUCCESS;
}

void CBusiLogin::MD5Password(char* pPassword)
{
	unsigned char digest[16];

	char password[255] = {0};
	sprintf_s(password,"%s",pPassword);

	MD5_CTX_IM md5T;
	md5T.MD5Init();
	md5T.MD5Update ((unsigned char*)password,(uint32)strlen(password) );
	md5T.MD5Final (digest);

	string sHex=ToHexString(digest,16);

	strcpy(pPassword,sHex.c_str());

}

int CBusiLogin::LoginSucess(const LPXT_LOGIN_REQ lpReq, LPXT_HEAD lphead,uint32 fd)
{
	//��½�ɹ�����
	CUserManager & userManager = CAppManager::GetInstance().GetUserManager();
	CUser * pUser = userManager.Find(m_uid);

	//�ж��Ƿ��ص�½
	RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
	LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;
	LPONLINE_USER_INFO lpOnlineUser = m_pAppNetwork->FindOnlineUser(m_uid);
	if(pUser && lpOnlineUser && lpOnlineUser->isDirect)
	{
		if(pRecvData->src_ip == lpOnlineUser->uip && pRecvData->port == lpOnlineUser->uport)
		{//�����ص�¼
			CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"Id:%u relogin !",m_uid);
			memcpy(m_ack.session_key,lpOnlineUser->sessionKey,sizeof(m_ack.session_key));
			return RESULT_SUCCESS;
		}
		else
		{//����ص�¼
			//�ǳ���ǰ�Ŀͻ���

			TransitReLoginMsg(lpOnlineUser);

			CBusiLogout busiLogout(m_pRouter);
			busiLogout.Logout(pUser);
			pUser = NULL;

		}
	}
	else if (pUser && pUser->m_bIsFullInfoValid)
	{
		CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"Id:%u server all ready has info !",m_uid);
		pUser->Reset();
	}


	// �������û���P2P SESSIONKEY
	char p2pSessionKey[16] = {0};
	unsigned char digest1[16];m_md5.MD5Init();
	m_md5.MD5Update ((unsigned char*)m_ack.session_key,sizeof(m_ack.session_key));
	m_md5.MD5Final ((unsigned char*)digest1);
	unsigned char digest2[16];m_md5.MD5Init();
	m_md5.MD5Update ((unsigned char*)&m_uid,sizeof(m_uid));
	m_md5.MD5Final ((unsigned char*)digest2);
	for(int i=0;i<16;i++) p2pSessionKey[i]=digest1[i]&digest2[i];


	//�����û���Ϣ
	if (!pUser)
	{
		pUser = userManager.AddHash(m_uid);
		pUser->Reset();
		pUser->m_nID = m_uid;

		//CLog::Log("CBusiLogin",CLog::TYPE_IMPORTANT,"begin AddToOnlineList ");
		pUser->AddToOnlineList();
		//CLog::Log("CBusiLogin",CLog::TYPE_IMPORTANT,"finish AddToOnlineList ");

		SysStat::GetInstance().addOnlineNum();
	}

	m_pRouter->GetDataBaseMini()->GetWokuMsgID(m_uid,pUser->m_nOperationID,pUser->m_nUserMsgID,pUser->m_nSysMsgID,pUser->m_nMobileMsgID,pUser->m_nUsercommentID,pUser->m_nLogcommentID);

	pUser->m_nID = m_uid;
	strcpy_s(pUser->m_szUserName,lpReq->uLoginKey.username);


	//�����û�״̬
	if (lpReq->status!= XTREAM_ONLINE && lpReq->status != XTREAM_HIDE && XTREAM_LEAVE != XTREAM_LEAVE)
		lpReq->status = XTREAM_ONLINE;

	pUser->m_nStatus = lpReq->status;
	pUser->m_nIP	 = pRecvData->src_ip;
	pUser->m_nPort	 = pRecvData->port ;
	pUser->m_nLanIP	 = lpReq->lanip;
	pUser->m_nLanPort= lpReq->lanport;
	pUser->m_nNewlyLiveTime = timeGetTime();
	pUser->m_nXmppID = fd;
	time_t tm;
	time(&tm);
	pUser->m_nLoginTime = (uint32)tm;
	memcpy(pUser->m_SessionKey,m_ack.session_key,MAX_SESSIONKEY_LEN);
	memcpy(pUser->m_P2PSessionKey,p2pSessionKey,MAX_SESSIONKEY_LEN);
	pUser->m_ClientVersion = lpRecvHead->from;
	pUser->m_nLoginType = lphead->flag;
	CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT,"id:%d, Login Type %x, m_nXmppID %u, ip %u, port %d",
		pUser->m_nID, pUser->m_nLoginType,pUser->m_nXmppID,
		pUser->m_nIP,pUser->m_nPort);

	//���õ�Ӧ�������ӿ�
	m_pAppNetwork->UserLogin(m_uid,pRecvData->src_ip,
		pRecvData->port,(const char *)m_ack.session_key,
		p2pSessionKey,lpReq->lanip,lpReq->lanport,lpRecvHead->flag);
	m_pRouter->GetDataBaseMini()->Save_User_Login(m_uid,pRecvData->src_ip,pRecvData->port,
			CAppManager::GetInstance().GetConfig().GetServerNo());

	if(pUser->m_nID < Id_Boundary)
	{
		m_pRouter->GetDataBaseMini()->Save_Merchant_Login(m_uid,(int)tm);
		//�ϴ���¼����
		using namespace LogInterface;
		using namespace std;
		hessianLogi hess(m_ack.merchant_id,m_uid);
		pUser->m_merchantid = 0;
		if(pUser->m_nID < Id_Boundary)
		{
			try 
			{
				SubMerchantAction info;
				info.merchantID = m_ack.merchant_id;
				info.subMerchantID = m_uid;
				info.actionTime = (int)tm;
				info.actionType = ACTION_LOGIN;
				pUser->m_merchantid = m_ack.merchant_id;
		
				ResultInfo res = hess.GetHessiani()->uploadAction(info);
		
		
				CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
					info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
			}
			catch (std::exception& e) {
				
				CLog::Log("hessian",CLog::TYPE_ERROR,"merchant_id:%d, subMerchantID:%d, other exception occured: %s",
					m_ack.merchant_id, m_uid, e.what());
			}
		}
	}
		
	return RESULT_SUCCESS;
}

//�ص�¼ʱ������ǰ��¼�ķ��ص�½�˳���ʾ
//��Ϊ�ѵ�½�û���һ��ҵ����
int CBusiLogin::TransitReLoginMsg(LPONLINE_USER_INFO lpPreOnlineInfo)
{
	CNotifyProtocol::Relogin(lpPreOnlineInfo->uid);
	return 0;
}

int CBusiLogin::SendReply()
{
	return 0;
}





bool CBusiLogin::CheckBuyer(const char* user, const char* password, char* guid,  char * szUserName)
{
		//�ж��Ƿ��ص�½
	RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
#ifdef PRE_INTERNAL
	string err_msg("");
	strcpy(guid,"123456789012345678901234567890123456");
	strcpy(szUserName,user);
	if(!AD_Auth(user,password,err_msg))
	{		
		CLog::Log("CheckBuyer-AD_Auth",CLog::TYPE_DEBUG,"errormsg: %s",err_msg.c_str());
		return false;
	}
	else
	{		
		CLog::Log("CheckBuyer-AD_Auth",CLog::TYPE_DEBUG,"AD_Auth success");
		return true;
	}

#endif
	//Ĭ��ʹ���û�����½
	std::string sPostParam = "type=3";
	if (strlen(user) == 11 && user[0] == '1' && atoi(user) != 0) {	//ʹ���ֻ��ŵ�½
		sPostParam = "type=2";
	} else if (CheckEmail(user)) {	//ʹ��Email��½
		sPostParam = "type=1";
	}
	/*
	
	username	string	Y	�û���
	password	string	Y	����
	token		string	Y	token
	ip			string	Y	��¼��ip
	referer 	string	Y	��¼��Դ: 
							pc�ǵ�¼��Դ����;
							PC IM��"moumou" ;������"app"
	channel 	int 	Y	��¼ƽ̨  
							1:	pc	
							2������ 	  
							3��android 
							4:	ios
	channelFrom int 	Y	ƽ̨��¼��Դ 
							0�� ��ţ���û� 
	isFromReg	Int 	Y	�Ƿ���ע����¼
							0�����Ǵ�ע����¼��1��ע����¼
	
	*/

	//�����û���
	sPostParam += "&username=";
	sPostParam += user;
	//��������
	sPostParam += "&password=";
	sPostParam += Base64Encode(password);
	//����token
	sPostParam += "&token=1";
	//�����½��IP
	sPostParam += "&ip=";
	char ip[20];
	moumou_ntoa(pRecvData->src_ip,ip);
	sPostParam += ip;
	//����referer
	sPostParam += "&referer=moumou";	
	//����channel
	sPostParam += "&channel=3";
	//����channelFrom
	sPostParam += "&channelFrom=0";
	//����isFromReg
	sPostParam += "&isFromReg=0";
		
	std::string data;

	//ͨ��http�ӿڻ�ȡ�����Ϣ
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, CAppManager::GetInstance().GetConfig().GetBuyerURL());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sPostParam.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	CURLcode res = curl_easy_perform(curl);

	//�����ӿ�
	curl_easy_cleanup(curl);

	//�жϻ�ȡ�Ƿ�ɹ�
	if (res != CURLE_OK) 
	{
		CLog::Log("CheckBuyer",CLog::TYPE_ERROR,"Get buyer infomation error!");
		return false;
	}

	//����json
	Json::Reader reader;
	Json::Value recv;
	if (reader.parse(data, recv)) {
		//����Header�е�Status
		uint32 code = -1;
		string status;
		if(recv["code"].isUInt())
		{
			code = recv["code"].asUInt();
			if(code != 200)
			{
				//û�ҵ��Ӵ�
				CLog::Log("CheckBuyer",CLog::TYPE_ERROR," code is not right %d!",code);
				return false;			
			}
		}
		
		if(recv["msg"].isString())
		{
			status = recv["msg"].asString();
		}
		//�ж��Ƿ����Ӵ�012002100
		if(status.find("ok") != std::string::npos)
		{
			string strGuid = recv["data"]["user"]["MEM_GUID"].asString();
			string strUserName = recv["data"]["user"]["MEM_USERNAME"].asString();
	
			strcpy(guid,strGuid.c_str());
			strcpy(szUserName, strUserName.c_str());
			//�ҵ����Ӵ�
			return true;
		}
	}

	//û�ҵ��Ӵ�
	CLog::Log("CheckBuyer",CLog::TYPE_ERROR," Not found userinfo!");
	return false;
}
bool CBusiLogin::GetBuyerInfo(const char* user, char* szMobileNo, char* szEmail, char* szNickName,char* szHeadImgUrl)
{	
	std::string sPostParam = "type=3";
	if (strlen(user) == 11 && user[0] == '1' && atoi(user) != 0) 
	{	//ʹ���ֻ��ŵ�½
		sPostParam = "type=2";
	}
	else if (CheckEmail(user))
	{	//ʹ��Email��½
		sPostParam = "type=1";
	}
	//�����û���
	sPostParam += "&name=";
	sPostParam += user;
	sPostParam += "&returnType=1";
	std::string data;

	//ͨ��http�ӿڻ�ȡ�����Ϣ
	CURL *curl = curl_easy_init();
	curl_easy_setopt(curl, CURLOPT_URL, CAppManager::GetInstance().GetConfig().GetBuyerInfoURL());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sPostParam.c_str());
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	CURLcode res = curl_easy_perform(curl);

	//�����ӿ�
	curl_easy_cleanup(curl);

	//�жϻ�ȡ�Ƿ�ɹ�
	if (res != CURLE_OK)
	{	
		CLog::Log("GetBuyerInfo",CLog::TYPE_ERROR,"user:%s curlFaild",user);
		return false;
	}

	//����json
	Json::Reader reader;
	Json::Value recv;
	if (reader.parse(data, recv)) 
	{
		//����Header�е�Status
		string status = recv["Header"]["Status"].asString();
		//�ж��Ƿ����Ӵ�012001100
		if(status.find("012001100") != std::string::npos)
		{
			string strMobileNo= recv["Body"]["Data"]["MEM_CELL_PHONE"].asString();
			string strEmail = recv["Body"]["Data"]["MEM_EMAIL"].asString();
			string strNickName =recv["Body"]["Data"]["NICKNAME"].asString();
			string strHeadImgFile =recv["Body"]["Data"]["HEAD_PORTRAIT"].asString();
 
			char szNickNameUtf8[61]= "";
			strncpy(szMobileNo, strMobileNo.c_str(), MAX_PHONE_LEN);
			strncpy(szEmail, strEmail.c_str(), MAX_EMAIL_LEN);
			strncpy(szNickNameUtf8, strNickName.c_str(), strNickName.size());
			utf2gbk(szNickNameUtf8,strlen(szNickNameUtf8), szNickName, 61);

			if (strlen(szNickName) == 0)
			{
				strncpy(szNickName,user,MAX_USERNAME_LEN);
			}
			if(strHeadImgFile.length()!=0)
			{
				char szHeadImgFile[MAX_HEADURL_LEN-30]="";
				char preHeadImgURL[MAX_HEADURL_LEN-70] = {0};
				strcpy(preHeadImgURL,CAppManager::GetInstance().GetConfig().GetHeadImgURL());
				
				strncpy(szHeadImgFile,strHeadImgFile.c_str(),MAX_HEADURL_LEN-30 );
				CLog::Log("GetBuyerInfo",CLog::TYPE_DEBUG,"preHeadImgURL:%s",preHeadImgURL);
				
				sprintf(szHeadImgUrl,"%s%s",preHeadImgURL,szHeadImgFile);
			}
			else
			{
				strncpy(szHeadImgUrl,strHeadImgFile.c_str(),MAX_HEADURL_LEN );
			}
			CLog::Log("GetBuyerInfo",CLog::TYPE_DEBUG,"user:%s, mobileNo:%s, email:%s, nickName:%s, szHeadImgFile:%s",
				user, szMobileNo, szEmail, szNickName, szHeadImgUrl);
			//�ҵ����Ӵ�
			return true;
		}
		else
		{
			 //�û�������
			CLog::Log("GetBuyerInfo",CLog::TYPE_ERROR,"user:%s, notExist", user);
			return false;
		
 		}
	}
	else
	{	
		//json������ʧ��
		CLog::Log(__FUNCTION__, CLog::TYPE_ERROR, "user:%s, data:%s ,jsonPraseFail", user, data.c_str());
		return false;
		
	}

}

const char* CBusiLogin::TransPwd(const char* pwd)
{
//	CLog::Log("CBusiLogin::TranPwd", CLog::TYPE_DEBUG, "PWD:%s",pwd);
	
	string symbol_space = "+";
	string symbol_split = "&";
	string symbol_special = "%";

	string url_space = "%2B";
	string url_split = "%26";
	string url_special = "%25";

	string UrlPwd(pwd);

	string::size_type nPos = 0;

	nPos = UrlPwd.find(symbol_special,0);
	while(nPos != string::npos)
	{
		UrlPwd.replace(nPos,symbol_special.length(),url_special);

		nPos = UrlPwd.find(symbol_special,nPos + 1);
	}

	nPos = UrlPwd.find(symbol_split,0);
	while(nPos != string::npos)
	{
		UrlPwd.replace(nPos,symbol_split.length(),url_split);

		nPos = UrlPwd.find(symbol_split,nPos + 1);
	}
	
	nPos = UrlPwd.find(symbol_space,0);
	while(nPos != string::npos)
	{
		UrlPwd.replace(nPos,symbol_space.length(),url_space);

		nPos = UrlPwd.find(symbol_space,nPos + 1);
	}

//	CLog::Log("CBusiLogin::TranPwd", CLog::TYPE_DEBUG, "Tran PWD:%s",UrlPwd.c_str());

	return UrlPwd.c_str();
}


bool CBusiLogin::CheckSellerMysql(const char* user, const char* pwd, uint32_t& uid, uint32_t& merchant_id, uint32_t& state)
{
	char cPassMysql[MAX_PASSWORD_LEN];
	char cUserUtf8[MAX_USERNAME_LEN];
	gbk2utf8(cUserUtf8, user);
	if(MYSQL_LIB->getPwdByName(cUserUtf8, uid, cPassMysql, merchant_id, state) == DB_SUCCESS)
	{
		char cPass[MAX_PASSWORD_LEN];
		strncpy(cPass, pwd, MAX_PASSWORD_LEN);
		MD5Password(cPass);
		if(strcmp(cPassMysql,cPass) == 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}

}





bool CBusiLogin::CheckSeller(char* user, const char* pwd, uint32_t& uid, uint32_t& merchant_id, uint32_t& state)
{
	//�������ת��
	merchant_id = 0;
	uid = 0;
	state = 0;
	const char* password = TransPwd(pwd);
	if(password == NULL)
	{
		CLog::Log("CBusiLogin::CheckSeller", CLog::TYPE_ERROR, "password:%s trans is NULL",pwd);
		return false;
	}	

	//��ʼ��http��get����
	std::string sPostParam = R"(version=1.0&method=feiniu.member.loginForMoumou&params={"loginName": ")";
	//�����û���
	sPostParam += user;
	sPostParam += R"(", "password": ")";
	//��������
	sPostParam += password;
	sPostParam += R"("})";

	char utfStr[512];
	//�����Ƚ�gbk����ת��Ϊunicode����
	if(NULL==setlocale(LC_ALL,"zh_CN.gbk"))//����ת��Ϊunicodeǰ����,��ǰΪgbk����
	{
		CLog::Log("CBusiLogin",CLog::TYPE_ERROR,"setlocale Bad Parameter");
		return false;
	}

	int unicodeLen=mbstowcs(NULL,sPostParam.c_str(),0);//����ת����ĳ���
	if(unicodeLen<=0)
	{
		CLog::Log("CBusiLogin",CLog::TYPE_ERROR,"mbstowcs Can not Transfer!!!");
		return false;
	}
	wchar_t *unicodeStr=(wchar_t *)calloc(sizeof(wchar_t),unicodeLen+1);
	mbstowcs(unicodeStr,sPostParam.c_str(),sPostParam.size());//��gbkת��Ϊunicode

	//��unicode����ת��Ϊutf8����
	if(NULL==setlocale(LC_ALL,"zh_CN.utf8"))//����unicodeת�������,��ǰΪutf8
	{
		CLog::Log("CBusiLogin", CLog::TYPE_ERROR, "setlocale Bad Parameter");
		return false;
	}
	int utfLen=wcstombs(NULL,unicodeStr,0);//����ת����ĳ���
	if(utfLen<=0)
	{
		CLog::Log("CBusiLogin", CLog::TYPE_ERROR, "wcstombs Can not Transfer!!!");
		return false;
	}
	else if(utfLen>=512)//�жϿռ��Ƿ��㹻
	{
		CLog::Log("CBusiLogin", CLog::TYPE_ERROR, "Dst Str memory not enough: %d", utfLen);
		return false;
	}
	wcstombs(utfStr,unicodeStr,utfLen);
	utfStr[utfLen]=0;//��ӽ�����
	free(unicodeStr);


	std::string data;

	//ͨ��HTTP�ӿڻ�ȡ������Ϣ
	CURL *curl = curl_easy_init();
	if(curl == NULL)
	{		
		CLog::Log("CBusiLogin", CLog::TYPE_ERROR, "curl_easy_init error");
	}
	curl_easy_setopt(curl, CURLOPT_URL, CAppManager::GetInstance().GetConfig().GetSellerURL());
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, utfStr);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
	curl_easy_setopt(curl, CURLOPT_POST, 1);
	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);  
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
	CURLcode res = curl_easy_perform(curl);

	//������ڴ�
	curl_easy_cleanup(curl);

	//�ж��Ƿ��ȡ��
	if (res != CURLE_OK) {		
		CLog::Log("CheckSeller", CLog::TYPE_ERROR, "Get seller infomation error!");
		return false;
	}

	//����json�ַ���
	Json::Reader reader;
	Json::Value recv;
	if (reader.parse(data, recv)) {
		//��ȡresult�ֶ�
		std::string result = recv["result"].asString();
		printf("%s\n", result.c_str());
		if(result != "null")
		{
			//����result�ֶ�
			Json::Value jvResult;
			if(reader.parse(result.c_str(),jvResult))
			{
				//��ȡauth�е�memberId�ֶ�
				uid = jvResult["auth"]["memberId"].asInt();
				
				//��ȡauth�е�merchantId�ֶ�
				merchant_id = jvResult["auth"]["merchantId"].asInt();
				state = jvResult["auth"]["state"].asInt();
				string username = jvResult["auth"]["username"].asString();
				//strcpy(user, username.c_str());
				int ret = utf2gbk((char *)username.c_str(),username.length(),user,MAX_USERNAME_LEN);		 	
				if(ret == -1)
				{
					CLog::Log("CheckSeller",CLog::TYPE_ERROR,"user:%s, data:%s, utf8 to gbk changed failed!",user, data.c_str());							
					return false;
				}
				return true;
			}
		}
	}
	CLog::Log("CheckSeller",CLog::TYPE_ERROR,"user:%s, data:%s, parse failed!",user, data.c_str());
	return false;
}


// AD��֤�ӿ�
bool CBusiLogin::AD_Auth(LPCTSTR szUserName, LPCTSTR szPwd, string &strErrorInfo)
{
	// ��ʼ��
	LDAP *pLdap = ldap_init((char *)AD_HOSTNAME, (int)AD_PORT);
	if(pLdap == NULL)
	{
		strErrorInfo = "����AD��֤������ʧ�ܣ�";
		return FALSE;
	}

	// ����AD��֤������
/*
	unsigned long  rlt = ldap_connect(pLdap, NULL);
	if(LDAP_SUCCESS != rlt)
	{
		ldap_unbind_s(pLdap);
		strErrorInfo = "����AD��֤������ʧ�ܣ�";
		return FALSE;
	}
*/
	// ��֤
	string strUser("FN\\");	
	//strUser.Format("FN\\%s", szUserName);
	strUser += string(szUserName);
	int rlt = ldap_simple_bind_s(pLdap, (char *)strUser.c_str(), (char *)szPwd);
	if(LDAP_SUCCESS != rlt)
	{
		ldap_unbind_s(pLdap);
		strErrorInfo = "��֤ʧ�ܣ��û������������";
		return FALSE;
	}

	ldap_unbind_s(pLdap);
	strErrorInfo = "��֤�ɹ���";
	return TRUE;	
}




/*///////////////////////////////
 BusiLogout.cpp
////////////////////////////////*/
CBusiLogout::CBusiLogout(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_LOGOUT,0)
{
	m_pAppNetwork = &(CAppManager::GetInstance().GetAppNetWork());
}


CBusiLogout::~CBusiLogout(void)
{
}

int CBusiLogout::DoWork()
{
	RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
	LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;
	XT_LOGOUT * lpReq = (XT_LOGOUT *)m_RecvPacket.GetData();	
	LPXT_HEAD lpSendHead = (LPXT_HEAD)m_pSendData->data;

	if(lpReq->authData != 0x00)
	{
		CLog::Log("CBusiLogout",CLog::TYPE_ERROR,"Id:%d,���յ�һ������ǳ���:d%",lpReq->id,lpReq->authData);
		return 1;
	}
	
	CUserManager & userManager = CAppManager::GetInstance().GetUserManager();
	CUser * pUser = userManager.Find(lpReq->id);
	if(pUser)
	{
		if(m_pRecvHead->flag == XT_HEAD::FLAG_MOBREQ)
			WebLogout(pUser);
		else
			Logout(pUser);
		
		//�����û���¼��ʽ�ж��Ƿ񽫷���ֵ���͸�jabberdIM�߳�

		if(pUser->m_nXmppID > 0)
		{
			m_pSendData->data_len =  sizeof(XT_HEAD);
			CLog::Log("CBusiLogin::DoWork",CLog::TYPE_IMPORTANT,"send to xmpp %u, length:%d", pUser->m_nXmppID,m_pSendData->data_len);		
			InitHead(XT_HEAD::FLAG_XMPPACK,lpSendHead,CMD_LOGOUT,lpRecvHead->seq,m_pSendData->data_len - sizeof(XT_HEAD),XT_HEAD::DEST_TYPE_USER,pUser->m_nID);
			CAppManager::GetInstance().SendToJabberdIM((BYTE*)m_pSendData->data, m_pSendData->data_len, pRecvData->vlink);
		}
		
		CLog::Log("CBusiLogout", CLog::TYPE_IMPORTANT, "sid:%u, did:%x, flag:%u, authData:%u" , m_pRecvHead->sid, m_pRecvHead->did,m_pRecvHead->flag, lpReq->authData); 
	}
	else
	{		
		CLog::Log("CBusiLogout", CLog::TYPE_IMPORTANT, "NO user context sid:%u, did:%x, flag:%u, authData:%u" , m_pRecvHead->sid, m_pRecvHead->did,m_pRecvHead->flag, lpReq->authData); 
	}

	return 0;
}

int CBusiLogout::SendReply()
{
	return 0;
}

void CBusiLogout::Logout(CUser * pUser)
{
	if(!pUser)
		return ;

	pUser->m_nStatus = XTREAM_EXIT;

	//�������ݿ����߱�״̬����ΪҪ����֪ͨ��������������ǰ����
	m_pRouter->GetDataBaseMini()->Save_User_Logout(pUser->m_nID);

	//�û�״̬�ı�֪ͨ��������
	pUser->NotifyFriendStatus();
	pUser->NotifyFamilyStatus(m_pAppNetwork,m_pRouter->GetGlobalFamilyBusi(),m_pRouter->GetDataBaseMini());//modify by lwl ver:0105
	
	//��ȫ��ע�����û���Ϣ
	uint32	uid = pUser->m_nID;
	uint8	count = 1;

	pUser->LogoutTempFamily(m_pAppNetwork, m_pRouter->GetGlobalFamilyBusi());

	pUser->m_PermFamilyMap.Lock();
	FamilyList_Type::iterator itf = pUser->m_PermFamilyMap.begin();
	for(;itf != pUser->m_PermFamilyMap.end(); ++itf)
	{
		//����̶�Ⱥ��Ϣid
		m_pRouter->GetDataBaseMini()->Save_User_RecvFamilyMsgID(pUser->m_nID, (*itf).first, (*itf).second);
	}
	pUser->m_PermFamilyMap.UnLock();

	m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USEROPERATION,pUser->m_nOperationID);
	m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERRECVMSG,pUser->m_nUserMsgID);
	m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::SYSTEMPUB,pUser->m_nSysMsgID);
	m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERPUB,pUser->m_nMobileMsgID);//�����ѽ��ն��ź�
	m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERCOMMENT,pUser->m_nUsercommentID);
	m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::LOGCOMMENT,pUser->m_nLogcommentID);

	//�����û�����ʱ��
	int money;
	time_t tm;
	time(&tm);
	m_pRouter->GetDataBaseMini()->Save_User_OnlineTime(pUser->m_nID, pUser->m_nLoginTime, (int)tm, money);
	m_pRouter->GetDataBaseMini()->Save_User_Timelen(pUser->m_nID, pUser->m_nLoginTime, (int)tm);
	if(pUser->m_nID < Id_Boundary)
	{
	
		m_pRouter->GetDataBaseMini()->Save_Merchant_Logout(pUser->m_nID,(int)tm);

		//�ϴ��ǳ�����
		using namespace LogInterface;
		using namespace std;
		hessianLogi hess(pUser->m_merchantid,pUser->m_nID);;
		try 
		{
			AccountArray ids;
			ids.push_back(pUser->m_nID);
			std::vector<tagGSRV_GET_USER_FLOW_ACK::tagUserFlow> userFlow(ids.size());
			uint8_t cnt = ids.size();

			if(m_pRouter->GetGlobalBusi()->QueryUserFlow(&ids[0], &userFlow[0], cnt))
			{
				CLog::Log("Logout",CLog::TYPE_ERROR,"id:%d ,Query User %u Flow Error",pUser->m_nID);
				cnt = 0;
			}
			else
			{			
				if(userFlow[0].status != XTREAM_OFFLINE && userFlow[0].flow == 1) //��ֹ�û��쳣�˳���û�˳�����״̬
				{
					SubMerchantAction info;
					info.merchantID = pUser->m_merchantid;
					info.subMerchantID = pUser->m_nID;
					info.actionTime = (int)tm;
					info.actionType = ACTION_RECEPTION;
				
					ResultInfo res = hess.GetHessiani()->uploadAction(info);
					CLog::Log("hessian",CLog::TYPE_IMPORTANT,
					"Client Forget to change Hangup state !merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
						info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
				}
			}
			SubMerchantAction info;
			info.merchantID = pUser->m_merchantid;
			info.subMerchantID = pUser->m_nID;
			info.actionTime = (int)tm;
			info.actionType = ACTION_LOGOUT;
		
			ResultInfo res = hess.GetHessiani()->uploadAction(info);
		
			CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
					info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
		}
	
		catch (std::exception& e) {
			CLog::Log("hessian",CLog::TYPE_ERROR,"merchant_id:%d, subMerchantID:%d, other exception occured: %s",
				pUser->m_merchantid, pUser->m_nID, e.what());
		}
		
	}
	
	
	if (money > 0 && pUser->m_nID != 0)
	{
		if (money > 5)
			money = 5;
		m_pRouter->GetDataBaseWeb()->Update_User_Money(pUser->m_nID, money);
		//���û����ͳ�ֵ��Ϣ
		CLog::Log("Money", CLog::TYPE_IMPORTANT, "�û�:%d���ͻ���:%d", pUser->m_nID, money*5);
	}
	
	//�����û��ڴ�
	pUser->m_FriendList.Clear();
	pUser->m_PermFamilyMap.Lock();
	pUser->m_PermFamilyMap.clear();
	pUser->m_PermFamilyMap.UnLock();
	pUser->m_TempFamilyList.Lock();
	pUser->m_TempFamilyList.clear();
	pUser->m_TempFamilyList.UnLock();
		
	CUserManager & userManager = CAppManager::GetInstance().GetUserManager();
	pUser->DelFromOnlineList();
	userManager.DelHash(pUser->m_nID);	
	m_pAppNetwork->UserLogout(pUser->m_nID);
	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userinfo;	
	m_pRouter->GetGlobalBusi()->QueryUserInfo(&uid,count,&userinfo);

	if(userinfo.serverip == (uint32)m_ConfigInfo.GetDeviceIp())	//ȫ���Ϻ��Լ���IP��ַһ��
	{
		CLog::Log("Logout",CLog::TYPE_DEBUG,"uid:%d, count:%d ipaddr: %u",uid,count,userinfo.serverip);
		m_pRouter->GetGlobalBusi()->DelUserInfo(&uid,count);
	}
	pUser = NULL;

	SysStat::GetInstance().rmOnlineNum();

}

//#define MAX_NUM 20
void CBusiLogout::WebLogout(CUser * pUser)
{
	const uint8 MAX_NUM = 20;
	if(!pUser)
		return ;

	pUser->m_nStatus = XTREAM_EXIT;

	//�������ݿ����߱�״̬����ΪҪ����֪ͨ��������������ǰ����
	m_pRouter->GetDataBaseMini()->Save_User_Logout(pUser->m_nID);

	//�û�״̬�ı�֪ͨ��������
	XT_WEB_BUSI_LIST_REQ WebBusiList;
	XT_WEB_BUSI_LIST_ACK::BusiList pListInfo[MAX_NUM];
	uint32 nRetCount = MAX_NUM;
	uint32 id_list[MAX_NUM];
	tagGSRV_GET_USER_STATUS_ACK::tagUserStatus UStatus[MAX_NUM];
	
	WebBusiList.id = pUser->m_nID;
	WebBusiList.msgnum = MAX_NUM;
	if(m_pRouter->GetDataBaseMini()->WebBusiListReq(&WebBusiList,pListInfo,nRetCount) == DB_ERROR)
	{
		CLog::Log("CBusiLogout::WebLogout", CLog::TYPE_ERROR, "%d WebBusiList Req Failed !",pUser->m_nID);	
	}

	if(nRetCount > 0)
	{
		uint8 count = nRetCount;
		for(uint32 j = 0; j < nRetCount; j ++)
		{
			id_list[j] = pListInfo[j].id;
		}
		
		if (RESULT_FAILED == m_pRouter->GetGlobalBusi()->QueryUserStatus(id_list,UStatus,count)) 
		{
			CLog::Log("CBusinessWebBusiList",CLog::TYPE_ERROR,"%d QueryUserStatus error !",pUser->m_nID);
		}

		for(int i = 0; i < count; i ++)
		{
			if(UStatus[i].status != XTREAM_OFFLINE)
			{
				FriendInfo* friendInfo = pUser->m_FriendList.Add(UStatus[i].id);
				if(friendInfo)
				{
					friendInfo->id = UStatus[i].id;
					friendInfo->status = UStatus[i].status;
				}
			}	
		}
	}
	pUser->NotifyFriendStatus();
	
	//��ȫ��ע�����û���Ϣ
	uint32	uid = pUser->m_nID;
	uint8	count = 1;

	pUser->LogoutTempFamily(m_pAppNetwork, m_pRouter->GetGlobalFamilyBusi());

	pUser->m_PermFamilyMap.Lock();
	FamilyList_Type::iterator itf = pUser->m_PermFamilyMap.begin();
	for(;itf != pUser->m_PermFamilyMap.end(); ++itf)
	{
		//����̶�Ⱥ��Ϣid
		m_pRouter->GetDataBaseMini()->Save_User_RecvFamilyMsgID(pUser->m_nID, (*itf).first, (*itf).second);
	}
	pUser->m_PermFamilyMap.UnLock();

	m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USEROPERATION,pUser->m_nOperationID);
	m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERRECVMSG,pUser->m_nUserMsgID);
	m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::SYSTEMPUB,pUser->m_nSysMsgID);
	m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERPUB,pUser->m_nMobileMsgID);//�����ѽ��ն��ź�
	m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERCOMMENT,pUser->m_nUsercommentID);
	m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::LOGCOMMENT,pUser->m_nLogcommentID);

	//�����û�����ʱ��
	int money;
	time_t tm;
	time(&tm);
	m_pRouter->GetDataBaseMini()->Save_User_OnlineTime(pUser->m_nID, pUser->m_nLoginTime, (int)tm, money);
	if(pUser->m_nID < Id_Boundary)
	{
		m_pRouter->GetDataBaseMini()->Save_Merchant_Logout(pUser->m_nID,(int)tm);
		using namespace LogInterface;
		using namespace std;		
		hessianLogi hess(pUser->m_merchantid,pUser->m_nID);		
		try 
		{
			
			SubMerchantAction info;
			info.merchantID = pUser->m_merchantid;
			info.subMerchantID = pUser->m_nID;
			info.actionTime = (int)tm;
			info.actionType = ACTION_LOGOUT;
			
		
			ResultInfo res = hess.GetHessiani()->uploadAction(info);
		
		CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
					info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
		}
	
		catch (std::exception& e) {
			CLog::Log("hessian",CLog::TYPE_ERROR,"merchantID:%d, subMerchantID:%d, other exception occured: %s",
				pUser->m_merchantid, pUser->m_nID, e.what());
		}
	}
	m_pRouter->GetDataBaseMini()->Save_User_Timelen(pUser->m_nID, pUser->m_nLoginTime, (int)tm);

	if (money > 0 && pUser->m_nID != 0)
	{
		if (money > 5)
			money = 5;
		m_pRouter->GetDataBaseWeb()->Update_User_Money(pUser->m_nID, money);
		//���û����ͳ�ֵ��Ϣ
		CLog::Log("Money", CLog::TYPE_IMPORTANT, "�û�:%d���ͻ���:%d", pUser->m_nID, money*5);
	}
	
	//�����û��ڴ�
	pUser->m_FriendList.Clear();
	pUser->m_PermFamilyMap.Lock();
	pUser->m_PermFamilyMap.clear();
	pUser->m_PermFamilyMap.UnLock();
	pUser->m_TempFamilyList.Lock();
	pUser->m_TempFamilyList.clear();
	pUser->m_TempFamilyList.UnLock();
		
	CUserManager & userManager = CAppManager::GetInstance().GetUserManager();
	pUser->DelFromOnlineList();
	userManager.DelHash(pUser->m_nID);	
	m_pAppNetwork->UserLogout(pUser->m_nID);
	
//	CLog::Log("WebLogout",CLog::TYPE_DEBUG,"uid:%d, count:%d ",uid,count);
	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userinfo;	
	m_pRouter->GetGlobalBusi()->QueryUserInfo(&uid,count,&userinfo);

	if(userinfo.serverip == (uint32)m_ConfigInfo.GetDeviceIp())	//ȫ���Ϻ��Լ���IP��ַһ��
	{
		CLog::Log("WebLogout",CLog::TYPE_DEBUG,"uid:%d, count:%d ipaddr: %u",uid,count,userinfo.serverip);
		m_pRouter->GetGlobalBusi()->DelUserInfo(&uid,count);
	}

	pUser = NULL;

	SysStat::GetInstance().rmOnlineNum();

}


CBusiRegister::CBusiRegister(CRouterBusiness* pRouter):
CBusiness(pRouter,CMD_REGIST_REQ,CMD_REGIST_ACK)
{
	m_uid = 0;
	m_pDataBaseWeb = m_pRouter->GetDataBaseWeb();
	m_pAppNetwork = &(CAppManager::GetInstance().GetAppNetWork());
	m_szError[0] = '\0';
}

CBusiRegister::~CBusiRegister(void)
{
}


//��½ҵ����Ϊһ�������ҵ����,�ǻ��������Ĵ�����ҪӦ�ò����мӽ���
int CBusiRegister::DoWork()
{
	//��ʼ����������
	RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
	LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)m_pSendData->data;
	m_szError[0] = '\0';

	CRecvPacket rp(pRecvData->data + sizeof(XT_HEAD),pRecvData->data_len - sizeof(XT_HEAD));
	//���
	XT_REGIST_REQ  req;
	rp.ReadPacket(req.key,sizeof(req.key));
	
	CLog::Log("CBusiRegister", CLog::TYPE_IMPORTANT, "sid:%u, did:%x, flag:%u" , lpRecvHead->sid, lpRecvHead->did,lpRecvHead->flag);	



	char plain[MAX_RECV_BUFFERS] = {0};
	int dwPlainLen = 0;
	dwPlainLen = rp.ReadPacket(plain,rp.GetSize() - 16);
	if( !xTEADecryptWithKey(plain,dwPlainLen,req.key, plain ,&dwPlainLen) ) 
	{
		return RESULT_FAILED;	//����ʧ��
	}

// 	rp.SetData(plain,dwPlainLen);
// 	rp	>>req.regfrom
// 		>>req.username
// 		>>req.sex
// 		>>req.nickname
// 		>>req.password
// 		>>req.email;
// 
// 	get_md5_password(req.password);
// 	uint8 ReturnCode = 0;
// 	int dbret = m_pDataBaseWeb->Add_Register_Info(&req, m_uid, ReturnCode);
// 
 	CSendPacket sp(m_pSendData->data + sizeof(XT_HEAD));	
// 	if(dbret != DB_SUCCESS)
// 	{
// 		//��װ���ش���ԭ���
// 		char szError[MAX_ERROR_BUF + 1];
// 		CError::DB_Error(dbret, szError, true);
// 		sp	<<(uint8)RESULT_FAILED
// 			<<szError;
// 	}
// 	else
// 	{
// 		sp	<<(uint8)RESULT_SUCCESS
// 			<<m_uid;
// 	}
	sp	<<(uint8)RESULT_FAILED
		<<"���ڷ�ţ��ע�ᣡhttp://www.feiniu.com";
	m_pSendData->data_len = static_cast<uint16>(sizeof(XT_HEAD) + sp.GetSize());
	InitHead(XT_HEAD::FLAG_SRVACK,lpSendHead,m_nBusinessIDAck,lpRecvHead->seq,m_pSendData->data_len - sizeof(XT_HEAD),XT_HEAD::DEST_TYPE_USER,lpRecvHead->sid);

	int tlen = 0;
	if(!xTEAEncryptWithKey(m_pSendData->data + sizeof(XT_HEAD),m_pSendData->data_len - sizeof(XT_HEAD),req.key,m_pSendData->data + sizeof(XT_HEAD),&tlen))
	{
		CLog::Log("CBusiRegister",CLog::TYPE_ERROR,"xTEAEncryptWithKey failed");
		return BUSIERR_INTERNAL;
	}
	m_pSendData->data_len = (uint16)tlen + sizeof(XT_HEAD);

	//�ظ�ע������,���ô����ֱ�ӷ���
	m_pSendData->retry_times = 0;
	m_pSendData->dest_ip = pRecvData->src_ip;
	m_pSendData->port = pRecvData->port;

	m_pAppNetwork->SendDataTo(*m_pSendData);


	return RESULT_SUCCESS;
}

int CBusiRegister::SendReply()
{
	return 0;
}

char *CBusiRegister::get_md5_password(char* password)
{
	char sSecret[255]="asdfds!@#$1234@!#$ASDFasdfQ@#4345asDF";
	char sPWD[255];
	strcpy_s(sPWD, password);
	strcat_s(sPWD, sSecret);

	unsigned char digest[16];
	m_md5.MD5Init();
	m_md5.MD5Update ((unsigned char*)sPWD, (unsigned int)strlen(sPWD));
	m_md5.MD5Final ((unsigned char*)digest);

	string sHex;//.GetBuffer(sPWD.GetLength())
	sHex = ToHexString(digest,16);
	strcpy_s(password,sHex.length(),sHex.c_str());

	return password;
}

