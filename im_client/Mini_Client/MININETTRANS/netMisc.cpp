/******************************************************************
** 文件名:	NetMisc.cpp
** 版  权:	
** 创建人:	Tony
** 日  期:	2007-6-4
** 描  述:  这个文件主要是一些NetTrans用到的辅助类

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
*******************************************************************/
#include "stdafx.h"
#include "netMisc.h"
#include "ZUdpComm.h"
#include "math.h"
#include "PublicType.h"
#undef Trace
#define Trace
//#undef Error
//#define Error

#define MS(x)   (1000 * x)                    //毫秒
#define SEC(x)  (1000 * MS(x))                //秒
#define MIN(x)  (60 * SEC(x))                 //分钟

const uint32 CSeqNo::m_iSeqNoTH = 0x3FFFFFFF;
const uint32 CSeqNo::m_iMaxSeqNo = 0x7FFFFFFF;



CNetSocket::CNetSocket():m_packTime(16)
{
	m_Status = INIT;
	//初始化逻辑连接的一些属性
	m_RTT = (uint32)SEC(0.1); //0.1秒
	m_RTTVar = (uint32)SEC(0.05); //0.05秒
	m_STPInt = 100;     ///1微妙
	m_wndSize = 0;
	m_iLastDecSeq = -1;
	m_iNAKCount = 0;
	m_iDecRandom = 1;
	m_iAvgNAKNum = 1;
	m_iDecCount = 1;
	m_bLoss = false;
	m_iMSS = MAX_NETWORK_PACKET - sizeof(P2P_HEAD);
	m_ackNo = 1;
	m_broken = false;
	m_nextMsgNo = 0; //下次应该收到的消息号,用于丢包序号控制
	m_friendId = 0;
	m_selfId   = 0;
	//初始化定时器类属性
    m_RTTInt = m_ACKInt = (uint32)SEC(0.05);  //固定为0.01 秒
	/*m_last_RTT = m_last_ACK = m_Timer.getTick();*/
	m_NAKInt = 3 * m_RTT;
	m_EXPInt = 3 * m_RTT + m_ACKInt;
	m_RTTVar = 0;
	//m_last_NAK = m_last_EXP = m_Timer.getTick();
	m_expCount = 0;
	m_bandWidth = 1;
	//统计信息属性初始化
	m_totalSendPkg = 0;               //总共发送的数据包
	m_totalRecvPkg = 0;               //总共收到的数据包
	m_sndLossPkg   = 0;                 //发送方丢失的数据包数
	m_rcvLossPkg   = 0;                 //接收方丢失的数据包数
	//滑动窗口指针
	m_pRcvWindow = NULL;   //在连接成功后初始化
	m_pSndWindow = NULL;  
}
CNetSocket::~CNetSocket()
{
	delete m_pSndWindow;
	delete m_pRcvWindow;
}



CNetUnited::CNetUnited()
{
	srand((uint32)GetTickCount());
	m_socketID = 1 + (int)((1 << 30) * (rand()/(RAND_MAX + 1.0)));
	m_maxSocketID = 0x7fff;
	m_bAccept = false;
	m_acceptSocket = NULL;
	m_pChannel = NULL;
	m_isP2pTesting = false;
	m_checkBrokenSocketInt = (uint32)MIN(3);   //3分钟检查一次
	m_lastCheckBrokenSocket = m_Timer.getTick();
}

CNetUnited::~CNetUnited()
{
	threadMode::scope_lock guard(m_sockets.m_mutex);
    socketsItr itr = m_sockets.begin();
	for(;itr != m_sockets.end(); ++itr)
		delete itr->second;
	m_sockets.clear();

	threadMode::scope_lock guard1(m_queryEnt.m_mutex);
	fclHash_map<uint32,CEventWrapper*>::iterator itr1 = m_queryEnt.begin();
	for(;itr1 != m_queryEnt.end();++itr1)
		delete itr1->second;
	m_queryEnt.clear();
}

int         CNetUnited::bind(NETSOCKET u,sockaddr_in* addr,int* addrlen)
{
	return SOCKET_ERROR;
}
NETSOCKET   CNetUnited::generateSocket(int wndSize /* = SOCK_WNDSIZE */) //生成一个用于流式传输的socket
{
	CNetSocket* pSocket = new CNetSocket;
	pSocket->m_last_RTT = pSocket->m_last_ACK = m_Timer.getTick();
	pSocket->m_last_NAK = pSocket->m_last_EXP = m_Timer.getTick();
	pSocket->m_pChannel = m_pChannel;
	pSocket->m_wndSize = wndSize;

	//生成isn序号
	srand((uint32)m_Timer.getTick());
	pSocket->m_iISN = (uint32)(double(rand()) * CSeqNo::m_iMaxSeqNo / (RAND_MAX + 1.0));
	pSocket->m_iMsgNo = 0;
	threadMode::scope_lock   guard(m_mutex);
	while(1)
	{
		pSocket->m_Socket = --m_socketID;
		pSocket->m_Socket == 0?m_maxSocketID:m_socketID;

	   if(m_sockets.find(pSocket->m_Socket)!= m_sockets.end())
	   {
		   Error("Generate sockets repeat !\n");
		   continue;
	   }
	   else
		   break;
	}
	m_sockets[pSocket->m_Socket] = pSocket;
	return pSocket->m_Socket;
}

NETSOCKET	CNetUnited::accept(NETSOCKET u ,sockaddr_in* addr, int* addrlen)
{
	m_bAccept = true;
	NETSOCKET s = NULL;
	int32 res;
	if(u == NULL)
	{
		res = WaitForSingleObject(m_acceptEnt.m_handle,10000); //等待10秒钟
		if(res != WAIT_OBJECT_0)
			m_acceptSocket = SOCKET_ERROR;
		*addr = m_peerAddr;
	}
	else
	{///not support，暂时不支持

		Error("暂时不支持这个选项!\n");
	}
	m_bAccept = false;

	return m_acceptSocket;
}


int	CNetUnited::connect(NETSOCKET u,sockaddr_in* addr, int* addrlen,uint32 selfId /* = 0 */,uint32 friendId /* = 0 */)
{
	fclHash_map<NETSOCKET,CNetSocket*>::iterator itr;
	itr = m_sockets.find(u);
	if(itr == m_sockets.end())
	{
		Error("没有这个socket\n");
		return SOCKET_ERROR;
	}
    CNetSocket* pS = (*itr).second;
	pS->m_selfId = selfId;
	pS->m_friendId = friendId;
	pS->m_peerAddr.sin_family = AF_INET;
	pS->m_peerAddr.sin_port = addr->sin_port;
	pS->m_peerAddr.sin_addr.S_un.S_addr = addr->sin_addr.S_un.S_addr;
	P2P_HEAD head;
	HandShake hs;
	obuffer2048 oBuf;
	head.cmd = CMD_P2P_CONNECT;
	head.flag = 0xF4;
	head.len = sizeof(HandShake);
	head.fid = friendId;   //服务器中转使用
	head.m_isn = pS->m_iISN;
	head.m_msgId = 0;
	head.m_sId = pS->m_Socket;
	hs.isn = pS->m_iISN;
	hs.mss = pS->m_iMSS;
	hs.selfId = selfId;
	hs.socketId = pS->m_Socket;
	hs.wndSize = pS->m_wndSize;
	oBuf << head;
	oBuf << hs;
	pS->m_pChannel->sendto((sockaddr_in*)&pS->m_peerAddr,(char*)oBuf.buffer(),(uint32)oBuf.size());
	uint32 count = 0;
	int32 res;
	while(count++ < 4)//发送4次握手协议
	{
		res = WaitForSingleObject(pS->m_connectEnt.m_handle,1500);  //等待1秒
		if(res != WAIT_OBJECT_0)
			pS->m_pChannel->sendto((sockaddr_in*)&pS->m_peerAddr,(char*)oBuf.buffer(),(uint32)oBuf.size());
		else 
			break;
	}
	if(res != WAIT_OBJECT_0)
		return SOCKET_ERROR; //连接失败
	else
	{//连接成功

		return  0;  
	}
}

int	CNetUnited::send(NETSOCKET u,char* buf,int len)
{
	PROFILE("CNetUnited::send");
	fclHash_map<NETSOCKET,CNetSocket*>::iterator itr;
	CNetSocket* pS;
	if(len <= 0)
		return len;
	itr = m_sockets.find(u);
	if(itr != m_sockets.end())
       pS = (*itr).second;
	else
		return SOCKET_ERROR;
    if(pS->m_Status == CLOSED && pS->m_broken)
	{
		Error("connect is broken!\n");
		return SOCKET_ERROR;
	}
	//char* pBuf = NULL;
	uint32 pos = 0,blockLen = 0;
	threadMode::scope_lock  guard(m_mutex); //区域锁 
	char sndBuf[MAX_NETWORK_PACKET];
	P2P_HEAD head;
	head.cmd = CMD_P2P_DATA;
	head.flag = 0xF4;
	uint64 tmp,inter;
    while(pos < (uint32)len)
	{
		sendLossPacket(pS); //如果有丢失的包，则先发送
		tmp = m_Timer.getTick();   //开始时间
      //  uint32 sp = pS->m_pSndWindow->m_windowSize - pS->m_pSndWindow->getSndSpace();
		//Trace("sp is:%d\n",sp);
		if(pS->m_pSndWindow->getSndSpace() == 0)   
		{//滑动窗口已经满，阻塞
			Sleep(1);
			Trace("sleep 1ms\n");
			if(pS->m_Status == CLOSED || pS->m_broken)
				return SOCKET_ERROR; //在这期间如果连接断开，则返回失败
            
			continue;
		}
        if(pos + pS->m_iMSS < (uint32)len)
			blockLen = pS->m_iMSS;
		else
			blockLen = len - pos;
        //
		//pBuf = new char[blockLen];
		//::CopyMemory(pBuf,buf+pos,blockLen);
		//pos += blockLen;
		//发送这个数据块
        head.len = blockLen;
		head.m_isn = pS->m_iISN;
		head.m_msgId = pS->m_iMsgNo;
		head.m_sId = pS->m_PeerID;
		head.fid   = pS->m_friendId;
		memcpy(sndBuf,&head,sizeof(head));
		//oBuf.push(pBuf,blockLen);
		memcpy(sndBuf+sizeof(head),buf+pos,blockLen);
		P2P_HEAD* pH = (P2P_HEAD*)sndBuf;
		Trace("send packet,isn:%d,msgNo%d,length:%d\n",pH->m_isn,pH->m_msgId,pH->len);
		pS->m_pChannel->sendto((sockaddr_in*)&pS->m_peerAddr,(char*)sndBuf,sizeof(head)+blockLen);
		pS->m_totalSendPkg++;
		//记录包的发送时间
		//pS->m_packTime.onPktSent(m_Timer.getTick());
        inter = m_Timer.getTick() - tmp;
		//加入滑动窗口
		pS->m_pSndWindow->addBlock(buf+pos,blockLen,pS->m_iISN,pS->m_iMsgNo);
		pos += blockLen;
		pS->m_iISN = CSeqNo::incseq(pS->m_iISN);
		++pS->m_iMsgNo;
		Error("Send :%d\n",blockLen);
		//等待stp时间
		if(blockLen < 1000)
		{
			if((pS->m_STPInt)/2 > inter)
		      sleep((pS->m_STPInt)/2 - inter);
		}
		else
		{
			if(pS->m_STPInt > inter)
				sleep(pS->m_STPInt - inter);
		}

		//Trace("send a Block\n");
	}
	return pos;
}

int	CNetUnited::recv(NETSOCKET u,char * buf,int nLen)
{
	PROFILE("CNetUnited::recv");
	fclHash_map<NETSOCKET,CNetSocket*>::iterator itr;
	itr = m_sockets.find(u);
	if(itr == m_sockets.end())
	{
		Error("Socket no exist!\n");
		return SOCKET_ERROR;
	}
	CNetSocket* pS = (*itr).second;
	if(pS->m_broken && pS->m_Status == CLOSED) 
		return SOCKET_ERROR;

	while(pS->m_pRcvWindow->getRecvQueueLen() == 0)
	{
		if(pS->m_Status == CLOSED || pS->m_broken)
		{
			Trace("connect is broken!\n");
			return SOCKET_ERROR;
		}
		Sleep(1);  ///阻塞，等待有数据到达
	}
	return pS->m_pRcvWindow->readData(buf,nLen);
}

void CNetUnited::close(NETSOCKET u)
{
	//关闭一个连接，只发送一次，如果丢包，对方会根据exp定时器超时关闭
	P2P_HEAD head;
	fclHash_map<NETSOCKET,CNetSocket*>::iterator itr;
	itr = m_sockets.find(u);
	if(itr == m_sockets.end())
	{
		Error("Socket no exist!\n");
		return;
	}
	CNetSocket* pS = (*itr).second;
	pS->m_broken = true;
	pS->m_Status = CLOSED;
	pS->m_TimeStamp = m_Timer.getTick();
	//向对方发送关闭socket连接包
	head.cmd = CMD_P2P_CLOSE;
	head.flag = 0xF4;
	head.fid = pS->m_friendId;
	head.len = 0;
	head.m_sId = pS->m_PeerID;
	pS->m_pChannel->sendto((sockaddr_in*)&pS->m_peerAddr,(char*)&head,sizeof(head));
}
void CNetUnited::recvP2pPacket(const uint32& srcIp,const uint16& srcPort,char* buf,uint32& size)
{
    HandShake* pHandShak;
	CNetSocket* s = NULL;
	AckStruct* pAck = NULL;
	uint32 len,msgNo;
	P2P_HEAD* pHead = (P2P_HEAD*)buf;
	if(pHead->cmd != CMD_P2P_CONNECT && pHead->cmd != CMD_P2P_TEST_REQ\
		&& pHead->cmd != CMD_P2P_TEST_ACK && pHead->cmd != CMD_QUERY_ACK)
	{
		if(m_sockets.find(pHead->m_sId) == m_sockets.end())
		{
			Error("未找到对应的逻辑socket对象,socket id is:%d",pHead->cmd);
			return ;
		}

		s = m_sockets[pHead->m_sId];
		//对收到的每个包都要进行的通用操作
		s->m_expCount =1;  //复位expCount计数器
		//s->m_EXPInt = s->m_RTT + 4 * s->m_RTTVar + s->m_ACKInt; //计算新的EXP周期
		//如果s的所有包都被应答，则复位exp定时器
		if(pHead->cmd != CMD_P2P_ACCEPT)
		   if(s->m_pSndWindow->isEmpty())
		    	s->m_last_EXP = m_Timer.getTick();

			s->m_packTime.onPktArrival(pHead->m_msgId);

	}
	switch(pHead->cmd)
	{
	case CMD_P2P_DATA:     //数据包
		processData(s,buf,size);
		break;
	case CMD_P2P_CONNECT:  //请求连接
		pHandShak = (HandShake*)pHead->data;
		processHandShake(srcIp,srcPort,pHandShak);
		break;
	case CMD_P2P_ACCEPT:
		processConnectACK(s,buf,size);
		break;
	case CMD_P2P_ACK: //ACK
		//pAck = (AckStruct*)pHead->data;
		processACK(s,buf,size);
		break;
	case CMD_P2P_NAK:  //NAK
        len = pHead->len/sizeof(uint32);
		processNAK(s,(uint32*)pHead->data,len);
		break;
	case CMD_P2P_ACK2:  //ACK2
		msgNo = *(uint32*)pHead->data;
		processACK2(s,msgNo);
		break;
	case CMD_P2P_CLOSE:
		processClose((NETSOCKET)pHead->m_sId);
		break;
	case CMD_P2P_TEST_REQ:
	case CMD_P2P_TEST_ACK:
		processP2PTest(pHead->cmd,pHead->fid,srcIp,srcPort,pHead->data,pHead->len);
		break;
	case CMD_QUERY_ACK:
		processQueryRes(pHead->data,pHead->len);
		break;
	case CMD_P2P_KEEP_LIVE:  //保活
		break;
	default:
		Error("Recv a unknow type packet!\n");
	}
}
void CNetUnited::processTimer()
{//检查所有的定时器，如果到期则进行响应处理
	PROFILE("CNetUnited::processTimer()");//性能评测

	threadMode::scope_lock guard(m_sockets.m_mutex);
    CNetSocket* s;
	socketsItr itr = m_sockets.begin();
	uint64 curTime = m_Timer.getTick();
	for(;itr != m_sockets.end() && m_sockets.size() > 0; ++itr)
	{
		s = (*itr).second;
		if(s->m_Status == CLOSED || s->m_broken || s->m_Status == INIT)
			continue;
		if(curTime - s->m_last_RTT > s->m_RTTInt)
		{
			//timerRC(s);
			sendLossPacket(s);//发送丢失的包
			curTime = s->m_last_RTT = m_Timer.getTick();
		}
		if(curTime - s->m_last_ACK > s->m_ACKInt)
		{
		/*	timerACK(s);
			curTime = s->m_last_ACK = m_Timer.getTick();*/
		}
		if(curTime - s->m_last_NAK > s->m_NAKInt)
		{
			timerNAK(s);
			curTime = s->m_last_NAK = m_Timer.getTick();
		}
		if(curTime - s->m_last_EXP > s->m_EXPInt)
		{
			timerEXP(s);
			curTime = s->m_last_EXP = m_Timer.getTick();
		}
	}
	//检测所有的socket，如果有关闭超过3分钟的，删除它
       if(curTime - m_lastCheckBrokenSocket > m_checkBrokenSocketInt)
	   {
		   checkBrokenSocket();
		   curTime = m_lastCheckBrokenSocket = m_Timer.getTick();
	   }
	////////////////////////////////////////////////////
}
void CNetUnited::timerRC(CNetSocket* s)
{
	PROFILE("CNetUnited::timerRC");

//	if(s->m_pRcvWindow->isEmpty() && s->m_rcvLossList.empty()) return;
    uint32 beg,end;
	s->m_pRcvWindow->getWndRange(beg,end);
	uint32 ackNo = beg;
	//如果有丢包，直接返回
	if(s->m_bLoss)
	{
		Trace("有丢包，返回\n");
		s->m_bLoss = false;
		return;
	}
   //如果在上一个timerRC内没有收到ACK，直接返回
	if(s->m_sndAckHistoryList.size() > 0)
      if((*s->m_sndAckHistoryList.end()).timeStamp + s->m_RTTInt < (uint64)m_Timer.getTick())
		  return;
   //计算新的STP发包间隔时间
  
   //s->m_STPInt =(uint32)(s->m_STPInt * 7 + s->m_bandWidth); 
	//uint32 stp = SEC(1)/s->m_bandWidth;
	uint32 stp = s->m_STPInt >> 5;   //减少1/16时间
	//s->m_STPInt = ((stp)>>1) + ((s->m_STPInt)>>1);
	s->m_STPInt -= stp;
   Error("New STP:%d\n",s->m_STPInt);
   //不能小于系统的最小发包间隔
  /* if (s->m_STPInt < (uint64)(m_Timer.getFrequency() * s->m_packTime.getMinPktSndInt() * 0.9))
	   s->m_STPInt = (uint64)(m_Timer.getFrequency() * s->m_packTime.getMinPktSndInt() * 0.9);*/
   if(s->m_STPInt < 1) 
	   s->m_STPInt =1;
   Trace("STPInt is:%d\n",s->m_STPInt);
   //发送ACK到对方
   obuffer2048 oBuf;
   P2P_HEAD head;
   AckStruct ackSt;
   head.cmd = CMD_P2P_ACK;
   head.flag = 0xF4;
   s->m_ackNo = CSeqNo::incseq(s->m_ackNo);
   head.m_isn =   s->m_ackNo;
   head.m_msgId = s->m_ackNo;
   head.m_sId = s->m_PeerID;
   head.fid   = s->m_friendId;
   ackSt.bandWidth = s->m_packTime.getPktRcvSpeed();
   ackSt.lastIsn = ackNo;
   ackSt.rtt = s->m_RTT;
   ackSt.rttVar = s->m_RTTVar;
   ackSt.wndSize = s->m_pRcvWindow->getRecvSpace();
   head.len = sizeof(AckStruct);
   oBuf << head << ackSt;
   s->m_pChannel->sendto((sockaddr_in*)&s->m_peerAddr,(char*)oBuf.buffer(),(uint32)oBuf.size());
   ///记录发出的ACK包时间
   AckStamp st;
   st.msgNo = s->m_ackNo;
   if(s->m_rcvAckHistoryList.size()>10)
	   s->m_rcvAckHistoryList.pop_front();
   st.timeStamp = m_Timer.getTick();
   s->m_rcvAckHistoryList.push_back(st);
   Error("AckNo is:%d\n",s->m_ackNo);
 //  PROFILE_START("ACK",ackNo);
}
void CNetUnited::timerACK(CNetSocket* s)
{
	PROFILE("timerACK");

	uint32 rs = s->m_packTime.getPktRcvSpeed();
	//if(rs > 0)
	//{
	//	s->m_wndSize = (uint32)ceil(s->m_wndSize * 0.875 + rs * 0.125);
	//	if(s->m_wndSize > SOCK_WNDSIZE)
	//		s->m_wndSize = SOCK_WNDSIZE;
	//}
	Trace("timerACK ,rcvSpeed is:%d, Rcv windSize is:%d\n",rs,s->m_pRcvWindow->getRecvSpace());
}
void CNetUnited::timerNAK(CNetSocket* s)
{
	PROFILE("timerNAK");

	//将所有的丢失包消息号发送到对方
	P2P_HEAD head;
	obuffer2048 oBuf;
	uint32 len = (uint32)s->m_rcvLossList.size();
	s->m_rcvLossPkg += len;//统计接收方总共丢失的包数
	fclList<uint32>::iterator itr;
	//s->m_rcvLossList.m_mutex.do_lock();
	threadMode::scope_lock guard(s->m_rcvLossList.m_mutex);
	//如果没有丢包，则返回
	if(s->m_rcvLossList.empty())
		return;
	else
	{  
		uint32 count = 0;  //每次发送200个丢失的包
		itr = s->m_rcvLossList.begin();
		head.cmd = CMD_P2P_NAK;
		head.flag = 0xf4;
		head.len = (uint16)(s->m_rcvLossList.size() * sizeof(uint32));
		Trace("NAK packets is:%d\n",s->m_rcvLossList.size());
		head.m_sId = s->m_PeerID;
		head.fid  = s->m_friendId;
		head.m_isn = head.m_msgId = s->m_ackNo;
		oBuf<< head;
       for(;itr != s->m_rcvLossList.end() || count > 200;++itr)
	   {
		   Trace("TimerNAK, send rcvLossList packet msgNo is:%d\n",(*itr));
		   oBuf <<(uint32)(*itr);
	   }
	}
	//s->m_rcvLossList.m_mutex.do_unlock();
	//发送NAK到对方
	s->m_pChannel->sendto((sockaddr_in*)&s->m_peerAddr,(char*)oBuf.buffer(),(uint32)oBuf.size());
	Error("send NAK packet,size:%d\n",s->m_rcvLossList.size());
}
void CNetUnited::timerEXP(CNetSocket* s)
{
	PROFILE("timerEXP");
	Trace("timerEXP kkkkkk:%d\n",GetTickCount());
	s->m_expCount++;
	Trace("timerExp count is:%d\n",s->m_expCount);
	//看是否超时，超时则断开连接
	if (((s->m_expCount > 16) && 
		(s->m_expCount * ((s->m_expCount - 1) * (s->m_RTT + 4 * s->m_RTTVar) / 2 + s->m_RTTInt) > 3000000))
		|| (s->m_expCount * ((s->m_expCount - 1) * (s->m_RTT + 4 * s->m_RTTVar) / 2 + s->m_RTTInt) > 30000000))
	{
		Trace("超时断开连接! socketID is:%x,expCount:%d\n",s->m_Socket,s->m_expCount);
		s->m_broken = true;
		s->m_Status = CLOSED;
		return;
	}

	//如果发送方的丢失链表不为空，则返回
	if(!s->m_sndLossList.empty())
		return;
	//将所有还在滑动窗口内，未应答的包放入发送方丢失链表中
	uint32 beginMsgNo,endMsgNo;
	if(!s->m_pSndWindow->isEmpty())
	{
		s->m_pSndWindow->getWndRange(beginMsgNo,endMsgNo);
		//beginMsgNo = s->m_pSndWindow->m_pWnd[s->m_pSndWindow->m_beginPos].m_iMsgNo;
		//endMsgNo = s->m_pSndWindow->m_pWnd[s->m_pSndWindow->m_endPos-1].m_iMsgNo;
		if(beginMsgNo > endMsgNo)
		{
			fclAssert(false);
			return;
		}
        for(;beginMsgNo <= endMsgNo; ++beginMsgNo)
		{
			s->m_sndLossList.push_back(beginMsgNo);
		}
	}
	
	//如果没有数据，则发送保活包到对方
	P2P_HEAD head;
    if(s->m_pSndWindow->isEmpty() && s->m_sndLossList.empty())
	{
		head.cmd = CMD_P2P_KEEP_LIVE;
		head.flag = 0xf4;
		head.fid = s->m_friendId;
		head.len = 0;
		head.m_sId = s->m_PeerID;
		head.m_isn = head.m_msgId = 0;
		s->m_pChannel->sendto((sockaddr_in*)&s->m_peerAddr,(char*)&head,sizeof(head));
	}
	//更新expCount 和EXPInt周期
	s->m_EXPInt = s->m_expCount*(s->m_RTT + 4 * s->m_RTTVar) + s->m_ACKInt;
	Trace("EXPInt is:%d\n",s->m_EXPInt);
}


/**处理收到ACK控制包
*/
void     CNetUnited::processACK(CNetSocket* s,char* buf ,uint32 len)
{
	PROFILE("processACK");
	P2P_HEAD* pHead = (P2P_HEAD*)buf;
	AckStruct* pAck = (AckStruct*)pHead->data;
	///首先发送ACK2应答包
	obuffer2048 oBuf;
	P2P_HEAD head;
	head.cmd = CMD_P2P_ACK2;
	head.flag = 0xf5;
	head.m_isn = pHead->m_isn;
	head.m_msgId = pHead->m_msgId;
	head.m_sId = s->m_PeerID;
	head.fid   = s->m_friendId;
	//s->m_ackNo = s->m_ackNo;
	head.len = sizeof(pHead->m_isn);
	oBuf << head << (uint32)pHead->m_isn;
	s->m_pChannel->sendto((sockaddr_in*)&s->m_peerAddr,(char*)oBuf.buffer(),(uint32)oBuf.size());

	//记录Ack包
	AckStamp st;
	st.msgNo = pHead->m_isn;
	st.timeStamp = m_Timer.getTick();
	s->m_sndAckHistoryList.push_back(st);
	if(s->m_sndAckHistoryList.size() > s->m_wndSize)
		s->m_sndAckHistoryList.pop_front();
	//确认包，滑动窗口右移
	s->m_pSndWindow->ackBlock(pAck->lastIsn);
	Trace("recive ACK,rtt:%d,bandWidth:%d,rttVar:%d,lastIsn:%d,Snd window is:%d\n",\
		pAck->rtt,pAck->bandWidth,pAck->rttVar,pAck->lastIsn,s->m_pSndWindow->getSndSpace());
	//发送方丢失链表中所有小于pAck->lsatIsn的序号删除
	fclList<uint32>::iterator itr,tmpItr;
	//同步操作，与sendLossPacket()函数同步
	threadMode::scope_lock guard(s->m_sndLossList.m_mutex);
	itr = s->m_sndLossList.begin();
    while(itr != s->m_sndLossList.end())
	{
		tmpItr = itr;
		++itr;
		if((*tmpItr) < pAck->lastIsn)
			s->m_sndLossList.erase(tmpItr);
	}

	//s->m_RTT = pAck->rtt;
	s->m_RTTVar = pAck->rttVar; 
    //更新NTP
	s->m_NAKInt = s->m_RTT + 4 * s->m_RTTVar;
	s->m_EXPInt = s->m_expCount * (((s->m_NAKInt*2)>>2) + ((s->m_ACKInt*2)>>2));
	if(s->m_EXPInt > 2000000)
	{
		//fclAssert(false);
		Trace("m_EXPInt > 2s\n");
		s->m_EXPInt = 2000000;
	}
	if(pAck->bandWidth > 0)
        s->m_bandWidth = (s->m_bandWidth * 7 + pAck->bandWidth) >> 3;
	Error("BandWidth is:%d\n",s->m_bandWidth);
	s->m_wndSize = pAck->wndSize;
	s->m_last_EXP = m_Timer.getTick();//复位exp定时器
	//调整新的发包间隔时间
	uint32 stp = s->m_STPInt;
	if(s->m_bandWidth > 0)
		stp = (SEC(1)/s->m_bandWidth) >> 2;
	//	s->m_STPInt = (s->m_STPInt * 2 + stp) >> 2;
	s->m_STPInt = (stp + ((s->m_STPInt)>>1));
}
///收到NAK包
void     CNetUnited::processNAK(CNetSocket* s,uint32* buf,uint32 size)
{
	//将丢失的包放到发送方丢失链表中
	Error("NAKK,loss packet num is:%d\n",size);
	for(uint32 i = 0; i < size; i++)
	{//丢失的包必须落在滑动窗口内
		Trace("processNAK ,,loss packet msgNo is:%d\n",buf[i]);
		//fclAssert(s->m_pSndWindow->isInWnd(buf[i]));
		if(!s->m_pSndWindow->isInWnd(buf[i]))
		{
			Trace("不在发送窗口内:%d\n",buf[i]);
			continue;
		}
		s->m_sndLossList.remove(buf[i]);
		s->m_sndLossList.push_back(buf[i]);
		s->m_sndLossPkg++;  //统计发送方丢失的包数
	}
	sendLossPacket(s);
	//更新发送间隔时间
 //   if(findMaxIsn(buf,size) > s->m_iLastDecSeq)
	//{
	//	s->m_iAvgNAKNum = (int)ceil((double)s->m_iAvgNAKNum * 0.875 + (double)s->m_iNAKCount * 0.125) + 1;
	//	s->m_iNAKCount = 1;
	//	s->m_iDecCount = 1;
	//	s->m_iLastDecSeq = s->m_pSndWindow->getCurMsgNo();

	//	srand(s->m_iLastDecSeq);
	//	s->m_iDecRandom = (int)(rand() * double(s->m_iAvgNAKNum) / (RAND_MAX + 1.0)) + 1;
	//}
	//else if((s->m_iDecCount ++ < 5) && (0 == (++ s->m_iNAKCount % s->m_iDecRandom)))
	//{
	//	s->m_STPInt = (uint32)ceil(s->m_STPInt * 1.125);
	//	Trace("ProcessNAK,STP:%d\n",s->m_STPInt);
	//	s->m_iLastDecSeq = s->m_pSndWindow->getCurMsgNo();
	//}
	//复位exp定时器
	s->m_last_EXP = m_Timer.getTick();//复位exp定时器
	//下一个rc周期不增加发包速度
	s->m_bLoss = true;
	//增加发包间隔时间
	s->m_STPInt = (uint32)ceil(s->m_STPInt * 1.125);
}
///收到ACK2应答包
void     CNetUnited::processACK2(CNetSocket* s, uint32 ackNo)
{
	PROFILE("processACK2");
	PROFILE_END(ackNo);
	Error("ACK2:%d\n",ackNo);
	uint32 rtt = 0;
	fclList<AckStamp>::iterator itr = s->m_rcvAckHistoryList.begin();
    for(;itr != s->m_rcvAckHistoryList.end(); ++itr)
		if((*itr).msgNo == ackNo)
			break;
	if(itr != s->m_rcvAckHistoryList.end())
	{
		rtt = uint32(m_Timer.getTick() - (*itr).timeStamp);
	/*	if(rtt > 100000)
			rtt = 100000;*/
		//rtt  = (rtt >> 3);
		//Warning("RTT:%dms,ackSeq:%d\n",rtt/1000,(*itr).msgNo);
		rtt = rtt > SEC(1)?SEC(1):rtt;  //rtt不大于1s
		s->m_RTTVar = (uint32)(s->m_RTTVar * 3 + abs(int32(rtt - s->m_RTT))) >>2;
		s->m_RTT = (s->m_RTT * 7 + rtt) >> 3;
		s->m_NAKInt = s->m_RTTVar*4 + s->m_RTT;
		s->m_NAKInt = s->m_NAKInt > SEC(2)?SEC(2):s->m_NAKInt; //不能大于2秒
		s->m_EXPInt = (s->m_expCount + 1) * s->m_RTT + s->m_ACKInt;
		s->m_RTTInt = s->m_RTT + s->m_STPInt;
		Error("update ACK2,m_RTTVar:%d,m_RTT:%d,m_NAKInt:%d,m_EXPInt:%d,bandWidth:%d,speed:%d,\
			  m_RTTInt:%d\n",\
			s->m_RTTVar,s->m_RTT,s->m_NAKInt,s->m_EXPInt,s->m_packTime.getBandwidth(),\
			s->m_packTime.getPktRcvSpeed(),s->m_RTTInt);
	}
}
///收到连接握手包
void     CNetUnited::processHandShake(const uint32& srcIp,const uint16& srcPort,HandShake* pHandShake)
{
	CNetSocket* pSocket = NULL;
	P2P_HEAD head;
	HandShake hs;
	obuffer2048 oBuf;
	sockaddr_in addr;
	//首先要判断是否是重复的握手协议，如果是则发送相同的反馈包
	socketsItr itr = m_sockets.begin();
	for(;itr != m_sockets.end(); ++itr)
	{
		pSocket = (*itr).second;
		if(pSocket->m_PeerID == pHandShake->socketId)
			break;
	}
	if(pSocket && itr != m_sockets.end()) //重复的握手协议
	{
	  //向对方发送握手响应包
	  head.cmd = CMD_P2P_ACCEPT;
	  head.flag = 0xF4;
	  head.len = sizeof(HandShake);
	  head.m_isn = pSocket->m_iISN;
	  head.m_msgId = 0;
	  head.m_sId = pSocket->m_PeerID;
	  head.fid = pSocket->m_friendId;
	  hs.isn = pSocket->m_iISN;
	  hs.mss = pSocket->m_iMSS;
	  hs.socketId = pSocket->m_Socket;
	  if(pHandShake->wndSize > pSocket->m_wndSize)
	      pHandShake->wndSize = pSocket->m_wndSize;
	  oBuf << head << hs;
	}
	else if(m_bAccept)  //新的连接
	{
		m_acceptSocket = generateSocket();
		pSocket = m_sockets[m_acceptSocket];
		pSocket->m_PeerID = pHandShake->socketId;
		if(pSocket->m_iMSS > pHandShake->mss)   //取2边最小的mss值
			pSocket->m_iMSS =  pHandShake->mss;
		pSocket->m_peerIsn = pHandShake->isn;
		addr.sin_family = AF_INET;
		addr.sin_port = srcPort;
		addr.sin_addr.S_un.S_addr = srcIp;
		pSocket->m_peerAddr = addr;
		pSocket->m_Status = CONNECTED;
		pSocket->m_friendId = pHandShake->selfId; //对方的id,服务器中转使用
		m_peerAddr = addr;
		if(pHandShake->wndSize > pSocket->m_wndSize)
			pHandShake->wndSize = pSocket->m_wndSize;
		else
			pSocket->m_wndSize = pHandShake->wndSize;
		//生成滑动窗口对象
		pSocket->m_pSndWindow = new CSndBuffer(pSocket->m_wndSize); //，测试后调整到最佳值
		pSocket->m_pRcvWindow = new CRcvBuffer(pSocket->m_wndSize*2);//
		m_acceptEnt.Active(); //激活正在侦听的accept函数
		//向对方发送握手响应包
		head.cmd = CMD_P2P_ACCEPT;
		head.flag = 0xF4;
		head.len = sizeof(HandShake);
		head.m_msgId = 0;
		head.m_isn = pSocket->m_iISN;
		head.m_sId = pSocket->m_PeerID;
		head.fid = pSocket->m_friendId;
		hs.isn = pSocket->m_iISN;
		hs.mss = pSocket->m_iMSS;
		hs.socketId = pSocket->m_Socket;
		hs.wndSize = pSocket->m_wndSize;
		oBuf << head << hs;
	}
	else  //没有接受连接，不响应
	{
		//向对方发送拒绝连接包
		/*addr.sin_family = AF_INET;
		addr.sin_port = srcPort;
		addr.sin_addr.S_un.S_addr = srcIp;
		head.cmd = CMD_P2P_CLOSE;
		head.flag = 0xF4;
		head.len = 0;
		head.m_sId = pHandShake->socketId;
		oBuf << head;*/
		return;
	}
	pSocket->m_pChannel->sendto((sockaddr_in*)&addr,(char*)oBuf.buffer(),(int32)oBuf.size());
}
void CNetUnited::processData(CNetSocket* s,char* buf,uint32 len)
{
	PROFILE("processData");

	bool res;
	/*uint32 los[1000],size;*/
	P2P_HEAD* pHead = (P2P_HEAD*)buf;
	if(pHead->len + sizeof(P2P_HEAD) > len)
	{
		Error("Error packet length:%d,msgNo:%d\n",len,pHead->m_msgId);
		return;
	}
	//Trace("data block,isn:%d,msgNo:%d\n",pHead->m_isn,pHead->m_msgId);
	/*if(pHead->m_msgId > 200000)
		Trace("error!\n");*/
	//s->m_packTime.onPktArrival(pHead->m_msgId);
    s->m_totalRecvPkg++;
	if(s->m_pRcvWindow->getRecvSpace() == 0)
	{
		Error("recv windows is full!\n");
		fclAssert(false);
		return;
	}
    //如果不是期望的消息号，将中间的消息号放到接收方丢失链表中
	if(pHead->m_msgId > s->m_nextMsgNo)
	{
		addRcvLossPacket(s,s->m_nextMsgNo,pHead->m_msgId);
		s->m_nextMsgNo = pHead->m_msgId + 1;
	}
	else if(pHead->m_msgId == s->m_nextMsgNo)
	{
		++s->m_nextMsgNo;
	}
	else
	{//在滑动窗口内
		s->m_rcvLossList.remove(pHead->m_msgId);//在丢失链表中删除这个消息号
	}
	////查找丢失的包，将他们放入丢失链表，在NAK定时器中发送
	////size = s->m_pRcvWindow->getLosssBlockMsgNo(los,1000);
	//for(uint32 i=0; i< size; ++i)
	//{
	//	s->m_rcvLossList.remove(los[i]);
	//	s->m_rcvLossList.push_back(los[i]);
	//}
	//将数据块添加到滑动窗口
	//char* pBuf = new char[pHead->len];
	//memcpy(pBuf,pHead->data,pHead->len);
	res = s->m_pRcvWindow->addBlock(pHead->data,pHead->len,pHead->m_isn,pHead->m_msgId);
	if(!res)
	{
		//delete[] pBuf;
		/*Trace("Recv a repeat packet!\n");*/
	}
	timerRC(s);
}
void CNetUnited::addRcvLossPacket(CNetSocket* s,uint32 begNo,uint32 endNo)
{
	if(s->m_rcvLossList.size() > 1000)
	   Trace("RcvLossList size:%d\n",s->m_rcvLossList.size());
	while(begNo < endNo)
	{
		s->m_rcvLossList.push_back(begNo);
		++begNo;
	}
}
uint32 CNetUnited::findMaxIsn(const uint32* buf,uint32 len)
{
	uint32 maxIsn = buf[0];
	for(uint32 i =1;i < len; i++)
	{
		if(buf[i] > maxIsn)
			maxIsn = buf[i];
	}
	return maxIsn;
}
void CNetUnited::processClose(NETSOCKET u)
{//处理连接被关闭或连接被对方拒绝
	CNetSocket* pS = m_sockets[u];
	pS->m_Status = CLOSED;
	pS->m_broken = true;
	pS->m_TimeStamp = m_Timer.getTick();
}
void CNetUnited::processConnectACK(CNetSocket* s ,char* buf,uint32 len)
{ ///连接成功则在这个函数处理
   P2P_HEAD* pHead = (P2P_HEAD*)buf;
   HandShake* pHs  = (HandShake*)pHead->data;
   s->m_PeerID = pHs->socketId;
   s->m_peerIsn = pHs->isn;
   s->m_iISN = pHs->mss;
   s->m_iLastDecSeq = pHead->m_msgId;
   s->m_Status = CONNECTED;
   //根据返回的窗口大小初始化这个socket连接的滑动窗口
   s->m_wndSize = pHs->wndSize;
   s->m_pSndWindow = new CSndBuffer(pHs->wndSize); //，测试后调整到最佳值
   s->m_pRcvWindow = new CRcvBuffer(pHs->wndSize*2);//
   s->m_connectEnt.Active();//激活这个连接的等待事件
}

void CNetUnited::sendLossPacket(CNetSocket* s)
{
	PROFILE("sendLossPacket");
	if(s->m_sndLossList.empty())
		return;
    if(s->m_sndLossList.size() > 1001)
	{//调试使用
		Error("Loss packet too much ! num:%d\n",s->m_sndLossList.size());
	}
	obuffer4096 oBuf;
	P2P_HEAD head;
	head.cmd = CMD_P2P_DATA;
	head.flag = 0xF4;
	head.m_sId = s->m_PeerID;
	head.fid =   s->m_friendId;
	char* pBuf = NULL;
	uint32 i = 0,count = 10;
	//s->m_sndLossList.m_mutex.do_lock();
	threadMode::scope_lock guard(s->m_sndLossList.m_mutex);
	while(i < count && !s->m_sndLossList.empty())
	{//每次发送丢失队列里的前个包
		//tm1 = m_Timer.getTick();
	   head.m_msgId = s->m_sndLossList.front();
	  if(s->m_pSndWindow->getBlock(head.m_msgId,head.m_isn,pBuf,head.len))
	  {
		  oBuf.clear();
		  oBuf << head;
		  oBuf.push(pBuf,head.len);
		  s->m_pChannel->sendto((sockaddr_in*)&s->m_peerAddr,(char*)oBuf.buffer(),(int32)oBuf.size());
		 // sleep(s->m_STPInt);
		  Trace("sendLossPacket,msgNo :%d\n",head.m_msgId);
		  //如果发送丢失包的时候进行发包间隔控制，会造成接收包队列越来越长，所以这里不
		  //再进行等待，直接发送
		  //tm2 = m_Timer.getTick() - tm1;
		  //if(tm2 < s->m_STPInt)
			 // sleep(s->m_STPInt - tm2);
	  }
	  else
	  {
		 // Warning("丢失的包不在滑动窗口范围内,msgNo:%d\n",head.m_msgId);
	  }
	   s->m_sndLossList.pop_front();
	   i++;
	}
	//s->m_sndLossList.m_mutex.do_unlock();
}
void CNetUnited::sleep(uint64 tim)
{
	uint64 tmp  = m_Timer.getTick();
	uint32 n = (uint32)(tim>>10);
	if(n > 0)
	{
		::Sleep(n);
	    tim = tim - (n << 10);
	}
	while(m_Timer.getTick() - tmp < tim)
	{
		_asm 
		{
			nop; 
			nop;
			nop;
			nop;
			nop;
		}
	}
}
void CNetUnited::checkBrokenSocket()
{
	threadMode::scope_lock guard(m_sockets.m_mutex);
	CNetSocket* s;
	socketsItr itr,tmpItr;
	uint64 curTime = m_Timer.getTick();
	itr = m_sockets.begin();
	while(itr != m_sockets.end() && m_sockets.size() > 0)
	{
		tmpItr = itr;
		++itr;
		s = (*tmpItr).second;
		//连接关闭超过3分钟
		if((s->m_broken || s->m_Status == CLOSED) && (curTime - s->m_TimeStamp) > m_checkBrokenSocketInt)
		{
			delete s;
			m_sockets.erase(tmpItr);
		}
	}
}
bool CNetUnited::p2pTest(uint32 fId,uint32 Ip,uint16 Port,uint32 timeout /* = 5000 */)
{
	P2P_HEAD head;
	sockaddr_in addr;
	int32 res = false;
	CEventWrapper* pEvent;
	head.cmd = CMD_P2P_TEST_REQ;
	head.flag = 0xF4;
	head.fid = fId;
	head.len = 0;
	addr.sin_family = AF_INET;
	addr.sin_addr.S_un.S_addr = Ip;
	addr.sin_port = Port;
    uint64 tim = m_Timer.getTick();
	m_pChannel->sendto(&addr,(char*)&head,sizeof(head));
	if(m_queryEnt.find(fId) == m_queryEnt.end())
	{
		pEvent = new CEventWrapper;
		m_queryEnt[fId] = pEvent;
	}
	while((m_Timer.getTick() - tim)/1000 < timeout)
	{
		res = WaitForSingleObject(pEvent->m_handle,1500);
		if(res != WAIT_OBJECT_0)
			m_pChannel->sendto(&addr,(char*)&head,sizeof(head));
		else
			break;
	}
	//删除使用的事件对象
	delete m_queryEnt[fId];
	m_queryEnt.erase(fId);
	return res == WAIT_OBJECT_0?true:false;
}
void CNetUnited::processP2PTest(const uint16& cmd,const uint32 fId,const uint32& srcIp,const uint16& srcPort,const char* buf,const uint16& len)
{
	sockaddr_in addr;
	P2P_HEAD head;
	addr.sin_family = AF_INET;
	if(cmd == CMD_P2P_TEST_REQ)
	{//发送应答
		head.cmd = CMD_P2P_TEST_ACK;
		head.flag = 0xF4;
		head.fid = fId;
		head.len = 0;
		addr.sin_addr.S_un.S_addr = srcIp;
		addr.sin_port = srcPort;
		m_pChannel->sendto(&addr,(char*)&head,sizeof(head));
	}
	else
	{//CMD_P2P_TEST_ACK
         if(m_queryEnt.find(fId) == m_queryEnt.end())
		 {
			 //fclAssert(false);
			 Error("processP2PTest,not find fid:%d\n",fId);
		 }
		 else
		 {
			 m_queryEnt[fId]->Active();
		 }
	}
}
bool CNetUnited::queryFriend(sockaddr_in* addr,uint32 selfId,uint32 friendId)
{
	P2P_HEAD head;
	CEventWrapper* pEvent;
	obuffer4096 oBuf;
	head.cmd = CMD_QUERY_REQ;
	head.len = sizeof(uint32)*2;  //自己的id和好友的id长度
	head.flag = 0xF4;
	head.m_isn = 0;
	head.fid = friendId;
	head.m_msgId = 0;
	head.m_sId = 0;
	oBuf << head << (uint32)selfId << (uint32)friendId;
	//threadMode::scope_lock guard(m_queryEnt.m_mutex);
	if(m_queryEnt.find(friendId) == m_queryEnt.end())
	{
		pEvent = new CEventWrapper;
		m_queryEnt[friendId] = pEvent;
	}
	else
	{
		pEvent = m_queryEnt[friendId];
	}
	m_pChannel->sendto(addr,(char*)oBuf.buffer(),(int32)oBuf.size());
	int32 i = 0,res;
	while(i < 6)
	{
		res = WaitForSingleObject(pEvent->m_handle,1000);
	    if(res != WAIT_OBJECT_0)
			m_pChannel->sendto(addr,(char*)oBuf.buffer(),(int32)oBuf.size());
		else
			break;
		i++;
	}
	//删除使用的事件对象
	delete m_queryEnt[friendId];
	m_queryEnt.erase(friendId);
	return res == WAIT_OBJECT_0?true:false;
}
void CNetUnited::processQueryRes(char* buf,uint32 len)
{
	//uint32 fid = *(uint32*)(buf+sizeof(uint32));
	ibuffer iBuf(buf,len);
	uint32 selfId,friendId;
	iBuf >> selfId >> friendId;
	//threadMode::scope_lock guard(m_sockets.m_mutex);
	if(m_queryEnt.find(friendId) != m_queryEnt.end())
		m_queryEnt[friendId]->Active();
}
void CNetUnited::sendPacket(CNetSocket*s, uint32 cmd,char* buf,uint32 len)
{
	//SendDataStruct sndSt;
	//AckStamp stamp;
	//P2P_HEAD* pHead = (P2P_HEAD*)sndSt.data;
	//sndSt.data_len = sizeof(sndSt);
	//switch(cmd)
	//{
	//case CMD_P2P_DATA:     //数据包
	//	break;
	//case CMD_P2P_CONNECT:  //请求连接
	//	sndSt.data_len = sizeof(sndSt) + sizeof(head);
	//	pHead->cmd = CMD_P2P_CONNECT;
	//	pHead->flag = 0xF4;
	//	pHead->len = 0;
	//	break;
	//case CMD_P2P_ACK: //ACK
 //       sndSt.data_len = sizeof(sndSt);
	//	pHead->cmd = CMD_P2P_ACK;
	//	pHead->flag = 0xf4;
	//	pHead->m_msgId = 
	//	pHead->m_sId = s->m_PeerID;
	//    pHead->len = len;
	//	memcpy(pHead->data,buf,len);
	//	stamp.msgNo
	//	s->m_rcvAckHistoryList.push_back()
	//	break;
	//case CMD_P2P_NAK:  //NAK
	//	len = size/sizeof(uint32);
	//	processNAK(s,(uint32*)pHead->data,len);
	//	break;
	//case CMD_P2P_ACK2:  //ACK2
	//	msgNo = *(uint32*)pHead->data;
	//	processACK2(s,msgNo);
	//	break;
	//case CMD_P2P_KEEP_LIVE:  //保活
	//	break;
	//default:
	//	Error("Recv a unknow type packet!\n");
	//}
}
