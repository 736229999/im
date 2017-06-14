#pragma once

// CProgressCtrlEx
#include <afxcmn.h>

class CProgressCtrlEx : public CProgressCtrl
{
	DECLARE_DYNAMIC(CProgressCtrlEx)
public:
	CProgressCtrlEx();
	virtual ~CProgressCtrlEx();
protected:
	DECLARE_MESSAGE_MAP()
private:
	CToolTipCtrl m_wndToolTip;
public:
	void SetToolTipText(const CString & strToolTipText, const int offset = 15);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg void OnNcPaint();
private:
	CBitmap m_bkBitmap;
	CBitmap m_foreBitmap;

	CBitmap	m_Bitmap_Gray;
	CBitmap m_Bitmap_Green;
	CBitmap m_Bitmap_Blue;
	
	CBitmap m_Bitmap_LeftLine;
	CBitmap m_Bitmap_RightLine;

	int m_nLower, m_nUpper;
	float m_nCurrentPosition;
	float m_nCurrentPosition2;
public:
	void Init();
	void SetCurPos(const float nPos);
	void SetCurPos2(const float nPos2);
private:
	int GetPosWidth(const float nPos, const CRect & rc);
public:
	afx_msg void OnPaint();
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
private:
	CRect m_rcCurrentPosition1, m_rcCurrentPosition2, m_rcCurrentPosition3;
	int m_nCurrentPositionWidth1, m_nCurrentPositionWidth2;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	BOOL	m_bFirstEntry;
	int		nPos;
};


