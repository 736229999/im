#pragma once

#include "..\controls\folderlist.h"

// COutlookContactSelDlg 对话框

class COutlookContactSelDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(COutlookContactSelDlg)

public:
	COutlookContactSelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~COutlookContactSelDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_INVITE_SEL };

public:
	void AddContact(CString strName, CString strMailAddress);
	CString GetEmailAddress();

	static COutlookContactSelDlg * GetInstance() { return pInstance; }
	static COutlookContactSelDlg * Show() ;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();	
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);

	INT_PTR DoModal() { return 0; }

	DECLARE_MESSAGE_MAP()

protected:
	DECLARE_XT_MESSAGE_MAP()

	void OnInviteAck(const char *data, WORD wRet);

protected:	
	CFolderList		m_wndListContacts;

	CBmpBtn			m_btnOk		;
	CBmpBtn			m_btnCancel	;
	CImageList		m_ImageList	;

	CString         m_strEmailAddress;

	map<int,CString>  m_mapAddress;

	static COutlookContactSelDlg * pInstance;

	uint32         m_nSequenceID;
public:
	afx_msg void OnDestroy();
};
