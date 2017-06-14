#pragma once


// CFlatEdit

class CFlatEdit : public CEdit
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
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	
	DECLARE_MESSAGE_MAP()

private:
	COLORREF  m_clrBack;
	COLORREF  m_clrBorder;
	COLORREF  m_clrText;
	HBRUSH	  m_hBack;
	bool      m_bEnablePaste;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnVscroll();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnEnChange();
private:
	BOOL	m_bFirst;
	CString m_strHintText;
};


