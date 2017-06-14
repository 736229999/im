#include "XmppSession.h"
#include "jabberd/c2s/c2s.h"

using namespace std;

struct STRU_XMPP_SESSION
{
	uint32 id;
	uint32 deviceid;
	sess_t sess;
};

CXmppSession::CXmppSession()
{

}

CXmppSession::~CXmppSession()
{
	
}

void CXmppSession::Add(uint32 userid, uint32 deviceid, void *sess)
{
	if(IsExist(userid,deviceid))
	{
		return;
	}

	m_lock.Lock();
	STRU_XMPP_SESSION *pxmppSess = new STRU_XMPP_SESSION();
	pxmppSess->id = userid;
	pxmppSess->deviceid = deviceid;
	pxmppSess->sess = (sess_t)sess;

	m_vecXmppSession.push_back(pxmppSess);
	m_lock.Unlock();
}

void CXmppSession::Del(uint32 userid, uint32 deviceid, BOOL bCloseSession /*= TRUE*/)
{
	m_lock.Lock();
	int idx = -1;
	for(int i = 0; i < (int)m_vecXmppSession.size(); ++i)
	{
		STRU_XMPP_SESSION *pxmppSess = m_vecXmppSession[i];
		if(pxmppSess->id == userid && pxmppSess->deviceid == deviceid)
		{
			idx = i;
			break;
		}
	}

	if(idx != -1)
	{
		STRU_XMPP_SESSION *pxmppSess = m_vecXmppSession[idx];
		m_vecXmppSession.erase(m_vecXmppSession.begin() + idx);
		if(bCloseSession)
        	mio_close(pxmppSess->sess->c2s->mio, pxmppSess->sess->fd);
        pxmppSess->sess->fd = NULL;
	}
	m_lock.Unlock();
}

BOOL CXmppSession::IsExist(uint32 userid, uint32 deviceid)
{
	int idx = -1;
	for(int i = 0; i < (int)m_vecXmppSession.size(); ++i)
	{
		STRU_XMPP_SESSION *pxmppSess = m_vecXmppSession[i];
		if(pxmppSess->id == userid && pxmppSess->deviceid == deviceid)
		{
			idx = i;
			break;
		}
	}
	return idx != -1;
}

void* CXmppSession::GetSess(uint32 userid, uint32 deviceid)
{
	for(int i = 0; i < (int)m_vecXmppSession.size(); ++i)
	{
		STRU_XMPP_SESSION *pxmppSess = m_vecXmppSession[i];
		if(pxmppSess->id == userid && pxmppSess->deviceid == deviceid)
		{
			return (void*)pxmppSess->sess;
		}
	}

	return NULL;
}

