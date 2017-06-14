/************************************************************************ 
 �ļ�������ȫ�ֿͻ���ҵ����,����ÿһ�������߳̽���һ�������
 
 �����ˣ� Zouyf, 2006-7-7 
 
 �޸ļ�¼�� 
            
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
		//��ѯ������Ϣ
		bool QueryFamilyInfo(uint32 nFamilyId, tagGSRV_FAMILYINFO_QUERY_ACK::tagFamilyInfo& pFamilyInfo,uint8 type);		
		//ɾ��������Ϣ
		bool DelFamilyInfo(uint32 nFamilyId,uint8 type);		
		bool SearchOnlineUser(tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo * pUserInfo,uint8 & count,uint32& nOnlineNum,uint16 startpos);
		bool SearchOnlineUser(tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo *pUserInfo,uint32 & count,uint32& nOnlineNum,uint16 startpos);//���������û���Ϣ
		
		//��������Ϣ��
		bool AddServerInfo(); //���ӷ�������Ϣ
		bool DelServerInfo(int server_info);//���ٷ�������Ϣ
		bool QueryServerInfo(vector<int> &server_info); //�����������Ϣ�Լ�������server_info.length��
		int GetServerId(){return m_ConfigInfo.GetServerNo();}		
		char * GetMemcachedServerIp(){return m_ConfigInfo.GetMemcachedAddr();}		
		int GetMemcachedServerPort(){return m_ConfigInfo.GetMemcachedPort();}
		int IsMemcachedMaster(){return m_ConfigInfo.IsMaster();}		
		char * MemcachedMaster(){return m_ConfigInfo.GetMasterIp();}	

		bool SetServerMagic();
		uint32 GetServerMagic();
		uint32 UpdateServerMagic();
		uint32 UpdateServerMagic(uint32 &time);
		
		bool query_total_user_number(uint32 &total_count);//���������û�����
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
		bool m_MemRc; 					//memcached ����ֵ
		bool m_check_magic; //magic �Ƿ���֤��
	
};
#endif

class CGlobalBusi
{
public:
	CGlobalBusi(GlobalBusiControl * pBusiControl);
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



