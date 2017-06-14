#include "..\stdafx.h"
#include ".\thumbnaiuplistctrl.h"


BEGIN_MESSAGE_MAP(CThumBnaiUpListCtrl, CThumBnaiListCtrl)
	//	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()
CThumBnaiUpListCtrl::CThumBnaiUpListCtrl(void)
{
	m_nFlags = 1;
}

CThumBnaiUpListCtrl::~CThumBnaiUpListCtrl(void)
{
}
void CThumBnaiUpListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CThumBnaiListCtrl::OnMouseMove(nFlags, point);
}

void CThumBnaiUpListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CThumBnaiListCtrl::OnLButtonDown(nFlags, point);
}