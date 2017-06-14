// im\WebFaceDlg.cpp : 实现文件
//
// im\MagicFaceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteArticle.h"
#include ".\webpicdlg.h"
#include "WebPictureProcess.h"
#include "WebDataProcess.h"
#include "WebInterfaceWrapper.h"
// CWebPicDlg 对话框

CWebPicDlg * CWebPicDlg::pInstance = NULL;

IMPLEMENT_DYNAMIC(CWebPicDlg, CDialog)
CWebPicDlg::CWebPicDlg(CWnd* pParent /*=NULL*/)
: CDialog(CWebPicDlg::IDD, pParent),
m_myPictures(g_pUserInfo->GetUserID())
{
	m_hPicSelWnd    = NULL;
	m_bWaittingFlag = false;
	m_iPicturePosition = 0;
	m_nFocusPictureID   = -1;
	m_iHotPicture       = -1;
	m_bmpPicFrame.LoadBitmap(IDB_PICTURE_FRAME);
	m_bmpPicPig.LoadBitmap(IDB_PICTURE_FLAG);
	m_bmpError.LoadBitmap(IDB_PICTURE_ERROR);

	Create(IDD,pParent);
}

CWebPicDlg::~CWebPicDlg()
{
	ClearBitmaps();
	pInstance=NULL;
}

void CWebPicDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BUTTON_LEFT,m_btnLeft);
	DDX_Control(pDX,IDC_BUTTON_RIGHT,m_btnRight);
	DDX_Control(pDX, IDC_SLIDER1, m_wndSlider);
}


BEGIN_MESSAGE_MAP(CWebPicDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_NCDESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_ACTIVATE()
	ON_WM_HSCROLL()
	ON_MESSAGE(WM_WEBPICTURE_NOTIFY,OnWebPictureNotify)
	ON_MESSAGE(WM_WEBDATA_NOTIFY,OnWebDataNotify)
	ON_MESSAGE(WM_THREAD_NOTIFY,OnThreadNotify)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT, OnBnClickedButtonRight)
	ON_BN_CLICKED(IDC_BUTTON_LEFT, OnBnClickedButtonLeft)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CWebPicDlg 消息处理程序
void CWebPicDlg::Show(int x, int y, HWND hWndSel)
{
	int screen_cx = GetSystemMetrics(SM_CXSCREEN);
	int screen_cy = GetSystemMetrics(SM_CYSCREEN)-25;

	int cx=490;
	int cy=130;

	if ( (x+cx)>screen_cx ) 
		x = screen_cx-cx;

	if ( (y+cy)>screen_cy )
		y = screen_cy-cy;

	if ( pInstance == NULL )
	{
		pInstance = new CWebPicDlg(GetDesktopWindow());
		pInstance->m_hPicSelWnd = hWndSel;
	}
	else
	{
		pInstance->m_hPicSelWnd = hWndSel;
	}

	pInstance->MoveWindow(x,y,cx,cy);	
	pInstance->ShowWindow(SW_SHOW);
	pInstance->AdjustUI();
	pInstance->BringWindowToTop();

}

void CWebPicDlg::Destory()
{
	if ( pInstance )
	{
		pInstance->DestroyWindow();
	}
}

void CWebPicDlg::OnClose()
{
	DestroyWindow();
}

void CWebPicDlg::OnNcDestroy()
{
	SafeTerminateThreads();
	CDialog::OnNcDestroy();
	delete this;
}

BOOL CWebPicDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ModifyStyleEx(0,WS_EX_TOOLWINDOW);	

	m_btnRight.SetStyle(0);
	m_btnRight.SetStyle(1);

	m_wndSlider.SetBackColor(RGB(255,255,255));
	m_wndSlider.SetRange(0,0);
	m_wndSlider.SetPos(0);

	EnableToolTips();

	InitToolTip();
	InitPictures();

	return TRUE; 
}

void CWebPicDlg::OnKillFocus(CWnd* pNewWnd)
{
	ShowWindow(SW_HIDE);
}

BOOL CWebPicDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(&rect,0xFFFFFF);
	pDC->Draw3dRect(&rect,IM_LINE_COLOR,IM_LINE_COLOR);
	return FALSE;
}

void CWebPicDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}

void CWebPicDlg::AdjustUI()
{
	m_ListRect.SetRect(22,3,470,103);
	m_btnLeft.MoveWindow(5,10,13,90);
	m_btnRight.MoveWindow(472,10,13,90);
	m_wndSlider.MoveWindow(5,105,480,20);
}

void CWebPicDlg::OnPaint()
{
	CPaintDC dc(this); 
	DrawList(&dc);
}

void CWebPicDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	if ( nState==WA_INACTIVE )
	{
		ShowWindow(SW_HIDE);
	}
}

void CWebPicDlg::InitPictures()
{
	g_spaceData.LoadUserWebPictures(g_pUserInfo->GetUserID(),m_myPictures);
	g_webDataProcess.GetWebPictureList(UIntToStr(g_pUserInfo->GetUserID()),m_myPictures.szLastUpdateDate,"",this);   

	m_bWaittingFlag = true;

	int n = m_myPictures.vecPics.size();
	m_nDownloadThreadRef = 0;

	int i;
	for ( i=0; i<5; i++ )
	{
		if ( i<n )
		{
			g_webPictureProcess.GetWebPicture(g_pUserInfo->GetUserID(),*(m_myPictures.vecPics[i]),PICTURE_CX,this);
			m_nDownloadThreadRef++;
		}
	}

	this->m_fifoPics.clear();

	n--;
	while(n>=5)
	{
		m_fifoPics.Push(m_myPictures.vecPics[n]);
		n--;
	}
}

LRESULT CWebPicDlg::OnWebDataNotify(WPARAM wParam, LPARAM lParam)
{
	CWebDataProcess::tagThreadResult * result = (CWebDataProcess::tagThreadResult *)lParam;

	if ( wParam==0 )
	{        
		vector<TWebPictureInfo *> info;
		CString strServerDate;

		ParaseWebPictures(result->strWebResult,info,strServerDate);

		uint32 fid = atol((LPCSTR)result->strID);		

		if ( fid==g_pUserInfo->GetUserID() )
		{
			strncpy(m_myPictures.szLastUpdateDate,(LPCSTR)strServerDate,sizeof(m_myPictures.szLastUpdateDate)-1);

			int n = info.size();

			if ( m_fifoPics.size()>0 )
			{
				n--;
				while (n>=0)
				{
					m_fifoPics.push_back(info[n]);
					n--;
				}
			}
			else
			{
				int i,c=0;

				for ( i=0; i<n; i++ )
				{
					int nItem = m_myPictures.FindPicture(info[i]->dwPictureID);
					if ( nItem==-1 )
					{
						m_myPictures.vecPics.push_back(info[i]);

						if ( ++c<5 )
						{
							g_webPictureProcess.GetWebPicture(fid/*info[i].uID*/,*(info[i]),80,this);	
							m_nDownloadThreadRef++;
						}
						else
							m_fifoPics.push_front(info[i]);
					}
					else
					{
						delete m_myPictures.vecPics[nItem];
						m_myPictures.vecPics[nItem]=info[i];					
					}					
				}
			}          

			if (info.size()>0 )
			{
				g_spaceData.SaveUserWebPictures(m_myPictures);
			}

		}


		if ( m_myPictures.vecPics.size()==0 )
			m_bWaittingFlag = false;
	}

	return 0;
}

LRESULT CWebPicDlg::OnThreadNotify(WPARAM wParam, LPARAM lParam)
{
	ReDrawList();
	UpdateButtonState();

	m_wndSlider.SetRange(0,m_vecBitmaps.size());
	return 0;
}

LRESULT CWebPicDlg::OnWebPictureNotify(WPARAM wParam, LPARAM lParam)
{	
	switch ( wParam )
	{
	case 1:
		{
			CWebPictureProcess::tagThreadResult * result = (CWebPictureProcess::tagThreadResult *)lParam;

			if ( result->iSize==PICTURE_CX )
			{
				tagThreadParam *p = new tagThreadParam;

				p->pic_id  = result->pic_id;
				p->dwCreateDate = result->dwCreateDate;
				strcpy(p->szFile,result->szLocalFile);

				DWORD dwThreadId;
				p->hThread = CreateThread( NULL,
					0,
					(LPTHREAD_START_ROUTINE)LoadBitmapProc,
					(LPVOID)p,
					0,
					&dwThreadId );	

				m_vecThreads.push_back(p->hThread);

				m_nDownloadThreadRef--;

				TWebPictureInfo * wi;
				if ( m_fifoPics.Pop(wi) )
				{
					m_nDownloadThreadRef++;
					g_webPictureProcess.GetWebPicture(g_pUserInfo->GetUserID(),*wi,PICTURE_CX,this);
				}

				if ( m_nDownloadThreadRef==0 )
				{
					
					//SetLabelPictureNum(m_vecBitmaps.size());
				}
			}			
		}
		break;
	case 2://download failed
		{
			CWebPictureProcess::tagThreadResult * result = (CWebPictureProcess::tagThreadResult *)lParam;

			if ( result->iSize==PICTURE_CX )
			{
				TWebPictureInfo * wi;
				m_nDownloadThreadRef--;


				tagPicture pic;
				pic.pBitmap = NULL;
				pic.pic_id  = result->pic_id;
				pic.dwCreateDate = result->dwCreateDate;
				m_vecBitmaps.push_back(pic);

				if ( m_fifoPics.Pop(wi) )
				{
					m_nDownloadThreadRef++;
					g_webPictureProcess.GetWebPicture(g_pUserInfo->GetUserID(),*wi,PICTURE_CX,this);
				}

				if ( m_nDownloadThreadRef==0 )
				{
					//SetLabelPictureNum(m_vecBitmaps.size());
				}
			}
		}
		break;	
	}

	return 0;
}

UINT CWebPicDlg::LoadBitmapProc(LPVOID p)
{
	tagThreadParam *param = (tagThreadParam *)p;

	CBitmap * pBitmap = ::CBitmapFromFile(param->szFile,PICTURE_CX,PICTURE_CY);

	if ( pBitmap )
	{
		tagPicture p;
		p.pBitmap = pBitmap;
		p.pic_id  = param->pic_id;
		p.dwCreateDate = param->dwCreateDate;

		pInstance->m_secBitmaps.Lock();
		pInstance->m_vecBitmaps.push_back(p);
		pInstance->m_secBitmaps.Unlock();

		//std::sort(pInstance->m_vecBitmaps.begin(),pInstance->m_vecBitmaps.end(),compare_tagPicture);

		pInstance->SendMessage(WM_THREAD_NOTIFY,0,(LPARAM)param);//refresh
	}

	pInstance->m_secThreads.Lock();
	int n = pInstance->m_vecThreads.size();

	for ( int i=0; i<n; i++ )
	{
		if ( param->hThread == pInstance->m_vecThreads[i] )
		{
			CloseHandle(param->hThread);
			pInstance->m_vecThreads.erase(pInstance->m_vecThreads.begin()+i);
			break;
		}
	}

	pInstance->m_secThreads.Unlock();

	delete param;

	return 0;
}

void CWebPicDlg::SafeTerminateThreads()
{
	m_secThreads.Lock();
	int n= m_vecThreads.size();
	for ( int i=0; i<n; i++ )
	{
		try
		{
			TerminateThread(m_vecThreads[i],0);
			CloseHandle(m_vecThreads[i]);
		}
		catch(...)
		{
		}
	}
	m_secThreads.Unlock();
}

void CWebPicDlg::ClearBitmaps()
{
	int n = m_vecBitmaps.size();
	for ( int i=0; i<n; i++ )
	{
		delete m_vecBitmaps[i].pBitmap;
	}
	m_vecBitmaps.clear();
}

void CWebPicDlg::ReDrawList()
{
	CDC *pdc = GetDC();
	if ( pdc )
	{
		DrawList(pdc);
		ReleaseDC(pdc);
	}
}
void CWebPicDlg::DrawList(CDC *pDC)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pDC);

	int cx  = m_ListRect.Width();
	int cy  = m_ListRect.Height();

	int nSavedMemDC  = dcMem.SaveDC();
	int nSavedTempDC = dcTemp.SaveDC();

	CBitmap bmMem;
	bmMem.CreateCompatibleBitmap(pDC,cx,cy);
	dcMem.SelectObject(&bmMem);

	dcMem.FillSolidRect(0,0,cx,cy,0xFFFFFF);

	int nBitmaps     = m_vecBitmaps.size();
	int nDrawBitmaps = cx/(PICTURE_CX+PICTURE_SPACE);

	int nTop         = (cy-PICTURE_CY)/2+1;

	int nOffset      = 0;

	for ( int i=0; i<(nDrawBitmaps+2); i++ )
	{
		int iPicture = i + m_iPicturePosition;
		int iPictureLeft = i*(PICTURE_CX+PICTURE_SPACE);

		if ( iPicture>=0 && iPicture<=nBitmaps )
		{
			CBitmap *pBitmap;

			if ( iPicture==nBitmaps )
			{
				pBitmap = &m_bmpPicPig;
			}
			else
			{
				pBitmap = m_vecBitmaps[iPicture].pBitmap;
			}

			if ( pBitmap==NULL )
				pBitmap=&m_bmpError;

			dcTemp.SelectObject(&m_bmpPicFrame);
			dcMem.BitBlt(iPictureLeft,nTop,FRAME_SZ,FRAME_SZ,&dcTemp,0,0,SRCCOPY);
			dcTemp.SelectObject(pBitmap);
			dcMem.BitBlt(iPictureLeft+1,nTop+1,PICTURE_CX,PICTURE_CY,&dcTemp,0,0,SRCCOPY);

			CRect rcLine;
			rcLine.left   = iPictureLeft+1+nOffset;
			rcLine.top    = nTop;
			rcLine.right  = rcLine.left+PICTURE_CX;
			rcLine.bottom = rcLine.top+PICTURE_CY;

			if ( iPicture<nBitmaps )
			{
				DWORD dwPicID = m_vecBitmaps[iPicture].pic_id;

				if ( dwPicID==m_nFocusPictureID )
				{
					dcMem.Draw3dRect(&rcLine,RGB(255,0,0),RGB(255,0,0));
				}

				if ( iPicture == m_iHotPicture )
				{
					dcMem.Draw3dRect(&rcLine,RGB(0,0,255),RGB(0,0,255));
				}
			}
		}

	}

	pDC->BitBlt(m_ListRect.left,m_ListRect.top,cx,cy,&dcMem,0,0,SRCCOPY);
	dcMem.RestoreDC(nSavedMemDC);
	dcTemp.RestoreDC(nSavedTempDC);
}

void CWebPicDlg::OnBnClickedButtonRight()
{
	int nBitmaps     = m_vecBitmaps.size();
	int nDrawBitmaps = m_ListRect.Width()/(PICTURE_CX+PICTURE_SPACE);

	int nScrolls     = 0;

	int iPrePositon  = m_iPicturePosition;
	int iCurPositon  = m_iPicturePosition+nDrawBitmaps;

	if ( iCurPositon>=nBitmaps )
		iCurPositon=nBitmaps-nDrawBitmaps;

	nScrolls = iCurPositon-iPrePositon;

	if ( nScrolls>0 )
	{
		while ( nScrolls!=0 )
		{
			m_iPicturePosition++;
			nScrolls--;
			ReDrawList();
			Sleep(20);
		}
	}

	UpdateButtonState();
}

void CWebPicDlg::OnBnClickedButtonLeft()
{
	int nBitmaps     = m_vecBitmaps.size();
	int nDrawBitmaps = m_ListRect.Width()/(PICTURE_CX+PICTURE_SPACE);

	int nScrolls     = 0;

	int iPrePositon  = m_iPicturePosition;
	int iCurPositon  = m_iPicturePosition-nDrawBitmaps;

	if ( iCurPositon<0 )
		iCurPositon=0;

	nScrolls = iPrePositon-iCurPositon;

	if ( nScrolls>0 )
	{
		while ( nScrolls!=0 )
		{
			m_iPicturePosition--;
			nScrolls--;
			ReDrawList();
			Sleep(20);
		}
	}

	UpdateButtonState();
}

int CWebPicDlg::HitTest(const CPoint & point)
{
	if ( m_ListRect.PtInRect(point) )
	{	
		int cx           =  m_ListRect.Width();
		int cy           =  m_ListRect.Height();

		int nBitmaps     = m_vecBitmaps.size();
		int nDrawBitmaps = cx/(PICTURE_CX+PICTURE_SPACE);

		int nTop         = (cy-PICTURE_CY)/2+1;

		for ( int i=-1; i<(nDrawBitmaps+2); i++ )
		{
			int   iPicture   = m_iPicturePosition+i;

			if ( iPicture>=0 && iPicture<nBitmaps )
			{
				CRect rcPicture  ;

				rcPicture.top  = nTop + m_ListRect.top;
				rcPicture.left = i*(PICTURE_CX+PICTURE_SPACE) + m_ListRect.left;
				rcPicture.right  =  rcPicture.left + PICTURE_CX;
				rcPicture.bottom =  rcPicture.top  + PICTURE_CY;

				if ( rcPicture.PtInRect(point) )
				{
					return iPicture;
				}
			}
		}
	}

	return -1;
}
void CWebPicDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( m_ListRect.PtInRect(point) )
	{
		int iHot = HitTest(point);

		if ( iHot!=m_iHotPicture && iHot!=-1 )
		{
			m_iHotPicture = iHot;
			ReDrawList();
			SetTimer(0x01,100,NULL);

			vector<TWebPictureInfo *> & w = m_myPictures.vecPics;
			for ( int i=0; i<w.size(); i++ )
			{
				if ( w[i]->dwPictureID==this->m_vecBitmaps[iHot].pic_id )
				{
					CString strTip;
					strTip.Format("描述:%s\n上传日期:%s\n地址:%s",
						w[i]->szTitle,
						w[i]->szCreateDate,
						w[i]->szUrl);
					ShowToolTip(strTip);
					break;
				}
			}

		}
	}
	else
	{
		ShowToolTip("");
	}


	CDialog::OnMouseMove(nFlags, point);
}

void CWebPicDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnLButtonDown(nFlags, point);
}

void CWebPicDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	int iHotPicture = HitTest(point);

	if ( iHotPicture!=-1 && iHotPicture==this->m_iHotPicture )
	{
		uint32 nPicId =	m_vecBitmaps[iHotPicture].pic_id;

		int n = m_myPictures.vecPics.size();
		for ( int i=0; i<n; i++ )
		{
			if ( m_myPictures.vecPics[i]->dwPictureID==nPicId )
			{
				//ShowMessage(m_myPictures.vecPics[i]->szUrl);
				CString strWebFile  = m_myPictures.vecPics[i]->szUrl;
				CString strFileName = GetWebFileName(strWebFile);
				int i=strFileName.Find(".");
				CString strNewFileName;
				strNewFileName.Format("%s_500.jpg",strFileName.Left(i));
				strWebFile.Replace(strFileName,strNewFileName);
			
				strWebFile.Trim();
				if ( strWebFile.Find("://")==-1 )
					strWebFile = "http://"+strWebFile;
                
				::SendMessage(m_hPicSelWnd,WM_WEBPIC_NOTIFY,(WPARAM)(LPCSTR)strWebFile,0);
				
				ShowWindow(SW_HIDE);
			}
		}
	}
}

void CWebPicDlg::UpdateButtonState()
{
	int nBitmaps = m_vecBitmaps.size();
	int nDrawBitmaps = m_ListRect.Width()/(PICTURE_CX+PICTURE_SPACE);

	if ( m_iPicturePosition>0 )
		m_btnLeft.EnableWindow(TRUE);
	else
		m_btnLeft.EnableWindow(FALSE);

	if ( (m_iPicturePosition+nDrawBitmaps)>=nBitmaps)
		m_btnRight.EnableWindow(FALSE);
	else
		m_btnRight.EnableWindow(TRUE);
}

void CWebPicDlg::OnTimer(UINT nIDEvent)
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	switch(nIDEvent)
	{
	case 0x01:
		{
			if ( m_ListRect.PtInRect(pt)==FALSE )
			{
				m_iHotPicture = -1;
				ReDrawList();
				KillTimer(nIDEvent);
			}
		}
		break;
	}

	CDialog::OnTimer(nIDEvent);
}

void CWebPicDlg::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		m_ToolTip.Create( this , TTS_ALWAYSTIP ) ;
		m_ToolTip.SetMaxTipWidth(400);
		m_ToolTip.Activate( TRUE ) ;
	} 
}

void CWebPicDlg::ShowToolTip( CString strTip)
{
	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() != 0)
	{
		m_ToolTip.DelTool(this,1);
	}

	if (strTip == "") 
	{
		return;
	}

	CRect rect; 
	GetClientRect(rect);
	m_ToolTip.AddTool(this, strTip, rect, 1);

	//m_ToolTip.UpdateTipText(strTip,this,1);

	m_ToolTip.Activate(TRUE);
}

BOOL CWebPicDlg::PreTranslateMessage(MSG* pMsg)
{
	if (m_ToolTip.m_hWnd)
		m_ToolTip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

void CWebPicDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if ( pScrollBar && pScrollBar->m_hWnd==m_wndSlider.m_hWnd )
	{
		int iPosition = m_wndSlider.GetPos();	
		m_iPicturePosition = iPosition;

		ReDrawList();
		UpdateButtonState();
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
