#pragma once


// CAboutBoxDlg 对话框

class CAboutBoxDlg : public CDialog
{
	DECLARE_DYNAMIC(CAboutBoxDlg)

public:
	virtual ~CAboutBoxDlg();

	static void Show();
	static void Close();

// 对话框数据
	enum { IDD = IDD_DIALOG_ABOUTBOX };

protected:
	CAboutBoxDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK() { DestroyWindow(); }
	virtual void OnCancel() { DestroyWindow(); }

	DECLARE_MESSAGE_MAP()

	CHyperBtn  m_btn1;

	static CAboutBoxDlg * pInstance;
public:
	afx_msg void OnStnClickedStaticLink();
	afx_msg void OnNcDestroy();
	afx_msg void OnClose();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedOk();
};
