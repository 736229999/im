#include "stdafx.h"
#include "NamedPipe.h"

#define CONNECTING_STATE 0 
#define READING_STATE 1 
#define WRITING_STATE 2 


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CNamedPipe::CNamedPipe()
{
	m_hMutex    = NULL;
	m_hPipe     = NULL;
	m_bStopPipe = TRUE;
}

CNamedPipe::~CNamedPipe()
{
	Close();
}

bool CNamedPipe::Initialize(DWORD dwUserID, HWND hNotifyWnd)
{
	DWORD  dwThreadId; 
	HANDLE hThread = NULL;

	m_hNotifyWnd = hNotifyWnd;

	m_bStopPipe = FALSE;

	hThread = CreateThread( 
				NULL,              // no security attribute 
				0,                 // default stack size 
				(LPTHREAD_START_ROUTINE)ListnerProc, 
				(LPVOID) this,    // thread parameter 
				0,                 // not suspended 
				&dwThreadId);      // returns thread ID 

	if (hThread!=NULL)
		CloseHandle(hThread);

	return true;
}

void CNamedPipe::Close()
{
	m_bStopPipe=TRUE;

	if ( m_hPipe )
	{		
		BOOL  fOpenPipe=TRUE;

		HANDLE hPipe = CreateFile( 
						(LPCSTR)m_strPipeName,   // pipe name 
						GENERIC_WRITE, 
						0,              // no sharing 
						NULL,           // no security attributes
						OPEN_EXISTING,  // opens existing pipe 
						0,              // default attributes 
						NULL);          // no template file 

		// Break if the pipe handle is valid. 

		if (hPipe == INVALID_HANDLE_VALUE) 
			fOpenPipe=FALSE;

		// Exit if an error other than ERROR_PIPE_BUSY occurs. 

		if (GetLastError() == ERROR_PIPE_BUSY) 
			fOpenPipe=FALSE;

		// All pipe instances are busy, so wait for 20 seconds. 
		if (! WaitNamedPipe((LPCSTR)m_strPipeName,300) ) 
			fOpenPipe=FALSE;

		char  message[]="exit";
		DWORD cbWritten;

		if ( fOpenPipe )
		{
			WriteFile( 
				hPipe,                  // pipe handle 
				message,     // message 
				strlen(message), // message length 
				&cbWritten,             // bytes written 
				NULL);                  // not overlapped 

			CloseHandle(hPipe); 
		}

		//CloseHandle(m_hPipe);
		m_hPipe = NULL;
	}

	if ( m_hMutex )
	{
		ReleaseMutex(m_hMutex);
        CloseHandle(m_hMutex);
		m_hMutex=NULL; 
	}
}

UINT CNamedPipe::ListnerProc(LPVOID lpParameter)
{
	CNamedPipe *pObj = (CNamedPipe *)lpParameter;

	int nPipeId=0;

	for ( int i=0; i<100;i++ )
	{ 
		CString strMutex;
		strMutex.Format("pn_139_%d",nPipeId);

		pObj->m_hMutex=::CreateMutex(NULL,FALSE,strMutex);

		if ( GetLastError()==ERROR_ALREADY_EXISTS )
		{
			CloseHandle(pObj->m_hMutex);
			nPipeId++;
			continue;
		}	
		else
		{
			pObj->m_strPipeName.Format("\\\\.\\pipe\\pn_139_%d",nPipeId);
			break;
		}
	}

	HANDLE & hPipe = pObj->m_hPipe;

	while ( pObj->m_bStopPipe==FALSE ) 
	{ 
		hPipe = CreateNamedPipe( 
			(LPCSTR)pObj->m_strPipeName,             // pipe name 
			PIPE_ACCESS_DUPLEX,       // read/write access 
			PIPE_TYPE_MESSAGE |       // message type pipe 
			PIPE_READMODE_MESSAGE |   // message-read mode 
			PIPE_WAIT,                // blocking mode 
			PIPE_UNLIMITED_INSTANCES, // max. instances  
			BUFSIZE,                  // output buffer size 
			BUFSIZE,                  // input buffer size 
			PIPE_TIMEOUT,             // client time-out 
			NULL);                    // no security attribute 

		// Wait for the client to connect; if it succeeds, 
		// the function returns a nonzero value. If the function returns 
		// zero, GetLastError returns ERROR_PIPE_CONNECTED. 
		BOOL fConnected = ConnectNamedPipe(hPipe, NULL) ? 

TRUE : (GetLastError() == ERROR_PIPE_CONNECTED); 

		if (fConnected) 
		{ 
			HANDLE hThread = NULL;
			DWORD dwThreadId;

			tagParam * pT = new tagParam();
			pT->pObj  = pObj;
			pT->hPipe = hPipe;
			//	 Create a thread for this client. 
			hThread = CreateThread( 
				NULL,              // no security attribute 
				0,                 // default stack size 
				(LPTHREAD_START_ROUTINE) ClientProc, 
				(LPVOID)pT,    // thread parameter 
				0,                 // not suspended 
				&dwThreadId);      // returns thread ID 

			if (hThread != NULL) 
				CloseHandle(hThread); 
		} 
		else 
		{
			// The client could not connect, so close the pipe. 
			CloseHandle(hPipe); 
		}
	} 

 	return 0;
}

UINT CNamedPipe::ClientProc(LPVOID lpvParam) 
{ 
	CHAR	chRequest[BUFSIZE]; 
	DWORD	cbBytesRead; 
	BOOL	fSuccess; 

	tagParam * pT = (tagParam *)lpvParam;

	if ( pT->pObj->m_bStopPipe==FALSE )
	{
		fSuccess = ReadFile( 
			pT->hPipe,        // handle to pipe 
			chRequest,    // buffer to receive data 
			BUFSIZE,      // size of buffer 
			&cbBytesRead, // number of bytes read 
			NULL);        // not overlapped I/O 

		if ( fSuccess && cbBytesRead>0) 
		{
			chRequest[cbBytesRead]=0;
			::SendMessage(pT->pObj->m_hNotifyWnd,WM_WEB_CALLME,0,(LPARAM)chRequest);
		}
	}

	DisconnectNamedPipe(pT->hPipe); 
	CloseHandle(pT->hPipe); 

	delete pT;

	return 0;
}