#include "stdafx.h"
#include ".\udpcomm.h"

#define SCOK_TIMEOUT    10000  // socket 接收,发送数据超时 10秒

CUdpComm::CUdpComm(void)
{
	m_bStopListen = true;
	m_pRecvList   = NULL;
	m_pThreadRecv = NULL;
}

CUdpComm::~CUdpComm(void)
{
	if (m_pThreadRecv)
	{
		TerminateThread(m_pThreadRecv->m_hThread,0);
		CloseHandle(m_pThreadRecv->m_hThread);
		delete m_pThreadRecv;
		m_pThreadRecv = NULL;
	}
}

UINT CUdpComm::RecvProc(LPVOID param)
{
	CUdpComm *pComm = (CUdpComm *)param;

	while (pComm->m_bStopListen==false)
	{
		char buffer[MAX_RECV_BUFFERS];

		try
		{
			CSockAddr saReceived;
			int nBytesReceived = pComm->m_udpSocket.ReceiveDatagram(buffer,MAX_RECV_BUFFERS-1,saReceived,SCOK_TIMEOUT);
			if(SOCKET_ERROR != nBytesReceived && nBytesReceived>10 && nBytesReceived<=MAX_RECV_BUFFERS)
			{
				buffer[nBytesReceived] = '\0';

				RecvDataStruct recvData;
				memcpy(recvData.data,buffer,MAX_RECV_BUFFERS);
				recvData.data_len = nBytesReceived;
				recvData.src_ip   = ntohl(inet_addr(saReceived.DottedDecimal()));
				recvData.port     = saReceived.Port();

				pComm->m_pRecvList->Lock();
				pComm->m_pRecvList->Push(recvData);
				pComm->m_pRecvList->Unlock();
			}
		}
		catch(CBasicSocketException* e)
		{
			e->GetErrorMessage(buffer,MAX_RECV_BUFFERS);
			IM_LOG(buffer);
			e->Delete();
		}
		catch(...)
		{
			IM_LOG("recve exception");
		}

		Sleep(1);
	}

	AfxEndThread(0);
	return 0;
}

bool CUdpComm::StartListen()
{
	if ( m_bStopListen )
	{
		m_dwLocalIP  = GetLocalIpAddress();

		try
		{
			m_udpSocket.Create(SOCK_DGRAM);
			CSockAddr saUDP;
			m_wPort = XTREAM_LOCAL_PORT;

			saUDP = CSockAddr(INADDR_ANY, (USHORT)m_wPort);
			if(SOCKET_ERROR == m_udpSocket.Bind(saUDP))
			{
				while(1)
				{
					saUDP = CSockAddr(INADDR_ANY, (USHORT)m_wPort);
					if(SOCKET_ERROR != m_udpSocket.Bind(saUDP))
						break;

					if(++m_wPort==(WORD)0xffff)
						return false;
				}
			}

			m_bStopListen = false;

			m_pThreadRecv = AfxBeginThread(RecvProc,this);
		}
		catch(...)
		{
			return false;
		}
	}
	else
	{
		return false;
	}

	return true;
}

bool CUdpComm::StopListen()
{
	if ( m_bStopListen==false )
	{
		m_bStopListen=true;

		if (m_pThreadRecv)
		{
			try
			{
				TerminateThread(m_pThreadRecv->m_hThread,0);
				CloseHandle(m_pThreadRecv->m_hThread);
				delete m_pThreadRecv;
				m_pThreadRecv = NULL;
			}
			catch(...)
			{
			}

		}

		m_pThreadRecv = NULL;

		m_udpSocket.Close();
	}

	return true;
}

void CUdpComm::SetRecvList(FIFO2<RecvDataStruct> *pList)
{
	m_pRecvList = pList;
}

int CUdpComm::SendUdpData(const SendDataStruct &sendData)
{
	if ( m_bStopListen==false && sendData.dest_ip!=0 )
	{
		CSockAddr saDest;
		saDest = CSockAddr(sendData.dest_ip, (USHORT)sendData.port);
		int nTimeOutMiliSecs=120; //xuexiaofeng : 因为不晓得udp 中使用select函数的意义,所以暂时设定upd发送超时2分钟

		int nBytesSend = 0;

		nBytesSend=m_udpSocket.SendDatagram((const char*)sendData.data,sendData.data_len,saDest,nTimeOutMiliSecs*1000);

		if ( nBytesSend!=sendData.data_len )
		{
		//	DWORD dwError = GetLastError();
		//	PostMessage(g_hMainWnd,WM_NETWORK_ERROR,dwError,0);		
		}
		
		return nBytesSend;
	}

	return 0;
}
