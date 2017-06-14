#pragma once


// CHyperBtn
class AFX_EXT_CLASS CHyperBtn : public CStatic
{
	DECLARE_DYNAMIC(CHyperBtn)

public:
	CHyperBtn();
	virtual ~CHyperBtn();

public:
	void SetLinkColor(COLORREF clrLink) { m_clrLink=clrLink; }
	void SetNormalColor(COLORREF clrNormal) { m_clrNormal=clrNormal; }
	void SetWindowText(LPCSTR lpszString);
	void GetWindowText(CString &s);
	void SizeToContent();
	void SetAutoSize(bool bAutoSize);
protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void PreSubclassWindow();
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();

private:
	void Invalidate();

private:
	bool	 m_bAutoSize;
	bool     m_bOver;
	COLORREF m_clrLink;
	COLORREF m_clrNormal;
	CString  m_strText;

};


