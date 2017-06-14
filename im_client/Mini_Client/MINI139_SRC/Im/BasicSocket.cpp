//
// Copyright 2005 HMG
// All Rights Reserved. 
// 
// @Author: Linda
// @Email: 
// @Date:  2005-06-10	
//
//////////////////////////////////////////////////////////////////////
#include <stdafx.h>
#include "basicsocket.h"
#include "..\utils\Function.h"
// Class CBasicSocketException
IMPLEMENT_DYNAMIC(CBasicSocketException, CException)

CBasicSocketException::CBasicSocketException(char* pchMessage)
{
	m_strMessage = pchMessage;
	m_nError = WSAGetLastError();
}

BOOL CBasicSocketException::GetErrorMessage(LPTSTR lpstrError, UINT nMaxError,
		PUINT pnHelpContext /*= NULL*/)
{

	char text[200];
	if(m_nError == 0) {
		wsprintf(text, "%s error", (const char*) m_strMessage);
	}
	else {
		wsprintf(text, "%s error #%d", (const char*) m_strMessage, m_nError);
	}
	strncpy(lpstrError, text, nMaxError - 1);
	return TRUE;
}

// Class CBasicSocket
IMPLEMENT_DYNAMIC(CBasicSocket, CObject)

void CBasicSocket::Cleanup()
{
	// doesn't throw an exception because it's called in a catch block
	if(m_hSocket == NULL) return;
	VERIFY(closesocket(m_hSocket) != SOCKET_ERROR);
	m_hSocket = NULL;
}

void CBasicSocket::Create(int nType /* = SOCK_STREAM */)
{
	ASSERT(m_hSocket == NULL);
	if((m_hSocket = socket(AF_INET, nType, 0)) == INVALID_SOCKET) {
		throw new CBasicSocketException("Create");
	}
/*
	int nrcvbuf=1024*100; 
int err=setsockopt(m_hSocket,
	  SOL_SOCKET, 
	  SO_RCVBUF,//Ð´»º³å£¬¶Á»º³åÎªSO_RCVBUF 
	  (char *)&nrcvbuf, 
	  sizeof(nrcvbuf)); 

		int n=255;
	int nOld;
	char out[255];
	int erro=getsockopt(m_hSocket,SOL_SOCKET,SO_RCVBUF,
		(char *)out,
		&n);*/
}

//modify by Andy 2004.1.4 13:41
//return value: SOCKET
SOCKET CBasicSocket::Bind(LPCSOCKADDR psa)
{
	ASSERT(m_hSocket != NULL);
	if(::bind(m_hSocket, psa, sizeof(SOCKADDR)) == SOCKET_ERROR) {
//		throw new CBasicSocketException("Bind");
		return SOCKET_ERROR;
	}

	//if (err!= NO_ERROR || n!=nrcvbuf) 
	//{TRACE("setsockopt Error!\n");} 

	return m_hSocket;
}

void CBasicSocket::Listen()
{
	ASSERT(m_hSocket != NULL);
	if(listen(m_hSocket, 5) == SOCKET_ERROR) {
		throw new CBasicSocketException("Listen");
	}
}

BOOL CBasicSocket::Accept(CBasicSocket& sConnect, LPSOCKADDR psa)
{
	ASSERT(m_hSocket != NULL);
	ASSERT(sConnect.m_hSocket == NULL);
	int nLengthAddr = sizeof(SOCKADDR);
	sConnect.m_hSocket = accept(m_hSocket, psa, &nLengthAddr);
	if(sConnect == INVALID_SOCKET) {
		// no exception if the listen was canceled
		if(WSAGetLastError() != WSAEINTR) {
			throw new CBasicSocketException("Accept");
		}
		return FALSE;
	}
	return TRUE;
}

void CBasicSocket::Close()
{
	ASSERT(m_hSocket != NULL);
	if(closesocket(m_hSocket) == SOCKET_ERROR) {
		// should be OK to close if closed already
		throw new CBasicSocketException("Close");
	}
	m_hSocket = NULL;
}

void CBasicSocket::Connect(LPCSOCKADDR psa)
{
	ASSERT(m_hSocket != NULL);
	// should timeout by itself
	if(connect(m_hSocket, psa, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		throw new CBasicSocketException("Connect");
	}
}

int CBasicSocket::Write(const char* pch, const int nSize, const int nSecs)
{
	int nBytesSent = 0;
	int nBytesThisTime;
	const char* pch1 = pch;
	do {
		nBytesThisTime = Send(pch1, nSize - nBytesSent, nSecs);
		nBytesSent += nBytesThisTime;
		pch1 += nBytesThisTime;
	} while(nBytesSent < nSize);
	return nBytesSent;
}

int CBasicSocket::Send(const char* pch, const int nSize, const int nSecs)
{
	ASSERT(m_hSocket != NULL);
	// returned value will be less than nSize if client cancels the reading
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(select(0, NULL, &fd, NULL, &tv) == 0) {
		throw new CBasicSocketException("Send timeout");
	}
	int nBytesSent;
	if((nBytesSent = send(m_hSocket, pch, nSize, 0)) == SOCKET_ERROR) {
		throw new CBasicSocketException("Send");
	}
	return nBytesSent;
}

int CBasicSocket::Receive(char* pch, const int nSize, const int nSecs)
{
	ASSERT(m_hSocket != NULL);
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(select(0, &fd, NULL, NULL, &tv) == 0) {
		throw new CBasicSocketException("Receive timeout");
	}

	int nBytesReceived;
	if((nBytesReceived = recv(m_hSocket, pch, nSize, 0)) == SOCKET_ERROR) {
		throw new CBasicSocketException("Receive");
	}
	return nBytesReceived;
}

int CBasicSocket::ReceiveM(char* pch, const int nSize, const int nSecs,SOCKET s1,SOCKET s2)
{
		int nBytesReceived;
	ASSERT(m_hSocket != NULL);
	FD_SET fd = {2, s1, s2};
	TIMEVAL tv = {nSecs, 0};
	int ti = select(0, &fd, NULL, NULL, &tv);
	if(ti == 0) {
		throw new CBasicSocketException("Receive timeout");
	}
	TRACE("SELECT return %d\n",ti);
	FD_SET fd1 = {1, s1};
	TIMEVAL tv1 = {0, 0};
	ti = select(0, &fd1, NULL, NULL, &tv1);
	if(ti == 0) {
		TRACE("Receive timeout1\n");
	}
	else
	{
		TRACE("Receive 1\n");
		if((nBytesReceived = recv(s1, pch, nSize, 0)) == SOCKET_ERROR) {
			throw new CBasicSocketException("Receive");
		}
	}

	FD_SET fd2 = {1, s2};
	TIMEVAL tv2 = {0, 0};
	ti = select(0, &fd2, NULL, NULL, &tv2);
	if(ti == 0) {
		TRACE("Receive timeout2\n");
	}
	else
	{
		TRACE("Receive 2\n");
		int nBytesReceived;
		if((nBytesReceived = recv(s2, pch, nSize, 0)) == SOCKET_ERROR) {
			throw new CBasicSocketException("Receive");
		}
	}

		return nBytesReceived;
}

int CBasicSocket::ReceiveDatagram(char* pch, const int nSize, LPSOCKADDR psa, const int nMicroSecs)
{
	//if(m_hSocket==NULL)
	//{
	//	throw new CBasicSocketException("Socket NULL");
	//	return 0;
	//}
	//FD_SET fd = {1, m_hSocket};
	//TIMEVAL tv = {0, nMicroSecs};
	//int ti = select(0, &fd, NULL, NULL, &tv);
	//if( ti == 0) {
//		throw new CBasicSocketException("Receive timeout");
//		return 0;
//	}
	// input buffer should be big enough for the entire datagram

	//int nFromSize = sizeof(SOCKADDR);
	//int nBytesReceived = recvfrom(m_hSocket, pch, nSize, 0, psa, &nFromSize);
	//if(nBytesReceived == SOCKET_ERROR) {
	//	throw new CBasicSocketException("ReceiveDatagram");
	//}
	//return nBytesReceived;

	int nFromSize = sizeof(SOCKADDR);
	return recvfrom(m_hSocket, pch, nSize, 0, psa, &nFromSize);
}

int CBasicSocket::SendDatagram(const char* pch, const int nSize, LPCSOCKADDR psa, const int nMicroSecs)
{
//	ASSERT(m_hSocket != NULL);
	//FD_SET fd = {1, m_hSocket};
	//TIMEVAL tv = {0, nMicroSecs};
	//if(select(0, NULL, &fd, NULL, &tv) == 0) {
	//	throw new CBasicSocketException("Send timeout");
	//}

	//int nBytesSent = sendto(m_hSocket, pch, nSize, 0, psa, sizeof(SOCKADDR));
	//if(nBytesSent == SOCKET_ERROR) {
	//	throw new CBasicSocketException("SendDatagram");
	//}

	//if ( nBytesSent==0 && nSize!=0 )
	//{
	//	throw new CBasicSocketException("SendDatagram error, no data sended!");
	//}

	//return nBytesSent;
	return sendto(m_hSocket, pch, nSize, 0, psa, sizeof(SOCKADDR));
}

void CBasicSocket::GetPeerAddr(LPSOCKADDR psa)
{
	ASSERT(m_hSocket != NULL);
	// gets the address of the socket at the other end
	int nLengthAddr = sizeof(SOCKADDR);
	if(getpeername(m_hSocket, psa, &nLengthAddr) == SOCKET_ERROR) {
		throw new CBasicSocketException("GetPeerName");
	}
}

void CBasicSocket::GetSockAddr(LPSOCKADDR psa)
{
	ASSERT(m_hSocket != NULL);
	// gets the address of the socket at this end
	int nLengthAddr = sizeof(SOCKADDR);
	if(getsockname(m_hSocket, psa, &nLengthAddr) == SOCKET_ERROR) {
		throw new CBasicSocketException("GetSockName");
	}
}

//static
CSockAddr CBasicSocket::GetHostByName(const char* pchName, const USHORT ushPort /* = 0 */)
{
	hostent* pHostEnt = gethostbyname(pchName);
	if(pHostEnt == NULL) {
		throw new CBasicSocketException("GetHostByName");
	}
	ULONG* pulAddr = (ULONG*) pHostEnt->h_addr_list[0];
	SOCKADDR_IN sockTemp;
	sockTemp.sin_family = AF_INET;
	sockTemp.sin_port = htons(ushPort);
	sockTemp.sin_addr.s_addr = *pulAddr; // address is already in network byte order
	return sockTemp;
}

//static
const char* CBasicSocket::GetHostByAddr(LPCSOCKADDR psa)
{
	hostent* pHostEnt = gethostbyaddr((char*) &((LPSOCKADDR_IN) psa)
				->sin_addr.s_addr, 4, PF_INET);
	if(pHostEnt == NULL) {
		throw new CBasicSocketException("GetHostByAddr");
	}
	return pHostEnt->h_name; // caller shouldn't delete this memory
}

