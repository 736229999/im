#include "StdAfx.h"
#include "xudsrecvwnd.h"

XUDSRecvWnd::XUDSRecvWnd(int nRecvWndSize)
{
	m_recvData.SetStackLen(RECV_BUF_LEN);
	m_pRecvPool = new XUDS_RECV_SEG[nRecvWndSize];
	m_nBeg = 0;
	m_nSegCount = nRecvWndSize;

#ifdef _MyDebug
	m_nRecvCount = 0;
	m_nRepeatCount = 0;
#endif
}

XUDSRecvWnd::~XUDSRecvWnd(void)
{
	if(m_pRecvPool)
	{
		delete [] m_pRecvPool;
	}
}

UINT XUDSRecvWnd::GetRecvLen()
{
	return m_recvData.GetDataSize();
}

UINT XUDSRecvWnd::ReadData(BYTE * pData,UINT nLen)
{
	UINT nRead = 0;
	nRead = m_recvData.Pop((char*)pData,nLen);
	return nRead;
}

int XUDSRecvWnd::GetNotifyWnd()
{
	return m_nBeg + m_nSegCount;
}

XUDS_DATA_ACK::RESULT XUDSRecvWnd::RecvPacket(UINT seq,BYTE * pData,UINT nLen)
{
	//return XUDS_DATA_ACK::RECV_BUF_FULL;
	
#ifdef _MyDebug
	m_nRecvCount++;
#endif

	if(seq > m_nBeg + m_nSegCount)
	{
#ifdef _MyDebug
		m_nRepeatCount++;
#endif
		TRACE("recv %d,���յ������ݰ��������մ��ڷ�Χ(%d,%d)\n",seq,m_nBeg,m_nBeg + m_nSegCount);
		return XUDS_DATA_ACK::NOT_IN_WND;
	}
	else if(seq < m_nBeg)
	{
		 TRACE("recv %d,�յ��ظ����ݰ�\n",seq);
		 return XUDS_DATA_ACK::REPEAT;
	}
	else
	{
		TRACE("recv %d,beg = %d\n",seq,m_nBeg);
	}
	// �����ݱ��浽���մ���
	int idx = seq % m_nSegCount;

	if(m_pRecvPool[idx].HasData())
	{
		TRACE("�յ��ظ����ݰ� %d,beg = %d\n",seq,m_nBeg);
#ifdef _MyDebug
		m_nRepeatCount++;
#endif
		return XUDS_DATA_ACK::REPEAT;
	}
	else
	{
		m_pRecvPool[idx].Write(pData,nLen);
	}

	// ���Ѿ����յ�����д����ջ���
	int i = 0;
	int nEnd = m_nBeg + m_nSegCount;
	BYTE * pSegData;
	int nData = 0;
	int nSaved = 0;
	for(i = m_nBeg; i < nEnd;i++)
	{
		idx = i % m_nSegCount;
		if(m_pRecvPool[idx].HasData() == false)
		{
			break;
		}
		else
		{
			pSegData = m_pRecvPool[idx].Get(nData);
			if(m_recvData.GetSpace() < nData)
			{
				TRACE("err,���ݽ��ջ������\n");
				return XUDS_DATA_ACK::RECV_BUF_FULL;
			}
			nSaved = m_recvData.Push((char*)pSegData,nData);
			m_pRecvPool[idx].Reset();
			m_nBeg++;
		}
	}
	
	return XUDS_DATA_ACK::OK;
		
}