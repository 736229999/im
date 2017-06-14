// im\FontToolBar.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\imagetoolbar.h"
#include "..\resource.h"
#include "afxpriv.h"

// CImageToolBar

IMPLEMENT_DYNAMIC(CImageToolBar, CToolBar)
CImageToolBar::CImageToolBar()
{
}

CImageToolBar::~CImageToolBar()
{
}


BEGIN_MESSAGE_MAP(CImageToolBar, CToolBar)
	ON_WM_NCPAINT()
	ON_WM_ERASEBKGND()
	ON_MESSAGE(WM_IDLEUPDATECMDUI,OnIdleUpdateCmdUI)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



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

LRESULT CImageToolBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{

	if ( message==WM_COMMAND )
	{
		::PostMessage(GetParent()->m_hWnd,
			message,wParam,lParam);
	}

	return CToolBar::WindowProc(message, wParam, lParam);
}

LRESULT CImageToolBar::OnIdleUpdateCmdUI(WPARAM wParam, LPARAM)
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
		hbr = g_hBackBrush;
		pDC->SetBkColor(IM_SPACE_COLOR);
	}

	return hbr;
}
