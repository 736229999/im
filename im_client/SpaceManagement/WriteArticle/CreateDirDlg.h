#pragma once
#include "afxwin.h"

// CCreateDirDlg �Ի���

class CCreateDirDlg : public CDialog
{
	DECLARE_DYNAMIC(CCreateDirDlg)

public:
	CCreateDirDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCreateDirDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_CREATEDIR };

	vector<TBookInfo> * m_pBookList;
	TBookInfo           m_biCreated;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CButton m_btnOk;
	CButton m_btnCancel;
};
