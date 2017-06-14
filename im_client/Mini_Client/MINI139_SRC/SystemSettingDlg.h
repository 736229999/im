#pragma once
#include "SystemSettingCommonDlg.h"
#include "SystemSettingSecurityDlg.h"
#include "afxcmn.h"
#include "..\..\SpaceManagement\public\public_controls\buttonext.h"
#include "..\..\..\DuiLib\UIlib.h"

///
//

class CSystemSettingDlg : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("CSystemSettingDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("SysSettingMainCfg.xml");  }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	static void CloseDlg();

private:
	DuiLib::COptionUI *m_pRadioBasic;
	DuiLib::COptionUI *m_pRadioSecurity;
	DuiLib::CTabLayoutUI *m_pTabLayout;

	DuiLib::CButtonUI *m_pBtnClose;
	DuiLib::CButtonUI *m_pBtnOK;
	DuiLib::CButtonUI *m_pBtnCancel;

public:
	CSystemSettingDlg();
	virtual ~CSystemSettingDlg();
	static void UpdateBk();
	void  InitTabCtrl();
	static std::tr1::shared_ptr<CSystemSettingDlg> Show();
	
public:
	CSystemSettingCommonDlg   m_SystemCommon;
	CSystemSettingSecurityDlg m_SystemSecurity;
	
private:
	CWndShadow m_WndShadow;
	static std::tr1::shared_ptr<CSystemSettingDlg> pInstance;
	
public:
	 void OnBnClickedOk();
	 void OnBnClickedCancel();
	void ShowTab(int nMainTabIndex, int nChildTabIndex);
	
	virtual DuiLib::CControlUI* CreateControl(LPCTSTR pstrClassName)
	{
		if (_tcsicmp(pstrClassName, _T("BasicCfgWnd")) == 0)
		{
			CWndUI  *pUI  = new CWndUI;      
			RECT rc;
			rc.left = 0;
			rc.right = 0;
			rc.top = 0;
			rc.bottom = 0;

			m_SystemCommon.Create(CSystemSettingCommonDlg::IDD, CWnd::FromHandle(GetHWND()));
			m_SystemCommon.MoveWindow(&rc);
			m_SystemCommon.ShowWindow(SW_SHOW);
			pUI->Attach(m_SystemCommon.GetSafeHwnd());               
			return pUI;
		}

		if (_tcsicmp(pstrClassName, _T("SecurityCfgWnd")) == 0)////离线框
		{
			CWndUI  *pUI  = new CWndUI;       
			RECT rc;
			rc.left = 0;
			rc.right = 0;
			rc.top = 0;
			rc.bottom = 0;

			m_SystemSecurity.Create(CSystemSettingSecurityDlg::IDD, CWnd::FromHandle(GetHWND()));
			m_SystemSecurity.MoveWindow(&rc);
	/*		m_SystemSecurity.ShowWindow(SW_SHOW);*/
			pUI->Attach(m_SystemSecurity.GetSafeHwnd());         
			return pUI;
		}
		return NULL;
	}
};

////////////////////////////////////////////////////////////////////////


// CSystemSettingDlg 对话框
//#define WM_SYSTEM_SETTING_MSG	WM_USER+33		// 系统设置窗口点击确认
////
////class CSystemSettingDlg : public CXSkinDlg
////{
////	DECLARE_DYNAMIC(CSystemSettingDlg)
////
////public:
////	CSystemSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
////	virtual ~CSystemSettingDlg();
////	virtual BOOL PreTranslateMessage(MSG* pMsg);
////
////	// 控制Tab页显示
////	void ShowTab(int nMainTabIndex, int nChildTabIndex);
////
////// 对话框数据
////	enum { IDD = IDD_WIZARD_CLUSTER_SYS_SETTING };
////
////protected:
////	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
////	virtual BOOL OnInitDialog();
////	DECLARE_MESSAGE_MAP()
////public:
////	void  InitTabCtrl();
////public:
////	static std::tr1::shared_ptr<CSystemSettingDlg> Show();
////public:
////	CSystemSettingCommonDlg   m_SystemCommon;
////	CSystemSettingSecurityDlg m_SystemSecurity;
////
////private:
////	static std::tr1::shared_ptr<CSystemSettingDlg> pInstance;
////
////public:
////	afx_msg void OnBnClickedOk();
////	afx_msg void OnBnClickedCancel();
////	// 菜单栏选着改变事件
////	afx_msg LRESULT OnMainTabSelChange(WPARAM wParam, LPARAM lParam);
////	//void  UpdateUserInfo();
////	CPngBtn m_btnOK;
////	CPngBtn m_btnCancel;
////	//XT_USER_INFO m_UserInfo;
////
////private:
////	CUIMainTab *m_pMainTab;
////};