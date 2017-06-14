#pragma once

enum AFX_EXT_CLASS MASK  { NONE, IP, NUMBER, };

class AFX_EXT_CLASS CGraphComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CGraphComboBox)

public:

	// -> Remarks:  Constructs a CGraphComboBox object.
	CGraphComboBox();

	// -> Remarks:  Destroys a CGraphComboBox object, handles cleanup and de-allocation.
	virtual ~CGraphComboBox();

protected:

	// enum used to determine the state the combo box should be
	enum STATE { NORMAL = 1, RAISED = 2, PRESSED = 3 };
	int      m_cxHThumb;     // Size of the drop arrow for the combo box.
	BOOL     m_bLBtnDown;    // TRUE if left mouse button is pressed
	BOOL     m_bPainted;     // used during paint operations
	BOOL     m_bHasFocus;    // TRUE if control has focus
	BOOL     m_bAutoComp;    // Used by Autocompleting.
	BOOL     m_bFlatLook;    // TRUE if the control is flat.
	BOOL     m_bDisableAC;   // TRUE when auto complete is disabled internally.
	COLORREF m_clrBorder ,m_clrBack, m_clrText;
	HBRUSH   m_hBack;
	MASK     m_mask      ;
public:

	// -> Input:   bDisable - TRUE to disable flat appearance.
	// -> Remarks: This member function will disable the flat look for the combo box.
	virtual void DisableFlatLook(BOOL bDisable);

	// -> Input:   bEnable - TRUE to enable auto completion; otherwise FALSE.
	// -> Remarks: This member function enables or disables auto completion.
	virtual void EnableAutoCompletion(BOOL bEnable=TRUE);

	// -> Returns: TRUE if the mouse is over the combo box; otherwise FALSE.
	// -> Remarks: This member function determines if the mouse is within the controls rect.
	virtual BOOL PointInRect();

	void SetBackColor(COLORREF clr);
	void SetTextColor(COLORREF clr);
	void SetBorderColor(COLORREF clr);
	void SetMask(MASK mask);
	bool IsFocused() { return m_bHasFocus; }

protected:

	// -> Input:   pDC            - Points to the current device context.
	//             eState         - Current state of the combo box, either NORMAL, RAISED, or PRESSED.
	//             clrTopLeft     - RGB color value for top left 3D borders.
	//             clrBottomRight - RGB color value for bottom right 3D borders.
	//             
	// -> Remarks: This member function is called by the combo box whenever a paint 
	//             operation should occur.
	virtual void DrawCombo(CDC* pDC, STATE eState);

	// -> Input:   pDC - Points to the current device context.
	// -> Remarks: This member function is called by the control to for flicker
	//             free drawing.
	void OnNoFlickerPaint(CDC* pDC);

	void DramBmp(CDC *pDC, RECT &rect, CBitmap *pBitmap);

	//{{AFX_VIRTUAL(CGraphComboBox)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CGraphComboBox)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnEditUpdate();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG
	HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};

//////////////////////////////////////////////////////////////////////////////

AFX_INLINE void CGraphComboBox::DisableFlatLook(BOOL bDisable) {
	m_bFlatLook = !bDisable;
}
AFX_INLINE void CGraphComboBox::EnableAutoCompletion(BOOL bEnable/*=TRUE*/) {
	ASSERT(::IsWindow(m_hWnd)); m_bAutoComp = bEnable;
}
AFX_INLINE BOOL CGraphComboBox::PointInRect() {
	ASSERT(::IsWindow(m_hWnd)); CPoint pt; GetCursorPos(&pt); CRect rcItem; GetWindowRect(&rcItem); return rcItem.PtInRect(pt);
}
AFX_INLINE void CGraphComboBox::SetBorderColor(COLORREF clr) {
	m_clrBorder = clr; 
}
AFX_INLINE void CGraphComboBox::SetTextColor(COLORREF clr){
	m_clrText = clr;
}
AFX_INLINE void CGraphComboBox::SetBackColor(COLORREF clr){
	m_clrBack = clr;
	if ( m_hBack )
		DeleteObject(m_hBack);
	m_hBack = CreateSolidBrush(m_clrBack);
}
