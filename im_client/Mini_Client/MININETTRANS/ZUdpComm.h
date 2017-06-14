/************************************************************************ 
 文件描述：udp网络通讯实现类
    实现快速的网络数据传输，采用重叠I/O、利用线程池、收发队列实现高效的网络数据传输
 
 创建人： zouyf, 2006-6-9 
 
 修改记录：
	添加了使用UDP协议模拟TCP协议的可靠数据流传输功能。
	目前功能亮点：
	1. 数据传输速度: 在数据传输速度上和TCP传输一致
	2. 联机能力强大: 可实现不同网络连接环境下的基于连接的数据流传输
	需要改进的地方:
	1. 系统性能消耗: 在传输速度超过5MB/s的时候,CPU占用率明显
	2. 对网络环境的适应性: 当前尚未对RTT进行动态估值,在一些网络环境可能会降低传输效率
	3. 需要处理接收窗体大小,用来协调发送窗口大小,以实现平稳数据传输,目前解决方式为被动方式
	建立数据传输可以分以下步骤:
	发送者:
	1.连接到接收者，SESSION_ID Connect(dest_ip,dest_port,dest_id,my_id)
	2.发送数据，int Send(session_id,data,data_len)
	3.发送结束，关闭会话, int CloseSession(session_id);
	接受者：
	1.监听指定用户的连接，SESSION_ID Accept(dest_id,my_id)
	2.接收数据, int Recv(session_id,buf,buf_len)
 修改人:	xy, 2007-4-2
************************************************************************/

#pragma once
#include <PublicType.h>
#include <ZObjectPool.h>
#include "ZSignalQueue.h"
#include <MiniNetTrans.h>
#include "winsock2.h"
#include "Mmsystem.h"
#include <SyncCritical.h>
//#include "fcl.h"
//#include "netMisc.h"
//#include "Channel.h"
#include "TransProcess.h"
#include "xuds.h"

//#include <list>
//using namespace std;

#pragma warning(disable:4200)

// 传输性能和cpu消耗调节控制参数（3-40）
const int MAX_SENDIO_COUNT = 6;



const uint32	SEND_TIME_OUT	= 60000;
const uint32	RECV_TIME_OUT	= 90000;





typedef struct tagSEND_IO_CONTEXT
{
	//char			buf[WSA_MAXIMUM_WAIT_EVENTS-1][MAX_NETWORK_PACKET];
	char *			datas[WSA_MAXIMUM_WAIT_EVENTS-1];
	HANDLE			hEvents[WSA_MAXIMUM_WAIT_EVENTS];
	WSAOVERLAPPED	ol[WSA_MAXIMUM_WAIT_EVENTS-1];
	bool			used[WSA_MAXIMUM_WAIT_EVENTS-1];
	void			Reset()
	{
		int i;
		for(i = 0;i < sizeof(used)/sizeof(bool); i++)
		{
			used[i] = false;	
		}
		for(i = 0; i < sizeof(hEvents)/sizeof(HANDLE); i++)
		{
			::ResetEvent(hEvents[i]);
		}
	}
	int				GetEmpty() const
	{	
		int i = 0;
		for( ;i < WSA_MAXIMUM_WAIT_EVENTS - 1; i++ )
		{
			if( used[i] == false )
			{
				return i;
			}
		}
		return -1;
	}
	tagSEND_IO_CONTEXT()
	{
		int i;
		memset(this,0,sizeof(tagSEND_IO_CONTEXT));
		for( i = 0; i < WSA_MAXIMUM_WAIT_EVENTS-1; i++ )
		{

			hEvents[i] = CreateEvent(NULL,FALSE,FALSE,NULL);//WSACreateEvent();//
			used[i] = false;
			ol[i].hEvent = hEvents[i];
		}
		hEvents[i] = CreateEvent(NULL,FALSE,FALSE,NULL);
	}
	~tagSEND_IO_CONTEXT()
	{
		int i;
		for( i = 0; i < WSA_MAXIMUM_WAIT_EVENTS; i++ )
		{
			CloseHandle(hEvents[i]);
		}

	}
}SEND_IO_CONTEXT, * LPSEND_IO_CONTEXT;




typedef struct tagRECV_IO_CONTEXT
{
	//char			buf[WSA_MAXIMUM_WAIT_EVENTS-1][MAX_NETWORK_PACKET];
	HANDLE			hEvents[WSA_MAXIMUM_WAIT_EVENTS];
	WSAOVERLAPPED	ol[WSA_MAXIMUM_WAIT_EVENTS-1];
	SOCKADDR_IN		peerAddr[WSA_MAXIMUM_WAIT_EVENTS-1];
	bool			used[WSA_MAXIMUM_WAIT_EVENTS-1];
	void			Reset()
	{
		int i;
		for(i = 0; i < sizeof(used)/sizeof(bool); i++)
		{
			used[i] = false;	
		}
		for(i = 0; i < sizeof(hEvents)/sizeof(HANDLE);i++)
		{
			::ResetEvent(hEvents[i]);
		}
	}
	int				GetEmpty() const
	{	
		int i = 0;
		for( ;i < WSA_MAXIMUM_WAIT_EVENTS - 1; i++ )
		{
			if( used[i] == false )
			{
				return i;
			}
		}
		return -1;
	}
	tagRECV_IO_CONTEXT()
	{
		int i;
		memset(this,0,sizeof(tagRECV_IO_CONTEXT));
		for( i = 0; i < WSA_MAXIMUM_WAIT_EVENTS-1; i++ )
		{
			hEvents[i] = CreateEvent(NULL,FALSE,FALSE,NULL);
			used[i] = false;
			ol[i].hEvent = hEvents[i];
		}
		hEvents[i] = CreateEvent(NULL,FALSE,FALSE,NULL);
	}
	~tagRECV_IO_CONTEXT()
	{
		int i;
		for( i = 0; i < WSA_MAXIMUM_WAIT_EVENTS; i++ )
		{
			CloseHandle(hEvents[i]);
		}
	}

}RECV_IO_CONTEXT, * LPRECV_IO_CONTEXT;



enum UDP_SEND_ERR{
	UDP_OK = 0,
};



class CP2PEvent
{
protected:

	CP2PEvent(const CP2PEvent & o);

	void operator = (const CP2PEvent & o);

public:

	operator HANDLE () const
	{
		return m_hEvent;
	}

	CP2PEvent()
	{
		m_hEvent = ::CreateEvent(NULL,FALSE,FALSE,0);
	};

	~CP2PEvent()
	{
		if(m_hEvent)
		{
			::CloseHandle(m_hEvent);
		}
	}
	HANDLE	m_hEvent;
};

class CP2PAcceptInfo
{
protected:

	CP2PAcceptInfo(const CP2PAcceptInfo & o);

	void operator = (const CP2PAcceptInfo & o);

public:

	CP2PAcceptInfo()
	{
		src_id = -1;
		my_id = -1;
	}

	uint32		src_id;

	uint32		my_id;

	CP2PEvent	event;

};


class CSendPacketInfo
{
public:
	enum STATUS{
		WAITING = 0,	// 等待发送中
		SENDING = 1,	// 发送中
		CONFIRM = 2,	// 已经确认
		RESEND = 3,		// 重发
	};
	STATUS	status;
};


class CZUdpComm: public CUdpNetTrans
{
public:

	friend class CP2PRecvPool;
	friend class CP2PSendPool;

	CZUdpComm(int nConsumeThreadCount = 1, int nQuereCount = 1000, int nBufferSize = 1600);

	virtual ~CZUdpComm(void);

	void SetUserCallback(void * pUserKey,pfnUserNetCallback pfnCallback)
	{
		m_pUserKey = pUserKey;
		m_pfnUserCallback = pfnCallback;
	}

	virtual int StartListen(const char * addr,int port = 0);

	int SendDataTo(const SendDataStruct &sendData,int nPrior = 0);

	virtual void StopListen();

	long	GetSendQueueSize()
	{ return m_SendQueue.GetSize(); };
	
	long	GetRecvQueueSize()
	{ return m_RecvQueue.GetSize(); };

	uint16 GetPort() {return m_listenPort;}

	uint32 GetLocalIP() {return m_listenIP;}


	// 接口成员函数

	//------------------------------------------------------
	// 创建套接字
	//------------------------------------------------------
	virtual XSOCKET		Socket(uint32 nWndSize,uint32 nSendbufSize ); 

	//------------------------------------------------------
	// 开始接受连接
	//------------------------------------------------------
	virtual XSOCKET		Accept(uint32 myId,uint32 friendId,uint32 ip,uint16 port,uint32 tranIp,uint16 tranPort);

	//------------------------------------------------------
	// 发送数据（阻塞）
	//------------------------------------------------------
	int					Send(XSOCKET s,const char * pData,int nLen);

	//------------------------------------------------------
	// 接收数据（阻塞）
	//------------------------------------------------------
	int					Recv(XSOCKET s,char * pBuf,int nLen);

	int					GetSuccSend(XSOCKET s);

	//------------------------------------------------------
	// 开始连接 
	//------------------------------------------------------
	int					Connect(
		XSOCKET s,
		uint32 myId,
		uint32 friendId,
		uint32 ip,
		uint16 port,
		uint32 tranIp,
		uint16 tranPort
		);

	//------------------------------------------------------
	// 关闭套接字
	//------------------------------------------------------
	void				CloseSocket(XSOCKET s) ;

	//------------------------------------------------------
	// 关闭连接，并等待数据发送完成
	//------------------------------------------------------
	void				CloseWait(XSOCKET s);

	//------------------------------------------------------
	// 是否提供中转服务器功能 
	//------------------------------------------------------
	void				setTransServer(bool val);

	virtual const char *	GetVersion() const;

public:

	void			ProcRecvStreamData(const RecvDataStruct * pRecvData);	// 处理收到的数据流
	virtual void	PostQueueData(const char * data,int size);				// 向上投递接收队列数据
	virtual void	SendQueueData(const char * data,int size);				// 网络发送 发送队列数据
	int				SendUdpData(const char* buf,uint32 size,uint32 destip,uint16 port);

private:

	friend void		RecvQueueCallback(void * pUserKey,const char * data,int size);
	friend void		SendQueueCallback(void * pUserKey,const char * data,int size);

	static DWORD WINAPI HandleP2PThreadFun(LPVOID lpParam); //处理发送异步事件线程函数
	static DWORD WINAPI WrokNetRecvThreadFun(LPVOID lpParam);  //处理接收网络数据线程函数
	//static DWORD WINAPI HandleRecvOverlappedFun(LPVOID lpParam);

	// 重传定时
	static void WINAPI	ReSendProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	// 窗口大小控制
	static void WINAPI	MaxReqContrlProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	// 
	void	HandleReSendData();
	//
	void	HandleReqControl();

	void RecvNetData();			// 接收网路数据

	//void HandleP2pPacket();		// 处理p2p包

protected:

	MMRESULT			m_hTimerResend;
	MMRESULT			m_hTimerReqControl;

	void *				m_pUserKey;

	pfnUserNetCallback	m_pfnUserCallback;

	bool				m_bWork;

	int					m_nRecvPacketNum;		//接收到的包数量

	void				OnRecvP2PDataAck(const RecvDataStruct * pRecvData);

	void				OnRecvP2PData(const RecvDataStruct * pRecvData);

	void				OnP2PConnect(const RecvDataStruct * pRecvData);
	
	void				OnP2PAccept(const RecvDataStruct * pRecvData);

private:
	
	SEND_IO_CONTEXT		m_SendIO;
	RECV_IO_CONTEXT		m_RecvIO;

	//char				m_recvBuf[MAX_NETWORK_PACKET];
	SOCKADDR_IN			m_peerAddr;

	CZSignalQueue m_RecvQueue;
	CZSignalQueue m_SendQueue;

	//HANDLE m_hP2PThread;	//
	HANDLE m_hNetRecvThread;	//处理接收网络数据线程
	HANDLE m_hRecvOverlapped;
	HANDLE m_hSemUsableSIO;		//可用的发送I/O信号量
	HANDLE m_hSemUsableRIO;		//可用的接收I/O信号量
	HANDLE m_hQuitEvent;		//服务停止事件
	uint32 m_listenIP;
	uint16 m_listenPort;
	SOCKET m_sockListen;

	
	HANDLE m_hSemSignal;

	bool	m_bSendEvents[MAX_SENDIO_COUNT];

	//typedef std::map<SESSION_ID,CP2PSendPool*>	CP2PSendPools;
	//typedef std::map<SESSION_ID,CP2PRecvPool*>	CP2PRecvPools;
	//typedef std::map<SESSION_ID,CP2PEvent*>		CP2PConnectInfo;
	//typedef std::list<CP2PAcceptInfo*>			CP2PAcceptInfoList;	
	
	//CSyncCritical		m_scRecvPool;
	//CSyncCritical		m_scSendPool;
	//CSyncCritical		m_scSendIO;
	//SafeList<int>		m_lstConnect;			// 当前连接

	CChannel			m_channel;			// 用来收发数据包的具体类
	XUDS				m_uds;				// 流数据传输
	bool				m_isTransServer;	// 是否是中转服务器
	CDataProc			m_dataProc;			// 中转服务处理对象
	int					m_nBufferSize;		
};




//inline int CP2PSendPool::SendTo(SendDataStruct * pData)
//{
//	int size =  sizeof(pData->vlink) + pData->data_len;
//	return m_pUDP->SendUdpData((char *)&pData->vlink,size,pData->dest_ip,pData->port);
//};