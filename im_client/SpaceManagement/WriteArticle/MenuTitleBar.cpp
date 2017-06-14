// MenuTitleBar.cpp : 实现文件
//

#include "stdafx.h"

#include "MenuTitleBar.h"
#include ".\menutitlebar.h"
#include "resource.h"
#define WM_MENUITEM_CLICK WM_USER+1
// CMenuTitleBar

IMPLEMENT_DYNAMIC(CMenuTitleBar, CWnd)
CMenuTitleBar::CMenuTitleBar()
{
	m_nHotItem = -1;
	m_nSelItem = -1;
	m_pPopMenu = NULL;
	m_bMouseOver = false;
	m_bIsPopup=FALSE;
}

CMenuTitleBar::~CMenuTitleBar()
{
}


BEGIN_MESSAGE_MAP(CMenuTitleBar, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_MENUITEM_CLICK,OnMenuItemClick)
	ON_WM_MEASUREITEM()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()



// CMenuTitleBar 消息处理程序
int CMenuTitleBar::AddTitle(UINT nMenu, CString strTitle)
{
	tagItemInfo info;

	info.nMenu    = nMenu;
	info.strTitle = strTitle;

	m_vecItems.push_back(info);

	if ( m_hWnd ) 
		Invalidate();

	return m_vecItems.size()-1;
}

int CMenuTitleBar::DeleteTitle(int nItem)
{
	if ( nItem>-1 && nItem<m_vecItems.size() )
	{
		m_vecItems.erase(m_vecItems.begin()+nItem);
	}

	if ( m_hWnd ) 
		Invalidate();

	return 0;
}

int CMenuTitleBar::GetCount()
{
	return m_vecItems.size();
}

int CMenuTitleBar::HitTest(CPoint point)
{
	int nCount = m_vecItems.size();
	for ( int n=0; n<nCount; n++ )
	{
		if ( m_vecItems[n].rcItem.PtInRect(point) )
			return n;
	}

	return -1;
}

BOOL CMenuTitleBar::Create(CWnd *pParent, CRect rc, DWORD dwStyle, UINT nID)
{
	return CWnd::Create( AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS),
		"CMenuTitleBar",
		dwStyle,
		rc,pParent,
		nID);
}

BOOL CMenuTitleBar::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

LRESULT CMenuTitleBar::OnMenuItemClick(WPARAM wParam, LPARAM lParam)
{
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	return 0;
}

void CMenuTitleBar::OnPaint()
{
	CPaintDC dc(this); 

	CRect rcClient;
	GetClientRect(&rcClient);

	int nWidth  = rcClient.Width();
	int nHeight = rcClient.Height();

	CDC dcMem;
	CBitmap bmpMem;

	dcMem.CreateCompatibleDC(&dc);
	bmpMem.CreateCompatibleBitmap(&dc,nWidth,nHeight);

	int nSavedDC = dcMem.SaveDC();

	dcMem.SelectObject(&bmpMem);
	dcMem.SelectObject(&m_Font);

	dcMem.SetBkMode(TRANSPARENT);

	//dcMem.FillSolidRect(&rcClient,RGB(233,222,222));
	dcMem.FillSolidRect(&rcClient,IM_SPACE_COLOR);

	int nLeft  = 0;
	int nCount = m_vecItems.size();

	for ( int nItem =0; nItem<nCount; nItem++ )
	{
		tagItemInfo & info = m_vecItems[nItem];

		CSize szText = dcMem.GetTextExtent(info.strTitle);

		info.rcItem.left   = nLeft;
		info.rcItem.top    = rcClient.top;
		info.rcItem.right  = nLeft + szText.cx + 10;
		info.rcItem.bottom = rcClient.bottom;

		if ( nItem==m_nHotItem )
		{
			dcMem.FillSolidRect(&info.rcItem,RGB(100,200,255));
			dcMem.Draw3dRect(&info.rcItem,RGB(0,0,255),RGB(0,0,255));
		}

		int nTextTop = (nHeight-szText.cy)/2;
		dcMem.TextOut(nLeft+5,nTextTop,info.strTitle);

		nLeft = info.rcItem.right;
	}

	dc.BitBlt(0,0,nWidth,nHeight,&dcMem,0,0,SRCCOPY);

	dcMem.RestoreDC(nSavedDC);

}

void CMenuTitleBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int nSelItem;
	
	 nSelItem = HitTest(point);

	if( nSelItem!=-1)
	{
       m_bIsPopup=FALSE;
	}
	
	if (nSelItem==-1)   
	{
		m_bIsPopup=FALSE; 
		return ;
	}
	if (m_bIsPopup&&nSelItem==m_nSelItem)
	{


		m_bIsPopup=FALSE;
		return ;
	}

	if ( nSelItem!=-1 )
	{
		tagItemInfo &info = m_vecItems[nSelItem];

		m_nSelItem = nSelItem;
		SetTimer(0x02,1,NULL);

		CMenuEx menu;
		menu.LoadMenu(info.nMenu);
				
		m_pPopMenu = menu.GetSubMenu(0);
		//m_pPopMenu->RemoveMenu(0,MF_BYPOSITION);
		CPoint pt;
		pt.x = info.rcItem.left;
		pt.y = info.rcItem.bottom;
		ClientToScreen(&pt);
		
		if ( nSelItem==1 )//编辑 
		{
			if ( m_pEditCtrl->QueryStatus(CDHtmlEditCtrl::DECMD_CUT)!=CDHtmlEditCtrl::DECMDF_ENABLED )
			{
				m_pPopMenu->EnableMenuItem(ID_CUT,MF_DISABLED);
			}	
			if ( m_pEditCtrl->QueryStatus(CDHtmlEditCtrl::DECMD_COPY)!=CDHtmlEditCtrl::DECMDF_ENABLED )
			{
				m_pPopMenu->EnableMenuItem(ID_COPY,MF_DISABLED);
			}	
			if ( m_pEditCtrl->QueryStatus(CDHtmlEditCtrl::DECMD_PASTE)!=CDHtmlEditCtrl::DECMDF_ENABLED )
			{
				m_pPopMenu->EnableMenuItem(ID_PASTE,MF_DISABLED);
			}
			if ( m_pEditCtrl->QueryStatus(CDHtmlEditCtrl::DECMD_SELECTALL)!=CDHtmlEditCtrl::DECMDF_ENABLED )
			{
				m_pPopMenu->EnableMenuItem(ID_SELALL,MF_DISABLED);
			}
			if ( m_pEditCtrl->QueryStatus(CDHtmlEditCtrl::DECMD_UNDO)!=CDHtmlEditCtrl::DECMDF_ENABLED )
			{
				m_pPopMenu->EnableMenuItem(ID_UNDO,MF_DISABLED);
			}
			if ( m_pEditCtrl->QueryStatus(CDHtmlEditCtrl::DECMD_REDO)!=CDHtmlEditCtrl::DECMDF_ENABLED )
			{
				m_pPopMenu->EnableMenuItem(ID_REDO,MF_DISABLED);
			}
		}

		m_pPopMenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
		m_pPopMenu = NULL;
		m_bIsPopup=TRUE;
		
		Invalidate();
		//KillTimer(0x02,10,NULL);
	}


	CWnd::OnLButtonDown(nFlags, point);
}

void CMenuTitleBar::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( m_bMouseOver==false )
	{
		SetTimer(0x01,1,NULL);
		m_bMouseOver = true;
	}

	int nHotItem = HitTest(point);
	if ( m_nHotItem!=nHotItem )
	{
		Invalidate(TRUE);
		m_nHotItem = nHotItem;
	}

	CWnd::OnMouseMove(nFlags, point);
}

int CMenuTitleBar::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_Font.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL, // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"宋体");

	return 0;
}

void CMenuTitleBar::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( nIDEvent==0x01 )
	{
		if ( m_bMouseOver && m_pPopMenu==NULL )
		{
			CRect rcWnd;
			GetWindowRect(&rcWnd);

			CPoint pt;
			GetCursorPos(&pt);

			if ( rcWnd.PtInRect(pt)==FALSE )
			{
				m_bMouseOver=false;
				m_nHotItem  = -1;
				KillTimer(0x01);
				Invalidate();
			}
		}
	}


	if ( nIDEvent==0x02 )
	{
		if ( m_pPopMenu )
		{
			CPoint pt;
			GetCursorPos(&pt);
			ScreenToClient(&pt);

			int nItem = HitTest(pt);

			if ( nItem!=-1 && m_nSelItem!=nItem )
			{
				m_nHotItem = nItem;
				Invalidate();

				KillTimer(0x02);
				mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0); 
				mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
			}
		}
		else
		{
			KillTimer(0x02);
		}
	}

	CWnd::OnTimer(nIDEvent);
}

BOOL CMenuTitleBar::PreTranslateMessage(MSG* pMsg)
{
	
	if ( pMsg->message==WM_COMMAND )
		GetParent()->PostMessage(WM_COMMAND,pMsg->wParam,pMsg->lParam);

	return CWnd::PreTranslateMessage(pMsg);
}

void CMenuTitleBar::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CWnd::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
	CMenuEx::MeasureItem(lpMeasureItemStruct);
}

LRESULT CMenuTitleBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	

	return CWnd::WindowProc(message, wParam, lParam);
}

void CMenuTitleBar::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);

	if ( nState==WA_CLICKACTIVE )
	{
		mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0); 
		mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 
	}
}
