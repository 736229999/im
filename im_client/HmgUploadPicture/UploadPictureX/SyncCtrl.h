
//////////////////////////////////////////////////////////////////////
//

#ifndef _eps_mfc_thread_SYNCCTRL_H_	
#define _eps_mfc_thread_SYNCCTRL_H_	

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSyncCriticalSection
{
public:
	CSyncCriticalSection()
	{
		::InitializeCriticalSection(&m_CritSect);
	}

	~CSyncCriticalSection()
	{
		::DeleteCriticalSection(&m_CritSect);	
	}
	
	void Lock(void)
	{
		::EnterCriticalSection(&m_CritSect);
	}
	
	void Unlock(void)
	{
		::LeaveCriticalSection(&m_CritSect);
	}
public:
	DWORD GetCriticalSectionThreadID()	const
	{
		return m_dwThreadID;
	}
private:
	DWORD m_dwThreadID;
private:
	CRITICAL_SECTION m_CritSect;
private:
	//dummy copy con-strutcor and operator= to prevent copying

//	CSyncCriticalSection(const CSyncCriticalSection &);
//	CSyncCriticalSection & operator= (const CSyncCriticalSection &);
};


class CSyncCriticalSectionLock
{
public:
	CSyncCriticalSectionLock(CSyncCriticalSection & _CriticalSection) : m_CriticalSection(_CriticalSection)
	{
	//	if(m_CriticalSection.GetCriticalSectionThreadID() != GetCurrentThreadId())
		{
			m_CriticalSection.Lock();
		}
	}

	~CSyncCriticalSectionLock()
	{
	//	if(m_CriticalSection.GetCriticalSectionThreadID() != GetCurrentThreadId())
		{
			m_CriticalSection.Unlock();
		}
	}

private:
	CSyncCriticalSection & m_CriticalSection;
};

class CSyncEvent
{
public:
	CSyncEvent()
	{
		m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	}

	~CSyncEvent()
	{
		if(m_hEvent != NULL)	CloseHandle(m_hEvent);
	}
public:
	BOOL Set()
	{
		return	SetEvent(m_hEvent);
	}
	BOOL Reset()
	{
		return	ResetEvent(m_hEvent);
	}

	DWORD Wait()
	{
		return	WaitForSingleObject(m_hEvent, INFINITE);
	}

	DWORD Wait(unsigned long ltm)
	{
		return	WaitForSingleObject(m_hEvent, ltm);
	}
private:
	HANDLE m_hEvent;
};



































#endif // _eps_mfc_thread_SYNCCTRL_H_	