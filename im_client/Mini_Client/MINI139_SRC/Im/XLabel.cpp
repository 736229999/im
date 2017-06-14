// XLabel.cpp : 实现文件
//

#include "stdafx.h"

#include "XLabel.h"

XY_BEG

// CXLabel

IMPLEMENT_DYNAMIC(CXLabel, CStatic)
CXLabel::CXLabel()
{
	m_clrBkg = RGB(255,255,255);
	m_clrText = RGB(0,84,166);
	m_bTransparent = FALSE;
	m_hBkg = CreateSolidBrush(m_clrBkg);
}

CXLabel::~CXLabel()
{
	if(m_hBkg)
	{
		DeleteObject(m_hBkg);
	}
}

void CXLabel::SetBkgColor(COLORREF clrBkg)
{
	m_clrBkg = clrBkg;
	if(m_hBkg)
	{
		DeleteObject(m_hBkg);
	}
	m_hBkg = CreateSolidBrush(m_clrBkg);
	if(m_hWnd)
	{
		Invalidate();
	}
}

void CXLabel::SetTextColor(COLORREF clrText)
{
	m_clrText = clrText;
	if(m_hWnd)
	{
		Invalidate();
	}
}


BEGIN_MESSAGE_MAP(CXLabel, CStatic)
	ON_WM_CTLCOLOR_REFLECT()
END_MESSAGE_MAP()



// CXLabel 消息处理程序


HBRUSH CXLabel::CtlColor(CDC* pDC, UINT nCtlColor)
{
	//if(m_bTransparent)
	//	pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(m_clrText);
	return m_hBkg;
}

XY_END