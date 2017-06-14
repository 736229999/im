#pragma once

#include <vector>
using namespace std;

extern HBRUSH  brushBack;
extern HBRUSH  brushWhite;

#define BORDER_COLOR   RGB(0x4d,0x95,0xc2)
#define BACKGND_COLOR  RGB(0x4d,0x95,0xc2)


#define TITLE_BAR_CY   25

#define SYS_BTN_SIZE 14

#include <afxcmn.h>

class CXSkinDlg : public CDialog
{
	DECLARE_DYNAMIC(CXSkinDlg)

public:
	CXSkinDlg(UINT nIDTemplate, CWnd* pParent = NULL);   // standard constructor
	virtual ~CXSkinDlg();

	enum DLG_STYLE { ALL, MINCLOSE, CLOSE, NONE, };

public:
	BOOL ShowWindow(int nCmdShow);
	INT_PTR DoModal();
	void SetDlgStyle(DLG_STYLE nStyle);

	//SetMinMaxInfo
	//nMinWidth 窗口最小宽度， 0表示默认尺寸
	//nMinHeight 窗口最小高度， 0表示默认尺寸
	void SetMinMaxInfo(int nMinWidth, int nMinHeight, int nMaxWidth, int nMaxHeight);
	
	void SetWindowText(LPCTSTR lpszString);
	void LockOperation();
	void UnLockOperation();

	void SetTitleIcon(HINSTANCE hInstance,UINT nID);
	void EnableHelp(bool bHelp);

// Implementation
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg BOOL OnNcActivate(BOOL bActive);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnHelp();

	DECLARE_MESSAGE_MAP()

	virtual void OnMinBtnClick();
	virtual void OnCloseBtnClick();
	virtual void OnMaxBtnClick();
	virtual void OnTitleBarDblClick();
	virtual void OnHelpClick();

	LRESULT	NcHitTest();

	void UpdateWindowLong();
	void DrawBitmap(CDC *pDC,int x, int y, CBitmap *pBitmap, COLORREF clrTrans=0);
	void Cx_Strech_Blt(CDC *pDestDC,int x, int y, int cx, CBitmap *pBitmap);

public :
	static		   void          InitResource();

protected:
	static         int				m_iInstaceCount	;
	static		   CFont			m_fontCaption   ;
	static		   CFont			m_fontText		;
	static		   CBitmap			m_bmpTitleLeft;
	static		   CBitmap			m_bmpTitleMid;
	static		   CBitmap			m_bmpTitleRight;
	static         CBitmap			m_bmpCaption	;
	static         CBitmap			m_bmpMinNormal;
	static		   CBitmap          m_bmpMinHot;
	static		   CBitmap			m_bmpMinDown;
	static         CBitmap			m_bmpMaxNormal;
	static         CBitmap			m_bmpMaxHot;
	static		   CBitmap	        m_bmpMaxDown;
	static         CBitmap			m_bmpCloseNormal;
	static         CBitmap          m_bmpCloseHot;
	static         CBitmap          m_bmpCloseDown;
	static		   CBitmap			m_bmpHelpNormal;
	static		   CBitmap			m_bmpHelpHot;
	static		   CBitmap			m_bmpHelpDown;
	static		   CBitmap			m_bmpRestoreNormal;
	static		   CBitmap		    m_bmpRestoreHot;
	static		   CBitmap			m_bmpRestoreDown;
	static         CBitmap			m_bmpTopNormal;
	static		   CBitmap			m_bmpTopHot;
	static		   CBitmap			m_bmpTopDown;

protected:
	CToolTipCtrl	m_ToolTip		;

	MINMAXINFO		m_objMinMaxInfo ;

	int				m_iCurBtn		;
	bool			m_bMouseDown    ;
	bool			m_bNcMouseDown  ;
	LRESULT			m_lNcTest		;
	int             m_iBtnDown		;
	DLG_STYLE		m_nStyle;	//0:max, min , close, btn;
								//1:min, close,btn
								//2:close, btn
	CRect			m_rcMin,m_rcMax,m_rcClose,m_rcHelp;
	
	bool			m_bMaxed		;//0,normal; 1,max
	CRect			m_rcRestore		;	
	bool			m_bModalDlg		;
	HBRUSH			m_hBkGround		;

	//HICON           m_hIconSmall  ;
	HICON           m_hIcon         ;
	CStatic         m_wndTitle      ;

protected:
	void InitMinMaxInfo();
	void UpdateCursor();
	void DrawCaptionBar();
	void DrawBorder();
	void ShowToolTip(CString strTip);
	bool IsModalDlg() { return m_bModalDlg; }
	void RemoveRgnOfWnd(CRgn &rgn, CWnd *pWnd,int iOffset=0);
	void InitToolTip();
	BOOL OnHelpInfo(HELPINFO* lpHelpInfo);

protected:
	static BOOL CALLBACK EnumChildProc (HWND hwnd, LPARAM lParam);
	vector<HWND>   m_vecHWndChild;
	vector<HWND>   m_vecLockedWnd;
	bool           m_bLockOperation;
	bool		   m_bHelp;
	DWORD		   m_dwHelpTick;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
