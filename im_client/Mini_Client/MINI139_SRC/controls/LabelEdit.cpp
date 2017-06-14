// controls\LabelEdit.cpp : 实现文件
//

#include "stdafx.h"
#include "LabelEdit.h"
#include ".\labeledit.h"
#include "..\im\impublic.h"

// CLabelEdit
#define BORDER_HOT_COLOR 0x3c6883
IMPLEMENT_DYNAMIC(CLabelEdit, CFlatEdit)

CLabelEdit::CLabelEdit()
{
	m_bFocused   = false;
	m_bMouseOver = false;

	m_nIMEChars  = 0;
	m_nMaxWidth  = -1;
}

CLabelEdit::~CLabelEdit()
{
}


BEGIN_MESSAGE_MAP(CLabelEdit, CFlatEdit)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_KILLFOCUS()
	ON_WM_SETFOCUS()
	ON_WM_CHAR()
	ON_CONTROL_REFLECT(EN_CHANGE, OnEnChange)
END_MESSAGE_MAP()



// CLabelEdit 消息处理程序


void CLabelEdit::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( m_bMouseOver==false && m_bFocused==false )
	{
		m_bMouseOver = true;
		SetTimer(0x01,10,NULL);
		SetBorderColor(BORDER_HOT_COLOR);
		
		SetToolTip("点击输入个性签名");
	}

	if (m_bFocused==false )
		::SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(32649)));

	CFlatEdit::OnMouseMove(nFlags, point);
}

void CLabelEdit::SetMaxWidth(int n)
{
	m_nMaxWidth = n;
	SizeToContent();
}

void CLabelEdit::SizeToContent()
{
	CDC * pDC = GetDC();

	if ( pDC )
	{
		CString s;
		GetWindowText(s);

		CFont *pFont = GetFont();
		CFont *pOld  = pDC->SelectObject(pFont);


		int n ;

		if ( s=="" )
			n=32;
		else
			n=pDC->GetTextExtent(s).cx+16;

		if ( n>m_nMaxWidth )
		{
			n=m_nMaxWidth;
		}
		
		CRect rc;
		GetWindowRect(&rc);

		CRect rc1 = rc;
		rc.right  = rc.left+n;

		CWnd *pParent = GetParent();

		pParent->ScreenToClient(&rc);
		::MoveWindow(m_hWnd,rc.left,rc.top,rc.Width(),rc.Height(),TRUE);

		if ( rc1.Width()>rc.Width() )
			pParent->InvalidateRect(rc1);
	
		pDC->SelectObject(pOld);
		ReleaseDC(pDC);
	}
}

void CLabelEdit::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		CPoint pt;
		GetCursorPos(&pt);

		CRect rc;
		GetWindowRect(&rc);

		if ( rc.PtInRect(pt)==FALSE && m_bFocused==false )
		{
			m_bMouseOver=false;
			KillTimer(0x01);
			SetBorderColor(IM_SPACE_COLOR);
		}
	}

	CFlatEdit::OnTimer(nIDEvent);
}

void CLabelEdit::PreSubclassWindow()
{
	CFlatEdit::PreSubclassWindow();

	SetBackColor(IM_SPACE_COLOR);
	SetBorderColor(IM_SPACE_COLOR);

	InitToolTip();
	EnableToolTips();
}

void CLabelEdit::TestWindowText()
{
	CString s;
	GetWindowText(s);
	s.Trim();

	if ( s=="" )
	{
		SetTextColor(0xFFFFFF);
		CFlatEdit::SetWindowText("<点击这里,输入你的个性签名>");
	}
	else
	{
		SetTextColor(RGB(255,255,255));;
	}
}

void CLabelEdit::OnKillFocus(CWnd* pNewWnd)
{
	CFlatEdit::OnKillFocus(pNewWnd);

	m_bFocused=false;
	SetBorderColor(IM_SPACE_COLOR);
	SetBackColor(IM_SPACE_COLOR);

	TestWindowText();
	m_nIMEChars		= 0;

	SetSel(0,0);
	SizeToContent();
}

void CLabelEdit::SetWindowText(LPCSTR lpszWindowText)
{
	CFlatEdit::SetWindowText(lpszWindowText);
	TestWindowText();
	SizeToContent();
}

void CLabelEdit::OnSetFocus(CWnd* pOldWnd)
{
	CFlatEdit::OnSetFocus(pOldWnd);

	m_nIMEChars = 0;

	m_bFocused=true;
	SetBorderColor(BORDER_HOT_COLOR);
	SetBackColor(0xFFFFFF);

	CString s;
	GetWindowText(s);
	s.Trim();

	SetTextColor(0);

	if ( s=="<点击这里,输入你的个性签名>" )
	{
		CFlatEdit::SetWindowText("");
	}
}


LRESULT CLabelEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//WM_IME_CHAR,IMN_CHANGECANDIDATE
	if(message>=0x0281 && message<=0x0291)
	{
		m_nIMEChars++;
		TRACE("imechar = %x  w=%x,  l=%x\n",message,wParam, lParam);
		SizeToContent();
	}

	return CFlatEdit::WindowProc(message, wParam, lParam);
}

void CLabelEdit::OnEnChange()
{
	SizeToContent();
}

void CLabelEdit::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		//m_ToolTip.Create(this);
		//m_ToolTip.Activate(FALSE);
		//m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
		m_ToolTip.Create( this , TTS_ALWAYSTIP ) ;
		m_ToolTip.Activate( TRUE ) ;
	} 
}

void CLabelEdit::SetToolTip(CString strTip)
{
	if (strTip == "") 
	{
		return;
	}

	InitToolTip();
	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, strTip, rectBtn, 1);
	} // if

	m_ToolTip.UpdateTipText(strTip,this,1);

	m_ToolTip.Activate(TRUE);
}

