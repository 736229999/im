#pragma once
/************************************************************************ 
文件描述：全局在线管理类的声明文件 

创建人：hechang
时间：	2006-6-9 
************************************************************************/

#include "OnlineUser.h"
#include "xtpacket.h"
#include "SyncCritical.h"
#include "Utility.h"
#include "ZObjectPool.h"
#include "ConfigInfo.h"


struct ReloginUser
{
	uint32 userid;
	uint32 serverip;
	uint32 userip;
	uint32 lanip;
};

typedef SafeMap<uint32, uint32> MerchantAccount_Map;
typedef SafeMap<uint32, uint32> GroupAccount_Map;

class COnlineManager
{
public:
	COnlineManager(void);
	~COnlineManager(void);

	//添加/修改一个上线的用户
	bool AddModifyOnlineUser(CRecvPacket& RecvPacket,CSendPacket& SendPacket,
							ReloginUser* pReloginUserList,uint8& nReloginCount);
	bool DelUser(uint32 userid);

	//删除一个上线用户
	bool DelOnlineUser(CRecvPacket& RecvPacket,CSendPacket& SendPacket,uint32 fromip=0);

	//得到用户详细信息
	bool GetUserFullInfo(CRecvPacket& RecvPacket,CSendPacket& SendPacket);
	
	//得到文件传输服务器用户信息
	bool GetTUserInfo(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//得到用户状态信息
	bool GetUserStatus(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//修改用户状态信息
	bool ModifyUserStatus(CRecvPacket& RecvPacket,CSendPacket& SendPacket);


	//获取分流信息
	bool GetUserFlow(CRecvPacket& RecvPacket,CSendPacket& SendPacket);
	//修改分流信息
	bool ModifyUserFlow(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//获取最佳子账号
	bool GetBestAccount(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//得到用户链路信息
	bool GetUserLinkInfo(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//查找在线用户
	bool SearchOnlineUser(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//某台IM服务器停机了
	void IMServerShutdown(uint32 serverip);

	//通知文件传输服务器用户登录
	void NotifyTransferLogin(COnlineUser* pNewUser);

	uint32 GetOnlineUserNum()
	{
		return m_nOnlineNum;
	}
	
	COnlineUser* GetUser(uint32 id)
	{
		if( id<0 || id >= MAX_139_ACCOUNT_NUM )
		{
			return NULL;
		}
		return m_pGlobalOnline[id]; 
	}

private:

	COnlineUser**		m_pGlobalOnline;	//所有用号的索引
	COnlineUser*		m_pOnlineList;		//在线用户列表

	COnlineUser*		m_pCurrentSearch;	//查找在线用户的当前索引

	CZObjectPool<COnlineUser>	m_ObjectPool;
	
	uint32				m_nOnlineNum;		//在线用户总数

	CSyncCritical		m_CriticOnlineList;
	CSyncCritical		m_CriticSearch;

	MerchantAccount_Map	Merchant_ids;
	GroupAccount_Map	Group_ids;

};
