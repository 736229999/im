#include "stdafx.h"
#include "User.h"
#include "ClientProtocol.h"
#include "AppManager.h"
#include "BusinessFamilyManager.h"
#include "UserManager.h"
//CUser* CUser::m_pUserListHead = NULL;
//CSyncCritical CUser::m_UserListLock;
//__gnu_cxx::hash_map<uint32, CUser*> CUser::m_userhash;


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
/*
void CUser::AddToOnlineList()
{
	m_UserListLock.Lock();
 	if (m_userhash.find(m_nID) != m_userhash.end())
	{
		CLog::Log("CUser::AddToOnlineList()", CLog::TYPE_IMPORTANT,"�б����Ѿ�����Ҫ��ӵ��û�:%d��", m_nID);
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
		CLog::Log("CUser::DelFromOnlineList()",CLog::TYPE_IMPORTANT, "�б��в�����Ҫɾ�����û�:%d��", m_nID);
	}
	else
	{
		m_userhash.erase(m_nID);
	}
	m_UserListLock.Unlock();
}
*/
#if 0
void CUser::AddToCSQueueList()
{
	m_UserListLock.Lock();
	vector<CUser *> *tmp_list = m_userQueue.find(m_nID);
 	if (tmp_list != m_userQueue.end())
	{
		CLog::Log("CUser::AddToOnlineList()", CLog::TYPE_IMPORTANT,"�б����Ѿ�����Ҫ��ӵĿͷ�:%d��", m_nID);
	}
	else
	{	
		tmp_list->push_back(this);
		m_userQueue.insert(make_pair(m_nID, tmp_list));
	}
	m_UserListLock.Unlock();
}

void CUser::CheckCSQueueSize(uint32 userid)
{
	m_UserListLock.Lock();
	vector<CUser *> *tmp_list = m_userQueue.find(m_nID);
 	if (tmp_list != m_userQueue.end())
	{
		CLog::Log("CUser::AddToOnlineList()", CLog::TYPE_IMPORTANT,"�б����Ѿ�����Ҫ��ӵĿͷ�:%d��", m_nID);
	}
	else
	{	
		tmp_list->push_back(this);
		m_userQueue.insert(make_pair(m_nID, tmp_list));
	}
	m_UserListLock.Unlock();
}


void CUser::DelFromCSQueueList()
{
	m_UserListLock.Lock();	
	vector<CUser *> *tmp_list = m_userQueue.find(m_nID);
	if (tmp_list == m_userQueue.end())
	{
		CLog::Log("CUser::DelFromOnlineList()",CLog::TYPE_IMPORTANT, "�б��в�����Ҫɾ���Ŀͷ�:%d��", m_nID);
	}
	else
	{
		vector<CUser *>::iterator Iter;	
		for(Iter = tmp_list->begin(); Iter != tmp_list.end(); Iter++) 
		{ 
		  if((*Iter).m_nID == m_nID)
		  { 
			 tmp_list.erase(Iter);
			 Iter = tmp_list.begin(); //��erase�󣬾ɵ������ᱻ���������һ���µ�����
		  } 
		}		
	}
	m_UserListLock.Unlock();
}
#endif

//ʹ��һ���µ�CUser����ʱ������
void CUser::Reset()
{

	//���
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
	m_nCSuserid = 0;
	m_szGUID[0]	= '\0';
	m_isLogout = 0;

	memset(m_SessionKey, 0, sizeof(m_SessionKey));
	memset(m_P2PSessionKey, 0, sizeof(m_P2PSessionKey));
	
	m_PermFamilyMap.Lock();		//add by lwl ver:0101
	m_PermFamilyMap.clear();
	m_PermFamilyMap.UnLock();
	m_TempFamilyList.Lock();
	m_TempFamilyList.clear();
	m_TempFamilyList.UnLock();
	m_FriendList.Clear();
	
	m_nflow = 0;			//�û�������ʽ
		//��ϸ����
	m_nCountry = 0; 							//����ID
	m_nProvince = 0;							//ʡ��ID
	m_nCity = 0;								//����ID
	m_nCareer = 0;								//ְҵID

	m_nMoney = 0;								//�û��������ֵ
//	uint8	m_nLevel;								//�û��ȼ�
	m_nOnlineTime = 0;							//�û��ۼ�����ʱ��
	m_linkpop = 0;								//��ϵ��ʽ��˽���ԣ�0 ˽��;1 ���ѿɼ���3 ������

	m_szUserSign[0] = '\0'; 	//����ǩ��	
	m_szAvatoruri[0] = '\0';		//�����ַ
	m_nMobileStatus = 0;						//���ֻ�	0δ�󶨣�1�Ѱ� 2�Ѱ󶨲��Ҷ�����ҵ��
	m_nIntegral = 0;							//�û��ܻ���
	m_nCSRecptionLimit = 0;
	
	//�������ʱ��

	m_nLoginType = 0; //�ο��û���¼���� flag
	m_nServerIp = 0;			//���ڷ�����IP��ַ
	flag = 0;					 //��¼��ʽ
	m_nLevel = 0;				//�û��ȼ�
	m_nUsertype = 0;				//���	 ����  ���ǿͷ�
	m_nQueueflag = 0;				//�Ƿ��Ŷ� 0:���Ŷ� 1:�Ŷ���

	m_nCSTeamid = 0;				//��Ϊ���ʱ�Ӵ��ͷ�����
	m_merchantid = 0;

	m_bMonitored = false;	// �ͷ��Ƿ񱻼��
	m_nChattingBuyerID = 0;		// �ͷ���ǰ������ϵ����ң����ڼ�ع����У���������ϵ����Ҹ�֪���ܣ�
	m_nDirectorID = 0;
/*
	// ����������ڴ�
#ifdef ACCESS_SERVER	
	LPONLINE_USER_INFO pOnlineUser = CAppManager::GetInstance().GetAppNetWork().FindOnlineUser(m_nID);
	if( pOnlineUser != NULL )
	{
		pOnlineUser->newlylivetime = m_nNewlyLiveTime;
		pOnlineUser->merchantID = m_merchantid;
	}
#endif
*/
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
 
int CUser::NotifyFriendStatus(CGlobalBusi *pGlobalBusi)
{
	CUserManager userManager(pGlobalBusi);

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

			CAppManager::GetInstance().GetAppNetWork().SendDataTo(pGlobalBusi,
				sendData.data,sendData.data_len,SEND_RETRY_TIME);


			//����ú�����ֱ������������,��ֱ���޸�������б�״̬
			shared_ptr<CUser> pFriendUser = userManager.Find(pFriendInfo->id);
			if( pFriendUser != NULL )
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

int CUser::NotifyFriendInfo(CGlobalBusi *pGlobalBusi)
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

			CAppManager::GetInstance().GetAppNetWork().SendDataTo(pGlobalBusi,
				sendData.data,sendData.data_len,SEND_RETRY_TIME);
		}

	}

	return RESULT_SUCCESS;
}


int CUser::NotifyServerClose(CGlobalBusi *pGlobalBusi)
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

	CAppManager::GetInstance().GetAppNetWork().SendDataTo(pGlobalBusi,
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
		if(pFamily)	//�ڸ�ҵ���������
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
						
					CAppManager::GetInstance().GetAppNetWork().SendDataTo(pGlobalFamilyBusi,sendData.data,sendData.data_len,SEND_RETRY_TIME);

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
			CAppManager::GetInstance().GetAppNetWork().SendDataTo(pGlobalFamilyBusi,sendData.data,sendData.data_len,0);
		}
	}
}

//����һ���˳�ҵ���
void CUser::SendLogoutReq(CUser* pUser)
{

	RecvDataStruct RecvData;
	RecvData.data_len = sizeof(XT_HEAD) + sizeof(XT_LOGOUT);
	XT_HEAD* pHead	= (XT_HEAD*)RecvData.data;
	pHead->cmd		= CMD_LOGOUT;
	pHead->sid		= pUser->m_nID;

	XT_LOGOUT* pLogOut = (XT_LOGOUT*)(RecvData.data + sizeof(XT_HEAD));
	pLogOut->id = pUser->m_nID;
	pLogOut->authData = 0x00;

	CRouterProtocol::GetInstance()->RouterOnePacket(&RecvData);
}


void CUser::LogoutTempFamily(CMiniAppNetwork * pAppNetwork ,CGlobalBusi* pGlobalFamilyBusi, CGlobalBusi *pGlobalBusi)
{
	CUserManager userManager(pGlobalBusi);
	CAutoLock myLock(&m_TempFamilyList.m_lstLock);
	SafeList<uint32>::iterator ittmp = m_TempFamilyList.begin();
	for(;ittmp != m_TempFamilyList.end(); ++ittmp)
	{
		//ɾ����ʱȺ???
		CTempFamilyManager& pTempFamilyManager = CAppManager::GetInstance().GetTempFamilyManager();
		CTempFamily *pFamily = pTempFamilyManager.Find(*ittmp);
		if (pFamily)
		{
			pFamily->DelMember(pGlobalBusi,m_nID,pGlobalFamilyBusi);
			CBusinessFamilyGlobalFunc familyGlobalFunc;

			//֪ͨ��ʱ�����Ա�����˳�
			SafeList<uint32>::iterator it = pFamily->m_FamilyMemberList.begin();
			for(; it != pFamily->m_FamilyMemberList.end(); ++it)
			{
				familyGlobalFunc.TransitSetMemberMsg(pGlobalBusi,(*it), FAMILY_REQ_QUIT, m_nID, pFamily->m_FamilyID, pFamily->m_FamilyProp, "");
			}

			if (pFamily->GetCurMemberNum() <= 2)
			{
				//CAutoLock mytemplock(&pFamily->m_FamilyMemberList.m_lstLock);//del by lwl ver:0101
				for(SafeList<uint32>::iterator iter = pFamily->m_FamilyMemberList.begin(); 
					iter != pFamily->m_FamilyMemberList.end(); ++iter)
				{
					//֪ͨ��Աҵ��������޸���ʱ�����б�
					shared_ptr<CUser> pUser = userManager.Find(*iter);
					if (pUser != NULL)	
						pUser->Del_Temp_Family(pFamily->m_FamilyID);
					familyGlobalFunc.TransitSetMemberMsg(pGlobalBusi,(*iter), FAMILY_REQ_QUIT, (*iter), pFamily->m_FamilyID, pFamily->m_FamilyProp, "");
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
			CAppManager::GetInstance().GetAppNetWork().SendDataTo(pGlobalBusi,sendData.data,sendData.data_len,SEND_RETRY_TIME);
		}
	}
}


int CUser::getCsuserId()
{	
	return m_nCSuserid;
}

void CUser::setCsuserId(uint32 CSid)
{
	m_nCSuserid = CSid;
}

int CUser::getCsTeamId()
{	
	return m_nCSTeamid;
}
void CUser::setCsTeamId(uint32 CSTeamid)
{	
	m_nCSTeamid = CSTeamid;
}

void CUser::ClearWebClient()//��յ�ǰweb�û��ڿͷ��ϵ�������
{
	m_nCSuserid = 0;
	m_nCSTeamid = 0;
	m_nQueueflag = 0;

}


// ���ҿͷ��Ƿ����(�Ȳ鱾�����ٲ�memcache)



// ��ȡ�ͷ����ڱ��ĸ����ܼ��
uint32 CUser::GetDirectorID()
{
	return m_nDirectorID;
}

// ���ÿͷ����ڱ��ĸ����ܼ��
void CUser::SetDirectorID(uint32 nDirectorID)
{
	m_nDirectorID = nDirectorID;
	m_bMonitored = (m_nDirectorID != (uint32)-1 && m_nDirectorID != 0);		// �����õļ������id��Ϊ-1ʱ����ʾ�ͷ����ڱ����
}


bool CUser::g_cs_exist(uint32 cs_id, CGlobalBusi *glob_busi)
{
	if(cs_id < cs_Boundary || cs_id >work_Boundary)
	{
		return false;
	}
	
	tagGSRV_GET_ONLINE_MONITOR_ACK::UserInfo csinfo_reply;

	if(glob_busi->QueryCsMonitor(cs_id,csinfo_reply) != true)
	{	// ȫ�ַ�������Ҳû��
		return false;
	}		

	return true;
}

// ���ҿͷ��Ƿ����ڱ���أ��������أ���������id(�Ȳ鱾�����ٲ�memcache)
bool CUser::g_cs_ismonitored(uint32 cs_id, bool &monitored, uint32 &cs_master_id, CGlobalBusi *glob_busi)
{
	if(cs_id < cs_Boundary || cs_id >work_Boundary)
	{
		return false;
	}
	cs_master_id = (uint32)-1;
	
	tagGSRV_GET_ONLINE_MONITOR_ACK::UserInfo csinfo_reply;

	if(glob_busi->QueryCsMonitor(cs_id,csinfo_reply) != true)
	{	// ȫ�ַ�������Ҳû��
		return false;
	}		

	monitored = (csinfo_reply.directorID != (uint32)-1 && csinfo_reply.directorID != 0);
	if(monitored)
	{	// ���ڱ����
		cs_master_id = csinfo_reply.directorID;
	}

	return true;
}

// ���ҿͷ����ڽӴ������(�Ȳ鱾�����ٲ�memcache)
bool CUser::g_cs_get_chatting_buyer(uint32 cs_id, uint32 &chatting_buyer_id, CGlobalBusi *glob_busi)
{
	if(cs_id < cs_Boundary || cs_id >work_Boundary)
	{
		return false;
	}
	chatting_buyer_id = (uint32)-1;
	
	tagGSRV_GET_ONLINE_MONITOR_ACK::UserInfo csinfo_reply;

	if(glob_busi->QueryCsMonitor(cs_id,csinfo_reply) != true)
	{	// ȫ�ַ�������Ҳû��
		return false;
	}		

	chatting_buyer_id = csinfo_reply.chattingBuyerID;

	return true;
}

// ���ÿͷ����ڱ�˭���(ͬʱ�޸������ļ�memcache)
bool CUser::g_cs_set_director(uint32 cs_id, uint32 cs_master_id, CGlobalBusi *glob_busi)
{
	if(cs_id < cs_Boundary || cs_id >work_Boundary)
	{
		return false;
	}
	// ���ڱ�������
	CUserManager userManager(glob_busi);
	shared_ptr<CUser> clientService = userManager.Find(cs_id);
	if(clientService != NULL)
	{
		clientService->SetDirectorID(cs_master_id);
		glob_busi->UpdateCsMonitor(clientService.get());
	}
	
	return false;
}

// ���ÿͷ�������ϵ�����(ͬʱ�޸������ļ�memcache)
bool CUser::g_cs_set_chatting_buyer(uint32 cs_id, uint32 chattring_buyer_id, CGlobalBusi *glob_busi)
{
	if(cs_id < cs_Boundary || cs_id >work_Boundary)
	{
		return false;
	}
	// ���ڱ�������
	CUserManager userManager(glob_busi);
	shared_ptr<CUser> clientService = userManager.Find(cs_id);
	if(clientService != NULL)
	{
		clientService->SetChattingBuyerID(chattring_buyer_id);		
		glob_busi->UpdateCsMonitor(clientService.get());
	}

	return false;
}

// ��ȡ�ͷ�����
bool CUser::g_cs_get_name(uint32 cs_id, char name[], CGlobalBusi *glob_busi)
{
/*
	// ���ڱ�������
	ClientService * clientService = NULL;
	CSManager & csManager = CAppManager::GetInstance().GetCSManager();
	clientService= csManager.Find(cs_id);
	if(clientService != NULL)
	{
		strcpy(name, clientService->m_CSname);
		return true;
	}

	// ���������ڣ���memcache�в���
*/	
	if(cs_id < cs_Boundary || cs_id >work_Boundary)
	{
		return false;
	}
	tagGSRV_GET_ONLINE_MONITOR_ACK::UserInfo csinfo_reply;

	if(glob_busi->QueryCsMonitor(cs_id,csinfo_reply) != true)
	{	// ȫ�ַ�������Ҳû��
		return false;
	}		

	strcpy(name, csinfo_reply.username);

	return true;
}


