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
*/
#if 0
void CUser::AddToCSQueueList()
{
	m_UserListLock.Lock();
	vector<CUser *> *tmp_list = m_userQueue.find(m_nID);
 	if (tmp_list != m_userQueue.end())
	{
		CLog::Log("CUser::AddToOnlineList()", CLog::TYPE_IMPORTANT,"列表中已经存在要添加的客服:%d！", m_nID);
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
		CLog::Log("CUser::AddToOnlineList()", CLog::TYPE_IMPORTANT,"列表中已经存在要添加的客服:%d！", m_nID);
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
		CLog::Log("CUser::DelFromOnlineList()",CLog::TYPE_IMPORTANT, "列表中不存在要删除的客服:%d！", m_nID);
	}
	else
	{
		vector<CUser *>::iterator Iter;	
		for(Iter = tmp_list->begin(); Iter != tmp_list.end(); Iter++) 
		{ 
		  if((*Iter).m_nID == m_nID)
		  { 
			 tmp_list.erase(Iter);
			 Iter = tmp_list.begin(); //当erase后，旧的容器会被重新整理成一个新的容器
		  } 
		}		
	}
	m_UserListLock.Unlock();
}
#endif

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
	
	m_nflow = 0;			//用户分流方式
		//详细资料
	m_nCountry = 0; 							//国家ID
	m_nProvince = 0;							//省份ID
	m_nCity = 0;								//城市ID
	m_nCareer = 0;								//职业ID

	m_nMoney = 0;								//用户虚拟货币值
//	uint8	m_nLevel;								//用户等级
	m_nOnlineTime = 0;							//用户累计在线时长
	m_linkpop = 0;								//联系方式隐私属性（0 私有;1 好友可见；3 公开）

	m_szUserSign[0] = '\0'; 	//个人签名	
	m_szAvatoruri[0] = '\0';		//闪秀地址
	m_nMobileStatus = 0;						//绑定手机	0未绑定，1已绑定 2已绑定并且订制了业务
	m_nIntegral = 0;							//用户总积分
	m_nCSRecptionLimit = 0;
	
	//最后心跳时间

	m_nLoginType = 0; //参考用户登录请求 flag
	m_nServerIp = 0;			//所在服务器IP地址
	flag = 0;					 //登录方式
	m_nLevel = 0;				//用户等级
	m_nUsertype = 0;				//买家	 卖家  还是客服
	m_nQueueflag = 0;				//是否排队 0:不排队 1:排队中

	m_nCSTeamid = 0;				//作为买家时接待客服的组
	m_merchantid = 0;

	m_bMonitored = false;	// 客服是否被监控
	m_nChattingBuyerID = 0;		// 客服当前正在联系的买家（用于监控过程中，将正在联系的买家告知主管）
	m_nDirectorID = 0;
/*
	// 更新网络层内存
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


			//如果该好友是直连到本服务器,则直接修改其好友列表状态
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

//发送一个退出业务包
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
		//删除临时群???
		CTempFamilyManager& pTempFamilyManager = CAppManager::GetInstance().GetTempFamilyManager();
		CTempFamily *pFamily = pTempFamilyManager.Find(*ittmp);
		if (pFamily)
		{
			pFamily->DelMember(pGlobalBusi,m_nID,pGlobalFamilyBusi);
			CBusinessFamilyGlobalFunc familyGlobalFunc;

			//通知临时家族成员有人退出
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
					//通知成员业务服务器修改临时家族列表
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

void CUser::ClearWebClient()//清空当前web用户在客服上的上下文
{
	m_nCSuserid = 0;
	m_nCSTeamid = 0;
	m_nQueueflag = 0;

}


// 查找客服是否存在(先查本机，再查memcache)



// 获取客服正在被哪个主管监控
uint32 CUser::GetDirectorID()
{
	return m_nDirectorID;
}

// 设置客服正在被哪个主管监控
void CUser::SetDirectorID(uint32 nDirectorID)
{
	m_nDirectorID = nDirectorID;
	m_bMonitored = (m_nDirectorID != (uint32)-1 && m_nDirectorID != 0);		// 当设置的监控主管id不为-1时，表示客服正在被监控
}


bool CUser::g_cs_exist(uint32 cs_id, CGlobalBusi *glob_busi)
{
	if(cs_id < cs_Boundary || cs_id >work_Boundary)
	{
		return false;
	}
	
	tagGSRV_GET_ONLINE_MONITOR_ACK::UserInfo csinfo_reply;

	if(glob_busi->QueryCsMonitor(cs_id,csinfo_reply) != true)
	{	// 全局服务器中也没有
		return false;
	}		

	return true;
}

// 查找客服是否正在被监控，如果被监控，返回主管id(先查本机，再查memcache)
bool CUser::g_cs_ismonitored(uint32 cs_id, bool &monitored, uint32 &cs_master_id, CGlobalBusi *glob_busi)
{
	if(cs_id < cs_Boundary || cs_id >work_Boundary)
	{
		return false;
	}
	cs_master_id = (uint32)-1;
	
	tagGSRV_GET_ONLINE_MONITOR_ACK::UserInfo csinfo_reply;

	if(glob_busi->QueryCsMonitor(cs_id,csinfo_reply) != true)
	{	// 全局服务器中也没有
		return false;
	}		

	monitored = (csinfo_reply.directorID != (uint32)-1 && csinfo_reply.directorID != 0);
	if(monitored)
	{	// 正在被监控
		cs_master_id = csinfo_reply.directorID;
	}

	return true;
}

// 查找客服正在接待的买家(先查本机，再查memcache)
bool CUser::g_cs_get_chatting_buyer(uint32 cs_id, uint32 &chatting_buyer_id, CGlobalBusi *glob_busi)
{
	if(cs_id < cs_Boundary || cs_id >work_Boundary)
	{
		return false;
	}
	chatting_buyer_id = (uint32)-1;
	
	tagGSRV_GET_ONLINE_MONITOR_ACK::UserInfo csinfo_reply;

	if(glob_busi->QueryCsMonitor(cs_id,csinfo_reply) != true)
	{	// 全局服务器中也没有
		return false;
	}		

	chatting_buyer_id = csinfo_reply.chattingBuyerID;

	return true;
}

// 设置客服正在被谁监控(同时修改上下文及memcache)
bool CUser::g_cs_set_director(uint32 cs_id, uint32 cs_master_id, CGlobalBusi *glob_busi)
{
	if(cs_id < cs_Boundary || cs_id >work_Boundary)
	{
		return false;
	}
	// 现在本机查找
	CUserManager userManager(glob_busi);
	shared_ptr<CUser> clientService = userManager.Find(cs_id);
	if(clientService != NULL)
	{
		clientService->SetDirectorID(cs_master_id);
		glob_busi->UpdateCsMonitor(clientService.get());
	}
	
	return false;
}

// 设置客服正在联系的买家(同时修改上下文及memcache)
bool CUser::g_cs_set_chatting_buyer(uint32 cs_id, uint32 chattring_buyer_id, CGlobalBusi *glob_busi)
{
	if(cs_id < cs_Boundary || cs_id >work_Boundary)
	{
		return false;
	}
	// 现在本机查找
	CUserManager userManager(glob_busi);
	shared_ptr<CUser> clientService = userManager.Find(cs_id);
	if(clientService != NULL)
	{
		clientService->SetChattingBuyerID(chattring_buyer_id);		
		glob_busi->UpdateCsMonitor(clientService.get());
	}

	return false;
}

// 获取客服名字
bool CUser::g_cs_get_name(uint32 cs_id, char name[], CGlobalBusi *glob_busi)
{
/*
	// 现在本机查找
	ClientService * clientService = NULL;
	CSManager & csManager = CAppManager::GetInstance().GetCSManager();
	clientService= csManager.Find(cs_id);
	if(clientService != NULL)
	{
		strcpy(name, clientService->m_CSname);
		return true;
	}

	// 本机不存在，在memcache中查找
*/	
	if(cs_id < cs_Boundary || cs_id >work_Boundary)
	{
		return false;
	}
	tagGSRV_GET_ONLINE_MONITOR_ACK::UserInfo csinfo_reply;

	if(glob_busi->QueryCsMonitor(cs_id,csinfo_reply) != true)
	{	// 全局服务器中也没有
		return false;
	}		

	strcpy(name, csinfo_reply.username);

	return true;
}


