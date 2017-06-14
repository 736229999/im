// EditScreen.cpp : 实现文件
//

#include "stdafx.h"
#include "EditScreen.h"

#define TRANS_BACK -1
// CEditScreen

IMPLEMENT_DYNAMIC(CEditScreen, CEdit)

CEditScreen::CEditScreen()
{
 	m_dwTextColor = RGB(255, 0, 0);
	m_brush.CreateStockObject(NULL_BRUSH);
	m_bIsDrawRectangle = TRUE;
	m_rect.left = 0;
	m_rect.right = 0;
	m_rect.top = 0;
	m_rect.bottom = 0;
	m_bIsPaint = TRUE;
	m_bIsSavePicture = TRUE;
}

CEditScreen::~CEditScreen()
{
	m_brush.DeleteObject();
}


BEGIN_MESSAGE_MAP(CEditScreen, CEdit)
	ON_WM_CTLCOLOR_REFLECT()
	ON_CONTROL_REFLECT(EN_CHANGE, OnChange)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_CHAR()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CEditScreen 消息处理程序

HBRUSH CEditScreen::CtlColor( CDC* pDC, UINT nCtlColor )
{
	//设置字体颜色和背景透明
	pDC->SetTextColor(m_dwTextColor);
	pDC->SetBkMode(TRANSPARENT);
	return (HBRUSH)m_brush;

}

void CEditScreen::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//如果不绘制则响应父窗口的点击消息
	CPoint ptCursor;
	GetCursorPos(&ptCursor);

	if(!m_bIsDrawRectangle)
	{
		if(GetParent() != NULL)
			GetParent()->SendMessage(WM_LBUTTONDOWN, HTCAPTION, MAKELPARAM(ptCursor.x, ptCursor.y));
		return;
	}

	if(m_rect.PtInRect(point))//如果在虚线框区域内则响应编辑框的点击消息
	{
		CEdit::OnLButtonDown(nFlags, point);
	}
	else//如果在虚线框区域外则响应父窗口的点击消息
	{
		if(GetParent() != NULL)
			GetParent()->SendMessage(WM_LBUTTONDOWN, HTCAPTION, MAKELPARAM(ptCursor.x, ptCursor.y));
	}
}

BOOL CEditScreen::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return FALSE;
}

void CEditScreen::OnPaint() 
{
	if(m_bIsPaint)//文字编辑
	{
		CDC* pDC = GetDC();

		//绘制背景图片
		CRect rc;
		GetWindowRect(&rc);

		CDC memDcLast;
		memDcLast.CreateCompatibleDC(pDC);
		memDcLast.SelectObject(m_bitmapLast);
		pDC->BitBlt(0, 0, rc.Width(), rc.Height(), &memDcLast, rc.left, rc.top, SRCCOPY);
		memDcLast.DeleteDC();

		
		CDC dc;//用于计算字体大小
		dc.CreateCompatibleDC(pDC);
		dc.SelectObject(g_font14);
		SetFont(&g_font14);//设置字体
		CString str;
		GetWindowText(str);
		CSize sizeText;
		if(str == "")//如果输入为空则获取一个字符的大小
		{
			sizeText = dc.GetTextExtent("a");
		}
		else
		{
			sizeText = dc.GetTextExtent(str);
		}
		dc.DeleteDC();
		pDC->SelectObject(GetStockObject(NULL_BRUSH)); 
		pDC->SelectObject(CreatePen(PS_DASHDOT,1,RGB(0,0,0))); 
		CSize size; 
		int lenStrLen;  
		lenStrLen=GetWindowTextLength(); 
		GetTextExtentPoint(::GetDC(m_hWnd),str,lenStrLen,&size); 

		int nWidthMax = (sizeText.cx/*size.cx*/ + rc.left + 5) >= m_rcParent.right ? m_rcParent.right - rc.left : sizeText.cx/*size.cx*/ + 5;
		int nHeightMax = sizeText.cy * GetLineCount() + rc.top >= m_rcParent.bottom ?  m_rcParent.bottom - rc.top : sizeText.cy * GetLineCount();
		pDC->Rectangle(0, 0, nWidthMax, nHeightMax); //绘制虚线边框
		//虚线边框动态变化
		m_rect.left = rc.left;
		m_rect.top = rc.top;
		m_rect.right = rc.left + nWidthMax;
		m_rect.bottom = rc.top + sizeText.cy * GetLineCount();
		m_bIsPaint = FALSE;
		ShowCaret();//显示鼠标
		if(GetParent() != NULL)//绘制截屏边框
			::PostMessage(GetParent()->GetSafeHwnd(),WM_DRAW_RECTTRACKER, 0, 0);
		CEdit::OnPaint();
	}
	if(!m_bIsDrawRectangle)//完成编辑,不绘制虚线边框
	{
		CRect rc;
		GetWindowRect(&rc);

		CDC* pDC = GetDC();
		CDC memDcLast;
		memDcLast.CreateCompatibleDC(pDC);
		memDcLast.SelectObject(m_bitmapLast);
		pDC->BitBlt(m_rcParent.left - rc.left, m_rcParent.top - rc.top, m_rcParent.Width(), m_rcParent.Height(), &memDcLast, m_rcParent.left, m_rcParent.top, SRCCOPY);
		memDcLast.DeleteDC();

		CEdit::OnPaint();
		HideCaret();//隐藏鼠标
		if(GetParent() != NULL)
			::PostMessage(GetParent()->GetSafeHwnd(),WM_EDIT_PIC_SAVE, 0, 0);//父窗口保存图片
	}
} 

////////////////////////////////////////////////////////////////////////// 
//暂不处理粘滞按键和功能键这2种情况。 
////////////////////////////////////////////////////////////////////////// 
void CEditScreen::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{ 
	if(m_bIsDrawRectangle)//编辑文字
	{
		m_bIsPaint = TRUE;
		CEdit::OnChar(nChar, nRepCnt, nFlags); 
		Invalidate();
	}
}

void CEditScreen::OnMouseMove(UINT nFlags, CPoint point)
{
}

void CEditScreen::OnChange() 
{
}

void CEditScreen::SetParentRect(CRect rcParent)
{
	m_rcParent = rcParent;
}