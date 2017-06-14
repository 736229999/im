#include "StdAfx.h"
#include ".\mmvideoplayer.h"
#pragma comment(lib,"fcl.lib")

#ifdef _DEBUG
//#define _WRITE_FILE
#ifdef _WRITE_FILE
#include <fstream>
using namespace std;
#endif

#endif

const int PLAY_TIME_SPAN = 20;


CMMVideoPlayer::CMMVideoPlayer(void)
{
	m_bTimer = false;
	//m_dwLastFrameTime = 0;
	m_dwAccTime = 0;
	m_dwLastPlayTime = 0;
	m_pCallBack = NULL;
	m_dwUser = 0;
}

CMMVideoPlayer::~CMMVideoPlayer(void)
{
	if(m_bTimer)
		killTimer(1,this);
	/*MMVideoFrameList::iterator it,itE;
	it = m_lstFrame.begin();
	itE = m_lstFrame.end();
	m_lstFrame.m_mutex.do_lock();
	for(;it != itE;it++)
	{
		delete *it;
	}
	m_lstFrame.m_mutex.do_unlock();*/

}

void CMMVideoPlayer::onTimer(uint32 lEventID)
{
	DWORD dwTick = GetTickCount();
	long nTimeSpan = dwTick - m_dwLastTick;
	if(nTimeSpan < 0)
		nTimeSpan = -nTimeSpan;
	m_dwLastTick = dwTick;
	m_dwAccTime += nTimeSpan;
	m_lstFrame.m_mutex.do_lock();
	MMVideoFrameList::iterator it;
	DWORD dwTime;
	size_t nC = m_lstFrame.size();
	BYTE * pBitmap = NULL;
	bool bPlay = false;

	int nTime1 = GetTickCount();

	if(nC != 0)
	{
		it = m_lstFrame.begin();
		dwTime = ((MMVideoFrame*)(char*)(**it))->dwTime;
		//TRACE("span time = %d\n",dwTime);
		if(nC > 2)
		{
			// 如果缓冲中帧数据太多则加快播放速度
			TRACE("%d,加快视频播放\n",nC);
			dwTime *= 0.8;
		}
		// 当前帧离前一帧超过一定长时间，则立刻播放
		if(m_dwAccTime >= dwTime)
		{
			m_dwAccTime -= dwTime;
			if(m_dwAccTime > dwTime)
				m_dwAccTime = 0;
			bPlay = true;
		}

		if(bPlay)
		{
			// 解压视频帧
			MMVideoFrame * pFrame = (MMVideoFrame*)(char*)(**it);
#ifdef _WRITE_FILE

			static bool bFile = false;
			static ofstream ofs;
			if(bFile == false)
			{
				ofs.open("c:\\video2.dat",ios::binary);
				bFile = true;
			}
			ofs.write((char *)pFrame,pFrame->dwLen + sizeof(MMVideoFrame));
#endif
			pBitmap = m_pCodec->DeCmpFrame((BYTE*)pFrame->data,pFrame->dwLen);
			
			m_alloc.ReleaseObj(*it);
			m_lstFrame.pop_front();	
		}
	}
	m_lstFrame.m_mutex.do_unlock();
	// 播放
	if(pBitmap)
	{
		if(m_pCallBack)
			m_pCallBack(pBitmap,m_dwUser);
		TRACE("play time = %d\n",GetTickCount() - nTime1);
	}


	
}

BOOL CMMVideoPlayer::Init(CMMVideoCodec * pCodec)
{
	m_bTimer = setTimer(1,static_cast<ITimerSink*>(this),PLAY_TIME_SPAN);
	m_dwLastTick = GetTickCount();
	m_pCodec = pCodec;
	return TRUE;
}


void CMMVideoPlayer::AddVideoFrame(MMVideoFrame * pFrame,int nLen)
{
	XMemBlock * pF = m_alloc.AllocData();
	if(pF->GetSize() < nLen)
		pF->Resize(nLen);
	memcpy((char*)*pF,pFrame,nLen);
	m_lstFrame.m_mutex.do_lock();
	m_lstFrame.push_back(pF);
	m_lstFrame.m_mutex.do_unlock();
	
}

void CMMVideoPlayer::SetFramePerSec(int n)
{
	m_nFramePerSec = n;
	m_nTimeSpan = 1000 / m_nFramePerSec;
	if(m_nTimeSpan < PLAY_TIME_SPAN)
		m_nTimeSpan = PLAY_TIME_SPAN;
}


void CMMVideoPlayer::Stop()
{
	if(m_bTimer)
	{
		killTimer(1,this);
	}
	m_lstFrame.m_mutex.do_lock();
	MMVideoFrameList::iterator it,itE;
	it = m_lstFrame.begin();
	itE = m_lstFrame.end();
	for(;it != itE; it++)
	{
		m_alloc.ReleaseObj(*it);
	}
	m_lstFrame.m_mutex.do_unlock();
	m_lstFrame.clear();
}