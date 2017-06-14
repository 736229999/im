// im\TabWndCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\tabwndctrl.h"

#define TAB_BTN_CX 0
#define TAB_BTN_CY 40
//#define TAB_LINE_COLOR  RGB(146,184,220)
#define TAB_LINE_COLOR  RGB(248,248,248)

// CTabWndCtrl 对话框

IMPLEMENT_DYNAMIC(CTabWndCtrl, CDialog)
CTabWndCtrl::CTabWndCtrl(CWnd* pParent /*=NULL*/)
	: CDialog(CTabWndCtrl::IDD, pParent)
{
	m_nCurSel   = -1;
	m_nHotTab   = -1;
	m_nPreHotTab = -1;
	m_bMoseOver  = false;
}

CTabWndCtrl::~CTabWndCtrl()
{
}

void CTabWndCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	
	
}


BEGIN_MESSAGE_MAP(CTabWndCtrl, CDialog)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CTabWndCtrl 消息处理程序

BOOL CTabWndCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	m_Tooltip.Create( this , TTS_ALWAYSTIP ) ;
	m_Tooltip.Activate( TRUE ) ;
	m_Tooltip.AddTool( this  , _T("") ) ;

	return TRUE;  
}


int  CTabWndCtrl::AddTab(CWnd *pWnd,CString strToolTip,UINT nBitmap)
{
	if ( pWnd->IsWindowVisible() )
		pWnd->ShowWindow(SW_HIDE);

	pWnd->SetParent(this);

	tagTabInfo info;
	info.pPage = pWnd;
	info.strToolTip = strToolTip;
	info.nBitmap = nBitmap;

	m_vecTab.push_back(info);

	int nItem = m_vecTab.size()-1;

	if ( m_nCurSel==-1 )
		m_nCurSel=nItem;

	return nItem;
}

void CTabWndCtrl::SetCurSel(int nSel)
{
	if ( m_nCurSel!=nSel && m_hWnd )
	{
		CRect rc;
		GetClientRect(&rc);

		if ( m_nCurSel!=-1 )
			m_vecTab[m_nCurSel].pPage->ShowWindow(SW_HIDE);

		rc.left+=TAB_BTN_CX;
		rc.InflateRect(-1,-1);
		rc.left++;

		CWnd *pTabToShow = m_vecTab[nSel].pPage;
		pTabToShow->MoveWindow(&rc);

		if ( !pTabToShow->IsWindowVisible() )
		{
			pTabToShow->ShowWindow(SW_SHOW);
		}

		pTabToShow->BringWindowToTop();

		m_nCurSel=nSel;

		//Invalidate();
		InvalidateRect(&m_rcTab);
	}
}

int  CTabWndCtrl::GetCurSel()
{
	return m_nCurSel;
}

CWnd * CTabWndCtrl::GetTabSheet(int nTab)
{
	return m_vecTab[nTab].pPage;
}

void CTabWndCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{

	CDialog::OnLButtonDown(nFlags, point);
}

void CTabWndCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	CDialog::OnLButtonUp(nFlags, point);

	int nTab = TabHitTest(point);

	if ( nTab!=-1 )
	{
		SetCurSel(nTab);
		NMHDR nmhr;
		nmhr.hwndFrom = m_hWnd;
		nmhr.code     = nTab;
		GetParent()->SendMessage(WM_NOTIFY,this->GetDlgCtrlID(),(LPARAM)&nmhr);
	}
}

int CTabWndCtrl::TabHitTest(const CPoint &pt)
{
	int nTabs = m_vecTab.size();

	CRect rcTab;

	rcTab.left  = 0;
	rcTab.right = TAB_BTN_CX;

	for ( int nTab=0; nTab<nTabs; nTab++ )
	{
		rcTab.top    = nTab*TAB_BTN_CY;
		rcTab.bottom = rcTab.top+TAB_BTN_CY;

		if ( rcTab.PtInRect(pt) )
		{
			return nTab;
		}
	}

	return -1;
}

BOOL CTabWndCtrl::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CTabWndCtrl::OnSize(UINT nType, int cx, int cy)
{
	if ( m_nCurSel!=-1 && m_hWnd )
	{
		CWnd *pTabToShow = m_vecTab[m_nCurSel].pPage;

		CRect rc;
		rc.SetRect(TAB_BTN_CX,0,cx,cy);
		rc.InflateRect(-1,-1);
		rc.left++;
		m_rcTab.SetRect(0,0,TAB_BTN_CX+2,cy);

		pTabToShow->MoveWindow(&rc);
	}

	CDialog::OnSize(nType, cx, cy);
}

void CTabWndCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( m_bMoseOver==false )
	{
		m_bMoseOver=true;
		SetTimer(0x01,10,NULL);
	}
		
	if (m_nPreHotTab!=m_nHotTab && m_nHotTab!=-1 )
	{
		CString strToolTip;
		strToolTip   = m_vecTab[m_nHotTab].strToolTip;

		m_Tooltip.Activate( FALSE );
		m_Tooltip.Activate( TRUE ) ;
		m_Tooltip.AddTool( this , strToolTip) ;
		
		MSG msg;
		msg.hwnd = m_hWnd;
		msg.message = WM_MOUSEMOVE;
		msg.wParam = nFlags;
		msg.lParam = point.x + ((point.y)<<8);
		msg.time = GetTickCount();
		POINT tmpPoint;
		GetCursorPos(&tmpPoint);
		ClientToScreen( &tmpPoint );
		msg.pt= tmpPoint;
		m_Tooltip.RelayEvent( &msg );
	};

	InvalidateRect(&m_rcTab);
//	Invalidate();

	m_nPreHotTab = m_nHotTab;

	CDialog::OnMouseMove(nFlags, point);
}

void CTabWndCtrl::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		CPoint pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);

		if ( !m_rcTab.PtInRect(pt) )
		{
			KillTimer(nIDEvent);
			m_bMoseOver=false;

			InvalidateRect(&m_rcTab);
		}
	}

	CDialog::OnTimer(nIDEvent);
}

void CTabWndCtrl::OnPaint()
{
	CPaintDC dc(this); 

	CRect rc;
	GetClientRect(&rc);

	CResBitmap bmpNormal0,bmpNormal1,bmpNormal2, bmpSel;

	bmpNormal0.LoadBitmap(IDB_TAB_NORMAL0);
	bmpNormal1.LoadBitmap(IDB_TAB_NORMAL1);
	bmpNormal2.LoadBitmap(IDB_TAB_NORMAL2);

	bmpSel.LoadBitmap(IDB_TAB_SEL);

	CPoint pt;
	GetCursorPos(&pt);

	ScreenToClient(&pt);

	int nTabHot   = TabHitTest(pt);
	int nTabCount = m_vecTab.size();

	CDC dcTemp;
	dcTemp.CreateCompatibleDC(&dc);
	int nSavedTemp = dcTemp.SaveDC();

	CDC dcIcon;
	dcIcon.CreateCompatibleDC(&dc);
	int nSavedIcon = dcIcon.SaveDC();

	CDC dcBack;
	dcBack.CreateCompatibleDC(&dc);
	int nSavedBack = dcBack.SaveDC();

	CBitmap bmpBack;
	bmpBack.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height());

	dcBack.SelectObject(&bmpBack);
	dcBack.FillSolidRect(0,0,rc.Width(),rc.Height(),0x00FF00);

	//画Tab背景
	//{
	//	dcTemp.SelectObject(&bmpNormal);
	//	dcBack.TransparentBlt(2,0,TAB_BTN_CX,4,&dcTemp,0,0,TAB_BTN_CX,4,RGB(0,255,0));
	//	int nHeight = TAB_BTN_CY*nTabCount-8;
	//	dcBack.StretchBlt(3,4,TAB_BTN_CX,nHeight,&dcTemp,0,4,TAB_BTN_CX,TAB_BTN_CY-9,SRCCOPY);
	//	dcBack.TransparentBlt(2,nHeight+4,TAB_BTN_CX,4,&dcTemp,0,TAB_BTN_CY-4,TAB_BTN_CX,4,RGB(0,255,0));
	//}

	int nSelTop = 0;

	for ( int nTab=(nTabCount-1); nTab>=0; nTab-- )
	{
		int nTop = nTab*TAB_BTN_CY;

		if ( nTab==m_nCurSel )
		{
			nSelTop = nTop;
			dcTemp.SelectObject(&bmpSel);
			//dcBack.TransparentBlt(0,nTop,TAB_BTN_CX,TAB_BTN_CY,&dcTemp,0,0,TAB_BTN_CX,TAB_BTN_CY,RGB(0,255,0));
		}
		else
		{

			if ( nTab==(nTabCount-1) )
				dcTemp.SelectObject(&bmpNormal2);
			else
			if ( nTab==0 )
				dcTemp.SelectObject(&bmpNormal0);
			else
				dcTemp.SelectObject(&bmpNormal1);

			//dcBack.TransparentBlt(3,nTop,TAB_BTN_CX-3,TAB_BTN_CY,&dcTemp,0,0,TAB_BTN_CX-3,TAB_BTN_CY,RGB(0,255,0));
		}

		//if ( nTab==m_nHotTab )
		//{

			//CRect rcBtn ;
			//rcBtn.top = nTab*TAB_BTN_CY;

	
			//rcBtn.left = 5;
			//rcBtn.right = rcBtn.left+TAB_BTN_CX;
			//rcBtn.bottom = rcBtn.top + TAB_BTN_CY;

			//if ( nTab==0 )
			//	rcBtn.top+=3;

			//if ( nTab==nTabCount-1 )
			//	rcBtn.bottom-=3;

			//dcBack.FillSolidRect(&rcBtn,RGB(233,222,188));
		//}


		CResBitmap bmpIcon;
		bmpIcon.LoadBitmap(m_vecTab[nTab].nBitmap);
		dcIcon.SelectObject(&bmpIcon);

		BITMAP bi;
		bmpIcon.GetBitmap(&bi);

		int nIconLeft = 5+(TAB_BTN_CX-3-bi.bmWidth)/2;
		int nIconTop  = (TAB_BTN_CY-bi.bmHeight)/2+nTab*TAB_BTN_CY;


		if ( nTab==m_nHotTab && nTab!=m_nCurSel )
		{
			nIconLeft--;
			nIconTop--;
		}

		//dcBack.TransparentBlt(nIconLeft,nIconTop,bi.bmWidth,bi.bmHeight,&dcIcon,0,0,bi.bmWidth,bi.bmHeight,RGB(0,255,0));
	}

	m_nHotTab = nTabHot;

	CRgn rgnClient;
	rgnClient.CreateRectRgn(0,0,rc.Width(),rc.Height());

	CRect rcPage;
	m_vecTab[m_nCurSel].pPage->GetWindowRect(rcPage);
	ScreenToClient(&rcPage);

	CRgn rgnPage;
	rgnPage.CreateRectRgn(rcPage.left,rcPage.top,rcPage.right,rcPage.bottom);

	CRgn rgnPaint;
	rgnPaint.CreateRectRgn(0,0,0,0);

	rgnPaint.CombineRgn(&rgnClient,&rgnPage,RGN_DIFF);

	dc.SelectObject(&rgnPaint);

	/* 这是画个框框，先不画
 	CPen *pOldback2 = NULL;
	
	CPen  pen(PS_SOLID,1,TAB_LINE_COLOR);

	pOldback2 = dcBack.SelectObject(&pen);
	
 	dcBack.MoveTo(TAB_BTN_CX+1,nSelTop);
	
	dcBack.LineTo(TAB_BTN_CX+1,0);
	dcBack.LineTo(rc.Width()-1,0);
	dcBack.LineTo(rc.Width()-1,rc.Height()-1);
	dcBack.LineTo(TAB_BTN_CX+1,rc.Height()-1);
	dcBack.LineTo(TAB_BTN_CX+1,nSelTop+TAB_BTN_CY-2);

	dc.BitBlt(0,0,rc.Width(),rc.Height(),&dcBack,0,0,SRCCOPY);
	*/

	dcBack.RestoreDC(nSavedBack);
	dcTemp.RestoreDC(nSavedTemp);
	dcIcon.RestoreDC(nSavedIcon);

	dc.SelectObject(&rgnClient);
//	dcTemp.SelectObject(pOldtemp2);

}




