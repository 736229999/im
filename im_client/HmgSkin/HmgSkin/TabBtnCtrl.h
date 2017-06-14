#pragma once
#include <atlimage.h>

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
#define WM_TABBTNCTRL_SEL_CHANGE (WM_USER+523)		

// CTabBtnCtrl
#include <vector>

class AFX_EXT_CLASS CTabBtnCtrl : public CStatic
{
	DECLARE_DYNAMIC(CTabBtnCtrl)

public:
	CTabBtnCtrl();
	virtual ~CTabBtnCtrl();
	// 添加条目(加载bmp图片，png图片不支持)
	int AddItem(CString strItemText, CString strImgPath = "", BOOL bRraw = FALSE);
	// 设置颜色
	void SetBackgroundColor(COLORREF clrBackgroundNormal, COLORREF clrBackgroundSel, COLORREF clrBackgroundMouseMove)
	{
		m_clrBackgroundNormal = clrBackgroundNormal;
		m_clrBackgroundSel = clrBackgroundSel;
		m_clrBackgroundMouseMove = clrBackgroundMouseMove;
	}
	// 设置文本颜色
	void SetItemTextColor(COLORREF clrTextNormal, COLORREF clrTextSel, COLORREF clrTextMouseMove)
	{
		m_clrTextNormal = clrTextNormal;
		m_clrTextSel = clrTextSel;
		m_clrTextMouseMove = clrTextMouseMove;
	}

	// 删除所有条目
	void RemoveAllItems();

	void SetCurSel(int nSel);
	int GetCurSel();

	// 获取文本内容
	CString GetItemText(int nIndex);

	// 设置ITEM的高度，只在纵向排列时才有效
	void SetItemHeight(int nItemHeight);

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

	// 计算每个item的相对坐标
	void CalcCoord();

private:
	// 条目属性
	struct STRU_ITEMPRO
	{
		CString strText;			// 文本
		CRect rcText;				// 文本所在位置

		CString strImgPath;			// 图片路径
		CImage img;					// 位图
		CPoint ptImg;				// 位图绘制坐标

		CRect rcRange;				// 条目所在区域

		BOOL bHasUnderline;			// 是否有下划线
		CPoint ptBgn;				// 下划线起始位置
		CPoint ptEnd;				// 下划线结束位置

		STRU_ITEMPRO()
		{
			strText = "";
		}
	};

private:
	CFont m_font;
	int m_nItemHeight;				// ITEM的高度，只有纵向排列时才有效
	int m_nItemWidth;				// 条目宽度
	COLORREF m_clrBackgroundNormal;	// 背景色
	COLORREF m_clrBackgroundSel;	// 选中时的背景色
	COLORREF m_clrBackgroundMouseMove;	// 鼠标移动时item的颜色

	COLORREF m_clrTextNormal;		// 文本颜色
	COLORREF m_clrTextSel;			// 条目选中时文本的颜色
	COLORREF m_clrTextMouseMove;	// 鼠标移动到条目上文本的颜色	

	std::vector<STRU_ITEMPRO> m_vecItem;
	int m_nCurSel;
	int m_nMoveSel;

	BOOL m_bTracking;
	TRACKMOUSEEVENT m_TMS; 
	int m_nPreSel;
};


