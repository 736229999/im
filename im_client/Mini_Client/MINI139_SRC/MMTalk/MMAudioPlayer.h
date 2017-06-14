// ----------------------------------------------------------------
// �ļ�:MMAudioPlayer.h
// �汾: 
// ����: xy
// ����: 2007-6-19
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
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
