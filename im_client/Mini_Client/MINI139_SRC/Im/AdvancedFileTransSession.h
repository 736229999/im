// ----------------------------------------------------------------
// 文件:AdvancedFileTransSession.h
// 版本: 
// 作者: xy
// 日期: 2007-1-9
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once
#include <mmsystem.h>

#include "P2PFile.h"
#include "IMProtocol.h"
#include <map>
using namespace std;


struct PEER_ENDPOINT
{
	uint32	ip;
	uint16	port;
	uint32	localIp;
	uint16	localPort;
	uint8	sessionkey[16];
};

enum FILE_TRAN_ERR
{
	FTE_OK = 0,
	FTE_GET_USERIP_TIMEOUT,		// 获取好友ip失败
	FTE_SEND_USER_CANCEL,		// 用户取消
	FTE_RECV_USER_CANCEL,		// 用户取消
	FTE_SEND_FILE_TIMEOUT,
	FTE_SEND_FILE_ERR,			// 发送文件请求超时
	FTE_DOWNLOAD_ERR,			// 下载出现错误
	FTE_DOWNLOADFILE_TIMEOUT,	// 请求下载文件超时
	FTE_DOWNLOADFILE_NOTEXIST,	// 请求下载的文件已经不存在
	FTE_CONNECT_TRANS_FAILED,	// 连接中转服务器失败
	FTE_P2P_CONN_FAILED,		// p2p连接失败
	FTE_P2P_ACCEPT_FAILED,		// p2p监听失败
	
};

enum TASK_STATUS
{
	TS_P2P_START_CONNECT,		// 开始准备p2p连接
	TS_P2P_START_DOWNLOAD,		// p2p连接成功，接收者B开始下载
	TS_P2P_WAIT_DOWNLOAD,		// p2p连接成功，发送者A等待接收者B下载
	TS_TRANS_LOGON,				// 登录中转服务器
	TS_TRANS_OK,				// 通过中转服务器建立连接成功
};

class CAdvancedFileTransSession
{
public:

	CAdvancedFileTransSession();
	
	virtual ~CAdvancedFileTransSession();

	// 发送文件返回状态
	enum SENDFILE_RESULT{
		SR_OK,						// 成功
		SR_OPEN_FAILED,				// 文件打开失败
		SR_SIZE_ZERO,				// 文件大小为0
		SR_GET_FILE_INFO_FAILED,	// 获取文件大小信息失败
		SR_ID_REPEAT,				// 指定的文件id已经存在
		SR_FILE_REPEAT,				// 文件已经存在
		SR_DIRECTORY,
	};
	
	friend class CP2PRecvFile;
	friend class CP2PSendFile;

public:
	//------------------------------------------------------
	// 中止接收所有文件 
	//------------------------------------------------------
	void			    AF_AbortAllRecvFiles(uint32 senderID);

	//------------------------------------------------------
	// 中止发送所有文件
	//------------------------------------------------------
	void				AF_AbortAllSendFiles();

	//------------------------------------------------------
	// 中止接收文件 
	//------------------------------------------------------
	void				AF_AbortRecvFile(uint32 id);

	//------------------------------------------------------
	// 中止发送文件
	//------------------------------------------------------
	void				AF_AbortSendFile(uint32 id);

	//------------------------------------------------------
	// 同意或拒绝接收文件
	//------------------------------------------------------
	bool				AF_AcceptFile(
		uint32 nFileID,
		uint32 nSize,
		const CString & sLocalName,
		uint32 nStartPos,
		uint32 nTranIp,
		uint16 nTranPort,
		bool bAccept = true
	);

	//------------------------------------------------------
	// 是否有正在发送的文件
	//------------------------------------------------------
	bool				AF_HasSendingFile();

	//------------------------------------------------------
	// 是否有正在接收的文件
	//------------------------------------------------------
	bool				AF_HasDownloadingFile();


	void				AF_SetRecvFilePath(int nFileID, CString strPath);

	//------------------------------------------------------
	// 开始发送一个文件
	//------------------------------------------------------
	SENDFILE_RESULT		AF_SendFile(const CString & sFullPath, long nFileID);


	//------------------------------------------------------
	// 设置信息
	//------------------------------------------------------
	void				AF_SetInfo(uint32 nMyId,uint32 nFriendId,uint32 destIP,uint16 destPort,uint32 myTranIp,uint16 myTranPort,CUdpNetTrans * pTran);
	

	CUdpNetTrans *		AF_GetDataTran() 
	{
		return		m_pDataTran;
	}

	void				AF_NotifyRecvOk(uint32 fid);

//------------------------------------------------------
// 接口
//------------------------------------------------------

protected:

	//------------------------------------------------------
	// 接收者返回同意或拒绝意见
	//------------------------------------------------------
	virtual void		AF_OnSendFileReturn(uint32 id,bool bAccept) = 0;

	//------------------------------------------------------
	// 当前接收任务的状态
	//------------------------------------------------------
	virtual void		AF_OnRecvStatusNotify(uint32 id,TASK_STATUS status) {};

	//------------------------------------------------------
	// 当前发送任务的状态
	//------------------------------------------------------
	virtual void		AF_OnSendStatusNotify(uint32 id,TASK_STATUS status) {};

	//------------------------------------------------------
	// 当前任务接收速度
	//------------------------------------------------------
	virtual void		AF_OnRecvSpeedNotify(uint32 id,int nBPS,float fPercent) = 0;

	//------------------------------------------------------
	// 当前任务发送速度
	//------------------------------------------------------
	virtual void		AF_OnSendSpeedNotify(uint32 id,int nBPS,float fPercent) = 0;

	//------------------------------------------------------
	// 发送文件成功
	//------------------------------------------------------
	virtual void		AF_OnSendOK(uint32 id) = 0;

	//------------------------------------------------------
	// 发送文件失败
	//------------------------------------------------------
	virtual void		AF_OnSendFailed(uint32 id,FILE_TRAN_ERR err) = 0;

	
	//------------------------------------------------------
	// 下载文件成功
	// 参数：	id		会话id
	//			nSpeed	平均下载速度bps
	//------------------------------------------------------
	virtual void		AF_OnDownloadOK(uint32 id) = 0;

	//------------------------------------------------------
	// 下载失败,超时
	//------------------------------------------------------
	virtual void		AF_OnDownloadFailed(uint32 id,FILE_TRAN_ERR err) = 0;

	//------------------------------------------------------
	// 收到中止传输请求
	//------------------------------------------------------
	virtual void		AF_OnRecvAbortTranReq(const P2P_ABORT_TRAN_REQ & req) = 0;

	//------------------------------------------------------
	// 询问是否可以通过服务器中转
	//------------------------------------------------------
	virtual bool		AF_CanTrans(uint32 id){ return true;};
	

protected:

	//------------------------------------------------------
	// 为下载成功的文件改名,返回改后名称
	//------------------------------------------------------
	CString				AF_RenameDownloadFile(const CString & sName);

	//------------------------------------------------------
	// 收到好友的ip信息
	//------------------------------------------------------
	void				AF_OnGetStatusOK(const PEER_ENDPOINT & pt);
	
	//------------------------------------------------------
	// 获取好友的ip信息失败
	//------------------------------------------------------
	void				AF_OnGetStatusFailed();

	//------------------------------------------------------
	// p2p连接测试成功
	//------------------------------------------------------
	void				AF_OnP2PTestOK(XT_P2PTEST_ACK * pAck);

	//------------------------------------------------------
	// p2p连接测试失败
	//------------------------------------------------------
	void				AF_OnP2PTestFailed();

	//------------------------------------------------------
	// 传输完成请求
	//------------------------------------------------------
	void				AF_OnRecvFinTranReq(const P2P_FIN_TRAN_REQ & req);

	//------------------------------------------------------
	// 超时重传,出现拥塞 
	//------------------------------------------------------
	void				AF_OnDownloadCongestion(SendDataStruct * pSendData);
	
	//
	//------------------------------------------------------
	// 网络事件通知
	//------------------------------------------------------
	//

	void				AF_RecvSendFileAck(const char *data,WORD wRet);

	void				AF_RecvDownloadFileReq(const char *data,WORD wRet);

	void				AF_RecvDownloadFileAck(const char *data,WORD wRet);

	void				AF_RecvFinTranReq(const char *data,WORD wRet);

	void				AF_RecvFinTranAck(const char *data,WORD wRet);

	void				AF_RecvAbortTranReq(const char *data,WORD wRet);

	void				AF_RecvAbortTranAck(const char *data,WORD wRet);

	void				AF_RecvNotifyLoginReq(const char *data,WORD wRet);

	void				AF_RecvALoginTransAck(const char *data,WORD wRet);

	void				AF_RecvBLoginTransAck(const char *data,WORD wRet);

	void				AF_RecvP2PTestReq(const char * data,WORD wRet);

	void				AF_RecvTransDataAck(const char * data,WORD wRet);

	// 接收端接收到文件包
	void                AF_OnRecvFileAck(const char *data,WORD wRet);

	// 服务端收到应答
	void                AF_OnSenderRecvAck(const char *data,WORD wRet);

	// 文件传输开始时，客户端发送给服务端通告消息后收到应答
	void				AF_OnRecvTranFileNotifyAck(const char *data,WORD wRet);
	//
	//------------------------------------------------------
	// 任务池管理
	//------------------------------------------------------
	//

	CP2PRecvFile *		AF_FindRecvFile(uint32 id);

	void				AF_DeleteRecvFile(uint32 id);
	
	CP2PSendFile *		AF_FindSendFile(uint32 id);

	void				AF_DeleteSendFile(uint32 id);

	void				AF_StopSendFile(CP2PSendFile * pSendFile);

	void				AF_StopRecvFile(CP2PRecvFile * pRecvFile);

	//CP2PRecvWnd *		AF_FindRecvWnd(P2PID id);

private:

	
	//------------------------------------------------------
	// 连接到中转服务器
	//------------------------------------------------------
	void				AF_ConnectTrans();

protected:

	typedef map<uint32,CP2PRecvFile* >		P2PRecvPool;
	typedef map<uint32,CP2PSendFile* >		P2PSendPool;


	// 任务池

	P2PRecvPool		m_poolRecv;		// 接收文件池
	P2PSendPool		m_poolSend;		// 发送文件池
	P2PRecvPool		m_delPoolRecv;	// 垃圾
	P2PSendPool		m_delPoolSend;	// 垃圾
	int			m_nFileIDSeq;	// 文件序号



	uint32			m_nMyId;
	uint32			m_nFriendId;
	uint32			m_destIp;
	uint16			m_destPort;
	uint32			m_uMyTranIP;
	uint16			m_uMyTranPort;
	CSyncCritical	m_scSendPool;
	CSyncCritical	m_scRecvPool;

	
protected:

	static void  CALLBACK AF_TimerSpeedContrlProc(
		UINT uTimerID, 
		UINT uMsg, 
		DWORD_PTR dwUser, 
		DWORD_PTR dw1, 
		DWORD_PTR dw2
	);
	
	/*void				AF_DoRecvSpeedControl();

	void				AF_DoSendSpeedControl();*/

	void				AF_RegistXtCmd(HWND hWnd);

	void				AF_NotifyRecvSpeed();

	void				AF_NotifySendSpeed();

protected:

	MMRESULT			m_mrTimer;

	CUdpNetTrans *		m_pDataTran;

	map<int,CString>	m_mapRecvFilePath;
	BOOL m_bRecvFileTranNotifyAck;		// 是否收到文件传输应答
};
