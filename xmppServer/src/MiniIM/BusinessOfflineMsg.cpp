#include "stdafx.h"
#include "BusinessOfflineMsg.h"
#include "BusinessWebMsg.h"
#include "BusinessMobileMsg.h"
#include "RouterBusiness.h"
#include <time.h>


#define MAX_OFFLINE_PUBMSG_BUF 20
CBusinessOfflineMsg::CBusinessOfflineMsg(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_OFFLINEMSG_REQ,CMD_OFFLINEMSG_ACK)
{
	m_nMaxOfflineMsgNum = max(MAX_OFFLINEMSG_NUM,MAX_FAMILEMSG_NUM);
	m_pOfflineMsgBuf = new ST_OFFLINE_MSG[m_nMaxOfflineMsgNum];
	m_pPubMsgBuf	 = new ST_WEB_MSG[MAX_OFFLINE_PUBMSG_BUF];

}

CBusinessOfflineMsg::~CBusinessOfflineMsg(void)
{
	if( m_pOfflineMsgBuf )
	{
		delete[] m_pOfflineMsgBuf;
		m_pOfflineMsgBuf = NULL;
	}

	if( m_pPubMsgBuf )
	{
		delete[] m_pPubMsgBuf;
		m_pPubMsgBuf = NULL;
	}

}


int CBusinessOfflineMsg::DoWork()
{
	XT_OFFLINEMSG_REQ* pOfflineMsgReq = (XT_OFFLINEMSG_REQ*)m_RecvPacket.GetData();
	//XT_OFFLINEMSG_ACK* pOfflineMsgAck = (XT_OFFLINEMSG_ACK*)m_SendPacket.GetData();

	m_SendPacket << (uint8)RESULT_SUCCESS;


	//数据库操作
	uint8 nCount = m_nMaxOfflineMsgNum;
	CDataBaseMini* pDatabaseMini = m_pRouter->GetDataBaseMini();
	/* 离线消息统一由定时线程收取，by Newton,20110822
	if( DB_SUCCESS == pDatabaseMini->Get_User_Offline_Msg(pOfflineMsgReq->id,m_pOfflineMsgBuf,nCount) )
	{
		for( int i = 0; i < nCount; ++i)
		{
			SendOfflineMsg(&m_pOfflineMsgBuf[i]);
		}
	}
	*/
#if 0
	nCount = m_nMaxOfflineMsgNum;
	if( DB_SUCCESS == pDatabaseMini->Get_User_Offline_SysMsg(pOfflineMsgReq->id,m_pOfflineMsgBuf,nCount) )
	{
		for( int i = 0; i < nCount; ++i)
		{
			SendOfflineMsg(&m_pOfflineMsgBuf[i]);
		}
	}
#endif

	//added by lwl ver:0106
// 	CUser* pUser = CAppManager::GetInstance().GetUserManager().Find( pOfflineMsgReq->id );
// 	if( pUser )
// 	{
// 		SendOfflineFamilyMsg(pUser);
// 	}


	CUser* pUser = CAppManager::GetInstance().GetUserManager().Find( pOfflineMsgReq->id );
	if( !pUser )
		return RESULT_FAILED;
	{
		FamilyList_Type::iterator iterator;
		CAutoLock mylock(&pUser->m_PermFamilyMap.m_mapLock);	//add by lwl ver:0101
		for( iterator = pUser->m_PermFamilyMap.begin(); iterator != pUser->m_PermFamilyMap.end(); ++iterator)
		{
			nCount = m_nMaxOfflineMsgNum;
			if( DB_SUCCESS == pDatabaseMini->Get_Family_Offline_Msg(pOfflineMsgReq->id,iterator->first ,iterator->second ,m_pOfflineMsgBuf,nCount) )
			{
				for( int i = 0; i < nCount; ++i)
				{
					SendFamilyMsg(&m_pOfflineMsgBuf[i]);
				}
			}


			uint8 userProp;
			CDataBaseFamily* pDatabaseFamily = m_pRouter->GetDataBaseFamily();
			if ( DB_SUCCESS == pDatabaseFamily->Get_FamilyMem_Porp(iterator->first,pOfflineMsgReq->id,userProp) )
			{
				if( userProp == FAMILY_CREATER ||
					userProp == FAMILY_MANAGER   )
				{
					nCount = m_nMaxOfflineMsgNum;
					if( DB_SUCCESS == pDatabaseMini->Get_Family_Offline_SysMsg(pOfflineMsgReq->id,iterator->first ,m_pOfflineMsgBuf,nCount) )
					{
						for( int i = 0; i < nCount; ++i)
						{
							
							SendFamilyMsg(&m_pOfflineMsgBuf[i]);
						}
					}
				}

			}
		}

	}


	nCount = MAX_OFFLINE_PUBMSG_BUF;
	if ( DB_SUCCESS == pDatabaseMini->Get_Pub_Msg(pOfflineMsgReq->id,m_pPubMsgBuf,nCount) )
	{
		if (nCount > 0)
		{
			for( int i = 1 ; i < nCount ; ++i )
			{
				strcat_s(m_pPubMsgBuf[0].msg, "#");
				strcat_s(m_pPubMsgBuf[0].msg, m_pPubMsgBuf[i].msg);
			}
			SendPubMsg(pOfflineMsgReq->id,&m_pPubMsgBuf[0]);
		}
	}

	
	pDatabaseMini->Delete_All_Offline_Msg(pOfflineMsgReq->id);
	
	char strFriend[5000]={0};
	if (!pUser)
		return RESULT_SUCCESS;

	FriendInfo* pFriendInfo;

	for(int i = 0; i < pUser->m_FriendList.GetSize(); ++i)
	{	
		char szUserID[10] = {0};
		pFriendInfo = pUser->m_FriendList.FindByPos(i);
		if( pFriendInfo  && pFriendInfo->id != pOfflineMsgReq->id )
		{
			_itoa_s(pFriendInfo->id, szUserID, 10);
			strcat_s(strFriend,szUserID);
			if (i != pUser->m_FriendList.GetSize() -1)
			{
				if(strlen(strFriend) > 4990)
					break;				
				strcat_s(strFriend,",");
			}

		}
	}

 	CBusinessOfflineWebMsg businessWebmsg;
	businessWebmsg.GetOfflineWokuMsg(pUser, strFriend,m_pRouter->GetDataBaseWeb(),m_pRouter->GetDataBaseMini());

	CBusinessOfflineMobileMsg busMobileMsg;
	busMobileMsg.GetOfflineMobileMsg(pUser, m_pRouter->GetDataBaseWeb());
	return RESULT_SUCCESS;
}

void CBusinessOfflineMsg::SendOfflineFamilyMsg(CUser *pUser)
{
	uint8 nCount = 0;
	FamilyList_Type::iterator it;
	CDataBaseMini* pDatabaseMini = m_pRouter->GetDataBaseMini();
	CDataBaseFamily* pDatabaseFamily = m_pRouter->GetDataBaseFamily();
	CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
	CPermFamily *pFamily = NULL;
	uint32 familyInfo[MAX_PERMFAMILY_NUM];
	memset(familyInfo, 0, sizeof(familyInfo));
	CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalFamilyBusi();
	uint8 nFamilyNum = 0;

    CAutoLock mylock(&pUser->m_PermFamilyMap.m_mapLock);
	for( it = pUser->m_PermFamilyMap.begin(); it != pUser->m_PermFamilyMap.end(); ++it)
	{
		pFamily = pPermFamilyManager.Find((*it).first);

		if(!pFamily)	//家族不在该服务器上 需要全局查询
		{
			if(MAX_PERMFAMILY_NUM >= nFamilyNum)
			{
				familyInfo[nFamilyNum] = (*it).first;
				++nFamilyNum;
			}
		}
		else		//从本地服务器获取获消息发送
		{
			nCount = m_nMaxOfflineMsgNum;
			uint8 nRet = pFamily->GetFamilyMsg((*it).second,pUser->m_nID,m_pOfflineMsgBuf, nCount,m_pRouter->GetDataBaseMini());
			if (nRet == RESULT_SUCCESS && nCount > 0)
			{
				for(int i = 0; i < nCount; ++i)
				{
					SendFamilyMsg(&m_pOfflineMsgBuf[i]);
				}
			}
		}
		uint8 userProp;
		
		if ( DB_SUCCESS == pDatabaseFamily->Get_FamilyMem_Porp((*it).first,pUser->m_nID,userProp) )
		{
			if( userProp == FAMILY_CREATER ||
				userProp == FAMILY_MANAGER   )
			{
				nCount = m_nMaxOfflineMsgNum;
				if( DB_SUCCESS == pDatabaseMini->Get_Family_Offline_SysMsg(pUser->m_nID,(*it).first ,m_pOfflineMsgBuf,nCount) )
				{
					for( int i = 0; i < nCount; ++i)
					{

						SendFamilyMsg(&m_pOfflineMsgBuf[i]);
					}
				}
			}

		}
	}

	
	tagGSRV_FAMILYINFO_QUERY_ACK::tagFamilyInfo FStatus[MAX_PERMFAMILY_NUM];
	memset(FStatus, 0, sizeof(FStatus));
	nCount = nFamilyNum;
	if (RESULT_SUCCESS != pGlobalBusi->FindFamilyInfo(familyInfo, nCount, FStatus))
		return;

	for(int i = 0; i < nCount; ++i)
	{
		FamilyList_Type::iterator it;
		it = pUser->m_PermFamilyMap.find(familyInfo[i]);
		if (it == pUser->m_PermFamilyMap.end())
			continue;

		if (FStatus[i].fid == familyInfo[i] && FStatus[i].srvip > 0)	//向家族服务器发送获取离线消息
		{
			SendDataStruct sendData;
			LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

			CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD));
			sendPacket<<pUser->m_nID<<(*it).first<<(*it).second;

			CAppManager::GetInstance().InitHead(
				lpSendHead,
				CMD_FAMILYOFFLINEMSG_REQ,
				(uint16)sendPacket.GetSize(),
				familyInfo[i],
				XT_HEAD::DEST_TYPE_FAMILY);	

			sendData.data_len = (uint16)(sizeof(XT_HEAD) + sendPacket.GetSize());

			CAppManager::GetInstance().GetAppNetWork().SendDataTo(
				sendData.data,sendData.data_len,0);
		}
		else	//从数据库获取
		{
			uint8 nCountMsg = m_nMaxOfflineMsgNum;
			if( DB_SUCCESS == pDatabaseMini->Get_Family_Offline_Msg(pUser->m_nID,it->first ,it->second ,m_pOfflineMsgBuf,nCountMsg) )
			{
				for( int i = 0; i < nCountMsg; ++i)
				{
					SendFamilyMsg(&m_pOfflineMsgBuf[i]);
				}
			}
		}
	}

}


void CBusinessOfflineMsg::SendOfflineMsg(ST_OFFLINE_MSG* pOfflineMsg)
{
	SendDataStruct sendData;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

	CAppManager::GetInstance().InitHead(
		lpSendHead,
		CMD_SERVERMSG_IND,
		pOfflineMsg->msglen,
		pOfflineMsg->recvID);//should  be change

	//LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
	//lpSrvTransmit->origin_id			= XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
	//lpSrvTransmit->msgtype				= pOfflineMsg->msgtype;


	char* pMsgData = sendData.data + sizeof(XT_HEAD) ;
	memcpy(pMsgData,pOfflineMsg->msg,pOfflineMsg->msglen);

	//对特殊离线消息设置离线标志
	LPXT_SERVER_TRANSMIT lpSrvTransmit = (LPXT_SERVER_TRANSMIT)pMsgData;
	if( (lpSrvTransmit->msgtype == CMD_CLUSTERMSG_SEND) ||
		(lpSrvTransmit->msgtype == CMD_P2PMSG_SEND)		)
	{
		lpSrvTransmit->msgtype |= 0x1000;
	}

	sendData.data_len = sizeof(XT_HEAD) + pOfflineMsg->msglen;

	CAppManager::GetInstance().GetAppNetWork().SendDataTo(
		sendData.data,sendData.data_len,SEND_RETRY_TIME);

}

void CBusinessOfflineMsg::SendFamilyMsg(ST_OFFLINE_MSG* pOfflineMsg)
{
	SendDataStruct sendData;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

	CAppManager::GetInstance().InitHead(
		lpSendHead,
		CMD_SERVERMSG_IND,
		pOfflineMsg->msglen,
		pOfflineMsg->recvID);//should  be change

	LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
	lpSrvTransmit->origin_id			= XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
	lpSrvTransmit->msgtype				= pOfflineMsg->msgtype;

	CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT));
	if(lpSrvTransmit->msgtype == CMD_CLUSTERMSG_SEND)
	{
		lpSrvTransmit->msgtype = CMD_CLUSTERMSG_SEND_OFFLINE;
		sendPacket<<(uint32)pOfflineMsg->msgid;
	}
	sendPacket.WritePacket(pOfflineMsg->msg, pOfflineMsg->msglen);


	sendData.data_len = uint16(sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sendPacket.GetSize() );

	CAppManager::GetInstance().GetAppNetWork().SendDataTo(
		sendData.data,sendData.data_len,SEND_RETRY_TIME);
}

void CBusinessOfflineMsg::SendPubMsg(uint32 nRcvId,ST_WEB_MSG* pWebMsg)
{
	SendDataStruct sendData;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

	LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
	lpSrvTransmit->origin_id			= XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
	lpSrvTransmit->msgtype				= XT_SERVER_TRANSMIT::TYPE_SYS_MESSAGE;


	char* pMsgData = sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT);
	CSendPacket SendPacket;
	SendPacket.SetData(pMsgData,MAX_PROTOLPACKET_LEN - sizeof(XT_SERVER_TRANSMIT) );
	
	//得到时间
	string strTime = pWebMsg->sendTime;
	tm tmSend;
	tmSend.tm_year	= atoi(strTime.substr(0,4).c_str()) - 1900 ;
	tmSend.tm_mon	= atoi(strTime.substr(5,2).c_str()) - 1 ;
	tmSend.tm_mday	= atoi(strTime.substr(8,2).c_str());
	tmSend.tm_hour	= atoi(strTime.substr(11,2).c_str());
	tmSend.tm_min	= atoi(strTime.substr(15,2).c_str());
	tmSend.tm_sec	= atoi(strTime.substr(17,2).c_str());
	time_t ttSend = mktime(&tmSend);

	SendPacket << (uint32)ttSend
			   << (uint8)3			//type:HTML
			   << pWebMsg->msg;


	sendData.data_len = uint16(sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + SendPacket.GetSize());

	CAppManager::GetInstance().InitHead(
		lpSendHead,
		CMD_SERVERMSG_IND,
		(uint16)(sizeof(XT_SERVER_TRANSMIT) + SendPacket.GetSize()),
		nRcvId);

	CAppManager::GetInstance().GetAppNetWork().SendDataTo(
		sendData.data,sendData.data_len,SEND_RETRY_TIME);
}

