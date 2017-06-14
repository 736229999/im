
#include "stdafx.h"
#include "ClientContext.h"


void CClientContext::SetIP(char* pIP)
{
	strcpy(m_szIp,pIP);
}

char* CClientContext::GetIP()
{
	return m_szIp;
}


//Get/Set calls
void CClientContext::SetOpCode(int n)
{
	m_nOpCode = n;
}

int CClientContext::GetOpCode()
{
	return m_nOpCode;
}

void CClientContext::SetTotalBytes(int n)
{
	m_nTotalBytes = n;
}

int CClientContext::GetTotalBytes()
{
	return m_nTotalBytes;
}

void CClientContext::SetSentBytes(int n)
{
	m_nSentBytes = n;
}

void CClientContext::IncrSentBytes(int n)
{
	m_nSentBytes += n;
}

int CClientContext::GetSentBytes()
{
	return m_nSentBytes;
}

void CClientContext::SetSocket(SOCKET s)
{
	m_Socket = s;
}

SOCKET CClientContext::GetSocket()
{
	return m_Socket;
}

void CClientContext::AddSendData(char *szBubber)
{
	if(sizeof(m_szBuffer))
	{
		strcpy(m_szBuffer,szBubber);
	}
	else
	{
		memcpy(m_szBuffer+sizeof(m_szBuffer),szBubber,sizeof(szBubber));
	}	
}

void CClientContext::SetBuffer(char *szBuffer)
{
	strcpy(m_szBuffer, szBuffer);
}

void CClientContext::GetBuffer(char *szBuffer)
{
	strcpy(szBuffer, m_szBuffer);
}

void CClientContext::ZeroBuffer()
{
	ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);
}

void CClientContext::SetWSABUFLength(int nLength)
{
	m_pwbuf->len = nLength;
}

int CClientContext::GetWSABUFLength()
{
	return m_pwbuf->len;
}

WSABUF* CClientContext::GetWSABUFPtr()
{
	return m_pwbuf;
}

OVERLAPPED* CClientContext::GetOVERLAPPEDPtr()
{
	return m_pol;
}

void CClientContext::ResetWSABUF()
{
	ZeroBuffer();
	m_pwbuf->buf = m_szBuffer;
	m_pwbuf->len = MAX_BUFFER_LEN;
}

//Constructor
CClientContext::CClientContext()
{
	m_pol = new OVERLAPPED;
	m_pwbuf = new WSABUF;

	ZeroMemory(m_pol, sizeof(OVERLAPPED));

	m_Socket =  SOCKET_ERROR;

	ZeroMemory(m_szBuffer, MAX_BUFFER_LEN);

	m_pwbuf->buf = m_szBuffer;
	m_pwbuf->len = MAX_BUFFER_LEN;

	m_nOpCode = 0;
	m_nTotalBytes = 0;
	m_nSentBytes = 0;
}

//destructor
CClientContext::~CClientContext()
{
	//Wait for the pending operations to complete
	while (!HasOverlappedIoCompleted(m_pol)) 
	{
		Sleep(0);
	}

	closesocket(m_Socket);

	//Cleanup
	delete m_pol;
	delete m_pwbuf;
}

