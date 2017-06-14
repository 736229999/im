/************************************************************************ 
 文件描述：服务器框架协议定义，定义服务器之间的交互协议
		   
 创建人： zouyf, 2006-6-9 
 
 修改记录： 
************************************************************************/

#ifndef __SRVFRAMEPROTOCOL_H_
#define __SRVFRAMEPROTOCOL_H_
#include "protocol.h"
#include "ClientProtocol.h"

#pragma pack(push,1)

//协议定义 ...

/***
全局服务器业务协议
***/
//文件传输服务器查询用户信息
typedef struct tagGSRV_TUSERINFO_QUERY
{
	uint32 id;
}GSRV_TUSERINFO_QUERY,*LGSRV_TUSERINFO_QUERY;

typedef struct tagGSRV_TUSERINFO_ACK
{
	uint8 ret;
	uint32 id;
	uint32 ip;
	uint16 port;
	char sessionkey[MAX_SESSIONKEY_LEN];
}GSRV_TUSERINFO_ACK,*LGSRV_TUSERINFO_ACK;

//查询在线用户控制信息
typedef struct tagGSRV_USERCONTROLINFO_QUERY
{
	uint8 count;
	uint32 uid[1];
}GSRV_USERCONTROLINFO_QUERY, * LPGSRV_USERCONTROLINFO_QUERY;

//查询在线用户控制信息应答
typedef struct tagGSRV_USERCONTROLINFO_QUERY_ACK
{
	uint8 ret;
	uint8 count;
	struct tagUserControlInfo
	{
		uint32 uid;
		uint32 srvip;
		char sessionKey[MAX_SESSIONKEY_LEN];
		char p2pSessionKey[MAX_SESSIONKEY_LEN];
		uint8 flag;
	}userControlInfo[1];
}GSRV_USERCONTROLINFO_QUERY_ACK, * LPGSRV_USERCONTROLINFO_QUERY_ACK;

//查找在线用户的返回应答
typedef struct tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD
{
	uint8 result;
	uint8  count;
	uint32 onlineNum;
	struct tagUserInfo
	{
		uint32 clientid;
		uint8	sexy;
		uint8	status;
		uint8	rule;
		char	username[MAX_USERNAME_LEN+1];
		char	nickname[MAX_NICKNAME_LEN+1];
	}UserInfo[1];
} GSRV_SEARCH_ONLINE_USER_ACK_HEAD,*LPGSRV_SEARCH_ONLINE_USER_ACK_HEAD;


//获取好友（自己）基本资料的请求包结构
// typedef struct tagIMSRV_GET_FRIEND_BASICINFO_REQ
// {
// 	uint8  count;
// 	uint32 FriendID[1];
// 
// }IMSRV_GET_FRIEND_BASICINFO_REQ,*LPIMSRV_GET_FRIEND_BASICINFO_REQ;
//获取好友（自己）基本资料的应答包结构

// typedef struct tagIMSRV_GET_FRIEND_BASICINFO_ACK
// {
// 	uint8  result;
// 	uint8  count;
// 
// 	struct tagFriendInfo
// 	{
// 		uint32  nID;
// 		char	szHeadUrl[MAX_HEADURL_LEN+1];
// 		uint8	nSexy;
// 		char	szNickName[MAX_NICKNAME_LEN+1];
// 		char	szUserName[MAX_USERNAME_LEN+1];
// 		uint16	nVersion;
// 
// 	} FriendInfo[1];
// 
// }IMSRV_GET_FRIEND_BASICINFO_ACK,*LPIMSRV_GET_FRIEND_BASICINFO_ACK;


//查询在线用户状态请求
typedef struct tagGSRV_GET_USER_STATUS_QUERY
{
	uint8	count;
	uint32	userid[1];
} GSRV_GET_USER_STATUS_QUERY,*LPGSRV_GET_USER_STATUS_QUERY;

//查询在线用户状态应答
typedef struct tagGSRV_GET_USER_STATUS_ACK
{
	uint8 result;
	uint8 count;
	struct tagUserStatus
	{
		uint32 id;
		uint8  status;	
	} UserStatus[1];
} GSRV_GET_USER_STATUS_ACK ,*LPGSRV_GET_USER_STATUS_ACK;

//修改在线用户状态请求
typedef struct tagGSRV_MODIFY_ONLINE_STATUS_QUERY
{
	uint8 count;
	struct tagUser
	{
		uint32 id;
		uint8  status;
	} user[1];
} GSRV_MODIFY_ONLINE_STATUS_QUERY,*LPGSRV_MODIFY_ONLINE_STATUS_QUERY;

//修改在线用户状态应答
typedef struct tagGSRV_MODIFY_ONLINE_STATUS_ACK
{
	uint8 result;
	uint8 count;
	struct tagUser
	{
		uint32 id;
		uint8  status;
	} user[1];
} GSRV_MODIFY_ONLINE_STATUS_ACK,*LPGSRV_MODIFY_ONLINE_STATUS_ACK;

//获取最佳子账号请求
typedef struct tagGSRV_GET_BEST_ACCOUNT_QUERY
{
	uint32 merchant_id;
	uint32 group_id;
	uint32 flow_type;
	uint8 count;
	uint32 id[1];
}GSRV_GET_BEST_ACCOUNT_QUERY,*LPGSRV_GET_BEST_ACCOUNT_QUERY;

//获取最佳子账号应答
typedef struct tagGSRV_GET_BEST_ACCOUNT_ACK
{
	uint8 result;
	uint32 account;
}GSRV_GET_BEST_ACCOUNT_ACK,*LPGSRV_GET_BEST_ACCOUNT_ACK;
/*
	------------------分流挂起------------------
*/

//查询分流挂起请求
typedef struct tagGSRV_GET_USER_FLOW_QUERY
{
	uint8	count;
	uint32	userid[1];
} GSRV_GET_USER_FLOW_QUERY,*LPGSRV_GET_USER_FLOW_QUERY;

//查询分流挂起应答
typedef struct tagGSRV_GET_USER_FLOW_ACK
{
	uint8 result;
	uint8 count;
	struct tagUserFlow
	{
		uint32 id;
		uint8  status;
		uint8  flow;
	} UserFlow[1];
} GSRV_GET_USER_FLOW_ACK ,*LPGSRV_GET_USER_FLOW_ACK;


//修改分流挂起请求
typedef struct tagGSRV_MODIFY_USER_FLOW_QUERY
{
	uint8 count;
	struct tagFlow
	{
		uint32 id;
		uint8  flow;
	} user[1];
} GSRV_MODIFY_USER_FLOW_QUERY,*LPGSRV_MODIFY_USER_FLOW_QUERY;

//修改分流挂起应答
typedef struct tagGSRV_MODIFY_USER_FLOW_ACK
{
	uint8 result;
	uint8 count;
	struct tagUserFlow
	{
		uint32 id;
		uint8  status;
		uint8  flow;
	} user[1];
} GSRV_MODIFY_USER_FLOW_ACK,*LPGSRV_MODIFY_USER_FLOW_ACK;


//获取在线用户全局详细信息请求
typedef struct tagGSRV_GET_ONLINE_FULLINFO_QUERY
{
	uint8 count;
	uint32 userid[1];
} GSRV_GET_ONLINE_FULLINFO_QUERY,*LPGSRV_GET_ONLINE_FULLINFO_QUERY;

//获取在线用户全局详细信息应答
typedef struct tagGSRV_GET_ONLINE_FULLINFO_ACK
{
	uint8 result;
	uint8 count;
	struct UserInfo
	{ 
		uint32	id;
		uint8	status;
		uint8 	rule;
		uint16	version;
		char	username[MAX_USERNAME_LEN+1];
		char	nickname[MAX_NICKNAME_LEN+1];
		uint32	clientip;
		uint16	clientport;
		uint8	sexy;
		uint8 	flow;
		uint32	serverip;
		uint32  lanip;
		char	sessionkey[MAX_SESSIONKEY_LEN];
		char	p2psessionkey[MAX_SESSIONKEY_LEN];
	} user[1];
 
} GSRV_GET_ONLINE_FULLINFO_ACK,*LPGSRV_GET_ONLINE_FULLINFO_ACK;

//添加/修改全局用户资料
typedef struct tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY
{
	enum action { ACTION_ADD = 0,ACTION_MODIFY = 1 };
	uint8	action;
	uint8	count;
	struct UserInfo
	{ 
		uint32 id;
		uint8  status;
		uint8	rule;
		char	username[MAX_USERNAME_LEN+1];
		char	nickname[MAX_NICKNAME_LEN+1];
		uint32	clientip;
		uint16	clientport;
		uint8	sexy;
		uint8 	flow;
		uint32	serverip;
		uint32	lanip;	//add by lwl ver:0101
//		uint16	lanport;
		char	sessionkey[MAX_SESSIONKEY_LEN];
		char	p2psessionkey[MAX_SESSIONKEY_LEN];
	} user[1];
	tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY()
	{
		memset(this,0,sizeof(tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY));
	}
}GSRV_ADDMODIFY_FULLUSERINFO_QUERY,*LPGSRV_ADDMODIFY_FULLUSERINFO_QUERY;

//添加/修改全局用户资料应答
typedef struct tagGSRV_ADDMODIFY_FULLUSERINFO_ACK
{
	uint8 result;
	uint8 count;
	uint32 user[1];
}GSRV_ADDMODIFY_FULLUSERINFO_ACK,*LPGSRV_ADDMODIFY_FULLUSERINFO_ACK;

//删除用户全局资料请求
typedef struct tagGSRV_DEL_ONLINE_USER_QUERY
{
	uint8	count;
	uint32	userid[1];
}GSRV_DEL_ONLINE_USER_QUERY,*LPGSRV_DEL_ONLINE_USER_QUERY;

//删除用户全局资料应答
typedef struct tagGSRV_DEL_ONLINE_USER_ACK
{
	uint8	result;
	uint8	count;
	uint32	userid[1];
}GSRV_DEL_ONLINE_USER_ACK,*LPGSRV_DEL_ONLINE_USER_ACK;

//通知业务服务器用户已重登录
typedef struct tagGSRV_NOTIFY_RELOGIN
{
	uint32 userid[1];
}GSRV_NOTIFY_RELOGIN,*LPGSRV_NOTIFY_RELOGIN;

//-----------家族全局在线操作---------------------------------

//固定家族
#define FAMILYINFO_QUERY_TYPE_PERM PERM_FAMILY
//临时家族
#define FAMILYINFO_QUERY_TYPE_TEMP TEMP_FAMILY

//查询在线家族全局资料
typedef struct tagGSRV_FAMILYINFO_QUERY
{
	uint8 count;
	uint8 type;
	uint32 fid[1];
}GSRV_FAMILYINFO_QUERY, * LPGSRV_FAMILYINFO_QUERY;

//查询在线家族全局资料应答
typedef struct tagGSRV_FAMILYINFO_QUERY_ACK
{
	uint8 ret;
	uint8 count;
	struct tagFamilyInfo
	{
		uint32 fid;
		uint32 srvip;
	}FamilyInfo[1];
}GSRV_FAMILYINFO_QUERY_ACK, * LPGSRV_FAMILYINFO_QUERY_ACK;

//添加全局家族资料
typedef struct tagGSRV_FAMILYINFO_ADD_QUERY
{
	uint8  count;
	uint8  type;
	struct tagFamilyInfo
	{
		uint32 FamilyID;
		uint32 ServerIp;
	} FamilyInfo[1];

}GSRV_FAMILYINFO_ADD_QUERY,*LPGSRV_FAMILYINFO_ADD_QUERY;

//添加全局家族资料应答
typedef struct tagGSRV_FAMILYINFO_ADD_ACK
{
	uint8	result;
	uint8	count;
	uint32	FamilyID[1];

}GSRV_FAMILYINFO_ADD_ACK,*LPGSRV_FAMILYINFO_ADD_ACK;


//删除全局家族资料
typedef struct tagGSRV_FAMILYINFO_DEL_QUERY
{
	uint8  count;
	uint8  type;
	uint32 FamilyID[1];
}GSRV_FAMILYINFO_DEL_QUERY,*LPGSRV_FAMILYINFO_DEL_QUERY;

//删除全局家族资料应答
typedef struct tagGSRV_FAMILYINFO_DEL_ACK
{
	uint8	result;
	uint8	count;
	uint32	FamilyID[1];

}GSRV_FAMILYINFO_DEL_ACK,*LPGSRV_FAMILYINFO_DEL_ACK;

//------------------集群服务业务----------------------------------------------

//更新用户/家族定位信息通知
typedef struct tagCS_UPDATELOCATION_NOTIFY
{
	uint8       emDestType;
	uint32      destID;
}CS_UPDATELOCATION_NOTIFY, * LPCS_UPDATELOCATION_NOTIFY;


/***
接入服务器、动态负载业务协议定义
***/
//服务器注册
//#define CMD_SERVER_REGISTER             0x8001
typedef struct tagSERVER_REGISTER
{
	uint16 maxCount;
	uint32 dnsIP;
	uint16 udpPort;
	uint16 tcpPort;
	uint8  flags; //0x00 普通服务器 0x01 vip服务器
	enum {FLAGS_NORMAL = 0x00,FLAGS_VIP = 0x01};
	uint8  srvType; //0x01 业务服务器 0x02 中转服务器
	enum {TYPE_BUSI = 0x01,TYPE_TRANS = 0x02};
}SERVER_REGISTER, * LPSERVER_REGISTER;

//#define CMD_SERVER_REGISTER_ACK         0x8002
typedef struct tagSERVER_REGISTER_ACK
{
	uint8 ret;
}SERVER_REGISTER_ACK, * LPSERVER_REGISTER_ACK;


//服务器注销
//#define CMD_SERVER_UNREGISTER           0x8003
typedef struct tagSERVER_UNREGISTER
{
	uint8 authData; //校验数据 = 0x00
}SERVER_UNREGISTER, * LPSERVER_UNREGISTER;



//服务器状态报告
//#define CMD_SERVER_STATUS_REPORT        0x8005
typedef struct tagSERVER_STATUS_REPORT
{
	uint16 linkCount;
	uint8  capValue;
}SERVER_STATUS_REPORT, * LPSERVER_STATUS_REPORT;

//服务器状态列表请求
//#define CMD_SERVER_STATUSLIST_REQ       0x8007
typedef struct tagSERVER_STATUSLIST_REQ
{
	uint16 startPos;
	uint8  srvType;  // 0x01 业务服务器 0x02 中转服务器

}SERVER_STATUSLIST_REQ, * LPSERVER_STATUSLIST_REQ;
//#define CMD_SERVER_STATUSLIST_REQ_ACK   0x8008
struct tagSrvStatus
{
	uint32 srvID;
	SERVER_REGISTER srvBaseInfo;
	SERVER_STATUS_REPORT srvDynamicInfo;
};
typedef struct tagSERVER_STATUSLIST_REQ_ACK
{
	uint8 ret;
	uint16 nextPos;
	uint8 retCount;
	tagSrvStatus SrvStatus[1];
}SERVER_STATUSLIST_REQ_ACK, * LPSERVER_STATUSLIST_REQ_ACK;


//IM业务服务器停机通知
typedef struct tagSERVER_IMSERVER_POWEROFF_NOTIFY
{
	uint32	severIp;
}SERVER_IMSERVER_POWEROFF_NOTIFY,*LPSERVER_IMSERVER_POWEROFF_NOTIFY;
//IM业务服务器停机通知应答
typedef struct tagSERVER_IMSERVER_POWEROFF_ACK
{
	uint32	severIp;
}SERVER_IMSERVER_POWEROFF_ACK,*LPSERVER_IMSERVER_POWEROFF_ACK;




//服务器透明转发客户端数据
//#define CMD_SERVER_CLARITY_TRANSMIT     0x8009
typedef struct tagSERVER_CLARITY_TRANSMIT
{
	char data[1]; //客户端自定义的协议数据，用客户端自己的p2p session key 加密
}SERVER_CLARITY_TRANSMIT, * LPSERVER_CLARITY_TRANSMIT;


typedef struct tagSERVER_RELOGIN_NOTIFY
{
	uint32 userid;
	uint8 chgpwd;
	uint32 userip;
	uint32 lanip;

} SERVER_RELOGIN_NOTIFY,*LPSERVER_RELOGIN_NOTIFY;

typedef struct tagSERVER_RELOGIN_ACK
{
	uint32 userid;

} SERVER_RELOGIN_ACK,*LPSERVER_RELOGIN_ACK;

#pragma pack(pop)
#endif
