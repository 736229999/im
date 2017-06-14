#ifndef _SKIN_CTRL_
#define _SKIN_CTRL_

#include "ResGdiObj.h"

class CStaticBase : public CStatic
{
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
};

class CTransparentLabel : public CStaticBase
{
public:
	CTransparentLabel()
	{
		m_clrText= 0;
	}

	void SetTextColor(COLORREF clr) { m_clrText=clr; }
	void SetWindowText(CString sEndWithDot, CString sFullStr = "")
	{
		m_strName   = sEndWithDot;
		m_strStatus = sFullStr;
		
		CWnd *pParent = GetParent();

		if ( pParent && pParent->m_hWnd )
		{
			CRect rc;
			GetClientRect(&rc);
			ClientToScreen(&rc);

			pParent->ScreenToClient(&rc);
			pParent->InvalidateRect(&rc);
		}
	}

protected:
	afx_msg void OnPaint();
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()
private:
	COLORREF m_clrText;
	CString  m_strName, m_strStatus;
};

class CImageBack  : public CStaticBase
{
public :
	void SetBitmap(UINT nBitmap) {
		if ( m_bitmap.m_hObject)
			m_bitmap.DeleteObject();
		m_bitmap.LoadBitmap(nBitmap);
	}
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	CResBitmap m_bitmap;
};

class CImageLogo  : public CStaticBase
{
public :
	CImageLogo() { 
	 m_bitmap.LoadBitmap(IDB_LOGINING_TITLE);
	 m_bitmapLogo.LoadBitmap(IDB_FLAG_LOGIN); }

	void SetLogoText(CString strLogoText) ;
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	CString    m_strLogo;
	CResBitmap m_bitmap, m_bitmapLogo ;
};

class CImageTitle : public CStaticBase
{
public:
	CImageTitle() {	m_bitmap.LoadBitmap(IDB_MAIN_TITLE_BACK);}

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	CResBitmap m_bitmap;
};

class CImageMenuBtn : public CStaticBase
{
public:
	CImageMenuBtn() {	
		m_bitmapL.LoadBitmap(IDB_DLG_CAPTION_L);
		m_bitmapR.LoadBitmap(IDB_DLG_CAPTION_R);
		m_bitmapM.LoadBitmap(IDB_DLG_CAPTION_M);
		m_bitmapI.LoadBitmap(IDB_MAIN_MENU_ICON);
		m_bMouseOver = false;
	}

protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

protected:
	void DrawMenuBtn(CDC *pDC, const CRect &rcMenu, bool bBold);

private:
	CResBitmap m_bitmapL, m_bitmapM, m_bitmapR, m_bitmapI;
	bool m_bMouseOver;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
};

class CImageToolBtn : public CStaticBase
{
protected:
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP();
};


class CSAFrame : public CStatic
{
public:
	void Invalidate(CWnd *pWnd);
	CString GetWindowText() { return m_strText; }
	void SetWindowText(LPCSTR lpszString);
protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
private:
	CString m_strText;
};

class CEmotionSelList : public CStatic
{
public:
	CEmotionSelList()
	{
		m_nSel       = -1	;
		m_nPreHot    = -1   ;
		m_bMouseDown = false;
		m_pImageList = NULL ;
		m_bOver      = false;
	}

public:
	int GetSel() { return m_nSel; }
	void SetImageList(CImageList * pImageList ) { m_pImageList = pImageList; }

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

	int HitTest(CPoint point);

private:
	int				m_nSel			;
	int             m_nPreHot       ;
	bool			m_bMouseDown	;
	bool            m_bOver         ;
	CImageList *	m_pImageList	;
};

#endif 