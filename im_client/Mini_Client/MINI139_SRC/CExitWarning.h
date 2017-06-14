#pragma once
#include "xskindlg.h"
#include "..\resource.h"
#include "..\im\LocalData.h"
#include "MainFrame.h"
#include "..\..\..\DuiLib\UIlib.h"
#include "..\..\..\Common\WndShadow.h"


class CExitWarningDlg : public DuiLib::WindowImplBase
{
public:
	static std::tr1::shared_ptr<CExitWarningDlg> Show();
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("ExitWarningDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("ExitWarningDlg.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	void Notify(DuiLib::TNotifyUI& msg);
	static void UpdateBk();

private:
	void OnBnClickedBtnAdmin();

private:
	CWndShadow m_WndShadow;
	static std::tr1::shared_ptr<CExitWarningDlg> pInstance;
	DuiLib::COptionUI *m_pCheckExitWarning;
};