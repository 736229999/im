#include "stdafx.h"
#include "UIPassword.h"
#include <imm.h>
#pragma comment(lib,"imm32.lib")

using namespace std;

namespace DuiLib
{

CPasswordUI::CPasswordUI():
m_iFontPsw(-1),
m_nPswChar(_T('¡ñ')),
m_nCharPos(0),
m_blShowCaret(TRUE),
m_dwCaretClr(0),
m_iSpeed(500),
m_iCaretWidth(1),
m_dwPswClr(0)
{
	memset(&m_szCaret,0,sizeof(m_szCaret));
}

LPCTSTR CPasswordUI::GetClass() const
{
	return _T("PasswordUI");
}
LPVOID CPasswordUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, _T("Password")) == 0 ) return static_cast<CPasswordUI*>(this);
	return CButtonUI::GetInterface(pstrName);
}

void CPasswordUI::DoEvent(TEventUI& event)
{
	if( !IsMouseEnabled() && event.Type > UIEVENT__MOUSEBEGIN && event.Type < UIEVENT__MOUSEEND ) {
		if( m_pParent != NULL ) m_pParent->DoEvent(event);
		else CButtonUI::DoEvent(event);
		return;
	}

	if( event.Type == UIEVENT_SETFOCUS ) 
	{
		ShowCaret();
		m_hIMC		= ImmAssociateContext(m_pManager->GetPaintWindow(), NULL);
	}
	else if(event.Type == UIEVENT_KILLFOCUS ) 
	{
		HideCaret();
		ImmAssociateContext(m_pManager->GetPaintWindow(), m_hIMC);
	}
	else if( event.Type == UIEVENT_BUTTONDOWN || event.Type == UIEVENT_DBLCLICK )
	{
		RECT rc	   = m_rcItem;
		rc.left   += m_rcTextPadding.left;
		rc.right  -= m_rcTextPadding.right;
		rc.top	  += m_rcTextPadding.top;
		rc.bottom -= m_rcTextPadding.bottom;
		
		if(::PtInRect(&rc,event.ptMouse))
		{
			m_nCharPos = CharFromPos(event.ptMouse.x-m_rcItem.left-m_rcTextPadding.left);
			int iPos  = PosFromChar(m_nCharPos);
			m_ptCursor.x = iPos+ m_rcItem.left + m_rcTextPadding.left;
			SetCaretPos(m_ptCursor.x	,m_rcItem.top + m_rcTextPadding.top);
		}

		return;
	}
	else if( event.Type == UIEVENT_CONTEXTMENU )
	{
		if(IsContextMenuUsed()) {
			m_pManager->SendNotify(this, DUI_MSGTYPE_MENU, event.wParam, event.lParam);
		}
		return;
	}
	else if(event.Type == UIEVENT_SETCURSOR ) 
	{
		RECT rt;
		rt.left = m_rcItem.left+ m_rcTextPadding.left;
		rt.right = m_rcItem.right - m_rcTextPadding.right;
		rt.top  = m_rcItem.top + m_rcTextPadding.top;
		rt.bottom  = m_rcItem.bottom - m_rcTextPadding.bottom;

		if(::PtInRect(&rt,event.ptMouse))
		{
			::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM)));
		}
		else
		::SetCursor(::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		return;
	}
	else if(event.Type ==UIEVENT_CHAR)
	{
		switch(event.chKey)
		{
		case VK_BACK:
			{
				if(m_strText.length()==0)break;

				if(m_nCharPos> 0)
				{
					m_strText.erase(m_nCharPos-1,1);
					m_nCharPos--;
					m_ptCursor.x = PosFromChar(m_nCharPos)+ m_rcItem.left + m_rcTextPadding.left;
					SetCaretPos(m_ptCursor.x	,m_rcItem.top + m_rcTextPadding.top);
					Invalidate();
				}
			}
			break;
		default:
			string strEnd = m_strText;		
			strEnd.insert(m_nCharPos,1,event.chKey);
			int lenth=GetTextLenth(strEnd);
			if(lenth>(m_rcItem.right-m_rcItem.left - m_rcTextPadding.left - m_rcTextPadding.right))
			{
				break;
			}		
			m_strText.insert(m_nCharPos,1,event.chKey);
			m_nCharPos++;	
			m_ptCursor.x = PosFromChar(m_nCharPos)+ m_rcItem.left + m_rcTextPadding.left;
			SetCaretPos(m_ptCursor.x	,m_rcItem.top + m_rcTextPadding.top);
			Invalidate();
	   }
		return;
	}
	else if( event.Type == UIEVENT_KEYDOWN )
	{
		if (IsKeyboardEnabled()) 
		{
			switch(event.chKey)
			{
				case VK_RETURN:
				break;
				case VK_LEFT:
					{
						if (m_nCharPos==0)
						{
							break;
						}
						m_nCharPos--;
						m_ptCursor.x = PosFromChar(m_nCharPos)+ m_rcItem.left + m_rcTextPadding.left;
						SetCaretPos(m_ptCursor.x	,m_rcItem.top + m_rcTextPadding.top);
						Invalidate();
					}
				break;
				case VK_RIGHT:
					{
						if (m_nCharPos>=m_strText.length())
						{
							break;
						}
						m_nCharPos++;
						m_ptCursor.x = PosFromChar(m_nCharPos)+ m_rcItem.left + m_rcTextPadding.left;
						SetCaretPos(m_ptCursor.x	,m_rcItem.top + m_rcTextPadding.top);
						Invalidate();
					}
				break;
				case VK_HOME:
					{
						m_ptCursor.x = PosFromChar(0)+ m_rcItem.left + m_rcTextPadding.left;
						SetCaretPos(m_ptCursor.x	,m_rcItem.top + m_rcTextPadding.top);
						Invalidate();
					}
					break;
				case VK_END:
					{
						m_ptCursor.x = PosFromChar(m_strText.length())+ m_rcItem.left + m_rcTextPadding.left;
						SetCaretPos(m_ptCursor.x	,m_rcItem.top + m_rcTextPadding.top);	
						Invalidate();
					}
					break;
				case VK_DELETE:
					{
						m_strText.erase(m_nCharPos,1);
						Invalidate();
					}
					break;
			default:
				break;
			
			}
		}
	}
	else if (event.Type == UIEVENT_TIMER && event.wParam == PASSWORD_CARET)
	{
		m_blShowCaret =!m_blShowCaret;
		Invalidate();
	}
	CButtonUI::DoEvent(event);
}

void CPasswordUI::PaintText(HDC hDC)
{
	if(m_strText==_T("")&&!m_bFocused)
	{
		CLabelUI::PaintText(hDC);	
	}
	else
	{
		m_uTextStyle |= DT_LEFT|DT_SINGLELINE;
		if( m_strText!=_T(""))
		{
			if( m_dwTextColor == 0 ) m_dwTextColor = m_pManager->GetDefaultFontColor();
			int nLinks = 0;
			RECT rc = m_rcItem;
			rc.left += m_rcTextPadding.left;
			rc.right -= m_rcTextPadding.right;
			rc.top += m_rcTextPadding.top;
			rc.bottom -= m_rcTextPadding.bottom;

			string strDraw;
			strDraw.insert(0,m_strText.length(),m_nPswChar);
			CRenderEngine::DrawText(hDC, m_pManager, rc, strDraw.c_str(), m_dwPswClr, \
				m_iFontPsw, m_uTextStyle);
		}
		
		if (m_blShowCaret)
		{
			RECT rtCaret;
			rtCaret.left = m_szCaret.cx;
			rtCaret.top  = m_szCaret.cy;
			rtCaret.right= m_szCaret.cx;
			rtCaret.bottom  = m_szCaret.cy + m_rcItem.bottom-m_rcItem.top-m_rcTextPadding.top-m_rcTextPadding.bottom;
			CRenderEngine::DrawLine(hDC,rtCaret,m_iCaretWidth,m_dwCaretClr);
		}
	}
}

void CPasswordUI::SetCaretPos(int x, int y)
{
	m_szCaret.cx = x;
	m_szCaret.cy = y;
	Invalidate();
}


void CPasswordUI::ShowCaret()
{
		m_pManager->SetTimer(this, PASSWORD_CARET, m_iSpeed);
		Invalidate();
}

void CPasswordUI::HideCaret()
{
	m_pManager->KillTimer(this, PASSWORD_CARET);
	m_blShowCaret = FALSE;
	Invalidate();
}

void CPasswordUI::Clear()
{
	m_strText = _T("");
	m_nCharPos = 0;
	Invalidate();
}

INT CPasswordUI::PosFromChar(UINT nChar)
{
	string strShow = m_strText.substr(0,nChar);
	string psw;
	psw.insert(0,strShow.length(),m_nPswChar);
	HDC	hdc = GetDC(m_pManager->GetPaintWindow());
	HFONT oldfont = (HFONT)::SelectObject(hdc,m_pManager->GetFont(m_iFontPsw));
	SIZE szStr;
	::GetTextExtentPoint32(hdc,psw.c_str(),psw.size(),&szStr);
	::SelectObject(hdc,oldfont);
	return szStr.cx;
}

int CPasswordUI::GetTextLenth( string strLen)
{ 
	SIZE szLength;
	string psw;
	psw.insert(0,strLen.length(),m_nPswChar);
	HDC	hdc = GetDC(m_pManager->GetPaintWindow());
	HFONT oldfont = (HFONT)::SelectObject(hdc,m_pManager->GetFont(m_iFontPsw));
	::GetTextExtentPoint32(hdc,psw.c_str(),psw.length(),&szLength);
	::SelectObject(hdc,oldfont);
	return szLength.cx;
}

INT CPasswordUI::CharFromPos(INT pos)
{
	INT charPos = 0;
	INT	strLength = m_strText.size();
	TCHAR charTemp[4];
	INT iBegin = 0;

	SIZE szOne;
	string psw;
	psw.insert(0,m_strText.length(),m_nPswChar);
	TCHAR* szText   = (TCHAR*)psw.c_str();
	HDC	hdc			= GetDC(m_pManager->GetPaintWindow());
	HFONT oldfont	= (HFONT)::SelectObject(hdc,m_pManager->GetFont(m_iFontPsw));

	while(strLength > 0)
	{
		_tcsncpy_s(charTemp,szText,1);
		charTemp[1] = 0;
		szText++;
		strLength--;
		charPos++;
		::GetTextExtentPoint32(hdc,charTemp,1,&szOne);
		if(iBegin + szOne.cx > pos)
		{
			if (pos - iBegin < iBegin + szOne.cx - pos)
			{
				charPos--;
			}
			break;
		}
		iBegin += szOne.cx;
	}	
	::SelectObject(hdc,oldfont);
	return charPos;
}


void CPasswordUI::SetPasswordChar(TCHAR nChar)
{
	m_nPswChar = nChar;
	Invalidate();
}


void CPasswordUI::SetPswFont(int index)
{
	m_iFontPsw = index;
	Invalidate();
}

TCHAR* CPasswordUI::GetPassword()
{
return (TCHAR*)m_strText.c_str();
}

void CPasswordUI::SetCaretColor( DWORD caretColor )
{
	m_dwCaretClr = caretColor;
}

void CPasswordUI::SetCaretSpeed( int speed )
{
	m_iSpeed = speed;
}

void CPasswordUI::SetCaretWidth( int iWidth )
{
	m_iCaretWidth = iWidth;
}

void CPasswordUI::SetPasswordClr( DWORD pswColor )
{
	m_dwPswClr = pswColor;
}


void CPasswordUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if ( _tcscmp(pstrName, _T("fontpsw")) == 0 ) SetPswFont(_ttoi(pstrValue));
	else if( _tcscmp(pstrName, _T("caretcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetCaretColor(clrColor);
	}
	else if( _tcscmp(pstrName, _T("pswcolor")) == 0 ) {
		if( *pstrValue == _T('#')) pstrValue = ::CharNext(pstrValue);
		LPTSTR pstr = NULL;
		DWORD clrColor = _tcstoul(pstrValue, &pstr, 16);
		SetPasswordClr(clrColor);
	}
	else if (_tcscmp(pstrName, _T("caretspeed")) == 0 )
	{
		SetCaretSpeed(_ttoi(pstrValue));
	}
	else if (_tcscmp(pstrName, _T("caretwidth")) == 0 )
	{
		SetCaretWidth(_ttoi(pstrValue));
	}
	else CButtonUI::SetAttribute( pstrName, pstrValue);
}

}