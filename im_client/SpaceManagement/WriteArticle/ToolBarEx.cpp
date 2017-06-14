// ToolBarEx.cpp : 实现文件
//

#include "stdafx.h"
#include "ToolBarEx.h"
#include ".\toolbarex.h"


// CToolBarEx

IMPLEMENT_DYNAMIC(CToolBarEx, CToolBar)
CToolBarEx::CToolBarEx()
{
}

CToolBarEx::~CToolBarEx()
{
}


BEGIN_MESSAGE_MAP(CToolBarEx, CToolBar)
	ON_WM_DRAWITEM()
	ON_WM_MEASUREITEM()
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()



// CToolBarEx 消息处理程序


void CToolBarEx::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	LPDRAWITEMSTRUCT& lpD=lpDrawItemStruct; 

	CDC* pDC=CDC::FromHandle(lpD->hDC);  
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(RGB(255,0,0));
	pDC->SetTextColor(RGB(0,0,0));
	MessageBox("OnDrawItem");




	CToolBar::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CToolBarEx::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	CToolBar::OnMeasureItem(nIDCtl, lpMeasureItemStruct);

}

void CToolBarEx::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	
	CToolBar::PreSubclassWindow();
}

HBRUSH CToolBarEx::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CToolBar::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
	//::SetTextColor( hdc , RGB(255,0,0) );
	//::SetBkMode( hdc , TRANSPARENT ) ;
	//return (LRESULT)GetStockObject( NULL_BRUSH ) ;

	static   HBRUSH   hbrEdit   =::CreateSolidBrush(RGB(0,255,   0));
	pDC->SetBkColor(RGB(0,0,250)); 
	pDC->SetTextColor(RGB(0,   0,   255));   
	return  hbrEdit;

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

BOOL CToolBarEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient,IM_SPACE_COLOR);
	//pDC->FillSolidRect(rcClient,RGB(0,255,2));
	pDC->Draw3dRect(rcClient,IM_SPACE_COLOR,IM_SPACE_COLOR);
	
	
	return FALSE;
	return CToolBar::OnEraseBkgnd(pDC);
}

//LRESULT CToolBarEx::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
//{
//	// TODO: 在此添加专用代码和/或调用基类
//	if ( message==WM_COMMAND )
//	{
//		::PostMessage(GetParent()->m_hWnd,
//			message,wParam,lParam);
//	}
//	return CToolBar::WindowProc(message, wParam, lParam);
//}
