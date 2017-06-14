// ColumnCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ColumnCtrl.h"

using namespace std;

#define ITEM_DEF_WIDTH 90
#define ITEM_DEF_HEIGHT 34
// CColumnCtrl

IMPLEMENT_DYNAMIC(CColumnCtrl, CStatic)

static CFont stafont;
static BOOL stabCreate = FALSE;


CColumnCtrl::CColumnCtrl()
{
	// Ĭ������
	if(!stabCreate)
	{
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

		stabCreate = TRUE;
	}

	m_pfont = &stafont;

	// Ĭ�Ϻ���
	m_enDire = ENDIRE::EN_X_DIRE;		
	// ����ɫ
	m_clrBackgroundNormal = RGB(219,221,222);
	// itemѡ��ʱ�ı���ɫ
	m_clrBackgroundSel = RGB(240,240,240);
	m_clrBackgroundMouseMove = RGB(206, 211, 214);

	// �ı���ɫ
	/*m_clrTextNormal = RGB(0,0,0);
	m_clrTextSel = RGB(0,0,0);
	m_clrTextMouseMove = RGB(0,0,0);*/
	m_clrTextNormal = RGB(70,70,70);
	m_clrTextSel = RGB(77,149,194);
	m_clrTextMouseMove = RGB(77,149,194);

	m_nItemHeight = 0;
	m_nItemWidth = 0;

	m_nCurSel = -1;

	m_bTracking = FALSE;
	m_nPreSel = -1;

	m_bDrawUnderline = FALSE;

	m_nModol = FRAME_DRAW_NULL;

	m_dPrecent = 0;
	m_nScrollHeight = 0;
}

CColumnCtrl::~CColumnCtrl()
{
}


BEGIN_MESSAGE_MAP(CColumnCtrl, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_SCROLLBAR_MSG, OnScrollBarMsg)
END_MESSAGE_MAP()

// CColumnCtrl ��Ϣ�������
BOOL CColumnCtrl::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

// �������з�ʽ
void CColumnCtrl::SetDirectMode(ENDIRE enDireMode)
{
	m_enDire = enDireMode;
}

void CColumnCtrl::OnPaint()
{
	CPaintDC dc(this);
	CRect rcCtrl;
	GetClientRect(&rcCtrl);

	CRect rc = rcCtrl;

	if(m_enDire == EN_Y_DIRE && m_nItemHeight * m_vecItem.size() > rc.Height())
	{	// ���ڹ����������Ʒ�Χ�б䶯
		rc.bottom = rc.top + m_nItemHeight * m_vecItem.size();
		rcCtrl.right -= 10;
		rc.right -= 10;
	}

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
	CFont *pOldFont = dcMem.SelectObject(m_pfont);
	for(size_t i = 0; i != m_vecItem.size(); ++i)
	{
		STRU_ITEMPRO &item = m_vecItem[i];
		UINT nFormat = DT_SINGLELINE | DT_VCENTER | DT_CENTER;

		//// �޶���ͼ����
		//dcMem.Rectangle(item.rcRange);		

		if(i == m_nCurSel)
		{	// Ϊ��ǰѡ�У����豳��
			// ���Ʊ���ɫ
			dcMem.FillSolidRect(&item.rcRange, m_clrBackgroundSel);
			// �����»���
			if(m_bDrawUnderline)
			{
				// ���Ʊ���ɫ
				CRect rcUnderLine;
				rcUnderLine.left = item.ptBgn.x;
				rcUnderLine.right = item.ptEnd.x;
				rcUnderLine.top = item.ptBgn.y;
				rcUnderLine.bottom = item.ptBgn.y+2;

				dcMem.FillSolidRect(&rcUnderLine, m_clrTextSel);
			}
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
				// ���Ʊ���ɫ
				dcMem.FillSolidRect(&item.rcRange, m_clrBackgroundMouseMove);
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
	m_nScrollHeight = (m_dPrecent/100.0)*(rc.Height()-rcCtrl.Height());
	dc.BitBlt(0,0,rc.Width(),rcCtrl.Height(),&dcMem,0,m_nScrollHeight,SRCCOPY);
	dcMem.RestoreDC(nSavedDC);

	// ���Ʊ߽�
	GetClientRect(&rc);
	CPen penFrame(PS_SOLID, 1, RGB(215, 215, 215));
	pOldPen = dc.SelectObject(&penFrame);
	CPoint ptLT(rc.left+1, rc.top+1);
	CPoint ptLB(rc.left+1, rc.bottom-1);
	CPoint ptRT(rc.right-1, rc.top+1);
	CPoint ptRB(rc.right-1, rc.bottom-1);
	if(m_nModol & FRAME_DRAW_LEFT)
	{
		dc.MoveTo(ptLT);
		dc.LineTo(ptLB);
	}
	if(m_nModol & FRAME_DRAW_RIGHT)
	{
		dc.MoveTo(ptRT);
		dc.LineTo(ptRB);
	}
	if(m_nModol & FRAME_DRAW_TOP)
	{
		dc.MoveTo(ptLT);
		dc.LineTo(ptRT);
	}
	if(m_nModol & FRAME_DRAW_BOTTOM)
	{
		dc.MoveTo(ptLB);
		dc.LineTo(ptRB);
	}
	dc.SelectObject(pOldPen);
}

int CColumnCtrl::AddItem(CString strItemText, CString strImgPath, BOOL bRraw)
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

void CColumnCtrl::SetCurSel(int nSel)
{
	if (::IsWindow(GetSafeHwnd()) && m_nCurSel != nSel)
	{
		int nOldSel = m_nCurSel;
		m_nCurSel = nSel;
		Invalidate();		
		::SendMessage(GetParent()->GetSafeHwnd(), WM_COLUMNCTRL_SEL_CHANGE, (DWORD)this, MAKELONG(nOldSel, m_nCurSel));
	}
	else
	{
		m_nCurSel = nSel;
	}
}

int  CColumnCtrl::GetCurSel()
{
	return m_nCurSel;
}

void CColumnCtrl::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);
}

void CColumnCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CStatic::OnMouseMove(nFlags, point);
	point.y += m_nScrollHeight;

	if(!m_bTracking)
	{
		m_TMS.hwndTrack = GetSafeHwnd();
		m_bTracking = TrackMouseEvent(&m_TMS);
	} 

	// ����ѡ�е�item
	m_nMoveSel = -1;
	if(m_enDire == EN_X_DIRE)
	{
		if((int)point.x % m_nItemWidth > 0)
		{
			m_nMoveSel = (int)point.x / m_nItemWidth;
		}
	}
	else
	{
		if((int)point.y % m_nItemHeight > 0)
		{
			m_nMoveSel = (int)point.y / m_nItemHeight;
		}
	}

	if(m_nMoveSel != -1 && m_nPreSel != m_nMoveSel)
	{
		Invalidate();
		m_nPreSel = m_nMoveSel;
	}
}

void CColumnCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonDown(nFlags, point);
}

void CColumnCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_vScroll.SetFocus();
	CStatic::OnLButtonUp(nFlags, point);
	point.y += m_nScrollHeight;
	int nSel = -1;
	if(m_enDire == EN_X_DIRE)
	{
		if((int)point.x % m_nItemWidth > 0)
		{
			nSel = (int)point.x / m_nItemWidth;
		}
	}
	else
	{
		if((int)point.y % m_nItemHeight > 0)
		{
			nSel = (int)point.y / m_nItemHeight;
		}
	}

	if(nSel != -1 && nSel != m_nCurSel && nSel < m_vecItem.size())
	{
		int nOldSel = m_nCurSel;
		m_nCurSel = nSel;		
		Invalidate();		
		::SendMessage(GetParent()->GetSafeHwnd(), WM_COLUMNCTRL_SEL_CHANGE, (DWORD)this, MAKELONG(nOldSel, m_nCurSel));
	}
}

// ��������
void CColumnCtrl::SetFont(CFont *pfont)
{
	m_pfont = pfont;
}

void CColumnCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyle(0,SS_NOTIFY);

	m_TMS.cbSize = sizeof(TRACKMOUSEEVENT);
	m_TMS.dwFlags = TME_HOVER | TME_LEAVE;
	m_TMS.hwndTrack = GetSafeHwnd();
	m_TMS.dwHoverTime = 10;
	TrackMouseEvent(&m_TMS); 

	CRect rect;
	GetClientRect(rect);

	CRect rectScroll;
	rectScroll.right = rect.right;
	rectScroll.left = rectScroll.right - 10;
	rectScroll.top = rect.top;
	rectScroll.bottom = rect.bottom;
	m_vScroll.Create(rectScroll, this, 12121);
	m_vScroll.ShowWindow(SW_HIDE);

	CalcCoord();	
}

 LRESULT CColumnCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
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
void CColumnCtrl::RemoveAllItems()
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
void CColumnCtrl::CalcCoord()
{
	if(!::IsWindow(GetSafeHwnd()))
		return;

	CRect rc;
	GetClientRect(&rc);

	if(m_enDire == EN_X_DIRE)
	{	// ��������
		if(m_nItemWidth == 0)
			m_nItemWidth = ITEM_DEF_WIDTH;
		m_nItemHeight = rc.Height();
	}
	else
	{	// ��������
		m_nItemWidth = rc.Width();
		if(m_nItemHeight == 0)
			m_nItemHeight = ITEM_DEF_HEIGHT;
	}

	CDC *pDC = GetDC();
	CFont *pOldFont = pDC->SelectObject(m_pfont);

	for(size_t i = 0; i != m_vecItem.size(); ++i)
	{
		STRU_ITEMPRO &item = m_vecItem[i];

		// ������Ŀ������
		if(m_enDire == EN_X_DIRE)
		{	// ��������
			item.rcRange.top = rc.top;
			item.rcRange.bottom = rc.bottom;
			item.rcRange.left = rc.left + i*m_nItemWidth;
			item.rcRange.right = item.rcRange.left + m_nItemWidth;
		}
		else
		{
			// ��������
			item.rcRange.left = rc.left;
			item.rcRange.right = rc.right;
			item.rcRange.top = rc.top + i*m_nItemHeight;
			item.rcRange.bottom = item.rcRange.top + m_nItemHeight;
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
		if(m_enDire == EN_X_DIRE)
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
		else
		{	// �������У��ı������
			if(!item.img.IsNull())
			{	// ��λͼ���ü���λͼ����
				item.ptImg.x = 5;
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
				item.rcText.left = item.rcRange.left + 8;
				item.rcText.right = item.rcText.left + nTotalWidth + 1;
			}
		}

		// �����»���λ��
		if(m_bDrawUnderline)
		{
			item.bHasUnderline = m_bDrawUnderline;
			if(!item.img.IsNull())
			{	// ��λͼ���»��������λͼ�·���ʼ
				item.ptBgn.x = item.ptImg.x;
				item.ptBgn.y = item.ptImg.y + item.img.GetHeight() + 5;

				item.ptEnd.x = item.ptBgn.x + nTotalWidth;
				item.ptEnd.y = item.ptBgn.y;
			}
			else
			{	// ��λͼ���»������ı����·�
				item.ptBgn.x = item.rcText.left;
				item.ptBgn.y = item.rcText.bottom + 3;

				item.ptEnd.x = item.ptBgn.x + nTotalWidth;
				item.ptEnd.y = item.ptBgn.y;
			}
		}
	}

	pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);

	if(m_enDire == EN_Y_DIRE && m_nItemHeight * m_vecItem.size() > rc.Height())
	{
		m_vScroll.SetWndLength(rc.Height(), m_nItemHeight * m_vecItem.size());
		m_vScroll.ShowWindow(SW_SHOW/*SW_HIDE*/);
	}
	else
	{
		m_vScroll.ShowWindow(SW_HIDE);
	}
}

// ��ȡ�ı�����
CString CColumnCtrl::GetItemText(int i)
{
	if(i >= 0 && i < m_vecItem.size())
	{
		return m_vecItem[i].strText;
	}

	return "";
}

void CColumnCtrl::SetItemHeight(int nItemHeight)
{
	m_nItemHeight = nItemHeight;

	if(::IsWindow(GetSafeHwnd()))
	{
		CalcCoord();
		Invalidate();
	}
}

LRESULT CColumnCtrl::OnScrollBarMsg(WPARAM wParam, LPARAM lParam)
{
	m_dPrecent = (double)lParam;
	Invalidate();
	return 1;
}

// ����ITEM�Ŀ�ȣ�ֻ�ں�������ʱ����Ч
void CColumnCtrl::SetItemWidth(int nItemWidth)
{
	m_nItemWidth = nItemWidth;

	if(::IsWindow(GetSafeHwnd()))
	{
		CalcCoord();
		Invalidate();
	}
}