// improtocol.h: IM协议层
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATACENTER_H__73413291_442C_4FA0_8E05_DBDCE77AF3F4__INCLUDED_)
#define AFX_DATACENTER_H__73413291_442C_4FA0_8E05_DBDCE77AF3F4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BasicSocket.h"
#include "impublic.h"
#include "UdpComm.h"
#include "MiniNetTrans.h"
#include "SrvFrameProtocol.h"
#include "DataPackId.h"
#include "DataPacketFilter.h"
#include "SimpleFileTransSession.h"
#include "../improtocol.pb.h"
using namespace xy;
#include <map>
#include <vector>
using namespace std;

typedef struct _tagWndArry
{
	vector<HWND> vecHWnd;
} tagWndArry;

typedef map< WORD, tagWndArry *> CMD_WND_MAP;

typedef void (*fXtProcess) (char * data, WORD wRetCode) ;

typedef XT_FRIENDLIST_ACK::tagFriendInfo	FRIENDITEM;

class FRIENDLIST_SEG:public std::vector<FRIENDITEM>
{
public:
	FRIENDLIST_SEG():m_nNextPos(0xffff){};
public:
	uint16		m_nNextPos;
};


#define SERVER_TIMEOUT			0xFFFF  // 服务器处理超时
#define NETWORK_DISCONNECT		0xFFFE	// 网络连接断开
#define IP_CANT_CONNECT			0xFFFD	// ip不存在

#define ON_XT_MESSAGE()		ON_MESSAGE(WM_XT_CMD_NOTIFY,OnXtMessage)
#define ON_XT_RMESSAGE()	ON_MESSAGE(WM_XT_RESEND_NOTIFY,OnXtResend)

#define DECLARE_XT_MESSAGE_MAP() \
protected:\
	LRESULT OnXtMessage(WPARAM wParam, LPARAM lParam);\
	LRESULT	OnXtResend(WPARAM wParam, LPARAM lParam);\


#define BEGIN_XT_MESSAGE_MAP(className) \
	LRESULT className::OnXtMessage(WPARAM wParam,LPARAM lParam)\
{\
	WORD xtCmd    = HIWORD(wParam);\
	WORD wRetCode = LOWORD(wParam);\
	switch (xtCmd)\
{\

#define ON_XT_COMMAND(cmd,func)\
	case cmd:\
	func((const char *)lParam,wRetCode);\
	break;\


#define END_XT_MESSAGE_MAP() \
	default:\
	break;\
}\
	return 1;\
}\

#define BEGIN_XT_RESEND_MAP(className) \
	LRESULT className::OnXtResend(WPARAM wParam,LPARAM lParam)\
{\
	SendDataStruct * pSendData = (SendDataStruct *)lParam;\
	LPXT_HEAD pHead = (LPXT_HEAD)pSendData->data;\
	switch (pHead->cmd)\
{\

#define ON_XT_RESEND(cmd,func)\
	case cmd:\
	func(pSendData);\
	break;\


#define END_XT_RESEND_MAP() \
	default:\
	break;\
}\
	return 1;\
}\

typedef void (*IMResendNotifyProc)(DWORD dwKey,SendDataStruct * pSendData);



DWORD  IMCallBack(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);

////A端文件分组传输到服务端,服务器传给B端请求////////////////////////////////////////////
#define BUFFER_SIZE 1024*50
typedef struct _SEND_FILE_PACK_REQ
{  
	uint32  send_id;         ////发送ID
	uint32  fid;             ////接收ID
	uint8  	flag;			 ////文件名、文件内容、文件结束
	enum  { SENDFILE_NAME = 1, SENDFILE_CONTENT = 2, SENDFILE_NAMEEND = 3 };
	uint32  file_id;         ////文件ID
	uint32  pack_id;         ////分组序号 
	uint32  size;            ////分组长度  
	char buf[BUFFER_SIZE];   ////分组文件内容 
}SEND_FILE_PACK_REQ,*LPSEND_FILE_PACK_REQ; 

////A端文件分组传输到服务端,服务器传给B端响应////////////////////////////////////////////
typedef struct _SEND_FILE_PACK_ACK
{  
	uint32  send_id;         ////发送ID
	uint32  fid;             ////接收ID
	uint8  	flag;			 ////文件名、文件内容、文件结束
	enum  { SENDFILE_NAME = 1, SENDFILE_CONTENT = 2, SENDFILE_NAMEEND = 3 };
	uint32  file_id;         ////文件ID
	uint32  pack_id;         ////分组序号 
	uint32  size;            ////分组长度  
	char buf[BUFFER_SIZE];   ////分组文件内容 
}SEND_FILE_PACK_ACK,*LPSEND_FILE_PACK_ACK; 


////B端传给服务器，服务器传给A端的确认请求///////////////////////////////////////////////
typedef struct _SEND_FILE_PACK_IND
{  
	uint32  send_id;         ////发送ID
	uint32  fid;             ////接收ID
	uint8  	flag;			 ////文件名、文件内容、文件结束
	enum  { SENDFILE_NAME = 1, SENDFILE_CONTENT = 2, SENDFILE_NAMEEND = 3 };
	uint32  file_id;         ////文件ID
	uint32  pack_id;         ////分组序号 
	uint32  size;            ////分组长度  
}SEND_FILE_PACK_IND,*LPSEND_FILE_PACK_IND; 

////B端传给服务器，服务器传给A端的确认回复///////////////////////////////////////////////////
typedef struct _SEND_FILE_PACK_IND_ACK
{  
	uint32  send_id;         ////发送ID
	uint32  fid;             ////接收ID
	uint8  	flag;			 ////文件名、文件内容、文件结束
	enum  { SENDFILE_NAME = 1, SENDFILE_CONTENT = 2, SENDFILE_NAMEEND = 3 };
	uint32  file_id;         ////文件ID
	uint32  pack_id;         ////分组序号 
	uint32  size;            ////分组长度  
}SEND_FILE_PACK_IND_ACK,*LPSEND_FILE_PACK_IND_ACK; 


// 文件段信息
struct IM_FILESEG_INFO
{	
	uint8		msgType;	// 临时群或固定群
	uint32		id;
	uint32		size;
	uint32		last_packet_id;
	uint32		last_file_pointer;
	char		file_name[100];
};

typedef std::map<uint32,IM_FILESEG_INFO> FILESEG_DIC;

void strxcpy(char * szDst,const char * szSrc,int x);

struct P2P_DATA
{
	XT_HEAD		head;
	uint16		subCmd;
	uint16		len;
	uint8 		data[1500];
};

//class CP2PRecvWnd;

class CIMProtocol   
{
public:
	static int		s_nCurVer;	// 当前程序版本
	// 消息发送方式
	enum SEND_MSG_TYPE{
		SMT_P2P,		// p2p发送
		SMT_LAN,		// 内网发送
		SMT_SERVER		// 服务器中转
	};

	//enum SEND_DEST_TYPE{
	//	SDT_P2P,	// 发送到p2p端点
	//	SDT_BUSI,	// 发送到业务服务器
	//	SDT_TRAN,	// 发送到中转服务器
	//}

	friend DWORD  IMCallBack(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);
	virtual ~CIMProtocol();
	inline void  SetIMServerIP(DWORD dwIp) {	m_dwIMServerIP = dwIp; }
	inline void	 SetIMServerPort(WORD port) { m_wIMServerPort = port; };
	inline void	 SetTranServerIP(DWORD dwIp){ m_dwTranServerIP = dwIp; };
	inline void	 SetTranServerPort(WORD port){ m_wTranServerPort = port; };
	inline WORD	 GetTranServerPort() const { return m_wTranServerPort; };
	inline DWORD GetTranServerIP() const { return m_dwTranServerIP; };
	inline DWORD GetIMServerIP() { return m_dwIMServerIP; }
	inline WORD  GetUdpCommPort() { return m_pUdpComm->GetPort(); }
	inline DWORD GetUdpCommIp() { return m_pUdpComm->GetLocalIP(); }
	inline uint32  GetCurSendSequnceId() { return m_wSequenceID; }
	inline uint32  GetLastAckSequenceId() { return m_wLastAckSequenceID; }
	inline char * GetLastError() { return m_szError; }
	//工作线程，对接收到的数据报文进行解包，解密，数据分发
	static UINT IMProtocolProc(LPVOID pParam,const RecvDataStruct * pData);
	static UINT	IMProtocolResend(LPVOID pParam,const SendDataStruct * pData);

public:
	//注册窗口命令
	void	RegistCmd(WORD dwType,HWND hWnd);

	void	RegistResendCmd(WORD dwCmd,HWND hWnd);

	//注销窗口命令
	void	UnRegistCmd(WORD dwType,HWND hWnd);

	void	UnRegistCmd(HWND hWnd);

	//开起线程
	bool	Start();

	bool	Stop();

	bool	IsRunning() { return m_bRunning;	}

	static	CIMProtocol * GetInstance();

	static DWORD s_nHeartBeat;
//
// 处理和协议相关的请求	
//
public:	
	int SendDownloadCPicReq(const XT_DOWNLOAD_CPIC_REQ & req,int nRetryTimes = 10);
	int SendCPicData(const XT_CPIC_DATA & picData);
	int SendGetSvrListReq(const tagSERVER_STATUSLIST_REQ& req);
	int SendClusterMsg(const XT_MSG &msg);
	int SendP2pTestPacket(const XT_P2PTEST_REQ &req);
	int SendP2PMsg(const XT_MSG &msg, int type=CIMProtocol::SMT_SERVER, int nSeqId = 0, int nInsertBuyerId = 0);
	int UserPayment(uint32 price,uint32 id, uint32 type);
	// 获取好友加入群标志
	//int QueryFriendFamilyFlag(const XT_USERFAMILYFLAG_GET_REQ & req);
	int ClusterInvite(const XT_CLUSTER_INVITE_REQ & req);
	int ClusterAgree(const XT_CLUSTER_AGREE_REQ & req);
	int SendReq(const XT_CLUSTER_SEARCH_REQ &req);
	int SendReq(const XT_CLUSTER_DISMISS_REQ &req);
	int SendReq(const XT_XTREAM_SEARCH_REQ &req);
	int SendReq(const XT_USERAUTH_REQ &req);
	int SendReq(const XT_FRIENDGROUP_MOD_REQ &req);
	int SendReq(const XT_GROUPINFO_MOD_REQ &req);
	int SendReq(const XT_GROUPINFO_GET_REQ &req);
	int SendReq(const XT_USERINFO_MOD_REQ &req);
	int SendReq(const XT_USERINFO_GET_REQ &req);
	int SendReq(const XT_FRIENDLIST_REQ &req);
	int SendReq(const XT_STATUS_MOD_REQ & req);
	int SendReq(const XT_DIRECTORY_REQ &req);
	int SendReq(const XT_LOGIN_REQ &req);
	//int SendReq(const XT_REGIST_REQ &req);
	int SendReq(const XT_LOGOUT &req);
	int SendReq(const XT_OFFLINEMSG_REQ &req);
	int SendReq(const XT_STATUS_GET_REQ &req);
	int SendReq(const XT_CREATE_CLUSTER_REQ &req,uint32 * pwSeqID=NULL/*out 返回协议报sequence id*/);
	int SendReq(const XT_SET_CMEMBERS_REQ &req);
	int SendReq(const XT_CLUSTER_APPLY_REQ &req);
	int SendReq(const XT_GET_CMEMBERS_REQ &req);
	int SendReq(const XT_GET_CINFO_REQ &req);
	int SendReq(const XT_GET_CLIST_REQ &req);
	int SendReq(const XT_SET_CINFO_REQ &req);
	int SendReq(const XT_COFFLINE_MSG_REQ &req);
	int SendReq(const XT_CLUSTER_AUTH_REQ &req);
	int SendReq(const XT_USER_OPERATION_REQ &req);
	int SendReq(const XT_PROFILENAME_SET_REQ & req);
	int SendReq(const XT_CLUSTERMSG_RECV_SET_REQ& req);///群信息接收设置
	int SendReq(const XT_MOD_FRIEND_COMMNAME_REQ & req);
	int SendReq(const XT_GET_FRIEND_BASICINFO_REQ &req);
	int SendReq(const char *pszUserLabel, uint32 uid);
	int SendReq(const XT_MODIFY_FRIENDFLAG_REQ &req);
	int SendReq(const XT_FRIENDINVITE_REQ &req);
	int SendReq(const XT_SMS_REQ &req);
	int SendReq(const XT_PROFILENAME_GET_REQ &req);////获取群名片信息
	int SendReq(const XT_MOD_PSW_REQ &req);////用户修改登录密码
	int SendReq(const XT_GET_USER_STATUS_REQ &req);
	int SendReq(const XT_WEB_MORE_MSG_REQ &req);
	// 更新后台”最近联系客户”信息，发送消息
	int SendReq(const XT_UPDATE_NEAR_COMM_CUST_INFO_REQ &req);

	// 获取商户最近联系的客户信息(发送消息)
	int SendReq(const XT_GET_NEAR_COMM_CUST_REQ &req);	
	////获取客服CSID
	//int SendReq(const XT_USER_MAP_CSID_REQ &req);

	////A向B传输文件，经服务器中转
	int SendReq(const SEND_FILE_PACK_REQ &req);

	/////////////////////////////////////////////////////////
	////B向A发送确认，经服务器中转
	int SendReq(const SEND_FILE_PACK_IND &req);

	// 发送已接待客户请求
	int SendReq(const XT_RECEPTE_CUST_REQ &req);

	// 客服设置当前聊天的买家
	int SendReq(const XT_MONITOR_SET_BUYER_REQ &req);

	// 主管监控客服
	int SendReq(const XT_MONITOR_START_REQ &req);

	//主管取消监控客服
	int SendReq(const XT_MONITOR_END_REQ &req);

	//主管插入会话
	int SendReq(const XT_MONITOR_INSERT_CHAT_REQ &req);

	//主管接手会话
	int SendReq(const XT_MONITOR_TRAN_CHAT_REQ &req);

#ifdef _BUSINESS_VESION
	int SendReq(const BaseReq &req, uint32 nCmd = CMD_GETMSGREMIND_REQ);
	int SendReq(const MsgRemindAck &req);
	int SendReq(const csTeamInfoReq &req);
#endif
	//上传头像
	int SendHeadFileUploadReq(const IMAGE_FILE_STRUCT &req);//上传头像文件
	int SendHeadFileUploadPacketReq(const SEND_FILE_STRUCT &req, char* buffer, int datalen);//上传头像文件包
	/////////////////////////////////////////////////////////
	//
	// p2p 通讯 
	// 角色 A ---> B
	// A=发送者,B=接收者
	//

	//------------------------------------------------------
	// 发送者A发送文件请求
	//------------------------------------------------------
	int		SendSendFileReq(const P2P_SENDFILE_REQ & req);

	//------------------------------------------------------
	// 请求者B发送文件下载请求
	//------------------------------------------------------
	int		SendDownloadFileReq(const P2P_DOWNLOAD_FILE_REQ & req);

	//------------------------------------------------------
	// 请求者B发送数据下载请求
	//------------------------------------------------------
	//int		SendDownloadReq(const P2P_DOWNLOAD_REQ & req,CP2PRecvWnd * pWnd);

	//------------------------------------------------------
	// 请求者B快速重传数据下载请求
	//------------------------------------------------------
	//int		QuickResendDownloadReq(const P2P_DOWNLOAD_REQ & req,CP2PRecvWnd * pWnd);

	//------------------------------------------------------
	// 发送者A提供下载数据
	//------------------------------------------------------
	//int		SendDownloadAck(const P2P_DOWNLOAD_ACK & ack);

	//------------------------------------------------------
	// 请求者B发送完成传输请求
	//------------------------------------------------------
	int		SendFinishTranReq(const P2P_FIN_TRAN_REQ & req);

	//------------------------------------------------------
	// 双方发出终止传输请求
	//------------------------------------------------------
	int		SendAbortTranReq(const P2P_ABORT_TRAN_REQ & req);

	//
	// 利用中转服务器进行数据传输
	// 角色 clientA	-> Tran server -> clientB
	// A=发送者,B=接收者,T=中转服务器

	//------------------------------------------------------
	// A客户端通知B登录到中转服务器
	//------------------------------------------------------
	void	P2SSendNotifyLogin(const XT_TRANS_NOTIFY_LOGIN_REQ & req);

	//------------------------------------------------------
	// A客户端连接中转服务器，申请会话
	//------------------------------------------------------
	//int		C2TSendLogin(const XT_TRANS_LOGIN_REQ & req);

	//------------------------------------------------------
	// A客户端向中转服务器注销会话
	//------------------------------------------------------
	//int		C2TSendCloseSession(const XT_TRANS_CLOSE_SEESION_REQ & req);
	
	//------------------------------------------------------
	// B接收者提交登录/注销到中转服务器
	//------------------------------------------------------
	//int		C2TSendRecvLogin(const XT_TRANS_RECVLOGIN_REQ & req);

#ifdef _BUSINESS_VESION
	// 修改挂起状态
	int SendReq(const XT_MOD_HANGUP_STATE_REQ &req);
#endif
#ifdef _VER_INTERNAL_	// 内部版
	int SendReq(const XT_GET_EMPLOYEE_INFO_REQ &req);
#endif

#ifdef _BUSINESS_VESION
	// 获取最近联系客户右侧页备注
	int SendReq(const XT_GET_NEAR_COMM_CUST_REMARK_REQ& req);
	// 修改最近联系客户右侧页备注
	int SendReq(const XT_UPDATE_NEAR_COMM_CUST_REMARK_REQ& req);
#endif

	// 文件传输开始时，发送的消息，便于服务端记录客户端的ip和端口
	int SendReq_FileTranNotify();

	// 发起客服转接
	int SendReq(const XT_CUSTSER_TRAN_REQ &req);

	// 接收方发送转接确认消息
	int SendReq(const XT_CUSTSER_TRAN_ACK &req);

	int SendReq(const Buf_ClientSend &req);

	////请求离线消息
	int GetOfflineMsgReq();
//
// 处理和协议相关的响应
//
private:
	void OnUserPaymentAck(const RecvDataStruct & recvData);
	void OnQueryFriendFamilyFlag(const RecvDataStruct & recvData);
	void OnClusterInviteAck(const RecvDataStruct & recvData);
	void OnClusterAgreeAck(const RecvDataStruct & recvData);
	void OnGetServerListAck(const RecvDataStruct & recvData); 
	void OnP2pTestPacketAck(const RecvDataStruct &recvData);
	void OnRecvSMSInd(const RecvDataStruct &recvData);
	void OnRecvWebInd(const RecvDataStruct &recvData);
	void OnClusterSearchAck(const RecvDataStruct &recvData);
	void OnCDismissAck(const RecvDataStruct &recvData);
	void OnGetCListAck(const RecvDataStruct &recvData);
	void OnCMembersGetAck(const RecvDataStruct &recvData);
	void OnCMembersSetAck(const RecvDataStruct &recvData);
	void OnClusterMsgRecSetAck(const RecvDataStruct &recvData);
	void OnSetProfileNameAck(const RecvDataStruct &recvData);////设置群名片
	void OnGetProfileNameAck(const RecvDataStruct &recvData);////获取群名片
	
	void OnCreateClusterAck(const RecvDataStruct &recvData);
	void OnStatusGetAck(const RecvDataStruct &recvData);
	void OnFriendSearchAck(const RecvDataStruct &recvData);
	void OnRecvOfflineMsg(const RecvDataStruct &recvData);
	void OnRecvSvrMsg(const RecvDataStruct &recvData);
	void OnRecvP2PMsg(const RecvDataStruct &recvData);
	void OnRecvClusterMsg(const RecvDataStruct &recvData);
	void OnClusterMsgAck(const RecvDataStruct &recvData);
	void OnRecvP2PMsgAck(const RecvDataStruct &recvData);
	void OnUserAuthAck(const RecvDataStruct &recvData);
	void OnFriendGroupModAck(const RecvDataStruct &recvData);
	void OnGroupInfoModAck(const RecvDataStruct &recvData);
	void OnGroupInfoGetAck(const RecvDataStruct &recvData);
	void OnStatusModAck(const RecvDataStruct &recvData);
	void OnUserInfoModAck(const RecvDataStruct &recvData);
	void OnUserInfoGetAck(const RecvDataStruct &recvData);
	void OnFriendInviteAck(const RecvDataStruct &recvData);
	void OnFriendListAck(const RecvDataStruct &recvData);
	void OnDirectoryAck(const RecvDataStruct &recvData);
	void OnLoginAck(const RecvDataStruct &recvData);
	//void OnRegistAck(const RecvDataStruct &recvData);	
	void OnClusterApplyAck(const RecvDataStruct &recvData);
	void OnGetCInfoAck(const RecvDataStruct &recvData);
	void OnSetCInfoAck(const RecvDataStruct &recvData);
	void OnRecvCofflineMsg(const RecvDataStruct &recvData);
	void OnClusterAuthAck(const RecvDataStruct &recvData);
	void OnRecvP2pTestPacket(const RecvDataStruct &recvData);
	void OnModFCNameAck(const RecvDataStruct & recvData);
	void OnModCMProfileAck(const RecvDataStruct & recvData);
	void OnUploadCPicAck(const RecvDataStruct & recvData);
	void OnDownloadCPicAck(const RecvDataStruct & recvData);
	void OnRecvSMSAck(const RecvDataStruct & recvData);
	void OnHeadFileUploadAck(const RecvDataStruct &recvData);
	void OnModPswAck(const RecvDataStruct &recvData); ////修改密码后服务端返回确认
	void OnGetCSIDFromUserIDAck(const RecvDataStruct &recvData);
	void OnGetUserStatusAck(const RecvDataStruct &recvData);
	void OnWebMoreMsgAck(const RecvDataStruct &recvData);
#ifdef _CS_PLAT_EX_
	// 邀约用户的回应
	void OnReceptCustAck(const RecvDataStruct &recvData);
#endif

	void		OnFriendBasicInfo(char * pData,int nLen);
	void		OnFriendStatusChange(char * pData,int nLen);

	void		RecvSendFileReq(const char * data,int nLen,const RecvDataStruct &recvData);
	void		RecvDownloadFileReq(const char * data,int nLen,const RecvDataStruct &recvData);
	void		RecvDownloadFileAck(const char * data,int nLen,const RecvDataStruct &recvData);
	void		RecvDownloadAck(const char * data,int nLen,const RecvDataStruct &recvData);
	void		RecvDownloadReq(const char * data,int nLen,const RecvDataStruct &recvData);
	void		RecvFinishReq(const char * data,int nLen,const RecvDataStruct &recvData);
	void		RecvAbortReq(const char * data,int nLen,const RecvDataStruct &recvData);
	void        FileTransAck(const char * data,int nLen,const RecvDataStruct &recvData);
	void        FileTransInd(const char * data,int nLen,const RecvDataStruct &recvData);
	void		RecvTransLoginAck(const RecvDataStruct & recvData);
	void		RecvTransRecvLoginAck(const RecvDataStruct & recvData);
	void		RecvTransLogoutAck(const RecvDataStruct & recvData);
	void		RecvNotifyLoginAck(const char * data,int nLen,const RecvDataStruct & recvData);
	void		RecvNotifyLoginReq(const char * data,int nLen,const RecvDataStruct & recvData);
	
	//------------------------------------------------------
	// 收到服务器中转数据
	//------------------------------------------------------
	void		RecvServerTran(const RecvDataStruct & recvData);
	void		RecvServerEncryptTran(const RecvDataStruct & recvData);
	void		DecryptFailedServerTran(WORD wRet,const SendDataStruct & sendData);
	void		ParseFailedServerTran(WORD wRet,const SendDataStruct & sendData);

	// 文件传输开始时，发送给服务端通告消息后，服务端返回的确认消息
	void FileTranNotifyAck(const RecvDataStruct &recvData);

	// 接收方接收到客服转接请求
	//void RecvCustSerTranReqInd(const RecvDataStruct &recvData);

	// 发送方收到客户转接确认消息
	//void RecvCustSerTranAckInd(const RecvDataStruct &recvData);

	// 更新后台”最近联系客户”信息，接收消息
	void OnRecvUpdateCommInfoAck(const RecvDataStruct &recvData);

	// 获取商户最近联系的客户信息(接收消息)
	void OnRecvGetNearCommCustAck(const RecvDataStruct &recvData);
#ifdef _VER_INTERNAL_	// 内部版
	void OnRecvGetEmployeeInfoAck(const RecvDataStruct &recvData);		
#endif
#ifdef _BUSINESS_VESION
	// 更新商户最近联系的客户右侧页备注(接收消息)
	void OnRecvUpdateNearCommCustRemarkAck(const RecvDataStruct &recvData);

	// 获取商户最近联系的客户右侧备注(接收消息)
	void OnRecvGetNearCommCustRemarkAck(const RecvDataStruct &recvData);
	//客服设置当前聊天的买家应答
	void OnMonitorSetBuyerAck(const RecvDataStruct &recvData);
	//主管获取客服接待的买家
	void OnMonitorStartAck(const RecvDataStruct &recvData);
	//主管取消监控客服应答
	void OnMonitorEndAck(const RecvDataStruct &recvData);
	//主管插入会话应答
	void OnMonitorInsertChatAck(const RecvDataStruct &recvData);
	//主管接手会话应答
	void OnMonitorTranChatAck(const RecvDataStruct &recvData);
	//获取订单订阅选项应答
	void OnGetMsgRemindAck(const RecvDataStruct &recvData);
	//设置订单订阅选项应答
	void OnSetMsgRemindAck(const RecvDataStruct &recvData);
	//获取分组信息
	void OnGetCsTeamInfo(const RecvDataStruct &recvData);
#endif
public:
#ifdef _BUSINESS_VESION
	void GetZmqMsg();
	void GetZmqMsgInfo();
#endif
private:

	static CIMProtocol * pInstance;

public:
	//负责向窗体发送消息
	//wParam,高字节命令字，低字节错误码
	//lParam,报文缓冲地址
	int		SendNotify(WPARAM wParam,LPARAM lParam);
	int		SendResendNotify(WPARAM WPARAM,LPARAM lParam);

private:

	CIMProtocol();
	
	bool	IsValideServerPacket(const RecvDataStruct &recvData);

public:
	CUdpNetTrans	*m_pUdpComm;
	CUdpNetTrans	*m_pUdpComm_FileTran;

private:

	//XFilter<MSG_ID>	m_filterMsg;	// 消息过滤器
	CMD_WND_MAP		m_mapCmdWnd;
	CMD_WND_MAP		m_mapCmdResendWnd;
	
	BOOL  m_bRunning ;
	//IM服务器IP地址
	DWORD m_dwIMServerIP		;
	WORD  m_wIMServerPort;
	// 中转服务器
	DWORD	m_dwTranServerIP;
	WORD	m_wTranServerPort;
	uint32	m_wSequenceID			;
	uint32	m_wLastAckSequenceID  ;
	// 记录当前发送的文件块的字典，按发送序列号来编排
	FILESEG_DIC	m_dicFileSeg;

private:
	BYTE  m_P2pKey[16];
	BYTE  m_SessionKey[16]		;// 登录成功后,client使用sessionkey作为密钥
	BYTE  m_RandomKey [16]		;
	char  m_szError[255]        ;

	bool  m_bLockUserPay		;
	uint8 m_nPayResult			;
};

extern CIMProtocol * g_pProtocol;

#endif // !defined(AFX_DATACENTER_H__73413291_442C_4FA0_8E05_DBDCE77AF3F4__INCLUDED_)
