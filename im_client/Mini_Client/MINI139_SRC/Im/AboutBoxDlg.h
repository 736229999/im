#pragma once


// CAboutBoxDlg �Ի���

class CAboutBoxDlg : public CDialog
{
	DECLARE_DYNAMIC(CAboutBoxDlg)

public:
	virtual ~CAboutBoxDlg();

	static void Show();
	static void Close();

// �Ի�������
	enum { IDD = IDD_DIALOG_ABOUTBOX };

protected:
	CAboutBoxDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
