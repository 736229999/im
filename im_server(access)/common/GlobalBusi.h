/************************************************************************ 
 �ļ�������ȫ�ֿͻ���ҵ����,����ÿһ�������߳̽���һ�������
 
 �����ˣ� Zouyf, 2006-7-7 
 
 �޸ļ�¼�� 
            
************************************************************************/
#pragma once
#include "BaseUdpTrans.h"
#include "GlobalBusiControl.h"
#include "SyncEvent.h"
#ifdef IM_USE_CACHE
#include <libmemcached/memcached.h>
#include <global_cached.hpp>
#include <libmemcached/memcached.h>
#include "SyncCritical.h"
#include <string>
#include <vector>
#include <iostream>
using namespace global_cache;

#endif
#include "../include/json/json.h"
#include "ConfigInfo.h"


class CUser;


#ifdef IM_USE_CACHE



class ImMemCache
{
	public:		
		ImMemCache();
		~ImMemCache();
		static ImMemCache& GetInstance();
		global_Cache* GetLinks(){return Links;};
		global_Cache *getLinks(){return Links; };

		bool GetLinkState(){return state;};
		void SetLinkState(bool Ostate){state = Ostate;};  // true :WORK  false:BAD
		int init_ImMemcached(char *ipaddr,int port);
		int init_ImMemcached_WebNotify(char *ipaddr,int port);
		int Reinit_ImMemcached();
		int releaseLink(global_Cache* links);
		int	CheckSuccess();		
		int CheckAndSetInitialized();
		
		int CheckAndSetInitializedThreads(); //�߳��ڲ���ʼ���ж�
		int GetServerId(){return m_ConfigInfo.GetServerNo();};	
		uint32 GetServerMagic(){ return m_ConfigInfo.GetMemcachedMagic();};
		bool TotalUserInit();		
		bool TotalServerInit();
		bool AddServerInfo(); //���ӷ�������Ϣ
		bool RecoverUsers(CUser* pUser);
		bool InitUsers(); //��Ⱥ��ʼ��һ��

	private:
		CConfigInfo	m_ConfigInfo;
		global_Cache *Links;
		uint32 max_links;
		bool state;
		
};


class CMemCBusi
{
	public:
		CMemCBusi();
		~CMemCBusi();
		CMemCBusi(ImMemCache *Memcache);
		int QueryUserControlInfo(uint32 userId,tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo &userControlInfo);		
		//�޸��û�״̬
		bool SetTUserInfo(uint32 userId, tagGSRV_TUSERINFO_ACK &TUserInfo);
		int GetTUserInfo(uint32 userId,tagGSRV_TUSERINFO_ACK &TUserInfo);
		int QueryUserStatus(uint32 userId, tagGSRV_GET_USER_STATUS_ACK::tagUserStatus& userStatus);
		bool ModiUserStatus(tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser user);		
		int QueryUserFlow(uint32 userId,tagGSRV_GET_USER_FLOW_ACK::tagUserFlow &userFlow);		
		bool ModiUserFlow(const tagGSRV_MODIFY_USER_FLOW_QUERY::tagFlow UserFlow);		
		bool AddModiUserInfo(CUser* pUser,uint8 action,uint8 flag);
		bool AddModiUserInfo(const tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo globalUserInfo,uint32 ServIp = (uint32)m_ConfigInfo.GetServerNo());
		int QueryUserInfo(uint32 nUserId, tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo& userInfo);
		bool DelUserInfo(uint32 nUserId,uint32 ServIp = (uint32)m_ConfigInfo.GetServerNo());		
		bool AddModiFamilyInfo(const tagGSRV_FAMILYINFO_ADD_QUERY::tagFamilyInfo pFamilyInfo,uint8 type);		
		//��ѯ������Ϣ
		int QueryFamilyInfo(uint32 nFamilyId, tagGSRV_FAMILYINFO_QUERY_ACK::tagFamilyInfo& pFamilyInfo,uint8 type);		
		//ɾ��������Ϣ
		bool DelFamilyInfo(uint32 nFamilyId,uint8 type);		
		int SearchOnlineUser(tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo * pUserInfo,uint8 & count,uint32& nOnlineNum,uint16 startpos);
		//��������Ϣ��

		bool DelServerInfo(int server_info);//���ٷ�������Ϣ
		bool QueryServerInfo(vector<int> &server_info); //�����������Ϣ�Լ�������server_info.length��
		int GetServerId(){return m_ConfigInfo.GetServerNo();}		
		char * GetMemcachedServerIp(){return m_ConfigInfo.GetMemcachedAddr();}		
		int GetMemcachedServerPort(){return m_ConfigInfo.GetMemcachedPort();}
		int IsMemcachedMaster(){return m_ConfigInfo.IsMaster();}		
		char * MemcachedMaster(){return m_ConfigInfo.GetMasterIp();}	
		int SearchDownSerUserinfo(tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo *pUserInfo,uint8 & count,uint32& nOnlineNum,uint16 startpos,uint32 serip);//���������û���Ϣ
		int QueryCSQueueLength(const uint32 queueid,tagGSRV_GET_ONLINE_CSQLEN_ACK::CSQinfo &csQlen);
		bool UpdateCsMonitor(CUser *pUser);
		bool QueryCsMonitor(uint32 userId,tagGSRV_GET_ONLINE_MONITOR_ACK::UserInfo &userInfo);
		
		bool setExpireUserinfo(uint32 userid);
		
		bool ModiUserIpPortInfo(const uint32 auid,uint32 ip,uint16 port);

//		bool SetServerMagic();
		
		
		uint32 GetServerMagic(){ return m_ConfigInfo.GetMemcachedMagic();};
//		uint32 UpdateServerMagic();
//		uint32 UpdateServerMagic(uint32 &time);
		
		bool query_total_user_number(uint32 &total_count);//���������û�����

		bool totaluser_init();		
		bool totalserver_init();
		bool AddServerInfo(); //���ӷ�������Ϣ	
		
		bool AddModiCSInfo(const CSinfo_t globalCSInfo,uint8 type,uint32 ServIp = (uint32)m_ConfigInfo.GetServerNo());
	//	bool AddModiCSInfo(const ClientService globalCSInfo,uint8 type,uint32 ServIp = (uint32)m_ConfigInfo.GetServerNo());
		bool DelCSInfo(uint32 nUserId,uint32 ServIp = (uint32)m_ConfigInfo.GetServerNo());		
		int QueryCSInfo(uint32 nUserId, CSinfo_t& CSInfo);			
	//	int QueryCSInfo(uint32 nUserId, ClientService& clientService);	
		
		bool AddDelCSQueueInfo(const tagGSRV_ADDMODIFY_CSQINFO_QUERY::CSinfo globalCSQInfo,uint8 type);				
	//	bool AddModiCSQueueInfo(uint32 teamID,uint32 customerID,uint8 type);				
		bool RemoveCSQueueInfo(uint32 teamID);				
		int QueryCSQueueInfo(uint32 teamID, tagGSRV_GET_ONLINE_CSQINFO_ACK::CSQinfo& CSQinfo);			
	//	int QueryCSQueueInfo(uint32 teamID, CsQueue& csqueue);
	bool BestAccountProcess(uint32 merchant_id, uint32 group_id, vector<uint32>& accounts, uint32 flow_type, uint32& id);
	int GetBestCSAccount(uint32 merchant_id, uint32 group_id, vector<uint32>& ids, uint32 flow_type, uint32& id);
	bool GetBestAccount(uint32 merchant_id, uint32 group_id, vector<uint32>& ids, uint32 flow_type, uint32& id);

	int QueryUserTeamID(uint32 userId, tagGSRV_GET_USER_TEAMID_ACK::tagUserTeamID& userTeamID);
	bool ModiUserTeamID(tagGSRV_MODIFY_ONLINE_TEAMID_QUERY::tagUserTeamID user);
	// ��redis�л�ȡ�û�ȫ����Ϣ
	int GetFUserInfo(uint32 userid, CUser &user);


	private:
		void MemCInit();
		void MemCFree();	
		
//		bool FetchKeysValues(vector<string> &keys,vector<string> &values);

		void MemcachedTryLock(int type);
		void MemcachedUnLock(int type);

		
	private:
		static uint32 m_magic_num;
		static CConfigInfo	m_ConfigInfo;
		static bool memcache_has_init;
		static CSyncCritical	m_memcached_lock;
		global_Cache *c_memc;
		ImMemCache *c_Memcache;
		bool m_MemRc; 					//memcached ����ֵ
		bool m_check_magic; //magic �Ƿ���֤��
	
};
#endif

class CGlobalBusi
{
public:
	CGlobalBusi(GlobalBusiControl * pBusiControl);
	CGlobalBusi(GlobalBusiControl * pBusiControl,ImMemCache *memcached);
	
	~CGlobalBusi(void);
	//�����û�ȫ�ֲ�ѯ
	int SearchOnlineUser(tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo * pUserInfo,uint8 & count,uint32& nOnlineNum,uint16 startpos);
	
	//��ѯ�����û�������Ϣ
	int QueryUserControlInfo(const uint32 * auid,tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo * pUserControlInfo,uint8 & count);

	//��ѯ���޸��û�״̬
	int QueryUserStatus(const uint32 * aID,tagGSRV_GET_USER_STATUS_ACK::tagUserStatus * pUStatus,uint8 & count);
	int ModiUserStatus(const tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser * pUserStatus,uint8 & count,tagGSRV_MODIFY_ONLINE_STATUS_ACK::tagUser * pUStatusNow = NULL);


	//��ѯ���޸��û�����
	int QueryUserFlow(const uint32 * aID, tagGSRV_GET_USER_FLOW_ACK::tagUserFlow * pUserFlow,uint8 & count);
	int ModiUserFlow(const tagGSRV_MODIFY_USER_FLOW_QUERY::tagFlow * pUserFlow, uint8 & count, tagGSRV_MODIFY_USER_FLOW_ACK::tagUserFlow * pUserFlowNow/* = NULL*/);
	int GetBestAccount(uint32 merchant_id, uint32 group_id, std::vector<uint32_t>& ids, uint32 flow_type, uint32& id);


	
	//��ѯ�����û�ȫ������
	int QueryUserInfo(const uint32 * auid,uint8 & count,tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo * pUserInfo);
	//��ѯ�����û�����
	
	int QueryOtherUserInfo(const uint32 * auid,uint8 & count,tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo * pUserInfo);
	
	//��ȫ�����/�޸��û�����
	int AddModiUserInfo(const tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo * pUserInfo,uint8 & count,uint32 * pUserID = NULL,uint8 action = tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD,uint8 flag = XT_HEAD::FLAG_CNTREQUEST);
	int AddModiUserInfo(CUser* pUser,uint8 action = tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD,uint8 flag = XT_HEAD::FLAG_CNTREQUEST);
	//ɾ���û�ȫ������
	int DelUserInfo(const uint32 * auid,uint8 & count,uint32 * aOKUserID = NULL);

	/************ ����ҵ��*******************/
	//��ѯȫ�ּ�������
	int FindFamilyInfo(const uint32 * pFamilyID,uint8 & count,tagGSRV_FAMILYINFO_QUERY_ACK::tagFamilyInfo * pFamilyInfo,uint8 type = FAMILYINFO_QUERY_TYPE_PERM);
	//���ȫ�ּ�������
	int AddFamilyInfo(const tagGSRV_FAMILYINFO_ADD_QUERY::tagFamilyInfo * pFamilyInfo,uint8 & count,uint32 * pFamilyID = NULL,uint8 type = FAMILYINFO_QUERY_TYPE_PERM);
	//ɾ��ȫ�ּ�������
	int DelFamilyInfo(const uint32 * pDelFamilyID,uint8 & count,uint32 * pOKFamilyID = NULL,uint8 type = FAMILYINFO_QUERY_TYPE_PERM);
#ifdef IM_USE_CACHE	
	int QueryOtherControlInfo(const uint32 * auid,tagGSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo * pUserControlInfo,uint8 & count);
	int QueryOtherUserStatus(const uint32 * aID,tagGSRV_GET_USER_STATUS_ACK::tagUserStatus * pUStatus,uint8 & count);
	int QueryOtherUserFlow(const uint32 * aID, tagGSRV_GET_USER_FLOW_ACK::tagUserFlow * pUserFlow,uint8 & count);
	//��ѯȫ�ּ�������
	int FindOtherFamilyInfo(const uint32 * pFamilyID,uint8 & count,tagGSRV_FAMILYINFO_QUERY_ACK::tagFamilyInfo * pFamilyInfo,uint8 type/* = FAMILYINFO_QUERY_TYPE_PERM*/);
	int GetOtherBestAccount(uint32 merchant_id, uint32 group_id, std::vector<uint32_t>& ids, uint32 flow_type, uint32& id);
	/*�߼�����ʱ����Ҫ�������պ�ʹ��*/
	int AddModiOtherUserInfo(CUser* pUser,uint8 action,uint8 flag)	;
	int AddModiOtherUserInfo(const tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo * pUserInfo,uint8 & count,uint32 * pUserID,uint8 action /* = tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD*/,uint8 flag);
	int ModiOtherUserStatus(const tagGSRV_MODIFY_ONLINE_STATUS_QUERY::tagUser * pUserStatus,uint8 & count,tagGSRV_MODIFY_ONLINE_STATUS_ACK::tagUser * pUStatusNow/* = NULL*/);
	int ModiOtherUserFlow(const tagGSRV_MODIFY_USER_FLOW_QUERY::tagFlow * pUserFlow, uint8 & count, tagGSRV_MODIFY_USER_FLOW_ACK::tagUserFlow * pUserFlowNow/* = NULL*/);
	int DelOtherUserInfo(const uint32 * auid,uint8 & count,uint32 * aOKUserID /* = NULL*/);
		/************ ����ҵ��*******************/
	//���ȫ�ּ�������
	int AddOtherFamilyInfo(const tagGSRV_FAMILYINFO_ADD_QUERY::tagFamilyInfo * pFamilyInfo,uint8 & count,uint32 * pFamilyID/* = NULL*/,uint8 type /*= FAMILYINFO_QUERY_TYPE_PERM*/);
	//ɾ��ȫ�ּ�������
	int DelOtherFamilyInfo(const uint32 * pDelFamilyID,uint8 & count,uint32 * pOKFamilyID/* = NULL*/,uint8 type/* = FAMILYINFO_QUERY_TYPE_PERM*/);
	/************ �ͷ�ҵ��*******************/

	//����ͷ���Ϣ
	int QueryCSInfo(const uint32 * auid,uint8 & count,CSinfo_t * pUserInfo);
//	int QueryCSInfo(const uint32 * auid,uint8 & count,ClientService* pUserInfo);
	//�����޸Ŀͷ���Ϣ
	int AddModiCSInfo(const CSinfo_t * pUserInfo,uint8 & count,uint8 action );
//	int AddModiCSInfo(const ClientService* pUserInfo,uint8 & count,uint8 action );
	//ɾ���ͷ���Ϣ
	int DelCSInfo(const uint32 * auid,uint8 & count);
	int QueryCSQueueInfo(const uint32 * auid,uint8  count,tagGSRV_GET_ONLINE_CSQINFO_ACK::CSQinfo* csQinfo);	
	int QueryCSQueueLength(vector<uint32> queueid,uint8  count,tagGSRV_GET_ONLINE_CSQLEN_ACK::CSQinfo* csQlen);
//	int QueryCSQueueInfo(const uint32 * auid,uint8 & count,CsQueue* csQueue);	
	int AddDelCSQueueInfo(const tagGSRV_ADDMODIFY_CSQINFO_QUERY::CSinfo * csQinfo,uint8 & count,uint8 action);		
	int RemoveCSQueueInfo(uint32 *teamID,uint8 & count);	
	int GetBestCSAccount(uint32 merchant_id, uint32 group_id, std::vector<uint32_t>& ids, uint32 flow_type, uint32& id);
	
	int QueryUserTeamID(const uint32 * aID,tagGSRV_GET_USER_TEAMID_ACK::tagUserTeamID * pUTeamID,uint8 & count);
	int ModiUserTeamID(const tagGSRV_MODIFY_ONLINE_TEAMID_QUERY::tagUserTeamID* pUserTeamID,uint8 & count);
	int setExpireUserinfo(uint32 userid);
	// ��redis�л�ȡ�û�ȫ����Ϣ
	int GetFUserInfo(uint32 userid, CUser &user);
	int ModiUserIpPortInfo(const uint32 auid,uint32 ip,uint16 port);

	bool UpdateCsMonitor(CUser *pUser);
	bool QueryCsMonitor(uint32 userId,tagGSRV_GET_ONLINE_MONITOR_ACK::UserInfo &userInfo);
	int GetTUserInfo(uint32 userId,tagGSRV_TUSERINFO_ACK &TUserInfo);
	bool SetTUserInfo(uint32 userId, tagGSRV_TUSERINFO_ACK &TUserInfo);
#endif		
private:
	void SetEvent(const RecvDataStruct * pRecvData)
	{
		memcpy(m_pRecvData,pRecvData,sizeof(RecvDataStruct) - MAX_NETWORK_PACKET + pRecvData->data_len);
		m_hEvent.SetEvent();
	}

	void SetMultiMessange(const RecvDataStruct * pRecvData)
	{
		RecvDataStruct RecvData;		
		memcpy(&RecvData,pRecvData,sizeof(RecvDataStruct) - MAX_NETWORK_PACKET + pRecvData->data_len);
		m_MultiRecvData.push_back(RecvData);		
	}
	void SetMultiEvent(const RecvDataStruct * pRecvData)
	{
		m_hEvent.SetEvent();
	}
	
private:
	friend class GlobalBusiControl;
	uint32 m_vLink;
	CSyncEvent	m_hEvent;

	GlobalBusiControl * m_pBusiControl;
	RecvDataStruct * m_pRecvData;
	SendDataStruct m_sendData;
	vector<RecvDataStruct> m_MultiRecvData;
#ifdef IM_USE_CACHE
	CMemCBusi	m_cMemCBusi;	//�̰߳�ȫ
	//CMemCBusi	t_cMemCBusi;	//�̰߳�ȫ
#endif

};



