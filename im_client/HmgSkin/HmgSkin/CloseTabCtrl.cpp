// MenuCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CloseTabCtrl.h"
#include "FlatPopupMenu.h"

using namespace std;

IMPLEMENT_DYNAMIC(CCloseTabCtrl, CStatic)

CCloseTabCtrl::CCloseTabCtrl()
{
	// Ĭ������
	m_font.CreateFont(
		18,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("΢���ź�"));

	m_clrBackground = RGB(238, 238, 238);		// ����ɫ
	m_clrTextNormal = 0x4f4f4f;				// �ı���ɫ
	
	m_nCurSel = -1;
	m_bTracking = FALSE;
	m_nPreSel = -1;
}

CCloseTabCtrl::~CCloseTabCtrl()
{
}


BEGIN_MESSAGE_MAP(CCloseTabCtrl, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

// CColumnCtrl ��Ϣ�������
BOOL CCloseTabCtrl::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CCloseTabCtrl::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bmMem;
	bmMem.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height());

	int nSavedDC = dcMem.SaveDC();

	dcMem.SelectObject(&bmMem);
	
	dcMem.FillSolidRect(&rc, m_clrBackground);		// ���Ʊ���	
	dcMem.SetBkColor(m_clrBackground);				// �������屳��ɫ

	// ���Ʊ߽�
	CPen pen(PS_SOLID, 1, RGB(215, 215, 215));
	CPen *pOldPen = dcMem.SelectObject(&pen);
	CPoint ptLT(rc.left, rc.top+1);
	CPoint ptLB(rc.left, rc.bottom-1);
	CPoint ptRT(rc.right, rc.top+1);
	CPoint ptRB(rc.right, rc.bottom-1);
	if(m_nModol & FRAME_DRAW_LEFT)
	{
		dcMem.MoveTo(ptLT);
		dcMem.LineTo(ptLB);
	}
	if(m_nModol & FRAME_DRAW_RIGHT)
	{
		dcMem.MoveTo(ptRT);
		dcMem.LineTo(ptRB);
	}
	if(m_nModol & FRAME_DRAW_TOP)
	{
		dcMem.MoveTo(ptLT);
		dcMem.LineTo(ptRT);
	}
	if(m_nModol & FRAME_DRAW_BOTTOM)
	{
		dcMem.MoveTo(ptLB);
		dcMem.LineTo(ptRB);
	}
	dcMem.SelectObject(pOldPen);

	// ������Ŀ
	CFont *pOldFont = dcMem.SelectObject(&m_font);
	int nLeft = 0;
	for(size_t i = 0; i != m_vecItem.size(); ++i)
	{
		STRU_ITEM &item = m_vecItem[i];
		UINT nFormat= DT_SINGLELINE | DT_VCENTER | DT_CENTER;

		// �޶���ͼ����
		CPen pen(PS_SOLID, 1, RGB(215, 215, 215));
		CPen *pOldPen = dcMem.SelectObject(&pen);

		CPoint ptLT(item.rcItem.left, item.rcItem.top);
		CPoint ptLB(item.rcItem.left, item.rcItem.bottom-1);
		CPoint ptRT(item.rcItem.right, item.rcItem.top);
		CPoint ptRB(item.rcItem.right, item.rcItem.bottom-1);
		
		if(i != 0)
		{
			dcMem.MoveTo(ptLT);
			dcMem.LineTo(ptLB);
		}
		dcMem.MoveTo(ptRT);
		dcMem.LineTo(ptRB);

		dcMem.SelectObject(pOldPen);

		if(i == m_nCurSel)
		{	// Ϊ��ǰѡ��
			CPen pen(PS_SOLID, 1, m_clrBackground);
			CPen *pOldPen = dcMem.SelectObject(&pen);
			dcMem.MoveTo(ptLB);
			dcMem.LineTo(ptRB);
			dcMem.SelectObject(pOldPen);
		}

		// ��������
		dcMem.SetTextColor(m_clrTextNormal);	
		dcMem.DrawText(item.strText, item.rcItem, nFormat);

		// ���ƹرհ�ť
		CImage imgClose;
		HMODULE hModule = GetModuleHandle("HmgSkinRes.dll"); 
		CPoint pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);
		if(item.rcCloseImg.PtInRect(pt))
			LoadImageFromResource(&imgClose, hModule, IDB_PNG_CLOSE_HOVER, _T("PNG"));
		else
			LoadImageFromResource(&imgClose, hModule, IDB_PNG_CLOSE_NORMAL, _T("PNG"));
		imgClose.Draw(dcMem.m_hDC, item.rcCloseImg);
	}
	dcMem.SelectObject(pOldFont);

	dc.BitBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.RestoreDC(nSavedDC);
}

int CCloseTabCtrl::AddItem(CString strItemText)
{
	STRU_ITEM item;
	item.strText = strItemText + "   ";
	item.nHeight = 0;
	item.nWidth = 0;
	item.dwData = 0;
	m_vecItem.push_back(item);

	if(::IsWindow(GetSafeHwnd()))
	{
		CalcCoord();
		Invalidate();
	}
	
	return m_vecItem.size()-1;
}

void CCloseTabCtrl::SetCurSel(int nSel)
{
	if (::IsWindow(GetSafeHwnd()) && m_nCurSel != nSel)
	{
		int nOldSel = m_nCurSel;
		m_nCurSel = nSel;
		Invalidate();
		::SendMessage(GetParent()->GetSafeHwnd(), WM_CLOSETABCTRL_SEL_CHANGE, (DWORD)this, MAKELONG(nOldSel, m_nCurSel));
	}
	else
	{
		m_nCurSel = nSel;
	}
}

int  CCloseTabCtrl::GetCurSel()
{
	return m_nCurSel;
}

void CCloseTabCtrl::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
	
}

void CCloseTabCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CStatic::OnMouseMove(nFlags, point);
	Invalidate();
}

void CCloseTabCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonDown(nFlags, point);
}

void CCloseTabCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonUp(nFlags, point);

	// ���ж��Ƿ���˹رհ�ť
	for(int i = 0; i < m_vecItem.size(); ++i)
	{
		STRU_ITEM &item = m_vecItem[i];
		if(item.rcCloseImg.PtInRect(point))
		{	// ���˹رհ�ť
			//�رհ�ť����Ҫ��tab�������򸸴��ڷ���WM_CLOSETABCTRL_SEL_CHANGE��WM_CLOSETABCTRL_CLOSE��Ϣ
// 			if(m_nCurSel == m_vecItem.size()-1)
// 			{
// 				m_nCurSel--; 
// 			}
// 			m_vecItem.erase(m_vecItem.begin() + i);
// 			CalcCoord();
// 			Invalidate();
			if(GetParent() != NULL)
			{
 				::SendMessage(GetParent()->GetSafeHwnd(), WM_CLOSETABCTRL_SEL_CHANGE, (DWORD)this, MAKELONG(-1, m_nCurSel));
				::SendMessage(GetParent()->GetSafeHwnd(), WM_CLOSETABCTRL_CLOSE, NULL, NULL);
			}
			return;
		}
	}

	// ���ж�ѡ����
	int nSel = -1;
	for(int i = 0; i < m_vecItem.size(); ++i)
	{
		STRU_ITEM &item = m_vecItem[i];
		if(item.rcItem.PtInRect(point))
		{
			nSel = i;
			break;
		}
	}

	if(nSel != -1 && nSel != m_nCurSel && nSel < m_vecItem.size())
	{
		int nOldSel = m_nCurSel;
		m_nCurSel = nSel;
		Invalidate();
		::SendMessage(GetParent()->GetSafeHwnd(), WM_CLOSETABCTRL_SEL_CHANGE, (DWORD)this, MAKELONG(nOldSel, m_nCurSel));
	}
}

// ������ɫ
void CCloseTabCtrl::SetBackgroundColor(COLORREF clrBackgound)
{
	m_clrBackground = clrBackgound;		// ����ɫ
}

void CCloseTabCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyle(0,SS_NOTIFY);

	CalcCoord();
}

// ���ù�������
BOOL CCloseTabCtrl::SetItemData(int nIndex, DWORD dwData)
{
	if(nIndex < 0 || nIndex >= m_vecItem.size())
	{
		return FALSE;
	}

	m_vecItem[nIndex].dwData = dwData;

	return TRUE;
}

// ��ȡ��������
BOOL CCloseTabCtrl::GetItemData(int nIndex, DWORD &dwData)
{
	if(nIndex < 0 || nIndex >= m_vecItem.size())
	{
		return FALSE;
	}

	dwData = m_vecItem[nIndex].dwData;

	return TRUE;
}

// ����ÿ��item���������
void CCloseTabCtrl::CalcCoord()
{
	if(!::IsWindow(GetSafeHwnd()))
		return;

	CRect rc;
	GetClientRect(&rc);

	CDC *pDC = GetDC();

	// ���������ֵĿ��
	CSize sizeTwoWord;
	sizeTwoWord = pDC->GetTextExtent("����");

	CFont *pFont = pDC->SelectObject(&m_font);
	int nLeft = 0;
	for(size_t i = 0; i != m_vecItem.size(); ++i)
	{
		STRU_ITEM &item = m_vecItem[i];

		// �����ı����
		CSize size;
		size = pDC->GetTextExtent(item.strText, item.strText.GetLength());
		item.nWidth = size.cx;
		if(size.cx < sizeTwoWord.cx)
		{
			item.nWidth = sizeTwoWord.cx;
		}
		item.nHeight = size.cy;

		// ����itemλ��
		int nItemWidth = item.nWidth*1.8;
		item.rcItem.left = nLeft;
		item.rcItem.right = item.rcItem.left + nItemWidth;
		item.rcItem.top = rc.top;
		item.rcItem.bottom = rc.bottom;
		nLeft += nItemWidth;

		// ����itemѡ��ʱ�߿��λ��
		item.rcCloseImg.right = item.rcItem.right - 0.2*item.nWidth;
		item.rcCloseImg.left = item.rcCloseImg.right - 18;
		item.rcCloseImg.top = rc.top + (rc.Height()-18)/2;
		item.rcCloseImg.bottom = item.rcCloseImg.top + 18;
	}
	pDC->SelectObject(pFont);
	ReleaseDC(pDC);
}