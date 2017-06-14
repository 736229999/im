// im\TabPage2.cpp : 实现文件
//

#include "stdafx.h"
#include "..\ImClient.h"
#include "TabPage2.h"
#include ".\tabpage2.h"

// CTabPage2 对话框

IMPLEMENT_DYNAMIC(CTabPage2, CDialog)

CTabPage2::CTabPage2(CWnd* pParent /*=NULL*/)
	: CDialog(CTabPage2::IDD, pParent)
{
}

CTabPage2::~CTabPage2()
{
}

void CTabPage2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabPage2, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ACTIVATE()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CTabPage2 消息处理程序

BOOL CTabPage2::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	pDC->FillSolidRect(&rect,RGB(255,255,255));

	return FALSE;
}

void CTabPage2::OnPaint()
{
	CPaintDC dc(this); 
	//DrawTitleBar();
}

BOOL CTabPage2::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rc;
	GetClientRect(rc);

	//m_wndFloorCtrl.Create(this,rc);
	//m_wndFloorCtrl.AddPage("我的空间");

	m_pageWoku.Create(CFloorPageWoku::IDD,this);
	m_pageWoku.ShowWindow(SW_SHOW);

	//m_wndFloorCtrl.AttachWindow(0,&m_pageWoku);

	return TRUE; 
}

void CTabPage2::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if ( m_pageWoku.m_hWnd )
		m_pageWoku.MoveWindow(0,2,cx,cy-2);
	//if ( m_wndFloorCtrl.m_hWnd )
	//	m_wndFloorCtrl.MoveWindow(0,0,cx,cy);
	//if ( m_wndTool.m_hWnd )
	//{
	//	m_wndTool.MoveWindow(0,22,cx-2,cy-22);
	//	DrawTitleBar();
	//}
}

//void CTabPage2::DrawTitleBar()
//{
//	CDC *pDC = GetDC();
//
//	if ( pDC )
//	{
//		CRect rDraw;
//		GetClientRect(&rDraw);
//
//		rDraw.bottom = rDraw.top+22;
//
//		CDC dc ;
//		dc.CreateCompatibleDC(pDC);
//
//		int cx = rDraw.Width();
//		int cy = rDraw.Height();
//
//		CBitmap bm;
//		bm.CreateCompatibleBitmap(pDC,rDraw.Width(),rDraw.Height());
//
//		int nSavedDC = dc.SaveDC();
//
//		dc.SelectObject(&bm);
//
//		dc.FillSolidRect(&rDraw,0xFFFFFF);
//		rDraw.top+=1;
//		rDraw.bottom=rDraw.top+20;
//		
//		rDraw.left+=1;
//		rDraw.right--;
//
//		dc.SetBkMode(TRANSPARENT);
//		dc.SelectObject(&g_fontText);
//
//		CRgn  rgnDraw;
//		rgnDraw.CreateRoundRectRgn(rDraw.left,rDraw.top,rDraw.right,rDraw.bottom,3,3);
//
//		dc.SelectObject(&rgnDraw);
//
//		CResBitmap bmpBar;
//		bmpBar.LoadBitmap(IDB_FLOOR_HOT);
//
//		CDC dcTemp;
//
//		dcTemp.CreateCompatibleDC(&dc);
//		CBitmap *pOldbmp = dcTemp.SelectObject(&bmpBar);
//
//		BITMAP bi;
//		bmpBar.GetBitmap(&bi);
//
//		dc.StretchBlt(rDraw.left,rDraw.top,rDraw.Width(),rDraw.Height(),
//			&dcTemp,0,0,bi.bmWidth,bi.bmHeight,SRCCOPY);
//
//		CPen pen(PS_SOLID,1,RGB(169,204,240));
//		dc.SelectObject(&pen);
//
//		dc.SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));
//		dc.RoundRect(rDraw.left,rDraw.top,rDraw.right-1,rDraw.bottom-1,3,3);
//
//		dc.SetTextColor(RGB(0,84,166));
//
//		rDraw.left+=4;
//		dc.DrawText("我的空间", rDraw, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
//
//
//		dcTemp.SelectObject(pOldbmp);
//
//		pDC->BitBlt(0,0,cx,cy,&dc,0,0,SRCCOPY);
//
//		dc.RestoreDC(nSavedDC);
//		ReleaseDC(pDC);
//	}
//}
//

void CTabPage2::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);
}

void CTabPage2::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if ( bShow )
	{
		this->m_pageWoku.m_wndTool.SetFocus();
		//try
		//{
		//	//CWnd *p = m_wndFloorCtrl.GetActivePage()->GetWindow();
		//	//((CFloorPageWoku *)p)->m_wndTool.SetFocus();
		//}
		//catch(...)
		//{
		//}
	}
}
