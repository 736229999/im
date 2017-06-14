#pragma once

#include "protocol.h"
#include "PublicType.h"
#include "Utility.h"
#include "SortArray.h"
#include <ext/hash_map>
#include "RouterProtocol.h"
#include "User.h"
#include <memory>
using namespace std;

class CGlobalBusi;

using namespace __gnu_cxx;

class CUserManager
{
public:
	CUserManager(CGlobalBusi *pGlobalBusi);
	~CUserManager();
	// 从redis中查找用户
	shared_ptr<CUser> Find(uint32 userid, BOOL bGetFriendList = FALSE);
	// 用户信息回写入redis
	bool AddUserToRedis(const shared_ptr<CUser> &pUser);
	// 从redis中删除用户
#ifndef IM_USE_MULTIDEVICE
	bool Del(uint32 userid);	
#else
	bool Del(uint32 userid,uint32 deviceid);	
#endif

	// 创建一个新用户内存
	shared_ptr<CUser> CreateNew();
	// 获取好友列表
	static bool GetFriendList(int id, int merchantId, CSortArray<FriendInfo> &friendlist);
	// 获取好友分组信息(暂时只获取客服分组信息)
	static bool GetGroupList(uint32 uid, vector<XT_GROUPINFO_GET_ACK::tagGroupInfo> &vecGroupInfo);

	//登出锁操作
	enum LOGOUT_LOCK_HANDLE_TYPE{ 
		LOGOUT_LOCK_ADD = 0,   //添加登出锁
		LOGOUT_LOCK_GET = 1,   //添加登出锁
		LOGOUT_LOCK_DEL = 2,   //添加登出锁
	};
	bool UserLogoutLockHandle(uint32 userId, LOGOUT_LOCK_HANDLE_TYPE handleType);

private:
	

private:
	CGlobalBusi *m_pGlobalBusi;
};
