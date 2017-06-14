/*
#include "stdafx.h"
#include "MobileMsgCheck.h"
#include "DataBaseLib.h"
#include "AppManager.h"
#include "Log.h"

#define MOBILECHECKIOD (1000 * 60 * 1) //时钟周期 
#define MAX_CHECK_NUM	300

CMobileMsgCheck::CMobileMsgCheck(void)
{
	m_hMMT = 0;
	m_Config.GetMobileRec(m_nRecID);

	m_pMobileMsgBuf	 = new ST_MOBILE_MSG[MAX_CHECK_NUM];

}

CMobileMsgCheck::~CMobileMsgCheck(void)
{
	StopWork();
	if(m_pMobileMsgBuf)
	{
		delete [] m_pMobileMsgBuf;
		m_pMobileMsgBuf = NULL;
	}
	m_Config.SetMobileRec(m_nRecID);
}


int CMobileMsgCheck::StartWork()
{
	char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
	m_Config.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
	m_pDataBaseWeb.SetConnectDB(webuid,webpwd,websid);
	if( !m_pDataBaseWeb.ConnectDB() )
	{
		CLog::Log("CMobileMsgCheck",CLog::TYPE_ERROR,"DatabaseWeb.ConnectDB() failed!");
		return RESULT_FAILED;
	}

	StartTimer();
	return 0;
}

void CMobileMsgCheck::StopWork()
{
	StopTiemr();
	m_Config.SetMobileRec(m_nRecID);

}

void MobileMsgProc(void* dwMobileMsg)
{
	CMobileMsgCheck* pObj = (CMobileMsgCheck*)dwMobileMsg;
	pObj->HandleTimer();
}

void MobileMsgProcNew( union sigval v)
{
	//CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG, "timer:6");
	MobileMsgProc(v.sival_ptr);
}

void CMobileMsgCheck::StartTimer()
{
//
//	m_hMMT = timeSetEvent(MOBILECHECKIOD,MOBILECHECKIOD,MobileMsgProc,(void *)this,TIME_PERIODIC);
//	if( NULL == m_hMMT )
//	{
//		CLog::Log("CMobileMsgCheck",CLog::TYPE_ERROR,"启动手机短信消息失败");
//	}
//

	if( !timeSetEventNew(m_timer, MOBILECHECKIOD, MOBILECHECKIOD, MobileMsgProcNew,(void *)this))
	{
		CLog::Log("CMobileMsgCheck", CLog::TYPE_ERROR, "启动手机短信消息失败");
	}
}

void CMobileMsgCheck::StopTiemr()
{
//
//	if( m_hMMT )
//	{
//		::timeKillEvent(m_hMMT);
//		m_hMMT = NULL;
//	}
//
	timer_delete(m_timer);
}


void CMobileMsgCheck::HandleTimer()
{
	bool bDeal;
	uint16 nReqCount = MAX_CHECK_NUM;

	if ( DB_SUCCESS == m_pDataBaseWeb.Get_Mobile_Msg(m_nRecID,m_pMobileMsgBuf,nReqCount) )
	{
#ifdef _DEBUG
		CLog::Log("CMobileMsgCheck",CLog::TYPE_NORMAL,"MobileMsgRecID:%d,nRetCount:%d!",m_nRecID,nReqCount);
#endif
		for( int i = 0 ; i < nReqCount ; ++i )
		{
			SendMobileMsg(&m_pMobileMsgBuf[i]);
		}
		if(nReqCount>0) bDeal=true;
	}

	if(bDeal)
		m_Config.SetMobileRec(m_nRecID);
}

void CMobileMsgCheck::SendMobileMsg(ST_MOBILE_MSG *pMsg)
{
#ifdef _DEBUG
	CLog::Log("GetMobileMsg","recid:%d,sendID:%d,nickname:%s,content:%s\n",pMsg->recID,pMsg->sendID,pMsg->nickname,pMsg->content);
#endif
	SendDataStruct sendData;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

	char* pMsgData = sendData.data + sizeof(XT_HEAD);

	CSendPacket SendPacket;
	SendPacket.SetData(pMsgData,MAX_PROTOLPACKET_LEN);

	SendPacket<< pMsg->recID
		<< pMsg->sendID
		<< pMsg->username
		<< pMsg->nickname
		<< pMsg->linkid
		<< pMsg->bizType
		<< pMsg->content
		<< pMsg->sendTime;

	sendData.data_len = (uint16)sizeof(XT_HEAD)  + SendPacket.GetSize();
	CAppManager::GetInstance().InitHead(
		lpSendHead,
		CMD_MOBILE_IND,
		(uint16)(SendPacket.GetSize()),
		pMsg->recvID);

	CAppManager::GetInstance().GetAppNetWork().SendDataTo(
		sendData.data,sendData.data_len,0);

}
*/
