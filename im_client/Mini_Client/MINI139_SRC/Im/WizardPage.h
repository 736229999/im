#pragma once


// CRegBasePage �Ի���

class CWizardPage : public CDialog
{
	DECLARE_DYNAMIC(CWizardPage)

public:
	
	CWizardPage(UINT nID, CWnd* pParent = NULL);   // ��׼���캯��
	
	virtual ~CWizardPage();

	virtual BOOL IsValid()
	{ return TRUE; };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnOK();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

protected:
	HBRUSH  m_hBrushBackGround;
};
