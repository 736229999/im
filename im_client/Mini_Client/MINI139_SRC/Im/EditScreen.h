/*====================================================================  
=  文件名:CEditScreen类                                    
=  修改日期:2015-5-5                                            
=  作者:wyx                  
=  作用:用于截屏工具文字编辑
====================================================================*/ 
#pragma once

// CEditScreen
#define WM_EDIT_PIC_SAVE WM_USER+1
#define WM_DRAW_RECTTRACKER WM_USER+2
class CEditScreen : public CEdit
{
	DECLARE_DYNAMIC(CEditScreen)

public:
	CEditScreen();
	virtual ~CEditScreen();

protected:
	//{{AFX_MSG(CEditTrans)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg	void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChange();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL m_bIsPaint;//判断是否需要绘制，文字到了右下角的边缘则无法继续绘制
	CBitmap* m_bitmapLast;//绘制的背景图片
	BOOL m_bIsDrawRectangle;//判断是否绘制虚线边框
	BOOL m_bIsSavePicture;//判断是否需要保存图片
public:
	void SetParentRect(CRect rcParent);//设置截屏区域

private:
	DWORD m_dwTextColor;//字体颜色
	CBrush m_brush;//透明画刷
	CRect m_rcParent;//父窗口截屏区域
	CRect m_rect;//边框区域，用于动态绘制虚线边框
};


