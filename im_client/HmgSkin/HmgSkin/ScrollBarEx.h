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

	// ���ô��ڳ���
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
	CRect		m_rcSBThumb;			// ��������
	CRect		m_rcScrollBar;			// ���黬������
			
	CRect		m_rcSBTopBtn;			// ���ϻ���ָʾͼ��	
	CRect		m_rcSBBottomBtn;		// ���»���ָʾͼ��

	BOOL		m_bThumbPressed;
	CPoint		m_ClickPoint;

	int m_nParentWndNeedLength;				// ��������Ҫ��ʾ���ܳ���
	int m_nParentWndActuallyLength;			// ������ʵ���ϵĳ���

	CImage m_imgScroll;

	// ����ÿһ�λ�����Ҫǰ���ľ���
	int m_nParentMoveLengthPerSlide;
	// ��Ҫ�����Ĵ���
	int m_nSlideCount;

	CRect m_rcImgTopNormal;
	CRect m_rcImgBottomNormal;
	CRect m_rcImgMoveArea;
	CRect m_rcImgSlider;

	// �����ܹ������ľ���
	int m_nSliderCanMoveLength;		

	// ���鵱ǰ�����ľ���
	int m_nSliderCurMoveLength;

	BOOL m_bPtInTopBtn;			// ��������ϰ�ť�ϵ��
	BOOL m_bPtInBottomBtn;		// ��������°�ť�ϵ��	
};

#endif // !defined(AFX_SCROLLBAREX_H__5AF255D5_2DC1_4A9C_8501_13B93AF7F692__INCLUDED_)
