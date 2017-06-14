#pragma once


// CCaptionBar

class CCaptionBar : public CStatic
{
	DECLARE_DYNAMIC(CCaptionBar)

public:
	CCaptionBar();
	virtual ~CCaptionBar();

public:
	void SetFaceId(int nId,int nStatus=0);

protected:
	virtual void PreSubclassWindow();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMouseHover(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	
	DECLARE_MESSAGE_MAP()

	//void SetCaptionRgn(CRgn &rgn,CRect rect);

private:
	bool		m_bOver;

	int			m_nFaceId;
	int			m_nStatus;

	COLORREF	m_clrTrans;
	COLORREF	m_clrBk;
	CRect		m_rcHover;
};


