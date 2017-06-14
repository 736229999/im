#pragma once
#include "afxwin.h"


// CArticleBackupDlg 对话框

class CArticleBackupDlg : public CDialog
{
	DECLARE_DYNAMIC(CArticleBackupDlg)

public:
	CArticleBackupDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CArticleBackupDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_ARTICLE_BACKUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPath();
	CEdit m_edtPath;
	CEdit m_edtName;
};
