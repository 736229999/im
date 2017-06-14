// ----------------------------------------------------------------
// �ļ�:AdvancedFileTransSession.h
// �汾: 
// ����: xy
// ����: 2007-1-9
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
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
	FTE_GET_USERIP_TIMEOUT,		// ��ȡ����ipʧ��
	FTE_SEND_USER_CANCEL,		// �û�ȡ��
	FTE_RECV_USER_CANCEL,		// �û�ȡ��
	FTE_SEND_FILE_TIMEOUT,
	FTE_SEND_FILE_ERR,			// �����ļ�����ʱ
	FTE_DOWNLOAD_ERR,			// ���س��ִ���
	FTE_DOWNLOADFILE_TIMEOUT,	// ���������ļ���ʱ
	FTE_DOWNLOADFILE_NOTEXIST,	// �������ص��ļ��Ѿ�������
	FTE_CONNECT_TRANS_FAILED,	// ������ת������ʧ��
	FTE_P2P_CONN_FAILED,		// p2p����ʧ��
	FTE_P2P_ACCEPT_FAILED,		// p2p����ʧ��
	
};

enum TASK_STATUS
{
	TS_P2P_START_CONNECT,		// ��ʼ׼��p2p����
	TS_P2P_START_DOWNLOAD,		// p2p���ӳɹ���������B��ʼ����
	TS_P2P_WAIT_DOWNLOAD,		// p2p���ӳɹ���������A�ȴ�������B����
	TS_TRANS_LOGON,				// ��¼��ת������
	TS_TRANS_OK,				// ͨ����ת�������������ӳɹ�
};

class CAdvancedFileTransSession
{
public:

	CAdvancedFileTransSession();
	
	virtual ~CAdvancedFileTransSession();

	// �����ļ�����״̬
	enum SENDFILE_RESULT{
		SR_OK,						// �ɹ�
		SR_OPEN_FAILED,				// �ļ���ʧ��
		SR_SIZE_ZERO,				// �ļ���СΪ0
		SR_GET_FILE_INFO_FAILED,	// ��ȡ�ļ���С��Ϣʧ��
		SR_ID_REPEAT,				// ָ�����ļ�id�Ѿ�����
		SR_FILE_REPEAT,				// �ļ��Ѿ�����
		SR_DIRECTORY,
	};
	
	friend class CP2PRecvFile;
	friend class CP2PSendFile;

public:
	//------------------------------------------------------
	// ��ֹ���������ļ� 
	//------------------------------------------------------
	void			    AF_AbortAllRecvFiles(uint32 senderID);

	//------------------------------------------------------
	// ��ֹ���������ļ�
	//------------------------------------------------------
	void				AF_AbortAllSendFiles();

	//------------------------------------------------------
	// ��ֹ�����ļ� 
	//------------------------------------------------------
	void				AF_AbortRecvFile(uint32 id);

	//------------------------------------------------------
	// ��ֹ�����ļ�
	//------------------------------------------------------
	void				AF_AbortSendFile(uint32 id);

	//------------------------------------------------------
	// ͬ���ܾ������ļ�
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
	// �Ƿ������ڷ��͵��ļ�
	//------------------------------------------------------
	bool				AF_HasSendingFile();

	//------------------------------------------------------
	// �Ƿ������ڽ��յ��ļ�
	//------------------------------------------------------
	bool				AF_HasDownloadingFile();


	void				AF_SetRecvFilePath(int nFileID, CString strPath);

	//------------------------------------------------------
	// ��ʼ����һ���ļ�
	//------------------------------------------------------
	SENDFILE_RESULT		AF_SendFile(const CString & sFullPath, long nFileID);


	//------------------------------------------------------
	// ������Ϣ
	//------------------------------------------------------
	void				AF_SetInfo(uint32 nMyId,uint32 nFriendId,uint32 destIP,uint16 destPort,uint32 myTranIp,uint16 myTranPort,CUdpNetTrans * pTran);
	

	CUdpNetTrans *		AF_GetDataTran() 
	{
		return		m_pDataTran;
	}

	void				AF_NotifyRecvOk(uint32 fid);

//------------------------------------------------------
// �ӿ�
//------------------------------------------------------

protected:

	//------------------------------------------------------
	// �����߷���ͬ���ܾ����
	//------------------------------------------------------
	virtual void		AF_OnSendFileReturn(uint32 id,bool bAccept) = 0;

	//------------------------------------------------------
	// ��ǰ���������״̬
	//------------------------------------------------------
	virtual void		AF_OnRecvStatusNotify(uint32 id,TASK_STATUS status) {};

	//------------------------------------------------------
	// ��ǰ���������״̬
	//------------------------------------------------------
	virtual void		AF_OnSendStatusNotify(uint32 id,TASK_STATUS status) {};

	//------------------------------------------------------
	// ��ǰ��������ٶ�
	//------------------------------------------------------
	virtual void		AF_OnRecvSpeedNotify(uint32 id,int nBPS,float fPercent) = 0;

	//------------------------------------------------------
	// ��ǰ�������ٶ�
	//------------------------------------------------------
	virtual void		AF_OnSendSpeedNotify(uint32 id,int nBPS,float fPercent) = 0;

	//------------------------------------------------------
	// �����ļ��ɹ�
	//------------------------------------------------------
	virtual void		AF_OnSendOK(uint32 id) = 0;

	//------------------------------------------------------
	// �����ļ�ʧ��
	//------------------------------------------------------
	virtual void		AF_OnSendFailed(uint32 id,FILE_TRAN_ERR err) = 0;

	
	//------------------------------------------------------
	// �����ļ��ɹ�
	// ������	id		�Ựid
	//			nSpeed	ƽ�������ٶ�bps
	//------------------------------------------------------
	virtual void		AF_OnDownloadOK(uint32 id) = 0;

	//------------------------------------------------------
	// ����ʧ��,��ʱ
	//------------------------------------------------------
	virtual void		AF_OnDownloadFailed(uint32 id,FILE_TRAN_ERR err) = 0;

	//------------------------------------------------------
	// �յ���ֹ��������
	//------------------------------------------------------
	virtual void		AF_OnRecvAbortTranReq(const P2P_ABORT_TRAN_REQ & req) = 0;

	//------------------------------------------------------
	// ѯ���Ƿ����ͨ����������ת
	//------------------------------------------------------
	virtual bool		AF_CanTrans(uint32 id){ return true;};
	

protected:

	//------------------------------------------------------
	// Ϊ���سɹ����ļ�����,���ظĺ�����
	//------------------------------------------------------
	CString				AF_RenameDownloadFile(const CString & sName);

	//------------------------------------------------------
	// �յ����ѵ�ip��Ϣ
	//------------------------------------------------------
	void				AF_OnGetStatusOK(const PEER_ENDPOINT & pt);
	
	//------------------------------------------------------
	// ��ȡ���ѵ�ip��Ϣʧ��
	//------------------------------------------------------
	void				AF_OnGetStatusFailed();

	//------------------------------------------------------
	// p2p���Ӳ��Գɹ�
	//------------------------------------------------------
	void				AF_OnP2PTestOK(XT_P2PTEST_ACK * pAck);

	//------------------------------------------------------
	// p2p���Ӳ���ʧ��
	//------------------------------------------------------
	void				AF_OnP2PTestFailed();

	//------------------------------------------------------
	// �����������
	//------------------------------------------------------
	void				AF_OnRecvFinTranReq(const P2P_FIN_TRAN_REQ & req);

	//------------------------------------------------------
	// ��ʱ�ش�,����ӵ�� 
	//------------------------------------------------------
	void				AF_OnDownloadCongestion(SendDataStruct * pSendData);
	
	//
	//------------------------------------------------------
	// �����¼�֪ͨ
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

	// ���ն˽��յ��ļ���
	void                AF_OnRecvFileAck(const char *data,WORD wRet);

	// ������յ�Ӧ��
	void                AF_OnSenderRecvAck(const char *data,WORD wRet);

	// �ļ����俪ʼʱ���ͻ��˷��͸������ͨ����Ϣ���յ�Ӧ��
	void				AF_OnRecvTranFileNotifyAck(const char *data,WORD wRet);
	//
	//------------------------------------------------------
	// ����ع���
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
	// ���ӵ���ת������
	//------------------------------------------------------
	void				AF_ConnectTrans();

protected:

	typedef map<uint32,CP2PRecvFile* >		P2PRecvPool;
	typedef map<uint32,CP2PSendFile* >		P2PSendPool;


	// �����

	P2PRecvPool		m_poolRecv;		// �����ļ���
	P2PSendPool		m_poolSend;		// �����ļ���
	P2PRecvPool		m_delPoolRecv;	// ����
	P2PSendPool		m_delPoolSend;	// ����
	int			m_nFileIDSeq;	// �ļ����



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
	BOOL m_bRecvFileTranNotifyAck;		// �Ƿ��յ��ļ�����Ӧ��
};
