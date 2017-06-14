// ColumnCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "TabBtnCtrl.h"

using namespace std;

#define ITEM_DEF_WIDTH 60
#define ITEM_DEF_HEIGHT 34
// CTabBtnCtrl

IMPLEMENT_DYNAMIC(CTabBtnCtrl, CStatic)

CTabBtnCtrl::CTabBtnCtrl()
{
	// 默认字体
	m_font.CreateFont(
		18,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,					// nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("微软雅黑"));

	// 背景色
	m_clrBackgroundNormal = RGB(219,221,222);
	// item选中时的背景色
	m_clrBackgroundSel = RGB(240,240,240);
	m_clrBackgroundMouseMove = RGB(206, 211, 214);

	// 文本颜色
	m_clrTextNormal = RGB(70,70,70);
	m_clrTextSel = RGB(77,149,194);
	m_clrTextMouseMove = RGB(77,149,194);

	m_nItemHeight = 0;
	m_nItemWidth = 0;

	m_nCurSel = -1;

	m_bTracking = FALSE;
	m_nPreSel = -1;
}

CTabBtnCtrl::~CTabBtnCtrl()
{
}


BEGIN_MESSAGE_MAP(CTabBtnCtrl, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

// CTabBtnCtrl 消息处理程序
BOOL CTabBtnCtrl::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CTabBtnCtrl::OnPaint()
{
	CPaintDC dc(this);
	CRect rcCtrl;
	GetClientRect(&rcCtrl);

	CRect rc = rcCtrl;

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bmMem;
	bmMem.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height());

	int nSavedDC = dcMem.SaveDC();

	dcMem.SelectObject(&bmMem);

	// 选择画笔
	CPen pen(PS_SOLID, 2, m_clrTextSel);
	CPen *pOldPen = dcMem.SelectObject(&pen);
	
	dcMem.FillSolidRect(&rc, m_clrBackgroundNormal);

	// 绘制栏目
	CFont *pOldFont = dcMem.SelectObject(&m_font);
	for(size_t i = 0; i != m_vecItem.size(); ++i)
	{
		STRU_ITEMPRO &item = m_vecItem[i];
		UINT nFormat = DT_SINGLELINE | DT_VCENTER | DT_CENTER;

		if(i == m_nCurSel)
		{	// 为当前选中，重设背景
			// 绘制背景色
			CRect rcTemp = item.rcRange;
			rcTemp.left = item.ptBgn.x;
			rcTemp.right = item.ptEnd.x;
			rcTemp.top = item.rcRange.top + (item.rcRange.bottom-item.ptBgn.y);
			rcTemp.bottom = item.ptBgn.y;
			dcMem.FillSolidRect(&rcTemp, m_clrBackgroundSel);
			// 设置字体背景色
			dcMem.SetBkColor(m_clrBackgroundSel);	
			// 设置字体颜色
			dcMem.SetTextColor(m_clrTextSel);
			// 绘制位图
			if(!item.img.IsNull())
			{
				item.img.Draw(dcMem.m_hDC, item.ptImg); 
			}
		}
		else 
		{
			if(i == m_nMoveSel)
			{
				CRect rcTemp = item.rcRange;
				rcTemp.left = item.ptBgn.x;
				rcTemp.right = item.ptEnd.x;
				rcTemp.top = item.rcRange.top + (item.rcRange.bottom-item.ptBgn.y);
				rcTemp.bottom = item.ptBgn.y;
				// 绘制背景色
				dcMem.FillSolidRect(&rcTemp, m_clrBackgroundMouseMove);
				// 设置字体背景色
				dcMem.SetBkColor(m_clrBackgroundMouseMove);
				// 设置字体颜色
				dcMem.SetTextColor(m_clrTextMouseMove);
				// 绘制位图
				if(!item.img.IsNull())
				{
					item.img.Draw(dcMem.m_hDC, item.ptImg); 
				}
			}
			else
			{
				// 绘制背景色
				dcMem.FillSolidRect(&item.rcRange, m_clrBackgroundNormal);
				// 设置字体背景色
				dcMem.SetBkColor(m_clrBackgroundNormal);
				// 设置字体颜色
				dcMem.SetTextColor(m_clrTextNormal);
				// 绘制位图
				if(!item.img.IsNull())
				{
					item.img.Draw(dcMem.m_hDC, item.ptImg); 
				}
			}	
		}

		// 绘制文字
		dcMem.DrawText(item.strText, item.rcText, nFormat);
	}
	dcMem.SelectObject(pOldFont);
	dcMem.SelectObject(&pOldPen);
	dc.BitBlt(0,0,rc.Width(),rcCtrl.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.RestoreDC(nSavedDC);

	dc.SelectObject(pOldPen);
}

int CTabBtnCtrl::AddItem(CString strItemText, CString strImgPath, BOOL bRraw)
{
	STRU_ITEMPRO item;
	item.strText = strItemText;
	item.strImgPath = strImgPath;
	m_vecItem.push_back(item);
	
	if(bRraw && ::IsWindow(GetSafeHwnd()))
	{
		CalcCoord();
		Invalidate();
	}

	return m_vecItem.size()-1;
}

void CTabBtnCtrl::SetCurSel(int nSel)
{
	if (::IsWindow(GetSafeHwnd()) && m_nCurSel != nSel)
	{
		int nOldSel = m_nCurSel;
		m_nCurSel = nSel;
		Invalidate();		
		::SendMessage(GetParent()->GetSafeHwnd(), WM_TABBTNCTRL_SEL_CHANGE, (DWORD)this, MAKELONG(nOldSel, m_nCurSel));
	}
	else
	{
		m_nCurSel = nSel;
	}
}

int  CTabBtnCtrl::GetCurSel()
{
	return m_nCurSel;
}

void CTabBtnCtrl::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
}

void CTabBtnCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CStatic::OnMouseMove(nFlags, point);

	if(!m_bTracking)
	{
		m_TMS.hwndTrack = GetSafeHwnd();
		m_bTracking = TrackMouseEvent(&m_TMS);
	} 

	// 计算选中的item
	m_nMoveSel = -1;
	{
		if((int)point.x % m_nItemWidth > 0)
		{
			m_nMoveSel = (int)point.x / m_nItemWidth;
		}
	}

	if(m_nMoveSel != -1 && m_nPreSel != m_nMoveSel)
	{
		Invalidate();
		m_nPreSel = m_nMoveSel;
	}
}

void CTabBtnCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonDown(nFlags, point);
}

void CTabBtnCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonUp(nFlags, point);
	int nSel = -1;
	{
		if((int)point.x % m_nItemWidth > 0)
		{
			nSel = (int)point.x / m_nItemWidth;
		}
	}

	if(nSel != -1 && nSel != m_nCurSel && nSel < (int)m_vecItem.size())
	{
		int nOldSel = m_nCurSel;
		m_nCurSel = nSel;		
		Invalidate();		
		::SendMessage(GetParent()->GetSafeHwnd(), WM_TABBTNCTRL_SEL_CHANGE, (DWORD)this, MAKELONG(nOldSel, m_nCurSel));
	}
}

void CTabBtnCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyle(0,SS_NOTIFY);

	m_TMS.cbSize = sizeof(TRACKMOUSEEVENT);
	m_TMS.dwFlags = TME_HOVER | TME_LEAVE;
	m_TMS.hwndTrack = GetSafeHwnd();
	m_TMS.dwHoverTime = 10;
	TrackMouseEvent(&m_TMS); 

	CalcCoord();	
}

 LRESULT CTabBtnCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
 {
	 m_bTracking = FALSE;
	 if(m_nMoveSel != -1)
	 {
		 m_nPreSel = -1;
		 m_nMoveSel = -1;
		 Invalidate();
	 }

	 return 1;
 }

// 删除所有条目
void CTabBtnCtrl::RemoveAllItems()
{
	m_vecItem.clear();
	m_nCurSel = -1;
	m_nMoveSel = -1;
	m_nPreSel = -1;

	if(::IsWindow(GetSafeHwnd()))
	{
		CalcCoord();
		Invalidate();
	}
}

// 计算每个item的相对坐标
void CTabBtnCtrl::CalcCoord()
{
	if(!::IsWindow(GetSafeHwnd()))
		return;

	CRect rc;
	GetClientRect(&rc);

	{	// 横向排列
		m_nItemWidth = ITEM_DEF_WIDTH;
		m_nItemHeight = rc.Height();
	}

	CDC *pDC = GetDC();
	CFont *pOldFont = pDC->SelectObject(&m_font);

	for(size_t i = 0; i != m_vecItem.size(); ++i)
	{
		STRU_ITEMPRO &item = m_vecItem[i];

		{
			// 横向排列
			item.rcRange.top = rc.top;
			item.rcRange.bottom = rc.bottom;
			item.rcRange.left = rc.left + i*m_nItemWidth;
			item.rcRange.right = item.rcRange.left + m_nItemWidth;
		}

		// 加载图片
		if(!item.strImgPath.IsEmpty())
		{
			item.img.Load(item.strImgPath);
		}
		// 计算图片宽度
		int nImgWidth = 0;
		int nSpaceWidth = 0;		// 图片与文本间的空隙宽度
		if(!item.img.IsNull())
		{
			nImgWidth = item.img.GetWidth();
			nSpaceWidth = 5;		// 当有位图时，设置空隙宽度
		}

		// 计算文本宽度
		CSize size = pDC->GetTextExtent(item.strText, item.strText.GetLength());
		int nTextWidth = size.cx;

		int nTotalWidth = nImgWidth + nSpaceWidth + nTextWidth;		// 总宽度

		// 计算条目内图片以及文本坐标
		{	// 横向排列，图片文本都居中排列
			if(!item.img.IsNull())
			{	// 有位图，得计算位图坐标
				item.ptImg.x = item.rcRange.left + (item.rcRange.Width()-nTotalWidth)/2;
				item.ptImg.y = item.rcRange.top + (item.rcRange.Height()-item.img.GetHeight())/2;

				// 计算文本位置
				item.rcText.top = item.rcRange.top + (item.rcRange.Height()-size.cy)/2;
				item.rcText.bottom = item.rcText.top + size.cy + 1;
				item.rcText.left = item.ptImg.x + nImgWidth + nSpaceWidth;
				item.rcText.right = item.rcText.left + nTextWidth + 1;
			}
			else
			{	// 无位图，只计算文本位置
				item.rcText.top = item.rcRange.top + (item.rcRange.Height()-size.cy)/2;
				item.rcText.bottom = item.rcText.top + size.cy + 1;
				item.rcText.left = item.rcRange.left + (item.rcRange.Width()-nTotalWidth)/2;
				item.rcText.right = item.rcText.left + nTotalWidth + 1;
			}
		}

		// 计算下划线位置
		{
			item.bHasUnderline = FALSE;
			{	// 无位图，下划线在文本的下方
				item.ptBgn.x = item.rcText.left - 5;
				item.ptBgn.y = item.rcText.bottom;

				item.ptEnd.x = item.ptBgn.x + nTotalWidth + 10;
				item.ptEnd.y = item.ptBgn.y;
			}
		}
	}

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);
}

// 获取文本内容
CString CTabBtnCtrl::GetItemText(int i)
{
	if(i >= 0 && i < (int)m_vecItem.size())
	{
		return m_vecItem[i].strText;
	}

	return "";
}

void CTabBtnCtrl::SetItemHeight(int nItemHeight)
{
	m_nItemHeight = nItemHeight;

	if(::IsWindow(GetSafeHwnd()))
	{
		CalcCoord();
		Invalidate();
	}
}