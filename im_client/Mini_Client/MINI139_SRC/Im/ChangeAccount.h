#pragma once

#include "XSkinDlg.h"
#include "../Resource.h"
#include "explorer1.h"
// CChangeAccountDlg �Ի���

class CChangeAccountDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CChangeAccountDlg)

public:
	
	CChangeAccountDlg(CWnd* pParent = NULL);   // ��׼���캯��
	
	virtual ~CChangeAccountDlg();

// �Ի�������
	enum { IDD = IDD_CHANGE_ACCOUNT };

	static void	NewWnd();

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
	CExplorer1 m_exp;
	afx_msg void OnPaint();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
};
