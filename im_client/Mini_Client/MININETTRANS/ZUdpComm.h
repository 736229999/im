/************************************************************************ 
 �ļ�������udp����ͨѶʵ����
    ʵ�ֿ��ٵ��������ݴ��䣬�����ص�I/O�������̳߳ء��շ�����ʵ�ָ�Ч���������ݴ���
 
 �����ˣ� zouyf, 2006-6-9 
 
 �޸ļ�¼��
	�����ʹ��UDPЭ��ģ��TCPЭ��Ŀɿ����������书�ܡ�
	Ŀǰ�������㣺
	1. ���ݴ����ٶ�: �����ݴ����ٶ��Ϻ�TCP����һ��
	2. ��������ǿ��: ��ʵ�ֲ�ͬ�������ӻ����µĻ������ӵ�����������
	��Ҫ�Ľ��ĵط�:
	1. ϵͳ��������: �ڴ����ٶȳ���5MB/s��ʱ��,CPUռ��������
	2. �����绷������Ӧ��: ��ǰ��δ��RTT���ж�̬��ֵ,��һЩ���绷�����ܻή�ʹ���Ч��
	3. ��Ҫ������մ����С,����Э�����ʹ��ڴ�С,��ʵ��ƽ�����ݴ���,Ŀǰ�����ʽΪ������ʽ
	�������ݴ�����Է����²���:
	������:
	1.���ӵ������ߣ�SESSION_ID Connect(dest_ip,dest_port,dest_id,my_id)
	2.�������ݣ�int Send(session_id,data,data_len)
	3.���ͽ������رջỰ, int CloseSession(session_id);
	�����ߣ�
	1.����ָ���û������ӣ�SESSION_ID Accept(dest_id,my_id)
	2.��������, int Recv(session_id,buf,buf_len)
 �޸���:	xy, 2007-4-2
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

// �������ܺ�cpu���ĵ��ڿ��Ʋ�����3-40��
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
		WAITING = 0,	// �ȴ�������
		SENDING = 1,	// ������
		CONFIRM = 2,	// �Ѿ�ȷ��
		RESEND = 3,		// �ط�
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


	// �ӿڳ�Ա����

	//------------------------------------------------------
	// �����׽���
	//------------------------------------------------------
	virtual XSOCKET		Socket(uint32 nWndSize,uint32 nSendbufSize ); 

	//------------------------------------------------------
	// ��ʼ��������
	//------------------------------------------------------
	virtual XSOCKET		Accept(uint32 myId,uint32 friendId,uint32 ip,uint16 port,uint32 tranIp,uint16 tranPort);

	//------------------------------------------------------
	// �������ݣ�������
	//------------------------------------------------------
	int					Send(XSOCKET s,const char * pData,int nLen);

	//------------------------------------------------------
	// �������ݣ�������
	//------------------------------------------------------
	int					Recv(XSOCKET s,char * pBuf,int nLen);

	int					GetSuccSend(XSOCKET s);

	//------------------------------------------------------
	// ��ʼ���� 
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
	// �ر��׽���
	//------------------------------------------------------
	void				CloseSocket(XSOCKET s) ;

	//------------------------------------------------------
	// �ر����ӣ����ȴ����ݷ������
	//------------------------------------------------------
	void				CloseWait(XSOCKET s);

	//------------------------------------------------------
	// �Ƿ��ṩ��ת���������� 
	//------------------------------------------------------
	void				setTransServer(bool val);

	virtual const char *	GetVersion() const;

public:

	void			ProcRecvStreamData(const RecvDataStruct * pRecvData);	// �����յ���������
	virtual void	PostQueueData(const char * data,int size);				// ����Ͷ�ݽ��ն�������
	virtual void	SendQueueData(const char * data,int size);				// ���緢�� ���Ͷ�������
	int				SendUdpData(const char* buf,uint32 size,uint32 destip,uint16 port);

private:

	friend void		RecvQueueCallback(void * pUserKey,const char * data,int size);
	friend void		SendQueueCallback(void * pUserKey,const char * data,int size);

	static DWORD WINAPI HandleP2PThreadFun(LPVOID lpParam); //�������첽�¼��̺߳���
	static DWORD WINAPI WrokNetRecvThreadFun(LPVOID lpParam);  //����������������̺߳���
	//static DWORD WINAPI HandleRecvOverlappedFun(LPVOID lpParam);

	// �ش���ʱ
	static void WINAPI	ReSendProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	// ���ڴ�С����
	static void WINAPI	MaxReqContrlProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	// 
	void	HandleReSendData();
	//
	void	HandleReqControl();

	void RecvNetData();			// ������·����

	//void HandleP2pPacket();		// ����p2p��

protected:

	MMRESULT			m_hTimerResend;
	MMRESULT			m_hTimerReqControl;

	void *				m_pUserKey;

	pfnUserNetCallback	m_pfnUserCallback;

	bool				m_bWork;

	int					m_nRecvPacketNum;		//���յ��İ�����

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
	HANDLE m_hNetRecvThread;	//����������������߳�
	HANDLE m_hRecvOverlapped;
	HANDLE m_hSemUsableSIO;		//���õķ���I/O�ź���
	HANDLE m_hSemUsableRIO;		//���õĽ���I/O�ź���
	HANDLE m_hQuitEvent;		//����ֹͣ�¼�
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
	//SafeList<int>		m_lstConnect;			// ��ǰ����

	CChannel			m_channel;			// �����շ����ݰ��ľ�����
	XUDS				m_uds;				// �����ݴ���
	bool				m_isTransServer;	// �Ƿ�����ת������
	CDataProc			m_dataProc;			// ��ת���������
	int					m_nBufferSize;		
};




//inline int CP2PSendPool::SendTo(SendDataStruct * pData)
//{
//	int size =  sizeof(pData->vlink) + pData->data_len;
//	return m_pUDP->SendUdpData((char *)&pData->vlink,size,pData->dest_ip,pData->port);
//};