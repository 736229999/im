// im\BkgFrame.cpp : 实现文件
//

#include "stdafx.h"

#include ".\bkgframe.h"

#include "impublic.h"
// CBkgFrame

IMPLEMENT_DYNAMIC(CBkgFrame, CStatic)
CBkgFrame::CBkgFrame()
{
	m_hBrush = CreateSolidBrush(RGB(255,255,255));
}

CBkgFrame::~CBkgFrame()
{
}


BEGIN_MESSAGE_MAP(CBkgFrame, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CBkgFrame 消息处理程序


void CBkgFrame::OnPaint()
{
	CPaintDC dc(this); 
	CRect rc;
	GetWindowRect(&rc);
	ScreenToClient(&rc);

	CBrush brush;
	brush.Attach(m_hBrush);
	CPen   pen(PS_SOLID,1,IM_LINE_COLOR);

	CBrush *pOldbrush = dc.SelectObject(&brush);
	CPen   *pOldpen   = dc.SelectObject(&pen);

	dc.Rectangle(&rc);
	brush.Detach();

}
