#pragma once
#include "..\resource.h"
#include "..\im\impublic.h"
#include "..\..\..\HmgSkin\HmgSkin\RadioButton.h"
// CSysSecurityVerify �Ի���
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
	// ����
	void SaveConfig();

private:
	DuiLib::COptionUI *m_pRadioAnyAllow; ////�����κ���
	DuiLib::COptionUI *m_pRadioVerify;     ////��Ҫ��֤
};
//class CSysSecurityVerify : public CDialog
//{
//	DECLARE_DYNAMIC(CSysSecurityVerify)
//
//public:
//	CSysSecurityVerify(CWnd* pParent = NULL);   // ��׼���캯��
//	virtual ~CSysSecurityVerify();
//	void SaveConfig();
//
//// �Ի�������
//	enum { IDD = IDD_WIZARD_SYS_SECURITY_VERIFY };
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
