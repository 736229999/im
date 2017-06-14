#pragma once
#include "..\resource.h"
#include "..\im\impublic.h"
#include "..\..\..\HmgSkin\HmgSkin\RadioButton.h"
// CSysSecurityFileTransfer �Ի���
////
#include "..\..\..\DuiLib\UIlib.h"

class CSysSecurityFileTransfer : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UISecurifyFileTrans");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("FileTransLevel.xml");  }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	// ����
	void SaveConfig();

private:
	DuiLib::COptionUI *m_pRadioHighLevel; ////��ȫ�����
	DuiLib::COptionUI *m_pRadioMidLevel;  ////��ȫ�����е�
	DuiLib::COptionUI *m_pRadioLowLevel;  ////��ȫ�����
};

////class CSysSecurityFileTransfer : public CDialog
////{
////	DECLARE_DYNAMIC(CSysSecurityFileTransfer)
////
////public:
////	CSysSecurityFileTransfer(CWnd* pParent = NULL);   // ��׼���캯��
////	virtual ~CSysSecurityFileTransfer();
////
////// �Ի�������
////	enum { IDD = IDD_WIZARD_SYS_SECURITY_FILE_TRANS };
////
////protected:
////	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
////	virtual BOOL OnInitDialog();
////	void OnOK()	{}
////	void OnCancel() {}
////	DECLARE_MESSAGE_MAP()
////public:
////	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
////	afx_msg void OnBnClickedRadioMid();
////	void SaveConfig();
////private:
////	CRadioButton m_radioHigh;
////	CRadioButton m_radioLow;
////	CRadioButton m_radioMid;
////};
