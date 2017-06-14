#pragma once
#include "xskindlg.h"
#include "..\resource.h"
#include "..\im\LocalData.h"
#include "MainFrame.h"
#include "..\..\..\DuiLib\UIlib.h"
#include "..\..\..\Common\WndShadow.h"


class CheckServiceLogWarn : public DuiLib::WindowImplBase
{
public:
	static std::tr1::shared_ptr<CheckServiceLogWarn> Show(int nTag,HWND wnd);
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("CheckServiceLogWarn");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("CheckServiceLogWarn.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	void  Notify(DuiLib::TNotifyUI& msg);
	static void  UpdateBk();
	void  ReSetWindow(int nTag);
	void  SetParentHwnd(HWND pDlg);
	void  SetWindowTop();
	HWND  m_parent;
	BOOL  m_bClickedBtnOK;//是否点击了确定按钮

private:
	void OnBnClickedBtnExit();

private:
	WORD m_wTag;
	CWndShadow m_WndShadow;
	static std::tr1::shared_ptr<CheckServiceLogWarn> pInstance;

	DuiLib::CButtonUI *m_pBtnClose;
	DuiLib::CLabelUI  *m_pLabelTitle;
	DuiLib::CLabelUI *m_pLabelWarn;
	DuiLib::CButtonUI  *m_pBtnAdmin;
	DuiLib::CButtonUI  *m_pBtnCancel;
};