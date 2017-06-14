#pragma once

#include <Windows.h>
#include <MMSystem.h>
#include <mmreg.h>
#include <Msacm.h>

#include <dsound.h>

#include "XSound.h"
#include "ISoundPlayer.h"

class XSoundPlayer:public ISoundPlayer
{
public:

	XSoundPlayer(void);

	~XSoundPlayer(void);

public:
	
	bool						Init(HWND hWnd);

	RESULT						Load(XSound * pSound);

	RESULT						Play();

	RESULT						Seek(int nTime);

	void						Pause();

	void						Stop();

	int							GetCurTime();

	bool						IsPlaying();

	void						SetVolume(long nVolume);


protected:

	bool						CreateDSB(WAVEFORMATEX * pWfx);

	static DWORD WINAPI			PlayThread(LPVOID lpParam);

	void						PlayNotify();

	//------------------------------------------------------
	// nBuf		即将播放的缓冲
	//------------------------------------------------------
	void						OnBufNotify(int nBuf);

protected:

	LPDIRECTSOUNDBUFFER			m_pDSB;							// directSound缓冲

	LPDIRECTSOUND				m_pDirectSound;					// directSound对象 

	HWND						m_hDirectSoundWnd;				// directSound对应的协作窗口句柄

	//HANDLE						m_phNotifEvents;			// 播放完成通知

	HANDLE						m_hNotifyEvent;					// 播放完成通知

	HANDLE						m_hStopEvent;					// 停止播放事件

	int							m_nCurNotify;					// 当前通知的位置

	DSBPOSITIONNOTIFY *			m_pPosNotify;					// 设置通知标志的数组

	HANDLE						m_hThreadPlay;					// 音频播放线程

	WAVEFORMATEX 				m_wfx;							// 当前播放格式

	XSound *					m_pSound;

	byte						m_bFill;

	bool						m_bWork;

	bool						m_bPause;

	int							m_nBufNotifySize ;

	int							m_nAudioBufCount;

	int							m_nCurPlayBuf;

	bool						m_bPlaying;
};
