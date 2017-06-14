#include "StdAfx.h"
#include ".\syncevent.h"

CSyncEvent::CSyncEvent(	bool bManualReset,bool bInitialState)
{
	m_handle=::CreateEvent(NULL,(BOOL)bManualReset,(BOOL)bInitialState,NULL);
}

CSyncEvent::~CSyncEvent(void)
{
	if( m_handle )
	{
		::CloseHandle(m_handle);
	}
}


void CSyncEvent::SetEvent()
{
	::SetEvent(m_handle);
}

void CSyncEvent::ResetEvent()
{
	::ResetEvent(m_handle);
}

void CSyncEvent::Wait()
{
	::WaitForSingleObject(m_handle,INFINITE);
}