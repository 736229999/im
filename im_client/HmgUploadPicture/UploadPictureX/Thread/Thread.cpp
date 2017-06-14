// Thread.cpp: implementation of the Thread class.
//
//////////////////////////////////////////////////////////////////////

#include "..\..\stdafx.h"
#include "Thread.h"

#include <process.h>
#include <sstream>


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


static unsigned __stdcall ThreadProc(LPVOID lParam)
{
//	const static _bstr_t FUNC (L"static unsigned __stdcall ThreadProc(LPVOID)");

	Thread * pThis = static_cast<Thread *> (lParam);
	
	CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

//	CSEH::MapSEtoEH();

	try
	{
		pThis ->Run();

		//////////////////////////////////////////////////////////////////////////
		
	//	CloseHandle(pThis ->GetThreadHandle());
	}
	catch ( ... )
	{
//		GlobalInstance::INSTANCE() ->WriteLog_Error(FUNC, "unknown exception caught.", NULL);
	}

	CoUninitialize();
	
	return 0;
}	

Thread::Thread()
{
	m_bStop		 = FALSE;
	m_hThread	 = NULL;
	m_bPause	 = FALSE;

	m_dwThreadID = 0;
	m_bExitThisThread = FALSE;

	m_bExitedRunFunc = FALSE;
}

Thread::~Thread()
{

}

HANDLE Thread::StartThread(const int nPriority)
{
	unsigned int dwThreadID = 0;	
	const static LPCTSTR FUNC = "HANDLE Thread::Start(int)";

	m_hThread = (HANDLE) _beginthreadex(NULL,	
								0,
								ThreadProc,
								reinterpret_cast<void *>(this),
								0,
								&dwThreadID);
	m_dwThreadID = dwThreadID;

	if(m_hThread != NULL )
	{
		SetThreadPriority(m_hThread, nPriority);
	}
	
	{
		ostringstream info;

		info <<"Thread ID: " << dwThreadID << " Is started.";
//		GlobalInstance ::WriteLog(FUNC, info.str().c_str(), NULL);
		TRACE("Thread::StartThread %s\n", info.str().c_str());
	}
	
	return m_hThread;
}

void Thread::Run()
{
	// Nothing ...
}

HANDLE Thread::GetThreadHandle() const
{
	return m_hThread;
}


void Thread::StopThread()
{
	m_bStop = TRUE;
}


DWORD Thread::GetThreadID() const
{
	return m_dwThreadID;
}

BOOL Thread::ThreadIsStop() const
{
	return m_bStop;
}

void Thread::ExitThisThread()
{
	m_bExitThisThread = TRUE;
}

BOOL Thread::ThreadIsExited() const
{
	return m_bExitThisThread;
}

BOOL Thread::IsExitRunFunc()const
{
	return m_bExitedRunFunc;
}

void Thread::ExitRunFunc()
{
	m_bExitedRunFunc = TRUE;
}
