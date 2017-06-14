#pragma once

#include "ResGdiObj.h"

// CAlbumFolder

class CAlbumFolder : public CButtonBase
{
	DECLARE_DYNAMIC(CAlbumFolder)

public:
	CAlbumFolder();
	virtual ~CAlbumFolder();

public:
	int AddItem(CString strItemText, UINT nBitmap, DWORD dwData, int nIndex=-1 );
	void SetItemText(int nItem, CString strItemText);
	int GetCurSel();
	int GetCount();
	DWORD GetItemData(int nItem);

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	void OnTimer(UINT nIDEvent);
	void OnMouseMove(UINT nFlags, CPoint point);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

protected:
	int	  HitTest(CPoint point);

protected:
	struct tagItemInfo
	{
		CString strItemText;
		CRect	rcItem;
		UINT    nBitmap;
		DWORD   dwData;
	};

	CRect   m_rcBtnUp, m_rcBtnDown;
	vector<tagItemInfo> m_vecItem;
	int		m_nCurSel;
	int		m_nOriginIndex;
	int		m_nHotItem;
	bool	m_bShowScrollBtn;
	bool	m_bMouseDown;
	CFont   m_Font;
};


