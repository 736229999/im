#if !defined(AFX_SCROLLBAREX_H__5AF255D5_2DC1_4A9C_8501_13B93AF7F692__INCLUDED_)
#define AFX_SCROLLBAREX_H__5AF255D5_2DC1_4A9C_8501_13B93AF7F692__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScrollBarEx.h : header file
#include <atlimage.h>
#define		WM_SCROLLBAR_MSG			WM_USER + 100

class CScrollBarEx : public CWnd
{
public:
	CScrollBarEx();
	virtual ~CScrollBarEx();

	// 设置窗口长度
	void SetWndLength(int nParentWndActuallyLength, int nParentWndNeedLength);

protected:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	DECLARE_MESSAGE_MAP()

public:
	BOOL Create(const RECT rect, CWnd *pParentWnd, UINT nID);

private:
	BOOL RegisterWindowClass(void); // Make it work as custom control
	void DrawTBButton(CDC *pDC);
	void DrawScrollZoneButton(CDC *pDC);
	void CalcRect();
	void SendMsgToParentWnd();
	void DrawEllipse(CDC *pDC, CRect &rcDest, CRect &rcImg);

private:
	CRect		m_rcSBThumb;			// 滑块区域
	CRect		m_rcScrollBar;			// 滑块滑动区域
			
	CRect		m_rcSBTopBtn;			// 向上滑动指示图标	
	CRect		m_rcSBBottomBtn;		// 向下滑动指示图标

	BOOL		m_bThumbPressed;
	CPoint		m_ClickPoint;

	int m_nParentWndNeedLength;				// 父窗口需要显示的总长度
	int m_nParentWndActuallyLength;			// 父窗口实际上的长度

	CImage m_imgScroll;

	// 计算每一次滑动需要前进的距离
	int m_nParentMoveLengthPerSlide;
	// 需要滑动的次数
	int m_nSlideCount;

	CRect m_rcImgTopNormal;
	CRect m_rcImgBottomNormal;
	CRect m_rcImgMoveArea;
	CRect m_rcImgSlider;

	// 滑块能够滑动的距离
	int m_nSliderCanMoveLength;		

	// 滑块当前滑动的距离
	int m_nSliderCurMoveLength;

	BOOL m_bPtInTopBtn;			// 鼠标在向上按钮上点击
	BOOL m_bPtInBottomBtn;		// 鼠标在向下按钮上点击	
};

#endif // !defined(AFX_SCROLLBAREX_H__5AF255D5_2DC1_4A9C_8501_13B93AF7F692__INCLUDED_)
