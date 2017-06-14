// ToolBarEx.cpp : ʵ���ļ�
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



// CToolBarEx ��Ϣ�������


void CToolBarEx::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	CToolBar::OnMeasureItem(nIDCtl, lpMeasureItemStruct);

}

void CToolBarEx::PreSubclassWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	
	CToolBar::PreSubclassWindow();
}

HBRUSH CToolBarEx::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CToolBar::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	//::SetTextColor( hdc , RGB(255,0,0) );
	//::SetBkMode( hdc , TRANSPARENT ) ;
	//return (LRESULT)GetStockObject( NULL_BRUSH ) ;

	static   HBRUSH   hbrEdit   =::CreateSolidBrush(RGB(0,255,   0));
	pDC->SetBkColor(RGB(0,0,250)); 
	pDC->SetTextColor(RGB(0,   0,   255));   
	return  hbrEdit;

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

BOOL CToolBarEx::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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
//	// TODO: �ڴ����ר�ô����/����û���
//	if ( message==WM_COMMAND )
//	{
//		::PostMessage(GetParent()->m_hWnd,
//			message,wParam,lParam);
//	}
//	return CToolBar::WindowProc(message, wParam, lParam);
//}
