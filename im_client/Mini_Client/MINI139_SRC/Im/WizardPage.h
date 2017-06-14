#pragma once


// CRegBasePage 对话框

class CWizardPage : public CDialog
{
	DECLARE_DYNAMIC(CWizardPage)

public:
	
	CWizardPage(UINT nID, CWnd* pParent = NULL);   // 标准构造函数
	
	virtual ~CWizardPage();

	virtual BOOL IsValid()
	{ return TRUE; };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

protected:
	HBRUSH  m_hBrushBackGround;
};
