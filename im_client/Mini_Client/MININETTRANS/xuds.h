#pragma once

#include "xudsokect.h"
#include "XDataPool.h"
#include <list>

class XUDSAcceptInfo
{
protected:

	XUDSAcceptInfo(const XUDSAcceptInfo & o);

	void operator = (const XUDSAcceptInfo & o);

public:

	XUDSAcceptInfo()
	{
		my_id = -1;
		friend_id = -1;
		xs = -1;
		need_tran_logon = false;
	}

	uint32			my_id;

	uint32			friend_id;

	bool			need_tran_logon;

	XSOCKET			xs;

	Common::CXEvent	event;

	Common::CXEvent	eventTranOk;

};

class XUDSAcceptList:public std::list<XUDSAcceptInfo*>
{
public:
	
	void Lock()
	{
		m_sc.Lock();
	}

	void Unlock()
	{
		m_sc.Unlock();
	}


protected:
	Common::CSyncCritical	m_sc;
};




class XUDS
{
public:

	XUDS();

	~XUDS(void);

public:

	//------------------------------------------------------
	// 初始化
	//------------------------------------------------------
	void				Init(SOCKET s);

	//------------------------------------------------------
	// 停止
	//------------------------------------------------------
	void				UnInit();

	//------------------------------------------------------
	// 创建套接字
	//------------------------------------------------------
	XSOCKET				Socket(uint32 nWndSize,uint32 nSendbufSize);

	//------------------------------------------------------
	// 发送数据（阻塞）
	//------------------------------------------------------
	int					Send(XSOCKET s,const char * pData,int nLen);

	//------------------------------------------------------
	// 接收数据（阻塞）
	//------------------------------------------------------
	int					Recv(XSOCKET s,char * pBuf,int nLen);

	//------------------------------------------------------
	// 开始连接 
	//------------------------------------------------------
	int					Connect(
		XSOCKET s,
		uint32 myId,
		uint32 friendId,
		uint32 ip,			// net byte
		uint16 port,		// net byte
		uint32 tranIp,		// net byte
		uint16 tranPort		// net byte
	);
	
	//------------------------------------------------------
	// 开始接受连接
	//------------------------------------------------------
	XSOCKET				Accept(
		uint32 myId,
		uint32 friendId,
		uint32 ip,			// net byte
		uint16 port,		// net byte
		uint32 tranIp,		// net byte
		uint16 tranPort		// net byte
		);

	//------------------------------------------------------
	// 关闭套接字
	//------------------------------------------------------
	void				CloseSocket(XSOCKET s);

	//------------------------------------------------------
	// 接收网络数据
	//------------------------------------------------------
	void				RecvNetData(XUDS_PACKET & packet,SOCKADDR_IN addrPeer);

	//------------------------------------------------------
	// 关闭连接，并等待数据发送完成
	//------------------------------------------------------
	void				CloseWait(XSOCKET s);

	//------------------------------------------------------
	// 当前连接成功发送的字节个数
	//------------------------------------------------------
	int					GetSuccSend(XSOCKET s);


#ifdef _MyDebug
public:
#else
protected:
#endif
	//------------------------------------------------------
	// 
	//------------------------------------------------------
	XUDSocket *			FindSocket(XSOCKET s)
	{
		CMyLock mlk(m_sockets.GetCS());
		XSocketMap::iterator it;
		it = m_sockets.find(s);
		if(it == m_sockets.end())
			return NULL;
		else
			return it->second;
	};

protected:

	//------------------------------------------------------
	// 手动关闭一个监听 
	//------------------------------------------------------
	void				CloseAccept(uint32 myId,uint32 friendId);

	//------------------------------------------------------
	// 开始中转服务器连接
	//------------------------------------------------------
	int					StartTranConnect(XUDSocket *pS,uint32 myId,uint32 friendId,uint32 tranIp,uint16 tranPort);

	//------------------------------------------------------
	// 开始接受中转服务器连接
	//------------------------------------------------------
	XSOCKET				StartTranAccept(XUDSAcceptInfo * pTask,uint32 tranIp,uint16 tranPort);

	//------------------------------------------------------
	// 清除无用的socket资源
	//------------------------------------------------------
	void				ClearGarbage();


protected:

	void				OnRecvStreamPacket(const XUDS_PACKET & pack);

	void				ProcessConnect(const XUDS_PACKET & pack);

	void				ProcessTest(const XUDS_PACKET & pack);

	void				ProcessAccept(const XUDS_PACKET & pack);

	void				ProcessQueryAck(const XUDS_PACKET & pack);

	void				ProcessClose(const XUDS_PACKET & pack);

protected:

	static DWORD WINAPI	ThreadFunProcStream(LPVOID lpParam);

	void				ProcStreamPacket();

	//static DWORD WINAPI	ThreadFunRecvData(LPVOID lpParam);

	//void				ProcRecvData();

	static DWORD WINAPI	ThreadFunTimer(LPVOID lpParam);

	void				ProcTimer();	

protected:

	class NouseSocket
	{
	public:
		XUDSocket * _pS;
		int			_tick;
	};

	class NouseSocketList:public std::list<NouseSocket>
	{

	};

	XSocketMap		m_sockets;		// 套接字库

	NouseSocketList	m_noUseSocket;	// 作废的套接字连接

	XSOCKET			m_nISN;			// 套接字内部序号

	SOCKET			m_s;			// udp套接字

	HANDLE			m_hStream;		// 流数据处理线程

	HANDLE			m_hRecv;		// 数据接收线程

	HANDLE			m_hTimer;		// 定时器线程

	XDataPool		m_recvDataPool;	// 接收流数据包缓冲区

	bool			m_bWork;		// 是否在工作状态

	XUDSAcceptList	m_acceptTasks;	// 监听任务

	Common::CXEvent	m_eventRecv;	// 接收到数据包的通知

};
