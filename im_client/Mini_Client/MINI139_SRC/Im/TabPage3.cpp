// im\TabPage3.cpp : 实现文件
//

#include "stdafx.h"
#include "..\ImClient.h"
#include "TabPage3.h"
#include ".\tabpage3.h"


// CTabPage3 对话框

IMPLEMENT_DYNAMIC(CTabPage3, CDialog)
CTabPage3::CTabPage3(CWnd* pParent /*=NULL*/)
	: CDialog(CTabPage3::IDD, pParent)
{

}

CTabPage3::~CTabPage3()
{
}

void CTabPage3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabPage3, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CTabPage3 消息处理程序

BOOL CTabPage3::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CTabPage3::OnPaint()
{
	CPaintDC dc(this);
}

//void CTabPage3::DrawTitleBar()
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
//		dc.DrawText("娱乐新鲜报", rDraw, DT_LEFT | DT_SINGLELINE | DT_VCENTER);
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

void CTabPage3::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if ( m_wndFloorCtrl.m_hWnd )
		m_wndFloorCtrl.MoveWindow(0,0,cx,cy);
}

void CTabPage3::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if ( bShow )
		SetTimer(0x01,100,NULL);
}

BOOL CTabPage3::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect(0,0,0,0);
	m_wndFloorCtrl.Create(this,rect);
	m_wndFloorCtrl.AddPage("娱乐新鲜报");

	m_pageWeb.Create(CFloorWebPage::IDD,this);
	m_wndFloorCtrl.AttachWindow(0,&m_pageWeb);

	return TRUE;  
}

void CTabPage3::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		KillTimer(0x01);

		CWnd *p = m_wndFloorCtrl.GetActivePage()->GetWindow();
		((CFloorWebPage *)p)->InitWebPage();	
	}

	CDialog::OnTimer(nIDEvent);
}
