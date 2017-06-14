#include "stdafx.h"
#include <time.h>
#include "BusinessFamilyManager.h"
#include "AppManager.h"
#include "RouterBusiness.h"
#include "protocol.h"
#include "ClientProtocol.h"

#include "BusiCacheInterface.h"
#include "UserManager.h"

#define MAX_FAMILYFIND_RETNUM	(MAX_SEND_BUFFERS - sizeof(XT_HEAD) - (sizeof(XT_CLUSTER_SEARCH_ACK) - sizeof(XT_CLUSTER_SEARCH_ACK::tagFamilyFindRet))) / sizeof(XT_CLUSTER_SEARCH_ACK::tagFamilyFindRet)


CBusinessFamilyGlobalFunc::CBusinessFamilyGlobalFunc()
{
}
CBusinessFamilyGlobalFunc::~CBusinessFamilyGlobalFunc()
{
}


//将家族指定消息加载到内存


//将所有消息保存到数据库中
int CBusinessFamilyGlobalFunc::SaveBufFamilyMsg(CPermFamily *pFamily, CDataBaseMini *pDataBaseMini)
{
	CAutoLock mylock(&pFamily->m_FamilyMsgMap.m_mapLock);
	FamilyMsg_Map::iterator it = pFamily->m_FamilyMsgMap.begin();
	for(; it != pFamily->m_FamilyMsgMap.end(); ++it)
	{
		FAMILYMSG_DATA *pData = it->second;
		if (pData->familyMsgID > pFamily->m_FamilySavedMsgID)
			pDataBaseMini->Save_Family_Msg(pData->familyMsgID, pFamily->m_FamilyID, pData->senderID, pData->tm, pData->msgType, pData->msgLen, pData->msg);
	}

	return RESULT_SUCCESS;
}

int CBusinessFamilyGlobalFunc::GetFamilyInfo(uint32 nFamilyID, CDataBaseFamily *pDataBaseFamily, CDataBaseMini *pDataBaseMini, CMiniAppNetwork *pAppNetwork,CGlobalBusi *pGlobalFamilyBusi)
{
	int dbret;

	XT_GET_CINFO_ACK Ack;
	
	dbret = pDataBaseFamily->Get_Family_Info(nFamilyID, &Ack);
	if (dbret != DB_SUCCESS)
		return dbret;

	//家族信息写入内存HASH
	CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
	CPermFamily *pFamily = pPermFamilyManager.Find(nFamilyID);

	if (!pFamily)
	{
		pFamily = pPermFamilyManager.AddHash(nFamilyID);
		pFamily->Reset();
	}
	else
	{
		dbret = pDataBaseMini->Get_Family_MaxMsgID(pFamily->m_FamilyID, pFamily->m_FamilyMaxMsgID,pFamily->m_FamilyMinMsgID);
		if(dbret != DB_SUCCESS)
			CLog::Log("Get_Family_MaxMsgID",CLog::TYPE_ERROR,"获取家族最大消息ID错误,m_FamilyID:%d!",pFamily->m_FamilyID);
		return RESULT_SUCCESS;
	}
	pFamily->m_FamilyID		 = nFamilyID;
	pFamily->m_CreatorID	 = Ack.nCreatorID;
	pFamily->m_FamilyClass	 = Ack.nFamilyClass;
	pFamily->m_FamilyFace	 = Ack.nFamilyFaceID;
	pFamily->m_FamilyType	 = Ack.nFamilyType;
	pFamily->m_JoinFlag		 = Ack.nJoinFlag;
	pFamily->m_ViewFlag		 = Ack.nViewFlag;
	strcpy_s(pFamily->m_FamilyDesc, Ack.szFamilyDesc);
	strcpy_s(pFamily->m_FamilyDomain, Ack.szFamilyDomain);
	strcpy_s(pFamily->m_FamilyFaceUrl, Ack.szFamilyFaceUrl);
	strcpy_s(pFamily->m_FamilyName, Ack.szFamilyName);
	pFamily->m_FamilyLimit = Ack.nMemberLimit;	
	strcpy_s(pFamily->m_FamilySign, Ack.szFamilySign);
	strcpy_s(pFamily->m_FamilyNotice, Ack.szFamilyNotice);
	time(&pFamily->tm);

	//获取家族最大、最小消息ID

	
	dbret = pDataBaseMini->Get_Family_MaxMsgID(pFamily->m_FamilyID, pFamily->m_FamilyMaxMsgID,pFamily->m_FamilyMinMsgID);
	if(dbret != DB_SUCCESS)
		CLog::Log("Get_Family_MaxMsgID",CLog::TYPE_ERROR,"获取家族最大消息ID错误,m_FamilyID:%d!",pFamily->m_FamilyID);
	pFamily->m_FamilyBufMinMsgID = pFamily->m_FamilyMaxMsgID;
	pFamily->m_FamilySavedMsgID = pFamily->m_FamilyMaxMsgID;



	//向全局服务器注册
	uint8 nCount = 1;
	tagGSRV_FAMILYINFO_ADD_QUERY::tagFamilyInfo FamilyInfo;
	FamilyInfo.FamilyID = pFamily->m_FamilyID;	
	FamilyInfo.ServerIp = inet_addr(CAppManager::GetInstance().GetConfig().GetinListenAddr());
	pFamily->m_FamilyServerIp = FamilyInfo.ServerIp;
	if (pGlobalFamilyBusi->AddFamilyInfo(&FamilyInfo,nCount) != RESULT_SUCCESS)
	{
		pPermFamilyManager.DelHash(nFamilyID);
		CLog::Log("AddFamilyInfo", CLog::TYPE_ERROR, "向全局注册家族失败,nFamilyID:%d", nFamilyID);
		return DB_ERROR;
	}

	pAppNetwork->FamilyLogin(pFamily->m_FamilyID);

	return RESULT_SUCCESS;
}
int CBusinessFamilyGlobalFunc::ReleaseFamilyInfo(uint32 nFamilyID,CMiniAppNetwork *pAppNetwork,CGlobalBusi *pGlobalFamilyBusi,CDataBaseMini* pDataBaseMini)
{
	CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
	//added by lwl ver:0106
	//CPermFamily *pFamily = pPermFamilyManager.Find(nFamilyID);
	//if (pFamily)
	//{
	//	SaveBufFamilyMsg(pFamily,pDataBaseMini);
	//}

	CLog::Log("ReleaseFamilyInfo", CLog::TYPE_NORMAL, "家族%d无在线成员，释放家族信息!", nFamilyID);

	pPermFamilyManager.DelHash(nFamilyID);

	pAppNetwork->FamilyLogout(nFamilyID);


	//向全局服务器注册
	uint8 nCount = 1;
	pGlobalFamilyBusi->DelFamilyInfo(&nFamilyID,nCount);
	return RESULT_SUCCESS;
}

int CBusinessFamilyGlobalFunc::ReleaseTempFamilyInfo(uint32 nFamilyID,CMiniAppNetwork *pAppNetwork,CGlobalBusi *pGlobalFamilyBusi)
{
	CTempFamilyManager& pTempFamilyManager = CAppManager::GetInstance().GetTempFamilyManager();
	pTempFamilyManager.DelHash(nFamilyID);

	pAppNetwork->FamilyLogout(nFamilyID, TEMP_FAMILY);

	//向全局服务器注册
	uint8 nCount = 1;
	pGlobalFamilyBusi->DelFamilyInfo(&nFamilyID,nCount, NULL, TEMP_FAMILY);
	return RESULT_SUCCESS;

}

void CBusinessFamilyGlobalFunc::TransitSetMemberMsg(CGlobalBusi *pGlobalBusi, uint32 recvID, uint8 action, uint32 actionID, uint32 familyID, uint8 familyProp, const char* familyName, uint32 familyMsgID, uint32 nInviteID)
{
	int len = 0;
	SendDataStruct sendData;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
	LPXT_SERVER_TRANSMIT lpSrvTransmit = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
	lpSrvTransmit->origin_id = familyID;
	lpSrvTransmit->msgtype = CMD_SET_CMEMBERS_REQ;
	len += sizeof(XT_SERVER_TRANSMIT);

	CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT));
	sendPacket<<action
		<<actionID
		<<familyID
		<<familyName
		<<familyProp
		<<familyMsgID
		<<nInviteID;
	len += sendPacket.GetSize();	

	CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,len,recvID);
	sendData.data_len = len + int(sizeof(XT_HEAD));
	CAppManager::GetInstance().GetAppNetWork().SendDataTo(pGlobalBusi, sendData.data,sendData.data_len,SEND_RETRY_TIME);
}


CBusinessGetFamilyList::CBusinessGetFamilyList(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_GET_CLIST_REQ,CMD_GET_CLIST_ACK)
{
	m_pDataBaseFamily = pRouter->GetDataBaseFamily();
	m_pDataBaseMini = pRouter->GetDataBaseMini();
}


int CBusinessGetFamilyList::DoWork()
{
	LPXT_GET_CLIST_REQ lpReq = (XT_GET_CLIST_REQ *)m_RecvPacket.GetData();
	CUserManager *pUserManager = m_pRouter->GetUserManager();
	shared_ptr<CUser> pUser = pUserManager->Find(m_pRecvHead->sid);
	if (pUser == NULL)
		return RESULT_FAILED;	
	
	int  nDataLen = 0;
	LPXT_GET_CLIST_ACK lpAck = (LPXT_GET_CLIST_ACK)m_SendPacket.GetData();
	
	if (lpReq->nCount == 0)
		lpAck->nCount = MAX_PERMFAMILY_NUM;
	else
		lpAck->nCount = lpReq->nCount;
	int tmpCount = lpAck->nCount;
	int dbret = m_pDataBaseFamily->Get_Family_List(m_pRecvHead->sid, lpAck->nFamilyID, tmpCount);
	if (dbret != DB_SUCCESS)
	{		
		char szError[MAX_ERROR_BUF];
		CError::DB_Error(dbret, szError, true);
		m_SendPacket<<uint8(RESULT_FAILED)<<szError;
		return RESULT_SUCCESS;
	}
	lpAck->nCount = tmpCount;
	lpAck->ret = RESULT_SUCCESS;
	nDataLen = sizeof(XT_GET_CLIST_ACK) + (lpAck->nCount - 1) * sizeof(lpAck->nFamilyID);
	m_SendPacket.SetCursor(nDataLen);
	lpAck->nFamilyProp = PERM_FAMILY;

	for(int i = 0; i < lpAck->nCount; ++i)
	{
		if (lpAck->nFamilyID[i] != 0)
		{	
			uint32 nMsgID = 0;
			m_pDataBaseMini->Get_User_FamilyMsgID(pUser->m_nID, lpAck->nFamilyID[i], nMsgID);
			pUser->Add_Perm_Family(lpAck->nFamilyID[i], nMsgID);
		}
	}
	
	pUser->NotifyFamilyStatus(&(CAppManager::GetInstance().GetAppNetWork()),m_pRouter->GetGlobalFamilyBusi(),m_pRouter->GetDataBaseMini());//modify by lwl ver:0105
	return RESULT_SUCCESS;
}

CBusinessGetFamilyInfo::CBusinessGetFamilyInfo(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_GET_CINFO_REQ,CMD_GET_CINFO_ACK)
{
	m_pDataBaseFamily = pRouter->GetDataBaseFamily();
	m_pDataBaseMini = pRouter->GetDataBaseMini();
}

int CBusinessGetFamilyInfo::DoWork()
{
	XT_GET_CINFO_REQ Req;
	m_RecvPacket>>Req.nFamilyID
				>>Req.nFamilyProp;
				
	LPXT_GET_CINFO_REQ lpReq = &Req;
	int dbret;

	XT_GET_CINFO_ACK Ack;// = (LPXT_GET_CINFO_ACK)m_SendPacket.GetData();
	if (lpReq->nFamilyProp == PERM_FAMILY)	//固定家族
	{		
		CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
		CPermFamily *pFamily = pPermFamilyManager.Find(lpReq->nFamilyID);
		

		if(!pFamily)	//需要加载家族信息
		{
			CBusinessFamilyGlobalFunc familyGlobalFunc;
			dbret = familyGlobalFunc.GetFamilyInfo(lpReq->nFamilyID,m_pDataBaseFamily, m_pDataBaseMini, m_pAppNetwork, m_pRouter->GetGlobalFamilyBusi());
			if (dbret != DB_SUCCESS)
			{
					char szError[MAX_ERROR_BUF];
					CError::DB_Error(dbret, szError, true);
					m_SendPacket<<uint8(RESULT_FAILED)<<szError;
					return RESULT_SUCCESS;
			}
			pFamily = pPermFamilyManager.Find(lpReq->nFamilyID);
			if (!pFamily)
			{
				m_SendPacket<<(uint8)RESULT_FAILED<<"获取家族信息失败";
				return RESULT_FAILED;
			}
		}

		//从内存中返回家族信息

		m_SendPacket<<(uint8)RESULT_SUCCESS
					<<lpReq->nFamilyID
					<<(uint8)PERM_FAMILY
					<<pFamily->m_CreatorID
					<<pFamily->m_FamilyType
					<<pFamily->m_FamilyClass
					<<pFamily->m_FamilyFace
                    <<pFamily->m_FamilyFaceUrl
					<<pFamily->m_JoinFlag
					<<pFamily->m_ViewFlag
					<<pFamily->m_FamilyName
					<<pFamily->m_FamilyDesc
					<<pFamily->m_FamilyDomain
					<<pFamily->m_FamilyLimit
					<<pFamily->m_FamilySign
					<<pFamily->m_FamilyNotice;
		if (strcmp(pFamily->m_FamilyName, "") == 0)
			CLog::Log("CBusinessGetFamilyInfo", CLog::TYPE_ERROR, "%FamilyID:%d ,家族名为空", lpReq->nFamilyID);
	}
	else if (lpReq->nFamilyProp == TEMP_FAMILY)	//临时家族
	{
		CTempFamilyManager& pTempFamilyManager = CAppManager::GetInstance().GetTempFamilyManager();
		CTempFamily *pFamily = pTempFamilyManager.Find(lpReq->nFamilyProp);
		
		if (!pFamily)	//临时家族不在该业务服务器上
		{	
			CLog::Log("CBusinessGetFamilyInfo",CLog::TYPE_ERROR, "FamilyID:%d ,该家族不存在", lpReq->nFamilyID);
			m_SendPacket<<uint8(RESULT_FAILED)<<(char*)"该家族不存在!";
			return RESULT_SUCCESS;
		}
		else	
		{
			m_SendPacket<<(uint8)RESULT_SUCCESS
						<<lpReq->nFamilyID
						<<(uint8)TEMP_FAMILY;
		}
	}
	else	//错误的家族类型
	{
		return RESULT_FAILED;
	}

	m_SendPacket.SetCursor(sizeof(XT_GET_CINFO_ACK));


	return RESULT_SUCCESS;
}

CBusinessGetFamilyMem::CBusinessGetFamilyMem(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_GET_CMEMBERS_REQ,CMD_GET_CMEMBERS_ACK)
{
	m_pDataBaseFamily = pRouter->GetDataBaseFamily();
	m_pDataBaseMini = pRouter->GetDataBaseMini();
}



int CBusinessGetFamilyMem::DoWork()
{
// 	CUserManager& UserManager = CAppManager::GetInstance().GetUserManager();
// 	CUser* pUser = NULL;
// 	pUser = UserManager.Find(m_pRecvHead->sid);
// 	if (!pUser)		
// 		return RESULT_FAILED;
	LPXT_GET_CMEMBERS_REQ lpReq;
	lpReq = (XT_GET_CMEMBERS_REQ *)m_RecvPacket.GetData();

	int dbret;
	int ret_count = 0;

	LPXT_GET_CMEMBERS_ACK lpAck = (LPXT_GET_CMEMBERS_ACK)m_SendPacket.GetData();
	lpAck->ret		= RESULT_SUCCESS;
	lpAck->next_pos = 0;
	lpAck->nFamilyID = lpReq->nFamilyID;
	lpAck->nFamilyProp = lpReq->nFamilyProp;
	lpAck->online_type = lpReq->online_type;
	lpAck->return_count = 0;				//retcount

	if (lpReq->nFamilyProp == PERM_FAMILY)	//固定家族
	{		
		CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
		CPermFamily *pFamily = pPermFamilyManager.Find(lpReq->nFamilyID);



// 		m_SendPacket<<(uint8)RESULT_SUCCESS
// 					<<(uint16)0										//nextpos
// 					<<lpReq->nFamilyID
// 					<<(uint8)PERM_FAMILY
// 					<<lpReq->online_type
// 					<<(uint16)0;									//retcount

		if(!pFamily)	//需要加载家族信息
		{	

			CBusinessFamilyGlobalFunc familyGlobalFunc;
			dbret = familyGlobalFunc.GetFamilyInfo(lpReq->nFamilyID, m_pDataBaseFamily, m_pDataBaseMini, m_pAppNetwork, m_pRouter->GetGlobalFamilyBusi());
			if (dbret != DB_SUCCESS)
			{
				char szError[MAX_ERROR_BUF];
				CError::DB_Error(dbret, szError, true);
				m_SendPacket<<uint8(RESULT_FAILED)<<szError;
				return RESULT_SUCCESS;
			}
			pFamily = pPermFamilyManager.Find(lpReq->nFamilyID);
			if (!pFamily)
			{
				m_SendPacket<<(uint8)RESULT_FAILED<<"获取家族成员失败";
				return RESULT_FAILED;
			}
		}

// 		CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
// 		CAutoLock listlock(&pFamily->m_FamilyMemberList.m_lstLock);
	
		if (pFamily->m_FamilyMemberList.size() == 0)	//获取家族成员
		{
			int nMemNum = 0;
			if (pFamily->m_FamilyLimit < MAX_VIPFAMILYMEM_NUM)
				nMemNum = pFamily->m_FamilyLimit;
			else
				nMemNum = MAX_VIPFAMILYMEM_NUM;


			ST_FAMILY_MEMBER pFamilyMem[MAX_VIPFAMILYMEM_NUM];
			dbret = m_pDataBaseFamily->Get_Family_Mem(pFamily->m_FamilyID,pFamilyMem, nMemNum);
			if (dbret != DB_SUCCESS)
			{
				m_SendPacket<<uint8(RESULT_FAILED)<<"获取家族成员失败.";
				return RESULT_SUCCESS;
			}
			uint32 familyMem[MAX_VIPFAMILYMEM_NUM];
			memset(familyMem, 0, sizeof(uint32) * MAX_VIPFAMILYMEM_NUM);
			for(int j = 0; j < nMemNum; ++j)
			{
				familyMem[j] = pFamilyMem[j].nUserID;
			}
			
			tagGSRV_GET_USER_STATUS_ACK::tagUserStatus UStatus[MAX_VIPFAMILYMEM_NUM];
			memset(UStatus, 0, sizeof(tagGSRV_GET_USER_STATUS_ACK::tagUserStatus) * MAX_VIPFAMILYMEM_NUM);
			uint8 nCount = nMemNum;
			
#ifdef IM_USE_CACHE
			CMemcacheQueryUserStatusi(m_pRouter,familyMem,nCount,UStatus);
#else
			CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();
			if (RESULT_FAILED ==pGlobalBusi->QueryUserStatus(familyMem, UStatus, nCount))
			{
				for(int i = 0; i < nCount; ++i)
				{
					UStatus[i].id = pFamilyMem[i].nUserID;
					UStatus[i].status = XTREAM_OFFLINE;
				}
			}
#endif			
		
			
			for(int i = 0; i < nMemNum; ++i)
 			{
				if (UStatus[i].id == pFamilyMem[i].nUserID)
				{
					pFamily->AddMember(pFamilyMem[i].nProfile,pFamilyMem[i].nUserID, pFamilyMem[i].nProp, UStatus[i].status);
					CLog::Log("CBusinessGetFamilyInfo",
						CLog::TYPE_IMPORTANT,
						"FamilyID:%d , profile %s, userid %u, nProp %d Status %d",
						lpReq->nFamilyID,
						pFamilyMem[i].nProfile,
						pFamilyMem[i].nUserID,
						pFamilyMem[i].nProp,
						UStatus[i].status);
				}
				else
				{
					pFamily->AddMember(pFamilyMem[i].nProfile,pFamilyMem[i].nUserID, pFamilyMem[i].nProp, XTREAM_OFFLINE);
					CLog::Log("CBusinessGetFamilyInfo",
						CLog::TYPE_IMPORTANT,
						"FamilyID:%d , profile %s, userid %u, nProp %d Status %d",
						lpReq->nFamilyID,
						pFamilyMem[i].nProfile,
						pFamilyMem[i].nUserID,
						pFamilyMem[i].nProp,
						XTREAM_OFFLINE);
				}

			}
			
		}
		//设置当前成员状态
		pFamily->SetMemStat(m_pRecvHead->sid, lpReq->userstatus);		

		if (pFamily->m_FamilyMembers.find(m_pRecvHead->sid) == pFamily->m_FamilyMembers.end())	//家族缓存中没有该成员，加载该成员
		{
			uint8 userProp;
			if (m_pDataBaseFamily->Get_FamilyMem_Porp(pFamily->m_FamilyID, m_pRecvHead->sid, userProp) == DB_SUCCESS)
                pFamily->AddMember(m_pRecvHead->sid, userProp);
			else
			{
				m_SendPacket<<uint8(RESULT_FAILED)<<"您不属于该家族成员";
				return RESULT_SUCCESS;
			}

		}	
		
		m_SendPacket.SetCursor(sizeof(XT_GET_CMEMBERS_ACK) - sizeof(FAMILYMEMBER_ACKINFO));

		char familyMemTmpBuf[sizeof(FAMILYMEMBER_ACKINFO)];
		CSendPacket SendPacketTemp(familyMemTmpBuf);

		//应答
		if (lpReq->online_type == 0)	//所有成员信息
		{
			if (lpReq->req_count == 0)	//不限制。返回所有成员
			{
				CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
				CAutoLock listlock(&pFamily->m_FamilyMemberList.m_lstLock);
				FamilyMember_List::iterator it = pFamily->m_FamilyMemberList.begin();
				int start = 0;
				for(; it != pFamily->m_FamilyMemberList.end(); ++it)
				{
					if (lpReq->start_pos > start)
					{
						++start;
                        continue;
					}
					
					SendPacketTemp<<(*it)
								  <<pFamily->m_FamilyMembers[(*it)].nProp
								  <<pFamily->m_FamilyMembers[(*it)].nStat
								  <<pFamily->m_FamilyMembers[(*it)].nProfile;

					if( !m_SendPacket.HasData(SendPacketTemp.GetSize()) )
					{
						//超过尺寸了,退出
						lpAck->next_pos = lpReq->start_pos + ret_count;			//下次开始获取的位置
						lpAck->return_count = ret_count;
						break;
					}
					else
					{
						m_SendPacket.WritePacket(SendPacketTemp.GetData(),SendPacketTemp.GetSize());
						SendPacketTemp.Reset();
					}

					++ret_count;
				}			
			}
			else		//返回固定个数
			{
				CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
				CAutoLock listlock(&pFamily->m_FamilyMemberList.m_lstLock);
				FamilyMember_List::iterator it = pFamily->m_FamilyMemberList.begin();
				int start = 0;
				for(; it != pFamily->m_FamilyMemberList.end(); ++it)
				{
					if (lpReq->start_pos < start)
					{
						++start;
						continue;
					}

					if (ret_count == lpReq->req_count)
					{
						lpAck->return_count = ret_count;
						break;
					}
					SendPacketTemp<<(*it)
						<<pFamily->m_FamilyMembers[(*it)].nProp
						<<pFamily->m_FamilyMembers[(*it)].nStat
						<<pFamily->m_FamilyMembers[(*it)].nProfile;

					if( !m_SendPacket.HasData(SendPacketTemp.GetSize()) )
					{
						//超过尺寸了,退出
						lpAck->next_pos = lpReq->start_pos + ret_count;			//下次开始获取的位置
						lpAck->return_count = ret_count;
						break;
					}
					else
					{
						m_SendPacket.WritePacket(SendPacketTemp.GetData(),SendPacketTemp.GetSize());
						SendPacketTemp.Reset();
					}
					++ret_count;
				}
			}
		}
		else if (lpReq->online_type == 1)	//仅在线成员
		{
			if (lpReq->req_count == 0)	//不限制。返回所有成员
			{
				CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
				CAutoLock listlock(&pFamily->m_FamilyMemberList.m_lstLock);
				FamilyMember_List::iterator it = pFamily->m_FamilyMemberList.begin();
				int start = 0;
				for(; it != pFamily->m_FamilyMemberList.end(); ++it)
				{
					if (lpReq->start_pos < start)
					{
						++start;
						continue;
					}

					if (pFamily->m_FamilyMembers[(*it)].nStat == XTREAM_ONLINE || pFamily->m_FamilyMembers[(*it)].nStat == XTREAM_LEAVE)
					{
						SendPacketTemp<<(*it)
							<<pFamily->m_FamilyMembers[(*it)].nProp
							<<pFamily->m_FamilyMembers[(*it)].nStat
							<<pFamily->m_FamilyMembers[(*it)].nProfile;
						if( !m_SendPacket.HasData(SendPacketTemp.GetSize()) )
						{
							//超过尺寸了,退出
							lpAck->next_pos = lpReq->start_pos + ret_count;			//下次开始获取的位置
							lpAck->return_count = ret_count;
							break;
						}
						else
						{
							m_SendPacket.WritePacket(SendPacketTemp.GetData(),SendPacketTemp.GetSize());
							SendPacketTemp.Reset();
							++ret_count;
						}
					}
				}
			}
			else		//返回固定个数
			{
				CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
				CAutoLock listlock(&pFamily->m_FamilyMemberList.m_lstLock);
				FamilyMember_List::iterator it = pFamily->m_FamilyMemberList.begin();
				int start = 0;
				for(; it != pFamily->m_FamilyMemberList.end(); ++it)
				{
					if (lpReq->start_pos < start)
					{
						++start;
						continue;
					}

					if (ret_count == lpReq->req_count)
					{			
						lpAck->return_count = ret_count;
						break;					
					}

					if (pFamily->m_FamilyMembers[(*it)].nStat == XTREAM_ONLINE || pFamily->m_FamilyMembers[(*it)].nStat == XTREAM_LEAVE)
					{
						SendPacketTemp<<(*it)
							<<pFamily->m_FamilyMembers[(*it)].nProp
							<<pFamily->m_FamilyMembers[(*it)].nStat
							<<pFamily->m_FamilyMembers[(*it)].nProfile;
						if( !m_SendPacket.HasData(SendPacketTemp.GetSize()) )
						{
							//超过尺寸了,退出
							lpAck->next_pos = lpReq->start_pos + ret_count;			//下次开始获取的位置
							lpAck->return_count = ret_count;
							break;
						}
						else
						{
							m_SendPacket.WritePacket(SendPacketTemp.GetData(),SendPacketTemp.GetSize());
							SendPacketTemp.Reset();
							++ret_count;
						}
						++ret_count;
					}
				}
			}
		}

	}
	else if (lpReq->nFamilyProp == TEMP_FAMILY)	//临时家族
	{
		CTempFamilyManager& pTempFamilyManager = CAppManager::GetInstance().GetTempFamilyManager();
		CTempFamily *pFamily = pTempFamilyManager.Find(lpReq->nFamilyID);

		if (!pFamily)	//临时家族不在该业务服务器上
		{	
			CLog::Log("CBusinessGetFamilyInfo",CLog::TYPE_ERROR, "FamilyID:%d ,临时家族不存在", lpReq->nFamilyID);
			m_SendPacket<<uint8(RESULT_FAILED)<<(char*)"该家族不存在!";
			return RESULT_SUCCESS;
		}
		else	
		{
			m_SendPacket.SetCursor(sizeof(XT_GET_CMEMBERS_ACK) - sizeof(FAMILYMEMBER_ACKINFO));
			char familyMemTmpBuf[sizeof(FAMILYMEMBER_ACKINFO)];
			CSendPacket SendPacketTemp(familyMemTmpBuf);
			if (find(pFamily->m_FamilyMemberList.begin(), pFamily->m_FamilyMemberList.end(), m_pRecvHead->sid) == pFamily->m_FamilyMemberList.end())
			{
				lpAck->next_pos = 0xFFFF;			//下次开始获取的位置
				lpAck->return_count = 0;
			}
			else
			{
				SafeList<uint32>::iterator it = pFamily->m_FamilyMemberList.begin();
				for(; it != pFamily->m_FamilyMemberList.end(); ++it)
				{

					SendPacketTemp<<*it
						<<(uint8)FAMILY_NORMAL
						<<(uint8)XTREAM_ONLINE;

					if( !m_SendPacket.HasData(SendPacketTemp.GetSize()) )
					{
						//超过尺寸了,退出
						lpAck->next_pos = lpReq->start_pos + ret_count;			//下次开始获取的位置
						lpAck->return_count = ret_count;
						break;
					}
					else
					{
						m_SendPacket.WritePacket(SendPacketTemp.GetData(),SendPacketTemp.GetSize());
						SendPacketTemp.Reset();
						++ret_count;
					}
				}
			}

		}

	}
	else	//错误的家族类型
	{
		return RESULT_FAILED;
	}

	if (lpAck->next_pos ==0)
		lpAck->next_pos = 0xFFFF;
	if (lpAck->return_count == 0 )
		lpAck->return_count = ret_count;
	//m_SendPacket.SetCursor(sizeof(XT_GET_CMEMBERS_ACK) + (lpAck->return_count - 1) * sizeof(FAMILYMEMBER_ACKINFO));

	return RESULT_SUCCESS;
}

CBusinessSetFamilyInfo::CBusinessSetFamilyInfo(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_SET_CINFO_REQ,CMD_SET_CINFO_ACK)
{
	m_pDataBaseFamily = pRouter->GetDataBaseFamily();
}

int CBusinessSetFamilyInfo::DoWork()
{
	XT_SET_CINFO_REQ Req;
	m_RecvPacket>>Req.nFamilyID
				>>Req.nFamilyProp
				>>Req.nJoinFlag
				>>Req.nFamilyClass
				>>Req.nFamilyFaceID
				>>Req.szFamilyFaceUrl
				>>Req.nViewFlag
				>>Req.szFamilyName
				>>Req.szFamilyDesc
				>>Req.szFamilySign
				>>Req.szFamilyNotice;
	char szError[MAX_ERROR_BUF];
	int dbret;
	CLog::Log("CBusinessSetFamilyInfo",CLog::TYPE_IMPORTANT,
		"IN FamilyID:%d,FamilyProp %d,joinFlag %d,FamilyClass %d,FaceID %d, FaceUrl %s, viewFlag %d, FamilyName %s,\
FamilyDesc %s, FamilySign %s, FamilyNotice %s",
				Req.nFamilyID,
				Req.nFamilyProp,
				Req.nJoinFlag,
				Req.nFamilyClass,
				Req.nFamilyFaceID,
				Req.szFamilyFaceUrl,
				Req.nViewFlag,
				Req.szFamilyName,
				Req.szFamilyDesc,
				Req.szFamilySign,
				Req.szFamilyNotice);

	LPXT_SET_CINFO_ACK lpAck = (LPXT_SET_CINFO_ACK)m_SendPacket.GetData();
	if (Req.nFamilyProp == PERM_FAMILY)	//固定家族
	{		
		CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
		CPermFamily *pFamily = pPermFamilyManager.Find(Req.nFamilyID);

		lpAck->ret			= RESULT_SUCCESS;
		lpAck->nFamilyID	= Req.nFamilyID;
		lpAck->nFamilyProp	= PERM_FAMILY;
		strcpy_s(lpAck->szFamilyNotice,Req.szFamilyNotice);

		if(!pFamily)	//无该家族信息
		{
			CLog::Log("CBusinessSetFamilyInfo",CLog::TYPE_IMPORTANT,
			"OUT 无该家族信息 FamilyID:%d,FamilyProp %d,joinFlag %d,FamilyClass %d,FaceID %d, FaceUrl %s, viewFlag %d, FamilyName %s,\
FamilyDesc %s, FamilySign %s, FamilyNotice %s",
				Req.nFamilyID,
				Req.nFamilyProp,
				Req.nJoinFlag,
				Req.nFamilyClass,
				Req.nFamilyFaceID,
				Req.szFamilyFaceUrl,
				Req.nViewFlag,
				Req.szFamilyName,
				Req.szFamilyDesc,
				Req.szFamilySign,
				Req.szFamilyNotice);
			return RESULT_FAILED;
		}
		//写入数据库
		dbret = m_pDataBaseFamily->Set_Family_Info(&Req);
		if (dbret != DB_SUCCESS)
		{
			lpAck->ret = RESULT_FAILED;
			CError::DB_Error(dbret, szError, true);
			goto end;
		}
		else
		{
			//家族信息写入内存HASH
			pFamily->m_FamilyClass	 = Req.nFamilyClass;
			pFamily->m_FamilyFace	 = Req.nFamilyFaceID;
			//pFamily->m_FamilyType	 = lpReq->nFamilyType;			无法修改FAMILY_COMMON|FAMILY_VIP
			pFamily->m_JoinFlag		 = Req.nJoinFlag;
			pFamily->m_ViewFlag		 = Req.nViewFlag;
			strcpy_s(pFamily->m_FamilyDesc, Req.szFamilyDesc);
			strcpy_s(pFamily->m_FamilyFaceUrl, Req.szFamilyFaceUrl);
			strcpy_s(pFamily->m_FamilySign, Req.szFamilySign);
			strcpy_s(pFamily->m_FamilyNotice, Req.szFamilyNotice);
			if(strcmp(pFamily->m_FamilyName,Req.szFamilyName) != 0)
			{
				strcpy_s(pFamily->m_FamilyName, Req.szFamilyName);
				char strFamilyID[10] = {0};
				_itoa_s(Req.nFamilyID,strFamilyID, 10);
				char strUserID[10] = {0};
				_itoa_s(m_pRecvHead->sid,strUserID, 10);
				m_pRouter->GetDataBaseWeb()->Write_System_Log(130204,strUserID,strFamilyID,13,m_pRecvHead->sid);
			}
			CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
			FamilyMember_List::iterator iter; 
			for(iter = pFamily->m_FamilyMemberList.begin(); iter != pFamily->m_FamilyMemberList.end(); ++ iter)
			{
				if(m_pRecvHead->sid == *iter)
					continue;
				if(pFamily->m_FamilyMembers.find(*iter) == pFamily->m_FamilyMembers.end())
					continue;
				if(pFamily->m_FamilyMembers[*iter].nStat != XTREAM_OFFLINE)
				{
					SyncFamilyInfo(m_RecvPacket,*iter);	
				}
			}
		}
	}
	else if (Req.nFamilyProp == TEMP_FAMILY)	// 临时家族暂无设置信息
	{
		lpAck->ret			= RESULT_SUCCESS;
		lpAck->nFamilyID	= Req.nFamilyID;
		lpAck->nFamilyProp	= TEMP_FAMILY;
		strcpy_s(lpAck->szFamilyNotice,Req.szFamilyNotice);
					
					CLog::Log("CBusinessSetFamilyInfo",CLog::TYPE_IMPORTANT,
				"OUT 错误的家族类型 FamilyID:%d,FamilyProp %d,joinFlag %d,FamilyClass %d,FaceID %d, FaceUrl %s, viewFlag %d, FamilyName %s,\
		FamilyDesc %s, FamilySign %s, FamilyNotice %s",
						Req.nFamilyID,
						Req.nFamilyProp,
						Req.nJoinFlag,
						Req.nFamilyClass,
						Req.nFamilyFaceID,
						Req.szFamilyFaceUrl,
						Req.nViewFlag,
						Req.szFamilyName,
						Req.szFamilyDesc,
						Req.szFamilySign,
						Req.szFamilyNotice);

		return RESULT_SUCCESS;
	}
	else	//错误的家族类型
	{
			CLog::Log("CBusinessSetFamilyInfo",CLog::TYPE_IMPORTANT,
		"OUT 错误的家族类型 FamilyID:%d,FamilyProp %d,joinFlag %d,FamilyClass %d,FaceID %d, FaceUrl %s, viewFlag %d, FamilyName %s,\
FamilyDesc %s, FamilySign %s, FamilyNotice %s",
				Req.nFamilyID,
				Req.nFamilyProp,
				Req.nJoinFlag,
				Req.nFamilyClass,
				Req.nFamilyFaceID,
				Req.szFamilyFaceUrl,
				Req.nViewFlag,
				Req.szFamilyName,
				Req.szFamilyDesc,
				Req.szFamilySign,
				Req.szFamilyNotice);
		return RESULT_FAILED;
	}
end:
	if (lpAck->ret == RESULT_FAILED)
	{
		m_SendPacket<<(uint8)RESULT_FAILED
					<<Req.nFamilyID
					<<Req.nFamilyProp
            		<<szError;
	}
	else
	{
        m_SendPacket.SetCursor(sizeof(XT_SET_CINFO_ACK));
				
	}
	CLog::Log("CBusinessSetFamilyInfo",CLog::TYPE_IMPORTANT,
				"OUT SUCCESS FamilyID:%d,FamilyProp %d,joinFlag %d,FamilyClass %d,FaceID %d, FaceUrl %s, viewFlag %d, FamilyName %s,\
		FamilyDesc %s, FamilySign %s, FamilyNotice %s",
						Req.nFamilyID,
						Req.nFamilyProp,
						Req.nJoinFlag,
						Req.nFamilyClass,
						Req.nFamilyFaceID,
						Req.szFamilyFaceUrl,
						Req.nViewFlag,
						Req.szFamilyName,
						Req.szFamilyDesc,
						Req.szFamilySign,
						Req.szFamilyNotice);

	return RESULT_SUCCESS;

}

void CBusinessSetFamilyInfo::SyncFamilyInfo(CRecvPacket& packet, uint32 uid)
{
	SendDataStruct sendData;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
	XT_SERVER_TRANSMIT* lpSrvTransmit = (XT_SERVER_TRANSMIT*)(sendData.data + sizeof(XT_HEAD));
	uint32 len = sizeof(XT_SERVER_TRANSMIT) + packet.GetSize();
	
	CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,len,uid);
	lpSrvTransmit->origin_id = 0; 
	lpSrvTransmit->msgtype = CMD_SET_CINFO_REQ;
	
	sendData.data_len = len + sizeof(XT_HEAD);

	memcpy(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT),packet.GetData(),packet.GetSize());
	
	CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),sendData.data,sendData.data_len,SEND_RETRY_TIME);
}


CBusinessCreateFamily::CBusinessCreateFamily(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_CREATE_CLUSTER_REQ,CMD_CREATE_CLUSTER_ACK)
{
	m_pDataBaseFamily = pRouter->GetDataBaseFamily();
	m_pDataBaseWeb = pRouter->GetDataBaseWeb();
}

int CBusinessCreateFamily::DoWork()
{
	CUserManager *pUserManager = m_pRouter->GetUserManager();
	shared_ptr<CUser> pUser = pUserManager->Find(m_pRecvHead->sid);
	if (pUser == NULL)				//用户不在线
		return RESULT_FAILED;

 	XT_CREATE_CLUSTER_REQ Req;
	m_RecvPacket>>Req.nFamilyProp
				>>Req.nJoinFlag
				>>Req.nFamilyClass
				>>Req.nFamilyFaceID
				>>Req.szFamilyFaceUrl
				>>Req.nViewFlag
				>>Req.szFamilyName
				>>Req.szFamilyDomain
				>>Req.szFamilyDesc;
 	int dbret;
 
 	LPXT_CREATE_CLUSTER_ACK lpAck = (XT_CREATE_CLUSTER_ACK *)m_SendPacket.GetData();
	lpAck->ret			= RESULT_SUCCESS;
	if (Req.nFamilyProp == PERM_FAMILY)	//固定家族
 	{		
 
		dbret = m_pDataBaseFamily->Create_Family(m_pRecvHead->sid, &Req, lpAck->nFamilyID);
		if (dbret != DB_SUCCESS)
		{
			char szError[MAX_ERROR_BUF];
			CError::DB_Error(dbret, szError, true);
			m_SendPacket<<uint8(RESULT_FAILED)<<szError;
			return RESULT_SUCCESS;
		}
		char strID[10] = {0};
		_itoa_s(lpAck->nFamilyID, strID, 10);
		char strUserID[10] = {0};
		_itoa_s(m_pRecvHead->sid,strUserID, 10);
		m_pDataBaseWeb->Write_System_Log(130203,strUserID,strID,13,m_pRecvHead->sid);
		
	  	CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
	  	CPermFamily *pFamily = pPermFamilyManager.AddHash(lpAck->nFamilyID);
		pFamily->Reset();

		//家族信息写入内存HASH
		pFamily->m_FamilyID		 = lpAck->nFamilyID;
		pFamily->m_FamilyClass	 = Req.nFamilyClass;
		pFamily->m_FamilyFace	 = Req.nFamilyFaceID;
		pFamily->m_FamilyType	 = FAMILY_COMMON;
		pFamily->m_JoinFlag		 = Req.nJoinFlag;
		pFamily->m_ViewFlag		 = Req.nViewFlag;
		pFamily->m_CreatorID	 = m_pRecvHead->sid;
		strcpy_s(pFamily->m_FamilyDesc, Req.szFamilyDesc);
		strcpy_s(pFamily->m_FamilyFaceUrl, Req.szFamilyFaceUrl);
		strcpy_s(pFamily->m_FamilyName, Req.szFamilyName);
		strcpy_s(pFamily->m_FamilyDomain, Req.szFamilyDomain);
		pFamily->AddMember(pUser->m_nID, FAMILY_CREATER, pUser->m_nStatus);	//将创建者添加到列表中

		m_pAppNetwork->FamilyLogin(pFamily->m_FamilyID);

		//向全局服务器注册
		uint8 nCount = 1;
		tagGSRV_FAMILYINFO_ADD_QUERY::tagFamilyInfo FamilyInfo;
		FamilyInfo.FamilyID = pFamily->m_FamilyID;
		FamilyInfo.ServerIp = inet_addr(CAppManager::GetInstance().GetConfig().GetinListenAddr());
		pFamily->m_FamilyServerIp = FamilyInfo.ServerIp;
		m_pRouter->GetGlobalFamilyBusi()->AddFamilyInfo(&FamilyInfo,nCount);
	}
 	else if (Req.nFamilyProp == TEMP_FAMILY)
 	{
		//获取临时家族ID
		uint8 nCount = 1;
		uint32 nFamilyID;
		tagGSRV_FAMILYINFO_ADD_QUERY::tagFamilyInfo FamilyInfo;
		FamilyInfo.FamilyID = 0;
		FamilyInfo.ServerIp = inet_addr(CAppManager::GetInstance().GetConfig().GetinListenAddr());
		m_pRouter->GetGlobalFamilyBusi()->AddFamilyInfo(&FamilyInfo,nCount,&nFamilyID, TEMP_FAMILY);
		if (!nFamilyID)
		{
			m_SendPacket<<uint8(RESULT_FAILED)<<"创建失败，请稍后再试";
			return RESULT_SUCCESS;
		}
		lpAck->nFamilyID	= nFamilyID;


		CTempFamilyManager& pTempFamilyManager = CAppManager::GetInstance().GetTempFamilyManager();
		CTempFamily *pFamily = pTempFamilyManager.AddHash(nFamilyID);
		pFamily->Reset();

		pFamily->m_FamilyID = nFamilyID;
		pFamily->m_FamilyServerIp = FamilyInfo.ServerIp;

		pFamily->AddMember(pUser->m_nID); 	

		m_pAppNetwork->FamilyLogin(nFamilyID, TEMP_FAMILY); 
		pUser->Add_Temp_Family(nFamilyID);
 	}
 	else	//错误的家族类型
 	{
 		return RESULT_FAILED;
 	}
 
 	m_SendPacket.SetCursor(sizeof(XT_CREATE_CLUSTER_ACK));

	return RESULT_SUCCESS;
}

CBusinessSetFamilyMember::CBusinessSetFamilyMember(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_SET_CMEMBERS_REQ,CMD_SET_CMEMBERS_ACK)
{
	m_pDataBaseFamily = pRouter->GetDataBaseFamily();
}

int CBusinessSetFamilyMember::DoWork()
{
	CUserManager *pUserManager = m_pRouter->GetUserManager();
	shared_ptr<CUser> pUser = NULL;

	LPXT_SET_CMEMBERS_REQ lpReq = (XT_SET_CMEMBERS_REQ *)m_RecvPacket.GetData();

	char szError[MAX_ERROR_BUF];
	int dbret;

	LPXT_SET_CMEMBERS_ACK lpAck = (XT_SET_CMEMBERS_ACK *)m_SendPacket.GetData();
	lpAck->ret			= RESULT_SUCCESS;
	lpAck->nFamilyID	= lpReq->nFamilyID;
	lpAck->nFamilyProp	= lpReq->nFamilyProp;
	lpAck->nAction		= lpReq->nAction;
	lpAck->nMemberID	= lpReq->nMemID[0];	//memberID返回给客户端
	lpAck->nInviteID	= lpReq->nInviteID;
	
	if (lpReq->nFamilyProp == PERM_FAMILY)	//固定家族
	{		
		CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
		CPermFamily *pFamily = pPermFamilyManager.Find(lpReq->nFamilyID);

		if (!pFamily)
			return RESULT_FAILED;

		if (pFamily->IsCreater(m_pRecvHead->sid) != RESULT_SUCCESS && 
			pFamily->IsManager(m_pRecvHead->sid) != RESULT_SUCCESS)
		{
			lpAck->ret = RESULT_FAILED;
			strcpy_s(szError,"您没有权限进行此操作" );
			goto end;
		}

		if (lpReq->nAction == CMEMBER_ADD)
		{
			// 要进行判断该用户是否允许被添加
			if (pFamily->GetCurMemberNum() >= pFamily->GetMaxMemberNum())
			{
				lpAck->ret = RESULT_FAILED;
				strcpy_s(szError,"家族成员数已满" );
				goto end;
			}
			if (pFamily->GetCurMemberNum() + lpReq->nCount > pFamily->GetMaxMemberNum())
			{
				lpAck->ret = RESULT_FAILED;
				sprintf_s(szError,"该家族最多只能再添加 %d 个成员", pFamily->GetMaxMemberNum() - pFamily->GetCurMemberNum() );
				goto end;
			}


			int nRetCount = lpReq->nCount;
			dbret = m_pDataBaseFamily->Family_Add_Member(pFamily->m_FamilyID, lpReq->nMemID, nRetCount);
			if (dbret != DB_SUCCESS)
			{
				lpAck->ret = RESULT_FAILED;
				strcpy_s(szError,"添加成员失败，请稍后再试" );
				goto end;
			}
			m_SendPacket.SetCursor(sizeof(XT_SET_CMEMBERS_ACK));
			SendReply();
			CancelReply();

			tagGSRV_GET_USER_STATUS_ACK::tagUserStatus UStatus[MAX_VIPFAMILYMEM_NUM];
			memset(UStatus, 0, sizeof(tagGSRV_GET_USER_STATUS_ACK::tagUserStatus) * MAX_VIPFAMILYMEM_NUM);
			uint8 nCount = nRetCount;
#ifdef IM_USE_CACHE			
			CMemcacheQueryUserStatusi(m_pRouter,lpReq->nMemID,nCount,UStatus);
#else
			CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();
			if ( nCount != 0 && RESULT_FAILED ==pGlobalBusi->QueryUserStatus(lpReq->nMemID, UStatus, nCount))
			{
				for(int i = 0; i < nCount; ++i)
				{
					UStatus[i].id = lpReq->nMemID[i];
					UStatus[i].status = XTREAM_OFFLINE;
				}
			}
#endif
			char strUserID[4000] = {0};
			char strUser[10] = {0};
			for(int i = 0; i < nRetCount; ++i)
			{
				if (UStatus[i].id == lpReq->nMemID[i])
                    pFamily->AddMember(lpReq->nMemID[i], FAMILY_NORMAL,UStatus[i].status);
				else
					pFamily->AddMember(lpReq->nMemID[i], FAMILY_NORMAL,XTREAM_OFFLINE);

				pUser = pUserManager->Find(lpReq->nMemID[i]);
				if (pUser != NULL)				//用户在线
				{
					pUser->Add_Perm_Family(pFamily->m_FamilyID, pFamily->m_FamilyMaxMsgID);
				}

				//通知家族有新成员加入
				CBusinessFamilyGlobalFunc familyGlobalFunc;
				CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
				FamilyMember_Map::iterator it = pFamily->m_FamilyMembers.begin();
				for(; it != pFamily->m_FamilyMembers.end(); ++it)
				{
					if ((*it).second.nStat != XTREAM_OFFLINE && (*it).first != m_pRecvHead->sid)
					{
						familyGlobalFunc.TransitSetMemberMsg(m_pRouter->GetGlobalBusi(),(*it).first, lpReq->nAction, lpReq->nMemID[i], pFamily->m_FamilyID, pFamily->m_FamilyProp, pFamily->m_FamilyName, pFamily->m_FamilyMaxMsgID);
					}
				}
				_itoa_s(lpReq->nMemID[i],strUser,10);
				strcat_s(strUserID,strUser);
				if(i != nRetCount -1)
				{
					strcat_s(strUserID,",");
				}
			}
			char strFamilyID[10] = {0};
			_itoa_s(pFamily->m_FamilyID, strFamilyID, 10);
			if(strcmp(strUserID,"") !=0)
				m_pRouter->GetDataBaseWeb()->Write_System_Log(130201,strUserID,strFamilyID,13,m_pRecvHead->sid);
		}
		else if (lpReq->nAction == CMEMBER_DEL)
		{
			lpAck->nMemberID = lpReq->nMemID[0];
			if (pFamily->IsManager(m_pRecvHead->sid) != RESULT_SUCCESS &&
				pFamily->IsCreater(m_pRecvHead->sid) != RESULT_SUCCESS)	
			{
				lpAck->ret = RESULT_FAILED;
				strcpy_s(szError,"您没有权限删除用户");
				goto end;				
			}

			if (pFamily->IsManager(m_pRecvHead->sid) == RESULT_SUCCESS)//管理员删除管理员或创建者
			{
 				if (pFamily->IsCreater(lpReq->nMemID[0])== RESULT_SUCCESS || pFamily->IsManager(lpReq->nMemID[0]) == RESULT_SUCCESS)
				{
					lpAck->ret = RESULT_FAILED;
					strcpy_s(szError,"您没有权限删除此用户");
					goto end;
				}
			}
			if (lpReq->nMemID[0] == m_pRecvHead->sid)
			{
				lpAck->ret = RESULT_FAILED;
				strcpy_s(szError,"您不能删除自己");
				goto end;
			}

			dbret = m_pDataBaseFamily->Family_Delete_Member(lpReq->nFamilyID, lpReq->nMemID[0]);

			if (dbret != DB_SUCCESS)
			{
				lpAck->ret = RESULT_FAILED;
				strcpy_s(szError,"删除失败，请稍后再试");
				goto end;
			}
			m_pRouter->GetDataBaseMini()->Delete_FamilyMem_RecvMsg(lpReq->nFamilyID, lpReq->nMemID[0]);
			
			char strUser[10] = {0};
			_itoa_s(lpReq->nMemID[0],strUser,10);
			char strFamilyID[10] = {0};
			_itoa_s(lpReq->nFamilyID, strFamilyID, 10);
			m_pRouter->GetDataBaseWeb()->Write_System_Log(130202,strUser,strFamilyID,13,m_pRecvHead->sid);


			pFamily->DelMember(lpReq->nMemID[0]);
			pUser = pUserManager->Find(lpReq->nMemID[0]);
			if (pUser)				//用户在线
				pUser->Del_Perm_Family(pFamily->m_FamilyID);
			CBusinessFamilyGlobalFunc familyGlobalFunc;

			familyGlobalFunc.TransitSetMemberMsg(m_pRouter->GetGlobalBusi(),lpReq->nMemID[0], lpReq->nAction, lpReq->nMemID[0], pFamily->m_FamilyID, pFamily->m_FamilyProp, pFamily->m_FamilyName);

            //通知用户所在业务服务器修改用户家族列表
			CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
			FamilyMember_Map::iterator it = pFamily->m_FamilyMembers.begin();
			for(; it != pFamily->m_FamilyMembers.end(); ++it)
			{
				if ((*it).second.nStat != XTREAM_OFFLINE && (*it).first != m_pRecvHead->sid)
				{
					familyGlobalFunc.TransitSetMemberMsg(m_pRouter->GetGlobalBusi(),(*it).first, lpReq->nAction, lpReq->nMemID[0], pFamily->m_FamilyID, pFamily->m_FamilyProp, pFamily->m_FamilyName);
				}
			}
		}
		else if (lpReq->nAction == CMEMBER_SET_ADMIN)
		{
			lpAck->nMemberID = lpReq->nMemID[0];
			if (pFamily->IsCreater(m_pRecvHead->sid) != RESULT_SUCCESS)	
			{
				lpAck->ret = RESULT_FAILED;
				strcpy_s(szError,"您没有权限进行此操作");
				goto end;				
			}
			if (pFamily->GetCurManagerNum() >= pFamily->GetMaxManagerNum())
			{
				lpAck->ret = RESULT_FAILED;
				strcpy_s(szError,"一个群最多10个管理员");
				goto end;
			}

			dbret = m_pDataBaseFamily->Family_SetMem_Prop(lpReq->nFamilyID, lpReq->nMemID[0], FAMILY_MANAGER);
			if (dbret != DB_SUCCESS)
			{
				lpAck->ret = RESULT_FAILED;
				strcpy_s(szError,"设置失败，请稍后再试");
				goto end;
			}

			pFamily->SetManager(lpReq->nMemID[0]);
			CBusinessFamilyGlobalFunc familyGlobalFunc;
			
			CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
			FamilyMember_Map::iterator it = pFamily->m_FamilyMembers.begin();
			for(; it != pFamily->m_FamilyMembers.end(); ++it)
			{
				if ((*it).second.nStat != XTREAM_OFFLINE && (*it).first != m_pRecvHead->sid)
					familyGlobalFunc.TransitSetMemberMsg(m_pRouter->GetGlobalBusi(),(*it).first, lpReq->nAction, lpReq->nMemID[0], pFamily->m_FamilyID, pFamily->m_FamilyProp, pFamily->m_FamilyName);
			}
		}
		else if (lpReq->nAction == CMEMBER_CANCEL_ADMIN)
		{
			lpAck->nMemberID = lpReq->nMemID[0];
			if (pFamily->IsCreater(m_pRecvHead->sid) != RESULT_SUCCESS)	
			{
				lpAck->ret = RESULT_FAILED;
				strcpy_s(szError,"您没有权限进行此操作");
				goto end;				
			}
			dbret = m_pDataBaseFamily->Family_SetMem_Prop(lpReq->nFamilyID, lpReq->nMemID[0], FAMILY_NORMAL);
			if (dbret != DB_SUCCESS)
			{
				lpAck->ret = RESULT_FAILED;
				strcpy_s(szError,"设置失败，请稍后再试");
				goto end;
			}

			pFamily->CancelManager(lpReq->nMemID[0]);
			CBusinessFamilyGlobalFunc familyGlobalFunc;
			CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
			FamilyMember_Map::iterator it = pFamily->m_FamilyMembers.begin();
			for(; it != pFamily->m_FamilyMembers.end(); ++it)
			{
				if ((*it).second.nStat != XTREAM_OFFLINE && (*it).first != m_pRecvHead->sid)
					familyGlobalFunc.TransitSetMemberMsg(m_pRouter->GetGlobalBusi(),(*it).first, lpReq->nAction, lpReq->nMemID[0], pFamily->m_FamilyID, pFamily->m_FamilyProp, pFamily->m_FamilyName);
			}
		}
		else if(lpReq->nAction < CMEMBER_REFUSE_IN_2 + 1)
		{
			CBusinessFamilyGlobalFunc familyGlobalFunc;
			CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
			FamilyMember_Map::iterator it = pFamily->m_FamilyMembers.begin();
			for(; it != pFamily->m_FamilyMembers.end(); ++it)
			{
				if ((*it).second.nStat != XTREAM_OFFLINE )
				{
					if((*it).first == pFamily->m_CreatorID || (*it).first == lpReq->nInviteID)
					{
						familyGlobalFunc.TransitSetMemberMsg(m_pRouter->GetGlobalBusi(),(*it).first, lpReq->nAction, lpReq->nMemID[0], pFamily->m_FamilyID, pFamily->m_FamilyProp, pFamily->m_FamilyName,0,lpReq->nInviteID);
					}
				}
					
			}

			
		}
 	}
 	else if (lpReq->nFamilyProp == TEMP_FAMILY)	//临时家族暂只有添加成员
 	{
		CTempFamilyManager& pTempFamilyManager = CAppManager::GetInstance().GetTempFamilyManager();
		CTempFamily *pFamily = pTempFamilyManager.Find(lpReq->nFamilyID);
		if (!pFamily)
		{
			lpAck->ret = RESULT_FAILED;
			strcpy_s(szError,"添加成员失败，请稍后再试");
			goto end;
		}

		if (pFamily->GetCurMemberNum() + lpReq->nCount > pFamily->GetMaxMemberNum())
		{
			lpAck->ret = RESULT_FAILED;
			strcpy_s(szError,"您添加的成员过多，临时群最多支持10人在线聊天!");
			goto end;
		}
		char szfamilyname[MAX_FAMILYNAME_LEN + 1];
		memset(szfamilyname, 0, sizeof(szfamilyname));
		for( int i = 0; i < lpReq->nCount; ++i)
		{
			if (RESULT_SUCCESS == pFamily->AddMember(lpReq->nMemID[i]))
			{
				pUser = pUserManager->Find(lpReq->nMemID[i]);
				if (pUser)				//用户在线
					pUser->Add_Temp_Family(pFamily->m_FamilyID);
				CBusinessFamilyGlobalFunc familyGlobalFunc;

				SafeList<uint32>::iterator it = pFamily->m_FamilyMemberList.begin();
				for(; it != pFamily->m_FamilyMemberList.end(); ++it)
				{
					if ((*it) != m_pRecvHead->sid)
					{
						familyGlobalFunc.TransitSetMemberMsg(m_pRouter->GetGlobalBusi(),(*it), lpReq->nAction, lpReq->nMemID[i], pFamily->m_FamilyID, pFamily->m_FamilyProp, szfamilyname);
					}
				}
			}
		}
 	}
 	else	//错误的家族类型
 	{
		this->CancelReply();
 		lpAck->ret = RESULT_FAILED;
 	}
 
end:
	if (lpAck->ret == RESULT_FAILED)
	{
		m_SendPacket<<uint8(RESULT_FAILED)
			<<lpReq->nFamilyID
			<<lpReq->nFamilyProp
			<<lpReq->nAction
			<<lpAck->nMemberID
			<<szError;
	}
	else
        m_SendPacket.SetCursor(sizeof(XT_SET_CMEMBERS_ACK));
	return RESULT_SUCCESS;
}

CBusinessFamilyApply::CBusinessFamilyApply(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_CLUSTER_APPLY_REQ,CMD_CLUSTER_APPLY_ACK)
{
	m_pDataBaseFamily = pRouter->GetDataBaseFamily();
	m_pDataBaseMini = pRouter->GetDataBaseMini();
}

int CBusinessFamilyApply::DoWork()
{
	XT_CLUSTER_APPLY_REQ Req;
	m_RecvPacket>>Req.nFamilyID
		>>Req.nFamilyProp
		>>Req.nAction
		>>Req.nUserID;

	int dbret;
	char szError[MAX_ERROR_BUF];

	LPXT_CLUSTER_APPLY_ACK lpAck = (XT_CLUSTER_APPLY_ACK * )m_SendPacket.GetData();
	lpAck->ret			= RESULT_SUCCESS;
	lpAck->nFamilyID	= Req.nFamilyID;
	lpAck->nFamilyProp	= PERM_FAMILY;
	lpAck->nAction		= Req.nAction;

	bool bReleaseFamily = false;


	if (Req.nAction == FAMILY_REQ_JOIN)
	{
		if (Req.nFamilyProp == PERM_FAMILY)	//固定家族
		{		
			CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
			CPermFamily *pFamily = pPermFamilyManager.Find(Req.nFamilyID);
			CBusinessFamilyGlobalFunc familyGlobalFunc;

			int retCode = 0;
			dbret = m_pDataBaseFamily->Get_Family_BlackList(Req.nFamilyID,Req.nUserID,retCode);
			if (dbret == DB_SUCCESS && retCode == 9)
			{
				lpAck->ret = RESULT_FAILED;
				strcpy_s(szError, "该家族拒绝您的加入！");
				goto end;
			}

			if(!pFamily)	//无该家族信息
			{
				dbret = familyGlobalFunc.GetFamilyInfo(Req.nFamilyID, m_pDataBaseFamily, m_pDataBaseMini,m_pAppNetwork, m_pRouter->GetGlobalFamilyBusi());
				if (dbret != DB_SUCCESS)
				{
					lpAck->ret = RESULT_FAILED;
					strcpy_s(szError, "加入失败，请稍后再试");
					goto end;
				}

				pFamily = pPermFamilyManager.Find(Req.nFamilyID);
				if (!pFamily)
				{
					m_SendPacket<<(uint8)RESULT_FAILED<<"获取家族信息失败";
					return RESULT_FAILED;
				}
				m_pDataBaseMini->Get_Family_MaxMsgID(pFamily->m_FamilyID,pFamily->m_FamilyMaxMsgID,pFamily->m_FamilyMinMsgID);
				pFamily->m_FamilyBufMinMsgID = pFamily->m_FamilyMaxMsgID;
				bReleaseFamily = true;
			}
			switch(pFamily->m_JoinFlag)
			{
			case FAMILY_JOIN_ANY:  // 不需要验证
				{
					// 写数据库
					if(pFamily->GetCurMemberNum() >= pFamily->GetMaxMemberNum())
					{
						lpAck->ret = RESULT_FAILED;
						strcpy_s(szError, "该家族成员已满");
						goto end;
					}
					else
					{
						int nReqcount = 1;
						dbret = m_pDataBaseFamily->Family_Add_Member(Req.nFamilyID, &Req.nUserID, nReqcount);
						if (dbret != DB_SUCCESS)
						{
							lpAck->ret = RESULT_FAILED;
							strcpy_s(szError, "加入失败，请稍后再试");
							goto end;
						}
						char strUserID[10] = {0};
						_itoa_s(Req.nUserID, strUserID, 10);
						char strFamilyID[10] = {0};
						_itoa_s(pFamily->m_FamilyID, strFamilyID, 10);
						m_pRouter->GetDataBaseWeb()->Write_System_Log(130201,strUserID,strFamilyID,13,m_pRecvHead->sid);
						//modify by lwl ver:0102
						shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(Req.nUserID);
						if (pUser != NULL)	
							pUser->Add_Perm_Family(pFamily->m_FamilyID, pFamily->m_FamilyMaxMsgID);
						else		//非直连本服务器
							familyGlobalFunc.TransitSetMemberMsg(m_pRouter->GetGlobalBusi(),Req.nUserID, Req.nAction, Req.nUserID, pFamily->m_FamilyID, pFamily->m_FamilyProp, pFamily->m_FamilyName,pFamily->m_FamilyMaxMsgID);

						if (!bReleaseFamily)
						{
							pFamily->AddMember(Req.nUserID, FAMILY_NORMAL, XTREAM_ONLINE);
							bReleaseFamily = false;
							CBusinessFamilyGlobalFunc familyGlobalFunc;

							//向家族所有成员发通知消息
							CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
							FamilyMember_Map::iterator it = pFamily->m_FamilyMembers.begin();
							for(; it != pFamily->m_FamilyMembers.end(); ++it)
							{
								if ((*it).second.nStat != XTREAM_OFFLINE && (*it).first != Req.nUserID)
									familyGlobalFunc.TransitSetMemberMsg(m_pRouter->GetGlobalBusi(),(*it).first, Req.nAction, Req.nUserID, pFamily->m_FamilyID, pFamily->m_FamilyProp, pFamily->m_FamilyName);
							}
						}
						lpAck->nFlag = FAMILY_JOIN_ANY;
					}
					break;
				}
			case FAMILY_JOIN_VALIDATE: // 需要验证
				{
					lpAck->nFlag = FAMILY_JOIN_VALIDATE;
					break;
				}
			case FAMILY_JOIN_UNALLOW: // 不允许添加
				{
					lpAck->nFlag = FAMILY_JOIN_UNALLOW;
					break;
				}
			default:
					return RESULT_FAILED;
			}

			if (bReleaseFamily)	//申请加入前该家族无在线成员
			{
				CBusinessFamilyGlobalFunc familyGlobalFunc;
				familyGlobalFunc.ReleaseFamilyInfo(Req.nFamilyID,m_pAppNetwork,m_pRouter->GetGlobalFamilyBusi(),m_pRouter->GetDataBaseMini());
			}
		}
		else if (Req.nFamilyProp == TEMP_FAMILY)
			return RESULT_FAILED;
	}
	else if (Req.nAction == FAMILY_REQ_QUIT)
	{
		if (Req.nFamilyProp == PERM_FAMILY)
		{
			CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
			CPermFamily *pFamily = pPermFamilyManager.Find(Req.nFamilyID);


			if(!pFamily)	//无该家族信息
				return RESULT_FAILED;

			dbret = m_pDataBaseFamily->Family_Delete_Member(Req.nFamilyID,Req.nUserID);
			if (dbret != DB_SUCCESS)
			{
				lpAck->ret = RESULT_FAILED;
				strcpy_s(szError, "退出失败，请稍后再试");
				goto end;
			}

			char strUser[10] = {0};
			_itoa_s(Req.nUserID,strUser,10);
			char strFamilyID[10] = {0};
			_itoa_s(Req.nFamilyID, strFamilyID, 10);
			m_pRouter->GetDataBaseWeb()->Write_System_Log(130202,strUser,strFamilyID,13,m_pRecvHead->sid);


			
			m_pRouter->GetDataBaseMini()->Delete_FamilyMem_RecvMsg(Req.nFamilyID, Req.nUserID); //add by lwl ver:0102
			CBusinessFamilyGlobalFunc familyGlobalFunc;
			pFamily->m_FamilyMembers.Lock();
			FamilyMember_Map::iterator it = pFamily->m_FamilyMembers.begin();
			bReleaseFamily = true;
			for(; it != pFamily->m_FamilyMembers.end(); ++it)
			{
				if ((*it).second.nStat != XTREAM_OFFLINE )
				{
					familyGlobalFunc.TransitSetMemberMsg(m_pRouter->GetGlobalBusi(),(*it).first, Req.nAction, Req.nUserID, pFamily->m_FamilyID, pFamily->m_FamilyProp,pFamily->m_FamilyName);
					if ((*it).first != Req.nUserID)
						bReleaseFamily = false;
				}
			}
			pFamily->m_FamilyMembers.UnLock();
			pFamily->DelMember(Req.nUserID);
			shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(Req.nUserID);
			if (pUser != NULL)	
				pUser->Del_Perm_Family(pFamily->m_FamilyID);
			if (bReleaseFamily)	//退出后该家族无在线成员
			{
				CBusinessFamilyGlobalFunc familyGlobalFunc;
				familyGlobalFunc.ReleaseFamilyInfo(Req.nFamilyID,m_pAppNetwork,m_pRouter->GetGlobalFamilyBusi(),m_pRouter->GetDataBaseMini());
			}

		}
		else if (Req.nFamilyProp == TEMP_FAMILY)
		{
			if (m_pRecvHead->flag == XT_HEAD::FLAG_SRVACTION)
			{
				m_pSendHead->dtype	= XT_HEAD::DEST_TYPE_SERVER;
			}

			CTempFamilyManager& pTempFamilyManager = CAppManager::GetInstance().GetTempFamilyManager();
			CTempFamily *pFamily = pTempFamilyManager.Find(Req.nFamilyID);
			if (!pFamily)
			{
				lpAck->ret = RESULT_FAILED;
				strcpy_s(szError,"该临时家族已经被解散！");
				goto end;
			}

			lpAck->nFamilyProp	= TEMP_FAMILY;

			pFamily->DelMember(m_pRouter->GetGlobalBusi(), Req.nUserID,m_pRouter->GetGlobalFamilyBusi());
			CBusinessFamilyGlobalFunc familyGlobalFunc;

			shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(Req.nUserID);
			if (pUser != NULL)	
				pUser->Del_Temp_Family(pFamily->m_FamilyID);
			else
				familyGlobalFunc.TransitSetMemberMsg(m_pRouter->GetGlobalBusi(),Req.nUserID, Req.nAction, Req.nUserID, pFamily->m_FamilyID, pFamily->m_FamilyProp, "");
		}
	}
	else	//错误的操作类型
	{
		return RESULT_FAILED;
	}
end:
	if (lpAck->ret == RESULT_FAILED)
	{
		m_SendPacket<<(uint8)RESULT_FAILED
					<<Req.nFamilyID
					<<Req.nFamilyProp
					<<Req.nAction
					<<szError;
	}
	else
        m_SendPacket.SetCursor(sizeof(XT_CLUSTER_APPLY_ACK));


	return RESULT_SUCCESS;
}


/*
void CBusinessFamilyApply::TransitApplyMsg(uint32 recvID, uint8 action, uint32 actionID, uint32 familyID, uint8 familyProp, char* familyName)
{
	int len = 0;
	SendDataStruct sendData;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
	LPXT_SERVER_TRANSMIT lpSrvTransmit = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
	lpSrvTransmit->origin_id = familyID;
	lpSrvTransmit->msgtype = CMD_SET_CMEMBERS_REQ;
	len += sizeof(XT_SERVER_TRANSMIT);

	CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMzIT));
	sendPacket<<action
			  <<actionID
			  <<familyID
			  <<familyName
			  <<familyProp;
	len += sendPacket.GetSize();	

	CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,len,recvID);
	sendData.data_len = len + (int)sizeof(XT_HEAD);
	m_pAppNetwork->SendDataTo(recvID,sendData.data,sendData.data_len,3);


// 	sp2 << (uint32)10000 << (uint32)pUser->m_userInfo.clientNo << (uint32)_pUser->m_userInfo.natIP
// 		<< (uint16)_pUser->m_userInfo.natPort << (uint16)CLIENT_DELADD_MEMBER ;
// 	sp2 << (uint8)_action << (uint32)_pUser->m_userInfo.clientNo <<(char*)_pUser->m_userInfo.nickname<< (uint32)_clusterno << (uint8)_type;
}
*/

CBusinessFamilyVerify::CBusinessFamilyVerify(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_CLUSTER_AUTH_REQ,CMD_CLUSTER_AUTH_ACK)
{
	m_pDataBaseFamily = pRouter->GetDataBaseFamily();
	m_pDataBaseMini = pRouter->GetDataBaseMini();
}

int CBusinessFamilyVerify::DoWork()
{
	LPXT_CLUSTER_AUTH_REQ lpReq = (XT_CLUSTER_AUTH_REQ *)m_RecvPacket.GetData();

	time_t tm;
	time(&tm);
	int dbret;
 	char szError[MAX_ERROR_BUF];
 
 	LPXT_CLUSTER_AUTH_ACK lpAck = (XT_CLUSTER_AUTH_ACK * )m_SendPacket.GetData();
 	lpAck->ret			= RESULT_SUCCESS;
 	lpAck->nFamilyID	= lpReq->nFamilyID;
 	lpAck->nFamilyProp	= lpReq->nFamilyProp;
 	lpAck->nAction		= lpReq->nAction;

	if (lpReq->nFamilyProp != PERM_FAMILY)
		return RESULT_FAILED;

	CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
	CPermFamily *pFamily = pPermFamilyManager.Find(lpReq->nFamilyID);

	uint8 nCount = 1;
	tagGSRV_GET_USER_STATUS_ACK::tagUserStatus UStatus;
	UStatus.id = lpReq->nRequestID;
	UStatus.status = XTREAM_OFFLINE;
#ifdef IM_USE_CACHE	
	CMemcacheQueryUserStatusi(m_pRouter,&lpReq->nRequestID,nCount,&UStatus);
#else
	CGlobalBusi* pGlobalBusi = m_pRouter->GetGlobalBusi();
	if( RESULT_FAILED == pGlobalBusi->QueryUserStatus(&lpReq->nRequestID, &UStatus, nCount))
	{
		UStatus.id = lpReq->nRequestID;
		UStatus.status = XTREAM_OFFLINE;
	}
#endif	
 	if (lpReq->nAction == FAMILY_JOIN_REQUEST)
 	{
		if(!pFamily)	//无该家族信息 	没有人在线
		{
			dbret = m_pDataBaseMini->Save_Family_RequestMsg(lpReq->nFamilyID, lpReq->nRequestID, lpReq->send_time, m_RecvPacket.GetData(), m_RecvPacket.GetSize());
			if (dbret != DB_SUCCESS)
			{
				lpAck->ret = RESULT_FAILED;
				strcpy_s(szError, "请求消息处理失败");
			}
			goto end;				
		}
		int num = 0;
		CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
		FamilyMember_Map::iterator it = pFamily->m_FamilyMembers.begin();
		for(; it != pFamily->m_FamilyMembers.end(); ++it)
		{
			if ((*it).second.nProp == FAMILY_CREATER || (*it).second.nProp == FAMILY_MANAGER )
			{
				if ((*it).second.nStat != XTREAM_OFFLINE)
				{
					TransitFamilyVerifyMsg((*it).first, pFamily->m_FamilyID,(char*)m_RecvPacket.GetData(), m_RecvPacket.GetSize());
					++num;
				}
			}
		}
		if( num ==0 )	//没有管理员在线
		{
			//保存到数据库
			dbret = m_pDataBaseMini->Save_Family_RequestMsg(lpReq->nFamilyID, lpReq->nRequestID, lpReq->send_time, m_RecvPacket.GetData(), m_RecvPacket.GetSize());
			if (dbret != DB_SUCCESS)
			{
				lpAck->ret = RESULT_FAILED;
				strcpy_s(szError, "请求消息处理失败");
			}
			goto end;
		}
	}
	else if (lpReq->nAction == FAMILY_JOIN_AGREE)
	{
		if (pFamily->GetCurMemberNum() >= pFamily->GetMaxMemberNum())
		{
			lpAck->ret = RESULT_FAILED;
			strcpy_s(szError, "家族成员已满");
			goto end;
		}
		
		if (pFamily->IsMember(lpReq->nRequestID) == RESULT_SUCCESS)	//请求方已经在家族中（已经有管理员同意)
			goto end;
		int nReqcount = 1;
		dbret = m_pDataBaseFamily->Family_Add_Member(pFamily->m_FamilyID, &lpReq->nRequestID, nReqcount);
		if (dbret != DB_SUCCESS)
		{
			lpAck->ret = RESULT_FAILED;
			strcpy_s(szError, "成员添加失败");
			goto end;
		}
		char strUserID[10] = {0};
		_itoa_s(lpReq->nRequestID, strUserID, 10);
		char strFamilyID[10] = {0};
		_itoa_s(pFamily->m_FamilyID, strFamilyID, 10);
		m_pRouter->GetDataBaseWeb()->Write_System_Log(130201,strUserID,strFamilyID,13,m_pRecvHead->sid);

		pFamily->AddMember(lpReq->nRequestID);

		if (UStatus.status == XTREAM_OFFLINE) //保存同意消息
		{
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
			m_pDataBaseMini->Save_User_Msg(
					lpReq->nRequestID, 
					0, 
					0, 
					0, 
					(uint32)tm, 
					CMD_CLUSTER_AUTH_REQ, 
					m_RecvPacket.GetData(), 
					m_RecvPacket.GetSize(),
					msg.data);
#endif
		}
		else
		{
			pFamily->SetMemStat(lpReq->nRequestID, UStatus.status);
			//向用户发送同意消息	收到该消息后修改用户家族列表
			TransitFamilyVerifyMsg(lpReq->nRequestID, lpReq->nFamilyID, (char*)m_RecvPacket.GetData(), m_RecvPacket.GetSize());			
			shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(lpReq->nRequestID);
			if (pUser != NULL)
				pUser->Add_Perm_Family(lpReq->nFamilyID, 0);
		}

		//通知家族内所有成员有人加入
		CBusinessFamilyGlobalFunc familyGlobalFunc;
		CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
		FamilyMember_Map::iterator it = pFamily->m_FamilyMembers.begin();
		for(; it != pFamily->m_FamilyMembers.end(); ++it)
		{
			if ((*it).second.nStat != XTREAM_OFFLINE && (*it).first != lpReq->nManagerID && (*it).first != lpReq->nRequestID)	//同意加入方不用接收该消息
                familyGlobalFunc.TransitSetMemberMsg(m_pRouter->GetGlobalBusi(),(*it).first, CMEMBER_ADD, lpReq->nRequestID, lpReq->nFamilyID,pFamily->m_FamilyProp,pFamily->m_FamilyName);
		}

	}
	else if (lpReq->nAction == FAMILY_JOIN_REFUSE)
	{
		if (UStatus.status == XTREAM_OFFLINE) //保存拒绝消息
		{
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

			m_pDataBaseMini->Save_User_Msg(
					lpReq->nRequestID, 
					0, 
					0, 
					0, 
					(uint32)tm, 
					CMD_CLUSTER_AUTH_REQ, 
					m_RecvPacket.GetData(), 
					m_RecvPacket.GetSize(),
					msg.data);
#endif

		}
		else
		{
			//向用户发送拒绝消息
			TransitFamilyVerifyMsg(lpReq->nRequestID, lpReq->nFamilyID, (char*)m_RecvPacket.GetData(), m_RecvPacket.GetSize());
		}
	}
	else //错误的操作类型
 		return RESULT_FAILED;
end:
	if (lpAck->ret == RESULT_FAILED)
	{
		m_SendPacket<<(uint8)RESULT_FAILED
			<<lpReq->nFamilyID
			<<lpReq->nFamilyProp
			<<lpReq->nAction
			<<szError;
	}
	else
		m_SendPacket.SetCursor(sizeof(XT_CLUSTER_AUTH_ACK));

	return RESULT_SUCCESS;
}


void CBusinessFamilyVerify::TransitFamilyVerifyMsg(uint32 recvID, uint32 familyID, char* msg,int msgLen)
{
	int len = 0;
	SendDataStruct sendData;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
	LPXT_SERVER_TRANSMIT lpSrvTransmit = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
	lpSrvTransmit->origin_id = familyID;
	lpSrvTransmit->msgtype = CMD_CLUSTER_AUTH_REQ;
	len += sizeof(XT_SERVER_TRANSMIT);

	CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT));
	sendPacket.WritePacket(msg, msgLen);
	len += sendPacket.GetSize();	

	CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,len,recvID);
	sendData.data_len = len + (int)sizeof(XT_HEAD);

	m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),sendData.data,sendData.data_len,SEND_RETRY_TIME);

}

CBusinessFamilyFind::CBusinessFamilyFind(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_CLUSTER_SEARCH_REQ,CMD_CLUSTER_SEARCH_ACK)
{
	m_pDataBaseFamily = pRouter->GetDataBaseFamily();
}

int CBusinessFamilyFind::DoWork()
{
	XT_CLUSTER_SEARCH_REQ Req;

	m_RecvPacket>>Req.start_pos
				>>Req.search_type
				>>Req.nCount;
	switch(Req.search_type)
	{
	case CLUSTER_SEARCH_ID:
		m_RecvPacket>>Req.type_data.nFamilyID;
		break;
	case CLUSTER_SEARCH_CLASS:
		m_RecvPacket>>Req.type_data.nFamilyClass;
		break;
	case CLUSTER_SEARCH_NAME:
		m_RecvPacket>>Req.type_data.szFamilyName;
		break;
	default:
		return RESULT_FAILED;
	}
	
	if (Req.nCount == 0)
		Req.nCount = MAX_FAMILYFIND_RETNUM;

	//CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
	
	int dbret;
	if(Req.search_type == CLUSTER_SEARCH_ID) //精确查找
	{
		//暂时改成全部load库
		//CPermFamily *pFamily = pPermFamilyManager.Find(Req.type_data.nFamilyID);
		//if (!pFamily)	//家族不在该服务器上
		{
			XT_CLUSTER_SEARCH_ACK::tagFamilyFindRet	FamilyData;
			int nRetCount = 1;
			dbret = m_pDataBaseFamily->Family_Find(Req, &FamilyData, nRetCount);
			if (dbret != DB_SUCCESS)
			{
				m_SendPacket<<(uint8)RESULT_SUCCESS
							<<(uint16)0xFFFF
							<<(uint8)0;
				return RESULT_SUCCESS;								
			}

			m_SendPacket<<(uint8)RESULT_SUCCESS
						<<(uint16)0xFFFF
						<<(uint8)nRetCount
						<<FamilyData.nFamilyID
						<<FamilyData.nCreaterID
						<<FamilyData.nJoinFlag
						<<FamilyData.nFamilyClass
						<<FamilyData.szFamilyName
						<<FamilyData.szFamilyDomain
						<<FamilyData.nMemCount;
			CLog::Log("CBusinessFamilyFind",CLog::TYPE_IMPORTANT, "FamilyID:%u, FamilyName:%s, MemCount:%d ",FamilyData.nFamilyID,FamilyData.szFamilyName,FamilyData.nMemCount);
			
		}
		/*
		else
		{	
			m_SendPacket<<(uint8)RESULT_SUCCESS
						<<(uint16)0xFFFF
						<<(uint8)1
						<<pFamily->m_FamilyID
						<<pFamily->m_CreatorID
						<<pFamily->m_JoinFlag
						<<pFamily->m_FamilyClass
						<<pFamily->m_FamilyName
						<<pFamily->m_FamilyDomain;

		}
		*/
	}
 	else if(Req.search_type == CLUSTER_SEARCH_CLASS)// 分类查找
 	{
		XT_CLUSTER_SEARCH_ACK::tagFamilyFindRet	FamilyData[MAX_FAMILYFIND_RETNUM];
		int nRetCount = Req.nCount;
		dbret = m_pDataBaseFamily->Family_Find(Req, FamilyData, nRetCount);
		if (dbret != DB_SUCCESS)
		{
			m_SendPacket<<(uint8)RESULT_SUCCESS
						<<(uint16)0xFFFF
						<<(uint8)0;
			return RESULT_SUCCESS;								
		}

		int next_pos = Req.start_pos + nRetCount;

		if (nRetCount < Req.nCount)	//返回个数少于请求个数	表示所有都返回了。
			next_pos = 0xFFFF;


		m_SendPacket<<(uint8)RESULT_SUCCESS
			<<(uint16)next_pos
			<<(uint8)nRetCount;
		for (int i = 0; i < nRetCount; ++i)
		{
			m_SendPacket<<FamilyData[i].nFamilyID
						<<FamilyData[i].nCreaterID
						<<FamilyData[i].nJoinFlag
						<<FamilyData[i].nFamilyClass
						<<FamilyData[i].szFamilyName
						<<FamilyData[i].szFamilyDomain;
		}
	}
 	else if(Req.search_type == CLUSTER_SEARCH_NAME ) //????????
 	{
		XT_CLUSTER_SEARCH_ACK::tagFamilyFindRet	FamilyData[MAX_FAMILYFIND_RETNUM];
		int nRetCount = Req.nCount + 1;			
		
		dbret = m_pDataBaseFamily->Family_Find(Req, FamilyData, nRetCount);
		if (dbret != DB_SUCCESS)
		{
			m_SendPacket<<(uint8)RESULT_SUCCESS
						<<(uint16)0xFFFF
						<<(uint8)0;
			return RESULT_SUCCESS;								
		}

		int next_pos = Req.start_pos + nRetCount - 1;
		//int next_pos = Req.start_pos + nRetCount;
		
		CLog::Log("CBusinessFamilyFind",CLog::TYPE_IMPORTANT, "nRetCount:%u, Req.nCount:%u ",nRetCount,Req.nCount);

		if (nRetCount <= Req.nCount)	//??????????????????	??????ж????????
			next_pos = 0xFFFF;
		
		CLog::Log("CBusinessFamilyFind",CLog::TYPE_IMPORTANT, "next_pos %#x, Req.nCount %u ",next_pos,Req.nCount);
		
		nRetCount = (nRetCount <= Req.nCount? nRetCount:Req.nCount);

		m_SendPacket<<(uint8)RESULT_SUCCESS
			<<(uint16)next_pos
			<<(uint8)nRetCount;
		for (int i = 0; i < nRetCount; ++i)
		{
			m_SendPacket<<FamilyData[i].nFamilyID
						<<FamilyData[i].nCreaterID
						<<FamilyData[i].nJoinFlag
						<<FamilyData[i].nFamilyClass
						<<FamilyData[i].szFamilyName
						<<FamilyData[i].szFamilyDomain
						<<FamilyData[i].nMemCount;
			CLog::Log("CBusinessFamilyFind",CLog::TYPE_IMPORTANT, "FamilyID:%u, FamilyName:%s, MemCount:%d ",FamilyData[i].nFamilyID,FamilyData[i].szFamilyName,FamilyData[i].nMemCount);
			
						
		}
 	}
 	else
	{
		m_SendPacket<<(uint8)RESULT_SUCCESS
			<<(uint16)0xFFFF
			<<(uint8)0;
		return RESULT_SUCCESS;								
	}

	return RESULT_SUCCESS;
}

CBusinessDismissFamily::CBusinessDismissFamily(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_CDISMISS_REQ,CMD_CDISMISS_ACK)
{
	m_pDataBaseFamily = pRouter->GetDataBaseFamily();
	m_pDataBaseMini = pRouter->GetDataBaseMini();
}

int CBusinessDismissFamily::DoWork()
{
	LPXT_CLUSTER_DISMISS_REQ lpReq = (XT_CLUSTER_DISMISS_REQ *)m_RecvPacket.GetData();

	int dbret;

	LPXT_CLUSTER_DISMISS_ACK lpAck = (XT_CLUSTER_DISMISS_ACK *)m_SendPacket.GetData();
	lpAck->ret			= RESULT_SUCCESS;
	lpAck->nFamilyID	= lpReq->nFamilyID;
	lpAck->nFamilyProp	= lpReq->nFamilyProp;

	char szError[MAX_ERROR_BUF];

	if (lpReq->nFamilyProp == PERM_FAMILY)	//固定家族
	{		
		CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
		CPermFamily *pFamily = pPermFamilyManager.Find(lpReq->nFamilyID);

		if (!pFamily)
			return RESULT_FAILED;

		if (pFamily->IsCreater(m_pRecvHead->sid) != RESULT_SUCCESS)
		{
			lpAck->ret = RESULT_FAILED;
			strcpy_s(szError, "您没有权限解散该家族");
			goto end;
		}	

		CLog::Log("CBusinessDismissFamily::DoWork()",CLog::TYPE_DEBUG,"用户:%d 解散家族:%d ", m_pRecvHead->sid,lpReq->nFamilyID);
		dbret = m_pDataBaseFamily->Delete_Family(lpReq->nFamilyID);
		if (dbret != DB_SUCCESS)
		{
			char szError[MAX_ERROR_BUF];
			CError::DB_Error(dbret, szError, true);
			m_SendPacket<<uint8(RESULT_FAILED)<<szError;
			return RESULT_SUCCESS;
		}

		char strFamilyID[10] = {0};
		_itoa_s(pFamily->m_FamilyID, strFamilyID, 10);
		char strUserID[10] = {0};
		_itoa_s(m_pRecvHead->sid, strUserID, 10);
		m_pRouter->GetDataBaseWeb()->Write_System_Log(130205,strUserID,strFamilyID,13,m_pRecvHead->sid);
		m_pDataBaseMini->Delete_Family_Msg(lpReq->nFamilyID);

		FamilyMember_Map::iterator it = pFamily->m_FamilyMembers.begin();

		for(; it != pFamily->m_FamilyMembers.end(); ++it)
		{
			if((*it).second.nStat == XTREAM_OFFLINE)
				continue;

			//当创建者收到该消息时从内存中移除该家族。不用下发该消息,其它的要下发
			CUserManager *pUserManager = m_pRouter->GetUserManager();
			shared_ptr<CUser> pUser = pUserManager->Find(m_pRecvHead->sid);
			if (pUser != NULL)				//用户在线
				pUser->Del_Perm_Family(lpReq->nFamilyID);
			TransitDisbandMsg((*it).first, m_pRecvHead->sid, lpReq->nFamilyID, lpReq->nFamilyProp);
		}
		CBusinessFamilyGlobalFunc familyGlobalFunc;

		familyGlobalFunc.ReleaseFamilyInfo(lpReq->nFamilyID,m_pAppNetwork,m_pRouter->GetGlobalFamilyBusi(),m_pRouter->GetDataBaseMini());
	}
	else if (lpReq->nFamilyProp == TEMP_FAMILY)	
	{
		CTempFamilyManager& pTempFamilyManager = CAppManager::GetInstance().GetTempFamilyManager();
		CTempFamily *pFamily = pTempFamilyManager.Find(lpReq->nFamilyID);
		if (!pFamily)
			return RESULT_FAILED;

		SafeList<uint32>::iterator it = pFamily->m_FamilyMemberList.begin();
		for(; it != pFamily->m_FamilyMemberList.end(); ++it)
		{
			CUserManager *pUserManager = m_pRouter->GetUserManager();
			shared_ptr<CUser> pUser = pUserManager->Find(m_pRecvHead->sid);
			if (pUser != NULL)				//用户在线
				pUser->Del_Temp_Family(lpReq->nFamilyID);

			//当创建者收到该消息时从内存中移除该家族。不用下发该消息,其它要同时下发该消息
			TransitDisbandMsg(*it, m_pRecvHead->sid, lpReq->nFamilyID, lpReq->nFamilyProp);
		}

		pTempFamilyManager.DelHash(lpReq->nFamilyID);
	}
	else	//错误的家族类型
	{
		return RESULT_FAILED;
	}
end:
	if (lpAck->ret == RESULT_FAILED)
	{
		m_SendPacket<<uint8(RESULT_FAILED)
					<<szError;
	}
	else
		m_SendPacket.SetCursor(sizeof(XT_CLUSTER_DISMISS_ACK));

	return RESULT_SUCCESS;
}

void CBusinessDismissFamily::TransitDisbandMsg(uint32 recvID,uint32 senderID, uint32 familyID, uint8 familyProp)
{
	int len = 0;
	SendDataStruct sendData;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
	LPXT_SERVER_TRANSMIT lpSrvTransmit = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
	lpSrvTransmit->origin_id = familyID;
	lpSrvTransmit->msgtype = CMD_CDISMISS_REQ;
	len += sizeof(XT_SERVER_TRANSMIT);

	CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT));
	sendPacket<<senderID
			  <<familyID
			  <<familyProp;	
	len += sendPacket.GetSize();

	CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,len,recvID);
	sendData.data_len = len + (int)sizeof(XT_HEAD);
	m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),sendData.data,sendData.data_len,SEND_RETRY_TIME);
	return;

// 	sp2 << (uint32)10000 << (uint32)pUser->m_userInfo.clientNo << (uint32)_pUser->m_userInfo.natIP
// 		<< (uint16)_pUser->m_userInfo.natPort << (uint16)CLIENT_DEL_CLUSTER ;
// 
// 	sp2 << (uint32)_pUser->m_userInfo.clientNo<<(uint32)_clusterno<<(uint8)_type;

}

CBusinessFamilySendMsg::CBusinessFamilySendMsg(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_CLUSTERMSG_SEND,CMD_CLUSTERMSG_ACK)
{
	m_pDataBaseMini = pRouter->GetDataBaseMini();
}

int CBusinessFamilySendMsg::DoWork()
{
 	XT_FAMILY_SENDMSG_REQ Req;
	m_RecvPacket>>Req.familyID
				>>Req.senderID
				>>Req.version
				>>Req.familyProp
				>>Req.msgType;
 
 	//LPXT_FAMILY_SENDMSG_ACK lpAck = (XT_FAMILY_SENDMSG_ACK *)m_SendPacket.GetData();
	m_SendPacket<<(uint8)RESULT_SUCCESS
				<<m_pRecvHead->did
				<<Req.msgType;

	int nSendUserNum =0;
	int retrytime = SEND_RETRY_TIME + SEND_RETRY_TIME;
	if( Req.familyProp == PERM_FAMILY ) // 固定群
	{
		CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
		CPermFamily *pFamily = pPermFamilyManager.Find(Req.familyID);
	
		if (!pFamily)
		 	return RESULT_FAILED;

		CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
		pFamily->m_FamilyMaxMsgID++;

		// 保存该消息到数据库
 		if( Req.msgType == 0x00 )
		{
			time_t tm;
			time(&tm);



			//modify by lwl ver:0106
// 			pFamily->SaveFamilyMsg( pFamily->m_FamilyMaxMsgID,
// 									Req.senderID,
// 									static_cast<int>(tm),
// 									CMD_CLUSTERMSG_SEND,
// 									m_RecvPacket.GetSize(),
// 									m_RecvPacket.GetData());

			m_pDataBaseMini->Save_Family_Msg(pFamily->m_FamilyMaxMsgID,
											pFamily->m_FamilyID,
											Req.senderID,
											static_cast<int>(tm),
											CMD_CLUSTERMSG_SEND,
											m_RecvPacket.GetSize(),
											m_RecvPacket.GetData());
		
		}

		//	retrytime = PIC_RETRY_TIME;
		FamilyMember_Map::iterator it = pFamily->m_FamilyMembers.begin();
		for(; it != pFamily->m_FamilyMembers.end(); ++it)
		{
			if (/*(*it).first == Req.senderID ||*/ (*it).second.nStat == XTREAM_OFFLINE) 
				continue;

			if ((*it).first == Req.senderID )
			{
				shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(Req.senderID);
				if (pUser != NULL)
				{
					pUser->Set_Family_MsgID(pFamily->m_FamilyID, pFamily->m_FamilyMaxMsgID);
					continue;
				}
			}
			//服务器中转该消息时需要修改用户接收到的家族消息ID
			TransitFamilyMsg((*it).first, pFamily->m_FamilyID,pFamily->m_FamilyMaxMsgID,(char*)m_RecvPacket.GetData(), m_RecvPacket.GetSize(), PERM_FAMILY, retrytime);
			++nSendUserNum;
		}
 	}
 	else if( Req.familyProp == TEMP_FAMILY )// 临时群
 	{


		CTempFamilyManager& pTempFamilyManager = CAppManager::GetInstance().GetTempFamilyManager();
		CTempFamily *pFamily = pTempFamilyManager.Find(Req.familyID);

		if (!pFamily)
			return RESULT_FAILED;

		SafeList<uint32>::iterator it = pFamily->m_FamilyMemberList.begin();
		for(; it != pFamily->m_FamilyMemberList.end(); ++it)
		{
			if ((*it) == Req.senderID)
				continue;
			TransitFamilyMsg(*it,pFamily->m_FamilyID, 0, (char*)m_RecvPacket.GetData(), m_RecvPacket.GetSize(), TEMP_FAMILY, retrytime);
			++nSendUserNum;
		}
 	}
 	else
		return RESULT_FAILED;

	if (Req.msgType == 0x08)
	{
		AddFileBuf(nSendUserNum,Req.familyID);
	}

	return RESULT_SUCCESS;
}

void CBusinessFamilySendMsg::TransitFamilyMsg(uint32 recvID,uint32 familyID,uint32 msgID,char * msg,int msgLen, uint8 familyProp, int retrytime)
{
	int len = 0;
	SendDataStruct sendData;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
	LPXT_SERVER_TRANSMIT lpSrvTransmit = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
	lpSrvTransmit->origin_id = familyID;
	lpSrvTransmit->msgtype = CMD_CLUSTERMSG_SEND;
	len += sizeof(XT_SERVER_TRANSMIT);

	CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT));
	sendPacket<<msgID;
	sendPacket.WritePacket(msg, msgLen);
	len += sendPacket.GetSize();	

	CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,len,recvID);
	sendData.data_len = len + (int)sizeof(XT_HEAD);
	m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),sendData.data,sendData.data_len,retrytime);

	shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(recvID);
	if (pUser != NULL && familyProp == PERM_FAMILY)
		pUser->Set_Family_MsgID(familyID, msgID);

	return;

}

void CBusinessFamilySendMsg::AddFileBuf(int nUserNum,uint32 nFamilyId)
{
	uint32 FileID;
	uint16 PocketID = 0;
	uint32 MsgLen;
	uint32 SendTime;
	uint8  faceid;
	m_RecvPacket>>SendTime>>faceid>>MsgLen>>FileID;		//>>PocketID;

	//不需要转发给其他用户，所以也就不需要缓存
	if( nUserNum == 0 )
	{
		return ;
	}

	FID fid;
	fid.fid = FileID;
	fid.sid = m_pRecvHead->sid;


	CTransFileBuf* pFileBuf = CAppManager::GetInstance().GetTransFileManager().AddTransFile(&fid);
	if( pFileBuf )
	{
		pFileBuf->Init(fid,PocketID,nFamilyId,nUserNum);
	}
	else
	{
		SetErrorReturn("add filebuf error");
		return ;
	}

	m_SendPacket<<FileID;
}

CBusinessFamilyNotify::CBusinessFamilyNotify(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_CLUSTER_NOTIFY_REQ,CMD_CLUSTER_NOTIFY_ACK)
{
	//CDataBaseMini *pDataBaseMini = pRouter->GetDataBaseMini();
}

int CBusinessFamilyNotify::DoWork()
{
	LPXT_FAMILY_NOTIFY_REQ lpReq = (XT_FAMILY_NOTIFY_REQ *)m_RecvPacket.GetData();

	CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
	CPermFamily *pFamily = pPermFamilyManager.Find(m_pRecvHead->did);
	if (!pFamily)
	{
		CancelReply();
		return RESULT_SUCCESS;
	}

	if (pFamily && pFamily->m_FamilyMembers.size() != 0)
	{
		//CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock); //modi by lwl ver:0101   SetMemStat内部有锁
		pFamily->SetMemStat(lpReq->nUserID, lpReq->nStat);
	}
	
	if (lpReq->nStat == XTREAM_OFFLINE && pFamily)
	{
		CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
		bool bRelease = true;
		FamilyMember_Map::iterator it = pFamily->m_FamilyMembers.begin();
		for( ; it != pFamily->m_FamilyMembers.end(); ++it )
		{
			if ((*it).second.nStat != XTREAM_OFFLINE)
			{
				bRelease = false;
				break;
			}
		}
		if (bRelease)
		{
			CBusinessFamilyGlobalFunc familyGlobalFunc;
			//CDataBaseMini *pDataBaseMini = m_pRouter->GetDataBaseMini();
			familyGlobalFunc.ReleaseFamilyInfo(pFamily->m_FamilyID,m_pAppNetwork,m_pRouter->GetGlobalFamilyBusi(),m_pRouter->GetDataBaseMini());
		}
	}

	CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
	FamilyMember_Map::iterator iter; 
	for(iter = pFamily->m_FamilyMembers.begin(); iter != pFamily->m_FamilyMembers.end(); ++ iter)
	{
		if(iter->second.nStat != XTREAM_OFFLINE && m_pRecvHead->sid != iter->first)
		{
			SyncFamilyStat(m_RecvPacket,iter->first); 
		}
	}

	m_SendPacket<<(uint8)RESULT_SUCCESS;
	return RESULT_SUCCESS;
}

void CBusinessFamilyNotify::SyncFamilyStat(CRecvPacket& packet,uint32 uid)
{
	SendDataStruct sendData;
	
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
	XT_SERVER_TRANSMIT* lpSrvTransmit = (XT_SERVER_TRANSMIT*)(sendData.data + sizeof(XT_HEAD));
	uint32 len = sizeof(XT_SERVER_TRANSMIT) + packet.GetSize();
	
	CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,len,uid);
	lpSrvTransmit->origin_id = 0; 
	lpSrvTransmit->msgtype = CMD_CLUSTER_NOTIFY_REQ;
	
	sendData.data_len = len + sizeof(XT_HEAD);

	memcpy(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT),packet.GetData(),packet.GetSize());
	
	CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),sendData.data,sendData.data_len,SEND_RETRY_TIME);
}

int CBusinessFamilyNotify::SendReply()
{
	RecvDataStruct* pRecvData = (RecvDataStruct*)m_RowPacketBuf;
	SendDataStruct* pSendData = m_pSendData;

	m_pSendHead->flag = XT_HEAD::FLAG_SRVACK;
	m_pSendHead->from = XT_HEAD::FROM_SERVER;
	m_pSendHead->sid  = m_pRecvHead->did;//CAppManager::GetInstance().GetConfig().GetServerNo();
	m_pSendHead->dtype = XT_HEAD::DEST_TYPE_SERVER;
	m_pSendHead->did	= m_pRecvHead->sid;

	m_pSendData->retry_times = 0;	//应答
	m_pSendData->time_interval = 0;
	m_pSendData->vlink		 = pRecvData->vlink;
#ifdef IM_USE_MULTIDEVICE
		m_pSendHead->deviceId = m_pRecvHead->deviceId;
#endif

	//构建网络层的信息
	pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));


	//构建协议头的信息
	XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
	pSendHead->len  = m_SendPacket.GetSize();

	//发送数据
	assert(m_pAppNetwork);
	m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),pSendData->data,
		pSendData->data_len,
		pSendData->retry_times,
		pSendData->time_interval,
		pRecvData->src_ip,
		pRecvData->port
		);
	return RESULT_SUCCESS;
}

//------------------------------------------------------------------
CBusinessFamilyNotifyAck::CBusinessFamilyNotifyAck(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_CLUSTER_NOTIFY_ACK,CMD_CLUSTER_NOTIFY_ACK)
{

}

void CBusinessFamilyNotifyAck::SetRowPacket(BYTE* pRowPacket,int nPacketLength)
{
	;
}

CBusinessFamiliyOfflineMsg::CBusinessFamiliyOfflineMsg(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_FAMILYOFFLINEMSG_REQ,CMD_FAMILYOFFLINEMSG_ACK)
{
	m_pFamilyMsgBuf = new ST_OFFLINE_MSG[MAX_FAMILEMSG_NUM];
}
CBusinessFamiliyOfflineMsg::~CBusinessFamiliyOfflineMsg(void)
{
	if( m_pFamilyMsgBuf )
	{
		delete[] m_pFamilyMsgBuf;
		m_pFamilyMsgBuf = NULL;
	}

}

int CBusinessFamiliyOfflineMsg::DoWork()
{
	uint32 userID, familyID, familyMsgID;
	m_RecvPacket>>userID
				>>familyID
				>>familyMsgID;

	CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
	CPermFamily *pFamily = pPermFamilyManager.Find(m_pRecvHead->did);
	if (!pFamily)	//是否要重新加载离线消息？？？如IMServer重启，FamilyServer未重启
	{
		m_SendPacket<<(uint8)RESULT_FAILED;
		return RESULT_SUCCESS;
	}

	if (pFamily)
	{
		uint8 nRetCount = MAX_FAMILEMSG_NUM;
		pFamily->GetFamilyMsg(familyMsgID, userID, m_pFamilyMsgBuf, nRetCount,m_pRouter->GetDataBaseMini());

		for(int i = 0; i < nRetCount; ++i)
		{
			pFamily->SendFamilyMsg(m_pRouter->GetGlobalBusi(), &m_pFamilyMsgBuf[i]);
		}

		shared_ptr<CUser> pUser = m_pRouter->GetUserManager()->Find(userID);
		if (pUser != NULL)
			pUser->Set_Family_MsgID(familyID,m_pFamilyMsgBuf[nRetCount - 1].msgid);
	}

	m_SendPacket<<(uint8)RESULT_SUCCESS;
	return RESULT_SUCCESS;
}

int CBusinessFamiliyOfflineMsg::SendReply()
{
	RecvDataStruct* pRecvData = (RecvDataStruct*)m_RowPacketBuf;
	SendDataStruct* pSendData = m_pSendData;

	m_pSendHead->flag = XT_HEAD::FLAG_SRVACK;
	m_pSendHead->from = XT_HEAD::FROM_SERVER;
	m_pSendHead->sid  = m_pRecvHead->did;//CAppManager::GetInstance().GetConfig().GetServerNo();//modify by lwl ver:0102
	m_pSendHead->dtype = XT_HEAD::DEST_TYPE_SERVER;
	m_pSendHead->did	= m_pRecvHead->sid;

	m_pSendData->retry_times = 0;	//应答
	m_pSendData->time_interval = 0;
	m_pSendData->vlink		 = pRecvData->vlink;

	//构建网络层的信息
	pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));


	//构建协议头的信息
	XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
	pSendHead->len  = m_SendPacket.GetSize();
#ifdef IM_USE_MULTIDEVICE
		m_pSendHead->deviceId = m_pRecvHead->deviceId;
#endif


	//发送数据
	assert(m_pAppNetwork);
	m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),pSendData->data,
		pSendData->data_len,
		pSendData->retry_times,
		pSendData->time_interval,
		pRecvData->src_ip,
		pRecvData->port
		);
	return RESULT_SUCCESS;
}

CBusinessFamiliyOfflineMsgAck::CBusinessFamiliyOfflineMsgAck(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_FAMILYOFFLINEMSG_ACK,CMD_FAMILYOFFLINEMSG_ACK)
{

}

void CBusinessFamiliyOfflineMsgAck::SetRowPacket(BYTE* pRowPacket,int nPacketLength)
{
	;
}

CBusinessFamiliyGetProfileByID::CBusinessFamiliyGetProfileByID(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter, CMD_GET_PROFILE_NAME_BYID_REQ, CMD_GET_PROFILE_NAME_BYID_ACK)
{
	m_pDataBaseFamily = pRouter->GetDataBaseFamily();
}

int CBusinessFamiliyGetProfileByID::DoWork()
{
	XT_PROFILENAME_GET_BYID_REQ req;
	m_RecvPacket >> req.nFamilyID
				>> req.nMemID;

	XT_PROFILENAME_GET_BYID_ACK ack;

	ack.ret = m_pDataBaseFamily->Get_Mem_Profile(req.nFamilyID, req.nMemID, ack.ProfileName);
	ack.nFamilyID = req.nFamilyID;
	ack.nMemID = req.nMemID;

	m_SendPacket << ack.ret
				<< ack.nFamilyID
				<< ack.nMemID
				<< ack.ProfileName;

	CLog::Log("CBusinessFamiliyGetProfileByID",CLog::TYPE_NORMAL,
			"get family id:%d, member id:%d, name:%s", req.nFamilyID, req.nMemID, ack.ProfileName);

	return RESULT_SUCCESS;
}



CBusinessFamiliyGetProfile::CBusinessFamiliyGetProfile(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter, CMD_GET_PROFILE_NAME_REQ, CMD_GET_PROFILE_NAME_ACK)
{
	m_pDataBaseFamily = pRouter->GetDataBaseFamily();
}

int CBusinessFamiliyGetProfile::DoWork()
{

	XT_PROFILENAME_GET_REQ* req = (XT_PROFILENAME_GET_REQ*)m_RecvPacket.GetData();
	XT_PROFILENAME_GET_ACK ack ;

	if(req == NULL)
	{
		CLog::Log("CBusinessFamiliyGetProfile",CLog::TYPE_IMPORTANT, "Req is NULL!");
		return  RESULT_FAILED;
	}
	
	ack.nFamilyID = req->nFamilyID;
	ack.ret = m_pDataBaseFamily->Get_All_Profile(ack.nFamilyID, (CMEM_INFO*)ack.stMemInfo, ack.nCount);
	
	m_SendPacket << ack.ret<< ack.nFamilyID << ack.nCount;
	
	for(uint32 i = 0; i< ack.nCount && i < MAX_MEM_CNT; i++)
	{
		m_SendPacket<< ack.stMemInfo[i].nMemID << ack.stMemInfo[i].sProfileName;
	}
	

	CLog::Log("CBusinessFamiliyGetProfile",CLog::TYPE_IMPORTANT,
				"get family id:%d, cnt:%d", ack.nFamilyID, ack.nCount);

	if( ack.nCount > 0)
	{
		CLog::Log("CBusinessFamiliyGetProfile",CLog::TYPE_IMPORTANT,
			"get family id:%d, cnt:%d, member_id:%d, name:%s, member_id:%d, name:%s", ack.nFamilyID,ack.nCount,
				ack.stMemInfo[0].nMemID, ack.stMemInfo[0].sProfileName,
				ack.stMemInfo[ack.nCount-1].nMemID, ack.stMemInfo[ack.nCount-1].sProfileName);
	}
	
	return RESULT_SUCCESS;
}

CBusinessFamiliySetProfile::CBusinessFamiliySetProfile(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter, CMD_SET_PROFILE_NAME_REQ, CMD_SET_PROFILE_NAME_ACK)
{
	m_pDataBaseFamily = pRouter->GetDataBaseFamily();
}

int CBusinessFamiliySetProfile::DoWork()
{
	XT_PROFILENAME_SET_REQ req;
	m_RecvPacket >> req.nFamilyID
				>> req.nMemID
				>> req.ProfileName;

	CLog::Log("CBusinessFamiliySetProfile",CLog::TYPE_IMPORTANT,
			"req nFamilyID:%d, nMemID:%d, ProfileName:%s", req.nFamilyID, req.nMemID, req.ProfileName);

	XT_PROFILENAME_SET_ACK ack;
	ack.ret = m_pDataBaseFamily->Set_Mem_Profile(req.nFamilyID, req.nMemID, req.ProfileName);
	ack.nFamilyID = req.nFamilyID;
	ack.nMemID = req.nMemID;
	strcpy(ack.ProfileName, req.ProfileName);

	m_SendPacket << ack.ret
				<< ack.nFamilyID
				<< ack.nMemID
				<< ack.ProfileName;
	Notify(ack);

	CLog::Log("CBusinessFamiliySetProfile",CLog::TYPE_IMPORTANT,
				"ack ret:%d, nFamilyID:%d, nMemID:%d, ProfileName:%s", ack.ret, ack.nFamilyID, ack.nMemID, ack.ProfileName);

	return RESULT_SUCCESS;
}



void CBusinessFamiliySetProfile::Notify(XT_PROFILENAME_SET_ACK ack )
{
	int len = 0;
	SendDataStruct sendData;
	
	CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
	CPermFamily *pFamily = pPermFamilyManager.Find(ack.nFamilyID);

	if(pFamily)
	{
		CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
		FamilyMember_Map::iterator it = pFamily->m_FamilyMembers.begin();
		for(; it != pFamily->m_FamilyMembers.end(); ++it)
		{
			CLog::Log("CBusinessFamiliySetProfile",CLog::TYPE_IMPORTANT,
				"has Family member :%u", (*it).first);
			if ((*it).first != m_pRecvHead->sid && (*it).second.nStat != XTREAM_OFFLINE)
			{	
				CLog::Log("CBusinessFamiliySetProfile",CLog::TYPE_IMPORTANT,
				"Notify :%u", (*it).first);
				LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
				LPXT_SERVER_TRANSMIT lpSrvTransmit = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
				lpSrvTransmit->origin_id = 0;
				lpSrvTransmit->msgtype = CMD_SET_PROFILE_NAME_REQ;
				len += sizeof(XT_SERVER_TRANSMIT);
				
				memcpy(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT),m_RecvPacket.GetData(),m_RecvPacket.GetSize());

				len += m_RecvPacket.GetSize();	
				
				CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,len,(*it).first);
				sendData.data_len = len + sizeof(XT_HEAD);
				
				m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),sendData.data,sendData.data_len,SEND_RETRY_TIME);
			}
		}		
	}
	else
	{
		int nMemNum = 0;
		if (pFamily->m_FamilyLimit < MAX_VIPFAMILYMEM_NUM)
			nMemNum = pFamily->m_FamilyLimit;
		else
			nMemNum = MAX_VIPFAMILYMEM_NUM;


		ST_FAMILY_MEMBER pFamilyMem[MAX_VIPFAMILYMEM_NUM];
		/*int dbret = */m_pDataBaseFamily->Get_Family_Mem(pFamily->m_FamilyID,pFamilyMem, nMemNum);

		uint32 familyMem[MAX_VIPFAMILYMEM_NUM];
		memset(familyMem, 0, sizeof(uint32) * MAX_VIPFAMILYMEM_NUM);
		for(int j = 0; j < nMemNum; ++j)
		{
			familyMem[j] = pFamilyMem[j].nUserID;
		}
		
		tagGSRV_GET_USER_STATUS_ACK::tagUserStatus UStatus[MAX_VIPFAMILYMEM_NUM];
		memset(UStatus, 0, sizeof(tagGSRV_GET_USER_STATUS_ACK::tagUserStatus) * MAX_VIPFAMILYMEM_NUM);
		uint8 nCount = nMemNum;
		

		CMemcacheQueryUserStatusi(m_pRouter,familyMem,nCount,UStatus);		
			
		for(int i = 0; i < nMemNum; ++i)
		{
			if (UStatus[i].id == pFamilyMem[i].nUserID)
			{
				CLog::Log("CBusinessFamiliySetProfile",CLog::TYPE_IMPORTANT,
				"Notify :%u", UStatus[i].id);
				LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
				LPXT_SERVER_TRANSMIT lpSrvTransmit = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
				lpSrvTransmit->origin_id = 0;
				lpSrvTransmit->msgtype = CMD_SET_PROFILE_NAME_REQ;
				len += sizeof(XT_SERVER_TRANSMIT);
				
				memcpy(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT),m_RecvPacket.GetData(),m_RecvPacket.GetSize());

				len += m_RecvPacket.GetSize();	
				
				CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,len,UStatus[i].id);
				sendData.data_len = len + sizeof(XT_HEAD);
				
				m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),sendData.data,sendData.data_len,SEND_RETRY_TIME);
			}
			
			
		}
		
	}
}


CBusinessFamiliyInviteReq::CBusinessFamiliyInviteReq(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter, CMD_CLUSTER_INVITE_REQ, CMD_CLUSTER_INVITE_ACK)
{
	
}

int CBusinessFamiliyInviteReq::DoWork()
{
	m_SendPacket<<(uint8)0;
	
	NotifyOthers();

	return RESULT_SUCCESS;
}

void CBusinessFamiliyInviteReq::NotifyOthers()
{
	SendDataStruct sendData;
	CSendPacket sendPacket;
	XT_CLUSTER_INVITE_REQ req;
	vector<uint32> ids;
	uint32 id;

	m_SendPacket.SetData(sendData.data  + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT),MAX_SEND_BUFFERS - sizeof(XT_HEAD) - sizeof(XT_SERVER_TRANSMIT));

	m_RecvPacket>>req.count
				>>req.clustername
				>>req.nickname
				>>req.cluster_id
				>>req.cluster_type;

	for(int i = 0; i < req.count; i ++)
	{
		m_RecvPacket>>id;

		LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
		
		XT_SERVER_TRANSMIT* lpSrvTransmit = (XT_SERVER_TRANSMIT*)(sendData.data + sizeof(XT_HEAD));
		lpSrvTransmit->origin_id = 0; 
		lpSrvTransmit->msgtype = CMD_CLUSTER_INVITE_REQ;
		
		m_SendPacket<<m_pRecvHead->sid
					<<req.clustername
					<<req.nickname
					<<req.cluster_id
					<<req.cluster_type;

		uint32 len = sizeof(XT_SERVER_TRANSMIT) + m_SendPacket.GetSize();
		sendData.data_len = len + sizeof(XT_HEAD);

		CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,len,id);
				
		memcpy(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT),m_SendPacket.GetData(),m_SendPacket.GetSize());
		
		CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),sendData.data,sendData.data_len,SEND_RETRY_TIME);
	}
	
}

CBusinessFamiliyAgreeReq::CBusinessFamiliyAgreeReq(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter, CMD_CLUSTER_AGREE_REQ, CMD_CLUSTER_AGREE_ACK)
{
	
}

int CBusinessFamiliyAgreeReq::DoWork()
{
	m_SendPacket<<(uint8)0;
	
	Notify();

	return RESULT_SUCCESS;
}

void CBusinessFamiliyAgreeReq::Notify()
{
	int len = 0;
	XT_CLUSTER_AGREE_REQ req;
	SendDataStruct sendData;

	m_RecvPacket>>req.action
				>>req.invite_id
				>>req.friend_id
				>>req.cluster_id
				>>req.cluster_type;

	CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
	CPermFamily *pFamily = pPermFamilyManager.Find(req.cluster_id);

	if(pFamily)
	{
		CAutoLock mylock(&pFamily->m_FamilyMembers.m_mapLock);
		FamilyMember_Map::iterator it = pFamily->m_FamilyMembers.begin();
		for(; it != pFamily->m_FamilyMembers.end(); ++it)
		{
			if ((*it).second.nProp == FAMILY_CREATER && (*it).second.nStat != XTREAM_OFFLINE)
			{	
				LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
				LPXT_SERVER_TRANSMIT lpSrvTransmit = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
				lpSrvTransmit->origin_id = 0;
				lpSrvTransmit->msgtype = CMD_CLUSTER_AGREE_REQ;
				len += sizeof(XT_SERVER_TRANSMIT);
				
				memcpy(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT),m_RecvPacket.GetData(),m_RecvPacket.GetSize());

				len += m_RecvPacket.GetSize();	
				
				CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,len,(*it).first);
				sendData.data_len = len + sizeof(XT_HEAD);
				
				m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),sendData.data,sendData.data_len,SEND_RETRY_TIME);
			}
		}
	}
}


//返回家族信息
CBusinessGetOtherClusterInfo::CBusinessGetOtherClusterInfo(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMDGSRV_FAMILYINFO_OTHER_QUERY,CMDGSRV_FAMILYINFO_OTHER_ACK)
{

}
int CBusinessGetOtherClusterInfo::DoWork()
{
	CLog::Log("CBusinessGetOtherClusterInfo",CLog::TYPE_ERROR,"CMDGSRV_GET_USER_OTHER_QUERY");
	GSRV_FAMILYINFO_QUERY* pHead = (GSRV_FAMILYINFO_QUERY*)m_RecvPacket.GetData();
	GSRV_FAMILYINFO_QUERY_ACK* pAckHead = (GSRV_FAMILYINFO_QUERY_ACK*)m_RecvPacket.GetData();

	m_SendPacket << (uint8)0;		//成功标志
	m_SendPacket << (uint8)0;		//返回个数,后面根据实际值重新填充

	int nReturnNum = pHead->count;
	uint32 nClusterId = 0;
	int i = 0;

	if( pHead->type == FAMILYINFO_QUERY_TYPE_PERM ) //固定家族
	{
		CPermFamilyManager& pPermFamilyManager = CAppManager::GetInstance().GetPermFamilyManager();
		CPermFamily *pFamily = NULL;			

		for( i = 0 ; i < nReturnNum ; ++i )
		{
			nClusterId = pHead->fid[i];
			m_SendPacket << nClusterId;
			pFamily = pPermFamilyManager.Find(nClusterId);
			if( NULL == pFamily)
			{
				m_SendPacket << (uint32)0;
				CancelReply();
			}
			else
			{
				m_SendPacket << pFamily->m_FamilyServerIp;
			}

			if( !m_SendPacket.HasData(sizeof(pAckHead->FamilyInfo) ) )
			{
				break;
			}
		}
	}
	else if( pHead->type == FAMILYINFO_QUERY_TYPE_TEMP ) //临时家族
	{
		CTempFamilyManager& pTempFamilyManager = CAppManager::GetInstance().GetTempFamilyManager();
		CTempFamily *pFamily = NULL;
		for( i = 0 ; i < nReturnNum ; ++i )
		{
			nClusterId = pHead->fid[i];
			m_SendPacket << nClusterId;
			pFamily = pTempFamilyManager.Find(nClusterId);
			if( NULL == pFamily)
			{
				m_SendPacket << (uint32)0;
				CancelReply();
			}
			else
			{
				m_SendPacket << pFamily->m_FamilyServerIp;
			}

			if( !m_SendPacket.HasData(sizeof(pAckHead->FamilyInfo) ) )
			{
				break;
			}
		}
	}
	else
	{
		CLog::Log("CClusterManager::GetClusterInfo",CLog::TYPE_ERROR,"检测到非法家族类型:%d!",pHead->type);
		return false;
	}
	//最后补上个数
	pAckHead->count = i;
	return true;
}

int CBusinessGetOtherClusterInfo::SendReply()
{
	
	RecvDataStruct* pRecvData = (RecvDataStruct*)m_RowPacketBuf;
	SendDataStruct* pSendData = m_pSendData;

	m_pSendHead->flag = XT_HEAD::FLAG_SRVACK;
	m_pSendHead->from = XT_HEAD::FROM_SERVER;
	m_pSendHead->sid  = m_pRecvHead->did;
	m_pSendHead->dtype = XT_HEAD::DEST_TYPE_SERVER;
	m_pSendHead->did	= m_pRecvHead->sid;

	m_pSendData->retry_times = 0;	//应答
	m_pSendData->time_interval = 0;
	m_pSendData->vlink		 = pRecvData->vlink;

	//构建网络层的信息
	pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));
#ifdef IM_USE_MULTIDEVICE
	m_pSendHead->deviceId = m_pRecvHead->deviceId;
#endif


	//构建协议头的信息
	XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
	pSendHead->len  = m_SendPacket.GetSize();
	CLog::Log("CBusinessGetOtherUser:SendReply",CLog::TYPE_ERROR,"发送请求:ip %u port %d vlink %d",
		pRecvData->src_ip,PORT_MEMCACHED_ONLINE, pRecvData->vlink);

	//发送数据
	assert(m_pAppNetwork);
	m_pAppNetwork->SendDataTo(m_pRouter->GetGlobalBusi(),pSendData->data,
		pSendData->data_len,
		pSendData->retry_times,
		pSendData->time_interval,
		pRecvData->src_ip,
		PORT_MEMCACHED_ONLINE,
		m_pSendData->vlink
		);
	return RESULT_SUCCESS;
}









