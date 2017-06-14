#pragma once

#include"XMp3Sound.h"
#include"XHttpFileStream.h"
#include "ISoundPlayer.h"
#include "XSound.h"
class CMusicPlayer
{
public:

	CMusicPlayer(void);
	~CMusicPlayer(void);

public:

	void Open(void);
	void Play(void);
	void Pause(void);
	void Stop(void);
	void Seek(int nTime);

private:

	ISoundPlayer * m_pPlayer;
	XSound * m_pSound;
	XHttpFileStream m_httpFile;
	XMp3Sound * m_pMp3;
	XMp3Codec m_mp3Codec;

public:
	void Init(HWND hWnd);
private:
	HWND m_hWnd;

public:
	int GetTotalTime(void);
	int GetCurTime(void);
	bool SetUrl(CString strPlayingUrl);
	void SetVolume(long nVolume);
};
