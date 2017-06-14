#include "stdafx.h"
#include "User.h"
#include "ClientProtocol.h"
#include "AppManager.h"
#include "BusinessFamilyManager.h"
//CUser* CUser::m_pUserListHead = NULL;
CSyncCritical CUser::m_UserListLock;
__gnu_cxx::hash_map<uint32, CUser*> CUser::m_userhash;
CUser::CUser(void):
m_FriendList(MAX_FRIENDS_NUM)
{
// 	m_next  = NULL;
// 	m_pre = NULL;

	Reset();
}

CUser::~CUser(void)
{
	m_FriendList.Clear();
}

void CUser::AddToOnlineList()
{
	m_UserListLock.Lock();
 	if (m_userhash.find(m_nID) != m_userhash.end())
	{
		CLog::Log("CUser::AddToOnlineList()", CLog::TYPE_IMPORTANT,"列表中已经存在要添加的用户:%d！", m_nID);
	}
	else
	{
		m_userhash.insert(make_pair(m_nID, this));
	}
	m_UserListLock.Unlock();
}

void CUser::DelFromOnlineList()
{
	m_UserListLock.Lock();
	if (m_userhash.find(m_nID) == m_userhash.end())
	{
		CLog::Log("CUser::DelFromOnlineList()",CLog::TYPE_IMPORTANT, "列表中不存在要删除的用户:%d！", m_nID);
	}
	else
	{
		m_userhash.erase(m_nID);
	}
	m_UserListLock.Unlock();
}


//使用一个新的CUser对象时，调用
void CUser::Reset()
{

	//清空
// 	m_next  = NULL;
// 	m_pre = NULL;
	m_nNewlyLiveTime = timeGetTime();
	m_nLoginTime = 0;
	m_nStatus = XTREAM_OFFLINE;

	m_nID		= 0 ;
	m_FamilyFlag = 0;
	m_AllowRule	= 0;
	m_ClientVersion = 0 ;
	m_nSexy		= 2;
	m_szHeadURL[0]	= '\0';
	m_szNickName[0]	= '\0';
	m_szUserName[0] = '\0';
	m_szEmail[0]	= '\0';
	m_szBirthday[0] = '\0';
	m_szAddress[0]  = '\0';
	m_szPhone[0]	= '\0';
	m_szDescription[0]= '\0';
	m_bIsFullInfoValid = false;
	m_nLanIP	= 0;
	m_nLanPort	= 0;
	m_nIP		= 0;
	m_nPort		= 0;
	m_nOperationID	= 0;
	m_nUserMsgID	= 0;
	m_nSysMsgID		= 0;
	m_nMobileMsgID	= 0;
	m_nUsercommentID= 0;
	m_nLogcommentID = 0;
	
	m_PermFamilyMap.Lock();		//add by lwl ver:0101
	m_PermFamilyMap.clear();
	m_PermFamilyMap.UnLock();
	m_TempFamilyList.Lock();
	m_TempFamilyList.clear();
	m_TempFamilyList.UnLock();
	m_FriendList.Clear();

}

bool CUser::Del_Perm_Family(uint32 nFamilyID)
{
	CAutoLock mylock(&m_PermFamilyMap.m_mapLock);
	FamilyList_Type::iterator it = m_PermFamilyMap.find(nFamilyID);
	if (it != m_PermFamilyMap.end())
	{
		m_PermFamilyMap.erase(it);
		return RESULT_SUCCESS;
	}
	return RESULT_FAILED;
}

bool CUser::Add_Perm_Family(uint32 nFamilyID, uint32 nMsgID)
{
	CAutoLock mylock(&m_PermFamilyMap.m_mapLock);
	FamilyList_Type::iterator it = m_PermFamilyMap.find(nFamilyID);
	if (it == m_PermFamilyMap.end())
	{
		m_PermFamilyMap.insert(FamilyList_ValueType(nFamilyID, nMsgID));
		return RESULT_SUCCESS;
	}
	return RESULT_FAILED;
}
bool CUser::Add_Temp_Family(uint32 nFamilyID)
{
	CAutoLock mylock(&m_TempFamilyList.m_lstLock);
	m_TempFamilyList.push_back(nFamilyID);
	return RESULT_SUCCESS;
}

bool CUser::Del_Temp_Family(uint32 nFamilyID)
{
	CAutoLock mylock(&m_TempFamilyList.m_lstLock);
	SafeList<uint32>::iterator it = m_TempFamilyList.begin();
	for(; it != m_TempFamilyList.end(); ++it)
	{
		if ((*it) == nFamilyID)
		{
			m_TempFamilyList.erase(it);
			return RESULT_SUCCESS;
		}
	}
	return RESULT_FAILED;
}
 
int CUser::NotifyFriendStatus()
{
	FriendInfo* pFriendInfo;
	for(int i = 0 ;i < m_FriendList.GetSize(); ++i )
	{
		pFriendInfo = m_FriendList.FindByPos(i);
		if( pFriendInfo &&(pFriendInfo->status!=XTREAM_OFFLINE) && (pFriendInfo->id != m_nID ) )
		{

			SendDataStruct sendData;
			LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
			lpSendHead->cmd		 = CMD_SERVERMSG_IND;
			lpSendHead->did		 = pFriendInfo->id;
			lpSendHead->dtype	 = XT_HEAD::DEST_TYPE_USER;
			lpSendHead->flag	 = XT_HEAD::FLAG_SRVACTION;
			lpSendHead->from	 = XT_HEAD::FROM_SERVER;
			CAppManager::GetInstance().GetSendSeq(lpSendHead->seq);
			lpSendHead->sid		 = CAppManager::GetInstance().GetConfig().GetServerNo();	

			LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
			lpSrvTransmit->origin_id			= XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
			lpSrvTransmit->msgtype				= CMD_FRIEND_STATUS_IND;


			XT_STATUS_IND* pFriendStatusNotify = (XT_STATUS_IND*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) );
			pFriendStatusNotify->fid    = m_nID;
			pFriendStatusNotify->status = m_nStatus;

			sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_STATUS_IND);

			CAppManager::GetInstance().GetAppNetWork().SendDataTo(
				sendData.data,sendData.data_len,SEND_RETRY_TIME);


			//如果该好友是直连到本服务器,则直接修改其好友列表状态
			CUser* pFriendUser = CAppManager::GetInstance().GetUserManager().Find(pFriendInfo->id);
			if( pFriendUser )
			{
				FriendInfo* pFriendInfo = pFriendUser->m_FriendList.Find(m_nID);
				if( pFriendInfo )
				{
					pFriendInfo->status = m_nStatus;
				}
			}
		}

	}

	return RESULT_SUCCESS;

}

int CUser::NotifyFriendInfo()
{
	FriendInfo* pFriendInfo;
	for(int i = 0 ;i < m_FriendList.GetSize(); ++i )
	{
		pFriendInfo = m_FriendList.FindByPos(i);
		if( pFriendInfo &&(pFriendInfo->status!=XTREAM_OFFLINE) && (pFriendInfo->id != m_nID ) )
		{

			SendDataStruct sendData;
			LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
			lpSendHead->cmd		 = CMD_SERVERMSG_IND;
			lpSendHead->did		 = pFriendInfo->id;
			lpSendHead->dtype	 = XT_HEAD::DEST_TYPE_USER;
			lpSendHead->flag	 = XT_HEAD::FLAG_SRVACTION;
			lpSendHead->from	 = XT_HEAD::FROM_SERVER;
			CAppManager::GetInstance().GetSendSeq(lpSendHead->seq);
			lpSendHead->sid		 = CAppManager::GetInstance().GetConfig().GetServerNo();	

			LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
			lpSrvTransmit->origin_id			= XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
			lpSrvTransmit->msgtype				= CMD_FRIEND_INFO_IND;


			XT_INFO_IND* pFriendInfoNotify = (XT_INFO_IND*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) );
			pFriendInfoNotify->fid = m_nID;

			sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_INFO_IND);

			CAppManager::GetInstance().GetAppNetWork().SendDataTo(
				sendData.data,sendData.data_len,SEND_RETRY_TIME);
		}

	}

	return RESULT_SUCCESS;
}


int CUser::NotifyServerClose()
{
	SendDataStruct sendData;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
	lpSendHead->cmd		 = CMD_SERVERMSG_IND;
	lpSendHead->did		 = m_nID;
	lpSendHead->dtype	 = XT_HEAD::DEST_TYPE_USER;
	lpSendHead->flag	 = XT_HEAD::FLAG_SRVACTION;
	lpSendHead->from	 = XT_HEAD::FROM_SERVER;
	CAppManager::GetInstance().GetSendSeq(lpSendHead->seq);
	lpSendHead->sid		 = CAppManager::GetInstance().GetConfig().GetServerNo();

	LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
	lpSrvTransmit->origin_id			= XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
	lpSrvTransmit->msgtype				= XT_SERVER_TRANSMIT::TYPE_SERVER_CLOSE;


	CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT));
	sendPacket<<m_nID;

	sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(sendPacket.GetSize());

	CAppManager::GetInstance().GetAppNetWork().SendDataTo(
		sendData.data,sendData.data_len,SEND_RETRY_TIME);

	return RESULT_SUCCESS;
}
void CUser::Set_Family_MsgID(uint32 nFamilyID,uint32 nMsgID)
{
	FamilyList_Type::iterator it = m_PermFamilyMap.begin();
	for( ; it != m_PermFamilyMap.end(); ++it)
	{
		if( (*it).first == nFamilyID )
		{
			(*it).second = nMsgID;
			return;
		}
	}
	return;
}

void CUser::NotifyFamilyStatus(CMiniAppNetwork * pAppNetwork,CGlobalBusi* pGlobalFamilyBusi,CDataBaseMini* pDataBaseMini) //modify by lwl ver:0105
{
	if (m_PermFamilyMap.size() == 0)
		return; 
	CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
	CPermFamily *pFamily = NULL;
	CAutoLock mylock(&m_PermFamilyMap.m_mapLock);
	FamilyList_Type::iterator it = m_PermFamilyMap.begin();
	for(; it != m_PermFamilyMap.end(); ++it)
	{
		pFamily = pPermFamilyManager.Find((*it).first);
		if(pFamily)	//在该业务服务器上
		{
			pFamily->SetMemStat(m_nID, m_nStatus);
			bool bRelease = true;
			//CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);//del by lwl ver:0101
			FamilyMember_Map::iterator itor = pFamily->m_FamilyMembers.begin();
			for( ; itor != pFamily->m_FamilyMembers.end(); ++itor )
			{
				if ((*itor).second.nStat != XTREAM_OFFLINE)
				{
					bRelease = false;
					break;
				}
			}
			if (bRelease)
			{
				CBusinessFamilyGlobalFunc familyGlobalFunc;
				familyGlobalFunc.ReleaseFamilyInfo(pFamily->m_FamilyID,pAppNetwork,pGlobalFamilyBusi,pDataBaseMini);//modify by lwl ver:0105
			}
			#if 1//bwwan
			
			CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
			FamilyMember_Map::iterator iter; 
			for(iter = pFamily->m_FamilyMembers.begin(); iter != pFamily->m_FamilyMembers.end(); ++ iter)
			{
				if(iter->second.nStat != XTREAM_OFFLINE )
				{
					
					SendDataStruct sendData;
					LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
					lpSendHead->cmd 	 = CMD_SERVERMSG_IND;
					lpSendHead->did 	 = iter->first;
					lpSendHead->dtype	 = XT_HEAD::DEST_TYPE_USER;
					lpSendHead->flag	 = XT_HEAD::FLAG_SRVACTION;
					lpSendHead->from	 = XT_HEAD::FROM_SERVER;
					CAppManager::GetInstance().GetSendSeq(lpSendHead->seq);
					lpSendHead->sid 	 = CAppManager::GetInstance().GetConfig().GetServerNo();	
						
					LPXT_SERVER_TRANSMIT lpSrvTransmit	= (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
					lpSrvTransmit->origin_id			= XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
					lpSrvTransmit->msgtype				= CMD_CLUSTER_NOTIFY_REQ;
						
						
					XT_STATUS_IND* pFriendStatusNotify = (XT_STATUS_IND*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) );
					pFriendStatusNotify->fid	= m_nID;
					pFriendStatusNotify->status = m_nStatus;
						
					sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_STATUS_IND);
						
					CAppManager::GetInstance().GetAppNetWork().SendDataTo(sendData.data,sendData.data_len,SEND_RETRY_TIME);

				}
			}

			#endif
		}
		else
		{
			int len = 0;
			SendDataStruct sendData;
			LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
			CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD) );
			sendPacket<<m_nID<<m_nStatus;
			len += sendPacket.GetSize();	

			CAppManager::GetInstance().InitHead(lpSendHead,CMD_CLUSTER_NOTIFY_REQ,len,(*it).first, XT_HEAD::DEST_TYPE_FAMILY);
			sendData.data_len = len + (int)sizeof(XT_HEAD);
			CAppManager::GetInstance().GetAppNetWork().SendDataTo(sendData.data,sendData.data_len,0);
		}
	}
}

void CUser::LogoutTempFamily(CMiniAppNetwork * pAppNetwork ,CGlobalBusi* pGlobalFamilyBusi)
{
	CAutoLock myLock(&m_TempFamilyList.m_lstLock);
	SafeList<uint32>::iterator ittmp = m_TempFamilyList.begin();
	for(;ittmp != m_TempFamilyList.end(); ++ittmp)
	{
		//删除临时群???
		CTempFamilyManager& pTempFamilyManager = CAppManager::GetInstance().GetTempFamilyManager();
		CTempFamily *pFamily = pTempFamilyManager.Find(*ittmp);
		if (pFamily)
		{
			pFamily->DelMember(m_nID,pGlobalFamilyBusi);
			CBusinessFamilyGlobalFunc familyGlobalFunc;

			//通知临时家族成员有人退出
			SafeList<uint32>::iterator it = pFamily->m_FamilyMemberList.begin();
			for(; it != pFamily->m_FamilyMemberList.end(); ++it)
			{
				familyGlobalFunc.TransitSetMemberMsg((*it), FAMILY_REQ_QUIT, m_nID, pFamily->m_FamilyID, pFamily->m_FamilyProp, "");
			}

			if (pFamily->GetCurMemberNum() <= 2)
			{
				//CAutoLock mytemplock(&pFamily->m_FamilyMemberList.m_lstLock);//del by lwl ver:0101
				for(SafeList<uint32>::iterator iter = pFamily->m_FamilyMemberList.begin(); 
					iter != pFamily->m_FamilyMemberList.end(); ++iter)
				{
					//通知成员业务服务器修改临时家族列表
					CUser *pUser = CAppManager::GetInstance().GetUserManager().Find(*iter);
					if (pUser)	
						pUser->Del_Temp_Family(pFamily->m_FamilyID);
					familyGlobalFunc.TransitSetMemberMsg((*iter), FAMILY_REQ_QUIT, (*iter), pFamily->m_FamilyID, pFamily->m_FamilyProp, "");
				}

				familyGlobalFunc.ReleaseTempFamilyInfo(pFamily->m_FamilyID, pAppNetwork,pGlobalFamilyBusi);
			}
		}
		else
		{
			int len = 0;
			SendDataStruct sendData;
			LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
			CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD) );
			sendPacket<<*ittmp
					<<(uint8)TEMP_FAMILY
					<<(uint8)FAMILY_REQ_QUIT
					<<m_nID;
					
			len += sendPacket.GetSize();	

			CAppManager::GetInstance().InitHead(lpSendHead,CMD_CLUSTER_APPLY_REQ,len,*ittmp, XT_HEAD::DEST_TYPE_FAMILY);
			sendData.data_len = len + (int)sizeof(XT_HEAD);
			CAppManager::GetInstance().GetAppNetWork().SendDataTo(sendData.data,sendData.data_len,SEND_RETRY_TIME);
		}
	}
}

