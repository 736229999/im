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
	bool Del(uint32 userid);
	// 创建一个新用户内存
	shared_ptr<CUser> CreateNew();
	// 获取好友列表
	static bool GetFriendList(int id, int merchantId, CSortArray<FriendInfo> &friendlist);
	// 获取好友分组信息(暂时只获取客服分组信息)
	static bool GetGroupList(uint32 uid, vector<XT_GROUPINFO_GET_ACK::tagGroupInfo> &vecGroupInfo);
	
private:
	
	
private:
	CGlobalBusi *m_pGlobalBusi;
};