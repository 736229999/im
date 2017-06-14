#include "stdafx.h"
#include "BusiHeartTick.h"
#include "BusiLogin.h"
#include "AppManager.h"
#include "RouterProtocol.h"
#include <time.h>
#include "hessian/hessianLogi.h"
#include "UserManager.h"
#include "Function.h"

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
#ifdef IM_USE_MULTIDEVICE   
    RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
    LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;
	uint32 curIp = 0;
	uint16 curPort = 0;
#endif
	LPXT_DIRECTORY_REQ lpReq = (LPXT_DIRECTORY_REQ)m_RecvPacket.GetData();

	CUserManager *pUserManager = m_pRouter->GetUserManager();
	shared_ptr<CUser> pUser = pUserManager->Find(lpReq->id);
	RESPONSE_ACK ack;
	
	if(pUser == NULL)
	{
		ack.ret = CLIENT_FORCE_RELOGIN;	//重登陆		
		CLog::Log("CBusiHeartTick", CLog::TYPE_ERROR,"用户%u不在本服务器上，强制客户端重登陆!", lpReq->id);
	}	
	else
	{

	
	//底层的连接信息
#ifdef WEBIM_TCP
		if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
		{
#endif
/*
			LPONLINE_USER_INFO pOnlineUser = CAppManager::GetInstance().GetAppNetWork().FindOnlineUser(lpReq->id);
			if( !pOnlineUser )
			{
				ack.ret = CLIENT_FORCE_RELOGIN;	//重登陆		
				CLog::Log("CBusiHeartTick", CLog::TYPE_ERROR,"用户pOnlineUser信息%u不在本服务器上，强制客户端重登陆!", lpReq->id);
			}
			else
*/			
			{
				RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;			
				
				//非web端增加redis 超时时间
				CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();	
#ifdef IM_USE_MULTIDEVICE   
				//获取到设备当前信息，如果与redis不相符，则修改redis
				multiDevice_t deviceInfo;
				if(pGlobalBusi->QueryUserDeviceInfo(lpReq->id,lpRecvHead->deviceId,deviceInfo))
				{
					if(deviceInfo.clientIp != pRecData->src_ip || deviceInfo.clientPort != pRecData->port )
					{				
						CLog::Log("CBusiHeartTick", CLog::TYPE_ERROR,"用户 %u 在本服务器IP地址由%u:%d变化为%u:%d",
							lpReq->id,deviceInfo.clientIp,deviceInfo.clientPort,pRecData->src_ip,pRecData->port);			
						
						if(RESULT_FAILED == pGlobalBusi->AddModiDeviceIpPortInfo(lpReq->id,lpRecvHead->deviceId,pRecData->src_ip,pRecData->port))
						{					
							CLog::Log("CBusiHeartTick", CLog::TYPE_ERROR,"更新用户 %u 设备%u 源IP到redis失败!", lpReq->id,lpRecvHead->deviceId);
						}
					}
				}

				
#else
				if((pUser->m_nPort != pRecData->port) || (pUser->m_nIP != pRecData->src_ip))
				{				
					CLog::Log("CBusiHeartTick", CLog::TYPE_ERROR,"用户 %u 在本服务器IP地址由%u:%d变化为%u:%d",
						lpReq->id,pUser->m_nIP,pUser->m_nPort,pRecData->src_ip,pRecData->port);
					
//					pOnlineUser->uport	= pRecData->port;
//					pOnlineUser->uip	= pRecData->src_ip;
					pUser->m_nPort		= pRecData->port;
					pUser->m_nIP		= pRecData->src_ip;					
					
					if(RESULT_FAILED == pGlobalBusi->ModiUserIpPortInfo(lpReq->id,pRecData->src_ip,pRecData->port))
			        {		        	
						CLog::Log("CBusiHeartTick", CLog::TYPE_ERROR,"更新用户 %u 源IP到redis失败!", lpReq->id);
			        }
				}	
#endif
		        if(RESULT_FAILED == pGlobalBusi->setExpireUserinfo(lpReq->id))
		        {
		            CLog::Log("CBusiHeartTick", CLog::TYPE_ERROR,"更新用户 %u 超时时间失败!", lpReq->id);
		        }
				else
				{
					CLog::Log("CBusiHeartTick", CLog::TYPE_IMPORTANT,"更新用户 %u 超时时间!", lpReq->id);
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
		

#ifdef IM_USE_MULTIDEVICE
		uint32 timestamp = timeGetTime();
		//增加相应设备的心跳时间

		if(RESULT_FAILED == m_pRouter->GetGlobalBusi()->AddModiDeviceHeartInfo(lpReq->id,lpRecvHead->deviceId,timestamp))
		{
			CLog::Log("CBusiHeartTick", CLog::TYPE_ERROR,"更新用户 %u 超时时间失败!", lpReq->id);
		}
		else
		{
			CLog::Log("CBusiHeartTick", CLog::TYPE_IMPORTANT,"更新用户 %u 超时时间!", lpReq->id);
		}			
#else
		// 用户信息回写入redis
		pUser->m_nNewlyLiveTime = timeGetTime();
		m_pRouter->GetGlobalBusi()->ModiNewLiveTime(lpReq->id,pUser->m_nNewlyLiveTime);

#endif

/*
		// 更新网络层内存
		LPONLINE_USER_INFO pOnlineUser = CAppManager::GetInstance().GetAppNetWork().FindOnlineUser(lpReq->id);
		if( pOnlineUser != NULL )
		{
			pOnlineUser->newlylivetime = pUser->m_nNewlyLiveTime;
		}
*/		
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
		CLog::Log("CHeartTickCheck",CLog::TYPE_ERROR,"心跳定时器设置失败!");
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



#ifndef IM_USE_MULTIDEVICE

void CHeartTickCheck::HandleTimer()
{
	vector<tagGSRV_TUSERINFO_ACK_EX> vecUserInfo;	
	int rlt = m_pGlobalBusi->GetAllUser(CAppManager::GetInstance().GetConfig().GetServerNo(), vecUserInfo);
	if(rlt != MEMCACHED_GET_INFO_SUCCESS)
	{
		CLog::Log("HeartTickCheck", CLog::TYPE_ERROR, "search all user failed!");
		return;
	}

	try
	{
		int nCheckUserNum = 0 ;
		int nBuyerCnt = 0;
		int nSellerCnt = 0;
		int nCSCnt = 0;
		int nWorkCnt = 0;
		int nTickOut = 0;

		list<UINT32>  lstEraseID;
		uint32 now = timeGetTime();

		for(int i = 0; i < (int)vecUserInfo.size(); ++i)
		{
			if ( nCheckUserNum > 30000)
			{
				break;
			}

			tagGSRV_TUSERINFO_ACK_EX &userInfo = vecUserInfo[i];
			if(userInfo.id> Id_Boundary &&  userInfo.id < cs_Boundary)
			{
				nBuyerCnt++;
			}
			else if(userInfo.id > cs_Boundary  && userInfo.id < work_Boundary )
			{
				nCSCnt++;
			}
			else if(userInfo.id > work_Boundary)
			{
				nWorkCnt++;
			}
			else
			{
				nSellerCnt++;
			}
			++nCheckUserNum;

			if(userInfo.flag != XT_HEAD::FLAG_MOBREQ)
			{
				if((now - userInfo.newlylivetime) > m_nHeartTickTimeOut)
				{	//心跳超时
					CLog::Log("HeartTickCheck", CLog::TYPE_IMPORTANT, "User %d timeout!", userInfo.id);
		
					lstEraseID.push_back(userInfo.id);
					++nTickOut;
					continue;
				}
			}
#ifdef WEBTIMEOUT			
			else	//web端参与超时,按最后一次发消息时间为准，超时为10分钟 20151123
			{
				if((now - userInfo.newlylivetime) > m_nHeartTickWebTimeOut)
				{	//心跳超时
					CLog::Log("HeartTickCheck", CLog::TYPE_IMPORTANT, "WEB User %u timeout!", userInfo.id);
		
					lstEraseID.push_back(userInfo.id);
					++nTickOut;
					continue;
				}
			}
#endif
		}
		
		for(list<UINT32>::iterator lit = lstEraseID.begin(); lit != lstEraseID.end(); ++lit)
		{
			UINT32 id = *lit;
			SendLogoutReq(id);
		}
		CLog::Log("HeartTickCheck", CLog::TYPE_IMPORTANT, "user:%d, buyer:%u, seller:%u,ClientServer:%u, logout:%u ", nCheckUserNum, nBuyerCnt, nSellerCnt, nCSCnt,nTickOut);
	}
	catch (...)
	{
		CLog::Log("HeartTickCheck", CLog::TYPE_ERROR, "catch error");
	}

	//防止因连续几天登录而导致日志服务器无法统计时间，故在每日00:00查询在线用户并上传login动作日志。
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

		for(int i = 0; i < (int)vecUserInfo.size(); ++i)
		{
			tagGSRV_TUSERINFO_ACK_EX &userInfo = vecUserInfo[i];

			if(userInfo.id < Id_Boundary || userInfo.id > cs_Boundary)
			{
				//上传登录动作
				using namespace LogInterface;
				using namespace std;
				hessianLogi hess(userInfo.merchantID,userInfo.id);
				try 
				{
					SubMerchantAction info;
					if(userInfo.id > cs_Boundary)
					{					
						info.merchantID = -1;
					}
					else
					{
						info.merchantID = userInfo.merchantID;
					}
					info.subMerchantID = userInfo.id;
					info.actionTime = (int)aclock;
					info.actionType = ACTION_LOGIN;

					ResultInfo res = hess.GetHessiani()->uploadAction(info);

					CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
							info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());

					// 要判断当前商家状态是否为离开和挂起状态，如果是，还应上传相应动作	
					if(userInfo.status == XTREAM_LEAVE)
					{
						SubMerchantAction info;
						if(userInfo.id > cs_Boundary)
						{					
							info.merchantID = -1;
						}
						else
						{
							info.merchantID = userInfo.merchantID;
						}
						info.subMerchantID = userInfo.id;
						info.actionTime = (int)aclock+1;
						info.actionType = ACTION_LEAVE;

						ResultInfo res = hess.GetHessiani()->uploadAction(info);

						CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
								info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
					}

					if(userInfo.flow == 1)
					{
						SubMerchantAction info;
						if(userInfo.id > cs_Boundary)
						{					
							info.merchantID = -1;
						}
						else
						{
							info.merchantID = userInfo.merchantID;
						}
						info.subMerchantID = userInfo.id;
						info.actionTime = (int)aclock+2;
						info.actionType = ACTION_HANGUP;

						ResultInfo res = hess.GetHessiani()->uploadAction(info);

						CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
								info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
					}
				}
				catch (std::exception& e) 
				{
					CLog::Log("hessian",CLog::TYPE_ERROR,"merchant_id:%d, subMerchantID:%d, other exception occured: %s",
						userInfo.merchantID, userInfo.id, e.what());
				}
			}
		}
	}
}


//发送一个退出业务包
void CHeartTickCheck::SendLogoutReq(uint32 userid)
{
	tagGSRV_TUSERINFO_ACK tUserInfo;
	if(MEMCACHED_GET_INFO_SUCCESS != m_pGlobalBusi->GetTUserInfo(userid,tUserInfo))
	{
		return;
	}

	RecvDataStruct RecvData;
	RecvData.data_len = sizeof(XT_HEAD) + sizeof(XT_LOGOUT);
	XT_HEAD* pHead	= (XT_HEAD*)RecvData.data;
	pHead->cmd		= CMD_LOGOUT;
	pHead->sid		= userid;

	char crypt[1500];
	CSendPacket cp(crypt);
	cp<<userid;
	cp<<(uint8)0x00;

	// 加密数据再放入发送队列 
	int tlen = 0;
	if(!xTEAEncryptWithKey((char *)crypt,cp.GetSize(),tUserInfo.sessionkey,RecvData.data + sizeof(XT_HEAD),&tlen))
	{
		CLog::Log("CHeartTickCheck",CLog::TYPE_ERROR,"SendLogoutReq(%d) 加密失败:sid:%d, from:%d, cmd:0x%.4x, did:%d, dtype:%d",
				__LINE__, pHead->sid,pHead->from,pHead->cmd,pHead->did, pHead->dtype);
		return;
	}
	RecvData.data_len = (uint16)tlen +	sizeof(XT_HEAD);

	CRouterProtocol::GetInstance()->RouterOnePacket(&RecvData);
}
#else		//支持多设备

void CHeartTickCheck::HandleTimer()
{
	vector<tagGSRV_TUSERINFO_ACK_EX> vecUserInfo;	
	int rlt = m_pGlobalBusi->GetAllUser(CAppManager::GetInstance().GetConfig().GetServerNo(), vecUserInfo);
	if(rlt != MEMCACHED_GET_INFO_SUCCESS)
	{
		CLog::Log("HeartTickCheck", CLog::TYPE_ERROR, "search all user failed!");
		return;
	}

	try
	{
		int nCheckUserNum = 0 ;
		int nBuyerCnt = 0;
		int nSellerCnt = 0;
		int nCSCnt = 0;
		int nWorkCnt = 0;
		int nTickOut = 0;

		list<multiDevice_t>  lstEraseID;
		uint32 now = timeGetTime();

		for(int i = 0; i < (int)vecUserInfo.size(); ++i)
		{
			if ( nCheckUserNum > 30000)
			{
				break;
			}

			tagGSRV_TUSERINFO_ACK_EX &userInfo = vecUserInfo[i];
			if(userInfo.id> Id_Boundary &&  userInfo.id < cs_Boundary)
			{
				nBuyerCnt++;
			}
			else if(userInfo.id > cs_Boundary  && userInfo.id < work_Boundary )
			{
				nCSCnt++;
			}
			else if(userInfo.id > work_Boundary)
			{
				nWorkCnt++;
			}
			else
			{
				nSellerCnt++;
			}
			++nCheckUserNum;
			
			vector<multiDevice_t> deviceinfo;
			rlt = m_pGlobalBusi->QueryUserDeviceInfo(userInfo.id,0,deviceinfo);
			if(rlt != MEMCACHED_GET_INFO_SUCCESS)
			{
				CLog::Log("HeartTickCheck", CLog::TYPE_ERROR, "search all device info failed!");
				return;
			}
			if(userInfo.flag != XT_HEAD::FLAG_MOBREQ)
			{
				for(size_t i = 0; i < deviceinfo.size(); i++)
				{
					if(( now - deviceinfo[i].heartBeatTime ) > m_nHeartTickTimeOut)
					{	//心跳超时
						CLog::Log("HeartTickCheck", CLog::TYPE_IMPORTANT, 
							"User %u device %d timeout!",userInfo.id,deviceinfo[i].deviceId);
			
						lstEraseID.push_back(deviceinfo[i]);
						++nTickOut;						
					}
				}
			}
#ifdef WEBTIMEOUT			
			else	//web端参与超时,按最后一次发消息时间为准，超时为10分钟 20151123
			{
				for(size_t i = 0; i < deviceinfo.size(); i++)
				{
					if((now - deviceinfo[i].heartBeatTime) > m_nHeartTickWebTimeOut)
					{	//心跳超时
						CLog::Log("HeartTickCheck", CLog::TYPE_IMPORTANT,
							"WEB User %u timeout!", userInfo.id,deviceinfo[i].deviceId);
			
						lstEraseID.push_back(deviceinfo[i]);
						++nTickOut;						
					}
				}
			}
			deviceinfo.clear();
#endif
		}
		
		for(list<multiDevice_t>::iterator lit = lstEraseID.begin(); lit != lstEraseID.end(); ++lit)
		{
			UINT32 id = lit->userId;
			UINT32 deviceId = lit->deviceId;
			SendLogoutReq(id,deviceId);
		}
		CLog::Log("HeartTickCheck", CLog::TYPE_IMPORTANT, "user:%d, buyer:%u, seller:%u,ClientServer:%u, logout:%u ", nCheckUserNum, nBuyerCnt, nSellerCnt, nCSCnt,nTickOut);
	}
	catch (...)
	{
		CLog::Log("HeartTickCheck", CLog::TYPE_ERROR, "catch error");
	}

	//防止因连续几天登录而导致日志服务器无法统计时间，故在每日00:00查询在线用户并上传login动作日志。
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

		for(int i = 0; i < (int)vecUserInfo.size(); ++i)
		{
			tagGSRV_TUSERINFO_ACK_EX &userInfo = vecUserInfo[i];

			if(userInfo.id < Id_Boundary || userInfo.id > cs_Boundary)
			{
				//上传登录动作
				using namespace LogInterface;
				using namespace std;
				hessianLogi hess(userInfo.merchantID,userInfo.id);
				try 
				{
					SubMerchantAction info;
					if(userInfo.id > cs_Boundary)
					{					
						info.merchantID = -1;
					}
					else
					{
						info.merchantID = userInfo.merchantID;
					}
					info.subMerchantID = userInfo.id;
					info.actionTime = (int)aclock;
					info.actionType = ACTION_LOGIN;

					ResultInfo res = hess.GetHessiani()->uploadAction(info);

					CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
							info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());

					// 要判断当前商家状态是否为离开和挂起状态，如果是，还应上传相应动作	
					if(userInfo.status == XTREAM_LEAVE)
					{
						SubMerchantAction info;
						if(userInfo.id > cs_Boundary)
						{					
							info.merchantID = -1;
						}
						else
						{
							info.merchantID = userInfo.merchantID;
						}
						info.subMerchantID = userInfo.id;
						info.actionTime = (int)aclock+1;
						info.actionType = ACTION_LEAVE;

						ResultInfo res = hess.GetHessiani()->uploadAction(info);

						CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
								info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
					}

					if(userInfo.flow == 1)
					{
						SubMerchantAction info;
						if(userInfo.id > cs_Boundary)
						{					
							info.merchantID = -1;
						}
						else
						{
							info.merchantID = userInfo.merchantID;
						}
						info.subMerchantID = userInfo.id;
						info.actionTime = (int)aclock+2;
						info.actionType = ACTION_HANGUP;

						ResultInfo res = hess.GetHessiani()->uploadAction(info);

						CLog::Log("hessian",CLog::TYPE_IMPORTANT,"merchant_id:%d, subMerchantID:%d, actionTime:%d, actionType:%d, Message: %s",
								info.merchantID, info.subMerchantID, info.actionTime, info.actionType, res.errorMessage.c_str());
					}		
				}
				catch (std::exception& e) 
				{
					CLog::Log("hessian",CLog::TYPE_ERROR,"merchant_id:%d, subMerchantID:%d, other exception occured: %s",
						userInfo.merchantID, userInfo.id, e.what());
				}
			}
		}
	}
}


void CHeartTickCheck::SendLogoutReq(uint32 userid,uint32 deviceId)
{
	multiDevice_t deviceInfo;
	if(MEMCACHED_GET_INFO_SUCCESS != m_pGlobalBusi->QueryUserDeviceInfo(userid,deviceId,deviceInfo))
	{
		return;
	}
	RecvDataStruct RecvData;
	RecvData.data_len = sizeof(XT_HEAD) + sizeof(XT_LOGOUT);
	XT_HEAD* pHead	= (XT_HEAD*)RecvData.data;
	pHead->cmd		= CMD_LOGOUT;
	pHead->sid		= userid;
	pHead->deviceId = deviceId;
	
	char crypt[1500];
	CSendPacket cp(crypt);
	cp<<userid;
	cp<<(uint8)0x00;

	// 加密数据再放入发送队列 
	int tlen = 0;
	if(!xTEAEncryptWithKey((char *)crypt,cp.GetSize(),deviceInfo.sessionKey,RecvData.data + sizeof(XT_HEAD),&tlen))
	{
		CLog::Log("CHeartTickCheck",CLog::TYPE_ERROR,"SendLogoutReq(%d) 加密失败:sid:%d, from:%d, cmd:0x%.4x, did:%d, dtype:%d",
				__LINE__, pHead->sid,pHead->from,pHead->cmd,pHead->did, pHead->dtype);
		return;
	}
	RecvData.data_len = (uint16)tlen +	sizeof(XT_HEAD);

	CRouterProtocol::GetInstance()->RouterOnePacket(&RecvData);
}


#endif

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
		CLog::Log("CTimeLenSave",CLog::TYPE_ERROR,"自动记录用户在线时长定时器启动失败!");
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
