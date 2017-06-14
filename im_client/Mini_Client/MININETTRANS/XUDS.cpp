#include "StdAfx.h"
#include "xuds.h"

#ifdef _DEBUG
//#define _TEST_TRAN_SERVER
#endif

#define P2P_CONNECT_TIME			10000
#define P2P_CONNECT_WAIT_TIME		2000
#define TRAN_SERVER_CONN_TIME		10000
#define TRAN_SERVER_CONN_WAIT_TIME	2000

DWORD WINAPI XUDS::ThreadFunProcStream(LPVOID lpParam)
{
	XUDS * pThis = (XUDS *)lpParam;
	pThis->ProcStreamPacket();
	return 0;
}

//DWORD WINAPI XUDS::ThreadFunRecvData(LPVOID lpParam)
//{
//	XUDS * pThis = (XUDS *)lpParam;
//	pThis->ProcRecvData();
//	return 0;
//}


DWORD WINAPI XUDS::ThreadFunTimer(LPVOID lpParam)
{
	XUDS * pThis = (XUDS *)lpParam;
	pThis->ProcTimer();
	return 0;
}




XUDS::XUDS()
{
	m_s = NULL;
	m_bWork = false;
	

	// 生成随机序号
	srand(GetTickCount());
	m_nISN = rand();
	m_nISN = m_nISN << 16;
	m_nISN += rand();

	m_recvDataPool.Init(256,1500);

}

XUDS::~XUDS(void)
{

	m_bWork = false;

	if(::WaitForSingleObject(m_hTimer,100) == WAIT_TIMEOUT)
	{
		TerminateThread(m_hTimer,-1);
	}

	XUDSAcceptList::iterator it,itE;
	itE = m_acceptTasks.end();
	for(it = m_acceptTasks.begin();it != itE;it++)
	{
		if(*it)
		{
			delete *it;
		}
	}

	m_sockets.Lock();
	XSocketMap::iterator its,itsE;
	itsE = m_sockets.end();
	for(its = m_sockets.begin(); its != itsE; its++)
	{
		if(its->second)
		{
			delete its->second;
		}
	}
	m_sockets.Unlock();

}


void XUDS::Init(SOCKET s)
{
	m_s = s;
	m_bWork = true;
	m_hStream = CreateThread(NULL,0,ThreadFunProcStream,this,0,NULL);
	m_hTimer = CreateThread(NULL,0,ThreadFunTimer,this,0,NULL);
	//m_hRecv = CreateThread(NULL,0,ThreadFunRecvData,this,0,NULL);
}

void XUDS::UnInit()
{
	m_s = NULL;
	m_bWork = FALSE;
}


XSOCKET XUDS::Socket(uint32 nWndSize,uint32 nSendbufSize)
{
	m_nISN++;
	if(m_nISN == 0)
		m_nISN++;
	XUDSocket * pS = new XUDSocket(m_nISN,nWndSize,nSendbufSize);
	m_sockets[m_nISN] = pS;

	ClearGarbage();

	return m_nISN;
}

int XUDS::Send(XSOCKET s,const char * pData,int nLen)
{
	XUDSocket * pS = FindSocket(s);
	if(pS == NULL)
	{
		return -1;
	}
	return pS->Send(pData,nLen);	
}

int XUDS::Recv(XSOCKET s,char * pBuf,int nLen)
{
	XUDSocket * pS = FindSocket(s);
	if(pS == NULL)
	{
		TRACE("XUDS_INVALID_SOCKET\n");
		return -1;
	}
	return pS->Recv(pBuf,nLen);	
}

void XUDS::ProcStreamPacket()
{
	XUDS_PACKET packet;
	int nLen = 0;
	while(m_bWork)
	{
		if(m_recvDataPool.GetSize() > 0)
		{
			nLen = m_recvDataPool.Pop((char*)&packet,sizeof(packet));
			OnRecvStreamPacket(packet);
		}
		else
		{
			WaitForSingleObject(m_eventRecv,2);
		}
	}
}




void XUDS::ProcTimer()
{
	// 人工定时器
	XSocketMap::iterator it,itE;
	
	while(m_bWork)
	{
		Sleep(1);
		m_sockets.Lock();
		itE = m_sockets.end();
		for(it = m_sockets.begin();it != itE; it++)
		{
			if(it->second)
			{
				it->second->OnTimer();
			};
		}
		m_sockets.Unlock();
	}
}

void XUDS::RecvNetData(XUDS_PACKET & packet,SOCKADDR_IN addrPeer)
{
	XUDS_HEADER * pHead = (XUDS_HEADER *)packet.data;
	if(pHead->flag == UDS_FLAG)
	{
		packet.tickCount = getTickCount();
		if(m_recvDataPool.IsFull())
		{
			TRACE("err,流数据接收缓冲已满!\n");
			return;
		}
		packet.srcIp = addrPeer.sin_addr.s_addr;
		packet.srcPort = addrPeer.sin_port;
		m_recvDataPool.Push((char*)&packet,sizeof(packet) - sizeof(packet.data) + packet.len);
		m_eventRecv.Set();
	}
}

//void XUDS::ProcRecvData()
//{
//	int nRecv = 0;
//	XUDS_PACKET packet;
//	SOCKADDR_IN addrPeer;
//	int nAddrLen = sizeof(SOCKADDR_IN);
//
//	for(;;)
//	{
//		// 接收网络数据
//		nRecv = recvfrom(m_s,packet.data,sizeof(packet.data),0,(sockaddr*)&addrPeer,&nAddrLen);		
//		if(nRecv <= 0)
//			continue;
//		packet.len = nRecv;
//		RecvNetData(packet,addrPeer);
//	}
//}

void XUDS::OnRecvStreamPacket(const XUDS_PACKET & pack)
{
	XUDS_HEADER * pHead = (XUDS_HEADER*)pack.data;
	XUDSocket *pS = FindSocket(pHead->socketId);
	switch(pHead->cmd)
	{
	case CMD_P2P_DATA:
		if(pS)
		{
			pS->ProcessData(pack);
		}
		break;
	case CMD_P2P_ACK:
		if(pS)
		{
			pS->ProcessDataAck(pack);
		}
		break;
	case CMD_P2P_KEEP_LIVE:
	    break;
	case CMD_P2P_CONNECT:
		ProcessConnect(pack);
	    break;
	case CMD_P2P_ACCEPT:
		ProcessAccept(pack);
		break;
	case CMD_QUERY_ACK:
		ProcessQueryAck(pack);
		break;
	case CMD_P2P_CLOSE:
		ProcessClose(pack);
		break;
	case CMD_P2P_CLOSE_ACK:
		break;
	case CMD_P2P_TEST_REQ:
		ProcessTest(pack);
		break;
	case CMD_P2P_TEST_ACK:
		break;
	case CMD_QUERY_REQ:
		break;
	default:
	    break;
	}
}

void XUDS::ProcessQueryAck(const XUDS_PACKET & pack)
{
	XUDS_QUERY_ACK * pAck = (XUDS_QUERY_ACK*)pack.data;

	if(pAck->rt == XUDS_QUERY_ACK::OK)
	{
		// 登陆中转并查询好友成功

		// 通知中转连接或中转接受
		XUDSocket * pS = NULL;
		if(pAck->socketId != 0)
			pS = FindSocket(pAck->socketId);
		if(pS)
		{
			pS->m_eventTranOk.Set();
		}
		else
		{
			XUDSAcceptList::iterator it,itE;
			m_acceptTasks.Lock();
			itE = m_acceptTasks.end();
			for(it = m_acceptTasks.begin();it != itE;it++)
			{
				XUDSAcceptInfo * pAcc = (*it);
				if((*it)->friend_id == pAck->friendId && (*it)->my_id == pAck->did)
				{
					// 登陆中转服务器成功,不需要再登陆
					(*it)->need_tran_logon = false;
					// 通知中转接受成功
					(*it)->eventTranOk.Set();
					break;
				}
			}
			m_acceptTasks.Unlock();
		}
	}
	else if(pAck->rt == XUDS_QUERY_ACK::INFO_TIMEOUT)
	{
		TRACE("connect err:被连接的用户没有准备好\n");
	}
	else
	{
		TRACE("connect err:被连接的用户没有登陆\n");
	}
}

void XUDS::ProcessClose(const XUDS_PACKET & pack)
{
	XUDS_CLOSE * pClose = (XUDS_CLOSE*)pack.data;
	
	XUDSocket * pS = FindSocket(pClose->socketId);
	if(pS == NULL)
		return;
	pS->Close();
}

void XUDS::ProcessAccept(const XUDS_PACKET & pack)
{
	XUDS_ACCEPT * pAcc = (XUDS_ACCEPT *)pack.data;

	// 判断该同意连接是否有效

	XUDSocket * pS = FindSocket(pAcc->socketId);
	if(pS == NULL)
	{
		return;
	}
	
	if(pAcc->rt == XUDS_ACCEPT::DENEY)
	{
		// 对方拒绝连接
		TRACE("p2p warning:对方拒绝连接\n");
		pS->m_state = XUDSocket::CLOSED;
	}
	else if(pAcc->rt == XUDS_ACCEPT::WAIT)
	{
		TRACE("p2p 对方没有准备好，稍后重试连接\n");
		return;
	}
	else
	{
		TRACE("p2p 对方同意连接\n");
		pS->m_state = XUDSocket::CONNECTED;
	}

	pS->m_bTranServer = false;
	pS->m_eventConnect.Set();
	
}

void XUDS::ProcessTest(const XUDS_PACKET & pack)
{
	XUDS_P2P_TEST * pTest = (XUDS_P2P_TEST*)pack.data;
	TRACE("p2p recv %d test\n",pTest->testId);
	CMyLock mlk(m_sockets.GetCS());
	XSocketMap::iterator it,itEnd;
	it = m_sockets.begin();
	itEnd = m_sockets.end();
	for(;it != itEnd;it++)
	{
		if(it->second->m_state == XUDSocket::INVALID)
		{
			if(it->second->m_nDestId == pTest->testId)
			{
				// update
				it->second->m_addrPeer.sin_addr.s_addr = pack.srcIp;
				it->second->m_addrPeer.sin_port = pack.srcPort;
				TRACE("recv p2p test,port = %d\n",pack.srcPort);
			}
		}
	}

	
}

void XUDS::ProcessConnect(const XUDS_PACKET & pack)
{
	XUDS_CONNECT * pReq = (XUDS_CONNECT*)pack.data;
	XUDS_CONNECT temp;
	if(pack.len < sizeof(XUDS_CONNECT))
	{
		pReq = &temp;
		memcpy(pReq,pack.data,pack.len);
	}
	XSOCKET xs = pReq->socketId;
	bool	bAccept = false;
	int rst = XUDS_ACCEPT::DENEY;

	SOCKADDR_IN addrPeer;
	addrPeer.sin_family = AF_INET;
	addrPeer.sin_addr.s_addr = pack.srcIp;
	addrPeer.sin_port = pack.srcPort;

	TRACE("p2pAccept from ip:%d.%d.%d.%d:%d\n",
		addrPeer.sin_addr.S_un.S_un_b.s_b1,
		addrPeer.sin_addr.S_un.S_un_b.s_b2,
		addrPeer.sin_addr.S_un.S_un_b.s_b3,
		addrPeer.sin_addr.S_un.S_un_b.s_b4,
		ntohs(pack.srcPort)
		);

	// 首先判断连接是否已经存在

	XUDSocket * pS = FindSocket(pReq->socketId);

	if(pS == NULL)
	{
		// 判断是否接受该连接

		XUDSAcceptList::iterator it,itE;
		m_acceptTasks.Lock();
		itE = m_acceptTasks.end();
		for(it = m_acceptTasks.begin();it != itE;it++)
		{
			if((*it)->friend_id == pReq->connectId && (*it)->my_id == pReq->did)
			{
				// 接受该连接请求
				bAccept = true;
				break;
			}
		}
		m_acceptTasks.Unlock();

		// 建立套接字
		if(bAccept)
		{
			// 判断是否需要再登陆中转服务器
			if((*it)->need_tran_logon == false)
			{
				XUDSocket * pNew = new XUDSocket(xs,pReq->sendWndSize,pReq->sendBufSize);
				pNew->m_state = XUDSocket::CONNECTED;
				(*it)->xs = xs;
				pNew->Init(m_s,addrPeer,pReq->connectId);
				m_sockets[xs] = pNew;
				// 通知连接已经建立事件
				(*it)->event.Set();
				rst = XUDS_ACCEPT::ACCEPT;
			}
			else
			{
				// 还没有登陆中转服务器
				rst = XUDS_ACCEPT::WAIT;
			}
		}
		else
		{
			rst = XUDS_ACCEPT::DENEY;
		}
	}
	else
	{
		// 连接已经存在(不过这里有可能出现不同连接者使用相同套接字的情况!!)
		bAccept = true;
		rst = XUDS_ACCEPT::ACCEPT;
	}

	// 返回应答

	XUDS_ACCEPT ack;
	ack.rt = rst;
	ack.socketId = pReq->socketId;
	ack.did = pReq->connectId;
	
	sendto(m_s,(char*)&ack,sizeof(ack),0,(sockaddr*)&addrPeer,sizeof(addrPeer));

}

XSOCKET XUDS::StartTranAccept(XUDSAcceptInfo * pTask,uint32 tranIp,uint16 tranPort)
{
	XSOCKET xs = -1;
	DWORD dwRst;
	XUDS_QUERY_REQ logon;

	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = tranIp;
	addr.sin_port = tranPort;

	logon.myId = pTask->my_id;
	logon.queryId = pTask->friend_id;
	logon.did = pTask->friend_id;

	// 等待登陆中转服务器成功
	int nTimes = TRAN_SERVER_CONN_TIME / TRAN_SERVER_CONN_WAIT_TIME;
	while(nTimes > 0)
	{
		// 发送登陆信息
		sendto(m_s,(char*)&logon,sizeof(logon),0,(sockaddr*)&addr,sizeof(addr));
		dwRst = WaitForSingleObject(pTask->eventTranOk,TRAN_SERVER_CONN_WAIT_TIME);
		if(dwRst == WAIT_OBJECT_0)
		{
			// 等待对方的p2p连接信息
			DWORD dwRst = WaitForSingleObject(pTask->event,P2P_CONNECT_TIME);
			if(dwRst == WAIT_OBJECT_0)
			{
				// 监听成功
				xs = pTask->xs;
			}
			break;
		}
		nTimes--;
	}
	
	return xs;
}


// 10 秒超时
XSOCKET XUDS::Accept(uint32 myId,uint32 friendId,uint32 ip,uint16 port,uint32 tranIp,uint16 tranPort)
{
	XSOCKET xs = -1;
	DWORD dwRst ;
	bool bOk = false;
	XUDSAcceptInfo * pTask = new XUDSAcceptInfo();
	pTask->my_id = myId;
	pTask->friend_id = friendId;
	pTask->xs = -1;
	m_acceptTasks.Lock();
	m_acceptTasks.push_back(pTask);
	m_acceptTasks.Unlock();


	SOCKADDR_IN addrPeer;
	addrPeer.sin_family = AF_INET;
	addrPeer.sin_addr.s_addr = ip;
	addrPeer.sin_port = port;

	

	int nRet = 0;
	XUDS_P2P_TEST test;
	test.testId = myId;
	int nC = P2P_CONNECT_TIME / P2P_CONNECT_WAIT_TIME;
	int i = 0;
	for(;i < nC;i++)
	{
		// 发送p2pTest
		nRet = sendto(m_s,(char*)&test,sizeof(test),0,(sockaddr*)&addrPeer,sizeof(addrPeer));
		TRACE("p2p test :%d.%d.%d.%d:%d\n",
			addrPeer.sin_addr.S_un.S_un_b.s_b1,
			addrPeer.sin_addr.S_un.S_un_b.s_b2,
			addrPeer.sin_addr.S_un.S_un_b.s_b3,
			addrPeer.sin_addr.S_un.S_un_b.s_b4,
			ntohs(port)
			);
		// 等待连接到达的通知
		dwRst = WaitForSingleObject(pTask->event,P2P_CONNECT_WAIT_TIME);
		if(dwRst == WAIT_OBJECT_0)
		{
			// 监听成功
			TRACE("p2p监听成功\n");
			xs = pTask->xs;
			bOk = true;
			break;
		}
		else
		{

		}
	}
	if(bOk == false)
	{
		if(tranIp)
		{
			TRACE("p2p监听失败，开始尝试服务器中转监听\n");
			// 失败,尝试服务器中转
			pTask->need_tran_logon = true;
			xs = StartTranAccept(pTask,tranIp,tranPort);	
		}
		else
		{
			TRACE("p2p监听失败，未提供服务器中转\n");
		}
	}
	// 清除一个监听任务
	CloseAccept(myId,friendId);
	return xs;
}

void XUDS::CloseAccept(uint32 myId,uint32 friendId)
{
	XUDSAcceptList::iterator it,itE;
	m_acceptTasks.Lock();
	itE = m_acceptTasks.end();
	for(it = m_acceptTasks.begin();it != itE;it++)
	{
		if((*it)->friend_id == friendId && (*it)->my_id == myId)
		{
			delete (*it);
			m_acceptTasks.erase(it);
			break;
		}
	}
	m_acceptTasks.Unlock();
}

int XUDS::StartTranConnect(XUDSocket *pS,uint32 myId,uint32 friendId,uint32 tranIp,uint16 tranPort)
{
	DWORD dwRst = 0;
	int nRet;
	SOCKADDR_IN addrPeer;

	addrPeer.sin_family = AF_INET;
	addrPeer.sin_addr.s_addr = tranIp;
	addrPeer.sin_port = tranPort;

	XUDS_QUERY_REQ logon;

	logon.socketId = pS->GetSocketId();
	logon.myId = myId;
	logon.queryId = friendId;

	// 等待n秒时间

	int nRetryTimes = TRAN_SERVER_CONN_TIME / TRAN_SERVER_CONN_WAIT_TIME;
	int nSend = 0;
	while(nRetryTimes > 0)
	{
		// 发送中转连接查询请求
		nSend = sendto(m_s,(char*)&logon,sizeof(logon),0,(sockaddr*)&addrPeer,sizeof(addrPeer));
		dwRst = ::WaitForSingleObject(pS->m_eventTranOk,TRAN_SERVER_CONN_WAIT_TIME);
		if(dwRst == WAIT_OBJECT_0)
		{
			// 登陆中转服务器成功,将通讯地址改为通过中转服务器路由
			pS->m_bTranServer = true;
			pS->m_addrPeer = addrPeer;

			// 开始中转方式的连接
			XUDS_CONNECT conn;
			conn.socketId = pS->GetSocketId();
			conn.did = friendId;
			conn.connectId = myId;
			conn.sendBufSize = pS->GetSendBufSize();
			conn.sendWndSize = pS->GetSendWndSize();

			// 重试n次连接
			int nTimes = P2P_CONNECT_TIME / P2P_CONNECT_WAIT_TIME;
			while(nTimes > 0)
			{
				// 发送连接请求
				nRet = sendto(m_s,(char*)&conn,sizeof(conn),0,(sockaddr*)&addrPeer,sizeof(addrPeer));
				dwRst = ::WaitForSingleObject(pS->m_eventConnect,P2P_CONNECT_WAIT_TIME);
				if(dwRst == WAIT_OBJECT_0)
				{
					// 连接成功
					return 0;
				}	
				nTimes--;	
			}
			return -1;
		}
		nRetryTimes--;
	}
	return -1;
}


int XUDS::Connect(XSOCKET s,uint32 myId,uint32 friendId,uint32 ip,uint16 port,uint32 tranIp,uint16 tranPort)
{
#ifdef _TEST_TRAN_SERVER
	port++;
#endif

	int nRt = 0;
	XUDSocket * pS = FindSocket(s);
	if(pS == NULL)
	{
		return -1;
	}

	int nRet;
	SOCKADDR_IN addrPeer;
	addrPeer.sin_family = AF_INET;
	addrPeer.sin_addr.s_addr = ip;
	addrPeer.sin_port = port;
	pS->Init(m_s,addrPeer,friendId);
	
	
	DWORD dwRst = -1;
	XUDS_CONNECT conn;
	conn.socketId = s;
	conn.did = friendId;
	conn.connectId = myId;
	conn.sendBufSize = pS->GetSendBufSize();
	conn.sendWndSize = pS->GetSendWndSize();

	int nRetryTimes = P2P_CONNECT_TIME / P2P_CONNECT_WAIT_TIME;
	while(nRetryTimes > 0)
	{
		// 发送连接请求
		nRet = sendto(m_s,(char*)&conn,sizeof(conn),0,(sockaddr*)&pS->m_addrPeer,sizeof(addrPeer));
		TRACE("p2pConn ip:%d.%d.%d.%d:%d\n",
			pS->m_addrPeer.sin_addr.S_un.S_un_b.s_b1,
			pS->m_addrPeer.sin_addr.S_un.S_un_b.s_b2,
			pS->m_addrPeer.sin_addr.S_un.S_un_b.s_b3,
			pS->m_addrPeer.sin_addr.S_un.S_un_b.s_b4,
			ntohs(pS->m_addrPeer.sin_port)
			);
		dwRst = ::WaitForSingleObject(pS->m_eventConnect,P2P_CONNECT_WAIT_TIME);
		if(dwRst == WAIT_OBJECT_0)
		{
			break;
		}	
		nRetryTimes--;	
	}

	if(dwRst != WAIT_OBJECT_0)
	{
		if(tranIp != 0)
		{
			TRACE("p2p连接失败，开始尝试中转服务器连接\n");
			// p2p连接失败，开始尝试中转服务器连接
			return StartTranConnect(pS,myId,friendId,tranIp,tranPort);			
		}
		else
		{
			TRACE("p2p连接失败，未提供中转服务器地址\n");
			return -1;
		}
	}
	if(pS->m_state == XUDSocket::CLOSED)
	{
		return -2;			// 对方拒绝连接
	}
	TRACE("p2p连接成功\n");
	return 0;
}


void XUDS::ClearGarbage()
{
	NouseSocketList::iterator it,itE;
	itE = m_noUseSocket.end();
	int nCur = GetTickCount();
	for(it = m_noUseSocket.begin();it != itE;)
	{
		if((nCur - (*it)._tick) > 1000)
		{
			delete (*it)._pS;
			it = m_noUseSocket.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void XUDS::CloseSocket(XSOCKET s)
{
	m_sockets.Lock();
	XSocketMap::iterator it;
	it = m_sockets.find(s);
	if(it == m_sockets.end())
	{
		m_sockets.Unlock();
		return;
	}
	XUDSocket * pS = it->second;
	m_sockets.erase(it);
	m_sockets.Unlock();
	
	if(pS)
	{
		pS->Close();
		NouseSocket nskt;
		nskt._pS = pS;
		nskt._tick = GetTickCount();
		m_noUseSocket.push_back(nskt);
	}
}

int XUDS::GetSuccSend(XSOCKET s)
{
	XUDSocket * pS = FindSocket(s);
	if(pS == NULL)
		return 0;
	return pS->GetSuccSend();
}

void XUDS::CloseWait(XSOCKET s)
{
	XUDSocket * pS = FindSocket(s);
	if(pS == NULL)
		return;
	pS->SendWait();
#ifdef _MyDebug
	pS->DebugWriteInfo("log.txt");
#endif
	CloseSocket(s);
}