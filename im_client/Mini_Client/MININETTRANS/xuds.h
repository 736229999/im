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
	// ��ʼ��
	//------------------------------------------------------
	void				Init(SOCKET s);

	//------------------------------------------------------
	// ֹͣ
	//------------------------------------------------------
	void				UnInit();

	//------------------------------------------------------
	// �����׽���
	//------------------------------------------------------
	XSOCKET				Socket(uint32 nWndSize,uint32 nSendbufSize);

	//------------------------------------------------------
	// �������ݣ�������
	//------------------------------------------------------
	int					Send(XSOCKET s,const char * pData,int nLen);

	//------------------------------------------------------
	// �������ݣ�������
	//------------------------------------------------------
	int					Recv(XSOCKET s,char * pBuf,int nLen);

	//------------------------------------------------------
	// ��ʼ���� 
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
	// ��ʼ��������
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
	// �ر��׽���
	//------------------------------------------------------
	void				CloseSocket(XSOCKET s);

	//------------------------------------------------------
	// ������������
	//------------------------------------------------------
	void				RecvNetData(XUDS_PACKET & packet,SOCKADDR_IN addrPeer);

	//------------------------------------------------------
	// �ر����ӣ����ȴ����ݷ������
	//------------------------------------------------------
	void				CloseWait(XSOCKET s);

	//------------------------------------------------------
	// ��ǰ���ӳɹ����͵��ֽڸ���
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
	// �ֶ��ر�һ������ 
	//------------------------------------------------------
	void				CloseAccept(uint32 myId,uint32 friendId);

	//------------------------------------------------------
	// ��ʼ��ת����������
	//------------------------------------------------------
	int					StartTranConnect(XUDSocket *pS,uint32 myId,uint32 friendId,uint32 tranIp,uint16 tranPort);

	//------------------------------------------------------
	// ��ʼ������ת����������
	//------------------------------------------------------
	XSOCKET				StartTranAccept(XUDSAcceptInfo * pTask,uint32 tranIp,uint16 tranPort);

	//------------------------------------------------------
	// ������õ�socket��Դ
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

	XSocketMap		m_sockets;		// �׽��ֿ�

	NouseSocketList	m_noUseSocket;	// ���ϵ��׽�������

	XSOCKET			m_nISN;			// �׽����ڲ����

	SOCKET			m_s;			// udp�׽���

	HANDLE			m_hStream;		// �����ݴ����߳�

	HANDLE			m_hRecv;		// ���ݽ����߳�

	HANDLE			m_hTimer;		// ��ʱ���߳�

	XDataPool		m_recvDataPool;	// ���������ݰ�������

	bool			m_bWork;		// �Ƿ��ڹ���״̬

	XUDSAcceptList	m_acceptTasks;	// ��������

	Common::CXEvent	m_eventRecv;	// ���յ����ݰ���֪ͨ

};
