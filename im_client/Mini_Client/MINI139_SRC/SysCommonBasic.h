#pragma once
#include "..\resource.h"
#include "..\im\impublic.h"
#include "afxwin.h"
#include "MainFrame.h"
#include "..\..\..\HmgSkin\HmgSkin\CheckButton.h"
// CSysCommonBasic �Ի���

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
	// ����
    void  SaveLoginSetting();

private:
	DuiLib::COptionUI *m_pCheckStartRun;    ////�Զ�����
	DuiLib::COptionUI *m_pCheckExitConfirm; ////�˳�ȷ��
	DuiLib::COptionUI *m_pCheckEndgeHide;   ////�Զ�����
	DuiLib::COptionUI *m_pCheckFrontShow;   ////��ǰ��ʾ
};



////
//////class CSysCommonBasic : public CDialog
//////{
//////	DECLARE_DYNAMIC(CSysCommonBasic)
//////
//////public:
//////	CSysCommonBasic(CWnd* pParent = NULL);   // ��׼���캯��
//////	virtual ~CSysCommonBasic();
//////// �Ի�������
//////	enum { IDD = IDD_WIZARD_SYS_COMMON_BASIC };
//////
//////protected:
//////	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
//////	BOOL m_bAutoRun;		// �Ƿ񿪻��Զ�����
//////};
