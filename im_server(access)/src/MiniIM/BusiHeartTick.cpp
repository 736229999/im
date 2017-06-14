#include "stdafx.h"
#include "BusiHeartTick.h"
#include "BusiLogin.h"
#include "AppManager.h"
#include "RouterProtocol.h"
#include <time.h>
#include "hessian/hessianLogi.h"
#include "UserManager.h"

#ifdef ACCESS_SERVER
CBusiHeartTick::CBusiHeartTick(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_DIRECTORY_REQ,CMD_DIRECTORY_ACK)
{

}

CBusiHeartTick::~CBusiHeartTick(void)
{

}

int CBusiHeartTick::DoWork()
{
	LPXT_DIRECTORY_REQ lpReq = (LPXT_DIRECTORY_REQ)m_RecvPacket.GetData();

	CUserManager *pUserManager = m_pRouter->GetUserManager();
	shared_ptr<CUser> pUser = pUserManager->Find(lpReq->id);
	RESPONSE_ACK ack;
	
	if(pUser == NULL)
	{
		ack.ret = CLIENT_FORCE_RELOGIN;	//�ص�½		
		CLog::Log("CBusiHeartTick", CLog::TYPE_ERROR,"�û�%u���ڱ��������ϣ�ǿ�ƿͻ����ص�½!", lpReq->id);
	}	
	else
	{

	
	//�ײ��������Ϣ
#ifdef WEBIM_TCP
		if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
		{
#endif
			LPONLINE_USER_INFO pOnlineUser = CAppManager::GetInstance().GetAppNetWork().FindOnlineUser(lpReq->id);
			if( !pOnlineUser )
			{
				ack.ret = CLIENT_FORCE_RELOGIN;	//�ص�½		
				CLog::Log("CBusiHeartTick", CLog::TYPE_ERROR,"�û�pOnlineUser��Ϣ%u���ڱ��������ϣ�ǿ�ƿͻ����ص�½!", lpReq->id);
			}
			else
			{
				RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;			
				
				//��web������redis ��ʱʱ��
				CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();	
				
				if((pUser->m_nPort != pRecData->port) || (pUser->m_nIP != pRecData->src_ip))
				{				
					CLog::Log("CBusiHeartTick", CLog::TYPE_ERROR,"�û� %u �ڱ�������IP��ַ��%u:%d�仯Ϊ%u:%d",
						lpReq->id,pUser->m_nIP,pUser->m_nPort,pRecData->src_ip,pRecData->port);
					
					pOnlineUser->uport	= pRecData->port;
					pOnlineUser->uip	= pRecData->src_ip;
					pUser->m_nPort		= pRecData->port;
					pUser->m_nIP		= pRecData->src_ip;					
					
					if(RESULT_FAILED == pGlobalBusi->ModiUserIpPortInfo(lpReq->id,pRecData->src_ip,pRecData->port))
			                {		        	
						CLog::Log("CBusiHeartTick", CLog::TYPE_ERROR,"�����û� %u ԴIP��redisʧ��!", lpReq->id);
			                }
				}			
		                if(RESULT_FAILED == pGlobalBusi->setExpireUserinfo(lpReq->id))
		                {
		                    CLog::Log("CBusiHeartTick", CLog::TYPE_ERROR,"�����û� %u ��ʱʱ��ʧ��!", lpReq->id);
		                }
				else
				{
					CLog::Log("CBusiHeartTick", CLog::TYPE_IMPORTANT,"�����û� %u ��ʱʱ��!", lpReq->id);
				}				
				ack.ret = 0;
			}
#ifdef WEBIM_TCP
		}
		else
		{
			 ack.ret = 0;
		}
#endif	
		pUser->m_nNewlyLiveTime = timeGetTime();

		// �û���Ϣ��д��redis
		pUserManager->AddUserToRedis(pUser);

		// ����������ڴ�
		LPONLINE_USER_INFO pOnlineUser = CAppManager::GetInstance().GetAppNetWork().FindOnlineUser(lpReq->id);
		if( pOnlineUser != NULL )
		{
			pOnlineUser->newlylivetime = pUser->m_nNewlyLiveTime;
		}
	}

	m_SendPacket<<ack.ret;

	return 0;
}

/////////////////////CHeartTickCheck.cpp//////////////////////
void  MyHeartTimeProc(void* dwUser)
{
	CHeartTickCheck* pObj = (CHeartTickCheck*)dwUser;
	pObj->HandleTimer();
}

void  MyHeartTimeProcNew(union sigval v)
{
	//CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG, "timer:1");
	MyHeartTimeProc(v.sival_ptr);
}

CHeartTickCheck::CHeartTickCheck()
{
	m_hMMT = NULL;
	m_pRedis = NULL;
	m_pGlobalBusi = NULL;
	m_pUserManager = NULL;
}
CHeartTickCheck::~CHeartTickCheck(void)
{
	StopWork();
}

int CHeartTickCheck::StartWork()
{
	CConfigInfo 	dbConfig;
	char *memcached_addr = dbConfig.GetMemcachedAddr();
	int memcached_port = dbConfig.GetMemcachedPort();
		
	m_pRedis = new ImMemCache();
	if(m_pRedis->init_ImMemcached(memcached_addr,memcached_port) < 0)
	{	
		CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"init_ImMemcached fail");
		return -1; 
	}
	
	m_pGlobalBusi = new CGlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl(),m_pRedis);
	m_pUserManager = new CUserManager(m_pGlobalBusi);

	StartTimer();
	return 0;
}

void CHeartTickCheck::StopWork()
{
	StopTiemr();

	if(m_pRedis != NULL)
	{
		delete m_pRedis;
		m_pRedis = NULL;
	}

	if(m_pGlobalBusi != NULL)
	{
		delete m_pGlobalBusi;
		m_pGlobalBusi = NULL;
	}

	if(m_pUserManager != NULL)
	{
		delete m_pUserManager;
		m_pUserManager = NULL;
	}
}

void CHeartTickCheck::StartTimer()
{
	/*
	m_hMMT = timeSetEvent(TIMERPERIOD,TIMERPERIOD, MyHeartTimeProcNew, (void*)this,TIME_PERIODIC);
	if( NULL == m_hMMT )
	{
		CLog::Log("CHeartTickCheck",CLog::TYPE_ERROR,"������ʱ������ʧ��!");
	}
	*/
	if(!timeSetEventNew(m_timer, m_nTimerDelay, m_nTimerResolution, MyHeartTimeProcNew, (void*)this ) )
	{
		CLog::Log("CHeartTickCheck", CLog::TYPE_ERROR, "StartTimerFailed");
	}
}

void CHeartTickCheck::StopTiemr()
{
	/*
	if( m_hMMT )
	{
		::timeKillEvent(m_hMMT);
		m_hMMT = NULL;
	}
	*/
	timer_delete(m_timer);
}


void CHeartTickCheck::HandleTimer()
{
	try
	{
		//CUser::m_UserListLock.Lock();

		int nCheckUserNum = 0 ;
		int nBuyerCnt = 0;
		int nSellerCnt = 0;
		int nCSCnt = 0;
		int nWorkCnt = 0;
		int nTickOut = 0;

//		CUser *pUserCurPoint = NULL;
		list<UINT32>  lstEraseID;
		uint32 now = timeGetTime();

		int hashelems = CAppManager::GetInstance().GetAppNetWork().GetUserHash_Elems();
		for(int i = 0; i < hashelems; ++i)
		{
			if ( nCheckUserNum > 30000)
			{
				break;
			}
			vector<uint32> vecID;
			CAppManager::GetInstance().GetAppNetWork().GetUserHash_ElemIDs(i, vecID);
			for(int j = 0; j < (int)vecID.size(); ++j)
			{
				uint32 userid = vecID[j];

				LPONLINE_USER_INFO pOnlineUser= CAppManager::GetInstance().GetAppNetWork().FindOnlineUser(userid);
				if(pOnlineUser == NULL)
				{
					continue;
				}

				if ( nCheckUserNum > 30000)
				{
					break;
				}
				if(userid > Id_Boundary &&  userid < cs_Boundary)
				{
					nBuyerCnt++;
				}
				else if(userid > cs_Boundary  && userid < work_Boundary )
				{
					nCSCnt++;
				}
				else if(userid > work_Boundary)
				{
					nWorkCnt++;
				}
				else
				{
					nSellerCnt++;
				}
				++nCheckUserNum;

				if(pOnlineUser->flag != XT_HEAD::FLAG_MOBREQ)
				{
					if((now - pOnlineUser->newlylivetime) > m_nHeartTickTimeOut)
					{	//������ʱ
						CLog::Log("HeartTickCheck", CLog::TYPE_IMPORTANT, "User %d timeout!", userid);
			
						lstEraseID.push_back(userid);
						++nTickOut;
						continue;
					}
				}
#ifdef WEBTIMEOUT			
				else	//web�˲��볬ʱ,�����һ�η���Ϣʱ��Ϊ׼����ʱΪ10���� 20151123
				{
					if((now - pOnlineUser->newlylivetime) > m_nHeartTickWebTimeOut)
					{	//������ʱ
						CLog::Log("HeartTickCheck", CLog::TYPE_IMPORTANT, "WEB User %u timeout!", userid);
			
						lstEraseID.push_back(userid);
						++nTickOut;
						continue;
					}
				}
#endif
			}
		}
		
		for(list<UINT32>::iterator lit = lstEraseID.begin(); lit != lstEraseID.end(); ++lit)
		{
			UINT32 id = *lit;
			LPONLINE_USER_INFO pOnlineUser= CAppManager::GetInstance().GetAppNetWork().FindOnlineUser(id);
			if(pOnlineUser != NULL)	
			{
				SendLogoutReq(id);
			}
			else
			{
				continue;
			}
		}
		CLog::Log("HeartTickCheck", CLog::TYPE_IMPORTANT, "user:%d, buyer:%u, seller:%u,ClientServer:%u, logout:%u ", nCheckUserNum, nBuyerCnt, nSellerCnt, nCSCnt,nTickOut);
		//CUser::m_UserListLock.Unlock();
	}
	catch (...)
	{
		CLog::Log("HeartTickCheck", CLog::TYPE_ERROR, "catch error");
		//CUser::m_UserListLock.Unlock();
	}
	//��ֹ�����������¼��������־�������޷�ͳ��ʱ�䣬����ÿ��00:00��ѯ�����û����ϴ�login������־��
	struct tm newtime;
	time_t aclock;
	time( &aclock ); 
	localtime_s(&newtime, &aclock );   
	char tmp[6];
	//CLog::Log("HandleTimer",CLog::TYPE_IMPORTANT,"tmp:%s",tmp);
	strftime(tmp,sizeof(tmp), "%H:%M", &newtime);
	if(strcmp(tmp, "00:00") == 0)
	{
		time_t tm;
		time(&tm);
		//CUser::m_UserListLock.Lock();

		int hashelems = CAppManager::GetInstance().GetAppNetWork().GetUserHash_Elems();
		for(int i = 0; i < hashelems; ++i)
		{
			vector<uint32> vecID;
			CAppManager::GetInstance().GetAppNetWork().GetUserHash_ElemIDs(i, vecID);
			for(int j = 0; j < (int)vecID.size(); ++i)
			{
				uint32 userid = vecID[j];

				LPONLINE_USER_INFO pOnlineUser= CAppManager::GetInstance().GetAppNetWork().FindOnlineUser(userid);
				if(pOnlineUser == NULL)
				{
					continue;
				}

				if(userid < Id_Boundary || userid > cs_Boundary)
				{
					//�ϴ���¼����
					using namespace LogInterface;
					using namespace std;
					hessianLogi hess(pOnlineUser->merchantID,userid);
					try 
					{
						SubMerchantAction info;
						if(userid > cs_Boundary)
						{					
							info.merchantID = -1;
						}
						else
						{
							info.merchantID = pOnlineUser->merchantID;
						}
						info.subMerchantID = userid;
						info.actionTime = (int)aclock;
						info.actionType = ACTION_LOGIN;

						ResultInfo res = hess.GetHessiani()->uploadAction(info);

						CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
							info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
					}
					catch (std::exception& e) 
					{
						CLog::Log("hessian",CLog::TYPE_ERROR,"merchant_id:%d, subMerchantID:%d, other exception occured: %s",
							pOnlineUser->merchantID, userid, e.what());
					}
				}
			}
		}
		//CUser::m_UserListLock.Unlock();
	}
}

//����һ���˳�ҵ���
void CHeartTickCheck::SendLogoutReq(uint32 userid)
{
	RecvDataStruct RecvData;
	RecvData.data_len = sizeof(XT_HEAD) + sizeof(XT_LOGOUT);
	XT_HEAD* pHead	= (XT_HEAD*)RecvData.data;
	pHead->cmd		= CMD_LOGOUT;
	pHead->sid		= userid;

	XT_LOGOUT* pLogOut = (XT_LOGOUT*)(RecvData.data + sizeof(XT_HEAD));
	pLogOut->id = userid;
	pLogOut->authData = 0x00;

	CRouterProtocol::GetInstance()->RouterOnePacket(&RecvData);
}
#endif


/////////////////////CTimeLenSave.cpp//////////////////////
/*
void  TimeLenProc(void* dwWebMsg)
{
	CTimeLenSave* pObj = (CTimeLenSave*)dwWebMsg;
	pObj->HandleTimer();
}


void  TimeLenProcNew(union sigval v)
{
	//CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT, "timer:2");
	TimeLenProc(v.sival_ptr);
}



CTimeLenSave::CTimeLenSave()
{
	m_hMMT = 0;
}


CTimeLenSave::~CTimeLenSave(void)
{
	StopWork();
}



int CTimeLenSave::StartWork()
{
	CConfigInfo		dbConfig;
	char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
	dbConfig.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
	m_pDataBaseMini.SetConnectDB(miniuid,minipwd,minisid);
	if( !m_pDataBaseMini.ConnectDB() )
	{
		CLog::Log("CTimeLenSave",CLog::TYPE_ERROR,"DatabaseMini.ConnectDB() failed!");
		return RESULT_FAILED;
	}

	StartTimer();
	return 0;
}

void CTimeLenSave::StopWork()
{
	StopTiemr();
}

void CTimeLenSave::StartTimer()
{
	if(!timeSetEventNew(m_timer, m_nTimerDelay, m_nTimerResolution, TimeLenProcNew, (void*)this) )
	{
		CLog::Log("CTimeLenSave",CLog::TYPE_ERROR,"�Զ���¼�û�����ʱ����ʱ������ʧ��!");
	}

}

void CTimeLenSave::StopTiemr()
{
	timer_delete(m_timer);
}


void CTimeLenSave::HandleTimer()
{
	struct tm newtime;
	time_t aclock;

	time( &aclock ); 

	localtime_s(&newtime, &aclock );   
	char tmp[6];
	strftime(tmp,sizeof(tmp), "%H:%M", &newtime);

	if (strcmp(tmp, "23:59") == 0)
	{
		//StopTiemr();	//del by lwl ver:0101
		CUser::m_UserListLock.Lock();;
		
		CUser *pUser = NULL;//CUser::m_pUserListHead;
		for(__gnu_cxx::hash_map<uint32,CUser*>::iterator it = CUser::m_userhash.begin(); it != CUser::m_userhash.end(); ++it)
		{
			pUser = it->second;
			if(!pUser)
				continue;
			m_pDataBaseMini.Save_User_Timelen(pUser->m_nID, pUser->m_nLoginTime, (int)aclock);
		}
		CUser::m_UserListLock.Unlock();
	}
}
*/
