#pragma once

#include "afxcmn.h"

#define CResBitmap CBitmap

class CButtonBase : public CButton
{
	DECLARE_DYNAMIC(CButtonBase)
public:
	CButtonBase();
	virtual ~CButtonBase();

public:
	void SetToolTip(CString strTip);
	void SetButtonData(DWORD dwData) { m_dwData=dwData;	}
	DWORD GetButtonData() { return m_dwData;	}


protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

	void InitToolTip();

protected:
	CToolTipCtrl m_ToolTip;
	CFont        m_Font   ;
	DWORD        m_dwData;
};




// CBmpBtn

class CBmpBtn : public CButtonBase
{
	DECLARE_DYNAMIC(CBmpBtn)

public:
	CBmpBtn();
	virtual ~CBmpBtn();

public:
	
protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// Implementation
public:
	void SetBitmap(UINT nHover, UINT nNormal, UINT nDown, UINT nDisable);
	CSize SizeToContent();

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

private:
	bool		 m_bTracking;
	bool		 m_bOver;
	COLORREF     m_clrTransparent;


	CResBitmap		 m_bmpHover, m_bmpDown, m_bmpNormal, m_bmpDisable;
};


class CBmpBtn2 : public CButtonBase
{
	DECLARE_DYNAMIC(CBmpBtn2)

public:
	CBmpBtn2();
	virtual ~CBmpBtn2();

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

// Implementation
public:
	void SetBitmap(UINT nNormal, UINT nDown, UINT nSel);
	bool IsCheck() { return m_bCheck; }
	void SetCheck(bool bCheck=true);
	bool SetDown(bool bDown=true) { m_bDown=true; }
	bool IsDown() { return m_bDown;	}

	CSize SizeToContent();
protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

private:
	bool		 m_bTracking;
	bool         m_bOver;
	bool         m_bCheck;
	bool         m_bDown;

	COLORREF     m_clrTransparent;

	CResBitmap		 m_bmpDown, m_bmpNormal, m_bmpSel;
};



class CXpButton : public CButtonBase
{
	DECLARE_DYNAMIC(CXpButton)

public:
	CXpButton();
	virtual ~CXpButton();

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CButtonEx)
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

	// Implementation
public:
	void SetImageIcon(UINT nImage);

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

private:
	bool		m_bTracking;
	bool		m_bOver;
	COLORREF    m_clrTransparent;

	UINT        m_nImage;
};



 //CFlatButton

class CFlatButton : public CButtonBase
{
	DECLARE_DYNAMIC(CFlatButton)

public:
	CFlatButton();
	virtual ~CFlatButton();

	enum FLAT_STYLE {
		FS_NORMAL,
		FS_OFFICE,
	};

	void Invalidate();

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();

public:
	void SetBitmap(UINT nIDNormal, UINT nIDDisable,COLORREF clrTransparent=RGB(0,255,0));
	void SetDown(bool bDown=true);
	void SetFlatStyle(FLAT_STYLE style) { m_btnFs=style; }
	inline bool IsDown() { return m_bDown; }
	void SetTextColor(COLORREF clrText) { m_clrText = clrText; }

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
private:
	CResBitmap		m_bmpNormal, m_bmpDisable;
	bool		m_bTracking;
	bool		m_bOver;
	bool		m_bMouseDown;
	bool		m_bDown;
	COLORREF    m_clrTransparent;
	COLORREF    m_clrText;
	CFont		m_Font;

	FLAT_STYLE  m_btnFs;
	BOOL        m_bLockOver ;

	bool        m_bErase;
};


class CColumnBtn: public CButtonBase
{
	DECLARE_DYNAMIC(CColumnBtn)

public:
	CColumnBtn();
	virtual ~CColumnBtn();

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	// Implementation
public:
	void SetStyle( int iStyle=0/*0:left , 1 right*/) { m_iStyle=iStyle; }
	CSize SizeToContent();

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

private:
	CResBitmap m_bmpTopNormal, m_bmpTopOver,m_bmpTopDown, m_bmpTopDisable;
	CResBitmap m_bmpMidNormal, m_bmpMidOver,m_bmpMidDown, m_bmpMidDisable;
	CResBitmap m_bmpBottomNormal, m_bmpBottomOver,m_bmpBottomDown, m_bmpBottomDisable;

	BOOL	m_bTracking;
	BOOL	m_bOver;
	int		m_iStyle;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
