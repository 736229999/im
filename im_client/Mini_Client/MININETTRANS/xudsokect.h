#pragma once

#include <map>
using namespace std;

#include "XUDSRecvWnd.h"
#include "XUDSSendWnd.h"

#include <WinSock2.h>
#pragma comment(lib,"Ws2_32.lib")



typedef int XSOCKET;



class XUDSRecvCapCalc
{
public:

	XUDSRecvCapCalc()
	{
		m_nBeg = 0;
		m_nEnd = 0;
		m_AI = 0;
		m_I = 0;
		m_nLastTick = -1;
		memset(m_interHistroy,0,sizeof(m_interHistroy));
	}

public:

	UINT				GetCurInter();

	//------------------------------------------------------
	// tickCount单位为微秒,返回和上次接收包之间的时间间隔
	//------------------------------------------------------
	UINT				AddRecvTick(ULONGLONG tickCount)
	{
		UINT nCount = m_nEnd - m_nBeg;
		UINT inter;
		UINT ai = 0;
		UINT cur = m_nEnd % 16;
		inter = tickCount - m_nLastTick;
		if(nCount == 16)
		{
			// 计算当前总时间间隔(加上新间隔，减去最旧的一个间隔)
			m_I += inter;
			m_I -= m_interHistroy[m_nBeg % 16];
			// 计算当前平均值
			ai = m_I / 16;
			m_nBeg++;
			m_nEnd++;
			m_nLastTick = tickCount;
		}
		else if(nCount == 0)
		{
			if(m_nLastTick != -1)
			{
				m_nEnd++;
				m_interHistroy[0] = inter;
				m_I += inter;
			}
			m_nLastTick = tickCount;
			return inter;
		}
		else
		{
			m_nEnd++;
			m_I += inter;
			m_interHistroy[cur] = inter;
			m_nLastTick = tickCount;
			return inter;
		}
		// 剔除误差太大的数据
		m_interHistroy[cur] = inter;
		int i;
		UINT ci = 0;
		int nC = 0;
		for(i = 0; i < 16;i++)
		{
			if(m_interHistroy[i] > ai * 8 || m_interHistroy[i] < ai / 8)
				continue;
			ci += m_interHistroy[i];
			nC++;
		}
		if(nC >= 8)
		{
			m_AI = ci / nC;
		}
		else
		{
			m_AI = 0;
		}
		return inter;
	};

	UINT				GetAI()
	{
		return m_AI;
	};

protected:

	ULONGLONG			m_interHistroy[16];

	ULONGLONG			m_nLastTick;

	UINT				m_nBeg;

	UINT				m_nEnd;

	UINT				m_I;				// 总时间间隔

	UINT				m_AI;				// 平均间隔时间
};


class XUDS;


class XUDSocket
{
public:
	
	XUDSocket(XSOCKET xs , int nWndSize ,int nSendBufSize);

	~XUDSocket(void);

	enum STATE{
		INVALID = 0,
		CONNECTED,
		CLOSED,
	};

	friend class XUDS;
	friend class XUDSSendWnd;

public:

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	XSOCKET			GetSocketId()
	{
		return m_xsocket;
	}

	//------------------------------------------------------
	// 关闭连接
	//------------------------------------------------------
	void			Close();

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	int				Send(const char * pData,int nLen);

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	bool			SendWait();

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	int				Recv(char * pBuf,int nLen);

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	void			Init(SOCKET s,SOCKADDR_IN addrPeer,uint32 nDestId);

	//------------------------------------------------------
	// 获取成功发送的字节数
	//------------------------------------------------------
	int				GetSuccSend();

	//------------------------------------------------------
	// 获取发送缓冲大小
	//------------------------------------------------------
	int				GetSendBufSize()
	{
		return m_sendWnd.GetSendBufSize();
	}

	int				GetSendWndSize()
	{
		return m_sendWnd.GetSendWndSize();
	}

protected:

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	void			ProcessData(const XUDS_PACKET & packet);

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	void			ProcessDataAck(const XUDS_PACKET & packet);

	//------------------------------------------------------
	// 休眠（微秒）
	//------------------------------------------------------
	void			USleep(UINT us)
	{
		UINT ms = us>>10;
		if(ms > 0)
		{
			Sleep(ms);
			us = us - (ms << 10);
		}
		CTimeCount::USleep(us);
	};

	//------------------------------------------------------
	// 定时重传
	//------------------------------------------------------
	void			OnTimerResend();
	
	//------------------------------------------------------
	// 拥塞控制
	//------------------------------------------------------
	void			OnTimerControl();

	//------------------------------------------------------
	// 定时响应
	//------------------------------------------------------
	void			OnTimer();
	
	//------------------------------------------------------
	// 重发数据包
	//------------------------------------------------------
	void			SendPacket(XUDS_DATA * pData);

	//------------------------------------------------------
	// 添加重传任务
	//------------------------------------------------------
	void			AddResend(int nSeq)
	{
		m_scResendList.Lock();
		if(m_lstResend.size() > 100)
		{
			m_lstResend.pop_back();
		}
		m_lstResend.push_back(nSeq);
		
		m_scResendList.Unlock();
	};

	//------------------------------------------------------
	// 添加快速重传任务
	//------------------------------------------------------
	void			AddQuickResend(int nSeq)
	{
		m_scResendList.Lock();
		if(m_lstResend.size() > 100)
		{
			m_lstResend.pop_back();
		}
		m_lstResend.push_front(nSeq);
		m_scResendList.Unlock();
	};

	void			SlowDown();


	
#ifdef _MyDebug


	int				DebugGetRecvTimes()
	{ return m_recvWnd.m_nRecvCount; };


	int				DebugGetRepeatRecvTimes()
	{ return m_recvWnd.m_nRepeatCount; } ;


	void			DebugPrintAckInfo(int nCountPerPage)
	{
		cout<<"ack info"<<endl;
		for(int i =0;i < m_sendWnd.m_nAckSeq;i++)
		{
			cout<<i<<":"<<m_sendWnd.m_pAckInfo[i]<<endl;
			if(i % nCountPerPage == (nCountPerPage - 1))
			{
				cout<<"press to continue.."<<endl;
				getchar();
			}
		}
	}

	void			DebugWriteInfo(const char * szFile,int nRecords = -1)
	{
		ofstream ofs(szFile);
		/*ofs<<"resend times = "<<DebugGetResendTimes()<<endl;
		ofs<<"send times = "<< DebugGetSendTimes()<<endl;
		ofs<<"wait times = "<< DebugGetWaitTimes()<<endl;
		ofs<<"RTT = "<< DebugGetRTT()<<endl;
		ofs<<"totoal send count="<<DebugGetResendTimes() + DebugGetSendCount()<<endl;
		ofs<<"quick resend ="<<DebugGetQuickResendTimes()<<endl;*/
		m_sendWnd.WriteInfo(ofs);
		/*int i;
		ofs<<"====ai info===="<<endl;
		for(i = 0;i < m_nAISeq;i++)
		{
			ofs<<i<<"\t"<<m_pAIInfo[i]<<endl;
		}*/
	}

#endif

protected:

	void			SendAck(uint32 seq,uint16 nLen,XUDS_DATA_ACK::RESULT rst);
	static DWORD WINAPI	ThreadSendData(LPVOID lpParam);

	void				SendData();

	ULONGLONG			m_nLastAckTick;

	ULONGLONG			m_nLastSendTick;

protected:

	XSOCKET			m_xsocket;		// 连接会话字

	XUDSSendWnd		m_sendWnd;		// 数据发送窗体

	XUDSRecvWnd		m_recvWnd;		// 数据接收窗体

	STATE			m_state;		// 套接字的状态

	UINT			m_SPT;			// 发送包时间间隔（微秒）

	UINT			m_RPT;			// 接收包时间间隔（微秒）

	UINT			m_PI;			// 

	double			m_fChannelCap;	// 信道容量

	BOOL			m_bQuickStart;	// 快启动
	
	SOCKADDR_IN		m_addrPeer;		// 连接方地址

	SOCKET			m_socket;		// 套接字

	uint32			m_nDestId;		// 目的用户id

	//uint32			m_seq;			// 自增序号

	ULONGLONG		m_nLastTick;	// 定时器，上次收到数据包时的嘀嗒时间

	ULONGLONG		m_nResendTick;	// 定时器，上次重传的tick

	ULONGLONG		m_nControlTick;	// 定时器，上次拥塞控制的tick

	XUDSRecvCapCalc	m_RecvCapCalc;	// 接收连接容量量测

	Common::CXEvent	m_eventConnect;	// 连接事件

	Common::CXEvent	m_eventTranOk;	// 中转连接建立事件 

	bool			m_bTranServer;	// 是否使用中转服务器

	//uint32			m_nRecvPos;

	UINT			m_nTickLastSlow;
	UINT			m_nTickLastAck;
	HANDLE			m_hSend;
	BOOL			m_bSend;
	list<int>		m_lstResend;
	CSyncCritical	m_scResendList;

	bool			HasResend()
	{
		CMyLock lock(m_scResendList.Get());
		return m_lstResend.size() > 0;
	}
#ifdef _MyDebug
	int				m_nWait;
	int				m_nSendCount;
	/*UINT *			m_pAIInfo;
	int				m_nAI;
	int				m_nAISeq;*/
#endif

};


class XSocketMap:public map<XSOCKET,XUDSocket*>
{
public:
	void Lock()
	{
		m_sc.Lock();
	};

	void Unlock()
	{
		m_sc.Unlock();
	};

	CRITICAL_SECTION *	GetCS()
	{
		return m_sc.Get();
	}

	
protected:

	CSyncCritical	m_sc;

};
