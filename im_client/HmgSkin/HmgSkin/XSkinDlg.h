#pragma once

#include "hmgskinpublic.h"
#include "SkinBitmap.h"

extern DLL_DECLSPEC HBRUSH  brushBack;
extern DLL_DECLSPEC HBRUSH  brushWhite;


#define BACKGND_COLOR   RGB(0x4d,0x95,0xc2)
#define BORDER_COLOR  RGB(0x50,0x63,0x6e)
#define TEXT_COLOR RGB(0xFF,0xFF,0xFF )


#define TITLE_BAR_CY   26
#define BORDER_W 1 


#define SYS_BTN_SIZE 26
//#define CAPTION_BAR_HIGHT	32
class AFX_EXT_CLASS CXSkinDlg : public CDialog
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
	virtual		void          InitResource();

protected:
	static         int				m_iInstaceCount	;
	CFont				m_fontCaption   ;
	CFont				m_fontText		;
	CSkinBitmap			m_bmpTitleLeft;
	CSkinBitmap			m_bmpTitleMid;
	CSkinBitmap			m_bmpTitleRight;
	CSkinBitmap			m_bmpCaption	;
	CSkinBitmap			m_bmpMinNormal;
	CSkinBitmap         m_bmpMinHot;
	CSkinBitmap			m_bmpMinDown;
	CSkinBitmap			m_bmpMaxNormal;
	CSkinBitmap			m_bmpMaxHot;
	CSkinBitmap	        m_bmpMaxDown;
	CSkinBitmap			m_bmpCloseNormal;
	CSkinBitmap         m_bmpCloseHot;
	CSkinBitmap         m_bmpCloseDown;
	CSkinBitmap			m_bmpHelpNormal;
	CSkinBitmap			m_bmpHelpHot;
	CSkinBitmap			m_bmpHelpDown;
	CSkinBitmap			m_bmpRestoreNormal;
	CSkinBitmap		    m_bmpRestoreHot;
	CSkinBitmap			m_bmpRestoreDown;
	CSkinBitmap			m_bmpTopNormal;
	CSkinBitmap			m_bmpTopHot;
	CSkinBitmap			m_bmpTopDown;

protected:
	CToolTipCtrl	m_ToolTip		;

	MINMAXINFO	*	m_pMinMaxInfo   ;

	int				m_iCurBtn		;
	bool			m_bMouseDown    ;
	bool			m_bNcMouseDown  ;
	LRESULT			m_lNcTest		;
	int             m_iBtnDown		;
	DLG_STYLE		m_nStyle;	//0:max, min , close, btn;
								//1:min, close,btn
								//2:close, btn
	CRect			m_rcMin,m_rcMax,m_rcClose,m_rcHelp;
	CRect			m_rcSizeWnd		;
	
	bool			m_bMaxed		;//0,normal; 1,max
	CRect			m_rcRestore		;	
	bool			m_bModalDlg		;
	HBRUSH			m_hBkGround		;

	HICON           m_hIconSmall  ;
	HICON           m_hIcon         ;
	CStatic         m_wndTitle      ;

protected:
	void InitMinMaxInfo();
	void UpdateCursor();
	virtual void DrawCaptionBar();
	void DrawStyle(CDC* pDC);
	virtual void DrawBorder();
	void ShowToolTip(CString strTip, const CRect &rc);
	bool IsModalDlg() { return m_bModalDlg; }
	void RemoveRgnOfWnd(CRgn &rgn, CWnd *pWnd,int iOffset=0);
	void InitToolTip();
	BOOL OnHelpInfo(HELPINFO* lpHelpInfo);
	void DrawLine();

protected:
	static BOOL CALLBACK EnumChildProc (HWND hwnd, LPARAM lParam);
	vector<HWND>   m_vecHWndChild;
	vector<HWND>   m_vecLockedWnd;
	bool           m_bLockOperation;
	bool		   m_bHelp;
	DWORD		   m_dwHelpTick;
	COLORREF		m_colorBackground;
	COLORREF		m_colorBorder;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	// 窗口是否可拖动，窗口风格为ALL时，默认支持可拖动
	void EnableTracker(BOOL bEnable)	{	m_bEnableTracker = bEnable;	}

private:
	BOOL m_bEnableTracker;			// 窗口是否可拖动，窗口风格为ALL时，默认支持可拖动
};
