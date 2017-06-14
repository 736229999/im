#pragma once

#include "XSyncCritical.h"

//------------------------------------------------------
// ��Դ��
//------------------------------------------------------

class XLock
{
public:

	XLock(CRITICAL_SECTION *pSec)
	{
		m_pSec = pSec;
		::EnterCriticalSection(pSec);
	};

	~XLock(void)
	{
		::LeaveCriticalSection(m_pSec);
	};

private:

	CRITICAL_SECTION *	m_pSec;

};

