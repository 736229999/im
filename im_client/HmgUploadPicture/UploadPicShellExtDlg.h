#pragma once
#include "afxcmn.h"


// CUploadPicShellExtDlg �Ի���

class CUploadPicShellExtDlg : public CDialog
{
	DECLARE_DYNAMIC(CUploadPicShellExtDlg)

public:
	CUploadPicShellExtDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUploadPicShellExtDlg();

// �Ի�������
	enum { IDD = IDD_DLG_UPLOADPICSHELLEXT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
