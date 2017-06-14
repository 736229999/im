/************************************************************************ 
 文件描述：udp网络通讯实现类
    实现快速的网络数据传输，采用重叠I/O、利用线程池、收发队列实现高效的网络数据传输
 
 创建人： zouyf, 2006-6-9 
 
 修改记录： 
************************************************************************/

#pragma once
#include "PublicType.h"
#include "ZObjectPool.h"
#include "ZSignalQueue.h"
#include "MiniNetTrans.h"
#include <netinet/in.h>    // for sockaddr_in
#include <sys/socket.h>    // for socket
#include "SysStatic.h"


#include <list>
using namespace std;

#define MAX_RECV_BUF_SIZE 1440
class t_sock_info
{
public:
	t_sock_info()
	{
		port = 0;
		remain_buf_len = 0;
		cur_pos = 0;
		recv_buf = NULL;
		recv_buf = (unsigned char *)malloc(MAX_RECV_BUF_SIZE+MAX_RECV_BUF_SIZE);
		if(recv_buf != NULL)
		{
			memset(recv_buf, 0, MAX_RECV_BUF_SIZE+MAX_RECV_BUF_SIZE);
		}
		else
		{
			throw -1;
		}
	}
	t_sock_info(uint32 _addr, unsigned int _port)
	{
		port = _port;
		addr = _addr;
		remain_buf_len = 0;
		cur_pos = 0;
		recv_buf = NULL;
		recv_buf = (unsigned char *)malloc(MAX_RECV_BUF_SIZE+MAX_RECV_BUF_SIZE);
		if(recv_buf != NULL)
		{
			memset(recv_buf, 0, MAX_RECV_BUF_SIZE+MAX_RECV_BUF_SIZE);
		}
		else
		{
			throw -1;
		}
	}
	t_sock_info& operator=(const t_sock_info &sobj)
	{
		port = sobj.port;
		addr = sobj.addr;
		memcpy(recv_buf, sobj.recv_buf, MAX_RECV_BUF_SIZE+MAX_RECV_BUF_SIZE);
		remain_buf_len = sobj.remain_buf_len;
		cur_pos = sobj.cur_pos;
		return *this;
	}
	~t_sock_info()
	{
		if(recv_buf != NULL)
		{
			free(recv_buf);
			recv_buf = NULL;
		}
	}
	unsigned int port;
	uint32 addr; 
	unsigned char *recv_buf;
	int remain_buf_len;
	unsigned int cur_pos;
};

typedef SafeMap<uint32, t_sock_info*> t_sock_containor;

class CZUdpComm
{
public:
	CZUdpComm(int nConsumeThreadCount = 1);
	virtual ~CZUdpComm(void);
	void SetUserCallback(void * pUserKey,pfnUserNetCallback pfnCallback)
	{
		m_pUserKey = pUserKey;
		m_pfnUserCallback = pfnCallback;
	}
	virtual int StartListen(const char * addr,int port = 0);
	int SendDataTo(const SendDataStruct &sendData);
	virtual void StopListen();
	uint16 GetPort() {return m_listenPort;}
	uint32 GetLocalIP() {return m_listenIP;}
	CZSignalQueue* GetRecvQueue() {return &m_RecvQueue;}
	void StartTcpWork();
protected:
	virtual void PostQueueData(const char * data,int size);  //向上投递接收队列数据
	virtual void SendQueueData(const char * data,int size);  //网络发送 发送队列数据

	int SendUdpData(LPWSABUF lpWsaBuf,uint32 destip,uint16 port);
	int SendTcpData(LPWSABUF lpWsaBuf,uint32 destip,uint16 port,uint16 cmd);
private:
	friend void RecvQueueCallback(void * pUserKey,const char * data,int size);
	friend void SendQueueCallback(void * pUserKey,const char * data,int size);

	static void* WrokNetRecvThreadFun(LPVOID lpParam);  //处理接收网络数据线程函数
	static void* WrokNetTcpThreadFun(LPVOID lpParam);

	void RecvTcpData();
	int webmsg_push(uint8* data,int fd);
	int recv_webmsg(int fd);
	void addfd(int epollfd,int fd,bool one_shot);
	void close_conn(int epollfd,int fd);
	void erase_conn(int fd);
	int setnonblocking(int fd);

	void RecvNetData();			// 接收网路数据
	void HandleSendIO();		// 处理发送异步事件
	void RecvOverlapped();		// 


protected:
	void *				m_pUserKey;
	pfnUserNetCallback	m_pfnUserCallback;
	bool				m_bWork;

	int		m_nRecvPacketNum;		//接收到的包数量

private:

	char				m_recvBuf[MAX_NETWORK_PACKET];
	SOCKADDR_IN			m_peerAddr;

	CZSignalQueue m_RecvQueue;
	CZSignalQueue m_SendQueue;

	pthread_t m_hNetRecvThread;	//处理接收网络数据线程
	pthread_t m_hNetTcpThread;

	uint32 m_listenIP;
	uint16 m_listenPort;
	SOCKET m_sockListen;
	t_sock_containor sock_conns;

public:

	int GetSendQueueSize() { return m_SendQueue.GetSize(); }
	int GetRecvQueueSize() { return m_RecvQueue.GetSize(); }
};

