// im\AdvertisePanel.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "AdvertisePanel.h"
#include ".\advertisepanel.h"

#define  SEARCH_HINT "站内搜索，在此输入关键字..."
// CAdvertisePanel 对话框

IMPLEMENT_DYNAMIC(CAdvertisePanel, CDialog)
CAdvertisePanel::CAdvertisePanel(CWnd* pParent /*=NULL*/)
	: CDialog(CAdvertisePanel::IDD, pParent)
{
	m_bNavigated=false;
	m_bNavigateError=false;
}

CAdvertisePanel::~CAdvertisePanel()
{
}

BEGIN_MESSAGE_MAP(CAdvertisePanel::CAdvStatic, CStatic)
	ON_WM_PAINT()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

void CAdvertisePanel::CAdvStatic::OnPaint()
{
	CPaintDC dc(this);
	if ( m_bEnableDraw )
	{
		CResBitmap bmp;
		bmp.LoadBitmap(IDB_DEFAULT_ADV);
		::DrawBitmap(&dc,0,0,&bmp,0);
	}
}

void CAdvertisePanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_wndExplore);
	DDX_Control(pDX, IDC_BUTTON_SEARCH,m_btnSearch);
	DDX_Control(pDX, IDC_EDIT_CONTENT,m_edtContent);
	DDX_Control(pDX, IDC_STATIC_FRAME,m_wndFrame);
	DDX_Control(pDX, IDC_COMBO_TYPE,m_combSearchType);
}


BEGIN_MESSAGE_MAP(CAdvertisePanel, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_WM_CTLCOLOR()
	ON_STN_CLICKED(IDC_STATIC_FRAME, OnStnClickedStaticFrame)
	ON_EN_SETFOCUS(IDC_EDIT_CONTENT, OnEnSetfocusEditContent)
	ON_EN_KILLFOCUS(IDC_EDIT_CONTENT, OnEnKillfocusEditContent)
	ON_BN_CLICKED(IDC_BUTTON_SEARCH, OnBnClickedButtonSearch)
END_MESSAGE_MAP()


// CAdvertisePanel 消息处理程序

BOOL CAdvertisePanel::OnInitDialog()
{
	CDialog::OnInitDialog();

	//m_edtContent.ShowWindow(SW_HIDE);
	//m_btnSearch.ShowWindow(SW_HIDE);

	// TODO:  在此添加额外的初始化
	m_btnSearch.SetBitmap(IDB_BTN_SEARCH,IDB_BTN_SEARCH);
	m_btnSearch.SetToolTip("搜索文章、图片、音乐、视频");
	m_combSearchType.SetBorderColor(0xAAAAAA);
	m_edtContent.SetBorderColor(0xAAAAAA);
	m_edtContent.SetTextColor(0xAAAAAA);
	m_edtContent.SetWindowText(SEARCH_HINT);

	m_font.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"宋体");

	m_edtContent.SetFont(&m_font);
	m_wndExplore.MoveWindow(0,0,0,0);
	m_wndExplore.EnableRButton(false);

	m_combSearchType.AddString("文章");
	m_combSearchType.AddString("图片");
	m_combSearchType.AddString("音乐");
	m_combSearchType.AddString("视频");
	m_combSearchType.SetCurSel(0);
	return TRUE; 
}

BOOL CAdvertisePanel::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CAdvertisePanel::OnPaint()
{
	CPaintDC dc(this); 

	int nSaveDC = dc.SaveDC();

	CRect rc1,rc2;
	GetClientRect(&rc1);

	m_wndFrame.GetWindowRect(&rc2);
	ScreenToClient(&rc2);

	CRgn rgn1, rgn2, rgn3;
	rgn1.CreateRectRgn(rc1.left,rc1.top,rc1.right,rc1.bottom);
	rgn2.CreateRectRgn(rc2.left,rc2.top,rc2.right,rc2.bottom);
	rgn3.CreateRectRgn(0,0,0,0);

	rgn3.CombineRgn(&rgn1,&rgn2,RGN_DIFF);
	dc.SelectObject(&rgn3);
	dc.FillSolidRect(&rc1,IM_SPACE_COLOR);

	dc.RestoreDC(nSaveDC);
}

void CAdvertisePanel::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		KillTimer(nIDEvent);

		m_wndFrame.EnableDraw(false);

		m_wndExplore.SetParent(&m_wndFrame);
		m_wndExplore.MoveWindow(-2,-2,250,80);
		m_wndFrame.Invalidate();
		m_wndExplore.Invalidate();
	}
	
	CDialog::OnTimer(nIDEvent);
}
void CAdvertisePanel::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if ( m_wndExplore.m_hWnd )
	{
		static int nPreCx=0;

		if ( cx<nPreCx )
		{
			CRect rcContent, rcType, rcSearch;
			m_edtContent.GetWindowRect(&rcContent);

			ScreenToClient(&rcContent);
			rcContent.right = cx-73;

			rcType = rcContent;
			
			rcType.left  = rcContent.right-1;
			rcType.right = rcType.left+51;

			m_edtContent.MoveWindow(&rcContent);
			m_combSearchType.MoveWindow(&rcType);
			
			m_btnSearch.GetWindowRect(&rcSearch);
			ScreenToClient(&rcSearch);
			rcSearch.left = cx-21;
			rcSearch.right = rcSearch.left+19;	
			m_btnSearch.MoveWindow(&rcSearch);
		}
		else
		{		
			CRect rcContent, rcType, rcSearch;
			m_edtContent.GetWindowRect(&rcContent);

			
			m_btnSearch.GetWindowRect(&rcSearch);
			ScreenToClient(&rcSearch);
			rcSearch.left = cx-21;
			rcSearch.right = rcSearch.left+19;	
			m_btnSearch.MoveWindow(&rcSearch);

			ScreenToClient(&rcContent);
			rcContent.right = cx-73;

			rcType = rcContent;
			
			rcType.left  = rcContent.right-1;
			rcType.right = rcType.left+51;

			m_combSearchType.MoveWindow(&rcType);
			m_edtContent.MoveWindow(&rcContent);			
		}

		nPreCx = cx;

		int nAdvWidth  = 220;
		int nAdvHeight = 60;
		//广告页面宽度 200 高 60
		int nLeft;
		nLeft = (cx-nAdvWidth)/2;

	
		m_wndFrame.MoveWindow(nLeft,2,nAdvWidth,nAdvHeight);
	}
}

void CAdvertisePanel::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if ( bShow && m_bNavigated==false && g_pWebConfig != NULL)
	{
		CString strAdvertiseUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(MINI_ADVERTISE,L"");
		strAdvertiseUrl.Trim();

		if ( strAdvertiseUrl!="" )
			m_wndExplore.Navigate(strAdvertiseUrl,NULL,NULL,NULL,NULL);
	}
}

HBRUSH CAdvertisePanel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( pWnd->GetDlgCtrlID()==IDC_STATIC1 )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr =::g_hBackBrush;
	}

	return hbr;
}

BOOL CAdvertisePanel::CAdvStatic::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	::SetCursor(m_hCursor);
	return FALSE;
}

void CAdvertisePanel::OnStnClickedStaticFrame()
{
	OpenHtmlPage("http://www.feiniu.com");
}

BEGIN_EVENTSINK_MAP(CAdvertisePanel, CDialog)
	ON_EVENT(CAdvertisePanel, IDC_EXPLORER1, 271, NavigateErrorExplorer1, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CAdvertisePanel, IDC_EXPLORER1, 252, NavigateComplete2Explorer1, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

void CAdvertisePanel::NavigateErrorExplorer1(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Frame, VARIANT* StatusCode, BOOL* Cancel)
{
	m_bNavigateError=true;
}

void CAdvertisePanel::NavigateComplete2Explorer1(LPDISPATCH pDisp, VARIANT* URL)
{
	if ( m_bNavigateError==false )
	{
		SetTimer(0x01,1000,NULL);
	}
}

void CAdvertisePanel::OnEnSetfocusEditContent()
{
	CString str;
	m_edtContent.GetWindowText(str);
	str.Trim();

	if ( str==SEARCH_HINT )
	{
		m_edtContent.SetTextColor(RGB(0,0,0));
		m_edtContent.SetWindowText("");
	}
}

void CAdvertisePanel::OnEnKillfocusEditContent()
{
	CString str;
	m_edtContent.GetWindowText(str);
	str.Trim();

	if ( str=="" )
	{
		m_edtContent.SetTextColor(0xAAAAAA);
		m_edtContent.SetWindowText(SEARCH_HINT);
	}
}

void CAdvertisePanel::OnOK()
{
	OnBnClickedButtonSearch();
}

void CAdvertisePanel::OnBnClickedButtonSearch()
{
	CString str;
	m_edtContent.GetWindowText(str);
	str.Trim();
	if ( str!=SEARCH_HINT && str!="" )
	{
		CString strSearch;
		strSearch.Format("http://s.feiniu.com/search.php?k=%s&t=%d",UrlEncode(str),m_combSearchType.GetCurSel());
		OpenHtmlPage(strSearch);
	}
}
