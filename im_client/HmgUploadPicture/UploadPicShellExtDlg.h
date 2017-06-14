#pragma once
#include "afxcmn.h"


// CUploadPicShellExtDlg 对话框

class CUploadPicShellExtDlg : public CDialog
{
	DECLARE_DYNAMIC(CUploadPicShellExtDlg)

public:
	CUploadPicShellExtDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUploadPicShellExtDlg();

// 对话框数据
	enum { IDD = IDD_DLG_UPLOADPICSHELLEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_wndUpFileListCtrl;
protected:
	virtual BOOL OnInitDialog();
	void ShowUpFileNumInfo();
protected:
	CString m_strWindowText;
public:
	void SetParamInfo(const CString & strParamInfo);
};
