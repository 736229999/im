#pragma once
#include "afxwin.h"

// CCreateDirDlg 对话框

class CCreateDirDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateDirDlg)

public:
	CCreateDirDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCreateDirDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_CREATEDIR };

	vector<TBookInfo> * m_pBookList;
	TBookInfo           m_biCreated;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CButton m_btnOk;
	CButton m_btnCancel;
};
