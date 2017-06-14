// ----------------------------------------------------------------
// 文件:MMVideoPlayer.h
// 版本: 
// 作者: xy
// 日期: 2007-6-15
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once

#include "MMVideoCap.h"
#include "MMData.h"
#include <fcl.h>
using namespace fcl;
#include "XDataPool.h"

typedef void (*VideoPlayerDrawFnc)(BYTE * pBmpData,DWORD dwUser);


class CMMVideoPlayer:public ITimerSink
{
public:

	CMMVideoPlayer(void);

	~CMMVideoPlayer(void);

public:

	BOOL				Init(CMMVideoCodec * pCodec);

	void				AddVideoFrame(MMVideoFrame * pFrame,int nLen);

	void				SetCallBackOnDraw(VideoPlayerDrawFnc pFnc,DWORD dwUser)
	{ m_pCallBack = pFnc; m_dwUser = dwUser; };

	void				SetFramePerSec(int n);

	void				Stop();

protected:

	void				onTimer(uint32 lEventID);	

protected:

	typedef fclList<XMemBlock *> MMVideoFrameList ;

	bool				m_bTimer;

	MMVideoFrameList	m_lstFrame;

	CMMVideoCodec *		m_pCodec;	

	VideoPlayerDrawFnc	m_pCallBack;

	DWORD				m_dwUser;

	XMemAlloc			m_alloc;

	DWORD				m_dwLastTick;

	int					m_nFramePerSec;

	int					m_nTimeSpan;

protected:

	DWORD				m_dwAccTime;		// 当前累计的空闲时间

	DWORD				m_dwLastPlayTime;	// 上一次播放时刻

};
