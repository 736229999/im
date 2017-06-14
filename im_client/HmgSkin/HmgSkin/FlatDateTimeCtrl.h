#pragma once


// CFlatDateTimeCtrl

class AFX_EXT_CLASS CFlatDateTimeCtrl : public CDateTimeCtrl
{
	DECLARE_DYNAMIC(CFlatDateTimeCtrl)

public:
	CFlatDateTimeCtrl();
	virtual ~CFlatDateTimeCtrl();
	void SetBorderColor(COLORREF color)
	{
		m_clrBorder = color;
	}

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	DECLARE_MESSAGE_MAP()

	enum STATE { NORMAL = 1, RAISED = 2, PRESSED = 3 };

	virtual BOOL PointInRect();
	virtual void DrawCombo(CDC* pDC, STATE eState);
	void OnNoFlickerPaint(CDC* pDC);
	void DramBmp(CDC *pDC, RECT &rect, CBitmap *pBitmap);

protected:
	int      m_cxHThumb;     // Size of the drop arrow for the combo box.
	BOOL     m_bLBtnDown;    // TRUE if left mouse button is pressed
	BOOL     m_bPainted;     // used during paint operations
	BOOL     m_bHasFocus;    // TRUE if control has focus
	BOOL     m_bAutoComp;    // Used by Autocompleting.
	BOOL     m_bFlatLook;    // TRUE if the control is flat.
	BOOL     m_bDisableAC;   // TRUE when auto complete is disabled internally.
	COLORREF m_clrBorder ,m_clrBack, m_clrText;
	HBRUSH	 m_hBack;
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnDtnDropdown(NMHDR *pNMHDR, LRESULT *pResult);
};

AFX_INLINE BOOL CFlatDateTimeCtrl::PointInRect() {
	ASSERT(::IsWindow(m_hWnd)); CPoint pt; GetCursorPos(&pt); CRect rcItem; GetWindowRect(&rcItem); return rcItem.PtInRect(pt);
}

