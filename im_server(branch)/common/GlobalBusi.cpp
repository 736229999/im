#include "stdafx.h"
#include "GlobalBusi.h"
#include "GlobalBusiControl.h"
#include "xtpacket.h"
#include "../src/MiniIM/User.h"
#include "Function.h"
#include <sched.h>
#include "ClientProtocol.h"

#define WAITTIMEOUT 1000 * 40 // 40 s  

#ifdef IM_USE_CACHE

using namespace std;
using namespace global_cache;
bool CMemCBusi::memcache_has_init = false;
CSyncCritical CMemCBusi::m_memcached_lock;
uint32 CMemCBusi::m_magic_num = 0;
CConfigInfo	CMemCBusi::m_ConfigInfo;

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
//	CLog::Log("CGlobalBusi::CGlobalBusi",CLog::TYPE_ERROR,"vlink %d",m_vLink);

	m_sendData.vlink = m_vLink;
}

CGlobalBusi::CGlobalBusi(GlobalBusiControl * pBusiControl,ImMemCache *memcached):
m_hEvent(FALSE,FALSE)
{
	m_pRecvData = new RecvDataStruct();
	assert(pBusiControl != NULL);
	//m_hEvent = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_pBusiControl = pBusiControl;
	m_vLink = m_pBusiControl->AllocVLink();
	m_pBusiControl->RegisterLink(m_vLink,this);
//	CLog::Log("CGlobalBusi::CGlobalBusi",CLog::TYPE_ERROR,"vlink %d",m_vLink);

	m_sendData.vlink = m_vLink;
	m_cMemCBusi = CMemCBusi(memcached);
}


CGlobalBusi::~CGlobalBusi(void)
{
	SAFE_DELETE(m_pRecvData);
	//::CloseHandle(m_hEvent);
}

#ifdef IM_USE_CACHE
int CGlobalBusi::SearchOnlineUser(tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo * pUserInfo,uint8 & count,uint32& nOnlineNum,uint16 startpos)
{
	//CLog::Log("CGlobalBusi::SearchOnlineUser-MEMCACHED",CLog::TYPE_DEBUG,"INTO SearchOnlineUser");
	//CLog::Log("CGlobalBusi::SearchOnlineUser-MEMCACHED",CLog::TYPE_DEBUG,"count %x %u",count,nOnlineNum);
	assert(pUserInfo);
	if(count < 1)
		return 0;
	int ret = -1;
	ret = m_cMemCBusi.SearchOnlineUser(pUserInfo,count,nOnlineNum,startpos);	
	CLog::Log("CGlobalBusi::SearchOnlineUser-MEMCACHED",CLog::TYPE_DEBUG,"ret %d count:%x, nOnlineNum:%u ",ret ,count,nOnlineNum);
	if(ret == MEMCACHED_SERVER_DOWN)
		return MEMCACHED_SERVER_DOWN;
	else
		return 0;
}
bool CGlobalBusi::UpdateCsMonitor(CUser *pUser)
{
	if(!pUser) 
	{
		return false;
	}
	else
	{
		m_cMemCBusi.UpdateCsMonitor(pUser);
		CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"UpdateCsMonitor uid:%u",
						pUser->m_nID);
		return true;
	}
}

bool CGlobalBusi::QueryCsMonitor(uint32 userId,tagGSRV_GET_ONLINE_MONITOR_ACK::UserInfo &userInfo)
{
	int ret = 0;
	if(userId == 0) 
	{
		return false;
	}
	else
	{
		ret = m_cMemCBusi.QueryCsMonitor(userId,userInfo);
		CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"QueryCsMonitor uid:%u",
						userId);
		if(ret == MEMCACHED_GET_INFO_SUCCESS)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
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
#ifdef IM_USE_MULTIDEVICE
int CGlobalBusi::QueryUserDeviceInfo(const uint32 auid,const uint32 deviceid,vector<multiDevice_t> &deviceinfo)
{
	if(auid == 0)
		return 0;
	if(deviceid != 0)	//请求单设备
	{
		multiDevice_t deviceInfoOne;
		if(m_cMemCBusi.GetDeviceUserInfo(auid,deviceid,deviceInfoOne))
		{
			deviceinfo.push_back(deviceInfoOne);
			return RESULT_SUCCESS;
		}
		else
		{
			return RESULT_FAILED;
		}		
	}
	else				//请求多设备
	{		
		if(m_cMemCBusi.GetAllDeviceUserInfo(auid,deviceinfo))
		{			
			return RESULT_SUCCESS;
		}
		else
		{			
			return RESULT_FAILED;
		}
	}	
}
//单设备
int CGlobalBusi::QueryUserDeviceInfo(const uint32 auid,const uint32 deviceid,multiDevice_t &deviceinfo)
{
	if(auid == 0 || deviceid == 0)
	{
		return RESULT_FAILED;
	}

	if(m_cMemCBusi.GetDeviceUserInfo(auid,deviceid,deviceinfo))
	{
		return RESULT_SUCCESS;
	}
	else
	{
		return RESULT_FAILED;
	}		
	
}


int CGlobalBusi::GetDeviceUserCount(uint32 userid, uint32 &count)
{
	if(userid == 0)
	{		
		return RESULT_FAILED;
	}
	
	if(m_cMemCBusi.GetDeviceUserCount(userid,count))
	{			
		return RESULT_SUCCESS;
	}
	else
	{		
		return RESULT_FAILED;
	}
}

bool CGlobalBusi::DeviceUserOnline(uint32 userid, uint32 ip,uint16 port ,multiDevice_t *deviceInfo)
{
	if(userid == 0)
	{		
		return RESULT_FAILED;
	}
	vector<multiDevice_t> deviceinfo;
	if(QueryUserDeviceInfo(userid,0,deviceinfo))
	{
		vector<multiDevice_t>::iterator it  = deviceinfo.begin();
		for(;
		it != deviceinfo.end();
		it++
		)
		{
			if(it->clientIp == ip && it->clientPort == port)
			{
				break;
			}
		}
		if(it != deviceinfo.end())
		{
			if(deviceInfo != NULL)
			{
				//memcpy(deviceInfo,it,sizeof(multiDevice_t));
				*deviceInfo = *it;
			}
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	

}

int CGlobalBusi::AddModiDeviceUserInfo(const shared_ptr<CUser> &pUser)
{
	m_cMemCBusi.AddDeviceUserInfo(pUser);	
	return 0;
}

int CGlobalBusi::AddModiDeviceHeartInfo(uint32 id,uint32 deviceId,uint32 timestamp)
{
	m_cMemCBusi.AddModiDeviceHeartInfo(id,deviceId,timestamp);	
	return 0;
}

int CGlobalBusi::AddModiDeviceIpPortInfo(uint32 id,uint32 deviceId,uint32 ip,uint16 port)
{
	m_cMemCBusi.AddModiDeviceIpPortInfo(id,deviceId,ip,port);	
	return 0;
}

int CGlobalBusi::DelDeviceUserInfo(uint32 userId,uint32 deviceId)
{
	if(userId == 0 || deviceId == 0)
		return 1;
	
	m_cMemCBusi.DelDeviceUserInfo(userId,deviceId);	
	return 0;
}

#endif

int CGlobalBusi::ModiUserIpPortInfo(const uint32 auid,uint32 ip,uint16 port)
{
	if(auid == 0)
	{
		return 0;
	}
	CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"ModiUserIpPortInfo uid:%u, srvip:%u port %d ",
			auid,ip,port);
	if(m_cMemCBusi.ModiUserIpPortInfo(auid,ip,port))
	{
		return RESULT_SUCCESS;
	}
	else
	{
		return RESULT_FAILED;
	}

}

int CGlobalBusi::QueryUserStatus(const uint32 * aID,tagGSRV_GET_USER_STATUS_ACK::tagUserStatus * pUStatus,uint8 & count)
{
	assert(aID);
	if(count < 1)
		return 0;
	int ret = -1;
	
	if(pUStatus)
	{
		for(int i = 0; i < count; ++i)
		{
			ret = m_cMemCBusi.QueryUserStatus(aID[i], pUStatus[i]);
			if(ret == MEMCACHED_GET_INFO_SUCCESS)
			{
				CLog::Log("CGlobalBusi-QueryUserStatus",CLog::TYPE_DEBUG,"QueryUserStatus OK id:%u, status:%u",
						pUStatus[i].id, pUStatus[i].status);			
			}
			else if(ret == MEMCACHED_NO_INFO)
			{
				pUStatus[i].status = XTREAM_OFFLINE; 
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"QueryUserStatus NO id:%u status %u",pUStatus[i].id,pUStatus[i].status);
			}
			else if(ret == MEMCACHED_SERVER_DOWN)
			{
				
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"MEMCACHED_SERVER_DOWN",pUStatus[i].id);
				return MEMCACHED_SERVER_DOWN;
			}
			

		}
	}
	return ret;
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


int CGlobalBusi::QueryUserTeamID(const uint32 * aID,tagGSRV_GET_USER_TEAMID_ACK::tagUserTeamID * pUTeamID,uint8 & count)
{
	assert(aID);
	if(count < 1)
		return 0;
	int ret = -1;
	
	if(pUTeamID)
	{
		for(int i = 0; i < count; ++i)
		{
			ret = m_cMemCBusi.QueryUserTeamID(aID[i], pUTeamID[i]);
			if(ret == MEMCACHED_GET_INFO_SUCCESS)
			{
				CLog::Log("CGlobalBusi-QueryUserStatus",CLog::TYPE_DEBUG,"QueryUserStatus OK id:%u, status:%u",
						pUTeamID[i].id, pUTeamID[i].teamID);			
			}
			else if(ret == MEMCACHED_NO_INFO)
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"QueryUserStatus NO id:%u",pUTeamID[i].id);
			}
			else if(ret == MEMCACHED_SERVER_DOWN)
			{
				
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"MEMCACHED_SERVER_DOWN",pUTeamID[i].id);
				return MEMCACHED_SERVER_DOWN;
			}
			

		}
	}
	return ret;
}

int CGlobalBusi::ModiUserTeamID(const tagGSRV_MODIFY_ONLINE_TEAMID_QUERY::tagUserTeamID* pUserTeamID,uint8 & count)
{
	assert(pUserTeamID);
	if(count < 1)
		return 0;

	int i = 0;
	for(i = 0; i < count; ++i)
	{

		m_cMemCBusi.ModiUserTeamID(pUserTeamID[i]);

	}
	CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"ModiUserTeamID OK");

	return 0;
}

// 从redis中获取用户全量信息
int CGlobalBusi::GetFUserInfo(uint32 userid, CUser &user)
{
	int rlt = m_cMemCBusi.GetFUserInfo(userid,user);
	if(rlt == MEMCACHED_GET_INFO_SUCCESS)
	{
		CLog::Log("CGlobalBusi::GetFUserInfo",CLog::TYPE_DEBUG,"success");
	}
	else
	{
		CLog::Log("CGlobalBusi::GetFUserInfo",CLog::TYPE_DEBUG,"failed");
	}

	return rlt;
}



bool CGlobalBusi::AddLogoutLock(uint32 userId)
{
	BOOL ret = false;
	ret = m_cMemCBusi.AddLogoutLock(userId);
	CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"AddLogoutLock userid:%u, ret:%d",userId, ret);
	return ret;
	
}
bool CGlobalBusi::DelLogoutLock(uint32 userId)
{
	BOOL ret = false;
	ret = m_cMemCBusi.DelLogoutLock(userId);
	CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"DelLogoutLock userid:%u, ret:%d",userId, ret);
	return ret;
}


bool CGlobalBusi::GetLogoutLock(uint32 userId)
{
	BOOL ret = false;
	ret = m_cMemCBusi.GetLogoutLock(userId);
	CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"GetLogoutLock userid:%u, ret:%d",userId, ret);
	return ret;
}

int CGlobalBusi::QueryUserFlow(const uint32 * aID, tagGSRV_GET_USER_FLOW_ACK::tagUserFlow * pUserFlow,uint8 & count)
{
	assert(aID);
	if(count < 1)
		return 0;
	int ret = -1;
	if(pUserFlow)
	{
		for(int i = 0; i < count; ++i)
		{

			ret = m_cMemCBusi.QueryUserFlow(aID[i],pUserFlow[i]);
			CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"QueryUserFlow  %u %u %u",
					pUserFlow[i].id, pUserFlow[i].status, pUserFlow[i].flow);
		}

	}
	
	return ret;
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
	int ret = -1;
	
	if(pUserInfo)
	{
		for(int i = 0; i < count; ++i)
		{
			ret = m_cMemCBusi.QueryUserInfo(auid[i], pUserInfo[i]);
			if( ret == MEMCACHED_GET_INFO_SUCCESS)
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"QueryUserInfo:");
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG," %u %u %u %s %s %u %u %u %u %s %s",
					pUserInfo[i].id, pUserInfo[i].status, pUserInfo[i].version,
					pUserInfo[i].username, pUserInfo[i].nickname, 
					pUserInfo[i].clientport, pUserInfo[i].sexy, pUserInfo[i].serverip,pUserInfo[i].logintime,
					Base64Encode(pUserInfo[i].sessionkey, MAX_SESSIONKEY_LEN).c_str(),
					Base64Encode(pUserInfo[i].p2psessionkey, MAX_SESSIONKEY_LEN).c_str()
					);
					
			}
			else  if(ret == MEMCACHED_NO_INFO || ret == MEMCACHED_JASON_ERROR)
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_IMPORTANT,"QueryUserInfo empty");
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_IMPORTANT," %u %u %u %s %s %u %u %u %u %s %s",
					pUserInfo[i].id, pUserInfo[i].status, pUserInfo[i].version,
					pUserInfo[i].username, pUserInfo[i].nickname, 
					pUserInfo[i].clientport, pUserInfo[i].sexy, pUserInfo[i].serverip,pUserInfo[i].logintime,
					Base64Encode(pUserInfo[i].sessionkey, MAX_SESSIONKEY_LEN).c_str(), 
					Base64Encode(pUserInfo[i].p2psessionkey, MAX_SESSIONKEY_LEN).c_str()
					);
			}
			else if(ret == MEMCACHED_SERVER_DOWN)
			{				
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_ERROR,"MEMCACHED_SERVER_DOWN");
				return MEMCACHED_SERVER_DOWN;
			}			
		}
	}
	return MEMCACHED_GET_INFO_SUCCESS;
}


int CGlobalBusi::AddModiUserInfo(CUser* pUser,uint8 action,uint8 flag)
{
	assert(pUser);
	m_cMemCBusi.AddModiUserInfo(pUser,action,flag);

	
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


int CGlobalBusi::DelUserInfo(const uint32 * auid,uint8 & count,uint32 * deviceID /* = NULL*/)
{
	assert(auid);
	if(count < 1)
		return 0;

	
	CLog::Log("CGlobalBusi::DelUserInfo-MEMCACHED",CLog::TYPE_DEBUG," DELETED count %u",count);
	for(int i = 0; i < count; ++i)
	{		
		CLog::Log("CGlobalBusi::DelUserInfo-MEMCACHED",CLog::TYPE_DEBUG,"lpReq->userid[%u] %u",i,auid[i]);
		
#ifdef IM_USE_MULTIDEVICE
		m_cMemCBusi.DelUserInfo(auid[i]);
		if(deviceID)
		{			
			m_cMemCBusi.DelDeviceUserInfo(auid[i],deviceID[i]);
		}
#else
		if(m_cMemCBusi.DelUserInfo(auid[i]) && (deviceID))
			deviceID[i] = auid[i];
#endif
	}		
	return 0;
}

int CGlobalBusi::QueryCSInfo(const uint32 * auid,uint8 & count,CSinfo_t * pUserInfo)
{
	assert(auid);
	if(count < 1)
		return 0;
	int ret = -1;
	
	if(pUserInfo)
	{
		for(int i = 0; i < count; ++i)
		{
			ret = m_cMemCBusi.QueryCSInfo(auid[i], pUserInfo[i]);
			if( ret == MEMCACHED_GET_INFO_SUCCESS)
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"QueryCSInfo:");
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG," %u %u %u %u %u %s",
					pUserInfo[i].m_nID, pUserInfo[i].m_State, pUserInfo[i].m_CurConsult,
					pUserInfo[i].m_CurReception, pUserInfo[i].m_MaxRecConsult, 
					pUserInfo[i].m_Csname);
				for(int j = 0 ; j < (int)pUserInfo[i].m_Customerid.size(); j++ )
				{
					CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"m_Customerid[%d][%d] %u",
					i,j,pUserInfo[i].m_Customerid[j]);
				}
			}
			else  if(ret == MEMCACHED_NO_INFO || ret == MEMCACHED_JASON_ERROR)
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_IMPORTANT,"QueryCSInfo empty");	
				return ret;
			}
			else if(ret == MEMCACHED_SERVER_DOWN)
			{				
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_ERROR,"MEMCACHED_SERVER_DOWN");
				return MEMCACHED_SERVER_DOWN;
			}			
		}
	}
	return MEMCACHED_GET_INFO_SUCCESS;
}

int CGlobalBusi::QueryCSQueueLength(vector<uint32> queueid,uint8  count,tagGSRV_GET_ONLINE_CSQLEN_ACK::CSQinfo* csQlen)
{
	//assert(queueid);
	if(count < 1)
		return 0;
	int ret = -1;
	
	if(csQlen)
	{
		for(int i = 0; i < count; ++i)
		{
			ret = m_cMemCBusi.QueryCSQueueLength(queueid[i], csQlen[i]);
			if( ret == MEMCACHED_GET_INFO_SUCCESS)
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"QueryCSQueueLength:");
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"team %u  QueueLength %u",
					csQlen[i].teamId, csQlen[i].queueLength);						
			}
			else if(ret == MEMCACHED_NO_INFO || ret == MEMCACHED_JASON_ERROR)
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_IMPORTANT,"QueryCSQueueLength empty");	
				return ret;
			}
			else if(ret == MEMCACHED_SERVER_DOWN)
			{
				
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_ERROR,"MEMCACHED_SERVER_DOWN");
				return MEMCACHED_SERVER_DOWN;
			}			
		}
	}
	return MEMCACHED_GET_INFO_SUCCESS;
}

int CGlobalBusi::setExpireUserinfo(uint32 userid,uint32 timeout)
{
	if(userid == 0)
	{
		return RESULT_FAILED;
	}
	m_cMemCBusi.setExpireUserinfo(userid,timeout);
	return RESULT_SUCCESS;
}

int CGlobalBusi::GetTUserInfo(uint32 userId,tagGSRV_TUSERINFO_ACK &TUserInfo)
{
	int rlt = m_cMemCBusi.GetTUserInfo(userId,TUserInfo);
	if(rlt == MEMCACHED_GET_INFO_SUCCESS)
	{
		CLog::Log("CGlobalBusi::GetTUserInfo",CLog::TYPE_DEBUG,"success");
	}
	else
	{
		CLog::Log("CGlobalBusi::GetTUserInfo",CLog::TYPE_DEBUG,"failed");
	}

	return rlt;
}

bool CGlobalBusi::SetTUserInfo(uint32 userId, tagGSRV_TUSERINFO_ACK &TUserInfo)
{
	return m_cMemCBusi.SetTUserInfo(userId,TUserInfo);
}

// 查询一台服务器上所有的用户
int CGlobalBusi::GetAllUser(int nServerNo, vector<tagGSRV_TUSERINFO_ACK_EX> &vecUser)
{
	return m_cMemCBusi.GetAllUser(nServerNo, vecUser);
}

// 修改用户签名
bool CGlobalBusi::ModiUserSign(uint32 userid,const char *pUserSign)
{
	return m_cMemCBusi.ModiUserSign(userid, pUserSign);
}

// 更新在线时长
bool CGlobalBusi::ModiNewLiveTime(uint32 userid, uint32 newlivetime)
{
	return m_cMemCBusi.ModiNewLiveTime(userid, newlivetime);
}

// 修改用户的部分信息
bool CGlobalBusi::ModiPartUserInfo(uint32 userid, const STRU_PART_USERINFO &struPartUserInfo)
{
	return m_cMemCBusi.ModiPartUserInfo(userid, struPartUserInfo);
}

// 修改用户登录时间
bool CGlobalBusi::ModiUserLoginTime(uint32 userid, uint32 logintime)
{
	return m_cMemCBusi.ModiUserLoginTime(userid, logintime);
}

// 获取排队列表
bool CGlobalBusi::GetCusterList(uint32 userid, vector<uint32> &vecList)
{
	return m_cMemCBusi.GetCusterList(userid, vecList);
}


#if 0
int CGlobalBusi::QueryCSInfo(const uint32 * auid,uint8 & count,ClientService* pUserInfo)
{
	assert(auid);
	if(count < 1)
		return 0;
	int ret = -1;
	
	if(pUserInfo)
	{
		for(int i = 0; i < count; ++i)
		{
			ret = m_cMemCBusi.QueryCSInfo(auid[i], pUserInfo[i]);
			if( ret == MEMCACHED_GET_INFO_SUCCESS)
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"QueryUserInfo:");
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG," %u %u %u %u %u %s",
					pUserInfo[i].m_nID, pUserInfo[i].m_State, pUserInfo[i].m_CurConsult,
					pUserInfo[i].m_CurReception, pUserInfo[i].m_MaxRecConsult, 
					pUserInfo[i].m_Csname);
				for(int j = 0 ; j <pUserInfo[i].m_Customerid.size(), j++ )
				{
					CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"m_Customerid[%d][%d] %u",
					i,j,pUserInfo[i].m_Customerid[j]);
				}
			}
			else  if(ret == MEMCACHED_NO_INFO || ret == MEMCACHED_JASON_ERROR)
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_ERROR,"QueryUserInfo empty");	
				return ret;
			}
			else if(ret == MEMCACHED_SERVER_DOWN)
			{				
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_ERROR,"MEMCACHED_SERVER_DOWN");
				return MEMCACHED_SERVER_DOWN;
			}			
		}
	}
	return MEMCACHED_GET_INFO_SUCCESS;
}
#endif

int CGlobalBusi::AddModiCSInfo(const CSinfo_t * pUserInfo,uint8 & count,uint8 action )
{
	assert(pUserInfo);
	if(count < 1)
		return 0;
	CLog::Log("CGlobalBusi::AddModiCSInfo-MEMCACHED",CLog::TYPE_DEBUG," ADDED count %u",count);
	for(int i = 0; i < count; ++i)
	{
		CLog::Log("CGlobalBusi::AddModiCSInfo-MEMCACHED",CLog::TYPE_DEBUG,"lpReq->userid[%u] %u",i,pUserInfo[i].m_nID);
		m_cMemCBusi.AddModiCSInfo(pUserInfo[i],action);
		
	}
	return RESULT_SUCCESS;
}
#if 0
int CGlobalBusi::AddModiCSInfo(const ClientService* pUserInfo,uint8 & count,uint8 action /* = tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD*/)
{
	assert(pUserInfo);
	if(count < 1)
		return 0;
	CLog::Log("CGlobalBusi::AddModiUserInfo-MEMCACHED",CLog::TYPE_DEBUG," ADDED count %u",count);
	for(int i = 0; i < count; ++i)
	{
		CLog::Log("CGlobalBusi::AddModiUserInfo-MEMCACHED",CLog::TYPE_DEBUG,"lpReq->userid[%u] %u",i,pUserInfo[i].id);
		m_cMemCBusi.AddModiCSInfo(pUserInfo[i],action);
	}
	return RESULT_SUCCESS;
}
#endif

int CGlobalBusi::DelCSInfo(const uint32 * auid,uint8 & count)
{
	assert(auid);
	if(count < 1)
		return 0;

	
	CLog::Log("CGlobalBusi::DelUserInfo-MEMCACHED",CLog::TYPE_DEBUG," DELETED count %u",count);
	for(int i = 0; i < count; ++i)
	{		
		CLog::Log("CGlobalBusi::DelUserInfo-MEMCACHED",CLog::TYPE_DEBUG,"lpReq->userid[%u] %u",i,auid[i]);
		m_cMemCBusi.DelCSInfo(auid[i]);

	}		
	return 0;
}

/************ 客服排队业务*******************/

int CGlobalBusi::QueryCSQueueInfo(const uint32 * teamID,uint8  count,tagGSRV_GET_ONLINE_CSQINFO_ACK::CSQinfo* csQinfo)
{
	assert(teamID);
	if(count < 1)
		return 0;
	int ret = -1;
	int last_ret = -1;
	if(csQinfo)
	{
		for(int i = 0; i < count; ++i)
		{
			ret = m_cMemCBusi.QueryCSQueueInfo(teamID[i], csQinfo[i]);
			if( ret == MEMCACHED_GET_INFO_SUCCESS)
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"QueryCSQueueInfo:");
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG," TeamID : %u ",
					teamID[i]);
				for(vector<CSQList_t>::iterator iter = csQinfo[i].m_Customerlist.begin();
					iter != csQinfo[i].m_Customerlist.end(); iter++ )
				{
					CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"m_Customerid [%d] %u",
					csQinfo[i].teamID,iter->userid);
				}
					last_ret = MEMCACHED_GET_INFO_SUCCESS;
			}
			else  if(ret == MEMCACHED_NO_INFO || ret == MEMCACHED_JASON_ERROR)
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_IMPORTANT,"QueryUserInfo empty");	
				if(last_ret != MEMCACHED_GET_INFO_SUCCESS)
				{
					ret = MEMCACHED_NO_INFO;
					last_ret = MEMCACHED_GET_INFO_SUCCESS;
				}
				//return ret;
			}
			else if(ret == MEMCACHED_SERVER_DOWN)
			{				
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_ERROR,"MEMCACHED_SERVER_DOWN");
				return MEMCACHED_SERVER_DOWN;
			}			
		}
	}
	return last_ret;
}

#if 0
int CGlobalBusi::QueryCSQueueInfo(const uint32 * auid,uint8 & count,CsQueue* csQueue)
{
	assert(auid);
	if(count < 1)
		return 0;
	int ret = -1;
	
	if(auid)
	{
		for(int i = 0; i < count; ++i)
		{
			ret = m_cMemCBusi.QueryCSInfo(auid[i], csQueue[i]);
			if( ret == MEMCACHED_GET_INFO_SUCCESS)
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"QueryCSQueueInfo:");
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG," TeamID : %u ",
					csQueue[i].teamID);
				for(list<uint32>::iterator iter = csQueue[i].m_Customerid.begin();
					iter != csQueue[i].m_Customerid.end(), iter++ )
				{
					CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG,"m_Customerid [%d] %u",
					csQueue[i].teamID,*iter);
				}

			}
			else  if(ret == MEMCACHED_NO_INFO || ret == MEMCACHED_JASON_ERROR)
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_ERROR,"QueryUserInfo empty");	
				return ret;
			}
			else if(ret == MEMCACHED_SERVER_DOWN)
			{				
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_ERROR,"MEMCACHED_SERVER_DOWN");
				return MEMCACHED_SERVER_DOWN;
			}			
		}
	}
	return MEMCACHED_GET_INFO_SUCCESS;
}
#endif

int CGlobalBusi::AddDelCSQueueInfo(const tagGSRV_ADDMODIFY_CSQINFO_QUERY::CSinfo * csQinfo,uint8 & count,uint8 action /* = tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD*/)
{
	assert(csQinfo);
	if(count < 1)
		return 0;
	CLog::Log("CGlobalBusi::AddDelCSQueueInfo-MEMCACHED",CLog::TYPE_DEBUG,"IN  ADDED or DELETE count %u",count);
	for(int i = 0; i < count; ++i)
	{
		CLog::Log("CGlobalBusi::AddDelCSQueueInfo-MEMCACHED",CLog::TYPE_DEBUG,
			"teamID : %u customerID : %u level %u requestTime %u",
			csQinfo[i].teamID ,csQinfo[i].customerID,csQinfo[i].customerLevel,csQinfo[i].customerReqTime);
		m_cMemCBusi.AddDelCSQueueInfo(csQinfo[i],action);
		
	}
	return RESULT_SUCCESS;
}

int CGlobalBusi::RemoveCSQueueInfo(uint32 *teamID,uint8 & count)
{
	assert(auid);
	if(count < 1)
		return 0;
	
	CLog::Log("CGlobalBusi::DelUserInfo-MEMCACHED",CLog::TYPE_DEBUG," DELETED count %u",count);
	for(int i = 0; i < count; ++i)
	{		
		CLog::Log("CGlobalBusi::DelUserInfo-MEMCACHED",CLog::TYPE_DEBUG,"lpReq->userid[%u] %u",i,teamID[i]);
		m_cMemCBusi.RemoveCSQueueInfo(teamID[i]);
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
			if( m_cMemCBusi.QueryFamilyInfo(pFamilyID[i],pFamilyInfo[i],type) == MEMCACHED_GET_INFO_SUCCESS)
			{						
				
					CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_DEBUG," %u %u",
					pFamilyInfo[i].fid, pFamilyInfo[i].srvip);	
				
			}
			else
			{
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_IMPORTANT,"QueryUserInfo empty");
				CLog::Log("CGlobalBusi-MEMCACHED",CLog::TYPE_IMPORTANT," %u %u",
						pFamilyInfo[i].fid, pFamilyInfo[i].srvip);
			}			
		}
	}
	return 0;
}


int CGlobalBusi::GetBestAccount(uint32 merchant_id, uint32 group_id, std::vector<uint32>& ids, uint32 flow_type, uint32& id)
{	
	return m_cMemCBusi.GetBestAccount(merchant_id,group_id,ids,flow_type,id);
}


int CGlobalBusi::GetBestCSAccount(uint32 merchant_id, uint32 group_id, std::vector<uint32>& ids, uint32 flow_type, uint32& id)
{	
	int ret = m_cMemCBusi.GetBestCSAccount(merchant_id,group_id,ids,flow_type,id);
	
	CLog::Log("CGlobalBusi::QueryOtherUserInfo",CLog::TYPE_IMPORTANT,"GetBestCSAccount %u",id);
	return ret;
}


//memcached服务器出现问题之后请求用户数据
int CGlobalBusi::QueryOtherUserInfo(const uint32 * auid,uint8 & count,tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo * pUserInfo)
{
	
//	CLog::Log("CGlobalBusi::QueryOtherUserInfo",CLog::TYPE_DEBUG,"INTO HERE!!");
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
		CLog::Log("CGlobalBusi::QueryOtherUserInfo",CLog::TYPE_IMPORTANT,"auid[i] %d",auid[i]);
	}

	int len = sizeof(GSRV_GET_ONLINE_FULLINFO_QUERY) + sizeof(lpReq->userid) * (count - 1);
	lpSendHead->from = XT_HEAD::FROM_SERVER;
	m_pBusiControl->InitHeadMulCast(lpSendHead,CMDGSRV_GET_USER_OTHER_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_CONNECT);
	CLog::Log("CGlobalBusi::QueryOtherUserInfo",CLog::TYPE_IMPORTANT,"lpSendHead->from %d should be %d",lpSendHead->from,XT_HEAD::FROM_SERVER);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	::ResetEvent(m_hEvent);
	m_pBusiControl->SendMultiRequestData(m_sendData); //multicast

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::QueryOtherUserInfo",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_GET_USER_OTHER_ACK)
		return 1;

	//LPGSRV_GET_ONLINE_FULLINFO_ACK
	uint8 uResult = 0;
	CRecvPacket recvPacket((char *)(m_pRecvData->data + sizeof(XT_HEAD)),m_pRecvData->data_len - sizeof(XT_HEAD));
	recvPacket>>uResult;
	if(uResult != 0)
	{
		CLog::Log("CGlobalBusi::QueryOtherUserInfo",CLog::TYPE_IMPORTANT," %s ",(char *)recvPacket.GetCursor());
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
			
			CLog::Log("CGlobalBusi::QueryOtherUserInfo",CLog::TYPE_IMPORTANT,"USERID %u STATE %d",pUserInfo[i].id,pUserInfo[i].status);
		}
	}
	return 0;
}



int CGlobalBusi::QueryOtherControlInfo(const uint32 * auid,tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo * pUserControlInfo,uint8 & count)
{
	
	assert(auid);
	if(count < 1 || count > 20)
		return 0;
	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo pUserInfo[20];
	if(pUserControlInfo)
	{
		QueryOtherUserInfo(auid,count,pUserInfo);
		for(int i = 0; i < count; ++i)
		{
			pUserControlInfo[i].uid = pUserInfo[i].id;
			pUserControlInfo[i].srvip = pUserInfo[i].serverip;
			memcpy(pUserControlInfo[i].sessionKey,pUserInfo[i].sessionkey,sizeof(pUserControlInfo[i].sessionKey));
			memcpy(pUserControlInfo[i].p2pSessionKey,pUserInfo[i].p2psessionkey,sizeof(pUserControlInfo[i].p2pSessionKey));			
		}
		return 0;
	}
	return 0;
}

int CGlobalBusi::QueryOtherUserStatus(const uint32 * aID,tagGSRV_GET_USER_STATUS_ACK::tagUserStatus * pUStatus,uint8 & count)
{
	
	//CLog::Log("CGlobalBusi::QueryOtherUserStatus",CLog::TYPE_IMPORTANT,"INTO HERE!!");
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
		CLog::Log("CGlobalBusi::QueryOtherUserStatus",CLog::TYPE_IMPORTANT,"auid[i] %d",aID[i]);

	}

	int len = sizeof(GSRV_GET_USER_STATUS_QUERY) + sizeof(lpReq->userid) * (count - 1);
	lpSendHead->from = XT_HEAD::FROM_SERVER;
	m_pBusiControl->InitHeadMulCast(lpSendHead,CMDGSRV_USERSTATUS_OTHER_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);
	CLog::Log("CGlobalBusi::QueryOtherUserStatus",CLog::TYPE_IMPORTANT,"lpSendHead->from %d should be %d",lpSendHead->from,XT_HEAD::FROM_SERVER);


	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	::ResetEvent(m_hEvent);
	m_pBusiControl->SendMultiRequestData(m_sendData); //multicast

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::QueryOtherUserStatus",CLog::TYPE_ERROR,"wait net data time out");
		return 1;
	}
	//checkdata
	for(auto recvMsgs:m_MultiRecvData)
	{		
		LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(recvMsgs.data);
		if(lpRecvHead->cmd != CMDGSRV_USERSTATUS_OTHER_ACK)
		{
			m_MultiRecvData.clear();
			return 1;
		}
		LPGSRV_GET_USER_STATUS_ACK lpAck = (LPGSRV_GET_USER_STATUS_ACK)(recvMsgs.data + sizeof(XT_HEAD));
		if(lpAck->result != 0)
		{
		   CLog::Log("CGlobalBusi::QueryOtherUserStatus",CLog::TYPE_ERROR," %s ",(char *)(lpAck + sizeof(lpAck->result)));
		   m_MultiRecvData.clear();
		   return 1;
		}
	
		count = lpAck->count;
		if(pUStatus)
		{
		   for(i = 0; i < count; ++i)
		   {
				pUStatus[i].id = lpAck->UserStatus[i].id;
				if(lpAck->UserStatus[i].status != (uint8)XTREAM_OFFLINE)//前面已经初始化为离线状态
				{
					pUStatus[i].status = lpAck->UserStatus[i].status;		   
				}
			  	CLog::Log("CGlobalBusi::QueryOtherUserStatus",CLog::TYPE_IMPORTANT,"REPLY -- USERID %u STATE %d",pUStatus[i].id,pUStatus[i].status);
		   }
		}
	}
	m_MultiRecvData.clear();
	return 0;
}


int CGlobalBusi::QueryOtherUserFlow(const uint32 * aID, tagGSRV_GET_USER_FLOW_ACK::tagUserFlow * pUserFlow,uint8 & count)
{
	assert(aID);
	if(count < 1 || count > 20)
		return 0;
	
	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo pUserInfo[20];

	if(pUserFlow)
	{		
		QueryOtherUserInfo(aID,count,pUserInfo);
		for(int i = 0; i < count; ++i)
		{
			pUserFlow[i].id = pUserInfo[i].id;
			pUserFlow[i].status = pUserInfo[i].status;
			pUserFlow[i].flow = pUserInfo[i].flow;
		}
	}
	return 0;
}

//查询全局家族资料
int CGlobalBusi::FindOtherFamilyInfo(const uint32 * pFamilyID,uint8 & count,tagGSRV_FAMILYINFO_QUERY_ACK::tagFamilyInfo * pFamilyInfo,uint8 type/* = FAMILYINFO_QUERY_TYPE_PERM*/)
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
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_FAMILYINFO_OTHER_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_CONNECT);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	m_hEvent.ResetEvent();
	m_pBusiControl->SendMultiRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::FindOtherFamilyInfo",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_FAMILYINFO_OTHER_ACK)
		return 1;

    LPGSRV_FAMILYINFO_QUERY_ACK lpAck = (LPGSRV_FAMILYINFO_QUERY_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->ret != 0)
	{
		CLog::Log("CGlobalBusi::FindOtherFamilyInfo",CLog::TYPE_ERROR," %s ",(char *)(lpAck + sizeof(lpAck->ret)));
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

int CGlobalBusi::GetOtherBestAccount(uint32 merchant_id, uint32 group_id, std::vector<uint32_t>& ids, uint32 flow_type, uint32& id)
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
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_GET_BEST_OTHER_ACCOUNT_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_CONNECT);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	
	m_hEvent.ResetEvent();
	m_pBusiControl->SendMultiRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::QueryUserStatus",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_GET_BEST_OTHER_ACCOUNT_ACK)
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



/*逻辑上暂时不需要，留作日后使用*/
#if 0
int CGlobalBusi::AddModiOtherUserInfo(CUser* pUser,uint8 action,uint8 flag)
{
	assert(pUser);
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


	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_MOD_USER_OTHER_QUERY,sendPacket.GetSize(),XT_HEAD::DEST_TYPE_SERVER,SERVERID_CONNECT);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	::ResetEvent(m_hEvent);
	m_pBusiControl->SendMultiRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::AddModiUserInfo",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}

	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_MOD_USER_OTHER_ACK)
		return 1;

	LPGSRV_ADDMODIFY_FULLUSERINFO_ACK lpAck = (LPGSRV_ADDMODIFY_FULLUSERINFO_ACK)(m_pRecvData->data + sizeof(XT_HEAD));
	if(lpAck->result != 0)
	{
		CLog::Log("CGlobalBusi::AddModiUserInfo",CLog::TYPE_ERROR," %s ",(char *)(lpAck + sizeof(lpAck->result)));
		return 1;
	}

	return 0;
}


int CGlobalBusi::AddModiOtherUserInfo(const tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo * pUserInfo,uint8 & count,uint32 * pUserID,uint8 action /* = tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD*/,uint8 flag)
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
		sendPacket<<pUserInfo[i].id<<pUserInfo[i].status<<pUserInfo[i].rule;
		sendPacket<<pUserInfo[i].username<<pUserInfo[i].nickname;
		sendPacket<<pUserInfo[i].clientip<<pUserInfo[i].clientport;
		sendPacket<<pUserInfo[i].sexy<<pUserInfo[i].serverip;
		sendPacket<<pUserInfo[i].lanip;//<<pUserInfo[i].lanport;
		sendPacket<<flag;
		sendPacket.WritePacket(pUserInfo[i].sessionkey,sizeof(pUserInfo[i].sessionkey));
		sendPacket.WritePacket(pUserInfo[i].p2psessionkey,sizeof(pUserInfo[i].p2psessionkey));
	}

	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_MOD_USER_OTHER_QUERY,sendPacket.GetSize(),XT_HEAD::DEST_TYPE_SERVER,SERVERID_CONNECT);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	::ResetEvent(m_hEvent);
	m_pBusiControl->SendMultiRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::AddModiUserInfo",CLog::TYPE_ERROR,"Wait net data time out");
		return RESULT_FAILED;
	}

	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_MOD_USER_OTHER_ACK)
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


int CGlobalBusi::ModiOtherUserStatus(const tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser * pUserStatus,uint8 & count,tagGSRV_MODIFY_ONLINE_STATUS_ACK::tagUser * pUStatusNow/* = NULL*/)
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
	}

	int len = sizeof(GSRV_MODIFY_ONLINE_STATUS_QUERY) + sizeof(lpReq->user) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_MOD_USERSTATUS_OTHER_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_CONNECT);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	//::ResetEvent(m_hEvent);
	m_hEvent.ResetEvent();
	m_pBusiControl->SendMultiRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::ModiUserStatus",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_MOD_USERSTATUS_OTHER_ACK)
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

int CGlobalBusi::ModiOtherUserFlow(const tagGSRV_MODIFY_USER_FLOW_QUERY::tagFlow * pUserFlow, uint8 & count, tagGSRV_MODIFY_USER_FLOW_ACK::tagUserFlow * pUserFlowNow/* = NULL*/)
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
	}

	int len = sizeof(GSRV_MODIFY_USER_FLOW_QUERY) + sizeof(lpReq->user) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_MOD_USERFLOW_OTHER_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_CONNECT);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	//::ResetEvent(m_hEvent);
	m_hEvent.ResetEvent();
	m_pBusiControl->SendMultiRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::ModiUserStatus",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_MOD_USERFLOW_OTHER_ACK)
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

int CGlobalBusi::DelOtherUserInfo(const uint32 * auid,uint8 & count,uint32 * aOKUserID /* = NULL*/)
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
	}

	int len = sizeof(GSRV_DEL_ONLINE_USER_QUERY) + sizeof(lpReq->userid) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_DEL_USER_OTHER_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_CONNECT);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	::ResetEvent(m_hEvent);
	m_pBusiControl->SendMultiRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::DelUserInfo",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_DEL_USER_OTHER_ACK)
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
int CGlobalBusi::AddOtherFamilyInfo(const tagGSRV_FAMILYINFO_ADD_QUERY::tagFamilyInfo * pFamilyInfo,uint8 & count,uint32 * pFamilyID/* = NULL*/,uint8 type /*= FAMILYINFO_QUERY_TYPE_PERM*/)
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
		lpReq->FamilyInfo[i].FamilyID = pFamilyInfo[i].FamilyID;
		lpReq->FamilyInfo[i].ServerIp = pFamilyInfo[i].ServerIp;
	}

	int len = sizeof(GSRV_FAMILYINFO_ADD_QUERY) + sizeof(lpReq->FamilyInfo) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_ADD_FAMILY_OTHER_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_CONNECT);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	::ResetEvent(m_hEvent);
	m_pBusiControl->SendMultiRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::AddFamilyInfo",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_ADD_FAMILY_OTHER_ACK)
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
int CGlobalBusi::DelOtherFamilyInfo(const uint32 * pDelFamilyID,uint8 & count,uint32 * pOKFamilyID/* = NULL*/,uint8 type/* = FAMILYINFO_QUERY_TYPE_PERM*/)
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
		lpReq->FamilyID[i] = pDelFamilyID[i];
	}

	int len = sizeof(GSRV_FAMILYINFO_DEL_QUERY) + sizeof(lpReq->FamilyID) * (count - 1);
	m_pBusiControl->InitHead(lpSendHead,CMDGSRV_DEL_FAMILY_OTHER_QUERY,len,XT_HEAD::DEST_TYPE_SERVER,SERVERID_CONNECT);

	//Send Data
	m_sendData.data_len = sizeof(XT_HEAD) + lpSendHead->len;
	m_hEvent.ResetEvent();
	m_pBusiControl->SendMultiRequestData(m_sendData);

	//wait result
	DWORD dwRet = WaitForSingleObject(m_hEvent,WAITTIMEOUT);
	if(dwRet != WAIT_OBJECT_0)
	{
		CLog::Log("CGlobalBusi::DelFamilyInfo",CLog::TYPE_ERROR,"Wait net data time out");
		return 1;
	}
	
	//get data
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(m_pRecvData->data);
	if(lpRecvHead->cmd != CMDGSRV_DEL_FAMILY_OTHER_ACK)
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

#endif


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
#ifdef IM_USE_CACHE
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
#ifdef IM_USE_CACHE
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
#ifdef IM_USE_CACHE
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
		}
	}
	return 0;
}


int CGlobalBusi::AddModiUserInfo(CUser* pUser,uint8 action,uint8 flag)
{
	assert(pUser);
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
#ifdef IM_USE_CACHE
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
#ifdef IM_USE_CACHE
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
#ifdef IM_USE_CACHE
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
#ifdef IM_USE_CACHE
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
				CLog::Log("CGlobalBusi::FindFamilyInfo",CLog::TYPE_IMPORTANT,"QueryUserInfo empty");
				CLog::Log("CGlobalBusi::FindFamilyInfo",CLog::TYPE_IMPORTANT,"pFamilyInfo.fid:%u, srvip:%u",
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


#ifdef IM_USE_CACHE



ImMemCache::ImMemCache()  //用于底层memcached的管理，断链，恢复
{

}


ImMemCache::~ImMemCache()
{
	if(Links != NULL)
		delete Links;
}


ImMemCache& ImMemCache::GetInstance()
{
	static ImMemCache SingleInstance;
	return SingleInstance;
}

int ImMemCache::CheckAndSetInitialized()	//集群初始化判断
{
	//ImMemCache &Memcache = GetInstance();
	//global_Cache *c_memc = Memcache.GetLinks();
	time_t tm;
	time(&tm);
	char CheckInfo[64] = {0};
	char CheckRes[32] = {0};
	sprintf(CheckInfo,"ImServerInitialized#%u",GetServerMagic());
	bool ret = Links->get(CheckInfo,CheckRes);
	if(ret != true)
	{
		CLog::Log("ImMemCache", CLog::TYPE_ERROR, 
			"CheckAndSetInitialized failed");	
		return -1;
	}
	else
	{
		if(CheckRes[0] == 0)
		{			
			sprintf(CheckInfo,"ImServerInitialized#%u",GetServerMagic());
			sprintf(CheckRes,"DY-HAS-ALREADY-INITIALIZED");
			Links->set(CheckInfo,CheckRes);
			return 1;	//需要初始化
		}
		else
		{
			return 0;	//已经初始化
		}
	}	
}

int ImMemCache::CheckAndSetInitializedThreads()	//线程内部初始化判断
{
	//ImMemCache &Memcache = GetInstance();
	//global_Cache *c_memc = Memcache.GetLinks();
	time_t tm;
	time(&tm);
	char CheckInfo[64] = {0};
	char CheckRes[32] = {0};
	sprintf(CheckInfo,"ImServerInitThreads#%u#%u",GetServerMagic(),GetServerId());
	bool ret = Links->get(CheckInfo,CheckRes);
	if(ret != true)
	{
		CLog::Log("ImMemCache", CLog::TYPE_ERROR, 
			"CheckAndSetInitializedThreads failed");	
		return -1;
	}
	else
	{
		if(CheckRes[0] == 0)
		{			
			sprintf(CheckRes,"DY-HAS-ALREADY-INITIALIZED");
			Links->set(CheckInfo,CheckRes);
			Links->expire(CheckInfo,600);
			return 1;	//需要初始化
		}
		else
		{
			return 0;	//已经初始化
		}
	}	
}

int ImMemCache::CheckSuccess()
{
	//ImMemCache &Memcache = GetInstance();
	//global_Cache *c_memc = Memcache.GetLinks();
	time_t tm;
	time(&tm);
	char CheckInfo[32] = {0};
	sprintf(CheckInfo,"%u",(uint32)tm);
	bool ret = Links->set("ImServerConnCheck",CheckInfo);
//	memcached_return_t args;

//	Links->error(args);
	if(ret != true)
	{
		CLog::Log("CMemCBusi-CheckSuccess", CLog::TYPE_ERROR, 
			"failed");
		
		SetLinkState(false);
		return -1;
	}
	else
	{		
		SetLinkState(true);
		return 0;
	}
	
	
}

int ImMemCache::init_ImMemcached(char *ipaddr,int port)
{
	Links = new global_Cache(ipaddr,port);
	
	if(CheckSuccess() < 0)  //check conncect success or not
	{
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			"Memcached connected %s : %d failed",
			ipaddr,port);
		return -1;
	}	
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_DEBUG, 
			"Memcached connected %s : %d successfully",
			ipaddr,port);	
	
	if(1 == CheckAndSetInitialized())
	{
		TotalServerInit();
		
	}
	if(1 == CheckAndSetInitializedThreads())
	{
		InitUsers();
//		TotalUserInit();
//		AddServerInfo();	
	}
		CLog::Log("ImMemCache", CLog::TYPE_DEBUG, 
		"init_ImMemcached initialized successfully",
		ipaddr,port);
		

	return 0;

}




int ImMemCache::init_ImMemcached_WebNotify(char *ipaddr,int port)
{
	Links = new global_Cache(ipaddr,port);
	
	if(CheckSuccess() < 0)  //check conncect success or not
	{
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			"Memcached connected %s : %d failed",
			ipaddr,port);
	return -1;
	}	
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_DEBUG, 
			"Memcached connected %s : %d successfully",
			ipaddr,port);	
	return 0;
}


int ImMemCache::Reinit_ImMemcached()
{
	TotalServerInit();
//	TotalUserInit();
//	AddServerInfo();
	return 0;
}


int ImMemCache::releaseLink(global_Cache* Olinks)
{
	delete Olinks;
	Olinks = NULL;
	return 0;
}

bool ImMemCache::TotalServerInit() //集群初始化一次
{
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = Memcache.GetLinks();
	char total_server[64] = {0};
	sprintf(total_server,"totalserver#%u",GetServerMagic());
	string return_value;
	char zero[] = {"0"};
	if(Links->get(total_server,return_value) == false)
	{
		if(Links->set(total_server,zero))
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

bool ImMemCache::InitUsers() //集群初始化一次
{
	char str_1[64] = {0};
	char str_a[64] = {0};	
	
	uint32 ServIp = (uint32)m_ConfigInfo.GetServerNo();
	sprintf(str_1,"useronline#%u#%u",GetServerMagic(),ServIp);
	Links->remove(str_1);		
	sprintf(str_a,"CServiceList#%u#%u",GetServerMagic(),ServIp);
	Links->remove(str_a);
	#if 0
	char str_b[64] = {0};
	for(uint32 i = 0 ; i < 500; i++)	
	{
		sprintf(str_b,"CSQueue#%u#%u",GetServerMagic(),i);		
		Links->remove(str_b);	
	}
	#endif
	return true;
}



bool ImMemCache::TotalUserInit() //服务器初始化一次
{
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = Memcache.GetLinks();
	char total_user[64] = {0};
	sprintf(total_user,"totaluser#%u#%u",GetServerMagic(),GetServerId());
	char zero[] = {"0"};
	return Links->set(total_user,zero);
	if(Links->set(total_user,zero))
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

bool ImMemCache::AddServerInfo() //服务器编号从1开始
{
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = Memcache.GetLinks();
	uint64_t value = 0;
	char serverid_request[128] = {0};
	char serverindex_set[128] = {0};
	char serverindex_value_set[128] = {0};
	char serverid_request_2[128] = {0};
	char serverindex_value_set_2[128] = {0};
	char check[128] = {0};
	int server_info = GetServerId();
//查询是否存在server_id
	sprintf(serverid_request,"serverid#%u#%u",GetServerMagic(),server_info);
	if(true == Links->get(serverid_request,check))
	{
		//cout << "already has such server id\r\n" << endl;
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %u: already has such server id %u",__FUNCTION__,__LINE__,server_info);		
		return false;
	}
	
	
//增加totalserver	并get其最新值
	char total_server[64] = {0};
	sprintf(total_server,"totalserver#%u",GetServerMagic());

	if(false == Links->increment(string(total_server),1,&value))
	{
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: increment totalserver fail",__FUNCTION__,__LINE__);
		return false;
	}
//增加serverindex 索引
	sprintf(serverindex_set,"serverindex#%u#%lu",GetServerMagic(),value);
	sprintf(serverindex_value_set,"%u",server_info);
	if(false == Links->set(string(serverindex_set),serverindex_value_set))
	{
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: set serverindex_value_set fail %s",__FUNCTION__,__LINE__,serverindex_set);
		return false;

	}
//增加serverid 索引	
	sprintf(serverid_request_2,"serverid#%u#%u",GetServerMagic(),server_info);
	sprintf(serverindex_value_set_2,"%lu",value);
	if(false == Links->set(string(serverid_request_2),serverindex_value_set_2))
	{
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d:set serverid_request_2 fail %s",__FUNCTION__,__LINE__,serverid_request_2);
		return false;
	}
	//cout << "inc: value is " << value << endl;
	return true;
}
/*给memcached使用的，redis未用*/
bool ImMemCache::RecoverUsers(CUser* pUser)
{ 
	uint64_t value = 1;
	char str_a[128] = {0};
	char str_b[128] = {0};
	char str_1[128] = {0};
	char str_2[128] = {0};
	char str_3[128] = {0};
	string check;
	unsigned int user_index = 0;
	//ImMemCache &Memcache =	ImMemCache::GetInstance();
	//global_Cache *c_memc = Memcache.GetLinks();
	CLog::Log("RecoverUsers", CLog::TYPE_IMPORTANT, 
			" ADD USER %u",pUser->m_nID);
	//查询是否存在userinfo
	sprintf(str_a,"userinfo#%u#%u",GetServerMagic(),pUser->m_nID);
	//cout << "check " << str_a << endl;
	Links->get(string(str_a),check);

	if(check.empty())//add info
	{
		////cout << "no such user id\r\n" << endl;
			//增加totaluser 并get其最新值
			
		sprintf(str_b,"totaluser#%u#%u",GetServerMagic(),GetServerId()/*serverid  ---  test only*/);
		
		if( false == Links->increment(string(str_b),1,&value))
		{
			CLog::Log("RecoverUsers", CLog::TYPE_ERROR, 
				" %s %d: increment %s fail",__FUNCTION__,__LINE__,str_b);
			return false;

		}
		//cout << "totaluser#" <<  GetServerId() << "now :" << value << endl;

		//增加useronline表 索引---value中只有id值
		sprintf(str_1,"useronline#%u#%u#%lu",GetServerMagic(),GetServerId(),value);
		sprintf(str_2,"%u",pUser->m_nID);		
		if( false == Links->set(string(str_1),str_2))
		{
		CLog::Log("RecoverUsers", CLog::TYPE_ERROR, 
			" %s %d: set %s fail",__FUNCTION__,__LINE__,str_1);
		return false;
		}
	}
	else //modified info
	{
			/*需要从check 取出 index值*/	

		Json::Reader jReader(Json::Features::strictMode());
		Json::Value jvUser;
		
		if((check[0] == '{') && (jReader.parse(check, jvUser)))
		{
			user_index = jvUser["onlineindex"].asUInt();
		}
		else
		{
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: add_user_info json error %s",__FUNCTION__,__LINE__,check.c_str());	
//			m_memcached_lock.Unlock();
			return false;
		}
	}
		
	//增加userinfo 索引 
	sprintf(str_3,"userinfo#%u#%u",GetServerMagic(),pUser->m_nID);

	/*add user context here*/
#if 1
	//value为json串
	Json::Value jvUser;
	Json::FastWriter jFastWriter;
	string strJvUser;
	jvUser["status"] = pUser->m_nStatus;
	jvUser["rule"] = pUser->m_AllowRule;
	jvUser["username"] = pUser->m_szUserName;
	jvUser["nickname"] = pUser->m_szNickName;
	jvUser["clientip"] = pUser->m_nIP;
	jvUser["clientport"] = pUser->m_nPort;
	jvUser["sexy"] = pUser->m_nSexy;
	jvUser["serverip"] = pUser->m_nServerIp;
	jvUser["lanip"] = pUser->m_nLanIP;
	jvUser["sessionkey"] = Base64Encode(pUser->m_SessionKey, MAX_SESSIONKEY_LEN).c_str();
	jvUser["p2psessionkey"] = Base64Encode(pUser->m_P2PSessionKey, MAX_SESSIONKEY_LEN).c_str();
	jvUser["flag"] = pUser->flag;
	jvUser["csTeamID"] = pUser->m_nCSTeamid;

	if(check.empty()) //new user
	{
		jvUser["onlineindex"] = (unsigned int)value;
		jvUser["flow"] = 0;
		//cout << "[add] index: " << value << " userid : " << globalUserInfo.id << endl;
	}
	else
	{
		jvUser["onlineindex"] = user_index;
		jvUser["flow"] = pUser->m_nflow;
		//cout << "[modified] index: " << user_index << " userid : " << globalUserInfo.id << endl;
	}	
	strJvUser = jFastWriter.write(jvUser);
#endif
	
	/*end add user context */
	if( false == Links->set(string(str_3),strJvUser.c_str()))
	{
		CLog::Log("RecoverUsers", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,str_3);
		return false;
	}
	//cout << "set : " << str_3 << " value :" << strJvUser << endl;
	////cout << "inc: value is " << value << endl;
	return true;
}


CMemCBusi::CMemCBusi()
{
	MemCInit();
}

CMemCBusi::CMemCBusi(ImMemCache *Memcache)
{		
	MemCInit();
	c_memc = Memcache->getLinks();
	c_Memcache = Memcache;
}

CMemCBusi::~CMemCBusi()
{
	//DelServerInfo(GetServerId());
	//memcache_has_init = false;
	//MemCFree();
	//cout << "we have quited MemCInit" << endl;
	
}
void CMemCBusi::MemCInit()
{

}
void CMemCBusi::MemCFree()
{
	//delete c_memc;
}

void CMemCBusi::MemcachedTryLock(int type)
{
	return;
//	ImMemCache &Memcache =  ImMemCache::GetInstance();
//	global_Cache *c_memc = Memcache.GetLinks();
	char key[128];
//	vector<char> locker;
//	char locker_each =(char)(type + '0');
//	locker.push_back(locker_each);
	sprintf(key, "PthreadLock#%u#%d",GetServerMagic(),type);
	
	if(c_Memcache->GetLinkState() == true) //和memcached正常连接状态
	{
	#if 0
		while(c_memc->add(key,key) != true)
		{			
			CLog::Log("CMemCBusi-MemcachedTryLock", CLog::TYPE_DEBUG, "lock key %s",key);
			sched_yield();
		}
	#endif
	    time_t tm;
        time(&tm);
		char timestamp_s[32] = {0};
        uint32 now = (uint32)tm;
		bool lock = false;
		uint32 timestamp = 0;
//		uint32 lock_timeout = now + global_Cache::REDIS_TIMEOUT + 1;
		while(lock == false)
		{
			timestamp = now + global_Cache::REDIS_TIMEOUT + 1;
			sprintf(timestamp_s,"%u",timestamp);
			lock = c_memc->setnx(key,timestamp_s);
			char getVal[32] = {0};		
			if(lock == true)
			{
				break;			
			}
			else if(c_memc->get(key,getVal))
			{
				uint32 lockTime = (uint32)atoi(getVal);
				if(lockTime != 0 && now > lockTime)
				{			
					char lock_timeout_s[32] = {0};
					char old_lock_timeout_s[32] = {0};
					sprintf(lock_timeout_s,"%u",timestamp);
					if(c_memc->getset(key,lock_timeout_s,old_lock_timeout_s))
					{
						uint32 old_lock_timeout = (uint32)atoi(old_lock_timeout_s);
						if(old_lock_timeout != 0 && now > old_lock_timeout)
						{
							break;
						}
					}
				}
			}
			else
			{
				CLog::Log("CMemCBusi-MemcachedTryLock", CLog::TYPE_DEBUG, "lock key %s",key);
				sched_yield();			
			}
		}
	}
}
void CMemCBusi::MemcachedUnLock(int type)
{
	return;
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	char key[128];	
	string str_key;
	sprintf(key, "PthreadLock#%u#%d",GetServerMagic(), type);
	str_key = key;
	if(c_Memcache->GetLinkState() == true) //和memcached正常连接状态
	{
	#if 0
		while(c_memc->remove(str_key) != true)
		{			
			CLog::Log("CMemCBusi-MemcachedUnLock", CLog::TYPE_DEBUG, "unlock key %s",key);
			sched_yield();
		}
	#endif
		time_t tm;
		time(&tm);
		
		uint32 now = (uint32)tm;
		char getVal[32] = {0};
		if(c_memc->get(key,getVal))
		{
			uint32 timestamp = (uint32) atoi(getVal);
			if(timestamp != 0 && now < timestamp )
			{
				c_memc->remove(str_key);
			}
		}

	
	
	}
	

	
}




bool CMemCBusi::AddModiFamilyInfo(const tagGSRV_FAMILYINFO_ADD_QUERY::tagFamilyInfo pFamilyInfo,uint8 type)
{
	//key
	char key[128];
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();	
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
int CMemCBusi::QueryFamilyInfo(uint32 nFamilyId, tagGSRV_FAMILYINFO_QUERY_ACK::tagFamilyInfo& pFamilyInfo,uint8 type)
{
	memset(&pFamilyInfo, 0, sizeof(pFamilyInfo));
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();

	char key[128];
	string strMemGet;
	sprintf(key, "familyinfo#%u#%u#%u",GetServerMagic(),type, nFamilyId);
	
	m_MemRc = c_memc->get(key,strMemGet);

	if(m_MemRc != true || strMemGet.empty())
	{
		CLog::Log("CMemCBusi::QueryFamilyInfo", CLog::TYPE_DEBUG, "NO family %u",nFamilyId);
		return  MEMCACHED_NO_INFO;
	}

	//CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryFamilyInfo %s", strMemGet.c_str());

	Json::Reader jReader(Json::Features::strictMode());
	Json::Value jvUser;
	
	if((strMemGet[0]=='{') && (jReader.parse(strMemGet, jvUser)))
	{		
		pFamilyInfo.fid = nFamilyId;
		pFamilyInfo.srvip = jvUser["ServerIp"].asUInt();	
	}
	else
	{
		CLog::Log("CMemCBusi-QueryFamilyInfo", CLog::TYPE_ERROR, "QueryUserInfo json error %s",strMemGet.c_str());
		return MEMCACHED_JASON_ERROR;
	}
	return MEMCACHED_GET_INFO_SUCCESS;
}


//删除家族信息
bool CMemCBusi::DelFamilyInfo(uint32 nFamilyId,uint8 type)
{
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	
	char key[128];
	sprintf(key, "familyinfo#%u#%u#%u",GetServerMagic(),type, nFamilyId);
	m_MemRc = c_memc->remove(key);
	//CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "DelFamilyInfo %u %d", nFamilyId, m_MemRc);
	return (m_MemRc==true);

	
}
bool CMemCBusi::totalserver_init()
{
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
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
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
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
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	uint64_t value = 0;
	char serverid_request[128] = {0};
	char serverindex_set[128] = {0};
	char serverindex_value_set[128] = {0};
	char serverid_request_2[128] = {0};
	char serverindex_value_set_2[128] = {0};
	char check[128] = {0};
	int server_info = GetServerId();
	//m_memcached_lock.Lock();
	m_MemRc = true;
//查询是否存在server_id
	sprintf(serverid_request,"serverid#%u#%u",GetServerMagic(),server_info);
	if(true == c_memc->get(serverid_request,check))
	{
		//cout << "already has such server id\r\n" << endl;
		//m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %u: already has such server id %u",__FUNCTION__,__LINE__,server_info);		
		return false;
	}
	
	
//增加totalserver	并get其最新值
	char total_server[64] = {0};
	sprintf(total_server,"totalserver#%u",GetServerMagic());

	if(false == c_memc->increment(string(total_server),1,&value))
	{
		//m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: increment totalserver fail",__FUNCTION__,__LINE__);
		return false;
	}
//增加serverindex 索引
	sprintf(serverindex_set,"serverindex#%u#%lu",GetServerMagic(),value);
	sprintf(serverindex_value_set,"%u",server_info);
	if(false == c_memc->set(string(serverindex_set),serverindex_value_set))
	{
		//m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: set serverindex_value_set fail %s",__FUNCTION__,__LINE__,serverindex_set);
		return false;

	}
//增加serverid 索引	
	sprintf(serverid_request_2,"serverid#%u#%u",GetServerMagic(),server_info);
	sprintf(serverindex_value_set_2,"%lu",value);
	if(false == c_memc->set(string(serverid_request_2),serverindex_value_set_2))
	{
		//m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d:set serverid_request_2 fail %s",__FUNCTION__,__LINE__,serverid_request_2);
		return false;
	}
	//cout << "inc: value is " << value << endl;
	//m_memcached_lock.Unlock();
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
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	//m_memcached_lock.Lock();
//获取totalserver的值
	char total_server_key[64] = {0};
	sprintf(total_server_key,"totalserver#%u",GetServerMagic());
	if(false == c_memc->get(string(total_server_key),total_server))
	{		
		//m_memcached_lock.Unlock();
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
		//m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: get %s fail",__FUNCTION__,__LINE__,str_1);
		return false;
		//cout << " no such sever id" << endl;
	}
	if(false == c_memc->remove(string(str_1)))
	{		
		//m_memcached_lock.Unlock();
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
				//m_memcached_lock.Unlock();
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
					" %s %d: get %s fail",__FUNCTION__,__LINE__,str_3);
				return false;
			}
			string set_insert(insert_server_id.begin(),insert_server_id.end());
	
		//insert
			if( false == c_memc->set(string(str_2),insert_server_id))
			{
				//m_memcached_lock.Unlock();
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
					" %s %d: set %s fail",__FUNCTION__,__LINE__,str_2);
				return false;				
			}

		//del max
			if(false == c_memc->remove(string(str_3)))
			{
				//m_memcached_lock.Unlock();
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
				//m_memcached_lock.Unlock();
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
					" %s %d: set %s fail",__FUNCTION__,__LINE__,set_str);
				return false;				
			}

	}
	else
	{
		if( false == c_memc->remove(string(str_2)))
		{
			//m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: remove %s fail",__FUNCTION__,__LINE__,str_2);
			return false;
		}

	}
//减少totalserver	并get其最新值	
	
	if( false == c_memc->decrement(total_server_key,1,&value))
	{
		//m_memcached_lock.Unlock();
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: decrement totalserver fail",__FUNCTION__,__LINE__);
		return false;
	}

	//m_memcached_lock.Unlock();
	return true;
}

bool CMemCBusi::QueryServerInfo(vector<int> &server_info) //请求服务器信息以及个数（server_info.length）
{
	list < uint32 > serverList;
	m_ConfigInfo.GetServerList(&serverList);

	server_info.clear();
	for(list < uint32 >::iterator it = serverList.begin();it != serverList.end();it++)
	{
		if(*it == 4294967295)
		{
			continue;
		}
		server_info.push_back((int)*it);
	}
	return true;
}


#ifdef IM_USE_REDIS


/*
memcached阶段使用

	用户信息	 		userinfo#%u#%u GetServerMagic(),id
	用户总数	 		totaluser#%u#%u",GetServerMagic(),ServIp
	在线	用户索引	useronline#%u#%u#%lu GetServerMagic(),ServIp,index

修改为
	
	用户信息	 		userinfo#%u#%u GetServerMagic(),id						hashkey
	在线	用户索引	useronline#%u#%u GetServerMagic(),ServIp			list
	
*/

/*
	需要同步到redis的数据如下
	
	uint32	m_nID;						//用户ID
	// Normal= 0x00; Hide= 0x02; Offline= 0x03;  Leave= 0x01;
	uint8 	m_nflow;			//用户分流方式
	uint8   m_nStatus ;                  //用户状态
	uint16	m_ClientVersion;			//用户使用的客户端版本
	uint8	m_nSexy;					//用户性别0-女，1-男，2-未知
	char	m_szHeadURL[MAX_HEADURL_LEN+1];		//自定义头像URL
	char	m_szNickName[MAX_NICKNAME_LEN+1];	//呢称
	char	m_szUserName[MAX_USERNAME_LEN+1];	//用户名
	bool	m_bIsFullInfoValid;
	//详细资料
	char	m_szEmail[MAX_EMAIL_LEN+1];				//Email地址
	char	m_szBirthday[MAX_BIRTHDAY_LEN+1];		//生日
	uint8	m_nCountry;								//国家ID
	uint8	m_nProvince;							//省份ID
	uint8	m_nCity;								//城市ID
	uint8	m_nCareer;								//职业ID
	uint8	m_AllowRule;							//加好友认证方式
	uint8	m_FamilyFlag;							//列为家族成员认证方式
	uint32	m_nMoney;								//用户虚拟货币值
//	uint8	m_nLevel;								//用户等级
	uint32	m_nOnlineTime;							//用户累计在线时长
	uint8	m_linkpop;								//联系方式隐私属性（0 私有;1 好友可见；3 公开）
	char	m_szAddress[MAX_ADDRESS_LEN+1];			//地址
	char	m_szPhone[MAX_PHONE_LEN+1];				//通讯号码
	char	m_szDescription[MAX_DESCRIPTION_LEN+1];	//个人说明
	char	m_szUserSign[MAX_USERSIGN_LEN + 1];		//个人签名	
	char	m_SessionKey[MAX_SESSIONKEY_LEN];		//Session Key
	char	m_P2PSessionKey[MAX_SESSIONKEY_LEN];	//P2P Session Key

	char	m_szAvatoruri[MAX_AVATOR_LEN + 1];		//闪秀地址
	uint8   m_nMobileStatus;						//绑定手机  0未绑定，1已绑定 2已绑定并且订制了业务
	uint32	m_nIntegral;							//用户总积分
	char 	m_szGUID[MAX_GUID_LEN + 1];				//GUID
	
	//最后心跳时间

	uint32  m_nNewlyLiveTime;
	uint32  m_nLoginTime;
	uint32  m_nLoginType; //参考用户登录请求 flag
	uint32	m_nIP;
	uint16	m_nPort;

	uint32	m_nLanIP;
	uint16	m_nLanPort;
	uint32  m_nServerIp; 			//所在服务器IP地址
	uint8 	flag;					 //登录方式
	uint8   m_nLevel;				//用户等级
	uint8	m_nUsertype;				//买家   卖家  还是客服
	uint8	m_nQueueflag;				//是否排队 0:不排队 1:排队中
	uint32	m_nCSuserid;				//作为买家时接待客服的id
	uint32	m_nCSTeamid;				//作为买家时接待客服的组
	uint32  m_merchantid;

*/



bool CMemCBusi::ModiUserIpPortInfo(const uint32 auid,uint32 ip,uint16 port)
{
	//key
	char key[128];
	sprintf(key, "userinfo#%u#%u",GetServerMagic(), auid);
	MemcachedTryLock(MEMCACHED_USER_LOCK);

	if( false == c_memc->redisClusterCommand_set(
		"hmset %s %s %u %s %d",
		key,
		"clientip",		ip,
		"clientport",	port		
		))
	{			
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("ModiUserIpPortInfo", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,key);
		return false;
	}	
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;
}


bool CMemCBusi::AddModiUserInfo(CUser* pUser,uint8 action,uint8 flag)
{
	//uint64_t value = 1;
	char str_a[128] = {0};
//	char str_b[128] = {0};
	char str_1[128] = {0};
	char str_2[128] = {0};
	char str_3[128] = {0};
	char check[1][1024] = {0};
//	unsigned int user_index = 0;
	m_MemRc = true; 
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, 
			" ADD USER %u",pUser->m_nID);
	//m_memcached_lock.Lock();
	
	uint32 ServIp = (uint32)m_ConfigInfo.GetServerNo();
	MemcachedTryLock(MEMCACHED_USER_LOCK);
	//查询是否存在userinfo
	sprintf(str_a,"userinfo#%u#%u",GetServerMagic(),pUser->m_nID);
	c_memc->redisClusterCommand_get(check,"hmget %s %u",str_a,pUser->m_nID);
	if(check[0][0] == 0)//add info
	{
		//增加useronline表 索引---value中只有id值
		sprintf(str_1,"useronline#%u#%u",GetServerMagic(),ServIp);
		sprintf(str_2,"%u",pUser->m_nID);	

		if( false == c_memc->zadd(str_1,str_2))
		{					
			MemcachedUnLock(MEMCACHED_USER_LOCK);
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
				" %s %d: lpush %s fail",__FUNCTION__,__LINE__,str_1);
			return false;
		}	
	}		
	//增加userinfo 索引 
	char utf8_username[MAX_USERNAME_LEN*3+1] = {0};
	char utf8_nickname[MAX_NICKNAME_LEN*3+1] = {0};
	char utf8_groupname[MAX_WORKGROUP_LEN*3+1] = {0};	
	char utf8_usersign[MAX_USERSIGN_LEN*3+1] = {0};
	gbk2utf8(utf8_username,pUser->m_szUserName);
	gbk2utf8(utf8_nickname,pUser->m_szNickName);
	gbk2utf8(utf8_groupname,pUser->m_nWorkGroup);	
	gbk2utf8(utf8_usersign,pUser->m_szUserSign);

	CLog::Log("CMemCBusi::AddModiUserInfo", CLog::TYPE_DEBUG,
				"修改[%u]个性签名为%s",pUser->m_nID,utf8_usersign);
	
	sprintf(str_3,"userinfo#%u#%u",GetServerMagic(),pUser->m_nID);
	if( false == c_memc->redisClusterCommand_set(
		"hmset %s \
%s %u \
%s %u \
%s %s \
%s %s \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %s \
%s %s \
%s %u \
%s %u \
%s %u \
%s %s \
%s %u \
%s %s \
%s %s \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %s \
%s %s \
%s %s \
%s %u \
%s %u \
%s %s \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %s \
%s %u \
%s %u \
%s %u \
%s %u",		 
		str_3,
		"status",		pUser->m_nStatus,
		"rule",			pUser->m_AllowRule,
		"username",		utf8_username,
		"nickname",		utf8_nickname,
		"clientip",		pUser->m_nIP,
		"clientport",	pUser->m_nPort,
		"sexy",			pUser->m_nSexy,
		"serverip", 	pUser->m_nServerIp,
		"lanip", 		pUser->m_nLanIP,	
		"flag" ,		pUser->flag,		
		"logintime", 	pUser->m_nLoginTime,		
		"sessionkey" ,	Base64Encode(pUser->m_SessionKey, MAX_SESSIONKEY_LEN).c_str(),
		"p2psessionkey",Base64Encode(pUser->m_P2PSessionKey, MAX_SESSIONKEY_LEN).c_str(),
		"flow" ,		pUser->m_nflow,
		"id",			pUser->m_nID,		
		"version",pUser->m_ClientVersion,
		"headURL",pUser->m_szHeadURL,
		"isFullInfoValid",pUser->m_bIsFullInfoValid,
		"eMail",pUser->m_szEmail,
		"birthday",pUser->m_szBirthday,
		"country",pUser->m_nCountry,
		"province",pUser->m_nProvince,
		"city",pUser->m_nCity,
		 "career",pUser->m_nCareer,
		 "familyFlag",pUser->m_FamilyFlag,
		 "money",pUser->m_nMoney,
		 "onlineTime",pUser->m_nOnlineTime,
		 "linkpop",pUser->m_linkpop,
		 "address",pUser->m_szAddress,
		 "phone",pUser->m_szPhone,
		 "description",pUser->m_szDescription,		 
		 "userSign",utf8_usersign,                
		"mobileStatus",pUser->m_nMobileStatus,    		
		"integral",pUser->m_nIntegral,								
		"u_GUID",pUser->m_szGUID,											
		"newlyLiveTime",pUser->m_nNewlyLiveTime,      
		"loginType",pUser->m_nLoginType,							
		"lanPort",pUser->m_nPort,											
		"level",pUser->m_nLevel,											
		"usertype",pUser->m_nUsertype,
		"csUserID",pUser->m_nCSuserid,
		"csTeamID",pUser->m_nCSTeamid,
		 "receptionLimit",pUser->m_nCSRecptionLimit,
		 "groupname",utf8_groupname,
		 "isMornitored",pUser->m_bMonitored,
		 "chattingBuyerID",pUser->m_nChattingBuyerID,
		 "directorID",pUser->m_nDirectorID,
		 "merchantID",pUser->m_merchantid
		))
	{
		//m_memcached_lock.Unlock();
			
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,str_3);
		return false;
	}

	if(pUser->m_nID < Id_Boundary || pUser->m_nID > cs_Boundary)
	{
		c_memc->expire(str_3,seller_Timeout);
	}
	else
	{		
		c_memc->expire(str_3,buyer_First_Timeout);		
	}


	
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;
}

#ifdef IM_USE_MULTIDEVICE
bool CMemCBusi::expireDeviceList(uint32 id)
{

	char str_4[128] = {0};

	if(id == 0)
		return false;

	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_DEBUG, 
			" ADD ExpireDeviceList %u",id);
	//m_memcached_lock.Lock();
		MemcachedTryLock(MEMCACHED_USER_LOCK);

	//增加deviceOnlineList#userid		(set)		查找		 (ttl) 包含每个deviceID
	sprintf(str_4,"deviceOnlineList#%u#%u",GetServerMagic(),id);
	
	if(id < Id_Boundary || id > cs_Boundary)
	{
		c_memc->expire(str_4,seller_Timeout);
	}
	else
	{		
		c_memc->expire(str_4,buyer_Timeout);
	}
	
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;
}



bool CMemCBusi::AddModiDeviceHeartInfo(uint32 id,uint32 deviceId,uint32 timestamp)
{
	char str_6[128] = {0};
	if(id == 0)
		return false;
	if(deviceId == 0)
		return false;
	
//	unsigned int user_index = 0;
	m_MemRc = true; 
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, 
			" AddModiDeviceHeartInfo %u %u",id,deviceId);
	//m_memcached_lock.Lock();
		MemcachedTryLock(MEMCACHED_USER_LOCK);

	sprintf(str_6,"userDeviceInfo#%u#%u#%u",GetServerMagic(),id,deviceId);
	if( false == c_memc->redisClusterCommand_set(
		"hmset %s \
%s %u \
%s %u \
%s %u", 	 
			str_6,
			"deviceId",		deviceId,
			"loginId",		id,
			"heartBeat",	timestamp
				))
	{
		//m_memcached_lock.Unlock();
			
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,str_6);
		return false;
	}	

	if(id < Id_Boundary || id > cs_Boundary)
	{
		c_memc->expire(str_6,seller_Timeout);	
	}
	else
	{		
		c_memc->expire(str_6,buyer_Timeout);	
	}
	expireDeviceList(id);
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;
}

bool CMemCBusi::AddModiDeviceIpPortInfo(uint32 id,uint32 deviceId,uint32 ip,uint16 port)
{
	char str_6[128] = {0};
	if(id == 0)
		return false;
	if(deviceId == 0)
		return false;
	
//	unsigned int user_index = 0;
	m_MemRc = true; 
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, 
			" AddModiDeviceIpPortInfo %u %u",id,deviceId);
	//m_memcached_lock.Lock();
		MemcachedTryLock(MEMCACHED_USER_LOCK);

	sprintf(str_6,"userDeviceInfo#%u#%u#%u",GetServerMagic(),id,deviceId);
	if( false == c_memc->redisClusterCommand_set(
		"hmset %s \
%s %u \
%s %u \
%s %u \
%s %u", 	 
			str_6,
			"deviceId",		deviceId,
			"loginId",		id,
			"clientIp", 	ip,
			"clientPort", 	port
				))
	{
		//m_memcached_lock.Unlock();
			
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,str_6);
		return false;
	}	

	if(id < Id_Boundary || id > cs_Boundary)
	{
		c_memc->expire(str_6,seller_Timeout);	
	}
	else
	{		
		c_memc->expire(str_6,buyer_Timeout);	
	}	
	expireDeviceList(id);
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;
}

bool CMemCBusi::AddDeviceUserInfo(const shared_ptr<CUser> &pUser)
{

	char str_4[128] = {0};
	char str_5[128] = {0};
	char str_6[128] = {0};
	if(!pUser)
		return false;
	if(pUser->m_nDeviceId == 0)
		return false;
	
//	unsigned int user_index = 0;
	m_MemRc = true; 
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, 
			" ADD DEVICE %u %u",pUser->m_nID,pUser->m_nDeviceId);
	//m_memcached_lock.Lock();
		MemcachedTryLock(MEMCACHED_USER_LOCK);

	//增加deviceOnlineList#userid       (set)		查找		 (ttl) 包含每个deviceID
	sprintf(str_4,"deviceOnlineList#%u#%u",GetServerMagic(),pUser->m_nID);
	sprintf(str_5,"%u",pUser->m_nDeviceId);	
	if( false == c_memc->zadd(str_4,str_5))
	{					
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: lpush  %s fail",__FUNCTION__,__LINE__,str_4);
		return false;
	}
	sprintf(str_6,"userDeviceInfo#%u#%u#%u",GetServerMagic(),pUser->m_nID,pUser->m_nDeviceId);
	if( false == c_memc->redisClusterCommand_set(
		"hmset %s \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %s \
%s %s", 	 
			str_6,
			"deviceId",		pUser->m_nDeviceId,
			"loginId",		pUser->m_nID,
			"flag",			pUser->flag,
			"heartBeat",	pUser->m_nNewlyLiveTime,
			"loginType",	0,
			"clientIp", 	pUser->m_nIP,
			"clientPort", 	pUser->m_nPort,
			"serverIp",		pUser->m_nServerIp,
			"sessionKey" ,	Base64Encode(pUser->m_SessionKey, MAX_SESSIONKEY_LEN).c_str(),
			"p2pSessionKey",Base64Encode(pUser->m_P2PSessionKey, MAX_SESSIONKEY_LEN).c_str()
				))
	{
		//m_memcached_lock.Unlock();
			
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,str_6);
		return false;
	}	

	if(pUser->m_nID < Id_Boundary || pUser->m_nID > cs_Boundary)
	{
		c_memc->expire(str_6,seller_Timeout);	
		c_memc->expire(str_4,seller_Timeout);
	}
	else
	{		
		c_memc->expire(str_6,buyer_Timeout);	
		c_memc->expire(str_4,buyer_Timeout);
	}
	
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;
}


bool CMemCBusi::GetDeviceUserInfo(uint32 userid,uint32 deviceId,multiDevice_t &deviceInfo)
{
	if(userid == 0 || deviceId == 0)
		return false;
	
	char str_1[128] = {0};
	sprintf(str_1,"userDeviceInfo#%u#%u#%u",GetServerMagic(),userid,deviceId);
	char retval[10][1024] = {0};
	m_MemRc = c_memc->redisClusterCommand_get(retval,"hmget %s %s %s %s %s %s %s %s %s %s %s",
			str_1,
			"loginId",
			"loginType",
			"heartBeat",	
			"clientIp", 	
			"clientPort",
			"flag",
			"sessionKey" ,	
			"p2pSessionKey",
			"deviceId",
			"serverIp"
			);	
	deviceInfo.userId = (uint32)atoi(retval[0]);
	deviceInfo.loginType = (uint32)atoi(retval[1]);
	deviceInfo.heartBeatTime = (uint32)atoi(retval[2]);
	deviceInfo.clientIp = (uint32)atoi(retval[3]);
	deviceInfo.clientPort = (uint16)atoi(retval[4]);	
	deviceInfo.flag = (uint32)atoi(retval[5]);
	string strSessionKey = Base64Decode(string(retval[6]));
	memcpy(deviceInfo.sessionKey, strSessionKey.c_str(), strSessionKey.length()); 
	string strP2PSessionKey = Base64Decode(string(retval[7]));
	memcpy(deviceInfo.p2pSessionKey, strP2PSessionKey.c_str(), strP2PSessionKey.length()); 
	deviceInfo.deviceId = (uint32)atoi(retval[8]);
	deviceInfo.serverip = (uint32)atoi(retval[9]);
	if(m_MemRc)
	{
		CLog::Log("CMemCBusi::GetDeviceUserInfo", CLog::TYPE_DEBUG,
					"get %s,userid %u, device %u,loginType %u, heartBeat %u, clientIp %u,\
clientPort %d, flag %u, sessionKey %s, p2pSessionKey %s serverip %u",
				str_1,deviceInfo.userId,deviceInfo.deviceId,deviceInfo.loginType,
				deviceInfo.heartBeatTime,deviceInfo.clientIp,deviceInfo.clientPort,
				deviceInfo.flag,retval[6],retval[7],deviceInfo.serverip);
	}
	else
	{
		CLog::Log("CMemCBusi::GetDeviceUserInfo", CLog::TYPE_DEBUG,
					"get %s failed",str_1);
	}
	return m_MemRc;
}

bool CMemCBusi::GetAllDeviceUserInfo(uint32 userid,vector<multiDevice_t> &deviceInfo)
{
	if(userid == 0)
		return false;
	
	char str_1[128] = {0};
	char str_2[128] = {0};	
	char deviceId[128][1024] = {0};
	int deviceCount = 127;
	
	sprintf(str_1,"deviceOnlineList#%u#%u",GetServerMagic(),userid);
	
	if( false == c_memc->zrange(str_1,0,-1,deviceCount,deviceId))
	{
		CLog::Log("CMemCBusi::GetAllDeviceUserInfo", CLog::TYPE_ERROR,
				" %s %d: get %s fail",__FUNCTION__,__LINE__,str_1);
		return 	false;
	}
	for(int i = 0 ; i < deviceCount ; i++)
	{

		uint32 id = atoi(deviceId[i]);
		if(id == 0)
			continue;
		CLog::Log("CMemCBusi::GetDeviceUser", CLog::TYPE_DEBUG,
				"user %u devicelist has %u",userid,id);
		sprintf(str_2,"userDeviceInfo#%u#%u#%u",GetServerMagic(),userid,id);

		multiDevice_t deviceOne;
		if(GetDeviceUserInfo(userid,id,deviceOne) && deviceOne.userId != 0)
		{
			deviceInfo.push_back(deviceOne);
		}		
	}	
	return	true;
}
bool CMemCBusi::GetDeviceUserCount(uint32 userid, uint32 &count)
{
	char str_1[128] = {0};
	uint64_t length = 0;
	sprintf(str_1,"deviceOnlineList#%u#%u",GetServerMagic(),userid);
	if( false == c_memc->zcard(str_1,&length))
	{
		CLog::Log("CMemCBusi::GetDeviceUserCount", CLog::TYPE_ERROR,
				" %s %d: get %s fail",__FUNCTION__,__LINE__,str_1);
		count = 0;
		return false;
	}	
	count = (uint32) length;
	return true;
}

bool CMemCBusi::DelDeviceUserInfo(uint32 nUserId,uint32 deviceId)
{
	char str_1[128] = {0};
	char str_2[128] = {0};
	char str_3[128] = {0};

	m_MemRc = true;

	MemcachedTryLock(MEMCACHED_USER_LOCK);

	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, 
			" DELETE Device %u %u",nUserId,deviceId);

	sprintf(str_1,"userDeviceInfo#%u#%u#%u",GetServerMagic(),nUserId,deviceId);
	sprintf(str_2,"%u",deviceId);	
	sprintf(str_3,"deviceOnlineList#%u#%u",GetServerMagic(),nUserId);
	if( false == c_memc->remove(string(str_1)))
	{
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: remove %s fail",__FUNCTION__,__LINE__,str_1);
		return false;
	}
	
	if( false == c_memc->zrem(str_3,str_2))
	{
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: lrem %s fail",__FUNCTION__,__LINE__,str_1);
		return false;
	}	

	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;

}


#endif
// 从redis中获取用户全量信息
int CMemCBusi::GetFUserInfo(uint32 userid, CUser &user)
{
	char key[128] = {0};
	if(c_Memcache->GetLinkState() == true) //和memcached正常连接状态
	{
		sprintf(key, "userinfo#%u#%u", GetServerMagic(),userid);
		string strMemGet;
		char retval[51][1024] = {0};
		m_MemRc = c_memc->redisClusterCommand_get(retval,"hmget %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
		key,
		"status",
		"rule",
		"username",
		"nickname",
		"clientip",
		"clientport",
		"sexy",
		"serverip",
		"lanip", 
		"flag" ,
		"logintime",
		"sessionkey" ,
		"p2psessionkey",
		"flow" ,
		"id",
		"version",
		"headURL",
		"isFullInfoValid",
		"eMail",
		"birthday",
		"country",
		"province",
		"city",
		"career",
		"familyFlag",
		"money",
		"onlineTime",
		"linkpop",
		"address",
		"phone",
		"description",
		"userSign",     
		"mobileStatus",
		"integral",		
		"u_GUID",
		"newlyLiveTime",
		"loginType",
		"lanPort",
		"level",
		"usertype",
		"csUserID",
		"csTeamID",
		"receptionLimit",
		"groupname",
		"isMornitored",
		"chattingBuyerID",
		"directorID",
		"merchantID"
		);		
		

		if(m_MemRc != true || retval[0][0] == 0)	//存储的id信息为0 
		{
			CLog::Log("CMemCBusi::GetFUserInfo", CLog::TYPE_DEBUG, "QueryUserInfo NO user %u",userid);
			return MEMCACHED_NO_INFO;
		}

		user.m_nStatus = (uint8)atoi(retval[0]);
		user.m_AllowRule = (uint8)atoi(retval[1]);
		utf2gbk(retval[2],strlen(retval[2]),user.m_szUserName,MAX_USERNAME_LEN);
		utf2gbk(retval[3],strlen(retval[3]),user.m_szNickName,MAX_NICKNAME_LEN);
		user.m_nIP = (uint32)atoi(retval[4]);
		user.m_nPort = (uint16)atoi(retval[5]);
		user.m_nSexy = (uint8)atoi(retval[6]);
		user.m_nServerIp = (uint32)atoi(retval[7]);
		user.m_nLanIP = (uint32)atoi(retval[8]);
		user.flag = (uint8)atoi(retval[9]);
		user.m_nLoginTime = (uint32)atoi(retval[10]);
		string strSessionKey = Base64Decode(string(retval[11]));
		memcpy(user.m_SessionKey, strSessionKey.c_str(), strSessionKey.length()); 
		string strP2PSessionKey = Base64Decode(string(retval[12]));
		memcpy(user.m_P2PSessionKey, strP2PSessionKey.c_str(), strP2PSessionKey.length()); 
		user.m_nflow = (uint8)atoi(retval[13]);
		user.m_nID = (uint32)atoi(retval[14]);
		strncpy(user.m_szHeadURL, retval[16], MAX_HEADURL_LEN);
		user.m_bIsFullInfoValid = (uint8)atoi(retval[17]);
		strncpy(user.m_szEmail, retval[18], MAX_EMAIL_LEN);
		strncpy(user.m_szBirthday, retval[19], MAX_BIRTHDAY_LEN);
		user.m_nCountry = (uint8)atoi(retval[20]);
		user.m_nProvince = (uint8)atoi(retval[21]);
		user.m_nCity = (uint8)atoi(retval[22]);
		user.m_nCareer = (uint8)atoi(retval[23]);
		user.m_FamilyFlag = (uint8)atoi(retval[24]);
		user.m_nMoney = (uint32)atoi(retval[25]);
		user.m_nOnlineTime = (uint32)atoi(retval[26]);
		user.m_linkpop = (uint8)atoi(retval[27]);
		strncpy(user.m_szAddress, retval[28], MAX_ADDRESS_LEN);
		strncpy(user.m_szPhone, retval[29], MAX_PHONE_LEN);
		strncpy(user.m_szDescription, retval[30], MAX_DESCRIPTION_LEN);
		utf2gbk(retval[31],strlen(retval[31]),user.m_szUserSign,MAX_USERSIGN_LEN);
		user.m_nMobileStatus = (uint8)atoi(retval[32]);
		user.m_nIntegral = (uint32)atoi(retval[33]);
		strncpy(user.m_szGUID, retval[34], MAX_GUID_LEN);
		user.m_nNewlyLiveTime = (uint32)atoi(retval[35]);
		user.m_nLoginType = (uint32)atoi(retval[36]);
		user.m_nLanPort = (uint16)atoi(retval[37]);
		user.m_nLevel = (uint8)atoi(retval[38]);
		user.m_nUsertype = (uint8)atoi(retval[39]);
		user.m_nCSuserid = (uint32)atoi(retval[40]);
		user.m_nCSTeamid = (uint32)atoi(retval[41]);
		user.m_nCSRecptionLimit = (uint32)atoi(retval[42]);
		utf2gbk(retval[43],strlen(retval[43]),user.m_nWorkGroup,MAX_WORKGROUP_LEN);
		user.m_bMonitored = (uint8)atoi(retval[44]);
		user.m_nChattingBuyerID = (uint32)atoi(retval[45]);
		user.m_nDirectorID = (uint32)atoi(retval[46]);
		user.m_merchantid= (uint32)atoi(retval[47]);
#ifdef IM_USE_MULTIDEVICE		
		user.m_nDeviceId = 0;
#endif

		CLog::Log("CMemCBusi::GetFUserInfo", CLog::TYPE_DEBUG,
		"QueryUserInfo--key %s [%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s][%s]",
			key,retval[0],retval[1],user.m_szUserName,user.m_szNickName,retval[4],retval[5],retval[6],retval[7],
			retval[8],retval[9],retval[10], retval[11], retval[12], retval[13], retval[14], retval[15],
			retval[16],retval[17],retval[18],retval[19],retval[20],retval[21],retval[22],retval[23],
			retval[24],retval[25],retval[26], retval[27], retval[28], retval[29], retval[30], user.m_szUserSign,
			retval[32],retval[33],retval[34],retval[35],retval[36],retval[37],retval[38],retval[39],
			retval[40],retval[41],retval[42],user.m_nWorkGroup,retval[44],retval[45],retval[46],retval[47]);
				

		return MEMCACHED_GET_INFO_SUCCESS;
	}
	else
	{		
		CLog::Log("CMemCBusi::GetFUserInfo", CLog::TYPE_ERROR, "MEMCACHED_SERVER_DOWN");
		return MEMCACHED_SERVER_DOWN;
	}
}

// 修改用户的部分信息
bool CMemCBusi::ModiPartUserInfo(uint32 userid, const STRU_PART_USERINFO &struPartUserInfo)
{
	char str_3[128] = {0};
	sprintf(str_3, "userinfo#%u#%u", GetServerMagic(),userid);
	CLog::Log("CMemCBusi::ModiPartUserInfo", CLog::TYPE_IMPORTANT, 
			" MOD USER %u",userid);

	// 修改
	MemcachedTryLock(MEMCACHED_USER_LOCK);
	char utf8_nickname[MAX_NICKNAME_LEN*3+1] = {0};
	gbk2utf8(utf8_nickname,struPartUserInfo.nickname);

	if( false == c_memc->redisClusterCommand_set(
		"hmset %s \
%s %s \
%s %s \
%s %s \
%s %s \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %u \
%s %s \
%s %s \
%s %s",		 
		str_3,
		"nickname",		utf8_nickname,
		"eMail",		struPartUserInfo.email,
		"birthday",		struPartUserInfo.birthday,
		"headURL",		struPartUserInfo.face_url,
		"sexy",			struPartUserInfo.sex,
		"country",		struPartUserInfo.country,
		"province",		struPartUserInfo.province,
		"city",			struPartUserInfo.city,
		"career",		struPartUserInfo.career,
		"familyFlag",	struPartUserInfo.familyflag,
		"linkpop",		struPartUserInfo.linkpop,
		"address",		struPartUserInfo.address,
		"phone",		struPartUserInfo.mobile,
		"description",	struPartUserInfo.description
		))
	{
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi::ModiPartUserInfo", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,str_3);
		return false;
	}

	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;
}

bool CMemCBusi::ModiUserLoginTime(uint32 userid, uint32 logintime)
{
	char str_3[128] = {0};
	sprintf(str_3, "userinfo#%u#%u", GetServerMagic(),userid);
	CLog::Log("CMemCBusi::ModiUserLoginTime", CLog::TYPE_IMPORTANT, 
				" MOD USER %u time %u",userid, logintime);
	
	// 修改
	MemcachedTryLock(MEMCACHED_USER_LOCK);
	
	if( false == c_memc->redisClusterCommand_set(
			"hmset %s %s %u", 	 
			str_3,
			"logintime", 	logintime
			))
	{
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi::ModiUserLoginTime", CLog::TYPE_ERROR,
				" %s %d: set %s fail",__FUNCTION__,__LINE__,str_3);
		return false;
	}
	
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;
}

// 获取排队列表
bool CMemCBusi::GetCusterList(uint32 userid, vector<uint32> &vecList)
{
	char str_3[128] = {0};
	sprintf(str_3, "CServiceRecptionList#%u#%u", GetServerMagic(),userid);
	CLog::Log("CMemCBusi::GetCusterList", CLog::TYPE_IMPORTANT, 
				" USER %u",userid);
		
	// 修改
	MemcachedTryLock(MEMCACHED_USER_LOCK);

	vector<string> vecAllKey;
	if( false == c_memc->redisGetAllKey(str_3, vecAllKey) )
	{
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi::GetCusterList", CLog::TYPE_ERROR,
				"%s fail",str_3);
		return false;
	}

	for(int i = 0; i < (int)vecAllKey.size(); ++i)
	{
		string strKey = vecAllKey[i];
		CLog::Log("CMemCBusi::GetCusterList", CLog::TYPE_DEBUG,
				"key %s id %s",str_3, strKey.c_str());
		uint32 id = (uint32)atoi(strKey.c_str());
		vecList.push_back(id);
	}
	
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;
}

bool CMemCBusi::AddModiUserInfo(const tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo globalUserInfo,uint32 ServIp)
{
	//uint64_t value = 1;
	char str_a[128] = {0};
//	char str_b[128] = {0};
	char str_1[128] = {0};
	char str_2[128] = {0};
	char str_3[128] = {0};
	char check[1][1024] = {0};
//	unsigned int user_index = 0;
	m_MemRc = true; 
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, 
			" ADD USER %u",globalUserInfo.id);
	//m_memcached_lock.Lock();
	
	MemcachedTryLock(MEMCACHED_USER_LOCK);
	//查询是否存在userinfo
	sprintf(str_a,"userinfo#%u#%u",GetServerMagic(),globalUserInfo.id);
	c_memc->redisClusterCommand_get(check,"hmget %s %u",str_a,globalUserInfo.id);
	if(check[0][0] == 0)//add info
	{
		//增加useronline表 索引---value中只有id值
		sprintf(str_1,"useronline#%u#%u",GetServerMagic(),ServIp);
		sprintf(str_2,"%u",globalUserInfo.id);	

		if( false == c_memc->zadd(str_1,str_2))
		{					
			MemcachedUnLock(MEMCACHED_USER_LOCK);
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
				" %s %d: lpush %s fail",__FUNCTION__,__LINE__,str_1);
			return false;
		}		
	}		
	//增加userinfo 索引 
	char utf8_username[MAX_USERNAME_LEN*3+1] = {0};
	char utf8_nickname[MAX_NICKNAME_LEN*3+1] = {0};

	gbk2utf8(utf8_username,globalUserInfo.username);
	gbk2utf8(utf8_nickname,globalUserInfo.nickname);
	sprintf(str_3,"userinfo#%u#%u",GetServerMagic(),globalUserInfo.id);
	if( false == c_memc->redisClusterCommand_set(
		"hmset %s %s %u %s %u %s %s %s %s %s %u %s %u %s %u %s %u %s %u %s %u %s %u %s %u %s %s %s %s %s %u %s %u",
		str_3,
		"status",		globalUserInfo.status,
		"rule",			globalUserInfo.rule,
		"username",		utf8_username,
		"nickname",		utf8_nickname,
		"clientip",		globalUserInfo.clientip,
		"clientport",	globalUserInfo.clientport,
		"sexy",			globalUserInfo.sexy,
		"serverip", 	globalUserInfo.serverip,
		"lanip", 		globalUserInfo.lanip,	
		"flag" ,		globalUserInfo.flag,
		"csTeamID", 	globalUserInfo.teamID,
		"logintime", 	globalUserInfo.logintime,		
		"sessionkey" ,	Base64Encode(globalUserInfo.sessionkey, MAX_SESSIONKEY_LEN).c_str(),
		"p2psessionkey",Base64Encode(globalUserInfo.p2psessionkey, MAX_SESSIONKEY_LEN).c_str(),
		"flow" ,		globalUserInfo.flow,
		"id",			globalUserInfo.id))
	{
			
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,str_3);
		return false;
	}

	if(globalUserInfo.id < Id_Boundary || globalUserInfo.id > cs_Boundary)
	{
		c_memc->expire(str_3,seller_Timeout);
	}
	else
	{		
		c_memc->expire(str_3,buyer_First_Timeout);
	}

	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;
}

bool CMemCBusi::DelUserInfo(uint32 nUserId,uint32 ServIp)
{
	char str_1[128] = {0};
	char str_2[128] = {0};
	char str_3[128] = {0};

	m_MemRc = true;

	MemcachedTryLock(MEMCACHED_USER_LOCK);

	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, 
			" DELETE USER %u",nUserId);

	sprintf(str_1,"userinfo#%u#%u",GetServerMagic(),nUserId);
	sprintf(str_2,"%u",nUserId);	
	sprintf(str_3,"useronline#%u#%u",GetServerMagic(),ServIp);
	if( false == c_memc->remove(string(str_1)))
	{
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: remove %s fail",__FUNCTION__,__LINE__,str_1);
		return false;
	}
	
	if( false == c_memc->zrem(str_3,str_2))
	{
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: lrem %s fail",__FUNCTION__,__LINE__,str_1);
		return false;
	}	

	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;

}




//查询用户信息自身不带锁，调用他的函数加锁
int CMemCBusi::QueryUserInfo(uint32 nUserId, tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo& globalUserInfo)
{
	memset(&globalUserInfo, 0, sizeof(globalUserInfo));

	char key[128] = {0};

	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();

	if(c_Memcache->GetLinkState() == true) //和memcached正常连接状态
	{
		sprintf(key, "userinfo#%u#%u", GetServerMagic(),nUserId);
		string strMemGet;
		char retval[18][1024] = {0};
		m_MemRc = c_memc->redisClusterCommand_get(retval,"hmget %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s %s",
		key,
		"id",
		"status",		
		"rule",			
		"username",		
		"nickname",		
		"clientip",		
		"clientport",	
		"sexy",			
		"serverip", 	
		"lanip",
		"flow",
		"flag" ,	
		"logintime",
		"csTeamID", 		 	
		"sessionkey" ,	
		"p2psessionkey"
		);		
		

		if(m_MemRc != true || retval[0][0] == 0)	//存储的id信息为0 
		{
			CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryUserInfo NO user %u",nUserId);
			return MEMCACHED_NO_INFO;
		}

		globalUserInfo.id = (uint32)atoi(retval[0]);
		globalUserInfo.status = (uint8)atoi(retval[1]);
		globalUserInfo.rule = (uint8)atoi(retval[2]);
		strcpy(globalUserInfo.username, retval[3]);
		strcpy(globalUserInfo.nickname, retval[4]);
		utf2gbk(retval[3],strlen(retval[3]),globalUserInfo.username,MAX_USERNAME_LEN);
		utf2gbk(retval[4],strlen(retval[4]),globalUserInfo.nickname,MAX_NICKNAME_LEN);
		globalUserInfo.clientip = (uint32)atoi(retval[5]);
		globalUserInfo.clientport = (uint32)atoi(retval[6]);
		globalUserInfo.sexy = (uint32)atoi(retval[7]);
		globalUserInfo.serverip = (uint32)atoi(retval[8]);
		globalUserInfo.lanip = (uint32)atoi(retval[9]);
		globalUserInfo.flow = (uint32)atoi(retval[10]);
		globalUserInfo.flag = (uint32)atoi(retval[11]);
		globalUserInfo.logintime = (uint32)atoi(retval[12]);
		globalUserInfo.teamID = (uint32)atoi(retval[13]);
		//写的时候没写这个字段，读的时候却要读这个字段，临时
		//Base64Decode(jvUser["sessionkey"].asString()).c_str();
		string strSessionKey = Base64Decode(string(retval[14])).c_str();
		string strP2pSessionKey = Base64Decode(string(retval[15])).c_str();
		memcpy(globalUserInfo.sessionkey, strSessionKey.c_str(), strSessionKey.size()); 
		memcpy(globalUserInfo.p2psessionkey, strP2pSessionKey.c_str(), strP2pSessionKey.size());		
		globalUserInfo.version = 0;

		CLog::Log("CMemCBusi", CLog::TYPE_DEBUG,
		"QueryUserInfo--key %s id : %s status %s rule %s username %s nickname %s clientip %s \
clientport %s sexy %s serverip %s lanip %s flow %s flag %s logintime %s teamID %s sessionkey %s p2psessionkey %s",
			key,retval[0],retval[1],retval[2],retval[3],retval[4],retval[5],retval[6],retval[7],
			retval[8],retval[9],retval[10], retval[11], retval[12], retval[13], retval[14], retval[15]);
				

		return MEMCACHED_GET_INFO_SUCCESS;
	}
	else
	{		
		CLog::Log("CMemCBusi-QueryUserInfo", CLog::TYPE_ERROR, "MEMCACHED_SERVER_DOWN");
		return MEMCACHED_SERVER_DOWN;
	}
}

bool CMemCBusi::query_total_user_number(uint32 &total_count)//请求在线用户信息
{
	vector<int> server_info;
	string total_user;
	uint64_t total_user_count = 0;
	total_count = 0;
	char str_1[128] = {0};
	char str_3[128] = {0};
	
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	if(false == QueryServerInfo(server_info))
	{
		return false;		
	}	
	//m_memcached_lock.Lock();
	for(vector<int>::iterator iter = server_info.begin();iter != server_info.end(); iter++)
	{	
		sprintf(str_3,"useronline#%u#%u",GetServerMagic(),*iter);		 
		if( false == c_memc->zcard(str_3,&total_user_count))
		{
			//m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: get %s fail",__FUNCTION__,__LINE__,str_1);
			return false;
		}		
		total_count += (uint32)total_user_count;		
	}
	//m_memcached_lock.Unlock();
	return true;

}

int CMemCBusi::SearchOnlineUser(tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo *pUserInfo,uint8 & count,uint32& nOnlineNum,uint16 startpos)//请求在线用户信息
{
	vector<int> server_info;
	
	string total_user;
	char user_id[1][1024] = {0};
	string strMemGet;
	uint32 each_total_user_count = 0;	
	uint32 calc_get_user_count = 0;
	char str_1[128] = {0};
	char str_2[128] = {0};
	char str_3[128] = {0};
	bool already_skip = false;
	uint64_t total_user_count = 0;
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	if(c_Memcache->GetLinkState() == true)
	{
		QueryServerInfo(server_info);
		
		//m_memcached_lock.Lock();
		for(vector<int>::iterator iter = server_info.begin();iter != server_info.end(); iter++)
		{
			sprintf(str_3,"useronline#%u#%u",GetServerMagic(),*iter);			//获取某一个server下totalserver的值 
			if( false == c_memc->zcard(str_3,&total_user_count))
			{
				//m_memcached_lock.Unlock();
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
					" %s %d: get %s fail",__FUNCTION__,__LINE__,str_1);
				return MEMCACHED_NO_INFO;
			}				
			each_total_user_count = (uint32)total_user_count;
			//count: query user number
			//left_count: total user in all
			//total_user_count :each server_id 's user number
			uint32 count_t = 0;
			//确定从哪个服务器开始取用户
			if(false == already_skip)
			{
				if(startpos < each_total_user_count)
				{
					count_t = startpos;
				}
				else
				{
					startpos -= each_total_user_count;
					continue;
				}
			}			
			sprintf(str_2,"useronline#%u#%u",GetServerMagic(),*iter);		
			for(;(count_t < each_total_user_count);count_t++)
			{
				int countx = 1;
				if( false == c_memc->zrange(str_2,count_t,count_t,countx,user_id))
				{
					//m_memcached_lock.Unlock();
					CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
						" %s %d: get %s fail",__FUNCTION__,__LINE__,str_2);
					return MEMCACHED_NO_INFO;
				}
			
				sprintf(str_3,"userinfo#%u#%s",GetServerMagic(),user_id[0]);	
#if 0				
				if(false == c_memc->get(string(str_3),strMemGet))
				{
					//m_memcached_lock.Unlock();
					CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
						" %s %d: get %s fail",__FUNCTION__,__LINE__,str_3);
					return MEMCACHED_NO_INFO;
				}


				/*parse info*/			
				CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryUserInfo %s", strMemGet.c_str());
#endif				
				tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo = {0};
				
				if(MEMCACHED_GET_INFO_SUCCESS == QueryUserInfo(atoi(user_id[0]),userInfo))
				{
					if(userInfo.status != 18 /*HIDE STATES*/ )
					{
						CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_DEBUG,
							"SearchOnlineUser_id %u",userInfo.id);
			
						
						pUserInfo[calc_get_user_count].clientid = userInfo.id;
						pUserInfo[calc_get_user_count].sexy = userInfo.sexy;
						pUserInfo[calc_get_user_count].status = userInfo.status;
						pUserInfo[calc_get_user_count].rule = userInfo.rule;
						strncpy(pUserInfo[calc_get_user_count].username,userInfo.username,MAX_USERNAME_LEN);
						strncpy(pUserInfo[calc_get_user_count].nickname, userInfo.nickname,MAX_USERNAME_LEN);
						 
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
					//m_memcached_lock.Unlock();
					query_total_user_number(nOnlineNum);
					return MEMCACHED_GET_INFO_SUCCESS;
				}	
				/*end parse info*/

				memset(user_id,0,1024);				
				memset(str_3,0,128);
				already_skip = true;
			}
		}
		count = calc_get_user_count;
		//m_memcached_lock.Unlock();
		query_total_user_number(nOnlineNum); //返回在线用户总数
		//获取totalserver的值
		return MEMCACHED_GET_INFO_SUCCESS;
	}
	else
	{
		CLog::Log("CMemCBusi-SearchOnlineUser", CLog::TYPE_ERROR, "MEMCACHED_SERVER_DOWN");
		return MEMCACHED_SERVER_DOWN;
	}
}

// 查询一台服务器上所有的用户
int CMemCBusi::GetAllUser(int nServerNo, vector<tagGSRV_TUSERINFO_ACK_EX> &vecUser)
{
	uint64_t total_user_count = 0;	
	char str_3[128] = {0};
	if(c_Memcache->GetLinkState() == true)
	{
		sprintf(str_3,"useronline#%u#%u",GetServerMagic(),nServerNo);			
		if( false == c_memc->zcard(str_3,&total_user_count))
		{
			CLog::Log("CMemCBusi::GetAllUser", CLog::TYPE_ERROR,
					" %s %d: get %s fail",__FUNCTION__,__LINE__,str_3);
			return MEMCACHED_NO_INFO;
		}	

		if(total_user_count <= 0)
		{
			return MEMCACHED_NO_INFO;
		}

		if(total_user_count > 3000)
		{	// 最多查询3000个好友
			total_user_count = 3000;
		}
		char **pUserID = new char*[total_user_count]; 
		for(int i = 0; i < (int)total_user_count; i++)
		{
			pUserID[i] = new char[24];
			memset(pUserID[i], 0, 24);
		}
			
		int countx = total_user_count;
		if( false == c_memc->zrange(str_3,0,-1,countx,pUserID,24))
		{
			CLog::Log("CMemCBusi::GetAllUser", CLog::TYPE_ERROR,
					" %s %d: get %s fail",__FUNCTION__,__LINE__,str_3);

			for(int i = 0; i < (int)total_user_count; i++)
			{
				delete[] pUserID[i];
			}
			delete[] pUserID;
			return 	MEMCACHED_NO_INFO;
		}

		for(int i = 0; i < countx; i++)
		{
			CLog::Log("CMemCBusi::GetAllUser", CLog::TYPE_DEBUG, 
					"userid [%s]",pUserID[i]);

			tagGSRV_TUSERINFO_ACK_EX userInfo;
			if(MEMCACHED_GET_INFO_SUCCESS == GetTUserInfoEx(atoi(pUserID[i]),userInfo))
			{
				if((int)userInfo.serverip == nServerNo)
				{
					vecUser.push_back(userInfo);
				}
			}
			else
			{			
				CLog::Log("CMemCBusi::GetAllUser", CLog::TYPE_DEBUG, 
					"no user [%s]",pUserID[i]);
			}
		}

		for(int i = 0; i < (int)total_user_count; i++)
		{
			delete[] pUserID[i];
		}
		delete[] pUserID;
		
		return MEMCACHED_GET_INFO_SUCCESS;
	}
	else
	{
		CLog::Log("CMemCBusi::GetAllUser", CLog::TYPE_ERROR, "MEMCACHED_SERVER_DOWN");
		return MEMCACHED_SERVER_DOWN;
	}
}


//查询用户控制信息
int CMemCBusi::QueryUserControlInfo(uint32 userId,tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo &userControlInfo)
{
//	int ret = -1;
	char key[128] = {0};
	if(c_Memcache->GetLinkState() == true) //和memcached正常连接状态
	{
		sprintf(key, "userinfo#%u#%u", GetServerMagic(),userId);
		string strMemGet;
		char retval[18][1024] = {0};
		m_MemRc = c_memc->redisClusterCommand_get(retval,"hmget %s %s %s %s %s %s",
		key,
		"id",		
		"serverip", 		 	
		"sessionkey" ,	
		"p2psessionkey",
		"csTeamID"
		);		
		if(m_MemRc != true || retval[0][0] == 0)	//存储的id信息为0 
		{
			CLog::Log("QueryUserControlInfo", CLog::TYPE_DEBUG, "no userinfo %u",userId);

			userControlInfo.srvip =0;
			userControlInfo.uid = userId;
			userControlInfo.flag = 0;         /*   flag    好像没用*/			
			userControlInfo.teamID = 0;
			return MEMCACHED_NO_INFO;
		}
		CLog::Log("QueryUserControlInfo", CLog::TYPE_DEBUG,
		"key %s id : %s serverip %s sessionkey %s p2psessionkey %s teamID %s",
			key,retval[0],retval[1],retval[2],retval[3],retval[3]);
		
		userControlInfo.uid = (uint32)atoi(retval[0]);
		userControlInfo.srvip = (uint32)atoi(retval[1]);		
		string strSessionKey = Base64Decode(string(retval[2])).c_str();
		string strP2pSessionKey = Base64Decode(string(retval[3])).c_str();
		memcpy(userControlInfo.sessionKey, strSessionKey.c_str(), strSessionKey.size()); 
		memcpy(userControlInfo.p2pSessionKey, strP2pSessionKey.c_str(), strP2pSessionKey.size());		
		userControlInfo.flag = 0;         /*   flag    好像没用*/
		userControlInfo.teamID = (uint8)atoi(retval[4]);

		return MEMCACHED_GET_INFO_SUCCESS;
	}
	else
	{		
		CLog::Log("QueryUserControlInfo", CLog::TYPE_ERROR, "MEMCACHED_SERVER_DOWN");
		return MEMCACHED_SERVER_DOWN;
	}	

}

//修改用户状态
bool CMemCBusi::SetTUserInfo(uint32 userId, tagGSRV_TUSERINFO_ACK &TUserInfo)
{
	//key
	char key[128];
	sprintf(key, "userinfo#%u#%u",GetServerMagic(), userId);
	MemcachedTryLock(MEMCACHED_USER_LOCK);

	if( false == c_memc->redisClusterCommand_set(
		"hmset %s %s %u %s %u %s %u %s %s %s %u %s %u %s %u %s %u",
		key,
		"id",				TUserInfo.id,
		"clientip",			TUserInfo.ip,
		"clientport",		TUserInfo.port,
		"sessionkey",		Base64Encode(TUserInfo.sessionkey, MAX_SESSIONKEY_LEN).c_str(),
		"flag",				TUserInfo.flag,
		"merchantID",		TUserInfo.merchantID,
		"newlyLiveTime",	TUserInfo.newlylivetime,
		"serverip",			TUserInfo.serverip
		))
	{			
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("SetTUserInfo", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,key);
		TUserInfo.ret = RESULT_FAILED;
		return false;
	}
	TUserInfo.ret = RESULT_SUCCESS;
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;
}


int CMemCBusi::GetTUserInfo(uint32 userId,tagGSRV_TUSERINFO_ACK &TUserInfo)
{
//	int ret = -1;
	char key[128] = {0};
	if(c_Memcache->GetLinkState() == true) //和memcached正常连接状态
	{
		sprintf(key, "userinfo#%u#%u", GetServerMagic(),userId);
		string strMemGet;
		char retval[18][1024] = {0};
		m_MemRc = c_memc->redisClusterCommand_get(retval,"hmget %s %s %s %s %s %s %s %s %s",
		key,
		"id",
		"clientip",		
		"clientport",		 	
		"sessionkey",
		"flag",
		"merchantID",
		"newlyLiveTime",
		"serverip"
		);		
		if(m_MemRc != true || retval[0][0] == 0)	//存储的id信息为0 
		{
			CLog::Log("GetTUserInfo", CLog::TYPE_DEBUG, "no userinfo %u ",userId);
			
			TUserInfo.ret = RESULT_FAILED;
			TUserInfo.id =userId;
			return MEMCACHED_NO_INFO;
		}
		CLog::Log("GetTUserInfo", CLog::TYPE_DEBUG,
		"key %s id : %s clientip %s clientport %s sessionkey %s flag %s merchantID %s newlgLiveTime %s",
			key,retval[0],retval[1],retval[2],retval[3],retval[4],retval[5],retval[6]);
		
		TUserInfo.ret = RESULT_SUCCESS;
		TUserInfo.id = (uint32)atoi(retval[0]);
		TUserInfo.ip = (uint32)atoi(retval[1]);
		TUserInfo.port = (uint16)atoi(retval[2]);
		string strSessionKey = Base64Decode(string(retval[3])).c_str();
		memcpy(TUserInfo.sessionkey, strSessionKey.c_str(), strSessionKey.size()); 
		TUserInfo.flag = (uint8)atoi(retval[4]);
		TUserInfo.merchantID = (uint32)atoi(retval[5]);
		TUserInfo.newlylivetime = (uint32)atoi(retval[6]);
		TUserInfo.serverip= (uint32)atoi(retval[7]);
		return MEMCACHED_GET_INFO_SUCCESS;
	}
	else
	{				
		TUserInfo.ret = RESULT_FAILED;
		CLog::Log("GetTUserInfo", CLog::TYPE_ERROR, "MEMCACHED_SERVER_DOWN");
		return MEMCACHED_SERVER_DOWN;
	}	

}


int CMemCBusi::GetTUserInfoEx(uint32 userId,tagGSRV_TUSERINFO_ACK_EX &TUserInfo)
{
//	int ret = -1;
	char key[128] = {0};
	if(c_Memcache->GetLinkState() == true) //和memcached正常连接状态
	{
		sprintf(key, "userinfo#%u#%u", GetServerMagic(),userId);
		string strMemGet;
		char retval[18][1024] = {0};
		m_MemRc = c_memc->redisClusterCommand_get(retval,"hmget %s %s %s %s %s %s %s %s %s %s %s",
		key,
		"id",
		"clientip",		
		"clientport",		 	
		"sessionkey",
		"flag",
		"merchantID",
		"newlyLiveTime",
		"serverip",
		"status",
		"flow"
		);		
		if(m_MemRc != true || retval[0][0] == 0)	//存储的id信息为0 
		{
			CLog::Log("GetTUserInfo", CLog::TYPE_DEBUG, "no userinfo %u ",userId);
			
			TUserInfo.ret = RESULT_FAILED;
			TUserInfo.id =userId;
			return MEMCACHED_NO_INFO;
		}
		CLog::Log("GetTUserInfoEx", CLog::TYPE_DEBUG,
		"key %s id : %s clientip %s clientport %s sessionkey %s flag %s merchantID %s newlgLiveTime %s",
			key,retval[0],retval[1],retval[2],retval[3],retval[4],retval[5],retval[6]);
		
		TUserInfo.ret = RESULT_SUCCESS;
		TUserInfo.id = (uint32)atoi(retval[0]);
		TUserInfo.ip = (uint32)atoi(retval[1]);
		TUserInfo.port = (uint16)atoi(retval[2]);
		string strSessionKey = Base64Decode(string(retval[3])).c_str();
		memcpy(TUserInfo.sessionkey, strSessionKey.c_str(), strSessionKey.size()); 
		TUserInfo.flag = (uint8)atoi(retval[4]);
		TUserInfo.merchantID = (uint32)atoi(retval[5]);
		TUserInfo.newlylivetime = (uint32)atoi(retval[6]);
		TUserInfo.serverip= (uint32)atoi(retval[7]);
		TUserInfo.status = (uint8)atoi(retval[8]);
		TUserInfo.flow = (uint8)atoi(retval[9]);
		return MEMCACHED_GET_INFO_SUCCESS;
	}
	else
	{				
		TUserInfo.ret = RESULT_FAILED;
		CLog::Log("GetTUserInfo", CLog::TYPE_ERROR, "MEMCACHED_SERVER_DOWN");
		return MEMCACHED_SERVER_DOWN;
	}	

}




//查询用户状态
int CMemCBusi::QueryUserStatus(uint32 userId, tagGSRV_GET_USER_STATUS_ACK::tagUserStatus &userStatus)
{
//	int ret = -1;

	char key[128] = {0};
	if(c_Memcache->GetLinkState() == true) //和memcached正常连接状态
	{
		sprintf(key, "userinfo#%u#%u", GetServerMagic(),userId);
		string strMemGet;
		char retval[2][1024] = {0};
		m_MemRc = c_memc->redisClusterCommand_get(retval,"hmget %s %s %s",
		key,
		"id",
		"status"			
		);		
		if(m_MemRc != true || retval[0][0] == 0)	//存储的id信息为0 
		{
			CLog::Log("QueryUserStatus", CLog::TYPE_DEBUG, "no userinfo %u",userId);
			
			userStatus.status = (uint8)XTREAM_OFFLINE;
			userStatus.id =userId;
			return MEMCACHED_NO_INFO;
		}
		CLog::Log("QueryUserStatus", CLog::TYPE_DEBUG,
		"key %s id : %s status %s ",
			key,retval[0],retval[1]);
		
		userStatus.id = (uint32)atoi(retval[0]);
		userStatus.status = (uint8)atoi(retval[1]);
 
		return MEMCACHED_GET_INFO_SUCCESS;
	}
	else
	{				
		userStatus.status = (uint8) XTREAM_OFFLINE;
		userStatus.id =userId;
		CLog::Log("QueryUserStatus", CLog::TYPE_ERROR, "MEMCACHED_SERVER_DOWN");
		return MEMCACHED_SERVER_DOWN;
	}
}

//修改用户状态
bool CMemCBusi::ModiUserStatus(tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser user)
{
	//key
	char key[128];
	sprintf(key, "userinfo#%u#%u",GetServerMagic(), user.id);
	MemcachedTryLock(MEMCACHED_USER_LOCK);

	if( false == c_memc->redisClusterCommand_set(
		"hmset %s %s %u",
		key,
		"status",		user.status
		))
	{			
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("ModiUserStatus", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,key);
		return false;
	}
	
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;
}


//查询分流
int CMemCBusi::QueryUserFlow(uint32 userId,tagGSRV_GET_USER_FLOW_ACK::tagUserFlow &userFlow)
{
//	int ret = -1;
	char key[128] = {0};
	if(c_Memcache->GetLinkState() == true) //和memcached正常连接状态
	{
		sprintf(key, "userinfo#%u#%u", GetServerMagic(),userId);
		string strMemGet;
		char retval[3][1024] = {0};
		m_MemRc = c_memc->redisClusterCommand_get(retval,"hmget %s %s %s %s",
		key,
		"id",
		"status",
		"flow"
		);		
		if(m_MemRc != true || retval[0][0] == 0)	//存储的id信息为0 
		{
			CLog::Log("QueryUserFlow", CLog::TYPE_DEBUG, "no userinfo %u",userId);
			userFlow.id =userId;
			userFlow.status = (uint8) XTREAM_OFFLINE;
			userFlow.flow = (uint8)0;
			return MEMCACHED_NO_INFO;
		}
		CLog::Log("QueryUserFlow", CLog::TYPE_DEBUG,
		"key %s id : %s status %s flow %s",
			key,retval[0],retval[1],retval[2]);
		
		userFlow.id = (uint32)atoi(retval[0]);
		userFlow.status = (uint8)atoi(retval[1]);
 		userFlow.flow =   (uint8)atoi(retval[2]);
		return MEMCACHED_GET_INFO_SUCCESS;
	}
	else
	{				
		userFlow.status = (uint8)XTREAM_OFFLINE;
		userFlow.id =userId;
		CLog::Log("QueryUserFlow", CLog::TYPE_ERROR, "MEMCACHED_SERVER_DOWN");
		return MEMCACHED_SERVER_DOWN;
	}

}


//修改分流
bool CMemCBusi::ModiUserFlow(const tagGSRV_MODIFY_USER_FLOW_QUERY::tagFlow UserFlow)
{
	//key
	char key[128];
	sprintf(key, "userinfo#%u#%u",GetServerMagic(), UserFlow.id);
	MemcachedTryLock(MEMCACHED_USER_LOCK);

	if( false == c_memc->redisClusterCommand_set(
		"hmset %s %s %u",
		key,
		"flow",		UserFlow.flow
		))
	{
		//m_memcached_lock.Unlock();
			
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("ModiUserFlow", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,key);
		return false;
	}

	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;


}


bool CMemCBusi::AddLogoutLock(uint32 userId)
{

	//key
	char key[128];
	sprintf(key, "logoutLock#%u#%u",GetServerMagic(), userId);

	//添加登出Lock
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	bool ret = c_memc->set(key, "locked");
	CLog::Log("CMemCBusi::AddLogoutLock", CLog::TYPE_IMPORTANT,
		"key:%s ret:%d",key, ret);
	c_memc->expire(key, LOGOUT_TIMEOUT);
	return ret;
}


bool CMemCBusi::GetLogoutLock(uint32 userId)
{

	//key
	char key[128];
	sprintf(key, "logoutLock#%u#%u",GetServerMagic(), userId);
	
	//获取登出Lock
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	string ret_val;
	bool ret = c_memc->get(key,ret_val);
	CLog::Log("CMemCBusi::GetLogoutLock", CLog::TYPE_IMPORTANT,
		"key:%s, val:%s, ret:%d", key, ret_val.c_str(), ret);

	return ret_val.length() > 0 ? true : false;;

}



bool CMemCBusi::DelLogoutLock(uint32 userId)
{

	//key
	char key[128];
	sprintf(key, "logoutLock#%u#%u",GetServerMagic(), userId);
	
	//删除登出Lock
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	bool ret = c_memc->remove(key);
	CLog::Log("CMemCBusi::DelLogoutLock", CLog::TYPE_ERROR,
		"key:%s, ret:%d",key, ret);

	return ret; 

}

//查询用户所在客服组ID
int CMemCBusi::QueryUserTeamID(uint32 userId, tagGSRV_GET_USER_TEAMID_ACK::tagUserTeamID& userTeamID)
{
//	int ret = -1;
	char key[128] = {0};
	if(c_Memcache->GetLinkState() == true) //和memcached正常连接状态
	{
		sprintf(key, "userinfo#%u#%u", GetServerMagic(),userId);
		string strMemGet;
		char retval[2][1024] = {0};
		m_MemRc = c_memc->redisClusterCommand_get(retval,"hmget %s %s %s %s",
		key,
		"id",
		"csTeamID",
		"csUserID"
		);		
		if(m_MemRc != true || retval[0][0] == 0)	//存储的id信息为0 
		{
			CLog::Log("QueryUserTeamID", CLog::TYPE_DEBUG, "no userinfo %u",userId);
			userTeamID.id =userId;
			userTeamID.teamID = (uint8)0xff;
			userTeamID.csid = (uint32)0xffffff;
			return MEMCACHED_NO_INFO;
		}
		CLog::Log("QueryUserTeamID", CLog::TYPE_DEBUG,
		"key %s id : %s teamID %s",
			key,retval[0],retval[1]);
		
		userTeamID.id = (uint32)atoi(retval[0]);
		userTeamID.teamID = (uint8)atoi(retval[1]);
		userTeamID.csid = (uint32)atoi(retval[2]);
		return MEMCACHED_GET_INFO_SUCCESS;
	}
	else
	{				
		userTeamID.id = userId;
		userTeamID.teamID =(uint8)0xff;
		userTeamID.csid = (uint32)0xffffff;
		CLog::Log("QueryUserTeamID", CLog::TYPE_ERROR, "MEMCACHED_SERVER_DOWN");
		return MEMCACHED_SERVER_DOWN;
	}
}

//修改用户所在客服组ID

bool CMemCBusi::ModiUserTeamID(tagGSRV_MODIFY_ONLINE_TEAMID_QUERY::tagUserTeamID user)
{
	//key
	char key[128];
	sprintf(key, "userinfo#%u#%u",GetServerMagic(), user.id);
	MemcachedTryLock(MEMCACHED_USER_LOCK);

	if( false == c_memc->redisClusterCommand_set(
		"hmset %s %s %u %s %u",
		key,
		"csTeamID",		user.teamID,
		"csUserID",		user.csid
		))
	{
		//m_memcached_lock.Unlock();
			
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("ModiUserTeamID", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,key);
		return false;
	}
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;
}

bool CMemCBusi::setExpireUserinfo(uint32 userid,uint32 timeout)
{
	char str_1[64] = {0};	
	char str_2[64] = {0};
	sprintf(str_1,"userinfo#%u#%u",GetServerMagic(),userid);
	sprintf(str_2,"CServiceInfo#%u#%u",GetServerMagic(),userid);

	if(timeout == 0)
	{
		if(userid < Id_Boundary)   //卖家
		{
			c_memc->expire(str_1,seller_Timeout);
		}
		else if(userid < cs_Boundary) //买家
		{
			c_memc->expire(str_1,buyer_Timeout);
		}
		else if(userid < work_Boundary)//客服
		{
			c_memc->expire(str_1,seller_Timeout);
			c_memc->expire(str_2,seller_Timeout);
		}
		else	//工作版
		{
			c_memc->expire(str_1,seller_Timeout);
		}
	}
	else
	{
		c_memc->expire(str_1,timeout);
	}
	return true;
}

bool CMemCBusi::UpdateCsMonitor(CUser *pUser)
{
	//key
	char key[128];
	sprintf(key, "userinfo#%u#%u",GetServerMagic(), pUser->m_nID);
	MemcachedTryLock(MEMCACHED_USER_LOCK);

	if( false == c_memc->redisClusterCommand_set(
		"hmset %s %s %u %s %u %s %u",
		key,
		"isMornitored",			pUser->m_bMonitored,
		"chattingBuyerID",		pUser->m_nChattingBuyerID,
		"directorID",			pUser->m_nDirectorID
		))
	{
		//m_memcached_lock.Unlock();
			
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("ModiUserTeamID", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,key);
		return false;
	}
	CLog::Log("UpdateCsMonitor", CLog::TYPE_DEBUG,
		"key %s isMornitored : %d chattingBuyerID %u directorID %u",
			key,pUser->m_bMonitored,pUser->m_nChattingBuyerID,pUser->m_nDirectorID);
	
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;
}



bool CMemCBusi::QueryCsMonitor(uint32 userId,tagGSRV_GET_ONLINE_MONITOR_ACK::UserInfo &userInfo)
{
//	int ret = -1;
	char key[128] = {0};
	if(c_Memcache->GetLinkState() == true) //和memcached正常连接状态
	{
		sprintf(key, "userinfo#%u#%u", GetServerMagic(),userId);
		string strMemGet;
		char retval[6][1024] = {0};
		m_MemRc = c_memc->redisClusterCommand_get(retval,"hmget %s %s %s %s %s %s %s",
		key,
		"id",
		"state",
		"username",
		"isMornitored",
		"chattingBuyerID",
		"directorID"
		);		
		if(m_MemRc != true || retval[0][0] == 0)	//存储的id信息为0 
		{
			CLog::Log("QueryUserTeamID", CLog::TYPE_DEBUG, "no userinfo %u",userId);
			userInfo.id =userId;
			return MEMCACHED_NO_INFO;
		}
		utf2gbk(retval[2],strlen(retval[2]),userInfo.username,MAX_USERNAME_LEN);
		CLog::Log("QueryUserTeamID", CLog::TYPE_DEBUG,
		"key %s  [%s][%s][%s][%s][%s][%s]",
			key,retval[0],retval[1],userInfo.username,retval[3],retval[4],retval[5]);
		
		userInfo.id = (uint32)atoi(retval[0]);
		userInfo.status = (uint8)atoi(retval[1]);
		userInfo.monitored = (bool)atoi(retval[3]);
		userInfo.chattingBuyerID = (uint32)atoi(retval[4]);
		userInfo.directorID = (uint32)atoi(retval[5]);
		return MEMCACHED_GET_INFO_SUCCESS;
	}
	else
	{				
		userInfo.id = userId;		
		CLog::Log("QueryUserTeamID", CLog::TYPE_ERROR, "MEMCACHED_SERVER_DOWN");
		return MEMCACHED_SERVER_DOWN;
	}
}

// 修改用户签名
bool CMemCBusi::ModiUserSign(uint32 userid,const char *pUserSign)
{
	char utf8_usersign[MAX_USERSIGN_LEN*3+1] = {0};
	gbk2utf8(utf8_usersign,pUserSign);

	CLog::Log("CMemCBusi::ModiUserSign", CLog::TYPE_DEBUG,
			"修改[%u]个性签名为%s",userid,utf8_usersign);

	char key[128];
	sprintf(key, "userinfo#%u#%u",GetServerMagic(), userid);
	MemcachedTryLock(MEMCACHED_USER_LOCK);

	if( false == c_memc->redisClusterCommand_set(
		"hmset %s %s %s",
		key,
		"userSign",		utf8_usersign
		))
	{
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi::ModiUserSign", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,key);
		return false;
	}
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;
}

// 更新在线时长
bool CMemCBusi::ModiNewLiveTime(uint32 userid, uint32 newlivetime)
{
	char key[128];
	sprintf(key, "userinfo#%u#%u",GetServerMagic(), userid);
	MemcachedTryLock(MEMCACHED_USER_LOCK);

	if( false == c_memc->redisClusterCommand_set(
		"hmset %s %s %u",
		key,
		"newlyLiveTime", newlivetime
		))
	{
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi::ModiNewLiveTime", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,key);
		return false;
	}
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;
}

#else
bool CMemCBusi::AddModiUserInfo(const tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo globalUserInfo,uint32 ServIp)
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
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, 
			" ADD USER %u",globalUserInfo.id);
	//m_memcached_lock.Lock();
	
	MemcachedTryLock(MEMCACHED_USER_LOCK);
	//查询是否存在userinfo
	sprintf(str_a,"userinfo#%u#%u",GetServerMagic(),globalUserInfo.id);
	//cout << "check " << str_a << endl;
	c_memc->get(string(str_a),check);

	if(check.empty())//add info
	{
		////cout << "no such user id\r\n" << endl;
			//增加totaluser	并get其最新值
			
		sprintf(str_b,"totaluser#%u#%u",GetServerMagic(),ServIp/*serverid  ---  test only*/);
		
		if( false == c_memc->increment(string(str_b),1,&value))
		{
			//m_memcached_lock.Unlock();
			
			MemcachedUnLock(MEMCACHED_USER_LOCK);
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
				" %s %d: increment %s fail",__FUNCTION__,__LINE__,str_b);
			return false;

		}
		//cout << "totaluser#" <<  GetServerId() << "now :" << value << endl;

		//增加useronline表 索引---value中只有id值
		sprintf(str_1,"useronline#%u#%u#%lu",GetServerMagic(),ServIp,value);
		sprintf(str_2,"%u",globalUserInfo.id);		
		if( false == c_memc->set(string(str_1),str_2))
		{
			//m_memcached_lock.Unlock();
			
			MemcachedUnLock(MEMCACHED_USER_LOCK);
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
				" %s %d: set %s fail",__FUNCTION__,__LINE__,str_1);
			return false;
		}
	}
	else //modified info
	{
			/*需要从check 取出 index值*/	

		Json::Reader jReader(Json::Features::strictMode());
		Json::Value jvUser;
		
		if((check[0]== '{' ) && (jReader.parse(check, jvUser)))
		{				
			user_index = jvUser["onlineindex"].asUInt();			
		}
		else
		{
			CLog::Log("CMemCBusi-AddModiUserInfo", CLog::TYPE_ERROR,
				" %s %d: add_user_info json error %s",__FUNCTION__,__LINE__,check.c_str());	
			//m_memcached_lock.Unlock();
			
			MemcachedUnLock(MEMCACHED_USER_LOCK);
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
	jvUser["flag"] = globalUserInfo.flag;
	jvUser["csTeamID"] = globalUserInfo.teamID;
	jvUser["logintime"] = globalUserInfo.logintime;
	
	jvUser["sessionkey"] = Base64Encode(globalUserInfo.sessionkey, MAX_SESSIONKEY_LEN).c_str();
	jvUser["p2psessionkey"] = Base64Encode(globalUserInfo.p2psessionkey, MAX_SESSIONKEY_LEN).c_str();


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
		//m_memcached_lock.Unlock();
		
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,str_3);
		return false;
	}
	//cout << "set : " << str_3 << " value :" << strJvUser << endl;
	////cout << "inc: value is " << value << endl;
	//m_memcached_lock.Unlock();
	
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;
}


bool CMemCBusi::DelUserInfo(uint32 nUserId,uint32 ServIp)
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
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();

	MemcachedTryLock(MEMCACHED_USER_LOCK);
	//m_memcached_lock.Lock();
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, 
			" DELETE USER %u",nUserId);
//获取totaluser的值
	sprintf(str_a,"totaluser#%u#%u",GetServerMagic(),ServIp);
	if( false == c_memc->get(string(str_a),total_user))
	{
		//m_memcached_lock.Unlock();
		
		MemcachedUnLock(MEMCACHED_USER_LOCK);
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
		//m_memcached_lock.Unlock();
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: get %s fail",__FUNCTION__,__LINE__,str_1);
		return false;
	}

	if( false == c_memc->remove(string(str_1)))
	{
		//m_memcached_lock.Unlock();
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: remove %s fail",__FUNCTION__,__LINE__,str_1);
		return false;
	}
 	//cout << "[1]remove: " << str_1 << endl;
	/*需要从del_user_info 取出 index值*/
	

	Json::Reader jReader(Json::Features::strictMode());
	Json::Value jvUser;
	
	if((del_user_info[0] == '{' ) && (jReader.parse(del_user_info, jvUser)))
	{	
		user_index = jvUser["onlineindex"].asUInt();
	}
	else
	{
		CLog::Log("CMemCBusi-DelUserInfo", CLog::TYPE_ERROR, 
			"del_user_info json error %s",del_user_info.c_str());	
		//m_memcached_lock.Unlock();
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		return false;
	}

	/*end parse*/	

	sprintf(str_2,"useronline#%u#%u#%u",GetServerMagic(),ServIp,user_index);  //删除或者修改该key

//多个用户且不是删除index最大的用户时，从表中插入
	if( (max_user_index > 1) && (user_index != max_user_index))
	{
		//将serverindex表中的最大项插入到删除项中

		//get max index's user_id

		sprintf(str_3,"useronline#%u#%u#%u",GetServerMagic(),ServIp,max_user_index);
		if( false == c_memc->get(string(str_3),insert_user_id))
		{
			//m_memcached_lock.Unlock();
			MemcachedUnLock(MEMCACHED_USER_LOCK);
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: get %s fail",__FUNCTION__,__LINE__,str_3);
			return false;
		}

		
//		string set_insert(insert_user_id.begin(),insert_user_id.end());	
			//insert			
		if( false == c_memc->set(str_2,insert_user_id.c_str()))
		{
			//m_memcached_lock.Unlock();
			MemcachedUnLock(MEMCACHED_USER_LOCK);
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
				" %s %d: set %s fail",__FUNCTION__,__LINE__,str_2);
			return false;
		}
			
		//del max
		if( false == c_memc->remove(string(str_3)))
		{
			//m_memcached_lock.Unlock();
			MemcachedUnLock(MEMCACHED_USER_LOCK);
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
			//m_memcached_lock.Unlock();
			MemcachedUnLock(MEMCACHED_USER_LOCK);
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
			//m_memcached_lock.Unlock();
			MemcachedUnLock(MEMCACHED_USER_LOCK);
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
			//m_memcached_lock.Unlock();
			MemcachedUnLock(MEMCACHED_USER_LOCK);
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: remove %s fail",__FUNCTION__,__LINE__,str_2);
			return false;
		}

		//cout << "[2]remove: " << str_2 << endl;

	}

	//减少totaluser	并get其最新值

	if( false == c_memc->decrement(string(str_a),1,&value))
	{
		//m_memcached_lock.Unlock();
		MemcachedUnLock(MEMCACHED_USER_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" %s %d: decrement %s fail",__FUNCTION__,__LINE__,str_a);
		return false;
	}
	//m_memcached_lock.Unlock();
	MemcachedUnLock(MEMCACHED_USER_LOCK);
	return true;

}
//查询用户信息自身不带锁，调用他的函数加锁
int CMemCBusi::QueryUserInfo(uint32 nUserId, tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo& globalUserInfo)
{


	memset(&globalUserInfo, 0, sizeof(globalUserInfo));

	char key[128];

	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();

	if(c_Memcache->GetLinkState() == true) //和memcached正常连接状态
	{
		sprintf(key, "userinfo#%u#%u", GetServerMagic(),nUserId);
		string strMemGet;
		
		m_MemRc = c_memc->get(key,strMemGet);
		CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryUserInfo--key %s value %s ",key,strMemGet.c_str());

		if(m_MemRc != true || strMemGet.empty())
		{
			CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryUserInfo NO user %u",nUserId);
			return MEMCACHED_NO_INFO;
		}

		CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryUserInfo %s", strMemGet.c_str());

		Json::Reader jReader(Json::Features::strictMode());
		Json::Value jvUser;
		
		if((strMemGet[0] == '{') && (jReader.parse(strMemGet, jvUser)))
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
			globalUserInfo.flag = jvUser["flag"].asUInt();
			globalUserInfo.logintime = jvUser["logintime"].asUInt();
			//写的时候没写这个字段，读的时候却要读这个字段，临时
			globalUserInfo.version = jvUser["version"].asUInt();;
			//Base64Decode(jvUser["sessionkey"].asString()).c_str();
			string strSessionKey = Base64Decode(jvUser["sessionkey"].asString()).c_str();
			string strP2pSessionKey = Base64Decode(jvUser["p2psessionkey"].asString()).c_str();
			memcpy(globalUserInfo.sessionkey, strSessionKey.c_str(), strSessionKey.size()); 
			memcpy(globalUserInfo.p2psessionkey, strP2pSessionKey.c_str(), strP2pSessionKey.size());
			globalUserInfo.teamID = jvUser["csTeamID"].asUInt();
		}
		else
		{
			CLog::Log("CMemCBusi-QueryUserInfo", CLog::TYPE_ERROR, "QueryUserInfo json error %s",strMemGet.c_str());
			return MEMCACHED_JASON_ERROR;
		}
		return MEMCACHED_GET_INFO_SUCCESS;
	}
	else
	{		
		CLog::Log("CMemCBusi-QueryUserInfo", CLog::TYPE_ERROR, "MEMCACHED_SERVER_DOWN");
		return MEMCACHED_SERVER_DOWN;
	}
}

bool CMemCBusi::query_total_user_number(uint32 &total_count)//请求在线用户信息
{
	vector<int> server_info;
	string total_user;
	int total_user_count = 0;
	total_count = 0;
	char str_1[128] = {0};
	
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	if(false == QueryServerInfo(server_info))
	{
		return false;		
	}	
	//m_memcached_lock.Lock();
	for(vector<int>::iterator iter = server_info.begin();iter != server_info.end(); iter++)
	{
		sprintf(str_1,"totaluser#%u#%u",GetServerMagic(),*iter);	//获取某一个server下totalserver的值
		if( false == c_memc->get(string(str_1),total_user))
		{
			//m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: get %s fail",__FUNCTION__,__LINE__,str_1);
			return false;
		}
		total_user_count = atoi(total_user.c_str());
		total_count += total_user_count;
		
	}
	//m_memcached_lock.Unlock();
	return true;

}

int CMemCBusi::SearchOnlineUser(tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo *pUserInfo,uint8 & count,uint32& nOnlineNum,uint16 startpos)//请求在线用户信息
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

	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	if(c_Memcache->GetLinkState() == true)
	{
		QueryServerInfo(server_info);
		
		//m_memcached_lock.Lock();
		for(vector<int>::iterator iter = server_info.begin();iter != server_info.end(); iter++)
		{
			

			sprintf(str_1,"totaluser#%u#%u",GetServerMagic(),*iter);	//获取某一个server下totalserver的值
			if(false == c_memc->get(string(str_1),total_user))
			{
				//m_memcached_lock.Unlock();
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
					" %s %d: get %s fail",__FUNCTION__,__LINE__,str_1);
				return MEMCACHED_NO_INFO;
			}
			
			each_total_user_count = atoi(total_user.c_str());
			//count: query user number
			//left_count: total user in all
			//total_user_count :each server_id 's user number
			uint32 count_t = 1;
			//确定从哪个服务器开始取用户
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
				sprintf(str_2,"useronline#%u#%u#%u",GetServerMagic(),*iter,count_t);		
				if( false == c_memc->get(string(str_2),user_id))
				{
					//m_memcached_lock.Unlock();
					CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
						" %s %d: get %s fail",__FUNCTION__,__LINE__,str_2);
					return MEMCACHED_NO_INFO;
				}
			
				sprintf(str_3,"userinfo#%u#%s",GetServerMagic(),user_id.c_str());		
				if(false == c_memc->get(string(str_3),strMemGet))
				{
					//m_memcached_lock.Unlock();
					CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
						" %s %d: get %s fail",__FUNCTION__,__LINE__,str_3);
					return MEMCACHED_NO_INFO;
				}


				/*parse info*/			
				CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryUserInfo %s", strMemGet.c_str());
				
				tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo = {0};
				
				if(MEMCACHED_GET_INFO_SUCCESS == QueryUserInfo(atoi(user_id.c_str()),userInfo))
				{
					if(userInfo.status != 18 /*HIDE STATES*/ )
					{
						CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_DEBUG,
							"SearchOnlineUser_id %u",userInfo.id);
			
						
						pUserInfo[calc_get_user_count].clientid = userInfo.id;
						pUserInfo[calc_get_user_count].sexy = userInfo.sexy;
						pUserInfo[calc_get_user_count].status = userInfo.status;
						pUserInfo[calc_get_user_count].rule = userInfo.rule;
						strncpy(pUserInfo[calc_get_user_count].username,userInfo.username,MAX_USERNAME_LEN);
						strncpy(pUserInfo[calc_get_user_count].nickname, userInfo.nickname,MAX_USERNAME_LEN);
						 
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
					//m_memcached_lock.Unlock();
					query_total_user_number(nOnlineNum);
					return MEMCACHED_GET_INFO_SUCCESS;
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
		//m_memcached_lock.Unlock();
		query_total_user_number(nOnlineNum); //返回在线用户总数
		//获取totalserver的值
		return MEMCACHED_GET_INFO_SUCCESS;
	}
	else
	{
		CLog::Log("CMemCBusi-SearchOnlineUser", CLog::TYPE_ERROR, "MEMCACHED_SERVER_DOWN");
		return MEMCACHED_SERVER_DOWN;
	}
}


//查询用户控制信息
int CMemCBusi::QueryUserControlInfo(uint32 userId,tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo &userControlInfo)
{
	int ret = -1;
	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo;
	
	//m_memcached_lock.Lock();
	ret = QueryUserInfo(userId,userInfo);
	if(ret == MEMCACHED_GET_INFO_SUCCESS)
	{
		memcpy(userControlInfo.sessionKey, userInfo.sessionkey, sizeof(userControlInfo.sessionKey));
		memcpy(userControlInfo.p2pSessionKey, userInfo.p2psessionkey, sizeof(userControlInfo.p2pSessionKey));
		userControlInfo.srvip = userInfo.serverip;
		userControlInfo.uid = userId;
		userControlInfo.flag = 0;         /*   flag    好像没用*/
		//m_memcached_lock.Unlock();
		return MEMCACHED_GET_INFO_SUCCESS;
	}
	else if(ret == MEMCACHED_NO_INFO)
	{
		CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryUserInfo no userinfo");
		//memcpy(userControlInfo.sessionKey, userInfo.sessionkey, sizeof(userControlInfo.sessionKey));
		//memcpy(userControlInfo.p2pSessionKey, userInfo.p2psessionkey, sizeof(userControlInfo.p2pSessionKey));
		userControlInfo.srvip =0;
		userControlInfo.uid = userId;
		userControlInfo.flag = 0;         /*   flag    好像没用*/
		//m_memcached_lock.Unlock();
//		MemcachedUnLock(MEMCACHED_USER_LOCK);
		return MEMCACHED_NO_INFO;
	}
	else if(ret == MEMCACHED_SERVER_DOWN)
	{
		CLog::Log("CMemCBusi-QueryUserControlInfo", CLog::TYPE_DEBUG, "MEMCACHED_SERVER_DOWN");
		//m_memcached_lock.Unlock();		
//		MemcachedUnLock(MEMCACHED_USER_LOCK);
		return MEMCACHED_SERVER_DOWN;
	}
	
	CLog::Log("CMemCBusi-QueryUserControlInfo", CLog::TYPE_DEBUG, "RETURN:%d",ret);
	return ret;
}

int CMemCBusi::GetTUserInfo(uint32 userId,tagGSRV_TUSERINFO_ACK &TUserInfo)
{
	int ret = -1;
	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo;
	//m_memcached_lock.Lock();
	ret = QueryUserInfo(userId,userInfo);
	if( ret == MEMCACHED_GET_INFO_SUCCESS )
	{
		TUserInfo.ret = RESULT_SUCCESS;
		TUserInfo.id =userId;		
		TUserInfo.ip = userInfo.clientip;
		TUserInfo.port = userInfo.clientport;		
		memcpy(TUserInfo.sessionkey, userInfo.sessionkey, sizeof(TUserInfo.sessionkey));
		//m_memcached_lock.Unlock();
		return MEMCACHED_GET_INFO_SUCCESS;
	}
	else if( ret == MEMCACHED_NO_INFO )
	{
		CLog::Log("CMemCBusi-GetTUserInfo", CLog::TYPE_DEBUG, "GetTUserInfo no userinfo");
		//memcpy(userControlInfo.sessionKey, userInfo.sessionkey, sizeof(userControlInfo.sessionKey));
		//memcpy(userControlInfo.p2pSessionKey, userInfo.p2psessionkey, sizeof(userControlInfo.p2pSessionKey));
		TUserInfo.ret = RESULT_FAILED;
		TUserInfo.id =userId;		
		//m_memcached_lock.Unlock();
		return MEMCACHED_NO_INFO;
	}
	else if( ret == MEMCACHED_SERVER_DOWN )
	{
		CLog::Log("CMemCBusi-GetTUserInfo", CLog::TYPE_DEBUG, "MEMCACHED_SERVER_DOWN");
		//m_memcached_lock.Unlock();
		return MEMCACHED_JASON_ERROR;

	}
	
	CLog::Log("CMemCBusi-GetTUserInfo", CLog::TYPE_DEBUG, "RETURN:%d",ret);
	return ret;
}



//查询用户状态
int CMemCBusi::QueryUserStatus(uint32 userId, tagGSRV_GET_USER_STATUS_ACK::tagUserStatus &userStatus)
{
	int ret = -1;
	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo;
	
//	//m_memcached_lock.Lock();
	ret = QueryUserInfo(userId, userInfo);	
	if(ret == MEMCACHED_GET_INFO_SUCCESS)
	{
		userStatus.id = userId;
		userStatus.status = userInfo.status;		
	}
//	//m_memcached_lock.Unlock();
	return ret;
}

//修改用户状态
bool CMemCBusi::ModiUserStatus(tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser user)
{
	//key
	char key[128];
	sprintf(key, "userinfo#%u#%u",GetServerMagic(), user.id);

	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo;

//可能造成死锁
//	//m_memcached_lock.Lock();
	int ret = QueryUserInfo(user.id, userInfo);
	if(ret != MEMCACHED_GET_INFO_SUCCESS)
	{
//		//m_memcached_lock.Unlock();	
		return ret;
	}
//	//m_memcached_lock.Unlock();

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
	userInfoModify.logintime = userInfo.logintime;
	strncpy(userInfoModify.sessionkey, userInfo.sessionkey,MAX_SESSIONKEY_LEN);
	strncpy(userInfoModify.p2psessionkey ,userInfo.p2psessionkey,MAX_SESSIONKEY_LEN);
	userInfoModify.teamID = userInfo.teamID;
	
	userInfoModify.status = user.status;
	AddModiUserInfo(userInfoModify);
	
	//CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "ModiUserStatus user:%u",userInfoModify.id);
	return true;
}


//查询分流
int CMemCBusi::QueryUserFlow(uint32 userId,tagGSRV_GET_USER_FLOW_ACK::tagUserFlow &userFlow)
{
	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo;
	int ret = -1;
	//CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryUserStatus");
	//m_memcached_lock.Lock();
	ret = QueryUserInfo(userId, userInfo);
	if( ret == MEMCACHED_GET_INFO_SUCCESS)
	{
		userFlow.id = userId;
		userFlow.status = userInfo.status;
		userFlow.flow = userInfo.flow;
	}

	//m_memcached_lock.Unlock();
	return ret;
}


//修改分流
bool CMemCBusi::ModiUserFlow(const tagGSRV_MODIFY_USER_FLOW_QUERY::tagFlow UserFlow)
{
	//key
	char key[128];
	sprintf(key, "userinfo#%u#%u",GetServerMagic(), UserFlow.id);

	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo;
	//m_memcached_lock.Lock();
	
	//MemcachedTryLock(MEMCACHED_USER_LOCK);
	if(QueryUserInfo(UserFlow.id, userInfo) != MEMCACHED_GET_INFO_SUCCESS)
	{
		//m_memcached_lock.Unlock();	
		
	//	MemcachedUnLock(MEMCACHED_USER_LOCK)
		return false;
	}
	//m_memcached_lock.Unlock();
	

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
	userInfoModify.logintime = userInfo.logintime;
	strncpy(userInfoModify.sessionkey, userInfo.sessionkey,MAX_SESSIONKEY_LEN);
	strncpy(userInfoModify.p2psessionkey ,userInfo.p2psessionkey,MAX_SESSIONKEY_LEN);	

	userInfoModify.flow = UserFlow.flow;

	AddModiUserInfo(userInfoModify);
	
	//CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "ModiUserStatus user:%u",userInfoModify.id);
	return true;
}

//查询用户所在客服组ID
int CMemCBusi::QueryUserTeamID(uint32 userId, tagGSRV_GET_USER_TEAMID_ACK::tagUserTeamID& userTeamID)
{
	int ret = -1;
	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo;
	
	//m_memcached_lock.Lock();
	ret = QueryUserInfo(userId, userInfo);	
	if(ret == MEMCACHED_GET_INFO_SUCCESS)
	{
		userTeamID.id = userId;
		userTeamID.teamID = userInfo.teamID;		
	}
	//m_memcached_lock.Unlock();
	return ret;
}

//修改用户所在客服组ID

bool CMemCBusi::ModiUserTeamID(tagGSRV_MODIFY_ONLINE_TEAMID_QUERY::tagUserTeamID user)
{
	//key
	char key[128];
	sprintf(key, "userinfo#%u#%u",GetServerMagic(), user.id);

	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo;

//可能造成死锁
//	//m_memcached_lock.Lock();
	int ret = QueryUserInfo(user.id, userInfo);
	if(ret != MEMCACHED_GET_INFO_SUCCESS)
	{
//		//m_memcached_lock.Unlock();	
		return ret;
	}
//	//m_memcached_lock.Unlock();

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
	userInfoModify.logintime = userInfo.logintime;
	strncpy(userInfoModify.sessionkey, userInfo.sessionkey,MAX_SESSIONKEY_LEN);
	strncpy(userInfoModify.p2psessionkey ,userInfo.p2psessionkey,MAX_SESSIONKEY_LEN);
	userInfoModify.status = userInfo.status;
	
	userInfoModify.teamID = user.teamID;	
	AddModiUserInfo(userInfoModify);
	
	//CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "ModiUserStatus user:%u",userInfoModify.id);
	return true;
}


#endif



bool CMemCBusi::AddModiCSInfo(const CSinfo_t globalCSInfo,uint8 type,uint32 ServIp)
{
	char str_a[128] = {0};
	char str_3[128] = {0};
	string check;
	m_MemRc = true; 
	//ImMemCache &Memcache =	ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, 
			" ADD CS %u",globalCSInfo.m_nID);
	//m_memcached_lock.Lock();
	MemcachedTryLock(MEMCACHED_CS_LOCK);
	
	if(type == tagGSRV_ADDMODIFY_CSINFO_QUERY::ACTION_ADD )//add info
	{
		//查询已经存在的客服列表
		sprintf(str_a,"CServiceList#%u#%u",GetServerMagic(),GetServerId());
		//cout << "check " << str_a << endl;
		c_memc->get(string(str_a),check);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
						" %s %d:  get key: %s value : %s",__FUNCTION__,__LINE__,str_a,check.c_str()); 

		//读出数据，并增加自己
			
		Json::Reader jReader(Json::Features::strictMode());
		Json::Value jvCS;
		Json::Value ids;
		Json::FastWriter jFastWriter;
		string strJvCS;
		if(!check.empty())
		{	
			if((check[0] == '{') && (jReader.parse(check, jvCS)))
			{
				ids = jvCS["ids"];
			}	
			else
			{
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
					" %s %d: add_user_info json error %s",__FUNCTION__,__LINE__,check.c_str()); 
				//m_memcached_lock.Unlock();
				MemcachedUnLock(MEMCACHED_CS_LOCK);
				return false;
			}
		}
	//防止重复,后面这段尽量自己保证---效率不高
		bool find_flag = false;
		for(int pos = 0 ; pos < (int)ids.size();pos++)
		{
			if(ids[pos].asUInt() == globalCSInfo.m_nID)
			{
				find_flag = true;
				break;
			}
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
				"查找到 %u  %u",globalCSInfo.m_nID,ids[pos].asUInt());
		}

		if(find_flag == false)
		{		
			
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
							"客服列表中没有找到客服 %u ",globalCSInfo.m_nID);
			ids.append(globalCSInfo.m_nID);

		}
		else
		{
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
							"客服列表中找到重复客服 %u",globalCSInfo.m_nID);
			

		}
		jvCS["ids"] = ids;
		strJvCS = jFastWriter.write(jvCS);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
				" %s %d: set key : %s value : %s ",__FUNCTION__,__LINE__,str_a,strJvCS.c_str());
		if( false == c_memc->set(string(str_a),strJvCS.c_str())) 
		{			
			//m_memcached_lock.Unlock();
			MemcachedUnLock(MEMCACHED_CS_LOCK);
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
				" %s %d: set %s fail",__FUNCTION__,__LINE__,str_3);
			return false;
		}			
		
	}
			
	//增加CServiceInfo 索引 
	sprintf(str_3,"CServiceInfo#%u#%u",GetServerMagic(),globalCSInfo.m_nID);
	
	/*add user context here*/

	//value为json串
	Json::Value jvUser;
	Json::Value Newids;

	Json::FastWriter jFastWriter;
	string strJvUser;

	jvUser["CServiceId"] = globalCSInfo.m_nID;
	jvUser["CServiceName"] = globalCSInfo.m_Csname;
	jvUser["State"] = globalCSInfo.m_State;
	jvUser["Recption"] = globalCSInfo.m_CurReception;	
	jvUser["RecpLimit"] = globalCSInfo.m_MaxRecConsult;
	jvUser["Consult"] = globalCSInfo.m_CurConsult;
	jvUser["ChattingBuyerId"] = globalCSInfo.m_nChattingBuyerID;	
	jvUser["DirectorId"] = globalCSInfo.m_nDirectorID;
	jvUser["Permission"] = globalCSInfo.m_Permission;
	for(uint32 i = 0; i < globalCSInfo.m_Customerid.size();i++)
	{
		Newids.append(globalCSInfo.m_Customerid[i]);
	}	
	
	jvUser["Customerlist"] = Newids;	
	strJvUser = jFastWriter.write(jvUser);	
		
	/*将客服信息写入*/
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
		" %s %d: set key : %s value : %s ",__FUNCTION__,__LINE__,str_3,strJvUser.c_str());
	if( false == c_memc->set(string(str_3),strJvUser.c_str()))
	{
		c_memc->remove(string(str_a));
		//m_memcached_lock.Unlock();
		MemcachedUnLock(MEMCACHED_CS_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,str_3);
		return false;
	}		
	c_memc->expire(str_3,seller_Timeout);

	//m_memcached_lock.Unlock();
	MemcachedUnLock(MEMCACHED_CS_LOCK);
	return true;
}

bool CMemCBusi::DelCSInfo(uint32 nUserId,uint32 ServIp)
{	
	char str_a[128] = {0};
	char str_3[128] = {0};
	string check;
	m_MemRc = true; 
	//ImMemCache &Memcache =	ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, 
			" DELETE CS %u",nUserId);
	//m_memcached_lock.Lock();
	MemcachedTryLock(MEMCACHED_CS_LOCK);
	

	//查询已经存在的客服列表
	
	sprintf(str_a,"CServiceList#%u#%u",GetServerMagic(),GetServerId());
	c_memc->get(string(str_a),check);
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
					" %s %d:  get key: %s value : %s",__FUNCTION__,__LINE__,str_a,check.c_str()); 

	//读出数据，并减少自己
		
	Json::Reader jReader(Json::Features::strictMode());
	Json::Value jvCS;
	Json::Value ids;
	Json::FastWriter jFastWriter;
	string strJvCS;
	if(!check.empty())		//非空
	{	if((check[0] == '{' ) && (jReader.parse(check, jvCS)))
		{
			ids = jvCS["ids"];			
		}	
		else
		{
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: add_user_info json error %s",__FUNCTION__,__LINE__,check.c_str()); 
			//m_memcached_lock.Unlock();			
			MemcachedUnLock(MEMCACHED_CS_LOCK);
			return false;
		}
		int pos = -1;
		uint32 val = -1;
		for(int i= 0; i < (int)ids.size();i++)
		{
			val = ids[i].asUInt();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
				"%s %d :本地元素 %u  需要查找客服 %u ",__FUNCTION__,__LINE__,val,nUserId);
			if(ids[i].asUInt() == nUserId)
			{
				pos = i;
				break;							
			}
		}
		if(pos != -1)
		{
			ids[pos] = ids[ids.size()-1];
			ids.resize(ids.size()-1);
		}
		else
		{
			//m_memcached_lock.Unlock();
			MemcachedUnLock(MEMCACHED_CS_LOCK);
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
				" %s %d: 没有找到需要删除的客服 %d fail",__FUNCTION__,__LINE__,nUserId);
			return false;
		}
	}
	else
	{
		//m_memcached_lock.Unlock();
		MemcachedUnLock(MEMCACHED_CS_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
				" %s %d: CServiceList is empty",__FUNCTION__,__LINE__);
		
		return true;
	}

	char str_m[32] = {0};
	sprintf(str_m,"%u",nUserId);	

	jvCS["ids"] = ids;
	
	strJvCS = jFastWriter.write(jvCS);
	if( false == c_memc->set(string(str_a),strJvCS.c_str())) 
	{			
		//m_memcached_lock.Unlock();
		MemcachedUnLock(MEMCACHED_CS_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: set %s fail",__FUNCTION__,__LINE__,str_3);
		return false;
	}			
	//删除CServiceInfo 信息
	sprintf(str_3,"CServiceInfo#%u#%u",GetServerMagic(),nUserId);	

	if( false == c_memc->remove(string(str_3)))
	{
		//恢复列表信息
		c_memc->set(string(str_a),check);
		//m_memcached_lock.Unlock();
		MemcachedUnLock(MEMCACHED_CS_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: delete CServiceInfo %s fail",__FUNCTION__,__LINE__,str_3);
		return false;
	}
	//m_memcached_lock.Unlock();
	MemcachedUnLock(MEMCACHED_CS_LOCK);
	return true;
}

int CMemCBusi::QueryCSInfo(uint32 nUserId, CSinfo_t& CSInfo)
{
	char str_3[128] = {0};
	string CSinfo;
	m_MemRc = true; 
	//ImMemCache &Memcache =	ImMemCache::GetInstance();
//	global_Cache *c_memc = c_memc->GetLinks();
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, 
			" QueryCSInfo CS %u",nUserId);
	
	if(c_Memcache->GetLinkState() == true) //和memcached正常连接状态
	{
//		//m_memcached_lock.Lock();
				
		//查询CServiceInfo 索引 
		sprintf(str_3,"CServiceInfo#%u#%u",GetServerMagic(),nUserId);
		c_memc->get(string(str_3),CSinfo);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
						" %s %d: KEY: %s VALUE: %s ",__FUNCTION__,__LINE__,str_3,CSinfo.c_str());

		Json::Reader jReader(Json::Features::strictMode());
		Json::Value jvCS;
		Json::Value buyerIDs;
		Json::FastWriter jFastWriter;
		string strJvCS;
	/*
			回复格式
			{"CServiceId":"001","CServiceName":"客服001","State":1,"Recption"：1,
			"Consult":1,"RecpLimit":10," Customerlist":[0,1,2,3,4]}
	*/
		if(!CSinfo.empty())
		{	if((CSinfo[0] == '{') && (jReader.parse(CSinfo, jvCS)))
			{
				CSInfo.m_nID =  jvCS["CServiceId"].asUInt();
				if(jvCS.isMember("CServiceName"))
					strcpy(CSInfo.m_Csname,jvCS["CServiceName"].asString().c_str());
				if(jvCS.isMember("State"))
					CSInfo.m_State=		jvCS["State"].asUInt();
				if(jvCS.isMember("Recption"))	
					CSInfo.m_CurReception = jvCS["Recption"].asInt();
				if(jvCS.isMember("Consult"))	
					CSInfo.m_CurConsult = jvCS["Consult"].asUInt();
				if(jvCS.isMember("RecpLimit"))		
					CSInfo.m_MaxRecConsult = jvCS["RecpLimit"].asUInt();
				if(jvCS.isMember("ChattingBuyerId"))		
					CSInfo.m_nChattingBuyerID = jvCS["ChattingBuyerId"].asUInt();
				if(jvCS.isMember("DirectorId"))			
					CSInfo.m_nDirectorID = jvCS["DirectorId"].asUInt();
				if(jvCS.isMember("Permission"))			
					CSInfo.m_Permission = jvCS["Permission"].asUInt();
				if(jvCS.isMember("Customerlist"))		
				{
					buyerIDs = jvCS["Customerlist"];
					Json::Reader jReadVec(Json::Features::strictMode());
					if((CSinfo[0] == '{') && jReader.parse(CSinfo, jvCS) && !buyerIDs.isNull())
					for(int i = 0; i < (int)buyerIDs.size();i++)
					{
						CSInfo.m_Customerid.push_back(buyerIDs[i].asUInt()); 
					}
				}
				
//				//m_memcached_lock.Unlock();	
				return MEMCACHED_GET_INFO_SUCCESS;
			}	
			else
			{
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
					" %s %d: add_user_info json error %s",__FUNCTION__,__LINE__,CSinfo.c_str()); 
//				//m_memcached_lock.Unlock();
				return MEMCACHED_JASON_ERROR;
			}
		}
		else
		{		
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: NO USERINFO %s",__FUNCTION__,__LINE__,str_3); 
//			//m_memcached_lock.Unlock();
			return MEMCACHED_NO_INFO;
		}	
	}
	else
	{
		return MEMCACHED_SERVER_DOWN;
	}
}



int CMemCBusi::QueryCSQueueLength(const uint32 queueid,tagGSRV_GET_ONLINE_CSQLEN_ACK::CSQinfo &csQlen)
{
	char str_3[128] = {0};
	string CSinfo;
	m_MemRc = true; 
	//ImMemCache &Memcache =	ImMemCache::GetInstance();
//	global_Cache *c_memc = c_memc->GetLinks();
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, 
			" QueryCSQueueLength CS %u",queueid);
	
	if(c_Memcache->GetLinkState() == true) //和memcached正常连接状态
	{
//		//m_memcached_lock.Lock();
				
		//查询CServiceInfo 索引 
		uint64_t queuelength = 0;
		sprintf(str_3,"CsQueue#%u#%u",GetServerMagic(),queueid);
		

		if( false == c_memc->zcard(str_3,&queuelength))
		{
			//m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: get %s fail",__FUNCTION__,__LINE__,str_3);
			csQlen.queueLength = 0;
			csQlen.teamId = queueid;
			return MEMCACHED_NO_INFO;
		}
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
						" %s %d: KEY: %s VALUE: %lu ",__FUNCTION__,__LINE__,str_3,queuelength);

		csQlen.queueLength = queuelength;
		csQlen.teamId = queueid;
		return MEMCACHED_GET_INFO_SUCCESS;
	}
	else
	{
		csQlen.queueLength = 1;
		csQlen.teamId = queueid;
		return MEMCACHED_SERVER_DOWN;
	}
}


/*与zookeeper相关，暂时未维护*/
int CMemCBusi::SearchDownSerUserinfo(tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo *pUserInfo,uint8 & count,uint32& nOnlineNum,uint16 startpos,uint32 serip)//请求在线用户信息
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

	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	if(c_Memcache->GetLinkState() == true)
	{
		//QueryServerInfo(server_info);
		
		//m_memcached_lock.Lock();			

		sprintf(str_1,"totaluser#%u#%u",GetServerMagic(),serip);	//获取某一个server下totalserver的值
		if(false == c_memc->get(string(str_1),total_user))
		{
			//m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: get %s fail",__FUNCTION__,__LINE__,str_1);
			return MEMCACHED_NO_INFO;
		}
		each_total_user_count = atoi(total_user.c_str());
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
				" %s %d: QUERY %s ans %d",__FUNCTION__,__LINE__,str_1,each_total_user_count);
		
		for(uint16 count_t = startpos+1;count_t <= each_total_user_count;count_t++)
		{
			sprintf(str_2,"useronline#%u#%u#%u",GetServerMagic(),serip,count_t);		
			if( false == c_memc->get(string(str_2),user_id))
			{
				//m_memcached_lock.Unlock();
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
					" %s %d: get %s fail",__FUNCTION__,__LINE__,str_2);
				return MEMCACHED_NO_INFO;
			}
		
			sprintf(str_3,"userinfo#%u#%s",GetServerMagic(),user_id.c_str());		
			if(false == c_memc->get(string(str_3),strMemGet))
			{
				//m_memcached_lock.Unlock();
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
					" %s %d: get %s fail",__FUNCTION__,__LINE__,str_3);
				return MEMCACHED_NO_INFO;
			}


			/*parse info*/			
			CLog::Log("CMemCBusi", CLog::TYPE_DEBUG, "QueryUserInfo %s", strMemGet.c_str());
			
			tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo userInfo = {0};
			
			if(MEMCACHED_GET_INFO_SUCCESS == QueryUserInfo(atoi(user_id.c_str()),pUserInfo[calc_get_user_count]))
			{
				if(pUserInfo[calc_get_user_count].status != 18 /*HIDE STATES*/ )
				{					 
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
				//m_memcached_lock.Unlock();
				nOnlineNum = each_total_user_count; //返回在线用户总数
				return MEMCACHED_GET_INFO_SUCCESS;
			}	
			/*end parse info*/

			user_id.clear();
			strMemGet.clear();
			memset(str_2,0,128);
			memset(str_3,0,128);
		}
	
		count = calc_get_user_count;
		//m_memcached_lock.Unlock();
		nOnlineNum = each_total_user_count; //返回在线用户总数
		//获取totalserver的值
		return MEMCACHED_GET_INFO_SUCCESS;
	}
	else
	{
		CLog::Log("CMemCBusi-SearchOnlineUser", CLog::TYPE_ERROR, "MEMCACHED_SERVER_DOWN");
		return MEMCACHED_SERVER_DOWN;
	}
}





#if 0
bool CMemCBusi::FetchKeysValues(vector<string> &keys,vector<string> &values)
{
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
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
#endif
bool CMemCBusi::BestAccountProcess(uint32 merchant_id, uint32 group_id, vector<uint32>& accounts, uint32 flow_type, uint32& id)
{
	char tmp_merchant_group[128] = {0};
	char tmp_best_id[32] = {0};
	string str_account;
	uint32 tmp_account = 0;
	uint32 best_id = 0;

	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();

	if(flow_type == 0)
	{
		sprintf(tmp_merchant_group,"Merchant_ID#%u#%u",GetServerMagic(),merchant_id);
	}
	else
	{
		sprintf(tmp_merchant_group,"Group_ID#%u#%u",GetServerMagic(),group_id);		
	}	
	c_memc->get(string(tmp_merchant_group),str_account);
	if(str_account.empty())
	{
		tmp_account = 0;
	}
	else
	{
		tmp_account = (uint32)atoi(str_account.c_str());
	}
	
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, " %s %d: tmp_account is %u",__FUNCTION__,__LINE__,tmp_account);
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
	
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, " %s %d: set best id is %s",__FUNCTION__,__LINE__,tmp_best_id);
	if(false == c_memc->set(tmp_merchant_group,tmp_best_id))
	{				
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, " %s %d: set %s fail",__FUNCTION__,__LINE__,tmp_merchant_group);
		return false;
	}
	id = best_id;
	return true;
}






bool CMemCBusi::GetBestAccount(uint32 merchant_id, uint32 group_id, vector<uint32>& ids, uint32 flow_type, uint32& id)
{

	vector<string> Merchants_keys;
	vector<string> Merchants_values;
	vector<uint32> accounts;
	bool success_rt = true;
	string tmp1_string;
	accounts.clear();

	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo UserInfo;
	
	for(vector<uint32>::iterator iter = ids.begin() ; iter != ids.end(); iter++)
	{
		//m_memcached_lock.Lock();
//		success_rt &= QueryUserStatus(*iter, userStatus);
//		success_rt &= QueryUserFlow(*iter,userFlow);
		success_rt = QueryUserInfo(*iter,UserInfo);
//		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, " %s %d: search user info ret : %d",__FUNCTION__,__LINE__,success_rt);
		
		//m_memcached_lock.Unlock();
		if(success_rt == MEMCACHED_GET_INFO_SUCCESS && 
			UserInfo.status != XTREAM_OFFLINE &&
			UserInfo.status != XTREAM_LEAVE &&
			UserInfo.flow == XT_MOD_HANGUP_STATE_REQ::RECEPTION) /*HIDE STATES*/
		{
			accounts.push_back(*iter);
		}		
	}
	if(! accounts.empty())
	{
		sort(accounts.begin(),accounts.end());
		//m_memcached_lock.Lock();
		if(false == BestAccountProcess(merchant_id,group_id,accounts,flow_type,id))
		{
			//m_memcached_lock.Unlock();
			return false;
		}	
		//m_memcached_lock.Unlock();
		return true;
	}
	else
	{		
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, " %s %d: accounts empty ",__FUNCTION__,__LINE__);
		return false;
	}

}


int CMemCBusi::GetBestCSAccount(uint32 merchant_id, uint32 group_id, vector<uint32>& ids, uint32 flow_type, uint32& id)
{

	vector<string> Merchants_keys;
	vector<string> Merchants_values;
	vector<uint32> accounts;
	bool success_rt = true;
	string tmp1_string;
	accounts.clear();
	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo UserInfo = {0};
	CSinfo_t CSInfo = {0};
	uint32 offlineCount = 0;
	for(vector<uint32>::iterator iter = ids.begin() ; iter != ids.end(); iter++)
	{
		//m_memcached_lock.Lock();
//		success_rt &= QueryUserStatus(*iter, userStatus);
//		success_rt &= QueryUserFlow(*iter,userFlow);
		success_rt = QueryUserInfo(*iter,UserInfo);
		success_rt = QueryCSInfo(*iter,CSInfo);



		CLog::Log("GetBestCSAccount", CLog::TYPE_IMPORTANT, " %s %d: search user info ret : %d",__FUNCTION__,__LINE__,success_rt);
		CLog::Log("GetBestCSAccount", CLog::TYPE_IMPORTANT, " 客服 %u 当前咨询量  %u , 客服最大接待量 %u ",CSInfo.m_nID,CSInfo.m_CurConsult,CSInfo.m_MaxRecConsult);
		
		//m_memcached_lock.Unlock();
		
		CLog::Log("GetBestCSAccount", CLog::TYPE_IMPORTANT, "success_rt %d state %d flow %d  ",success_rt,UserInfo.status,UserInfo.flow);
		if(success_rt == MEMCACHED_GET_INFO_SUCCESS &&
			UserInfo.status != XTREAM_OFFLINE &&
			UserInfo.status != XTREAM_LEAVE &&
			UserInfo.status != XTREAM_BUSY &&
			/*  	UserInfo.flow == XT_MOD_HANGUP_STATE_REQ::RECEPTION */    //设置挂起么？
			CSInfo.m_CurConsult < CSInfo.m_MaxRecConsult ) /*HIDE STATES*/
		{
		
			CLog::Log("GetBestCSAccount", CLog::TYPE_IMPORTANT, "add accounts %u ",*iter);
			accounts.push_back(*iter);
		}
		if(CSInfo.m_nID == 0 && UserInfo.id == 0)	//没获取在线的用户
		{
			offlineCount++;
		}
	}
	if(! accounts.empty())
	{
		sort(accounts.begin(),accounts.end());
		//m_memcached_lock.Lock();
		if(false == BestAccountProcess(merchant_id,group_id,accounts,flow_type,id))
		{			
		
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, "BestAccountProcess failed id %u",id);
			//m_memcached_lock.Unlock();
			return NO_USABLE_CS;
		}	
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, "BestAccountProcess success id %u",id);
		//m_memcached_lock.Unlock();
		return CS_SUCCESS;
	}
	else if(offlineCount == ids.size())
	{
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, " %s %d: 没有客服在线 ",__FUNCTION__,__LINE__);
		return NO_CS_ONLINE;
	}
	else 
	{		
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, " %s %d: accounts empty ",__FUNCTION__,__LINE__);
		return NO_USABLE_CS;
	}

}



#if 0
int CMemCBusi::QueryCSInfo(uint32 nUserId, ClientService& clientService)
{
	
	uint64_t value = 1;
	char str_a[128] = {0};
	char str_b[128] = {0};
	char str_1[128] = {0};
	char str_2[128] = {0};
	char str_3[128] = {0};
	CSinfo_t CSInfo;
	string CSinfo;
	unsigned int user_index = 0;
	m_MemRc = true; 
	ImMemCache &Memcache =	ImMemCache::GetInstance();
	global_Cache *c_memc = c_memc->GetLinks();
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" ADD CS %u",globalCSInfo.id);
	//m_memcached_lock.Lock();
			
	//查询CServiceInfo 索引 
	sprintf(str_3,"CServiceList#%u#%u",GetServerMagic(),nUserId);
	c_memc->get(string(str_3),CSinfo);

	Json::Reader jReader;
	Json::Value jvCS;
	Json::Value buyerIDs;
	Json::FastWriter jFastWriter;
	string strJvCS;
/*
		回复格式
		{"CServiceId":"001","CServiceName":"客服001","State":1,"Recption"：1,
		"Consult":1,"RecpLimit":10," Customerlist":[0,1,2,3,4]}
*/
	if(!CSinfo.empty())
	{	if(jReader.parse(CSinfo, jvCS))
		{
			clientService.m_nID = jvCS["CServiceId"];
			clientService.m_CSname = jvCS["CServiceName"];
			clientService.m_State = jvCS["State"];
			clientService.m_CurConsult = 

			clientService.m_CurReception = jvCS["Recption"];
			clientService.m_CurConsult = jvCS["Consult"];
			clientService.m_MaxRecConsult = jvCS["RecpLimit"];
			buyerIDs = jvCS["Customerlist"];
			for(int i = 0; i < buyerIDs.size();i++)
			{
				clientService.m_Customerid.push_back(buyerIDs[i]); 
			}		
			
			//m_memcached_lock.Unlock();	
			return true;
		}	
		else
		{
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: add_user_info json error %s",__FUNCTION__,__LINE__,CSinfo.c_str()); 
			//m_memcached_lock.Unlock();
			return false;
		}
	}
	else
	{		
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: NO USERINFO %s",__FUNCTION__,__LINE__,str_3); 
		//m_memcached_lock.Unlock();
		return false;
	}	
}

#endif

bool CMemCBusi::AddDelCSQueueInfo(const tagGSRV_ADDMODIFY_CSQINFO_QUERY::CSinfo globalCSQInfo,uint8 type)
	{
			char str_a[128] = {0};
			string check;
			m_MemRc = true; 
			//ImMemCache &Memcache =	ImMemCache::GetInstance();
			//global_Cache *c_memc = c_memc->GetLinks();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, 
					" 1ADD CS-QUEUE teamID %u customerID %u customerLevel %u customerReqTime %u",
					globalCSQInfo.teamID,globalCSQInfo.customerID,
					globalCSQInfo.customerLevel,globalCSQInfo.customerReqTime);
			//m_memcached_lock.Lock();	
		
			MemcachedTryLock(MEMCACHED_QUEUE_LOCK);
			//查询已经存在的客服列表
			sprintf(str_a,"CSQueue#%u#%u",GetServerMagic(),globalCSQInfo.teamID);
			//cout << "check " << str_a << endl;
			c_memc->get(string(str_a),check);
			//读出数据，并增加自己
				
			Json::Reader jReader(Json::Features::strictMode());
			Json::Value jvCS;
			Json::Value ids;
			Json::Value eachValue;
			Json::FastWriter jFastWriter;
			string strJvCS;
			if(!check.empty())	//非空
			{	if((check[0] == '{') && jReader.parse(check, jvCS))
				{
					ids = jvCS["customerList"];
				}	
				else
				{
					CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
						" %s %d: add_user_info json error %s",__FUNCTION__,__LINE__,check.c_str()); 
					//m_memcached_lock.Unlock();
					MemcachedUnLock(MEMCACHED_QUEUE_LOCK);
					return false;
				}
				if(type == tagGSRV_ADDMODIFY_CSQINFO_QUERY::ACTION_DEL )	//非空才能删除
				{
					char tmp_customerID[128] = {0};
					sprintf(tmp_customerID,"%u",globalCSQInfo.customerID);
					int pos = -1;
					uint32 val = -1;
					for(int i= 0; i < (int)ids.size();i++)
					{
						val = ids[i]["iD"].asUInt();
						CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
							"%s %d :本地元素 %u  需要查找元素 %u ",__FUNCTION__,__LINE__,val,globalCSQInfo.customerID);
						if(val == globalCSQInfo.customerID)
						{
							pos = i;
							break;							
						}
					}
					if(pos != -1)
					{
						ids[pos] = ids[ids.size()-1];
						ids.resize(ids.size()-1);
					}
					else
					{
						//m_memcached_lock.Unlock();
						MemcachedUnLock(MEMCACHED_QUEUE_LOCK);
						CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
							" %s %d: 没有找到需要删除的元素 %s fail",__FUNCTION__,__LINE__,strJvCS.c_str());
						return false;
					}
					strJvCS = jFastWriter.write(jvCS);
					CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
							" %s %d: set %s",__FUNCTION__,__LINE__,strJvCS.c_str());

					//ids.removeMember(tmp_customerID);			
					jvCS["customerList"] = ids;
					strJvCS = jFastWriter.write(jvCS);
					CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
						" %s %d: set %s ",__FUNCTION__,__LINE__,strJvCS.c_str());

					if( false == c_memc->set(string(str_a),strJvCS.c_str())) 
					{			
						//m_memcached_lock.Unlock();
						MemcachedUnLock(MEMCACHED_QUEUE_LOCK);
						CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
							" %s %d: set %s fail",__FUNCTION__,__LINE__,strJvCS.c_str());
						return false;
					}			
				}
			}		
			if(type == tagGSRV_ADDMODIFY_CSQINFO_QUERY::ACTION_ADD )//add info
			{

				eachValue["iD"] = globalCSQInfo.customerID;
				eachValue["requestTime"] = globalCSQInfo.customerReqTime;
				eachValue["level"] = globalCSQInfo.customerLevel;
				eachValue["teamID"] = globalCSQInfo.teamID;
				ids.append(eachValue);
				jvCS["customerList"] = ids;
				strJvCS = jFastWriter.write(jvCS);
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
						" %s %d: set key %s : value : %s ",__FUNCTION__,__LINE__, str_a,strJvCS.c_str());
				if( false == c_memc->set(string(str_a),strJvCS)) 
				{			
					//m_memcached_lock.Unlock();
					MemcachedUnLock(MEMCACHED_QUEUE_LOCK);
					CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
						" %s %d: set key : %s value : %s fail",__FUNCTION__,__LINE__, str_a,strJvCS.c_str());
					return false;
				}			
			}		
			//m_memcached_lock.Unlock();
			MemcachedUnLock(MEMCACHED_QUEUE_LOCK);
			return true;
		}

#if 0
bool CMemCBusi::AddModiCSQueueInfo(uint32 teamID,uint32 customerID,uint8 type)
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
	ImMemCache &Memcache =	ImMemCache::GetInstance();
	global_Cache *c_memc = c_memc->GetLinks();
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, 
			" ADD CS-QUEUE teamID %u customerID %u",teamID,customerID);
	//m_memcached_lock.Lock();	

	//查询已经存在的客服列表
	sprintf(str_a,"CSQueue#%u#%u",GetServerMagic(),teamID);
	//cout << "check " << str_a << endl;
	c_memc->get(string(str_a),check);
	//读出数据，并增加自己
		
	Json::Reader jReader;
	Json::Value jvCS;
	Json::Value ids;
	Json::FastWriter jFastWriter;
	string strJvCS;
	if(!check.empty())	//非空
	{	if(jReader.parse(check, jvCS))
		{
			ids = jvCS["customerList"];
		}	
		else
		{
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: add_user_info json error %s",__FUNCTION__,__LINE__,check.c_str()); 
			//m_memcached_lock.Unlock();
			return false;
		}
		if(type == tagGSRV_ADDMODIFY_CSQINFO_QUERY::ACTION_DEL )	//非空才能删除
		{
			char tmp_customerID[128] = {0};
			sprintf(tmp_customerID,"%u",customerID);
			ids.removeMember(tmp_customerID);			
			jvCS["ids"] = ids;
			strJvCS = jFastWriter.write(jvCS);
			if( false == c_memc->set(string(str_a),strJvUser.c_str())) 
			{			
				//m_memcached_lock.Unlock();
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
					" %s %d: set %s fail",__FUNCTION__,__LINE__,str_3);
				return false;
			}			
		}
	}		
	if(type == tagGSRV_ADDMODIFY_CSQINFO_QUERY::ACTION_ADD )//add info
	{
		ids.append(customerID);
		jvCS["ids"] = ids;
		strJvCS = jFastWriter.write(jvCS);
		if( false == c_memc->set(string(str_a),strJvUser.c_str())) 
		{			
			//m_memcached_lock.Unlock();
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: set %s fail",__FUNCTION__,__LINE__,str_3);
			return false;
		}			
	}		
	//m_memcached_lock.Unlock();
	return true;
}

#endif

bool CMemCBusi::RemoveCSQueueInfo(uint32 teamID)
{	
	char str_a[128] = {0};
	//ImMemCache &Memcache =	ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, 
			" DELETE CS QUEUE teamID %u",teamID);
	//m_memcached_lock.Lock();	
	MemcachedTryLock(MEMCACHED_QUEUE_LOCK);
	//删除已经存在的客服排队列表
	sprintf(str_a,"CSQueue#%u#%u",GetServerMagic(),teamID);
	if( false == c_memc->remove(string(str_a)))
	{		
		//m_memcached_lock.Unlock();
		MemcachedUnLock(MEMCACHED_QUEUE_LOCK);
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: delete CServiceInfo %s fail",__FUNCTION__,__LINE__,str_a);
		return false;
	}
	//m_memcached_lock.Unlock();
	MemcachedUnLock(MEMCACHED_QUEUE_LOCK);
	return true;
}

int CMemCBusi::QueryCSQueueInfo(uint32 teamID, tagGSRV_GET_ONLINE_CSQINFO_ACK::CSQinfo& CSQinfo)
{	
	char str_a[128] = {0};
	string Qinfo;
	m_MemRc = true; 
	//ImMemCache &Memcache =	ImMemCache::GetInstance();
	//global_Cache *c_memc = c_memc->GetLinks();
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT, 
			" QUERY CS QUEUE %u",teamID);
	CSQinfo.teamID = teamID;
	if(c_Memcache->GetLinkState() == true) //和memcached正常连接状态
	{
		//m_memcached_lock.Lock();				
		MemcachedTryLock(MEMCACHED_QUEUE_LOCK);
		//查询CServiceInfo 索引 		
		sprintf(str_a,"CSQueue#%u#%u",GetServerMagic(),teamID);
		c_memc->get(string(str_a),Qinfo);

		Json::Reader jReader(Json::Features::strictMode());
		Json::Value jvCS;
		Json::Value buyerIDs;
		Json::FastWriter jFastWriter;
		string strJvCS;
	/*
			回复格式
			{"CServiceId":"001","CServiceName":"客服001","State":1,"Recption"：1,
			"Consult":1,"RecpLimit":10," Customerlist":[0,1,2,3,4]}
	*/
		if(!Qinfo.empty())
		{	if((Qinfo[0] == '{' )&& jReader.parse(Qinfo, jvCS))
			{
				buyerIDs = jvCS["customerList"];
				for(int i = 0; i < (int)buyerIDs.size();i++)
				{
					CSQList_t each;
					each.userid = buyerIDs[i]["iD"].asUInt();
					each.requestTime = buyerIDs[i]["requestTime"].asUInt();					
					each.Level = buyerIDs[i]["level"].asUInt();
					each.teamID = buyerIDs[i]["teamID"].asUInt();
					CSQinfo.m_Customerlist.push_back(each); 
				}						
				//m_memcached_lock.Unlock();
				MemcachedUnLock(MEMCACHED_QUEUE_LOCK);
				return MEMCACHED_GET_INFO_SUCCESS;
			}	
			else
			{
				CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
					" %s %d: add_user_info json error %s",__FUNCTION__,__LINE__,Qinfo.c_str()); 				
				MemcachedUnLock(MEMCACHED_QUEUE_LOCK);
				//m_memcached_lock.Unlock();
				return MEMCACHED_JASON_ERROR;
			}
		}
		else
		{		
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_IMPORTANT,
				" %s %d: NO USERINFO %s",__FUNCTION__,__LINE__,str_a); 
			//m_memcached_lock.Unlock();			
			MemcachedUnLock(MEMCACHED_QUEUE_LOCK);
			return MEMCACHED_NO_INFO;
		}	
	}
	else
	{
		return MEMCACHED_SERVER_DOWN;
	}
}


#if 0
int CMemCBusi::QueryCSQueueInfo(uint32 teamID, CsQueue& csqueue)
{
	
	uint64_t value = 1;
	char str_a[128] = {0};
	char str_b[128] = {0};
	char str_1[128] = {0};
	char str_2[128] = {0};
	char str_3[128] = {0};
	CSinfo_t CSInfo;
	string CSQinfo;
	unsigned int user_index = 0;
	m_MemRc = true; 
	ImMemCache &Memcache =	ImMemCache::GetInstance();
	global_Cache *c_memc = c_memc->GetLinks();
	CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR, 
			" QUERY CS QUEUE %u",teamID);
	//m_memcached_lock.Lock();
			
	//查询CServiceInfo 索引 
	
	sprintf(str_a,"CSQueue#%u#%u",GetServerMagic(),teamID);
	c_memc->get(string(str_3),CSQinfo);

	Json::Reader jReader;
	Json::Value jvCS;
	Json::Value buyerIDs;
	Json::FastWriter jFastWriter;
	string strJvCS;
/*
		回复格式
		{"CServiceId":"001","CServiceName":"客服001","State":1,"Recption"：1,
		"Consult":1,"RecpLimit":10," Customerlist":[0,1,2,3,4]}
*/
	if(!CSQinfo.empty())
	{	if(jReader.parse(CSQinfo, jvCS))
		{
			buyerIDs = jvCS["customerList"];
			for(int i = 0; i < buyerIDs.size();i++)
			{
				csqueue.m_Customerlist.push_back(buyerIDs[i]); 
			}		
			
			//m_memcached_lock.Unlock();	
			return MEMCACHED_GET_INFO_SUCCESS;
		}	
		else
		{
			CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
				" %s %d: add_user_info json error %s",__FUNCTION__,__LINE__,CSinfo.c_str()); 
			//m_memcached_lock.Unlock();
			return MEMCACHED_JASON_ERROR;
		}
	}
	else
	{		
		CLog::Log("CMemCBusi-MEMCACHED", CLog::TYPE_ERROR,
			" %s %d: NO USERINFO %s",__FUNCTION__,__LINE__,str_3); 
		//m_memcached_lock.Unlock();
		return MEMCACHED_NO_INFO;
	}	
}
#endif

#endif



#if 0

/**
     * lock, 分布式互斥锁
     * 
     *  
     *
     * @param string $key, memcache键名
     * @param int $timeout, kv对生存时间，默认30s
     * @return boolean
     */
    public function lock($key, $timeout = 30)
    {
        $memc = memcache_connect("localhost", 11211);
        return $memc->add($key, 1, FALSE, $timeout);
    }


#endif
