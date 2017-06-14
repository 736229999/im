#pragma once
#include "..\resource.h"
#include "..\im\impublic.h"
#include "afxwin.h"
#include "MainFrame.h"
#include "..\..\..\HmgSkin\HmgSkin\CheckButton.h"
// CSysCommonBasic 对话框

#include "..\..\..\DuiLib\UIlib.h"

class CSysCommonBasic : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("BasicSetting");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("BasicSetting.xml");  }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	// 保存
    void  SaveLoginSetting();

private:
	DuiLib::COptionUI *m_pCheckStartRun;    ////自动运行
	DuiLib::COptionUI *m_pCheckExitConfirm; ////退出确认
	DuiLib::COptionUI *m_pCheckEndgeHide;   ////自动隐藏
	DuiLib::COptionUI *m_pCheckFrontShow;   ////最前显示
};



////
//////class CSysCommonBasic : public CDialog
//////{
//////	DECLARE_DYNAMIC(CSysCommonBasic)
//////
//////public:
//////	CSysCommonBasic(CWnd* pParent = NULL);   // 标准构造函数
//////	virtual ~CSysCommonBasic();
//////// 对话框数据
//////	enum { IDD = IDD_WIZARD_SYS_COMMON_BASIC };
//////
//////protected:
//////	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//////	virtual BOOL OnInitDialog();
//////	void OnOK()	{}
//////	void OnCancel() {}
//////	DECLARE_MESSAGE_MAP()
//////public:
//////	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//////	afx_msg void OnBnClickedCheckExit();
//////	CCheckButton m_btnCheckAutoStart;
//////	afx_msg void OnBnClickedCheckAutorun();
//////	int SaveLoginSetting();
//////	afx_msg void OnBnClickedCheckTop();
//////	CCheckButton m_btnCheckHide;
//////	CCheckButton m_btnCheckShowTop;
//////	CCheckButton m_btnCheckExitWarn;
//////	BOOL m_bAutoRun;		// 是否开机自动启动
//////};
