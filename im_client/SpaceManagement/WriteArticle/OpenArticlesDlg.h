#pragma once
#include "resource.h"
#include "afxcmn.h"
#include "SEOListCtrl.h"
#include "afxwin.h"
#include "listboxst.h"


// COpenArticlesDlg �Ի���

class COpenArticlesDlg : public CDialog
{
	DECLARE_DYNAMIC(COpenArticlesDlg)

public:
	COpenArticlesDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COpenArticlesDlg();

	int		m_nArticleType;//0 local, 1 web
	DWORD	m_dwArticleID;
	
// �Ի�������
	enum { IDD = IDD_DIALOG_OPEN };
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeList1();
	DECLARE_MESSAGE_MAP()

	LRESULT OnWebDataNotify(WPARAM wParam, LPARAM lParam);

protected:
	CImageList		m_ImageList;
	CListBoxST		m_wndListBox;
	CSEOListCtrl	m_wndListArticles;


	TUserWebArticleInfo m_myArticles;//(g_pUserInfo->GetUserID());

public:
	afx_msg void OnBnClickedButtonDel();
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnBnClickedButtonPcancle();
	afx_msg void OnBnClickedButtonRefresh();
protected:
	CButton m_btnRefresh;
};
