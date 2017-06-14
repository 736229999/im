// MenuCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MenuCtrl.h"

using namespace std;

IMPLEMENT_DYNAMIC(CMenuCtrl, CStatic)

CMenuCtrl::CMenuCtrl()
{
	// Ĭ������
	static CFont stafont;
	stafont.CreateFont(
		18,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("΢���ź�"));
	m_pfont = &stafont;

	m_clrBackground = RGB(238, 238, 238);		// ����ɫ
	m_clrTextNormal = RGB(0, 0, 0);				// �ı���ɫ
	m_clrTextSel = RGB(0, 109, 176);			// �ı�ѡ��ʱ����ɫ
	
	m_nModol = FRAME_DRAW_ALL;

	m_nCurSel = -1;
	m_bTracking = FALSE;
	m_nPreSel = -1;
}

CMenuCtrl::~CMenuCtrl()
{
}


BEGIN_MESSAGE_MAP(CMenuCtrl, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
END_MESSAGE_MAP()

// ���ñ߿���ʽ
void CMenuCtrl::SetFrameModol(BYTE nModol)
{
	m_nModol = nModol;
}

// CColumnCtrl ��Ϣ�������
BOOL CMenuCtrl::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CMenuCtrl::OnPaint()
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

	// ������Ŀ
	CFont *pOldFont = dcMem.SelectObject(m_pfont);
	int nLeft = 0;
	for(size_t i = 0; i != m_vecItem.size(); ++i)
	{
		STRU_ITEM &item = m_vecItem[i];
		UINT nFormat= DT_SINGLELINE | DT_VCENTER | DT_CENTER;

		// �޶���ͼ����
		dcMem.Rectangle(item.rcItem);	
		dcMem.FillSolidRect(&item.rcItem, m_clrBackground);		// ���Ʊ���	

		if(i == m_nCurSel)
		{	// Ϊ��ǰѡ��
			dcMem.SetTextColor(m_clrTextSel);		// �����ı���ɫ		
		}
		else 
		{
			dcMem.SetTextColor(m_clrTextNormal);	
		}

		if(i == m_nMoveSel)
		{	// ����ƶ�����item
			// �����ı������
			CPen pen(PS_SOLID, 1, RGB(157, 159, 160));
			CPen *pOldPen = dcMem.SelectObject(&pen);
			CPoint ptLT(item.rcSelItem.left, item.rcSelItem.top);
			CPoint ptLB(item.rcSelItem.left, item.rcSelItem.bottom);
			CPoint ptRT(item.rcSelItem.right, item.rcSelItem.top);
			CPoint ptRB(item.rcSelItem.right, item.rcSelItem.bottom);
			dcMem.MoveTo(ptLT);
			dcMem.LineTo(ptLB);
			dcMem.LineTo(ptRB);
			dcMem.LineTo(ptRT);
			dcMem.LineTo(ptLT);
			dcMem.SelectObject(pOldPen);
		}

		// ��������
		dcMem.DrawText(item.strText, item.rcItem, nFormat);
	}
	dcMem.SelectObject(pOldFont);

	// ���Ʊ߽�
	CPen pen(PS_SOLID, 1, RGB(215, 215, 215));
	CPen *pOldPen = dcMem.SelectObject(&pen);
	CPoint ptLT(rc.left+1, rc.top+1);
	CPoint ptLB(rc.left+1, rc.bottom-1);
	CPoint ptRT(rc.right-1, rc.top+1);
	CPoint ptRB(rc.right-1, rc.bottom-1);
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

	dc.BitBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.RestoreDC(nSavedDC);
}

int CMenuCtrl::AddItem(CString strItemText)
{
	STRU_ITEM item;
	item.strText = strItemText;
	item.nHeight = 0;
	item.nWidth = 0;
	m_vecItem.push_back(item);
	
	return m_vecItem.size()-1;
}

void CMenuCtrl::SetCurSel(int nSel)
{
	if (::IsWindow(GetSafeHwnd()) && m_nCurSel != nSel)
	{
		int nOldSel = m_nCurSel;
		m_nCurSel = nSel;
		Invalidate();
		::SendMessage(GetParent()->GetSafeHwnd(), WM_MENUCTRL_SEL_CHANGE, (DWORD)this, MAKELONG(nOldSel, m_nCurSel));
	}
	else
	{
		m_nCurSel = nSel;
	}
}

int  CMenuCtrl::GetCurSel()
{
	return m_nCurSel;
}

void CMenuCtrl::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
	
}

void CMenuCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CStatic::OnMouseMove(nFlags, point);

	if(!m_bTracking)
	{
		m_TMS.hwndTrack = GetSafeHwnd();
		m_bTracking = TrackMouseEvent(&m_TMS);
	} 

	// ����ѡ�е�item
	m_nMoveSel = -1;
	for(int i = 0; i < m_vecItem.size(); ++i)
	{
		STRU_ITEM &item = m_vecItem[i];
		if(item.rcItem.PtInRect(point))
		{
			m_nMoveSel = i;
			break;
		}
	}

	if(m_nMoveSel != -1 && m_nPreSel != m_nMoveSel)
	{
		Invalidate();
		m_nPreSel = m_nMoveSel;
	}
}

void CMenuCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonDown(nFlags, point);
}

void CMenuCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonUp(nFlags, point);
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
		::SendMessage(GetParent()->GetSafeHwnd(), WM_MENUCTRL_SEL_CHANGE, (DWORD)this, MAKELONG(nOldSel, m_nCurSel));
	}
}

// ��������
void CMenuCtrl::SetFont(CFont *pfont)
{
	m_pfont = pfont;
}

// ������ɫ
void CMenuCtrl::SetColor(COLORREF clrBackgound, COLORREF clrTextNormal, COLORREF clrTextSel)
{
	m_clrBackground = clrBackgound;		// ����ɫ
	m_clrTextNormal = clrTextNormal;	// �ı���ɫ
	m_clrTextSel = clrTextSel;			// �ı�ѡ��ʱ����ɫ
}

void CMenuCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyle(0,SS_NOTIFY);
	m_TMS.cbSize = sizeof(TRACKMOUSEEVENT);
	m_TMS.dwFlags = TME_HOVER | TME_LEAVE;
	m_TMS.hwndTrack = GetSafeHwnd();
	m_TMS.dwHoverTime = 10;
	TrackMouseEvent(&m_TMS); 

	CRect rc;
	GetClientRect(&rc);

	CDC *pDC = GetDC();

	// ���������ֵĿ��
	CSize sizeTwoWord;
	sizeTwoWord = pDC->GetTextExtent("����");

	CFont *pFont = pDC->SelectObject(m_pfont);
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
		item.rcSelItem.left = item.rcItem.left + 0.2*item.nWidth;
		item.rcSelItem.right = item.rcItem.right - 0.2*item.nWidth;
		item.rcSelItem.top = rc.top + (rc.Height()/2-item.nHeight*0.8);
		item.rcSelItem.bottom = rc.bottom - (rc.Height()/2-item.nHeight*0.8);
	}
	pDC->SelectObject(pFont);
	ReleaseDC(pDC);
}

 LRESULT CMenuCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
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
