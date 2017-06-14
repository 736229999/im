#include "stdafx.h"
#include "xskindlg.h"
#include "resource.h"
#include ".\xskindlg.h"


#define MAX_BTN    0x01
#define CLOSE_BTN  0x02
#define MIN_BTN    0x03
#define HELP_BTN   0x04


HBRUSH brushBack  = NULL;
HBRUSH brushWhite = NULL;

////////////////////////////////////////////////////////////////////////////
// CXSkinDlg dialog
IMPLEMENT_DYNAMIC(CXSkinDlg, CDialog)

int				CXSkinDlg::m_iInstaceCount = 0;
static bool			s_bVista  =  false;
static int			s_nBorderWidth = 0;
static int			s_nBorderHeight = 0;

CXSkinDlg::CXSkinDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{	
	m_colorBackground	= BACKGND_COLOR;
	m_colorBorder		= BORDER_COLOR;
	m_iCurBtn			= -1;
	m_iBtnDown			= -1;
	m_nStyle			= ALL;
	m_bMaxed			= false;

	m_bModalDlg			= false;
	
	m_bLockOperation    = false;
	m_bMouseDown        = false;
	m_bNcMouseDown      = false;
	m_bHelp				= false;

	m_ToolTip.m_hWnd    = NULL;
	m_dwHelpTick        = 0;

	m_hIcon     =HMG_ResLoadIcon(IDR_MAINFRAME1);//// AfxGetApp()->LoadIcon(IDR_MAINFRAME1);// ::LoadIcon(hInst, MAKEINTRESOURCE(IDR_MAINFRAME1));//LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_ARROW_DOWN));// LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME1));
	m_hBkGround = ::CreateSolidBrush(m_colorBackground);

	
	m_hIconSmall = (HICON)::LoadImage(GetHMGResourceDLLHandle(), MAKEINTRESOURCE(IDR_MAINFRAME1), IMAGE_ICON, 16, 16, LR_SHARED);
	m_pMinMaxInfo = new MINMAXINFO();
	EnableTracker(FALSE);
}

CXSkinDlg::~CXSkinDlg()
{
	delete m_pMinMaxInfo;
	DestroyIcon(m_hIcon);
	if( m_hIconSmall )
	{
		::DestroyIcon(m_hIconSmall);
	}
	DeleteObject(m_hBkGround);
}

void CXSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


void CXSkinDlg::InitResource()
{
	m_fontCaption.CreateFont(
		18,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("微软雅黑"));

	m_fontText.CreateFont(
		10,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("微软雅黑"));


	BOOL b  = m_bmpCaption.LoadBitmap(IDB_DLG_CAPTION_BAR);

	m_bmpMinHot.LoadBitmap(IDB_MIN_HOT);
	m_bmpMinNormal.LoadBitmap(IDB_MIN_NORMAL);
	m_bmpMinDown.LoadBitmap(IDB_MIN_DOWN);

	m_bmpRestoreHot.LoadBitmap(IDB_MAX_HOT);
	m_bmpRestoreNormal.LoadBitmap(IDB_MAX_NORMAL);
	m_bmpRestoreDown.LoadBitmap(IDB_MAX_DOWN);

	m_bmpTitleLeft.LoadBitmap(IDB_DLG_CAPTION_L);
	m_bmpTitleRight.LoadBitmap(IDB_DLG_CAPTION_R);
	m_bmpTitleMid.LoadBitmap(IDB_DLG_CAPTION_M);

	m_bmpMaxNormal.LoadBitmap(IDB_MAX_NORMAL);
	m_bmpMaxHot.LoadBitmap(IDB_MAX_HOT);
	m_bmpMaxDown.LoadBitmap(IDB_MAX_DOWN);
		
	m_bmpCloseNormal.LoadBitmap(IDB_CLOSE_NORMAL);
	m_bmpCloseHot.LoadBitmap(IDB_CLOSE_HOT);
	m_bmpCloseDown.LoadBitmap(IDB_CLOSE_DOWN);

	m_bmpHelpNormal.LoadBitmap(IDB_HELP_NORMAL);
	m_bmpHelpHot.LoadBitmap(IDB_HELP_HOT);
	m_bmpHelpDown.LoadBitmap(IDB_HELP_DOWN);

	OSVERSIONINFO osi;

	osi.dwOSVersionInfoSize = sizeof(osi);

	GetVersionEx(&osi);

	if ( osi.dwMajorVersion>5 )
	{
		s_bVista = true;
	}
	else
	{
		s_bVista = false;
	}
}

BEGIN_MESSAGE_MAP(CXSkinDlg, CDialog)
	ON_WM_NCHITTEST()
	ON_WM_LBUTTONUP()
	ON_WM_NCLBUTTONDBLCLK()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_GETMINMAXINFO()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_TIMER()
	ON_WM_CTLCOLOR()
	ON_WM_NCACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_HELPINFO()
	ON_WM_ACTIVATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CXSkinDlg::DrawBitmap(CDC *pDC,int x, int y, CBitmap *pBitmap, COLORREF clrTrans )
{
	BITMAP bi;
	pBitmap->GetBitmap(&bi);

	CDC dc;
	dc.CreateCompatibleDC(pDC);

	CBitmap *pOldbmp = dc.SelectObject(pBitmap);

	if ( clrTrans==0 )
		pDC->BitBlt(x,y,bi.bmWidth,bi.bmHeight,&dc,0,0,SRCCOPY);
	else
		TransparentBlt(pDC->m_hDC,x,y,bi.bmWidth,bi.bmHeight,dc.m_hDC,0,0,bi.bmWidth,bi.bmHeight,clrTrans);
	
	dc.SelectObject(pOldbmp);
}

void CXSkinDlg::DrawCaptionBar()
{
	CDC *pDC = GetWindowDC();

	if ( pDC==NULL )
		return ;

	CRect rcWindow;
	GetWindowRect(rcWindow);
	int cx  = rcWindow.Width();
	int cy  = rcWindow.Height();
	/*

	CRect rcClient;
	GetClientRect(rcClient);

	if ( s_bVista )
	{
		s_nBorderWidth  = (rcWindow.Width()-rcClient.Width())/4;
		s_nBorderHeight = (rcWindow.Height()-rcClient.Height())/4;
	}
	else
	{
		s_nBorderWidth    = (rcWindow.Width()-rcClient.Width())/2;
		s_nBorderHeight  = (rcWindow.Height()-rcClient.Height())/2;
	}

	CRect rcDraw;
	rcDraw.left   = s_nBorderWidth;
	rcDraw.top    = s_nBorderHeight;
	rcDraw.right  = rcDraw.left + rcClient.Width();
	rcDraw.bottom = rcDraw.top + rcClient.Height();


*/
	CBitmap backBmp;

	int nCaptionHeight = TITLE_BAR_CY;

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	if ( backBmp.CreateCompatibleBitmap(pDC,cx,nCaptionHeight)==FALSE )//caption bar
	{
		ReleaseDC(pDC);
		return ;
	}

	int nSavedMemDC = dcMem.SaveDC();

	dcMem.SelectObject(&backBmp);

	Cx_Strech_Blt(&dcMem,0,0,cx,&m_bmpCaption);

	CString strTitle;
	GetWindowText(strTitle);
/*
	int nTitleWidth = dcMem.GetTextExtent(strTitle).cx+45;
	int nTitleLeft  = 3;
	*/

/*标题不另外画背景zhangmin
	dcTemp.SelectObject(&m_bmpTitleLeft);
	TransparentBlt(dcMem.m_hDC,
		nTitleLeft,2,4,CAPTION_BAR_HIGHT-1,
		dcTemp.m_hDC,
		0,0,4,CAPTION_BAR_HIGHT-1,
		RGB(0,255,0));

	if ( nTitleWidth>cx-65 )
	{
		Cx_Strech_Blt(&dcMem,nTitleLeft+4,2,cx-65-nTitleLeft,&m_bmpTitleMid);
	}
	else
	{
		Cx_Strech_Blt(&dcMem,nTitleLeft+4,2,nTitleWidth-8,&m_bmpTitleMid);
	}


	dcTemp.SelectObject(&m_bmpTitleRight);
	TransparentBlt(dcMem.m_hDC,
		nTitleLeft+nTitleWidth-4,2,4,CAPTION_BAR_HIGHT-1,
		dcTemp.m_hDC,
		0,0,4,CAPTION_BAR_HIGHT-1,
		RGB(0,255,0));
    */
	dcMem.SetBkMode(TRANSPARENT);
	CFont* pOldFontMem = dcMem.SelectObject(&m_fontCaption);

	//DrawBitmap(&dcMem,9,nTop+2,&m_bmpTitleIcon,RGB(0,255,0));
	//dcMem.DrawIcon(9,nTop+2,m_hIcon);
	//DrawIconEx(dcMem.m_hDC, 9, 4, m_hIcon, 17, 17, 0, NULL, DI_NORMAL);
	//不画系统ico，zhangmin
	//DrawIconEx(dcMem.m_hDC, 9, 4, m_hIconSmall, 16, 16, 0, NULL, DI_NORMAL);

	dcMem.SetTextColor(0xFFFFFF);
	//int nTextLeft = 10, nTextTop = 10;
	//if ( nTitleWidth>cx-65 )
	
	{
	
		CRect rcText;
		rcText.left = 8;
		rcText.top  = 4;
		rcText.bottom = rcText.top + 25;
		rcText.right  = rcText.left + 200;

 		dcMem.DrawText(strTitle,&rcText,DT_TOP|DT_LEFT|DT_END_ELLIPSIS);
	}
	/*
	else
	{
		dcMem.TextOut(nTextLeft+1,nTextTop+1,strTitle);
		dcMem.TextOut(nTextLeft-1,nTextTop-1,strTitle);
		dcMem.TextOut(nTextLeft-1,nTextTop+1,strTitle);
		dcMem.TextOut(nTextLeft+1,nTextTop-1,strTitle);
		dcMem.TextOut(nTextLeft,nTextTop-1,strTitle);
		dcMem.TextOut(nTextLeft,nTextTop+1,strTitle);
		dcMem.TextOut(nTextLeft-1,nTextTop,strTitle);
		dcMem.TextOut(nTextLeft+1,nTextTop,strTitle);

 		dcMem.TextOut(nTextLeft,nTextTop,strTitle);
	}
	*/

	pDC->BitBlt(1,1,cx-2,TITLE_BAR_CY,&dcMem,0,0,SRCCOPY);
/*
	m_rcClose.MoveToXY(m_rcClose.left+2,m_rcClose.top+2);
	m_rcMin.MoveToXY(m_rcMin.left+2,m_rcMin.top+2);
	m_rcMax.MoveToXY(m_rcMax.left+2,m_rcMax.top+2);
*/
	dcMem.RestoreDC(nSavedMemDC);


	//覆盖原来的按钮
	/*
	CRect rcTop = rcWindow;
	rcTop.left = rcTop.left + 410;
	rcTop.bottom = rcTop.top + SYS_BTN_SIZE;
	pDC->FillSolidRect(rcTop, m_colorBackground);
	*/
	DrawStyle(pDC);

	ReleaseDC(pDC);
}

void CXSkinDlg::DrawStyle(CDC* pDC)
{
	if ( m_nStyle ==NONE )
	{
		return;
	}

	CRect rcWindow;
	GetWindowRect(rcWindow);
	int cx  = rcWindow.Width();
	int nTop = 1+s_nBorderHeight;

	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pDC);

	int nSavedTempDC = dcTemp.SaveDC();

	CBitmap *pbmpMin, * pbmpMax, * pbmpClose, *pbmpHelp;

	if ( m_iCurBtn==MIN_BTN )
	{
		if ( m_bMouseDown )
			pbmpMin = &m_bmpMinDown;
		else
			pbmpMin = &m_bmpMinHot;//bmpMin.LoadBitmap(IDB_MIN_HOT);
	}
	else
		pbmpMin = &m_bmpMinNormal;//bmpMin.LoadBitmap(IDB_MIN_NORMAL);

	if ( m_iCurBtn==MAX_BTN )
	{
		if ( m_bMaxed )
		{
			if ( m_bMouseDown )
				pbmpMax = &m_bmpRestoreDown;
			else
				pbmpMax = &m_bmpRestoreHot;//bmpMax.LoadBitmap(IDB_RESTORE_HOT);
		}
		else
		{
			if ( m_bMouseDown )
				pbmpMax = &m_bmpMaxDown;
			else
				pbmpMax = &m_bmpMaxHot;//bmpMax.LoadBitmap(IDB_MAX_HOT);
		}
	}
	else
	{
		if ( m_bMaxed )
		{
			pbmpMax = &m_bmpRestoreNormal;//bmpMax.LoadBitmap(IDB_RESTORE_NORMAL);
		}
		else
		{
			pbmpMax = &m_bmpMaxNormal;//bmpMax.LoadBitmap(IDB_MAX_NORMAL);
		}
	}

	if ( m_iCurBtn==CLOSE_BTN)
	{
		if ( m_bMouseDown )
			pbmpClose = &m_bmpCloseDown;//bmpClose.LoadBitmap(IDB_CLOSE_HOT);
		else
			pbmpClose = &m_bmpCloseHot;
	}
	else
		pbmpClose = &m_bmpCloseNormal;

	if ( m_iCurBtn==HELP_BTN )
	{
		if ( m_bMouseDown )
			pbmpHelp = &m_bmpHelpDown;
		else
			pbmpHelp = &m_bmpHelpHot;
	}
	else
		pbmpHelp = &m_bmpHelpNormal;

	const int btn_space = 0;

	dcTemp.SelectObject(pbmpClose);	
	m_rcClose.left   = cx-SYS_BTN_SIZE-1-s_nBorderWidth; 
	m_rcClose.right  = m_rcClose.left+SYS_BTN_SIZE;
	m_rcClose.top    = nTop; 
	m_rcClose.bottom = m_rcClose.top+SYS_BTN_SIZE;
	//backDC.BitBlt(width-20-4,5,16,16,&memDC,0,0,SRCCOPY);
	TransparentBlt(pDC->m_hDC,
		m_rcClose.left,
		nTop
		,SYS_BTN_SIZE,SYS_BTN_SIZE,
		dcTemp.m_hDC,
		0,0,SYS_BTN_SIZE,SYS_BTN_SIZE,
		RGB(0,255,0));

	memset(&m_rcMax,0,sizeof(m_rcMax));
	memset(&m_rcMin,0,sizeof(m_rcMin));

	if ( m_nStyle==ALL ) //min,max,close
	{
		dcTemp.SelectObject(pbmpMax);

		m_rcMax.left   = m_rcClose.left-SYS_BTN_SIZE-btn_space;
		m_rcMax.right  = m_rcMax.left+SYS_BTN_SIZE;
		m_rcMax.top    = nTop;   
		m_rcMax.bottom = m_rcMax.top+SYS_BTN_SIZE;

		//backDC.BitBlt(width-60-4,5,16,16,&memDC,0,0,SRCCOPY);
		TransparentBlt(pDC->m_hDC,
			m_rcMax.left,nTop,SYS_BTN_SIZE,SYS_BTN_SIZE,
			dcTemp.m_hDC,0,0,SYS_BTN_SIZE,SYS_BTN_SIZE,
			RGB(0,255,0));

		dcTemp.SelectObject(pbmpMin);

		m_rcMin.left   = m_rcMax.left-SYS_BTN_SIZE-btn_space;
		m_rcMin.right  = m_rcMin.left+SYS_BTN_SIZE;
		m_rcMin.top    = nTop;   
		m_rcMin.bottom = m_rcMin.top+SYS_BTN_SIZE;

		//backDC.BitBlt(width-60-4,5,16,16,&memDC,0,0,SRCCOPY);
		TransparentBlt(pDC->m_hDC,
			m_rcMin.left,nTop,SYS_BTN_SIZE,SYS_BTN_SIZE,
			dcTemp.m_hDC,0,0,SYS_BTN_SIZE,SYS_BTN_SIZE,
			RGB(0,255,0));

		if(m_bHelp)
		{
			m_rcHelp       = m_rcMin;
			m_rcHelp.left  = m_rcMin.left-SYS_BTN_SIZE-btn_space-3;
			m_rcHelp.right = m_rcHelp.left+SYS_BTN_SIZE;
		}		
	}
	else
	if ( m_nStyle==MINCLOSE ) //min,close
	{
		dcTemp.SelectObject(pbmpMin);

		m_rcMin.left   = m_rcClose.left-SYS_BTN_SIZE-btn_space;
		m_rcMin.right  = m_rcMin.left+SYS_BTN_SIZE;
		m_rcMin.top    = nTop;   
		m_rcMin.bottom = m_rcMin.top+SYS_BTN_SIZE;

		//backDC.BitBlt(width-60-4,5,16,16,&memDC,0,0,SRCCOPY);
		TransparentBlt(pDC->m_hDC,
			m_rcMin.left,nTop,SYS_BTN_SIZE,SYS_BTN_SIZE,
			dcTemp.m_hDC,0,0,SYS_BTN_SIZE,SYS_BTN_SIZE,
			RGB(0,255,0));

		if(m_bHelp)
		{
			m_rcHelp       = m_rcMin;
			m_rcHelp.left  = m_rcMin.left-SYS_BTN_SIZE-btn_space-3;
			m_rcHelp.right = m_rcHelp.left+SYS_BTN_SIZE;
		}		
	}
	else
	{
		if(m_bHelp)
		{
			m_rcHelp       = m_rcClose;
			m_rcHelp.left  = m_rcClose.left-SYS_BTN_SIZE-btn_space-3;
			m_rcHelp.right = m_rcHelp.left+SYS_BTN_SIZE;
		}		
	}

	if ( m_bHelp )
	{
		dcTemp.SelectObject(pbmpHelp);	
		TransparentBlt(pDC->m_hDC,
			m_rcHelp.left,
			nTop
			,SYS_BTN_SIZE,SYS_BTN_SIZE,
			dcTemp.m_hDC,
			0,0,SYS_BTN_SIZE,SYS_BTN_SIZE,
			RGB(0,255,0));
	}

	dcTemp.RestoreDC(nSavedTempDC);
}

void CXSkinDlg::EnableHelp(bool bHelp)
{
	m_bHelp=bHelp;
}

BOOL CXSkinDlg::ShowWindow(int nCmdShow)
{
	if ( nCmdShow==SW_MINIMIZE )
	{
		return CDialog::ShowWindow(nCmdShow);
	}
	else
	if ( nCmdShow==SW_RESTORE )
	{
		if ( IsIconic() )
			CDialog::ShowWindow(SW_RESTORE);
		else
			MoveWindow(m_rcRestore);

		m_bMaxed = false;
	}
	else
	if ( nCmdShow==SW_MAXIMIZE || nCmdShow==SW_SHOWMAXIMIZED )
	{
		GetWindowRect(&m_rcRestore);

		int screen_cx = GetSystemMetrics(SM_CXSCREEN);
		int screen_cy = GetSystemMetrics(SM_CYSCREEN);

		CWnd *pShellTrayWnd = FindWindow(_T("Shell_TrayWnd"),NULL);

		if (pShellTrayWnd)
		{
			CRect rcShellTrayWnd;
			pShellTrayWnd->GetWindowRect(&rcShellTrayWnd);

			if (rcShellTrayWnd.left<=0 && rcShellTrayWnd.bottom>=screen_cy )
			{
				//shell tray wnd on screen bottom
				screen_cy=rcShellTrayWnd.top;
			}
		}

		MoveWindow(1,1,screen_cx,screen_cy);

		m_bMaxed=true;
		//m_rcRestore
	}
	else
	{
		return CDialog::ShowWindow(nCmdShow);
	}

	return TRUE;
}

void CXSkinDlg::UpdateCursor()
{
	if ( this->m_nStyle==ALL || m_bEnableTracker )
	{
		LRESULT lNcTest = NcHitTest();

		switch(lNcTest)
		{
		case HTRIGHT:
		case HTLEFT:
			::SetCursor(::LoadCursor(NULL,IDC_SIZEWE));
			break;
		case HTTOP:
		case HTBOTTOM:
			::SetCursor(::LoadCursor(NULL,IDC_SIZENS));
			break;
		case HTTOPLEFT:
		case HTBOTTOMRIGHT:
			::SetCursor(::LoadCursor(NULL,IDC_SIZENWSE));
			break;
		case HTTOPRIGHT:
		case HTBOTTOMLEFT:
			::SetCursor(::LoadCursor(NULL,IDC_SIZENESW));
			break;
		}
	}
}

void CXSkinDlg::DrawBorder()
{
	CDC *pDC = GetWindowDC();
	if(pDC==NULL)
		return;
	
	int nSavedDC = pDC->SaveDC();
	
	CRect rc;
	GetWindowRect(rc);

	CRect rcClient;
	GetClientRect(rcClient);
   
	

	int cx=rc.Width(); 
	int cy=rc.Height();

	int nCx = ( cx-rcClient.Width() )/2;
	int nCy = ( cy-rcClient.Height() )/2;
    
	if ( nCx>0 )
	{	pDC->FillSolidRect(1,TITLE_BAR_CY,nCx-1,cy-TITLE_BAR_CY,m_colorBackground);
		pDC->FillSolidRect(cx-nCx,TITLE_BAR_CY,nCx-1,cy-TITLE_BAR_CY,m_colorBackground);
	}
		
	if ( nCy>0 )
	{
		pDC->FillSolidRect(0,cy-nCy,cx,nCy-1,m_colorBackground);
	}
	int base = 80;
	int x = 10,y = 50;
	pDC->Draw3dRect(0,0,cx,cy,BORDER_COLOR,BORDER_COLOR);
	//pDC->Draw3dRect(1,1,cx-2,cy-2,0xFF0000,0xFF0000);
	//pDC->Draw3dRect(0,0,cx,cy,RGB(base+x+y,base+x+y,base+x+y),RGB(base+x+y,base+x+y,base+x+y));
	//pDC->Draw3dRect(1,1,cx-2,cy-2,RGB(base+x*1,base+x*1,base+x*1),RGB(base+x*1,base+x*1,base+x*1));
	//pDC->Draw3dRect(2,2,cx-4,cy-4,RGB(150,150,150),RGB(150,150,150));
	
	//pDC->Draw3dRect(3,3,cx-6,cy-6,RGB(137,137,137),RGB(137,137,137));
 /*	pDC->SetPixel(5,5,0xFFFFFF);
	pDC->SetPixel(cx-1,0,0xFFFFFF);
 	pDC->SetPixel(cx-1,cy-1,0xFFFFFF);
	pDC->SetPixel(0,cy-1,0xFFFFFF);
*/
	pDC->RestoreDC(nSavedDC);


	ReleaseDC(pDC);
}

//void CXSkinDlg::UpdateWindowRgn(int cx, int cy)
//{
//	HRGN hRgn = ::CreateRoundRectRgn(0,0,cx,cy,ROUND_SIZE,ROUND_SIZE);
//	SetWindowRgn(hRgn,true);
//}

LRESULT CXSkinDlg::NcHitTest()
{
	CRect rcWindow;
	GetWindowRect(rcWindow);

	//CPoint pt(point.x-rect.left,point.y-rect.top);
	CPoint point;
	GetCursorPos(&point);

	int nSize = 5;

	CRect rcTop = rcWindow;
	rcTop.right = rcWindow.right-nSize;
	rcTop.left  = rcWindow.left+nSize;
	rcTop.bottom = rcWindow.top+3;
    
	CRect rcTopLeft = rcWindow;
	rcTopLeft.right = rcWindow.left+nSize;
	rcTopLeft.bottom = rcWindow.top+nSize;

	CRect rcLeft = rcWindow;
	rcLeft.right=rcWindow.left+3;
	rcLeft.top+=nSize;
	rcLeft.bottom-=nSize;

	CRect rcRight = rcWindow;
	rcRight.left = rcWindow.right-3;
	rcRight.top+=nSize;
	rcRight.bottom-=nSize;

	CRect rcTopRight = rcWindow;
	rcTopRight.bottom=rcWindow.top+nSize;
	rcTopRight.left=rcWindow.right-nSize;

	CRect rcBottom = rcWindow;
	rcBottom.left+=nSize;
	rcBottom.right-=nSize;
	rcBottom.top=rcBottom.bottom-3;

	CRect rcBLeft = rcWindow;
	rcBLeft.top = rcBLeft.bottom-nSize;
	rcBLeft.right = rcBLeft.left+nSize;

	CRect rcBRight = rcWindow;
	rcBRight.left = rcBRight.right-nSize;
	rcBRight.top = rcBRight.bottom-nSize;

	LRESULT lNcTest = HTERROR;

	if (rcTopLeft.PtInRect(point) )
		lNcTest= HTTOPLEFT;
	else
	if (rcTop.PtInRect(point) )
		lNcTest = HTTOP;
	else
	if (rcLeft.PtInRect(point) )
		lNcTest = HTLEFT;
	else
	if (rcRight.PtInRect(point))
		lNcTest = HTRIGHT;
	else
	if (rcTopRight.PtInRect(point) )
		lNcTest = HTTOPRIGHT;
	else
	if (rcBottom.PtInRect(point))
		lNcTest = HTBOTTOM;
	else
	if (rcBLeft.PtInRect(point) )
		lNcTest = HTBOTTOMLEFT;
	else
	if (rcBRight.PtInRect(point) )
		lNcTest = HTBOTTOMRIGHT;

	return lNcTest;

}

LRESULT CXSkinDlg::OnNcHitTest(CPoint point)
{
	CRect rcWindow;
	GetWindowRect(rcWindow);

	int nSize = 5;

	CRect rcCaption;
	rcCaption = rcWindow;

	rcCaption.top = rcWindow.top+4;
	rcCaption.bottom = rcWindow.top+20;

	rcCaption.left+=24;

	if ( m_nStyle==ALL )
		rcCaption.right-=60;
	else
	if ( m_nStyle==MINCLOSE )
		rcCaption.right-=40;
	else
	if ( m_nStyle==CLOSE )
		rcCaption.right-=20;
	else
		rcCaption.right-=nSize;

	if ( m_bHelp )
		rcCaption.right-=25;

	
	if (rcCaption.PtInRect(point))
		return HTCAPTION;

	if ( m_nStyle==ALL )
	{
		CRect rcMenu  = rcWindow;
		rcMenu.right = rcMenu.left+TITLE_BAR_CY;
		rcMenu.bottom = rcMenu.top+TITLE_BAR_CY;

		LRESULT lNcTest = HTERROR;

		if ( rcMenu.PtInRect(point) )
			lNcTest = HTMENU;
		/*else
		if (rcTopLeft.PtInRect(point) )
			lNcTest= HTTOPLEFT;
		else
		if (rcTop.PtInRect(point) )
			lNcTest = HTTOP;
		else
		if (rcLeft.PtInRect(point) )
			lNcTest = HTLEFT;
		else
		if (rcRight.PtInRect(point))
			lNcTest = HTRIGHT;
		else
		if (rcTopRight.PtInRect(point) )
			lNcTest = HTTOPRIGHT;
		else
		if (rcBottom.PtInRect(point))
			lNcTest = HTBOTTOM;
		else
		if (rcBLeft.PtInRect(point) )
			lNcTest = HTBOTTOMLEFT;
		else
		if (rcBRight.PtInRect(point) )
			lNcTest = HTBOTTOMRIGHT;*/

		if ( lNcTest!=HTERROR )
			return lNcTest;

		int count=m_vecHWndChild.size();

		rcWindow.bottom = rcWindow.top+TITLE_BAR_CY;

		if ( rcWindow.PtInRect(point)==FALSE )
		{
			bool bFlag =false;
			for ( int i=0; i<count; i++ )
			{
				CRect rcChild;
				::GetWindowRect(m_vecHWndChild[i],&rcChild);
				ScreenToClient(&rcChild);

				if ( rcChild.PtInRect(point) )
				{
					bFlag = true;
					break;
				}
			}

			if ( bFlag==false )
			{
			//	return HTCAPTION;
			// 任何空白的地方都可以拖动窗口
			}
		}

	}

	return CDialog::OnNcHitTest(point);
}


void CXSkinDlg::ShowToolTip( CString strTip, const CRect &rc)
{
	InitToolTip();
	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() != 0)
	{
		m_ToolTip.DelTool(this,1);
	}
	
	if (strTip == "") 
	{
		return;
	}

	//CRect rect; 
	//GetClientRect(rect);
	m_ToolTip.AddTool(this, strTip, rc, 1);

	//m_ToolTip.UpdateTipText(strTip,this,1);

	m_ToolTip.Activate(TRUE);
}

void CXSkinDlg::SetWindowText(LPCTSTR lpszString)
{
	CDialog::SetWindowText(lpszString);
	DrawCaptionBar();
}

void CXSkinDlg::OnMinBtnClick()
{
	ShowWindow(SW_MINIMIZE);
}

void CXSkinDlg::OnHelpClick()
{

}

void CXSkinDlg::OnMaxBtnClick()
{
	if(m_bMaxed)
	{
		ShowWindow(SW_RESTORE);
		Invalidate();
	}
	else
	{
		ShowWindow(SW_MAXIMIZE);
		Invalidate();
	}
}

void CXSkinDlg::OnCloseBtnClick()
{
	if ( m_bModalDlg )
	{	
		CDialog::OnCancel();
	}
	else
	{
		PostMessage(WM_CLOSE);
	} 
}

void CXSkinDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	if ( !m_rcClose.PtInRect(pt) && !m_rcMin.PtInRect(pt) && !m_rcMax.PtInRect(pt) && !m_rcHelp.PtInRect(pt))
		PostMessage(WM_NCLBUTTONDOWN,HTCAPTION,MAKELPARAM(point.x,   point.y));

	if ( m_rcMin.PtInRect(point) || m_rcClose.PtInRect(point) || m_rcMax.PtInRect(point) || m_rcHelp.PtInRect(point) )
	{
		m_bMouseDown = true;
		SetTimer(0xFFFE,100,NULL);
		DrawCaptionBar();
	}

	if ( (m_nStyle==ALL || m_bEnableTracker) && m_bNcMouseDown==false )
	{
		m_lNcTest = NcHitTest();

		if ( m_lNcTest!=HTERROR )
		{
			m_bNcMouseDown = true;
			GetWindowRect(&m_rcSizeWnd);
			DrawLine();
			SetCapture();
		}
	}

	UpdateCursor();

	CDialog::OnLButtonDown(nFlags, point);
}

void CXSkinDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_iBtnDown = -1;

	m_bMouseDown = false;

	if ( m_bNcMouseDown )
	{
		m_bNcMouseDown = false;
		DrawLine();
		MoveWindow(m_rcSizeWnd);
		Invalidate();
		::ReleaseCapture();
	}

	if ( m_rcClose.PtInRect(point) )
	{
		m_iBtnDown = CLOSE_BTN;
		OnCloseBtnClick();
		return ;
	}

	if ( m_rcMin.PtInRect(point) )
	{
		m_iBtnDown = MIN_BTN;
		OnMinBtnClick();
		return;
	}

	if ( m_rcMax.PtInRect(point) )
	{
		m_iBtnDown = MAX_BTN;
		OnMaxBtnClick();
		return ;
	}

	if ( m_rcHelp.PtInRect(point) )
	{
		m_iBtnDown = HELP_BTN;
		OnHelpClick();
		DrawCaptionBar();
		return ;
	}

	
	CDialog::OnLButtonUp(nFlags, point);
}

BOOL CXSkinDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 绘制阴影
	/*m_pWndShadow->Create(GetSafeHwnd());
	m_pWndShadow->SetSize(50);
	m_pWndShadow->SetPosition(0, 0);
	m_pWndShadow->SetSharpness(10);*/
	//m_WndShadow.SetColor(RGB(10,10,10));

	InitResource();

	SetIcon(m_hIcon,FALSE);

	CRect rc(0,0,10,10);
	m_wndTitle.Create(_T("AAA"),0,rc,this);

	//EnableToolTips(TRUE);		// 不必调用，此函数只针对CWnd自带的ToolTips，调用会造成内存泄露
	InitToolTip();

	UpdateWindowLong();

	InitMinMaxInfo();

	EnumChildWindows(this->m_hWnd,(WNDENUMPROC)EnumChildProc,(LPARAM)this);

/*
	CRgn   m_rgn; 
	RECT   rc2;                             
	GetWindowRect(&rc2);             
	m_rgn.CreateRoundRectRgn(rc2.left,rc2.top,rc2.right,rc2.bottom,10,10);         
	SetWindowRgn(m_rgn,TRUE);
*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CXSkinDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if ( m_hWnd )
	{
		if ( m_wndTitle.m_hWnd )
			m_wndTitle.MoveWindow(cx-10,0,10,10);
	}
}


void CXSkinDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcWindow;
	GetWindowRect(rcWindow);

	//CPoint pt(point.x-rect.left,point.y-rect.top);
	CPoint pt;
	GetCursorPos(&pt);

	ScreenToClient(&rcWindow);
	ScreenToClient(&pt);

	bool bRedrawCaption = false;

	if ( m_rcClose.PtInRect(pt)  )
	{
		if ( m_iCurBtn!=CLOSE_BTN )
		{
			ShowToolTip(_T("关闭"), m_rcClose);
			m_iCurBtn = CLOSE_BTN;
			bRedrawCaption = true;
		}
	}
	else
	if ( m_iCurBtn==CLOSE_BTN )
	{
		ShowToolTip(_T(""), rcWindow);
	}

	if ( m_rcMin.PtInRect(pt) )
	{
		if ( m_iCurBtn!=MIN_BTN )
		{
			ShowToolTip(_T("最小化"), m_rcMin);
			m_iCurBtn = MIN_BTN;
			bRedrawCaption = true;
		}
	}
	else
	if ( m_iCurBtn==MIN_BTN )
	{
		ShowToolTip(_T(""), rcWindow);
	}

	if ( m_rcHelp.PtInRect(pt) )
	{
		if ( m_iCurBtn!=HELP_BTN )
		{
			ShowToolTip(_T("帮助"), m_rcHelp);
			m_iCurBtn = HELP_BTN;
			bRedrawCaption = true;
		}
	}
	else
	if ( m_iCurBtn==HELP_BTN )
	{
		ShowToolTip(_T(""), rcWindow);
	}

	if ( m_rcMax.PtInRect(pt) )
	{
		if ( m_iCurBtn!=MAX_BTN )
		{
			if(m_bMaxed)
			{
				ShowToolTip(_T("恢复"), m_rcMax);		
			}
			else
			{
				ShowToolTip(_T("最大化"), m_rcMax);
			}

			m_iCurBtn = MAX_BTN;
			bRedrawCaption = true;	
		}		
	}
	else
	if ( m_iCurBtn==MAX_BTN )
	{
		ShowToolTip(_T(""), rcWindow);
	}


	if ( bRedrawCaption )
	{
		SetTimer(0xFFFE,100,NULL);
		DrawCaptionBar();
	}

	UpdateCursor();

	if ( m_bNcMouseDown )
	{
		CRect rcWnd = m_rcSizeWnd;
		
		CRect rcInter = rcWnd;

		CPoint pt;
		GetCursorPos(&pt);

		int nSize = 2;

		switch(m_lNcTest)
		{
		case HTRIGHT:
			rcInter.left = rcWnd.right-nSize;
			rcInter.right = pt.x;
			rcWnd.right = pt.x;
			break;
		case HTLEFT:
			rcInter.right = rcWnd.right;
			rcInter.left  = pt.x + rcWnd.Width()-nSize;
			rcWnd.left = pt.x;
			break;
		case HTTOP:
			rcInter.bottom = pt.x+rcWnd.Height()-nSize;
			rcInter.top = pt.y;
			rcWnd.top = pt.y;
			break;
		case HTBOTTOM:
			rcInter.top = rcWnd.bottom-nSize;
			rcInter.bottom = pt.y;
			rcWnd.bottom = pt.y;
			break;
		case HTTOPLEFT:
		//	rcInter.bottom = 
			rcWnd.left = pt.x;
			rcWnd.top = pt.y;
			break;
		case HTBOTTOMRIGHT:
			rcWnd.right = pt.x;
			rcWnd.bottom = pt.y;
			break;
		case HTBOTTOMLEFT:
			rcWnd.left = pt.x;
			rcWnd.bottom = pt.y;
			break;
		case HTTOPRIGHT:
			rcWnd.top = pt.y;
			rcWnd.right = pt.x;
			break;
		}
	
		int nDestCx = rcWnd.Width();
		int nDestCy = rcWnd.Height();

		bool bSizeWnd = false;

		if ( nDestCx>25 && nDestCy>80 &&
			   nDestCx>m_pMinMaxInfo->ptMinTrackSize.x &&
			   nDestCy>m_pMinMaxInfo->ptMinTrackSize.y &&
			   nDestCx<m_pMinMaxInfo->ptMaxTrackSize.x &&
			   nDestCy<m_pMinMaxInfo->ptMaxTrackSize.y 
			)
		{
			bSizeWnd = true;		
		}
		else
		{
			if ( nDestCx > m_pMinMaxInfo->ptMaxTrackSize.x )
			{
				if ( m_lNcTest==HTRIGHT || m_lNcTest==HTBOTTOMRIGHT || m_lNcTest==HTTOPRIGHT )
				{
					rcWnd.right = rcWnd.left + m_pMinMaxInfo->ptMaxTrackSize.x;
				}
				else
				{
					rcWnd.left = rcWnd.right - m_pMinMaxInfo->ptMaxTrackSize.x;
				}
			}

			if ( nDestCy > m_pMinMaxInfo->ptMaxTrackSize.y )
			{
				if ( m_lNcTest==HTBOTTOM || m_lNcTest==HTBOTTOMRIGHT || m_lNcTest==HTBOTTOMLEFT )
				{
					rcWnd.bottom = rcWnd.top + m_pMinMaxInfo->ptMaxTrackSize.y;
				}
				else
				{
					rcWnd.top = rcWnd.bottom - m_pMinMaxInfo->ptMaxTrackSize.y;
				}
			}
			
			if ( nDestCx < m_pMinMaxInfo->ptMinTrackSize.x )
			{
				if ( m_lNcTest==HTRIGHT || m_lNcTest==HTBOTTOMRIGHT || m_lNcTest==HTTOPRIGHT )
				{
					rcWnd.right = rcWnd.left + m_pMinMaxInfo->ptMinTrackSize.x;
				}
				else
				{
					rcWnd.left = rcWnd.right - m_pMinMaxInfo->ptMinTrackSize.x;
				}
			}

			if ( nDestCy < m_pMinMaxInfo->ptMinTrackSize.y )
			{
				if ( m_lNcTest==HTBOTTOM || m_lNcTest==HTBOTTOMRIGHT || m_lNcTest==HTBOTTOMLEFT )
				{
					rcWnd.bottom = rcWnd.top + m_pMinMaxInfo->ptMinTrackSize.y;
				}
				else
				{
					rcWnd.top = rcWnd.bottom - m_pMinMaxInfo->ptMinTrackSize.y;
				}
			}

			bSizeWnd = true;
		}

		if ( bSizeWnd )
		{
			DrawLine();
			m_rcSizeWnd = rcWnd;
			DrawLine();
		}
	}

	
	CDialog::OnMouseMove(nFlags, point);
}

void CXSkinDlg::SetDlgStyle(DLG_STYLE nStyle)
{
	m_nStyle=nStyle;

	UpdateWindowLong();

//	if ( nStyle==ALL )
//	{
//		long lStyle = ::GetWindowLong(this->m_hWnd ,GWL_STYLE); 
//		lStyle = lStyle & (~WS_CAPTION);
//		::SetWindowLong(this->m_hWnd,GWL_STYLE,lStyle); 
//	}

	//	long lStyle = ::GetWindowLong(this->m_hWnd ,GWL_STYLE); 
//	::SetWindowLong(this->m_hWnd,GWL_STYLE,lStyle & (~WS_CAPTION) ); 
}

void CXSkinDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	//m_objMinMaxInfo = *lpMMI;

	CDialog::OnGetMinMaxInfo(lpMMI);
}

void CXSkinDlg::OnPaint()
{
	CPaintDC dc(this);	
	DrawCaptionBar();
	DrawBorder();
}

void CXSkinDlg::OnNcPaint()
{
	DrawCaptionBar();
	DrawBorder();
}

void CXSkinDlg::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0xFFFE )
	{
		CPoint pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);

		bool bRedrawCaption=false;

		switch ( m_iCurBtn )
		{
		case CLOSE_BTN:
			if ( !m_rcClose.PtInRect(pt) )
			{
				bRedrawCaption=true;
			}
			break;
		case MIN_BTN:
			if ( !m_rcMin.PtInRect(pt) )
			{
				bRedrawCaption=true;
			}
			break;
		case MAX_BTN:
			if ( !m_rcMax.PtInRect(pt) )
			{
				bRedrawCaption=true;
			}
			break;
		case HELP_BTN:
			if ( !m_rcHelp.PtInRect(pt) )
			{
				bRedrawCaption=true;
			}
			break;
		default:
			break;
		}

		if ( bRedrawCaption )
		{
			KillTimer(0xFFFE);
			m_iCurBtn    = -1;
			m_bMouseDown = false;
			DrawCaptionBar();
		}
	}

	CDialog::OnTimer(nIDEvent);
}

HBRUSH CXSkinDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_DLG)
	{
		hbr = this->m_hBkGround;
	}

	return hbr;
}

void CXSkinDlg::RemoveRgnOfWnd(CRgn &rgn, CWnd *pWnd,int iOffset)
{
	if ( pWnd && pWnd->m_hWnd )
	{
		CRgn rgnWnd;
		CRect rectWnd;
		pWnd->GetWindowRect(&rectWnd);
		ScreenToClient(&rectWnd);

		rectWnd.InflateRect(iOffset,iOffset);
		rgnWnd.CreateRectRgn(rectWnd.left,rectWnd.top,rectWnd.right,rectWnd.bottom);

		rgn.CombineRgn( &rgn, &rgnWnd, RGN_DIFF);
	}
}

void CXSkinDlg::OnTitleBarDblClick()
{
	if ( this->m_nStyle!=ALL )
		return;

	if ( this->m_bMaxed )
	{
		ShowWindow(SW_RESTORE);
	}
	else
	{
		ShowWindow(SW_MAXIMIZE);
	}
}

void CXSkinDlg::OnNcLButtonDblClk(UINT nFlags, CPoint point)
{
	OnTitleBarDblClick();
//	CDialog::OnLButtonDblClk(nFlags, point);	
}

BOOL CXSkinDlg::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message==WM_KEYDOWN)
	{
		if (pMsg->wParam==VK_F1)
		{
			if ( m_bHelp )
			{
				DWORD dwTick = GetTickCount();
				if ((dwTick-m_dwHelpTick)>300)
				{
					OnHelpClick();
				}
				m_dwHelpTick=dwTick;
			}
		}
	}

	if ( m_bLockOperation==true )
	{
		if (   
			pMsg->message==WM_LBUTTONDOWN ||
			pMsg->message==WM_LBUTTONUP ||
			pMsg->message==WM_RBUTTONDOWN ||
			pMsg->message==WM_RBUTTONUP ||
			pMsg->message==WM_LBUTTONDBLCLK ||
			pMsg->message==WM_RBUTTONDBLCLK ||
			pMsg->message==WM_NCLBUTTONDOWN ||
			pMsg->message==WM_NCLBUTTONUP ||
			pMsg->message==WM_NCRBUTTONDOWN ||
			pMsg->message==WM_NCRBUTTONUP ||
			pMsg->message==WM_NCLBUTTONDBLCLK ||
			pMsg->message==WM_NCRBUTTONDBLCLK ||
			pMsg->message==WM_KEYDOWN ||
			pMsg->message==WM_KEYUP ||
			pMsg->message==WM_COMMAND )
		{
			return 0;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CXSkinDlg::OnNcActivate(BOOL bActive)
{
	BOOL bRet = CDialog::OnNcActivate(bActive);
	DrawCaptionBar();
	DrawBorder();
	return bRet;
}

BOOL CXSkinDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	CDialog::PreCreateWindow(cs);

	SetFont(&m_fontText);

	return TRUE;
}

void CXSkinDlg::Cx_Strech_Blt(CDC *pDestDC, int x, int y, int cx, CBitmap *pBitmap)
{
	BITMAP bi;
	pBitmap->GetBitmap(&bi);

	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pDestDC);
	CBitmap *pOldbmp=dcTemp.SelectObject(pBitmap);

	int nMuli = cx/bi.bmWidth;
	int nRema = cx%bi.bmWidth;

	int left ;
	for ( int ii=0; ii<nMuli; ii++ )
	{
		left= ii*bi.bmWidth;
		pDestDC->BitBlt(left+x,y,bi.bmWidth,bi.bmHeight, &dcTemp,0,0,SRCCOPY);
	}
	
	left = nMuli*bi.bmWidth;
	pDestDC->BitBlt(left+x,y,nRema,bi.bmHeight,&dcTemp,0,0,SRCCOPY);

	dcTemp.SelectObject(pOldbmp);
}

BOOL CALLBACK CXSkinDlg::EnumChildProc(HWND hWnd, LPARAM lParam)
{
	CXSkinDlg *pDlg = (CXSkinDlg *)lParam;

	if ( hWnd )//&& ::IsWindowVisible(hWnd) )
		pDlg->m_vecHWndChild.push_back(hWnd);

	return TRUE;
}

void CXSkinDlg::SetTitleIcon(HINSTANCE hInstance,UINT nID)
{
	if ( m_hIcon )
		DestroyIcon(m_hIcon);

	if( m_hIconSmall)
		DestroyIcon(m_hIconSmall);

	m_hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(nID));
	m_hIconSmall = (HICON)::LoadImage(hInstance, MAKEINTRESOURCE(nID), IMAGE_ICON, 16, 16, LR_SHARED);


	SetIcon(m_hIcon,FALSE);
	SetIcon(m_hIcon,TRUE);
}

void CXSkinDlg::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		m_ToolTip.Create( this , TTS_ALWAYSTIP ) ;
		m_ToolTip.SetMaxTipWidth(400);
		m_ToolTip.Activate( TRUE ) ;
	} 
}
 
void CXSkinDlg::UpdateWindowLong()
{
	if ( m_hWnd )
	{
		if ( m_nStyle==MINCLOSE )
		{
			DWORD d = GetWindowLong(m_hWnd,GWL_STYLE);
			d = d|(WS_SYSMENU|WS_MINIMIZEBOX);
			SetWindowLong(m_hWnd,GWL_STYLE,d);
		}

		if ( m_nStyle==ALL )
		{
			DWORD d = GetWindowLong(m_hWnd,GWL_STYLE);
			d = d|(WS_SYSMENU|WS_MINIMIZEBOX|WS_MAXIMIZEBOX);
			SetWindowLong(m_hWnd,GWL_STYLE,d);
		}
	}
}


void CXSkinDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CDialog::OnWindowPosChanged(lpwndpos);//SWP_SHOWWINDOW

	DWORD d = SWP_NOSIZE|SWP_NOACTIVATE|SWP_NOZORDER;
	if ( this->m_bMaxed && (lpwndpos->flags&d)==d )
	{
		if ( lpwndpos->y<0 || lpwndpos->x<0 )
		{
			//CRect rc;
			int x = lpwndpos->x<0? 0:lpwndpos->x;
			int y = lpwndpos->y<0? 0:lpwndpos->y;
		//	rc.SetRect(x,y,x+lpwndpos->cx,y+lpwndpos->cy);
		//	MoveWindow(&rc);
			::SetWindowPos(m_hWnd,lpwndpos->hwndInsertAfter,x,y,lpwndpos->cx,lpwndpos->cy,d);
		}
	}
}

INT_PTR CXSkinDlg::DoModal()
{
	m_bModalDlg=true;
	return CDialog::DoModal();
}

BOOL CXSkinDlg::OnHelpInfo(HELPINFO* lpHelpInfo)
{
	if ( m_bHelp )
	{
		DWORD dwTick = GetTickCount();
		if ((dwTick-m_dwHelpTick)>300)
		{
			OnHelpClick();
		}
		m_dwHelpTick=dwTick;

	}

	return TRUE;
}

void CXSkinDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	if ( m_nStyle==MINCLOSE && nState==WA_CLICKACTIVE && IsIconic() )
	{
		ShowWindow(SW_RESTORE);
	}
}

void CXSkinDlg::DrawLine()
{
	CWindowDC dc(NULL);

	CDC *pDC = &dc;

	int nRop = pDC->SetROP2(R2_NOTXORPEN);

	CRect rcWnd = m_rcSizeWnd;

	int d = 1;

	CPen penDot;
	//penDot.CreatePen(PS_SOLID,1,RGB(0,0,0));
	LOGBRUSH logBrush;
	logBrush.lbStyle = BS_SOLID;
	logBrush.lbColor = RGB(0,0,0);
	penDot.CreatePen(PS_DOT|PS_GEOMETRIC|PS_JOIN_ROUND, 1, &logBrush);
	
	CPen *pOP = pDC->SelectObject(&penDot);
	CBrush * pOB = pDC->SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));
	
	rcWnd.InflateRect(1,1);
	pDC->Rectangle(&rcWnd);
	
	pDC->SetROP2(nRop);
	pDC->SelectObject(pOP);
	pDC->SelectObject(pOB);
}

void CXSkinDlg::LockOperation()
{
	if ( m_bLockOperation==false )
	{
		int n = m_vecHWndChild.size();
		for ( int i=0; i<n; i++ )
		{
			HWND hWnd = m_vecHWndChild[i];
			if ( ::IsWindowEnabled(hWnd) )
			{
				m_vecLockedWnd.push_back(hWnd);
				::EnableWindow(hWnd,FALSE);
			}
		}
	}
	m_bLockOperation=true;
}

void CXSkinDlg::UnLockOperation()
{
	if ( m_bLockOperation )
	{
		int n = m_vecLockedWnd.size();
		for ( int i=0; i<n; i++ )
			::EnableWindow(m_vecLockedWnd[i],TRUE);

		m_vecLockedWnd.clear();
	}

	m_bLockOperation=false;
}

void CXSkinDlg::SetMinMaxInfo(int nMinWidth, int nMinHeight, int nMaxWidth, int nMaxHeight)
{
	MINMAXINFO * lpMMI = m_pMinMaxInfo;

	if ( nMinWidth!=0 )
		lpMMI->ptMinTrackSize.x = nMinWidth;

	if ( nMinHeight!=0 )
		lpMMI->ptMinTrackSize.y = nMinHeight;

	if ( nMaxWidth!=0 )
		lpMMI->ptMaxTrackSize.x = nMaxWidth;

	if ( nMaxHeight!=0 )
		lpMMI->ptMaxTrackSize.y = nMaxHeight;

	if ( m_hWnd )
	{
		CRect rcWnd;
		GetWindowRect(&rcWnd);

		int cx = rcWnd.Width();
		int cy = rcWnd.Height();

		if ( cx>lpMMI->ptMaxTrackSize.x )
			rcWnd.right = rcWnd.left+lpMMI->ptMaxTrackSize.x;

		if ( cx<lpMMI->ptMinTrackSize.x )
			rcWnd.right = rcWnd.left+lpMMI->ptMinTrackSize.x;

		if ( cy>lpMMI->ptMaxTrackSize.y )
			rcWnd.bottom = rcWnd.top + lpMMI->ptMaxTrackSize.y ;

		if ( cy<lpMMI->ptMinTrackSize.y )
			rcWnd.bottom = rcWnd.top + lpMMI->ptMinTrackSize.y;

		m_rcRestore = rcWnd;
		MoveWindow(&rcWnd);
	}
}

void CXSkinDlg::InitMinMaxInfo()
{
	int screen_cx = GetSystemMetrics(SM_CXSCREEN);
	int screen_cy = GetSystemMetrics(SM_CYSCREEN);

	CWnd *pShellTrayWnd = FindWindow(_T("Shell_TrayWnd"),NULL);

	if (pShellTrayWnd)
	{
		CRect rcShellTrayWnd;
		pShellTrayWnd->GetWindowRect(&rcShellTrayWnd);

		if (rcShellTrayWnd.left<=0 && rcShellTrayWnd.bottom>=screen_cy )
		{
			//shell tray wnd on screen bottom
			screen_cy=rcShellTrayWnd.top;
		}
	}

	MINMAXINFO * lpMMI = m_pMinMaxInfo;

	lpMMI->ptMinTrackSize.x = 80;
	lpMMI->ptMinTrackSize.y = 35;
	lpMMI->ptMaxTrackSize.x = screen_cx + 3;
	lpMMI->ptMaxTrackSize.y = screen_cy + 3;
}

LRESULT CXSkinDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( m_ToolTip.m_hWnd && message==WM_MOUSEMOVE )
	{
		MSG msg;
		msg.hwnd = m_hWnd;
		msg.message = message;
		msg.time = GetTickCount();
		msg.wParam = wParam;
		msg.lParam = lParam;
		GetCursorPos(&msg.pt);
		m_ToolTip.RelayEvent(&msg);
	}

	if ( m_bLockOperation==true )
	{
		if (    message==WM_LBUTTONDOWN ||
				message==WM_LBUTTONUP ||
				message==WM_RBUTTONDOWN ||
				message==WM_RBUTTONUP ||
				message==WM_LBUTTONDBLCLK ||
				message==WM_RBUTTONDBLCLK ||
				message==WM_NCLBUTTONDOWN ||
				message==WM_NCLBUTTONUP ||
				message==WM_NCRBUTTONDOWN ||
				message==WM_NCRBUTTONUP ||
				message==WM_NCLBUTTONDBLCLK ||
				message==WM_NCRBUTTONDBLCLK ||
				message==WM_KEYDOWN ||
				message==WM_KEYUP ||
				message==WM_COMMAND )
		{
			return 0;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CXSkinDlg::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
}


BOOL CXSkinDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(rcClient);

	pDC->FillSolidRect(rcClient,m_colorBackground);
	return FALSE;
}
