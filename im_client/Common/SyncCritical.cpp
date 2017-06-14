#include "StdAfx.h"
#include ".\synccritical.h"

CSyncCritical::CSyncCritical(void)
{
	::InitializeCriticalSection(&m_sect);
}

CSyncCritical::~CSyncCritical(void)
{
	::DeleteCriticalSection(&m_sect);
}


void CSyncCritical::Lock()
{
	::EnterCriticalSection(&m_sect);
}

void CSyncCritical::Unlock()
{
	::LeaveCriticalSection(&m_sect);	
}