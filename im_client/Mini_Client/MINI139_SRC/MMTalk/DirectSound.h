// ----------------------------------------------------------------
// 文件:DirectSound.h
// 版本: 
// 作者: xy
// 日期: 2007-5-29
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------
#pragma once

#include "MMCommon.h"
#include "XDataPool.h"
#include <fcl.h>
using namespace fcl;

typedef void (*AudioCapDataFnc)( BYTE * pWave,int nLen,DWORD dwUser);

typedef void (*WaveNotifyFnc)(BYTE val,DWORD dwUser);

//------------------------------------------------------
// 音频播放缓冲区
//------------------------------------------------------

//class CAudioBuf
//{
//public:
//
//	friend class CDirectSound;
//
//	CAudioBuf()
//	{
//		m_nExtSize = 0;
//		m_nNotifySize = 0;
//		m_nIntSize = 0;
//		m_pDSB = NULL;
//		m_nNeedBuff = 0;
//		m_nBuffingC = 0;
//		m_nLastPlayPos = 0;
//		m_bHasData = false;
//		m_nLeft = 0;
//		m_nLastNotify = 0;
//	};
//
//	~CAudioBuf()
//	{
//
//	};
//
//
//protected:
//
//	//------------------------------------------------------
//	// 将外部缓冲的数据写入内部缓冲
//	//------------------------------------------------------
//	void					WriteBuf();
//
//	//------------------------------------------------------
//	// 初始化音频播放缓冲区 
//	//------------------------------------------------------
//	void					Init(LPDIRECTSOUNDBUFFER pDSB,int nExtBufSize,BYTE bFill,int nNotifySize,int nNotifyCount)
//	{
//		m_pDSB = pDSB;
//		m_nExtSize = nExtBufSize;
//		m_nIntSize = nNotifySize * nNotifyCount;
//		m_nNotifySize = nNotifySize;
//		m_dataPool.SetStackLen(nExtBufSize);
//		m_nLastNotify = m_nIntSize - nNotifySize;
//		m_bFill = bFill;
//	}
//
//	//------------------------------------------------------
//	// 获取外部缓冲的大小
//	//------------------------------------------------------
//	int						GetSize() const
//	{ return m_nExtSize; };
//
//
//	//------------------------------------------------------
//	// 添加数据到音频播放缓冲区，长度最好和缓冲块大小一致
//	//------------------------------------------------------
//	int						PushData(const BYTE * pData,int nLen);
//
//
//	//------------------------------------------------------
//	// 音频块播放结束通知
//	//------------------------------------------------------
//	void					OnBufNotify(int nPos);
//
//	//------------------------------------------------------
//	// 
//	//------------------------------------------------------
//	void					SetBuffing(int nBuffing)
//	{
//		m_nNeedBuff = nBuffing > m_nNotifySize ?  nBuffing : m_nNotifySize;
//		if(m_nNeedBuff > m_nIntSize)
//			m_nNeedBuff = m_nIntSize;
//		m_nBuffingC = m_nNeedBuff;
//		if(nBuffing == 0)
//		{
//			m_nNeedBuff = 0;
//			m_nBuffingC = 0;
//		}
//	}
//
//	void					RestoreBuff();
//
//	LPDIRECTSOUNDBUFFER		m_pDSB;			// directSound缓冲
//
//	int						m_nExtSize;		// 当前外部缓冲的大小
//
//	int						m_nNotifySize;	// 通知缓冲大小
//
//	int						m_nIntSize;		// 内部缓冲的大小
//
//	//int						m_nIntDataSize;	// 内部缓冲数据的大小
//
//	int						m_nLastPlayPos;	// 上次播放位置
//
//	BYTE					m_bFill;		// 填充静音
//
//	XStreamStack			m_dataPool;		// 数据缓冲区
//
//	int						m_nNeedBuff;	// 播放时需要缓冲的数据大小
//
//	int						m_nBuffingC;	// 播放时音频缓冲区的大小
//
//	//int						m_nDataEndPos;	// 内部缓冲内数据的结尾位置
//
//	bool					m_bHasData;		// 内部缓冲内是否包含数据
//
//	//bool					m_bCarry;		// 数据地址是否有进位
//
//	int						m_nLastNotify;	// 上次通知的位置
//
//	//int						m_nDataBeg;		// 数据开始地址
//
//	int						m_nDataEnd;		// 数据结束地址
//
//	int						m_nLeft;			// 内部缓冲内剩下没有播放的数据长度
//};

class CAudioBuf
{
public:

	friend class CDirectSound;

	CAudioBuf()
	{
		m_nExtSize = 0;
		m_nNotifySize = 0;
		m_nIntSize = 0;
		m_pDSB = NULL;
		m_nNeedBuff = 0;
		m_nBuffingC = 0;
		m_nLastPlayPos = 0;
		m_bHasData = false;
		m_nLeft = 0;
		m_nLastNotify = 0;
		m_nNotifyCount = 0;
	};

	~CAudioBuf()
	{

	};


protected:

	//------------------------------------------------------
	// 将外部缓冲的数据写入内部缓冲
	//------------------------------------------------------
	void					WriteBuf();

	//------------------------------------------------------
	// 初始化音频播放缓冲区 
	//------------------------------------------------------
	void					Init(LPDIRECTSOUNDBUFFER pDSB,int nExtBufSize,BYTE bFill,int nNotifySize,int nNotifyCount)
	{
		m_pDSB = pDSB;
		m_nExtSize = nExtBufSize;
		m_nIntSize = nNotifySize * nNotifyCount;
		m_nNotifyCount = nNotifyCount;
		m_nNotifySize = nNotifySize;
		m_dataPool.SetStackLen(nExtBufSize);
		m_nLastNotify = m_nIntSize - nNotifySize;
		m_bFill = bFill;
	}

	//------------------------------------------------------
	// 获取外部缓冲的大小
	//------------------------------------------------------
	int						GetSize() const
	{ return m_nExtSize; };


	//------------------------------------------------------
	// 添加数据到音频播放缓冲区，长度最好和缓冲块大小一致
	//------------------------------------------------------
	int						PushData(const BYTE * pData,int nLen);


	//------------------------------------------------------
	// 音频块播放结束通知
	//------------------------------------------------------
	void					OnBufNotify(int nPos);

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	void					SetBuffing(int nBuffing)
	{
		m_nNeedBuff = nBuffing > m_nNotifySize ?  nBuffing : m_nNotifySize;
		if(m_nNeedBuff > m_nIntSize)
			m_nNeedBuff = m_nIntSize;
		m_nBuffingC = m_nNeedBuff;
		if(nBuffing == 0)
		{
			m_nNeedBuff = 0;
			m_nBuffingC = 0;
		}
	}

	void					RestoreBuff();

	int						m_nNotifyCount;

	LPDIRECTSOUNDBUFFER		m_pDSB;			// directSound缓冲

	int						m_nExtSize;		// 当前外部缓冲的大小

	int						m_nNotifySize;	// 通知缓冲大小

	int						m_nIntSize;		// 内部缓冲的大小

	//int						m_nIntDataSize;	// 内部缓冲数据的大小

	int						m_nLastPlayPos;	// 上次播放位置

	BYTE					m_bFill;		// 填充静音

	XStreamStack			m_dataPool;		// 数据缓冲区

	int						m_nNeedBuff;	// 播放时需要缓冲的数据大小

	int						m_nBuffingC;	// 播放时音频缓冲区的大小

	//int						m_nDataEndPos;	// 内部缓冲内数据的结尾位置

	bool					m_bHasData;		// 内部缓冲内是否包含数据

	//bool					m_bCarry;		// 数据地址是否有进位

	int						m_nLastNotify;	// 上次通知的位置

	//int						m_nDataBeg;		// 数据开始地址

	int						m_nDataEnd;		// 数据结束地址

	int						m_nLeft;			// 内部缓冲内剩下没有播放的数据长度
};



//------------------------------------------------------
// 音频播放类
//------------------------------------------------------

class CDirectSound
{
public:
	
	CDirectSound(void);

	~CDirectSound(void);

	//static const int AUDIO_EXTBUF_SIZE = 1024 * 50;			// 外部缓冲大小

	static const int AUDIO_BUF_COUNT = 6;					// 内部缓冲个数

	//static const int AUDIO_NOTIFY_SIZE = 1024 * 2  ;		// 内部缓冲通知大小

public:

	//------------------------------------------------------
	// 播放
	//------------------------------------------------------
	void						Play(const BYTE * pData,int nLen);

	//------------------------------------------------------
	// 播放初始化
	//------------------------------------------------------
	BOOL						Init(
		HWND			hWnd,
		WAVEFORMATEX *	pWfx,
		int				nBufNotifySize 	,	// 内部缓冲通知大小
		int				nExtBufSize			// 外部缓冲大小
	);

	//------------------------------------------------------
	// 让播放器缓冲
	//------------------------------------------------------
	void						SetBuffering(int nBuffing)
	{ 
		m_audioBuf.SetBuffing(nBuffing);
	};

	//------------------------------------------------------
	// 设置音量(0~100)
	//------------------------------------------------------
	void						SetVolume(int nPercent);

	//------------------------------------------------------
	// 获取音量(0~100)
	//------------------------------------------------------
	int							GetVolume();

	//------------------------------------------------------
	// 停止
	//------------------------------------------------------
	void						Stop();

protected:

	//------------------------------------------------------
	// 播放线程
	//------------------------------------------------------
	static DWORD WINAPI			PlayThread(LPVOID lpParam);

	//------------------------------------------------------
	// 播放状态通知
	//------------------------------------------------------
	void						PlayNotify();

protected:

	LPDIRECTSOUNDBUFFER			m_pDSB;							// directSound缓冲

	LPDIRECTSOUND				m_pDirectSound;					// directSound对象 

	//HANDLE *					m_phNotifEvents;				// 播放完成通知

	HANDLE						m_hNotifyEvent;					// 播放完成通知事件

	HANDLE						m_hStopEvent;					// 停止播放事件

	int							m_nCurNotify;					// 当前通知的位置

	DSBPOSITIONNOTIFY *			m_pPosNotify;					// 设置通知标志的数组

	HANDLE						m_hThreadPlay;					// 音频播放线程

	WAVEFORMATEX 				m_wfx;							// 当前播放格式

	CAudioBuf					m_audioBuf;						// 待播放的音频缓冲区

	bool						m_bWork;
	//HANDLE						m_hCanPush;						// 音频缓冲可以使用的事件


};



//------------------------------------------------------
// 音频捕捉类
//------------------------------------------------------

class CMMAudioCap: public ITimerSink
{
public:
	
	CMMAudioCap();

	~CMMAudioCap();

	static const int	NUM_REC_NOTIFICATIONS = 8;

	static const int	SIZE_REC_BLOCK = 1024 * 4 ;

	static const int	NUM_CAP_BUF = 16;

public:

	//------------------------------------------------------
	// 初始化录音设备
	//------------------------------------------------------
	BOOL						Init(
		WAVEFORMATEX * pWfx,
		AudioCapDataFnc pFnc,
		DWORD dwUser
	);

	//------------------------------------------------------
	// 开始音频采集
	//------------------------------------------------------
	void						Start();

	//------------------------------------------------------
	// 停止音频采集
	//------------------------------------------------------
	void						Stop();

	//------------------------------------------------------
	// 设置波形通知时间间隔(单位ms)
	//------------------------------------------------------
	void						SetWaveNotify(WaveNotifyFnc pFnc,DWORD dwUser,int nTime);

	//------------------------------------------------------
	// 设置音频输入音量(0 - 100)
	//------------------------------------------------------
	void						SetCapVolume(DWORD nV);

protected:

	//------------------------------------------------------
	// 数据捕获线程
	//------------------------------------------------------
	static DWORD WINAPI			ThreadRecord(LPVOID lpParam) ;

	//------------------------------------------------------
	// 音频数据处理工作线程
	//------------------------------------------------------
	static DWORD WINAPI			ThreadWork(LPVOID lpParam);

	//------------------------------------------------------
	// 数据捕获线程
	//------------------------------------------------------
	void						ProcRecord();

	//------------------------------------------------------
	// 音频数据处理工作线程
	//------------------------------------------------------
	void						ProcWork();

	//------------------------------------------------------
	// 数据捕获通知 
	//------------------------------------------------------
	void						OnCapturedNotify();
	
	//------------------------------------------------------
	// 波形通知定时响应
	//------------------------------------------------------
	void						onTimer(uint32 lEventID );

	//------------------------------------------------------
	// 处理波形数据
	//------------------------------------------------------
	void						ProcWaveData(BYTE * pData,int nLen);

	//------------------------------------------------------
	// 判断数据是否为静音
	//------------------------------------------------------
	bool						IsSilence(BYTE * pData,int nLen);

	//------------------------------------------------------
	// 获得音频线路信息
	//------------------------------------------------------
	BOOL						GetLineInfo(LPMIXERLINE pmxl, DWORD dwComponentType);

	//------------------------------------------------------
	// 获取线路控制信息
	//------------------------------------------------------
	BOOL						GetLineControl(LPMIXERCONTROL pmxc, LPMIXERLINE pmxl, DWORD dwType);

	//------------------------------------------------------
	// 设置音量控制
	//------------------------------------------------------
	void						SetControlDetails(DWORD dwControlID,DWORD dwValue);


protected:

	typedef fclList<int>		MMList ;

	WaveNotifyFnc				m_pNotifyFnc;			// 波形通知回调函数

	DWORD						m_dwUserNotify;			// 用户数据

	int							m_nNotifyTime;			// 通知时间间隔

	MMList						m_lstWave;				// 波形采样

	int							m_nNotify;				// 通知间隔长度

	BYTE						m_bSilence;				// 静音

	bool						m_bTimer;				// 是否有定时期

	LPDIRECTSOUNDCAPTURE		m_pDSC ;				// 录入设备对象指针 

	LPDIRECTSOUNDCAPTUREBUFFER	m_pDSBCapture;			// 录入缓冲区对象指针
	
	WAVEFORMATEX				m_wfxInput;				// 输入的音频格式

	DSBPOSITIONNOTIFY			m_aPosNotify[ NUM_REC_NOTIFICATIONS + 1 ]; //设置通知标志的数组
	
	HANDLE						m_hNotificationEvent;	// 通知事件 

	HANDLE						m_hQuitRecord;			// 退出录制事件

	HANDLE						m_hQuitWork;			// 退出工作事件

	HANDLE						m_hThreadWork;			// 工作线程
	
	HANDLE						m_hThreadNotify;		// 录制通知线程 

	HANDLE						m_hSemSignal;			// 信号量
	
	DWORD						m_dwCaptureBufferSize;	// 录音用缓冲区的大小
	
	DWORD						m_dwNextCaptureOffset;	// 偏移位置
	
	DWORD						m_dwNotifySize;			// 通知位置

	AudioCapDataFnc				m_pCallback;			// 音频数据捕捉通知回调函数

	DWORD						m_dwUser;				// 用户数据

	XDataPool					m_audioData;			// 捕获的音频数据缓存

	UINT						m_uMixID;				// 混音器设备ID

	DWORD						m_dwMax;				// 最大音量

	DWORD						m_dwMin;				// 最小音量

};

//------------------------------------------------------
// 音频压缩解压类
//------------------------------------------------------

class CMMAudioCodec
{
public:
	
	CMMAudioCodec();

	~CMMAudioCodec();

public:

	//------------------------------------------------------
	// 根据指定的格式，找到对应的驱动程序ID
	//------------------------------------------------------
	static HACMDRIVERID			FindAcmDriverID(WORD wFormatTag);

	//------------------------------------------------------
	// 在驱动程序中查找指定格式，如果支持则返回波形格式。指针外部释放
	//------------------------------------------------------
	static WAVEFORMATEX *		FindWaveFormat(HACMDRIVERID hadid,WAVEFORMATEX * pWfx);

	//------------------------------------------------------
	// 用指定压缩格式初始化驱动器
	//------------------------------------------------------
	BOOL						Init(
		WAVEFORMATEX * pWfxSrc,
		WAVEFORMATEX * pWfxCmp,
		int nSrcBufLen = CMMAudioCap::SIZE_REC_BLOCK
	);

	//------------------------------------------------------
	// 压缩音频数据,输入长度超过缓冲长度会截断 
	//------------------------------------------------------
	BYTE *						CmpWaveData(/*IN*/ BYTE * pData,/*IN-OUT*/int & nLen);

	//------------------------------------------------------
	// 解压音频数据
	//------------------------------------------------------
	BYTE *						DecmpWaveData(/*IN*/ BYTE * pData,/*IN-OUT*/int & nLen);

	//------------------------------------------------------
	// 获取中间音频格式
	//------------------------------------------------------
	WAVEFORMATEX *				GetPcmWaveFormat()
	{ return m_pWfxPcm; };

	//------------------------------------------------------
	// 获取源音频格式
	//------------------------------------------------------
	WAVEFORMATEX *				GetSrcWaveFormat()
	{ return &m_wfxSrc; };
	
	//------------------------------------------------------
	// 获取压缩音频格式
	//------------------------------------------------------
	WAVEFORMATEX *				GetCmpWaveFormat()
	{ return m_pWfxCmp; };

	//------------------------------------------------------
	// 获取中间格式缓冲长度
	//------------------------------------------------------
	int							GetPcmLen()
	{
		int nPcmLen = (int)((double)m_nSrcBufLen * ((double)m_pWfxPcm->nAvgBytesPerSec / (double)m_wfxSrc.nAvgBytesPerSec));
		nPcmLen = (nPcmLen + 3) / 4 * 4;
		return nPcmLen;
	};

protected:

	void						Clear();

private:

	static BOOL CALLBACK		FindFormatProc(HACMDRIVERID hadid, LPACMFORMATDETAILS pafd, DWORD dwInstance, DWORD fdwSupport) ;

	static BOOL CALLBACK		FindDriverProc(HACMDRIVERID hadid, DWORD dwInstance, DWORD fdwSupport); 


protected:

	WAVEFORMATEX 				m_wfxSrc;			// 源格式

	WAVEFORMATEX *				m_pWfxCmp;			// 压缩格式

	WAVEFORMATEX *				m_pWfxPcm;			// 中间格式

	HACMDRIVERID				m_acmDID;			// 压缩驱动器id

	HACMDRIVER					m_had;				// 压缩驱动程序

	BYTE *						m_pBufPcm;			// 中间格式缓冲

	BYTE *						m_pBufCmp;			// 压缩缓冲

	BYTE *						m_pBufDecmp;		// 解压缓冲

	int							m_nSrcBufLen;		// 源数据缓冲长度 

	int							m_nPcmBufLen;		// pcm缓冲长度

	int							m_nCmpBufLen;		// cmp缓冲长度

};

