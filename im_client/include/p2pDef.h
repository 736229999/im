/******************************************************************
** �ļ���:	p2pDef.h
** ��  Ȩ:	���ڻ��Ǻ�Ѷ
** ������:	Tony
** ��  ��:	2007-6-15 15:10
** ��  ��:  udp��ʽ������Ϣ�����������ֶ���

**************************** �޸ļ�¼ ******************************
** �޸���: xy
** ��  ��: 
** ��  ��: 
*******************************************************************/
#ifndef __P2P_MSG_DEF__
#define __P2P_MSG_DEF__
#pragma  once

#pragma warning(disable:4200)

// ���䴰�ڵĴ�С
#define XUDS_WND_SIZE	128
// ��Ѵ�����С
#define BEST_SEG_SIZE	1024
#define	MAX_SEG_SIZE	1500


typedef unsigned char    uint8;
typedef unsigned short   uint16;
typedef unsigned int     uint32;
typedef unsigned __int64 uint64;


#define	UDS_FLAG		0x7


const uint16	CMD_P2P_CONNECT		=	0x0001;  // ������������
const uint16	CMD_P2P_ACCEPT		=	0x0002;  // ���ӳɹ�
const uint16	CMD_P2P_CLOSE		=	0x0003;  // �ر�һ������,�򱻾ܾ�����
const uint16	CMD_P2P_CLOSE_ACK	=	0x0004;
const uint16	CMD_P2P_DATA		=	0x0101;  // �շ�����
const uint16	CMD_P2P_ACK	        =	0x0102;  // ����Ӧ��
//const uint16    CMD_P2P_NAK         =   0x0103;  // NAK��
const uint16    CMD_P2P_ACK2        =   0x0104;  // ACK��Ӧ���
const uint16    CMD_P2P_KEEP_LIVE   =   0x0105;  // �����
const uint16    CMD_P2P_TEST_REQ    =   0x0106;  // p2p���������
const uint16    CMD_P2P_TEST_ACK    =   0x0107;  // p2p����Ӧ���
const uint16    CMD_QUERY_REQ       =   0x0108;  // ����ת��������ѯ������Ϣ����  
const uint16    CMD_QUERY_ACK       =   0x0109;  // ����ת���������ز�ѯӦ��


#pragma warning(push)
#pragma pack(push , 1)

struct XUDS_HEAD
{
	XUDS_HEAD()
	{
		len = 0;
		socketId = 0;
		did = 0;
		seq = 0;
		cmd = 0;
		flag = UDS_FLAG;
	}

	uint16	flag:4;		// UDS����־λ (0~15)
	uint16	cmd:12;		// ������
	uint16	len;		// ���ݳ���
	uint32	did;		// ������id
	uint32	socketId;	// �Ựid
	uint32	seq;		// ���ݷְ����
};

struct XUDS_HEADER:public XUDS_HEAD
{
	uint8	data[0];	// ����
};

struct XUDS_PACKET
{
	uint32		srcIp;
	uint16		srcPort;
	uint64		tickCount;
	uint32		len;
	char		data[MAX_SEG_SIZE];
};


struct XUDS_DATA:public XUDS_HEAD
{
	XUDS_DATA()
	{
		cmd = CMD_P2P_DATA;
	}
	uint8		udsData[BEST_SEG_SIZE];
};

struct XUDS_DATA_ACK:public XUDS_HEAD
{
	XUDS_DATA_ACK()
	{
		cmd = CMD_P2P_ACK;
		len = sizeof(XUDS_DATA_ACK) - sizeof(XUDS_HEAD);
	}
	enum RESULT{ OK = 0,REPEAT,NOT_IN_WND,RECV_BUF_FULL};
	uint8		rt;			// ���ؽ��
	uint32		ai;			// ���հ�ʱ������΢�룩
	//uint32		wndPos;		// ֪ͨ���մ���λ��
};

struct XUDS_CONNECT:public XUDS_HEAD
{
	XUDS_CONNECT()
	{
		cmd = CMD_P2P_CONNECT;
		len = sizeof(XUDS_CONNECT) - sizeof(XUDS_HEAD);
		socketId = 0;
		connectId = 0;
		sendWndSize = XUDS_WND_SIZE;
		sendBufSize = XUDS_WND_SIZE * BEST_SEG_SIZE;
		recvWndSize = XUDS_WND_SIZE;
		recvBufSize = XUDS_WND_SIZE * BEST_SEG_SIZE;
	}
	uint32		connectId;
	uint32		sendBufSize;
	uint32		recvBufSize;
	uint16		sendWndSize;
	uint16		recvWndSize;		
};

struct XUDS_P2P_TEST:public XUDS_HEAD
{
	XUDS_P2P_TEST()
	{
		cmd = CMD_P2P_TEST_REQ;
		len = sizeof(XUDS_P2P_TEST) - sizeof(XUDS_HEAD);
		socketId = 0;
		testId = 0;
	}
	uint32		testId;
};

struct XUDS_ACCEPT:public XUDS_HEAD
{
	enum RESULT{ ACCEPT,DENEY,WAIT};
	XUDS_ACCEPT()
	{
		cmd = CMD_P2P_ACCEPT;
		len = sizeof(XUDS_ACCEPT) - sizeof(XUDS_HEAD);
		rt = 1;
		seq = 0;
	}
	uint8	rt;
};

struct XUDS_CLOSE:public XUDS_HEAD
{
	XUDS_CLOSE()
	{
		cmd = CMD_P2P_CLOSE;
		len = sizeof(XUDS_CLOSE) - sizeof(XUDS_HEAD);
	}
};

struct XUDS_CLOSE_ACK:public XUDS_HEAD
{
	XUDS_CLOSE_ACK()
	{
		cmd = CMD_P2P_CLOSE_ACK;
		len = sizeof(XUDS_CLOSE_ACK)- sizeof(XUDS_HEAD);
	}
};

struct XUDS_QUERY_REQ:public  XUDS_HEAD
{
	XUDS_QUERY_REQ()
	{
		cmd = CMD_QUERY_REQ;
		len = sizeof(XUDS_QUERY_REQ) - sizeof(XUDS_HEAD);;
	}
	uint32	myId;
	uint32	queryId;
};

struct XUDS_QUERY_ACK:public XUDS_HEAD
{
	XUDS_QUERY_ACK()
	{
		cmd = CMD_QUERY_ACK;
		len = sizeof(XUDS_QUERY_ACK) - sizeof(XUDS_HEAD);
	}
	enum RESULT{
		OK = 0,
		INFO_TIMEOUT,
		ID_NOT_EXIST
	};
	uint8	rt;
	uint16	port;
	uint32	ip;
	uint32	friendId;
};


#pragma  pack(push , 1)
#pragma warning(pop)



#endif/*__P2P_MSG_DEF__*/
