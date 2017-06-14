#pragma once
#include "xskindlg.h"
#include "..\resource.h"
#include "..\im\LocalData.h"
#include "MainFrame.h"
#include "..\..\..\DuiLib\UIlib.h"
#include "..\..\..\Common\WndShadow.h"


class CModPassWordDlg : public DuiLib::WindowImplBase
{
	DECLARE_XT_MESSAGE_MAP()
public:
	static std::tr1::shared_ptr<CModPassWordDlg> Show();
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("CModPassWordDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("ModPSW.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	void Notify(DuiLib::TNotifyUI& msg);
	static void UpdateBk();
	void OnModPswAck(const char *data, WORD wRet);
	static void CloseDlg();
private:
	void OnBnClickedBtnAdmin();

private:
	CWndShadow m_WndShadow;
	static std::tr1::shared_ptr<CModPassWordDlg> pInstance;
	DuiLib::CEditUI *m_pEditNewPSW;
	DuiLib::CLabelUI *m_pLabelInputTips;
	DuiLib::CEditUI *m_pEditConfirmPSW;
	DuiLib::CLabelUI *m_pLabelCheckError;
};