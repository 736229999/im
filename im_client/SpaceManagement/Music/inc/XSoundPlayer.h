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
	// nBuf		�������ŵĻ���
	//------------------------------------------------------
	void						OnBufNotify(int nBuf);

protected:

	LPDIRECTSOUNDBUFFER			m_pDSB;							// directSound����

	LPDIRECTSOUND				m_pDirectSound;					// directSound���� 

	HWND						m_hDirectSoundWnd;				// directSound��Ӧ��Э�����ھ��

	//HANDLE						m_phNotifEvents;			// �������֪ͨ

	HANDLE						m_hNotifyEvent;					// �������֪ͨ

	HANDLE						m_hStopEvent;					// ֹͣ�����¼�

	int							m_nCurNotify;					// ��ǰ֪ͨ��λ��

	DSBPOSITIONNOTIFY *			m_pPosNotify;					// ����֪ͨ��־������

	HANDLE						m_hThreadPlay;					// ��Ƶ�����߳�

	WAVEFORMATEX 				m_wfx;							// ��ǰ���Ÿ�ʽ

	XSound *					m_pSound;

	byte						m_bFill;

	bool						m_bWork;

	bool						m_bPause;

	int							m_nBufNotifySize ;

	int							m_nAudioBufCount;

	int							m_nCurPlayBuf;

	bool						m_bPlaying;
};
