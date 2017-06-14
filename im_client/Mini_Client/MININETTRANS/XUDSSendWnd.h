#pragma once

#include "p2pdef.h"
#include "XDataPool.h"
#include "TimeCount.h"
#include "SyncCritical.h" 
#include "Utility.h"

#ifdef _MyDebug
#include <fstream>
using namespace std;
#endif

class XUDSocket;
class XUDSSendWnd;

enum SEND_PACK_STATE {
	SPS_SENDING = 0,
	SPS_CONFIRMED,
	SPS_RESEND,
	SPS_RESENDING,
};

class XUDS_SEND_SEG
{
public:

	friend class XUDSSendWnd;

	enum TEST_RESULT{IS_SENDING,NEED_RESEND};

	XUDS_SEND_SEG(UINT nBufLen = BEST_SEG_SIZE)
	{
		m_state = SPS_SENDING;
		m_pBuf = new BYTE[nBufLen];
		m_nBufLen = nBufLen;
		m_nDataLen = 0;
		m_retryTimes = 1;
		m_tickCount = 0;
	}

	~XUDS_SEND_SEG()
	{
		if(m_pBuf)
		{
			delete [] m_pBuf;
			m_pBuf = NULL;
		}
	}

	void		SetSending(ULONGLONG tick,int nLen)
	{
		m_tickCount = tick;
		m_state = SPS_SENDING;
		m_retryTimes = 1;
		m_nDataLen = nLen;
	}

	SEND_PACK_STATE	GetState()
	{
		return m_state;
	};

	void		SetState(SEND_PACK_STATE s)
	{
		m_state = s;
	}

	void		SetResend()
	{
		m_state = SPS_RESEND;
		//m_retryTimes++;
	};

	void		SetResending()
	{
		m_state = SPS_RESENDING;
		m_tickCount = getTickCount();
		m_retryTimes++;
	}

	int			TestTimeOut(ULONGLONG uTime,UINT nRTO)
	{
		ULONGLONG inter = uTime - m_tickCount;
		if(inter > nRTO * m_retryTimes)
		//if(inter > nRTO)
			return NEED_RESEND;
		else
			return IS_SENDING;
	}

	LONGLONG	GetInterTime(ULONGLONG uEndTime)
	{
		return uEndTime - m_tickCount;
	};

	BYTE *		GetData()
	{
		return m_pBuf;
	}

	UINT		GetDataLen()
	{
		return m_nDataLen;
	}


protected:

	WORD		m_retryTimes;	// 重传次数
	ULONGLONG	m_tickCount;	// 时间戳

	SEND_PACK_STATE	m_state;		// 数据块状态,0.发送中,1.已经确认，2.重发中
	BYTE *		m_pBuf;			// 数据缓冲区
	UINT		m_nBufLen;		// 缓冲长度
	UINT			m_nDataLen;		// 数据长度

};


// 

class XUDSSendWnd
{
public:

	XUDSSendWnd(uint32 nSendWndSize,uint32 nSendBufSize);

	~XUDSSendWnd(void);

	enum ACK_STATE{ACK_OK = 0,ACK_ERR,ACK_REPEAT,ACK_QUICK_RESEND};

public:

	int				GetSendBufSize()
	{
		return m_buf.GetStackSize();
	}

	int				GetSendWndSize()
	{
		return m_nWndSize;
	}

	bool			ReSend(int nPack,XUDSocket * pS);

	int				PushData(const BYTE * pData,int nLen);

	bool			SendBuf(XUDSocket * pS);
	bool			HasBufData()
	{
		return m_buf.GetDataSize() > 0;
	};
	bool			CheckResend(XUDSocket * pS);

	//void			AddTask(int nSeq,const BYTE * pData,int nLen,ULONGLONG tickCount);

	ACK_STATE		ConfirmAck(XUDSocket * pS,uint32 seq,ULONGLONG tickCount);

	INT				GetRTO()
	{ 
		return m_RTO; 
	};

	INT				GetRTTA()
	{
		return m_RTT_A;
	}

	INT				GetRTTC()
	{
		return m_RTT_C;
	}

	void			Slow();
	
	int				GetSpace()
	{
		CMyLock lock(m_sc.Get());
		return (m_nWndSize - (m_nEnd - m_nBeg));
	};

	int				GetSendingPacket()
	{
		CMyLock lock(m_sc.Get());
		return m_nSending;
	}

	int				ClearSendBytes()
	{
		int n = m_nSendBytes;
		m_nSendBytes = 0;
		return n;
	}

	int				GetSuccSend()
	{
		return m_nSuccSend;
	}

	bool			IsEmpty()
	{
		CMyLock lock(m_sc.Get());
		return m_nBeg == m_nEnd;
	}

#ifdef _MyDebug
public:

	void			WriteDebugInfo(ofstream & ofs,int nRecords = -1);

	void			WriteSendInfo(ofstream & ofs,int nRecords = -1);

	void			WriteInfo(ofstream & ofs);

	

	UINT *			m_pQuickResend;
	int *			m_pAckInfo;
	UINT *			m_pAckTick;
	UINT *			m_pSendSeq;
	UINT *			m_pSendTick;
	UINT *			m_pSendWait;
	UINT *			m_pAIInfo;
	UINT *			m_pCurSPTInfo;
	int				m_nAckSeq;		// 
	int				m_nNeedSend;
	int				m_nSendSeq;	

	int				m_nSendInfo;	// 记录最大个数
	int				m_nAckInfo;
	int				m_nNextAck;		// 期望收到的下一个ack序号

	int				m_nAI;
	int				m_nAISeq;
	int				m_nResend;		// 重传的个数
	int				m_nQResend;		// 快速重传的个数
	UINT			m_nDebugBeg;	// 开始调试的时间
#endif


	bool			WaitAck(int nMS)
	{
		return ::WaitForSingleObject(m_eventAck,nMS) == WAIT_OBJECT_0;
	};

protected:

	XUDS_SEND_SEG *	m_pSendPool;	// 数据

	uint32			m_nWndSize;		// 当前发送窗口的大小

	uint32			m_nBeg;			// 当前发送窗口开始的位置
	
	uint32			m_nEnd;			// 当前发送窗口的结束位置

	uint32			m_nSending;		// 当前正在发送的包个数

	INT				m_RTT_A;		// 平滑的RTT(微秒）均值

	INT				m_RTO;			// 超时重传时间间隔

	INT				m_RTT_C;

	INT				m_RTT_D;		// 平滑的RTT均值偏差

	INT				m_nSendBytes;	// 单位时间内发送的字节数

	INT				m_nSuccSend;

	CSyncCritical	m_sc;

	Common::CXEvent	m_eventAck;

	INT				m_nAckNext;		// 期望的下一个序号

	INT				m_nAckTimes;	// 重复收到非预期ack的次数 

	XStreamStack	m_buf;			// 发送缓冲

	bool			CheckQuickResend(XUDSocket * pS,int nSeq,SEND_PACK_STATE st);

};
