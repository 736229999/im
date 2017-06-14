#include "stdafx.h"
#include "GlobalBusi.h"
#include "Family.h"
#include "AppManager.h"
#include "BusinessFamilyManager.h"
#include "UserManager.h"

uint8 CPermFamily::m_FamilyProp = PERM_FAMILY;
uint8 CTempFamily::m_FamilyProp = TEMP_FAMILY;												//临时家族	TEMP_CLUSTER


CPermFamily::CPermFamily(void)
{
	Reset();
}

CPermFamily::~CPermFamily(void)
{
}
bool CPermFamily::IsManager(uint32 UserID)
{
	CAutoLock mylock(&m_FamilyMembers.m_mapLock);
	FamilyMember_Map::iterator it = m_FamilyMembers.find(UserID);
	if ( it != m_FamilyMembers.end() && (it->second).nProp == FAMILY_MANAGER)
		return RESULT_SUCCESS;
	return RESULT_FAILED;
}

bool CPermFamily::IsCreater(uint32 UserID)
{
	CAutoLock mylock(&m_FamilyMembers.m_mapLock);
	FamilyMember_Map::iterator it = m_FamilyMembers.find(UserID);
	if ( it != m_FamilyMembers.end() && (it->second).nProp == FAMILY_CREATER)
		return RESULT_SUCCESS;
	return RESULT_FAILED;
}

bool CPermFamily::IsMember(uint32 UserID)
{
	CAutoLock mylock(&m_FamilyMembers.m_mapLock);
	if (m_FamilyMembers.find(UserID) != m_FamilyMembers.end())
		return RESULT_SUCCESS;
	return RESULT_FAILED;
}

uint16 CPermFamily::GetMaxMemberNum()
{
	if (m_FamilyLimit <= 0 )
		return 300;
	return m_FamilyLimit;
}

uint16 CPermFamily::GetMaxManagerNum()
{

	if (m_FamilyType == FAMILY_VIP)
		return MAX_VIPFAMILYMAN_NUM;
	return MAX_PERMFAMILYMAN_NUM;

}

//获取家族当前管理员数
uint16 CPermFamily::GetCurManagerNum()
{
	CAutoLock mylock(&m_FamilyMembers.m_mapLock);
	uint16 nManagerNum = 0;
	FamilyMember_Map::iterator it = m_FamilyMembers.begin();
	for(; it != m_FamilyMembers.end(); ++it)
	{
		if ( (it->second).nProp == FAMILY_MANAGER)
			++nManagerNum;
	}

	return nManagerNum;
}
								
//获取家族当前所有成员数，包括Creater、Manager
uint16 CPermFamily::GetCurMemberNum()									
{
	CAutoLock mylock(&m_FamilyMembers.m_mapLock);
	return static_cast<int>(m_FamilyMembers.size());
}

//添加成员	可能是Creater、Manager、Normal
bool CPermFamily::AddMember(uint32 UserID, uint8 UserProp,uint8 UserStat)	
{
	CAutoLock mylock(&m_FamilyMembers.m_mapLock);

	if (m_FamilyMembers.find(UserID) != m_FamilyMembers.end())	//家族成员中已经包含该用户
		return RESULT_SUCCESS;

	if ( GetCurMemberNum() > GetMaxMemberNum())	
		return RESULT_FAILED;
	m_FamilyMembers[UserID].nProp = UserProp;
	m_FamilyMembers[UserID].nStat = UserStat;
	if (find(m_FamilyMemberList.begin(), m_FamilyMemberList.end(), UserID) == m_FamilyMemberList.end())
		m_FamilyMemberList.push_back(UserID);
	return RESULT_SUCCESS;
}

//添加成员	可能是Creater、Manager、Normal
bool CPermFamily::AddMember(char *Profile,uint32 UserID, uint8 UserProp,uint8 UserStat)	
{
	CAutoLock mylock(&m_FamilyMembers.m_mapLock);

	if (m_FamilyMembers.find(UserID) != m_FamilyMembers.end())	//家族成员中已经包含该用户
		return RESULT_SUCCESS;

	if ( GetCurMemberNum() > GetMaxMemberNum())	
		return RESULT_FAILED;
	m_FamilyMembers[UserID].nProp = UserProp;
	m_FamilyMembers[UserID].nStat = UserStat;
	if(strlen(Profile) > 0)
	{
		strcpy(m_FamilyMembers[UserID].nProfile,Profile);
	}
	if (find(m_FamilyMemberList.begin(), m_FamilyMemberList.end(), UserID) == m_FamilyMemberList.end())
		m_FamilyMemberList.push_back(UserID);
	return RESULT_SUCCESS;
}


uint8 CPermFamily::SaveFamilyMsg(uint32 familyMsgID,uint32 senderID, int tm, int msgType,int msgLen,const char *msg)
{
	CAutoLock mylock(&m_FamilyMsgMap.m_mapLock);
	if (m_FamilyMsgMap.find(familyMsgID) != m_FamilyMsgMap.end())
		return RESULT_SUCCESS;
	FAMILYMSG_DATA *pData;
	if (m_FamilyMsgMap.size() > FAMILE_MAX_MSGNUM)
	{
		pData = (*(m_FamilyMsgMap.begin())).second;
		m_FamilyMsgMap.erase(m_FamilyMsgMap.begin());
	}
	else
		pData = new FAMILYMSG_DATA;
	pData->familyID	= m_FamilyID;
	pData->familyMsgID	= familyMsgID;
	pData->senderID	= senderID;
	pData->tm			= tm;
	pData->msgType		= msgType;
	pData->msgLen		= msgLen;
	memcpy(pData->msg, msg, msgLen);
	m_FamilyMsgMap[familyMsgID] = pData;
	return RESULT_SUCCESS;
}

//获取大于minMsgID的离线消息
int CPermFamily::GetOfflineFamilyMsg(uint32 minMsgID,CDataBaseMini *pDataBaseMini)
{

	uint8 nCount = FAMILE_MAX_MSGNUM;
//	uint32 userMsgID = 0;

	ST_OFFLINE_MSG pOfflineMsgBuf[FAMILE_MAX_MSGNUM];


	if( DB_SUCCESS == pDataBaseMini->Get_Family_Offline_Msg(0,m_FamilyID,minMsgID ,pOfflineMsgBuf,nCount) )
	{
		for( int i = 0; i < nCount; ++i)
		{
			SaveFamilyMsg(pOfflineMsgBuf[i].msgid,pOfflineMsgBuf[i].sendID, pOfflineMsgBuf[i].tm, pOfflineMsgBuf[i].msgtype,pOfflineMsgBuf[i].msglen,pOfflineMsgBuf[i].msg);
		}
	}
	return RESULT_SUCCESS;
}

uint8 CPermFamily::GetFamilyMsg(uint32 &familyMsgID,uint32 userID, LPST_OFFLINE_MSG lpMsg, uint8 &nRetCount,CDataBaseMini *pDataBaseMini)
{
	if (familyMsgID >=m_FamilyMaxMsgID )
	{
		nRetCount = 0;
		return RESULT_SUCCESS;
	}

	uint32 userMaxMsgID = familyMsgID;
	if (userMaxMsgID <m_FamilyBufMinMsgID)
	{
		m_FamilyBufMinMsgID = userMaxMsgID;
		GetOfflineFamilyMsg(userMaxMsgID,pDataBaseMini);//need modify ver:0106 修改从数据库获取用户的离线消息ID 从userMaxMsgID ~ m_FamilyBufMinMsgID 
	}


	CAutoLock mylock(&m_FamilyMsgMap.m_mapLock);
	FamilyMsg_Map::iterator it = m_FamilyMsgMap.find(userMaxMsgID);
	if (it == m_FamilyMsgMap.end())
	{
		for(it = m_FamilyMsgMap.begin();it != m_FamilyMsgMap.end();++it)
		{
			if ((*it).first > familyMsgID)
			{
				break;
			}
		}
	}
	else
		++it; //从下一条离线消息发送

	int i=0;
	for(; it != m_FamilyMsgMap.end(); ++it)
	{
		if (i > nRetCount)
			break;
		FAMILYMSG_DATA *pData = it->second;
		lpMsg[i].recvID			= userID;
		lpMsg[i].msgid			= pData->familyMsgID;
		lpMsg[i].sendID			= pData->senderID;
		lpMsg[i].tm				= pData->tm;
		lpMsg[i].msgtype		= pData->msgType;
		lpMsg[i].msglen			= pData->msgLen;
		memcpy(lpMsg[i].msg, pData->msg, lpMsg[i].msglen);
		familyMsgID = pData->familyMsgID;         //更新用户的家族消息ID
		++i;
	}
	nRetCount = i;

	return RESULT_SUCCESS;
}

void CPermFamily::SendFamilyMsg(CGlobalBusi *pGlobalBusi, ST_OFFLINE_MSG* pOfflineMsg)
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

	CAppManager::GetInstance().GetAppNetWork().SendDataTo(pGlobalBusi,
		sendData.data,sendData.data_len,SEND_RETRY_TIME);

	return;
}

//删除成员，可能是Manager、Common
bool CPermFamily::DelMember(uint32 UserID)	
{
	CAutoLock mylock(&m_FamilyMembers.m_mapLock);
	if (m_FamilyMembers.erase(UserID) == 0)	//没有该成员
		return RESULT_FAILED;
	FamilyMember_List::iterator it = m_FamilyMemberList.begin();
	for(; it != m_FamilyMemberList.end(); ++it)
	{
		if ((*it) == UserID)
		{
            m_FamilyMemberList.erase(it);
			break;
		}
	}
	return RESULT_SUCCESS;
}

bool CPermFamily::SetManager(uint32 UserID)
{
	CAutoLock mylock(&m_FamilyMembers.m_mapLock);
	FamilyMember_Map::iterator it;
	it = m_FamilyMembers.find(UserID);
	if (it != m_FamilyMembers.end() && (it->second).nProp == FAMILY_NORMAL)	
	{
		if (GetCurManagerNum() < GetMaxManagerNum())
		{
			it->second.nProp = FAMILY_MANAGER;
			return RESULT_SUCCESS;
		}
	}
	return RESULT_FAILED;	//已经是管理员或成员不存在
}

bool CPermFamily::CancelManager(uint32 UserID)
{
	CAutoLock(&m_FamilyMembers.m_mapLock);
	FamilyMember_Map::iterator it;
	it = m_FamilyMembers.find(UserID);
	if (it != m_FamilyMembers.end() && it->second.nProp == FAMILY_MANAGER)
	{
		it->second.nProp = FAMILY_NORMAL;
		return RESULT_SUCCESS;
	}
	return RESULT_FAILED;	//不是管理员或成员不存在
}

void CPermFamily::Reset()
{
	m_FamilyID			= 0;
	m_FamilyName[0]		= '\0';
	m_FamilyDomain[0]	= '\0';
	m_FamilyDesc[0]		= '\0';
	m_JoinFlag			= 0	;
	m_FamilyType		= 0;
	m_FamilyClass		= 0;
	m_FamilyMaxMsgID	= 0;
	m_FamilyMinMsgID	= 0;
	m_FamilySavedMsgID  = 0;
	m_FamilyLimit		= 0;

	m_FamilyMembers.clear();
	m_FamilyMemberList.clear();
	m_FamilyMsgMap.clear();

}


CTempFamily::CTempFamily(void)
{
	Reset();
}

CTempFamily::~CTempFamily(void)
{
}

void CTempFamily::Reset()
{
	m_FamilyID = 0;
	m_FamilyMemberList.clear();
}
bool CTempFamily::DelMember(CGlobalBusi *pGlobalBusi, uint32 UserID, CGlobalBusi* pGlobalFamilyBusi)
{
	CAutoLock mylock(&m_FamilyMemberList.m_lstLock);
	SafeList<uint32>::iterator it;
	it = find(m_FamilyMemberList.begin(), m_FamilyMemberList.end(), UserID);
	if (it != m_FamilyMemberList.end())
	{
		m_FamilyMemberList.erase(it);
	}

	CBusinessFamilyGlobalFunc familyGlobalFunc;
	//通知临时家族成员有人退出
	it = m_FamilyMemberList.begin();
	for(; it != m_FamilyMemberList.end(); ++it)
	{
		if ((*it) != UserID)
			familyGlobalFunc.TransitSetMemberMsg(pGlobalBusi, (*it), FAMILY_REQ_QUIT, UserID, m_FamilyID, TEMP_FAMILY, "");
	}

	if (GetCurMemberNum() <= 2)
	{

		for(SafeList<uint32>::iterator it = m_FamilyMemberList.begin(); 
			it != m_FamilyMemberList.end(); ++it)
		{
			//通知成员业务服务器修改临时家族列表
			//CUser *pUser = CAppManager::GetInstance().GetUserManager()->Find(*it);
			//if (pUser)	
			//	pUser->Del_Temp_Family(m_FamilyID);
			familyGlobalFunc.TransitSetMemberMsg(pGlobalBusi, (*it), FAMILY_REQ_QUIT, (*it), m_FamilyID, TEMP_FAMILY, "");
		}

		//familyGlobalFunc.ReleaseTempFamilyInfo(m_FamilyID, &CAppManager::GetInstance().GetAppNetWork(),pGlobalFamilyBusi);
	}
	return RESULT_SUCCESS;
};

