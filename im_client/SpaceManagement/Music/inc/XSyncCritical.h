#pragma once

#include <Windows.h>

//------------------------------------------------------
// �첽�ٽ���
//------------------------------------------------------

class XSyncCritical
{
public:

	XSyncCritical(void)
	{
		::InitializeCriticalSection(&m_sect);
	}

	~XSyncCritical(void)
	{
		::DeleteCriticalSection(&m_sect);
	}


	void Lock()
	{
		::EnterCriticalSection(&m_sect);
	}

	void Unlock()
	{
		::LeaveCriticalSection(&m_sect);	
	}

	CRITICAL_SECTION * Get()
	{ 
		return &m_sect;
	};

private:

	CRITICAL_SECTION	m_sect;

};
