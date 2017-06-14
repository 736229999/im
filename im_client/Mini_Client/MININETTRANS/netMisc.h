/******************************************************************
** 文件名:	NetMisc.h
** 版  权:	深圳华亚和讯
** 创建人:	Tony
** 日  期:	2007-6-4
** 描  述:  这个文件主要是一些NetTrans用到的辅助类

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
*******************************************************************/

#ifndef __NET_MISC__
#define __NET_MISC__
#pragma  once
#include "config.h"
#include "fcl.h"
#include "winsock2.h"
#include "BufferManager.h"
#include "Channel.h"
#include "PacketTime.h"
#include "p2pDef.h"

using namespace fcl;



enum NETSTATUS {INIT = 1, OPENED, LISTENING, CONNECTED, CLOSED};
//数据包isn序号生成类
class CSeqNo
{
public:
	inline static const int seqcmp(const uint32& seq1,const uint32& seq2)
	{return ((uint32)abs(seq1 - seq2) < m_iSeqNoTH) ? (seq1 - seq2) : (seq2 - seq1);}

	inline static const int seqlen(const uint32& seq1,const uint32& seq2)
	{return (seq1 <= seq2) ? (seq2 - seq1 + 1) : (seq2 - seq1 + 1 + m_iMaxSeqNo);}

	inline static const int seqoff(const uint32& seq1,const uint32& seq2)
	{
		if ((uint32)abs(seq1 - seq2) < m_iSeqNoTH)
			return seq2 - seq1;

		if (seq1 < seq2)
			return seq2 - seq1 - m_iMaxSeqNo;

		return seq2 - seq1 + m_iMaxSeqNo;
	}

	inline static const uint32 incseq(const uint32& seq)
	{return (seq == m_iMaxSeqNo - 1) ? 0 : seq + 1;}

	inline static const uint32 decseq(const uint32& seq)
	{return (seq == 0) ? m_iMaxSeqNo - 1 : seq - 1;}

	inline static const uint32 incseq(const uint32& seq,const  uint32& inc)
	{return (m_iMaxSeqNo - seq > inc) ? seq + inc : seq - m_iMaxSeqNo + inc;}
    //add by Tony
	inline static const uint32 generateIsn()
	{  srand((unsigned int)GetTickCount());
	  return (uint32)(double(rand()) * CSeqNo::m_iMaxSeqNo / (RAND_MAX + 1.0));
	}
public:
	static const uint32 m_iSeqNoTH;             // threshold for comparing seq. no.
	static const uint32 m_iMaxSeqNo;            // maximum sequence number used in UDT
};
//记录ack收发时间戳结构
typedef struct 
{
	uint32 msgNo;
	uint64 timeStamp;
}AckStamp;
class CNetSocket
{
	friend class CNetUnited;
public:
	CNetSocket();
	~CNetSocket();
	//bool operator < (const CNetSocket& d)
	//{
	//	return m_Socket < d.m_Socket;
	//}
private:
	
	NETSTATUS m_Status;                       // current socket state

	uint64 m_TimeStamp;                     // 连接关闭的时间戳

	sockaddr_in m_selfAddr;                    // 本socket使用的地址
	sockaddr_in m_peerAddr;                    // 对方地址

	NETSOCKET m_Socket;                       // socket ID
	uint32   m_iMSS;                          //每个包的最大字节数
	NETSOCKET m_PeerID;                       // peer socket ID
	uint32 m_iISN;                           // 连接初始化序号
	uint32 m_iMsgNo;                         //消息号
	uint32 m_peerIsn;                        //连接对端初始化讯号
	CSndBuffer* m_pSndWindow;               //发送窗口
	CRcvBuffer* m_pRcvWindow;               ///接收窗口
	fclList<uint32> m_sndLossList;  ///发送端数据包丢失链表
	fclList<uint32> m_rcvLossList;  ///接收端数据包丢失链表
	list<AckStamp>  m_rcvAckHistoryList;  //接收方ack包记录链表
	list<AckStamp>  m_sndAckHistoryList;  //发送方ack包记录链表
    CPacketTime m_packTime;                ///记录包的收发时间
	///用来在网络socket上收发数据包,由外部传入，析构时不用删除
	CChannel*  m_pChannel;                   
	CEventWrapper m_connectEnt;              ///握手协议等待事件
	typedef multiThread  threadMode;        
	threadMode::mutex      m_mutex;          ///线程锁，同时只能有一个线程在同一个逻辑连接发送数据
	uint32     m_nextMsgNo;                  //下次应该收到的消息号
	uint32     m_friendId;                   //对方id，服务器中转方式使用
	uint32     m_selfId;                     //自己的id 
	//std::set<NETSOCKET> m_queuedSockets;    // set of connections waiting for accept()
	//std::set<NETSOCKET> m_acceptSockets;    // set of accept()ed connections

	//CEventWrapper m_acceptCond;
	//CMutexWrapper m_AcceptLock;

    uint32 m_ackNo;                        //发送ack的isn序号，每次加一
	unsigned int m_uiBackLog;                 // maximum number of connections in queue
	bool m_broken;                            //逻辑连接是否已经断开
	////速率控制属性类
	uint32  m_RTTVar;                        //rtt修正变量
    uint32  m_RTT;                           //RTT值
	uint32  m_wndSize;                       //流量窗口大小
	uint32 m_iLastDecSeq;                    //自上次速率降低之后发送的最大消息号
	uint32 m_iNAKCount;                      //自上次降低速率后收到的NAK数量
	uint32 m_iDecRandom;                     //1到m_iAvgNAKNum之间的随机数
	uint32 m_iAvgNAKNum;                     //当最大序号大于LSD时两次事件之间的NAK移动的平均数
	uint32 m_iDecCount;
	bool   m_bLoss;                          //在一个rc周期内是否丢过包
	///定时器使用变量
	uint32  m_EXPInt;                           ///exp定时器周期
	uint64  m_last_EXP;                      ///exp最后一次触发时间(毫秒)
	uint32  m_RTTInt;                          //RTT定时器周期,初始化值为10ms
	uint64  m_last_RTT;                     //rtt最后一次触发时间（毫秒）
	uint32  m_ACKInt;                          ///ack定时器周期
	uint64  m_last_ACK;                     ///ACK最后一次触发时间（毫秒）
	uint32  m_NAKInt;                         ///NAK定时器周期
	uint64  m_last_NAK;                    ///NAK最后一次触发时间
	uint32  m_STPInt;                      ///数据包发送间隔时间
	uint32  m_expCount;                    ///exp定时器到期次数
	uint32  m_bandWidth;                   ///估计带宽
	//Socket 统计信息
	uint32  m_totalSendPkg;               //总共发送的数据包
	uint32  m_totalRecvPkg;               //总共收到的数据包
	uint32  m_sndLossPkg;                 //发送方丢失的数据包数
	uint32  m_rcvLossPkg;                 //接收方丢失的数据包数
};
///socket的全局管理类
class CZUdpComm;
struct RecvDataStruct;
class CNetUnited
{
	friend class CZUdpComm;
public:
     CNetUnited();
	 ~CNetUnited();
	 int         bind(NETSOCKET u,sockaddr_in* addr,int* addrlen);
	 NETSOCKET   generateSocket(int wndSize = SOCK_WNDSIZE); //生成一个用于流式传输的socket

	 NETSOCKET	accept(NETSOCKET u ,sockaddr_in* addr, int* addrlen);


	 int	       connect(NETSOCKET u,sockaddr_in* addr, int* addrlen,uint32 selfId = 0,uint32 friendId = 0);

	 int			send(NETSOCKET u,char* buf,int len);

	 int			recv(NETSOCKET u,char * buf,int nLen);
	 ///p2p打洞
     bool           p2pTest(uint32 fId,uint32 Ip,uint16 Port,uint32 timeout = 5000);
	 void		close(NETSOCKET u);
	 //查询好友是否登陆中转服务器
	 bool     queryFriend(sockaddr_in* addr,uint32 selfId,uint32 friendId);
	 void     processTimer();///定时器函数，处理各种rtt定时器
	 void     recvP2pPacket(const uint32& srcIp,const uint16& srcPort,char* buf,uint32& size);///处理收到的p2p流式数据（包括控制包）
	 //发送丢失的包
	 void     sendLossPacket(CNetSocket* s);
public:
	CChannel* m_pChannel;  
	fclList<DataBlock> m_dataQueue;         //缓存收到的包
private:
	/**速率控制定时器,周期10ms
	  @param s:  socket连接对象
	*/
	void     timerRC(CNetSocket* s);  
	/**发送丢失包报告定时器
	  @note:定时周期为3*RTT,随着rtt的值而改变周期
	*/
	void     timerNAK(CNetSocket* s);
	/**ACK定时器,周期10ms
	*/
	void     timerACK(CNetSocket* s);
	/**重传定时器
	  @note: 周期为3*RTT+ATP
	*/
	void     timerEXP(CNetSocket* s);  
	/**处理收到ACK控制包
	*/
	void     processACK(CNetSocket* s,char* buf ,uint32 len);
	///收到NAK包
	void     processNAK(CNetSocket* s,uint32* buf,uint32 size);
	///收到ACK应答包
	void     processACK2(CNetSocket* s, uint32 ackNo);
	///收到连接握手包
	void     processHandShake(const uint32& srcIp,const uint16& srcPort,HandShake* pHandShake);
	///关闭或连接被拒绝
	void     processClose(NETSOCKET u);
	///收到连接响应包
	void     processConnectACK(CNetSocket* s ,char* buf,uint32 len);
	//辅助函数，查找nak中最大的丢失包的isn号
	uint32   findMaxIsn(const uint32* buf,uint32 len);
	//处理收到的数据包
	void     processData(CNetSocket* s,char* buf,uint32 len);
	//发送控制包
	void     sendPacket(CNetSocket*s, uint32 cmd,char* buf,uint32 len);
	//微妙级休眠函数
	void   sleep(uint64 tim);
	///检查已经断开的连接，如果超过一定时间，删除连接
	void   checkBrokenSocket();
	//将begNo至endNo之间的消息号加入到发送接收丢失链表中
	void   addRcvLossPacket(CNetSocket* s,uint32 begNo,uint32 endNo);
	//处理收到的中转服务器查询结果
	void   processQueryRes(char* buf,uint32 len);
	//收到p2p打洞包
	void   processP2PTest(const uint16& cmd,const uint32 fId,const uint32& srcIp,const uint16& srcPort,const char* buf,const uint16& len);
	//CZUdpComm* m_udpComm;
	typedef fclHash_map<NETSOCKET,CNetSocket*>::iterator socketsItr;
	fclHash_map<NETSOCKET,CNetSocket*> m_sockets;
	fclHash_map<uint32,CEventWrapper*> m_queryEnt;  //查询结果激活事件
	uint32 m_socketID;   //用于生成逻辑socket的全局值
	typedef multiThread   threadMode;
	threadMode::mutex  m_mutex;  //线程安全
	uint32 m_maxSocketID;
	NewTimer<>  m_Timer;                     //高精度时间类
	bool m_bAccept;                         //是否正在侦听，接受连接
	NETSOCKET m_acceptSocket;               ///接受到连接的NetSOCKETID
	CEventWrapper m_acceptEnt;              //是否有用户连接进来
	sockaddr_in m_peerAddr;                 //连接进来的用户地址
	CEventWrapper m_p2pEnt;                  ///p2p测试等待事件
	bool          m_isP2pTesting;            ///是否正在P2P打洞
	uint32        m_checkBrokenSocketInt;   ///检查已关闭socket周期
	uint64        m_lastCheckBrokenSocket;  //上次检查时间
};


#endif/*__NET_MISC*/