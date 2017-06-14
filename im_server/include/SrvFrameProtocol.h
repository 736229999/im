/************************************************************************ 
 文件描述：服务器框架协议定义，定义服务器之间的交互协议
		   
 创建人： zouyf, 2006-6-9 
 
 修改记录： 
************************************************************************/

#ifndef __SRVFRAMEPROTOCOL_H_
#define __SRVFRAMEPROTOCOL_H_
#include "protocol.h"
#include "ClientProtocol.h"
#include <vector>
using std::vector;

//#pragma pack(push,1)

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
	uint8 flag;
	uint32 newlylivetime;	// 登陆时长
	uint32 merchantID;		// merchantID
	uint32 serverip;		// 服务器ip	
}GSRV_TUSERINFO_ACK,*LGSRV_TUSERINFO_ACK;

typedef struct tagGSRV_TUSERINFO_ACK_EX
{
	uint8 ret;
	uint32 id;
	uint32 ip;
	uint16 port;
	char sessionkey[MAX_SESSIONKEY_LEN];
	uint8 flag;
	uint32 newlylivetime;	// 登陆时长
	uint32 merchantID;		// merchantID
	uint32 serverip;		// 服务器ip	
	uint8 status;			// 状态
	uint8 flow;				// 分流状态	
}GSRV_TUSERINFO_ACK_EX,*LGSRV_TUSERINFO_ACK_EX;

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
		uint8 teamID;	
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
		uint32  logintime;
		char	sessionkey[MAX_SESSIONKEY_LEN];
		char	p2psessionkey[MAX_SESSIONKEY_LEN];
#ifdef IM_USE_CACHE
		uint8 	flag;					 //登录方式
		uint32  teamID;				 //客服模式下用户接待的组teamID
		
#endif
	} user[1];
 
} GSRV_GET_ONLINE_FULLINFO_ACK,*LPGSRV_GET_ONLINE_FULLINFO_ACK;



//获取在线用户全局监控信息
typedef struct tagGSRV_GET_ONLINE_MONITOR_ACK
{
	uint8 result;
	uint8 count;
	struct UserInfo
	{ 
		uint32	id;
		uint8	status;
		char	username[MAX_USERNAME_LEN+1];				
		bool    monitored;	// 客服是否被监控
		uint32  chattingBuyerID;		// 客服当前正在联系的买家（用于监控过程中，将正在联系的买家告知主管）
		uint32  directorID;	// 如果客服被监控，记录监控他的主管id
	} user[1];
 
} GSRV_GET_ONLINE_MONITOR_ACK,*LPGSRV_GET_ONLINE_MONITOR_ACK;


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
		uint32  logintime;
//		uint16	lanport;
		char	sessionkey[MAX_SESSIONKEY_LEN];
		char	p2psessionkey[MAX_SESSIONKEY_LEN];
#ifdef IM_USE_CACHE		
		uint8 	flag;					 //登录方式
		uint32  teamID;				 //客服模式下用户接待的组teamID
#endif
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



//获取在线客服全局详细信息请求
typedef struct tagGSRV_GET_ONLINE_CSINFO_QUERY
{
	uint8 count;
	uint32 userid[1];
} GSRV_GET_ONLINE_CSINFO_QUERY,*LPGSRV_GET_ONLINE_CSINFO_QUERY;


typedef struct tagCSinfo
{
	uint32 m_nID;				//当前clientservice的ID号
	char m_Csname[MAX_USERNAME_LEN]; 	
	int m_State;
	int m_MaxRecConsult;		//最大咨询接待量		
	int m_CurReception; //当前接待量
	int m_CurConsult;		//当前咨询量	
	int m_Permission;	//当前用户权限
	uint32 m_nChattingBuyerID;		// 客服当前正在联系的买家（用于监控过程中，将正在联系的买家告知主管）
	uint32 m_nDirectorID;	// 如果客服被监控，记录监控他的主管id,如果没有被监控，写入-1 
	vector<uint32> m_Customerid;//向客服咨询的用户[用户id-----用户信息]
}CSinfo_t;


//获取在线客服全局详细信息应答
typedef struct tagGSRV_GET_ONLINE_CSINFO_ACK
{
	uint8 result;
	uint8 count;
	CSinfo_t CSinfo[1];
 
} GSRV_GET_ONLINE_CSINFO_ACK,*LPGSRV_GET_ONLINE_CSINFO_ACK;

//添加/修改全局客服资料
typedef struct tagGSRV_ADDMODIFY_CSINFO_QUERY
{
	enum action { ACTION_ADD = 0,ACTION_MODIFY = 1 };
	uint8	action;
	uint8	count;
	CSinfo_t 	csinfo[1];
	
	tagGSRV_ADDMODIFY_CSINFO_QUERY()
	{
		memset(this,0,sizeof(tagGSRV_ADDMODIFY_CSINFO_QUERY));
	}
}GSRV_ADDMODIFY_CSINFO_QUERY,*LPGSRV_ADDMODIFY_CSINFO_QUERY;

//添加/修改全局用户资料应答
typedef struct tagGSRV_ADDMODIFY_CSINFO_ACK
{
	uint8 result;
	uint8 count;
	uint32 user[1];
}GSRV_ADDMODIFY_CSINFO_ACK,*LPGSRV_ADDMODIFY_CSINFO_ACK;

//删除用户全局资料请求
typedef struct tagGSRV_DEL_ONLINE_CSINFO_QUERY
{
	uint8	count;
	uint32	userid[1];
}GSRV_DEL_ONLINE_CSINFO_QUERY,*LPGSRV_DEL_ONLINE_CSINFO_QUERY;

//删除用户全局资料应答
typedef struct tagGSRV_DEL_ONLINE_CSINFO_ACK
{
	uint8	result;
	uint8	count;
	uint32	userid[1];
}GSRV_DEL_ONLINE_CSINFO_ACK,*LPGSRV_DEL_ONLINE_CSINFO_ACK;



//获取在线客服排队全局详细信息请求
typedef struct tagGSRV_GET_ONLINE_CSQINFO_QUERY
{
	uint8 count;
	uint32 teamID;
} GSRV_GET_ONLINE_CSQINFO_QUERY,*LPGSRV_GET_ONLINE_CSQINFO_QUERY;

typedef struct CSQList
{
	uint32 userid;		//用户ID
	uint32 requestTime;	//请求时间
	uint32 Level;		//用户等级
	uint32 teamID;		//用户所在组
}CSQList_t;


//获取在线客服排队全局详细信息应答
typedef struct tagGSRV_GET_ONLINE_CSQINFO_ACK
{
	uint8 result;
	uint8 count;
	struct CSQinfo
	{ 
		uint32 teamID;				//当前clientservice的ID号
		vector<CSQList_t> m_Customerlist;//向客服咨询的用户[用户id-----用户信息]
	} CSQInfo[1];
 
} GSRV_GET_ONLINE_CSQINFO_ACK,*LPGSRV_GET_ONLINE_CSQINFO_ACK;

//获取在线客服排队全局详细信息应答
typedef struct tagGSRV_GET_ONLINE_CSQLEN_ACK
{
	uint8 result;
	uint8 count;
	struct CSQinfo
	{ 
		uint32 teamId;				//当前teamID
		uint32 queueLength;			//当前teamID中排队数量
	} CSQInfo[1];
 
} GSRV_GET_ONLINE_CSQLEN_ACK,*LPGSRV_GET_ONLINE_CSQLEN_ACK;


//添加/修改全局客服排队资料
typedef struct tagGSRV_ADDMODIFY_CSQINFO_QUERY
{
	enum action { ACTION_ADD = 0,ACTION_MODIFY = 1 ,ACTION_DEL = 2};
	uint8	action;
	uint8	count;	
	struct CSinfo
	{		
		uint32 teamID;				//当前客服的组号
		uint32 customerID;			//当前排队用户
		uint32 customerLevel;		//当前排队用户等级
		uint32 customerReqTime;		//当前排队用户登录时间
	} csInfo[1];
	
	tagGSRV_ADDMODIFY_CSQINFO_QUERY()
	{
		memset(this,0,sizeof(tagGSRV_ADDMODIFY_CSINFO_QUERY));
	}
}GSRV_ADDMODIFY_CSQINFO_QUERY,*LPGSRV_ADDMODIFY_CSQINFO_QUERY;

//添加/修改全局客服排队资料应答
typedef struct tagGSRV_ADDMODIFY_CSQINFO_ACK
{
	uint8 result;
	uint8 count;
	uint32 user[1];
}GSRV_ADDMODIFY_CSQINFO_ACK,*LPGSRV_ADDMODIFY_CSQINFO_ACK;

//删除全局客服排队资料请求
typedef struct tagGSRV_DEL_ONLINE_CSQINFO_QUERY
{
	uint8	count;
	uint32	teamID[1];
}GSRV_DEL_ONLINE_CSQINFO_QUERY,*LPGSRV_DEL_ONLINE_CSQINFO_QUERY;

//删除全局客服排队资料应答
typedef struct tagGSRV_DEL_ONLINE_CSQINFO_ACK
{
	uint8	result;
	uint8	count;
	uint32	userid[1];
}GSRV_DEL_ONLINE_CSQINFO_ACK,*LPGSRV_DEL_ONLINE_CSQINFO_ACK;

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


//查询在线用户组teamID请求
typedef struct tagGSRV_GET_USER_TEAMID_QUERY
{
	uint8	count;
	uint32	userid[1];
} GSRV_GET_USER_TEAMID_QUERY,*LPGSRV_GET_USER_TEAMID_QUERY;


//查询在线用户组teamID应答
typedef struct tagGSRV_GET_USER_TEAMID_ACK
{
	uint8 result;
	uint8 count;
	struct tagUserTeamID
	{
		uint32 id;
		uint8  teamID;
		uint32 csid;
	} UserStatus[1];
} GSRV_GET_USER_TEAMID_ACK ,*LPGSRV_GET_USER_TEAMID_ACK;

//修改在线用户组teamID请求
typedef struct tagGSRV_MODIFY_ONLINE_TEAMID_QUERY
{
	uint8 count;
	struct tagUserTeamID
	{
		uint32 id;
		uint8  teamID;
		uint32 csid;
	} user[1];
} GSRV_MODIFY_ONLINE_TEAMID_QUERY,*LPGSRV_MODIFY_ONLINE_TEAMID_QUERY;

//修改在线用户组teamID应答
typedef struct tagGSRV_MODIFY_ONLINE_TEAMID_ACK
{
	uint8 result;
	uint8 count;
	struct tagUserTeamID
	{
		uint32 id;
		uint8  teamID;
	} user[1];
} GSRV_MODIFY_ONLINE_TEAMID_ACK,*LPGSRV_MODIFY_ONLINE_TEAMID_ACK;

// 局部用户信息
struct STRU_PART_USERINFO
{
	char    nickname[MAX_NICKNAME_LEN+1] ;
	char    email[MAX_EMAIL_LEN+1]	  ;	// 1 电子邮件
	char    birthday[MAX_BIRTHDAY_LEN+1] ;
	char    password[MAX_PASSWORD_LEN+1] ;	// 2 密码
	char    face_url[MAX_HEADURL_LEN+1] ;
	uint8   sex          ;
	uint8   province	 ;//5 省
	uint8   country		 ;//6 国家
	uint8   city	     ;//7 城市
	uint8   career       ;//9 职业
	uint8   rule         ;//10 加为好友的认证方式
	uint8	familyflag	 ;// 列为家族成员认证方式    0：允许     1：拒绝
	uint8   linkpop		 ;//联系方式隐私属性（0 私有;1 好友可见；3 公开）
	char    address[MAX_ADDRESS_LEN+1]  ;//11 联系地址
	char    mobile[MAX_PHONE_LEN+1]   ;//12 手机
	char    description[MAX_DESCRIPTION_LEN+1]; //13 个人说明

	STRU_PART_USERINFO()
	{
		memset(this,0,sizeof(STRU_PART_USERINFO));
	}
};


//#pragma pack(pop)
#endif
