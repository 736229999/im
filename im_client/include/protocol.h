#ifndef _PROTOCAL_
#define _PROTOCAL_

#include "./publictype.h"

/************************************************************************ 
* 
* 文件名：protocol.h
* 
* 文件描述: 新框架通讯协议相关的命令字
* 
* 创建人： 薛小峰,2005年8月25日
* 
* 版本号：2.0 
* 
* 修改记录： 增加新框架系统服务器间的通讯协议命令字定义 by zouyf 2006-06-15
* 
************************************************************************/




#define SENDER_XTREAM_SERVER		0x0100
#define XTREAM_CLIENT_VERSION		0x011D





//群、群成员类别
#define CMEMBER_CREATOR             2
#define CMEMBER_ADMIN               1
#define CMEMBER_NORMAL              0

//本协议规定 0为成功标志,非0为错误码
#define	V_SUCCESS					0     
#define V_FAIL						1

//公告消息
#define CMD_PUBLIC_MSG				0x00d


//用户注册
#define CMD_REGIST_REQ				0x201
#define CMD_REGIST_ACK				0x202

#define CMD_LOGOUT					0x203

//登录
#define CMD_LOGIN_REQ				0x211  
#define CMD_LOGIN_ACK				0x212  

//心跳,Directory
#define CMD_DIRECTORY_REQ			0x301  
#define CMD_DIRECTORY_ACK			0x302 

//更改用户状态
#define CMD_STATUS_MOD_REQ				0x303
#define CMD_STATUS_MOD_ACK				0x304

#define CMD_USERFAMILYFLAG_GET_REQ		0x413
#define CMD_USERFAMILYFLAG_GET_ACK		0x414


//查询用户资料
#define CMD_USERINFO_GET_REQ		0x415
#define CMD_USERINFO_GET_ACK		0x416

//修改用户资料
#define CMD_USERINFO_MOD_REQ		0x417
#define CMD_USERINFO_MOD_ACK		0x418

//添加修改删除组
#define CMD_GROUPINFO_MOD_REQ		0x501
#define CMD_GROUPINFO_MOD_ACK		0x502

//获取组信息
#define CMD_GROUPINFO_GET_REQ		0x503
#define CMD_GROUPINFO_GET_ACK		0x504

//获取好友清单
#define CMD_FRIENDLIST_REQ			0x601
#define CMD_FRIENDLIST_ACK			0x602

//查询xtream用户
#define CMD_XTREAM_SEARCH_REQ		0x603
#define CMD_XTREAM_SEARCH_ACK		0x604

//添加,修改,删除好友分组
#define CMD_FRIENDGROUP_MOD_REQ		0x605
#define CMD_FRIENDGROUP_MOD_ACK		0x606

//好友验证请求、应答
#define CMD_USERAUTH_REQ			0x607
#define CMD_USERAUTH_IND			0x607
#define CMD_USERAUTH_ACK			0x608

//P2P聊天消息
#define CMD_P2P_TEST_REQ			0x609
#define CMD_P2P_TEST_ACK			0x60a
#define CMD_P2PMSG_SEND				0x60b
#define CMD_P2PMSG_RECV				CMD_P2PMSG_SEND
#define CMD_P2PMSG_SEND_OFFLINE		0x160b
#define CMD_P2PMSG_ACK				0x60c


//服务器发送给客户端的系统消息或转发消息
#define CMD_SERVERMSG_IND			0x60d
#define CMD_SERVERMSG_IND_ACK		0x60e



//获取好友（自己）基本资料请求，应答,通知
#define CMD_GET_FRIEND_BASICINFO_REQ	0x0611
#define CMD_GET_FRIEND_BASICINFO_ACK	0x0612
#define CMD_GET_FRIEND_BASICINFO_NOTIFY 0x0613

//获取历史消息
#define CMD_WEB_MORE_MSG_REQ	0x0616
#define CMD_WEB_MORE_MSG_ACK	0x0617

//服务器主动发起的好友状态更改通知
#define CMD_FRIEND_STATUS_IND		0x701
#define CMD_FRIEND_STATUS_RET		0x702


//客户端登录成功后向服务器请求离线消息
#define CMD_OFFLINEMSG_REQ			0x703
#define CMD_OFFLINEMSG_ACK			0x704

#define CMD_STATUS_GET_REQ			0x705
#define CMD_STATUS_GET_ACK			0x706

// 修改好友备注
#define CMD_MOD_FCNAME_REQ			0x707
#define CMD_MOD_FCNAME_ACK			0x708
// 给好友发送邮件邀请
#define CMD_FRIENDINVITE_REQ		0x709
#define CMD_FRIENDINVITE_ACK		0x70a

//修改个人签名
#define CMD_MODIFYUNDERWRITE_REQ	0x070b
#define CMD_MODIFYUNDERWRITE_ACK	0x070c

//修改好友关注标记
#define CMD_MODIFYFRIENDFLAG_REQ	0x070d
#define CMD_MODIFYFRIENDFLAG_ACK	0x070e

//// 修改个人群名片
//#define CMD_MOD_CMPRO_REQ			0x70f
//#define CMD_MOD_CMPRO_ACK			0x710

#define CMD_CREATE_CLUSTER_REQ      0x901
#define CMD_CREATE_CLUSTER_ACK      0x902

#define CMD_SET_CMEMBERS_REQ        0x903
#define CMD_SET_CMEMBERS_ACK        0x904

#define CMD_SET_CINFO_REQ           0x905
#define CMD_SET_CINFO_ACK           0x906

#define CMD_GET_CINFO_REQ           0x907
#define CMD_GET_CINFO_ACK           0x908

//群成员（退出，添加）通知
#define CMD_CMEMBERS_IND          CMD_SET_CMEMBERS_REQ

#define CMD_CLUSTER_SEARCH_REQ      0x909
#define CMD_CLUSTER_SEARCH_ACK      0x90a

#define CMD_CLUSTER_APPLY_REQ       0x90b
#define CMD_CLUSTER_APPLY_ACK       0x90c

#define CMD_CLUSTER_AUTH_REQ        0x90d
#define CMD_CLUSTER_AUTH_ACK        0x90e
#define CMD_CLUSTER_AUTH_IND        CMD_CLUSTER_AUTH_REQ

#define CMD_CLUSTERMSG_SEND         0x911
#define CMD_CLUSTERMSG_ACK          0x912
#define CMD_CLUSTERMSG_SEND_OFFLINE 0x1911

#define CMD_CLUSTERMSG_RECV         0x917
#define CMD_CLUSTERMSG_RET          0x918

#define CMD_GET_CMEMBERS_REQ        0x913
#define CMD_GET_CMEMBERS_ACK        0x914

#define CMD_CDISMISS_REQ			0x915
#define CMD_CDISMISS_ACK            0x916
#define CMD_CDISMISS_IND			CMD_CDISMISS_REQ

#define CMD_COFFLINE_MSG_REQ        0x919
#define CMD_COFFLINE_MSG_ACK        0x91a

#define CMD_GET_CLIST_REQ           0x91d
#define CMD_GET_CLIST_ACK           0x91e

#define CMD_CLUSTER_NOTIFY_REQ		0x920	//用户向群发送状态改变通知
#define CMD_CLUSTER_NOTIFY_ACK		0x921	

#define CMD_FAMILYOFFLINEMSG_REQ	0x922	//获取群离线消息(由服务器发送至群服务器)
#define CMD_FAMILYOFFLINEMSG_ACK	0x923

//获取群名片
#define CMD_GET_PROFILE_NAME_REQ	0x924
#define CMD_GET_PROFILE_NAME_ACK	0x925

//设置群名片
#define CMD_SET_PROFILE_NAME_REQ	0x926
#define CMD_SET_PROFILE_NAME_ACK	0x927

//群聊上传图片
#define CMD_SEND_CPIC_REQ			0x931
#define CMD_SEND_CPIC_ACK			0x932
//群聊下载图片
#define CMD_GET_CPIC_REQ			0x933
#define CMD_GET_CPIC_ACK			0x934

#define CMD_CLUSTER_INVITE_REQ      0x935	// 邀请好友入群请求
#define CMD_CLUSTER_INVITE_ACK      0x936	// 邀请好友入群应答
#define CMD_CLUSTER_INVITE_IND      CMD_CLUSTER_INVITE_REQ	// 被邀请方转发

#define CMD_CLUSTER_AGREE_REQ      0x937	// 好友是否入群请求
#define CMD_CLUSTER_AGREE_ACK      0x938	// 好友是否入群应答
#define CMD_CLUSTER_AGREE_IND       CMD_CLUSTER_AGREE_REQ       

//#define CMD_START_SEND_CPIC_REQ		0x941	
#define CMD_START_SEND_CPIC_ACK		0x942	// 请求开始发送群图片应答

//查询用户在线状态
#define CMD_GET_USER_STATUS_REQ		0x947
#define CMD_GET_USER_STATUS_ACK		0x948

#define CDM_HEAD_FILE_UPLOAD_REQ 0x951 //上传头像文件请求

#define CDM_HEAD_FILE_UPLOAD_ACK 0x952 //上传头像文件应答

//客服当前聊天买家
#define CMD_MONITOR_SET_BUYER_REQ 0x955
#define CMD_MONITOR_SET_BUYER_ACK 0x956
#define CMD_MONITOR_SET_BUYER_IND CMD_MONITOR_SET_BUYER_REQ

//监控
#define CMD_MONITOR_START_REQ 0x957
#define CMD_MONITOR_START_ACK 0x958
#define CMD_MONITOR_START_IND CMD_MONITOR_START_REQ

//取消监控
#define CMD_MONITOR_END_REQ 0x959
#define CMD_MONITOR_END_ACK 0x960
#define CMD_MONITOR_END_IND CMD_MONITOR_END_REQ

//插入会话
#define CMD_MONITOR_INSERT_CHAT_REQ 0x961
#define CMD_MONITOR_INSERT_CHAT_ACK 0x962
#define CMD_MONITOR_INSERT_CHAT_IND CMD_MONITOR_INSERT_CHAT_REQ

//接手会话
#define CMD_MONITOR_TRAN_CHAT_REQ 0x963
#define CMD_MONITOR_TRAN_CHAT_ACK 0x964
#define CMD_MONITOR_TRAN_CHAT_IND CMD_MONITOR_TRAN_CHAT_REQ
////设置群消息接收方式
//#define CMD_SET_CLUSTERMSG_RECV_REQ	 0x943
//#define CMD_SET_CLUSTERMSG_RECV_ACK	 0x944

////获取客服的CSID
//#define CMD_GET_USERID_MAP_CSID_REQ  0x967
//#define CMD_GET_USERID_MAP_CSID_ACK  0x968

//包括加好友、更新通知（日志，图片等）、站内留言、评论通知等温馨提示
#define CMD_WEB_IND                 0x1f1c
#define CMD_WEB_ACK                 0x1f1d

#define CMD_SMS_REQ					0x1f10
#define CMD_SMS_ACK                 0x1f11

#define CMD_SMS_IND					0x1f12

#define CMD_USERPAY_REQ				0x711
#define CMD_USERPAY_ACK				0x712

// 通知接待客服买家下线
#define CMD_BUYER_OFFLINE_IND		0x978

//获取客服所在组信息
#define CMD_CS_TEAM_INFO_REQ			0x305
#define CMD_CS_TEAM_INFO_ACK			0x306

//推送强制客服下线通知
#define CMD_FORCE_OFFLINE_CS_REQ		0x30b
#define CMD_FORCE_OFFLINE_CS_ACK		0x30c
#define CMD_FORCE_OFFLINE_CS_IND 	CMD_FORCE_OFFLINE_CS_ACK

const uint16	CMD_SEND_FILE_REQ	= 0x0c01;
const uint16	CMD_SEND_FILE_ACK	= 0x0c02;
const uint16	CMD_DOWNLOAD_FILE_REQ = 0x0c03;
const uint16	CMD_DOWNLOAD_FILE_ACK = 0x0c04;
const uint16	CMD_DOWNLOAD_REQ	= 0x0c05;
const uint16	CMD_DOWNLOAD_ACK	= 0x0c06;
const uint16	CMD_FIN_TRAN_REQ	= 0x0c09;
const uint16	CMD_FIN_TRAN_ACK	= 0x0c0a;
const uint16	CMD_ABORT_TRAN_REQ	= 0x0c0b;
const uint16	CMD_ABORT_TRAN_ACK	= 0x0c0c;



const uint16 CMD_ROUTERUSERPACKET_REQ = 0x8009;
const uint16 CMD_ROUTERUSERPACKET_ACK =	0x8009;
const uint16 CMD_ROUTERUSERACKPACKET_REQ = 0x800a;
const uint16 CMD_ROUTERUSERACKPACKET_ACK = 0x800a;
const uint16 CMD_ROUTERPACKET_REQ = 0x800b;
const uint16 CMD_ROUTERPACKET_ACK = 0x800c;

const uint16 CMD_TRANS_LOGIN_REQ = 0x8101;
const uint16 CMD_TRANS_LOGIN_ACK = 0x8102;
const uint16 CMD_TRANS_SETRECV_REQ = 0x8103;
const uint16 CMD_TRANS_SETRECV_ACK = 0x8104;

const uint16 CMD_P2P_NOTIFY_LOGIN_REQ = 0x8105;
const uint16 CMD_P2P_NOTIFY_LOGIN_ACK = 0x8106;

const uint16 CMD_TRANS_RECVLOGIN_REQ = 0x8107;
const uint16 CMD_TRANS_RECVLOGIN_ACK = 0x8108;
const uint16 CMD_TRANS_NOTIFYSENDER_IND = 0x8109;
const uint16 CMD_TRANS_NOTIFYSENDER_ACK = 0x810a;
const uint16 CMD_TRANS_SENDERCLOSE_REQ = 0x8111;
const uint16 CMD_TRANS_SENDERCLOSE_ACK = 0x8112;
const uint16 CMD_TRANS_NOTIFYCLOSE_IND = 0x8113;
const uint16 CMD_TRANS_NOTIFYCLOSE_ACK = 0x8114;


#define CMD_NETWORK_ERR				0x2000


/***
全局服务器业务协议命令字
***/
#define CMDGSRV_BASE  0x8200
 
//在线用户全局查询
#define CMDGSRV_SEARCH_USER_QUERY				0x8201
//在线用户全局查询应答
#define CMDGSRV_SEARCH_USER_ACK					0x8202

//查询在线用户状态
#define CMDGSRV_GET_USERSTATUS_QUERY			0x8203
//查询在线用户状态应答
#define CMDGSRV_GET_USERSTATUS_ACK				0x8204

//修改在线用户状态
#define CMDGSRV_MODIFY_USERSTATUS_QUERY			0x8205
//修改在线用户状态应答
#define CMDGSRV_MODIFY_USERSTATUS_ACK			0x8206

//查询在线用户全局资料
#define CMDGSRV_GET_USER_FULLINFO_QUERY			0x8207
//查询在线用户全局资料应答
#define CMDGSRV_GET_USER_FULLINFO_ACK			0x8208

//添加/修改全局用户资料
#define CMDGSRV_ADDMODIFY_USER_FULLINFO_QUERY	0x8209
//添加/修改全局用户资料应答
#define CMDGSRV_ADDMODIFY_USER_FULLINFO_ACK		0x8210

//删除用户全局资料
#define CMDGSRV_DEL_USER_FULLINFO_QUERY			0x8211
//删除用户全局资料应答
#define CMDGSRV_DEL_USER_FULLINFO_ACK			0x8212

//查询在线群全局资料
#define CMDGSRV_FAMILYINFO_QUERY			0x8213
//查询在线群全局资料应答
#define CMDGSRV_FAMILYINFO_QUERY_ACK		0x8214
//添加全局群资料
#define CMDGSRV_FAMILY_ADD_QUERY			0x8215
//添加全局群资料应答
#define CMDGSRV_FAMILY_ADD_ACK				0x8216
//删除全局群资料
#define CMDGSRV_FAMILY_DEL_QUERY			0x8217
//删除全局群资料应答
#define CMDGSRV_FAMILY_DEL_ACK				0x8218

//查询在线用户控制信息
#define CMDGSRV_USERCONTROLINFO_QUERY		0x8219
//查询在线用户控制信息应答
#define CMDGSRV_USERCONTROLINFO_QUERY_ACK	0x8220


//通知业务服务器用户已重登录
#define CMDGSRV_USERRELOGIN_NOTIFY			0x8221
//通知业务服务器用户已重登录应答
#define CMDGSRV_USERRELOGIN_ACK				0x8222

 
//IM业务服务器停机通知
#define	CMDCSRV_IMSERVER_SHUTDOWN_NOTIFY	0x8231
//IM业务服务器停机通知应答
#define CMDCSRV_IMSERVER_SHUTDOWN_ACK		0x8232


#define NTF_HEART_TIMEOUT			0xE001

/***
集群服务业务命令字
***/
#define CMDCSERVICE_BASE 0x8300

//更新用户/群定位信息通知
#define CMDCS_UPDATELOCATION_NOTIFY (CMDCSERVICE_BASE)

/***
接入服务器、动态负载业务命令字
***/
//应答ACK
#define CMD_ACK                         0x8000

//服务器注册
#define CMD_SERVER_REGISTER             0x8001
#define CMD_SERVER_REGISTER_ACK         0x8002

//服务器注销
#define CMD_SERVER_UNREGISTER           0x8003

//服务器状态报告
#define CMD_SERVER_STATUS_REPORT        0x8005
//服务器状态报告应答
#define CMD_SERVER_STATUS_ACK			0x8006

//服务器状态列表请求
#define CMD_SERVER_STATUSLIST_REQ       0x8007
#define CMD_SERVER_STATUSLIST_REQ_ACK   0x8008

//服务器透明转发客户端数据
#define CMD_SERVER_CLARITY_TRANSMIT     0x8009



//用户行为分析
#define CMD_USER_ACTION_AUDIT_REQ			0x0A05
//用户行为分析应答
#define CMD_USER_ACTION_AUDIT_ACK			0x0A06

// 获取商户最近联系的客户信息
#define CMD_GET_NEAR_COMM_CUST_REQ         0x0A07
#define CMD_GET_NEAR_COMM_CUST_ACK         0x0A08

// 更新后台中“商户最新联系客户”信息
#define CMD_UPDATE_NEAR_COMM_CUST_INFO_REQ		0x0A09
#define CMD_UPDATE_NEAR_COMM_CUST_INFO_ACK		0x0A10

// 系统消息：好友信息修改
#define CMD_FRIEND_INFO_IND			0x0713

#ifdef _BUSINESS_VESION
// 修改挂起状态
#define CMD_MOD_HANGUP_STATE_REQ	0x0A11
#define CMD_MOD_HANGUP_STATE_ACK	0x0A12
#endif

#ifdef _VER_INTERNAL_		// 内部版，获取员工信息
#define CMD_GET_EMPLOYEE_INFO_REQ	0x0A13
#define CMD_GET_EMPLOYEE_INFO_ACK	0x0A14
#endif

#ifdef _BUSINESS_VESION
#define CMD_UPDATE_NEAR_COMM_CUST_REMARK_REQ		0x0A15
#define CMD_UPDATE_NEAR_COMM_CUST_REMARK_ACK		0x0A16

#define CMD_GET_NEAR_COMM_CUST_REMARK_REQ		0x0A17
#define CMD_GET_NEAR_COMM_CUST_REMARK_ACK		0x0A18
#endif

// 文件传输协议
#define CMD_FILE_TRANS_REQ   	0x8010		// 发送方发送数据包（服务端接收到后直接转发给接收方）
#define CMD_FILE_TRANS_ACK   	0x8011		// 服务端接收到到数据包后，发给发送方的确认消息（现未使用）

#define CMD_FILE_TRANS_IND   	0x8012		// 接收方接收到数据包后，发个发送方的确认数据包（服务端收到后直接转发给发送方）
#define CMD_FILE_TRANS_IND_ACK  0x8013		// 服务端接收到到确认数据包后，发给接收方的确认消息（现未使用）

#define CMD_FILE_TRANS_NOTIFY   0x8019		// 开始文件传输时，发给服务端的消息，以便于服务端记录客户端的ip和端口（服务端和客户端都需要发送）	
#define CMD_FILE_TRANS_NOTIFY_ACK	0x801A	// 服务端返回的确认消息

/////线程响应//////////////////////////
#define CMD_FILE_START   	    0x8014
#define CMD_FILE_NAME   	    0x8015
#define CMD_FILE_ACK  	        0x8016
#define CMD_FILE_RETRY   	    0x8017
#define CMD_FILE_END  	        0x8018
#endif

#define CMD_CUSTSER_TRAN_REQ	0x801B		// 客户转接发送方发送转接请求给服务器，服务器直接把这条消息转给接收方
#define CMD_CUSTSER_TRAN_ACK	0x801C		//  客服转接接收方接到转接请求后，发送确认包给服务器，服务器直接转发给发送方
#define CMD_CUSTSER_TRAN_REQ_IND CMD_CUSTSER_TRAN_REQ	// 服务器直接转接客服转接发过来的请求
#define CMD_CUSTSER_TRAN_ACK_IND CMD_CUSTSER_TRAN_ACK	// 服务器直接转接客服转接的确认包
  
#define CMD_MOD_PSW_REQ         0x810D      //用户修改登录密码请求
#define CMD_MOD_PSW_ACK         0x810E      //用户修改密码服务端返回确认

// 排队相关协议（服务端推送给客户端的，处于系统消息CMD_SERVERMSG_IND 0x60d之下）
#define CMD_WAITQUEU_CHANGE		0x810F		// 等待队列增加成功或减少成员

// 排队相关协议（客户端发送给服务端的协议）
#define CMD_RECEPTE_CUST_REQ	0x8110		// 接待客户请求
#define CMD_RECEPTE_CUST_ACK	0x8111		// 接待客户应答

// 发送buf
#define CMD_SEND_BUF_IND 0x984
// 请求未接收完的buf
#define CMD_SEND_BUF_REQ 0x985

// 消息提醒类型
#define CMD_GETMSGREMIND_REQ 0x986
#define CMD_GETMSGREMIND_ACK 0x987
#define CMD_SETMSGREMIND_REQ 0x988
#define CMD_SETMSGREMIND_ACK 0x989

// 获取离线消息推送数据
#define CMD_GET_OFFLINE_MSGONTIFY 0x98A

// 请求离线消息
#define CMD_GET_OFFLINE_MSG_REQ 0x98B