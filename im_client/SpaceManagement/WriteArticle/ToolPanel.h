#pragma once
#include "resource.h"
#include "buttonext.h"
#include "hyperbtn.h"
#include  "insertPanel.h"
#include "DlgMyArts.h"
#include "afxwin.h"

// CToolPanel 对话框

class CToolPanel : public CDialog
{
	DECLARE_DYNAMIC(CToolPanel)

public:
	CToolPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CToolPanel();
	void DrawFrameOpen(CPaintDC * dc);
	void AdjustUI();
	

// 对话框数据
	enum { IDD = IDD_TOOL_PANEL };
private:
	CinsertPanel m_insertDlg;
	CDlgMyArts     m_DlgMyArts;
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	CFlatButton m_delButton;
	CHyperBtn m_staticOpen;
	virtual BOOL OnInitDialog();
	CHyperBtn m_staticOpen2;
	CFlatButton m_delButton2;
	CHyperBtn m_staticOpen3;
	CFlatButton m_delButton3;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	CHyperBtn m_StaticMore;
/*	CHyperBtn m_staticPArts1;
	CHyperBtn m_staticPArt2;
	CHyperBtn m_staticPArt3;
	CHyperBtn m_staticPArts*/;
	//CHyperBtn m_staticMySpace;
	CHyperBtn m_staticViewSpace;
	CFont    m_font;
	
	CResBitmap  m_resBmp;
};
