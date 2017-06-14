#pragma once
#include "..\resource.h"
#include "..\im\impublic.h"
#include "..\..\..\HmgSkin\HmgSkin\RadioButton.h"
// CSysSecurityVerify 对话框
#include "..\..\..\DuiLib\UIlib.h"

class CSysSecurityVerify : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UISecurifyVerify");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("VerifySetting.xml");  }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	// 保存
	void SaveConfig();

private:
	DuiLib::COptionUI *m_pRadioAnyAllow; ////允许任何人
	DuiLib::COptionUI *m_pRadioVerify;     ////需要验证
};
//class CSysSecurityVerify : public CDialog
//{
//	DECLARE_DYNAMIC(CSysSecurityVerify)
//
//public:
//	CSysSecurityVerify(CWnd* pParent = NULL);   // 标准构造函数
//	virtual ~CSysSecurityVerify();
//	void SaveConfig();
//
//// 对话框数据
//	enum { IDD = IDD_WIZARD_SYS_SECURITY_VERIFY };
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//	virtual BOOL OnInitDialog();
//	void OnOK()	{}
//	void OnCancel() {}
//	DECLARE_MESSAGE_MAP()
//private:
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnBnClickedRadioVerify();
//	afx_msg void OnBnClickedRadioAny();
//	
//	CRadioButton m_radioAny;
//	CRadioButton m_radioVerify;
