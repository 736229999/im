#include "StdAfx.h"
#include "xudsokect.h"

//#define Mytrace

#ifdef Mytrace
#include <iostream>
using namespace std;
#endif

XUDSocket::XUDSocket(XSOCKET xs,int nWndSize,int nSendBufSize):m_sendWnd(nWndSize,nSendBufSize),m_recvWnd(nWndSize)
{
	m_state = INVALID;
	m_socket = NULL;
	m_bQuickStart = TRUE;
	m_nDestId = 0;
	m_xsocket = xs;
	m_SPT = 10000;	// ΢��
	m_RPT = 0;
	m_fChannelCap = 1;

	m_nLastTick = 0;
	m_nControlTick = 0;
	m_nResendTick = 0;

	m_bTranServer = false;

	m_nTickLastSlow = 0;
	m_nTickLastAck = 0;
	m_bSend = TRUE;
	m_hSend = CreateThread(NULL,NULL,ThreadSendData,this,0,0);
	m_nLastAckTick = 0;
	//m_nRecvPos = 64;

#ifdef _MyDebug
	m_nWait = 0;
	m_nSendCount = 0;
	/*m_nAI = 1024*100;
	m_pAIInfo = new UINT[m_nAI];
	memset(m_pAIInfo,0,sizeof(UINT)*m_nAI);
	m_nAISeq = 0;*/
#endif

}

XUDSocket::~XUDSocket(void)
{
	m_bSend = false;
	WaitForSingleObject(m_hSend,100);
}

DWORD WINAPI XUDSocket::ThreadSendData(LPVOID lpParam)
{
	if(lpParam == NULL)
		return 1;
	XUDSocket * pS = (XUDSocket *)lpParam;
	pS->SendData();
	return 0;
};

void XUDSocket::SendData()
{
	int nSeq = -1;
	int nSleep = 0;
	while(m_bSend)
	{
		// �����ж��Ƿ����ش�����
	
		if(HasResend())
		{
			m_scResendList.Lock();
			nSeq = *m_lstResend.begin();
			m_lstResend.pop_front();
			m_scResendList.Unlock();
			if(m_sendWnd.ReSend(nSeq,this))	
			{
				nSleep = m_SPT / 1000;
				if(nSleep <= 0)
					nSleep = 1;
				if(m_SPT < 100)
					nSleep = 0;
				Sleep(nSleep);
			}
			continue;
		}
		
		// ��ʼ���ͻ����е�����

		if(m_sendWnd.HasBufData())
		{

			if(m_sendWnd.SendBuf(this))
			{
				nSleep = m_SPT / 1000;
				if(nSleep <= 0)
					nSleep = 1;
				if(m_SPT < 100)
					nSleep = 0;
				Sleep(nSleep);
			}
			else
				m_sendWnd.WaitAck(5);
		}
		else
		{
			Sleep(5);
		}
	}
};



//------------------------------------------------------
// ����ʽ
//------------------------------------------------------

int XUDSocket::Send(const char * pData,int nLen)
{
	if(m_state != CONNECTED)
		return -1;

	int nLeft = nLen;	// ʣ�µ�������
	int nDataLen = 0;	// ÿ�η��͵�������
	int nSending = 0;

	int nPush = 0;
	while(nLeft > 0 && m_state == CONNECTED)
	{

		nPush = m_sendWnd.PushData((const BYTE *)pData,nLeft);
		if(nPush > 0)
		{
			nSending += nPush;
			nLeft -= nPush;
			pData += nPush;
		}
		else if(nPush == 0)
		{
			Sleep(5);
		}
		else
		{
			break;
		}

	}

	return nSending;
}

bool XUDSocket::SendWait()
{
	while (m_state == CONNECTED)
	{
		if(m_sendWnd.IsEmpty() == false || m_sendWnd.HasBufData())
		{
			m_sendWnd.WaitAck(5);
		}
		else
		{
			TRACE("Send Wait OK \n");
			return true;
		}
	}
	return false;
}

//------------------------------------------------------
// ����ʽ
//------------------------------------------------------

int XUDSocket::Recv(char * pBuf,int nLen)
{
	int nRecv = -1;
	while(m_recvWnd.GetRecvLen() > 0 || m_state == CONNECTED)
	{
		nRecv = m_recvWnd.ReadData((BYTE*)pBuf,nLen);
		if(nRecv == 0)
			Sleep(2);
		else
			break;
	}
	if(nRecv == -1 && m_state == CLOSED)
	{
		return 0;
	}
	return nRecv;
}

void XUDSocket::ProcessData(const XUDS_PACKET & packet)
{
	if(m_state != CONNECTED)
		return;
	/*ULONGLONG nInter = 0;
	if(m_nLastTick != 0)
	{
		nInter = packet.tickCount - m_nLastTick;
		m_nLastTick = packet.tickCount;
		m_RPT += (nInter - m_RPT) >> 3;
	}
	else
	{
		m_RPT = nInter;
		m_nLastTick = packet.tickCount;
	}*/
	// ��tickCount���ݼ�����������������������
	
	m_RecvCapCalc.AddRecvTick(packet.tickCount);
	XUDS_HEADER * pData = (XUDS_HEADER*)packet.data;
	int nSeq = pData->seq;

#ifdef _MyDebug
	//printf("recv %d\n",nSeq);
#endif
	// �ڽ��մ�����ȷ�Ͻ��յ�����
	
	XUDS_DATA_ACK::RESULT rst = m_recvWnd.RecvPacket(nSeq,pData->data,pData->len);

	// ����Ӧ��
	SendAck(nSeq,packet.len,rst);
}

void XUDSocket::SendPacket(XUDS_DATA * pData)
{
	m_nLastSendTick = getTickCount();
	sendto(m_socket,(char*)pData,sizeof(XUDS_HEAD) + pData->len,0,(sockaddr*)&m_addrPeer,sizeof(SOCKADDR_IN));	
}

void XUDSocket::SendAck(uint32 seq,uint16 nLen,XUDS_DATA_ACK::RESULT rst)
{
	XUDS_DATA_ACK ack;
	ack.did = m_nDestId;
	ack.socketId = m_xsocket;
	ack.seq = seq;
	ack.rt = rst;
	ack.ai = m_RecvCapCalc.GetAI();
	sendto(m_socket,(char*)&ack,sizeof(ack),0,(sockaddr*)&m_addrPeer,sizeof(SOCKADDR_IN));
}

void XUDSocket::ProcessDataAck(const XUDS_PACKET & packet)
{
	m_nLastAckTick = getTickCount();
	if(m_state != CONNECTED)
		return;
	XUDS_DATA_ACK * pAck = (XUDS_DATA_ACK*)packet.data;
	if(pAck->rt == XUDS_DATA_ACK::RECV_BUF_FULL ||
		pAck->rt == XUDS_DATA_ACK::NOT_IN_WND)
		return;
	XUDSSendWnd::ACK_STATE s = m_sendWnd.ConfirmAck(this,pAck->seq,packet.tickCount);
	if(pAck->ai != 0)
	{

	}

	m_SPT = m_sendWnd.GetRTTC() / 3;

#ifdef _MyDebug
	
	m_sendWnd.m_pAIInfo[m_sendWnd.m_nAISeq] = pAck->ai;
	m_sendWnd.m_nAISeq++;
#endif
	if(s == XUDSSendWnd::ACK_OK)
	{
		if(m_bQuickStart)
		{
			m_fChannelCap += 1 ;
		}
		else
		{
			m_fChannelCap += 1.0 / m_fChannelCap;
		}
	}
	else if(s == XUDSSendWnd::ACK_QUICK_RESEND)
	{
		// ������ӵ��
		if(m_bQuickStart)
		{
			 m_fChannelCap *= 0.5;
			 if(m_fChannelCap < 1)
				 m_fChannelCap = 1;
			m_bQuickStart = false;
#ifdef _MyDebug
			printf("quick start stop f = %f\n",m_fChannelCap);
#endif
		}
		else
		{
			SlowDown();
		}
	}
}

void XUDSocket::Init(SOCKET s,SOCKADDR_IN addrPeer,uint32 nDestId)
{
	m_socket = s;
	m_addrPeer = addrPeer;
	m_nDestId = nDestId;
}


void XUDSocket::OnTimer()
{
	if(m_state != CONNECTED)
		return;
	OnTimerResend();
	OnTimerControl();
}

void XUDSocket::OnTimerControl()
{
	ULONGLONG curTime = getTickCount();
	ULONGLONG nDelta = curTime - m_nControlTick ;
	if(nDelta < 10*1000)	// 10ms
		return;
	if(m_nLastAckTick == 0)
		return;
	if(curTime < m_nLastAckTick)
	{
		assert(0 && "��ǰʱ��С���ϴ�ackʱ�䣡��");
		return;
	}
	ULONGLONG nTimeSpan = curTime - m_nLastAckTick;
	// ����2���ش�����û���յ�Ack?
	if(nTimeSpan > 2 * m_sendWnd.GetRTO())
	{
		// �Ƿ�����?
		if(m_nLastSendTick > m_nLastAckTick)
		{
			m_fChannelCap *= 0.5;
			if(m_fChannelCap < 1)
				m_fChannelCap = 1;
			m_nTickLastSlow = curTime;	
			m_SPT *= 1.2;
			if(m_SPT > 2000000)
			{
				m_SPT = 2000000;
			}
			TRACE("����..f = %f ,spt = %d\n",m_fChannelCap,m_SPT);
#ifdef _MyDebug
			printf("����..f = %f\n",m_fChannelCap);
#endif
		}
		// 2����δ��Ӧ����Ͽ�����
		if(nTimeSpan > 1000 * 1000 *120 )
		{
			m_state = CLOSED;
		}
	}
	
#ifdef _MyDebug
	//cout<<m_fChannelCap<<endl;
	//cout<<"SPT="<<m_SPT/1000.0<<endl;
	//cout<<"rtt="<<m_sendWnd.GetRTT()<<endl;
	//cout<<"rto="<<m_sendWnd.GetRTO()<<endl;
#endif
	


}

void XUDSocket::OnTimerResend()
{
	ULONGLONG curTime = getTickCount();
	if(m_nResendTick - curTime < m_sendWnd.GetRTO())
		return;
	m_nResendTick = curTime;
	if(m_sendWnd.CheckResend(this) == true)
	{
		if(m_bQuickStart)
		{
			m_fChannelCap *= 0.5;
			m_bQuickStart = false;
#ifdef _MyDebug
			printf("���������� f = %f\n",m_fChannelCap);
#endif
		}
		else
		{
			SlowDown();
		}
	}
}

void XUDSocket::Close()
{
	if(m_state == CLOSED)
		return;
	m_state = CLOSED;
	XUDS_CLOSE close;
	close.socketId = m_xsocket;
	close.did = m_nDestId;
	for(int i = 0;i < 3;i++)
	{
		sendto(m_socket,(char*)&close,sizeof(close),0,(sockaddr*)&m_addrPeer,sizeof(m_addrPeer));
		Sleep(30);
	}
}

int XUDSocket::GetSuccSend()
{
	return m_sendWnd.GetSuccSend();
}

void XUDSocket::SlowDown()
{
	ULONGLONG nTick = getTickCount();
	if(nTick - m_nTickLastSlow > m_sendWnd.GetRTO())
	{
		m_fChannelCap -= 1;
		if(m_fChannelCap < 1)
			m_fChannelCap = 1;
		m_nTickLastSlow = nTick;	
	}
	
}
