#include "StdAfx.h"
#include ".\syncsemaphore.h"

CSyncSemaphore::CSyncSemaphore(int nInitValue,int nMaxCount)
{
	m_handle = ::CreateSemaphore( NULL, nInitValue, nMaxCount, NULL );
}

CSyncSemaphore::~CSyncSemaphore(void)
{
	if( m_handle != NULL )
	{
		::CloseHandle( m_handle );
		m_handle = NULL;
	}
}


void CSyncSemaphore::Lock()
{
	assert(m_handle);

	WaitForSingleObject(m_handle,INFINITE);
}

void CSyncSemaphore::Unlock()
{
	assert(m_handle);

	::ReleaseSemaphore( m_handle, 1, NULL ) ;
}