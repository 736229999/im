/******************************************************************
** 文件名:	Channel.cpp
** 版  权:	
** 创建人:	Tony
** 日  期:	2007-6-4
** 描  述:  这个类主要用UDT的设计思想，面向连接的逻辑通道类

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
*******************************************************************/

#ifdef WIN32
#define socklen_t int
#endif

#define NET_ERROR WSAGetLastError()

#include "stdafx.h"
//#include "fcl.h"
#include "Channel.h"
#include "Ws2tcpip.h"
#include "p2pDef.h"
//#include "NetException.h"
//using namespace fcl;

CChannel::CChannel():
m_iIPversion(AF_INET),
m_iSndBufSize(65536),
m_iRcvBufSize(65536)
{
}

CChannel::CChannel(int version):
m_iIPversion(version),
m_iSndBufSize(65536),
m_iRcvBufSize(65536)
{
}

CChannel::~CChannel()
{
}

int CChannel::open(const sockaddr_in* addr)
{
	// construct an socket
	m_iSocket = socket(AF_INET, SOCK_DGRAM,0);
	if (m_iSocket < 0)
	{
		//Error("CChannel::open,socket error!\n");
		return -1;
	}

	if (NULL != addr)
	{
		int32 namelen = sizeof(sockaddr_in);

		if (0 != bind(m_iSocket, (struct sockaddr*)addr, namelen))
		{
			//Error("socket bind error!\n");
			return -1;
		}
	}
	else
	{
		//sendto or WSASendTo will also automatically bind the socket
		addrinfo hints;
		addrinfo* res;

		memset(&hints, 0, sizeof(struct addrinfo));

		hints.ai_flags = AI_PASSIVE;
		hints.ai_family = m_iIPversion;
		hints.ai_socktype = SOCK_DGRAM;

		if (0 != getaddrinfo(NULL, "0", &hints, &res))
		{
			//Error("Get host addressinfo error!\n");
			return -1;
			//throw CNetException(1, 3, NET_ERROR);
		}

		if (0 != bind(m_iSocket, res->ai_addr, (int)res->ai_addrlen))
		{
			//Error("bind address error!\n");
			//throw CNetException(1, 3, NET_ERROR);
		}

		freeaddrinfo(res);
	}

	if ((0 != setsockopt(m_iSocket, SOL_SOCKET, SO_RCVBUF, (char *)&m_iRcvBufSize, sizeof(int))) ||
		(0 != setsockopt(m_iSocket, SOL_SOCKET, SO_SNDBUF, (char *)&m_iSndBufSize, sizeof(int))))
	{
		//throw CNetException(1, 3, NET_ERROR);
		//Error("设置socket系统收发缓冲区大小失败\n");
		return -1;
	}

	return 0;
}

void CChannel::close()
{
	closesocket(m_iSocket);
	m_iSocket = NULL;
}

int CChannel::getSndBufSize()
{
	socklen_t size = sizeof(socklen_t);

	getsockopt(m_iSocket, SOL_SOCKET, SO_SNDBUF, (char *)&m_iSndBufSize, &size);

	return m_iSndBufSize;
}

int CChannel::getRcvBufSize()
{
	socklen_t size = sizeof(socklen_t);

	getsockopt(m_iSocket, SOL_SOCKET, SO_RCVBUF, (char *)&m_iRcvBufSize, &size);

	return m_iRcvBufSize;
}

void CChannel::setSndBufSize(const int& size)
{
	m_iSndBufSize = size;
}

void CChannel::setRcvBufSize(const int& size)
{
	m_iRcvBufSize = size;
}

void CChannel::getSockAddr(sockaddr_in* addr) 
{
	socklen_t namelen =  sizeof(sockaddr_in);

	getsockname(m_iSocket, (sockaddr*)&addr, &namelen);
}

void CChannel::getPeerAddr(sockaddr_in* addr) 
{
	socklen_t namelen = sizeof(sockaddr_in);

	getpeername(m_iSocket, (sockaddr*)&addr, &namelen);
}

int CChannel::sendto(const sockaddr_in* addr,  char* buf, int32 len)
{
	//PROFILE("CChannel::sendto");//性能评测

	WSABUF wsaBuf;
	wsaBuf.buf = buf;
	wsaBuf.len = len;

	// 限制buf的长度，不要太长，以免程序消耗很高的CPU

	DWORD size = 0;
	int addrsize = sizeof(sockaddr_in);
	int res = WSASendTo(m_iSocket, &wsaBuf, 1, &size, 0, (const sockaddr*)addr, addrsize, NULL, NULL);

	return res==0?size:res;
}

int CChannel::recvfrom(sockaddr_in* addr, char* buf, int32 len)
{
    WSABUF wsaBuf;
	DWORD size = 0;
	DWORD flag = 0;
	wsaBuf.buf = buf;
	wsaBuf.len = len;
	int addrsize = sizeof(sockaddr_in);
	
	int res = WSARecvFrom(m_iSocket, &wsaBuf, 1, &size, &flag, (sockaddr*)addr, &addrsize, NULL, NULL);

	return res==0?size:res;
}

SOCKET CChannel::getSocket()
{
	return m_iSocket;
}
