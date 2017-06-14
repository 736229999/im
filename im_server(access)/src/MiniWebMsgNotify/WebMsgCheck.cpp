#include "stdafx.h"
#include "WebMsgCheck.h"
#include "DataBaseLib.h"
#include "AppManager.h"
#include "Log.h"

#define TIMECHECKIOD (1000 * 10 * 1) //时钟周期 10 秒钟
#define MAX_CHECK_NUM 300  //每次获取条数
/*
CBusinessWebMsg::CBusinessWebMsg()
{

}

CBusinessWebMsg::~CBusinessWebMsg(void)
{
}


void CBusinessWebMsg::SendSystemLog(ST_SYSTEM_LOG *pSystemLog)
{

	uint8 dtype = 0;
	switch(pSystemLog->projID)
	{
	case ST_SYSTEM_LOG::ADD_FRIENDS:
	case ST_SYSTEM_LOG::DEL_FRIENDS:
		{
			dtype = XT_HEAD::DEST_TYPE_USER;
			//uint32 UserID = 0, LogID = 0;
			uint32 UserID = 0;
			char strUID[1024] = {0};
			strcpy(strUID,pSystemLog->strUserID);
			char *p = strUID;
			char *q = strUID;
			while(*p)
			{
				if(*p != ',')
				{
					p++;
					continue;
				}
				*p = '\0';
				p++;
				UserID = atoi(q);
				q = p;

				SystemLog(pSystemLog->recID,pSystemLog->projID,pSystemLog->operID,UserID,dtype);

			}

			UserID = atoi(q);

			SystemLog(pSystemLog->recID,pSystemLog->projID,pSystemLog->operID,UserID,dtype);
		}
		break;
	case ST_SYSTEM_LOG::MODIFY_FAMINAME:
	case ST_SYSTEM_LOG::MODIFY_FAMIFACEURI:
		{
			dtype = XT_HEAD::DEST_TYPE_FAMILY;
			uint32 familyid = atoi(pSystemLog->strLogID);
			SystemLog(pSystemLog->recID,pSystemLog->projID,familyid,0,dtype);
		}
		break;
	case ST_SYSTEM_LOG::ADD_FAMIMEM:
	case ST_SYSTEM_LOG::DEL_FAMIMEM:
		{
			dtype = XT_HEAD::DEST_TYPE_FAMILY;
			uint32 familyid = atoi(pSystemLog->strLogID);
			//uint32 UserID = 0, LogID = 0;
			uint32 UserID = 0;
			char strUID[1024] = {0};
			strcpy(strUID,pSystemLog->strUserID);
			char *p = strUID;
			char *q = strUID;
			while(*p)
			{
				if(*p != ',')
				{
					p++;
					continue;
				}
				*p = '\0';
				p++;
				UserID = atoi(q);
				q = p;

				SystemLog(pSystemLog->recID,pSystemLog->projID,familyid,UserID,dtype);

			}

			UserID = atoi(q);

			SystemLog(pSystemLog->recID,pSystemLog->projID,familyid,UserID,dtype);

		}
		break;
	default:
		dtype = XT_HEAD::DEST_TYPE_USER;
		break;
	}
}


void CBusinessWebMsg::SystemLog(uint32 recID,uint32 projID,uint32 operid,uint32 userid,uint8 dtype)
{
	SendDataStruct sendData;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

	char* pMsgData = sendData.data + sizeof(XT_HEAD);

	CSendPacket SendPacket;
	SendPacket.SetData(pMsgData,MAX_PROTOLPACKET_LEN);

	SendPacket	<< recID
		<< projID
		<< userid;
	sendData.data_len = (uint16)sizeof(XT_HEAD)  + SendPacket.GetSize();

	
	CAppManager::GetInstance().InitHead(
		lpSendHead,
		CMD_WEBSYSLOG_IND,
		(uint16)(SendPacket.GetSize()),
		operid,
		dtype);
	CAppManager::GetInstance().GetAppNetWork().SendDataTo(
		sendData.data,sendData.data_len,0);
}

void CBusinessWebMsg::SendWokuMsg(ST_WOKU_MSG *pMsg)
{
	SendDataStruct sendData;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

	char* pMsgData = sendData.data + sizeof(XT_HEAD);

	CSendPacket SendPacket;
	SendPacket.SetData(pMsgData,MAX_PROTOLPACKET_LEN);

	SendPacket	<< pMsg->type
		<< pMsg->recID
		<< pMsg->recvID
		<< pMsg->title
		<< pMsg->content
		<< pMsg->classID
		<< pMsg->logID
		<< pMsg->srcuri
		<< pMsg->sendID
		<< pMsg->username
		<< pMsg->nickname
		<< pMsg->sendTime
		<< pMsg->toids;

	sendData.data_len = (uint16)sizeof(XT_HEAD)  + SendPacket.GetSize();

	if (pMsg->type == ST_WOKU_MSG::SYSTEMPUB || pMsg->type == ST_WOKU_MSG::USERPUB) //公告，发给所有在线用户
	{
		list<uint32>::iterator it = CAppManager::GetInstance().GetServerList()->begin();
		for(;it !=  CAppManager::GetInstance().GetServerList()->end(); it++)
		{
			CAppManager::GetInstance().InitHead(
				lpSendHead,
				CMD_WOKU_IND,
				(uint16)(SendPacket.GetSize()),
				1,XT_HEAD::DEST_TYPE_SERVER);
			CAppManager::GetInstance().GetAppNetWork().SendDataTo(
				sendData.data,sendData.data_len,0,0, *it, PORT_IM_SVRP2P);
		}
	}
	else
	{
		CAppManager::GetInstance().InitHead(
		lpSendHead,
		CMD_WOKU_IND,
		(uint16)(SendPacket.GetSize()),
		pMsg->recvID);

		CAppManager::GetInstance().GetAppNetWork().SendDataTo(
			sendData.data,sendData.data_len,0);
	}
}


/////////////////////CWebMsgCheck.cpp//////////////////////
void  MySystemLogProc(void * dwSystemLog)
{
	CSystemLogCheck* pObj = (CSystemLogCheck*)dwSystemLog;
	pObj->HandleTimer();
}


void  MySystemLogProcNew(union sigval v)
{
	//CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG, "timer:8");
	MySystemLogProc(v.sival_ptr);
}



CSystemLogCheck::CSystemLogCheck()
{
	m_hMMT = NULL;
	m_Config.GetSysLogRec(m_nRecID);
	m_pSystemLogBuf	 = new ST_SYSTEM_LOG[MAX_CHECK_NUM];
}


CSystemLogCheck::~CSystemLogCheck(void)
{
	if( m_pSystemLogBuf )
	{
		delete[] m_pSystemLogBuf;
		m_pSystemLogBuf = NULL;
	}
	m_Config.SetSysLogRec(m_nRecID);
	StopWork();
}



int CSystemLogCheck::StartWork()
{
	char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
	m_Config.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
	m_pDataBaseWeb.SetConnectDB(webuid,webpwd,websid);
	if( !m_pDataBaseWeb.ConnectDB() )
	{
		CLog::Log("CSystemLogCheck",CLog::TYPE_ERROR,"DatabaseWeb.ConnectDB() failed!");
		return RESULT_FAILED;
	}

	StartTimer();
	return 0;
}

void CSystemLogCheck::StopWork()
{
	StopTiemr();
	m_Config.SetSysLogRec(m_nRecID);
}

void CSystemLogCheck::StartTimer()
{
	if( !timeSetEventNew(m_timer, 3*TIMECHECKIOD, TIMECHECKIOD, MySystemLogProcNew, (void*)this) )
	{
		CLog::Log("CSystemLogCheck", CLog::TYPE_ERROR, "StartTimerFailed");
	}
}

void CSystemLogCheck::StopTiemr()
{
	timer_delete(m_timer);
	
}


void CSystemLogCheck::HandleTimer()
{
	bool bDeal=false;
	uint16 nReqCount = MAX_CHECK_NUM;

	if ( DB_SUCCESS == m_pDataBaseWeb.Get_System_Log(m_nRecID,m_pSystemLogBuf,nReqCount) )
	{
		//CLog::Log("CSystemLogCheck",CLog::TYPE_NORMAL,"SystemLogID:%d",m_nRecID);
		for( int i = 0 ; i < nReqCount ; ++i )
		{
			m_WebMsg.SendSystemLog(&m_pSystemLogBuf[i]);
		}
		if(nReqCount>0) bDeal=true;
	}

	if(bDeal)
		m_Config.SetSysLogRec(m_nRecID);
}


/////////////////////CWebMsgCheck.cpp//////////////////////
void  MyOperationProc(void * dwOperation)
{
	COperationCheck* pObj = (COperationCheck*)dwOperation;
	pObj->HandleTimer();
}

void  MyOperationProcNew(union sigval v)
{
	//CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG, "timer:9");
	MyOperationProc(v.sival_ptr);
}




COperationCheck::COperationCheck()
{
	m_hMMT = NULL;
	m_Config.GetWebMsgID(m_nOperationID,m_nUserMsgID,m_nSysMsgID,m_nUserCommentID,m_nLogCommentID);
// 	m_nBroadcastID = 0;
// 	m_nUsercommentID = 0;
// 	m_nLogcommentID = 0;
	m_pOperationBuf	 = new ST_WOKU_MSG[MAX_CHECK_NUM];
}


COperationCheck::~COperationCheck(void)
{
	if( m_pOperationBuf )
	{
		delete[] m_pOperationBuf;
		m_pOperationBuf = NULL;
	}
	StopWork();
}



int COperationCheck::StartWork()
{
	char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
	m_Config.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
	m_pDataBaseWeb.SetConnectDB(webuid,webpwd,websid);
	if( !m_pDataBaseWeb.ConnectDB() )
	{
		CLog::Log("CWebMsgCheck",CLog::TYPE_ERROR,"DatabaseWeb.ConnectDB() failed!");
		return RESULT_FAILED;
	}

	StartTimer();
	return 0;
}

void COperationCheck::StopWork()
{
	StopTiemr();
	m_Config.SetWebMsgID(m_nOperationID,m_nUserMsgID,m_nSysMsgID,m_nUserCommentID, m_nLogCommentID);
}

void COperationCheck::StartTimer()
{
	if( !timeSetEventNew(m_timer, 2*TIMECHECKIOD, TIMECHECKIOD, MyOperationProcNew, (void*)this) )
	{
		CLog::Log("COperationCheck",CLog::TYPE_ERROR, "StartTimerFailed" );
	}
}

void COperationCheck::StopTiemr()
{
	timer_delete(m_timer);
}

#define TOID_LEN 512
void COperationCheck::HandleTimer()
{
	bool bDeal=false;
	uint16 nReqCount = MAX_CHECK_NUM;
#if 0
	memset(m_pOperationBuf, 0, sizeof(m_pOperationBuf) * MAX_CHECK_NUM);
	if ( DB_SUCCESS == m_pDataBaseWeb.Get_Operation_Msg(m_nOperationID,m_pOperationBuf,nReqCount) )
	{
		for( int i = 0 ; i < nReqCount ; ++i )
		{
			m_WebMsg.SendWokuMsg(&m_pOperationBuf[i]);
		}
		if( nReqCount>0 ) bDeal=true;
	}
	#ifdef DEBUG
	CLog::Log("WebMsgNotify",CLog::TYPE_NORMAL,"OperationID:%d",m_nOperationID);
	#endif
#endif
	//memset(m_pOperationBuf, 0, sizeof(m_pOperationBuf) * MAX_CHECK_NUM);
	//nReqCount = MAX_CHECK_NUM;
	if ( DB_SUCCESS == m_pDataBaseWeb.Get_Sys_Msg(m_nSysMsgID,m_pOperationBuf,nReqCount) )
	{
		for( int i = 0 ; i < nReqCount ; ++i )
		{
			m_WebMsg.SendWokuMsg(&m_pOperationBuf[i]);
		}
		if( nReqCount>0 ) bDeal=true;
	}
	
	CLog::Log("WebMsgNotify",CLog::TYPE_IMPORTANT," SystemMsgID:%d ",m_nSysMsgID);
	
#if 0
	memset(m_pOperationBuf, 0, sizeof(m_pOperationBuf) * MAX_CHECK_NUM);
	nReqCount = MAX_CHECK_NUM;
	if ( DB_SUCCESS == m_pDataBaseWeb.Get_UserComment(m_nUserCommentID,m_pOperationBuf,nReqCount) )
	{
		for( int i = 0 ; i < nReqCount ; ++i )
		{
			m_WebMsg.SendWokuMsg(&m_pOperationBuf[i]);
		}
		if( nReqCount>0 ) bDeal=true;
	}
	#ifdef DEBUG
	CLog::Log("WebMsgNotify",CLog::TYPE_NORMAL,"UserCommentMsgID:%d",m_nUserCommentID);
	#endif
	memset(m_pOperationBuf, 0, sizeof(m_pOperationBuf) * MAX_CHECK_NUM);
	nReqCount = MAX_CHECK_NUM;
	if ( DB_SUCCESS == m_pDataBaseWeb.Get_LogComment(m_nLogCommentID,m_pOperationBuf,nReqCount) )
	{
		for( int i = 0 ; i < nReqCount ; ++i )
		{
			m_WebMsg.SendWokuMsg(&m_pOperationBuf[i]);
		}
		if( nReqCount>0 ) bDeal=true;
	}
	#ifdef DEBUG
	CLog::Log("WebMsgNotify",CLog::TYPE_NORMAL,"LogCommentMsgID:%d",m_nLogCommentID);
	#endif
#endif
	if(bDeal)
		m_Config.SetWebMsgID(m_nOperationID,m_nUserMsgID,m_nSysMsgID,m_nUserCommentID, m_nLogCommentID);
}
*/

