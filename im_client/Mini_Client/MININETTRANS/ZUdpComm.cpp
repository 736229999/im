#include "StdAfx.h"
#include ".\zudpcomm.h"
//#include "fcl.h"


//using namespace fcl;

#pragma comment(lib,"Ws2_32.lib")

#pragma comment(lib,"Winmm.lib")

#ifdef _DEBUG
//#define _DEBUG_P2P
#endif

#define ACKTIMEOUT				(50 * 1000) // ack应答超时时间 ms
	
#define TIME_RTT				1500			//*1000 目前还是静态时间，需要改善
#define TIME_DELETE_RECV_POOL	3000		// 垃圾删除的时间
#define TIME_DELETE_SEND_POOL	3000	
#define SEND_RETRY_TIMES		300			// 发送包的重传次数

#define TIME_RESEND_CHECK		200			// 200 ms
#define TIME_REQ_CONTROL		6000	

const char SZ_VERSION[] = "1.02";
//#define _DEBUG_INFO



//CNetUnited CZUdpComm::m_united; 




//int CZUdpComm::bind(NETSOCKET u,sockaddr_in* addr,int* addrlen)
//{
//	return m_united.bind(u,addr,addrlen);
//}
//
//NETSOCKET   CZUdpComm::socket(int wndSize /* = SOCK_WNDSIZE */)
//{
//	return m_united.generateSocket(wndSize);
//}
//bool        CZUdpComm::queryFriend(sockaddr_in* addr,uint32 selfId,uint32 friendId)
//{
//	return m_united.queryFriend(addr,selfId,friendId);
//}
//NETSOCKET	CZUdpComm::accept(NETSOCKET u,sockaddr_in* addr, int* addrlen)
//{
//	return m_united.accept(u,addr,addrlen);
//}
//
//int	CZUdpComm::connect(NETSOCKET u,sockaddr_in* addr, int* addrlen,uint32 selfId /* = 0 */,uint32 friendId /* = 0 */)
//{
//	return m_united.connect(u,addr,addrlen,selfId,friendId);
//}
//
//int	CZUdpComm::send(NETSOCKET u,char* buf,int len)
//{
//	return  m_united.send(u,buf,len);
//}
//
//int	CZUdpComm::recv(NETSOCKET u,char * buf,int nLen)
//{
//	return m_united.recv(u,buf,nLen);
//}
//
//void CZUdpComm::close(NETSOCKET u)
//{
//	m_united.close(u);
//}
//bool CZUdpComm::p2pTest(uint32 fId,uint32 Ip,uint16 Port,uint32 timeout /* = 5000 */)
//{
//	return m_united.p2pTest(fId,Ip,Port,timeout); //默认5秒钟超时
//}

void CZUdpComm::setTransServer(bool val)
{
	m_isTransServer = val;
}
void RecvQueueCallback(void * pUserKey,const char * data,int size)
{
	CZUdpComm* pObj = (CZUdpComm*)pUserKey;
	pObj->PostQueueData(data,size);
}

void SendQueueCallback(void * pUserKey,const char * data,int size)
{
	CZUdpComm* pObj = (CZUdpComm*)pUserKey;
	pObj->SendQueueData(data,size);
}



//DWORD WINAPI CZUdpComm::HandleP2PThreadFun(LPVOID lpParam)
//{
//	CZUdpComm * pObj = (CZUdpComm *)lpParam;
//	pObj->HandleP2pPacket();
//	return 0;
//}

DWORD WINAPI CZUdpComm::WrokNetRecvThreadFun(LPVOID lpParam)
{
	CZUdpComm* pObj = (CZUdpComm*)lpParam;
	pObj->RecvNetData();
	return 0;
}

CZUdpComm::CZUdpComm(int nConsumeThreadCount /* = 1 */ , int nQuereCount /*= 1000*/, int nBufferSize /*= 1600*/) : 
		   m_RecvQueue(nConsumeThreadCount,nQuereCount,nBufferSize),m_SendQueue(1,nQuereCount,nBufferSize)
{

	//m_hP2PThread = NULL; //
	m_hNetRecvThread = NULL;   //处理接收网络数据线程
	m_hSemUsableSIO = NULL;    //可用的发送I/O信号量
	m_hQuitEvent = NULL;      //服务停止事件
	m_hRecvOverlapped = NULL;

	m_hTimerResend = 0;
	m_hTimerReqControl = 0;

	m_pUserKey = NULL;
	m_pfnUserCallback = NULL;

	m_bWork = false;


	m_nRecvPacketNum = 0 ;

	//memset(m_bSendEvents,0,sizeof(m_bSendEvents));

	//srand(GetTickCount());
	//m_nSessionIDSeq = rand();
   // m_united.m_pChannel = &m_channel;
	m_isTransServer = false;
	//预先生成缓冲区
	/*m_bufferQueue.m_mutex.do_lock();
	for(uint32 i = 0;i < m_bufferQueueSize; ++i)
	     m_bufferQueue.push_back(new char[MAX_NETWORK_PACKET]);
	m_bufferQueue.m_mutex.do_unlock();*/
	m_nBufferSize = nBufferSize - 1;
}


CZUdpComm::~CZUdpComm(void)
{
	/*CP2PAcceptInfoList::iterator it,itE;
	itE = m_acceptInfo.end();
	for(it = m_acceptInfo.begin();it != itE;it++)
	{
		delete (*it);
	}*/
    StopListen(); //停止所有的线程
	//删除所有的p2p队列数据块
	//m_united.m_dataQueue.m_mutex.do_lock();
	//fclList<DataBlock>::iterator itr = m_united.m_dataQueue.begin();
	//for(;itr != m_united.m_dataQueue.end();++itr)
	//	delete[] (*itr).data;
	//m_united.m_dataQueue.clear();
	//m_united.m_dataQueue.m_mutex.do_lock();
	////删除所有的缓冲区数据块
	//m_bufferQueue.m_mutex.do_lock();
	//while(!m_bufferQueue.empty())
	//{
	//	delete[] m_bufferQueue.front();
	//	m_bufferQueue.pop_front();
	//}
	//m_bufferQueue.m_mutex.do_unlock();

}


//接收队列回调处理函数
void CZUdpComm::PostQueueData(const char * data,int size)
{
	RecvDataStruct * pRecvDataST = new RecvDataStruct();
	memcpy(pRecvDataST, data, size);
	LPXT_HEAD pHead = (LPXT_HEAD)pRecvDataST->data;
	m_pfnUserCallback(m_pUserKey,NE_RECVDATA,pRecvDataST);
	delete pRecvDataST;
	pRecvDataST = NULL;
}

//#define _OVER_IO
int CZUdpComm::SendUdpData(const char* buf,uint32 size,uint32 destip,uint16 port)
{
	DWORD dwNumBytesSent = 0;
	DWORD dwFlags = 0;
	DWORD dwRet = 0;
	DWORD dwLen = 0;
	int	nRet = 0;
    
	sockaddr_in addrto = {0};
	addrto.sin_family = AF_INET;
	addrto.sin_addr.s_addr = destip;
	addrto.sin_port = htons(port);
   
	/*nRet = sendto(m_sockListen,lpWsaBuf->buf,lpWsaBuf->len,dwFlags,
		(struct sockaddr *)&addrto,sizeof(addrto));*/
	nRet = m_channel.sendto(&addrto,const_cast<char*>(buf),size);
	if(nRet < 0)
		return -1;

	return 0;
}
//发送队列回调处理函数
void CZUdpComm::SendQueueData(const char * data,int size)
{
	SendDataStruct * pSendDataST = new SendDataStruct();
	memcpy(pSendDataST, data, size);
//	WSABUF wsaBuf;
//	wsaBuf.buf = (char *)&pSendDataST->vlink;
//	wsaBuf.len = sizeof(pSendDataST->vlink) + pSendDataST->data_len;
//#ifdef _DEBUG
//	assert(pSendDataST->data_len > 0);
//#endif
    int sndSize = sizeof(pSendDataST->vlink) + pSendDataST->data_len;
	int nRet = SendUdpData((char*)&pSendDataST->vlink,sndSize,pSendDataST->dest_ip,pSendDataST->port);
	if(nRet < 0)
	{
		//error occur
		m_pfnUserCallback(m_pUserKey,NE_SENDFAILED,(RecvDataStruct *)pSendDataST); 
		delete pSendDataST;
		pSendDataST = NULL;
		return ;
	}
	else
	{
		m_pfnUserCallback(m_pUserKey,NE_SENDOK,(RecvDataStruct *)pSendDataST);
		
	}
	delete pSendDataST;
	pSendDataST = NULL;
}

////线程回调函数,负责循环发送所有逻辑连接中滑动窗口中的数据
//void CZUdpComm::HandleP2pPacket()
//{
//	DataBlock block;
//	uint64 beg,interval;
//	while(m_bWork)
//	{
////	    fclDispatchTrace();
//		if(!m_united.m_dataQueue.empty())
//		{
//			block = m_united.m_dataQueue.front();
//			m_united.recvP2pPacket(block.srcIp,block.srcPort,block.data,block.len);
//			m_united.m_dataQueue.pop_front();
//			//delete[] block.data;
//			m_bufferQueue.push_back(block.data);
//		}
//		else
//			Sleep(1);
//	   m_united.processTimer();  ///处理各种定时器函数
//	}
//	Trace("Exit HandleP2pPacket thread\n");
//}

//void CZUdpComm::RecvOverlapped()
//{
//	DWORD dwResult = 0;
//	int nRet = 0;
//	DWORD dwcbTransfer = 0;
//	DWORD dwFlags = 0;
//	RecvDataStruct recvData;	
//	LONG lPreCount = 0;
//	for(;;)
//	{
//		dwResult = WSAWaitForMultipleEvents(WSA_MAXIMUM_WAIT_EVENTS,m_RecvIO.hEvents,FALSE,WSA_INFINITE,FALSE);
//		if(dwResult == WAIT_FAILED)
//		{
//			m_pfnUserCallback(m_pUserKey,NE_LISTENSTOP,NULL); 
//			break; //error occur
//		}
//		if(dwResult == WAIT_OBJECT_0 + WSA_MAXIMUM_WAIT_EVENTS - 1)
//		{
//			m_pfnUserCallback(m_pUserKey,NE_LISTENSTOP,NULL); 
//			break; //Quit Event Set
//		}
//		
//		// 获取接收数据结果
//		dwResult -= WAIT_OBJECT_0;
//		WSAGetOverlappedResult(m_sockListen,&m_RecvIO.ol[dwResult],&dwcbTransfer,TRUE,&dwFlags);
//		
//		ReleaseSemaphore(m_hSemUsableRIO,1,&lPreCount);
//		m_RecvIO.used[dwResult] = false;
//		if(dwcbTransfer <= 0)
//			continue;
//		//根据包固定标志过滤非法包???
//		memcpy(&recvData.vlink,&m_RecvIO.buf[dwResult],sizeof(recvData.vlink));
//		recvData.data_len = uint16(dwcbTransfer - sizeof(recvData.vlink));
//		memcpy(recvData.data,m_RecvIO.buf[dwResult]+sizeof(recvData.vlink),recvData.data_len);
//
//		recvData.src_ip = m_RecvIO.peerAddr[dwResult].sin_addr.s_addr;
//		recvData.port = ntohs( m_RecvIO.peerAddr[dwResult].sin_port);
//		
//		
//		nRet = m_RecvQueue.Push((const char * )&recvData,sizeof(RecvDataStruct) - sizeof(recvData.data) + recvData.data_len);
//
//	}
//}
//

//网络接收数据线程回调函数
void CZUdpComm::RecvNetData()
{
	int nRet = 0;
	uint32 size;
	DWORD dwResult = 0;
	DWORD dwRet = 0;
	int nAddrLen = sizeof(SOCKADDR_IN);
	DWORD dwRecvBytes = 0;
	DWORD dwFlags = 0;
	char* pBuf = NULL;
	//DataBlock dataBlock;
	//int idx;
	RecvDataStruct recvData;
	XUDS_PACKET packet;
	//m_RecvIO.hEvents[WSA_MAXIMUM_WAIT_EVENTS-1] = m_hQuitEvent;
	//WSABUF wsaBuf;

	for(;;)
	{
		//nRet = recvfrom( m_sockListen,
		//	(char*)(&recvData.vlink),
		//	MAX_NETWORK_PACKET + 4,
		//	dwFlags,
		//	(struct sockaddr *)&m_peerAddr,
		//	&nAddrLen
		//);
		//if( nRet == SOCKET_ERROR )
		//{
		//	if(WSAGetLastError() == WSAESHUTDOWN )
		//	{
		//		//error occur
		//		m_pfnUserCallback(m_pUserKey,NE_LISTENSTOP,NULL); 
		//		Error("CZUdpComm::RecvNetData(),socket 连接出错!\n");
		//		break;
		//	}
		//}
		nRet = m_channel.recvfrom(&m_peerAddr,(char*)&recvData.vlink,m_nBufferSize + 4);
		//if(nRet < 0)
		//{
		//	m_pfnUserCallback(m_pUserKey,NE_LISTENSTOP,NULL); 
		//	Error("CZUdpComm::RecvNetData(),socket 连接出错!\n");
		//	break;
		//}
		//if(nRet <= 0)
		//{
		if(nRet <= 0)
			continue;

		recvData.data_len = (uint16)(nRet - sizeof(recvData.vlink));
		recvData.src_ip = m_peerAddr.sin_addr.s_addr; 
		recvData.port = ntohs( m_peerAddr.sin_port);

		//P2P_HEAD * pHead = (P2P_HEAD *)&recvData.vlink;
			XUDS_HEAD * pUds = (XUDS_HEAD *)&recvData.vlink;
		if(pUds->flag == UDS_FLAG)
		{
			if(m_isTransServer)
			{
				m_dataProc.ProcessTran(&m_channel,m_peerAddr.sin_addr.S_un.S_addr,m_peerAddr.sin_port,(char*)pUds,nRet);
			}
			else
			{
					memcpy(packet.data,&recvData.vlink,nRet);
				packet.len = nRet;
				m_uds.RecvNetData(packet,m_peerAddr);

			}
			continue;
		}
		//else if(pHead->flag == 0xf4 || pHead->flag == 0xf5)
		//{
		//	//ProcRecvStreamData(&recvData);
		//	if(m_isTransServer)
		//	{//中转服务器模式
		//		m_dataProc.processPacket(&m_channel,m_peerAddr.sin_addr.S_un.S_addr,m_peerAddr.sin_port,(char*)pHead,nRet);
		//		continue;
		//	}
		//	if(!m_bufferQueue.empty())
		//	{
		//	  pBuf = m_bufferQueue.front();
		//	  m_bufferQueue.pop_front();
		//	}
		//	else
		//	{
		//		//fclAssert(false);
		//		pBuf = new char[MAX_NETWORK_PACKET];
		//	}
		//	size = nRet;
		//	::CopyMemory(pBuf,pHead,size);
		//	dataBlock.len = size;
		//	dataBlock.data = pBuf;
		//	dataBlock.srcIp = recvData.src_ip;
		//	dataBlock.srcPort = m_peerAddr.sin_port;
		//	m_united.m_dataQueue.push_back(dataBlock);
		//	if(m_united.m_dataQueue.size() > 1000)
		//	{
		//		//Trace("m_united.m_dataQueue size:%d\n",m_united.m_dataQueue.size());
		//	}
		//	//m_united.recvP2pPacket(recvData.src_ip,m_peerAddr.sin_port,(char*)pHead,size);
		//	continue;
		//}
			nRet = m_RecvQueue.Push((const char * )&recvData,sizeof(RecvDataStruct) - sizeof(recvData.data) + recvData.data_len);
		if(nRet < 0)
		{
			// 接收缓冲溢出
			TRACE("CZUdpComm::RecvNetData,接收缓冲区溢出!\n");
			m_pfnUserCallback(m_pUserKey,NE_RECVCONGEST,&recvData);
		}
		//XT_HEAD* phead = (XT_HEAD*)recvData.data;
		//printf("--接收包cmd:0x%.4x ,%d 字节--\n",phead->cmd,recvData.data_len);

		++m_nRecvPacketNum;
	}

}

//void  WINAPI CZUdpComm::ReSendProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
//{
//	CZUdpComm * p = (CZUdpComm*)dwUser;
//	p->HandleReSendData();
//};
//
//void WINAPI CZUdpComm::MaxReqContrlProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
//{
//	CZUdpComm * p = (CZUdpComm *)dwUser;
//	p->HandleReqControl();
//}


int CZUdpComm::StartListen(const char * addr,int port)
{
	m_hTimerResend = NULL;// timeSetEvent(TIME_RESEND_CHECK,0,(LPTIMECALLBACK)ReSendProc,(DWORD_PTR)this,TIME_PERIODIC);
	m_hTimerReqControl = NULL;//timeSetEvent(TIME_REQ_CONTROL,0,MaxReqContrlProc,(DWORD_PTR)this,TIME_PERIODIC);	
	
	//初始化、监听socket
	m_bWork = false;
	int	iResult; 
	WSADATA ws; 
	iResult = WSAStartup(0x0202,&ws); 
	if(iResult)
		return -1;

	m_listenIP = inet_addr(addr);
	sockaddr_in addrBnd;
	addrBnd.sin_family = AF_INET;
	addrBnd.sin_addr.s_addr = m_listenIP;
	addrBnd.sin_port = htons(port);
     if(-1 == m_channel.open(&addrBnd))
	 {
		 //Error("CZUdpComm::StartListen,端口开启失败!\n");
		 return -1;
	 }
	 m_uds.Init(m_channel.getSocket());
	//m_sockListen = WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	//if(m_sockListen == INVALID_SOCKET)
	//	return -1;

	//int nRet = bind(m_sockListen,(struct sockaddr *)&addrBnd,sizeof(addrBnd));
	//if(nRet != 0)
	//{
	//	//trace("socket err:%d",WSAGetLastError());
	//	return -1;
	//}
	
	int namelen = sizeof(addrBnd);
	getsockname(m_sockListen,(struct sockaddr *)&addrBnd,&namelen);
	m_listenPort = ntohs(addrBnd.sin_port);

	//启动发送和接收信号队列
	m_SendQueue.SetUserCallback(this,SendQueueCallback);
	m_SendQueue.Start();

	m_RecvQueue.SetUserCallback(this,RecvQueueCallback);
	m_RecvQueue.Start();

	//启动异步发送事件和网络接收数据线程
	m_hQuitEvent = WSACreateEvent(); 
	m_hSemUsableSIO = CreateSemaphore(NULL,MAX_SENDIO_COUNT,MAX_SENDIO_COUNT,NULL);
	m_hSemUsableRIO = CreateSemaphore(NULL,20,WSA_MAXIMUM_WAIT_EVENTS-1,NULL);
	
	//m_hHandleSIOThread = CreateThread(NULL,0,HandleSendIOThreadFun,this,0,NULL);
	m_hNetRecvThread = CreateThread(NULL,0,WrokNetRecvThreadFun,this,0,NULL);
	//m_hP2PThread = CreateThread(NULL,0,HandleP2PThreadFun,this,0,NULL);
	m_bWork = true;
	return 0;
}

int CZUdpComm::SendDataTo(const SendDataStruct &sendData,int nPrior)
{
	assert(m_pfnUserCallback && "没有设置回调函数");
	if(m_bWork == false)
		return -1;
	int nRt;
	nRt = m_SendQueue.Push((const char *)&sendData,sizeof(SendDataStruct) - sizeof(sendData.data) + sendData.data_len,nPrior);
	if(nRt == -2)
	{
		//
		// 当前缓冲队列为1000个，平均每个包大小为1k，传输速度假设为10M/s,
		// 则需要约0.1s的时间才能传输完成,故等待100毫秒之内的时间
		// 
		Sleep(100);
		return m_SendQueue.Push((const char *)&sendData,sizeof(SendDataStruct) - sizeof(sendData.data) + sendData.data_len,nPrior);
	}
	return nRt;
}

void CZUdpComm::StopListen()
{
	m_uds.UnInit();

	if(m_hTimerResend)
	{
		timeKillEvent(m_hTimerResend);
		m_hTimerResend = 0;
	}
	if(m_hTimerReqControl)
	{
		timeKillEvent(m_hTimerReqControl);
		m_hTimerReqControl = 0;
	}

	m_bWork = false;
	m_RecvIO.Reset();
	m_SendIO.Reset();

	if(!m_hQuitEvent)
		return ;

	//退出工作线程
	SetEvent(m_hQuitEvent);
	DWORD dwRet = 0;

	//dwRet = WaitForSingleObject(m_hP2PThread,1000);
	//if(dwRet != WAIT_OBJECT_0)
	//	TerminateThread(m_hP2PThread,-1);
	//CloseHandle(m_hP2PThread);
	//m_hP2PThread = NULL;

	dwRet = WaitForSingleObject(m_hNetRecvThread,10);
	if(dwRet != WAIT_OBJECT_0)
		TerminateThread(m_hNetRecvThread,-1);
	CloseHandle(m_hNetRecvThread);
	m_hNetRecvThread = NULL;


	//停止队列服务
	m_SendQueue.Stop();
	m_RecvQueue.Stop();

	//关闭内核对象
	CloseHandle(m_hQuitEvent);
	m_hQuitEvent = NULL;

	/*closesocket(m_sockListen);*/
	m_channel.close();
	m_sockListen = 0;

	CloseHandle(m_hSemUsableSIO);
	CloseHandle(m_hSemUsableRIO);
	m_hSemUsableSIO = NULL;
	m_hSemUsableRIO = NULL;

	WSACleanup();
}




// 接口成员函数

//------------------------------------------------------
// 创建套接字
//------------------------------------------------------
XSOCKET		CZUdpComm::Socket(uint32 nWndSize,uint32 nSendbufSize)
{
	return m_uds.Socket(nWndSize,nSendbufSize);
}; 

//------------------------------------------------------
// 开始接受连接
//------------------------------------------------------

XSOCKET		CZUdpComm::Accept(uint32 myId,uint32 friendId,uint32 ip,uint16 port,uint32 tranIp,uint16 tranPort)
{
	return m_uds.Accept(myId,friendId,ip,port,tranIp,tranPort);
};

//------------------------------------------------------
// 发送数据（阻塞）
//------------------------------------------------------
int					CZUdpComm::Send(XSOCKET s,const char * pData,int nLen)
{
	return m_uds.Send(s,pData,nLen);
};



//------------------------------------------------------
// 接收数据（阻塞）
//------------------------------------------------------
int					CZUdpComm::Recv(XSOCKET s,char * pBuf,int nLen)
{
	return m_uds.Recv(s,pBuf,nLen);
};

int					CZUdpComm::GetSuccSend(XSOCKET s)
{
	return m_uds.GetSuccSend(s);
}

//------------------------------------------------------
// 开始连接 
//------------------------------------------------------
int					CZUdpComm::Connect(
							XSOCKET s,
							uint32 myId,
							uint32 friendId,
							uint32 ip,
							uint16 port,
							uint32 tranIp,
							uint16 tranPort
							)
{
	return m_uds.Connect(s,myId,friendId,ip,port,tranIp,tranPort);
};

//------------------------------------------------------
// 关闭套接字
//------------------------------------------------------
void				CZUdpComm::CloseSocket(XSOCKET s)
{
	return m_uds.CloseSocket(s);
};

//------------------------------------------------------
// 关闭连接，并等待数据发送完成
//------------------------------------------------------
void				CZUdpComm::CloseWait(XSOCKET s)
{
	return m_uds.CloseWait(s);
};

const char * CZUdpComm::GetVersion() const
{
	return SZ_VERSION;
}