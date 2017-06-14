#pragma once
#ifndef _CLIENTCONTEXT_H_
#define _CLIENTCONTEXT_H_

#include <winsock2.h>

#define MAX_BUFFER_LEN 1024

class CClientContext  //To store and manage client related information
{
private:

	OVERLAPPED        *m_pol;
	WSABUF            *m_pwbuf;

	int               m_nTotalBytes;
	int               m_nSentBytes;

	SOCKET            m_Socket;  //accepted socket
	int               m_nOpCode; //will be used by the worker thread to decide what operation to perform
	char              m_szBuffer[MAX_BUFFER_LEN];
	char			  m_szIp[16];

public:
	void SetIP(char* pIP);


	char* GetIP();

	void AddSendData(char *szBubber);


	//Get/Set calls
	void SetOpCode(int n);


	int GetOpCode();


	void SetTotalBytes(int n);

	int GetTotalBytes();


	void SetSentBytes(int n);


	void IncrSentBytes(int n);

	int GetSentBytes();
	
	void SetSocket(SOCKET s);

	SOCKET GetSocket();

	void SetBuffer(char *szBuffer);

	void GetBuffer(char *szBuffer);

	void ZeroBuffer();

	void SetWSABUFLength(int nLength);

	int GetWSABUFLength();

	WSABUF* GetWSABUFPtr();

	OVERLAPPED* GetOVERLAPPEDPtr();

	void ResetWSABUF();

	//Constructor
	CClientContext();

	//destructor
	~CClientContext();
};

#endif

