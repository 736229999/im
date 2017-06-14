#pragma once

class CScreenCopyHintDlg : public CDialog
{
	DECLARE_DYNAMIC(CScreenCopyHintDlg)

public:
	CScreenCopyHintDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScreenCopyHintDlg();

public:
	void SetScreenSize(int cx, int cy) { m_iCx=cx, m_iCy=cy; }

	void SetPointColor(const CPoint &pt, COLORREF color);
	// 对话框数据
	enum { IDD = IDD_SCREEN_COPY_HINT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:
	int m_iCx, m_iCy;//screen size
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};

// CScreenCopyDlg 对话框

class CScreenCopyDlg : public CDialog
{
	DECLARE_DYNAMIC(CScreenCopyDlg)

public:
	CScreenCopyDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScreenCopyDlg();

	CString    m_strImageFile   ;
// 对话框数据
	enum { IDD = IDD_SCREEN_COPY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

private:
	CBitmap    m_bitmapScreen       ;
	CBitmap    m_bitmapBack         ;
	CResBitmap m_bitmapTip          ;
	BOOL       m_bMouseDown			;

	CSize      m_szScreen           ;
	CPoint     m_ptNow  , m_ptOrigin;

	int		   m_iTipHeight      ;
	BITMAP     m_biTip			 ;
	POINT      m_ptTip			 ;
	
};
