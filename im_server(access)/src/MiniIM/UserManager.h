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
	// ��redis�в����û�
	shared_ptr<CUser> Find(uint32 userid, BOOL bGetFriendList = FALSE);
	// �û���Ϣ��д��redis
	bool AddUserToRedis(const shared_ptr<CUser> &pUser);
	// ��redis��ɾ���û�
	bool Del(uint32 userid);
	// ����һ�����û��ڴ�
	shared_ptr<CUser> CreateNew();
	// ��ȡ�����б�
	static bool GetFriendList(int id, int merchantId, CSortArray<FriendInfo> &friendlist);
	// ��ȡ���ѷ�����Ϣ(��ʱֻ��ȡ�ͷ�������Ϣ)
	static bool GetGroupList(uint32 uid, vector<XT_GROUPINFO_GET_ACK::tagGroupInfo> &vecGroupInfo);
	
private:
	
	
private:
	CGlobalBusi *m_pGlobalBusi;
};