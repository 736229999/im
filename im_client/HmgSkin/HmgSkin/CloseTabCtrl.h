#pragma once

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

// CCloseTabCtrl
#include <vector>

// 消息
// item选中项改变的事件，第一个参数为this指针，第二个参数地位字节为原来选择项，高位字节为现选择项
#define WM_CLOSETABCTRL_SEL_CHANGE (WM_USER+423)		
// 点了关闭按钮向父窗口发送WM_CLOSETABCTRL消息
#define WM_CLOSETABCTRL_CLOSE (WM_USER+424)

class AFX_EXT_CLASS CCloseTabCtrl : public CStatic
{
	DECLARE_DYNAMIC(CCloseTabCtrl)

public:
	CCloseTabCtrl();
	virtual ~CCloseTabCtrl();
	// 添加条目
	int AddItem(CString strItemText);
	// 设置颜色
	void SetBackgroundColor(COLORREF clrBackgound);
	void SetCurSel(int nSel);
	int GetCurSel();

	// 设置关联数据
	BOOL SetItemData(int nIndex, DWORD dwData);
	// 获取关联数据
	BOOL GetItemData(int nIndex, DWORD &dwData);

	// 设置边框样式
	void SetFrameModol(BYTE nModol)
	{
		m_nModol = nModol;
	}

protected:
	DECLARE_MESSAGE_MAP()

	BOOL OnEraseBkgnd(CDC* pDC);
	void OnPaint();	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void PreSubclassWindow();

	// 计算每个item的相对坐标
	void CalcCoord();

private:
	// 条目信息
	struct STRU_ITEM
	{
		CString strText;	// 文本
		int nWidth;			// 文本宽度
		int nHeight;		// 文本高度

		CRect rcItem;
		CRect rcCloseImg;	// 关闭按钮的位置	

		DWORD dwData;		// 关联数据	
	};

private:
	CFont m_font;
	COLORREF m_clrBackground;		// 背景色
	COLORREF m_clrTextNormal;		// 文本颜色
	std::vector<STRU_ITEM> m_vecItem;

	int m_nCurSel;

	BOOL m_bTracking;
	int m_nPreSel;
	BYTE m_nModol;				// 边框样式
};


