// ----------------------------------------------------------------
// 文件:MMAudio.h
// 版本: 
// 作者: xy
// 日期: 2007-5-18
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once

#include <mmsystem.h>
#include <mmreg.h>

#include <Msacm.h>
#pragma  comment(lib,"Msacm32.lib")

#include "Mmsystem.h"
#pragma comment(lib,"Winmm.lib")
#include "XDataPool.h"

#include "DirectSound.h"

typedef  void (*AudioCapDataFnc)( BYTE * pCmpWave,int nLen,DWORD dwUser);


class CMMAudio
{
public:

	CDirectSound	m_ds;
	
	CMMAudio(void);
	
	~CMMAudio(void);

	static const int	CAPBUF_LEN = 1024 * 1;
	
public:

	//------------------------------------------------------
	// 设置音频采集缓冲长度
	//------------------------------------------------------
	void					SetCapBufLen(int nLen);

	//------------------------------------------------------
	// 根据指定的格式，找到对应的驱动程序ID
	//------------------------------------------------------
	static HACMDRIVERID		FindAcmDriverID(WORD wFormatTag);

	//------------------------------------------------------
	// 在驱动程序中查找指定格式，如果支持则返回波形格式。指针外部释放
	//------------------------------------------------------
	static WAVEFORMATEX *	FindWaveFormat(HACMDRIVERID hadid,WORD wFormat);

	//------------------------------------------------------
	// 开始录制
	//------------------------------------------------------
	BOOL					StartRecord();

	//------------------------------------------------------
	// 停止录制
	//------------------------------------------------------
	void					StopRecord();
	
	//------------------------------------------------------
	// 设置压缩的音频捕获回调函数
	//------------------------------------------------------
	void					SetCallBackOnCmpData(AudioCapDataFnc pFnc,DWORD dwUser);

	//------------------------------------------------------
	// 播放音频数据 
	//------------------------------------------------------
	int						PlaySound(const BYTE * pData,int nLen);
		
	//------------------------------------------------------
	// 压缩音频数据 
	//------------------------------------------------------
	BYTE *					CmpWaveData(/*IN*/BYTE * pData,/*IN-OUT*/int & nLen);

	//------------------------------------------------------
	// 解压音频数据
	//------------------------------------------------------
	BYTE *					DecmpWaveData(/*IN*/BYTE * pData,/*IN-OUT*/int & nLen);

protected:

	//------------------------------------------------------
	// 音频数据
	//------------------------------------------------------
	void					OnWaveData(WAVEHDR * pData);

	//------------------------------------------------------
	// 关闭音频输入事件
	//------------------------------------------------------
	void					OnWaveInClose();

	//------------------------------------------------------
	// 初始化
	//------------------------------------------------------
	BOOL					Init();

	//------------------------------------------------------
	// 音频数据输出完成
	//------------------------------------------------------
	void					OnWaveOutDone();


private:

	//------------------------------------------------------
	// 内部使用的回调函数
	//------------------------------------------------------
	static void CALLBACK WaveInProc( HWAVEIN hwi, UINT uMsg,DWORD dwInstance, DWORD dwParam1,DWORD dwParam2);

	//------------------------------------------------------
	// 内部使用的回调函数
	//------------------------------------------------------
	static void CALLBACK WaveOutProc(HWAVEOUT hwo,UINT uMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2);


protected:

	WAVEFORMATEX			m_waveformIn;		// 音频捕捉格式
	
	HWAVEIN					m_hWaveIn ;			// 音频输入设备

	HWAVEOUT				m_hWaveOut;			// 音频输出设备

	AudioCapDataFnc			m_pCallBackCmpData;	// 已压缩的音频捕捉数据

	DWORD					m_dwUser;			// 用户数据

	PWAVEHDR				m_pInHeader1;		// 音频输入缓冲1
	
	PWAVEHDR				m_pInHeader2;		// 音频输入缓冲2

	PWAVEHDR				m_pOutHeader1;		// 音频输出缓冲1

	PWAVEHDR				m_pOutHeader2;		// 音频输出缓冲2

	BYTE *					m_pInBuf1;			// 音频输入数据缓冲1

	BYTE *					m_pInBuf2;			// 音频输入数据缓冲2

	BYTE *					m_pOutBuf1;			// 音频输出数据缓冲1

	BYTE *					m_pOutBuf2;			// 音频输出数据缓冲2

	BYTE *					m_pCmpData;			// 已压缩数据

	BYTE *					m_pCmpBufPcm;		// pcm压缩缓冲区

	BYTE *					m_pDecmpData;		// 解压的数据

	BYTE *					m_pCmpBufDst;		// 目的格式压缩缓冲区

	HACMDRIVERID			m_acmDID;			// 压缩驱动器id

	HACMDRIVER				m_had;				// 压缩驱动程序
	
	WAVEFORMATEX *			m_pWfxCmp;			// 压缩格式

	WAVEFORMATEX *			m_pWfxPcm;			// pcm中间格式

	int						m_nCapBufLen;		// 音频采集缓冲长度

	int						m_nCmpBufLen;		// 音频压缩缓冲长度

	bool					m_bRecording;		// 是否在录音

	XDataPool				m_audioData;		// 保存采集的音频数据

	//Common::CXEvent			m_eventPlay;		// 音频播放完成事件
	
	bool					m_bPlayOk;			// 音频播放完成
};
