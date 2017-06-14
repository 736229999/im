#pragma once
#include "..\resource.h"
#include "..\im\impublic.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "..\..\..\HmgSkin\HmgSkin\CheckButton.h"
#include "..\..\..\HmgSkin\HmgSkin\RadioButton.h"
#include "..\..\..\HmgSkin\HmgSkin\FlatHotKeyCtrl.h"
// CSysCommonMsg 对话框
//
#include "..\..\..\DuiLib\UIlib.h"

//

class CSysCommonMsg : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("MsgSetting");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("MsgSetting.xml");  }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	// 保存
	void  SaveConfig();

private:
	DuiLib::COptionUI *m_pCheckBanShake;    ////禁止振屏
	DuiLib::COptionUI *m_pRadioEnterKey;    ////ENTER键
	DuiLib::COptionUI *m_pRadioCtrlEnterKey;////CTRL + ENTER键
	DuiLib::COptionUI *m_pCheckViewMsg;     ////消息显示
	DuiLib::COptionUI *m_pCheckSnapShot;    ////截图快捷键

	CFlatHotKeyCtrl m_hotkeyMsglog;  ////查看消息热键
	CFlatHotKeyCtrl m_hotkeySnapShot;////截图热键



	virtual DuiLib::CControlUI* CreateControl(LPCTSTR pstrClassName)
	{
		if (_tcsicmp(pstrClassName, _T("MsgViewWnd")) == 0)
		{
			CWndUI  *pUI  = new CWndUI;      
			RECT rc;
			rc.left = 0;
			rc.right = 0;
			rc.top = 0;
			rc.bottom = 0;
			m_hotkeyMsglog.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | ES_WANTRETURN
				/*| WS_HSCROLL | ES_AUTOHSCROLL*/  // 自动水平滚动
				/*| WS_VSCROLL | ES_AUTOVSCROLL*/, rc, CWnd::FromHandle(GetHWND()), 10004);
			pUI->Attach(m_hotkeyMsglog.GetSafeHwnd());          
			return pUI;
		}

		if (_tcsicmp(pstrClassName, _T("SnapShotWnd")) == 0)////离线框
		{
			CWndUI  *pUI  = new CWndUI;      
			RECT rc;
			rc.left = 0;
			rc.right = 0;
			rc.top = 0;
			rc.bottom = 0;

			m_hotkeySnapShot.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | ES_WANTRETURN
				/*| WS_HSCROLL | ES_AUTOHSCROLL*/  // 自动水平滚动
				/*| WS_VSCROLL | ES_AUTOVSCROLL*/, rc, CWnd::FromHandle(GetHWND()), 10005);
			pUI->Attach(m_hotkeySnapShot.GetSafeHwnd());          
			return pUI;
		}
		return NULL;
	}
};


//class CSysCommonMsg : public CDialog
//{
//	DECLARE_DYNAMIC(CSysCommonMsg)
//
//public:
//	CSysCommonMsg(CWnd* pParent = NULL);   // 标准构造函数
//	virtual ~CSysCommonMsg();
//
//// 对话框数据
//	enum { IDD = IDD_WIZARD_SYS_COMMON_MSG_LOG };
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//	virtual BOOL OnInitDialog();
//	void OnOK()	{}
//	void OnCancel() {}
//	DECLARE_MESSAGE_MAP()
//public:
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnBnClickedRadioEnter();
//	void  SaveConfig();
//	CFlatHotKeyCtrl m_hotkeyMsglog;
//	CFlatHotKeyCtrl m_hotkeySnapShot;
//	afx_msg void OnBnClickedCheckLog();
//	afx_msg void OnBnClickedCheckSnapshot();
//	void  OnVScroll(UINT   nSBCode,   UINT   nPos,   CScrollBar*   pScrollBar);
//	CEdit m_cEdit;
//	CComboBox m_cComboStatus;
//	afx_msg void OnBnClickedCheckStatus();
//
//	CCheckButton m_checkShake;	/*IDC_CHECK_SHAKE*/
//	CRadioButton m_radioEnter;/* IDC_RADIO_ENTER*/
//	CRadioButton m_radioCtrlEnter;/* IDC_RADIO_CTRL_ENTER*/
//	CCheckButton m_checkLog;	/*IDC_CHECK_LOG*/
//	CCheckButton m_checkSnapshot;	/*IDC_CHECK_SNAPSHOT*/
//};
