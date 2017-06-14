#pragma once

#include "p2pdef.h"
#include "xdatapool.h"
#include "SyncCritical.h"
#include "TimeCount.h"


const int RECV_BUF_LEN = 200 * 1024;

struct XUDS_RECV_SEG
{
	XUDS_RECV_SEG(UINT nBufLen = MAX_SEG_SIZE)
	{
		m_state = 0;
		m_pBuf = new BYTE[nBufLen];
		m_nBufLen = nBufLen;
		m_pData = m_pBuf;
		m_nDataLen = 0;
	}

	~XUDS_RECV_SEG()
	{
		if(m_pBuf)
		{
			delete [] m_pBuf;
			m_pBuf = NULL;
		}
	}

	void		Write(const BYTE * pData,uint32 nLen)
	{
		m_state = 1;
		assert(nLen <= m_nBufLen && "�������ݰ����ȳ����洢�ռ�");
		memcpy(m_pBuf,pData,nLen);
		m_pData = m_pBuf;
		m_nDataLen = nLen;
	}

	bool		HasData() const
	{
		return m_state != 0;
	}

	BYTE *		Get(int & nLen)
	{
		nLen = m_nDataLen;
		return m_pData;
	};

	void		Reset()
	{
		m_state = 0;
		m_pData = m_pBuf;
		m_nDataLen = 0;
	};

	int			Read(BYTE * pData,int nLen)
	{
		if(nLen >= m_nDataLen)
		{
			m_state = 0;
			memcpy(pData,m_pData,m_nDataLen);
			return m_nDataLen;
		}
		else
		{
			memcpy(pData,m_pData,nLen);
			m_pData += nLen;
			m_nDataLen -= nLen;
			return nLen;
		}
	};

protected:

	BYTE		m_state;	// ���ݿ�״̬,0.������,1.������
	BYTE *		m_pBuf;		// ���ݻ�����
	UINT		m_nBufLen;	// ���峤��
	UINT		m_nDataLen;	// ���ݳ���
	BYTE *		m_pData;	// ���ݿ�ʼλ��
};





class XUDSRecvWnd
{
public:

	XUDSRecvWnd(int nRecvWndSize);

	~XUDSRecvWnd(void);

public:

	UINT				GetRecvLen();

	UINT				ReadData(BYTE * pData,UINT nLen);

	XUDS_DATA_ACK::RESULT	RecvPacket(UINT seq,BYTE * pData,UINT nLen);

	int					GetNotifyWnd();

protected:

	XStreamStack		m_recvData;		// �Ѿ����յ�����

	UINT				m_nSegCount;	// ���մ��ڵĴ�С

	UINT				m_nBeg;			// ���ڿ�ʼλ��

	XUDS_RECV_SEG *		m_pRecvPool;	// ���մ���

	UINT				m_nSegLen;		// �����Ĺ̶�����


	CSyncCritical		m_sc;

#ifdef _MyDebug
public:
	int					m_nRecvCount;
	int					m_nRepeatCount;
	
#endif

};
