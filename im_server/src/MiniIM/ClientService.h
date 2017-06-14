
#pragma once
#include "User.h"
#include "protocol.h"
#include "PublicType.h"
#include "Utility.h"
#include "SortArray.h"
#include <ext/hash_map>
#include "ClientProtocol.h"
#include "RouterBusiness.h"

using namespace __gnu_cxx;


class ClientService
{
	public:
		ClientService();
		~ClientService();
		void Reset();
		
		void AddCustomer(uint32 teamid,uint32 customerId);		//������ѯ�û�
		void RemoveCustomer(uint32 teamid,uint32 customerId);	//ɾ����ѯ�û�
		void ModifyCustomerState(uint32 teamid,uint32 customerId);	//�����û�Ϊ�ѽӴ�

		void AddCustomer_x(uint32 teamid,uint32 customerId);		//������ѯ�û�		
		void CustomerQueueDebug_x();		
		void RemoveCustomer_x(uint32 customerId);	//ɾ����ѯ�û�		
		void ModifyCustomerState_x(uint32 customerId);	//�����û�Ϊ�ѽӴ�
		
		bool	getCustomerInfo(uint32 customerId,customer_t &info);	//��ȡ�Ӵ��û���Ϣ
		bool    getCustomerTeamId(uint32 customerId,uint32 &teamid);//��ȡ�Ӵ��û���������Ϣ
		bool	hasCustomer(uint32 customerId);//�ж��Ƿ���ѯ��

		void AddToOnlineList();	//����ǰ���Ҽ�������������
		void DelFromOnlineList();		//����ǰ���Ҵ�����������ɾ��		
		void SendLogoutReq(CUser* pUser);
		int PushQueue(CGlobalBusi *pGlobalBusi, XT_WAITQUEU_CHANGE* QueueChange,uint32 dst_id ); //֪ͨ����������Ա�Ŷ�״��
		int	NotifyTeamQueue(CGlobalBusi *pGlobalBusi, XT_WAITQUEU_CHANGE* QueueChange,uint32 type =0);
		void CustomerQueueDebug();
		uint32 getCSID(){return this->m_nID;}
		uint32 setCSID(uint32 id){this->m_nID = id; return this->m_nID;}

		
		void AddTeamMatesList(uint32 teamId,uint32 friendId);	//�Ӻ���
		void DelTeamMatesList(uint32 teamId,uint32 friendId);	//ɾ������
		void DebugTeamMatesList();
		static __gnu_cxx::hash_map<uint32, ClientService*> m_cshash;
		static CSyncCritical m_CSListLock;
		
		// ��ȡ�ͷ����ڽӴ�����ң����ڼ�ع����У���������ϵ����Ҹ�֪���ܣ�
		uint32 GetChattingBuyerID();

		// ���ÿͷ����ڽӴ������
		void SetChattingBuyerID(uint32 nBuyerID);

		// ��ȡ�ͷ����ڱ��ĸ����ܼ��
		uint32 GetDirectorID();

		// ���ÿͷ����ڱ��ĸ����ܼ��
		void SetDirectorID(uint32 nDirectorID);

		// �жϿͷ��Ƿ����ڱ����
		bool IsMonitored()		{	return 	m_bMonitored;		}
		uint32 m_nID;				//��ǰclientservice��ID��
		char m_CSname[MAX_USERNAME_LEN];	
		int m_State;				//�˺�״̬ 1�������ã�0����ͣ�ã�2����ȫ��
		int m_MaxRecConsult;		//�����ѯ�Ӵ���		
		int m_CurReception;	//��ǰ�Ӵ���
		int m_CurConsult;		//��ǰ��ѯ��
		int m_Permission;	//�ͷ�Ȩ��
		map<uint32,CSortArray<customer_t> *> m_Customerlist;	//��ͷ���ѯ���û�[�û�id-----�û���Ϣ]
		map<uint32,customer_t> m_CSCustomerlist;	//���û�IDά�ȴ洢�Ӵ��û�
		vector<csTeam_t> m_teamMates;	//������� ������Ψһ�ԣ�������sortarray

	private:
		
		bool m_bMonitored;	// �ͷ��Ƿ񱻼��
		uint32 m_nChattingBuyerID;		// �ͷ���ǰ������ϵ����ң����ڼ�ع����У���������ϵ����Ҹ�֪���ܣ�
		uint32 m_nDirectorID;	// ����ͷ�����أ���¼�����������id

};

// ���ҿͷ��Ƿ����(�Ȳ鱾�����ٲ�memcache)
bool g_cs_exist(uint32 cs_id, CGlobalBusi *glob_busi);
// ���ҿͷ��Ƿ����ڱ���أ��������أ���������id(�Ȳ鱾�����ٲ�memcache)
bool g_cs_ismonitored(uint32 cs_id, bool &monitored, uint32 &cs_master_id, CGlobalBusi *glob_busi);
// ���ҿͷ����ڽӴ������(�Ȳ鱾�����ٲ�memcache)
bool g_cs_get_chatting_buyer(uint32 cs_id, uint32 &chatting_buyer_id, CGlobalBusi *glob_busi);
// ���ÿͷ����ڱ�˭���(ͬʱ�޸������ļ�memcache)
bool g_cs_set_director(uint32 cs_id, uint32 cs_master_id, CRouterBusiness *glob_busi);
// ���ÿͷ�������ϵ�����(ͬʱ�޸������ļ�memcache)
bool g_cs_set_chatting_buyer(uint32 cs_id, uint32 chattring_buyer_id, CRouterBusiness *glob_busi);
// ��ȡ�ͷ�����
bool g_cs_get_name(uint32 cs_id, char name[], CGlobalBusi *glob_busi);
