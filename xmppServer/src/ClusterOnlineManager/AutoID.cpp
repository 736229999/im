#include "stdafx.h"
#include "AutoID.h"

CAutoID::CAutoID(void)
{
	m_nMaxIDNum = 1;
}

CAutoID::~CAutoID(void)
{
}

int CAutoID::GetNewID()
{
	m_SyncLock.Lock();

	int nNewID = 0;
	if( m_listBackup.size() > 0 )
	{
		nNewID = m_listBackup.front();
		m_listBackup.pop_front();
	}
	else
	{
		nNewID = m_nMaxIDNum++;
	}
	
	m_SyncLock.Unlock();

	return nNewID;
}

void CAutoID::DeleteID(int nOldID)
{
	m_SyncLock.Lock();	

	m_listBackup.push_back(nOldID);

	m_SyncLock.Unlock();
}
