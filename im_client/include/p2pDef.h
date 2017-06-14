/******************************************************************
** 文件名:	p2pDef.h
** 版  权:	深圳华亚和讯
** 创建人:	Tony
** 日  期:	2007-6-15 15:10
** 描  述:  udp流式传输消息，控制命令字定义

**************************** 修改记录 ******************************
** 修改人: xy
** 日  期: 
** 描  述: 
*******************************************************************/
#ifndef __P2P_MSG_DEF__
#define __P2P_MSG_DEF__
#pragma  once

#pragma warning(disable:4200)

// 传输窗口的大小
#define XUDS_WND_SIZE	128
// 最佳传输块大小
#define BEST_SEG_SIZE	1024
#define	MAX_SEG_SIZE	1500


typedef unsigned char    uint8;
typedef unsigned short   uint16;
typedef unsigned int     uint32;
typedef unsigned __int64 uint64;


#define	UDS_FLAG		0x7


const uint16	CMD_P2P_CONNECT		=	0x0001;  // 连接握手命令
const uint16	CMD_P2P_ACCEPT		=	0x0002;  // 连接成功
const uint16	CMD_P2P_CLOSE		=	0x0003;  // 关闭一个连接,或被拒绝连接
const uint16	CMD_P2P_CLOSE_ACK	=	0x0004;
const uint16	CMD_P2P_DATA		=	0x0101;  // 收发数据
const uint16	CMD_P2P_ACK	        =	0x0102;  // 数据应答
//const uint16    CMD_P2P_NAK         =   0x0103;  // NAK包
const uint16    CMD_P2P_ACK2        =   0x0104;  // ACK的应答包
const uint16    CMD_P2P_KEEP_LIVE   =   0x0105;  // 保活包
const uint16    CMD_P2P_TEST_REQ    =   0x0106;  // p2p测试请求包
const uint16    CMD_P2P_TEST_ACK    =   0x0107;  // p2p测试应答包
const uint16    CMD_QUERY_REQ       =   0x0108;  // 向中转服务器查询好友信息请求  
const uint16    CMD_QUERY_ACK       =   0x0109;  // 向中转服务器返回查询应答


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

	uint16	flag:4;		// UDS包标志位 (0~15)
	uint16	cmd:12;		// 命令字
	uint16	len;		// 数据长度
	uint32	did;		// 接收者id
	uint32	socketId;	// 会话id
	uint32	seq;		// 数据分包序号
};

struct XUDS_HEADER:public XUDS_HEAD
{
	uint8	data[0];	// 数据
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
	uint8		rt;			// 返回结果
	uint32		ai;			// 接收包时间间隔（微秒）
	//uint32		wndPos;		// 通知接收窗口位置
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
