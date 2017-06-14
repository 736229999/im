/******************************************************************
** �ļ���:	NetMisc.h
** ��  Ȩ:	���ڻ��Ǻ�Ѷ
** ������:	Tony
** ��  ��:	2007-6-4
** ��  ��:  ����ļ���Ҫ��һЩNetTrans�õ��ĸ�����

**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
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
//���ݰ�isn���������
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
//��¼ack�շ�ʱ����ṹ
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

	uint64 m_TimeStamp;                     // ���ӹرյ�ʱ���

	sockaddr_in m_selfAddr;                    // ��socketʹ�õĵ�ַ
	sockaddr_in m_peerAddr;                    // �Է���ַ

	NETSOCKET m_Socket;                       // socket ID
	uint32   m_iMSS;                          //ÿ����������ֽ���
	NETSOCKET m_PeerID;                       // peer socket ID
	uint32 m_iISN;                           // ���ӳ�ʼ�����
	uint32 m_iMsgNo;                         //��Ϣ��
	uint32 m_peerIsn;                        //���ӶԶ˳�ʼ��Ѷ��
	CSndBuffer* m_pSndWindow;               //���ʹ���
	CRcvBuffer* m_pRcvWindow;               ///���մ���
	fclList<uint32> m_sndLossList;  ///���Ͷ����ݰ���ʧ����
	fclList<uint32> m_rcvLossList;  ///���ն����ݰ���ʧ����
	list<AckStamp>  m_rcvAckHistoryList;  //���շ�ack����¼����
	list<AckStamp>  m_sndAckHistoryList;  //���ͷ�ack����¼����
    CPacketTime m_packTime;                ///��¼�����շ�ʱ��
	///����������socket���շ����ݰ�,���ⲿ���룬����ʱ����ɾ��
	CChannel*  m_pChannel;                   
	CEventWrapper m_connectEnt;              ///����Э��ȴ��¼�
	typedef multiThread  threadMode;        
	threadMode::mutex      m_mutex;          ///�߳�����ͬʱֻ����һ���߳���ͬһ���߼����ӷ�������
	uint32     m_nextMsgNo;                  //�´�Ӧ���յ�����Ϣ��
	uint32     m_friendId;                   //�Է�id����������ת��ʽʹ��
	uint32     m_selfId;                     //�Լ���id 
	//std::set<NETSOCKET> m_queuedSockets;    // set of connections waiting for accept()
	//std::set<NETSOCKET> m_acceptSockets;    // set of accept()ed connections

	//CEventWrapper m_acceptCond;
	//CMutexWrapper m_AcceptLock;

    uint32 m_ackNo;                        //����ack��isn��ţ�ÿ�μ�һ
	unsigned int m_uiBackLog;                 // maximum number of connections in queue
	bool m_broken;                            //�߼������Ƿ��Ѿ��Ͽ�
	////���ʿ���������
	uint32  m_RTTVar;                        //rtt��������
    uint32  m_RTT;                           //RTTֵ
	uint32  m_wndSize;                       //�������ڴ�С
	uint32 m_iLastDecSeq;                    //���ϴ����ʽ���֮���͵������Ϣ��
	uint32 m_iNAKCount;                      //���ϴν������ʺ��յ���NAK����
	uint32 m_iDecRandom;                     //1��m_iAvgNAKNum֮��������
	uint32 m_iAvgNAKNum;                     //�������Ŵ���LSDʱ�����¼�֮���NAK�ƶ���ƽ����
	uint32 m_iDecCount;
	bool   m_bLoss;                          //��һ��rc�������Ƿ񶪹���
	///��ʱ��ʹ�ñ���
	uint32  m_EXPInt;                           ///exp��ʱ������
	uint64  m_last_EXP;                      ///exp���һ�δ���ʱ��(����)
	uint32  m_RTTInt;                          //RTT��ʱ������,��ʼ��ֵΪ10ms
	uint64  m_last_RTT;                     //rtt���һ�δ���ʱ�䣨���룩
	uint32  m_ACKInt;                          ///ack��ʱ������
	uint64  m_last_ACK;                     ///ACK���һ�δ���ʱ�䣨���룩
	uint32  m_NAKInt;                         ///NAK��ʱ������
	uint64  m_last_NAK;                    ///NAK���һ�δ���ʱ��
	uint32  m_STPInt;                      ///���ݰ����ͼ��ʱ��
	uint32  m_expCount;                    ///exp��ʱ�����ڴ���
	uint32  m_bandWidth;                   ///���ƴ���
	//Socket ͳ����Ϣ
	uint32  m_totalSendPkg;               //�ܹ����͵����ݰ�
	uint32  m_totalRecvPkg;               //�ܹ��յ������ݰ�
	uint32  m_sndLossPkg;                 //���ͷ���ʧ�����ݰ���
	uint32  m_rcvLossPkg;                 //���շ���ʧ�����ݰ���
};
///socket��ȫ�ֹ�����
class CZUdpComm;
struct RecvDataStruct;
class CNetUnited
{
	friend class CZUdpComm;
public:
     CNetUnited();
	 ~CNetUnited();
	 int         bind(NETSOCKET u,sockaddr_in* addr,int* addrlen);
	 NETSOCKET   generateSocket(int wndSize = SOCK_WNDSIZE); //����һ��������ʽ�����socket

	 NETSOCKET	accept(NETSOCKET u ,sockaddr_in* addr, int* addrlen);


	 int	       connect(NETSOCKET u,sockaddr_in* addr, int* addrlen,uint32 selfId = 0,uint32 friendId = 0);

	 int			send(NETSOCKET u,char* buf,int len);

	 int			recv(NETSOCKET u,char * buf,int nLen);
	 ///p2p��
     bool           p2pTest(uint32 fId,uint32 Ip,uint16 Port,uint32 timeout = 5000);
	 void		close(NETSOCKET u);
	 //��ѯ�����Ƿ��½��ת������
	 bool     queryFriend(sockaddr_in* addr,uint32 selfId,uint32 friendId);
	 void     processTimer();///��ʱ���������������rtt��ʱ��
	 void     recvP2pPacket(const uint32& srcIp,const uint16& srcPort,char* buf,uint32& size);///�����յ���p2p��ʽ���ݣ��������ư���
	 //���Ͷ�ʧ�İ�
	 void     sendLossPacket(CNetSocket* s);
public:
	CChannel* m_pChannel;  
	fclList<DataBlock> m_dataQueue;         //�����յ��İ�
private:
	/**���ʿ��ƶ�ʱ��,����10ms
	  @param s:  socket���Ӷ���
	*/
	void     timerRC(CNetSocket* s);  
	/**���Ͷ�ʧ�����涨ʱ��
	  @note:��ʱ����Ϊ3*RTT,����rtt��ֵ���ı�����
	*/
	void     timerNAK(CNetSocket* s);
	/**ACK��ʱ��,����10ms
	*/
	void     timerACK(CNetSocket* s);
	/**�ش���ʱ��
	  @note: ����Ϊ3*RTT+ATP
	*/
	void     timerEXP(CNetSocket* s);  
	/**�����յ�ACK���ư�
	*/
	void     processACK(CNetSocket* s,char* buf ,uint32 len);
	///�յ�NAK��
	void     processNAK(CNetSocket* s,uint32* buf,uint32 size);
	///�յ�ACKӦ���
	void     processACK2(CNetSocket* s, uint32 ackNo);
	///�յ��������ְ�
	void     processHandShake(const uint32& srcIp,const uint16& srcPort,HandShake* pHandShake);
	///�رջ����ӱ��ܾ�
	void     processClose(NETSOCKET u);
	///�յ�������Ӧ��
	void     processConnectACK(CNetSocket* s ,char* buf,uint32 len);
	//��������������nak�����Ķ�ʧ����isn��
	uint32   findMaxIsn(const uint32* buf,uint32 len);
	//�����յ������ݰ�
	void     processData(CNetSocket* s,char* buf,uint32 len);
	//���Ϳ��ư�
	void     sendPacket(CNetSocket*s, uint32 cmd,char* buf,uint32 len);
	//΢����ߺ���
	void   sleep(uint64 tim);
	///����Ѿ��Ͽ������ӣ��������һ��ʱ�䣬ɾ������
	void   checkBrokenSocket();
	//��begNo��endNo֮�����Ϣ�ż��뵽���ͽ��ն�ʧ������
	void   addRcvLossPacket(CNetSocket* s,uint32 begNo,uint32 endNo);
	//�����յ�����ת��������ѯ���
	void   processQueryRes(char* buf,uint32 len);
	//�յ�p2p�򶴰�
	void   processP2PTest(const uint16& cmd,const uint32 fId,const uint32& srcIp,const uint16& srcPort,const char* buf,const uint16& len);
	//CZUdpComm* m_udpComm;
	typedef fclHash_map<NETSOCKET,CNetSocket*>::iterator socketsItr;
	fclHash_map<NETSOCKET,CNetSocket*> m_sockets;
	fclHash_map<uint32,CEventWrapper*> m_queryEnt;  //��ѯ��������¼�
	uint32 m_socketID;   //���������߼�socket��ȫ��ֵ
	typedef multiThread   threadMode;
	threadMode::mutex  m_mutex;  //�̰߳�ȫ
	uint32 m_maxSocketID;
	NewTimer<>  m_Timer;                     //�߾���ʱ����
	bool m_bAccept;                         //�Ƿ�������������������
	NETSOCKET m_acceptSocket;               ///���ܵ����ӵ�NetSOCKETID
	CEventWrapper m_acceptEnt;              //�Ƿ����û����ӽ���
	sockaddr_in m_peerAddr;                 //���ӽ������û���ַ
	CEventWrapper m_p2pEnt;                  ///p2p���Եȴ��¼�
	bool          m_isP2pTesting;            ///�Ƿ�����P2P��
	uint32        m_checkBrokenSocketInt;   ///����ѹر�socket����
	uint64        m_lastCheckBrokenSocket;  //�ϴμ��ʱ��
};


#endif/*__NET_MISC*/