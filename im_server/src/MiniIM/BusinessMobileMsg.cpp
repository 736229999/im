/*

#include "stdafx.h"
#include "BusinessMobileMsg.h"
#include "RouterBusiness.h"
#include "DataBaseLib.h"
#include "UserManager.h"

CBusinessMobileMsg::CBusinessMobileMsg(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_MOBILE_IND,CMD_MOBILE_ACK)
{
}

CBusinessMobileMsg::~CBusinessMobileMsg(void)
{
}

int CBusinessMobileMsg::DoWork()
{
	CUserManager *pUserManager = m_pRouter->GetUserManager();
	shared_ptr<CUser> pUser = pUserManager->Find(m_pRecvHead->did);
	if(pUser == NULL)
		return RESULT_FAILED;
	
	SendDataStruct sendData;
	RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;
	memcpy(sendData.data, pRecData->data, pRecData->data_len);
	sendData.data_len = pRecData->data_len;
	sendData.vlink = pRecData->vlink;


	CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
		sendData.data,sendData.data_len,SEND_RETRY_TIME,0,0,0,sendData.vlink);


	return RESULT_SUCCESS;
}

int CBusinessMobileMsg::SendReply()
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
	m_pAppNetwork->SendDataTo( m_pRouter->GetGlobalBusi(),
		pSendData->data,
		pSendData->data_len,
		m_pSendData->retry_times,
		m_pSendData->time_interval,
		pRecvData->src_ip,pRecvData->port );

	return 0;
}

CBusinessMobileMsgAck::CBusinessMobileMsgAck(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_MOBILE_ACK,CMD_MOBILE_ACK)
{

}

CBusinessMobileMsgAck::~CBusinessMobileMsgAck(void)
{
}

void CBusinessMobileMsgAck::SetRowPacket(BYTE* pRowPacket,int nPacketLength)
{
	CBusiness::SetRowPacket(pRowPacket,nPacketLength);

	//初始化基本数据
	RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;
	m_pRecvHead = (XT_HEAD*)pRecData->data;
	m_RecvPacket.SetData(pRecData->data + sizeof(XT_HEAD) ,pRecData->data_len - sizeof(XT_HEAD));
}

int CBusinessMobileMsgAck::DoWork()
{
	ST_MOBILE_ACK ack;
	m_RecvPacket >> ack.recID;

	shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(m_pRecvHead->sid);
	if(pUser != NULL)
	{
		pUser->m_nMobileMsgID = ack.recID;
		m_pRouter->GetUserManager()->AddUserToRedis(pUser);
	}
	return RESULT_SUCCESS;
}

int CBusinessMobileMsgAck::SendReply()
{
	return RESULT_SUCCESS;
}

#define MAX_OFFLINE_MOBILEMSG_NUM 20 
CBusinessOfflineMobileMsg::CBusinessOfflineMobileMsg()
{
	m_pMobileMsgBuf	 = new ST_MOBILE_MSG[MAX_OFFLINE_MOBILEMSG_NUM];
}

CBusinessOfflineMobileMsg::~CBusinessOfflineMobileMsg()
{
	if( m_pMobileMsgBuf )
	{
		delete[] m_pMobileMsgBuf;
		m_pMobileMsgBuf = NULL;
	}
}

int CBusinessOfflineMobileMsg::GetOfflineMobileMsg(CUser *pUser,CDataBaseWeb* pDataBaseWeb)
{
	if(!pUser)
		return RESULT_FAILED;
	uint32 userid = pUser->m_nID;

	uint16 nReqCount = MAX_OFFLINE_MOBILEMSG_NUM;
	if ( DB_SUCCESS == pDataBaseWeb->Get_OneMobile_Msg(pUser->m_nMobileMsgID,userid,m_pMobileMsgBuf,nReqCount) )
	{
		for( int i = 0 ; i < nReqCount ; ++i )
		{
			SendOfflineMobileMsg(&m_pMobileMsgBuf[i]);
		}
	}
	return RESULT_SUCCESS;
}

void CBusinessOfflineMobileMsg::SendOfflineMobileMsg(ST_MOBILE_MSG *pMsg)
{
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
		sendData.data,sendData.data_len,SEND_RETRY_TIME);

}


CBusinessSendMobileMessage::CBusinessSendMobileMessage(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_SEND_MOBILE_REQ,CMD_SEND_MOBILE_ACK)
{

}


int CBusinessSendMobileMessage::DoWork()
{
	static string strError[10] = {
		"",
		"操作失败，请稍后再试！",
		"用户未绑定手机！",
		"对方手机号码错误！",
		"发送成功，本月免费短信已用完，本条短信已扣 0.1 金币！",
		"发送失败，本月免费短信已用完，金币余额不足，请登录http://pay.woku.com/充值！",
		"暂不支持联通号码！",
		"",
		"",
		""
	};
	
	shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find( m_pRecvHead->sid );
	if( pUser == NULL )
		return RESULT_FAILED;
	if(pUser->m_nMobileStatus == 0)
	{
	 	m_SendPacket << uint8(RESULT_FAILED)
			<< "您尚未绑定手机，请登录http://passport.woku.com/bind/index.aspx进行绑定!";
	 	return RESULT_SUCCESS;
	}
	

	ST_MOBILE_SEND mobileMsg;
	m_RecvPacket >> mobileMsg.recvID
				 >> mobileMsg.mobileNO 
				 >> mobileMsg.linkID
				 >> mobileMsg.bizType
				 //>> mobileMsg.bizID
				 >> mobileMsg.smsType
				 >> mobileMsg.content;
	mobileMsg.bizID = m_pRecvHead->sid;
	int len = (int)strlen(mobileMsg.content);
	bool bHasC = false;
	for(int i = 0; i < len; ++i)
	{
		if(mobileMsg.content[i] > 125 || mobileMsg.content[i] < 0)
		{
			bHasC = true;
			break;
		}
	}
	if (!bHasC)
	{
		if( len > 70)
		{
			m_SendPacket << (uint8)RESULT_FAILED
						 << "发送失败，短信长度不能超过70个字符!";

			return RESULT_SUCCESS;
		}
	}

	int dbret  = m_pRouter->GetDataBaseWeb()->Send_Mobile_Msg(mobileMsg.recvID,pUser->m_szNickName,&mobileMsg);

	ST_MOBILE_SEND_ACK ack;
	ack.ret = RESULT_SUCCESS;

	if(dbret != 0 && dbret != 4)
	{
		m_SendPacket << (uint8)RESULT_FAILED
					 << (char*)strError[dbret].c_str();

	}
	else
	{
		m_SendPacket << (uint8)RESULT_SUCCESS
					 << (uint8)dbret
					 << (char*)strError[dbret].c_str();
	}
	return RESULT_SUCCESS;
}
*/
