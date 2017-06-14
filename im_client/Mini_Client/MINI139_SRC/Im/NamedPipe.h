// NamedPipe.h: interface for the CNamedPipe class.
//
// Author:    Emil Gustafsson (e@ntier.se), 
//            NTier Solutions (www.ntier.se)
// Created:   2000-01-25
// Copyright: This code may be reused and/or editied in any project
//            as long as this original note (Author and Copyright)
//            remains in the source files.
//////////////////////////////////////////////////////////////////////

#if !defined(NAMEDPIPE_H)
#define NAMEDPIPE_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
using namespace std;

#define BUFSIZE		 1024
#define PIPE_TIMEOUT  (120*1000) /*120 seconds*/

class CNamedPipe  
{
public:
	CNamedPipe();
	virtual ~CNamedPipe();

	bool Initialize(DWORD dwUserID, HWND hNotifyWnd);
	void Close();

	static UINT ListnerProc(LPVOID lpParameter);	
	static UINT ClientProc(LPVOID lpParameter);

protected:
	struct tagParam
	{
		CNamedPipe * pObj;
		HANDLE  hPipe;
	};

protected:
	CString m_strPipeName;
	HWND    m_hNotifyWnd;
	HANDLE  m_hMutex;
	HANDLE  m_hPipe;
	BOOL	m_bStopPipe ;

};

#endif // !defined(NAMEDPIPE_H)
