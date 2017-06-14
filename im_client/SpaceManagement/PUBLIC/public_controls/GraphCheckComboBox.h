#pragma once


class CGraphCheckComboBox : public CComboBox
{
	DECLARE_DYNAMIC(CGraphCheckComboBox)

public:

	// -> Remarks:  Constructs a CGraphCheckComboBox object.
	CGraphCheckComboBox();

	// -> Remarks:  Destroys a CGraphCheckComboBox object, handles cleanup and de-allocation.
	virtual ~CGraphCheckComboBox();

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


	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID);

	// Selects all/unselects the specified item
	void SetCheck(INT nIndex, BOOL bFlag);

	// Returns checked state
	BOOL GetCheck(INT nIndex);

	// Selects all/unselects all
	void SelectAll(BOOL bCheck = TRUE);


protected:
	// Routine to update the text
	void RecalcText();

	// The subclassed COMBOLBOX window (notice the 'L')
	HWND m_hListBox;

	// The string containing the text to display
	CString m_strText;

	BOOL m_bTextUpdated;

	// A flag used in MeasureItem, see comments there
	BOOL m_bItemHeightSet;

	map<int,DWORD> m_mapCheck;
	// -> Input:   bDisable - TRUE to disable flat appearance.
	// -> Remarks: This member function will disable the flat look for the combo box.
	virtual void DisableFlatLook(BOOL bDisable);

	// -> Input:   bEnable - TRUE to enable auto completion; otherwise FALSE.
	// -> Remarks: This member function enables or disables auto completion.
	virtual void EnableAutoCompletion(BOOL bEnable=TRUE);

	// -> Returns: TRUE if the mouse is over the combo box; otherwise FALSE.
	// -> Remarks: This member function determines if the mouse is within the controls rect.
	virtual BOOL PointInRect();

public:

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

	//{{AFX_VIRTUAL(CGraphCheckComboBox)
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CGraphCheckComboBox)
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
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg LRESULT OnCtlColorListBox(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetText(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetTextLength(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDropDown();
	//}}AFX_MSG
	HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	DECLARE_MESSAGE_MAP()

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	virtual void PreSubclassWindow();
};

//////////////////////////////////////////////////////////////////////////////

AFX_INLINE void CGraphCheckComboBox::DisableFlatLook(BOOL bDisable) {
	m_bFlatLook = !bDisable;
}
AFX_INLINE void CGraphCheckComboBox::EnableAutoCompletion(BOOL bEnable/*=TRUE*/) {
	ASSERT(::IsWindow(m_hWnd)); m_bAutoComp = bEnable;
}
AFX_INLINE BOOL CGraphCheckComboBox::PointInRect() {
	ASSERT(::IsWindow(m_hWnd)); CPoint pt; GetCursorPos(&pt); CRect rcItem; GetWindowRect(&rcItem); return rcItem.PtInRect(pt);
}
AFX_INLINE void CGraphCheckComboBox::SetBorderColor(COLORREF clr) {
	m_clrBorder = clr; 
}
AFX_INLINE void CGraphCheckComboBox::SetTextColor(COLORREF clr){
	m_clrText = clr;
}
AFX_INLINE void CGraphCheckComboBox::SetBackColor(COLORREF clr){
	m_clrBack = clr;
	if ( m_hBack )
		DeleteObject(m_hBack);
	m_hBack = CreateSolidBrush(m_clrBack);
}
