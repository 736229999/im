#include "stdafx.h"
#include "BusinessOperationMsg.h"
#include "RouterBusiness.h"
#include "DataBaseLib.h"
#include "AppManager.h"
#include "ErrorCode.h"


CBusinessOperationMsg::CBusinessOperationMsg(CRouterBusiness* pRouter):
CBusiness(pRouter,CMD_WOKU_IND,CMD_WOKU_ACK)
{
	m_pDataBaseWeb = m_pRouter->GetDataBaseWeb();
	m_pAppNetwork = &(CAppManager::GetInstance().GetAppNetWork());
}

CBusinessOperationMsg::~CBusinessOperationMsg(void)
{

}
int CBusinessOperationMsg::DoWork()	//�����߲�����ʱ��ȡ������ѣ�֪ͨ�����Ѹ�����Ϣ
{
	RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;
	LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;
	CRecvPacket rp(pRecvData->data + sizeof(XT_HEAD),pRecvData->data_len - sizeof(XT_HEAD));
	
	ST_WOKU_MSG pMsg; 
	rp  >> pMsg.type
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
		>> pMsg.sendTime;


	if(pMsg.type == ST_WOKU_MSG::USEROPERATION) //�û����£��������ѣ�
	{
		if(pMsg.sendID != lpRecvHead->did) //���͸�Ŀ�꣬Ŀ���ֲ�����
			return RESULT_FAILED;

		CLog::Log("CBusinessOperationMsg",CLog::TYPE_NORMAL,"UserOperation-SendID:%d,RecvID:%d ���͸����Ѹ���~!!!!!!",pMsg.sendID,pMsg.recvID);
		const int nFriendNumInOnPacket = 300;

		XT_FRIENDLIST_ACK::tagFriendInfo friendInfo[nFriendNumInOnPacket] ;

		//�����ݿ��в�ѯ
		int nReturnNum = nFriendNumInOnPacket;
		if(DB_SUCCESS !=  m_pDataBaseWeb->Get_User_FriendList( pMsg.recvID,
			0,
			friendInfo,
			nReturnNum) )
				return RESULT_FAILED;
		uint32 idList[nFriendNumInOnPacket];
		GSRV_GET_USER_STATUS_ACK::tagUserStatus userStatus[nFriendNumInOnPacket];
		uint8	idcount=0; 
		int j =0;
		for( j = 0 ; j < nReturnNum; ++j)
		{
			idList[j] = friendInfo[j].friendId;
			userStatus[j].id = idList[j];
			userStatus[j].status = XTREAM_OFFLINE;
		}
		idcount = nReturnNum;
		if( RESULT_FAILED == m_pRouter->GetGlobalBusi()->QueryUserStatus(
			idList ,
			userStatus ,
			idcount ) )
		{
			return RESULT_FAILED;
		}

		//�����û�״̬
		for(int j =0; j < idcount; ++j)
		{
			if( friendInfo[j].friendId == userStatus[j].id)
			{
				friendInfo[j].status = userStatus[j].status;
			}
			else
			{
				continue;
			}
		}

		for(int i = 0; i < nReturnNum; ++i)
		{
			if (friendInfo[i].friendId != 0  && friendInfo[i].status != XTREAM_OFFLINE)
			{
				SendDataStruct sendData;
				RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;
				memcpy(sendData.data, pRecData->data, pRecData->data_len);
				XT_HEAD *pHead = (XT_HEAD*)sendData.data;
				pHead->did = friendInfo[i].friendId;
				if(pHead->did == pMsg.sendID) //�������Լ�
					continue;
				CLog::Log("CBusinessOperationMsg",CLog::TYPE_NORMAL,"SendOperation SendID:%d���͸����¸�RecvID:%d~!!!!!!",pMsg.sendID,pHead->did);
				sendData.data_len = pRecData->data_len;
				sendData.vlink = pRecData->vlink;
				m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),
					sendData.data,sendData.data_len,0,0,0,0,sendData.vlink);
			}
		}
	}
	return RESULT_SUCCESS;
}

int CBusinessOperationMsg::SendReply()
{
	return RESULT_SUCCESS;
}


CBusinessSystemLog::CBusinessSystemLog(CRouterBusiness* pRouter):
CBusiness(pRouter,CMD_WEBSYSLOG_IND,CMD_WEBSYSLOG_ACK)
{

}

CBusinessSystemLog::~CBusinessSystemLog(void)
{

}
int CBusinessSystemLog::DoWork()
{
	
	return RESULT_SUCCESS;
}

int CBusinessSystemLog::SendReply()
{
	return RESULT_SUCCESS;
}
/*
//�û������ߡ��ֻ����Ų��ô������û�����ʱ��ȡ
CBusinessMobileMsg::CBusinessMobileMsg(CRouterBusiness* pRouter):
CBusiness(pRouter,CMD_MOBILE_IND,CMD_MOBILE_ACK)
{

}

CBusinessMobileMsg::~CBusinessMobileMsg(void)
{

}
int CBusinessMobileMsg::DoWork()
{

	return RESULT_SUCCESS;
}

int CBusinessMobileMsg::SendReply()
{
	return RESULT_SUCCESS;
}
*/
