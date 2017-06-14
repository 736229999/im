#ifdef _WIN32

#include "stdafx.h"
#include "ZTcpComm.h"
#include <stdio.h>


CZTcpComm::CZTcpComm(int nConsumeThreadCount)
{
	m_pfnUserCallback = NULL;
	m_nThreads = nConsumeThreadCount;
}

CZTcpComm::~CZTcpComm(void)
{
}


bool CZTcpComm::Initialize()
{
	m_phWorkerThreads = new HANDLE[m_nThreads];
	InitializeCriticalSection(&m_csClientList);
	m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	m_hIOCompletionPort = NULL;

	// Initialize Winsock
	WSADATA wsaData;
	int nResult;
	nResult = WSAStartup(MAKEWORD(2,2), &wsaData);

	if (NO_ERROR != nResult)
	{
		//TRACE("\nError occurred while executing WSAStartup().");
		return false; //error
	}
	if (false == InitializeIOCP())
	{
//		TRACE("\nError occurred while initializing IOCP");
		return false;
	}
	return true;
}
bool CZTcpComm::InitializeIOCP()
{
	//Create I/O completion port
	m_hIOCompletionPort = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, 0 );

	if ( NULL == m_hIOCompletionPort)
	{
//		TRACE("\nError occurred while creating IOCP: %d.", WSAGetLastError());
		return false;
	}

	DWORD nThreadID;

	//Create worker threads
	for (int i = 0; i < m_nThreads; i++)
	{
		m_phWorkerThreads[i] = CreateThread(0, 0, WorkerThread, this, 0, &nThreadID);
	}

	return true;
}
void CZTcpComm::CleanUp()
{
	//Ask all threads to start shutting down
	SetEvent(m_hShutdownEvent);

	//Let Accept thread go down
	WaitForSingleObject(m_hAcceptThread, INFINITE);

	for (int i = 0; i < m_nThreads; i++)
	{
		//Help threads get out of blocking - GetQueuedCompletionStatus()
		PostQueuedCompletionStatus(m_hIOCompletionPort, 0, (DWORD) NULL, NULL);
	}

	//Let Worker Threads shutdown
	WaitForMultipleObjects(m_nThreads, m_phWorkerThreads, TRUE, INFINITE);

	//We are done with this event
	WSACloseEvent(m_hAcceptEvent);

	//Cleanup dynamic memory allocations, if there are any.
	CleanClientList();
}

void CZTcpComm::DeInitialize()
{
	//Delete the Client List Critical Section.
	DeleteCriticalSection(&m_csClientList);

	//Cleanup IOCP.
	CloseHandle(m_hIOCompletionPort);

	//Clean up the event.
	CloseHandle(m_hShutdownEvent);

	//Clean up memory allocated for the storage of thread handles
	delete[] m_phWorkerThreads;

	//Cleanup Winsock
	WSACleanup();
}

int CZTcpComm::StartListen(const char* addr,int nPort)
{
	Initialize();
	char szError[128] = {0};
	SOCKADDR_IN ServerAddress;
	ZeroMemory((char *)&ServerAddress, sizeof(ServerAddress));
	ServerAddress.sin_family = AF_INET;
	if(strcmp(addr,"") == 0)
		ServerAddress.sin_addr.s_addr = INADDR_ANY;
	else
		ServerAddress.sin_addr.s_addr = inet_addr(addr);
	ServerAddress.sin_port = htons(nPort);

	m_ListenSocket = WSASocket(AF_INET,SOCK_STREAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	if(m_ListenSocket == INVALID_SOCKET )
	{
//		TRACE("Error ccurred while opening socket: %d.", WSAGetLastError());
		return 1;
	}

	if(SOCKET_ERROR == bind(m_ListenSocket,(struct sockaddr *)&ServerAddress,sizeof(ServerAddress)))
	{
		closesocket(m_ListenSocket);
//		TRACE("\nError occurred while binding:%d!", WSAGetLastError());
		return 1;
	}

	if (SOCKET_ERROR == listen(m_ListenSocket,SOMAXCONN))
	{
		closesocket(m_ListenSocket);
//		TRACE("Error occurred while listening:%d!", WSAGetLastError());
		return 1;
	}

	m_hAcceptEvent = WSACreateEvent();

	if (WSA_INVALID_EVENT == m_hAcceptEvent)
	{
//		TRACE("\nError occurred while WSACreateEvent().");
		return 1;
	}

	if (SOCKET_ERROR == WSAEventSelect(m_ListenSocket, m_hAcceptEvent, FD_ACCEPT))
	{
//		TRACE("\nError occurred while WSAEventSelect().");
		WSACloseEvent(m_hAcceptEvent);
		return 1;
	}

	DWORD nThreadID;
	m_hAcceptThread = CreateThread(0, 0, AcceptThread, this, 0, &nThreadID);

	return 0;
}
int CZTcpComm::StopListen()
{
	//Start cleanup
	CleanUp();

	//Close open sockets
	closesocket(m_ListenSocket);

	DeInitialize();

	return 0; //success
}

DWORD WINAPI CZTcpComm::AcceptThread(LPVOID lParam)
{
	CZTcpComm *pNet  = (CZTcpComm*)lParam;

	WSANETWORKEVENTS WSAEvents;

	//Accept thread will be around to look for accept event, until a Shutdown event is not Signaled.
	while(WAIT_OBJECT_0 != WaitForSingleObject(pNet->m_hShutdownEvent, 0))
	{
		if (WSA_WAIT_TIMEOUT != WSAWaitForMultipleEvents(1, &pNet->m_hAcceptEvent, FALSE, WAIT_TIMEOUT_INTERVAL, FALSE))
		{
			WSAEnumNetworkEvents(pNet->m_ListenSocket, pNet->m_hAcceptEvent, &WSAEvents);
			if ((WSAEvents.lNetworkEvents & FD_ACCEPT) && (0 == WSAEvents.iErrorCode[FD_ACCEPT_BIT]))
			{
				pNet->AcceptConnection(pNet->m_ListenSocket);
			}
		}
	}

	return 0;
}

//This function will process the accept event
void CZTcpComm::AcceptConnection(SOCKET ListenSocket)
{
	sockaddr_in ClientAddress;
	int nClientLength = sizeof(ClientAddress);

	//Accept remote connection attempt from the client
	SOCKET Socket = accept(ListenSocket, (sockaddr*)&ClientAddress, &nClientLength);

	if (INVALID_SOCKET == Socket)
	{
//		TRACE("\nError occurred while accepting socket: %ld.", WSAGetLastError());
	}

	//Display Client's IP
#ifdef _DEBUG
	printf("\nClient connected from: %s", inet_ntoa(ClientAddress.sin_addr));
#endif

	//Create a new ClientContext for this newly accepted client
	CClientContext   *pClientContext  = new CClientContext;

	pClientContext->SetOpCode(OP_READ);
	pClientContext->SetSocket(Socket);

	//Store this object
	AddToClientList(pClientContext);

	if (true == AssociateWithIOCP(pClientContext))
	{
		//Once the data is successfully received, we will print it.
		pClientContext->SetOpCode(OP_WRITE);
		pClientContext->SetIP(inet_ntoa(ClientAddress.sin_addr));

		WSABUF *p_wbuf = pClientContext->GetWSABUFPtr();
		OVERLAPPED *p_ol = pClientContext->GetOVERLAPPEDPtr();

		//Get data.
		DWORD dwFlags = 0;
		DWORD dwBytes = 0;

		//Post initial Recv
		//This is a right place to post a initial Recv
		//Posting a initial Recv in WorkerThread will create scalability issues.
		int nBytesRecv = WSARecv(pClientContext->GetSocket(), p_wbuf, 1, 
			&dwBytes, &dwFlags, p_ol, NULL);

		if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != WSAGetLastError()))
		{
			//WriteToConsole("\nError in Initial Post.");
		}
	}
}

bool CZTcpComm::AssociateWithIOCP(CClientContext   *pClientContext)
{
	//Associate the socket with IOCP
	HANDLE hTemp = CreateIoCompletionPort((HANDLE)pClientContext->GetSocket(), m_hIOCompletionPort, (DWORD)pClientContext, 0);

	if (NULL == hTemp)
	{
//		TRACE("\nError occurred while executing CreateIoCompletionPort().");

		//Let's not work with this client
		RemoveFromClientListAndFreeMemory(pClientContext);

		return false;
	}

	return true;
}

//Worker thread will service IOCP requests
DWORD WINAPI CZTcpComm::WorkerThread(LPVOID lpParam)
{    
	CZTcpComm *pNet = (CZTcpComm*)lpParam;
	pNet->ProcessData();
	return 0;
}


//Store client related information in a vector
void CZTcpComm::AddToClientList(CClientContext   *pClientContext)
{
	EnterCriticalSection(&m_csClientList);

	//Store these structures in vectors
	m_ClientContext.push_back(pClientContext);

	LeaveCriticalSection(&m_csClientList);
}

//This function will allow to remove one single client out of the list
void CZTcpComm::RemoveFromClientListAndFreeMemory(CClientContext   *pClientContext)
{
	EnterCriticalSection(&m_csClientList);

	std::vector <CClientContext *>::iterator IterClientContext;

	//Remove the supplied ClientContext from the list and release the memory
	for (IterClientContext = m_ClientContext.begin(); IterClientContext != m_ClientContext.end(); IterClientContext++)
	{
		if (pClientContext == *IterClientContext)
		{
			m_ClientContext.erase(IterClientContext);

			//i/o will be cancelled and socket will be closed by destructor.
			delete pClientContext;
			break;
		}
	}

	LeaveCriticalSection(&m_csClientList);
}

//Clean up the list, this function will be executed at the time of shutdown
void CZTcpComm::CleanClientList()
{
	EnterCriticalSection(&m_csClientList);

	std::vector <CClientContext *>::iterator IterClientContext;

	for (IterClientContext = m_ClientContext.begin(); IterClientContext != m_ClientContext.end( ); IterClientContext++)
	{
		//i/o will be cancelled and socket will be closed by destructor.
		delete *IterClientContext;
	}

	m_ClientContext.clear();

	LeaveCriticalSection(&m_csClientList);
}

//The use of static variable will ensure that 
//we will make a call to GetSystemInfo() 
//to find out number of processors, 
//only if we don't have the information already.
//Repeated use of this function will be efficient.
int CZTcpComm::GetNoOfProcessors()
{
	static int nProcessors = 0;

	if (0 == nProcessors)
	{
		SYSTEM_INFO si;

		GetSystemInfo(&si);

		nProcessors = si.dwNumberOfProcessors;
	}

	return nProcessors;
}

int CZTcpComm::SendData(void *pContext,char *sendData,int len)
{
	CClientContext *pClientContext = (CClientContext*)pContext;

	int nBytesSent = 0;

	pClientContext->AddSendData(sendData);
	WSABUF *p_wbuf = pClientContext->GetWSABUFPtr();
	OVERLAPPED *p_ol = pClientContext->GetOVERLAPPEDPtr();
	p_wbuf->buf += pClientContext->GetSentBytes();
	p_wbuf->len = pClientContext->GetTotalBytes() - pClientContext->GetSentBytes();

	DWORD	dwBytes = 0, dwFlags = 0;

	//Overlapped send
	
	nBytesSent = WSASend(pClientContext->GetSocket(), p_wbuf, 1, 
		&dwBytes, dwFlags, p_ol, NULL);

	if ((SOCKET_ERROR == nBytesSent) && (WSA_IO_PENDING != WSAGetLastError()))
	{
		//WriteToConsole("\nThread %d: Error occurred while executing WSASend().", nThreadNo);

		//Let's not work with this client
		RemoveFromClientListAndFreeMemory(pClientContext);
	}	
	return 0;
}

void CZTcpComm::ProcessData()
{
	void *lpContext = NULL;
	OVERLAPPED       *pOverlapped = NULL;
	CClientContext   *pClientContext = NULL;
	DWORD            dwBytesTransfered = 0;
	int nBytesRecv = 0;
	int nBytesSent = 0;
	DWORD             dwBytes = 0, dwFlags = 0;

	//Worker thread will be around to process requests, until a Shutdown event is not Signaled.
	while (WAIT_OBJECT_0 != WaitForSingleObject(m_hShutdownEvent, 0))
	{
		//waiting recv event on completionport
		BOOL bReturn = GetQueuedCompletionStatus(
			m_hIOCompletionPort,
			&dwBytesTransfered,
			(LPDWORD)&lpContext,
			&pOverlapped,
			INFINITE);

		if (NULL == lpContext)
		{
			//We are shutting down
			break;
		}

		//Get the client context
		pClientContext = (CClientContext *)lpContext;

		if ((FALSE == bReturn) || ((TRUE == bReturn) && (0 == dwBytesTransfered)))
		{
			//Client connection gone, remove it.
#ifdef _DEBUG
			printf("\nClient connection exit: %s", pClientContext->GetIP());
#endif			
			RemoveFromClientListAndFreeMemory(pClientContext);

			continue;
		}

		WSABUF *p_wbuf = pClientContext->GetWSABUFPtr();
		OVERLAPPED *p_ol = pClientContext->GetOVERLAPPEDPtr();

		switch (pClientContext->GetOpCode())
		{
		case OP_READ:

			pClientContext->IncrSentBytes(dwBytesTransfered);

			//Write operation was finished, see if all the data was sent.
			//Else post another write.
			if(pClientContext->GetSentBytes() < pClientContext->GetTotalBytes())
			{
				pClientContext->SetOpCode(OP_READ);

				p_wbuf->buf += pClientContext->GetSentBytes();
				p_wbuf->len = pClientContext->GetTotalBytes() - pClientContext->GetSentBytes();

				dwFlags = 0;

				//Overlapped send
				nBytesSent = WSASend(pClientContext->GetSocket(), p_wbuf, 1, 
					&dwBytes, dwFlags, p_ol, NULL);

				if ((SOCKET_ERROR == nBytesSent) && (WSA_IO_PENDING != WSAGetLastError()))
				{
					//Let's not work with this client
					RemoveFromClientListAndFreeMemory(pClientContext);
				}
#ifdef _DEBUG
				printf("\nSend message To: %s %d Bytes", pClientContext->GetIP(),nBytesSent);
#endif
			}
			else
			{
				//Once the data is successfully received, we will print it.
				pClientContext->SetOpCode(OP_WRITE);
				pClientContext->ResetWSABUF();

				dwFlags = 0;

				//Get the data.
				nBytesRecv = WSARecv(pClientContext->GetSocket(), p_wbuf, 1, 
					&dwBytes, &dwFlags, p_ol, NULL);

				if ((SOCKET_ERROR == nBytesRecv) && (WSA_IO_PENDING != WSAGetLastError()))
				{
					//WriteToConsole("\nThread %d: Error occurred while executing WSARecv().", nThreadNo);

					//Let's not work with this client
					RemoveFromClientListAndFreeMemory(pClientContext);
				}
			}

			break;

		case OP_WRITE://has Recv Client a msg

			char szBuffer[MAX_BUFFER_LEN];

			//Display the message we recevied
			pClientContext->GetBuffer(szBuffer);


			//WriteToConsole("\nThread %d: The following message was received: %s", nThreadNo, szBuffer);
#ifdef _DEBUG
			printf("\nThe message was received from: %s %d Bytes", pClientContext->GetIP(),dwBytesTransfered);
#endif
			//Send the message back to the client.
			pClientContext->SetOpCode(OP_READ);


			pClientContext->SetTotalBytes(dwBytesTransfered);
			pClientContext->SetSentBytes(0);

			m_pfnUserCallback((void*)pClientContext,pClientContext->GetOpCode(),szBuffer,(int)dwBytesTransfered);

// 			p_wbuf->len  = dwBytesTransfered;
// 
// 			dwFlags = 0;
// 
// 			//Overlapped send
// 			nBytesSent = WSASend(pClientContext->GetSocket(), p_wbuf, 1, 
// 				&dwBytes, dwFlags, p_ol, NULL);
// 
// 			if ((SOCKET_ERROR == nBytesSent) && (WSA_IO_PENDING != WSAGetLastError()))
// 			{
// 				//WriteToConsole("\nThread %d: Error occurred while executing WSASend().", nThreadNo);
// 
// 				//Let's not work with this client
// 				RemoveFromClientListAndFreeMemory(pClientContext);
// 			}

			break;

		default:
			//We should never be reaching here, under normal circumstances.
			break;
		} // switch
	} // while

}


void CZTcpComm::RejectConn(void *pContext)
{


}

#endif

