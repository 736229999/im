#include "StdAfx.h"
#include "mmaudio.h"

struct FIND_AD_PARAM
{
	WORD			wFormatTag;
	BOOL			bFind;
	HACMDRIVERID	hAdID;
	WAVEFORMATEX *	pWfx;
};


CMMAudio::CMMAudio():m_audioData(20,CAPBUF_LEN*2)//,m_eventPlay(TRUE)
{
	m_hWaveIn = NULL;
	m_hWaveOut = NULL;
	m_dwUser = 0;
	m_pCallBackCmpData = NULL;
	m_acmDID = NULL;
	m_pWfxCmp = NULL;
	m_pWfxPcm = NULL;
	m_had = NULL;
	m_nCapBufLen = CAPBUF_LEN;
	m_nCmpBufLen = CAPBUF_LEN * 2;
	m_bRecording = FALSE;
	m_bPlayOk = false;

	m_pInBuf1 = new BYTE[m_nCapBufLen];
	m_pInBuf2 = new BYTE[m_nCapBufLen];
	m_pOutBuf1 = new BYTE[m_nCmpBufLen];
	m_pOutBuf2 = new BYTE[m_nCmpBufLen];

	
	m_pInHeader1 = (PWAVEHDR)new char[sizeof(WAVEHDR)];
	m_pInHeader2 = (PWAVEHDR)new char[sizeof(WAVEHDR)];	
	m_pOutHeader1 = (PWAVEHDR)new char[sizeof(WAVEHDR)];
	m_pOutHeader2 = (PWAVEHDR)new char[sizeof(WAVEHDR)];

	m_pCmpBufPcm = new BYTE[m_nCmpBufLen];
	m_pCmpBufDst = new BYTE[m_nCmpBufLen];
	m_pDecmpData = new BYTE[m_nCmpBufLen];

	// 默认使用最差音质
	m_waveformIn.wFormatTag = WAVE_FORMAT_PCM;
	m_waveformIn.nChannels = 1;
	m_waveformIn.nSamplesPerSec = 11025;
	m_waveformIn.nAvgBytesPerSec = 11025;
	m_waveformIn.nBlockAlign = 1;
	m_waveformIn.wBitsPerSample = 8;
	m_waveformIn.cbSize = 0;

}

CMMAudio::~CMMAudio(void)
{
	if(m_had)
	{
		acmDriverClose(m_had,0);
		m_had = NULL;
	}

	if(m_hWaveIn)
	{
		waveInUnprepareHeader(m_hWaveIn,m_pInHeader1,sizeof(WAVEHDR));
		waveInUnprepareHeader(m_hWaveIn,m_pInHeader2,sizeof(WAVEHDR));
		waveInClose(m_hWaveIn);
		Sleep(300);
	}

	if(m_hWaveOut)
	{
		waveOutUnprepareHeader(m_hWaveOut,m_pOutHeader1,sizeof(WAVEHDR));
		waveOutUnprepareHeader(m_hWaveOut,m_pOutHeader2,sizeof(WAVEHDR));
		waveOutClose(m_hWaveOut);
		Sleep(300);
	}

	if(m_pCmpBufDst)
	{
		delete [] m_pCmpBufDst;
	}

	if(m_pCmpBufPcm)
	{
		delete [] m_pCmpBufPcm;
	}


	if(m_pDecmpData)
	{
		delete [] m_pDecmpData;
	}
	if(m_pInBuf1)
	{
		delete [] m_pInBuf1;
	}
	if(m_pInBuf2)
	{
		delete [] m_pInBuf2;
	}
	if(m_pInHeader1)
	{
		delete [] m_pInHeader1;
	}
	if(m_pInHeader2)
	{
		delete [] m_pInHeader2;
	}
	if(m_pOutBuf1)
	{
		delete [] m_pOutBuf1;
	}
	if(m_pOutBuf2)
	{
		delete [] m_pOutBuf2;
	}
	if(m_pOutHeader1)
	{
		delete [] m_pOutHeader1;
	}
	if(m_pOutHeader2)
	{
		delete [] m_pOutHeader2;
	}


	
	

}

BOOL CALLBACK FindFormatProc(HACMDRIVERID hadid, LPACMFORMATDETAILS pafd, DWORD dwInstance, DWORD fdwSupport) 
{
	FIND_AD_PARAM & param = *(FIND_AD_PARAM *)dwInstance;
	if(pafd->dwFormatTag == param.wFormatTag)
	{
		// 查找第一种格式
		param.bFind  = TRUE;
		return FALSE;
	}
	return TRUE; 
}


BOOL CALLBACK FindDriverProc(HACMDRIVERID hadid, DWORD dwInstance, DWORD fdwSupport) 
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
		mmr = acmFormatEnum(had, &fd, FindFormatProc, (DWORD_PTR)&param, 0);  
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


// 根据指定的格式，找到对应的驱动程序ID

HACMDRIVERID CMMAudio::FindAcmDriverID(WORD wFormatTag)
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


WAVEFORMATEX * CMMAudio::FindWaveFormat(HACMDRIVERID hadid,WORD wFormat)
{
	MMRESULT mmr;
	FIND_AD_PARAM param;
	HACMDRIVER had;
	DWORD dwSize = 0;
	param.pWfx = NULL;
	param.wFormatTag = wFormat;

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

void CALLBACK CMMAudio::WaveInProc( HWAVEIN hwi, UINT uMsg,DWORD dwInstance, DWORD dwParam1,DWORD dwParam2)
{

	CMMAudio * pAudioCap = (CMMAudio *)dwInstance;
	static int nTime = 0;
	switch(uMsg)
	{
	case MM_WIM_CLOSE:	// Sent when the device is closed using the waveInClose function. 
		TRACE("audio close!\n");
		pAudioCap->OnWaveInClose();
		break;
	case MM_WIM_DATA:	// Sent when the device driver is finished with a data block sent using the waveInAddBuffer function. 
		pAudioCap->OnWaveData((WAVEHDR *)dwParam1);
		break;
	case MM_WIM_OPEN :	// Sent when the device is opened using the waveInOpen function
		TRACE("audio open\n");
		break;
	};
}

void CALLBACK CMMAudio::WaveOutProc(HWAVEOUT hwo,UINT uMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2)
{
	CMMAudio * pAudioCap = (CMMAudio * )dwInstance;
	switch(uMsg)
	{
	case WOM_DONE:
		if(pAudioCap)
			pAudioCap->OnWaveOutDone();
		break;
	default:
	    break;
	}
}

void CMMAudio::OnWaveOutDone()
{
	//m_eventPlay.Set();
	/*MMRESULT mmr;
	mmr = waveOutPrepareHeader(m_hWaveOut,m_pOutHeader1,sizeof(WAVEHDR));	
	mmr = waveOutPrepareHeader(m_hWaveOut,m_pOutHeader2,sizeof(WAVEHDR));*/
	m_bPlayOk = true;
}

BOOL CMMAudio::Init()
{
	// WAVE_FORMAT_MPEGLAYER3   WAVE_FORMAT_DSPGROUP_TRUESPEECH WAVE_FORMAT_MSG723
	MMRESULT mmr;
	if(m_acmDID == NULL)
	{
		m_acmDID = FindAcmDriverID(WAVE_FORMAT_DSPGROUP_TRUESPEECH);
	}
	if(m_pWfxCmp == NULL)
	{
		m_pWfxCmp = FindWaveFormat(m_acmDID, WAVE_FORMAT_DSPGROUP_TRUESPEECH);
		if(m_pWfxCmp == NULL)
			return FALSE;
	}
	if(m_pWfxPcm == NULL)
	{
		m_pWfxPcm = FindWaveFormat(m_acmDID,WAVE_FORMAT_PCM);
		if(m_pWfxPcm == NULL)
			return FALSE;
	}
	if(m_had == NULL)
	{
		mmr = acmDriverOpen(&m_had,m_acmDID, 0);//打开驱动程序
		if(mmr)
			return FALSE;
	}
	

	// 设置波形数据头
	m_pInHeader1->dwBufferLength = m_nCapBufLen;
	m_pInHeader1->lpData = (LPSTR)m_pInBuf1;
	m_pInHeader1->dwBytesRecorded = 0;
	m_pInHeader1->dwUser = 0;
	m_pInHeader1->dwFlags = 0;
	m_pInHeader1->dwLoops = 1;
	m_pInHeader1->lpNext = NULL;
	m_pInHeader1->reserved = 0;
	*m_pInHeader2 = *m_pInHeader1;
	*m_pOutHeader1 = *m_pInHeader1;
	*m_pOutHeader2 = *m_pInHeader1;
	m_pInHeader1->lpData = (LPSTR)m_pInBuf1;
	m_pInHeader2->lpData = (LPSTR)m_pInBuf2;
	m_pOutHeader1->lpData = (LPSTR)m_pOutBuf1;
	m_pOutHeader2->lpData = (LPSTR)m_pOutBuf2;



	MMRESULT rst ;
	DWORD dwData = (DWORD)this;
	rst = waveInOpen(&m_hWaveIn,WAVE_MAPPER,&m_waveformIn,(DWORD)WaveInProc,dwData,CALLBACK_FUNCTION);
	if(rst != 0)
	{
		TRACE("输入设备打开失败!\n");
		return FALSE;
	}

	rst = waveOutOpen(&m_hWaveOut,WAVE_MAPPER,&m_waveformIn,(DWORD)WaveOutProc,dwData,CALLBACK_FUNCTION);
	if(rst)
	{
		TRACE("输出设备打开失败!");
		return FALSE;
	}

	rst = waveOutOpen(&m_hWaveOut,WAVE_MAPPER,&m_waveformIn,(DWORD)0,0,CALLBACK_NULL);
	if(rst)
	{
		TRACE("输出设备打开失败!");
		return FALSE;
	}

	//rst = waveOutOpen(&m_hWaveOut,WAVE_MAPPER,m_pWfxPcm,(DWORD)0,0,CALLBACK_NULL);
	//if(rst)
	//{
	//	TRACE("输出设备打开失败!");
	//	return FALSE;
	//}

	rst = waveInPrepareHeader(m_hWaveIn,m_pInHeader1,sizeof(WAVEHDR));
	if(rst)
	{
		TRACE("准备输入缓冲失败");
		return FALSE;
	}
	rst = waveInAddBuffer (m_hWaveIn, m_pInHeader1, sizeof (WAVEHDR)) ;
	if(rst)
	{
		TRACE("添加输入缓冲失败");
		return FALSE;
	}

	rst = waveInPrepareHeader(m_hWaveIn,m_pInHeader2,sizeof(WAVEHDR));
	if(rst)
	{
		TRACE("准备输入缓冲失败");
		return FALSE;
	}
	rst = waveInAddBuffer (m_hWaveIn, m_pInHeader2, sizeof (WAVEHDR)) ;
	if(rst)
	{
		TRACE("添加输入缓冲失败");
		return  FALSE;
	}

	rst = waveOutPrepareHeader(m_hWaveOut,m_pOutHeader1,sizeof(WAVEHDR));
	if(rst)
	{
		TRACE("准备输出缓冲失败");
		return FALSE;
	}

	rst = waveOutPrepareHeader(m_hWaveOut,m_pOutHeader2,sizeof(WAVEHDR));
	if(rst)
	{
		TRACE("准备输出缓冲失败");
		return FALSE;
	}
	
	
	return TRUE;
}


void CMMAudio::StopRecord()
{
	if(m_hWaveIn)
	{
		waveInStop(m_hWaveIn);
		waveInClose(m_hWaveIn);
		m_hWaveIn = NULL;
	}
	if(m_hWaveOut)
	{
		waveOutClose(m_hWaveOut);
		m_hWaveOut = NULL;
	}

	m_bRecording = FALSE;
}

BOOL CMMAudio::StartRecord()
{
	if(m_hWaveIn)
		return FALSE;

	if(m_hWaveOut)
		return FALSE;

	if(Init() == FALSE)
		return FALSE;



	m_bRecording = TRUE;
	// 开始采样
	waveInStart (m_hWaveIn) ;
	return TRUE;
}

void CMMAudio::OnWaveInClose()
{
	waveInUnprepareHeader (m_hWaveIn, m_pInHeader1, sizeof (WAVEHDR)) ;
	waveInUnprepareHeader (m_hWaveIn, m_pInHeader2, sizeof (WAVEHDR)) ;
	m_bRecording = FALSE;
}


void CMMAudio::OnWaveData(WAVEHDR * pData)
{
	int nLen = pData->dwBytesRecorded;
	BYTE * pCmpData = NULL;
	// 压缩数据
	//pCmpData = CmpWaveData((BYTE *)pData->lpData,nLen);
	pCmpData = (BYTE *)pData->lpData;
	
	if(m_pCallBackCmpData && pCmpData)
	{
		m_pCallBackCmpData(pCmpData,nLen,m_dwUser);
	}
	::waveInAddBuffer(m_hWaveIn,pData,sizeof(WAVEHDR)); 
}


void CMMAudio::SetCapBufLen(int nLen)
{
	m_nCapBufLen = nLen;
}


int CMMAudio::PlaySound(const BYTE * pData,int nLen)
{
	
	if(m_hWaveOut == NULL)
	{
		Init();
	}
	static bool bOutBuf1 = true;

	int nPlay = nLen > m_nCmpBufLen ? m_nCmpBufLen : nLen	;
	MMRESULT rst;
	PWAVEHDR pWaveHdr;

	if(bOutBuf1)
		pWaveHdr = m_pOutHeader1;
	else
		pWaveHdr = m_pOutHeader2;
	bOutBuf1 = !bOutBuf1;
	memcpy(pWaveHdr->lpData,pData,nPlay);
	pWaveHdr->dwBytesRecorded = nPlay;
	//pWaveHdr->dwFlags = 0;
	rst = waveOutPrepareHeader(m_hWaveOut,pWaveHdr,sizeof(WAVEHDR));
	if(rst)
		return 0;
	m_bPlayOk = false;
	rst = waveOutWrite(m_hWaveOut,pWaveHdr,sizeof(WAVEHDR));
	if(rst)
		return 0;
	/*while(true)
	{
		if(m_bPlayOk)
		{
			break;
		}
		Sleep(10);
	}*/
	//if(WaitForSingleObject(m_eventPlay,INFINITE) != WAIT_OBJECT_0)
	//	return 0;
	
	return nPlay;
}

BYTE * CMMAudio::DecmpWaveData(BYTE * pData,int & nLen)
{
	MMRESULT mmr;
	HACMSTREAM hAcmStream = NULL;
	mmr = acmStreamOpen(&hAcmStream,m_had,m_pWfxCmp,m_pWfxPcm,NULL,(DWORD_PTR)0,0,ACM_STREAMOPENF_NONREALTIME);

	ACMSTREAMHEADER strhdr; //填充转换信息
	memset(&strhdr, 0, sizeof(strhdr));
	strhdr.cbStruct = sizeof(strhdr);
	strhdr.pbSrc = (BYTE*)pData; // 源数据
	strhdr.cbSrcLength = nLen;
	strhdr.pbDst = m_pDecmpData;
	strhdr.cbDstLength = m_nCapBufLen;
	memset(m_pDecmpData,0,m_nCmpBufLen);

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
	return m_pDecmpData;
}

BYTE * CMMAudio::CmpWaveData(BYTE * pData,int & nLen)
{
	if(nLen > m_nCapBufLen)
	{
		nLen = 0;
		return NULL;
	}
	MMRESULT mmr;
	HACMSTREAM hAcmStream1 = NULL,hAcmStream2 = NULL;
	HACMDRIVER had = NULL;

	// 压缩后的格式

	HACMDRIVERID id = FindAcmDriverID(WAVE_FORMAT_DSPGROUP_TRUESPEECH);
	MMRESULT rst;
	
	// src --> pcm
	rst = acmStreamOpen(&hAcmStream1,NULL,&m_waveformIn,m_pWfxPcm,NULL,(DWORD_PTR)0,0,ACM_STREAMOPENF_NONREALTIME);//CALLBACK_WINDOW
	// pcm --> cmp
	rst = acmStreamOpen(&hAcmStream2,had,m_pWfxPcm,m_pWfxCmp,NULL,(DWORD_PTR)0,0,ACM_STREAMOPENF_NONREALTIME);//CALLBACK_WINDOW
	

	ACMSTREAMHEADER strhdr; //填充转换信息
	memset(&strhdr, 0, sizeof(strhdr));

	// 先由源格式转为pcm格式 src->pcm
	strhdr.cbStruct = sizeof(strhdr);
	strhdr.pbSrc = (BYTE*)pData; // 源数据
	strhdr.cbSrcLength = nLen;
	strhdr.pbDst = m_pCmpBufPcm;
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


	// pcm格式到目的格式的压缩 pcm->src
	strhdr.cbStruct = sizeof(strhdr);
	strhdr.pbSrc = (BYTE*)m_pCmpBufPcm; // 源数据
	strhdr.cbSrcLength = strhdr.cbDstLengthUsed;
	strhdr.pbDst = m_pCmpBufDst;
	strhdr.cbDstLength = m_nCmpBufLen;

	mmr = acmStreamPrepareHeader(hAcmStream2, &strhdr, 0); 
	if(mmr)
		return NULL;
	mmr = acmStreamConvert(hAcmStream2, &strhdr, 0); // 转换数据
	if(mmr)
		return NULL;
	mmr = acmStreamUnprepareHeader(hAcmStream2,&strhdr,0);
	if(mmr)
		return NULL;
	mmr = acmStreamClose(hAcmStream2, 0);
	if(mmr)
		return NULL;
	//TRACE("压缩比为%4.3f\n",m_nCmpBufLen/(float)strhdr.cbDstLengthUsed);
	nLen = strhdr.cbDstLengthUsed;
	return m_pCmpBufDst;
}


void CMMAudio::SetCallBackOnCmpData(AudioCapDataFnc pFnc,DWORD dwUser)
{
	m_dwUser = dwUser;
	m_pCallBackCmpData = pFnc;
}

