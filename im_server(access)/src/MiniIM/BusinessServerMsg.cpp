#include "stdafx.h"
#include "BusinessServerMsg.h"
#include "RouterBusiness.h"
#include "DataBaseLib.h"
#include <time.h>
#include "ThreadMsgNotify.h"
#include "UserManager.h"

CBusinessServerMsg::CBusinessServerMsg(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_SERVERMSG_IND,CMD_SERVERMSG_IND)
{
	m_pServerMsgReq = NULL;
}

CBusinessServerMsg::~CBusinessServerMsg(void)
{
}

int CBusinessServerMsg::DoWork()
{ 
	m_pServerMsgReq = (XT_SERVER_TRANSMIT*)m_RecvPacket.GetData();
	//XT_SERVER_TRANSMIT*  pServerMsgAck = (XT_SERVER_TRANSMIT*)m_SendPacket.GetData();

	switch( m_pServerMsgReq->msgtype )
	{
	case XT_SERVER_TRANSMIT::TYPE_FRIEND_DEL:
		DelFriend();
		break;
	case XT_SERVER_TRANSMIT::TYPE_VERIFY_FRIEND:
		VerifyFriend();
		break;
	case CMD_FRIEND_STATUS_IND:
		FriendStatusChanged();
		break;
	case CMD_P2PMSG_SEND:
		P2PMessage();
		break;
	case CMD_CLUSTERMSG_SEND:
		break;
	case CMD_CLUSTERMSG_SEND_OFFLINE:
		FamilyMsg();
		break;
	case CMD_CDISMISS_REQ:
		FamilyDismissMsg();
		break;
	case CMD_CLUSTER_AUTH_REQ:
		FamilyVerifyMsg();
		break;
	case CMD_SET_CMEMBERS_REQ:
		FamilySetMemMsg();
		break;
	case CMD_WEB_IND:
		CLog::Log("CBusinessServerMsg",CLog::TYPE_DEBUG,"用户%d不在线,取消发送站内通知消息",m_pRecvHead->did);
		break;
	case CMD_SERVER_SEND_BUF_IND:
		SendMsgNotify();
		break;
	}

	shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(m_pRecvHead->did);
	//LPONLINE_USER_INFO lpOnlineUser = m_pAppNetwork->FindOnlineUser(m_pRecvHead->did);
#ifdef ACCESS_SERVER	
	if (pUser != NULL && m_pAppNetwork->FindOnlineUser(m_pRecvHead->did))
#else
	if (pUser != NULL && m_pAppNetwork->FindOnlineUser(m_pRouter->GetGlobalBusi(), m_pRecvHead->did))
#endif
		return RESULT_SUCCESS;
/*
	if (pUser != NULL)
	{
		pUser->DelFromOnlineList();
		m_pRouter->GetUserManager()->Del(pUser->m_nID);
#ifdef ACCESS_SERVER		
		m_pAppNetwork->UserLogout(m_pRecvHead->did);
#endif		
		CLog::Log("CBusinessServerMsg",CLog::TYPE_IMPORTANT,"用户%d不在线,取消应答",m_pRecvHead->did);
	}
	CancelReply();	
*/	

	return RESULT_SUCCESS;
}

int CBusinessServerMsg::SendReply()
{
	RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;

	m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),pRecData->data,
							  pRecData->data_len);
	return RESULT_SUCCESS;
}

void CBusinessServerMsg::DelFriend()
{
	tagXT_FRIENDDEL_NOTIFY* pFriendDelNotify = (XT_FRIENDADD_NOTIFY*)(m_RecvPacket.GetData() + sizeof(XT_SERVER_TRANSMIT) );
	shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(m_pRecvHead->did, TRUE);
	if( pUser != NULL )
	{
		pUser->m_FriendList.Del(pFriendDelNotify->ThatOneID);
	}
	else
	{
		CancelReply();
	}
}
//好友验证相关
void CBusinessServerMsg::VerifyFriend()
{
	XT_USERAUTH_REQ* pUserAuthReq = (XT_USERAUTH_REQ*)(m_RecvPacket.GetData() + sizeof(XT_SERVER_TRANSMIT));

	shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(pUserAuthReq->to_id, TRUE);
	if( XT_USERAUTH_REQ::ACTION_AGREE== pUserAuthReq->action )
	{
		//同意加为好友,则修改数据库与内存好友列表

		//数据库操作,加入好友列表
		CDataBaseWeb* pDatabase = m_pRouter->GetDataBaseWeb();
		if( DB_SUCCESS == pDatabase->Add_Friend(pUserAuthReq->to_id,pUserAuthReq->from_id,pUserAuthReq->groupId) )
		{
			//内存好友列表操作
			if( pUser != NULL )
			{
				FriendInfo* pFriendInfo = pUser->m_FriendList.Add(pUserAuthReq->from_id);
				if( pFriendInfo )
				{
					pFriendInfo->id		 = pUserAuthReq->from_id;
					pFriendInfo->GroupID = pUserAuthReq->groupId;	//默认加到我的好友组
					pFriendInfo->status  = XTREAM_ONLINE;

				}
			}
		}
		else
		{
			CancelReply();
			return;
		}

	}
	if(pUser != NULL)
		return;

	//不在线,保存到离线消息
	time_t current;
	time(&current);
	CRecvPacket rp((char*)m_RecvPacket.GetData() + sizeof(XT_SERVER_TRANSMIT), m_RecvPacket.GetSize()- sizeof(XT_SERVER_TRANSMIT));
	XT_MSG msg;
	memset(&msg,0,sizeof(msg));

	rp  >>msg.from_id
		>>msg.to_id
		>>msg.data_type
		>>msg.ver
		>>msg.send_time
		>>msg.recv_flag;
	if ( msg.data_type== XT_MSG::IM_MSG )
	{
		rp  >>msg.fontSize
			>>msg.fontColor
			>>msg.fontStyle;
		rp  >>msg.fontName;
	}

	rp.ReadData(msg.data,sizeof(msg.data) );

	if ( msg.data_type==XT_MSG::IM_MSG )
		rp>>msg.from_nickname;
#ifdef SAVE_OFFLINE	
	m_pRouter->GetDataBaseMini()->Save_User_Msg(pUserAuthReq->to_id,
					pUserAuthReq->from_id,
					0,
					0,
					(uint32)current,
					m_pServerMsgReq->msgtype,
					m_RecvPacket.GetData(),
					m_RecvPacket.GetSize(),
					msg.data);
#endif
	this->CancelReply();

}

//好友状态更改
void CBusinessServerMsg::FriendStatusChanged()
{
	XT_STATUS_IND* pFriendStatusNotify = (XT_STATUS_IND*)(m_RecvPacket.GetData() + sizeof(XT_SERVER_TRANSMIT) );
	shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(m_pRecvHead->did, TRUE);
	if( pUser != NULL )
	{
		FriendInfo* pFriendInfo = pUser->m_FriendList.Find(pFriendStatusNotify->fid);
		if( pFriendInfo )
		{
			pFriendInfo->status  = pFriendStatusNotify->status;
		}
		else
			this->CancelReply();
	}
	else
	{
		//不在线,不用通知
		this->CancelReply();
	}

}

void CBusinessServerMsg::P2PMessage()
{
	XT_SERVER_P2PMSG_SEND* pP2PMsgSend = (XT_SERVER_P2PMSG_SEND*)(m_RecvPacket.GetData() + sizeof(XT_SERVER_TRANSMIT) );
	shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(pP2PMsgSend->Recver);
	if(pUser != NULL)
		return;

	if( pP2PMsgSend->type == SERVER_P2PMSG_TYPE_MESSAGE ||
		pP2PMsgSend->type == SERVER_P2PMSG_TYPE_MYWORD	)
	{
		//离线消息保存
		time_t current;
		time(&current);
		CRecvPacket rp((char*)m_RecvPacket.GetData() + sizeof(XT_SERVER_TRANSMIT), m_RecvPacket.GetSize()- sizeof(XT_SERVER_TRANSMIT));
		XT_MSG msg;
		memset(&msg,0,sizeof(msg));

		rp  >>msg.from_id
			>>msg.to_id
			>>msg.data_type
			>>msg.ver
			>>msg.send_time
			>>msg.recv_flag;
		if ( msg.data_type== XT_MSG::IM_MSG )
		{
			rp  >>msg.fontSize
				>>msg.fontColor
				>>msg.fontStyle;
			rp  >>msg.fontName;
		}

		rp.ReadData(msg.data,sizeof(msg.data) );

		if ( msg.data_type==XT_MSG::IM_MSG )
			rp>>msg.from_nickname;
		
#ifdef SAVE_OFFLINE
		if(m_pAppNetwork->UserType(m_pRouter->GetGlobalBusi(),msg.from_id) == TYPE_WEB || m_pAppNetwork->UserType(msg.to_id) == TYPE_WEB)
		{
			

		}	
		else
		{
			m_pRouter->GetDataBaseMini()->
				Save_User_Msg(pP2PMsgSend->Recver,
				pP2PMsgSend->Sender,
				0,
				0,
				(uint32)current,
				m_pServerMsgReq->msgtype,
				m_RecvPacket.GetData(),
				m_RecvPacket.GetSize(),
				msg.data);
		}
#endif

	}

	//不在线,不用通知
	this->CancelReply();
}

void CBusinessServerMsg::FamilyMsg()
{
	shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(m_pRecvHead->did);
	if(pUser != NULL)
	{
		XT_FAMILY_SENDMSG_REQ Req;
		uint32 nMsgID;
		CRecvPacket pRecvPacket( (char*)m_RecvPacket.GetData() + sizeof(XT_SERVER_TRANSMIT), m_RecvPacket.GetSize() - sizeof(XT_SERVER_TRANSMIT));
		pRecvPacket>>nMsgID
				>>Req.familyID
				>>Req.senderID
				>>Req.version
				>>Req.familyProp
				>>Req.msgType;
		if (Req.familyProp == PERM_FAMILY)
			pUser->Set_Family_MsgID(Req.familyID, nMsgID);
	}
	else
	{
		LPXT_SERVER_TRANSMIT  lpTransmitHead = (LPXT_SERVER_TRANSMIT)(m_RecvPacket.GetData() );
		if (lpTransmitHead->origin_id &0x80000000 )
		{
			CTempFamily* pTempFamily = CAppManager::GetInstance().GetTempFamilyManager().Find(lpTransmitHead->origin_id);
			if (pTempFamily)
				pTempFamily->DelMember(m_pRouter->GetGlobalBusi(),m_pRecvHead->did, m_pRouter->GetGlobalFamilyBusi());				

		}
		else
		{
			CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
			CPermFamily *pFamily = pPermFamilyManager.Find(lpTransmitHead->origin_id);
			if (pFamily)
				pFamily->SetMemStat(m_pRecvHead->did, XTREAM_OFFLINE);
		}
	
		this->CancelReply();
	}
}

void CBusinessServerMsg::FamilyDismissMsg()
{
	shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(m_pRecvHead->did);
	if(pUser != NULL)
	{
		uint32 senderID, familyID;
		uint8  familyProp;
		CRecvPacket pRecvPacket( (char*)m_RecvPacket.GetData() + sizeof(XT_SERVER_TRANSMIT), m_RecvPacket.GetSize() - sizeof(XT_SERVER_TRANSMIT));
		pRecvPacket>>senderID
				>>familyID
				>>familyProp;
		if (familyProp == PERM_FAMILY)
			pUser->Del_Perm_Family(familyID);
		else if (familyProp == TEMP_FAMILY)
			pUser->Del_Temp_Family(familyID);

		if (m_pRecvHead->did == senderID)	//主动解散者不用接收通知消息
			this->CancelReply();
	}
	else
	{
		this->CancelReply();
	}
}

void CBusinessServerMsg::FamilyVerifyMsg()
{
	CRecvPacket pRecvPacket( (char*)m_RecvPacket.GetData() + sizeof(XT_SERVER_TRANSMIT), m_RecvPacket.GetSize() - sizeof(XT_SERVER_TRANSMIT));
	LPXT_CLUSTER_AUTH_REQ lpReq = (XT_CLUSTER_AUTH_REQ *)pRecvPacket.GetData();
	shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(m_pRecvHead->did);	//如果验证者不在线。保存数据库
	if(pUser != NULL)
	{	
		if ( FAMILY_JOIN_AGREE == lpReq->nAction )	//同意消息需要修改用户内存中的家族列表
				pUser->Add_Perm_Family(lpReq->nFamilyID, 0);
	}
	else
	{
		m_pRouter->GetDataBaseMini()->Save_Family_RequestMsg(lpReq->nFamilyID, lpReq->nRequestID, lpReq->send_time, pRecvPacket.GetData(), pRecvPacket.GetSize());
		this->CancelReply();
	}
}

void CBusinessServerMsg::FamilyMemJoinMsg()
{
	shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(m_pRecvHead->did);	//接收通知者不在线
	if(pUser != NULL)
		return;
	else
	{
		this->CancelReply();
	}
}
/*
void CBusinessServerMsg::FamilyApplyMsg()
{
	CUser* pUser = CAppManager::GetInstance().GetUserManager()->Find(m_pRecvHead->did);
	if(pUser)
	{
		uint32 actionID, familyID;
		uint8  action, familyProp;
		CRecvPacket pRecvPacket( (char*)m_RecvPacket.GetData() + sizeof(XT_SERVER_TRANSMIT), m_RecvPacket.GetSize() - sizeof(XT_SERVER_TRANSMIT));
		pRecvPacket>>action
				>>actionID
				>>familyID
				>>familyProp;

		if (familyProp == PERM_FAMILY && action == FAMILY_REQ_JOIN && actionID == m_pRecvHead->did)	//修改申请加入者的内存家族列表
		{
			pUser->Add_Perm_Family(familyID, 0);
			this->CancelReply();
		}

		if (familyProp == PERM_FAMILY && action == FAMILY_REQ_QUIT && actionID == m_pRecvHead->did)	//修改退出者的内存家族列表
		{
			pUser->Del_Perm_Family(familyID);
			this->CancelReply();
		}

		if (familyProp == TEMP_FAMILY && action == FAMILY_REQ_QUIT && actionID == m_pRecvHead->did)
		{
			pUser->Del_Temp_Family(familyID);
			this->CancelReply();
		}
	}
	else
	{
		this->CancelReply();
	}
}
*/
void CBusinessServerMsg::FamilySetMemMsg()
{
	shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(m_pRecvHead->did);
	if(pUser != NULL)
	{
		uint32 actionID, familyID,familyMsgID;
		uint8 action, familyProp;
		char familyName[MAX_FAMILYNAME_LEN + 1] = {0};
		CRecvPacket pRecvPacket( (char*)m_RecvPacket.GetData() + sizeof(XT_SERVER_TRANSMIT), m_RecvPacket.GetSize() - sizeof(XT_SERVER_TRANSMIT));
		pRecvPacket>>action
			>>actionID
			>>familyID
			>>familyName
			>>familyProp
			>>familyMsgID;
		if (familyProp == PERM_FAMILY && action == CMEMBER_ADD && actionID == m_pRecvHead->did)
			pUser->Add_Perm_Family(familyID, familyMsgID);

		if (familyProp == PERM_FAMILY && action == CMEMBER_DEL && actionID == m_pRecvHead->did)
			pUser->Del_Perm_Family(familyID);

		if (familyProp == TEMP_FAMILY && action == CMEMBER_ADD && actionID == m_pRecvHead->did)
			pUser->Add_Temp_Family(familyID);
		if (familyProp == TEMP_FAMILY && action == CMEMBER_DEL && actionID == m_pRecvHead->did)
			pUser->Del_Temp_Family(familyID);
	}
	else
	{
		this->CancelReply();
	}
}

// 通过服务器中转的消息推送
void CBusinessServerMsg::SendMsgNotify()
{
	shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(m_pRecvHead->did);
	if(pUser != NULL)
	{
		CLog::Log("CBusinessServerMsg::SendMsgNotify", CLog::TYPE_IMPORTANT, "用户[%u]在线", m_pRecvHead->did);
	
		CRecvPacket pRecvPacket( (char*)m_RecvPacket.GetData() + sizeof(XT_SERVER_TRANSMIT), m_RecvPacket.GetSize() - sizeof(XT_SERVER_TRANSMIT));
		char szData[2048] = {0};
		int nLength = pRecvPacket.ReadData(szData, 2047);
		
		Buf_ClientSend ack;
		if(!ack.ParseFromArray(szData, nLength))
		{
			CLog::Log("CBusinessServerMsg::SendMsgNotify", CLog::TYPE_ERROR, "PB反序列化失败");
			return;
		}

		CLog::Log("CBusinessServerMsg::SendMsgNotify", CLog::TYPE_IMPORTANT, "PB反序列化成功，发送给用户[%u]", m_pRecvHead->did);
		CThreadMsgNotify::SendToSeller(m_pRouter->GetGlobalBusi(), m_pRouter->GetDataBaseMini(), m_pRecvHead->did, ack);
	}
	else
	{
		CLog::Log("CBusinessServerMsg::SendMsgNotify", CLog::TYPE_ERROR, "用户[%u]不在线", m_pRecvHead->did);
	}
}



//---------------------------------------------------------------------------

//对客户端的应答,不需要进一步继续处理
CBusinessServerMsgAck::CBusinessServerMsgAck(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_SERVERMSG_IND_ACK,CMD_SERVERMSG_IND_ACK)
{

}

CBusinessServerMsgAck::~CBusinessServerMsgAck(void)
{
}

void CBusinessServerMsgAck::SetRowPacket(BYTE* pRowPacket,int nPacketLength)
{
	;
}

int CBusinessServerMsgAck::DoWork()
{
	return RESULT_SUCCESS;
}

int CBusinessServerMsgAck::SendReply()
{
	return RESULT_SUCCESS;
}
