#pragma once

#ifndef XBUTTON_H
#define XBUTTON_H

#include "SkinBitmap.h"

class AFX_EXT_CLASS CButtonBase : public CButton
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


class AFX_EXT_CLASS CXpButton : public CButtonBase
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
	void SetButtonImage(HINSTANCE hInst, UINT nImage);

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

	CBitmap		m_bmpImage;
};


class AFX_EXT_CLASS CColumnBtn: public CButtonBase
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
	CSkinBitmap m_bmpTopNormal, m_bmpTopOver,m_bmpTopDown, m_bmpTopDisable;
	CSkinBitmap m_bmpMidNormal, m_bmpMidOver,m_bmpMidDown, m_bmpMidDisable;
	CSkinBitmap m_bmpBottomNormal, m_bmpBottomOver,m_bmpBottomDown, m_bmpBottomDisable;

	BOOL	m_bTracking;
	BOOL	m_bOver;
	int		m_iStyle;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
#endif XBUTTON_H