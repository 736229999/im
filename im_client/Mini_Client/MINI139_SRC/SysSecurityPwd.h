#pragma once
#include "..\resource.h"
#include "..\im\impublic.h"
#include "..\..\SpaceManagement\public\public_controls\ResGdiObj.h"
// CSysSecurityPwd �Ի���
//

#include "..\..\..\DuiLib\UIlib.h"

class CSysSecurityPwd : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UISecurifyModPsw");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("ModPassWD.xml");  } 
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	afx_msg void OnBnClickedBtnPwd();

private:
	DuiLib::CButtonUI *m_pBtnModpwd; ////�޸�����
};

//class CSysSecurityPwd : public CDialog
//{
//	DECLARE_DYNAMIC(CSysSecurityPwd)
//
//public:
//	CSysSecurityPwd(CWnd* pParent = NULL);   // ��׼���캯��
//	virtual ~CSysSecurityPwd();
//
//// �Ի�������
//	enum { IDD = IDD_WIZARD_SYS_SECURITY_PWD };
//
//protected:
//	BOOL OnInitDialog();
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
//	void OnOK()	{}
//	void OnCancel() {}
//
//	DECLARE_MESSAGE_MAP()
//public:
//	afx_msg void OnBnClickedBtnPwd();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	CPngBtn m_btnPWD;
//};
