#pragma once

// CMenuCtrl
#include <vector>

// 控件边框绘制
#ifndef FRAME_DRAW_NULL
#define FRAME_DRAW_NULL 0x00
#endif
#ifndef FRAME_DRAW_LEFT
#define FRAME_DRAW_LEFT 0x01
#endif
#ifndef FRAME_DRAW_RIGHT
#define FRAME_DRAW_RIGHT 0x02
#endif
#ifndef FRAME_DRAW_TOP
#define FRAME_DRAW_TOP 0x04
#endif
#ifndef FRAME_DRAW_BOTTOM
#define FRAME_DRAW_BOTTOM 0x08
#endif
#ifndef FRAME_DRAW_ALL
#define FRAME_DRAW_ALL 0x0F
#endif

// 消息
// item选中项改变的事件，第一个参数为this指针，第二个参数地位字节为原来选择项，高位字节为现选择项
#define WM_MENUCTRL_SEL_CHANGE (WM_USER+421)		

class AFX_EXT_CLASS CMenuCtrl : public CStatic
{
	DECLARE_DYNAMIC(CMenuCtrl)

public:
	CMenuCtrl();
	virtual ~CMenuCtrl();
	// 设置边框样式
	void SetFrameModol(BYTE nModol);
	// 添加条目
	int AddItem(CString strItemText);
	// 设置字体
	void SetFont(CFont *pfont);
	// 设置颜色
	void SetColor(COLORREF clrBackgound, COLORREF clrTextNormal, COLORREF clrTextSel);
	void SetCurSel(int nSel);
	int GetCurSel();

protected:
	DECLARE_MESSAGE_MAP()

	BOOL OnEraseBkgnd(CDC* pDC);
	void OnPaint();	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();

private:
	// 条目信息
	struct STRU_ITEM
	{
		CString strText;	// 文本
		int nWidth;			// 文本宽度
		int nHeight;		// 文本高度

		CRect rcItem;
		CRect rcSelItem;
	};

private:
	CFont *m_pfont;
	COLORREF m_clrBackground;	// 背景色
	COLORREF m_clrTextNormal;	// 文本颜色
	COLORREF m_clrTextSel;		// 文本选中时的颜色
	BYTE m_nModol;				// 边框样式
	std::vector<STRU_ITEM> m_vecItem;

	int m_nCurSel;
	int m_nMoveSel;

	BOOL m_bTracking;
	TRACKMOUSEEVENT m_TMS; 
	int m_nPreSel;
};


