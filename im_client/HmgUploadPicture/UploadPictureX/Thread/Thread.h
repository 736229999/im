// Thread.h: interface for the Thread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_eps_mfc_thread_THREAD_H__4BB3C935_129A_46FA_9D5D_CFAE3EE29A14__INCLUDED_)
#define AFX_eps_mfc_thread_THREAD_H__4BB3C935_129A_46FA_9D5D_CFAE3EE29A14__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class Thread  
{
public:
	Thread();
	virtual ~Thread();
public:
	BOOL ThreadIsStop()const;
	DWORD GetThreadID()const;
	void StopThread();
	HANDLE GetThreadHandle()const;
	HANDLE StartThread(const int nPriority = THREAD_PRIORITY_NORMAL);

public:
	void ExitRunFunc();
	BOOL IsExitRunFunc()const;
	BOOL ThreadIsExited()const;
	void ExitThisThread();
	virtual void Run();
private:
	BOOL m_bPause;
	HANDLE m_hThread;
private:
	BOOL m_bStop;
	DWORD m_dwThreadID;
private:
	BOOL m_bExitedRunFunc;
	BOOL m_bExitThisThread;
};


#endif // !defined(AFX_eps_mfc_thread_THREAD_H__4BB3C935_129A_46FA_9D5D_CFAE3EE29A14__INCLUDED_)
