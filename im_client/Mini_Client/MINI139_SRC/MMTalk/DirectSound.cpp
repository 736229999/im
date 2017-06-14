#include "StdAfx.h"
#include ".\directsound.h"

#ifdef _DEBUG
#include <fstream>
using namespace std;
#endif

#pragma comment(lib,"dxguid.lib")

const int DSBVOLUME_BASE = DSBVOLUME_MAX - (DSBVOLUME_MAX - DSBVOLUME_MIN) / 4;

static void DSError( HRESULT hRes ) {
	switch(hRes) {
		case DS_OK: TRACE0("NO ERROR\n"); break;
		case DSERR_ALLOCATED: TRACE0("ALLOCATED\n"); break;
		case DSERR_INVALIDPARAM: TRACE0("INVALIDPARAM\n"); break;
		case DSERR_OUTOFMEMORY: TRACE0("OUTOFMEMORY\n"); break;
		case DSERR_UNSUPPORTED: TRACE0("UNSUPPORTED\n"); break;
		case DSERR_NOAGGREGATION: TRACE0("NOAGGREGATION\n"); break;
		case DSERR_UNINITIALIZED: TRACE0("UNINITIALIZED\n"); break;
		case DSERR_BADFORMAT: TRACE0("BADFORMAT\n"); break;
		case DSERR_ALREADYINITIALIZED: TRACE0("ALREADYINITIALIZED\n"); break;
		case DSERR_BUFFERLOST: TRACE0("BUFFERLOST\n"); break;
		case DSERR_CONTROLUNAVAIL: TRACE0("CONTROLUNAVAIL\n"); break;
		case DSERR_GENERIC: TRACE0("GENERIC\n"); break;
		case DSERR_INVALIDCALL: TRACE0("INVALIDCALL\n"); break;
		case DSERR_OTHERAPPHASPRIO: TRACE0("OTHERAPPHASPRIO\n"); break;
		case DSERR_PRIOLEVELNEEDED: TRACE0("PRIOLEVELNEEDED\n"); break;
		default: TRACE1("%lu\n",hRes);break;
	}
}




DWORD WINAPI CDirectSound::PlayThread(LPVOID lpParam)
{
	CDirectSound * pMe = (CDirectSound *)lpParam;
	pMe->PlayNotify();
	return 0;
}




CDirectSound::CDirectSound(void)
{
	m_pDirectSound = NULL;
	m_pDSB = NULL;
	m_pPosNotify = NULL;
	//m_phNotifEvents = NULL;
	m_hThreadPlay = NULL;
	//m_hCanPush = ::CreateEvent(0,0,0,0);
	m_hNotifyEvent = NULL;
	m_hStopEvent = NULL;
	m_nCurNotify = 0;
	m_bWork = false;

}

CDirectSound::~CDirectSound(void)
{
	m_bWork = false;
	if(WaitForSingleObject(m_hThreadPlay,100) == WAIT_TIMEOUT)
	{
		TerminateThread(m_hThreadPlay,-1);
		m_hThreadPlay = NULL;
	}

	if( m_pDSB )
	{
		m_pDSB->Release();
		m_pDSB = NULL;
	}

	if(m_pDirectSound)
	{
		m_pDirectSound->Release();
	}
	if(m_hStopEvent)
	{
		::CloseHandle(m_hStopEvent);
	}
	if(m_hNotifyEvent)
	{
		::CloseHandle(m_hNotifyEvent);
	}
	/*int i;
	if(m_phNotifEvents)
	{
		int nC = AUDIO_BUF_COUNT + 1;
		for(i = 0;i < nC;i++)
		{
			if(m_phNotifEvents[i])
				::CloseHandle(m_phNotifEvents[i]);
		}
		delete [] m_phNotifEvents;
		m_phNotifEvents = NULL;
	}*/

	if(m_pPosNotify)
	{
		delete [] m_pPosNotify;
		m_pPosNotify = NULL;
	}

	
}


void CDirectSound::Stop()
{
	m_nCurNotify = 0;
	if(m_pDSB)
		m_pDSB->Stop();
}

void CDirectSound::SetVolume(int nPercent)
{
	HRESULT hr;
	int nVolume;
	if (nPercent <= 0)
		nVolume = DSBVOLUME_MIN;
	else if (nPercent >= 100)
		nVolume = DSBVOLUME_MAX;
	else
		nVolume = DSBVOLUME_BASE + (DSBVOLUME_MAX - DSBVOLUME_BASE) * nPercent / 100;
	if(m_pDSB)
	{
		hr = m_pDSB->SetVolume(nVolume);
	}
}

int CDirectSound::GetVolume()
{
	long nVolume = 0;
	if(m_pDSB)
	{
		m_pDSB->GetVolume(&nVolume);
		nVolume = (nVolume - DSBVOLUME_BASE) * 100 / (DSBVOLUME_MAX - DSBVOLUME_BASE);
		if(nVolume < 0)
			nVolume = 0;
		else if(nVolume > 100)
			nVolume = 100;
	}
	return nVolume;
}

void CDirectSound::Play(const BYTE * pData,int nLen)
{
	m_audioBuf.PushData(pData,nLen);

	//int nPush;
	//while(nLen > 0)
	//{
	//	nPush = m_audioBuf.PushData(pData,nLen);
	//	if(m_audioBuf.m_nBuffing > 0)
	//		m_audioBuf.m_nBuffing--;
	//	if(nPush < 0)
	//		break;
	//	else if(nPush == 0)
	//	{
	//		break;
	//		// 等待缓冲可以用
	//		//::WaitForSingleObject(m_hCanPush,1000);
	//	}
	//	else
	//	{
	//		nLen -= nPush;
	//		pData += nPush;
	//	}
	//};
	//

}

BOOL CDirectSound::Init(HWND hWnd, WAVEFORMATEX * pWfx, int nBufNotifySize /* = AUDIO_NOTIFY_SIZE  */, int nExtBufSize /* = AUDIO_EXTBUF_SIZE  */)
{

	if(pWfx == NULL)
		return FALSE;
	m_wfx = *pWfx;
	if(m_hThreadPlay != NULL)
	{
		//// 关闭线程
		//if(m_phNotifEvents)
		//{
		//	::SetEvent(m_phNotifEvents[AUDIO_BUF_COUNT]);
		//	WaitForSingleObject( m_hThreadPlay, 5000 );
		//	CloseHandle( m_hThreadPlay );
		//	m_hThreadPlay = NULL;
		//}
		if(m_hStopEvent)
		{
			::SetEvent(m_hStopEvent);
			WaitForSingleObject( m_hThreadPlay, 5000 );
			CloseHandle( m_hThreadPlay );
			m_hThreadPlay = NULL;
		}
	}
	
	if( m_pDSB )
	{
		m_pDSB->Release();
		m_pDSB = NULL;
	}
	if(m_pDirectSound)
	{
		m_pDirectSound->Release();
		m_pDirectSound = NULL;
	}

	HRESULT hRes = DS_OK;
	if( m_pDirectSound == NULL ) 
	{
		int nRes = 0;
		do {
			if( nRes )
				::Sleep(500);
			hRes = ::DirectSoundCreate(0, &m_pDirectSound, 0);
			++nRes;
		} while( nRes < 5 && (hRes == DSERR_ALLOCATED || hRes == DSERR_NODRIVER) );

		if( hRes != DS_OK )
			return FALSE;
		hRes = m_pDirectSound->SetCooperativeLevel(hWnd,DSSCL_PRIORITY);//DSSCL_WRITEPRIMARY  DSSCL_PRIORITY
		if(hRes != DS_OK)
			return FALSE;
	}

	

	HRESULT hr = 0;
	DSBUFFERDESC dsbd;
	ZeroMemory( &dsbd, sizeof(DSBUFFERDESC) );
	dsbd.dwSize = sizeof(DSBUFFERDESC);
	dsbd.dwFlags = DSBCAPS_GLOBALFOCUS;
	dsbd.dwFlags |= 
		//DSBCAPS_CTRLPAN | 
		DSBCAPS_CTRLVOLUME |
		//DSBCAPS_CTRLFREQUENCY|
		DSBCAPS_CTRLPOSITIONNOTIFY|
		DSBCAPS_GETCURRENTPOSITION2;
	
	dsbd.dwBufferBytes = nBufNotifySize * AUDIO_BUF_COUNT; 
	dsbd.lpwfxFormat = pWfx;
	dsbd.guid3DAlgorithm = GUID_NULL;

	if(FAILED(hr = m_pDirectSound->CreateSoundBuffer(&dsbd,&m_pDSB,NULL)))
		return FALSE;

	int i;

	//if(m_phNotifEvents)
	//{
	//	for(i = 0;i < AUDIO_BUF_COUNT + 1;i++)
	//	{
	//		if(m_phNotifEvents[i])
	//			::CloseHandle(m_phNotifEvents[i]);
	//	}
	//	delete [] m_phNotifEvents;
	//	m_phNotifEvents = NULL;
	//}
	//m_phNotifEvents = new HANDLE[AUDIO_BUF_COUNT + 1];
	//for(i = 0;i < AUDIO_BUF_COUNT + 1;i++)
	//{
	//	m_phNotifEvents[i] = ::CreateEvent(0,FALSE,FALSE,0);
	//}
	if(m_hNotifyEvent == NULL)
		m_hNotifyEvent = ::CreateEvent(0,FALSE,FALSE,0);
	if(m_hStopEvent == NULL)
		m_hStopEvent = ::CreateEvent(0,FALSE,FALSE,0);
	if(m_pPosNotify)
	{
		delete [] m_pPosNotify;
		m_pPosNotify = NULL;
	}
	m_pPosNotify = new DSBPOSITIONNOTIFY[AUDIO_BUF_COUNT];
	// 设置DirectSound通知
	for(i = 0; i < AUDIO_BUF_COUNT;i++)
	{
		m_pPosNotify[i].dwOffset = i * nBufNotifySize ;
		m_pPosNotify[i].hEventNotify = m_hNotifyEvent;
		//m_pPosNotify[i].hEventNotify = m_phNotifEvents[i];
	}

	LPDIRECTSOUNDNOTIFY pDSNotify = NULL;
	if(FAILED(hr = m_pDSB->QueryInterface(IID_IDirectSoundNotify,(LPVOID*)&pDSNotify )))
		return FALSE;
	hRes = pDSNotify->SetNotificationPositions(AUDIO_BUF_COUNT,m_pPosNotify);
	if(hRes)
		return FALSE;
	pDSNotify->Release();

	// 音频缓冲初始化
	BYTE bFill = pWfx->wBitsPerSample == 8 ? 128 : 0;
	m_audioBuf.Init(m_pDSB,nExtBufSize,bFill,nBufNotifySize,AUDIO_BUF_COUNT);

	m_bWork = true;
	m_hThreadPlay = CreateThread(0,0,PlayThread,this,NULL,0);

	

	//VOID *  pLockedBuf1 = NULL;
	//VOID *  pLockedBuf2 = NULL;
	//DWORD   dwLen1 = 0;
	//DWORD   dwLen2 = 0;
	//hr = m_pDSB->Lock(0,dsbd.dwBufferBytes,&pLockedBuf1,&dwLen1,&pLockedBuf2,&dwLen2,0);
	//if(hr)
	//{
	//	TRACE("lock buf failed\n");
	//	return FALSE;
	//}
	//// 填充静音
	//FillMemory((char *)pLockedBuf1,dwLen1,bFill);
	//hr = m_pDSB->Unlock(pLockedBuf1,dwLen1,pLockedBuf2,dwLen2);
	m_pDSB->SetCurrentPosition(0);
	hRes = m_pDSB->Play(0,0,DSBPLAY_LOOPING);
	return TRUE;
}



void CDirectSound::PlayNotify()
{
	DWORD dwRst;
	HANDLE events[2];
	events[0] = m_hNotifyEvent;
	events[1] = m_hStopEvent;
	while(m_bWork)
	{
		dwRst = ::WaitForMultipleObjects(2,events,FALSE,INFINITE);
		if(dwRst == WAIT_OBJECT_0 )
		{
			m_audioBuf.OnBufNotify(m_nCurNotify);
			m_nCurNotify++;
			m_nCurNotify %= CDirectSound::AUDIO_BUF_COUNT;
		}
		else
		{
			break;
		}
	}
	TRACE("音频播放控制中止\n");
}

int CAudioBuf::PushData(const BYTE * pData,int nLen)
{
	if(m_pDSB == NULL)
		return -1;
	if(pData == NULL)
		return 0;

	/*static bool b = false;
	if(b == false)
	{
		m_nBuffingC = 0;
		m_nNeedBuff = 0;
		b = true;
		
		char * pData = new char[1024*100];
		ifstream ifs;
		ifs.open("t.dat",ios::binary);
		ifs.read(pData,1024*100);
		m_dataPool.Push(pData,1024*100);
		ifs.close();
		delete [] pData;
		return 0;
	}
	else
	{
		return 0;
	}*/

	int nSpace = m_dataPool.GetSpace();
	if(nSpace < nLen)
	{
		TRACE("播放外部缓冲不够用，数据丢失！\n");
		// 丢弃旧数据腾出空间
		m_dataPool.Pop(NULL,nLen - nSpace);
	}
	int nRst = m_dataPool.Push((char*)pData,nLen);

	//int nSpace = m_dataPool.GetSpace();
	//if(nSpace < nLen)
	//{
	//	TRACE("播放外部缓冲不够用，数据丢失！\n");
	//	// 丢弃旧数据腾出空间
	//	m_dataPool.Pop(NULL,nLen - nSpace);
	//}
	//int nRst = m_dataPool.Push((char*)pData,nLen);
	//if(m_nNeedBuff > 0)
	//{
	//	m_nNeedBuff -= nRst;
	//	if(m_nNeedBuff < 0)
	//		m_nNeedBuff = 0;
	//}

	return nRst;
}


void CAudioBuf::RestoreBuff()
{
	HRESULT hr;
	DWORD dwStatus;
	if( FAILED( hr = m_pDSB->GetStatus( &dwStatus ) ) )
		return ;
	if( dwStatus & DSBSTATUS_BUFFERLOST )
	{
		TRACE("buffer lost!!!!!!!!!\n");
		do
		{
			hr = m_pDSB->Restore();
			if( hr == DSERR_BUFFERLOST )
				Sleep( 10 );
		}
		while( ( hr = m_pDSB->Restore() ) == DSERR_BUFFERLOST );
	}
}

void CAudioBuf::WriteBuf()
{
	//HRESULT hr;
	VOID *  pLockedBuf1 = NULL;
	VOID *  pLockedBuf2 = NULL;
	DWORD   dwLen1 = 0;
	DWORD   dwLen2 = 0;
	DWORD	dwPlay = 0;
	DWORD	dwWrite = 0;

	// 察看当前播放状态
	m_pDSB->GetCurrentPosition(&dwPlay,&dwWrite);
	

	
	////
	//// 将外部缓冲里的数据写入内部缓冲
	////

	//int nTask = m_dataPool.GetDataSize();
	//int nSpace = 0;
	//int nStart;
	//int nCanWrite = 0;	
	//if(nTask != 0)
	//{
	//	// 如果内部缓冲还有数据就将数据写入其后
	//	if(m_bHasData)
	//	{
	//		if(m_bCarry)
	//		{
	//			if(dwWrite > dwPlay)
	//			{
	//				// end为数据结束位置，p为当前播放位置，w为当前写入位置
	//				//        .end     .p .w
	//				//  [  |  |  |  |  |  |  |  |  |  |  ]
	//				// 最常见
	//				nSpace = dwPlay - m_nDataEndPos;
	//				nStart = m_nDataEndPos;
	//			}
	//			else
	//			{
	//				if(dwWrite > m_nDataEndPos)
	//				{
	//					//     .end  .w                   .p
	//					//  [  |  |  |  |  |  |  |  |  |  |  ]
	//					nSpace = dwPlay - dwWrite;
	//					nStart = dwWrite;
	//				}
	//				else
	//				{
	//					//     .w       .end              .p
	//					//  [  |  |  |  |  |  |  |  |  |  |  ]
	//					nSpace = dwPlay - m_nDataEndPos;
	//					nStart = m_nDataEndPos;
	//				}
	//			}
	//		}
	//		else
	//		{
	//			if(dwWrite > m_nDataEndPos)
	//			{
	//				if(dwWrite > dwPlay)
	//				{
	//					//           .p .end  .w       
	//					//  [  |  |  |  |  |  |  |  |  |  |  ]
	//					nSpace = dwPlay + m_nIntSize - dwWrite;
	//					nStart = dwWrite;
	//					
	//				}
	//				else
	//				{
	//					
	//					//     .end  .w                   .p
	//					//  [  |  |  |  |  |  |  |  |  |  |  ]
	//					// 缓冲设置得太小才会出现这种情况
	//					nSpace = dwPlay - dwWrite;
	//					nStart = dwWrite;
	//				}
	//			}
	//			else
	//			{
	//				if(dwWrite > dwPlay)
	//				{
	//					//           .p .w       .end
	//					//  [  |  |  |  |  |  |  |  |  |  |  ]
	//					// 最常见
	//					nSpace = dwPlay + m_nIntSize - m_nDataEndPos;
	//					nStart = m_nDataEndPos;
	//				}
	//				else
	//				{
	//					//     .w                   .end  .p 
	//					//  [  |  |  |  |  |  |  |  |  |  |  ]
	//					// 这个时候内部缓冲区其实已经播放完
	//					nSpace = dwPlay  - dwWrite;
	//					nStart = dwWrite;
	//				}			
	//			}
	//		}

	//	}
	//	// 内部缓冲已经没有数据，直接将数据写入缓冲内
	//	else
	//	{
	//		m_bHasData = true;
	//		if(dwWrite < dwPlay)
	//		{
	//			nSpace = dwPlay - dwWrite;
	//		}
	//		else
	//		{
	//			nSpace = dwPlay + m_nIntSize - dwWrite;
	//		}
	//		
	//		nCanWrite = nTask > nSpace ? nSpace : nTask;
	//		
	//	}

	//	if(nCanWrite == 0)
	//	{
	//		// 内部缓冲已满
	//		TRACE("内部缓冲已满\n");
	//		return;
	//	}
	//	nStart = dwWrite;
	//	hr = m_pDSB->Lock(nStart,nCanWrite,&pLockedBuf1,&dwLen1,&pLockedBuf2,&dwLen2,0);
	//	if(hr)
	//	{
	//		TRACE("lock buf failed,nStart = %d,lockSize= %d\n",nStart,nCanWrite);
	//		return;
	//	}
	//	int nRt1 = m_dataPool.Pop((char *)pLockedBuf1,dwLen1);
	//	int nRt2 = m_dataPool.Pop((char *)pLockedBuf2,dwLen2);
	//	hr = m_pDSB->Unlock(pLockedBuf1,dwLen1,pLockedBuf2,dwLen2);
	//	m_nDataEndPos = nStart + nCanWrite;
	//	m_nDataEndPos %= m_nIntSize;
	//	if(m_nDataEndPos < dwPlay)
	//	{
	//		m_bCarry = true;	// 缓冲循环则进位
	//	}
	//}
	
}

void CAudioBuf::OnBufNotify(int nPos)
{
	//TRACE("notify %d\n",nPos);

	int nTick = GetTickCount();
	if(m_pDSB == NULL)
		return;
	static bool bStop = false;

	HRESULT hr;
	VOID *  pLockedBuf1 = NULL;
	VOID *  pLockedBuf2 = NULL;
	DWORD   dwLen1 = 0;
	DWORD   dwLen2 = 0;
	DWORD	dwPlay = 0;
	DWORD	dwWrite = 0;

	m_pDSB->GetCurrentPosition(&dwPlay,&dwWrite);

	// 通知到当前已经播放的位置
	//int nCurPos = (nPos) % m_nNotifyCount;
	//nCurPos *= m_nNotifySize;
	//nCurPos += 1024*8;
	int nCurPos = nPos * m_nNotifySize;
	nCurPos += 200;
	nCurPos = nCurPos % m_nIntSize;

	// 当前外部缓冲中是否有数据
	int nSize = m_dataPool.GetDataSize();

	int nInter = dwWrite >= dwPlay ? dwWrite - dwPlay : dwWrite + m_nIntSize - dwPlay;
	TRACE("cur play = %d,cur write=%d,write = %d,inter = %d\n",dwPlay,dwWrite,nCurPos,nInter);
	
	// 
	// 然后将数据写入播放缓冲
	// 
	hr = m_pDSB->Lock(nCurPos,m_nNotifySize,&pLockedBuf1,&dwLen1,&pLockedBuf2,&dwLen2,0);
	if(hr)
	{
		TRACE("lock 失败！\n");
		return;
	}

	if(nSize < m_nNotifySize)
	{
		//TRACE("数据不够播放，需要缓冲----\n");
		memset(pLockedBuf1,m_bFill,dwLen1);
		if(dwLen2 != 0)
		{
			memset(pLockedBuf2,m_bFill,dwLen2);
		}
	}
	else
	{


		int nPlayed = 0;
		if(pLockedBuf1)
			nPlayed = m_dataPool.Pop((char *)pLockedBuf1,dwLen1);
		if(pLockedBuf2)
		{
			TRACE("buf 2 :%d\n",dwLen2);
			//memset(pLockedBuf2,m_bFill,dwLen2);
			nPlayed = m_dataPool.Pop((char *)pLockedBuf2,dwLen2);
		}
	}

	hr = m_pDSB->Unlock(pLockedBuf1,dwLen1,pLockedBuf2,dwLen2);
}

//void CAudioBuf::OnBufNotify(int nPos)
//{
//
//	if(m_pDSB == NULL)
//		return;
//	static bool bStop = false;
//
//	HRESULT hr;
//	VOID *  pLockedBuf1 = NULL;
//	VOID *  pLockedBuf2 = NULL;
//	DWORD   dwLen1 = 0;
//	DWORD   dwLen2 = 0;
//	DWORD	dwPlay = 0;
//	DWORD	dwWrite = 0;
//
//
//	// 通知到当前已经播放的位置
//	int nCurPos = nPos * m_nNotifySize;
//	// 当前外部缓冲中是否有数据
//	int nSize = m_dataPool.GetDataSize();
//
//	//
//	// 首先清理播放缓冲
//	//
//
//	int nLastPos = (nPos + m_nNotifyCount - 1) % m_nNotifyCount;
//	nLastPos *= m_nNotifySize;
//
//	hr = m_pDSB->Lock(nLastPos,m_nNotifySize,&pLockedBuf1,&dwLen1,&pLockedBuf2,&dwLen2,0);
//	if(hr)
//	{
//		TRACE("锁定directSound缓冲失败\n");
//		return;
//	}
//	memset(pLockedBuf1,m_bFill,dwLen1);
//	if(pLockedBuf2)
//		memset(pLockedBuf2,m_bFill,dwLen2);
//	hr = m_pDSB->Unlock(pLockedBuf1,dwLen1,pLockedBuf2,dwLen2);
//
//
//	// 
//	// 然后将数据写入播放缓冲
//	// 
//
//	if(nSize < m_nNotifySize)
//	{
//		//TRACE("数据不够播放，需要缓冲----\n");
//		
//	}
//	else
//	{
//		//TRACE("%d还没有播放\n",nSize);
//	}
//	hr = m_pDSB->Lock(nCurPos,m_nNotifySize,&pLockedBuf1,&dwLen1,&pLockedBuf2,&dwLen2,0);
//	if(hr)
//	{
//		return;
//	}
//	if(pLockedBuf1)
//		m_dataPool.Pop((char *)pLockedBuf1,dwLen1);
//	if(pLockedBuf2)
//		m_dataPool.Pop((char *)pLockedBuf2,dwLen2);
//	hr = m_pDSB->Unlock(pLockedBuf1,dwLen1,pLockedBuf2,dwLen2);
//}

//void CAudioBuf::OnBufNotify(int nPos)
//{
//	
//	if(m_pDSB == NULL)
//		return;
//	static bool bStop = false;
//
//	HRESULT hr;
//	VOID *  pLockedBuf1 = NULL;
//	VOID *  pLockedBuf2 = NULL;
//	DWORD   dwLen1 = 0;
//	DWORD   dwLen2 = 0;
//	DWORD	dwPlay = 0;
//	DWORD	dwWrite = 0;
//
//	
//	// 通知到当前已经播放的位置
//	int nCurNotifyPos = nPos * m_nNotifySize;
//	
//	hr = m_pDSB->Lock(m_nLastNotify,m_nNotifySize,&pLockedBuf1,&dwLen1,&pLockedBuf2,&dwLen2,0);
//	if(hr)
//	{
//		TRACE("锁定directSound缓冲失败\n");
//		return;
//	}
//	memset(pLockedBuf1,m_bFill,dwLen1);
//	if(pLockedBuf2)
//		memset(pLockedBuf2,m_bFill,dwLen2);
//	hr = m_pDSB->Unlock(pLockedBuf1,dwLen1,pLockedBuf2,dwLen2);
//
//	
//
//	int nLeft;
//	// 如果当前缓冲还有数据
//	if(m_nLeft > 0)
//	{
//		//            .last       .cur   
//		//  [         |           |          |          ]
//		if(nCurNotifyPos > m_nLastNotify)
//		{
//			nLeft = m_nLastNotify + m_nLeft - nCurNotifyPos;
//			if(nLeft < 0)
//				m_nLeft = 0; // 缓冲已经播放完毕
//			else
//				m_nLeft = nLeft;
//		}
//		//            .cur                              .last
//		//  [         |           |          |          ]
//		else
//		{
//			nLeft = m_nLastNotify + m_nLeft - (nCurNotifyPos + m_nIntSize);
//			if(nLeft < 0)
//				m_nLeft = 0; // 缓冲已经播放完毕
//			else
//				m_nLeft = nLeft;
//		}
//	}
//
//	
//	
//	
//	m_nLastNotify = nCurNotifyPos;
//
//	//
//	// 将外部缓冲数据写入内部缓冲,长度不超过2倍通知大小
//	//
//
//	int nTotal = m_dataPool.GetDataSize();
//	int nCanFill = m_nNotifySize * 4;;	// 能写入到内部缓冲的大小
//	int nWrite = 0;	// 实际写入的字节数
//	int nStart = 0;	// 相对当前通知位置的开始写入数据的位置
//	int nWC;		// dwWrite和当前通知位置的距离
//	if(nTotal <= 0)
//	{
//		if(m_nNeedBuff <= 0)
//		{
//			m_nNeedBuff = m_nBuffingC;
//			TRACE("need buffing %d...\n",m_nBuffingC);
//		}
//		return;
//	}
//	else
//	{
//		if(m_nNeedBuff > 0)
//		{
//			return ;
//		}
//	}
//
//	TRACE("intleft = %d,extleft = %d\n",m_nLeft,nTotal);
//
//	// 察看当前播放状态
//	m_pDSB->GetCurrentPosition(&dwPlay,&dwWrite);
//	TRACE("cur = %d,play=%d,write=%d\n",nCurNotifyPos,dwPlay,dwWrite);
//
//	if(dwWrite >= nCurNotifyPos)
//		nWC = dwWrite - nCurNotifyPos;
//	else
//		nWC = dwWrite + m_nIntSize - nCurNotifyPos;
//	// 距离远一点
//	//nWC += m_nNotifySize * 3;
//	if(nWC > m_nNotifySize)
//	{
//		//m_nNeedBuff = m_nBuffingC;
//		//TRACE("nWC %d> 通知大小,write = %d,cur= %d\n",nWC,dwWrite,nCurNotifyPos);
//	}
//#ifdef _DEBUG
//	
//	if(nWC > m_nLeft)
//	{
//		TRACE("可能有破音,nWC=%d,leftData=%d\n",nWC,m_nLeft);
//	}
//#endif
//
//	// 开始写入数据的位置
//	//nStart = nWC > m_nLeft ? nWC : m_nLeft;
//	nStart = m_nLeft;
//	// 计算可以写入的数据大小
//	nWrite = nCanFill - nStart;
//	nWrite = nTotal > nWrite ? nWrite : nTotal;
//	if(nWrite < 0)
//	{
//		nWrite = 0;
//		return ;
//	}
//	// 更新当前缓冲内数据的长度
//	m_nLeft = nStart + nWrite;
//
//	//            .cur  .write.data                       
//	//  [         |           |          |          ]
//	nStart = (nCurNotifyPos + nStart) % m_nIntSize;
//	
//	RestoreBuff();
//
//	/*static   bool bDebug = false;
//
//	if(bDebug == false)
//	{
//		TRACE("first audio play time = %d\n",GetTickCount());
//		bDebug = true;
//	}*/
//
//	TRACE("write pos=%d len= %d ",nStart,nWrite);
//	hr = m_pDSB->Lock(nStart,nWrite,&pLockedBuf1,&dwLen1,&pLockedBuf2,&dwLen2,0);
//	if(hr)
//	{
//		TRACE("lock buf failed,nStart = %d,lockSize= %d\n",nStart,nWrite);
//		return;
//	}
//	int nRt1 = m_dataPool.Pop((char *)pLockedBuf1,dwLen1);
//	int nRt2 = m_dataPool.Pop((char *)pLockedBuf2,dwLen2);
//	TRACE("pop1= %d,pop2=%d\n",dwLen1,dwLen2);
//	hr = m_pDSB->Unlock(pLockedBuf1,dwLen1,pLockedBuf2,dwLen2);
//	if(hr)
//	{
//		TRACE("unlock failed\n");
//	}
//
//}

//void CAudioBuf::OnBufNotify2(int nIdx)
//{
//	
//	if(m_pDSB == NULL )
//		return;
//
//#ifdef _DEBUG
//	//TRACE("play %d ok \n",nIdx);
//	
//#endif
//
//	HRESULT hr;
//	int		nPos;
//	int		nLen;
//	VOID *  pLockedBuf1 = NULL;
//	VOID *  pLockedBuf2 = NULL;
//	DWORD   dwLen1 = 0;
//	DWORD   dwLen2 = 0;
//	
//
//	//nIdx = (nIdx + 1 ) % AUDIO_BUF_COUNT;
//	
//	nPos = m_nBlockLen * nIdx;
//	
//	DWORD dwPlay = 0;
//	DWORD dwWrite = 0;
//	
//
//	m_pDSB->GetCurrentPosition(&dwPlay,&dwWrite);
//
//	hr = m_pDSB->Lock(nPos,m_nBlockLen,&pLockedBuf1,&dwLen1,&pLockedBuf2,&dwLen2,0);
//	
//	TRACE("notify=%d, play = %d,write = %d\n",nPos,dwPlay,dwWrite);
//	
//	if(hr)
//	{
//		TRACE("lock buf failed\n");
//		return;
//	}
//
//	// 当缓冲区中没有数据的时候，设置开始缓冲
//	if(m_dataPool.GetSize() == 0 )
//	{
//		// 清除已经播放的数据
//		if(pLockedBuf2)
//			FillMemory(pLockedBuf2,dwLen2,m_bFill);
//		FillMemory(pLockedBuf1,dwLen1,m_bFill);
//		
//		// 如果当前没有播放的数据,也没有缓冲数据，则设置一个播放缓冲
//		if(m_nBuffing <= 0)
//			m_nBuffing = m_nBuffingC;
//	}
//	// 当前缓冲还没有满，则不播放，继续缓冲
//	else if(m_nBuffing > 0)
//	{
//		// 清除已经播放的数据
//		if(pLockedBuf2)
//			FillMemory(pLockedBuf2,dwLen2,m_bFill);
//		FillMemory(pLockedBuf1,dwLen1,m_bFill);
//	}
//	else
//	{
//		// 填充数据
//		if(dwLen1 < (DWORD)m_nBlockLen)
//		{
//			int nLen1 = dwLen1;
//			int nLen2 = dwLen2;
//			// 填充数据
//			m_dataPool.Pop((char *)pLockedBuf1,nLen1,(char *)pLockedBuf2,nLen2);
//			// 填充静音
//			if((DWORD)nLen1 < dwLen1)
//			{
//				FillMemory(((char *)pLockedBuf1) + nLen1,dwLen1 - nLen1,m_bFill);
//				FillMemory((char *)pLockedBuf2,dwLen2,m_bFill);
//			}
//			else if((DWORD)nLen2 < dwLen2)
//			{
//				FillMemory(((char *)pLockedBuf2) + nLen2,dwLen2 - nLen2,m_bFill);
//			}	
//		}
//		else
//		{
//			// 填充数据
//			nLen = m_dataPool.Pop((char *)pLockedBuf1,m_nBlockLen);
//			if(nLen < m_nBlockLen)
//			{
//				// 填充静音
//				FillMemory(((char *)pLockedBuf1) + nLen,m_nBlockLen - nLen,m_bFill);
//			}
//		}
//		
//	}
//	hr = m_pDSB->Unlock(pLockedBuf1,dwLen1,pLockedBuf2,dwLen2);
//
//	m_pDSB->GetCurrentPosition(&dwPlay,&dwWrite);
//	TRACE("notify=%d, play = %d,write = %d\n",nPos,dwPlay,dwWrite);
//
//	if(hr)
//	{
//		TRACE("err!!!unlock err!!\n");
//	}
//}
//
//
//
//

DWORD WINAPI CMMAudioCap::ThreadRecord(LPVOID lpParam) 
{
	CMMAudioCap * pMe = (CMMAudioCap *)lpParam;
	pMe->ProcRecord();
	return 0;
}

DWORD WINAPI CMMAudioCap::ThreadWork(LPVOID lpParam)
{
	CMMAudioCap * pMe = (CMMAudioCap *)lpParam;
	pMe->ProcWork();
	return 0;
}

CMMAudioCap::CMMAudioCap()
{
	m_pDSC = NULL;
	m_pDSBCapture = NULL;
	m_hNotificationEvent = NULL;
	m_hQuitRecord = NULL;
	m_hQuitWork = NULL;
	m_dwNextCaptureOffset = 0;
	m_dwCaptureBufferSize = 0;
	m_dwNotifySize = 0;
	m_hThreadNotify = NULL;
	m_hThreadWork = NULL;
	m_dwUser = 0;
	m_pCallback = NULL;
	m_hSemSignal = NULL;
	m_bSilence = 0;
	m_dwUserNotify = 0;
	m_pNotifyFnc = NULL;
	m_nNotifyTime = 1000;
	m_bTimer = false;
	m_nNotify = 0;

	m_uMixID = 0;
	m_dwMax = 65535;
	m_dwMin = 0;
	
}


CMMAudioCap::~CMMAudioCap()
{

	if(m_bTimer)
	{
		killTimer(10,this);
	}

	if(m_hThreadNotify != NULL)
	{
		// 关闭线程
		::SetEvent(m_hQuitRecord);
		WaitForSingleObject( m_hThreadNotify, 5000 );
		CloseHandle( m_hThreadNotify );
		m_hThreadNotify = NULL;
	}
	
	if(m_pDSBCapture)
	{
		m_pDSBCapture->Release();
		m_pDSBCapture = NULL;
	}

	if( m_pDSC )
	{
		m_pDSC->Release();
		m_pDSC = NULL;
	}
	
	if(m_hNotificationEvent)
	{
		::CloseHandle(m_hNotificationEvent);
		m_hNotificationEvent = NULL;
	}
	if(m_hQuitRecord)
	{
		::CloseHandle(m_hQuitRecord);
		m_hQuitRecord = NULL;
	}
	if(m_hQuitWork)
	{
		::CloseHandle(m_hQuitWork);
		m_hQuitWork = NULL;
	}
	if(m_hSemSignal)
	{
		::CloseHandle(m_hSemSignal);
		m_hSemSignal = NULL;
	}
}




BOOL CMMAudioCap::Init(WAVEFORMATEX * pWfx,AudioCapDataFnc pFnc,DWORD dwUser)
{
	m_dwNextCaptureOffset = 0;
	if(pWfx)
	{
		m_bSilence = pWfx->wBitsPerSample == 8 ? 128 : 0;
	}
	
	if(m_hThreadNotify != NULL)
	{
		// 关闭线程
		::SetEvent(m_hQuitRecord);
		WaitForSingleObject( m_hThreadNotify, 5000 );
		CloseHandle( m_hThreadNotify);
		m_hThreadNotify = NULL;
	}
	if(m_hThreadWork)
	{
		::SetEvent(m_hQuitWork);
		WaitForSingleObject( m_hThreadWork, 5000 );
		CloseHandle( m_hThreadWork);
		m_hThreadWork = NULL;

	}

	if(m_hSemSignal)
	{
		::CloseHandle(m_hSemSignal);
		m_hSemSignal = NULL;
	}
	// 创建信号量
	m_hSemSignal = CreateSemaphore(NULL,0,NUM_CAP_BUF,NULL);
	m_audioData.Init(NUM_CAP_BUF,SIZE_REC_BLOCK);
	if(m_pDSBCapture)
	{
		m_pDSBCapture->Release();
		m_pDSBCapture = NULL;
	}
	if(m_pDSC)
	{
		m_pDSC->Release();
		m_pDSC = NULL;
	}
	if(m_hNotificationEvent)
	{
		::CloseHandle(m_hNotificationEvent);
		m_hNotificationEvent = NULL;
	}
	if(m_hQuitRecord)
	{
		::CloseHandle(m_hQuitRecord);
		m_hQuitRecord = NULL;
	}
	if(m_hQuitWork)
	{
		::CloseHandle(m_hQuitWork);
		m_hQuitWork = NULL;
	}

	HRESULT hr ;
	hr = DirectSoundCaptureCreate(NULL,&m_pDSC,NULL);
	if(hr)
		return FALSE;
	
	LPDIRECTSOUNDCAPTUREBUFFER pDSCB = NULL;
	DSCBUFFERDESC dscbd;
	dscbd.dwSize = sizeof(DSCBUFFERDESC);
	dscbd.dwFlags = 0;
	dscbd.dwBufferBytes = SIZE_REC_BLOCK * NUM_REC_NOTIFICATIONS;
	dscbd.dwReserved = 0;
	dscbd.lpwfxFormat = pWfx;	// 设置录音用的wave格式
	dscbd.dwFXCount = 0;
	dscbd.lpDSCFXDesc = NULL;

	if (SUCCEEDED(hr = m_pDSC->CreateCaptureBuffer(&dscbd, &pDSCB, NULL)))
	{
		hr = pDSCB->QueryInterface(IID_IDirectSoundCaptureBuffer, (LPVOID*)&m_pDSBCapture);
		pDSCB->Release(); 
	}
	if(hr != S_OK)
	{
		return FALSE;
	}

	m_hNotificationEvent = CreateEvent(NULL,FALSE,FALSE,NULL); //创建事件
	m_hQuitRecord = CreateEvent(NULL,FALSE,FALSE,NULL);
	m_hQuitWork = CreateEvent(NULL,FALSE,FALSE,NULL);
	LPDIRECTSOUNDNOTIFY pDSCNotify = NULL;
	if(FAILED(hr = m_pDSBCapture ->QueryInterface(IID_IDirectSoundNotify,(VOID**)&pDSCNotify)))
		return FALSE;

	for( int i = 0; i < NUM_REC_NOTIFICATIONS; i++ )
	{
		m_aPosNotify[i].dwOffset = (SIZE_REC_BLOCK  * i);
		m_aPosNotify[i].hEventNotify = m_hNotificationEvent; 
	}
	hr = pDSCNotify->SetNotificationPositions( NUM_REC_NOTIFICATIONS, m_aPosNotify );
	pDSCNotify->Release();
	if(hr != S_OK)
		return FALSE;


	m_wfxInput = *pWfx;
	m_dwCaptureBufferSize = dscbd.dwBufferBytes;
	m_dwNotifySize = SIZE_REC_BLOCK;
	m_dwUser = dwUser;
	m_pCallback = pFnc;

	m_hThreadNotify = ::CreateThread(NULL,0,ThreadRecord,this,0,NULL);
	m_hThreadWork = ::CreateThread(NULL,0,ThreadWork,this,0,NULL); 

	// 判断当前的录入混音器
	HWAVEIN	hWaveIn = NULL;
	WAVEFORMATEX wfx;
	MMRESULT mmr;
	wfx.wFormatTag = WAVE_FORMAT_PCM;
	wfx.nChannels = 1;
	wfx.nSamplesPerSec = 11025;
	wfx.nAvgBytesPerSec = 11025;
	wfx.nBlockAlign = 1;
	wfx.wBitsPerSample = 8;
	wfx.cbSize = 0;
	mmr = waveInOpen( &hWaveIn,WAVE_MAPPER, &wfx, 0L, 0L, CALLBACK_NULL );
	if ( mmr == MMSYSERR_NOERROR )
	{
		mmr = mixerGetID( (HMIXEROBJ)hWaveIn, &m_uMixID, MIXER_OBJECTF_HWAVEIN );
		waveInClose( hWaveIn);
	}

	return TRUE;
}

void CMMAudioCap::Start()
{
	HRESULT hr; 
	if(m_pDSBCapture)
		hr = m_pDSBCapture->Start(DSCBSTART_LOOPING);
}

void CMMAudioCap::Stop()
{
	HRESULT hr; 
	if(m_pDSBCapture)
		hr = m_pDSBCapture->Stop();
}


void CMMAudioCap::ProcRecord()
{

	DWORD	dwResult =0;
	HANDLE	events[2];
	events[0] = m_hNotificationEvent;
	events[1] = m_hQuitRecord;
	while(true)
	{
		dwResult = WaitForMultipleObjects(2,events,FALSE,INFINITE );
		if( dwResult == WAIT_OBJECT_0 + 0)
		{
			OnCapturedNotify();
		}
		else
		{
			break;
		}
	};
}

void CMMAudioCap::SetWaveNotify(WaveNotifyFnc pFnc,DWORD dwUser,int nTime)
{
	m_pNotifyFnc = pFnc;
	m_dwUserNotify = dwUser;
	m_nNotifyTime = nTime;
	m_bTimer = setTimer(10,static_cast<ITimerSink*>(this),nTime);
	
}

void CMMAudioCap::onTimer(uint32 lEventID )
{
	if(lEventID == 10)
	{
		if(m_pNotifyFnc)
		{
			if(m_lstWave.size() > 0)
			{
				int b = m_lstWave.front();
				m_lstWave.pop_front();
				m_pNotifyFnc(b,m_dwUserNotify);
			}
		}
	}
}

bool CMMAudioCap::IsSilence(BYTE * pData,int nLen)
{
	if(*pData != m_bSilence)
	{
		return false;
	}
	//else
	//{
	//	int i;
	//	for(i = 0;i < nLen; i++)
	//	{
	//		if(*pData != m_bSilence)
	//			return false;
	//	}
	//	return true;
	//}
	return false;
}

void CMMAudioCap::ProcWork()
{

	DWORD	dwResult =0;
	HANDLE	events[2];
	events[0] = m_hSemSignal;
	events[1] = m_hQuitWork;
	char * pData = new char[SIZE_REC_BLOCK];
	int nLen;
	while(true)
	{
		dwResult = WaitForMultipleObjects(2,events,FALSE,INFINITE );
		if( dwResult == WAIT_OBJECT_0 + 0)
		{
			//TRACE("pop\n");
			nLen = m_audioData.Pop(pData,SIZE_REC_BLOCK);
			if(m_pCallback && nLen > 0)
			{
				// 判断是否为静音,并采样波形
				if(m_bTimer)
				{
					ProcWaveData((BYTE*)pData,nLen);
				}
				if(IsSilence((BYTE*)pData,nLen))
				{
					m_pCallback((BYTE*)pData,0,m_dwUser);
				}
				else
				{
					m_pCallback((BYTE*)pData,nLen,m_dwUser);
				}
				
			}
		}
		else
		{
			break;
		}
	}
	delete [] pData;
}

void CMMAudioCap::ProcWaveData(BYTE * pData,int nLen)
{
	// 定时间隔时间内的字节数
	int nSpan = m_wfxInput.nAvgBytesPerSec * m_nNotifyTime / 1000; 
	int nV = 0;
	if(nSpan < 1)
	{
		nSpan = 1;
	}
	assert(m_nNotify < nSpan);
	pData += m_nNotify;
	m_nNotify += nLen;
	while(m_nNotify >= nSpan)
	{
		if(m_lstWave.size() > 1000)
		{
			m_lstWave.pop_front();
		}
		if(*pData == m_bSilence)
		{
			nV = 0;
		}
		else
		{
			nV = *pData;
			nV = nV * 100 / 256;
		}
		m_lstWave.push_back(nV);
		m_nNotify -= nSpan;
		pData += nSpan;
	}
	
}



void CMMAudioCap::OnCapturedNotify()
{
	
	HRESULT hr;
	VOID *pbCaptureData = NULL;
	DWORD dwLen;
	VOID *pbCaptureData2 = NULL;
	DWORD dwLen2;
	DWORD dwReadPos;
	DWORD dwCapturePos;
	LONG lLockSize;

	if(FAILED( hr = m_pDSBCapture->GetCurrentPosition(&dwCapturePos,&dwReadPos)))
		return ;
	lLockSize = dwReadPos - m_dwNextCaptureOffset;
	if( lLockSize < 0 )
		lLockSize += m_dwCaptureBufferSize;
	lLockSize -= (lLockSize % m_dwNotifySize);
	if( lLockSize == 0 )
		return;

	if( FAILED( hr = m_pDSBCapture->Lock( m_dwNextCaptureOffset, lLockSize, 
		&pbCaptureData, &dwLen, 
		&pbCaptureData2, &dwLen2, 0L ) ) )
		return ;

	//TRACE("audio data len = %d\n",dwLen);
	//TRACE("audio cap buf size = %d\n",m_audioData.GetSize());
	
	long nPreCount = 0;
	// 如果音频捕捉存储缓冲区已满，则覆盖
	if(m_audioData.GetSize() >= NUM_CAP_BUF)
	{
		TRACE("音频捕捉存储缓冲区已满\n");
		m_audioData.Pop(NULL,0);
	}
	m_audioData.Push((char*)pbCaptureData,dwLen);
	::ReleaseSemaphore(m_hSemSignal,1,&nPreCount);

	// 移动偏移标志,循环移动
	m_dwNextCaptureOffset += dwLen; 
	m_dwNextCaptureOffset %= m_dwCaptureBufferSize;

	if( pbCaptureData2 != NULL )
	{
		TRACE("err 音频捕捉有数据丢失,len = %d \n",dwLen2);
		// 处理内存中的数据
		/*if(m_pCallback)
		{
			m_pCallback((BYTE*)pbCaptureData2,dwLen2,m_dwUser);
		}*/
		// 移动偏移标志
		m_dwNextCaptureOffset += dwLen2; 
		m_dwNextCaptureOffset %= m_dwCaptureBufferSize; 
	}

	// 内存解锁
	m_pDSBCapture->Unlock( pbCaptureData, dwLen, pbCaptureData2, dwLen2 );


}


struct FIND_AD_PARAM
{
	WORD			wFormatTag;
	BOOL			bFind;
	HACMDRIVERID	hAdID;
	WAVEFORMATEX *	pWfx;
};

BOOL CALLBACK IdentifyDriverProc(HACMDRIVERID hadid, LPACMFORMATDETAILS pafd, DWORD dwInstance, DWORD fdwSupport) 
{
	FIND_AD_PARAM & param = *(FIND_AD_PARAM *)dwInstance;
	//TRACE("index = %d,%d,%s\n",pafd->dwFormatIndex,pafd->dwFormatTag,pafd->szFormat);
	if(pafd->dwFormatTag == param.wFormatTag)
	{
		//return TRUE;
		param.bFind  = TRUE;
		return FALSE;
	}
	return TRUE; 
}


BOOL CALLBACK CMMAudioCodec::FindFormatProc(HACMDRIVERID hadid, LPACMFORMATDETAILS pafd, DWORD dwInstance, DWORD fdwSupport) 
{
	FIND_AD_PARAM & param = *(FIND_AD_PARAM *)dwInstance;
	if(pafd->dwFormatTag == param.wFormatTag)
	{
		TRACE("%s\n",pafd->szFormat);
		if(param.pWfx)
		{
			// 查找匹配的格式
			WAVEFORMATEX * pCur = pafd->pwfx;
			WAVEFORMATEX * pWfx = param.pWfx;
			if( pWfx->nChannels == pCur->nChannels &&
				pWfx->nSamplesPerSec == pCur->nSamplesPerSec)
			{
				//if(pWfx->nAvgBytesPerSec != 0 && pWfx->nAvgBytesPerSec != pCur->nAvgBytesPerSec)
				//	return FALSE;
				param.bFind  = TRUE;
				return FALSE;
			}


		}
	}
	return TRUE; 
}


BOOL CALLBACK CMMAudioCodec::FindDriverProc(HACMDRIVERID hadid, DWORD dwInstance, DWORD fdwSupport) 
{
	FIND_AD_PARAM & param = *(FIND_AD_PARAM *)dwInstance;
	ACMDRIVERDETAILS acmdd;
	HACMDRIVER had = NULL;
	MMRESULT mmr;
	DWORD dwSize = 0;
	acmdd.cbStruct = sizeof(acmdd);
	mmr = acmDriverOpen(&had, hadid, 0);//打开驱动程序
	if (mmr)  
	{
		// 打开失败
		param.bFind = FALSE;
	}
	else 
	{
		// 获取正确的格式大小
		mmr = acmMetrics((HACMOBJ)had, ACM_METRIC_MAX_SIZE_FORMAT, &dwSize);
		WORD cbSize =  0;
		if(LOWORD(dwSize) > sizeof(WAVEFORMATEX))
			cbSize = LOWORD(dwSize) - sizeof(WAVEFORMATEX);
		if(dwSize < sizeof(WAVEFORMATEX))
			dwSize = sizeof(WAVEFORMATEX);

		WAVEFORMATEX* pwf = (WAVEFORMATEX*) new char[dwSize];
		memset(pwf, 0, dwSize);

		pwf->cbSize = cbSize;
		pwf->wFormatTag = WAVE_FORMAT_UNKNOWN;

		ACMFORMATDETAILS fd;
		memset(&fd, 0, sizeof(fd));
		fd.cbStruct = sizeof(fd);  
		fd.pwfx = pwf;  
		fd.cbwfx = dwSize;
		fd.dwFormatTag = WAVE_FORMAT_UNKNOWN;
		// 需要判断当前驱动器ID是否为需要的驱动器
		mmr = acmFormatEnum(had, &fd, IdentifyDriverProc, (DWORD_PTR)&param, 0);  
		if(pwf)
			delete [] pwf;
		acmDriverClose(had, 0);
		if(param.bFind)
		{
			param.hAdID = hadid;
			return FALSE;
		}
	}
	return TRUE; 
}



CMMAudioCodec::CMMAudioCodec()
{
	m_acmDID = NULL;
	m_had = NULL;
	m_pWfxCmp = NULL;
	m_pWfxPcm = NULL;
	m_pBufPcm = NULL;
	m_pBufCmp = NULL;
	m_pBufDecmp = NULL;
}

CMMAudioCodec::~CMMAudioCodec()
{
	Clear();

}
void CMMAudioCodec::Clear()
{
	m_acmDID = 0;
	if(m_had)
	{
		acmDriverClose(m_had,0);
		m_had = NULL;
	}
	if(m_pWfxPcm)
	{
		delete m_pWfxPcm;
		m_pWfxPcm = NULL;
	}
	if(m_pWfxCmp)
	{
		delete m_pWfxCmp;
		m_pWfxCmp = NULL;
	}
	if(m_pBufCmp)
	{
		delete [] m_pBufCmp;
		m_pBufCmp = NULL;
	}
	if(m_pBufPcm)
	{
		delete [] m_pBufPcm;
		m_pBufPcm = NULL;
	}
	if(m_pBufDecmp)
	{
		delete [] m_pBufDecmp;
		m_pBufDecmp = NULL;
	}
}


BOOL CMMAudioCodec::Init(WAVEFORMATEX * pWfxSrc, WAVEFORMATEX * pWfxCmp, int nSrcBufLen /* = CMMAudioCap::SIZE_REC_BLOCK  */)
{
	Clear();
	MMRESULT mmr;
	if(m_acmDID == NULL)
	{
		m_acmDID = FindAcmDriverID(pWfxCmp->wFormatTag);
	}
	if(m_pWfxCmp == NULL)
	{
		m_pWfxCmp = FindWaveFormat(m_acmDID,pWfxCmp);
		if(m_pWfxCmp == NULL)
			return FALSE;
	}
	if(m_pWfxPcm == NULL)
	{
		WAVEFORMATEX wavePcm = *m_pWfxCmp;
		wavePcm.wFormatTag = WAVE_FORMAT_PCM;
		m_pWfxPcm = FindWaveFormat(m_acmDID,&wavePcm);
		if(m_pWfxPcm == NULL)
			return FALSE;
	}
	if(m_had == NULL)
	{
		mmr = acmDriverOpen(&m_had,m_acmDID,0);//打开驱动程序
		if(mmr)
			return FALSE;
	}
	m_wfxSrc = *pWfxSrc;
	m_nSrcBufLen = nSrcBufLen;
	m_nPcmBufLen = (int)((double)m_nSrcBufLen * ((double)m_pWfxPcm->nAvgBytesPerSec / (double)m_wfxSrc.nAvgBytesPerSec));
	m_nPcmBufLen = (m_nPcmBufLen * 2 + 4) / 4 * 4;
	m_nCmpBufLen = m_nSrcBufLen * 2;
	
	if(m_pBufPcm == NULL)
	{
		m_pBufPcm = new BYTE[m_nPcmBufLen];
	}
	if(m_pBufCmp == NULL)
	{
		m_pBufCmp = new BYTE[m_nCmpBufLen];
	}
	if(m_pBufDecmp == NULL)
	{
		m_pBufDecmp = new BYTE[m_nPcmBufLen];
	}

	return TRUE;
}


// 根据指定的格式，找到对应的驱动程序ID

HACMDRIVERID CMMAudioCodec::FindAcmDriverID(WORD wFormatTag)
{
	FIND_AD_PARAM param;
	param.bFind = FALSE;
	param.wFormatTag = wFormatTag;
	param.hAdID = 0;
	param.pWfx = NULL;
	// 枚举所有驱动程序
	MMRESULT mmr = acmDriverEnum(FindDriverProc,(DWORD_PTR)&param, 0);
	if(param.bFind)
	{
		return param.hAdID;
	}
	return 0;
};

WAVEFORMATEX * CMMAudioCodec::FindWaveFormat(HACMDRIVERID hadid,WAVEFORMATEX * pFindWfx)
{
	if(pFindWfx == NULL)
		return NULL;
	MMRESULT mmr;
	FIND_AD_PARAM param;
	HACMDRIVER had;
	DWORD dwSize = 0;
	param.bFind = FALSE;
	param.pWfx = pFindWfx;
	param.wFormatTag = pFindWfx->wFormatTag;

	mmr = acmDriverOpen(&had, hadid, 0);//打开驱动程序
	if (mmr)  
	{
		return NULL;
	}
	else 
	{
		// 获取正确的格式大小
		mmr = acmMetrics((HACMOBJ)had, ACM_METRIC_MAX_SIZE_FORMAT, &dwSize);
		WORD cbSize =  0;
		if(LOWORD(dwSize) > sizeof(WAVEFORMATEX))
			cbSize = LOWORD(dwSize) - sizeof(WAVEFORMATEX);
		if(dwSize < sizeof(WAVEFORMATEX))
			dwSize = sizeof(WAVEFORMATEX);

		WAVEFORMATEX* pwf = (WAVEFORMATEX*) new char[dwSize];
		memset(pwf, 0, dwSize);

		pwf->cbSize = cbSize;
		pwf->wFormatTag = WAVE_FORMAT_UNKNOWN;

		ACMFORMATDETAILS fd;
		memset(&fd, 0, sizeof(fd));
		fd.cbStruct = sizeof(fd);  
		fd.pwfx = pwf;  
		fd.cbwfx = dwSize;
		fd.dwFormatTag = WAVE_FORMAT_UNKNOWN;
		mmr = acmFormatEnum(had, &fd, FindFormatProc, (DWORD_PTR)&param, 0);  
		acmDriverClose(had, 0);
		if(param.bFind)
		{
			return pwf;
		}
	}
	return NULL;
}

//WAVEFORMATEX * CMMAudioCodec::FindWaveFormat(HACMDRIVERID hadid,WAVEFORMATEX * pFindWfx)
//{
//	if(pFindWfx == NULL)
//		return NULL;
//	MMRESULT mmr;
//	FIND_AD_PARAM param;
//	HACMDRIVER had;
//	DWORD dwSize = 0;
//	param.bFind = FALSE;
//	param.pWfx = pFindWfx;
//	param.wFormatTag = pFindWfx->wFormatTag;
//
//	mmr = acmDriverOpen(&had, hadid, 0);//打开驱动程序
//	if (mmr)  
//	{
//		return NULL;
//	}
//	else 
//	{
//		// 获取正确的格式大小
//		mmr = acmMetrics((HACMOBJ)had, ACM_METRIC_MAX_SIZE_FORMAT, &dwSize);
//		WORD cbSize =  0;
//		if(LOWORD(dwSize) > sizeof(WAVEFORMATEX))
//			cbSize = LOWORD(dwSize) - sizeof(WAVEFORMATEX);
//		if(dwSize < sizeof(WAVEFORMATEX))
//			dwSize = sizeof(WAVEFORMATEX);
//
//		WAVEFORMATEX* pwf = (WAVEFORMATEX*) new char[dwSize];
//		memset(pwf, 0, dwSize);
//
//		pwf->cbSize = cbSize;
//		pwf->wFormatTag = WAVE_FORMAT_UNKNOWN;
//
//		ACMFORMATDETAILS fd;
//		memset(&fd, 0, sizeof(fd));
//		fd.cbStruct = sizeof(fd);  
//		fd.pwfx = pwf;  
//		fd.cbwfx = dwSize;
//		fd.dwFormatTag = WAVE_FORMAT_UNKNOWN;
//		mmr = acmFormatEnum(had, &fd, FindFormatProc, (DWORD_PTR)&param, 0);  
//		acmDriverClose(had, 0);
//		if(param.bFind)
//		{
//			return pwf;
//		}
//	}
//	return NULL;
//}


 BYTE * CMMAudioCodec::CmpWaveData( BYTE * pData,int & nLen)
{
	if(nLen > m_nSrcBufLen)
	{
		nLen = m_nSrcBufLen;
	}
	MMRESULT mmr;
	HACMSTREAM hAcmStream1 = NULL,hAcmStream2 = NULL;
	HACMDRIVER had = NULL;

	// 压缩后的格式

	MMRESULT rst;

	// src --> cmp
	rst = acmStreamOpen(&hAcmStream1,m_had,&m_wfxSrc,m_pWfxCmp,NULL,(DWORD_PTR)0,0,ACM_STREAMOPENF_NONREALTIME);//ACM_STREAMOPENF_NONREALTIME
	if(rst)
		return NULL;
	ACMSTREAMHEADER strhdr; //填充转换信息
	memset(&strhdr, 0, sizeof(strhdr));

	// 先由源格式转为cmp格式 src->cmp
	strhdr.cbStruct = sizeof(strhdr);
	strhdr.pbSrc = (BYTE*)pData; // 源数据
	strhdr.cbSrcLength = nLen ;
	strhdr.pbDst = m_pBufCmp;
	strhdr.cbDstLength = m_nCmpBufLen; 

	mmr = acmStreamPrepareHeader(hAcmStream1, &strhdr, 0); 
	if(mmr)
		return NULL;
	mmr = acmStreamConvert(hAcmStream1, &strhdr, 0); // 转换数据
	if(mmr)
		return NULL;
	mmr = acmStreamUnprepareHeader(hAcmStream1,&strhdr,0);
	if(mmr)
		return NULL;
	mmr = acmStreamClose(hAcmStream1, 0);
	if(mmr)
		return NULL;
	if(strhdr.cbSrcLengthUsed != strhdr.cbSrcLength)
	{
		TRACE("notice!!!!!!!!!!!\n");
	}

	//// src --> pcm
	//rst = acmStreamOpen(&hAcmStream1,NULL,&m_wfxSrc,m_pWfxPcm,NULL,(DWORD_PTR)0,0,ACM_STREAMOPENF_NONREALTIME);//CALLBACK_WINDOW
	//// pcm --> cmp
	//rst = acmStreamOpen(&hAcmStream2,had,m_pWfxPcm,m_pWfxCmp,NULL,(DWORD_PTR)0,0,ACM_STREAMOPENF_NONREALTIME);//CALLBACK_WINDOW

	//ACMSTREAMHEADER strhdr; //填充转换信息
	//memset(&strhdr, 0, sizeof(strhdr));

	//// 先由源格式转为pcm格式 src->pcm
	//strhdr.cbStruct = sizeof(strhdr);
	//strhdr.pbSrc = (BYTE*)pData; // 源数据
	//strhdr.cbSrcLength = nLen;
	//strhdr.pbDst = m_pBufPcm;
	//strhdr.cbDstLength = m_nPcmBufLen;

	//mmr = acmStreamPrepareHeader(hAcmStream1, &strhdr, 0); 
	//if(mmr)
	//	return NULL;
	//mmr = acmStreamConvert(hAcmStream1, &strhdr, 0); // 转换数据
	//if(mmr)
	//	return NULL;
	//mmr = acmStreamUnprepareHeader(hAcmStream1,&strhdr,0);
	//if(mmr)
	//	return NULL;
	//mmr = acmStreamClose(hAcmStream1, 0);
	//if(mmr)
	//	return NULL;

	//// pcm格式到目的格式的压缩 pcm->src
	//strhdr.cbStruct = sizeof(strhdr);
	//strhdr.pbSrc = (BYTE*)m_pBufPcm; // 源数据
	//strhdr.cbSrcLength = strhdr.cbDstLengthUsed;
	//strhdr.pbDst = m_pBufCmp;
	//strhdr.cbDstLength = m_nCmpBufLen;

	//mmr = acmStreamPrepareHeader(hAcmStream2, &strhdr, 0); 
	//if(mmr)
	//	return NULL;
	//mmr = acmStreamConvert(hAcmStream2, &strhdr, 0); // 转换数据
	//if(mmr)
	//	return NULL;
	//mmr = acmStreamUnprepareHeader(hAcmStream2,&strhdr,0);
	//if(mmr)
	//	return NULL;
	//mmr = acmStreamClose(hAcmStream2, 0);
	//if(mmr)
	//	return NULL;
	//TRACE("压缩比为%4.3f\n",m_nCmpBufLen/(float)strhdr.cbDstLengthUsed);
	nLen = strhdr.cbDstLengthUsed;
	return m_pBufCmp;
}


 BYTE * CMMAudioCodec::DecmpWaveData( BYTE * pData,int & nLen)
{
	MMRESULT mmr;
	HACMSTREAM hAcmStream = NULL;
	// 还没有初始化成功
	if(m_pWfxCmp == NULL)
		return NULL;
	// cmp -> pcm
	//mmr = acmStreamOpen(&hAcmStream,m_had,m_pWfxCmp,m_pWfxPcm,NULL,(DWORD_PTR)0,0,ACM_STREAMOPENF_NONREALTIME);
	// cmp -> src
	mmr = acmStreamOpen(&hAcmStream,m_had,m_pWfxCmp,&m_wfxSrc,NULL,(DWORD_PTR)0,0,ACM_STREAMOPENF_NONREALTIME);//ACM_STREAMOPENF_NONREALTIME

	ACMSTREAMHEADER strhdr; //填充转换信息
	memset(&strhdr, 0, sizeof(strhdr));
	strhdr.cbStruct = sizeof(strhdr);
	strhdr.pbSrc = (BYTE*)pData; // 源数据
	strhdr.cbSrcLength = nLen;
	strhdr.pbDst = m_pBufDecmp;
	strhdr.cbDstLength = m_nPcmBufLen;
	memset(m_pBufDecmp,0x80,m_nPcmBufLen);

	mmr = acmStreamPrepareHeader(hAcmStream, &strhdr, 0); 
	if(mmr)
		return NULL;
	mmr = acmStreamConvert(hAcmStream, &strhdr, 0); //转换数据
	if(mmr)
		return NULL;
	mmr = acmStreamUnprepareHeader(hAcmStream,&strhdr,0);
	if(mmr)
		return NULL;
	mmr = acmStreamClose(hAcmStream, 0);
	if(mmr)
		return NULL;
	nLen = strhdr.cbDstLengthUsed;
	if(strhdr.cbSrcLength != strhdr.cbSrcLengthUsed)
	{
		TRACE("notice!!!!!\n");
	}
	return m_pBufDecmp;
}


BOOL CMMAudioCap::GetLineInfo(LPMIXERLINE pmxl, DWORD dwComponentType)
{
	MMRESULT mmr;
	//MIXERLINE MixerLine;
	memset( pmxl, 0, sizeof(MIXERLINE) );
	pmxl->cbStruct = sizeof(MIXERLINE);
	pmxl->dwComponentType = dwComponentType;
	mmr = mixerGetLineInfo( (HMIXEROBJ)m_uMixID, pmxl, MIXER_GETLINEINFOF_COMPONENTTYPE );
	if(mmr)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CMMAudioCap::GetLineControl(LPMIXERCONTROL pmxc, LPMIXERLINE pmxl, DWORD dwType)
{
	LPMIXERCONTROL pamxctrl;
	MMRESULT mmr;
	if(pmxl->cControls == 0)
		pmxl->cControls = 1;
	DWORD cbmxctrls = sizeof(*pamxctrl) * (UINT)pmxl->cControls;
	pamxctrl = (LPMIXERCONTROL)LocalAlloc(LPTR, cbmxctrls);

	MIXERLINECONTROLS mxlc;
	mxlc.cbStruct = sizeof(mxlc);
	mxlc.dwLineID = pmxl->dwLineID;
	mxlc.dwControlType = dwType;
	mxlc.cControls = pmxl->cControls;
	mxlc.cbmxctrl = sizeof(*pamxctrl);
	mxlc.pamxctrl = pamxctrl;

	if (mmr = mixerGetLineControls((HMIXEROBJ)m_uMixID, &mxlc, MIXER_GETLINECONTROLSF_ONEBYTYPE))
	{
		return FALSE;
	}
	memcpy(pmxc, pamxctrl, sizeof(*pamxctrl));
	LocalFree(pamxctrl);

	m_dwMax = pmxc->Bounds.dwMaximum;
	m_dwMin = pmxc->Bounds.dwMinimum;
	return TRUE;
}

void CMMAudioCap::SetControlDetails(DWORD dwControlID,DWORD dwValue)
{
	MMRESULT mmr;
	MIXERCONTROLDETAILS mxcd;
	MIXERCONTROLDETAILS_UNSIGNED mxcd_u1;

	mxcd.cbStruct = sizeof(mxcd);
	mxcd.dwControlID = dwControlID;
	mxcd.cMultipleItems = 0;

	mxcd.cChannels = 1;
	mxcd.cbDetails = sizeof(mxcd_u1);
	mxcd.paDetails = &mxcd_u1;
	mxcd_u1.dwValue = dwValue;	
	mmr = mixerSetControlDetails( (HMIXEROBJ)m_uMixID, &mxcd, MIXER_SETCONTROLDETAILSF_VALUE );

}

void CMMAudioCap::SetCapVolume(DWORD nV)
{
	double nTimes = (m_dwMax - m_dwMin) / 100.0;
	nV *= nTimes;
	if(nV < m_dwMin)
		nV = m_dwMin;
	else if(nV > m_dwMax)
		nV = m_dwMax;

	MIXERLINE mxl;	
	MIXERCONTROL mxc;

	if (GetLineInfo(&mxl,MIXERLINE_COMPONENTTYPE_DST_WAVEIN))
	{
		if(GetLineControl(&mxc, &mxl, MIXERCONTROL_CONTROLTYPE_VOLUME))
			SetControlDetails(mxc.dwControlID,nV);
	}

	if (GetLineInfo(&mxl,MIXERLINE_COMPONENTTYPE_SRC_MICROPHONE))
	{
		if(GetLineControl(&mxc, &mxl, MIXERCONTROL_CONTROLTYPE_VOLUME))
			SetControlDetails(mxc.dwControlID,nV);
	}
}