#include "stdafx.h"
#include "GlobalBusi.h"
#include "GlobalBusiControl.h"
#include "xtpacket.h"
#include "../src/MiniIM/User.h"
#include "Function.h"

#define WAITTIMEOUT 1000 * 40 // 40 s  

#ifdef IM_USE_MEMCACHED

using namespace std;
using namespace global_memcache;
bool CMemCBusi::memcache_has_init = false;
CSyncCritical CMemCBusi::m_memcached_lock;
uint32 CMemCBusi::m_magic_num = 0;

#endif



CGlobalBusi::CGlobalBusi(GlobalBusiControl * pBusiControl):
m_hEvent(FALSE,FALSE)
{
	m_pRecvData = new RecvDataStruct();
	assert(pBusiControl != NULL);
	//m_hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_pBusiControl = pBusiControl;
	m_vLink = m_pBusiControl->AllocVLink();
	m_pBusiControl->RegisterLink(m_vLink,this);

	m_sendData.vlink = m_vLink;
}

CGlobalBusi::~CGlobalBusi(void)
{
	SAFE_DELETE(m_pRecvData);
	//::CloseHandle(m_hEvent);
}

#ifdef IM_USE_MEMCACHED
int CGlobalBusi::SearchOnlineUser(tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo * pUserInfo,uint8 & count,uint32& nOnlineNum,uint16 startpos)
{
	//CLog::Log("CGlobalBusi::SearchOnlineUser-MEMCACHED",CLog::TYPE_DEBUG,"INTO SearchOnlineUser");
	//CLog::Log("CGlobalBusi::SearchOnlineUser-MEMCACHED",CLog::TYPE_DEBUG,"count %x %u",count,nOnlineNum);
	assert(pUserInfo);
	if(count < 1)
		return 0;
	m_cMemCBusi.SearchOnlineUser(pUserInfo,count,nOnlineNum,startpos);	
	CLog::Log("CGlobalBusi::SearchOnlineUser-MEMCACHED",CLog::TYPE_DEBUG,"count:%x, nOnlineNum:%u ",count,nOnlineNum);
	return 0;
}

int CGlobalBusi::QueryUserControlInfo(const uint32 * auid,tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo * pUserControlInfo,uint8 & count)
{
	assert(auid);
	if(count < 1)
		return 0;


	if(pUserControlInfo)
	{
		for(int i = 0; i < count; ++i)
		{
			m_cMemCBusi.QueryUserControlInfo(auid[i],pUserControlInfo[i]);
			CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"QueryUserControlInfo uid:%u, srvip:%u ",
						pUserControlInfo[i].uid, pUserControlInfo[i].srvip);
		}
	}
	return 0;
}


int CGlobalBusi::QueryUserStatus(const uint32 * aID,tagGSRV_GET_USER_STATUS_ACK::tagUserStatus * pUStatus,uint8 & count)
{
	assert(aID);
	if(count < 1)
		return 0;
	
	if(pUStatus)
	{
		for(int i = 0; i < count; ++i)
		{
			if(m_cMemCBusi.QueryUserStatus(aID[i], pUStatus[i]))
			{
			
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"QueryUserStatus OK");
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"id:%u, status:%u",
						pUStatus[i].id, pUStatus[i].status);
			
			}
			else
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"QueryUserStatus empty");

			}

		}
	}
	return 0;
}

int CGlobalBusi::ModiUserStatus(const tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser * pUserStatus,uint8 & count,tagGSRV_MODIFY_ONLINE_STATUS_ACK::tagUser * pUStatusNow/* = NULL*/)
{
	assert(pUserStatus);
	if(count < 1)
		return 0;

	int i = 0;
	for(i = 0; i < count; ++i)
	{

		m_cMemCBusi.ModiUserStatus(pUserStatus[i]);

	}
	CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"ModiUserStatus OK");

	return 0;
}

int CGlobalBusi::QueryUserFlow(const uint32 * aID, tagGSRV_GET_USER_FLOW_ACK::tagUserFlow * pUserFlow,uint8 & count)
{
	assert(aID);
	if(count < 1)
		return 0;

	if(pUserFlow)
	{
		for(int i = 0; i < count; ++i)
		{

			m_cMemCBusi.QueryUserFlow(aID[i],pUserFlow[i]);
			CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"QueryUserFlow  %u %u %u",
					pUserFlow[i].id, pUserFlow[i].status, pUserFlow[i].flow);
		}

	}
	
	return 0;
}

int CGlobalBusi::ModiUserFlow(const tagGSRV_MODIFY_USER_FLOW_QUERY::tagFlow * pUserFlow, uint8 & count, tagGSRV_MODIFY_USER_FLOW_ACK::tagUserFlow * pUserFlowNow/* = NULL*/)
{
	assert(pUserFlow);
	if(count < 1)
		return 0;

	int i = 0;
	for(i = 0; i < count; ++i)
	{
		m_cMemCBusi.ModiUserFlow(pUserFlow[i]);
		pUserFlowNow[i].id = pUserFlow[i].id;
		pUserFlowNow[i].flow= pUserFlow[i].flow;
		pUserFlowNow[i].status = 0;
		CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"ModiUserFlow  %u %u %u",
					pUserFlowNow[i].id, pUserFlowNow[i].status, pUserFlowNow[i].flow);
	}

	return 0;
}

int CGlobalBusi::QueryUserInfo(const uint32 * auid,uint8 & count,tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo * pUserInfo)
{
	assert(auid);
	if(count < 1)
		return 0;

	if(pUserInfo)
	{
		for(int i = 0; i < count; ++i)
		{
			
			if( m_cMemCBusi.QueryUserInfo(auid[i], pUserInfo[i]) )
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"QueryUserInfo:");
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG," %u %u %u %s %s %u %u %u %s %s",
					pUserInfo[i].id, pUserInfo[i].status, pUserInfo[i].version,
					pUserInfo[i].username, pUserInfo[i].nickname, 
					pUserInfo[i].clientport, pUserInfo[i].sexy, pUserInfo[i].serverip,
					Base64Encode(pUserInfo[i].sessionkey).c_str(), Base64Encode(pUserInfo[i].p2psessionkey).c_str()
					);
					
			}
			else
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_ERROR,"QueryUserInfo empty");
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_ERROR," %u %u %u %s %s %u %u %u %s %s",
					pUserInfo[i].id, pUserInfo[i].status, pUserInfo[i].version,
					pUserInfo[i].username, pUserInfo[i].nickname, 
					pUserInfo[i].clientport, pUserInfo[i].sexy, pUserInfo[i].serverip,
					Base64Encode(pUserInfo[i].sessionkey).c_str(), Base64Encode(pUserInfo[i].p2psessionkey).c_str()
					);
			}

			
		}
	}
	return 0;
}

int CGlobalBusi::AddModiUserInfo(CUser* pUser,uint8 action,uint8 flag)
{
	assert(pUser);

	tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo userInfo;
	userInfo.id = pUser->m_nID;
	userInfo.rule = pUser->m_AllowRule;
	userInfo.status = pUser->m_nStatus;
	strcpy(userInfo.username, pUser->m_szUserName);
	strcpy(userInfo.nickname, pUser->m_szNickName);
	
	userInfo.clientip = pUser->m_nIP;
	userInfo.clientport = pUser->m_nPort;
	userInfo.sexy  = pUser->m_nSexy;
	userInfo.serverip = (uint32)inet_addr(m_pBusiControl->GetIPAddr());
	userInfo.lanip = pUser->m_nLanIP;
	memcpy(userInfo.sessionkey, pUser->m_SessionKey, sizeof(userInfo.sessionkey));
	memcpy(userInfo.p2psessionkey, pUser->m_P2PSessionKey, sizeof(userInfo.sessionkey));
	
	m_cMemCBusi.AddModiUserInfo(userInfo);

	
	return 0;
}

int CGlobalBusi::AddModiUserInfo(const tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo * pUserInfo,uint8 & count,uint32 * pUserID,uint8 action /* = tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD*/,uint8 flag)
{
	assert(pUserInfo);
	if(count < 1)
		return 0;
	CLog::Log("CGlobalBusi::AddModiUserInfo-MEMCACHED",CLog::TYPE_DEBUG," ADDED count %u",count);
	for(int i = 0; i < count; ++i)
	{
		CLog::Log("CGlobalBusi::AddModiUserInfo-MEMCACHED",CLog::TYPE_DEBUG,"lpReq->userid[%u] %u",i,pUserInfo[i].id);
		if(m_cMemCBusi.AddModiUserInfo(pUserInfo[i]) && (pUserID))
			pUserID[i] = pUserInfo[i].id;	
	}
	return RESULT_SUCCESS;
}


int CGlobalBusi::DelUserInfo(const uint32 * auid,uint8 & count,uint32 * aOKUserID /* = NULL*/)
{
	assert(auid);
	if(count < 1)
		return 0;

	
	CLog::Log("CGlobalBusi::DelUserInfo-MEMCACHED",CLog::TYPE_DEBUG," DELETED count %u",count);
	for(int i = 0; i < count; ++i)
	{		
		CLog::Log("CGlobalBusi::DelUserInfo-MEMCACHED",CLog::TYPE_DEBUG,"lpReq->userid[%u] %u",i,auid[i]);
		if(m_cMemCBusi.DelUserInfo(auid[i]) && (aOKUserID))
			aOKUserID[i] = auid[i];
	}	
	
	return 0;
}


/************ 家族业务*******************/
//添加全局家族资料
int CGlobalBusi::AddFamilyInfo(const tagGSRV_FAMILYINFO_ADD_QUERY::tagFamilyInfo * pFamilyInfo,uint8 & count,uint32 * pFamilyID/* = NULL*/,uint8 type /*= FAMILYINFO_QUERY_TYPE_PERM*/)
{
	assert(pFamilyInfo);
	if(count < 1)
		return 0;
	
	CLog::Log("CGlobalBusi::AddFamilyInfo-MEMCACHED",CLog::TYPE_DEBUG," ADD count %u",count);
	for(int i = 0; i < count; ++i)
	{		
		CLog::Log("CGlobalBusi::AddFamilyInfo-MEMCACHED",CLog::TYPE_DEBUG,"pFamilyInfo[%u].FamilyID %u",i,pFamilyInfo[i].FamilyID);
		if(m_cMemCBusi.AddModiFamilyInfo(pFamilyInfo[i],type) && (pFamilyID))
			pFamilyID[i]= pFamilyInfo[i].FamilyID;
	}
	return 0;
}

//删除全局家族资料
int CGlobalBusi::DelFamilyInfo(const uint32 * pDelFamilyID,uint8 & count,uint32 * pOKFamilyID/* = NULL*/,uint8 type/* = FAMILYINFO_QUERY_TYPE_PERM*/)
{
	assert(pDelFamilyID);
	if(count < 1)
		return 0;
	CLog::Log("CGlobalBusi::DelFamilyInfo-MEMCACHED",CLog::TYPE_DEBUG," DELETED count %u",count);
	for(int i = 0; i < count; ++i)
	{
	
		CLog::Log("CGlobalBusi::DelFamilyInfo-MEMCACHED",CLog::TYPE_DEBUG,"pFamilyInfo[%u].FamilyID %u",i,pDelFamilyID[i]);
		if(m_cMemCBusi.DelFamilyInfo(pDelFamilyID[i],type) && (pOKFamilyID))
			pOKFamilyID[i] = pDelFamilyID[i];
	}
	return 0;
}

int CGlobalBusi::FindFamilyInfo(const uint32 * pFamilyID,uint8 & count,tagGSRV_FAMILYINFO_QUERY_ACK::tagFamilyInfo * pFamilyInfo,uint8 type/* = FAMILYINFO_QUERY_TYPE_PERM*/)
{
	assert(pFamilyID);
	if(count < 1)
		return 0;
	
	CLog::Log("CGlobalBusi::FindFamilyInfo-MEMCACHED",CLog::TYPE_DEBUG," FIND count %u",count);
	if(pFamilyInfo)
	{
		for(int i = 0; i < count; ++i)
		{
			if( m_cMemCBusi.QueryFamilyInfo(pFamilyID[i],pFamilyInfo[i],type) )
			{						
				
					CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG," %u %u",
					pFamilyInfo[i].fid, pFamilyInfo[i].srvip);	
				
			}
			else
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_ERROR,"QueryUserInfo empty");
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_ERROR," %u %u",
						pFamilyInfo[i].fid, pFamilyInfo[i].srvip);
			}			
		}
	}
	return 0;
}


int CGlobalBusi::GetBestAccount(uint32 merchant_id, uint32 group_id, std::vector<uint32_t>& ids, uint32 flow_type, uint32& id)
{	
	return m_cMemCBusi.GetBestAccount(merchant_id,group_id,ids,flow_type,id);
}

#else
int CGlobalBusi::SearchOnlineUser(tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo * pUserInfo,uint8 & count,uint32& nOnlineNum,uint16 startpos)
{
	//CLog::Log("CGlobalBusi::SearchOnlineUser",CLog::TYPE_DEBUG,"INTO SearchOnlineUser");
	//CLog::Log("CGlobalBusi::SearchOnlineUser",CLog::TYPE_DEBUG,"count %x %u",count,nOnlineNum);
	assert(pUserInfo);
	if(count < 1)
		return 0;
	CSendPacket sendPacket(m_sendData.data);
	XT_HEAD head;
	m_pBusiControl->InitHead(&head,CMDGSRV_SEARCH_USER_QUERY,sizeof(uint32),XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);
	sendPacket.SetXtHead(head);
	sendPacket<<count;
	sendPacket<<startpos;

	//Send Data
	m_sendData.data_len = sendPacket.GetSize();
	//::ResetEvent(m_hEvent);
	m_hEvent.ResetEvent();
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::SearchOnlineUser",CLog::TYPE_ERROR,"wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_SEARCH_USER_ACK)
		return 1;

	//LPGSRV_SEARCH_ONLINE_USER_ACK_HEAD
	uint8 uResult = 0;
	CRecvPacket recvPacket((char *)(m_pRecvData->data + sizeof(XT_HEAD)),m_pRecvData->data_len - sizeof(XT_HEAD));
	recvPacket>>uResult;
  	if(uResult != 0)
	{
		CLog::Log("CGlobalBusi::SearchOnlineUser",CLog::TYPE_ERROR," %s ",(char *)recvPacket.GetCursor());
		return 1;
	}

	recvPacket>>count>>nOnlineNum;
	if(pUserInfo)
	{

		for(int i = 0; i < count; ++i)
		{

			recvPacket	>>pUserInfo[i].clientid
						>>pUserInfo[i].sexy
						>>pUserInfo[i].status
						>>pUserInfo[i].rule
						>>pUserInfo[i].username
						>>pUserInfo[i].nickname 
							;
		}
	}
#ifdef IM_USE_MEMCACHED
			m_cMemCBusi.SearchOnlineUser(pUserInfo,count,nOnlineNum);
#endif

	
	CLog::Log("CGlobalBusi::SearchOnlineUser",CLog::TYPE_DEBUG,"count:%x, nOnlineNum:%u",count,nOnlineNum);

	return 0;
}

int CGlobalBusi::QueryUserControlInfo(const uint32 * auid,tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo * pUserControlInfo,uint8 & count)
{
	assert(auid);
	if(count < 1)
		return 0;
	LPXT_HEAD lpSendHead  = (LPXT_HEAD)(m_sendData.data);
	LPGSRV_USERCONTROLINFO_QUERY lpReq = (LPGSRV_USERCONTROLINFO_QUERY)(m_sendData.data + sizeof(XT_HEAD));
	lpReq->count = count;
	int i = 0;
	for(i = 0; i < count; ++i)
	{
		lpReq->uid[i] = auid[i];
		assert( (lpReq->uid[i] > 0) && (lpReq->uid[i] < MAX_139_ACCOUNT_NUM) );
	}

	int len = sizeof(GSRV_USERCONTROLINFO_QUERY) + sizeof(lpReq->uid) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_USERCONTROLINFO_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	//::ResetEvent(m_hEvent);
	m_hEvent.ResetEvent();
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::QueryUserControlInfo",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_USERCONTROLINFO_QUERY_ACK)
		return 1;

    LPGSRV_USERCONTROLINFO_QUERY_ACK lpAck = (LPGSRV_USERCONTROLINFO_QUERY_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->ret != 0)
	{
		CLog::Log("CGlobalBusi::QueryUserControlInfo",CLog::TYPE_ERROR," %s ",(char *)(lpAck + sizeof(lpAck->ret)));
		return 1;
	}

	count = lpAck->count;
	if(pUserControlInfo)
	{
		for(i = 0; i < count; ++i)
		{
			pUserControlInfo[i].uid = lpAck->userControlInfo[i].uid;
			pUserControlInfo[i].srvip = lpAck->userControlInfo[i].srvip;
			memcpy(pUserControlInfo[i].sessionKey,lpAck->userControlInfo[i].sessionKey,sizeof(pUserControlInfo[i].sessionKey));
			memcpy(pUserControlInfo[i].p2pSessionKey,lpAck->userControlInfo[i].p2pSessionKey,sizeof(pUserControlInfo[i].p2pSessionKey));
#ifdef IM_USE_MEMCACHED
			tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo userControlInfo;
			m_cMemCBusi.QueryUserControlInfo(auid[i],userControlInfo);

			if(memcmp(&pUserControlInfo[i], &userControlInfo, sizeof(userControlInfo)) != 0)
			{
				CLog::Log("CGlobalBusi::QueryUserControlInfo",CLog::TYPE_ERROR,"QueryUserControlInfo error");
				
				CLog::Log("CGlobalBusi::QueryUserControlInfo",CLog::TYPE_ERROR,"pUserControlInfo.uid:%u, srvip:%u, sessionKey:%s, p2pSessionKey:%s",
					pUserControlInfo[i].uid, pUserControlInfo[i].srvip, 
					Base64Encode(pUserControlInfo[i].sessionKey).c_str(), Base64Encode(pUserControlInfo[i].p2pSessionKey).c_str()
					);
				CLog::Log("CGlobalBusi::QueryUserControlInfo",CLog::TYPE_ERROR,"userControlInfo.uid:%u, srvip:%u, sessionKey:%s, p2pSessionKey:%s",
					userControlInfo.uid, userControlInfo.srvip, 
					Base64Encode(userControlInfo.sessionKey).c_str(), Base64Encode(userControlInfo.p2pSessionKey).c_str()
					);
			}
#endif
			
		}
	}
	return 0;
}

int CGlobalBusi::QueryUserStatus(const uint32 * aID,tagGSRV_GET_USER_STATUS_ACK::tagUserStatus * pUStatus,uint8 & count)
{
	assert(aID);
	if(count < 1)
		return 0;
	LPXT_HEAD lpSendHead  = (LPXT_HEAD)(m_sendData.data);
	LPGSRV_GET_USER_STATUS_QUERY lpReq = (LPGSRV_GET_USER_STATUS_QUERY)(m_sendData.data + sizeof(XT_HEAD));
	lpReq->count = count;
	int i = 0;
	for(i = 0; i < count; ++i)
	{
		lpReq->userid[i] = aID[i];
	}

	int len = sizeof(GSRV_GET_USER_STATUS_QUERY) + sizeof(lpReq->userid) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_GET_USERSTATUS_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	//::ResetEvent(m_hEvent);
	m_hEvent.ResetEvent();
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::QueryUserStatus",CLog::TYPE_ERROR,"wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_GET_USERSTATUS_ACK)
		return 1;

    LPGSRV_GET_USER_STATUS_ACK lpAck = (LPGSRV_GET_USER_STATUS_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->result != 0)
	{
		CLog::Log("CGlobalBusi::QueryUserStatus",CLog::TYPE_ERROR," %s ",(char *)(lpAck + sizeof(lpAck->result)));
		return 1;
	}

	count = lpAck->count;
	if(pUStatus)
	{
		for(i = 0; i < count; ++i)
		{
			pUStatus[i].id = lpAck->UserStatus[i].id;
			pUStatus[i].status = lpAck->UserStatus[i].status;
#ifdef IM_USE_MEMCACHED
			tagGSRV_GET_USER_STATUS_ACK::tagUserStatus userStatus;
			if(m_cMemCBusi.QueryUserStatus(aID[i], userStatus))
			{
				if(memcmp(&pUStatus[i], &userStatus, sizeof(userStatus)) != 0)
				{
					CLog::Log("CGlobalBusi::QueryUserStatus",CLog::TYPE_ERROR," QueryUserStatus error");
					
					CLog::Log("CGlobalBusi::QueryUserStatus",CLog::TYPE_ERROR," pUStatus.id:%u, pUStatus.status:%u",
						pUStatus[i].id, pUStatus[i].status);
					CLog::Log("CGlobalBusi::QueryUserStatus",CLog::TYPE_ERROR," userStatus.id:%u, userStatus.status:%u",
						userStatus.id, userStatus.status);

				}
				else
				{
					CLog::Log("CGlobalBusi::QueryUserStatus",CLog::TYPE_DEBUG,"QueryUserStatus OK");
				}
			}
			else
			{
				CLog::Log("CGlobalBusi::QueryUserStatus",CLog::TYPE_DEBUG," QueryUserStatus empty");
				CLog::Log("CGlobalBusi",CLog::TYPE_DEBUG," pUStatus.id:%u, pUStatus.status:%u",
						pUStatus[i].id, pUStatus[i].status);
			}

#endif		


		}
	}
	return 0;
}

int CGlobalBusi::ModiUserStatus(const tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser * pUserStatus,uint8 & count,tagGSRV_MODIFY_ONLINE_STATUS_ACK::tagUser * pUStatusNow/* = NULL*/)
{
	assert(pUserStatus);
	if(count < 1)
		return 0;
	LPXT_HEAD lpSendHead  = (LPXT_HEAD)(m_sendData.data);
	LPGSRV_MODIFY_ONLINE_STATUS_QUERY lpReq = (LPGSRV_MODIFY_ONLINE_STATUS_QUERY)(m_sendData.data + sizeof(XT_HEAD));
	lpReq->count = count;
	int i = 0;
	for(i = 0; i < count; ++i)
	{
		lpReq->user[i].id = pUserStatus[i].id;
		lpReq->user[i].status = pUserStatus[i].status;
#ifdef IM_USE_MEMCACHED
		m_cMemCBusi.ModiUserStatus(pUserStatus[i]);
#endif
	}

	int len = sizeof(GSRV_MODIFY_ONLINE_STATUS_QUERY) + sizeof(lpReq->user) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_MODIFY_USERSTATUS_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	//::ResetEvent(m_hEvent);
	m_hEvent.ResetEvent();
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::ModiUserStatus",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_MODIFY_USERSTATUS_ACK)
		return 1;

    LPGSRV_MODIFY_ONLINE_STATUS_ACK lpAck = (LPGSRV_MODIFY_ONLINE_STATUS_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->result != 0)
	{
		CLog::Log("CGlobalBusi::ModiUserStatus",CLog::TYPE_ERROR," %s ",(char *)(lpAck + sizeof(lpAck->result)));
		return 1;
	}

	count = lpAck->count;
	if(pUStatusNow)
	{
		for(i = 0; i < count; ++i)
		{
			pUStatusNow[i].id = lpAck->user[i].id;
			pUStatusNow[i].status = lpAck->user[i].status;
		}
	}
	return 0;
}

int CGlobalBusi::QueryUserFlow(const uint32 * aID, tagGSRV_GET_USER_FLOW_ACK::tagUserFlow * pUserFlow,uint8 & count)
{
	assert(aID);
	if(count < 1)
		return 0;
	LPXT_HEAD lpSendHead  = (LPXT_HEAD)(m_sendData.data);
	LPGSRV_GET_USER_FLOW_QUERY lpReq = (LPGSRV_GET_USER_FLOW_QUERY)(m_sendData.data + sizeof(XT_HEAD));
	lpReq->count = count;
	int i = 0;
	for(i = 0; i < count; ++i)
	{
		lpReq->userid[i] = aID[i];
	}

	int len = sizeof(GSRV_GET_USER_FLOW_ACK) + sizeof(lpReq->userid) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_GET_USERFLOW_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	//::ResetEvent(m_hEvent);
	m_hEvent.ResetEvent();
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::QueryUserStatus",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_GET_USERFLOW_ACK)
		return 1;

    LPGSRV_GET_USER_FLOW_ACK lpAck = (LPGSRV_GET_USER_FLOW_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->result != 0)
	{
		CLog::Log("CGlobalBusi::QueryUserStatus",CLog::TYPE_ERROR," %s ",(char *)(lpAck + sizeof(lpAck->result)));
		return 1;
	}

	count = lpAck->count;
	if(pUserFlow)
	{
		for(i = 0; i < count; ++i)
		{
			pUserFlow[i].id = lpAck->UserFlow[i].id;
			pUserFlow[i].status = lpAck->UserFlow[i].status;
			pUserFlow[i].flow = lpAck->UserFlow[i].flow;
#ifdef IM_USE_MEMCACHED
			tagGSRV_GET_USER_FLOW_ACK::tagUserFlow userFlow;
			m_cMemCBusi.QueryUserFlow(aID[i],userFlow);
			if(memcmp(&pUserFlow[i], &userFlow, sizeof(userFlow)) != 0)
			{
				CLog::Log("CGlobalBusi::QueryUserFlow",CLog::TYPE_ERROR," QueryUserFlow error");
				
				CLog::Log("CGlobalBusi::QueryUserFlow",CLog::TYPE_ERROR," pUserFlow.id:%u, status:%u, flow:%u",
					pUserFlow[i].id, pUserFlow[i].status, pUserFlow[i].flow);
				CLog::Log("CGlobalBusi::QueryUserFlow",CLog::TYPE_ERROR," userFlow.id:%u, status:%u, flow:%u",
					userFlow.id, userFlow.status, userFlow.flow);

			}
#endif
		}
	}
	return 0;
}



int CGlobalBusi::ModiUserFlow(const tagGSRV_MODIFY_USER_FLOW_QUERY::tagFlow * pUserFlow, uint8 & count, tagGSRV_MODIFY_USER_FLOW_ACK::tagUserFlow * pUserFlowNow/* = NULL*/)
{
	assert(pUserFlow);
	if(count < 1)
		return 0;
	LPXT_HEAD lpSendHead  = (LPXT_HEAD)(m_sendData.data);
	LPGSRV_MODIFY_USER_FLOW_QUERY lpReq = (LPGSRV_MODIFY_USER_FLOW_QUERY)(m_sendData.data + sizeof(XT_HEAD));
	lpReq->count = count;
	int i = 0;
	for(i = 0; i < count; ++i)
	{
		lpReq->user[i].id = pUserFlow[i].id;
		lpReq->user[i].flow = pUserFlow[i].flow;
#ifdef IM_USE_MEMCACHED
		m_cMemCBusi.ModiUserFlow(pUserFlow[i]);
#endif
	}

	int len = sizeof(GSRV_MODIFY_USER_FLOW_QUERY) + sizeof(lpReq->user) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_MODIFY_USERFLOW_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	//::ResetEvent(m_hEvent);
	m_hEvent.ResetEvent();
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::ModiUserStatus",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_MODIFY_USERFLOW_ACK)
		return 1;


    LPGSRV_MODIFY_USER_FLOW_ACK lpAck = (LPGSRV_MODIFY_USER_FLOW_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->result != 0)
	{
		CLog::Log("CGlobalBusi::ModiUserFlow",CLog::TYPE_ERROR," %s ",(char *)(lpAck + sizeof(lpAck->result)));
		return 1;
	}

	count = lpAck->count;
	if(pUserFlowNow)
	{
		for(i = 0; i < count; ++i)
		{
			pUserFlowNow[i].id = lpAck->user[i].id;
			pUserFlowNow[i].status = lpAck->user[i].status;
			pUserFlowNow[i].flow = lpAck->user[i].flow;
		}
	}

	return 0;
}


int CGlobalBusi::QueryUserInfo(const uint32 * auid,uint8 & count,tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo * pUserInfo)
{
	assert(auid);
	if(count < 1)
		return 0;
	LPXT_HEAD lpSendHead  = (LPXT_HEAD)(m_sendData.data);
	LPGSRV_GET_ONLINE_FULLINFO_QUERY lpReq = (LPGSRV_GET_ONLINE_FULLINFO_QUERY)(m_sendData.data + sizeof(XT_HEAD));
	lpReq->count = count;
	int i = 0;
	for(i = 0; i < count; ++i)
	{
		lpReq->userid[i] = auid[i];
	}

	int len = sizeof(GSRV_GET_ONLINE_FULLINFO_QUERY) + sizeof(lpReq->userid) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_GET_USER_FULLINFO_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	::ResetEvent(m_hEvent);
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::QueryUserInfo",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_GET_USER_FULLINFO_ACK)
		return 1;

	//LPGSRV_GET_ONLINE_FULLINFO_ACK
	uint8 uResult = 0;
	CRecvPacket recvPacket((char *)(m_pRecvData->data + sizeof(XT_HEAD)),m_pRecvData->data_len - sizeof(XT_HEAD));
	recvPacket>>uResult;
	if(uResult != 0)
	{
		CLog::Log("CGlobalBusi::QueryUserInfo",CLog::TYPE_ERROR," %s ",(char *)recvPacket.GetCursor());
		return 1;
	}

	recvPacket>>count;
	if(pUserInfo)
	{
		for(i = 0; i < count; ++i)
		{
			recvPacket>>pUserInfo[i].id>>pUserInfo[i].status>>pUserInfo[i].version \
				>>pUserInfo[i].username>>pUserInfo[i].nickname \
				>>pUserInfo[i].clientip>>pUserInfo[i].clientport>>pUserInfo[i].sexy>>pUserInfo[i].serverip;
			recvPacket.ReadPacket(pUserInfo[i].sessionkey,sizeof(pUserInfo[i].sessionkey));
			recvPacket.ReadPacket(pUserInfo[i].p2psessionkey,sizeof(pUserInfo[i].p2psessionkey));
#ifdef IM_USE_MEMCACHED

			tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo;
			if( m_cMemCBusi.QueryUserInfo(auid[i], userInfo) )
			{
				if(memcmp(&pUserInfo[i], &userInfo, sizeof(userInfo)) != 0)
				{
					CLog::Log("CGlobalBusi::QueryUserInfo",CLog::TYPE_ERROR,"QueryUserInfo error");
					
					CLog::Log("CGlobalBusi::QueryUserInfo",CLog::TYPE_ERROR,"pUserInfo.id:%u, status:%u, version:%u, username:%s, nickname:%s, clientport:%u, sexy:%u, serverip:%u, sessionkey:%s, p2psessionkey:%s",
						pUserInfo[i].id, pUserInfo[i].status, pUserInfo[i].version,
						pUserInfo[i].username, pUserInfo[i].nickname, 
						pUserInfo[i].clientport, pUserInfo[i].sexy, pUserInfo[i].serverip,
						Base64Encode(pUserInfo[i].sessionkey).c_str(), Base64Encode(pUserInfo[i].p2psessionkey).c_str()
						);
					CLog::Log("CGlobalBusi::QueryUserInfo",CLog::TYPE_ERROR,"userInfo.id:%u, status:%u, version:%u, username:%s, nickname:%s, clientport:%u, sexy:%u, serverip:%u, sessionkey:%s, p2psessionkey:%s",
							userInfo.id, userInfo.status, userInfo.version,
							userInfo.username, userInfo.nickname, 
							userInfo.clientport, userInfo.sexy, userInfo.serverip,
							Base64Encode(userInfo.sessionkey).c_str(), Base64Encode(userInfo.p2psessionkey).c_str()
							);

				
				}
			}
			else
			{
				CLog::Log("CGlobalBusi::QueryUserInfo",CLog::TYPE_ERROR,"QueryUserInfo empty");
				CLog::Log("CGlobalBusi::QueryUserInfo",CLog::TYPE_ERROR,"pUserInfoid:%u, status:%u, version:%u, username:%s, nickname:%s, clientport:%u, sexy:%u, serverip:%u, sessionkey:%s, p2psessionkey:%s",
					pUserInfo[i].id, pUserInfo[i].status, pUserInfo[i].version,
					pUserInfo[i].username, pUserInfo[i].nickname, 
					pUserInfo[i].clientport, pUserInfo[i].sexy, pUserInfo[i].serverip,
					Base64Encode(pUserInfo[i].sessionkey).c_str(), Base64Encode(pUserInfo[i].p2psessionkey).c_str()
					);
			}
#endif
			
		}
	}
	return 0;
}


int CGlobalBusi::AddModiUserInfo(CUser* pUser,uint8 action,uint8 flag)
{
	assert(pUser);
#ifdef IM_USE_MEMCACHED
	tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo userInfo;
	userInfo.id = pUser->m_nID;
	userInfo.rule = pUser->m_AllowRule;
	userInfo.status = pUser->m_nStatus;
	strcpy(userInfo.username, pUser->m_szUserName);
	strcpy(userInfo.nickname, pUser->m_szNickName);
	
	userInfo.clientip = pUser->m_nIP;
	userInfo.clientport = pUser->m_nPort;
	userInfo.sexy  = pUser->m_nSexy;
	userInfo.serverip = (uint32)inet_addr(m_pBusiControl->GetIPAddr());
	userInfo.lanip = pUser->m_nLanIP;
	memcpy(userInfo.sessionkey, pUser->m_SessionKey, sizeof(userInfo.sessionkey));
	memcpy(userInfo.p2psessionkey, pUser->m_P2PSessionKey, sizeof(userInfo.sessionkey));
	
	m_cMemCBusi.AddModiUserInfo(userInfo);
#endif
	LPXT_HEAD lpSendHead  = (LPXT_HEAD)(m_sendData.data);
	CSendPacket sendPacket(m_sendData.data + sizeof(XT_HEAD));
	sendPacket<<action<<(uint8)1;

	sendPacket<<pUser->m_nID<<pUser->m_nStatus<<pUser->m_AllowRule;
	sendPacket<<pUser->m_szUserName<<pUser->m_szNickName;
	sendPacket<<pUser->m_nIP<<pUser->m_nPort;
	sendPacket<<pUser->m_nSexy<<(uint32)inet_addr(m_pBusiControl->GetIPAddr());
	sendPacket<<pUser->m_nLanIP;//<<pUser->m_nLanPort;
	sendPacket<<flag;
	
	sendPacket.WritePacket(pUser->m_SessionKey,sizeof(pUser->m_SessionKey));
	sendPacket.WritePacket(pUser->m_P2PSessionKey ,sizeof(pUser->m_P2PSessionKey));


	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_ADDMODIFY_USER_FULLINFO_QUERY,sendPacket.GetSize(),XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	::ResetEvent(m_hEvent);
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::AddModiUserInfo",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}

	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_ADDMODIFY_USER_FULLINFO_ACK)
		return 1;

	LPGSRV_ADDMODIFY_FULLUSERINFO_ACK lpAck = (LPGSRV_ADDMODIFY_FULLUSERINFO_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->result != 0)
	{
		CLog::Log("CGlobalBusi::AddModiUserInfo",CLog::TYPE_ERROR," %s ",(char *)(lpAck + sizeof(lpAck->result)));
		return 1;
	}

	return 0;
}

int CGlobalBusi::AddModiUserInfo(const tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo * pUserInfo,uint8 & count,uint32 * pUserID,uint8 action /* = tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD*/,uint8 flag)
{
	assert(pUserInfo);
	if(count < 1)
		return 0;
	LPXT_HEAD lpSendHead  = (LPXT_HEAD)(m_sendData.data);
	CSendPacket sendPacket(m_sendData.data + sizeof(XT_HEAD));
	sendPacket<<action<<count;
	int i = 0;
	for(i = 0; i < count; ++i)
	{
#ifdef IM_USE_MEMCACHED
		m_cMemCBusi.AddModiUserInfo(pUserInfo[i]);
#endif		
		sendPacket<<pUserInfo[i].id<<pUserInfo[i].status<<pUserInfo[i].rule;
		sendPacket<<pUserInfo[i].username<<pUserInfo[i].nickname;
		sendPacket<<pUserInfo[i].clientip<<pUserInfo[i].clientport;
		sendPacket<<pUserInfo[i].sexy<<pUserInfo[i].serverip;
		sendPacket<<pUserInfo[i].lanip;//<<pUserInfo[i].lanport;
		sendPacket<<flag;
		sendPacket.WritePacket(pUserInfo[i].sessionkey,sizeof(pUserInfo[i].sessionkey));
		sendPacket.WritePacket(pUserInfo[i].p2psessionkey,sizeof(pUserInfo[i].p2psessionkey));
	}

	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_ADDMODIFY_USER_FULLINFO_QUERY,sendPacket.GetSize(),XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	::ResetEvent(m_hEvent);
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::AddModiUserInfo",CLog::TYPE_ERROR,"Wait net data time out");
		return RESULT_FAILED;
	}

	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_ADDMODIFY_USER_FULLINFO_ACK)
		return 1;

    LPGSRV_ADDMODIFY_FULLUSERINFO_ACK lpAck = (LPGSRV_ADDMODIFY_FULLUSERINFO_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->result != 0)
	{
		CLog::Log("CGlobalBusi::AddModiUserInfo",CLog::TYPE_ERROR," %s ",(char *)(lpAck + sizeof(lpAck->result)));
		return RESULT_FAILED;
	}

	count = lpAck->count;
	if(pUserID)
	{
		for(i = 0; i < count; ++i)
		{
			pUserID[i] = lpAck->user[i];
		}
	}
	return RESULT_SUCCESS;
}


int CGlobalBusi::DelUserInfo(const uint32 * auid,uint8 & count,uint32 * aOKUserID /* = NULL*/)
{
	assert(auid);
	if(count < 1)
		return 0;
	LPXT_HEAD lpSendHead  = (LPXT_HEAD)(m_sendData.data);
	LPGSRV_DEL_ONLINE_USER_QUERY lpReq = (LPGSRV_DEL_ONLINE_USER_QUERY)(m_sendData.data + sizeof(XT_HEAD));
	lpReq->count = count;
	int i = 0;
	
	CLog::Log("CGlobalBusi::DelUserInfo",CLog::TYPE_DEBUG," lpReq->count:%u ",lpReq->count);
	for(i = 0; i < count; ++i)
	{
		lpReq->userid[i] = auid[i];
		CLog::Log("CGlobalBusi::DelUserInfo",CLog::TYPE_DEBUG," lpReq->userid[i]:%u ",lpReq->userid[i]);
#ifdef IM_USE_MEMCACHED
		m_cMemCBusi.DelUserInfo(auid[i]);
#endif
	}

	int len = sizeof(GSRV_DEL_ONLINE_USER_QUERY) + sizeof(lpReq->userid) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_DEL_USER_FULLINFO_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	::ResetEvent(m_hEvent);
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::DelUserInfo",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_DEL_USER_FULLINFO_ACK)
		return 1;

    LPGSRV_DEL_ONLINE_USER_ACK lpAck = (LPGSRV_DEL_ONLINE_USER_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->result != 0)
	{
		CLog::Log("CGlobalBusi::DelUserInfo",(char *)(lpAck + sizeof(lpAck->result)));
		return 1;
	}

	count = lpAck->count;
	if(aOKUserID)
	{
		for(i = 0; i < count; ++i)
		{
			aOKUserID[i] = lpAck->userid[i];
		}
	}
	return 0;
}

/************ 家族业务*******************/
//添加全局家族资料
int CGlobalBusi::AddFamilyInfo(const tagGSRV_FAMILYINFO_ADD_QUERY::tagFamilyInfo * pFamilyInfo,uint8 & count,uint32 * pFamilyID/* = NULL*/,uint8 type /*= FAMILYINFO_QUERY_TYPE_PERM*/)
{
	assert(pFamilyInfo);
	if(count < 1)
		return 0;
	LPXT_HEAD lpSendHead  = (LPXT_HEAD)(m_sendData.data);
	LPGSRV_FAMILYINFO_ADD_QUERY lpReq = (LPGSRV_FAMILYINFO_ADD_QUERY)(m_sendData.data + sizeof(XT_HEAD));
	lpReq->count = count;
	lpReq->type = type;
	int i = 0;
	for(i = 0; i < count; ++i)
	{
#ifdef IM_USE_MEMCACHED
		m_cMemCBusi.AddModiFamilyInfo(pFamilyInfo[i],type);
#endif		
		lpReq->FamilyInfo[i].FamilyID = pFamilyInfo[i].FamilyID;
		lpReq->FamilyInfo[i].ServerIp = pFamilyInfo[i].ServerIp;
	}

	int len = sizeof(GSRV_FAMILYINFO_ADD_QUERY) + sizeof(lpReq->FamilyInfo) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_FAMILY_ADD_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_CLUSTER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	::ResetEvent(m_hEvent);
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::AddFamilyInfo",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_FAMILY_ADD_ACK)
		return 1;

    LPGSRV_FAMILYINFO_ADD_ACK lpAck = (LPGSRV_FAMILYINFO_ADD_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->result != 0)
	{
		CLog::Log("CGlobalBusi::AddFamilyInfo",CLog::TYPE_ERROR," %s ",(char *)(lpAck + sizeof(lpAck->result)));
		return 1;
	}

	count = lpAck->count;
	if(pFamilyID)
	{
		for(i = 0; i < count; ++i)
		{
			pFamilyID[i] = lpAck->FamilyID[i];
		}
	}
	return 0;
}


//删除全局家族资料
int CGlobalBusi::DelFamilyInfo(const uint32 * pDelFamilyID,uint8 & count,uint32 * pOKFamilyID/* = NULL*/,uint8 type/* = FAMILYINFO_QUERY_TYPE_PERM*/)
{
	assert(pDelFamilyID);
	if(count < 1)
		return 0;
	LPXT_HEAD lpSendHead  = (LPXT_HEAD)(m_sendData.data);
	LPGSRV_FAMILYINFO_DEL_QUERY lpReq = (LPGSRV_FAMILYINFO_DEL_QUERY)(m_sendData.data + sizeof(XT_HEAD));
	lpReq->count = count;
	lpReq->type = type;
	int i = 0;
	for(i = 0; i < count; ++i)
	{
#ifdef IM_USE_MEMCACHED
		m_cMemCBusi.DelFamilyInfo(pDelFamilyID[i],type);
#endif
		lpReq->FamilyID[i] = pDelFamilyID[i];
	}

	int len = sizeof(GSRV_FAMILYINFO_DEL_QUERY) + sizeof(lpReq->FamilyID) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_FAMILY_DEL_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_CLUSTER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	m_hEvent.ResetEvent();
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::DelFamilyInfo",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_FAMILY_DEL_ACK)
		return 1;

    LPGSRV_FAMILYINFO_DEL_ACK lpAck = (LPGSRV_FAMILYINFO_DEL_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->result != 0)
	{
		CLog::Log("CGlobalBusi::DelFamilyInfo",CLog::TYPE_ERROR," %s ",(char *)(lpAck + sizeof(lpAck->result)));
		return 1;
	}

	count = lpAck->count;
	if(pOKFamilyID)
	{
		for(i = 0; i < count; ++i)
		{
			pOKFamilyID[i] = lpAck->FamilyID[i];
		}
	}
	return 0;
}



//查询全局家族资料
int CGlobalBusi::FindFamilyInfo(const uint32 * pFamilyID,uint8 & count,tagGSRV_FAMILYINFO_QUERY_ACK::tagFamilyInfo * pFamilyInfo,uint8 type/* = FAMILYINFO_QUERY_TYPE_PERM*/)
{
	assert(pFamilyID);
	if(count < 1)
		return 0;
	LPXT_HEAD lpSendHead  = (LPXT_HEAD)(m_sendData.data);
	LPGSRV_FAMILYINFO_QUERY lpReq = (LPGSRV_FAMILYINFO_QUERY)(m_sendData.data + sizeof(XT_HEAD));
	lpReq->count = count;
	lpReq->type = type;
	int i = 0;
	for(i = 0; i < count; ++i)
	{
		lpReq->fid[i] = pFamilyID[i];
	}

	int len = sizeof(GSRV_FAMILYINFO_QUERY) + sizeof(lpReq->fid) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_FAMILYINFO_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_CLUSTER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	m_hEvent.ResetEvent();
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::FindFamilyInfo",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}

	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_FAMILYINFO_QUERY_ACK)
		return 1;

    LPGSRV_FAMILYINFO_QUERY_ACK lpAck = (LPGSRV_FAMILYINFO_QUERY_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->ret != 0)
	{
		CLog::Log("CGlobalBusi::FindFamilyInfo",CLog::TYPE_ERROR," %s ",(char *)(lpAck + sizeof(lpAck->ret)));
		return 1;
	}

	count = lpAck->count;
	if(pFamilyInfo)
	{
		for(i = 0; i < count; ++i)
		{
		
			pFamilyInfo[i].fid = lpAck->FamilyInfo[i].fid;
			pFamilyInfo[i].srvip = lpAck->FamilyInfo[i].srvip;
#ifdef IM_USE_MEMCACHED
			tagGSRV_FAMILYINFO_QUERY_ACK::tagFamilyInfo cached_FamilyInfo;			

			if( m_cMemCBusi.QueryFamilyInfo(pFamilyID[i],cached_FamilyInfo,type) )
			{
				if(memcmp(&pFamilyInfo[i], &cached_FamilyInfo, sizeof(cached_FamilyInfo)) != 0)
				{
					CLog::Log("CGlobalBusi::FindFamilyInfo",CLog::TYPE_ERROR,"FindFamilyInfo error");
					
					CLog::Log("CGlobalBusi::FindFamilyInfo",CLog::TYPE_ERROR,"pFamilyInfo.fid:%u, srvip:%u",
						pFamilyInfo[i].fid, pFamilyInfo[i].srvip);
					CLog::Log("CGlobalBusi::FindFamilyInfo",CLog::TYPE_ERROR,"cached_FamilyInfo.fid:%u, srvip:%u",
						cached_FamilyInfo.fid, cached_FamilyInfo.srvip);				
				}
			}
			else
			{
				CLog::Log("CGlobalBusi::FindFamilyInfo",CLog::TYPE_ERROR,"QueryUserInfo empty");
				CLog::Log("CGlobalBusi::FindFamilyInfo",CLog::TYPE_ERROR,"pFamilyInfo.fid:%u, srvip:%u",
						pFamilyInfo[i].fid, pFamilyInfo[i].srvip);
			}


#endif
			

			
		}
	}
	return 0;
}

int CGlobalBusi::GetBestAccount(uint32 merchant_id, uint32 group_id, std::vector<uint32_t>& ids, uint32 flow_type, uint32& id)
{	
	if(ids.size() == 0)
		return 0;
	
	LPXT_HEAD lpSendHead  = (LPXT_HEAD)(m_sendData.data);
	LPGSRV_GET_BEST_ACCOUNT_QUERY lpReq = (LPGSRV_GET_BEST_ACCOUNT_QUERY)(m_sendData.data + sizeof(XT_HEAD));

	lpReq->merchant_id = merchant_id;
	lpReq->group_id = group_id;
	lpReq->flow_type = flow_type;
	lpReq->count = ids.size();
	
	for(int i = 0; i < lpReq->count; ++i)
	{
		lpReq->id[i] = ids[i];
	}

	int len = sizeof(GSRV_GET_BEST_ACCOUNT_QUERY) + sizeof(lpReq->id) * (lpReq->count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_GET_BEST_ACCOUNT_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	
	m_hEvent.ResetEvent();
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::QueryUserStatus",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_GET_BEST_ACCOUNT_ACK)
		return 1;

    LPGSRV_GET_BEST_ACCOUNT_ACK lpAck = (LPGSRV_GET_BEST_ACCOUNT_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->result != 0)
	{
		CLog::Log("CGlobalBusi::QueryUserStatus",CLog::TYPE_ERROR," %s ",(char *)(lpAck + sizeof(lpAck->result)));
		return 1;
	}

	id = lpAck->account;

	return 0;
}

#endif


#ifdef IM_USE_MEMCACHED
CMemCBusi::CMemCBusi()
{
	MemCInit();
}

CMemCBusi::~CMemCBusi()
{
	DelServerInfo(GetServerId());
	memcache_has_init = false;
	MemCFree();
	cout << "we have quited MemCInit" << endl;
	
}

void CMemCBusi::MemCInit( )
{	
	m_memcached_lock.Lock();

	c_memc = new global_Memcache(GetMemcachedServerIp(),GetMemcachedServerPort());
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_DEBUG, 
			"111Memcached connected %s : %d successfully  %s",
			GetMemcachedServerIp(),GetMemcachedServerPort(),MemcachedMaster());	
	if(c_memc == NULL)
	{
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			"111Memcached connected %s : %d failed  %s",
			GetMemcachedServerIp(),GetMemcachedServerPort(),MemcachedMaster());
		m_memcached_lock.Unlock();		
	}

	if(memcache_has_init == false)	//one server only add once
	{	
		memcache_has_init = true;

#if 1
		if(IsMemcachedMaster() == 0) //是主服务器
		{
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_DEBUG,"This is main server");
			SetServerMagic();
		}
		else
		{
			//备等待1分钟获取
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_DEBUG,"This is slave server");
			//sleep(5);
			UpdateServerMagic();
			
		}
		
		totalserver_init();
		totaluser_init();
		AddServerInfo();

#endif		
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_DEBUG, 
			"Memcached connected %s : %d successfully  %s",
			GetMemcachedServerIp(),GetMemcachedServerPort(),MemcachedMaster());	
		
	}	
	m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_DEBUG, 
			"222Memcached connected %s : %d successfully  %s",
			GetMemcachedServerIp(),GetMemcachedServerPort(),MemcachedMaster());	
#if 0
	m_MemC = memcached_create(NULL);	

	//memcached server
	memcached_server_st *memServers;			//connect multi server		
	memServers = memcached_server_list_append(NULL, (char*)"127.0.0.1", 11211, &m_MemRc);	  
	m_MemRc = memcached_server_push(m_MemC, memServers);		
	
	if(m_MemRc == MEMCACHED_SUCCESS)
	{
		CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "Memcached connected successfully ");	
		memcached_server_free(memServers);
	}
	else
	{
		CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "Memcached connected fail");
	}
	
	//参数设置
	memcached_behavior_set(m_MemC,MEMCACHED_BEHAVIOR_DISTRIBUTION,MEMCACHED_DISTRIBUTION_CONSISTENT); 	 
	memcached_behavior_set(m_MemC, MEMCACHED_BEHAVIOR_RETRY_TIMEOUT, 20) ;	  
	memcached_behavior_set(m_MemC, MEMCACHED_BEHAVIOR_SERVER_FAILURE_LIMIT, 5) ;		
	memcached_behavior_set(m_MemC, MEMCACHED_BEHAVIOR_AUTO_EJECT_HOSTS, true) ;
#endif
	

}

void CMemCBusi::MemCFree( )
{
	delete c_memc;
}



//查询用户控制信息
bool CMemCBusi::QueryUserControlInfo(uint32 userId,tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo &userControlInfo)
{
	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo;
	m_memcached_lock.Lock();
	if(QueryUserInfo(userId,userInfo))
	{
		memcpy(userControlInfo.sessionKey, userInfo.sessionkey, sizeof(userControlInfo.sessionKey));
		memcpy(userControlInfo.p2pSessionKey, userInfo.p2psessionkey, sizeof(userControlInfo.p2pSessionKey));
		userControlInfo.srvip = userInfo.serverip;
		userControlInfo.uid = userId;
		userControlInfo.flag = 0;         /*   flag    好像没用*/
		m_memcached_lock.Unlock();
		return true;
	}
	else
	{
		CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryUserControlInfo json error");
		m_memcached_lock.Unlock();
		return false;
	}
}


//查询用户状态
bool CMemCBusi::QueryUserStatus(uint32 userId, tagGSRV_GET_USER_STATUS_ACK::tagUserStatus &userStatus)
{
	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo;
	
	m_memcached_lock.Lock();
	if(QueryUserInfo(userId, userInfo))
	{
		userStatus.id = userId;
		userStatus.status = userInfo.status;
		m_memcached_lock.Unlock();
		return true;
	}
	else
	{
		m_memcached_lock.Unlock();
		return false;
	}
}

//修改用户状态
bool CMemCBusi::ModiUserStatus(tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser user)
{
	//key
	char key[128];
	sprintf(key, "userinfo#%u#%u",GetServerMagic(), user.id);

	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo;
	m_memcached_lock.Lock();
	if(QueryUserInfo(user.id, userInfo) ==false)
	{
		m_memcached_lock.Unlock();	
		return false;
	}
	m_memcached_lock.Unlock();

	tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo userInfoModify;
	userInfoModify.id= user.id;
	//memcpy(&userInfoModify, &userInfo, sizeof(userInfoModify));
	userInfoModify.rule= userInfo.rule;
	strncpy(userInfoModify.username,userInfo.username,MAX_USERNAME_LEN);
	strncpy(userInfoModify.nickname,userInfo.nickname,MAX_USERNAME_LEN);

	userInfoModify.clientip= userInfo.clientip;
	userInfoModify.clientport= userInfo.clientport;
	userInfoModify.sexy= userInfo.sexy;
	userInfoModify.serverip= userInfo.serverip;
	userInfoModify.lanip= userInfo.lanip;
	strncpy(userInfoModify.sessionkey, userInfo.sessionkey,MAX_SESSIONKEY_LEN);
	strncpy(userInfoModify.p2psessionkey ,userInfo.p2psessionkey,MAX_SESSIONKEY_LEN);

	
	userInfoModify.status = user.status;
	AddModiUserInfo(userInfoModify);
	
	//CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "ModiUserStatus user:%u",userInfoModify.id);
	return true;
}


//查询分流
bool CMemCBusi::QueryUserFlow(uint32 userId,tagGSRV_GET_USER_FLOW_ACK::tagUserFlow &userFlow)
{
	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo;
	
	//CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryUserStatus");
	m_memcached_lock.Lock();
	if(QueryUserInfo(userId, userInfo))
	{
		userFlow.id = userId;
		userFlow.status = userInfo.status;
		userFlow.flow = userInfo.flow;
		m_memcached_lock.Unlock();
		return true;
	}
	else
	{
		m_memcached_lock.Unlock();
		return false;
	}
}


//修改分流
bool CMemCBusi::ModiUserFlow(const tagGSRV_MODIFY_USER_FLOW_QUERY::tagFlow UserFlow)
{
	//key
	char key[128];
	sprintf(key, "userinfo#%u#%u",GetServerMagic(), UserFlow.id);

	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo;
	m_memcached_lock.Lock();
	if(QueryUserInfo(UserFlow.id, userInfo) ==false)
	{
		m_memcached_lock.Unlock();	
		return false;
	}
	m_memcached_lock.Unlock();

	tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo userInfoModify;
	userInfoModify.id= UserFlow.id;
	//memcpy(&userInfoModify, &userInfo, sizeof(userInfoModify));
	userInfoModify.rule= userInfo.rule;
	strncpy(userInfoModify.username,userInfo.username,MAX_USERNAME_LEN);
	strncpy(userInfoModify.nickname,userInfo.nickname,MAX_USERNAME_LEN);

	userInfoModify.clientip= userInfo.clientip;
	userInfoModify.clientport= userInfo.clientport;
	userInfoModify.sexy= userInfo.sexy;
	userInfoModify.serverip= userInfo.serverip;
	userInfoModify.lanip= userInfo.lanip;
	userInfoModify.status = userInfo.status;
	
	strncpy(userInfoModify.sessionkey, userInfo.sessionkey,MAX_SESSIONKEY_LEN);
	strncpy(userInfoModify.p2psessionkey ,userInfo.p2psessionkey,MAX_SESSIONKEY_LEN);	

	userInfoModify.flow = UserFlow.flow;

	AddModiUserInfo(userInfoModify);
	
	//CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "ModiUserStatus user:%u",userInfoModify.id);
	return true;
}


#if 0
//查询分流
bool CMemCBusi::QueryUserFlow(uint32 userId,tagGSRV_GET_USER_FLOW_ACK::tagUserFlow &userFlow)
{
	char key[128];
	sprintf(key, "userflow#%u", userId);
	string strMemGet;
	m_MemRc = c_memc->get(key,strMemGet);
	CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryUserFlow %s %d", strMemGet.c_str(), m_MemRc);



	Json::Reader jReader;
	Json::Value jvUser;
	
	if(jReader.parse(strMemGet, jvUser))
	{
		userFlow.id = userId;
		userFlow.flow= jvUser["flow"].asUInt();
		return true;
	}
	else
	{
		CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryUserFlow json error %s", strMemGet.c_str());
		return false;
	}

}

//修改分流
bool CMemCBusi::ModiUserFlow(const tagGSRV_MODIFY_USER_FLOW_QUERY::tagFlow UserFlow)
{

	//key
	char key[128];
	sprintf(key, "userflow#%u", UserFlow.id);

	//value为json串
	Json::Value jValue;
	Json::FastWriter jFastWriter;
	string strJValue;
	jValue["flow"] = UserFlow.flow;
		
	strJValue = jFastWriter.write(jValue);
	CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "ModiUserFlow %s", strJValue.c_str());
	
	m_MemRc = c_memc->set(key,strJValue.c_str());

	CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "ModiUserFlow ret %d",  m_MemRc);

	return m_MemRc;
}

#endif

//查询用户信息自身不带锁，调用他的函数加锁
bool CMemCBusi::QueryUserInfo(uint32 nUserId, tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo& globalUserInfo)
{

	memset(&globalUserInfo, 0, sizeof(globalUserInfo));

	char key[128];
	sprintf(key, "userinfo#%u#%u", GetServerMagic(),nUserId);
	string strMemGet;
	
	m_MemRc = c_memc->get(key,strMemGet);

	if(m_MemRc != true || strMemGet.empty())
	{
		//CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryUserInfo NO user %u",nUserId);
		return false;
	}

	//CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryUserInfo %s", strMemGet.c_str());

	Json::Reader jReader;
	Json::Value jvUser;
	
	if(jReader.parse(strMemGet, jvUser))
	{
		globalUserInfo.id = nUserId;
		globalUserInfo.status = jvUser["status"].asUInt();
		globalUserInfo.rule = jvUser["rule"].asUInt();
		strcpy(globalUserInfo.username, jvUser["username"].asString().c_str());
		strcpy(globalUserInfo.nickname, jvUser["nickname"].asString().c_str());
		globalUserInfo.clientip = jvUser["clientip"].asUInt();
		globalUserInfo.clientport = jvUser["clientport"].asUInt();
		globalUserInfo.sexy = jvUser["sexy"].asUInt();
		globalUserInfo.serverip = jvUser["serverip"].asUInt();
		globalUserInfo.lanip = jvUser["lanip"].asUInt();
		globalUserInfo.flow = jvUser["flow"].asUInt();

		//写的时候没写这个字段，读的时候却要读这个字段，临时
		globalUserInfo.version = 0;
		//Base64Decode(jvUser["sessionkey"].asString()).c_str();
		string strSessionKey = Base64Decode(jvUser["sessionkey"].asString()).c_str();
		string strP2pSessionKey = Base64Decode(jvUser["p2psessionkey"].asString()).c_str();
		memcpy(globalUserInfo.sessionkey, strSessionKey.c_str(), strSessionKey.size()); 
		memcpy(globalUserInfo.p2psessionkey, strP2pSessionKey.c_str(), strP2pSessionKey.size());
 
	}
	else
	{
		CLog::Log("CMemCBusi", CLog::TYPE_ERROR, "QueryUserInfo json error %s",strMemGet.c_str());
		return false;
	}
	return true;
}


bool CMemCBusi::AddModiFamilyInfo(const tagGSRV_FAMILYINFO_ADD_QUERY::tagFamilyInfo pFamilyInfo,uint8 type)
{
	//key
	char key[128];
	sprintf(key, "familyinfo#%u#%u#%u", GetServerMagic(),type,pFamilyInfo.FamilyID);

	//value为json串
	Json::Value jvUser;
	Json::FastWriter jFastWriter;
	string strJvUser;
	jvUser["ServerIp"] = pFamilyInfo.ServerIp;			
	strJvUser = jFastWriter.write(jvUser);

	m_MemRc = c_memc->set(key,strJvUser.c_str());
	if(m_MemRc != true)
	{
		CLog::Log("CMemCBusi::AddModiFamilyInfo", CLog::TYPE_ERROR, "AddModiFamilyInfo %s %d",strJvUser.c_str(), m_MemRc);		
		return false;
	}
	
	//CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "AddModiFamilyInfo %s",strJvUser.c_str());
	return true;

}


//查询家族信息
bool CMemCBusi::QueryFamilyInfo(uint32 nFamilyId, tagGSRV_FAMILYINFO_QUERY_ACK::tagFamilyInfo& pFamilyInfo,uint8 type)
{

	memset(&pFamilyInfo, 0, sizeof(pFamilyInfo));

	char key[128];
	string strMemGet;
	sprintf(key, "familyinfo#%u#%u#%u",GetServerMagic(),type, nFamilyId);
	
	m_MemRc = c_memc->get(key,strMemGet);

	if(m_MemRc != true || strMemGet.empty())
	{
		CLog::Log("CMemCBusi::QueryFamilyInfo", CLog::TYPE_DEBUG, "NO family %u",nFamilyId);
		return false;
	}

	//CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryFamilyInfo %s", strMemGet.c_str());

	Json::Reader jReader;
	Json::Value jvUser;
	
	if(jReader.parse(strMemGet, jvUser))
	{
		pFamilyInfo.fid = nFamilyId;
		pFamilyInfo.srvip = jvUser["ServerIp"].asUInt(); 
	}
	else
	{
		CLog::Log("CMemCBusi", CLog::TYPE_ERROR, "QueryUserInfo json error %s",strMemGet.c_str());
		return false;
	}
	return true;
}


//删除家族信息
bool CMemCBusi::DelFamilyInfo(uint32 nFamilyId,uint8 type)
{
	char key[128];
	sprintf(key, "familyinfo#%u#%u#%u",GetServerMagic(),type, nFamilyId);
	m_MemRc = c_memc->remove(key);
	//CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "DelFamilyInfo %u %d", nFamilyId, m_MemRc);
	return (m_MemRc==true);

	
}

bool CMemCBusi::SetServerMagic()
{
	time_t tm;
	char magicnum[32] = {0};
	time(&tm);	
	sprintf(magicnum,"%u",(uint32)tm);
	char get_magicnum[32] = {0};
	if(c_memc->get("MagicNum",get_magicnum))
	{		
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_DEBUG, "GET Magic num %s success",get_magicnum);
		if(tm - (uint32)atoi(get_magicnum) < 15 )  //相差不超过15秒则不更新MagicNum
		{			
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_DEBUG, "We don't renew MagicNum");			
			return true;
		}
	}	

	if(c_memc->set("MagicNum",magicnum))
	{			
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_DEBUG, "set Magic num %s %u success",magicnum,tm);
		m_magic_num = tm;
		return true;
	}
	else
	{
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, "set Magic num %s %u failed",magicnum,tm);
		return false;
	}
}

uint32 CMemCBusi::GetServerMagic()
{
	uint32 timer = 0;
	time_t tm;
	time(&tm);	
	if(((tm - m_magic_num) % 300 == 250) && (m_check_magic==false))//每300秒check一下memcached是否正常，程序是否正常
	{
		UpdateServerMagic(timer);
		if(timer != m_magic_num)
		{
			if(IsMemcachedMaster() == 0)//主服务器
			{				
				SetServerMagic();				
				CLog::Log("CMemCBusi-GetServerMagic", CLog::TYPE_ERROR, "MAGIC TIME WAS EXPIRED !");
			}
			else
			{
				m_magic_num = timer;				
				CLog::Log("CMemCBusi-GetServerMagic", CLog::TYPE_ERROR, "MAGIC TIME WAS SET AS %u",m_magic_num);
			}
		}
		m_check_magic = true;	
	}
	else
	{
	
	m_check_magic = false;	 //防止重复校验
	}
	return m_magic_num;
}

uint32 CMemCBusi::UpdateServerMagic()
{
	string magic_num;
	if(c_memc->get("MagicNum",magic_num) == false)
	{		
		CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "GetServerMagic failed");
		return 0;
	}
	else
	{		
		CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "GetServerMagic %s success",magic_num.c_str());
		m_magic_num = (uint32)atoi(magic_num.c_str());
		return m_magic_num;
	}
}

uint32 CMemCBusi::UpdateServerMagic(uint32 &time)
{
	string magic_num;
	if(c_memc->get("MagicNum",magic_num) == false)
	{		
		CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "GetServerMagic failed");
		return 0;
	}
	else
	{		
		CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "GetServerMagic %s success",magic_num.c_str());
		time = (uint32)atoi(magic_num.c_str());
		return time;
	}
}


bool CMemCBusi::totalserver_init()
{
	
	char total_server[64] = {0};
	sprintf(total_server,"totalserver#%u",GetServerMagic());
	string return_value;
	char zero[] = {"0"};
	if(c_memc->get(total_server,return_value) == false)
	{
		if(c_memc->set(total_server,zero))
		{			
			CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "totalserver_init success %u",GetServerMagic());
			return true;
		}
		else
		{
			CLog::Log("CMemCBusi", CLog::TYPE_ERROR, "totalserver_init failed %u",GetServerMagic());
			return false;
		}			
	}
	return true;

}


bool CMemCBusi::totaluser_init()
{
	char total_user[64] = {0};
	sprintf(total_user,"totaluser#%u#%u",GetServerMagic(),GetServerId());
	char zero[] = {"0"};
	return c_memc->set(total_user,zero);
	if(c_memc->set(total_user,zero))
	{			
		CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "totaluser_init success %u",GetServerMagic());
		return true;
	}
	else
	{
		CLog::Log("CMemCBusi", CLog::TYPE_ERROR, "totaluser_init failed %u",GetServerMagic());
		return false;
	}	
}

bool CMemCBusi::AddServerInfo() //服务器编号从1开始
{
	uint64_t value = 0;
	char serverid_request[128] = {0};
	char serverindex_set[128] = {0};
	char serverindex_value_set[128] = {0};
	char serverid_request_2[128] = {0};
	char serverindex_value_set_2[128] = {0};
	char check[128] = {0};
	int server_info = GetServerId();
	m_memcached_lock.Lock();
	m_MemRc = true;
//查询是否存在server_id
	sprintf(serverid_request,"serverid#%u#%u",GetServerMagic(),server_info);
	if(true == c_memc->get(serverid_request,check))
	{
		//cout << "already has such server id\r\n" << endl;
		m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %u: already has such server id %u",__FUNCTION__,__LINE__,server_info);		
		return false;
	}
	
	
//增加totalserver	并get其最新值
	char total_server[64] = {0};
	sprintf(total_server,"totalserver#%u",GetServerMagic());

	if(false == c_memc->increment(string(total_server),1,&value))
	{
		m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: increment totalserver fail",__FUNCTION__,__LINE__);
		return false;
	}
//增加serverindex 索引
	sprintf(serverindex_set,"serverindex#%u#%lu",GetServerMagic(),value);
	sprintf(serverindex_value_set,"%u",server_info);
	if(false == c_memc->set(string(serverindex_set),serverindex_value_set))
	{
		m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: set serverindex_value_set fail %s",__FUNCTION__,__LINE__,serverindex_set);
		return false;

	}
//增加serverid 索引	
	sprintf(serverid_request_2,"serverid#%u#%u",GetServerMagic(),server_info);
	sprintf(serverindex_value_set_2,"%lu",value);
	if(false == c_memc->set(string(serverid_request_2),serverindex_value_set_2))
	{
		m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d:set serverid_request_2 fail %s",__FUNCTION__,__LINE__,serverid_request_2);
		return false;
	}
	//cout << "inc: value is " << value << endl;
	m_memcached_lock.Unlock();
	return true;
}


bool CMemCBusi::DelServerInfo(int server_info)//删除指定服务器，并将最后一个服务器放在删除的位置
{

	uint64_t value = 0;
	char str_1[128] = {0};
	char str_2[128] = {0};
	char str_3[128] = {0};

	string server_index;
	string total_server;
	string insert_server_id;
	unsigned int max_server_index = 0;
	m_MemRc = true;
	
	m_memcached_lock.Lock();
//获取totalserver的值
	char total_server_key[64] = {0};
	sprintf(total_server_key,"totalserver#%u",GetServerMagic());
	if(false == c_memc->get(string(total_server_key),total_server))
	{		
		m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: get totalserver fail",__FUNCTION__,__LINE__);
		return false;
	}	
	max_server_index = atoi(total_server.c_str());
	//cout <<  "max server number: " << max_server_index << endl;

//从serverid表获取serverindex
	sprintf(str_1,"serverid#%u#%u",GetServerMagic(),server_info);
	if(false == c_memc->get(string(str_1),server_index))
	{
		m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: get %s fail",__FUNCTION__,__LINE__,str_1);
		return false;
		//cout << " no such sever id" << endl;
	}
	if(false == c_memc->remove(string(str_1)))
	{		
		m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: remove %s fail",__FUNCTION__,__LINE__,str_1);
		return false;
	}

//从serverindex表中删除index 项
	sprintf(str_2,"serverindex#%u#%s",GetServerMagic(),server_index.c_str());
	//cout << "str2" << str_2 << endl;
	
	if(max_server_index > 1 && max_server_index != (unsigned int)atoi(server_index.c_str()))
	{
		//将serverindex表中的最大项插入到删除项中

		//get max index's server_id

			sprintf(str_3,"serverindex#%u#%u",GetServerMagic(),max_server_index);
			if(	false ==  c_memc->get(string(str_3),insert_server_id))
			{
				m_memcached_lock.Unlock();
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
					" %s %d: get %s fail",__FUNCTION__,__LINE__,str_3);
				return false;
			}
			string set_insert(insert_server_id.begin(),insert_server_id.end());
	
		//insert
			if( false == c_memc->set(string(str_2),insert_server_id))
			{
				m_memcached_lock.Unlock();
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
					" %s %d: set %s fail",__FUNCTION__,__LINE__,str_2);
				return false;				
			}

		//del max
			if(false == c_memc->remove(string(str_3)))
			{
				m_memcached_lock.Unlock();
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
					" %s %d: remove %s fail",__FUNCTION__,__LINE__,str_3);
				return false;					
			}

		//修改serverid表中index值
			char set_str[128] = {0};			
			sprintf(set_str,"serverid#%u#%s",GetServerMagic(),insert_server_id.c_str());			
		/*	
			string set_str(string("serverid#")+insert_server_id);
		*/
		
			char set_val[128];
			strncpy(set_val,server_index.c_str(),128);	
			if( false == c_memc->set(set_str,set_val))
			{
				m_memcached_lock.Unlock();
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
					" %s %d: set %s fail",__FUNCTION__,__LINE__,set_str);
				return false;				
			}

	}
	else
	{
		if( false == c_memc->remove(string(str_2)))
		{
			m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: remove %s fail",__FUNCTION__,__LINE__,str_2);
			return false;
		}

	}
//减少totalserver	并get其最新值	
	
	if( false == c_memc->decrement(total_server_key,1,&value))
	{
		m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: decrement totalserver fail",__FUNCTION__,__LINE__);
		return false;
	}

	m_memcached_lock.Unlock();
	return true;
}


bool CMemCBusi::QueryServerInfo(vector<int> &server_info) //请求服务器信息以及个数（server_info.length）
{
	string total_server;
	string server_id;
	int total_server_count = 0;	
	int current_server_infomation ;
	char serverindex_request[128] = {0};	
	server_info.clear();
	m_memcached_lock.Lock();	
	//获取totalserver的值
	char total_server_key[64] = {0};
	sprintf(total_server_key,"totalserver#%u",GetServerMagic());
	if( false == c_memc->get(total_server_key,total_server))
	{
		m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: get totalserver fail",__FUNCTION__,__LINE__);
		return false;
	}
	
	total_server_count = atoi(total_server.c_str());
	//cout <<  "total server: " << total_server_count << endl;
	
	for(int i = 1;i <= total_server_count;i++)
	{			
		sprintf(serverindex_request,"serverindex#%u#%d",GetServerMagic(),i);
		if(false == c_memc->get(string(serverindex_request),server_id))
		{
			m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: get %s fail",__FUNCTION__,__LINE__,serverindex_request);
			return false;
		}
		current_server_infomation = atoi(server_id.c_str());

		server_info.push_back(current_server_infomation);

		memset(serverindex_request,0,128);
		server_id.clear();		
	}
	m_memcached_lock.Unlock();
	return true;
}


bool CMemCBusi::AddModiUserInfo(const tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo globalUserInfo)
{
	uint64_t value = 1;
	char str_a[128] = {0};
	char str_b[128] = {0};
	char str_1[128] = {0};
	char str_2[128] = {0};
	char str_3[128] = {0};
	string check;
	unsigned int user_index = 0;
	m_MemRc = true; 
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" ADD USER %u",globalUserInfo.id);
	m_memcached_lock.Lock();
	//查询是否存在userinfo
	sprintf(str_a,"userinfo#%u%u",GetServerMagic(),globalUserInfo.id);
	//cout << "check " << str_a << endl;
	c_memc->get(string(str_a),check);

	if(check.empty())//add info
	{
		////cout << "no such user id\r\n" << endl;
			//增加totaluser	并get其最新值
			
		sprintf(str_b,"totaluser#%u#%u",GetServerMagic(),GetServerId()/*serverid  ---  test only*/);
		
		if( false == c_memc->increment(string(str_b),1,&value))
		{
			m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
				" %s %d: increment %s fail",__FUNCTION__,__LINE__,str_b);
			return false;

		}
		//cout << "totaluser#" <<  GetServerId() << "now :" << value << endl;

		//增加useronline表 索引---value中只有id值
		sprintf(str_1,"useronline#%u#%u#%lu",GetServerMagic(),GetServerId(),value);
		sprintf(str_2,"%u",globalUserInfo.id);		
		if( false == c_memc->set(string(str_1),str_2))
		{
		m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: set %s fail",__FUNCTION__,__LINE__,str_1);
		return false;
		}
	}
	else //modified info
	{
			/*需要从check 取出 index值*/	

		Json::Reader jReader;
		Json::Value jvUser;
		
		if(jReader.parse(check, jvUser))
		{
			user_index = jvUser["onlineindex"].asUInt();
		}
		else
		{
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: add_user_info json error %s",__FUNCTION__,__LINE__,check.c_str());	
			m_memcached_lock.Unlock();
			return false;
		}

	}

		
	//增加userinfo 索引 
	sprintf(str_3,"userinfo#%u#%u",GetServerMagic(),globalUserInfo.id);
	
	/*add user context here*/
#if 1
	//value为json串
	Json::Value jvUser;
	Json::FastWriter jFastWriter;
	string strJvUser;
	jvUser["status"] = globalUserInfo.status;
	jvUser["rule"] = globalUserInfo.rule;
	jvUser["username"] = globalUserInfo.username;
	jvUser["nickname"] = globalUserInfo.nickname;
	jvUser["clientip"] = globalUserInfo.clientip;
	jvUser["clientport"] = globalUserInfo.clientport;
	jvUser["sexy"] = globalUserInfo.sexy;
	jvUser["serverip"] = globalUserInfo.serverip;
	jvUser["lanip"] = globalUserInfo.lanip;
	jvUser["sessionkey"] = Base64Encode(globalUserInfo.sessionkey).c_str();
	jvUser["p2psessionkey"] = Base64Encode(globalUserInfo.p2psessionkey).c_str();
	if(check.empty()) //new user
	{
		jvUser["onlineindex"] = (unsigned int)value;
		jvUser["flow"] = 0;
		//cout << "[add] index: " << value << " userid : " << globalUserInfo.id << endl;
	}
	else
	{
		jvUser["onlineindex"] = user_index;
		jvUser["flow"] = globalUserInfo.flow;
		//cout << "[modified] index: " << user_index << " userid : " << globalUserInfo.id << endl;
	}
	
	strJvUser = jFastWriter.write(jvUser);
#endif
	
	/*end add user context */
	if( false == c_memc->set(string(str_3),strJvUser.c_str()))
	{
		m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,str_3);
		return false;
	}
	//cout << "set : " << str_3 << " value :" << strJvUser << endl;
	////cout << "inc: value is " << value << endl;
	m_memcached_lock.Unlock();
	return true;
}


bool CMemCBusi::DelUserInfo(uint32 nUserId)
{

	uint64_t value = 0;
	char str_a[128] = {0};
	char str_1[128] = {0};
	char str_2[128] = {0};
	char str_3[128] = {0};
	string del_user_info;
	string total_user;
	string insert_user_id;
	string user_information;
	unsigned int max_user_index = 0;
	unsigned int user_index = 1;
	m_MemRc = true;
	m_memcached_lock.Lock();
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" DELETE USER %u",nUserId);
//获取totaluser的值
	sprintf(str_a,"totaluser#%u#%u",GetServerMagic(),GetServerId());
	if( false == c_memc->get(string(str_a),total_user))
	{
		m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: get %s fail",__FUNCTION__,__LINE__,str_a);
		return false;
	}
	
	max_user_index = (unsigned int)atoi(total_user.c_str());
	//cout <<  "DELETE :" << nUserId << " dec: " << total_user << endl;

//从userinfo表获取userindex
	sprintf(str_1,"userinfo#%u#%u",GetServerMagic(),nUserId);
	if( false == c_memc->get(string(str_1),del_user_info))
	{
		m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: get %s fail",__FUNCTION__,__LINE__,str_1);
		return false;
	}

	if( false == c_memc->remove(string(str_1)))
	{
		m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: remove %s fail",__FUNCTION__,__LINE__,str_1);
		return false;
	}
 	//cout << "[1]remove: " << str_1 << endl;
	/*需要从del_user_info 取出 index值*/
	

	Json::Reader jReader;
	Json::Value jvUser;
	
	if(jReader.parse(del_user_info, jvUser))
	{
		user_index = jvUser["onlineindex"].asUInt();
	}
	else
	{
		CLog::Log("CMemCBusi", CLog::TYPE_ERROR, 
			"del_user_info json error %s",del_user_info.c_str());	
		m_memcached_lock.Unlock();
		return false;
	}

	/*end parse*/	

	sprintf(str_2,"useronline#%u#%u#%u",GetServerMagic(),GetServerId(),user_index);  //删除或者修改该key

//多个用户且不是删除index最大的用户时，从表中插入
	if( (max_user_index > 1) && (user_index != max_user_index))
	{
		//将serverindex表中的最大项插入到删除项中

		//get max index's user_id

		sprintf(str_3,"useronline#%u#%u#%u",GetServerMagic(),GetServerId(),max_user_index);
		if( false == c_memc->get(string(str_3),insert_user_id))
		{
			m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: get %s fail",__FUNCTION__,__LINE__,str_3);
			return false;
		}

		
//		string set_insert(insert_user_id.begin(),insert_user_id.end());	
			//insert			
		if( false == c_memc->set(str_2,insert_user_id.c_str()))
		{
			m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
				" %s %d: set %s fail",__FUNCTION__,__LINE__,str_2);
			return false;
		}
			
		//del max
		if( false == c_memc->remove(string(str_3)))
		{
			m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: remove %s fail",__FUNCTION__,__LINE__,str_3);
			return false;
		}

		//cout << "[3]remove: " << str_3 << endl;
		//修改userinfo表中曾经最大index对应用户的index值
		char userinfo_key[128] = {0};
		sprintf(userinfo_key,"userinfo#%u#%s",GetServerMagic(),insert_user_id.c_str());
		if( false == c_memc->get(userinfo_key,user_information))
		{
			m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: get %s fail",__FUNCTION__,__LINE__,userinfo_key);
			return false;
		}

		
		//修改user_information中index值
		
		//cout << "before modified" << user_information << endl;
		int pos1 = user_information.find("onlineindex");
		string part_A(user_information.begin(),user_information.begin()+pos1);
		string part_X(user_information.begin()+pos1+1,user_information.end());
		int pos2 = part_X.find(",");
		string part_C(part_X.begin()+pos2,part_X.end());
		//cout << "Part A "<< part_A << " Part C " << part_C << endl;
		char exchange_modified[128] = {0};
		sprintf(exchange_modified,"onlineindex\":%u",user_index);
		string abc(exchange_modified);

		string final = part_A + abc + part_C;
		//cout << "after modified: " << final << endl; 
		/*end parse*/	

		if( false == c_memc->set(userinfo_key,final.c_str(),strlen(final.c_str()),0,0))
		{
			m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: set userinfo fail",__FUNCTION__,__LINE__);
			return false;
		}

	}
	else
	{
		//从useronline表中删除index 项

		if( false == c_memc->remove(string(str_2)))
		{
			m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: remove %s fail",__FUNCTION__,__LINE__,str_2);
			return false;
		}

		//cout << "[2]remove: " << str_2 << endl;

	}

	//减少totaluser	并get其最新值

	if( false == c_memc->decrement(string(str_a),1,&value))
	{
		m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: decrement %s fail",__FUNCTION__,__LINE__,str_a);
		return false;
	}
	m_memcached_lock.Unlock();
	return true;

}

bool CMemCBusi::query_total_user_number(uint32 &total_count)//请求在线用户信息
{
	vector<int> server_info;
	string total_user;
	int total_user_count = 0;
	total_count = 0;
	char str_1[128] = {0};
	
	if(false == QueryServerInfo(server_info))
	{
		return false;		
	}	
	m_memcached_lock.Lock();
	for(vector<int>::iterator iter = server_info.begin();iter != server_info.end(); iter++)
	{
		sprintf(str_1,"totaluser#%u#%u",GetServerMagic(),*iter);	//获取某一个server下totalserver的值
		if( false == c_memc->get(string(str_1),total_user))
		{
			m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: get %s fail",__FUNCTION__,__LINE__,str_1);
			return false;
		}
		total_user_count = atoi(total_user.c_str());
		total_count += total_user_count;
		
	}
	m_memcached_lock.Unlock();
	return true;

}

bool CMemCBusi::SearchOnlineUser(tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo *pUserInfo,uint8 & count,uint32& nOnlineNum,uint16 startpos)//请求在线用户信息
{
	vector<int> server_info;
	
	string total_user;
	string user_id;
	string strMemGet;
	uint32 each_total_user_count = 0;	
	uint32 calc_get_user_count = 0;
	char str_1[128] = {0};
	char str_2[128] = {0};
	char str_3[128] = {0};
	bool already_skip = false;
	QueryServerInfo(server_info);
	
	m_memcached_lock.Lock();
	for(vector<int>::iterator iter = server_info.begin();iter != server_info.end(); iter++)
	{
		

		sprintf(str_1,"totaluser#%u#%u",GetServerMagic(),*iter);	//获取某一个server下totalserver的值
		if(false == c_memc->get(string(str_1),total_user))
		{
			m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: get %s fail",__FUNCTION__,__LINE__,str_1);
			return false;
		}
		
		each_total_user_count = atoi(total_user.c_str());
		//count: query user number
		//left_count: total user in all
		//total_user_count :each server_id 's user number
		uint32 count_t = 1;

		if(false == already_skip)
		{
			if(startpos < each_total_user_count)
			{
				count_t = startpos + 1;
			}
			else
			{
				startpos -= each_total_user_count;
				continue;
			}
		}
		
		for(;(count_t <= each_total_user_count);count_t++)
		{
			sprintf(str_2,"useronline#%u#%u#%u",GetServerMagic(),GetServerId(),count_t);		
			if( false == c_memc->get(string(str_2),user_id))
			{
				m_memcached_lock.Unlock();
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
					" %s %d: get %s fail",__FUNCTION__,__LINE__,str_2);
				return false;
			}
		
			sprintf(str_3,"userinfo#%u#%s",GetServerMagic(),user_id.c_str());		
			if(false == c_memc->get(string(str_3),strMemGet))
			{
				m_memcached_lock.Unlock();
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
					" %s %d: get %s fail",__FUNCTION__,__LINE__,str_3);
				return false;
			}


			/*parse info*/			
			CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryUserInfo %s", strMemGet.c_str());
			
			tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo = {0};
			
			if(true == QueryUserInfo(atoi(user_id.c_str()),userInfo))
			{
				if(userInfo.status != 18 /*HIDE STATES*/ )
				{
					CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_DEBUG,
						"SearchOnlineUser_id %u",userInfo.id);
		
					int pUserInfo_index = count_t - 1;
					pUserInfo[pUserInfo_index].clientid = userInfo.id;
					pUserInfo[pUserInfo_index].sexy = userInfo.sexy;
					pUserInfo[pUserInfo_index].status = userInfo.status;
					pUserInfo[pUserInfo_index].rule = userInfo.rule;
					strncpy(pUserInfo[pUserInfo_index].username,userInfo.username,MAX_USERNAME_LEN);
					strncpy(pUserInfo[pUserInfo_index].nickname, userInfo.nickname,MAX_USERNAME_LEN);
					 
					calc_get_user_count++;//success get non hide users
				}
				else
				{			
		
					CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_DEBUG, 
						"SearchOnlineUser_id %u --HIDE STATES",userInfo.id);
				}
						
			}
			if(calc_get_user_count == count)
			{
				m_memcached_lock.Unlock();
				query_total_user_number(nOnlineNum);
				return true;
			}	
			/*end parse info*/

			user_id.clear();
			strMemGet.clear();
			memset(str_2,0,128);
			memset(str_3,0,128);
			already_skip = true;
		}
		
		
	}

	count = calc_get_user_count;
	
	m_memcached_lock.Unlock();
	
	query_total_user_number(nOnlineNum);
	
	//获取totalserver的值
	return true;
}


bool CMemCBusi::SearchOnlineUser(tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo *pUserInfo,uint32 & count,uint32& nOnlineNum,uint16 startpos)//请求在线用户信息
{
	vector<int> server_info;
	
	string total_user;
	string user_id;
	string strMemGet;
	uint32 each_total_user_count = 0;	
	uint32 calc_get_user_count = 0;
	char str_1[128] = {0};
	char str_2[128] = {0};
	char str_3[128] = {0};
	bool already_skip = false;
	QueryServerInfo(server_info);
	
	m_memcached_lock.Lock();
	for(vector<int>::iterator iter = server_info.begin();iter != server_info.end(); iter++)
	{

		sprintf(str_1,"totaluser%u#%u",GetServerMagic(),*iter);	//获取某一个server下totalserver的值
		if(false == c_memc->get(string(str_1),total_user))
		{
			m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: get %s fail",__FUNCTION__,__LINE__,str_1);
			return false;
		}
		
		each_total_user_count = atoi(total_user.c_str());
		//cout <<  "total user: " << each_total_user_count << endl;
		//count: query user number
		//left_count: total user in all
		//total_user_count :each server_id 's user number
		uint32 count_t = 1;
		if(false == already_skip)
		{
			if(startpos < each_total_user_count)
			{
				count_t = startpos + 1;
			}
			else
			{
				startpos -= each_total_user_count;
				continue;
			}
		}
		
		for(;(count_t <= each_total_user_count);count_t++)
		{
			sprintf(str_2,"useronline#%u#%u#%u",GetServerMagic(),GetServerId(),count_t);		
			if( false == c_memc->get(string(str_2),user_id))
			{
				m_memcached_lock.Unlock();
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
					" %s %d: get %s fail",__FUNCTION__,__LINE__,str_2);
				return false;
			}
		
			//cout << "head: " << str_2 << "   content: " << user_id << endl;
			sprintf(str_3,"userinfo#%u#%s",GetServerMagic(),user_id.c_str());		
			if(false == c_memc->get(string(str_3),strMemGet))
			{
				m_memcached_lock.Unlock();
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
					" %s %d: get %s fail",__FUNCTION__,__LINE__,str_3);
				return false;
			}


			/*parse info*/			
			//cout << "head: " << str_3 << "   content(user_information): " << strMemGet << endl;
			CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryUserInfo %s", strMemGet.c_str());
			
			tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo = {0};
			
			if(true == QueryUserInfo(atoi(user_id.c_str()),userInfo))
			{
				if(userInfo.status != 18 /*HIDE STATES*/ )
				{
					CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_DEBUG,
						"SearchOnlineUser_id %u",userInfo.id);
//					printf("SearchOnlineUser_id %u\r\n",userInfo.id);
					int pUserInfo_index = count_t - 1;
					pUserInfo[pUserInfo_index].clientid = userInfo.id;
					pUserInfo[pUserInfo_index].sexy = userInfo.sexy;
					pUserInfo[pUserInfo_index].status = userInfo.status;
					pUserInfo[pUserInfo_index].rule = userInfo.rule;
					strncpy(pUserInfo[pUserInfo_index].username,userInfo.username,MAX_USERNAME_LEN);
					strncpy(pUserInfo[pUserInfo_index].nickname, userInfo.nickname,MAX_USERNAME_LEN);
					 
					calc_get_user_count++;//success get non hide users
				}
				else
				{			
//					printf("SearchOnlineUser_id %u --HIDE STATES\r\n",userInfo.id);
					CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_DEBUG,
						"SearchOnlineUser_id %u --HIDE STATES",userInfo.id);
				}
						
			}
			if(calc_get_user_count == count)
			{
				m_memcached_lock.Unlock();
				query_total_user_number(nOnlineNum);
				return true;
			}	
			/*end parse info*/

			user_id.clear();
			strMemGet.clear();
			memset(str_2,0,128);
			memset(str_3,0,128);
			already_skip = true;
		}
		
		
	}

	count = calc_get_user_count;
	
	m_memcached_lock.Unlock();
	
	query_total_user_number(nOnlineNum);
	
	//获取totalserver的值
	return true;
} 	




bool CMemCBusi::FetchKeysValues(vector<string> &keys,vector<string> &values)
{
	cout << "ready to fetch values" << endl;
	bool rc1 = c_memc->mget(keys);
	cout << "end fetch values: " << rc1 <<  endl;
	string keys1;
	string values1;

	keys.clear();
	values.clear();
	while(c_memc->fetch(keys1,values1))
	{
		keys.push_back(keys1);
		values.push_back(values1);
		
		cout << "key: " << keys1 << " values: " << values1 << endl;
	}
	cout << "finished FetchKeysValues: "  << endl;
	
	return true;
}

bool CMemCBusi::BestAccountProcess(uint32 merchant_id, uint32 group_id, std::vector<uint32_t>& accounts, uint32 flow_type, uint32& id)
{
	char tmp_merchant_group[128] = {0};
	char tmp_best_id[32] = {0};
	string str_account;
	uint32 tmp_account = 0;
	uint32 best_id = 0;
	
	if(flow_type == 0)
	{
		sprintf(tmp_merchant_group,"Merchant_ID#%u#%u",GetServerMagic(),merchant_id);
	}
	else
	{
		sprintf(tmp_merchant_group,"Group_ID#%u#%u",GetServerMagic(),group_id);		
	}	
	c_memc->get(string(tmp_merchant_group),str_account);
	tmp_account = (uint32)atoi(str_account.c_str());
	for(size_t i = 0; i < accounts.size(); i ++)
	{
		if(accounts[i] > tmp_account)
		{
			best_id = accounts[i];
			break;
		}	
	}
	if(best_id == 0)
	{
		best_id = accounts[0];
	}
	sprintf(tmp_best_id,"%u",best_id);
	if(false == c_memc->set(tmp_merchant_group,tmp_best_id))
	{				
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, " %s %d: set %s fail",__FUNCTION__,__LINE__,tmp_merchant_group);
		return false;
	}
	id = best_id;
	return true;
}


bool CMemCBusi::GetBestAccount(uint32 merchant_id, uint32 group_id, std::vector<uint32_t>& ids, uint32 flow_type, uint32& id)
{

	vector<string> Merchants_keys;
	vector<string> Merchants_values;
	vector<uint32_t> accounts;
	bool success_rt = true;
	string tmp1_string;

	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo UserInfo;
	for(vector<uint32_t>::iterator iter = ids.begin() ; iter != ids.end(); iter++)
	{
		m_memcached_lock.Lock();
//		success_rt &= QueryUserStatus(*iter, userStatus);
//		success_rt &= QueryUserFlow(*iter,userFlow);
		success_rt = QueryUserInfo(*iter,UserInfo);
//		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, " %s %d: search user info ret : %d",__FUNCTION__,__LINE__,success_rt);
		
		m_memcached_lock.Unlock();
		if(success_rt == true && UserInfo.status != XTREAM_OFFLINE &&
			UserInfo.flow == XT_MOD_HANGUP_STATE_REQ::RECEPTION) /*HIDE STATES*/
		{
			accounts.push_back(*iter);
		}		
	}
	if(! accounts.empty())
	{
		sort(accounts.begin(),accounts.end());
		m_memcached_lock.Lock();
		if(false == BestAccountProcess(merchant_id,group_id,accounts,flow_type,id))
		{
			m_memcached_lock.Unlock();
			return false;
		}	
		m_memcached_lock.Unlock();
		return true;
	}
	else
	{		
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, " %s %d: accounts empty ",__FUNCTION__,__LINE__);
		return false;
	}

}


#endif

