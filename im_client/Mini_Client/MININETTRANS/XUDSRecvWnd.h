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
		assert(nLen <= m_nBufLen && "接收数据包长度超出存储空间");
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

	BYTE		m_state;	// 数据块状态,0.无数据,1.有数据
	BYTE *		m_pBuf;		// 数据缓冲区
	UINT		m_nBufLen;	// 缓冲长度
	UINT		m_nDataLen;	// 数据长度
	BYTE *		m_pData;	// 数据开始位置
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

	XStreamStack		m_recvData;		// 已经接收的数据

	UINT				m_nSegCount;	// 接收窗口的大小

	UINT				m_nBeg;			// 窗口开始位置

	XUDS_RECV_SEG *		m_pRecvPool;	// 接收窗口

	UINT				m_nSegLen;		// 缓冲块的固定长度


	CSyncCritical		m_sc;

#ifdef _MyDebug
public:
	int					m_nRecvCount;
	int					m_nRepeatCount;
	
#endif

};
