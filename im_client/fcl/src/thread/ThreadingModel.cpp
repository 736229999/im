/******************************************************************
** �ļ���:	ThreadingModel.cpp
** ��  Ȩ:	
** ������:		
** ��  ��:	
** ��  ��: 	�߳�ģ�Ͷ���,���԰Ѹ����߳�ģ����Ϊ��������ģ���

**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
*******************************************************************/

#include "stdafx.h"
#include ".\thread\ThreadingModel.h"

#include <windows.h>



///////////////////////////////////////////////////////////////////
void CMutexWrapper::do_lock()
{

	EnterCriticalSection((CRITICAL_SECTION*)m_handle);
}

void CMutexWrapper::do_unlock()
{

	LeaveCriticalSection((CRITICAL_SECTION*)m_handle);

}

CMutexWrapper::CMutexWrapper() : m_handle(0)
{

	m_handle = new CRITICAL_SECTION;
	InitializeCriticalSection((CRITICAL_SECTION*)m_handle);

}

CMutexWrapper::~CMutexWrapper()
{
	if (m_handle)
	{

		DeleteCriticalSection((CRITICAL_SECTION*)m_handle);
		delete (CRITICAL_SECTION*)m_handle;
		m_handle = 0;
	}

}

///////////////////////////////////////////////////////////////////
CEventWrapper::CEventWrapper(): m_handle(0)
{
	m_handle = CreateEvent(NULL,FALSE,FALSE,NULL);
}

CEventWrapper::~CEventWrapper()
{
	if (m_handle)
	{
		CloseHandle(m_handle);
		m_handle = 0;
	}
}

void CEventWrapper::Active()
{

	SetEvent(m_handle);
}


///////////////////////////////////////////////////////////////////
uint32 CMultiThread::AtomicIncrement(volatile uint32& lval)
{ 
	return InterlockedIncrement((long*)&(lval)); 
}

uint32 CMultiThread::AtomicDecrement(volatile uint32& lval)
{ 
	return InterlockedDecrement((long*)&(lval)); 
}

void CMultiThread::AtomicAssign(volatile uint32& lval, uint32 val)
{
	InterlockedExchange((long*)&(lval), val); 
}

void CMultiThread::AtomicAssign(uint32& lval, volatile uint32& val)
{
	InterlockedExchange((long*)&lval, val); 

}

