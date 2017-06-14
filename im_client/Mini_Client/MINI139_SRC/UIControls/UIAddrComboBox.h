#pragma once

#include "..\..\..\DuiLib\UIlib.h"

#define WM_COMBO_ITEMSELECT (WM_USER+32)	// itemÑ¡Ôñ¸Ä±ä

class CUIAddrComboBox : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UIAddrComboBox");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("UIAddrComboBox.xml");  }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	virtual void Notify(DuiLib::TNotifyUI& msg);
	bool AddString(const char *szText);
	bool SelectItem(int nIndex);
	int GetCurSel();
	void RemoveAll();
	void SetLabel(const char *szLabel);
};