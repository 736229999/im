#include "StdAfx.h"
#include ".\musicplayer.h"

CMusicPlayer::CMusicPlayer(void)
{
	m_pPlayer = ISoundPlayer::CreatePlayer();
	m_pMp3 = NULL;
	m_pSound = NULL;
	m_hWnd = NULL;
}

CMusicPlayer::~CMusicPlayer(void)
{
	if(m_pPlayer != NULL)
	{
		delete m_pPlayer;
		m_pPlayer = NULL;
	}
	if (m_pMp3 != NULL)
	{
		delete m_pMp3;
		m_pMp3 = NULL;
	}
	if (m_pSound != NULL)
	{
		delete m_pSound;
		m_pSound = NULL;
	}
}

void CMusicPlayer::Init(HWND hWnd)
{
	m_hWnd = hWnd;
}

bool CMusicPlayer::SetUrl(CString strPlayingUrl)
{
	//m_httpFile.Open("http://podcache.cctv.com/published1/2007/07/05/pub1183603399403.mp3", "pub1183603399403.mp3");

	//CString s = "http://podcache.cctv.com/published1/2007/07/05/pub1183603399403.mp3";

	strPlayingUrl.Trim();
	CString sFile;
	const char * p = strPlayingUrl;
	int nC = strPlayingUrl.GetLength();
	for(int i = nC;i >= 0;i--)
	{
		if( *( p + i) == '\\' ||
			*(p + i) == '/')
		{
			sFile = p + i + 1;
			break;
		}
	}

	bool bOpen = m_httpFile.Open(strPlayingUrl,sFile);
	if(bOpen == false)
	{
		AfxMessageBox("无法打开链接");
		return false;
	}
	return true;
}

void CMusicPlayer::Open(void)
{
	bool b = m_pPlayer->Init(m_hWnd);
	b = m_mp3Codec.Init();

	//m_pPlayer->Load(m_pSound);
	if (m_httpFile.GetSize() > 1024 * 10)
	{
		if (m_pMp3 == NULL)
		{
			m_pMp3 = new XMp3Sound(&m_mp3Codec);
		}
		if (m_pMp3->Open(&m_httpFile) != XSound::OK)
		{
			AfxMessageBox( "open mp3 error1!");
		}
		if (m_pPlayer->Load(m_pMp3) != ISoundPlayer::OK)
		{
			AfxMessageBox( "open mp3 error2!" );
		}
		else
		{
			AfxMessageBox("load ok!");
		}

	}
}

void CMusicPlayer::Play( )
{

	ISoundPlayer::RESULT rst;
	rst = m_pPlayer->Play();

}

void CMusicPlayer::Pause(void)
{
	m_pPlayer->Pause();
}

void CMusicPlayer::Stop(void)
{
	m_pPlayer->Stop();
}

void CMusicPlayer::Seek(int nTime)
{
	m_pPlayer->Seek(nTime);
}




int CMusicPlayer::GetTotalTime(void)
{
	int nTotalTime;
	if(m_pMp3)
	{
		nTotalTime = m_pMp3->GetTotalTime();
		return nTotalTime;
	}
	else
		return 0;
}

int CMusicPlayer::GetCurTime(void)
{
	int nCurTime;
	if(m_pMp3)
	{
		nCurTime = m_pMp3->GetCurTime();
		return nCurTime;
	}
	else
		return 0;
}


void CMusicPlayer::SetVolume(long nVolume)
{
	m_pPlayer->SetVolume(nVolume);
}
