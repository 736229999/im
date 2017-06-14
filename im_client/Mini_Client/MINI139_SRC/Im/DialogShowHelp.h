#pragma once

#include "..\Resource.h"
// CDialogShowHelp �Ի���
#include "explorer1.h"
#include "afxwin.h"

class CDialogShowHelp : public CXSkinDlg
{
	DECLARE_DYNAMIC(CDialogShowHelp)

public:
	CDialogShowHelp(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogShowHelp();

// �Ի�������
	enum { IDD = IDD_DIALOG_SHOW_HELP };

	static void Show();
	static void Exit();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	CExplorer1 m_ieHelp;
	virtual BOOL OnInitDialog();
	CButton m_wndCheckNeedRun;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual void PostNcDestroy();

	static CDialogShowHelp* m_pInstance;
public:
	afx_msg void OnBnClickedCheckNeedRun();
	afx_msg void OnNcDestroy();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
