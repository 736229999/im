// im\FlatEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "flatedit.h"

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0 )
// CFlatEdit
IMPLEMENT_DYNAMIC(CFlatEdit, CEdit)
CFlatEdit::CFlatEdit()
{
	m_clrBack   = RGB(255,255,255);
	m_clrBorder = m_clrBack;
	m_clrText   = RGB(0,0,0);
	m_hBack     = CreateSolidBrush(m_clrBack);
	m_bEnablePaste = true;
}

CFlatEdit::~CFlatEdit()
{
	DeleteObject(m_hBack);
}


BEGIN_MESSAGE_MAP(CFlatEdit, CEdit)
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(EN_VSCROLL, OnEnVscroll)
END_MESSAGE_MAP()


// CFlatEdit 消息处理程序

void CFlatEdit::OnPaint()
{
	//Default();
	CEdit::OnPaint();

	CPaintDC dc(this);

	DrawBorder();
}

void CFlatEdit::DrawBorder()
{
	CDC *pDC = GetWindowDC();
	CFont* pOldFont = pDC->GetCurrentFont();

	CRect rcItem;
	GetWindowRect(&rcItem);

	rcItem.right-=rcItem.left;
	rcItem.left=0;

	rcItem.bottom-=rcItem.top;
	rcItem.top=0;

	//rcItem.InflateRect(0,);
	//ES_READONLY
	//BOOL bReadOnly = GetStyle() & ES_READONLY;

	if ( IsWindowEnabled()==false )
		pDC->Draw3dRect(rcItem, RGB(122,122,122), RGB(122,122,122));
	else
        pDC->Draw3dRect(rcItem, m_clrBorder, m_clrBorder);


	rcItem.InflateRect(-1,-1);
	pDC->Draw3dRect(rcItem, m_clrBack, m_clrBack);

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
}

void CFlatEdit::OnNcPaint()
{
	//CEdit::OnNcPaint();
	DrawBorder();
}

void CFlatEdit::SetBorderColor(COLORREF clr)
{
	m_clrBorder = clr;
	
	if ( m_hWnd )
	{
		Invalidate();
	}
}

void CFlatEdit::SetTextColor(COLORREF clr)
{
	m_clrText = clr;

	if ( m_hWnd )
	{
		Invalidate();
	}
}

void CFlatEdit::SetBackColor(COLORREF clr)
{
	m_clrBack = clr;

	DeleteObject(m_hBack);
	m_hBack = CreateSolidBrush(clr);

	if ( m_hWnd )
	{
		Invalidate();
	}
}

HBRUSH CFlatEdit::CtlColor(CDC* pDC, UINT nCtlColor)
{
	//BOOL bReadOnly = GetStyle() & ES_READONLY;

	//if ( bReadOnly )
	//	pDC->SetTextColor(0xAAAAAA);
	//else
		pDC->SetTextColor(m_clrText);
	//pDC->SetBkMode(TRANSPARENT);
	pDC->SetBkColor(this->m_clrBack);

	return m_hBack;
}

void CFlatEdit::EnablePaste(bool bEnable)
{
	m_bEnablePaste=bEnable;
}

BOOL CFlatEdit::PreTranslateMessage(MSG* pMsg)
{
	return CEdit::PreTranslateMessage(pMsg);
}

void CFlatEdit::OnEnVscroll()
{
	Invalidate();
}

LRESULT CFlatEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	UINT nRet= CEdit::WindowProc(message, wParam, lParam);

	if ( message==WM_VSCROLL )
	{
		Invalidate();
	}

	if (message==WM_KEYDOWN )
	{
		if (  m_bEnablePaste==false && wParam=='V' && KEYDOWN(VK_CONTROL) )
		{
			return TRUE;
		}
	}

	return nRet;
}
