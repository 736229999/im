// improtocol.h: IMЭ���
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


#define SERVER_TIMEOUT			0xFFFF  // ����������ʱ
#define NETWORK_DISCONNECT		0xFFFE	// �������ӶϿ�
#define IP_CANT_CONNECT			0xFFFD	// ip������

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

////A���ļ����鴫�䵽�����,����������B������////////////////////////////////////////////
#define BUFFER_SIZE 1024*50
typedef struct _SEND_FILE_PACK_REQ
{  
	uint32  send_id;         ////����ID
	uint32  fid;             ////����ID
	uint8  	flag;			 ////�ļ������ļ����ݡ��ļ�����
	enum  { SENDFILE_NAME = 1, SENDFILE_CONTENT = 2, SENDFILE_NAMEEND = 3 };
	uint32  file_id;         ////�ļ�ID
	uint32  pack_id;         ////������� 
	uint32  size;            ////���鳤��  
	char buf[BUFFER_SIZE];   ////�����ļ����� 
}SEND_FILE_PACK_REQ,*LPSEND_FILE_PACK_REQ; 

////A���ļ����鴫�䵽�����,����������B����Ӧ////////////////////////////////////////////
typedef struct _SEND_FILE_PACK_ACK
{  
	uint32  send_id;         ////����ID
	uint32  fid;             ////����ID
	uint8  	flag;			 ////�ļ������ļ����ݡ��ļ�����
	enum  { SENDFILE_NAME = 1, SENDFILE_CONTENT = 2, SENDFILE_NAMEEND = 3 };
	uint32  file_id;         ////�ļ�ID
	uint32  pack_id;         ////������� 
	uint32  size;            ////���鳤��  
	char buf[BUFFER_SIZE];   ////�����ļ����� 
}SEND_FILE_PACK_ACK,*LPSEND_FILE_PACK_ACK; 


////B�˴���������������������A�˵�ȷ������///////////////////////////////////////////////
typedef struct _SEND_FILE_PACK_IND
{  
	uint32  send_id;         ////����ID
	uint32  fid;             ////����ID
	uint8  	flag;			 ////�ļ������ļ����ݡ��ļ�����
	enum  { SENDFILE_NAME = 1, SENDFILE_CONTENT = 2, SENDFILE_NAMEEND = 3 };
	uint32  file_id;         ////�ļ�ID
	uint32  pack_id;         ////������� 
	uint32  size;            ////���鳤��  
}SEND_FILE_PACK_IND,*LPSEND_FILE_PACK_IND; 

////B�˴���������������������A�˵�ȷ�ϻظ�///////////////////////////////////////////////////
typedef struct _SEND_FILE_PACK_IND_ACK
{  
	uint32  send_id;         ////����ID
	uint32  fid;             ////����ID
	uint8  	flag;			 ////�ļ������ļ����ݡ��ļ�����
	enum  { SENDFILE_NAME = 1, SENDFILE_CONTENT = 2, SENDFILE_NAMEEND = 3 };
	uint32  file_id;         ////�ļ�ID
	uint32  pack_id;         ////������� 
	uint32  size;            ////���鳤��  
}SEND_FILE_PACK_IND_ACK,*LPSEND_FILE_PACK_IND_ACK; 


// �ļ�����Ϣ
struct IM_FILESEG_INFO
{	
	uint8		msgType;	// ��ʱȺ��̶�Ⱥ
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
	static int		s_nCurVer;	// ��ǰ����汾
	// ��Ϣ���ͷ�ʽ
	enum SEND_MSG_TYPE{
		SMT_P2P,		// p2p����
		SMT_LAN,		// ��������
		SMT_SERVER		// ��������ת
	};

	//enum SEND_DEST_TYPE{
	//	SDT_P2P,	// ���͵�p2p�˵�
	//	SDT_BUSI,	// ���͵�ҵ�������
	//	SDT_TRAN,	// ���͵���ת������
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
	//�����̣߳��Խ��յ������ݱ��Ľ��н�������ܣ����ݷַ�
	static UINT IMProtocolProc(LPVOID pParam,const RecvDataStruct * pData);
	static UINT	IMProtocolResend(LPVOID pParam,const SendDataStruct * pData);

public:
	//ע�ᴰ������
	void	RegistCmd(WORD dwType,HWND hWnd);

	void	RegistResendCmd(WORD dwCmd,HWND hWnd);

	//ע����������
	void	UnRegistCmd(WORD dwType,HWND hWnd);

	void	UnRegistCmd(HWND hWnd);

	//�����߳�
	bool	Start();

	bool	Stop();

	bool	IsRunning() { return m_bRunning;	}

	static	CIMProtocol * GetInstance();

	static DWORD s_nHeartBeat;
//
// �����Э����ص�����	
//
public:	
	int SendDownloadCPicReq(const XT_DOWNLOAD_CPIC_REQ & req,int nRetryTimes = 10);
	int SendCPicData(const XT_CPIC_DATA & picData);
	int SendGetSvrListReq(const tagSERVER_STATUSLIST_REQ& req);
	int SendClusterMsg(const XT_MSG &msg);
	int SendP2pTestPacket(const XT_P2PTEST_REQ &req);
	int SendP2PMsg(const XT_MSG &msg, int type=CIMProtocol::SMT_SERVER, int nSeqId = 0, int nInsertBuyerId = 0);
	int UserPayment(uint32 price,uint32 id, uint32 type);
	// ��ȡ���Ѽ���Ⱥ��־
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
	int SendReq(const XT_CREATE_CLUSTER_REQ &req,uint32 * pwSeqID=NULL/*out ����Э�鱨sequence id*/);
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
	int SendReq(const XT_CLUSTERMSG_RECV_SET_REQ& req);///Ⱥ��Ϣ��������
	int SendReq(const XT_MOD_FRIEND_COMMNAME_REQ & req);
	int SendReq(const XT_GET_FRIEND_BASICINFO_REQ &req);
	int SendReq(const char *pszUserLabel, uint32 uid);
	int SendReq(const XT_MODIFY_FRIENDFLAG_REQ &req);
	int SendReq(const XT_FRIENDINVITE_REQ &req);
	int SendReq(const XT_SMS_REQ &req);
	int SendReq(const XT_PROFILENAME_GET_REQ &req);////��ȡȺ��Ƭ��Ϣ
	int SendReq(const XT_MOD_PSW_REQ &req);////�û��޸ĵ�¼����
	int SendReq(const XT_GET_USER_STATUS_REQ &req);
	int SendReq(const XT_WEB_MORE_MSG_REQ &req);
	// ���º�̨�������ϵ�ͻ�����Ϣ��������Ϣ
	int SendReq(const XT_UPDATE_NEAR_COMM_CUST_INFO_REQ &req);

	// ��ȡ�̻������ϵ�Ŀͻ���Ϣ(������Ϣ)
	int SendReq(const XT_GET_NEAR_COMM_CUST_REQ &req);	
	////��ȡ�ͷ�CSID
	//int SendReq(const XT_USER_MAP_CSID_REQ &req);

	////A��B�����ļ�������������ת
	int SendReq(const SEND_FILE_PACK_REQ &req);

	/////////////////////////////////////////////////////////
	////B��A����ȷ�ϣ�����������ת
	int SendReq(const SEND_FILE_PACK_IND &req);

	// �����ѽӴ��ͻ�����
	int SendReq(const XT_RECEPTE_CUST_REQ &req);

	// �ͷ����õ�ǰ��������
	int SendReq(const XT_MONITOR_SET_BUYER_REQ &req);

	// ���ܼ�ؿͷ�
	int SendReq(const XT_MONITOR_START_REQ &req);

	//����ȡ����ؿͷ�
	int SendReq(const XT_MONITOR_END_REQ &req);

	//���ܲ���Ự
	int SendReq(const XT_MONITOR_INSERT_CHAT_REQ &req);

	//���ܽ��ֻỰ
	int SendReq(const XT_MONITOR_TRAN_CHAT_REQ &req);

#ifdef _BUSINESS_VESION
	int SendReq(const BaseReq &req, uint32 nCmd = CMD_GETMSGREMIND_REQ);
	int SendReq(const MsgRemindAck &req);
	int SendReq(const csTeamInfoReq &req);
#endif
	//�ϴ�ͷ��
	int SendHeadFileUploadReq(const IMAGE_FILE_STRUCT &req);//�ϴ�ͷ���ļ�
	int SendHeadFileUploadPacketReq(const SEND_FILE_STRUCT &req, char* buffer, int datalen);//�ϴ�ͷ���ļ���
	/////////////////////////////////////////////////////////
	//
	// p2p ͨѶ 
	// ��ɫ A ---> B
	// A=������,B=������
	//

	//------------------------------------------------------
	// ������A�����ļ�����
	//------------------------------------------------------
	int		SendSendFileReq(const P2P_SENDFILE_REQ & req);

	//------------------------------------------------------
	// ������B�����ļ���������
	//------------------------------------------------------
	int		SendDownloadFileReq(const P2P_DOWNLOAD_FILE_REQ & req);

	//------------------------------------------------------
	// ������B����������������
	//------------------------------------------------------
	//int		SendDownloadReq(const P2P_DOWNLOAD_REQ & req,CP2PRecvWnd * pWnd);

	//------------------------------------------------------
	// ������B�����ش�������������
	//------------------------------------------------------
	//int		QuickResendDownloadReq(const P2P_DOWNLOAD_REQ & req,CP2PRecvWnd * pWnd);

	//------------------------------------------------------
	// ������A�ṩ��������
	//------------------------------------------------------
	//int		SendDownloadAck(const P2P_DOWNLOAD_ACK & ack);

	//------------------------------------------------------
	// ������B������ɴ�������
	//------------------------------------------------------
	int		SendFinishTranReq(const P2P_FIN_TRAN_REQ & req);

	//------------------------------------------------------
	// ˫��������ֹ��������
	//------------------------------------------------------
	int		SendAbortTranReq(const P2P_ABORT_TRAN_REQ & req);

	//
	// ������ת�������������ݴ���
	// ��ɫ clientA	-> Tran server -> clientB
	// A=������,B=������,T=��ת������

	//------------------------------------------------------
	// A�ͻ���֪ͨB��¼����ת������
	//------------------------------------------------------
	void	P2SSendNotifyLogin(const XT_TRANS_NOTIFY_LOGIN_REQ & req);

	//------------------------------------------------------
	// A�ͻ���������ת������������Ự
	//------------------------------------------------------
	//int		C2TSendLogin(const XT_TRANS_LOGIN_REQ & req);

	//------------------------------------------------------
	// A�ͻ�������ת������ע���Ự
	//------------------------------------------------------
	//int		C2TSendCloseSession(const XT_TRANS_CLOSE_SEESION_REQ & req);
	
	//------------------------------------------------------
	// B�������ύ��¼/ע������ת������
	//------------------------------------------------------
	//int		C2TSendRecvLogin(const XT_TRANS_RECVLOGIN_REQ & req);

#ifdef _BUSINESS_VESION
	// �޸Ĺ���״̬
	int SendReq(const XT_MOD_HANGUP_STATE_REQ &req);
#endif
#ifdef _VER_INTERNAL_	// �ڲ���
	int SendReq(const XT_GET_EMPLOYEE_INFO_REQ &req);
#endif

#ifdef _BUSINESS_VESION
	// ��ȡ�����ϵ�ͻ��Ҳ�ҳ��ע
	int SendReq(const XT_GET_NEAR_COMM_CUST_REMARK_REQ& req);
	// �޸������ϵ�ͻ��Ҳ�ҳ��ע
	int SendReq(const XT_UPDATE_NEAR_COMM_CUST_REMARK_REQ& req);
#endif

	// �ļ����俪ʼʱ�����͵���Ϣ�����ڷ���˼�¼�ͻ��˵�ip�Ͷ˿�
	int SendReq_FileTranNotify();

	// ����ͷ�ת��
	int SendReq(const XT_CUSTSER_TRAN_REQ &req);

	// ���շ�����ת��ȷ����Ϣ
	int SendReq(const XT_CUSTSER_TRAN_ACK &req);

	int SendReq(const Buf_ClientSend &req);

	////����������Ϣ
	int GetOfflineMsgReq();
//
// �����Э����ص���Ӧ
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
	void OnSetProfileNameAck(const RecvDataStruct &recvData);////����Ⱥ��Ƭ
	void OnGetProfileNameAck(const RecvDataStruct &recvData);////��ȡȺ��Ƭ
	
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
	void OnModPswAck(const RecvDataStruct &recvData); ////�޸���������˷���ȷ��
	void OnGetCSIDFromUserIDAck(const RecvDataStruct &recvData);
	void OnGetUserStatusAck(const RecvDataStruct &recvData);
	void OnWebMoreMsgAck(const RecvDataStruct &recvData);
#ifdef _CS_PLAT_EX_
	// ��Լ�û��Ļ�Ӧ
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
	// �յ���������ת����
	//------------------------------------------------------
	void		RecvServerTran(const RecvDataStruct & recvData);
	void		RecvServerEncryptTran(const RecvDataStruct & recvData);
	void		DecryptFailedServerTran(WORD wRet,const SendDataStruct & sendData);
	void		ParseFailedServerTran(WORD wRet,const SendDataStruct & sendData);

	// �ļ����俪ʼʱ�����͸������ͨ����Ϣ�󣬷���˷��ص�ȷ����Ϣ
	void FileTranNotifyAck(const RecvDataStruct &recvData);

	// ���շ����յ��ͷ�ת������
	//void RecvCustSerTranReqInd(const RecvDataStruct &recvData);

	// ���ͷ��յ��ͻ�ת��ȷ����Ϣ
	//void RecvCustSerTranAckInd(const RecvDataStruct &recvData);

	// ���º�̨�������ϵ�ͻ�����Ϣ��������Ϣ
	void OnRecvUpdateCommInfoAck(const RecvDataStruct &recvData);

	// ��ȡ�̻������ϵ�Ŀͻ���Ϣ(������Ϣ)
	void OnRecvGetNearCommCustAck(const RecvDataStruct &recvData);
#ifdef _VER_INTERNAL_	// �ڲ���
	void OnRecvGetEmployeeInfoAck(const RecvDataStruct &recvData);		
#endif
#ifdef _BUSINESS_VESION
	// �����̻������ϵ�Ŀͻ��Ҳ�ҳ��ע(������Ϣ)
	void OnRecvUpdateNearCommCustRemarkAck(const RecvDataStruct &recvData);

	// ��ȡ�̻������ϵ�Ŀͻ��Ҳ౸ע(������Ϣ)
	void OnRecvGetNearCommCustRemarkAck(const RecvDataStruct &recvData);
	//�ͷ����õ�ǰ��������Ӧ��
	void OnMonitorSetBuyerAck(const RecvDataStruct &recvData);
	//���ܻ�ȡ�ͷ��Ӵ������
	void OnMonitorStartAck(const RecvDataStruct &recvData);
	//����ȡ����ؿͷ�Ӧ��
	void OnMonitorEndAck(const RecvDataStruct &recvData);
	//���ܲ���ỰӦ��
	void OnMonitorInsertChatAck(const RecvDataStruct &recvData);
	//���ܽ��ֻỰӦ��
	void OnMonitorTranChatAck(const RecvDataStruct &recvData);
	//��ȡ��������ѡ��Ӧ��
	void OnGetMsgRemindAck(const RecvDataStruct &recvData);
	//���ö�������ѡ��Ӧ��
	void OnSetMsgRemindAck(const RecvDataStruct &recvData);
	//��ȡ������Ϣ
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
	//�������巢����Ϣ
	//wParam,���ֽ������֣����ֽڴ�����
	//lParam,���Ļ����ַ
	int		SendNotify(WPARAM wParam,LPARAM lParam);
	int		SendResendNotify(WPARAM WPARAM,LPARAM lParam);

private:

	CIMProtocol();
	
	bool	IsValideServerPacket(const RecvDataStruct &recvData);

public:
	CUdpNetTrans	*m_pUdpComm;
	CUdpNetTrans	*m_pUdpComm_FileTran;

private:

	//XFilter<MSG_ID>	m_filterMsg;	// ��Ϣ������
	CMD_WND_MAP		m_mapCmdWnd;
	CMD_WND_MAP		m_mapCmdResendWnd;
	
	BOOL  m_bRunning ;
	//IM������IP��ַ
	DWORD m_dwIMServerIP		;
	WORD  m_wIMServerPort;
	// ��ת������
	DWORD	m_dwTranServerIP;
	WORD	m_wTranServerPort;
	uint32	m_wSequenceID			;
	uint32	m_wLastAckSequenceID  ;
	// ��¼��ǰ���͵��ļ�����ֵ䣬���������к�������
	FILESEG_DIC	m_dicFileSeg;

private:
	BYTE  m_P2pKey[16];
	BYTE  m_SessionKey[16]		;// ��¼�ɹ���,clientʹ��sessionkey��Ϊ��Կ
	BYTE  m_RandomKey [16]		;
	char  m_szError[255]        ;

	bool  m_bLockUserPay		;
	uint8 m_nPayResult			;
};

extern CIMProtocol * g_pProtocol;

#endif // !defined(AFX_DATACENTER_H__73413291_442C_4FA0_8E05_DBDCE77AF3F4__INCLUDED_)
