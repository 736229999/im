#include "stdafx.h"
#include "UIAddrComboBox.h"
#include "..\resource.h"
using namespace DuiLib;

bool CUIAddrComboBox::AddString(const char *szText)
{
	CComboBoxUI* pCombo = static_cast<CComboBoxUI*>(m_PaintManager.FindControl("ComboSel"));
	if(pCombo == NULL)
	{
		return false;
	}

	CListLabelElementUI* pListElement = new CListLabelElementUI;
	pListElement->SetAttribute("text", szText);
	return pCombo->Add(pListElement);
}

bool CUIAddrComboBox::SelectItem(int nIndex)
{
	CComboBoxUI* pCombo = static_cast<CComboBoxUI*>(m_PaintManager.FindControl("ComboSel"));
	if(pCombo == NULL)
	{
		return false;
	}

	return pCombo->SelectItem(nIndex);
}

int CUIAddrComboBox::GetCurSel()
{
	CComboBoxUI* pCombo = static_cast<CComboBoxUI*>(m_PaintManager.FindControl("ComboSel"));
	if(pCombo == NULL)
	{
		return -1;
	}

	return pCombo->GetCurSel();
}

void CUIAddrComboBox::SetLabel(const char *szLabel)
{
	CLabelUI* pLabel = static_cast<CLabelUI*>(m_PaintManager.FindControl("Addr"));
	if(pLabel == NULL)
	{
		return;
	}
	pLabel->SetText(szLabel);
}

void CUIAddrComboBox::Notify(TNotifyUI& msg)
{
	WindowImplBase::Notify(msg);
	CComboBoxUI* pCombo = static_cast<CComboBoxUI*>(m_PaintManager.FindControl("ComboSel"));
	if(pCombo == msg.pSender)
	{
		if(msg.sType == DUI_MSGTYPE_ITEMSELECT)
		{
			::SendMessage(::GetParent(m_hWnd), WM_COMBO_ITEMSELECT, (WPARAM)this, pCombo->GetCurSel());
		}
	}
}

void CUIAddrComboBox::RemoveAll()
{
	CComboBoxUI* pCombo = static_cast<CComboBoxUI*>(m_PaintManager.FindControl("ComboSel"));
	if(pCombo == NULL)
	{
		return;
	}

	pCombo->RemoveAll();
}

DuiLib::UILIB_RESOURCETYPE CUIAddrComboBox::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}
LPCTSTR CUIAddrComboBox::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}