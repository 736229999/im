// ----------------------------------------------------------------
// 文件:MMTalker.h
// 版本: 
// 作者: xy
// 日期: 2007-6-12
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

//#define _TCP_TEST
//#define _LOCAL_TEST
//#define _MY_UDP

#pragma once
#include "MMTalk.h"
#include "MMAudioPlayer.h"
#include "MMVideoPlayer.h"
#include "MMDataMaker.h"

#include "MiniNetTrans.h"

#pragma comment(lib,"MiniNetTrans.lib")

#ifdef _DEBUG
#define _TRACE_BITRATE
#endif



class CMMTalker:public CMMDataParse
{
public:

	CMMTalker(void);

	~CMMTalker(void);

public:

	//------------------------------------------------------
	// 初始化视频设备 
	//------------------------------------------------------
	BOOL				InitVideo(
		const CRect	&	rc,
		HWND			hParent,
		double			dbQuality = 0,
		int				nCapIdx = 0
	);

	//------------------------------------------------------
	// 初始化音频设备
	//------------------------------------------------------
	BOOL				InitAudio(HWND hWnd,WAVEFORMATEX *	pWfxIn = NULL);

	//------------------------------------------------------
	// 初始化聊天环境
	// nBufCount		为视频音频采集数据提供的缓冲帧的个数
	// pFnc				视频帧绘制实现函数
	//------------------------------------------------------
	BOOL				Init(VideoPlayerDrawFnc pFnc,DWORD dwUser,uint32 nMyId,CUdpNetTrans * pNetTran,int nBufCount = 5);

	//------------------------------------------------------
	// 开始和用户聊天
	//------------------------------------------------------
	void				StartTalk(uint32 nUserID,uint32 nIP,uint16 nPort,uint32 nTranIP,uint16 nTranPort);

	//------------------------------------------------------
	// 等待用户的连接
	//------------------------------------------------------
	void				WaitTalk(int nUserID,uint32 ip,uint16 port,uint32 nTranIP,uint16 nTranPort);

	//------------------------------------------------------
	// 开始视频采集
	//------------------------------------------------------
	void				StartVideoCap();

	//------------------------------------------------------
	// 停止视频采集
	//------------------------------------------------------
	void				CloseVideoCap();
	
	//------------------------------------------------------
	// 开始音频采集
	//------------------------------------------------------
	void				StartAudioCap();

	//------------------------------------------------------
	// 停止音频采集
	//------------------------------------------------------
	void				StopAudioCap();

	//------------------------------------------------------
	// 停止语音视频聊天
	//------------------------------------------------------
	void				Stop();

	//------------------------------------------------------
	// 设置播放音量大小(0-100)
	//------------------------------------------------------
	void				SetPlayVolume(int n);

	//------------------------------------------------------
	// 设置录制音量大小(0-100)
	//------------------------------------------------------
	void				SetRecordVolume(int n);

	//------------------------------------------------------
	// 设置视频质量(0-100)
	//------------------------------------------------------
	void				SetVideoQuality(int n);

	//------------------------------------------------------
	// 获取视频帧的格式
	//------------------------------------------------------
	BITMAPINFO *		GetVideoFormat();

	//------------------------------------------------------
	// 设置每秒采集的帧数
	//------------------------------------------------------
	//void				SetFramePerSec(WORD nC);

//
// 接口
//

protected:

	//------------------------------------------------------
	// 聊天连接成功事件
	//------------------------------------------------------
	virtual void		OnConnectTalkOk(bool bOk){};

	//------------------------------------------------------
	// 接受聊天成功事件
	//------------------------------------------------------
	virtual void		OnAcceptTalkOk(bool bOk){};

protected:

	//------------------------------------------------------
	// 将采集的数据发送到接收方（接口）
	//------------------------------------------------------
	static void			SendData(XDataPoolEx * pPool,DWORD dwUser);

	//------------------------------------------------------
	// 将采集的数据发送到接收方（实现）
	//------------------------------------------------------
	void				OnSendData(XDataPoolEx * pPool);

	//------------------------------------------------------
	// 接收网络数据（接口）
	//------------------------------------------------------
	static DWORD WINAPI	RecvData(void * pParam);

	//------------------------------------------------------
	// 接受网络数据（实现）
	//------------------------------------------------------
	void				OnRecvData();

	//------------------------------------------------------
	// 发送连接请求（接口）
	//------------------------------------------------------
	static DWORD WINAPI ConnectProc(void * param);

	//------------------------------------------------------
	// 发送连接请求（实现）
	//------------------------------------------------------
	void				OnConnect(uint32 fid,uint32 ip,uint16 port,uint32 tranIp,uint16 tranPort);

	//------------------------------------------------------
	// 发送接受请求（接口）
	//------------------------------------------------------
	static DWORD WINAPI AcceptProc(void * param);

	//------------------------------------------------------
	// 发送接受请求（实现）
	//------------------------------------------------------
	void				OnAccept(uint32 fid,uint32 ip,uint16 port,uint32 tranIp,uint16 tranPort);

protected:

	//------------------------------------------------------
	// 收到音频数据帧
	//------------------------------------------------------
	bool				OnAudioFrame(const BYTE * pData,int nLen);

	//------------------------------------------------------
	// 收到视频数据帧
	//------------------------------------------------------
	bool				OnVideFrame(const BYTE * pData,int nLen);

	//------------------------------------------------------
	// 收到控制数据帧
	//------------------------------------------------------
	bool				OnControlFrame(const BYTE * pData,int nLen);

	//------------------------------------------------------
	// 编码音频视频数据
	//------------------------------------------------------
	int					EncodeData(const BYTE * pData,int nLen);

	//------------------------------------------------------
	// 压缩音频数据
	//------------------------------------------------------
	int					CmpAudioFrame(MMAudioFrame * pFrame);

	//------------------------------------------------------
	// 压缩视频数据
	//------------------------------------------------------
	int					CmpVideoFrame(MMVideoFrame * pFrame);

protected:

	CMMDataMaker		m_dataMaker;

	CMMAudioPlayer		m_aplayer;

	CMMVideoPlayer		m_vplayer;

	CMMAudioCodec		m_acodec;

	CMMVideoCodec		m_vcodec;

	CMMAudioCap			m_acap;

	CMMVideoCap			m_vcap;

	BYTE *				m_pDataSend;

	int					m_nDataSend;	

	BYTE *				m_pDataRecv;

	int					m_nDataRecv;
#ifdef _MY_UDP
	CUdpNetTrans		m_netTran;

	SESSION_ID			m_session;
#else
	CUdpNetTrans *		m_pDataTran;	// 数据传输模块
	
	XSOCKET				m_netsoc;

#endif

	uint32				m_nMyId;					

	HANDLE				m_hThreadRecv;

	HANDLE				m_hThreadConnect;

	HANDLE				m_hThreadAccpet;

#ifdef _TCP_TEST
	SOCKET				m_socketListen;
	SOCKET				m_socketConn;
	SOCKET				m_socketAccept;
#endif

#ifdef _TRACE_BITRATE
	int				m_nData;
	int				m_nLastTime;
	int				m_nTotData;
	int				m_nTotTime;
#endif

};
