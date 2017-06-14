
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
		
		void AddCustomer(uint32 teamid,uint32 customerId);		//新增咨询用户
		void RemoveCustomer(uint32 teamid,uint32 customerId);	//删除咨询用户
		void ModifyCustomerState(uint32 teamid,uint32 customerId);	//设置用户为已接待

		void AddCustomer_x(uint32 teamid,uint32 customerId);		//新增咨询用户		
		void CustomerQueueDebug_x();		
		void RemoveCustomer_x(uint32 customerId);	//删除咨询用户		
		void ModifyCustomerState_x(uint32 customerId);	//设置用户为已接待
		
		bool	getCustomerInfo(uint32 customerId,customer_t &info);	//获取接待用户信息
		bool    getCustomerTeamId(uint32 customerId,uint32 &teamid);//获取接待用户所在组信息
		bool	hasCustomer(uint32 customerId);//判断是否咨询中

		void AddToOnlineList();	//将当前卖家加入在线链表中
		void DelFromOnlineList();		//将当前卖家从在线链表中删除		
		void SendLogoutReq(CUser* pUser);
		int PushQueue(CGlobalBusi *pGlobalBusi, XT_WAITQUEU_CHANGE* QueueChange,uint32 dst_id ); //通知组内其他成员排队状况
		int	NotifyTeamQueue(CGlobalBusi *pGlobalBusi, XT_WAITQUEU_CHANGE* QueueChange,uint32 type =0);
		void CustomerQueueDebug();
		uint32 getCSID(){return this->m_nID;}
		uint32 setCSID(uint32 id){this->m_nID = id; return this->m_nID;}

		
		void AddTeamMatesList(uint32 teamId,uint32 friendId);	//加好友
		void DelTeamMatesList(uint32 teamId,uint32 friendId);	//删除好友
		void DebugTeamMatesList();
		static __gnu_cxx::hash_map<uint32, ClientService*> m_cshash;
		static CSyncCritical m_CSListLock;
		
		// 获取客服正在接待的买家（用于监控过程中，将正在联系的买家告知主管）
		uint32 GetChattingBuyerID();

		// 设置客服正在接待的买家
		void SetChattingBuyerID(uint32 nBuyerID);

		// 获取客服正在被哪个主管监控
		uint32 GetDirectorID();

		// 设置客服正在被哪个主管监控
		void SetDirectorID(uint32 nDirectorID);

		// 判断客服是否正在被监控
		bool IsMonitored()		{	return 	m_bMonitored;		}
		uint32 m_nID;				//当前clientservice的ID号
		char m_CSname[MAX_USERNAME_LEN];	
		int m_State;				//账号状态 1代表启用，0代表停用，2代表全部
		int m_MaxRecConsult;		//最大咨询接待量		
		int m_CurReception;	//当前接待量
		int m_CurConsult;		//当前咨询量
		int m_Permission;	//客服权限
		map<uint32,CSortArray<customer_t> *> m_Customerlist;	//向客服咨询的用户[用户id-----用户信息]
		map<uint32,customer_t> m_CSCustomerlist;	//按用户ID维度存储接待用户
		vector<csTeam_t> m_teamMates;	//分组好友 不具有唯一性，不能用sortarray

	private:
		
		bool m_bMonitored;	// 客服是否被监控
		uint32 m_nChattingBuyerID;		// 客服当前正在联系的买家（用于监控过程中，将正在联系的买家告知主管）
		uint32 m_nDirectorID;	// 如果客服被监控，记录监控他的主管id

};

// 查找客服是否存在(先查本机，再查memcache)
bool g_cs_exist(uint32 cs_id, CGlobalBusi *glob_busi);
// 查找客服是否正在被监控，如果被监控，返回主管id(先查本机，再查memcache)
bool g_cs_ismonitored(uint32 cs_id, bool &monitored, uint32 &cs_master_id, CGlobalBusi *glob_busi);
// 查找客服正在接待的买家(先查本机，再查memcache)
bool g_cs_get_chatting_buyer(uint32 cs_id, uint32 &chatting_buyer_id, CGlobalBusi *glob_busi);
// 设置客服正在被谁监控(同时修改上下文及memcache)
bool g_cs_set_director(uint32 cs_id, uint32 cs_master_id, CRouterBusiness *glob_busi);
// 设置客服正在联系的买家(同时修改上下文及memcache)
bool g_cs_set_chatting_buyer(uint32 cs_id, uint32 chattring_buyer_id, CRouterBusiness *glob_busi);
// 获取客服名字
bool g_cs_get_name(uint32 cs_id, char name[], CGlobalBusi *glob_busi);
