#include "StdAfx.h"
#include "MMVideoCap.h"

// λͼ��fcc��
const DWORD BI_I420 = 0x30323449;	//
const DWORD BI_IYUV = 0x56555949;	//	
const DWORD BI_YV12 = 0x32315659;	// 

const DWORD BI_YUY2 = 0x32595559;	// 
const DWORD BI_UYVY = 0x59565955;	// 
const DWORD BI_YVYU = 0x55595659;	// 

inline void YUV2RGB(BYTE y,BYTE u,BYTE v,BYTE & r,BYTE & g,BYTE & b)
{
	int nR,nG,nB;

	nB = int(1.164*(y - 16) + 2.018*(u - 128));
	nG = int(1.164*(y - 16) - 0.813*(v - 128) - 0.391*(u - 128));
	nR = int(1.164*(y - 16) + 1.596*(v - 128));

	nR = nR * 220/250;
	nG = nG * 220/250;
	nB = nB * 220/250;

	if(nB < 0)
		b = 0;
	else if(nB > 255)
		b = 255;
	else
		b = nB;

	if(nG < 0)
		g = 0;
	else if(nG > 255)
		g = 255;
	else
		g = nG;	

	if(nR < 0)
		r = 0;
	else if(nR > 255)
		r = 255;
	else
		r = nR;
}



LRESULT CALLBACK ProcErrorCallback(HWND hWnd, int nErrID,LPCSTR lpErrorText)
{
	if (nErrID == 0) 
		return TRUE; 
	return (LRESULT) TRUE; 
};

LRESULT CALLBACK ProcStatusCallback(HWND hWnd, int nID,LPCSTR lpsz )
{
	// update���񴰵�״̬ 
	//capGetStatus(m_hWndCap, &m_capStatus,sizeof (CAPSTATUS)); 
	return 0;
};

LRESULT CALLBACK ProcFrameCallback(HWND hWnd, LPVIDEOHDR lpVHdr)
{
	static int i = 0;
	i++;
	lpVHdr->lpData;
	lpVHdr->dwBufferLength; 
	CMMVideoCap * pCap = (CMMVideoCap *)capGetUserData(hWnd);
	if(pCap)
	{
		pCap->ProcRawData(lpVHdr);
	}
	return 0;
}



LRESULT CALLBACK ProcVideoStreamCallback(
	HWND hWnd,         
	LPVIDEOHDR lpVHdr  
	)
{
	lpVHdr->lpData;
	lpVHdr->dwBufferLength; 
	CMMVideoCap * pCap = (CMMVideoCap *)capGetUserData(hWnd);
	if(pCap)
	{
		pCap->ProcRawData(lpVHdr);
	}
	return 0;
}




CMMVideoCodec::CMMVideoCodec()
{
	memset(&m_CV,0,sizeof(COMPVARS));
	m_CV.dwFlags = ICMF_COMPVARS_VALID;
	m_CV.cbSize = sizeof(m_CV);
	m_CV.cbState = 0;
	// HEKY_CURRENT_USER\Software\Intel\Indeo\5.0\QuickCompress
	m_CV.fccHandler = mmioFOURCC('I','V','5','0'); // IV50
	m_CV.fccType = ICTYPE_VIDEO;
	m_CV.hic = NULL;
	m_CV.lDataRate = 8;//780;	// ����Ƶ��kb/s
	m_CV.lFrame = 0;
	m_CV.lKey = 0;
	m_CV.lKeyCount = 0;
	m_CV.lpbiIn = NULL;
	m_CV.lpBitsOut = NULL;
	m_CV.lpBitsPrev = m_CV.lpState = NULL;
	m_CV.lQ = 5000;//50% ICQUALITY_DEFAULT;

	m_pBmpInfoRaw = NULL;
	m_pBmpInfoCmp = NULL;
	m_pBmpDeCmp = NULL;
	m_bInit = FALSE;
	m_pCmpData = NULL;
}

CMMVideoCodec::~CMMVideoCodec()
{
	Close();
}

void CMMVideoCodec::Close()
{
	m_xvid.destory();
	if(m_pBmpInfoCmp)
	{
		delete [] m_pBmpInfoCmp;
		m_pBmpInfoCmp = NULL;
	}
	if(m_pCmpData)
	{
		delete [] m_pCmpData;
		m_pCmpData = NULL;
	}

	if(m_pBmpDeCmp)
	{
		delete [] m_pBmpDeCmp;
		m_pBmpDeCmp = NULL;
	}
	if(m_pBmpInfoRaw)
	{
		delete [] m_pBmpInfoRaw;
		m_pBmpInfoRaw = NULL;
	}
	if(m_CV.hic)
	{
		if(m_bInit)
		{
			ICSeqCompressFrameEnd(&m_CV);
			ICDecompressEnd(m_CV.hic);
		}
		ICClose(m_CV.hic);
		m_CV.hic = NULL;
	}

}


BOOL CMMVideoCodec::Init(BITMAPINFO * pRawFormat,double dbQuality,DWORD fcc)
{
	if(pRawFormat == NULL)
		return FALSE;
	
	m_xvid.init(pRawFormat->bmiHeader.biWidth,pRawFormat->bmiHeader.biHeight);
	if(m_pBmpInfoRaw)
	{
		delete m_pBmpInfoRaw;
		m_pBmpInfoRaw = NULL;
	}

	//// ����Ϊ����ѹ����ʽ
	//HKEY hKey = NULL;
	//LONG nRs;
	//DWORD dwV = 1;
	//nRs = RegCreateKey(HKEY_CURRENT_USER,"Software\\Intel\\Indeo\\5.0",&hKey);
	//nRs = RegSetValueEx(hKey,"QuickCompress",0,REG_DWORD,(const BYTE*)&dwV,sizeof(DWORD));
	//nRs = RegCloseKey(hKey);


	// ���ƵĿ���
	int nBInfo = sizeof(BITMAPINFO) + 256 * 4;
	m_pBmpInfoRaw = (BITMAPINFO *)new BYTE[nBInfo];
	memset(m_pBmpInfoRaw,0,nBInfo);
	*m_pBmpInfoRaw = *pRawFormat;

	//m_CV.fccHandler = fcc;
	//SetCmpQuality(dbQuality);
	//m_CV.hic= ICOpen(ICTYPE_VIDEO,m_CV.fccHandler,ICMODE_COMPRESS);// ��Ƶѹ�������
	//if (m_CV.hic != NULL)
	//{
	//	// ��ȡѹ����ʽ�Ĵ�С
	//	int nOutFormatSize = ICCompressGetFormatSize(m_CV.hic,m_pBmpInfoRaw);
	//	if(nOutFormatSize > 0)
	//	{
	//		if(m_pBmpInfoCmp)
	//		{
	//			delete  m_pBmpInfoCmp;
	//			m_pBmpInfoCmp = NULL;
	//		}
	//		m_pBmpInfoCmp = (BITMAPINFO *)new BYTE[nOutFormatSize];
	//		// ���ѹ����ʽ��Ϣ
	//		ICCompressGetFormat(m_CV.hic,m_pBmpInfoRaw,m_pBmpInfoCmp);
	//		if (ICSeqCompressFrameStart(&m_CV,m_pBmpInfoRaw))
	//		{
	//			TRACE("ѹ����ʼ���ɹ�\n");
	//		}
	//		else
	//		{
	//			TRACE("ѹ����ʼ��ʧ��\n");
	//			return FALSE;
	//		}
	//		if(ICDecompressBegin(m_CV.hic,m_pBmpInfoCmp,m_pBmpInfoRaw)!=ICERR_OK)
	//		{
	//			TRACE("��ѹ��ʼ��ʧ�ܣ�\n");
	//			return FALSE;
	//		}
	//		else
	//		{
	//			TRACE("��ѹ��ʼ���ɹ���\n");
	//		}
	//	}
	//}
	//else
	//{
	//	return FALSE;
	//}

	if(m_pBmpDeCmp)
	{
		delete [] m_pBmpDeCmp;
		m_pBmpDeCmp = NULL;
	}
	if(m_pCmpData)
	{
		delete [] m_pCmpData;
		m_pCmpData = NULL;
	}
	m_pBmpDeCmp = new BYTE[m_pBmpInfoRaw->bmiHeader.biSizeImage];
	m_pCmpData = new BYTE[m_pBmpInfoRaw->bmiHeader.biSizeImage];
	m_bInit = TRUE;
	return TRUE;
}

BYTE * CMMVideoCodec::CmpFrame(BYTE * lpData,int & nSize)
{
	CMyLock lock(m_sc.Get());

	int nOutSize = 0;
	if(m_xvid.compress((char*)lpData,nSize,(char*)m_pCmpData,nOutSize))
	{
		nSize = nOutSize;
		return m_pCmpData;
	}
	else
		return NULL;
	
	//BOOL	bKeyFrame = FALSE;
	//long	nOutDataSize = m_pBmpInfoRaw->bmiHeader.biSizeImage;
	//// ѹ����Ƶ
	//BYTE* pCmpData =(BYTE*)ICSeqCompressFrame(
	//	&m_CV,
	//	0,
	//	lpData,
	//	&bKeyFrame,
	//	(long*)& nOutDataSize
	//	);
	//if(pCmpData)
	//	nSize = nOutDataSize;
	//else
	//	nSize = 0;
	//return pCmpData;
}

BYTE * CMMVideoCodec::DeCmpFrame( BYTE * pData,int nLen)
{
	CMyLock lock(m_sc.Get());
	int nOutSize = 0;
	if(m_xvid.deCompress((char*)pData,nLen,(char*)m_pBmpDeCmp,nOutSize))
		return m_pBmpDeCmp;
	else
		return NULL;
	/*if(pData != NULL)
	{
		DWORD dwRs;
		dwRs = ICDecompress(m_CV.hic,ICDECOMPRESS_HURRYUP,&m_pBmpInfoCmp->bmiHeader,pData,&m_pBmpInfoRaw->bmiHeader,m_pBmpDeCmp);
		if(dwRs != ICERR_OK)
		{
			TRACE("��ѹʧ�ܣ�\n");
			return NULL;
		}
		else
		{
			return m_pBmpDeCmp;
		}
	}
	return NULL;*/
}

void CMMVideoCodec::SetCmpQuality(double fQuality)
{
	int nQuality;
	if(fQuality > 100)
		nQuality = 10000;
	else if(fQuality < 0)
		nQuality = 0;
	else
		nQuality = (int)(fQuality * 100);
	m_CV.lQ = nQuality;
}







CMMVideoCap::CMMVideoCap()
{
	m_hWndCap = NULL;
	m_pBmpRGB24 = NULL;
	m_pBmpInfoSrc = NULL;
	m_pCallBackData = NULL;
	m_pBmpInfoCmp = NULL;
	

	m_dwUser = 0;
	m_nTimeSpan = 0;
	m_nFramePerSec = 10;

	m_nLastTick = 0;
	m_nTimeLeft = 0;

	memset(&m_bmpInfoRGB24,0,sizeof(BITMAPINFO));
	m_bmpInfoRGB24.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	m_bmpInfoRGB24.bmiHeader.biWidth = 320;
	m_bmpInfoRGB24.bmiHeader.biHeight = 240;
	m_bmpInfoRGB24.bmiHeader.biSizeImage = 320 * 240 * 3 ;
	m_bmpInfoRGB24.bmiHeader.biBitCount = 24;
	m_bmpInfoRGB24.bmiHeader.biPlanes = 1;

	//m_pBmpDeCmp = NULL;
	//memset(&m_CV,0,sizeof(COMPVARS));
	//m_CV.dwFlags=ICMF_COMPVARS_VALID;
	//m_CV.cbSize=sizeof(m_CV);
	//m_CV.cbState=0;
	//m_CV.fccHandler=mmioFOURCC('I','V','5','0'); // IV50
	//m_CV.fccType=ICTYPE_VIDEO;
	//m_CV.hic= ICOpen(ICTYPE_VIDEO,m_CV.fccHandler,ICMODE_COMPRESS);// ��Ƶѹ�������
	//m_CV.lDataRate = 8;//780;	// ����Ƶ��kb/s
	//m_CV.lFrame= 0;
	//m_CV.lKey= 0;
	//m_CV.lKeyCount=0;
	//m_CV.lpbiIn=NULL;
	//m_CV.lpBitsOut=NULL;
	//m_CV.lpBitsPrev=m_CV.lpState=NULL;
	//m_CV.lQ = 5000;//ICQUALITY_DEFAULT;50%

}

CMMVideoCap::~CMMVideoCap()
{
	Close();
	if(m_pBmpRGB24)
	{
		delete [] m_pBmpRGB24;
		m_pBmpRGB24 = NULL;
	}
	if(m_pBmpInfoSrc)
	{
		delete [] m_pBmpInfoSrc;
		m_pBmpInfoSrc = NULL;
	}
	if(m_pBmpInfoCmp)
	{
		delete [] m_pBmpInfoCmp;
		m_pBmpInfoCmp = NULL;
	}
	/*if(m_pBmpDeCmp)
	{
		delete [] m_pBmpDeCmp;
		m_pBmpDeCmp = NULL;
	}*/
}

// 4:1:1 YUVƽ��
void CMMVideoCap::ConvertPlanarYUV12(BITMAPINFO * pBmi,BYTE * pIn,BYTE * pOut,bool bUV)
{
	int nWidth = pBmi->bmiHeader.biWidth;
	int nHeight = pBmi->bmiHeader.biHeight;
	int nW2 = nWidth / 2;
	int i,j;
	BYTE * pRGB = pOut + nWidth * nHeight * 3 ;
	BYTE * pY = pIn;
	BYTE * pU; 
	BYTE * pV;
	if(bUV)
	{
		pU = pIn + nWidth * nHeight;
		pV = pU + nWidth * nHeight /4;
	}
	else
	{
		pV = pIn + nWidth * nHeight;
		pU = pV + nWidth * nHeight /4;
	}

	int nPix = 0;
	for(i = 0; i < nHeight ;i++)
	{
		pY += nWidth;
		pRGB -= nWidth * 3;
		nPix = 0;
		for(j = 0; j < nWidth ;j++)
		{
			YUV2RGB(
				pY[j],
				pU[i/2 * nW2 + j / 2],
				pV[i/2 * nW2 + j / 2],
				pRGB[nPix + 2],
				pRGB[nPix + 1],
				pRGB[nPix]);
			nPix += 3;
		}
	}

}


// Y0 U0 Y1 V0
void CMMVideoCap::ConvertPackedYUV16(BITMAPINFO * pBmi,BYTE * pIn,BYTE * pOut)
{
	//int nWidth = pBmi->bmiHeader.biWidth;
	//int nHeight = pBmi->bmiHeader.biHeight;
	//BYTE * pRGB = pOut + nWidth * nHeight * 3;
	//int nPix = 0;
	//BYTE y0,y1,u,v;
	//BYTE b1,b2;
	//int i,j;
	//int nIdx = 0;
	//int nC = nWidth / 2;

	//for(i = 0; i < nHeight ;i++)
	//{
	//	pRGB -= nWidth * 3;
	//	nPix = 0;
	//	for(j = 0; j < nC ;j++)
	//	{
	//		b1 = pIn[nIdx++];
	//		b2 = pIn[nIdx++];
	//		y0 = b1 << 4;
	//		u = b1 & 0xf0;
	//		y1 = b2 << 4;
	//		v = b2 & 0xf0;

	//		YUV2RGB(
	//			y0,
	//			u,
	//			v,
	//			pRGB[nPix + 2],
	//			pRGB[nPix + 1],
	//			pRGB[nPix]);

	//		YUV2RGB(
	//			y1,
	//			u,
	//			v,
	//			pRGB[nPix + 5],
	//			pRGB[nPix + 4],
	//			pRGB[nPix + 3]);
	//		nPix += 6;
	//	}
	//}
	int nWidth = pBmi->bmiHeader.biWidth;
	int nHeight = pBmi->bmiHeader.biHeight;
	BYTE * pRGB= pOut  + nWidth * nHeight * 3;
	int nPix = 0;
	int i,j;
	int nIdx = 0;
	int nC = nWidth / 2;

	for(i = 0; i < nHeight ;i++)
	{
		pRGB -= nWidth * 3;
		nPix = 0;
		for(j = 0; j < nC ;j++)
		{
			YUV2RGB(
				pIn[0],
				pIn[1],
				pIn[3],
				pRGB[nPix + 2],
				pRGB[nPix + 1],
				pRGB[nPix]
				);

				YUV2RGB(
					pIn[2],
					pIn[1],
					pIn[3],
					pRGB[nPix + 5],
					pRGB[nPix + 4],
					pRGB[nPix + 3]
					);
					;
					nPix += 6;
					pIn += 4;
		}
	}

}



void CMMVideoCap::Convert2RGB24(BITMAPINFO * pBmi,BYTE * pIn,BYTE * pOut)
{
	switch(pBmi->bmiHeader.biCompression)
	{
	case BI_I420:
	case BI_IYUV:
		ConvertPlanarYUV12(pBmi,pIn,pOut,true);
		break;
	case BI_YV12:
		ConvertPlanarYUV12(pBmi,pIn,pOut,false);
		break;
	case BI_YUY2:
		ConvertPackedYUV16(pBmi,pIn,pOut);
		break;
	case BI_UYVY:
		break;
	case BI_YVYU:
		break;
	default :
		break;
	};
}

//BYTE * CMMVideoCap::DeCmpFrame(BYTE * pData)
//{
//	if(pData != NULL)
//	{
//		DWORD dwRs;
//		dwRs = ICDecompress(m_CV.hic,ICDECOMPRESS_HURRYUP,&m_bmpInfoRGB24.bmiHeader,pData,&m_bmpInfoRGB24.bmiHeader,m_pBmpDeCmp);
//		if(dwRs != ICERR_OK)
//		{
//			TRACE("��ѹʧ�ܣ�\n");
//			return NULL;
//		}
//		else
//		{
//			return m_pBmpDeCmp;
//		}
//	}
//	return NULL;
//}
//
//
//BYTE * CMMVideoCap::CmpFrame(BYTE * lpData,int & nSize)
//{	
//	BOOL	bKeyFrame = FALSE;
//	long	nOutDataSize = m_bmpInfoRGB24.bmiHeader.biSizeImage;
//	nSize = 0;
//	// ѹ����Ƶ
//	BYTE* pCmpData =(BYTE*)ICSeqCompressFrame(
//		&m_CV,
//		0,
//		lpData,
//		&bKeyFrame,
//		(long*)& nOutDataSize
//	);
//	if(pCmpData)
//		nSize = nOutDataSize;
//	return pCmpData;
//}

void CMMVideoCap::ProcRawData(LPVIDEOHDR pVHdr)
{
	int nCurTick = GetTickCount();
	int nSpan = nCurTick - m_nLastTick;

	if(nSpan + m_nTimeLeft >= m_nTimeSpan)
	{
		//TRACE("bmp %d\n",nSpan);
		m_nTimeLeft = nSpan + m_nTimeLeft - m_nTimeSpan;
		m_nLastTick = nCurTick;
		if(m_nTimeLeft >= m_nTimeSpan)
			m_nTimeLeft = m_nTimeSpan / 2;
	}
	else
	{
		return;
	}
	
	

	if(m_pBmpInfoSrc == NULL || m_pBmpRGB24 == NULL)
	{
		TRACE("exception!\n");
		return;
	}

	if(m_pCallBackData == NULL)
		return;

	BYTE * pBmpData;
	int nOutSize = 0;
	// �ж��Ƿ���Ҫת����ʽΪRGB24
	if(m_pBmpInfoSrc->bmiHeader.biBitCount == 24)
	{
		pBmpData = pVHdr->lpData;
		nOutSize = pVHdr->dwBytesUsed;
	}
	else
	{
		Convert2RGB24(m_pBmpInfoSrc,pVHdr->lpData,m_pBmpRGB24);
		pBmpData = m_pBmpRGB24;
		nOutSize =  m_bmpInfoRGB24.bmiHeader.biSizeImage;
	}
	if(nOutSize > 1024 * 1024 * 4)
	{
		assert(0 && "�ɼ�����Ƶͼ���С�쳣��");
		return;
	}

	if(m_pCallBackData)
	{
		m_pCallBackData(pBmpData,nOutSize,nSpan,m_dwUser);
	}
}

void CMMVideoCap::SetCallBackOnData(VideoCapDataFnc pFnc,DWORD dwUser)
{
	m_pCallBackData = pFnc;
	m_dwUser = dwUser;
}

BOOL CMMVideoCap::InitCap(const CRect & rcCapWnd,HWND hWndParent,int nCapIdx)
{
	BOOL bRt;
	if(m_hWndCap)
		return FALSE;

	//��������,����hWndΪ�����ھ�� 
	m_hWndCap = capCreateCaptureWindow(
		(LPSTR)"Capture Window",
		WS_CHILD | WS_VISIBLE,
		rcCapWnd.left, 
		rcCapWnd.top, 
		rcCapWnd.Width(),
		rcCapWnd.Height(),
		(HWND) hWndParent, 
		(int) 0); 
	// �Ǽ������ص�����
	bRt = capSetCallbackOnError(m_hWndCap, (FARPROC)ProcErrorCallback); 
	bRt = capSetCallbackOnStatus(m_hWndCap, (FARPROC)ProcStatusCallback); 
	//bRt = capSetCallbackOnFrame(m_hWndCap, (FARPROC)ProcFrameCallback); 
	bRt = capSetCallbackOnVideoStream(m_hWndCap,(FARPROC)ProcVideoStreamCallback);

	DWORD dwData = (DWORD)this;
	// �����û�����
	bRt = capSetUserData(m_hWndCap,dwData);

	// ������ͬ���������� 
	bRt = capDriverConnect(m_hWndCap,nCapIdx); 

	// ���������������,��ص���Ϣ���ڽṹ����
	bRt = capDriverGetCaps(m_hWndCap,&m_capDriverCaps,sizeof(CAPDRIVERCAPS)) ;

	CAPTUREPARMS cparam;
	bRt = capCaptureGetSetup(m_hWndCap,&cparam,sizeof(cparam));
	cparam.fCaptureAudio = FALSE;
	cparam.dwRequestMicroSecPerFrame = 100 * 1000;
	m_nTimeSpan = cparam.dwRequestMicroSecPerFrame / 1000;
	cparam.fAbortLeftMouse = FALSE;
	cparam.fAbortRightMouse = FALSE;
	cparam.fYield = TRUE;
	bRt = capCaptureSetSetup(m_hWndCap,&cparam,sizeof(cparam));


	// ������ƵԴ��ʽ
	if(m_pBmpInfoSrc)
	{
		delete [] m_pBmpInfoSrc;
		m_pBmpInfoSrc = NULL;
	}
	int nSrcFormatSize = capGetVideoFormatSize(m_hWndCap);
	if(nSrcFormatSize > 0)
	{
		m_pBmpInfoSrc = (BITMAPINFO *)new BYTE[nSrcFormatSize];
		memset(m_pBmpInfoSrc,0,nSrcFormatSize);
		memset(&m_bmpInfoRGB24,0,sizeof(m_bmpInfoRGB24));
	}
	else
	{
		return FALSE;
	}

	// ��ȡ��Ƶ��ʽ
	DWORD dwRt = capGetVideoFormat(m_hWndCap,m_pBmpInfoSrc,nSrcFormatSize);

	m_bmpInfoRGB24 = *m_pBmpInfoSrc;
	m_bmpInfoRGB24.bmiHeader.biWidth = 320;
	m_bmpInfoRGB24.bmiHeader.biHeight = 240;
	m_bmpInfoRGB24.bmiHeader.biSizeImage = 320 * 240 * m_bmpInfoRGB24.bmiHeader.biBitCount / 8 ;
	// ����Ϊ320*240
	bRt = capSetVideoFormat(m_hWndCap,&m_bmpInfoRGB24,m_bmpInfoRGB24.bmiHeader.biSize);
	if(dwRt)
	{
		
	}
	// ��ȡ���ú����Ƶ��ʽ
	dwRt = capGetVideoFormat(m_hWndCap,m_pBmpInfoSrc,nSrcFormatSize);
	
	// �����ʽΪRGB24
	m_bmpInfoRGB24 = *m_pBmpInfoSrc;
	m_bmpInfoRGB24.bmiHeader.biBitCount = 24;
	m_bmpInfoRGB24.bmiHeader.biSizeImage = m_bmpInfoRGB24.bmiHeader.biWidth * m_bmpInfoRGB24.bmiHeader.biHeight * 3;
	m_bmpInfoRGB24.bmiHeader.biCompression = BI_RGB;

	if(m_pBmpRGB24)
	{
		delete [] m_pBmpRGB24;
		m_pBmpRGB24 = NULL;
	}
	m_pBmpRGB24 = new BYTE[m_bmpInfoRGB24.bmiHeader.biSizeImage];
	
	/*if(m_pBmpDeCmp)
	{
		delete [] m_pBmpDeCmp;
		m_pBmpDeCmp = NULL;
	}
	m_pBmpDeCmp = new BYTE[m_bmpInfoRGB24.bmiHeader.biSizeImage];*/


	/*bRt = ICCompressorChoose(m_hWndCap,
		ICMF_CHOOSE_ALLCOMPRESSORS|ICMF_CHOOSE_KEYFRAME|ICMF_CHOOSE_DATARATE|ICMF_CHOOSE_PREVIEW
		,NULL,NULL,&m_CV,"ok");
	if(bRt)
	{
		TRACE("select ok\n");
	}*/

	//if (m_CV.hic != NULL)
	//{

	//	int nOutFormatSize = ICCompressGetFormatSize(m_CV.hic,&m_bmpInfoRGB24);
	//	if(nOutFormatSize > 0)
	//	{
	//		if(m_pBmpInfoCmp)
	//		{
	//			delete [] m_pBmpInfoCmp;
	//			m_pBmpInfoCmp = NULL;
	//		}
	//		m_pBmpInfoCmp = (BITMAPINFO *)new BYTE[nOutFormatSize];
	//		// ��������ʽ
	//		ICCompressGetFormat(m_CV.hic,&m_bmpInfoRGB24,m_pBmpInfoCmp);
	//		if (ICSeqCompressFrameStart(&m_CV,&m_bmpInfoRGB24))
	//		{
	//			TRACE("ѹ����ʼ���ɹ�\n");
	//		}
	//		else
	//		{
	//			TRACE("ѹ����ʼ��ʧ��\n");
	//			return FALSE;
	//		}
	//		if(ICDecompressBegin(m_CV.hic,m_pBmpInfoCmp,&m_bmpInfoRGB24)!=ICERR_OK)
	//		{
	//			TRACE("��ѹ��ʼ��ʧ�ܣ�\n");
	//			return FALSE;
	//		}
	//		else
	//		{
	//			TRACE("��ѹ��ʼ���ɹ���\n");
	//		}
	//	}
	//}

	//bRt = capPreviewScale(m_hWndCap,TRUE);
	//bRt = capPreviewRate(m_hWndCap, 300); // ����Previewģʽ����ʾ����,Ĭ��ÿ��3.3֡
	//bRt = capPreview(m_hWndCap, TRUE); //����Previewģʽ 

	

	return TRUE;
}

void CMMVideoCap::Start(int nFramePerSec)
{
	m_nTimeLeft = 0;
	BOOL bRt;
	CAPTUREPARMS cparam;
	if(nFramePerSec <= 5)
	{
		nFramePerSec = 5;
	}
	else if(nFramePerSec >= 25)
	{
		nFramePerSec = 25;
	}
	m_nTimeSpan = 1000 / nFramePerSec;
	//m_nTimeSpan = 50;
	bRt = capCaptureGetSetup(m_hWndCap,&cparam,sizeof(cparam));
	cparam.dwRequestMicroSecPerFrame = m_nTimeSpan * 1000;
	bRt = capCaptureSetSetup(m_hWndCap,&cparam,sizeof(cparam));
	bRt = capPreviewScale(m_hWndCap,TRUE);
	//bRt = SetFramePerSec(m_nFramePerSec); // ����Previewģʽ����ʾ����,Ĭ��ÿ��10֡
	//bRt = capPreviewRate( m_hWndCap, m_nTimeSpan);
	bRt = capPreview(m_hWndCap, TRUE); //����Previewģʽ 
	capCaptureSequenceNoFile(m_hWndCap);
	//capCaptureSequence(m_hWndCap);
	m_nLastTick = GetTickCount();
}


void CMMVideoCap::Close()
{
	BOOL bRt;

	if ( m_hWndCap )
	{		
		try
		{
			bRt = capCaptureStop(m_hWndCap);
		}
		catch(...)
		{
		}

		try
		{
			bRt = capPreview(m_hWndCap,FALSE);
		}
		catch(...)
		{
		}

		try
		{
			bRt = capDriverDisconnect(m_hWndCap);
		}
		catch(...)
		{
			TRACE("�쳣���Ͽ���Ƶ�豸!!\n");
		}

		try
		{
			//::SendMessage(m_hWndCap,WM_DESTROY,0,0);
			bRt = ::DestroyWindow(m_hWndCap);
		}
		catch(...)
		{
		}
	}

	m_hWndCap = NULL;
}

void CMMVideoCap::Preview()
{
	BOOL bRt;
	bRt = capPreview(m_hWndCap, TRUE); //����Previewģʽ
}


BITMAPINFO * CMMVideoCap::GetBmpInfo()
{
	return &m_bmpInfoRGB24;
}

//void CMMVideoCap::SetCmpQuality(double fQuality)
//{
//	int nQuality;
//	if(fQuality > 100)
//		nQuality = 10000;
//	else if(fQuality < 0)
//		nQuality = 0;
//	else
//		nQuality = int(fQuality * 100);
//	m_CV.lQ = nQuality;
//}

void CMMVideoCap::ShowVideoFormatDlg()
{
	if (m_capDriverCaps.fHasDlgVideoFormat) 
		capDlgVideoFormat(m_hWndCap);
}

//BOOL CMMVideoCap::SetFramePerSec(DWORD nC)
//{
//	int nRate;
//	BOOL bRt;
//	if(nC == 0)
//		nC = 1;
//	nRate = 1000 / nC;
//	// ������ÿ��30֡����ÿ��1֡
//	if(nRate <= 33)
//		nRate = 33;
//	else if(nRate > 1000)
//		nRate = 1000;
//	bRt = capPreviewRate(m_hWndCap, nRate); // ����Previewģʽ����ʾ���� 
//	m_nTimeSpan = nRate;
//	return bRt;
//}
