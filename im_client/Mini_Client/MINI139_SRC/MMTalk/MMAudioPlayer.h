// ----------------------------------------------------------------
// 文件:MMAudioPlayer.h
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

#include "MMData.h"
#include "DirectSound.h"

class CMMAudioPlayer
{
public:

	CMMAudioPlayer(void);

	~CMMAudioPlayer(void);

public:

	BOOL				Init(HWND hWnd,WAVEFORMATEX * pWfx,CMMAudioCodec * pCodec);

	void				PlayAudio(MMAudioFrame * pAudio);

	void				SetAudioVolume(int n);

	void				Stop();

protected:

	CMMAudioCodec *		m_pCodec;

	CDirectSound		m_directSound;

};
