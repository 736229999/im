// im\FlatEdit.cpp : 实现文件
//

#include "..\..\stdafx.h"
#include ".\flatedit.h"

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0 )

// CFlatEdit

IMPLEMENT_DYNAMIC(CFlatEdit, CEdit)
CFlatEdit::CFlatEdit()
{
	m_clrBack   = RGB(255,255,255);
	m_clrBorder = IM_LINE_COLOR;//RGB(0,0,0);
	m_clrText   = RGB(0,0,0);
	m_hBack     = CreateSolidBrush(m_clrBack);
	m_bEnablePaste = true;

	m_bFirst = TRUE;
	m_strHintText = _T("请用逗号分隔,最多10个.");
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
	ON_CONTROL_REFLECT(EN_CHANGE, &CFlatEdit::OnEnChange)
END_MESSAGE_MAP()


// CFlatEdit 消息处理程序

void CFlatEdit::OnPaint()
{
	Default();

	CDC *pDC = GetWindowDC();
	CFont* pOldFont = pDC->GetCurrentFont();

	CRect rcItem;
	GetWindowRect(&rcItem);

	rcItem.right-=rcItem.left;
	rcItem.left=0;

	rcItem.bottom-=rcItem.top;
	rcItem.top=0;

	//rcItem.InflateRect(0,);

	if ( IsWindowEnabled()==false )
		pDC->Draw3dRect(rcItem, RGB(122,122,122), RGB(122,122,122));
	else
        pDC->Draw3dRect(rcItem, m_clrBorder, m_clrBorder);


	rcItem.InflateRect(-1,-1);
	pDC->Draw3dRect(rcItem, m_clrBack, m_clrBack);

	pDC->SelectObject(pOldFont);

	CString strText;
	GetWindowText(strText);
	if(strText.GetLength() < 1)
	{
		CRect rc;
		GetClientRect(&rc);
		
		rc.DeflateRect(3, 3, 0, 0);

		CFont font;
		font.CreatePointFont(90, "宋体");
		CFont *pOldFont = pDC ->SelectObject(&font);

		pDC ->SetTextColor(RGB(154,154,154));
		pDC ->DrawText(m_strHintText, m_strHintText.GetLength(), rc, DT_LEFT | DT_VCENTER | DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP);
		
		pDC ->SelectObject(pOldFont);
	}

	ReleaseDC(pDC);
}


void CFlatEdit::OnNcPaint()
{
	//CEdit::OnNcPaint();
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
	if (pMsg->message==WM_KEYDOWN )
	{
		if (  m_bEnablePaste==false && pMsg->wParam=='V' && KEYDOWN(VK_CONTROL) )
		{
			return TRUE;
		}
	}



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

	return nRet;
}

void CFlatEdit::OnEnChange()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CEdit::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	
	BOOL bNeed = m_bFirst;
	m_bFirst = FALSE;
	if(bNeed) Invalidate();
}
