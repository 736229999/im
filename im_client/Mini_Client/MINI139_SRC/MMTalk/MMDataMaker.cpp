#include "StdAfx.h"
#include ".\mmdatamaker.h"
#include "MMData.h"

CMMDataMaker::CMMDataMaker(void)
{
	m_bInit = false;
	m_hThreadWork = NULL;
}

CMMDataMaker::~CMMDataMaker(void)
{

}


 DWORD WINAPI CMMDataMaker::WorkThreadProc(LPVOID lpParam)
{
	CMMDataMaker * pThis = (CMMDataMaker *)lpParam;
	if(pThis)
	{
		TRACE("数据采集工作线程开始\n");
		pThis->WorkProc();
	}
	return 0;
}

void CMMDataMaker::WorkProc()
{
	DWORD dwRst;
	HANDLE events[2];
	events[0] = m_eventWork;
	events[1] = m_eventQuit;
	while(true)
	{
		dwRst = ::WaitForMultipleObjects(2,events,0,1000);
		if(dwRst == WAIT_OBJECT_0)
		{
			if(m_pFncOnMMData)
			{
				m_pFncOnMMData(&m_dataPool,m_dwUser);
			}
		}
		else if(dwRst == WAIT_TIMEOUT)
		{
			continue;
		}
		else
		{
			TRACE("!!!!数据采集工作线程结束！\n");
			break;
		}
	}
}

void CMMDataMaker::Init(CMMAudioCodec * pAudioCodec,
						CMMVideoCodec * pVideoCodec, 
						int nBufCount, 
						OnMMDataFnc pFncOnMMData, 
						DWORD dwUser )
{
	if(m_bInit)
		return;
	m_eventWork.ReCreate();
	m_eventQuit.ReCreate();
	m_pAudioCodec = pAudioCodec;
	m_pVideoCodec = pVideoCodec;
	m_dataPool.Init(nBufCount,1024 * 4);
	m_pFncOnMMData = pFncOnMMData;
	m_dwUser = dwUser;
	if(m_hThreadWork == NULL)
		m_hThreadWork = ::CreateThread(0,0,WorkThreadProc,this,0,0);
	m_bInit = true;
	
}

void CMMDataMaker::OnAudioData(BYTE * pData,int nLen,DWORD dwUser)
{
	CMMDataMaker * pThis = (CMMDataMaker *)dwUser;
	pThis->AddAudioData(pData,nLen);
}

void CMMDataMaker::OnVideoData(BYTE * pData,int nLen,int nTime,DWORD dwUser)
{
	CMMDataMaker * pThis = (CMMDataMaker *)dwUser;
	if ( nTime!=0 )
		pThis->AddVideoData(pData,nLen,nTime);
}

void CMMDataMaker::AddControlData(DWORD dwCmd,BYTE * pData,int nLen)
{
	MMControlFrame cFrame;
	int n1 = sizeof(MMControlFrame);
	cFrame.dwLen = nLen;
	cFrame.dwCmd = dwCmd; 
	if(m_dataPool.IsFull())
	{
		m_dataPool.Pop(NULL,0);
	}
	m_dataPool.Push((char*)&cFrame,n1,(char*)pData,nLen);
	m_eventWork.Set();
}

void CMMDataMaker::AddAudioData(BYTE * pData,int nLen)
{
	
	//return;

	if(m_pAudioCodec == NULL)
		return;
	BYTE * pCmpData;
	//pCmpData = m_pAudioCodec->CmpWaveData(pData,nLen);
	pCmpData = pData;
	if(pCmpData == NULL)
	{
		TRACE("压缩音频帧失败！\n");
		return ;
	}

	MMAudioFrame aFrame;
	aFrame.dwLen = nLen;
	int n1 = sizeof(aFrame);
	if(m_dataPool.IsFull())
	{
		//TRACE("warning:丢失音频帧，缓冲满\n");
		m_dataPool.Pop(NULL,0);
	}

	m_dataPool.Push((char*)&aFrame,n1,(char*)pCmpData,nLen);
	m_eventWork.Set();
	
}

void CMMDataMaker::AddVideoData(BYTE * pData,int nLen,int nTime)
{
	if(m_pVideoCodec == NULL)
		return;
	MMVideoFrame vFrame;
	vFrame.dwTime = nTime;
	BYTE * pCmpData;
	//pCmpData = m_pVideoCodec->CmpFrame(pData,nLen);
	pCmpData = pData;
	if(pCmpData == NULL)
	{
		TRACE("压缩视频帧失败!\n");
		return;
	}
	vFrame.dwLen = nLen;
	int n1 = sizeof(vFrame);
	if(m_dataPool.IsFull())
	{
		//TRACE("warning:丢失视频帧，缓冲满\n");
		m_dataPool.Pop(NULL,0);
	}
	m_dataPool.Push((char*)&vFrame,n1,(char*)pCmpData,nLen);
	m_eventWork.Set();
}

void CMMDataMaker::Clear()
{
	m_dataPool.Clear();
	m_eventQuit.Set();
	if(::WaitForSingleObject(m_hThreadWork,1000) == WAIT_TIMEOUT)
	{
		::TerminateThread(m_hThreadWork,0);
	}
	m_hThreadWork = NULL;
	m_bInit = false;
}

bool CMMDataMaker::IsInit()
{
	return m_bInit;
}



