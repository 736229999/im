#pragma once
#include "afxext.h"
#include "stdafx.h"

#define TRACKER_TYPE_SCREEN 1
#define TRACKER_TYPE_HEAD 2
class CScreenCtrl;
class CRectTrackerEx :
	public CRectTracker
{
public:
	CRectTrackerEx(void);
	~CRectTrackerEx(void);

	CScreenCtrl* m_pScreenCtrl;
	void SetScreenCtrl(CScreenCtrl* pScreenCtrl);

	BOOL m_bIsRefresh;
	BOOL m_bIsDrawPointCross;
	int m_nTrackerType;
	DWORD m_dwColor;
	void SetTrackerType(int nType);
	void SetTrackerColor(DWORD dwColor);
	void Draw(CDC* pDC) const;
	int HitTestHandles(CPoint point) const;
	BOOL SetCursor(CWnd* pWnd, UINT nHitTest) const;
	BOOL Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert,
		CWnd* pWndClipTo = NULL);
	BOOL TrackHandle(int nHandle, CWnd* pWnd, CPoint point,
		CWnd* pWndClipTo);
	void DrawTrackerRect(
		LPCRECT lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd);
};

