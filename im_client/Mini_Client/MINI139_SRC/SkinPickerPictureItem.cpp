#include "stdafx.h"
#include "SkinPickerPictureItem.h"
using namespace DuiLib;
///////////////////////////////////////////////////////////////////////
//
//
//组图的TOP值
#define TOP_GROUP1  36
#define TOP_GROUP2  124
#define TOP_GROUP3  214
CSkinPikerPictureItemUI::CSkinPikerPictureItemUI()
{
	m_Author = _T("");
	m_ChooseTips = _T("");
}
LPCTSTR CSkinPikerPictureItemUI::GetClass() const
{
	return kSkinPickerPictureItemClassName;
}

LPVOID CSkinPikerPictureItemUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, kSkinPickerPictureItemInterface) == 0 ) return static_cast<CSkinPikerPictureItemUI*>(this);
	return CButtonUI::GetInterface(pstrName);
}

void CSkinPikerPictureItemUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("bkname")) == 0 ) m_BkName = pstrValue;
	else if( _tcscmp(pstrName, _T("author")) == 0 ) m_Author += pstrValue;
	else if( _tcscmp(pstrName, _T("foreimage")) == 0 )
	{
		if( TOP_GROUP1 == m_rcItem.top ||TOP_GROUP2 == m_rcItem.top)////上面两排背景
		{
			pstrValue = _T("file='SysBtn\\icon_choose.png' dest='70,70,86,86'");
		}
		else if(TOP_GROUP3 == m_rcItem.top)////第三排纯色背景
		{
			pstrValue = _T("file='SysBtn\\icon_choose.png' dest='17,17,33,33'");
		}
		m_ChooseTips = pstrValue;
	}
	CButtonUI::SetAttribute(pstrName, pstrValue);
}

void CSkinPikerPictureItemUI::EraseAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
{
	if( _tcscmp(pstrName, _T("foreimage")) == 0 )
	{
	    m_ChooseTips = pstrValue;
	}
	CButtonUI::SetAttribute(pstrName, pstrValue);
}

CString CSkinPikerPictureItemUI::GetChooseTips()
{
	return m_ChooseTips;
}

void CSkinPikerPictureItemUI::PaintStatusImage(HDC hDC)
{
	CButtonUI::PaintStatusImage(hDC);

	if( IsFocused() ) m_uButtonState |= UISTATE_FOCUSED;
	else m_uButtonState &= ~ UISTATE_FOCUSED;
	if( !IsEnabled() ) m_uButtonState |= UISTATE_DISABLED;
	else m_uButtonState &= ~ UISTATE_DISABLED;

	if( (m_uButtonState & UISTATE_PUSHED) != 0 || (m_uButtonState & UISTATE_HOT) != 0)
	{
		DrawImage(hDC, kSkinPickerPictureItemForeImage) ;

		//计算作者信息文字和背景图片名字文字的显示位置，这里是用了硬编码，请使用者自己修改
		RECT rcBkName = m_rcItem;
		if( TOP_GROUP1 == m_rcItem.top ||TOP_GROUP2 == m_rcItem.top)////上面两排背景
		{
			LONG nTextPadding = (m_rcItem.right - m_rcItem.left  - CRenderEngine::GetTextSize(hDC, GetManager(),\
				m_BkName.GetData(), m_iFont, m_uTextStyle).cx) / 2;
			rcBkName.left += nTextPadding;
			rcBkName.right -= nTextPadding;
			rcBkName.top += 30;
			rcBkName.bottom = rcBkName.top + 20;

			RECT rcAuthor = m_rcItem;
			nTextPadding = (m_rcItem.right - m_rcItem.left - CRenderEngine::GetTextSize(hDC, GetManager(),\
				m_Author.GetData(), m_iFont, m_uTextStyle).cx) / 2;
			rcAuthor.left += nTextPadding;
			rcAuthor.right -= nTextPadding;
			rcAuthor.top += 40;
			rcAuthor.bottom = rcAuthor.top + 20;

			CRenderEngine::DrawText(hDC, m_pManager, rcBkName, m_BkName, kBkNameColor, m_iFont, m_uTextStyle);
			//CRenderEngine::DrawText(hDC, m_pManager, rcAuthor, m_Author, kAuthorColor, m_iFont, m_uTextStyle);
			CRenderEngine::DrawRect(hDC, m_rcItem, 2, kBorderColor);
		}
		else if(TOP_GROUP3 == m_rcItem.top)////第三排纯色背景
		{
			LONG nTextPadding = (m_rcItem.right - m_rcItem.left  - CRenderEngine::GetTextSize(hDC, GetManager(),\
				m_BkName.GetData(), m_iFont, m_uTextStyle).cx) / 2;
			rcBkName.left += nTextPadding;
			rcBkName.right -= nTextPadding;
			rcBkName.top += 10;
			rcBkName.bottom = rcBkName.top + 20;

			RECT rcAuthor = m_rcItem;
			nTextPadding = (m_rcItem.right - m_rcItem.left - CRenderEngine::GetTextSize(hDC, GetManager(),\
				m_Author.GetData(), m_iFont, m_uTextStyle).cx) / 2;
			rcAuthor.left += nTextPadding;
			rcAuthor.right -= nTextPadding;
			rcAuthor.top += 40;
			rcAuthor.bottom = rcAuthor.top + 20;

			CRenderEngine::DrawText(hDC, m_pManager, rcBkName, m_BkName, kBkNameColor, m_iFont, m_uTextStyle);
			//CRenderEngine::DrawText(hDC, m_pManager, rcAuthor, m_Author, kAuthorColor, m_iFont, m_uTextStyle);
			CRenderEngine::DrawRect(hDC, m_rcItem, 2, kBorderColor);
		}
	}
}


