#pragma once


// CVersionInfoDlg 对话框
#include "VersionUpdatgeDlg.h"

class CVersionInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CVersionInfoDlg)
	DECLARE_HMG_SCROLLBAR()

public:
	CVersionInfoDlg(int nVersonFlag, CString strVersionInfoUrl, CWnd* pParent );   // 标准构造函数
	virtual ~CVersionInfoDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_VERSION_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

private:
	int  m_nVersionFlag;//0正常,1可以不用下载,2强制下载

	CString			m_strTip;
	CString			m_strVersionInfoUrl;
	CFlatEdit		m_edtInfo;
public:
	afx_msg void OnPaint();
};
