#pragma once
#include "..\resource.h"
#include "..\im\impublic.h"
//#include "..\..\..\HmgSkin\HmgSkin\RadioButton.h"
// CSysCommonSoftwareUpdate �Ի���
#include "..\..\..\DuiLib\UIlib.h"

class CSysCommonSoftwareUpdate : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UISoftUpdate");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("SoftwareUpdate.xml");  }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	// ����
	void Save();

private:
	DuiLib::COptionUI *m_pRadioAutoInstall; ////�Զ���װ
	DuiLib::COptionUI *m_pRadioOnlyMention; ////����ʾ
};



//class CSysCommonSoftwareUpdate : public CDialog
//{
//	DECLARE_DYNAMIC(CSysCommonSoftwareUpdate)
//
//public:
//	CSysCommonSoftwareUpdate(CWnd* pParent = NULL);   // ��׼���캯��
//	virtual ~CSysCommonSoftwareUpdate();
//
//// �Ի�������
//	enum { IDD = IDD_WIZARD_SYS_COMMON_SOFT_UPDATE };
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
//	void OnOK()	{}
//	void OnCancel() {}
//	DECLARE_MESSAGE_MAP()
//	virtual BOOL OnInitDialog();
//public:
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnBnClickedRadioAuto();
//	void Save();
//
//private:
//	CRadioButton m_radioAuto;
//	CRadioButton m_radioMention;
//};
