// ColumnCtrl.cpp : ʵ���ļ�
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
	// Ĭ������
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
		_T("΢���ź�"));

	// ����ɫ
	m_clrBackgroundNormal = RGB(219,221,222);
	// itemѡ��ʱ�ı���ɫ
	m_clrBackgroundSel = RGB(240,240,240);
	m_clrBackgroundMouseMove = RGB(206, 211, 214);

	// �ı���ɫ
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

// CTabBtnCtrl ��Ϣ�������
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

	// ѡ�񻭱�
	CPen pen(PS_SOLID, 2, m_clrTextSel);
	CPen *pOldPen = dcMem.SelectObject(&pen);
	
	dcMem.FillSolidRect(&rc, m_clrBackgroundNormal);

	// ������Ŀ
	CFont *pOldFont = dcMem.SelectObject(&m_font);
	for(size_t i = 0; i != m_vecItem.size(); ++i)
	{
		STRU_ITEMPRO &item = m_vecItem[i];
		UINT nFormat = DT_SINGLELINE | DT_VCENTER | DT_CENTER;

		if(i == m_nCurSel)
		{	// Ϊ��ǰѡ�У����豳��
			// ���Ʊ���ɫ
			CRect rcTemp = item.rcRange;
			rcTemp.left = item.ptBgn.x;
			rcTemp.right = item.ptEnd.x;
			rcTemp.top = item.rcRange.top + (item.rcRange.bottom-item.ptBgn.y);
			rcTemp.bottom = item.ptBgn.y;
			dcMem.FillSolidRect(&rcTemp, m_clrBackgroundSel);
			// �������屳��ɫ
			dcMem.SetBkColor(m_clrBackgroundSel);	
			// ����������ɫ
			dcMem.SetTextColor(m_clrTextSel);
			// ����λͼ
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
				// ���Ʊ���ɫ
				dcMem.FillSolidRect(&rcTemp, m_clrBackgroundMouseMove);
				// �������屳��ɫ
				dcMem.SetBkColor(m_clrBackgroundMouseMove);
				// ����������ɫ
				dcMem.SetTextColor(m_clrTextMouseMove);
				// ����λͼ
				if(!item.img.IsNull())
				{
					item.img.Draw(dcMem.m_hDC, item.ptImg); 
				}
			}
			else
			{
				// ���Ʊ���ɫ
				dcMem.FillSolidRect(&item.rcRange, m_clrBackgroundNormal);
				// �������屳��ɫ
				dcMem.SetBkColor(m_clrBackgroundNormal);
				// ����������ɫ
				dcMem.SetTextColor(m_clrTextNormal);
				// ����λͼ
				if(!item.img.IsNull())
				{
					item.img.Draw(dcMem.m_hDC, item.ptImg); 
				}
			}	
		}

		// ��������
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

	// ����ѡ�е�item
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

// ɾ��������Ŀ
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

// ����ÿ��item���������
void CTabBtnCtrl::CalcCoord()
{
	if(!::IsWindow(GetSafeHwnd()))
		return;

	CRect rc;
	GetClientRect(&rc);

	{	// ��������
		m_nItemWidth = ITEM_DEF_WIDTH;
		m_nItemHeight = rc.Height();
	}

	CDC *pDC = GetDC();
	CFont *pOldFont = pDC->SelectObject(&m_font);

	for(size_t i = 0; i != m_vecItem.size(); ++i)
	{
		STRU_ITEMPRO &item = m_vecItem[i];

		{
			// ��������
			item.rcRange.top = rc.top;
			item.rcRange.bottom = rc.bottom;
			item.rcRange.left = rc.left + i*m_nItemWidth;
			item.rcRange.right = item.rcRange.left + m_nItemWidth;
		}

		// ����ͼƬ
		if(!item.strImgPath.IsEmpty())
		{
			item.img.Load(item.strImgPath);
		}
		// ����ͼƬ���
		int nImgWidth = 0;
		int nSpaceWidth = 0;		// ͼƬ���ı���Ŀ�϶���
		if(!item.img.IsNull())
		{
			nImgWidth = item.img.GetWidth();
			nSpaceWidth = 5;		// ����λͼʱ�����ÿ�϶���
		}

		// �����ı����
		CSize size = pDC->GetTextExtent(item.strText, item.strText.GetLength());
		int nTextWidth = size.cx;

		int nTotalWidth = nImgWidth + nSpaceWidth + nTextWidth;		// �ܿ��

		// ������Ŀ��ͼƬ�Լ��ı�����
		{	// �������У�ͼƬ�ı�����������
			if(!item.img.IsNull())
			{	// ��λͼ���ü���λͼ����
				item.ptImg.x = item.rcRange.left + (item.rcRange.Width()-nTotalWidth)/2;
				item.ptImg.y = item.rcRange.top + (item.rcRange.Height()-item.img.GetHeight())/2;

				// �����ı�λ��
				item.rcText.top = item.rcRange.top + (item.rcRange.Height()-size.cy)/2;
				item.rcText.bottom = item.rcText.top + size.cy + 1;
				item.rcText.left = item.ptImg.x + nImgWidth + nSpaceWidth;
				item.rcText.right = item.rcText.left + nTextWidth + 1;
			}
			else
			{	// ��λͼ��ֻ�����ı�λ��
				item.rcText.top = item.rcRange.top + (item.rcRange.Height()-size.cy)/2;
				item.rcText.bottom = item.rcText.top + size.cy + 1;
				item.rcText.left = item.rcRange.left + (item.rcRange.Width()-nTotalWidth)/2;
				item.rcText.right = item.rcText.left + nTotalWidth + 1;
			}
		}

		// �����»���λ��
		{
			item.bHasUnderline = FALSE;
			{	// ��λͼ���»������ı����·�
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

// ��ȡ�ı�����
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