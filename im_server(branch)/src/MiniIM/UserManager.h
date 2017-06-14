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
#ifndef IM_USE_MULTIDEVICE
	bool Del(uint32 userid);	
#else
	bool Del(uint32 userid,uint32 deviceid);	
#endif

	// ����һ�����û��ڴ�
	shared_ptr<CUser> CreateNew();
	// ��ȡ�����б�
	static bool GetFriendList(int id, int merchantId, CSortArray<FriendInfo> &friendlist);
	// ��ȡ���ѷ�����Ϣ(��ʱֻ��ȡ�ͷ�������Ϣ)
	static bool GetGroupList(uint32 uid, vector<XT_GROUPINFO_GET_ACK::tagGroupInfo> &vecGroupInfo);

	//�ǳ�������
	enum LOGOUT_LOCK_HANDLE_TYPE{ 
		LOGOUT_LOCK_ADD = 0,   //��ӵǳ���
		LOGOUT_LOCK_GET = 1,   //��ӵǳ���
		LOGOUT_LOCK_DEL = 2,   //��ӵǳ���
	};
	bool UserLogoutLockHandle(uint32 userId, LOGOUT_LOCK_HANDLE_TYPE handleType);

private:
	

private:
	CGlobalBusi *m_pGlobalBusi;
};
