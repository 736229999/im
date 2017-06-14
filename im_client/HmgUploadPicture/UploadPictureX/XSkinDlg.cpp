#include "..\stdafx.h"
#include "xskindlg.h"
#include "..\resource.h"
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

CBitmap	        CXSkinDlg::m_bmpCaption	;
CBitmap			CXSkinDlg::m_bmpMinNormal, CXSkinDlg::m_bmpMinHot,CXSkinDlg::m_bmpMinDown;
CBitmap			CXSkinDlg::m_bmpMaxNormal, CXSkinDlg::m_bmpMaxHot,CXSkinDlg::m_bmpMaxDown;
CBitmap			CXSkinDlg::m_bmpCloseNormal, CXSkinDlg::m_bmpCloseHot, CXSkinDlg::m_bmpCloseDown;
CBitmap			CXSkinDlg::m_bmpHelpNormal, CXSkinDlg::m_bmpHelpHot, CXSkinDlg::m_bmpHelpDown;
CBitmap			CXSkinDlg::m_bmpRestoreNormal, CXSkinDlg::m_bmpRestoreHot,  CXSkinDlg::m_bmpRestoreDown;
CBitmap			CXSkinDlg::m_bmpTitleLeft, CXSkinDlg::m_bmpTitleRight,  CXSkinDlg::m_bmpTitleMid;
CFont			CXSkinDlg::m_fontCaption			;
CFont			CXSkinDlg::m_fontText				;

CXSkinDlg::CXSkinDlg(UINT nIDTemplate, CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
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

	m_hIcon     = AfxGetApp()->LoadIcon(IDR_MAINFRAME);// ::LoadIcon(hInst, MAKEINTRESOURCE(IDR_MAINFRAME1));//LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_ARROW_DOWN));// LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME1));
	m_hBkGround = ::CreateSolidBrush(BACKGND_COLOR);
}

CXSkinDlg::~CXSkinDlg()
{
	DestroyIcon(m_hIcon);
	DeleteObject(m_hBkGround);
}

void CXSkinDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


void CXSkinDlg::InitResource()
{
	static bool bInit=false;

	if ( bInit==false )
	{
		m_fontCaption.CreateFont(
			14,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_BOLD,                   // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			ANSI_CHARSET,              // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			_T("Verdana"));

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
			_T("宋体"));


		m_bmpCaption.LoadBitmap(IDB_DLG_CAPTION_BAR);

		m_bmpMinHot.LoadBitmap(IDB_MIN_HOT);
		m_bmpMinNormal.LoadBitmap(IDB_MIN_NORMAL);
		m_bmpMinDown.LoadBitmap(IDB_MIN_DOWN);

		m_bmpRestoreHot.LoadBitmap(IDB_RESTORE_HOT);
		m_bmpRestoreNormal.LoadBitmap(IDB_RESTORE_NORMAL);
		m_bmpRestoreDown.LoadBitmap(IDB_RESTORE_DOWN);

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

		bInit=true;
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

	int cx  = rcWindow.Width();
	int cy  = rcWindow.Height();

	CDC dcMem;
	CBitmap backBmp;

	int nCaptionHeight = 23;

	dcMem.CreateCompatibleDC(pDC);

	if ( backBmp.CreateCompatibleBitmap(pDC,cx,nCaptionHeight)==FALSE )//caption bar
	{
		ReleaseDC(pDC);
		return ;
	}

	int nSavedMemDC = dcMem.SaveDC();

	dcMem.SelectObject(&backBmp);

	Cx_Strech_Blt(&dcMem,0,0,cx,&m_bmpCaption);

	int nTop = 4+s_nBorderHeight;

	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pDC);
	
	int nSavedTempDC = dcTemp.SaveDC();


	if ( m_nStyle!=NONE )
	{
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

		const int btn_space = 4;

		dcTemp.SelectObject(pbmpClose);	
		m_rcClose.left   = cx-SYS_BTN_SIZE-8-s_nBorderWidth; 
		m_rcClose.right  = m_rcClose.left+SYS_BTN_SIZE;
		m_rcClose.top    = nTop; 
		m_rcClose.bottom = m_rcClose.top+SYS_BTN_SIZE;
		//backDC.BitBlt(width-20-4,5,16,16,&memDC,0,0,SRCCOPY);
		TransparentBlt(dcMem.m_hDC,
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
			TransparentBlt(dcMem.m_hDC,
				m_rcMax.left,nTop,SYS_BTN_SIZE,SYS_BTN_SIZE,
				dcTemp.m_hDC,0,0,SYS_BTN_SIZE,SYS_BTN_SIZE,
				RGB(0,255,0));

			dcTemp.SelectObject(pbmpMin);

			m_rcMin.left   = m_rcMax.left-SYS_BTN_SIZE-btn_space;
			m_rcMin.right  = m_rcMin.left+SYS_BTN_SIZE;
			m_rcMin.top    = nTop;   
			m_rcMin.bottom = m_rcMin.top+SYS_BTN_SIZE;

			//backDC.BitBlt(width-60-4,5,16,16,&memDC,0,0,SRCCOPY);
			TransparentBlt(dcMem.m_hDC,
				m_rcMin.left,nTop,SYS_BTN_SIZE,SYS_BTN_SIZE,
				dcTemp.m_hDC,0,0,SYS_BTN_SIZE,SYS_BTN_SIZE,
				RGB(0,255,0));

			m_rcHelp       = m_rcMin;
			m_rcHelp.left  = m_rcMin.left-SYS_BTN_SIZE-btn_space-3;
			m_rcHelp.right = m_rcHelp.left+SYS_BTN_SIZE;
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
			TransparentBlt(dcMem.m_hDC,
				m_rcMin.left,nTop,SYS_BTN_SIZE,SYS_BTN_SIZE,
				dcTemp.m_hDC,0,0,SYS_BTN_SIZE,SYS_BTN_SIZE,
				RGB(0,255,0));

			m_rcHelp       = m_rcMin;
			m_rcHelp.left  = m_rcMin.left-SYS_BTN_SIZE-btn_space-3;
			m_rcHelp.right = m_rcHelp.left+SYS_BTN_SIZE;
		}
		else
		{
			m_rcHelp       = m_rcClose;
			m_rcHelp.left  = m_rcClose.left-SYS_BTN_SIZE-btn_space-3;
			m_rcHelp.right = m_rcHelp.left+SYS_BTN_SIZE;
		}

		if ( m_bHelp )
		{
			dcTemp.SelectObject(pbmpHelp);	
			TransparentBlt(dcMem.m_hDC,
				m_rcHelp.left,
				nTop
				,SYS_BTN_SIZE,SYS_BTN_SIZE,
				dcTemp.m_hDC,
				0,0,SYS_BTN_SIZE,SYS_BTN_SIZE,
				RGB(0,255,0));
		}
	}//end if m_nStyle!=NONE



	CString strTitle;
	GetWindowText(strTitle);

	int nTitleWidth = dcMem.GetTextExtent(strTitle).cx+45;
	int nTitleLeft  = 3;

	dcTemp.SelectObject(&m_bmpTitleLeft);
	TransparentBlt(dcMem.m_hDC,
		nTitleLeft,2,4,22,
		dcTemp.m_hDC,
		0,0,4,22,
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
		nTitleLeft+nTitleWidth-4,2,4,22,
		dcTemp.m_hDC,
		0,0,4,22,
		RGB(0,255,0));
    
	dcMem.SetBkMode(TRANSPARENT);
	CFont* pOldFontMem = dcMem.SelectObject(&m_fontCaption);

	//DrawBitmap(&dcMem,9,nTop+2,&m_bmpTitleIcon,RGB(0,255,0));
	//dcMem.DrawIcon(9,nTop+2,m_hIcon);
	DrawIconEx(dcMem.m_hDC, 9, 4, m_hIcon, 17, 17, 0, NULL, DI_NORMAL);

	dcMem.SetTextColor(RGB(255,255,255));

	int nTextLeft = 30;
	int nTextTop  = 5;


	if ( nTitleWidth>cx-65 )
	{
		CRect rcText;
		rcText.left = nTextLeft;
		rcText.top  = nTextTop;
		rcText.bottom = rcText.top+25;
		rcText.right  = cx-65;

		dcMem.SetTextColor(RGB(0,84,166));
		dcMem.DrawText(strTitle,&rcText,DT_TOP|DT_LEFT|DT_END_ELLIPSIS);
	}
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

		dcMem.SetTextColor(RGB(0,84,166));
		dcMem.TextOut(nTextLeft,nTextTop,strTitle);
	}

	pDC->BitBlt(2,2,cx-4,nCaptionHeight,&dcMem,0,0,SRCCOPY);

	m_rcClose.MoveToXY(m_rcClose.left+2,m_rcClose.top+2);
	m_rcMin.MoveToXY(m_rcMin.left+2,m_rcMin.top+2);
	m_rcMax.MoveToXY(m_rcMax.left+2,m_rcMax.top+2);

	dcMem.RestoreDC(nSavedMemDC);
	dcTemp.RestoreDC(nSavedTempDC);

	ReleaseDC(pDC);
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
	{	pDC->FillSolidRect(1,TITLE_BAR_CY,nCx-1,cy-TITLE_BAR_CY,BACKGND_COLOR);
		pDC->FillSolidRect(cx-nCx,TITLE_BAR_CY,nCx-1,cy-TITLE_BAR_CY,BACKGND_COLOR);
	}
		
	if ( nCy>0 )
	{
		pDC->FillSolidRect(0,cy-nCy,cx,nCy-1,BACKGND_COLOR);
	}

	pDC->Draw3dRect(0,0,cx,cy,BORDER_COLOR,BORDER_COLOR);
	pDC->Draw3dRect(1,1,cx-2,cy-2,RGB(255,255,253),RGB(255,255,253));
	pDC->SetPixel(0,0,RGB(162,208,224));
	pDC->SetPixel(cx-1,0,RGB(162,208,224));
	pDC->SetPixel(cx-1,cy-1,RGB(162,208,224));
	pDC->SetPixel(0,cy-1,RGB(162,208,224));

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


void CXSkinDlg::ShowToolTip( CString strTip)
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

	CRect rect; 
	GetClientRect(rect);
	m_ToolTip.AddTool(this, strTip, rect, 1);

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
		m_bMaxed=false;
		ShowWindow(SW_RESTORE);
		Invalidate();
	}
	else
	{
		m_bMaxed=true;
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
	if ( m_rcMin.PtInRect(point) || m_rcClose.PtInRect(point) || m_rcMax.PtInRect(point) || m_rcHelp.PtInRect(point) )
	{
		m_bMouseDown = true;
		SetTimer(0xFFFE,100,NULL);
		DrawCaptionBar();
	}

	if ( m_nStyle==ALL && m_bNcMouseDown==false )
	{
		m_lNcTest = NcHitTest();

		if ( m_lNcTest!=HTERROR )
		{
			m_bNcMouseDown = true;
			SetCapture();
		}
	}

	UpdateCursor();

	CDialog::OnLButtonDown(nFlags, point);
}

void CXSkinDlg::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CDialog::OnLButtonUp(nFlags, point);

	m_iBtnDown = -1;

	m_bMouseDown = false;

	if ( m_bNcMouseDown )
	{
		m_bNcMouseDown = false;
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
}

BOOL CXSkinDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon,FALSE);

	CRect rc(0,0,10,10);
	m_wndTitle.Create(_T("AAA"),0,rc,this);

	EnableToolTips(TRUE);
	InitToolTip();

	UpdateWindowLong();

	InitMinMaxInfo();

	EnumChildWindows(this->m_hWnd,(WNDENUMPROC)EnumChildProc,(LPARAM)this);

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

	pt.x -= rcWindow.left;
	pt.y -= rcWindow.top;

	rcWindow.right-=rcWindow.left;
	rcWindow.left=0;
	rcWindow.bottom-=rcWindow.top;
	rcWindow.top=0;

	bool bRedrawCaption = false;

	if ( m_rcClose.PtInRect(pt)  )
	{
		if ( m_iCurBtn!=CLOSE_BTN )
		{
			ShowToolTip(_T("关闭"));
			m_iCurBtn = CLOSE_BTN;
			bRedrawCaption = true;
		}
	}
	else
	if ( m_iCurBtn==CLOSE_BTN )
	{
		ShowToolTip(_T(""));
	}

	if ( m_rcMin.PtInRect(pt) )
	{
		if ( m_iCurBtn!=MIN_BTN )
		{
			ShowToolTip(_T("最小化"));
			m_iCurBtn = MIN_BTN;
			bRedrawCaption = true;
		}
	}
	else
	if ( m_iCurBtn==MIN_BTN )
	{
		ShowToolTip(_T(""));
	}

	if ( m_rcHelp.PtInRect(pt) )
	{
		if ( m_iCurBtn!=HELP_BTN )
		{
			ShowToolTip(_T("帮助"));
			m_iCurBtn = HELP_BTN;
			bRedrawCaption = true;
		}
	}
	else
	if ( m_iCurBtn==HELP_BTN )
	{
		ShowToolTip(_T(""));
	}

	if ( m_rcMax.PtInRect(pt) )
	{
		if ( m_iCurBtn!=MAX_BTN )
		{
			if(m_bMaxed)
			{
				ShowToolTip(_T("恢复"));		
			}
			else
			{
				ShowToolTip(_T("最大化"));
			}

			m_iCurBtn = MAX_BTN;
			bRedrawCaption = true;	
		}		
	}
	else
	if ( m_iCurBtn==MAX_BTN )
	{
		ShowToolTip(_T(""));
	}


	if ( bRedrawCaption )
	{
		SetTimer(0xFFFE,100,NULL);
		DrawCaptionBar();
	}

	UpdateCursor();

	if ( m_bNcMouseDown )
	{
		CRect rcWnd;
		GetWindowRect(&rcWnd);
		
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

		if ( nDestCx>25 && nDestCy>80 &&
			   nDestCx>m_objMinMaxInfo.ptMinTrackSize.x &&
			   nDestCy>m_objMinMaxInfo.ptMinTrackSize.y &&
			   nDestCx<m_objMinMaxInfo.ptMaxTrackSize.x &&
			   nDestCy<m_objMinMaxInfo.ptMaxTrackSize.y 
			)
		{
			MoveWindow(&rcWnd);				
		}
		else
		{
			if ( nDestCx > m_objMinMaxInfo.ptMaxTrackSize.x )
			{
				if ( m_lNcTest==HTRIGHT || m_lNcTest==HTBOTTOMRIGHT || m_lNcTest==HTTOPRIGHT )
				{
					rcWnd.right = rcWnd.left + m_objMinMaxInfo.ptMaxTrackSize.x;
				}
				else
				{
					rcWnd.left = rcWnd.right - m_objMinMaxInfo.ptMaxTrackSize.x;
				}
			}

			if ( nDestCy > m_objMinMaxInfo.ptMaxTrackSize.y )
			{
				if ( m_lNcTest==HTBOTTOM || m_lNcTest==HTBOTTOMRIGHT || m_lNcTest==HTBOTTOMLEFT )
				{
					rcWnd.bottom = rcWnd.top + m_objMinMaxInfo.ptMaxTrackSize.y;
				}
				else
				{
					rcWnd.top = rcWnd.bottom - m_objMinMaxInfo.ptMaxTrackSize.y;
				}
			}
			
			if ( nDestCx < m_objMinMaxInfo.ptMinTrackSize.x )
			{
				if ( m_lNcTest==HTRIGHT || m_lNcTest==HTBOTTOMRIGHT || m_lNcTest==HTTOPRIGHT )
				{
					rcWnd.right = rcWnd.left + m_objMinMaxInfo.ptMinTrackSize.x;
				}
				else
				{
					rcWnd.left = rcWnd.right - m_objMinMaxInfo.ptMinTrackSize.x;
				}
			}

			if ( nDestCy < m_objMinMaxInfo.ptMinTrackSize.y )
			{
				if ( m_lNcTest==HTBOTTOM || m_lNcTest==HTBOTTOMRIGHT || m_lNcTest==HTBOTTOMLEFT )
				{
					rcWnd.bottom = rcWnd.top + m_objMinMaxInfo.ptMinTrackSize.y;
				}
				else
				{
					rcWnd.top = rcWnd.bottom - m_objMinMaxInfo.ptMinTrackSize.y;
				}
			}

			MoveWindow(&rcWnd);		
		}

		
		ScreenToClient(&rcInter);
		InvalidateRect(&rcInter);

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
	DrawBorder();
	DrawCaptionBar();
}

void CXSkinDlg::OnNcPaint()
{
	DrawBorder();
	DrawCaptionBar();
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
		m_bMaxed = false;
	}
	else
	{
		ShowWindow(SW_MAXIMIZE);
		m_bMaxed = true;
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

	m_hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(nID));

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
	MINMAXINFO * lpMMI = &this->m_objMinMaxInfo;

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

	MINMAXINFO * lpMMI = &this->m_objMinMaxInfo;

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
