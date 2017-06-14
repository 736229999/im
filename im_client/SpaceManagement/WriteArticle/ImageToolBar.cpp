// im\FontToolBar.cpp : 实现文件
//

#include "stdafx.h"
#include "imagetoolbar.h"
#include "resource.h"
#include "afxpriv.h"
#include ".\imagetoolbar.h"

// CImageToolBar
IMPLEMENT_DYNAMIC(CImageToolBar, CToolBar)
CImageToolBar::CImageToolBar()
{
	m_ToolTip.m_hWnd = NULL;
	m_strPreTip		 = "";
}

CImageToolBar::~CImageToolBar()
{
}


BEGIN_MESSAGE_MAP(CImageToolBar, CToolBar)
	ON_WM_NCPAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_IDLEUPDATECMDUI,OnIdleUpdateCmdUI)
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

void CImageToolBar::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		m_ToolTip.Create( this , TTS_ALWAYSTIP ) ;
		m_ToolTip.SetMaxTipWidth(400);
		m_ToolTip.Activate( TRUE ) ;
	} 
}

void CImageToolBar::AddToolTip(UINT nID,CString strTip)
{
	map<UINT,CString>::iterator it;
	it = m_mapToolTipString.find(nID);

	if ( it!=m_mapToolTipString.end() )
	{
		it->second = strTip;
	}
	else
	{
		m_mapToolTipString[nID]=strTip;
	}
}

void CImageToolBar::InitToolBar(SIZE sz, UINT nIDNormal, UINT nIDHot)
{
	CResBitmap bmp;
	bmp.LoadBitmap(nIDNormal);//IDB_TOOL_FONT_NORMAL);
	m_imgToolNormal.Create(sz.cx,sz.cy,ILC_COLOR24,0,1);
	m_imgToolNormal.Add(&bmp,RGB(0,255,0));

	bmp.DeleteObject();

	bmp.LoadBitmap(nIDHot);
	m_imgToolHot.Create(sz.cx,sz.cy,ILC_COLOR24,0,1);
	m_imgToolHot.Add(&bmp,RGB(0,255,0));

	GetToolBarCtrl().SetImageList(&m_imgToolNormal);
	GetToolBarCtrl().SetHotImageList(&m_imgToolHot);
	GetToolBarCtrl().SetDisabledImageList(&m_imgToolNormal);	
}


void CImageToolBar::OnNcPaint()
{
//	CToolBar::OnNcPaint();
}

void CImageToolBar::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CToolBar::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

BOOL CImageToolBar::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	pDC->FillSolidRect(rcClient,IM_SPACE_COLOR);
	pDC->Draw3dRect(rcClient,IM_SPACE_COLOR,IM_SPACE_COLOR);

	return FALSE;
}


void CImageToolBar::ShowToolTip( CString strTip)
{
	if ( strTip==m_strPreTip )
		return ;	

	InitToolTip();
	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() != 0)
	{
		m_ToolTip.DelTool(this,1);
	}

	CRect rect; 
	GetClientRect(rect);
	m_ToolTip.AddTool(this, strTip, rect, 1);

	m_ToolTip.Activate(TRUE);

	m_strPreTip = strTip;
}


LRESULT CImageToolBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

	if ( message==WM_COMMAND )
	{
		::PostMessage(GetParent()->m_hWnd,
			message,wParam,lParam);
	}

	return CToolBar::WindowProc(message, wParam, lParam);
}

LRESULT CImageToolBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam)
{
	if (IsWindowVisible())
	{
		CFrameWnd *pParent = (CFrameWnd *)GetParent();
		if (pParent)
			OnUpdateCmdUI(pParent, (BOOL)wParam);
	}
	return 0L;
}

HBRUSH CImageToolBar::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CToolBar::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		hbr = brushBack;
		pDC->SetBkColor(IM_SPACE_COLOR);
	}

	return hbr;
}

INT_PTR CImageToolBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	// TODO: 在此添加专用代码和/或调用基类

	return CToolBar::OnToolHitTest(point, pTI);
}

void CImageToolBar::OnMouseMove(UINT nFlags, CPoint point)
{
	int nCount = GetCount();
	int nHotItem = -1;

	for ( int i=0; i<nCount; i++ )
	{
		CRect rcItem;
		GetItemRect(i,&rcItem);

		if ( rcItem.PtInRect(point) )
		{
			nHotItem = i;
			break;
		}
	}

	if ( nHotItem!=-1 )
	{
		UINT nID=GetItemID(nHotItem);

		map<UINT,CString>::iterator  it;
		it = m_mapToolTipString.find(nID);

		if ( it!=m_mapToolTipString.end() )
		{
			ShowToolTip(it->second);
		}
		else
		{
			ShowToolTip("");
		}
	}

	CToolBar::OnMouseMove(nFlags, point);
}

void CImageToolBar::PreSubclassWindow()
{
	CToolBar::PreSubclassWindow();
}
