#pragma once
#include "resource.h"
#include "hyperbtn.h"
#include "afxwin.h"
#include "buttonext.h"
// CinsertPanel 对话框

class CinsertPanel : public CDialog
{
	DECLARE_DYNAMIC(CinsertPanel)

public:
	CinsertPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CinsertPanel();

// 对话框数据
	enum { IDD = IDD_INSERT_PANEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	CHyperBtn m_staticLink;
	CHyperBtn m_staticPic;
	CHyperBtn m_statWebPic;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
public :	
	CFlatButton m_buttonMyArts;
	CFlatButton m_buttonArtsInWeb;
	CBitmap    m_bmpInternet,m_bmpLocal,m_bmpLink;
//	afx_msg void OnStnClickedButtonWebp();
//	afx_msg void OnStnClickedButtonLocalfile();
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	CHyperBtn m_staticMyArts;
	CHyperBtn m_staticWebArts;
//	afx_msg void OnStnClickedStaticLink3();
//	afx_msg void OnStnClickedStaticLink2();
	afx_msg void OnPaint();
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
