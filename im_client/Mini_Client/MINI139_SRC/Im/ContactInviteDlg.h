#pragma once

// CContactInviteDlg 对话框

class CContactInviteDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CContactInviteDlg)

public:
	CContactInviteDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CContactInviteDlg();

public:

	static void Show(CString strEmail,CString strDisplayName);
	static void Close();

// 对话框数据
	enum { IDD = IDD_DIALOG_INVITE_CONTACT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual INT_PTR DoModal() { return 0; }
	virtual void OnCancel() { DestroyWindow(); }
	virtual void OnOK() { }
	virtual void OnHelpClick();
	afx_msg void OnPaint();	
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedInvite();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()
	
	void SetEmail(CString strEmail, CString strDisplayName);

protected:
	DECLARE_XT_MESSAGE_MAP()

	void OnInviteAck(const char *data, WORD wRet);

protected:
	static vector<CContactInviteDlg *> vecInstance;

protected:
	CString     m_strEmail;
	CString		m_strDisplayName;

	uint32      m_nSequenceID;

	CFlatEdit	m_edtTo;
	CFlatEdit	m_edtMail;
	CFlatEdit	m_edtTitle;
	CBmpBtn		m_btnInvite;
	CBmpBtn		m_btnCancel;

	bool		m_bCanEditTo;
public:
	afx_msg void OnEnSetfocusEditTo();
	afx_msg void OnEnKillfocusEditTo();
};
