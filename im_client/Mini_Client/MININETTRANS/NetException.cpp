/******************************************************************
** 文件名:	NetException.cpp
** 版  权:	
** 创建人:	Tony
** 日  期:	2007-6-4
** 描  述:  这个类主要用UDT的设计思想，代码修改过来，UDT面向连接的逻辑通道类

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
*******************************************************************/

#include "stdafx.h"
#include "NetException.h"


CNetException::CNetException(int major, int minor, int err):
m_iMajor(major),
m_iMinor(minor)
{
	if (-1 == err)
		m_iErrno = GetLastError();
	else
		m_iErrno = err;
}

CNetException::CNetException(const CNetException& e):
m_iMajor(e.m_iMajor),
m_iMinor(e.m_iMinor),
m_iErrno(e.m_iErrno)
{
}

CNetException::~CNetException()
{
}

const char* CNetException::getErrorMessage()
{
	// translate "Major:Minor" code into text message.

	switch (m_iMajor)
	{
	case 0:
		strcpy(m_pcMsg, "Success");
		break;

	case 1:
		strcpy(m_pcMsg, "Connection setup failure");

		switch (m_iMinor)
		{
		case 1:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "connection time out");

			break;

		case 2:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "connection rejected");

			break;

		case 3:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "unable to create/configure UDP socket");

			break;

		case 4:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "abort for security reasons");

			break;

		default:
			break;
		}

		break;

	case 2:
		switch (m_iMinor)
		{
		case 1:
			strcpy(m_pcMsg, "Connection was broken");

			break;

		case 2:
			strcpy(m_pcMsg, "Connection does not exist");

			break;

		default:
			break;
		}

		break;

	case 3:
		strcpy(m_pcMsg, "System resource failure");

		switch (m_iMinor)
		{
		case 1:
			strcpy(m_pcMsg, "unable to create new threads");

			break;

		case 2:
			strcpy(m_pcMsg, "unable to allocate buffers");

			break;

		default:
			break;
		}

		break;

	case 4:
		strcpy(m_pcMsg, "File system failure");

		switch (m_iMinor)
		{
		case 1:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "cannot seek read position");

			break;

		case 2:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "failure in read");

			break;

		case 3:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "cannot seek write position");

			break;

		case 4:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "failure in write");

			break;

		default:
			break;
		}

		break;

	case 5:
		strcpy(m_pcMsg, "Operation not supported");

		switch (m_iMinor)
		{
		case 1:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "Cannot do this operation on a BOUND socket");

			break;

		case 2:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "Cannot do this operation on a CONNECTED socket");

			break;

		case 3:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "Bad parameters");

			break;

		case 4:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "Invalid socket ID");

			break;

		case 5:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "Cannot do this operation on an UNBOUND socket");

			break;

		case 6:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "Socket is not in listening state");

			break;

		case 7:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "Listen/accept is not supported in rendezous connection setup");

			break;

		case 8:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "Cannot call connect on UNBOUND socket in rendezvous connection setup");

			break;

		case 9:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "This operation is not supported in SOCK_STREAM mode");

			break;

		case 10:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "This operation is not supported in SOCK_DGRAM mode");

			break;

		default:
			break;
		}

		break;

	case 6:
		strcpy(m_pcMsg, "Non-blocking call failure");

		switch (m_iMinor)
		{
		case 1:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "no buffer available for sending");

			break;

		case 2:
			strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
			strcpy(m_pcMsg + strlen(m_pcMsg), "no data available for reading");

			break;

		default:
			break;
		}

		break;

	default:
		strcpy(m_pcMsg, "Unknown error");
	}

	// Adding "errno" information
	if (0 < m_iErrno)
	{
		strcpy(m_pcMsg + strlen(m_pcMsg), ": ");
		LPVOID lpMsgBuf;
		FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, NULL, m_iErrno, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
		strncpy(m_pcMsg + strlen(m_pcMsg), (char*)lpMsgBuf, 1024 - strlen(m_pcMsg) - 2);
		LocalFree(lpMsgBuf);
	}


	return m_pcMsg;
}

const int CNetException::getErrorCode() const
{
	return m_iMajor * 1000 + m_iMinor;
}

