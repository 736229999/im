#pragma once
#include "resource.h"
#include "hyperbtn.h"

// CDlgMyArts �Ի���

class CDlgMyArts : public CDialog
{
	DECLARE_DYNAMIC(CDlgMyArts)

public:
	CDlgMyArts(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgMyArts();

// �Ի�������
	enum { IDD = IDD_FORMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CHyperBtn m_staticArts;
	CHyperBtn m_staticWebArts;
	virtual BOOL OnInitDialog();
protected:
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
//	afx_msg void OnStnClickedStaticArts();
//	afx_msg void OnStnClickedStaticWebarts();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
