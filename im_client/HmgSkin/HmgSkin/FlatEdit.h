#pragma once



// CFlatEdit

class AFX_EXT_CLASS CFlatEdit : public CEdit
{
	DECLARE_DYNAMIC(CFlatEdit)

public:
	CFlatEdit();
	virtual ~CFlatEdit();

public:
	void SetBorderColor(COLORREF clr);
	void SetBackColor(COLORREF clr);
	void SetTextColor(COLORREF clr);
	void EnablePaste(bool bEnable);

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnEnVscroll();
	HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	
	DECLARE_MESSAGE_MAP()

	void DrawBorder();

protected:
	COLORREF  m_clrBack;
	COLORREF  m_clrBorder;
	COLORREF  m_clrText;
	HBRUSH	  m_hBack;
	bool      m_bEnablePaste;

};


