#include "..\..\stdafx.h"
#include ".\skinctrl.h"

BEGIN_MESSAGE_MAP(CStaticBase, CStatic)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CStaticBase::OnEraseBkgnd(CDC *pDC)
{
	return FALSE;
}

BEGIN_MESSAGE_MAP(CTransparentLabel, CStaticBase)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CTransparentLabel::OnPaint()
{
	CPaintDC dc(this);

	int nSave=dc.SaveDC();

	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(GetFont());
	dc.SetTextColor(m_clrText);

	CSize size1 = dc.GetTextExtent(m_strStatus);
	CSize size2 = dc.GetTextExtent(m_strName);


	CRect rc;
	GetClientRect(rc);

	rc.right-=(size1.cx);

	if ( size2.cx>rc.Width() )
	{
		dc.TextOut(rc.right,rc.top,m_strStatus);
		dc.DrawText(m_strName,&rc,DT_END_ELLIPSIS|DT_TOP|DT_LEFT);
	}
	else
	{
		dc.TextOut(0,rc.top,m_strName);
		dc.TextOut(size2.cx,rc.top,m_strStatus);
	}

	dc.RestoreDC(nSave);

	//CSize sz = dc.GetTextExtent(m_strText);

	//if ( rc.Width()>sz.cx )
	//{
	//	ClientToScreen(&rc);
	//	GetParent()->ScreenToClient(&rc);
	//	rc.right=rc.left+sz.cx+1;
	//	MoveWindow(&rc,false);
	//}
}


void CTransparentLabel::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	CStaticBase::PreSubclassWindow();
	GetWindowText(m_strName);
}
