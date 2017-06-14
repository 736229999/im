#include "stdafx.h"
#include "BusinessWebMsg.h"
#include "RouterBusiness.h"
#include "DataBaseLib.h"
#include <stdlib.h> 
#include <sstream>
#include <stdlib.h>

#define MAX_OFFLINE_WEBMSG_BUF 20

CBusinessWebMsg::CBusinessWebMsg(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_WOKU_IND,CMD_WOKU_ACK)
{
	
}

CBusinessWebMsg::~CBusinessWebMsg(void)
{

}

void CBusinessWebMsg::split(string& id, string& pattern, vector<string>* sub_id)
{
	string::size_type pos;
	
	id += pattern;
	size_t size = id.size();

	for(size_t i = 0; i < size; i ++)
	{
		pos = id.find(pattern,i);
		if(pos < size)
		{
			string s = id.substr(i, pos-i);
			sub_id->push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
}

int CBusinessWebMsg::DoWork()
{
	CUserManager& UserManager = CAppManager::GetInstance().GetUserManager();
	
	CUser* pUser = UserManager.Find(m_pRecvHead->did);
	
	ST_WOKU_MSG pMsg; 
	m_RecvPacket >> pMsg.type
				 >> pMsg.recID
				 >> pMsg.recvID
				 >> pMsg.title
				 >> pMsg.content
				 >> pMsg.classID
				 >> pMsg.logID
				 >> pMsg.srcuri
				 >> pMsg.sendID
				 >> pMsg.username
				 >> pMsg.nickname
				 >> pMsg.sendTime
				 >> pMsg.toids;
	
	if(pMsg.type == ST_WOKU_MSG::SYSTEMPUB)	//公告
	{
		CUser::m_UserListLock.Lock();

		uint32 did = 0;

		if(pMsg.toids != NULL)
			did = atoi(pMsg.toids);
		else
			return RESULT_FAILED;

		CUser* pmUser = NULL;
		__gnu_cxx::hash_map<uint32,CUser*>::iterator iter = CUser::m_userhash.find(did);;
		CUser* phUser = CAppManager::GetInstance().GetUserManager().Find(did);

		if(iter != CUser::m_userhash.end() && phUser != NULL)
		{
			pmUser = iter->second;
			if( pMsg.recID > pmUser->m_nSysMsgID)
			{
				pmUser->m_nSysMsgID = pMsg.recID;
				phUser->m_nSysMsgID = pMsg.recID;
			
				//转发该消息
				SendDataStruct sendData;
				CSendPacket sendPacket;

				sendPacket.SetData(sendData.data + sizeof(XT_HEAD),MAX_SEND_BUFFERS - sizeof(XT_HEAD));
				
				sendPacket<<pMsg.type
						  <<pMsg.recID
						  <<pMsg.recvID
						  <<pMsg.title
						  <<pMsg.content
						  <<pMsg.classID
						  <<pMsg.logID
						  <<pMsg.srcuri
						  <<pMsg.sendID
						  <<pMsg.username
						  <<pMsg.nickname
						  <<pMsg.sendTime;

				XT_HEAD *pHead = (XT_HEAD*)sendData.data;
				uint32 len = sendPacket.GetSize();

				CAppManager::GetInstance().InitHead(pHead,CMD_WOKU_IND,len,did);
				pHead->sid	= pMsg.sendID;

				sendData.data_len = len + sizeof(XT_HEAD);

				CAppManager::GetInstance().GetAppNetWork().SendDataTo(sendData.data,sendData.data_len,SEND_RETRY_TIME);
			}
		}
			
		CUser::m_UserListLock.Unlock();

		return RESULT_SUCCESS;
	}

	if (!pUser)
		return RESULT_FAILED;

	if(pMsg.type == ST_WOKU_MSG::USEROPERATION && m_pRecvHead->did == pMsg.sendID) //用户更新，发给好友！ 当目标ID与发送ID相同时表示要发给好友的，不同的表示直接发送出去的。
	{
		for(int i = 0; i < pUser->m_FriendList.GetSize(); ++i)
		{
			FriendInfo *finfo = pUser->m_FriendList.FindByPos(i);
			if(finfo && finfo->status != XTREAM_OFFLINE)
			{
				m_pRecvHead->did = finfo->id;
				if(m_pRecvHead->did == pMsg.sendID) //好友是自己
					continue;
				SendDataStruct sendData;
				RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;
				memcpy(sendData.data, pRecData->data, pRecData->data_len);
				sendData.data_len = pRecData->data_len;
				sendData.vlink = pRecData->vlink;
				CAppManager::GetInstance().GetAppNetWork().SendDataTo(
					sendData.data,sendData.data_len,0,0,0,0,sendData.vlink);
			}
		}
	}
	else 
	{
		//uint32 m_nOperationID,m_nUserMsgID,m_nSysMsgID,m_nMobileMsgID,m_nUsercommentID,m_nLogcommentID;
		//enum{USEROPERATION,USERRECVMSG,SYSTEMPUB,USERPUB,USERCOMMENT,LOGCOMMENT};
		if(pMsg.type == ST_WOKU_MSG::USEROPERATION)
		{
			if( pMsg.recID > pUser->m_nOperationID)
				pUser->m_nOperationID = pMsg.recID;
			else
				return RESULT_FAILED;
		}
		if(pMsg.type == ST_WOKU_MSG::USERRECVMSG)
		{
			if( pMsg.recID > pUser->m_nUserMsgID)
				pUser->m_nUserMsgID = pMsg.recID;
			else
				return RESULT_FAILED;
		}
		if(pMsg.type == ST_WOKU_MSG::USERCOMMENT)
		{
			if( pMsg.recID > pUser->m_nUsercommentID)
				pUser->m_nUsercommentID = pMsg.recID;
			else
				return RESULT_FAILED;
		}
		if(pMsg.type == ST_WOKU_MSG::LOGCOMMENT)
		{
			if( pMsg.recID > pUser->m_nLogcommentID)
				pUser->m_nLogcommentID = pMsg.recID;
			else
				return RESULT_FAILED;
		}
		
		SendDataStruct sendData;
		RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;
		memcpy(sendData.data, pRecData->data, pRecData->data_len);
		sendData.data_len = pRecData->data_len;
		sendData.vlink = pRecData->vlink;


		CAppManager::GetInstance().GetAppNetWork().SendDataTo(
			sendData.data,sendData.data_len,SEND_RETRY_TIME,0,0,0,sendData.vlink);

	}
	return RESULT_SUCCESS;
}

int CBusinessWebMsg::SendReply()
{
	RecvDataStruct* pRecvData = (RecvDataStruct*)m_RowPacketBuf;
	SendDataStruct* pSendData = m_pSendData;

	m_pSendHead->flag = XT_HEAD::FLAG_SRVACK;
	m_pSendHead->from = XT_HEAD::FROM_SERVER;
	m_pSendHead->sid  = m_pRecvHead->did;
	m_pSendHead->dtype= XT_HEAD::DEST_TYPE_SERVER;
	m_pSendHead->did	= m_pRecvHead->sid;

	m_pSendData->retry_times = 0;	
	m_pSendData->time_interval = 0;
	m_pSendData->vlink		 = pRecvData->vlink;

	//构建网络层的信息
	pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));

	//构建协议头的信息
	XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
	pSendHead->len  = m_SendPacket.GetSize();

	//发送数据
	assert(m_pAppNetwork);
	m_pAppNetwork->SendDataTo( 
		pSendData->data,
		pSendData->data_len,
		m_pSendData->retry_times,
		m_pSendData->time_interval,
		pRecvData->src_ip,pRecvData->port );

	return 0;
}

CBusinessWebMsgAck::CBusinessWebMsgAck(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_WOKU_ACK,CMD_WOKU_ACK)
{

}

CBusinessWebMsgAck::~CBusinessWebMsgAck(void)
{
}

void CBusinessWebMsgAck::SetRowPacket(BYTE* pRowPacket,int nPacketLength)
{
	CBusiness::SetRowPacket(pRowPacket,nPacketLength);

	//初始化基本数据
	RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;
	m_RecvPacket.SetData(pRecData->data + sizeof(XT_HEAD) ,pRecData->data_len - sizeof(XT_HEAD));
}

int CBusinessWebMsgAck::DoWork()
{
	ST_WOKU_ACK ack;
	m_RecvPacket >> ack.type
				 >> ack.recID
				 >> ack.recvID;

	//m_pRouter->GetDataBaseMini()->SetWokuMsgRecvID(ack.recvID,ack.type,ack.recID);
	if (ack.type == ST_WOKU_MSG::USERRECVMSG)
		m_pRouter->GetDataBaseWeb()->SetUserMessageStatus(ack.recID);
	return RESULT_SUCCESS;
}

int CBusinessWebMsgAck::SendReply()
{
	return RESULT_SUCCESS;
}

CBusinessOfflineWebMsg::CBusinessOfflineWebMsg()
{
	m_pOperationBuf	 = new ST_WOKU_MSG[MAX_OFFLINE_WEBMSG_BUF];
}

CBusinessOfflineWebMsg::~CBusinessOfflineWebMsg()
{
	if( m_pOperationBuf )
	{
		delete[] m_pOperationBuf;
		m_pOperationBuf = NULL;
	}
}

int CBusinessOfflineWebMsg::GetOfflineWokuMsg(CUser *pUser,char* strFriend,CDataBaseWeb* pDataBaseWeb,CDataBaseMini* pDataBaseMini)
{
	if(!pUser)
		return RESULT_FAILED;
	
	uint32 userid = pUser->m_nID;
	char uid[64] = {0};

	itoa(userid,uid,10);

#if 0
	uint16 nReqCount = MAX_OFFLINE_WEBMSG_BUF;
	if ( DB_SUCCESS == pDataBaseWeb->Get_OneOperation_Msg(pUser->m_nOperationID,strFriend,m_pOperationBuf,nReqCount) )
	{
		for( int i = 0 ; i < nReqCount ; ++i )
		{
			m_pOperationBuf[i].recvID = userid;
			SendOfflineWokuMsg(&m_pOperationBuf[i]);
		}
	}
	memset(m_pOperationBuf, 0, sizeof(m_pOperationBuf) * MAX_OFFLINE_WEBMSG_BUF);
	nReqCount = MAX_OFFLINE_WEBMSG_BUF;
	if ( DB_SUCCESS == pDataBaseWeb->Get_OneUser_Msg(pUser->m_nUserMsgID,userid,m_pOperationBuf,nReqCount) )
	{
		for( int i = 0 ; i < nReqCount ; ++i )
		{
			m_pOperationBuf[i].recvID = userid;
			SendOfflineWokuMsg(&m_pOperationBuf[i]);
		}
	}
#endif
	//memset(m_pOperationBuf, 0, sizeof(m_pOperationBuf) * MAX_OFFLINE_WEBMSG_BUF);
	uint16 nReqCount = MAX_OFFLINE_WEBMSG_BUF;
	if ( DB_SUCCESS == pDataBaseWeb->Get_SysOff_Msg(pUser->m_nSysMsgID,m_pOperationBuf,nReqCount,uid) )
	{
		for( int i = 0 ; i < nReqCount ; ++i )
		{
			m_pOperationBuf[i].recvID = userid;
			SendOfflineWokuMsg(&m_pOperationBuf[i]);
		}
	}
#if 0
	memset(m_pOperationBuf, 0, sizeof(m_pOperationBuf) * MAX_OFFLINE_WEBMSG_BUF);
	nReqCount = MAX_OFFLINE_WEBMSG_BUF;
	if ( DB_SUCCESS == pDataBaseWeb->Get_OneUserComment(pUser->m_nUsercommentID,userid,m_pOperationBuf,nReqCount) )
	{
		for( int i = 0 ; i < nReqCount ; ++i )
		{
			m_pOperationBuf[i].recvID = userid;
			SendOfflineWokuMsg(&m_pOperationBuf[i]);
		}
	}
	memset(m_pOperationBuf, 0, sizeof(m_pOperationBuf) * MAX_OFFLINE_WEBMSG_BUF);
	nReqCount = MAX_OFFLINE_WEBMSG_BUF;
	if ( DB_SUCCESS == pDataBaseWeb->Get_OneLogComment(pUser->m_nLogcommentID,userid,m_pOperationBuf,nReqCount) )
	{
		for( int i = 0 ; i < nReqCount ; ++i )
		{
			m_pOperationBuf[i].recvID = userid;
			SendOfflineWokuMsg(&m_pOperationBuf[i]);
		}
	}
#endif
	return RESULT_SUCCESS;
}

void CBusinessOfflineWebMsg::SendOfflineWokuMsg(ST_WOKU_MSG *pMsg)
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
		<< pMsg->sendTime;

	sendData.data_len = (uint16)sizeof(XT_HEAD)  + SendPacket.GetSize();

	CAppManager::GetInstance().InitHead(
		lpSendHead,
		CMD_WOKU_IND,
		(uint16)(SendPacket.GetSize()),
		pMsg->recvID);

	CAppManager::GetInstance().GetAppNetWork().SendDataTo(
		sendData.data,sendData.data_len,SEND_RETRY_TIME);
}


CBusinessSystemLog::CBusinessSystemLog(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_WEBSYSLOG_IND,CMD_WEBSYSLOG_ACK)
{

}

CBusinessSystemLog::~CBusinessSystemLog(void)
{

}
int CBusinessSystemLog::DoWork()
{
	ST_SYSTEM_LOG pSystemLog; 
	uint32 UserID = 0;
	m_RecvPacket >> pSystemLog.recID
				 >> pSystemLog.projID
				 >> UserID;
	CUserManager& UserManager = CAppManager::GetInstance().GetUserManager();
	CPermFamilyManager& FamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
	CPermFamily * pFamily = NULL;
	CUser* pUser = NULL;	
	switch(pSystemLog.projID)
	{
// 	case ST_SYSTEM_LOG::MODIFY_NICKNAME:
// 		{
// 			pUser = UserManager.Find(m_pRecvHead->did);
// 			if (!pUser)
// 				return RESULT_FAILED;
// 		}
// 		break;
// 	case ST_SYSTEM_LOG::MODIFY_FACEURI:
// 		{
// 			pUser = UserManager.Find(m_pRecvHead->did);
// 			if (!pUser)
// 				return RESULT_FAILED;
// 
// 			//strcpy_s(pUser->m_szHeadURL, 
// 
// 		}
// 		break;
	case ST_SYSTEM_LOG::ADD_FRIENDS:
		{
			pUser = UserManager.Find(m_pRecvHead->did);
			if (!pUser)
				return RESULT_FAILED;
			pUser->m_FriendList.Add(UserID);
		}
		break;
	case ST_SYSTEM_LOG::DEL_FRIENDS:
		{
			pUser = UserManager.Find(m_pRecvHead->did);
			if (!pUser)
				return RESULT_FAILED;
			pUser->m_FriendList.Del(UserID);
		}
		break;

	case ST_SYSTEM_LOG::MODIFY_FAMIFACEURI:
	case ST_SYSTEM_LOG::MODIFY_FAMINAME:
		{
			pFamily = FamilyManager.Find(m_pRecvHead->did);
			if(!pFamily)
				return RESULT_FAILED;
			//从数据库获取家族新名称和描述，更改缓存。
			XT_GET_CINFO_ACK Ack;
			m_pRouter->GetDataBaseFamily()->Get_Family_Info(pFamily->m_FamilyID,&Ack);
			strcpy_s(pFamily->m_FamilyDesc, Ack.szFamilyDesc);
			strcpy_s(pFamily->m_FamilyName, Ack.szFamilyName);
		}
	case ST_SYSTEM_LOG::ADD_FAMIMEM:
		{
			pFamily = FamilyManager.Find(m_pRecvHead->did);
			if(!pFamily)
				return RESULT_FAILED;
			pFamily->AddMember(UserID,FAMILY_NORMAL,XTREAM_OFFLINE);
		}
		break;
	case ST_SYSTEM_LOG::DEL_FAMIMEM:
		{
			pFamily = FamilyManager.Find(m_pRecvHead->did);
			if(!pFamily)
				return RESULT_FAILED;
			pFamily->DelMember(UserID);
		}
		break;
	default:
	    break;
	}

	return RESULT_SUCCESS;
}

int CBusinessSystemLog::SendReply()
{
	RecvDataStruct* pRecvData = (RecvDataStruct*)m_RowPacketBuf;
	SendDataStruct* pSendData = m_pSendData;

	m_pSendHead->flag = XT_HEAD::FLAG_SRVACK;
	m_pSendHead->from = XT_HEAD::FROM_SERVER;
	m_pSendHead->sid  = m_pRecvHead->did;
	m_pSendHead->dtype= XT_HEAD::DEST_TYPE_SERVER;
	m_pSendHead->did	= m_pRecvHead->sid;

	m_pSendData->retry_times = 0;	
	m_pSendData->time_interval = 0;
	m_pSendData->vlink		 = pRecvData->vlink;

	//构建网络层的信息
	pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));

	//构建协议头的信息
	XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
	pSendHead->len  = m_SendPacket.GetSize();

	//发送数据
	assert(m_pAppNetwork);
	m_pAppNetwork->SendDataTo( 
		pSendData->data,
		pSendData->data_len,
		m_pSendData->retry_times,
		m_pSendData->time_interval,
		pRecvData->src_ip,pRecvData->port );

	return 0;
}

