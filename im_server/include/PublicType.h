/************************************************************************ 
 文件描述：公共类型定义
 
 创建人： zouyf, 2006-6-9 
 
 修改记录： 
************************************************************************/

#ifndef __ZPUBLICTYPE_H_
#define __ZPUBLICTYPE_H_

#include <string.h>

typedef unsigned char    uint8;
typedef unsigned short   uint16;
typedef unsigned int     uint32;
typedef unsigned long long uint64;

#define MAX_MSG_LEN					1400	  //聊天消息最大长度

#define XTREAM_ONLINE				0x00  //上线
#define XTREAM_LEAVE				0x01  //离开
#define XTREAM_BUSY					0x02  //忙碌
#define XTREAM_HANGUP				0x03  //挂起
#define XTREAM_HIDE					0x12  //隐身
#define XTREAM_OFFLINE				0x13  //离线
#define XTREAM_EXIT					XTREAM_OFFLINE


//好友组ID定义
#define GROUPID_MYFRIEND			0	//我的好友
#define GROUPID_STRANGER			1	//陌生人
#define GROUPID_BLACKLIST			2	//黑名单
//其他组ID是自定义组



#define PERM_FAMILY				1	//固定家族
#define TEMP_FAMILY				2	//临时家族

#define FAMILY_COMMON				0	//普通家族
#define FAMILY_VIP					1	//VIP家族

//家族成员操作Action
#define CMEMBER_ADD					0	//添加
#define CMEMBER_DEL					1	//删除
#define CMEMBER_SET_ADMIN			2	//设为管理员
#define CMEMBER_CANCEL_ADMIN		3	//取消管理员
//#define CMEMBER_PROFILE_MOD		    4	//设置群名片
#define CMEMBER_REFUSE_IN_1			4	//被邀请人拒绝入群
#define CMEMBER_REFUSE_IN_2			5	//群主拒绝入群

#define FAMILY_REQ_JOIN				CMEMBER_ADD	//申请加入固定家族
#define FAMILY_REQ_QUIT				CMEMBER_DEL	//请求退出家族

//家族验证方式
#define FAMILY_JOIN_ANY				0	//任意加入
#define FAMILY_JOIN_VALIDATE		1	//验证
#define FAMILY_JOIN_UNALLOW			2	//拒绝

//请求加入家族消息
#define FAMILY_JOIN_REQUEST			0	//请求加入
#define FAMILY_JOIN_AGREE			1	//同意加入
#define FAMILY_JOIN_REFUSE			2	//拒绝加入

//家族页面浏览权限
#define FAMILY_VIEW_MEM				1	//家族成员（私有）
#define FAMILY_VIEW_REG				2	//注册用户（半公开）
#define FAMILY_VIEW_ALL				3	//所有（公开）

//家族成员属性
#define FAMILY_CREATER				2	//家族创建者
#define FAMILY_MANAGER				1	//家族管理员
#define FAMILY_NORMAL				0	//普通成员



//#define MAX_NAME_LEN				24

//用户资料基本信息长度定义

#define MAX_USERNAME_LEN			120				//用户名 原长度20
#define MAX_NICKNAME_LEN			40				//呢称 原长度32
#define MAX_HEADURL_LEN				100				//自定义头像图片URL
#define MAX_EMAIL_LEN				60				//Email
#define MAX_PASSWORD_LEN			60				//数据库存储加密后的长度
#define MAX_BIRTHDAY_LEN			8				//生日
#define MAX_ADDRESS_LEN				80				//地址
#define MAX_PHONE_LEN				20				//通讯号码
#define MAX_DESCRIPTION_LEN			100				//个人说明
#define MAX_VERSION_URL             100				//版本更新URL
#define MAX_UPDATECONFIG_URL		100				//版本配置文件URL
#define MAX_UPDATE_INFO             512				//升级信息
#define MAX_WEB_LOCATION            100				//web location
#define MAX_GROUPNAME_LEN			20				//好友分组的组名长度
#define MAX_SESSIONKEY_LEN			16				//Session key
#define MAX_COMMENTNAME_LEN			16				//好友备注
#define MAX_USERSIGN_LEN			100				//个人签名
#define MAX_AVATOR_LEN				100				//闪秀地址
#define MAX_GUID_LEN				100				//GUID
#define MAX_MSG_LENGTH				1315			//聊天最大消息长度
#define MAX_WORKGROUP_LEN			100				//工作组信息

//好友加入验证方式

//允许任何人加为好友
#define FRIEND_JOIN_ANYONE	0
//需要验证
#define FRIEND_JOIN_VERIFY	1
//拒绝任何人加为好友
#define	FRIEND_JOIN_REJECT	2

#ifndef IM_TRANSFER
//数据包最大接收,发送字节
#define MAX_RECV_BUFFERS			1440
#define MAX_SEND_BUFFERS			1440
#else
#define MAX_RECV_BUFFERS			1440*50
#define MAX_SEND_BUFFERS			1440*50
#endif

#define MAX_NETWORK_PACKET		(MAX_RECV_BUFFERS + sizeof(XT_HEAD))
#define MAX_REVBUF_LENGTH		MAX_NETWORK_PACKET + sizeof(uint32)
#define MAX_JSON_BUFFER			14400

#define MAX_ERROR_BUF				128				//错误描述长度

//限制
#define FAMILE_MAX_MSGNUM			100         //家族留言信息保存条数
#define MAX_PERMFAMILY_NUM				100			//最多加入固定家族数,待更改，包括被动拖入家族
#define MAX_PERMFAMILYMEM_NUM			100			//普通固定家族成员数
#define MAX_VIPFAMILYMEM_NUM			500		//VIP家族成员数限制
#define MAX_PERMFAMILYMAN_NUM			10			//普通固定家族管理员数限制
#define MAX_VIPFAMILYMAN_NUM			10			//VIP家族管理员数

#define MAX_TEMPFAMILY_NUM				10			//最多加入的临时群数
#define MAX_TEMPFAMILYMEM_NUM			30			//临时群成员数

#define MAX_FAMILEMSG_NUM				100         //群留言信息保存条数
#define MAX_OFFLINEMSG_NUM				20			//好友留言信息保存条数
#define MAX_GROUP_NUM					50			//用户组
#define MAX_FRIENDS_NUM					500         //好友数限制 
#define MAX_CREATEFAMILY_NUM			2			//用户最多创建的固定家族数

//家族基本资料
#define MAX_FAMILYNAME_LEN				100			//家族名长度
#define MAX_FAMILYDOMAIN_LEN			20			//家族域名长度
#define MAX_FAMILYDESC_LEN				500			//家族描述长度
#define MAX_FAMILYFACEURL_LEN			100			//家族形象地址长度

#define OFFLINE_MAX_MSGNUM				20			//好友留言信息保存条数
#define MANAGER_ID						260953		//管理员ID，不限制留言	

#define DAY_SEND_MONEY					1			//每天超过两小时送的金币数


//群名片
#define CPIC_PACKET_LEN				1400
#define MAX_CLUSTER_MEMBER_PROFILE  10				//群名片
#define MAX_MEM_CNT 				30				//群名片最大数量


#pragma pack(push,1)

//应用层数据包头
typedef struct XT_HEAD
{

	/*MINI139包标识 
	0xF0服务器主动发包，
	0xF1服务器应答  
	0xF2客户端请求 
	0xF3客户端应答
	0xF4Web端请求
	0xF5Web端应答
	0xF6Web端服务器主动发包 
	0xFD卖家客户端请求
	*/
	uint8  flag;  
	enum  { 
		FLAG_SRVACTION = 0xF0,
		FLAG_SRVACK = 0xF1, 
		FLAG_CNTREQUEST = 0xF2, 
		FLAG_CNTACK = 0xF3 , 
		FLAG_MOBREQ = 0xF4, 
		FLAG_MOBSRVACK = 0xF5, 
		FLAG_MOBSRVACTION = 0xF6, 
		FLAG_SELLCNTREQ = 0xFD
	};


	uint16 from;  //2  来源,对客户端来说是版本号
	enum { 
		FROM_SERVER = 0x0100, 
		FROM_CLIENT = 0x203, 
		FROM_TRANSFER = 0x0200 
	};

	uint16 cmd;   //3 命令号

	uint32 seq;   //4 命令序列号

	uint32 sid;   //5 源ID (详细说明见协议文档)
	
	enum 
	{ 
		SID_SERVER = 0, //通用的服务器ID,不建议使用
	 };
	
	uint8  dtype;// 6 目标类型 (0x01 用户，0x02 家族，0x03 中转会话成员)
	enum { DEST_TYPE_SERVER = 0x00, DEST_TYPE_USER = 0x01, DEST_TYPE_FAMILY = 0x02 , DEST_TYPE_TRANSMIT = 0x03 };

	uint32 did;   //7 目标ID(详细说明见协议文档)
#ifdef IM_USE_MULTIDEVICE
	uint32 deviceId;
#endif
	uint16 len;  //8 包体长度
	
	XT_HEAD()
	{
		memset(this,0,sizeof(XT_HEAD));
	}



} XT_HEAD, * LPXT_HEAD;

struct SendDataStruct
{
	uint32	dest_ip;//Destination peer IP
	uint16	port;   //host order
	int		retry_times;	//报文重发次数，超过3就丢弃
	uint32	tick_count;     //重发计时器
	uint16	time_interval;	//重发   间隔 
	uint16	data_len;//total data bytes will be sent,include first lead byte
	uint32  vlink;  //virtual link
	char	data[MAX_NETWORK_PACKET];

	SendDataStruct()
	{
		memset(this, 0, sizeof(SendDataStruct));
		retry_times = -1;
	}
};

struct RecvDataStruct
{
	uint32	src_ip;
	uint16  port;
	uint8	src_type;
	uint16  data_len;
	uint32  vlink;  //virtual link
	char	data[MAX_NETWORK_PACKET];

	RecvDataStruct()
	{
		memset(this, 0, sizeof(RecvDataStruct));
	}
};

struct tagClientReply
{
	uint32 sid;
	uint32 seq;
	uint32 vlink;
	tagClientReply()
	{
		sid = 0;
		seq = 0;
		vlink = 0;
	}
};

struct tagServerReply
{
	tagClientReply  requestId;  // 唯一确定该用户的请求消息
	SendDataStruct  replyMsg; // 对该请求消息的回复
};

#pragma pack(pop)

//定义各个服务器的监听端口

//SSDB
#define PORT_SSDB					8888
//全局在线用户管理
#define PORT_ONLINE_USER_MANAGER	10000
//全局在线家族管理
#define PORT_ONLINE_CLUSTER_MANAGER 10001
//IM业务服务器
#define PORT_IM_BUSINESS			10002
//文件转发服务器
#define PORT_TRANSPORT_SERVER		10003
//接入服务器
#define PORT_CONN_SERVER            10004
//站内消息通知服务器
#define PORT_NOTIFY_BUSINESS		10005
//TCP
#define PORT_TCP_BUSINESS		    10006

//memcached
#define PORT_MEMCACHED_ONLINE	    10007

//应用网络层与全局通讯端口
#define PORT_APPNET2GLOBAL          11000  
#define PORT_NOTIFY2GLOBAL          11001

//业务服务器之间通讯端口
#define PORT_IM_SVRP2P              11002
//WEB NOTIFY通讯接口
#define PORT_WEB_NOTIFY             11003

//文件传输服务器与全局接口
#define PORT_TRANS2GLOBAL           11004

//服务器的ID定义
#define SERVERID_CONNECT		1 //接入服务器
#define SERVERID_ONLINE_USER	2 //全局用户在线管理
#define SERVERID_ONLINE_CLUSTER 3 //全局家族在线管理
#define SERVERID_TRANSFER		-4 //中转服务器





//发送重试次数
#define SEND_RETRY_TIME				4		
#define PIC_RETRY_TIME				10	







//最大的139用户数
#define MAX_139_ACCOUNT_NUM  10000000
#define MAX_SubMerchantID    10000000000

//错误值
#define RESULT_SUCCESS 0
#define RESULT_FAILED  1

/*define MEMCACHED_ERROR_TYPE*/

#define MEMCACHED_GET_INFO_SUCCESS 0

#define MEMCACHED_SERVER_DOWN 1

#define MEMCACHED_NO_INFO 2

#define MEMCACHED_JASON_ERROR 3

#define CS_SUCCESS   0
#define NO_USABLE_CS 1
#define NO_CS_ONLINE 2

#define MEMCACHED_USER_LOCK 1
#define MEMCACHED_CS_LOCK 2
#define MEMCACHED_QUEUE_LOCK 3

/*end define MEMCACHED_ERROR_TYPE*/

#define CLIENT_FORCE_RELOGIN 0xAA

#define MAX_PROTOLPACKET_LEN ( MAX_SEND_BUFFERS - sizeof(XT_HEAD)) 

//检查用户类型
#define TYPE_CLIENT 0
#define TYPE_WEB 1
#define TYPE_NULL 2

#define MAX_MSG_BUFFER_NUM 	512
#define MAX_MSG_PTHREAD_NUM 1
#define FILE_PACKET_LEN 	1240

//ID小于500000为卖家id,大于5000000为买家
#define Id_Boundary 5000000
#define cs_Boundary 2000000000
#define work_Boundary 2050000000	//工作版

#define seller_Timeout 210

#define buyer_First_Timeout 86400
#define buyer_Timeout 700
#define buyer_Quit_Timeout 60
#define LOGOUT_TIMEOUT 10
//获取好友信息数量
#define GET_FRIEND_NUMBER 150

//卖家动作
#define ACTION_ERROR     0
#define ACTION_LOGIN     1
#define ACTION_LOGOUT    2
#define ACTION_HANGUP    3
#define ACTION_RECEPTION 4
#define ACTION_LEAVE 	 5
#define ACTION_NOT_LEAVE 6


#define MYGROUP_ID 250
 
#define MAX_QueryUserStatus_NUM  50

#endif

