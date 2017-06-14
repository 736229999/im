#pragma once

#include "..\..\..\DuiLib\UIlib.h"

class CSellerMobileAuthDlg : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("SellerMobileAuthDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("SellerMobileAuth.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	void Notify(DuiLib::TNotifyUI& msg);
	static void UpdateBk();
	static std::tr1::shared_ptr<CSellerMobileAuthDlg> Show(HWND parent);

private:
	afx_msg void OnTimer(UINT nIDEvent);

	enum
	{
		TIMERID_REGETAUTHCODE = 100,
	};

	static std::tr1::shared_ptr<CSellerMobileAuthDlg> pInstance;
	int m_nDescend;
};