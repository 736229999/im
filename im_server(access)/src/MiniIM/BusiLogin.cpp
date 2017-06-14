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
#include "BusiCacheInterface.h"
#include "improtocol.pb.h"
#ifndef IM_USE_IMAPI
#include "ClientService.h"
#endif
#include "ThreadMsgNotify.h"
#include "httpclient.h"
#include "imapi.h"
#include "UserManager.h"

#define MAX_SEARCH_FRINED_RETURN_NUM 50

int CalcQueueLength(int QueueOrigin,int row = 4,int col = 4);

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
    for(i=0 ; i<len; i++)
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
#ifdef ACCESS_SERVER
/*
static long curl_writer(void *data, int size, int nmemb, string *content)
{
    long sizes = size * nmemb;
    *content += string((char*)data, sizes);
    return sizes;
}
*/


/////////////////////// 版本方面的错误 //////////////////////////
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


//登陆业务作为一个特殊的业务处理,是基础传输层的处理，需要应用层自行加解密
int CBusiLogin::DoWork()
{
    //初始化基本数据
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
        //解包
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

        if ((req.login_flag& 0xC) >> 2 == 2 )
            rp >>req.auth_session;
        rp.ReadPacket(req.random_key,sizeof(req.random_key));

        char plain[MAX_RECV_BUFFERS] = {0};
        int dwPlainLen = 0;
        dwPlainLen = rp.ReadData(plain,sizeof(plain));
    
		if( !xTEADecryptWithKey(plain,dwPlainLen,req.random_key, plain ,&dwPlainLen) )
        {
        	CLog::Log("crashInfo", CLog::TYPE_IMPORTANT, "DecryptFailed");
            return RESULT_FAILED;   //解密失败
        }
		else
		{
        	CLog::Log("crashInfo", CLog::TYPE_IMPORTANT, "DecryptSuccess");
		}

        rp.SetData(plain,dwPlainLen);

        rp.ReadPacket(req.password,sizeof(req.password));
        rp>>req.status>>req.lanip>>req.lanport;

        uRet = HandleReq(&req, lpRecvHead);


        CLog::Log("Login1", CLog::TYPE_IMPORTANT, "sid:%u, did:%u, flag:%x, login_flag:%u, username:%s, status:%u, lanip:%u, lanport:%u, sessionID:%s" ,
                  lpRecvHead->sid, lpRecvHead->did, lpRecvHead->flag, req.login_flag, req.uLoginKey.username, req.status, req.lanip, req.lanport, req.sessionID);


    }
    else
    {
        XT_LOGIN_REQ*  req = (XT_LOGIN_REQ*)(pRecvData->data + sizeof(XT_HEAD));
        uRet = HandleReq(req, lpRecvHead);


        CLog::Log("Login2", CLog::TYPE_IMPORTANT, "sid:%u, did:%u, flag:%x, login_flag:%u, username:%s, status:%u, lanip:%u, lanport:%u, sessionID:%s" ,
                  lpRecvHead->sid, lpRecvHead->did, lpRecvHead->flag, req->login_flag, req->uLoginKey.username, req->status, req->lanip, req->lanport, req->sessionID);

    }


    if(uRet != RESULT_SUCCESS)
    {
        //组装返回错误原因包
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
                sp<<"登录失败";
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
                len = strlen("登录失败");
                strncpy((char*)ack + sizeof(uint8),"登录失败",len);
                *((char*)ack + sizeof(uint8) + len) = 0;
            }
            m_pSendData->data_len = static_cast<uint16>(sizeof(XT_HEAD) + len);
        }
    }
    if(lpRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
        InitHead(XT_HEAD::FLAG_SRVACK,lpSendHead,m_nBusinessIDAck,lpRecvHead->seq,m_pSendData->data_len - sizeof(XT_HEAD),XT_HEAD::DEST_TYPE_USER,lpRecvHead->sid);
    else
        InitHead(XT_HEAD::FLAG_MOBSRVACK,lpSendHead,m_nBusinessIDAck,lpRecvHead->seq,m_pSendData->data_len - sizeof(XT_HEAD),XT_HEAD::DEST_TYPE_USER,lpRecvHead->sid);

    if( m_IsOverLoad )
    {
        //暂时不用用户过载(超过最大同时在线用户数量)，zhangmin 20150425，会挂
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
        //加密，发送数据，根据ret选择加密方式,ret == 0 采用md5(md5(password)) 加密，ret == 1 采用randomkey 加密
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

        int tlen = 0;
        if(!xTEAEncryptWithKey(m_pSendData->data + sizeof(XT_HEAD),m_pSendData->data_len - sizeof(XT_HEAD),szKey,m_pSendData->data + sizeof(XT_HEAD),&tlen))
        {
            CLog::Log("CBusiLogin::DoWork",CLog::TYPE_IMPORTANT,"%d xTEAEncryptWithKey failed", m_uid);
            return BUSIERR_INTERNAL;
        }

        m_pSendData->data_len = (uint16)tlen + sizeof(XT_HEAD);
    }

    //回复登陆请求,调用传输层直接发送
    m_pSendData->retry_times = 0;
    m_pSendData->dest_ip = pRecvData->src_ip;
    m_pSendData->port = pRecvData->port;

    m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),*m_pSendData);

    return RESULT_SUCCESS;
}
uint32 CBusiLogin::HandleReq(const LPXT_LOGIN_REQ lpReq, LPXT_HEAD lphead)
{
    //处理包
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
                sprintf_s(m_szError, "%s", "您的账号尚未激活");
            else
                CError::DB_Error(uRet,m_szError,true);


            CLog::Log("CBusiLogin",CLog::TYPE_IMPORTANT,"取密码失败：%s",m_szError);
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
            CLog::Log("CBusiLogin",CLog::TYPE_IMPORTANT,"取密码失败2：%s",m_szError);
            return RESULT_FAILED;
        }
    }

    cPass[sizeof(cPass)-1]='\0';
    if (strcmp(cPass, "") == 0)
    {
        sprintf_s(m_szError, "%s", "密码错误");
        CLog::Log("CBusiLogin",CLog::TYPE_IMPORTANT,"密码比较错误（为空）：%s",m_szError);
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
        sprintf_s(m_szError, "%s", "密码错误");
        CLog::Log("CBusiLogin",CLog::TYPE_IMPORTANT,"密码二进制比较错误：%s",m_szError);
        return RESULT_FAILED;
    }
#endif
    char cPass[40 + 1];
    unsigned char digest[16];

    strcpy(cPass, lpReq->password);
    MD5Password(cPass);

    //判断登陆的是卖家还是买家
    char szGuid[MAX_GUID_LEN] = "";
    char szUserName[MAX_USERNAME_LEN + 1] = "";
    char szMobileNo[MAX_PHONE_LEN + 1]="";
    char szEmail[MAX_EMAIL_LEN+1]="";
    char szNickName[61]="";
    char szHeadImgUrl[MAX_HEADURL_LEN+1] ="";
    char szPassword[MAX_PASSWORD_LEN]="";
    uint8 buyerLevel = 0;
    switch ((lpReq->login_flag & 0xC) >> 2)
    {
        case XT_LOGIN_REQ::CS:
        {
            CLog::Log("CheckCS",CLog::TYPE_IMPORTANT,"CS");
            uint8 state = 0;

            strncpy (szPassword,lpReq->password,MAX_PASSWORD_LEN);
            if(!(m_pDataBaseWeb->Get_CS_PWD(lpReq->uLoginKey.username, szPassword, m_ack.user_id, state) ))
            {
                CLog::Log("CheckCS",CLog::TYPE_IMPORTANT,"Get_CS_PWD! username:%s, id:%d, state:%d,dbPassword:%s",
                          lpReq->uLoginKey.username, m_ack.user_id, state,szPassword);
                if(state == 0)
                {
                    sprintf_s(m_szError, "%s", "您的帐号已停用");
                    return RESULT_FAILED;
                }

                if(strcmp(szPassword,cPass) == 0)
                {
                    CLog::Log("CheckCS",CLog::TYPE_IMPORTANT,"check cs PSW right! username:%s, user_id:%d, state:%d, dbpassword:%s, password:%s",
                              lpReq->uLoginKey.username, m_ack.user_id, state, szPassword, cPass);
                    m_uid = m_ack.user_id;
                }
                else
                {
                    sprintf_s(m_szError, "%s", "用户名或密码错误");
                    CLog::Log("CheckCS",CLog::TYPE_ERROR,"check cs PSW error! username:%s, user_id:%d",
                              lpReq->uLoginKey.username,  m_ack.user_id);
                    return RESULT_FAILED;
                }

            }
            else
            {
                sprintf_s(m_szError, "%s", "用户不存在");
                CLog::Log("CheckCS",CLog::TYPE_ERROR,"Get pwd fail! username:%s, id:%d",
                          lpReq->uLoginKey.username, m_ack.user_id);
                return RESULT_FAILED;
            }

            break;
        }
        case XT_LOGIN_REQ::BUYER:
        {
            //web端买家
            if(lpReq->sessionID[0] != 0 )
            {
                CLog::Log("CheckBuyer",CLog::TYPE_DEBUG,"sessionID:%s",lpReq->sessionID);
                int nlen = strlen(lpReq->sessionID);
                if(nlen >= 36)
                {
                	memcpy(szGuid, lpReq->sessionID + (nlen-36), 36);
                	strcpy(szUserName, lpReq->uLoginKey.username);
                	strcpy(szMobileNo, lpReq->mobilephoneno);
                	strcpy(szEmail,lpReq->email);
                	//strcpy(szNickname,lpReq->uLoginKey.username);
                	szGuid[36] = '\0';
                }
                else
                {
                	CLog::Log("CheckBuyer",CLog::TYPE_ERROR,"GUID错误");
                }
            }
            //客户端买家
            else if(CheckBuyer(lpReq->uLoginKey.username, lpReq->password,szGuid, szUserName))
            {
                CLog::Log("CheckBuyer",CLog::TYPE_DEBUG,"username:%s",szUserName);
            }
            else
            {
                sprintf_s(m_szError, "%s", "用户名或密码错误");
                CLog::Log("CBusiLogin SaveAccountBuyer", CLog::TYPE_ERROR, "Buyer:%s pwsError", szUserName);
                return RESULT_FAILED;
            }
#ifdef IM_USE_IMAPI
	    buyerLevel = 0;
#else
            CLog::Log("CBusiLogin", CLog::TYPE_DEBUG, "check GUID 1:%s ", szGuid);
            CheckBuyerLevel(szGuid, buyerLevel);
            CLog::Log("CBusiLogin", CLog::TYPE_DEBUG, "check GUID 2:%s ", szGuid);
#endif
            //获取买家信息
#if 0
            if(GetBuyerInfo(szGuid, szMobileNo, szEmail, szNickName, szHeadImgUrl))
            {
                CLog::Log("GetBuyerInfo",CLog::TYPE_IMPORTANT,
                          "username:%s, email:%s, mobileno:%s, szNickname:%s, szHeadImgFile:%s",
                          szUserName, szEmail, szMobileNo, szNickName, szHeadImgUrl);
            }
#endif
            if(strcmp(szNickName, "") == 0)
            {
                strncpy(szNickName,szUserName,60);
                CLog::Log("GetBuyerInfo",CLog::TYPE_IMPORTANT,"Buyer:%s, getInfoSelf",szUserName);
            }


            //内部版必须在员工系统录入
#ifdef PRE_INTERNAL
            uint32 uid = 0;
            char cPass2[60];
            uRet = m_pDataBaseWeb->Get_UserID_PWD(lpReq->uLoginKey.username,(uint32 &)uid,cPass2);
            if(uRet != DB_SUCCESS)
            {
                sprintf_s(m_szError, "%s", "您的域帐号尚未在哞哞激活");
                return RESULT_FAILED;
            }
#endif


            //验证通过,存储帐号
            CLog::Log("CBusiLogin", CLog::TYPE_DEBUG, "check GUID 3:%s, length:%d", szGuid, strlen(szGuid));
            if(strlen(szGuid)== 36 )
            {
                if(strlen(szUserName) == 0)
                {
                    sprintf_s(m_szError, "%s", "用户名错误");
                    CLog::Log("CBusiLogin SaveAccountBuyer",CLog::TYPE_ERROR,"Buyer: %s GetUserName Failed!",lpReq->uLoginKey.username);
                    return RESULT_FAILED;
                }
                //else if(m_pDataBaseWeb->SaveAccountBuyer(szUserName, cPass, szGuid, m_uid, szMobileNo, szEmail, szNickName, szHeadImgUrl) != DB_SUCCESS)
                else if(! SaveBuyer(szUserName, cPass, szGuid, m_uid, szMobileNo, szEmail, szNickName, szHeadImgUrl))
				{
                    sprintf_s(m_szError, "%s", "您的用户名或密码错误");
                    CLog::Log("CBusiLogin SaveAccountBuyer",CLog::TYPE_ERROR,"Buyer:%s Save Account Failed!",lpReq->uLoginKey.username);
                    return RESULT_FAILED;
                }
            }
            else
            {
                sprintf_s(m_szError, "%s", "您用户名或密码错误");
                CLog::Log("CBusiLogin SaveAccountBuyer",CLog::TYPE_ERROR,"Buyer:%s, GUID:%s error!",lpReq->uLoginKey.username,szGuid);
                return RESULT_FAILED;
            }

            break;
        }
        case XT_LOGIN_REQ::SELLER:
        {
            if((lpReq->login_flag & 0x10) >> 4 == XT_LOGIN_REQ::CHECK_DIFF_PLACES)
            {
            #if 0	// 先屏蔽手机验证码功能
                // 校验同地登录
                string strError;
                int nRlt = IsSameAreaLogin(lpReq->uLoginKey.username, strError);
                if(nRlt == -1)
                {	// 调用失败
                	sprintf_s(m_szError, "%s", strError.c_str());
                    return RESULT_FAILED;
                }
                else if(nRlt == 0)
                {
                    // 异地登录
                    nRlt = IsBindMobile(lpReq->uLoginKey.username, strError);
                    if(nRlt == -1)
                    {	// 调用失败
                		sprintf_s(m_szError, "%s", strError.c_str());
                    	return RESULT_FAILED;
                    }
                    
                    else if(nRlt == 1)
                    {
                        // 已绑定手机
                        sprintf_s(m_szError, "%s", "您的登录出现异常，需进行手机验证");
                        return RESULT_FAILED;
                    }
                    else
                    {
                        // 未绑定手机
                        sprintf_s(m_szError, "%s", "您的登录出现异常，为了您的账户安全，请尽快绑定手机");
                        return RESULT_FAILED;
                    }
                }
            #endif
            }

            //if( CheckSellerMysql(lpReq->uLoginKey.username, lpReq->password, m_ack.user_id, m_ack.merchant_id, state) )
            int state = 0;
            string code = "";
            if( CheckSeller(lpReq->uLoginKey.username, lpReq->password, m_ack.user_id, m_ack.merchant_id, state, code) )
            {
                // 解析成功
                CLog::Log("CheckSeller",CLog::TYPE_IMPORTANT,"CheckSeller ok! username:%s, id:%d, state:%d, code:%s",
                          lpReq->uLoginKey.username, m_ack.user_id, state, code.c_str());
                if(state == 2)
                {
                    sprintf_s(m_szError, "%s", "您的帐号已停用");
                    return RESULT_FAILED;
                }

                // 解析code
                if(code != "200")
                {
                    // 登录失败
                    if(code == "205")
                    {
                        sprintf_s(m_szError, "%s", "参数错误");
                        return RESULT_FAILED;
                    }
                    else if(code == "204")
                    {
                        sprintf_s(m_szError, "%s", "参数为空");
                        return RESULT_FAILED;
                    }
                    else if(code == "210")
                    {
                        sprintf_s(m_szError, "%s", "用户名或密码错误");
                        return RESULT_FAILED;
                    }
                    else if(code == "211")
                    {
                        sprintf_s(m_szError, "%s", "此账号已被限制登录，重新设置密码，可以解除限制");
                        return RESULT_FAILED;
                    }
                }


                //if(m_pDataBaseWeb->SaveAccount(lpReq->uLoginKey.username,cPass,m_ack.user_id,lpReq->uLoginKey.username) == DB_SUCCESS)
                if(SaveSeller(lpReq->uLoginKey.username,cPass,m_ack.user_id,lpReq->uLoginKey.username) )
                {
                    m_uid = m_ack.user_id;
                    if(m_ack.merchant_id == 0)
                    {
                        sprintf_s(m_szError, "%s", "您尚未入驻飞牛，请入驻后再登录哞哞");
                        return RESULT_FAILED;
                    }
                    /*
                    else if(m_ack.merchant_id != m_ack.user_id)
                    {
                        CLog::Log("CBusiLogin SaveSubAccount",CLog::TYPE_DEBUG,"Save Account %d %d",m_ack.merchant_id,m_ack.user_id);
                        //以卖家为主账号存储卖家的子账号
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
                    sprintf_s(m_szError, "%s", "您的用户名或密码错误");
                    CLog::Log("CBusiLogin SaveAccount",CLog::TYPE_ERROR,"Seller SaveAccount Failed! username:%s, user_id:%d, merchant_id:%d",
                              lpReq->uLoginKey.username, m_ack.user_id, m_ack.merchant_id);
                    return RESULT_FAILED;
                }

				CLog::Log("crashInfo",CLog::TYPE_IMPORTANT,"SaveAccountPass");
            }
            else
            {
                // 解析失败
                sprintf_s(m_szError, "%s", "用户名或密码错误");
                CLog::Log("CBusiLogin SaveAccount",CLog::TYPE_ERROR,"Seller PSW error! username:%s, user_id:%d, merchant_id:%d",
                          lpReq->uLoginKey.username,  m_ack.user_id, m_ack.merchant_id);
                return RESULT_FAILED;
            }
            break;
        }
        case XT_LOGIN_REQ::WORK:
		{

            int code = 0;
			string email;
			string realname;
			string mobilenumber;
			string groupname;
			char groupnameStr[100] = "";
            if( CheckWork(lpReq->uLoginKey.username, cPass,code, realname,email, mobilenumber,groupname) )
            {
                // 解析成功
                CLog::Log("CheckWork",CLog::TYPE_IMPORTANT,"CheckWork ok! username:%s, realname %s,email:%s mobile %s,group %s code:%d",
                          lpReq->uLoginKey.username,realname.c_str(),email.c_str(), mobilenumber.c_str(),groupname.c_str(),code);


                // 解析code
                if(code != 200)
                {
                    // 登录失败
                    if(code == 400)
                    {
                        sprintf_s(m_szError, "%s", "用户名或密码错误");
                        return RESULT_FAILED;
                    }                    
                }
				if(realname.length() > 0 && realname.length() < 60)
				{
					utf2gbk((char *) realname.c_str(),(int) realname.length(),szNickName,60);
				}
				if(groupname.length() > 0 && groupname.length() < 100)
				{
					utf2gbk((char *) groupname.c_str(),(int) groupname.length(),groupnameStr,99);
				}
                if(m_pDataBaseWeb->SaveAccountWork(lpReq->uLoginKey.username, cPass, m_uid, (char *)mobilenumber.c_str(), (char *)email.c_str(),szNickName, szHeadImgUrl,groupnameStr) != DB_SUCCESS)
                {
                    sprintf_s(m_szError, "%s", "您的用户名或密码错误");
                    CLog::Log("CBusiLogin SaveAccountBuyer",CLog::TYPE_ERROR,"Work:%s Save Account Failed!",lpReq->uLoginKey.username);
                    return RESULT_FAILED;
                }

				CLog::Log("crashInfo",CLog::TYPE_IMPORTANT,"SaveAccountPass,id:%d nickname %s %s",m_uid,szNickName,realname.c_str());
            }
			break;

		}
				
        default:
            return RESULT_FAILED;
    }
	CLog::Log("CBusiLogin::HandleReq",CLog::TYPE_IMPORTANT,"BEFORE HexStr2BYTEBCDArray");

    HexStr2BYTEBCDArray(cPass,sizeof(digest)*2,digest);
    memcpy(m_digest,digest,sizeof(m_digest));

    //CAppManager& app = CAppManager::GetInstance();
    /*CUserManager *pUserManager = m_pRouter->GetUserManager();
    if( pUserManager->GetElemCount() >= app.GetConfig().GetMaxUserCount() )
    {
        //过载了
        m_IsOverLoad = true;
        //暂时屏蔽最大在线用户限制，zhangmin 20140425
        //return RESULT_SUCCESS;
    }*/
	CLog::Log("CBusiLogin::HandleReq",CLog::TYPE_IMPORTANT,"AFTER HexStr2BYTEBCDArray");

    RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;
    //数据有效性校验...
    //生成用户session key
    unsigned char bRandomStr[4];
    srand( (unsigned)time( NULL ) );
    *(unsigned char*)bRandomStr = rand();
    int dwSessionLen;
    if(!xTEAEncryptWithKey((const char*)bRandomStr, sizeof(bRandomStr), (char  *)digest,(char *)m_ack.session_key,&dwSessionLen))
    {
        CLog::Log("CBusiLogin::HandleReq",CLog::TYPE_IMPORTANT,"%u xTEAEncryptWithKey failed", lpRecvHead->sid);
        return BUSIERR_INTERNAL;
    }
	CLog::Log("CBusiLogin::HandleReq",CLog::TYPE_IMPORTANT,"AFTER xTEAEncryptWithKey");

    m_ack.ret = 0;
    m_ack.user_id = m_uid;
    //检查版本
    bool bLogin = true;
    int canUseVersion = 0;
    int agreeVersion = 0; 
    //int updateOption = CAppManager::GetInstance().GetUpdateOption();
    char szUpdateinfo[256] = {0}, szUpdateConfigBuyer[256] = {0}, szUpdateConfigSeller[256] = {0};

//  strcpy_s(m_ack.version_url,szUpdateUrl);
    if(m_uid < Id_Boundary) //商家
    {
    	canUseVersion = CAppManager::GetInstance().GetCanUserver();
		agreeVersion = CAppManager::GetInstance().GetAgreever();
		strcpy(szUpdateConfigSeller, CAppManager::GetInstance().GetUpdateConfigSeller());
        strcpy(m_ack.update_config,szUpdateConfigSeller);		
		strcpy(szUpdateinfo, CAppManager::GetInstance().GetUpdateInfo());
    }
	else if(m_uid < cs_Boundary)//买家
	{
		canUseVersion = CAppManager::GetInstance().GetCanUserver();
		agreeVersion = CAppManager::GetInstance().GetAgreever();
		strcpy(szUpdateConfigBuyer, CAppManager::GetInstance().GetUpdateConfigBuyer());
		strcpy(m_ack.update_config,szUpdateConfigBuyer);		
		strcpy(szUpdateinfo, CAppManager::GetInstance().GetUpdateInfo());
	}
    else	//客服
    {
    	canUseVersion = CAppManager::GetInstance().GetCSCanUserver();
		agreeVersion = CAppManager::GetInstance().GetCSAgreever();
		strcpy(szUpdateConfigSeller, CAppManager::GetInstance().GetCSUpdateConfigSeller());
        strcpy(m_ack.update_config,szUpdateConfigSeller);		
		strcpy(szUpdateinfo, CAppManager::GetInstance().GetCSUpdateInfo());
		
    }

    if(lpRecvHead->from >= agreeVersion)
    {
        m_ack.version_flag = (uint8)IMVERSION_AGREE;
    }
    else if(lpRecvHead->from >= canUseVersion)
    {
    	if(m_uid < cs_Boundary)
    	{
	        m_ack.version_flag = (uint8)CAppManager::GetInstance().GetUpdateOption(lpRecvHead->from);        
    	}
		else
		{
			m_ack.version_flag = (uint8)CAppManager::GetInstance().GetCSUpdateOption(lpRecvHead->from);	
		}
		strcpy(m_ack.update_info,szUpdateinfo);
    }
    else
    {
        bLogin = false;
        m_ack.version_flag = (uint8)IMVERSION_LOW;
        strcpy(m_ack.update_info,szUpdateinfo);
    }
	CLog::Log("CBusiLogin::HandleReq",CLog::TYPE_IMPORTANT,"AFTER UPDATE");

    if(bLogin)
    {
        uRet = LoginSucess(lpReq,lphead,buyerLevel);
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
        //组装正确的回应包
        CSendPacket sp(m_pSendData->data + sizeof(XT_HEAD));
        sp<<m_ack.ret;
        sp.WritePacket((const char *)m_ack.session_key,sizeof(m_ack.session_key));
        sp  <<m_ack.user_id
            <<m_ack.merchant_id
            <<m_ack.publicip
            <<m_ack.version_flag
            <<""
            <<m_ack.update_config
            <<m_ack.update_info
            <<uint8(0)
            <<szGuid
            <<szUserName
            <<(uint32)tm;
        CLog::Log("CBusiLogin",CLog::TYPE_DEBUG,"id:%d, LOGIN TIME %u", m_ack.user_id, tm);

        m_pSendData->data_len = static_cast<uint16>(sizeof(XT_HEAD) + sp.GetSize());
    }
    else
    {
		int nlen = strlen(lpReq->sessionID);
        if(nlen >= 36)
        {
        	memcpy(m_ack.sessionID,lpReq->sessionID,(nlen-36));
			m_ack.sessionID[nlen - 36] = '\0';
        }
        else
        {        	
        	CLog::Log("CheckBuyer",CLog::TYPE_ERROR,"GUID错误1");
        }
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

    MD5_CTX md5T;
    md5T.MD5Init();
    md5T.MD5Update ((unsigned char*)password,(uint32)strlen(password) );
    md5T.MD5Final (digest);

    string sHex=ToHexString(digest,16);

    strcpy(pPassword,sHex.c_str());

}

#ifdef IM_USE_IMAPI

void CBusiLogin::ReLoginLogout(CUser * pUser,uint8 type)
{
    if(!pUser)
        return ;

    pUser->m_nStatus = XTREAM_EXIT;

    //更新数据库在线表状态（因为要尽快通知，所以在这里提前处理）
    m_pRouter->GetDataBaseMini()->Save_User_Logout(pUser->m_nID);

    //用户状态改变通知其他好友
    pUser->NotifyFriendStatus(m_pRouter->GetGlobalBusi());
    pUser->NotifyFamilyStatus(m_pAppNetwork,m_pRouter->GetGlobalFamilyBusi(),m_pRouter->GetDataBaseMini());//modify by lwl ver:0105

    //向全局注销该用户信息
    uint32  uid = pUser->m_nID;
    uint8   count = 1;

    pUser->LogoutTempFamily(m_pAppNetwork, m_pRouter->GetGlobalFamilyBusi(), m_pRouter->GetGlobalBusi());

    pUser->m_PermFamilyMap.Lock();
    FamilyList_Type::iterator itf = pUser->m_PermFamilyMap.begin();
    for(; itf != pUser->m_PermFamilyMap.end(); ++itf)
    {
        //保存固定群消息id
        m_pRouter->GetDataBaseMini()->Save_User_RecvFamilyMsgID(pUser->m_nID, (*itf).first, (*itf).second);
    }
    pUser->m_PermFamilyMap.UnLock();

    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USEROPERATION,pUser->m_nOperationID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERRECVMSG,pUser->m_nUserMsgID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::SYSTEMPUB,pUser->m_nSysMsgID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERPUB,pUser->m_nMobileMsgID);//代表已接收短信号
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERCOMMENT,pUser->m_nUsercommentID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::LOGCOMMENT,pUser->m_nLogcommentID);

    //保存用户在线时长
    int money;
    time_t tm;
    time(&tm);
    m_pRouter->GetDataBaseMini()->Save_User_OnlineTime(pUser->m_nID, pUser->m_nLoginTime, (int)tm, money);
    m_pRouter->GetDataBaseMini()->Save_User_Timelen(pUser->m_nID, pUser->m_nLoginTime, (int)tm);
    if(pUser->m_nID < Id_Boundary || pUser->m_nID > cs_Boundary)
    {

        m_pRouter->GetDataBaseMini()->Save_Merchant_Logout(pUser->m_nID,(int)tm);

        //上传登出动作
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
                if(userFlow[0].status != XTREAM_OFFLINE && userFlow[0].flow == 1) //防止用户异常退出，没退出挂起状态
                {
                    SubMerchantAction info;
                    if(pUser->m_nID > cs_Boundary)
                    {
                        info.merchantID = -1;
                    }
                    else
                    {
                        info.merchantID = pUser->m_merchantid;
                    }
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
            if(pUser->m_nID > cs_Boundary)
            {
                info.merchantID = -1;
            }
            else
            {
                info.merchantID = pUser->m_merchantid;
            }
            //info.merchantID = pUser->m_merchantid;
            info.subMerchantID = pUser->m_nID;
            info.actionTime = (int)tm;
            info.actionType = ACTION_LOGOUT;

            ResultInfo res = hess.GetHessiani()->uploadAction(info);

            CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
                      info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
        }

        catch (std::exception& e)
        {
            CLog::Log("hessian",CLog::TYPE_ERROR,"merchant_id:%d, subMerchantID:%d, other exception occured: %s",
                      pUser->m_merchantid, pUser->m_nID, e.what());
        }

    }


    if (money > 0 && pUser->m_nID != 0)
    {
        if (money > 5)
            money = 5;
        m_pRouter->GetDataBaseWeb()->Update_User_Money(pUser->m_nID, money);
        //给用户发送冲值消息
        CLog::Log("Money", CLog::TYPE_IMPORTANT, "用户:%d赠送积分:%d", pUser->m_nID, money*5);
    }

    //回收用户内存
    pUser->m_FriendList.Clear();
    pUser->m_PermFamilyMap.Lock();
    pUser->m_PermFamilyMap.clear();
    pUser->m_PermFamilyMap.UnLock();
    pUser->m_TempFamilyList.Lock();
    pUser->m_TempFamilyList.clear();
    pUser->m_TempFamilyList.UnLock();

    CUserManager *pUserManager = m_pRouter->GetUserManager();
    pUser->DelFromOnlineList();
    pUserManager->Del(pUser->m_nID);
#ifdef ACCESS_SERVER
    m_pAppNetwork->UserLogout(pUser->m_nID);
#endif    
    CLog::Log("Logout",CLog::TYPE_DEBUG,"uid:%d, count:%d, type:%d ",uid,count,type);
    if(type != 0xAA)
    {
        m_pRouter->GetGlobalBusi()->DelUserInfo(&uid,count);
    }
    pUser = NULL;

    SysStat::GetInstance().rmOnlineNum();

}


#else

void CBusiLogin::ReLoginLogout(CUser * pUser,ClientService * clientService,uint8 type)
{
    if(!pUser)
        return ;

    pUser->m_nStatus = XTREAM_EXIT;

    //更新数据库在线表状态（因为要尽快通知，所以在这里提前处理）
    m_pRouter->GetDataBaseMini()->Save_User_Logout(pUser->m_nID);

    //用户状态改变通知其他好友
    pUser->NotifyFriendStatus(m_pRouter->GetGlobalBusi());
    pUser->NotifyFamilyStatus(m_pAppNetwork,m_pRouter->GetGlobalFamilyBusi(),m_pRouter->GetDataBaseMini());//modify by lwl ver:0105

    //向全局注销该用户信息
    uint32  uid = pUser->m_nID;
    uint8   count = 1;

    pUser->LogoutTempFamily(m_pAppNetwork, m_pRouter->GetGlobalFamilyBusi(), m_pRouter->GetGlobalBusi());

    pUser->m_PermFamilyMap.Lock();
    FamilyList_Type::iterator itf = pUser->m_PermFamilyMap.begin();
    for(; itf != pUser->m_PermFamilyMap.end(); ++itf)
    {
        //保存固定群消息id
        m_pRouter->GetDataBaseMini()->Save_User_RecvFamilyMsgID(pUser->m_nID, (*itf).first, (*itf).second);
    }
    pUser->m_PermFamilyMap.UnLock();

    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USEROPERATION,pUser->m_nOperationID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERRECVMSG,pUser->m_nUserMsgID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::SYSTEMPUB,pUser->m_nSysMsgID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERPUB,pUser->m_nMobileMsgID);//代表已接收短信号
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERCOMMENT,pUser->m_nUsercommentID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::LOGCOMMENT,pUser->m_nLogcommentID);

    //保存用户在线时长
    int money;
    time_t tm;
    time(&tm);
    m_pRouter->GetDataBaseMini()->Save_User_OnlineTime(pUser->m_nID, pUser->m_nLoginTime, (int)tm, money);
    m_pRouter->GetDataBaseMini()->Save_User_Timelen(pUser->m_nID, pUser->m_nLoginTime, (int)tm);
    if(pUser->m_nID < Id_Boundary || pUser->m_nID > cs_Boundary)
    {

        m_pRouter->GetDataBaseMini()->Save_Merchant_Logout(pUser->m_nID,(int)tm);

        //上传登出动作
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
                if(userFlow[0].status != XTREAM_OFFLINE && userFlow[0].flow == 1) //防止用户异常退出，没退出挂起状态
                {
                    SubMerchantAction info;
                    if(pUser->m_nID > cs_Boundary)
                    {
                        info.merchantID = -1;
                    }
                    else
                    {
                        info.merchantID = pUser->m_merchantid;
                    }
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
            if(pUser->m_nID > cs_Boundary)
            {
                info.merchantID = -1;
            }
            else
            {
                info.merchantID = pUser->m_merchantid;
            }
            //info.merchantID = pUser->m_merchantid;
            info.subMerchantID = pUser->m_nID;
            info.actionTime = (int)tm;
            info.actionType = ACTION_LOGOUT;

            ResultInfo res = hess.GetHessiani()->uploadAction(info);

            CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
                      info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
        }

        catch (std::exception& e)
        {
            CLog::Log("hessian",CLog::TYPE_ERROR,"merchant_id:%d, subMerchantID:%d, other exception occured: %s",
                      pUser->m_merchantid, pUser->m_nID, e.what());
        }

    }


    if (money > 0 && pUser->m_nID != 0)
    {
        if (money > 5)
            money = 5;
        m_pRouter->GetDataBaseWeb()->Update_User_Money(pUser->m_nID, money);
        //给用户发送冲值消息
        CLog::Log("Money", CLog::TYPE_IMPORTANT, "用户:%d赠送积分:%d", pUser->m_nID, money*5);
    }

    //回收用户内存
    pUser->m_FriendList.Clear();
    pUser->m_PermFamilyMap.Lock();
    pUser->m_PermFamilyMap.clear();
    pUser->m_PermFamilyMap.UnLock();
    pUser->m_TempFamilyList.Lock();
    pUser->m_TempFamilyList.clear();
    pUser->m_TempFamilyList.UnLock();

    CUserManager *pUserManager = m_pRouter->GetUserManager();
//    pUser->DelFromOnlineList();
    pUserManager->Del(pUser->m_nID);
#ifdef ACCESS_SERVER    
    m_pAppNetwork->UserLogout(pUser->m_nID);
#endif    
    CLog::Log("Logout",CLog::TYPE_DEBUG,"uid:%d, count:%d, type:%d ",uid,count,type);
    if(type != 0xAA)
    {
        m_pRouter->GetGlobalBusi()->DelUserInfo(&uid,count);
    }
    pUser = NULL;

    SysStat::GetInstance().rmOnlineNum();


    if(clientService != NULL)
    {
        //清空客服上下文
        //清除全局服务器上下文
        //duyong

        CSManager& csManager = CAppManager::GetInstance().GetCSManager();
        csManager.DelHash(clientService->getCSID());

#ifdef IM_USE_CACHE
        if(type != 0xAA)
        {
            m_pRouter->GetGlobalBusi()->DelCSInfo(&uid,count);
        }

#endif
        clientService = NULL;
    }


}

#endif
int CBusiLogin::LoginSucess(const LPXT_LOGIN_REQ lpReq, LPXT_HEAD lphead,uint8 level)
{
    //登陆成功处理

    CConfigInfo m_Config;
    CUserManager *pUserManager = m_pRouter->GetUserManager();
    shared_ptr<CUser> pUser = pUserManager->Find(m_uid);
    int pUsertype = ((lpReq->login_flag & 0xC) >> 2);
	#ifndef IM_USE_IMAPI
    ClientService * clientService = NULL;
    CSManager & csManager = CAppManager::GetInstance().GetCSManager();
//  CSQueueManager& csQueueManager = CAppManager::GetInstance().GetCSQueueManager();
    if( pUsertype == XT_LOGIN_REQ::CS )
    {
        //为客服创建一个上下文
        clientService= csManager.Find(m_uid);
    }
	#endif
	int LogoutCount=0;
	
	if(pUser != NULL && 1 == pUser->m_isLogout)	//有退出线程正在退出，锁住用户
	{
		while(LogoutCount < 10)
		{
			if(pUser == NULL)
			{
				CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"1用户%d 地址%p 从等待中退出，继续处理登录操作!",m_uid,pUser.get());
				break;
			}
			else if(0 == pUser->m_isLogout)
			{				
				pUser = pUserManager->Find(m_uid);	//更新一下pUser数据				
				CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"2用户%d 地址%p 从等待中退出，继续处理登录操作!",m_uid,pUser.get());
				break;
			}
			LogoutCount++;			
			CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"用户 %d正在删除操作中,等待中 %d!",m_uid,LogoutCount);
			sleep(1);
		}		
	}
    //判断是否重登陆
    RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;   
    LPONLINE_USER_INFO lpOnlineUser = m_pAppNetwork->FindOnlineUser(m_uid);
    //在本服务器上的登录
    if(pUser != NULL && lpOnlineUser && lpOnlineUser->isDirect)
    {
        if(pRecvData->src_ip == lpOnlineUser->uip && pRecvData->port == lpOnlineUser->uport)
        {
			// 如果是客服重登录，清空监控状态
			#ifdef IM_USE_IMAPI
			if(pUser != NULL)
			{
				pUser->g_cs_set_director(m_uid,(uint32)-1,m_pRouter);
    			pUser->g_cs_set_chatting_buyer(m_uid,(uint32)-1,m_pRouter);
    			pUserManager->AddUserToRedis(pUser);
			}   
			#else
			
			if(clientService != NULL)
			{
				g_cs_set_director(m_uid,(uint32)-1,m_pRouter);
    			g_cs_set_chatting_buyer(m_uid,(uint32)-1,m_pRouter);
			}   
			#endif
#ifdef IM_USE_MULTIDEVICE

			//本地多用户登录
		   CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"another Id:%u relogin !",m_uid);
			

#else			
            //本地重登录
            CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"Id:%u relogin !",m_uid);
            memcpy(m_ack.session_key,lpOnlineUser->sessionKey,sizeof(m_ack.session_key));
#endif
            return RESULT_SUCCESS;
        }
        else
        {
            //异地重登录
            //登出先前的客户端

            TransitReLoginMsg(lpOnlineUser);
            //CBusiLogout busiLogout(m_pRouter);
            //busiLogout.Logout(pUser,clientService);
            //不使用上面代码避免read阻塞
			#ifdef IM_USE_IMAPI
            ReLoginLogout(pUser.get());
			#else
			ReLoginLogout(pUser.get(),clientService);
			#endif
            pUser = NULL;
        }
    }
    else if (pUser != NULL && pUser->m_bIsFullInfoValid)
    {
        CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"Id:%u server already has info !",m_uid);
        pUser->Reset();
    }
    //广播其他服务器登出用户
    CConfigInfo m_ConfigInfo;
    list < uint32 >  ServerList;
    m_ConfigInfo.GetServerList(&ServerList);


    SendDataStruct sendData;
    LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

    char* pMsgData = sendData.data + sizeof(XT_HEAD);

    CSendPacket SendPacket;
    SendPacket.SetData(pMsgData,MAX_PROTOLPACKET_LEN);

    SendPacket  << m_uid
                << (uint8)0xAA;

    sendData.data_len = (uint16)sizeof(XT_HEAD)  + SendPacket.GetSize();

    list<uint32>::iterator it = ServerList.begin();
    for(; it !=  ServerList.end(); it++)
    {
        if(*it == (uint32)m_ConfigInfo.GetDeviceIp())
        {
            CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"跳过自己的服务器 %u 去掉用户 %d的全局信息 !",*it,m_uid);
            continue;
        }
        CAppManager::GetInstance().InitHead(
            lpSendHead,
            CMD_LOGOUT,
            (uint16)(SendPacket.GetSize()),
            1,XT_HEAD::DEST_TYPE_SERVER);
        CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
            sendData.data,sendData.data_len,0,0, *it, PORT_IM_SVRP2P);
        CLog::Log("CBusiLogin::LoginSucess",CLog::TYPE_IMPORTANT,"通知服务器 %u 去掉用户 %d的全局信息 !",*it,m_uid);
    }

    // 制作该用户的P2P SESSIONKEY
    char p2pSessionKey[16] = {0};
    unsigned char digest1[16];
    m_md5.MD5Init();
    m_md5.MD5Update ((unsigned char*)m_ack.session_key,sizeof(m_ack.session_key));
    m_md5.MD5Final ((unsigned char*)digest1);
    unsigned char digest2[16];
    m_md5.MD5Init();
    m_md5.MD5Update ((unsigned char*)&m_uid,sizeof(m_uid));
    m_md5.MD5Final ((unsigned char*)digest2);
    for(int i=0; i<16; i++) p2pSessionKey[i]=digest1[i]&digest2[i];


    //重置用户信息
    if (pUser == NULL)
    {
    	pUser = pUserManager->CreateNew();
        pUser->Reset();
        pUser->m_nID = m_uid;

        //CLog::Log("CBusiLogin",CLog::TYPE_IMPORTANT,"begin AddToOnlineList ");
//        pUser->AddToOnlineList();
        //CLog::Log("CBusiLogin",CLog::TYPE_IMPORTANT,"finish AddToOnlineList ");

        SysStat::GetInstance().addOnlineNum();

        pUser->m_nUsertype = pUsertype;

#ifndef IM_USE_IMAPI
        if( pUsertype == XT_LOGIN_REQ::CS ) //增加客服上下文
        {
            clientService = csManager.AddHash(m_uid);
            clientService->Reset();
            clientService->setCSID(m_uid);
            clientService->AddToOnlineList();
        }
#endif
    }


	
    m_pRouter->GetDataBaseMini()->GetWokuMsgID(m_uid,pUser->m_nOperationID,pUser->m_nUserMsgID,pUser->m_nSysMsgID,pUser->m_nMobileMsgID,pUser->m_nUsercommentID,pUser->m_nLogcommentID);

    pUser->m_nID = m_uid;
    strcpy_s(pUser->m_szUserName,lpReq->uLoginKey.username);


    //更新用户状态
    if (lpReq->status!= XTREAM_ONLINE && lpReq->status != XTREAM_HIDE && XTREAM_LEAVE != XTREAM_LEAVE)
        lpReq->status = XTREAM_ONLINE;

    pUser->m_nStatus = lpReq->status;
    pUser->m_nIP     = pRecvData->src_ip;
    pUser->m_nPort   = pRecvData->port ;
    pUser->m_nLanIP  = lpReq->lanip;
    pUser->m_nLanPort= lpReq->lanport;
    pUser->m_nNewlyLiveTime = timeGetTime();
    pUser->m_nServerIp = m_Config.GetDeviceIp();
    pUser->flag     = lpRecvHead->flag;
    pUser->m_nLevel = level;
    time_t tm;
    time(&tm);
    pUser->m_nLoginTime = (uint32)tm;
    memcpy(pUser->m_SessionKey,m_ack.session_key,MAX_SESSIONKEY_LEN);
    memcpy(pUser->m_P2PSessionKey,p2pSessionKey,MAX_SESSIONKEY_LEN);
    pUser->m_ClientVersion = lpRecvHead->from;
    pUser->m_nLoginType = lphead->flag;
	pUser->m_nflow = 0;
    CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT,"id:%d, Login Type %x Level %d",pUser->m_nID, pUser->m_nLoginType,pUser->m_nLevel);
#ifdef IM_USE_MULTIDEVICE
	multiDevice_t device;
	device.login_type = lpRecvHead->flag;
	device.m_nIP= pRecvData->src_ip;
	device.m_nPort = pRecvData->port;	
    memcpy(pUser->m_SessionKey,m_ack.session_key,MAX_SESSIONKEY_LEN);	
	pUser->m_multiDeviceinfo.push_back(device);
#endif
    //调用的应用网络层接口
#ifdef ACCESS_SERVER    
    m_pAppNetwork->UserLogin(m_pRouter->GetGlobalBusi(), m_uid,pRecvData->src_ip,
                             pRecvData->port,(const char *)m_ack.session_key,
                             p2pSessionKey,lpReq->lanip,lpReq->lanport,lpRecvHead->flag);
#endif                             
    m_pRouter->GetDataBaseMini()->Save_User_Login(m_uid,pRecvData->src_ip,pRecvData->port,
            CAppManager::GetInstance().GetConfig().GetServerNo());

    if(pUser->m_nID < Id_Boundary || pUser->m_nID > cs_Boundary)
    {
        m_pRouter->GetDataBaseMini()->Save_Merchant_Login(m_uid,(int)tm);
        //上传登录动作
        using namespace LogInterface;
        using namespace std;
        hessianLogi hess(m_ack.merchant_id,m_uid);
        pUser->m_merchantid = 0;
        {
            try
            {
                SubMerchantAction info;
                if(pUser->m_nID > cs_Boundary)
                {
                    info.merchantID = -1;
                }
                else
                {
                    info.merchantID = m_ack.merchant_id;
                }
                info.subMerchantID = m_uid;
                info.actionTime = (int)tm;
                info.actionType = ACTION_LOGIN;
                pUser->m_merchantid = m_ack.merchant_id;

                ResultInfo res = hess.GetHessiani()->uploadAction(info);


                CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
                          info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
            }
            catch (std::exception& e)
            {

                CLog::Log("hessian",CLog::TYPE_ERROR,"merchant_id:%d, subMerchantID:%d, other exception occured: %s",
                          m_ack.merchant_id, m_uid, e.what());
            }
        }
    }


    //将web用户向全局更新一下
    if(pUser->m_nID  > Id_Boundary && pUser->m_nID <cs_Boundary)
    {

//          if ( == XTREAM_OFFLINE)
        CLog::Log("LoginSuccess", CLog::TYPE_IMPORTANT, "向全局注册了一个WEB用户：%d 状态 %d", pUser->m_nID,pUser->m_nStatus);
        m_pRouter->GetGlobalBusi()->AddModiUserInfo(pUser.get(),tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_MODIFY,0);
    }

	// 用户全量信息写到redis
	pUserManager->AddUserToRedis(pUser);

	// 更新网络层内存
	LPONLINE_USER_INFO pOnlineUser = CAppManager::GetInstance().GetAppNetWork().FindOnlineUser(pUser->m_nID);
	if( pOnlineUser != NULL )
	{
		pOnlineUser->newlylivetime = pUser->m_nNewlyLiveTime;
		pOnlineUser->merchantID = pUser->m_merchantid;
	}

    return RESULT_SUCCESS;
}

//重登录时，给先前登录的发重登陆退出提示
//做为已登陆用户的一个业务处理
int CBusiLogin::TransitReLoginMsg(LPONLINE_USER_INFO lpPreOnlineInfo)
{
    CNotifyProtocol::Relogin(m_pRouter->GetGlobalBusi(),lpPreOnlineInfo->uid);
    return 0;
}

int CBusiLogin::SendReply()
{
    return 0;
}


bool CBusiLogin::SaveBuyer(char* szUserName, char* cPass, char* szGuid, uint32& userId, char *szMobileNo, char *szEmail, char *szNickName, char* szHeadImgUrl)
{
	CHttpClient httpClient;
	httpClient.SetDebug(true);
	string saveBuyerUrl(CAppManager::GetInstance().GetConfig().GetImLoggerBaseUrl());
	saveBuyerUrl += "/login/save-buyer";


    char szUserNameUtf8[MAX_USERNAME_LEN + 1] = "";
    char szNickNameUtf8[61]="";

	gbk2utf8(szUserNameUtf8,szUserName);
	gbk2utf8(szNickNameUtf8,szNickName);


	stringstream ssParams;
	ssParams<<"userName=" << szUserNameUtf8;
	ssParams<<"&password=" << cPass;
	ssParams<<"&memGuid=" << szGuid;
	ssParams<<"&mobileNo=" << szMobileNo;
	ssParams<<"&email=" << szEmail;
	ssParams<<"&nickName=" << szNickNameUtf8;
	ssParams<<"&headImgUrl=" << szHeadImgUrl;

	string saveBuyerRespose;
	int httpStaus = httpClient.Post(saveBuyerUrl, ssParams.str(), saveBuyerRespose);

	if(httpStaus == CURLE_OK )	
	{
		Json::Reader jReader(Json::Features::strictMode());
	    Json::Value jvRecvResponse;
	    if (jReader.parse(saveBuyerRespose, jvRecvResponse)){
	    	if(jvRecvResponse.isMember("success") && jvRecvResponse["success"].asString() == "true"){
				if(jvRecvResponse.isMember("data")) {
					if(jvRecvResponse["data"].isMember("userId")){
						userId =jvRecvResponse["data"]["userId"].asUInt();
						return true;
					}
				}
			}
	    }
	}

	userId = 0;
	return false;

}


bool CBusiLogin::SaveSeller(char * account,char* pwd,uint32 id, char* truename)
{
	CHttpClient httpClient;
	
	string saveSellerUrl(CAppManager::GetInstance().GetConfig().GetImLoggerBaseUrl());



    char szUserNameUtf8[MAX_USERNAME_LEN + 1] = "";
    char szNickNameUtf8[61]="";

	gbk2utf8(szUserNameUtf8,account);
	gbk2utf8(szNickNameUtf8,truename);

	saveSellerUrl += "/login/save-seller";

	stringstream ssParams;
	ssParams<<"userName=" << szUserNameUtf8;
	ssParams<<"&password=" << pwd;
	ssParams<<"&userId=" << id;
	ssParams<<"&trueName=" << szNickNameUtf8;
			
	
	string saveBuyerRespose;

	int httpStatus = httpClient.Post(saveSellerUrl, ssParams.str(), saveBuyerRespose);

	return (httpStatus == CURLE_OK ? true : false);	
}
bool CBusiLogin::CheckWork(const char* user, const char* password,int &code,
	string &szUserName,string &szEmail,string &szMobileNumber,string &structure)
{


	CHttpClient httpclient;
	
	string data;
	string sPostParam;
	CConfigInfo config;
	
	//sPostParam += config.GetimapiUrl();
	string url;
	//url += "http://backend-merchant.beta.fn.com:8080/rest/userLoginValidateApi/userLoginValidate";
	url += config.GetWorkURL();
	sPostParam += "param={operator_name=";
	sPostParam += user;
	sPostParam += ",operator_password=";
	sPostParam += password;
	sPostParam += "}";
	int res = httpclient.Post(url,sPostParam,data);

	// 0 为成功
    //判断获取是否成功
    if (res != CURLE_OK)
    {        
		CLog::Log("CheckWork",CLog::TYPE_ERROR,"失败，返回结果为res %d data %s",res,data.c_str());
        
        return false;
    }	
	
    //解析json
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    CLog::Log("CheckWork",CLog::TYPE_DEBUG,"JSON:%s",data.c_str());
    if (reader.parse(data, recv))
    {
        if(!recv.isMember("code") && !recv["code"].isInt())
        {
            return false;
        }
        code = recv["code"].asInt();
        if(code != 200)
        {
            //用户不存在
            CLog::Log("CheckWork",CLog::TYPE_ERROR,"账号或密码错误 code: %d",code);
            return true;
        }
        else 
        {    
			if(recv.isMember("data"))
			{				
				Json::Reader reader1(Json::Features::strictMode());
				string workInfoStr = recv["data"].asString();
				Json::Value workInfo;
				if (reader.parse(workInfoStr,workInfo))
				{
					if(workInfo["operator_realname"].isString())
					{
						szUserName = workInfo["operator_realname"].asString();					
					}
					if(workInfo["email"].isString())
					{				
						szEmail = workInfo["email"].asString();
					}
					if(workInfo["mobile_number"].isString())
					{				
						szMobileNumber = workInfo["mobile_number"].asString();
					}
					if(workInfo["name"].isString())
					{				
						structure = workInfo["name"].asString();
					}	
					return true;
				}
				else
				{
					//json串解析失败
			        CLog::Log("CheckWork", CLog::TYPE_ERROR, "请求%d data:%s ,jsonPraseFail", data.c_str());
			        return false;
				}
			}		
            //找到了子串
            //json串解析失败
	        CLog::Log("CheckWork", CLog::TYPE_ERROR, "请求%d data:%s ,jsonPraseFail", data.c_str());
	        return false;
        }
    }
    else
    {
        //json串解析失败
        CLog::Log("CheckWork", CLog::TYPE_ERROR, "请求%d data:%s ,jsonPraseFail", data.c_str());
        return false;
    }	
	

}

bool CBusiLogin::CheckBuyer(const char* user, const char* password, char* guid,  char * szUserName)
{
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
    //默认使用用户名登陆
    std::string sPostParam = "type=3";
    if (strlen(user) == 11 && user[0] == '1' && atoi(user) != 0)    //使用手机号登陆
    {
        sPostParam = "type=2";
    }
    else if (CheckEmail(user))      //使用Email登陆
    {
        sPostParam = "type=1";
    }

    //加入用户名
    sPostParam += "&username=";
    sPostParam += user;
    //加入密码
    sPostParam += "&password=";
    sPostParam += Base64Encode(password, strlen(password));

    std::string data;
	std::string url = CAppManager::GetInstance().GetConfig().GetBuyerURL();
	CHttpClient httpClient;
	httpClient.SetDebug(true);
	CURLcode res = (CURLcode)httpClient.Post(url,sPostParam,data);
#if 0
    //通过http接口获取买家信息
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, CAppManager::GetInstance().GetConfig().GetBuyerURL());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sPostParam.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);	
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    CURLcode res = curl_easy_perform(curl);

    //清理库接口
    curl_easy_cleanup(curl);
#endif
    //判断获取是否成功
    if (res != CURLE_OK)
    {
        CLog::Log("CheckBuyer",CLog::TYPE_ERROR,"Get buyer infomation error!");
        return false;
    }

    //解析json
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    if (reader.parse(data, recv))
    {
        //解析Header中的Status
        string status = recv["Header"]["Status"].asString();

        //判断是否有子串012002100
        if(status.find("012002100") != std::string::npos)
        {
            string strGuid = recv["Body"]["MEM_GUID"].asString();
            string strUserName = recv["Body"]["MEM_USERNAME"].asString();

            strcpy(guid,strGuid.c_str());
            strcpy(szUserName, strUserName.c_str());
            //找到了子串
            return true;
        }
    }

    //没找到子串
    CLog::Log("CheckBuyer",CLog::TYPE_ERROR," The status didn't have 012002100!");
    return false;
}

#if 1
bool CBusiLogin::CheckBuyerLevel(char* guid,  uint8 &level)
{

    Json::FastWriter writer;
    Json::Value post_val;
    post_val["memGuid"] = guid;

    string sPostParam = "data=";
    sPostParam += writer.write(post_val);

    std::string data;
    CLog::Log("CheckBuyerLevel",CLog::TYPE_IMPORTANT,"Request Level post %s!",sPostParam.c_str());

	std::string url = CAppManager::GetInstance().GetConfig().GetBuyerLevelURL();
	CHttpClient httpClient;
	httpClient.SetDebug(true);
	CURLcode res = (CURLcode)httpClient.Post(url,sPostParam,data);
#if 0
    //通过http接口获取买家信息
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, CAppManager::GetInstance().GetConfig().GetBuyerLevelURL());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sPostParam.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    long timeout = 15; /* seconds */
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);

    CURLcode res = curl_easy_perform(curl);

    //清理库接口
    curl_easy_cleanup(curl);
#endif
    //判断获取是否成功
    if (res != CURLE_OK)
    {
        CLog::Log("CheckBuyer",CLog::TYPE_ERROR,"Get buyer infomation error!");
        return false;
    }

    //解析json
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    if (reader.parse(data, recv))
    {
        if(recv["code"].asUInt() == 100)
        {
            char stackLevel[32] = {0};

            int len = recv["data"]["memLevel"].asString().size();
            if (len > 0)
            {
                strncpy(stackLevel,recv["data"]["memLevel"].asCString(),len > 3 ? 3: len);
            }


            if(stackLevel[0] == 'T')
            {
                switch(stackLevel[1])
                {
                    case '0':
                        level = 0;
                        break;
                    case '1':
                        level = 1;
                        break;
                    case '2':
                        level = 2;
                        break;
                    case '3':
                        level = 3;
                        break;
                }
                CLog::Log("CheckBuyerLevel",CLog::TYPE_IMPORTANT,"Get buyer level %s level %d",stackLevel,level);
                return true;
            }
            else
            {
                CLog::Log("CheckBuyerLevel",CLog::TYPE_ERROR,"Get buyer level %s level %d",stackLevel,level);
                return false;
            }

        }

    }
    CLog::Log("CheckBuyerLevel",CLog::TYPE_ERROR," get level fail!");
    return false;

}
#endif

/*
bool CBusiLogin::GetBuyerInfo(const char* user, char* szMobileNo, char* szEmail, char* szNickName,char* szHeadImgUrl)
{
    std::string sPostParam = "type=3";
    if (strlen(user) == 11 && user[0] == '1' && atoi(user) != 0)
    {
        //使用手机号登陆
        sPostParam = "type=2";
    }
    else if (CheckEmail(user))
    {
        //使用Email登陆
        sPostParam = "type=1";
    }
    //加入用户名
    sPostParam += "&name=";
    sPostParam += user;
    sPostParam += "&returnType=1";
    std::string data;

    //通过http接口获取买家信息
    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, CAppManager::GetInstance().GetConfig().GetBuyerInfoURL());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sPostParam.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    CURLcode res = curl_easy_perform(curl);

    //清理库接口
    curl_easy_cleanup(curl);

    //判断获取是否成功
    if (res != CURLE_OK)
    {
        CLog::Log("GetBuyerInfo",CLog::TYPE_ERROR,"user:%s curlFaild",user);
        return false;
    }

    //解析json
    Json::Reader reader;
    Json::Value recv;
    if (reader.parse(data, recv))
    {
        //解析Header中的Status
        string status = recv["Header"]["Status"].asString();
        //判断是否有子串012001100
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
            //找到了子串
            return true;
        }
        else
        {
            //用户不存在
            CLog::Log("GetBuyerInfo",CLog::TYPE_ERROR,"user:%s, notExist", user);
            return false;

        }
    }
    else
    {
        //json串解析失败
        CLog::Log(__FUNCTION__, CLog::TYPE_ERROR, "user:%s, data:%s ,jsonPraseFail", user, data.c_str());
        return false;

    }

}
*/

bool CBusiLogin::GetBuyerInfo(const char* guid, char* szMobileNo, char* szEmail, char* szNickName,char* szHeadImgUrl)
{
    if(strcmp(guid, "") == 0)
    {
        CLog::Log("GetBuyerInfo",CLog::TYPE_ERROR,"guid为空");
        return false;
    }

    std::string sPostParam = "type=5";

    //加入用户名
    sPostParam += "&name=";
    sPostParam += guid;
    sPostParam += "&returnType=1";
    std::string data;

	std::string url = CAppManager::GetInstance().GetConfig().GetBuyerInfoURL();
	CHttpClient httpClient;
	httpClient.SetDebug(true);
	CURLcode res = (CURLcode)httpClient.Post(url,sPostParam,data);
#if 0
    //通过http接口获取买家信息
    CURL *curl = curl_easy_init();
    string strHttp = CAppManager::GetInstance().GetConfig().GetBuyerInfoURL();
    CLog::Log("GetBuyerInfo",CLog::TYPE_DEBUG,"http:%s, param:%s", strHttp.c_str(), sPostParam.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, strHttp.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, sPostParam.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    CURLcode res = curl_easy_perform(curl);

    //清理库接口
    curl_easy_cleanup(curl);
#endif
    //判断获取是否成功
    if (res != CURLE_OK)
    {
        CLog::Log("GetBuyerInfo",CLog::TYPE_ERROR,"guid:%s curlFaild", guid);
        return false;
    }

	// utf8转gbk
    char szDataGbk[4096] = {0};
    utf2gbk((char*)data.c_str(),data.length(), szDataGbk, 4095);

    //解析json
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    CLog::Log("GetBuyerInfo",CLog::TYPE_DEBUG,"JSON:%s",szDataGbk);
    if (reader.parse(szDataGbk, recv))
    {
        if(!recv.isMember("msg"))
        {
            return false;
        }
        string strMsg = recv["msg"].asString();
        if(strMsg != "success")
        {
            //用户不存在
            CLog::Log("GetBuyerInfo",CLog::TYPE_ERROR,"guid:%s, notExist", guid);
            return false;
        }
        else
        {
            string strMobileNo= recv["data"]["MEM_CELL_PHONE"].asString();
            string strEmail = recv["data"]["MEM_EMAIL"].asString();
            string strNickName =recv["data"]["NICKNAME"].asString();
            string strHeadImgFile =recv["data"]["HEAD_PORTRAIT"].asString();

            strncpy(szMobileNo, strMobileNo.c_str(), MAX_PHONE_LEN);
            strncpy(szEmail, strEmail.c_str(), MAX_EMAIL_LEN);
            strncpy(szNickName, strNickName.c_str(), 61);

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
            CLog::Log("GetBuyerInfo",CLog::TYPE_DEBUG,"guid:%s, mobileNo:%s, email:%s, nickName:%s, szHeadImgFile:%s",
                      guid, szMobileNo, szEmail, szNickName, szHeadImgUrl);
            //找到了子串
            return true;
        }
    }
    else
    {
        //json串解析失败
        CLog::Log(__FUNCTION__, CLog::TYPE_ERROR, "guid:%s, data:%s ,jsonPraseFail", guid, szDataGbk);
        return false;
    }
}


string CBusiLogin::TransPwd(const char* pwd)
{
//  CLog::Log("CBusiLogin::TranPwd", CLog::TYPE_DEBUG, "PWD:%s",pwd);

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

//  CLog::Log("CBusiLogin::TranPwd", CLog::TYPE_DEBUG, "Tran PWD:%s",UrlPwd.c_str());

    return UrlPwd;
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





bool CBusiLogin::CheckSeller(char* user, const char* pwd, uint32_t& uid, uint32_t& merchant_id, int &state, string &code)
{
    // 客户端ip
    RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    sockaddr_in p;
    p.sin_addr.s_addr=pRecvData->src_ip;
    char szClientIP[64] = {0};
    strcpy( szClientIP, (char*)inet_ntoa(p.sin_addr));

    // 服务端ip
    char szServerIP[64] = {0};
    CAppManager::GetInstance().GetConfig().GetDeviceIp(szServerIP);

    //特殊符号转义
    merchant_id = 0;
    uid = 0;
    string password = TransPwd(pwd);
    if(password.empty())
    {
        CLog::Log("CBusiLogin::CheckSeller", CLog::TYPE_ERROR, "password:%s trans is NULL",pwd);
        return false;
    }

    //初始化http　get请求
    //params={"loginName":"zhangkai01","password":"pass1234","clientIp":"127.0.0.1","localIp":"10.212.192.81","channelFrom":"moumou","channel":"1","version":"v1.1"}
    char szParam[1024] = {0};
    sprintf_s(szParam, "params={\"loginName\":\"%s\",\"password\":\"%s\",\"clientIp\":\"%s\",\"localIp\":\"%s\",\"channelFrom\":\"moumou\",\"channel\":\"1\",\"version\":\"v1.1\"}",
              user, password.c_str(), szClientIP, szServerIP);
    CLog::Log("CBusiLogin",CLog::TYPE_DEBUG, "Login param:%s", szParam);

    // 转换为utf8编码
    char szParamU[1024] = {0};
    gbk2utf8(szParamU,szParam);
	std::string sPostParam = szParamU;
    std::string data;
	std::string url = CAppManager::GetInstance().GetConfig().GetSellerURL();
	CHttpClient httpClient;
	httpClient.SetDebug(true);
	CURLcode res =(CURLcode) httpClient.Post(url,sPostParam,data);
#if 0	

    //通过HTTP接口获取卖家信息
    CURL *curl = curl_easy_init();
    if(curl == NULL)
    {
        CLog::Log("CBusiLogin", CLog::TYPE_ERROR, "curl_easy_init error");
    }
    curl_easy_setopt(curl, CURLOPT_URL, CAppManager::GetInstance().GetConfig().GetSellerURL());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szParamU);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
    CURLcode res = curl_easy_perform(curl);

    //清理库内存
    curl_easy_cleanup(curl);
#endif
    //判断是否获取成
    if (res != CURLE_OK)
    {
        CLog::Log("CheckSeller", CLog::TYPE_ERROR, "Get seller infomation error!");
        return false;
    }

    // utf8转gbk
    char szDataGbk[4096] = {0};
    utf2gbk((char*)data.c_str(),data.length(), szDataGbk, 4095);

    //解析json字符创
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    CLog::Log("CheckSeller",CLog::TYPE_DEBUG,"user:%s, data:%s",user, szDataGbk);
    if (reader.parse(szDataGbk, recv))
    {
        //获取code字段
        if(recv.isMember("code"))
        {
            code = recv["code"].asString();
        }
        else
        {
            CLog::Log("CheckSeller", CLog::TYPE_ERROR, "Get seller infomation error! no auth field");
        }

        // 继续解析
        if(!recv.isMember("auth"))
        {
            CLog::Log("CheckSeller", CLog::TYPE_ERROR, "Get seller infomation error! no auth field");
        }
        else
        {
            Json::Value jAuth = recv["auth"];
            //获取auth中的memberId字段
            if(jAuth.isMember("memberId"))
            {
                uid = jAuth["memberId"].asInt();
            }
            //获取auth中的merchantId字段
            if(jAuth.isMember("merchantId"))
            {
                merchant_id = jAuth["merchantId"].asInt();
            }
            //username
            if(jAuth.isMember("username"))
            {
                string username = jAuth["username"].asString();
                strncpy(user, username.c_str(), MAX_USERNAME_LEN);
            }
            // state
            if(jAuth.isMember("state"))
            {
                state = jAuth["state"].asInt();
            }
        }

        return true;
    }

    return false;
}

// 检查用户是否同地登录		返回值 1 通地登录 0 异地登录  -1 调用失败，调用失败时，会返回错误信息
int CBusiLogin::IsSameAreaLogin(const char* loginname, string &strError)
{
	strError = "用户名或密码错误";

    // 客户端ip
    RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    sockaddr_in p;
    p.sin_addr.s_addr=pRecvData->src_ip;
    char szClientIP[64] = {0};
    strcpy( szClientIP, (char*)inet_ntoa(p.sin_addr));

    //初始化http　get请求
    char szParam[1024] = {0};
    sprintf_s(szParam, "params={\"loginName\":\"%s\",\"clientIp\":\"%s\"}",
              loginname, szClientIP);
    CLog::Log("IsSameAreaLogin",CLog::TYPE_IMPORTANT, "param:%s", szParam);

    // 转换为utf8编码
    char szParamU[1024] = {0};
    gbk2utf8(szParamU,szParam);

    std::string data;
	std::string sPostParam = szParamU;
	std::string url = CAppManager::GetInstance().GetConfig().GetUrlIsSameAreaLogin();
	CHttpClient httpClient;
	httpClient.SetDebug(true);
	CURLcode res = (CURLcode) httpClient.Post(url,sPostParam,data);	
#if 0
    //通过HTTP接口获取卖家信息
    CURL *curl = curl_easy_init();
    if(curl == NULL)
    {
        CLog::Log("IsSameAreaLogin", CLog::TYPE_ERROR, "curl_easy_init error");
        return -1;
    }
    curl_easy_setopt(curl, CURLOPT_URL, CAppManager::GetInstance().GetConfig().GetUrlIsSameAreaLogin());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szParamU);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
    CURLcode res = curl_easy_perform(curl);

    //清理库内存
    curl_easy_cleanup(curl);
#endif
    //判断是否获取成
    if (res != CURLE_OK)
    {
        CLog::Log("IsSameAreaLogin", CLog::TYPE_ERROR, "Get seller infomation error!");
        return -1;
    }

    // utf8转gbk
    char szDataGbk[4096] = {0};
    utf2gbk((char*)data.c_str(),data.length(), szDataGbk, 4095);

    //解析json字符创
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    CLog::Log("IsSameAreaLogin",CLog::TYPE_IMPORTANT,"user:%s, data:%s",loginname, szDataGbk);
    if (reader.parse(szDataGbk, recv))
    {
        //获取code字段
        string code;
        if(recv.isMember("code"))
        {
            code = recv["code"].asString();

            if(code != "200")
            {
            	return -1;
            }
        }
        else
        {
            return -1;
        }

        // 继续解析
        string data;
        if(recv.isMember("data"))
        {
            data = recv["data"].asString();
        }
        else
        {
            CLog::Log("IsSameAreaLogin", CLog::TYPE_ERROR, "Get seller infomation error! no data field");
            return -1;
        }

        if(data == "true")
        {
            CLog::Log("IsSameAreaLogin", CLog::TYPE_IMPORTANT, "同一地址登录");
            strError = "";
            return 1;
        }

        CLog::Log("IsSameAreaLogin", CLog::TYPE_IMPORTANT, "异地登录");
        strError = "";
        return 0;
    }

    CLog::Log("IsSameAreaLogin", CLog::TYPE_ERROR, "json解析失败");
    return -1;
}

// 检查用户是否绑定手机		返回值 1 绑定了手机  0 未绑定手机  -1 调用失败
int CBusiLogin::IsBindMobile(const char* loginname, string &strError)
{
	strError = "用户名或密码错误";
    //初始化http　get请求
    char szParam[1024] = {0};
    sprintf_s(szParam, "params={\"loginName\":\"%s\"}",
              loginname);
    CLog::Log("IsBindMobile",CLog::TYPE_IMPORTANT, "param:%s", szParam);

    // 转换为utf8编码
    char szParamU[1024] = {0};
    gbk2utf8(szParamU,szParam);

    std::string data;
	std::string sPostParam = szParamU;

	std::string url = CAppManager::GetInstance().GetConfig().GetUrlIsBindMobile();
	CHttpClient httpClient;
	httpClient.SetDebug(true);
	CURLcode res = (CURLcode) httpClient.Post(url,sPostParam,data);	
	
#if 0
    //通过HTTP接口获取卖家信息
    CURL *curl = curl_easy_init();
    if(curl == NULL)
    {
        CLog::Log("IsBindMobile", CLog::TYPE_ERROR, "curl_easy_init error");
        return -1;
    }
    curl_easy_setopt(curl, CURLOPT_URL, CAppManager::GetInstance().GetConfig().GetUrlIsBindMobile());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, szParamU);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
    curl_easy_setopt(curl, CURLOPT_POST, 1);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1);
    curl_easy_setopt(curl, CURLOPT_FORBID_REUSE, 1);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 3);
    CURLcode res = curl_easy_perform(curl);

    //清理库内存
    curl_easy_cleanup(curl);
#endif
    //判断是否获取成
    if (res != CURLE_OK)
    {
        CLog::Log("IsBindMobile", CLog::TYPE_ERROR, "Get seller infomation error!");
        return -1;
    }

    // utf8转gbk
    char szDataGbk[4096] = {0};
    utf2gbk((char*)data.c_str(),data.length(), szDataGbk, 4095);

    //解析json字符创
    Json::Reader reader(Json::Features::strictMode());
    Json::Value recv;
    CLog::Log("IsBindMobile",CLog::TYPE_IMPORTANT,"user:%s, data:%s",loginname, szDataGbk);
    if (reader.parse(szDataGbk, recv))
    {
        //获取code字段
        string code;
        if(recv.isMember("code"))
        {
            code = recv["code"].asString();

            if(code != "200")
            {
            	return -1;
            }
        }
        else
        {
            CLog::Log("IsBindMobile", CLog::TYPE_ERROR, "Get seller infomation error! no code field");
            return -1;
        }

        // 继续解析
        string data;
        if(recv.isMember("data"))
        {
            data = recv["data"].asString();
        }
        else
        {
            CLog::Log("IsBindMobile", CLog::TYPE_ERROR, "Get seller infomation error! no data field");
            return -1;
        }

        if(data == "true")
        {
            CLog::Log("IsBindMobile", CLog::TYPE_IMPORTANT, "绑定手机");
            strError = "";
            return 1;
        }

        CLog::Log("IsBindMobile", CLog::TYPE_IMPORTANT, "未绑定手机");
        strError = "";
        return 0;
    }

    CLog::Log("IsBindMobile", CLog::TYPE_ERROR, "json解析失败");
    return -1;
}

// AD认证接口
bool CBusiLogin::AD_Auth(LPCTSTR szUserName, LPCTSTR szPwd, string &strErrorInfo)
{
    // 初始化
    LDAP *pLdap = ldap_init((char *)AD_HOSTNAME, (int)AD_PORT);
    if(pLdap == NULL)
    {
        strErrorInfo = "连接AD认证服务器失败！";
        return FALSE;
    }

    // 连接AD认证服务器
    /*
        unsigned long  rlt = ldap_connect(pLdap, NULL);
        if(LDAP_SUCCESS != rlt)
        {
            ldap_unbind_s(pLdap);
            strErrorInfo = "连接AD认证服务器失败！";
            return FALSE;
        }
    */
    // 认证
    string strUser("FN\\");
    //strUser.Format("FN\\%s", szUserName);
    strUser += string(szUserName);
    int rlt = ldap_simple_bind_s(pLdap, (char *)strUser.c_str(), (char *)szPwd);
    if(LDAP_SUCCESS != rlt)
    {
        ldap_unbind_s(pLdap);
        strErrorInfo = "认证失败，用户名或密码错误！";
        return FALSE;
    }

    ldap_unbind_s(pLdap);
    strErrorInfo = "认证成功！";
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
    XT_LOGOUT * lpReq = (XT_LOGOUT *)m_RecvPacket.GetData();

    CLog::Log("CBusiLogout",CLog::TYPE_IMPORTANT,"Id:%d auth %#x IN",lpReq->id,lpReq->authData);

    if(lpReq->authData != 0x00  && lpReq->authData != 0xAA && lpReq->authData != 0x01 && lpReq->authData != 0x02)
    {
        CLog::Log("CBusiLogout",CLog::TYPE_ERROR,"Id:%d,接收到一个错误登出包:%d",lpReq->id,lpReq->authData);
        return 1;
    }
	
    CUserManager *pUserManager = m_pRouter->GetUserManager();
    shared_ptr<CUser> pUser = pUserManager->Find(lpReq->id, TRUE);
#ifndef IM_USE_IMAPI

    //为客服查询一个上下文
    CSManager & csManager = CAppManager::GetInstance().GetCSManager();
    ClientService * clientService = csManager.Find(lpReq->id);
#endif


	
    if(pUser != NULL)
    {
    	
        if(lpReq->authData == 0xAA)
        {
            CNotifyProtocol::Relogin(m_pRouter->GetGlobalBusi(),lpReq->id);
            CLog::Log("CBusiLogout",CLog::TYPE_IMPORTANT,"登出其他server分布式重登录用户 %u",lpReq->id);

        }
        if(m_pRecvHead->flag == XT_HEAD::FLAG_MOBREQ)
        {        	
            WebLogout(pUser.get(),lpReq->authData);
			
        }
        else
        {			
#ifdef IM_USE_IMAPI
			Logout(pUser.get(),lpReq->authData);
#else
			Logout(pUser.get(),clientService,lpReq->authData);
#endif
        }
    }


    CLog::Log("CBusiLogout", CLog::TYPE_IMPORTANT, "sid:%u, did:%x, flag:%u, authData:%u OUT" , m_pRecvHead->sid, m_pRecvHead->did,m_pRecvHead->flag, lpReq->authData);

    return 0;
}

int CBusiLogout::SendReply()
{
    return 0;
}
#ifdef IM_USE_IMAPI

void CBusiLogout::Logout(CUser * pUser,uint8 type)
{
    if(!pUser)
        return ;

    pUser->m_nStatus = XTREAM_EXIT;

    //更新数据库在线表状态（因为要尽快通知，所以在这里提前处理）
    m_pRouter->GetDataBaseMini()->Save_User_Logout(pUser->m_nID);

    //用户状态改变通知其他好友
    pUser->NotifyFriendStatus();
    pUser->NotifyFamilyStatus(m_pAppNetwork,m_pRouter->GetGlobalFamilyBusi(),m_pRouter->GetDataBaseMini());//modify by lwl ver:0105

    //向全局注销该用户信息
    uint32  uid = pUser->m_nID;
    uint8   count = 1;

    pUser->LogoutTempFamily(m_pAppNetwork, m_pRouter->GetGlobalFamilyBusi());

    pUser->m_PermFamilyMap.Lock();
    FamilyList_Type::iterator itf = pUser->m_PermFamilyMap.begin();
    for(; itf != pUser->m_PermFamilyMap.end(); ++itf)
    {
        //保存固定群消息id
        m_pRouter->GetDataBaseMini()->Save_User_RecvFamilyMsgID(pUser->m_nID, (*itf).first, (*itf).second);
    }
    pUser->m_PermFamilyMap.UnLock();

    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USEROPERATION,pUser->m_nOperationID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERRECVMSG,pUser->m_nUserMsgID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::SYSTEMPUB,pUser->m_nSysMsgID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERPUB,pUser->m_nMobileMsgID);//代表已接收短信号
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERCOMMENT,pUser->m_nUsercommentID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::LOGCOMMENT,pUser->m_nLogcommentID);

    //保存用户在线时长
    int money;
    time_t tm;
    time(&tm);
    m_pRouter->GetDataBaseMini()->Save_User_OnlineTime(pUser->m_nID, pUser->m_nLoginTime, (int)tm, money);
    m_pRouter->GetDataBaseMini()->Save_User_Timelen(pUser->m_nID, pUser->m_nLoginTime, (int)tm);
    if(pUser->m_nID < Id_Boundary || pUser->m_nID > cs_Boundary)
    {

        m_pRouter->GetDataBaseMini()->Save_Merchant_Logout(pUser->m_nID,(int)tm);

        //上传登出动作
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
                if(userFlow[0].status != XTREAM_OFFLINE && userFlow[0].flow == 1) //防止用户异常退出，没退出挂起状态
                {
                    SubMerchantAction info;
                    if(pUser->m_nID > cs_Boundary)
                    {
                        info.merchantID = -1;
                    }
                    else
                    {
                        info.merchantID = pUser->m_merchantid;
                    }
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
            if(pUser->m_nID > cs_Boundary)
            {
                info.merchantID = -1;
            }
            else
            {
                info.merchantID = pUser->m_merchantid;
            }
            //info.merchantID = pUser->m_merchantid;
            info.subMerchantID = pUser->m_nID;
            info.actionTime = (int)tm;
            info.actionType = ACTION_LOGOUT;

            ResultInfo res = hess.GetHessiani()->uploadAction(info);

            CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
                      info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
        }

        catch (std::exception& e)
        {
            CLog::Log("hessian",CLog::TYPE_ERROR,"merchant_id:%d, subMerchantID:%d, other exception occured: %s",
                      pUser->m_merchantid, pUser->m_nID, e.what());
        }

        CThreadMsgNotify::DeleteSellerData(pUser->m_nID);
#ifdef IM_USE_IMAPI
		//  4 : 客服下线，service_id必填。

		imapi csOfflineReq;
		string info;
		csOfflineReq.imapiReq(imapi::CSOFFLINEREQ,info,pUser->m_nID);
#endif
		
    }


    if (money > 0 && pUser->m_nID != 0)
    {
        if (money > 5)
            money = 5;
        m_pRouter->GetDataBaseWeb()->Update_User_Money(pUser->m_nID, money);
        //给用户发送冲值消息
        CLog::Log("Money", CLog::TYPE_IMPORTANT, "用户:%d赠送积分:%d", pUser->m_nID, money*5);
    }

    //回收用户内存
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
    CLog::Log("Logout",CLog::TYPE_DEBUG,"uid:%d, count:%d, type:%d ",uid,count,type);
    if(type != 0xAA)
    {
        m_pRouter->GetGlobalBusi()->DelUserInfo(&uid,count);
    }
    pUser = NULL;

    SysStat::GetInstance().rmOnlineNum();

}

#else

void CBusiLogout::Logout(CUser * pUser,ClientService * clientService,uint8 type)
{
    if(!pUser)
        return ;

    pUser->m_nStatus = XTREAM_EXIT;

    //更新数据库在线表状态（因为要尽快通知，所以在这里提前处理）
    m_pRouter->GetDataBaseMini()->Save_User_Logout(pUser->m_nID);

    //用户状态改变通知其他好友
    pUser->NotifyFriendStatus(m_pRouter->GetGlobalBusi());
    pUser->NotifyFamilyStatus(m_pAppNetwork,m_pRouter->GetGlobalFamilyBusi(),m_pRouter->GetDataBaseMini());//modify by lwl ver:0105

    //向全局注销该用户信息
    uint32  uid = pUser->m_nID;
    uint8   count = 1;

    pUser->LogoutTempFamily(m_pAppNetwork, m_pRouter->GetGlobalFamilyBusi(), m_pRouter->GetGlobalBusi());

    pUser->m_PermFamilyMap.Lock();
    FamilyList_Type::iterator itf = pUser->m_PermFamilyMap.begin();
    for(; itf != pUser->m_PermFamilyMap.end(); ++itf)
    {
        //保存固定群消息id
        m_pRouter->GetDataBaseMini()->Save_User_RecvFamilyMsgID(pUser->m_nID, (*itf).first, (*itf).second);
    }
    pUser->m_PermFamilyMap.UnLock();

    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USEROPERATION,pUser->m_nOperationID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERRECVMSG,pUser->m_nUserMsgID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::SYSTEMPUB,pUser->m_nSysMsgID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERPUB,pUser->m_nMobileMsgID);//代表已接收短信号
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERCOMMENT,pUser->m_nUsercommentID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::LOGCOMMENT,pUser->m_nLogcommentID);

    //保存用户在线时长
    int money;
    time_t tm;
    time(&tm);
    m_pRouter->GetDataBaseMini()->Save_User_OnlineTime(pUser->m_nID, pUser->m_nLoginTime, (int)tm, money);
    m_pRouter->GetDataBaseMini()->Save_User_Timelen(pUser->m_nID, pUser->m_nLoginTime, (int)tm);
    if(pUser->m_nID < Id_Boundary || pUser->m_nID > cs_Boundary)
    {

        m_pRouter->GetDataBaseMini()->Save_Merchant_Logout(pUser->m_nID,(int)tm);

        //上传登出动作
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
                if(userFlow[0].status != XTREAM_OFFLINE && userFlow[0].flow == 1) //防止用户异常退出，没退出挂起状态
                {
                    SubMerchantAction info;
                    if(pUser->m_nID > cs_Boundary)
                    {
                        info.merchantID = -1;
                    }
                    else
                    {
                        info.merchantID = pUser->m_merchantid;
                    }
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
            if(pUser->m_nID > cs_Boundary)
            {
                info.merchantID = -1;
            }
            else
            {
                info.merchantID = pUser->m_merchantid;
            }
            //info.merchantID = pUser->m_merchantid;
            info.subMerchantID = pUser->m_nID;
            info.actionTime = (int)tm;
            info.actionType = ACTION_LOGOUT;

            ResultInfo res = hess.GetHessiani()->uploadAction(info);

            CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
                      info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
        }

        catch (std::exception& e)
        {
            CLog::Log("hessian",CLog::TYPE_ERROR,"merchant_id:%d, subMerchantID:%d, other exception occured: %s",
                      pUser->m_merchantid, pUser->m_nID, e.what());
        }

        CThreadMsgNotify::DeleteSellerData(pUser->m_nID);
#ifdef IM_USE_IMAPI
		//  4 : 客服下线，service_id必填。

		imapi csOfflineReq;
		string info;
		csOfflineReq.imapiReq(imapi::CSOFFLINEREQ,info,pUser->m_nID);
#endif
		
    }


    if (money > 0 && pUser->m_nID != 0)
    {
        if (money > 5)
            money = 5;
        m_pRouter->GetDataBaseWeb()->Update_User_Money(pUser->m_nID, money);
        //给用户发送冲值消息
        CLog::Log("Money", CLog::TYPE_IMPORTANT, "用户:%d赠送积分:%d", pUser->m_nID, money*5);
    }

    //回收用户内存
    pUser->m_FriendList.Clear();
    pUser->m_PermFamilyMap.Lock();
    pUser->m_PermFamilyMap.clear();
    pUser->m_PermFamilyMap.UnLock();
    pUser->m_TempFamilyList.Lock();
    pUser->m_TempFamilyList.clear();
    pUser->m_TempFamilyList.UnLock();

    CUserManager *pUserManager = m_pRouter->GetUserManager();
//    pUser->DelFromOnlineList();
    pUserManager->Del(pUser->m_nID);
#ifdef ACCESS_SERVER    
    m_pAppNetwork->UserLogout(pUser->m_nID);
#endif    
    CLog::Log("Logout",CLog::TYPE_DEBUG,"uid:%d, count:%d, type:%d ",uid,count,type);
    if(type != 0xAA)
    {
        m_pRouter->GetGlobalBusi()->DelUserInfo(&uid,count);
    }
    pUser = NULL;

    SysStat::GetInstance().rmOnlineNum();


    if(clientService != NULL)
    {
        //清空客服上下文
        //清除全局服务器上下文
        //duyong

        CSManager& csManager = CAppManager::GetInstance().GetCSManager();
        csManager.DelHash(clientService->getCSID());

#ifdef IM_USE_CACHE
        if(type != 0xAA)
        {
            m_pRouter->GetGlobalBusi()->DelCSInfo(&uid,count);
        }

#endif
        clientService = NULL;
    }


}
#endif
//#define MAX_NUM 20
//判断是否需要删除全局服务器信息
//如果是其他服务器发过来的0xaa请求就不删除，只等对方服务器更新即可
//以免造成先写入，后删除的BUG
//

void CBusiLogout::ClearCSContext(uint32 selfID,uint32 teamid,uint32 toid)
{
    //查询全局服务器并清除web所在服务器其接待上下文
    CLog::Log("CBusiLogout::ClearCSContext",CLog::TYPE_DEBUG,"准备发送到指定服务器清除客服%u接待",toid);
    if(toid == 0)
    {
        return;
    }
    tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo UserInfo;
    uint8 count = 1;
    m_pRouter->GetGlobalBusi()->QueryUserControlInfo(&toid,&UserInfo,count);

    if(UserInfo.uid != toid)
    {
        CLog::Log("CBusiLogout::ClearCSContext",CLog::TYPE_DEBUG,"获取客服%d信息失败,清空其接待状态可能失败",toid);
        return;
    }
    else
    {
        SendDataStruct sendData;
        LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
        char* pMsgData = sendData.data + sizeof(XT_HEAD);
        CSendPacket SendPacket;
        SendPacket.SetData(pMsgData,MAX_PROTOLPACKET_LEN);
        SendPacket << (uint32)0
                   << selfID
                   << toid
                   << teamid;
        sendData.data_len = (uint16)sizeof(XT_HEAD)  + SendPacket.GetSize();
        CAppManager::GetInstance().InitHead(
            lpSendHead,
            CMD_SET_CS_CONSULT_REQ,
            (uint16)(SendPacket.GetSize()),
            1,XT_HEAD::DEST_TYPE_SERVER);
        CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
            sendData.data,sendData.data_len,0,0,UserInfo.srvip, PORT_IM_SVRP2P);
        CLog::Log("CBusiLogout::ClearCSContext",CLog::TYPE_IMPORTANT,"通知服务器 %u 去掉客服 %u中的接待信息 !",UserInfo.srvip,toid);
    }
}

void CBusiLogout::SendWebQueueInfoIND(CSendPacket& m_SendPacket)
{
    XT_HEAD* pHead = (XT_HEAD*)pData->data;

    pHead->flag = XT_HEAD::FLAG_MOBSRVACK;
    pHead->from = XT_HEAD::FROM_SERVER;
    pHead->seq  = CAppManager::GetInstance().GetSendSeq();
    pHead->cmd  = CMD_QUEUE_LENGTH_IND;
    pHead->sid  = m_pRecvHead->did;
    pHead->dtype = XT_HEAD::DEST_TYPE_USER;
    pHead->did  = m_pRecvHead->sid;
    pHead->len = m_SendPacket.GetSize();
    pData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));

    pData->retry_times = SEND_RETRY_TIME;
    pData->time_interval = 0;

    memcpy(pData->data+sizeof(XT_HEAD), m_SendPacket.GetData(), m_SendPacket.GetSize());

    CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),pData->data,pData->data_len,SEND_RETRY_TIME);

}

void CBusiLogout::WebLogout(CUser * pUser,uint8 type)
{
	if(type == 0x00)
	{	
		CLog::Log("WebLogout",CLog::TYPE_DEBUG,"lock uid:%d,type:%d ",pUser->m_nID,type);
		pUser->m_isLogout = 1;	//加入用户锁
		
	}
	
    const uint8 MAX_NUM = 20;
    if(pUser == NULL)
        return ;
    uint32 CSuserid = pUser->getCsuserId();
    uint32 teamid = pUser->getCsTeamId();
#ifndef IM_USE_IMAPI
    if(teamid != 0 && CSuserid == 0)   //未被接待，则用户在排队
    {
        CLog::Log("CBusiLogout::WebLogout", CLog::TYPE_ERROR, "web用户%u未被接待，不用清除接待上下文 !",pUser->m_nID);

        // 需要删除排队列表中的买家

        tagGSRV_ADDMODIFY_CSQINFO_QUERY::CSinfo cSinfo;
        cSinfo.customerID = pUser->m_nID;
        cSinfo.teamID = teamid;
        uint8 count = 1;

        CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT,"从全局memcached删除排队用户 %u!",cSinfo.customerID);
        uint8 ret = m_pRouter->GetGlobalBusi()->AddDelCSQueueInfo(&cSinfo,count,tagGSRV_ADDMODIFY_CSQINFO_QUERY::ACTION_DEL);
        if(ret != MEMCACHED_GET_INFO_SUCCESS)
        {
            CLog::Log("CBusiLogout",CLog::TYPE_ERROR,"ACTION_DEL user %u failed!");
        }
        // 并通知相关客服
        vector<uint32> TeamUser;
        CDataBaseMini *pMinibase = m_pRouter->GetDataBaseMini();
        if( DB_SUCCESS != pMinibase->CS_Get_Group_Account(teamid,TeamUser))
        {
            CLog::Log("CBusiLogout::WebLogout", CLog::TYPE_ERROR, "CS_Get_Group_Account %u failed!", teamid);
        }
        for(unsigned int i = 0 ; i < TeamUser.size(); ++i )
        {

            CLog::Log("CBusiLogout::WebLogout", CLog::TYPE_IMPORTANT,"内部队列:TEAM %d 查询队列: TEAM %d ,发送给 %u",
                      teamid,teamid,TeamUser[i]);
            //通知所有组内的客服
            {
                SendDataStruct sendData;
                LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
                lpSendHead->cmd      = CMD_SERVERMSG_IND;
                lpSendHead->did      = TeamUser[i];
                lpSendHead->dtype    = XT_HEAD::DEST_TYPE_USER;
                lpSendHead->flag     = XT_HEAD::FLAG_SRVACTION;
                lpSendHead->from     = XT_HEAD::FROM_SERVER;
                CAppManager::GetInstance().GetSendSeq(lpSendHead->seq);
                lpSendHead->sid      = CAppManager::GetInstance().GetConfig().GetServerNo();
                lpSendHead->len		 = sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_WAITQUEU_CHANGE);

                LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
                lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
                lpSrvTransmit->msgtype              = CMD_WAITQUEU_CHANGE;


                XT_WAITQUEU_CHANGE* pTeamMatesNotify = (XT_WAITQUEU_CHANGE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) );
                pTeamMatesNotify->bAdd    = XT_WAITQUEU_CHANGE::TYPE_DELETE;
                pTeamMatesNotify->idBuyer = pUser->m_nID;
                pTeamMatesNotify->teamId = teamid;
                strcpy(pTeamMatesNotify->nicknameBuyer,pUser->m_szNickName);
                pTeamMatesNotify->QueueLength = 0;  //每个客服的队列长度是不同的，推送的时候是增量更新
                //pFriendStatusNotify->status = m_nStatus;

                sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_WAITQUEU_CHANGE);
                CLog::Log("CBusiLogout::WebLogout", CLog::TYPE_IMPORTANT,"准备发送");

                CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
                    sendData.data,sendData.data_len,SEND_RETRY_TIME);

            }
        }
    }
    //会先判断teamid 是否为0 区分是否是客服用户

    ClientService * clientService = NULL;
    CSManager & csManager = CAppManager::GetInstance().GetCSManager();
    if(teamid != 0 && CSuserid != 0)
    {
        //为客服创建一个上下文
        clientService= csManager.Find(CSuserid);
        if(!clientService)
        {
            ClearCSContext(pUser->m_nID,pUser->getCsTeamId(),CSuserid);
        }
        else//客服在本地，则清除本地上下文之后，同步全局服务器
        {
            clientService->RemoveCustomer_x(pUser->m_nID);
            CMemcacheUpdateConsultCSInfo(m_pRouter,clientService);
        }

        // 通知此买家的客服，买家下线
		if(type != 0x02)
		{
            SendDataStruct sendData;
            LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
            lpSendHead->cmd      = CMD_SERVERMSG_IND;
            lpSendHead->did      = pUser->m_nCSuserid;
            lpSendHead->dtype    = XT_HEAD::DEST_TYPE_USER;
            lpSendHead->flag     = XT_HEAD::FLAG_SRVACTION;
            lpSendHead->from     = XT_HEAD::FROM_SERVER;
            CAppManager::GetInstance().GetSendSeq(lpSendHead->seq);
            lpSendHead->sid      = CAppManager::GetInstance().GetConfig().GetServerNo();

            LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
            lpSrvTransmit->origin_id            = XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
            lpSrvTransmit->msgtype              = CMD_BUYER_OFFLINE_IND;

            UserOfflineIND ind;
            ind.set_userid(pUser->m_nID);

            uint32_t msg_size = ind.ByteSize();
            char* szData = new char[msg_size];
            if (!ind.SerializeToArray(szData, msg_size))
            {
                CLog::Log("WebLogout", CLog::TYPE_IMPORTANT, "发送买家退出消息时序列号失败");
            }
            else
            {
                memcpy(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT), szData, msg_size);
                sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + msg_size;

                lpSendHead->len		 = sizeof(XT_SERVER_TRANSMIT) + msg_size;

                CLog::Log("WebLogout", CLog::TYPE_IMPORTANT,"发送买家[%u]退出消息给客服[%u]", pUser->m_nID, pUser->m_nCSuserid);
                CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
                    sendData.data,sendData.data_len,SEND_RETRY_TIME);
            }
            delete[] szData;
            szData = NULL;
        }
    }

    pUser->ClearWebClient();
    //return;
#endif
    if(type == 0x01 || type==0x02 ) //清除WEB用户信息,只做上面的操作
    {
#ifdef IM_USE_IMAPI
		// 1：web用户停止排队，该用户正在排队，customer_id和group_Id必填。

		imapi stopQueueReq;
		string info;
		stopQueueReq.imapiReq(imapi::STOPQUEUE,info,0,pUser->m_nID,pUser->getCsTeamId());
#else	
        PushQueueSubAccountToKafka(m_pRouter,teamid);   //关闭窗口的时候，才推送新的分流信息
#endif
        return;
    }


    pUser->m_nStatus = XTREAM_EXIT;

    //更新数据库在线表状态（因为要尽快通知，所以在这里提前处理）
    m_pRouter->GetDataBaseMini()->Save_User_Logout(pUser->m_nID);

    //用户状态改变通知其他好友
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
#ifdef IM_USE_CACHE
        if (MEMCACHED_NO_INFO < CMemcacheQueryUserStatusi(m_pRouter,id_list,count,UStatus))
        {
            CLog::Log("CBusinessWebBusiList",CLog::TYPE_ERROR,"%d QueryUserStatus error !",pUser->m_nID);
        }
#else
        if (RESULT_FAILED == m_pRouter->GetGlobalBusi()->QueryUserStatus(id_list,UStatus,count))
        {
            CLog::Log("CBusinessWebBusiList",CLog::TYPE_ERROR,"%d QueryUserStatus error !",pUser->m_nID);
        }
#endif
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
    pUser->NotifyFriendStatus(m_pRouter->GetGlobalBusi());

    //向全局注销该用户信息
    uint32  uid = pUser->m_nID;
    uint8   count = 1;

    pUser->LogoutTempFamily(m_pAppNetwork, m_pRouter->GetGlobalFamilyBusi(), m_pRouter->GetGlobalBusi());

    pUser->m_PermFamilyMap.Lock();
    FamilyList_Type::iterator itf = pUser->m_PermFamilyMap.begin();
    for(; itf != pUser->m_PermFamilyMap.end(); ++itf)
    {
        //保存固定群消息id
        m_pRouter->GetDataBaseMini()->Save_User_RecvFamilyMsgID(pUser->m_nID, (*itf).first, (*itf).second);
    }
    pUser->m_PermFamilyMap.UnLock();

    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USEROPERATION,pUser->m_nOperationID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERRECVMSG,pUser->m_nUserMsgID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::SYSTEMPUB,pUser->m_nSysMsgID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERPUB,pUser->m_nMobileMsgID);//代表已接收短信号
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERCOMMENT,pUser->m_nUsercommentID);
    m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::LOGCOMMENT,pUser->m_nLogcommentID);

    //保存用户在线时长
    int money;
    time_t tm;
    time(&tm);
    m_pRouter->GetDataBaseMini()->Save_User_OnlineTime(pUser->m_nID, pUser->m_nLoginTime, (int)tm, money);
    if(pUser->m_nID < Id_Boundary || pUser->m_nID > cs_Boundary)
    {
        m_pRouter->GetDataBaseMini()->Save_Merchant_Logout(pUser->m_nID,(int)tm);
        using namespace LogInterface;
        using namespace std;
        hessianLogi hess(pUser->m_merchantid,pUser->m_nID);
        try
        {

            SubMerchantAction info;
            if(pUser->m_nID > cs_Boundary)
            {
                info.merchantID = -1;
            }
            else
            {
                info.merchantID = pUser->m_merchantid;
            }
            info.subMerchantID = pUser->m_nID;
            info.actionTime = (int)tm;
            info.actionType = ACTION_LOGOUT;


            ResultInfo res = hess.GetHessiani()->uploadAction(info);

            CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
                      info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
        }

        catch (std::exception& e)
        {
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
        //给用户发送冲值消息
        CLog::Log("Money", CLog::TYPE_IMPORTANT, "用户:%d赠送积分:%d", pUser->m_nID, money*5);
    }

    //回收用户内存
    pUser->m_FriendList.Clear();
    pUser->m_PermFamilyMap.Lock();
    pUser->m_PermFamilyMap.clear();
    pUser->m_PermFamilyMap.UnLock();
    pUser->m_TempFamilyList.Lock();
    pUser->m_TempFamilyList.clear();
    pUser->m_TempFamilyList.UnLock();

    CUserManager *pUserManager = m_pRouter->GetUserManager();
//    pUser->DelFromOnlineList();
    pUserManager->Del(pUser->m_nID);
#ifdef ACCESS_SERVER    
    m_pAppNetwork->UserLogout(pUser->m_nID);
#endif
    CLog::Log("WebLogout",CLog::TYPE_DEBUG,"uid:%d, count:%d, type:%d ",uid,count,type);
    if(type != 0xAA)
    {
        m_pRouter->GetGlobalBusi()->DelUserInfo(&uid,count);
    }

	if(type == 0x00)
	{
		pUser->m_isLogout = 0;	//加入用户锁			
		CLog::Log("WebLogout",CLog::TYPE_DEBUG,"unlock uid:%d,type:%d ",uid,type);
	}
    pUser = NULL;

    SysStat::GetInstance().rmOnlineNum();

}
#endif
/*
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


//登陆业务作为一个特殊的业务处理,是基础传输层的处理，需要应用层自行加解密
int CBusiRegister::DoWork()
{
    //初始化基本数据
    RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;
    LPXT_HEAD lpSendHead = (LPXT_HEAD)m_pSendData->data;
    m_szError[0] = '\0';

    CRecvPacket rp(pRecvData->data + sizeof(XT_HEAD),pRecvData->data_len - sizeof(XT_HEAD));
    //解包
    XT_REGIST_REQ  req;
    rp.ReadPacket(req.key,sizeof(req.key));

    CLog::Log("CBusiRegister", CLog::TYPE_IMPORTANT, "sid:%u, did:%x, flag:%u" , lpRecvHead->sid, lpRecvHead->did,lpRecvHead->flag);



    char plain[MAX_RECV_BUFFERS] = {0};
    int dwPlainLen = 0;
    dwPlainLen = rp.ReadPacket(plain,rp.GetSize() - 16);
    if( !xTEADecryptWithKey(plain,dwPlainLen,req.key, plain ,&dwPlainLen) )
    {
        return RESULT_FAILED;   //解密失败
    }

//  rp.SetData(plain,dwPlainLen);
//  rp  >>req.regfrom
//      >>req.username
//      >>req.sex
//      >>req.nickname
//      >>req.password
//      >>req.email;
//
//  get_md5_password(req.password);
//  uint8 ReturnCode = 0;
//  int dbret = m_pDataBaseWeb->Add_Register_Info(&req, m_uid, ReturnCode);
//
    CSendPacket sp(m_pSendData->data + sizeof(XT_HEAD));
//  if(dbret != DB_SUCCESS)
//  {
//      //组装返回错误原因包
//      char szError[MAX_ERROR_BUF + 1];
//      CError::DB_Error(dbret, szError, true);
//      sp  <<(uint8)RESULT_FAILED
//          <<szError;
//  }
//  else
//  {
//      sp  <<(uint8)RESULT_SUCCESS
//          <<m_uid;
//  }
    sp  <<(uint8)RESULT_FAILED
        <<"请在飞牛网注册！http://www.feiniu.com";
    m_pSendData->data_len = static_cast<uint16>(sizeof(XT_HEAD) + sp.GetSize());
    InitHead(XT_HEAD::FLAG_SRVACK,lpSendHead,m_nBusinessIDAck,lpRecvHead->seq,m_pSendData->data_len - sizeof(XT_HEAD),XT_HEAD::DEST_TYPE_USER,lpRecvHead->sid);

    int tlen = 0;
    if(!xTEAEncryptWithKey(m_pSendData->data + sizeof(XT_HEAD),m_pSendData->data_len - sizeof(XT_HEAD),req.key,m_pSendData->data + sizeof(XT_HEAD),&tlen))
    {
        CLog::Log("CBusiRegister",CLog::TYPE_ERROR,"xTEAEncryptWithKey failed");
        return BUSIERR_INTERNAL;
    }
    m_pSendData->data_len = (uint16)tlen + sizeof(XT_HEAD);

    //回复注册请求,调用传输层直接发送
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
*/
