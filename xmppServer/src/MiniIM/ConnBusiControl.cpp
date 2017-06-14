#include "stdafx.h"
#include "ConnBusiControl.h"
#include "Function.h"
#include "ConnBusi.h"
#include "ConfigInfo.h"
#include "AppManager.h"


#define TIMERPERIOD (1000 * 30 * 1) //时钟周期
#define TIMESTATUSLSTREQ (TIMERPERIOD * 2)
#define STATUSEXPIRE  (12 * 60 * 60 * 1000) //服务器状态失效  12 小时

CConnBusiControl::CConnBusiControl(uint32 connSrvIP,uint16 connSrvPort)
{
	printf("CConnBusiControl::CConnBusiControl\n");
	m_pConnBusi = NULL;
	m_hMMT = NULL;

	CConfigInfo ConfigInfo;
	m_nManSrvID = ConfigInfo.GetServerNo();
	m_srvType = tagSERVER_REGISTER::TYPE_BUSI;

	//与接入服务器交互
	srand(timeGetTime());
	m_nCntREQSeq = (rand()<<16)|rand();
	m_connSrvIP = connSrvIP;
	m_connSrvPort = connSrvPort;
	memset(m_connSessionKey,0,sizeof(m_connSessionKey));    //接入服务器session key
	memset(m_mineSessionKey,0,sizeof(m_mineSessionKey));    //自己的 session key

	m_timerStatusReq = 0; //状态列表自动请求计时器

	m_nextPos = 0;

	m_pBestSrvInfo = NULL;
}

CConnBusiControl::~CConnBusiControl(void)
{
}

void  BusiControlMyTimeProc(void* dwUser)
{
	//printf("BusiControlMyTimeProc dwUser=%d\n",(long)dwUser);
	CConnBusiControl* pObj = (CConnBusiControl*)dwUser;
	pObj->HandleTimer();
}

void CConnBusiControl::HandleTimer()
{
	//printf("CConnBusiControl::HandleTimer this=%ld\n",(long)this);
	TimerBusiRequest();

	//定时请求服务器状态列表，每次请求开始位置为上一次返回的nextPos
	m_timerStatusReq += TIMERPERIOD;
	if(m_timerStatusReq % TIMESTATUSLSTREQ == 0)
	{
		m_timerStatusReq = 0;
		TimerBusiSrvStatusLstReq();
	}
}

void CConnBusiControl::HandleRecvData(const RecvDataStruct * pRecvData)
{
	LPXT_HEAD lpHead = (LPXT_HEAD)pRecvData->data;
	//解密数据

	//CLog::Log("CConnBusiControl::HandleRecvData",CLog::TYPE_DEBUG,"返回命令 (0x%.4x) %d字节",
	//	lpHead->cmd,pRecvData->data_len);

	RecvDataStruct * pRecvDataTmp = (RecvDataStruct * )pRecvData;
	int tlen = 0;
    if(!xTEADecryptWithKey(pRecvData->data + sizeof(XT_HEAD),pRecvData->data_len - sizeof(XT_HEAD),m_mineSessionKey,(char *)(pRecvData->data + sizeof(XT_HEAD)),&tlen))
		return ;
	pRecvDataTmp->data_len = (uint16)tlen + sizeof(XT_HEAD);

	char * plainBody = (char *)(pRecvData->data + sizeof(XT_HEAD));
	switch(lpHead->cmd)
	{
	case CMD_SERVER_REGISTER_ACK: //注册应答
		{
			OnSrvRegisterAck(lpHead,plainBody);
		}
		break;
	case CMD_SERVER_STATUSLIST_REQ_ACK: //服务器状态列表请求应答
		{
			OnStatusListReqAck(lpHead,plainBody);
		}
		break;
	default:
		;
	}
}

void CConnBusiControl::TimerBusiRequest()
{
	SERVER_STATUS_REPORT reqStatusReport;
	reqStatusReport.capValue = 0;  
	reqStatusReport.linkCount = CAppManager::GetInstance().GetUserManager().GetElemCount(); 
	m_pConnBusi->BusiRequest(&reqStatusReport);
}


void CConnBusiControl::TimerBusiSrvStatusLstReq()
{
	if(m_nextPos == 0xffff)
		m_nextPos = 0;

	SERVER_STATUSLIST_REQ req;
	req.srvType = m_srvType;
	req.startPos = m_nextPos;
	m_pConnBusi->BusiRequest(&req);
}

void CConnBusiControl::SendRequestData(SendDataStruct & sendData)
{	
	sendData.dest_ip = m_connSrvIP;
	sendData.port = m_connSrvPort;
	sendData.retry_times = SEND_RETRY_TIME;

	//加密
	int tlen = 0;
	if(!xTEAEncryptWithKey(sendData.data + sizeof(XT_HEAD),sendData.data_len - sizeof(XT_HEAD),m_connSessionKey,sendData.data + sizeof(XT_HEAD),&tlen))
		return ;
	sendData.data_len = (uint16)tlen + sizeof(XT_HEAD);

	SendDataTo(sendData);
}

int CConnBusiControl::StartWork(const char * addr,uint16 port)
{
	//printf("CConnBusiControl::StartWork this=%ld\n",(int)this);
	m_hMMT = timeSetEvent(TIMERPERIOD,TIMERPERIOD,BusiControlMyTimeProc,(void*)this,TIME_PERIODIC);
	
	return CBaseUdpTrans::StartWork(addr,port);
}

void CConnBusiControl::StopWork()
{
	if(m_hMMT)
	{
		timeKillEvent(m_hMMT); m_hMMT = NULL;
		CBaseUdpTrans::StopWork();
	}
}


void CConnBusiControl::OnSrvRegisterAck(const LPXT_HEAD lpHead,const char * plainBody)
{
	LPSERVER_REGISTER_ACK lpAck = (LPSERVER_REGISTER_ACK)(plainBody);
	if(lpAck->ret != 0)
	{
		CLog::Log("CConnBusi",CLog::TYPE_DEBUG,"OnBusiAck ret = %d\n",lpAck->ret);
		return ;
	}
}

void CConnBusiControl::OnStatusListReqAck(const LPXT_HEAD lpHead,const char * plainBody)
{
	LPSERVER_STATUSLIST_REQ_ACK lpAck = (LPSERVER_STATUSLIST_REQ_ACK)plainBody;
	if(lpAck->ret != 0)
	{
		CLog::Log("CConnBusi",CLog::TYPE_DEBUG,"OnStatusListReqAck ret = %d\n",lpAck->ret);
		return ;
	}
	if(lpAck->retCount <= 0)
		return ;

	MAPSRVSTATUS mapSrvStatus;
	MAPSRVSTATUS::iterator itmap;
	m_nextPos = lpAck->nextPos;
	for(int i = 0; i < lpAck->retCount; ++i)
	{
		mapSrvStatus.insert(PAIRSRVSTATUS(lpAck->SrvStatus[i].srvID,&lpAck->SrvStatus[i]));
	}

	SmartList<SRVSTATUSINFO>::iterator itlst;
	LPSRVSTATUSINFO lpSrvStatusInfo = NULL;
	uint32 now = timeGetTime();
	//更新已存在的服务器状态信息
	m_stLstSrvStatusInfo.Lock();
	itlst = m_stLstSrvStatusInfo.begin();
	if( (NULL == m_pBestSrvInfo) && itlst !=m_stLstSrvStatusInfo.end() )
	{
		m_pBestSrvInfo = *itlst;
	}

	while(itlst != m_stLstSrvStatusInfo.end())
	{

		lpSrvStatusInfo = (*itlst);
		itmap = mapSrvStatus.find(lpSrvStatusInfo->SrvStatus.srvID);
		if(itmap != mapSrvStatus.end())
		{
			memcpy(&lpSrvStatusInfo->SrvStatus,itmap->second,sizeof(tagSrvStatus));
			(*itlst)->liveTime = timeGetTime();
			mapSrvStatus.erase(itmap);
		}
		else
		{
			if((now - lpSrvStatusInfo->liveTime) > STATUSEXPIRE)
			{//清除失效的状态信息
				itlst = m_stLstSrvStatusInfo.erase(itlst);
				CAppManager::GetInstance().GetAppNetWork().DelImServer(lpSrvStatusInfo->SrvStatus.srvID);
				continue;
			}
		}
		
		if( m_pBestSrvInfo->SrvStatus.srvDynamicInfo.linkCount > (*itlst)->SrvStatus.srvDynamicInfo.linkCount)
		{
			//得到当前最少连接数的服务器
			m_pBestSrvInfo = *itlst;
		}
		++itlst;
	}
	m_stLstSrvStatusInfo.UnLock();

	//加入新的服务器状态信息

	for(itmap = mapSrvStatus.begin(); itmap != mapSrvStatus.end(); ++itmap)
	{
		lpSrvStatusInfo = m_stLstSrvStatusInfo.NewObject();
		memcpy(&lpSrvStatusInfo->SrvStatus,itmap->second,sizeof(tagSrvStatus));
		lpSrvStatusInfo->liveTime = timeGetTime();

		m_stLstSrvStatusInfo.Lock();
		m_stLstSrvStatusInfo.push_back(lpSrvStatusInfo);
		m_stLstSrvStatusInfo.UnLock();
	
		CAppManager::GetInstance().GetAppNetWork().AddImServer(
									lpSrvStatusInfo->SrvStatus.srvID,
									lpSrvStatusInfo->SrvStatus.srvBaseInfo.dnsIP);
	
		if( NULL == m_pBestSrvInfo )
		{
			m_pBestSrvInfo = lpSrvStatusInfo;
		}

		if( m_pBestSrvInfo->SrvStatus.srvDynamicInfo.linkCount > lpSrvStatusInfo->SrvStatus.srvDynamicInfo.linkCount)
		{
			//得到当前最少连接数的服务器
			m_pBestSrvInfo = lpSrvStatusInfo;
		}

	}


	//额外添加,跟获取服务器列表业务无关,仅用于测试本IM服务器工作正常
	CAppManager::GetInstance().ResetProtectEvent();
}

