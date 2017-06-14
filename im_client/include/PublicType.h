/************************************************************************ 
 文件描述：公共类型定义
 
 创建人： zouyf, 2006-6-9 
 
 修改记录： 
************************************************************************/

#ifndef __ZPUBLICTYPE_H_
#define __ZPUBLICTYPE_H_

// beta环境、preview版本、online版本 协议号一致
//客服工作台协议号和商家版协议号区分
#ifdef _CS_PLAT_EX_
	#define CLIENT_VERSION 2400
#elif defined _WORK_EX_
	#define CLIENT_VERSION 2300
#else
	#define CLIENT_VERSION 2400
#endif


#ifndef HAS_COMMON_TYPEDEF
#define HAS_COMMON_TYPEDEF

typedef unsigned __int64	uint64;
typedef unsigned int		uint32;
typedef unsigned short		uint16;
typedef unsigned char		uint8;
typedef __int64				int64;
typedef int					int32;
typedef short				int16;
typedef char				int8;

#endif // !HAS_COMMON_TYPEDEF

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



#define PERM_FAMILY				1	//固定群
#define TEMP_FAMILY				2	//临时群

#define FAMILY_COMMON				0	//普通群
#define FAMILY_VIP					1	//VIP群

//群成员操作Action
#define CMEMBER_ADD					0	//添加
#define CMEMBER_DEL					1	//删除
#define CMEMBER_SET_ADMIN			2	//设为管理员
#define CMEMBER_CANCEL_ADMIN		3	//取消管理员
#define CMEMBER_INVITE_REFUSE		4	//被邀请方拒绝入群
#define CMEMBER_CREATOR_REFUSE		5	//群主拒绝被邀请方入群
#define CMEMBER_PROFILE_MOD		    6	//修改群名片

#define FAMILY_REQ_JOIN				CMEMBER_ADD	//申请加入固定群
#define FAMILY_REQ_QUIT				CMEMBER_DEL	//请求退出群

//群验证方式
#define FAMILY_JOIN_ANY				0	//任意加入
#define FAMILY_JOIN_VALIDATE		1	//验证
#define FAMILY_JOIN_UNALLOW			2	//拒绝

//请求加入群消息
#define FAMILY_JOIN_REQUEST			0	//请求加入
#define FAMILY_JOIN_AGREE			1	//同意加入
#define FAMILY_JOIN_REFUSE			2	//拒绝加入

//群页面浏览权限
#define FAMILY_VIEW_MEM				1	//群成员（私有）
#define FAMILY_VIEW_REG				2	//注册用户（半公开）
#define FAMILY_VIEW_ALL				3	//所有（公开）

//群成员属性
#define FAMILY_CREATER				2	//群创建者
#define FAMILY_MANAGER				1	//群管理员
#define FAMILY_NORMAL				0	//普通成员



//#define MAX_NAME_LEN				24

//用户资料基本信息长度定义

#define MAX_USERNAME_LEN			120				//用户名 长度40
#define MAX_USERNAME_LEN_LAST       40              //用户名原长度40，用于兼容上一个版本读取用户名文件
#define MAX_NICKNAME_LEN			40				//呢称 原长度32，原长度30
#define MAX_HEADURL_LEN				100				//自定义头像图片URL
#define MAX_EMAIL_LEN				60				//Email
#define MAX_PASSWORD_LEN			40				//数 据库存储加密后的长度
#define MAX_PASSWORD_LEN_LAST		40				//数据库存储加密后的原长度，用于兼容上一个版本读取用户名文件
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
#define MAX_CLUSTER_MEMBER_PROFILE  10				//群名片
#define MAX_GUID_LENGTH             100				//自营GUID内容
#define MAX_JOB_LENGTH				40				//职务
#define MAX_DEPT_LENGTH				100				//部门
#define MAX_UUID_LEN				36				//uuid
//好友加入验证方式

//允许任何人加为好友
#define FRIEND_JOIN_ANYONE	0
//需要验证
#define FRIEND_JOIN_VERIFY	1
//拒绝任何人加为好友
#define	FRIEND_JOIN_REJECT	2


//数据包最大接收,发送字节
#define MAX_RECV_BUFFERS			1440
#define MAX_SEND_BUFFERS			1440

// 文件传输数据包buf大小
#define MAX_FILE_TRAN_RECV_BUF MAX_RECV_BUFFERS*40
#define MAX_FILE_TRAN_SEND_BUF MAX_SEND_BUFFERS*40

#define MAX_NETWORK_PACKET		(MAX_RECV_BUFFERS + sizeof(XT_HEAD))

// 文件传输时包大小
#define MAX_FILE_TRAN_NETWORK_PACKET		(MAX_FILE_TRAN_RECV_BUF + sizeof(XT_HEAD))

#define MAX_ERROR_BUF				128				//错误描述长度

//限制
#define FAMILE_MAX_MSGNUM			100         //群留言信息保存条数
#define MAX_PERMFAMILY_NUM				100			//最多加入固定群数,待更改，包括被动拖入群
#ifdef _BUSINESS_VESION
#define MAX_PERMFAMILYMEM_NUM			1000		//卖家版普通固定群成员数
#else
#define MAX_PERMFAMILYMEM_NUM			500			//普通固定群成员数
#endif
#define MAX_VIPFAMILYMEM_NUM			300			//VIP群成员数限制
#define MAX_PERMFAMILYMAN_NUM			3			//普通固定群管理员数限制
#define MAX_VIPFAMILYMAN_NUM			4			//VIP群管理员数

#define MAX_TEMPFAMILY_NUM				10			//最多加入的临时群数
#define MAX_TEMPFAMILYMEM_NUM			30			//临时群成员数

#define MAX_FAMILEMSG_NUM				100         //群留言信息保存条数
#define MAX_OFFLINEMSG_NUM				20			//好友留言信息保存条数
#define MAX_GROUP_NUM					5			//用户组
#define MAX_FRIENDS_NUM					300         //好友数限制 
#define MAX_CREATEFAMILY_NUM			100			//用户最多创建的固定群数

//群基本资料
#define MAX_FAMILYNAME_LEN				100			//群名长度
#define MAX_FAMILYDOMAIN_LEN			20			//群域名长度
#define MAX_FAMILYDESC_LEN				500			//群描述长度
#define MAX_FAMILYFACEURL_LEN			10			//群名片长度

#define OFFLINE_MAX_MSGNUM				20			//好友留言信息保存条数
#define MANAGER_ID						260953		//管理员ID，不限制留言	

#define DAY_SEND_MONEY					1			//每天超过两小时送的金币数

#define MIN_PSW_LEN		                6           //新密码最小长度
#define MAX_PSW_LEN		                18          //新密码最大长度

#pragma pack(push,1)

typedef unsigned __int64	uint64;
typedef unsigned int		uint32;
typedef unsigned short		uint16;
typedef unsigned char		uint8;
typedef __int64				int64;
typedef int					int32;
typedef short				int16;
typedef char				int8;

//应用层数据包头
typedef struct XT_HEAD
{
	uint8  flag;  //1  MINI139包标识 0xF0服务器主动发包，0xF1服务器应答  0xF2客户端请求 0xF3客户端应答 0xFD卖家版标志，现只在获取好友列表以及组信息时用到
	enum  { FLAG_SRVACTION = 0xF0, FLAG_SRVACK = 0xF1, FLAG_CNTREQUEST = 0xF2, FLAG_CNTACK = 0xF3, FLAG_SELLCNTREQ = 0xFD };


	uint16 from;  //2  来源,对客户端来说是版本号
	enum { FROM_SERVER = 0x0100, FROM_CLIENT = CLIENT_VERSION, FROM_TRANSFER = 0x0200 };

	uint16 cmd;   //3 命令号

	uint32 seq;   //4 命令序列号

	uint32 sid;   //5 源ID (详细说明见协议文档)
	
	enum 
	{ 
		SID_SERVER = 0, //通用的服务器ID,不建议使用
	 };
	
	uint8  dtype;// 6 目标类型 (0x01 用户，0x02 群，0x03 中转会话成员)
	enum { DEST_TYPE_SERVER = 0x00, DEST_TYPE_USER = 0x01, DEST_TYPE_FAMILY = 0x02 , DEST_TYPE_TRANSMIT = 0x03 };

	uint32 did;   //7 目标ID(详细说明见协议文档)

#ifdef _IM_USE_MULTIDEVICE_
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
	char	data[MAX_FILE_TRAN_NETWORK_PACKET];

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
	uint8	src_type;	// P2P_ENDPOINT_TYPE  
	uint16  data_len;
	uint32  vlink;		// virtual link
	char	data[MAX_FILE_TRAN_NETWORK_PACKET];

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

typedef struct _SESSION_ID
{
	uint32		uid;
	uint32		fid;

	_SESSION_ID()
	{
		uid = -1;
		fid = -1;
	}

	bool	IsValid() const
	{
		return uid != -1 && fid != -1;
	}

	bool operator < (_SESSION_ID  o) const
	{
		return (uid < o.uid) || ((uid == o.uid) && (fid < o.fid));
	}

	bool operator == (_SESSION_ID  o) const
	{
		return (uid == o.uid) && (fid == o.fid);
	}

} SESSION_ID;

class CIStream
{
public:
	virtual const char * Read( /*IN-OUT*/int & nLen) = 0;
};


#pragma pack(pop)

//定义各个服务器的监听端口

//全局在线用户管理
#define PORT_ONLINE_USER_MANAGER	10000
//全局在线群管理
#define PORT_ONLINE_CLUSTER_MANAGER 10001
//IM业务服务器
#define PORT_IM_BUSINESS			10002
//文件转发服务器
#define PORT_TRANSPORT_SERVER		10003
//接入服务器
#define PORT_CONN_SERVER            10004

//应用网络层与全局通讯端口
#define PORT_APPNET2GLOBAL          11000  


//服务器的ID定义
#define SERVERID_CONNECT		1 //接入服务器
#define SERVERID_ONLINE_USER	2 //全局用户在线管理
#define SERVERID_ONLINE_CLUSTER 3 //全局群在线管理
#define SERVERID_TRANSFER		-4 //中转服务器





//发送重试次数
#define SEND_RETRY_TIME				4		
#define PIC_RETRY_TIME				10	







//最大的139用户数
#define MAX_139_ACCOUNT_NUM  10000000

//错误值
#define RESULT_SUCCESS 0
#define RESULT_FAILED  1

#define MAX_PROTOLPACKET_LEN ( MAX_SEND_BUFFERS - sizeof(XT_HEAD)) 




#endif
