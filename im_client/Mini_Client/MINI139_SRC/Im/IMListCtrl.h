#pragma once

#define ICON_LINE RGB(173,187,210)

class CResBitmap;

class CIMListCtrl : public CWnd
{
	DECLARE_DYNAMIC(CIMListCtrl)

public:
	CIMListCtrl();
	virtual ~CIMListCtrl();

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()

protected:
	void DrawBitmap(CDC *pDC,int nDestX, int nDestY, CBitmap *pBitmap, int nSrcX, int nSrcY, int nWidth, int nHeight, COLORREF clrTrans);
	void DrawBitmap(CDC *pDC,int x, int y, UINT nIDBmp,bool bBlackWhite=false);
	void DrawBitmap(CDC *pDC,int x, int y, CBitmap *pBitmap,bool bBlackWhite=false);
	void DrawPtInRect(const CPoint &ptCursor,const CRect & rcRect,CDC *pDC,int nStyle, bool bMouseDown);
	void ShowToolTip(CString strTip);
	virtual void InitToolTip();

protected:
	CToolTipCtrl m_ToolTip;	
	CString      m_strPreTip ;
	bool		 m_bMouseDown;
};
