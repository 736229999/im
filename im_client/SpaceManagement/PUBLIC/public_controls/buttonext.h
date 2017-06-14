#pragma once

#include "HmgSkinControl.h"
#include "ResGdiObj.h"
#include "atlimage.h"
#include <atlimage.h>
class CBmpBtn : public CButtonBase
{
	DECLARE_DYNAMIC(CBmpBtn)

public:
	CBmpBtn();
	virtual ~CBmpBtn();

public:
	bool IsDown() { return m_bDown;	}
	void SetDown(bool bDown) { m_bDown=bDown;
	if ( m_hWnd )
	{
		Invalidate();
	}
	}

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

public:
	void SetBitmap(UINT nHover, UINT nNormal, UINT nDown, UINT nDisable);
	CSize SizeToContent();
	void SetTextColor(COLORREF clrText);
	void SetPng( UINT nNormal);

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

private:
	bool		 m_bTracking;
	bool		 m_bOver;
	bool		 m_bDown;
	COLORREF     m_clrTransparent;
	COLORREF	 m_clrText;


	CResBitmap		 m_bmpHover, m_bmpDown, m_bmpNormal, m_bmpDisable;
	CImage m_imgNormal;
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
	void SetDown(bool bDown=true) { m_bDown=true; }
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

	enum ALIGN_STYLE {
		AS_LEFT,
		AS_BOTTOM,
	};

	void Invalidate();

protected:
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void PreSubclassWindow();

public:
	void SetBitmap(UINT nIDNormal, UINT nIDDisable,COLORREF clrTransparent=RGB(0,255,0));
	void SetDown(bool bDown=true);
	void SetFlatStyle(FLAT_STYLE flat_style,ALIGN_STYLE align_style=AS_LEFT) { m_btnFs=flat_style; m_btnAs=align_style; }
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

	FLAT_STYLE  m_btnFs;
	ALIGN_STYLE m_btnAs;
	BOOL        m_bLockOver ;

	bool        m_bErase;
};

/*
class AFX_EXT_CLASS CCheckBox : public CButton
{

public:
	CCheckBox();
	virtual ~CCheckBox();

public:
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);


protected:


protected:

};
*/

class CPngBtn : public CButtonBase
{
	DECLARE_DYNAMIC(CPngBtn)

public:
	CPngBtn();
	virtual ~CPngBtn();

public:
	bool IsDown() { return m_bDown; }
	void SetDown(bool bDown) {
		m_bDown = bDown;
		if (m_hWnd)
		{
			Invalidate();
		}
	}

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

public:
	CSize SizeToContent();
	void SetTextColor(COLORREF clrText);
	void SetPng(UINT nHover, UINT nNormal, UINT nDown, UINT nDisable);

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

private:
	bool		 m_bTracking;
	bool		 m_bOver;
	bool		 m_bDown;
	COLORREF	 m_clrText;

	CImage m_imgNormal;
	CImage m_imgHover;
	CImage m_imgDown;
	CImage m_imgDisable;

	CBitmap m_bitmap;
	BOOL m_bSaveBackground;
};



class CSplitBtn : public CButtonBase
{
	DECLARE_DYNAMIC(CSplitBtn)

public:
	CSplitBtn();
	virtual ~CSplitBtn();

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

public:
	CSize SizeToContent();
	void SetTextColor(COLORREF clrText);
	void SetBitmap(int nNormal, int nDown, int nSel);
	void SetBitmapSplit(int nNormal, int nDown, int nSel );
	void SetTogether(BOOL bTogether);

	// 鼠标是否点在了文本上
	BOOL PressedInText()
	{
		return m_bPressedText;
	}

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

private:
	void SetBitmap();
	void SetBitmapSplit();

	// 判断鼠标处于文本按钮上还是下拉按钮上
	BOOL PtInText(CPoint pt);

private:
	bool		 m_bTracking;
	bool		 m_bOver;
	COLORREF     m_clrTransparent;
	COLORREF	 m_clrText;

	bool		 m_bSplitTracking;
	bool		 m_bSplitOver;

	CImage       m_bmpHover, m_bmpDown, m_bmpNormal;
	CImage		 m_bmpSplitHover, m_bmpSplitDown, m_bmpSplitNormal;
	BOOL		 m_bPressedText;
	BOOL		 m_bTogether;
};
