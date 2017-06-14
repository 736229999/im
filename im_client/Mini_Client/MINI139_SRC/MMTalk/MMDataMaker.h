// ----------------------------------------------------------------
// 文件:MMDataMaker.h
// 版本: 
// 作者: xy
// 日期: 2007-6-19
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once

#include "DirectSound.h"
#include "MMVideoCap.h"



typedef  void (*OnMMDataFnc)(XDataPoolEx * pPool,DWORD dwUser);



//------------------------------------------------------
// 音频视频数据加工厂
//------------------------------------------------------

class CMMDataMaker
{
public:

	friend class CMMTalker;

	CMMDataMaker(void);

	~CMMDataMaker(void);

public:

	//------------------------------------------------------
	// 初始化数据加工
	//------------------------------------------------------
	void			Init(
		CMMAudioCodec * pAudioCodec,
		CMMVideoCodec * pVideoCodec,
		int				nBufCount,
		OnMMDataFnc		pFncOnMMData,
		DWORD			dwUser
	);

	//------------------------------------------------------
	// 添加音频数据并加工
	//------------------------------------------------------
	void			AddAudioData(BYTE * pData,int nLen);

	//------------------------------------------------------
	// 添加视频数据并加工
	//------------------------------------------------------
	void			AddVideoData(BYTE * pData,int nLen,int nTime);

	//------------------------------------------------------
	// 添加控制帧
	//------------------------------------------------------
	void			AddControlData(DWORD dwCmd,BYTE * pData,int nLen);

	//------------------------------------------------------
	// 清除数据
	//------------------------------------------------------
	void			Clear();

	//------------------------------------------------------
	// 判断是否已经初始化
	//------------------------------------------------------
	bool			IsInit();

protected:

	//------------------------------------------------------
	// 处理采集的视频数据
	//------------------------------------------------------
	static void		OnVideoData( BYTE * pData,int nLen,int nTime,DWORD dwUser);

	//------------------------------------------------------
	// 处理采集的音频数据
	//------------------------------------------------------
	static void		OnAudioData( BYTE * pData,int nLen,DWORD dwUser);

	//------------------------------------------------------
	// 工作线程（接口）
	//------------------------------------------------------
	static DWORD WINAPI	WorkThreadProc(LPVOID lpParam);

	//------------------------------------------------------
	// 工作线程
	//------------------------------------------------------
	void			WorkProc();
	
protected:


	CMMAudioCodec * m_pAudioCodec;	// 音频编码器
	
	CMMVideoCodec *	m_pVideoCodec;	// 视频编码器

	XDataPoolEx		m_dataPool;		// 已加工的数据队列

	HANDLE			m_hThreadWork;	// 工作线程

	Common::CXEvent	m_eventWork;	// 工作线程通知事件

	Common::CXEvent	m_eventQuit;	// 退出工作线程通知事件

	OnMMDataFnc		m_pFncOnMMData;	// 数据处理的回调函数

	DWORD			m_dwUser;		// 用户数据

	bool			m_bInit;		// 是否已经初始化

};
