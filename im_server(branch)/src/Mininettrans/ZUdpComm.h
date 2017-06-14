/************************************************************************ 
 �ļ�������udp����ͨѶʵ����
    ʵ�ֿ��ٵ��������ݴ��䣬�����ص�I/O�������̳߳ء��շ�����ʵ�ָ�Ч���������ݴ���
 
 �����ˣ� zouyf, 2006-6-9 
 
 �޸ļ�¼�� 
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

#define MAX_EVENT_NUMBER 1024    //event �����Ŀ

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
	void SetUserCallback(FunRecvNetData funRecvNetData);
	//{
	//	m_funRecvNetData = funRecvNetData;
	//}
	virtual int StartListen(const char * addr,int port = 0);
	int SendDataTo(const SendDataStruct &sendData);
	virtual void StopListen();
	uint16 GetPort() {return m_listenPort;}
	uint32 GetLocalIP() {return m_listenIP;}
	CZSignalQueue* GetRecvQueue() {return &m_RecvQueue;}
	void StartTcpWork();
protected:
	virtual void PostQueueData(const char * data,int size);  //����Ͷ�ݽ��ն�������
	virtual void SendQueueData(const char * data,int size);  //���緢�� ���Ͷ�������

	int SendUdpData(LPWSABUF lpWsaBuf,uint32 destip,uint16 port);
	int SendTcpData(LPWSABUF lpWsaBuf,uint32 destip,uint16 port,uint16 cmd);
private:
	friend void RecvQueueCallback(void * pUserKey,const char * data,int size);
	friend void SendQueueCallback(void * pUserKey,const char * data,int size);

	static void* WrokNetRecvThreadFun(LPVOID lpParam);  //����������������̺߳���
	static void* WrokNetTcpThreadFun(LPVOID lpParam);

	void sendStytemTimeToNewConnect(int acptFd);
	void recvTcpData();
	void webmsg_push(char* data,int fd);
	int recv_webmsg(int fd);
	void addFd(int epollfd,int fd,bool one_shot);
	void closeConn(int epollfd,int fd);
	void eraseConn(int fd);

	void RecvNetData();			// ������·����
	void HandleSendIO();		// �������첽�¼�
	void RecvOverlapped();		// 


protected:
	void *				m_pUserKey;
	pfnUserNetCallback	m_pfnUserCallback;
	bool				m_bWork;

	int		m_nRecvPacketNum;		//���յ��İ�����
	FunRecvNetData m_funRecvNetData;

private:

	char				m_recvBuf[MAX_NETWORK_PACKET];
	SOCKADDR_IN			m_peerAddr;

	CZSignalQueue m_RecvQueue;
	CZSignalQueue m_SendQueue;

	pthread_t m_hNetRecvThread;	//����������������߳�
	pthread_t m_hNetTcpThread;

	uint32 m_listenIP;
	uint16 m_listenPort;
	SOCKET m_sockListen;
	t_sock_containor sock_conns;

public:

	int GetSendQueueSize() { return m_SendQueue.GetSize(); }
	int GetRecvQueueSize() { return m_RecvQueue.GetSize(); }
};

