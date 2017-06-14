#pragma once
#include "resource.h"
#include "afxwin.h"
#include "SEOListCtrl.h"

// CPostedArtsDlg �Ի���

class CPostedArtsDlg : public CDialog
{
	DECLARE_DYNAMIC(CPostedArtsDlg)

public:
	CPostedArtsDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPostedArtsDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_POSTARTS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	virtual BOOL OnInitDialog();
	CComboBox m_listCount;
	CSEOListCtrl m_ListArticles;
	//afx_msg void OnBnClickedButton1();
};
