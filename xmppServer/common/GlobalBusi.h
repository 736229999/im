/************************************************************************ 
 文件描述：全局客户端业务类,对于每一个调用线程建立一个类对象
 
 创建人： Zouyf, 2006-7-7 
 
 修改记录： 
            
************************************************************************/
#pragma once
#include "BaseUdpTrans.h"
#include "GlobalBusiControl.h"
#include "SyncEvent.h"
#ifdef IM_USE_MEMCACHED
#include <libmemcached/memcached.h>
#include <global_memcached.hpp>
#include <libmemcached/memcached.h>
#include "SyncCritical.h"
#include <string>
#include <vector>
#include <iostream>
using namespace global_memcache;

#endif
#include "../include/json/json.h"
#include "ConfigInfo.h"


class CUser;

#ifdef IM_USE_MEMCACHED




class CMemCBusi
{
	public:
		CMemCBusi();
		~CMemCBusi();
		
		bool QueryUserControlInfo(uint32 userId,tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo &userControlInfo);
		bool QueryUserStatus(uint32 userId, tagGSRV_GET_USER_STATUS_ACK::tagUserStatus& userStatus);
		bool ModiUserStatus(tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser user);		
		bool QueryUserFlow(uint32 userId,tagGSRV_GET_USER_FLOW_ACK::tagUserFlow &userFlow);		
		bool ModiUserFlow(const tagGSRV_MODIFY_USER_FLOW_QUERY::tagFlow UserFlow);
		bool AddModiUserInfo(const tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo globalUserInfo);
		bool QueryUserInfo(uint32 nUserId, tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo& userInfo);
		bool DelUserInfo(uint32 nUserId);		
		bool AddModiFamilyInfo(const tagGSRV_FAMILYINFO_ADD_QUERY::tagFamilyInfo pFamilyInfo,uint8 type);		
		//查询家族信息
		bool QueryFamilyInfo(uint32 nFamilyId, tagGSRV_FAMILYINFO_QUERY_ACK::tagFamilyInfo& pFamilyInfo,uint8 type);		
		//删除家族信息
		bool DelFamilyInfo(uint32 nFamilyId,uint8 type);		
		bool SearchOnlineUser(tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo * pUserInfo,uint8 & count,uint32& nOnlineNum,uint16 startpos);
		bool SearchOnlineUser(tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo *pUserInfo,uint32 & count,uint32& nOnlineNum,uint16 startpos);//请求在线用户信息
		
		//服务器信息：
		bool AddServerInfo(); //增加服务器信息
		bool DelServerInfo(int server_info);//减少服务器信息
		bool QueryServerInfo(vector<int> &server_info); //请求服务器信息以及个数（server_info.length）
		int GetServerId(){return m_ConfigInfo.GetServerNo();}		
		char * GetMemcachedServerIp(){return m_ConfigInfo.GetMemcachedAddr();}		
		int GetMemcachedServerPort(){return m_ConfigInfo.GetMemcachedPort();}
		int IsMemcachedMaster(){return m_ConfigInfo.IsMaster();}		
		char * MemcachedMaster(){return m_ConfigInfo.GetMasterIp();}	

		bool SetServerMagic();
		uint32 GetServerMagic();
		uint32 UpdateServerMagic();
		uint32 UpdateServerMagic(uint32 &time);
		
		bool query_total_user_number(uint32 &total_count);//请求在线用户总数
		bool GetBestAccount(uint32 merchant_id, uint32 group_id, std::vector<uint32_t>& ids, uint32 flow_type, uint32& id);
		
		
	private:
		void MemCInit();
		void MemCFree();	
		bool totaluser_init();		
		bool totalserver_init();
		bool FetchKeysValues(vector<string> &keys,vector<string> &values);
		bool BestAccountProcess(uint32 merchant_id, uint32 group_id, std::vector<uint32_t>& accounts, uint32 flow_type, uint32& id);
	private:
		static uint32 m_magic_num;
		CConfigInfo				m_ConfigInfo;
		static bool memcache_has_init;
		static CSyncCritical	m_memcached_lock;
		global_Memcache *c_memc;
		bool m_MemRc; 					//memcached 返回值
		bool m_check_magic; //magic 是否验证过
	
};
#endif

class CGlobalBusi
{
public:
	CGlobalBusi(GlobalBusiControl * pBusiControl);
	~CGlobalBusi(void);
	//在线用户全局查询
	int SearchOnlineUser(tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo * pUserInfo,uint8 & count,uint32& nOnlineNum,uint16 startpos);
	
	//查询在线用户控制信息
	int QueryUserControlInfo(const uint32 * auid,tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo * pUserControlInfo,uint8 & count);

	//查询和修改用户状态
	int QueryUserStatus(const uint32 * aID,tagGSRV_GET_USER_STATUS_ACK::tagUserStatus * pUStatus,uint8 & count);
	int ModiUserStatus(const tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser * pUserStatus,uint8 & count,tagGSRV_MODIFY_ONLINE_STATUS_ACK::tagUser * pUStatusNow = NULL);


	//查询和修改用户分流
	int QueryUserFlow(const uint32 * aID, tagGSRV_GET_USER_FLOW_ACK::tagUserFlow * pUserFlow,uint8 & count);
	int ModiUserFlow(const tagGSRV_MODIFY_USER_FLOW_QUERY::tagFlow * pUserFlow, uint8 & count, tagGSRV_MODIFY_USER_FLOW_ACK::tagUserFlow * pUserFlowNow/* = NULL*/);
	int GetBestAccount(uint32 merchant_id, uint32 group_id, std::vector<uint32_t>& ids, uint32 flow_type, uint32& id);


	
	//查询在线用户全局资料
	int QueryUserInfo(const uint32 * auid,uint8 & count,tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo * pUserInfo);
	//向全局添加/修改用户资料
	int AddModiUserInfo(const tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo * pUserInfo,uint8 & count,uint32 * pUserID = NULL,uint8 action = tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD,uint8 flag = XT_HEAD::FLAG_CNTREQUEST);
	int AddModiUserInfo(CUser* pUser,uint8 action = tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD,uint8 flag = XT_HEAD::FLAG_CNTREQUEST);
	//删除用户全局资料
	int DelUserInfo(const uint32 * auid,uint8 & count,uint32 * aOKUserID = NULL);

	/************ 家族业务*******************/
	//查询全局家族资料
	int FindFamilyInfo(const uint32 * pFamilyID,uint8 & count,tagGSRV_FAMILYINFO_QUERY_ACK::tagFamilyInfo * pFamilyInfo,uint8 type = FAMILYINFO_QUERY_TYPE_PERM);
	//添加全局家族资料
	int AddFamilyInfo(const tagGSRV_FAMILYINFO_ADD_QUERY::tagFamilyInfo * pFamilyInfo,uint8 & count,uint32 * pFamilyID = NULL,uint8 type = FAMILYINFO_QUERY_TYPE_PERM);
	//删除全局家族资料
	int DelFamilyInfo(const uint32 * pDelFamilyID,uint8 & count,uint32 * pOKFamilyID = NULL,uint8 type = FAMILYINFO_QUERY_TYPE_PERM);

		
private:
	void SetEvent(const RecvDataStruct * pRecvData)
	{
		memcpy(m_pRecvData,pRecvData,sizeof(RecvDataStruct) - MAX_NETWORK_PACKET + pRecvData->data_len);
		m_hEvent.SetEvent();
	}
private:
	friend class GlobalBusiControl;
	uint32 m_vLink;
	CSyncEvent	m_hEvent;

	GlobalBusiControl * m_pBusiControl;
	RecvDataStruct * m_pRecvData;
	SendDataStruct m_sendData;
#ifdef IM_USE_MEMCACHED
	CMemCBusi	m_cMemCBusi;
#endif

};



