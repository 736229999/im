#pragma once

#include "impublic.h"

struct RECV_FILE_STRUCT
{
	char		file_name[255];
	int			nWaitTimes;	// 当服务器要求等待时设置的等待次数
	uint32		nClusterId;
	uint32		file_size ;
	uint32		file_id   ;
	uint32		from_id   ;
	uint16		nNextPacketId;
	uint16		nPacketCount;
	CStdioFile * fileObject;
	
	
	uint32   recv_size ;
	DWORD    recv_tick ;
	map<int,int> mapPacketRecv;

	RECV_FILE_STRUCT()
	{
		memset(file_name,0,sizeof(file_name));
		nNextPacketId = 0;
		nPacketCount = 0;
		fileObject = 0;
		file_size  = 0;
		file_id    = 0;
		recv_size  = 0;
		from_id    = 0;
		recv_tick  = GetTickCount();
		nWaitTimes = -1;
	}
};

struct SEND_FILE_STRUCT
{
	char    file_name[255];
	uint32  file_size ;
	uint32  file_id   ;
	uint32  send_size ;
	uint32  last_packet_id    ;//
	uint32  last_file_pointer ;
	int     window_size  ;
	int     ack_count    ;
	DWORD   last_ack_tick;
	uint32	cid;
	uint16	nPacketCount;
	

	CStdioFile * fileObject;

	SEND_FILE_STRUCT()
	{
		memset(file_name,0,sizeof(file_name));

		fileObject         = 0;
		file_size          = 0;
		file_id            = 0;
		send_size          = 0;
		last_packet_id     = 0;
		last_file_pointer  = 0;
		window_size        = 0;
		ack_count          = 0;
		last_ack_tick      = 0;
		nPacketCount	   = 0;
	}
};

// 重载原有发送图片结构体，新增消息体字段
struct SEND_FILE_STRUCT_NEW : public SEND_FILE_STRUCT
{
	std::tr1::shared_ptr<XT_MSG> pMsg;
	int nType;
	int nFailCount;//失真次数
	SEND_FILE_STRUCT_NEW()
		: pMsg(NULL), nType(0), nFailCount(0)
	{
	}

	// 替换url
	void ReplaceUrl(const char *pUrl)
	{
		if(pMsg == NULL)
		{
			return;
		}

		CString strData = pMsg->data;
		strData.Replace(file_name, pUrl);
		strcpy(pMsg->data, strData);
		pMsg->data_len = strlen(pMsg->data);
	}

	// 检查图片地址是否都已替换
	BOOL IsReplaceAll()
	{
		if(pMsg == NULL)
		{
			return TRUE; 
		}

		CString strData = pMsg->data;

		int nBgn = 0;
		int nIdx = -1;
		do 
		{
			nIdx = strData.Find("<image:{", nBgn);		// 还存在本地图片
			if(nIdx != -1)
			{
				return FALSE;
			}
			nBgn = nIdx + 8;
		} while (nIdx != -1);
		
		return TRUE;
	}

	// 发送消息
	void SendMsg();
};

class CSimpleFileTransSession
{
public:
	CSimpleFileTransSession(void);
	virtual ~CSimpleFileTransSession(void);

protected:
	void SF_SetTransMode( int type) { m_nType = type; }

protected:
	virtual void SF_InitXtMsg(XT_MSG &msg)=0;
	virtual void SF_OnAbortFile(const char * file_name)=0;
	virtual void SF_OnRecvFileComplete(const char *file_name)=0;
	virtual void SF_OnRecvFileReq(char *file_name) = 0;
	virtual void SF_OnRecvFilePacket(char *file_name, int file_id) {}

	void SF_RecvPfileReq(const XT_MSG & msg);
	virtual void SF_RecvPfileAck(const XT_MSG & msg);
	void SF_RecvPfilePacket(const XT_MSG & msg);
	virtual void SF_RecvPfilePacketAck(const XT_MSG &msg);	
	void SF_RecvAbortPfileTransmit(const XT_MSG & msg);
	// 群图片发起者通知本帐号下载
	bool SF_RecvStartDownloadCfileNotify(const XT_MSG & msg);

	virtual void SF_SendPfileReq(const IMAGE_FILE_STRUCT &is,int nPackets, std::tr1::shared_ptr<XT_MSG> &msgO, int &nType);
	// 开始上传图片的通知
	void SF_StartUploadCfileReq(const IMAGE_FILE_STRUCT &is,uint32 cid, int nPackets);
	void SF_StartUploadCfileAck(uint32 fid);
	void SF_UploadCfileAck(uint32 fid);
	void SF_DownloadCfile(const RECV_FILE_STRUCT * pDwnFile);
	void SF_DownloadCfileAck(const XT_DOWNLOAD_CPIC_ACK & ack);

	void SF_AbortPfileTransmit();

	bool SF_InitRecvFile(RECV_FILE_STRUCT *pRecv);

	RECV_FILE_STRUCT * SF_FindRecvFile(int file_id, uint32 sender );//send
	SEND_FILE_STRUCT_NEW * SF_FindSendFile(int file_id);
	void SF_RemoveRecvFile(int file_id, uint32 sender );
	void SF_RemoveSendFile(int file_id);
	BOOL ComparePicture(CString strFile1, CString strFile2);

protected:
	vector<RECV_FILE_STRUCT *>  m_vecRecvFiles;
	vector<SEND_FILE_STRUCT_NEW *>  m_vecSendFiles;
	int							m_nType   ; //P2P File directly flag
};

