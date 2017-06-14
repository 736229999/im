#pragma once


class CVoiceSliderCtrl : public CStatic
{
	DECLARE_DYNAMIC(CVoiceSliderCtrl)

public:
	CVoiceSliderCtrl();
	virtual ~CVoiceSliderCtrl();

public:
	void SetRange(int iMin, int iMax);
	void SetPos(int iPos);
	int  GetPos() { return m_iPos; }
	void SetBackColor(COLORREF clr) { m_clrBackground=clr; }

protected:
	virtual void PreSubclassWindow();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
	
protected:
	int CalculatePosition(const CPoint &point);
	void Draw(CDC *pDC);
	void ReDraw();

protected:
	CRect   m_rcThum;

	int		m_iMin, m_iMax;
	int		m_iPos;

	bool	m_bMouseDown;
	bool	m_bMouseOver;

	int     m_nThumWidth, m_nThumHeight;

	COLORREF m_clrBackground;

	CResBitmap m_bmpThum;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


