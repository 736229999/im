#pragma once

#include "PublicType.h"
#include "WinCommonForLinux.h"
#include "SyncCritical.h"
#include <vector>


// xmpp会话管理类
struct STRU_XMPP_SESSION;
class CXmppSession
{
public:
	CXmppSession();
    ~CXmppSession();
	void Add(uint32 userid, uint32 deviceid, void *sess);
	void Del(uint32 userid, uint32 deviceid, BOOL bCloseSession = TRUE);
	BOOL IsExist(uint32 userid, uint32 deviceid);
	void* GetSess(uint32 userid, uint32 deviceid);
	
private:
	std::vector<STRU_XMPP_SESSION*>	m_vecXmppSession;
	CSyncCritical m_lock;		
};
