#include "StdAfx.h"
#include ".\globalbusi.h"
#include "GlobalBusiControl.h"
#include "xtpacket.h"
#include "..\src\miniim\User.h"

#define WAITTIMEOUT 1000 * 40 // 40 s  

CGlobalBusi::CGlobalBusi(GlobalBusiControl * pBusiControl)
{
	m_pRecvData = new RecvDataStruct();
	assert(pBusiControl != NULL);
	m_hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_pBusiControl = pBusiControl;
	m_vLink = m_pBusiControl->AllocVLink();
	m_pBusiControl->RegisterLink(m_vLink,this);

	m_sendData.vlink = m_vLink;
}

CGlobalBusi::~CGlobalBusi(void)
{
	SAFE_DELETE(m_pRecvData);
	::CloseHandle(m_hEvent);
}
int CGlobalBusi::SearchOnlineUser(tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo * pUserInfo,uint8 & count,uint32& nOnlineNum)
{
	assert(pUserInfo);
	if(count < 1)
		return 0;
	CSendPacket sendPacket(m_sendData.data);
	XT_HEAD head;
	m_pBusiControl->InitHead(&head,CMDGSRV_SEARCH_USER_QUERY,sizeof(uint32),XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);
	sendPacket.SetXtHead(head);
	sendPacket<<count;

	//Send Data
	m_sendData.data_len = sendPacket.GetSize();
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::SearchOnlineUser","wait net data time out");
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
		CLog::Log("CGlobalBusi::SearchOnlineUser",(char *)recvPacket.GetCursor());
		return 1;
	}

	recvPacket>>count>>nOnlineNum;
	if(pUserInfo)
	{
		for(int i = 0; i < count; ++i)
		{
			recvPacket	>>pUserInfo[i].clientid
						>>pUserInfo[i].headid
						>>pUserInfo[i].sexy
						>>pUserInfo[i].status
						>>pUserInfo[i].rule
						>>pUserInfo[i].username
						>>pUserInfo[i].nickname 
							;
		}
	}


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
	for(int i = 0; i < count; ++i)
	{
		lpReq->uid[i] = auid[i];
		assert( (lpReq->uid[i] > 0) && (lpReq->uid[i] < MAX_139_ACCOUNT_NUM) );
	}

	int len = sizeof(GSRV_USERCONTROLINFO_QUERY) + sizeof(lpReq->uid) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_USERCONTROLINFO_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::QueryUserControlInfo","wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_USERCONTROLINFO_QUERY_ACK)
		return 1;

    LPGSRV_USERCONTROLINFO_QUERY_ACK lpAck = (LPGSRV_USERCONTROLINFO_QUERY_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->ret != 0)
	{
		CLog::Log("CGlobalBusi::QueryUserControlInfo",(char *)(lpAck + sizeof(lpAck->ret)));
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
	for(int i = 0; i < count; ++i)
	{
		lpReq->userid[i] = aID[i];
	}

	int len = sizeof(GSRV_GET_USER_STATUS_QUERY) + sizeof(lpReq->userid) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_GET_USERSTATUS_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::QueryUserStatus","wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_GET_USERSTATUS_ACK)
		return 1;

    LPGSRV_GET_USER_STATUS_ACK lpAck = (LPGSRV_GET_USER_STATUS_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->result != 0)
	{
		CLog::Log("CGlobalBusi::QueryUserStatus",(char *)(lpAck + sizeof(lpAck->result)));
		return 1;
	}

	count = lpAck->count;
	if(pUStatus)
	{
		for(i = 0; i < count; ++i)
		{
			pUStatus[i].id = lpAck->UserStatus[i].id;
			pUStatus[i].status = lpAck->UserStatus[i].status;
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
	for(int i = 0; i < count; ++i)
	{
		lpReq->user[i].id = pUserStatus[i].id;
		lpReq->user[i].status = pUserStatus[i].status;
	}

	int len = sizeof(GSRV_MODIFY_ONLINE_STATUS_QUERY) + sizeof(lpReq->user) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_MODIFY_USERSTATUS_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::ModiUserStatus","wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_MODIFY_USERSTATUS_ACK)
		return 1;

    LPGSRV_MODIFY_ONLINE_STATUS_ACK lpAck = (LPGSRV_MODIFY_ONLINE_STATUS_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->result != 0)
	{
		CLog::Log("CGlobalBusi::ModiUserStatus",(char *)(lpAck + sizeof(lpAck->result)));
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

int CGlobalBusi::QueryUserInfo(const uint32 * auid,uint8 & count,tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo * pUserInfo)
{
	assert(auid);
	if(count < 1)
		return 0;
	LPXT_HEAD lpSendHead  = (LPXT_HEAD)(m_sendData.data);
	LPGSRV_GET_ONLINE_FULLINFO_QUERY lpReq = (LPGSRV_GET_ONLINE_FULLINFO_QUERY)(m_sendData.data + sizeof(XT_HEAD));
	lpReq->count = count;
	for(int i = 0; i < count; ++i)
	{
		lpReq->userid[i] = auid[i];
	}

	int len = sizeof(GSRV_GET_ONLINE_FULLINFO_QUERY) + sizeof(lpReq->userid) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_GET_USER_FULLINFO_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::QueryUserInfo","wait net data time out");
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
		CLog::Log("CGlobalBusi::QueryUserInfo",(char *)recvPacket.GetCursor());
		return 1;
	}

	recvPacket>>count;
	if(pUserInfo)
	{
		for(i = 0; i < count; ++i)
		{
			recvPacket>>pUserInfo[i].id>>pUserInfo[i].status>>pUserInfo[i].version \
				>>pUserInfo[i].username>>pUserInfo[i].nickname \
				>>pUserInfo[i].clientip>>pUserInfo[i].clientport>>pUserInfo[i].sexy>>pUserInfo[i].headid>>pUserInfo[i].serverip;
			recvPacket.ReadPacket(pUserInfo[i].sessionkey,sizeof(pUserInfo[i].sessionkey));
			recvPacket.ReadPacket(pUserInfo[i].p2psessionkey,sizeof(pUserInfo[i].p2psessionkey));
		}
	}
	return 0;
}

int CGlobalBusi::AddModiUserInfo(CUser* pUser,uint8 action )
{
	assert(pUser);

	LPXT_HEAD lpSendHead  = (LPXT_HEAD)(m_sendData.data);
	CSendPacket sendPacket(m_sendData.data + sizeof(XT_HEAD));
	sendPacket<<action<<(uint8)1;

	sendPacket<<pUser->m_nID<<pUser->m_nStatus<<pUser->m_AllowRule;
	sendPacket<<pUser->m_szUserName<<pUser->m_szNickName;
	sendPacket<<pUser->m_nIP<<pUser->m_nPort;
	sendPacket<<pUser->m_nSexy<<pUser->m_nHeadID<< m_pBusiControl->GetIPAddr();
	sendPacket.WritePacket(pUser->m_SessionKey,sizeof(pUser->m_SessionKey));
	sendPacket.WritePacket(pUser->m_P2PSessionKey ,sizeof(pUser->m_P2PSessionKey));


	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_ADDMODIFY_USER_FULLINFO_QUERY,sendPacket.GetSize(),XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::AddModiUserInfo","wait net data time out");
		return 1;
	}

	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_ADDMODIFY_USER_FULLINFO_ACK)
		return 1;

	LPGSRV_ADDMODIFY_FULLUSERINFO_ACK lpAck = (LPGSRV_ADDMODIFY_FULLUSERINFO_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->result != 0)
	{
		CLog::Log("CGlobalBusi::AddModiUserInfo",(char *)(lpAck + sizeof(lpAck->result)));
		return 1;
	}

	return 0;
}

int CGlobalBusi::AddModiUserInfo(const tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo * pUserInfo,uint8 & count,uint32 * pUserID,uint8 action /* = tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD*/)
{
	assert(pUserInfo);
	if(count < 1)
		return 0;
	LPXT_HEAD lpSendHead  = (LPXT_HEAD)(m_sendData.data);
	CSendPacket sendPacket(m_sendData.data + sizeof(XT_HEAD));
	sendPacket<<action<<count;
	for(int i = 0; i < count; ++i)
	{
		sendPacket<<pUserInfo[i].id<<pUserInfo[i].status<<pUserInfo[i].rule;
		sendPacket<<pUserInfo[i].username<<pUserInfo[i].nickname;
		sendPacket<<pUserInfo[i].clientip<<pUserInfo[i].clientport;
		sendPacket<<pUserInfo[i].sexy<<pUserInfo[i].headid<<pUserInfo[i].serverip;
		sendPacket.WritePacket(pUserInfo[i].sessionkey,sizeof(pUserInfo[i].sessionkey));
		sendPacket.WritePacket(pUserInfo[i].p2psessionkey,sizeof(pUserInfo[i].p2psessionkey));
	}

	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_ADDMODIFY_USER_FULLINFO_QUERY,sendPacket.GetSize(),XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::AddModiUserInfo","wait net data time out");
		return 1;
	}

	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_ADDMODIFY_USER_FULLINFO_ACK)
		return 1;

    LPGSRV_ADDMODIFY_FULLUSERINFO_ACK lpAck = (LPGSRV_ADDMODIFY_FULLUSERINFO_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->result != 0)
	{
		CLog::Log("CGlobalBusi::AddModiUserInfo",(char *)(lpAck + sizeof(lpAck->result)));
		return 1;
	}

	count = lpAck->count;
	if(pUserID)
	{
		for(i = 0; i < count; ++i)
		{
			pUserID[i] = lpAck->user[i];
		}
	}
	return 0;
}

int CGlobalBusi::DelUserInfo(const uint32 * auid,uint8 & count,uint32 * aOKUserID /* = NULL*/)
{
	assert(auid);
	if(count < 1)
		return 0;
	LPXT_HEAD lpSendHead  = (LPXT_HEAD)(m_sendData.data);
	LPGSRV_DEL_ONLINE_USER_QUERY lpReq = (LPGSRV_DEL_ONLINE_USER_QUERY)(m_sendData.data + sizeof(XT_HEAD));
	lpReq->count = count;
	for(int i = 0; i < count; ++i)
	{
		lpReq->userid[i] = auid[i];
	}

	int len = sizeof(GSRV_DEL_ONLINE_USER_QUERY) + sizeof(lpReq->userid) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_DEL_USER_FULLINFO_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::DelUserInfo","wait net data time out");
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
	for(int i = 0; i < count; ++i)
	{
		lpReq->FamilyInfo[i].FamilyID = pFamilyInfo[i].FamilyID;
		lpReq->FamilyInfo[i].ServerIp = pFamilyInfo[i].ServerIp;
	}

	int len = sizeof(GSRV_FAMILYINFO_ADD_QUERY) + sizeof(lpReq->FamilyInfo) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_FAMILY_ADD_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_CLUSTER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::AddFamilyInfo","wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_FAMILY_ADD_ACK)
		return 1;

    LPGSRV_FAMILYINFO_ADD_ACK lpAck = (LPGSRV_FAMILYINFO_ADD_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->result != 0)
	{
		CLog::Log("CGlobalBusi::AddFamilyInfo",(char *)(lpAck + sizeof(lpAck->result)));
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
	for(int i = 0; i < count; ++i)
	{
		lpReq->FamilyID[i] = pDelFamilyID[i];
	}

	int len = sizeof(GSRV_FAMILYINFO_DEL_QUERY) + sizeof(lpReq->FamilyID) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_FAMILY_DEL_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_CLUSTER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::DelFamilyInfo","wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_FAMILY_DEL_ACK)
		return 1;

    LPGSRV_FAMILYINFO_DEL_ACK lpAck = (LPGSRV_FAMILYINFO_DEL_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->result != 0)
	{
		CLog::Log("CGlobalBusi::DelFamilyInfo",(char *)(lpAck + sizeof(lpAck->result)));
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
	for(int i = 0; i < count; ++i)
	{
		lpReq->fid[i] = pFamilyID[i];
	}

	int len = sizeof(GSRV_FAMILYINFO_QUERY) + sizeof(lpReq->fid) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_FAMILYINFO_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_CLUSTER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	m_pBusiControl->SendRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::FindFamilyInfo","wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_FAMILY_ADD_ACK)
		return 1;

    LPGSRV_FAMILYINFO_QUERY_ACK lpAck = (LPGSRV_FAMILYINFO_QUERY_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->ret != 0)
	{
		CLog::Log("CGlobalBusi::FindFamilyInfo",(char *)(lpAck + sizeof(lpAck->ret)));
		return 1;
	}

	count = lpAck->count;
	if(pFamilyInfo)
	{
		for(i = 0; i < count; ++i)
		{
			pFamilyInfo[i].fid = lpAck->FamilyInfo[i].fid;
			pFamilyInfo[i].srvip = lpAck->FamilyInfo[i].srvip;
		}
	}
	return 0;
}