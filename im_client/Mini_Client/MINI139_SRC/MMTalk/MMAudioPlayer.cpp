#include "StdAfx.h"
#include ".\mmaudioplayer.h"

CMMAudioPlayer::CMMAudioPlayer(void)
{
	m_pCodec = NULL;
}

CMMAudioPlayer::~CMMAudioPlayer(void)
{

}


BOOL CMMAudioPlayer::Init(HWND hWnd,WAVEFORMATEX * pWfx,CMMAudioCodec * pCodec)
{

//#ifdef _DEBUG
//	if(m_directSound.Init(hWnd,pWfx) == FALSE)
//		return FALSE;
//#endif
	//if(m_directSound.Init(hWnd,pWfx,pCodec->GetPcmLen()) == FALSE)
	if(m_directSound.Init(hWnd,pWfx,1152 * 2,1152 * 100) == FALSE)
		return FALSE;

	//m_directSound.SetBuffering(1024*10);
	m_pCodec = pCodec;
	return TRUE;
}

void CMMAudioPlayer::PlayAudio(MMAudioFrame * pAudio)
{
	int nLen = pAudio->dwLen;
	if(m_pCodec == NULL)
		return;
	const BYTE * pData = m_pCodec->DecmpWaveData((BYTE *)pAudio->data,nLen);	
	if(pData)
	{
		m_directSound.Play(pData,nLen);
	}
	else
	{
		TRACE("“Ù∆µΩ‚—π ß∞‹\n");
	}
}

void CMMAudioPlayer::SetAudioVolume(int n)
{
	m_directSound.SetVolume(n);
}


void CMMAudioPlayer::Stop()
{
	m_directSound.Stop();
}