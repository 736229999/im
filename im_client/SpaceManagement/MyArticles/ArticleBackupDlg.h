#pragma once
#include "afxwin.h"


// CArticleBackupDlg �Ի���

class CArticleBackupDlg : public CDialog
{
	DECLARE_DYNAMIC(CArticleBackupDlg)

public:
	CArticleBackupDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CArticleBackupDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_ARTICLE_BACKUP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPath();
	CEdit m_edtPath;
	CEdit m_edtName;
};
