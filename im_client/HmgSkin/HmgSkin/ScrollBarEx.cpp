// ScrollBarEx.cpp : implementation file
//

#include "stdafx.h"
#include "MemDC.h"
#include "ScrollBarEx.h"
//#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define SCROLLBAREXCLASSNAME "ScrollBarExWnd"

/////////////////////////////////////////////////////////////////////////////
// CScrollBarEx
const int c_nSlideTimesOnePage = 20;			// 将一页滑完需要滑动的次数
const int c_nSliderMoveLengthPerSlide = 4;			// 滑块滑动一次前进的距离

enum
{
	TIMERID_TOPBTNHOVER = 100,
	TIMERID_BOTTOMBTNHOVER
};

CScrollBarEx::CScrollBarEx()
{
	RegisterWindowClass();	
	
	m_bThumbPressed = FALSE;
	m_nParentWndActuallyLength = 1;
	m_nParentWndNeedLength = 1;

	HMODULE hModule = GetModuleHandle("HmgSkinRes.dll"); 
	m_imgScroll.LoadFromResource(hModule, IDB_BITMAP_SCROLL);

	m_rcImgTopNormal = CRect(0, 0, 10, 10);
	m_rcImgBottomNormal = CRect(0, 11, 9, 21);

	m_rcImgMoveArea = CRect(0, 22, 10, 113);
	m_rcImgSlider = CRect(33, 0, 43, 35);

	m_nSliderCurMoveLength = 0;

	m_bPtInTopBtn = FALSE;
	m_bPtInBottomBtn = FALSE;
}

CScrollBarEx::~CScrollBarEx()
{
	m_imgScroll.Destroy();
}

// 设置窗口长度
void CScrollBarEx::SetWndLength(int nParentWndActuallyLength, int nParentWndNeedLength)
{
	m_nParentWndActuallyLength = nParentWndActuallyLength;
	m_nParentWndNeedLength = nParentWndNeedLength;

	if(::IsWindow(GetSafeHwnd()))
	{
		Invalidate();
	}
}


BEGIN_MESSAGE_MAP(CScrollBarEx, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CScrollBarEx message handlers
BOOL CScrollBarEx::RegisterWindowClass(void)
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetResourceHandle();
	
	if (!(::GetClassInfo(hInst, SCROLLBAREXCLASSNAME, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc      = ::DefWindowProc;
		wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
		wndcls.hInstance        = hInst;
		wndcls.hIcon            = NULL;
		wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground    = NULL; // No Background brush (gives flicker)
		wndcls.lpszMenuName     = NULL;
		wndcls.lpszClassName    = SCROLLBAREXCLASSNAME;
		
		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}
	
	return TRUE;
}

BOOL CScrollBarEx::Create(const RECT rect, CWnd *pParentWnd, UINT nID)
{
	BOOL bSuccess = CWnd::Create(SCROLLBAREXCLASSNAME, "", WS_VISIBLE | WS_CHILD, rect, pParentWnd, nID);
	
	/*CRect rc;
	GetClientRect(&rc);
	rc.right = 10;
	MoveWindow(&rc);*/

	return bSuccess;
}

void CScrollBarEx::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rcClient;
	CRgn rgnScrollBar;
	CDC		MemDC;
	CBitmap	bm;
	CBitmap *pOldBm;	

	GetClientRect(rcClient); 
	
	rgnScrollBar.CreateRectRgnIndirect(&rcClient);
	dc.SelectClipRgn(&rgnScrollBar);

	MemDC.CreateCompatibleDC(&dc);
	bm.CreateCompatibleBitmap(&dc, rcClient.Width(), rcClient.Height());
	pOldBm = (CBitmap *)MemDC.SelectObject(&bm);

	CalcRect();
	DrawTBButton(&MemDC);
	DrawScrollZoneButton(&MemDC);
	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &MemDC, 0, 0, SRCCOPY);
	
	MemDC.SelectObject(pOldBm);
	MemDC.DeleteDC();
}

void CScrollBarEx::CalcRect()
{
	// 计算每一次滑动需要前进的距离
	m_nParentMoveLengthPerSlide = m_nParentWndActuallyLength/c_nSlideTimesOnePage;
	// 需要滑动的次数
	m_nSlideCount = (m_nParentWndNeedLength-m_nParentWndActuallyLength)/m_nParentMoveLengthPerSlide;

	// 计算滑块的长度
	CRect rc;
	GetClientRect(&rc);

	// 向上按钮，向下按钮的长度
	int nTopBtnHeight = 10;
	int nBottomBtnHeight = 10;

	// 滑动区的长度
	int nMoveAreaHeight = rc.Height()-nTopBtnHeight-nBottomBtnHeight;

	// 滑块的长度
	int nSliderHeight = nMoveAreaHeight - (c_nSliderMoveLengthPerSlide*m_nSlideCount);
	if(nSliderHeight <= 15)
	{
		nSliderHeight = 15;
	}

	// 滑块能够滑动的距离
	m_nSliderCanMoveLength = nMoveAreaHeight-nSliderHeight;

	m_rcSBTopBtn.left = 0;
	m_rcSBTopBtn.right = 10;
	m_rcSBTopBtn.top = 0;
	m_rcSBTopBtn.bottom = 10;

	m_rcSBBottomBtn.left = 0;
	m_rcSBBottomBtn.right = 10;
	m_rcSBBottomBtn.top = rc.bottom-10;
	m_rcSBBottomBtn.bottom = rc.bottom;

	m_rcScrollBar.left = 0;
	m_rcScrollBar.right = 10;
	m_rcScrollBar.top = m_rcSBTopBtn.bottom;
	m_rcScrollBar.bottom = m_rcScrollBar.top + nMoveAreaHeight;

	m_rcSBThumb.left = 0;
	m_rcSBThumb.right = 10;
	m_rcSBThumb.top = m_rcSBTopBtn.bottom+m_nSliderCurMoveLength;
	m_rcSBThumb.bottom = m_rcSBThumb.top + nSliderHeight;
}

void CScrollBarEx::DrawTBButton(CDC *pDC)
{
	m_imgScroll.Draw(pDC->m_hDC, m_rcSBTopBtn, m_rcImgTopNormal);
	m_imgScroll.Draw(pDC->m_hDC, m_rcSBBottomBtn, m_rcImgBottomNormal);
}

void CScrollBarEx::DrawScrollZoneButton(CDC *pDC)
{
	DrawEllipse(pDC, m_rcScrollBar, m_rcImgMoveArea);
	DrawEllipse(pDC, m_rcSBThumb, m_rcImgSlider);
}

void CScrollBarEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (GetFocus()->GetSafeHwnd() != GetSafeHwnd())
	{
		SetFocus();
	}
	
	SetCapture();

	BOOL bSndMsg = FALSE;
	m_bPtInTopBtn = FALSE;
	m_bPtInBottomBtn = FALSE;

	// 判断是否在各个区域内部
	if (m_rcSBTopBtn.PtInRect(point))
	{	
		if(m_nSliderCurMoveLength > 0)
		{
			m_nSliderCurMoveLength -= c_nSliderMoveLengthPerSlide;
			if(m_nSliderCurMoveLength < 0)
			{
				m_nSliderCurMoveLength = 0;
			}
			bSndMsg = TRUE;
			m_bPtInTopBtn = TRUE;
			SetTimer(TIMERID_TOPBTNHOVER, 30, NULL);
		}
	}
	else if (m_rcSBBottomBtn.PtInRect(point))
	{	
		if(m_nSliderCurMoveLength < m_nSliderCanMoveLength)
		{
			m_nSliderCurMoveLength += c_nSliderMoveLengthPerSlide;
			if(m_nSliderCurMoveLength > m_nSliderCanMoveLength)
			{
				m_nSliderCurMoveLength = m_nSliderCanMoveLength;
			}
			bSndMsg = TRUE;
			m_bPtInBottomBtn = TRUE;
			SetTimer(TIMERID_BOTTOMBTNHOVER, 30, NULL);
		}
	}
	else if (m_rcSBThumb.PtInRect(point))
	{	
		m_bThumbPressed = TRUE;
		m_ClickPoint = point;
	}
	else if (m_rcScrollBar.PtInRect(point))
	{	
		//m_bSBPressed = TRUE;
		// 看看是增加还是减少
		if (point.y > m_rcSBThumb.bottom)
		{
			m_nSliderCurMoveLength += c_nSliderMoveLengthPerSlide*2;	
			bSndMsg = TRUE;
		}
		else if (point.y < m_rcSBThumb.top)
		{
			m_nSliderCurMoveLength -= c_nSliderMoveLengthPerSlide*2;
			bSndMsg = TRUE;
		}

		if(m_nSliderCurMoveLength < 0)
		{
			m_nSliderCurMoveLength = 0;
			bSndMsg = TRUE;
		}
		if(m_nSliderCurMoveLength > m_nSliderCanMoveLength)
		{
			m_nSliderCurMoveLength = m_nSliderCanMoveLength;
			bSndMsg = TRUE;
		}
	}

	if (bSndMsg)
	{
		Invalidate();
		SendMsgToParentWnd();
	}
}

void CScrollBarEx::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();
	m_bPtInTopBtn = FALSE;
	m_bPtInBottomBtn = FALSE;

	if (m_bThumbPressed)
	{	
		m_bThumbPressed = FALSE;		
	}
	else
	{
		Invalidate();

		KillTimer(TIMERID_TOPBTNHOVER);
		KillTimer(TIMERID_BOTTOMBTNHOVER);

		SendMsgToParentWnd();
	}
}

void CScrollBarEx::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bThumbPressed)
	{
		if(point.y < m_rcSBTopBtn.bottom || point.y > m_rcSBBottomBtn.top)
			return;

		m_nSliderCurMoveLength += (point.y - m_ClickPoint.y);
		if(m_nSliderCurMoveLength < 0)
		{
			m_nSliderCurMoveLength = 0;
		}
		if(m_nSliderCurMoveLength > m_nSliderCanMoveLength)
		{
			m_nSliderCurMoveLength = m_nSliderCanMoveLength;
		}
		
		SendMsgToParentWnd();
		
		m_ClickPoint = point;

		Invalidate();
	}	

	if(m_bPtInTopBtn)
	{
		if(m_rcSBTopBtn.PtInRect(point))
		{
			SetTimer(TIMERID_TOPBTNHOVER, 30, NULL);
		}
		else
		{
			KillTimer(TIMERID_TOPBTNHOVER);
		}
	}

	if(m_bPtInBottomBtn)
	{
		if(m_rcSBBottomBtn.PtInRect(point))
		{
			SetTimer(TIMERID_BOTTOMBTNHOVER, 30, NULL);
		}
		else
		{
			KillTimer(TIMERID_BOTTOMBTNHOVER);
		}
	}
}

void CScrollBarEx::OnTimer(UINT nIDEvent) 
{
	switch(nIDEvent)
	{
	case TIMERID_TOPBTNHOVER:
		{
			KillTimer(nIDEvent);

			if(m_bPtInTopBtn && m_nSliderCurMoveLength > 0)
			{
				m_nSliderCurMoveLength -= c_nSliderMoveLengthPerSlide;
				if(m_nSliderCurMoveLength < 0)
				{
					m_nSliderCurMoveLength = 0;
				}

				Invalidate();
				SendMsgToParentWnd();

				SetTimer(TIMERID_TOPBTNHOVER, 30, NULL);
			}
		}
		break;
	case TIMERID_BOTTOMBTNHOVER:
		{
			KillTimer(nIDEvent);

			if(m_bPtInBottomBtn && m_nSliderCurMoveLength < m_nSliderCanMoveLength)
			{
				m_nSliderCurMoveLength += c_nSliderMoveLengthPerSlide;
				if(m_nSliderCurMoveLength > m_nSliderCanMoveLength)
				{
					m_nSliderCurMoveLength = m_nSliderCanMoveLength;
				}

				Invalidate();
				SendMsgToParentWnd();

				SetTimer(TIMERID_BOTTOMBTNHOVER, 30, NULL);
			}
		}
	}

	CWnd::OnTimer(nIDEvent);
}

void CScrollBarEx::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	OnLButtonDown(nFlags, point);
}

void CScrollBarEx::SendMsgToParentWnd()
{
	CWnd *pWnd = GetParent();	

	// 滑动百分百
	double dPercent = (m_nSliderCurMoveLength / (double)m_nSliderCanMoveLength) * 100;

	::SendMessage(pWnd->m_hWnd, WM_SCROLLBAR_MSG, (WPARAM)GetDlgCtrlID(), (LPARAM)dPercent);
}

BOOL CScrollBarEx::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	//return CWnd::OnEraseBkgnd(pDC);
}

void CScrollBarEx::DrawEllipse(CDC *pDC, CRect &rcDest, CRect &rcImg)
{
	CRect rcDstUpper, rcDstMiddle, rcDstUnder;
	CRect rcImgUpper, rcImgMiddle, rcImgUnder;
	
	rcDstUpper = CRect(rcDest.left, rcDest.top, rcDest.right, rcDest.top+5);
	rcDstUnder = CRect(rcDest.left, rcDest.bottom-4, rcDest.right, rcDest.bottom);
	rcDstMiddle = CRect(rcDest.left, rcDstUpper.bottom, rcDest.right, rcDstUnder.top);

	rcImgUpper = CRect(rcImg.left, rcImg.top, rcImg.right, rcImg.top+5);
	rcImgUnder = CRect(rcImg.left, rcImg.bottom-4, rcImg.right, rcImg.bottom);
	rcImgMiddle = CRect(rcImg.left, rcImgUpper.bottom, rcImg.right, rcImgUnder.top);

	m_imgScroll.Draw(pDC->m_hDC, rcDstUpper, rcImgUpper);
	m_imgScroll.Draw(pDC->m_hDC, rcDstUnder, rcImgUnder);
	m_imgScroll.StretchBlt(pDC->m_hDC, rcDstMiddle, rcImgMiddle);
}

BOOL CScrollBarEx::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if(zDelta < 0)
	{
		if(m_nSliderCurMoveLength < m_nSliderCanMoveLength)
		{
			m_nSliderCurMoveLength += c_nSliderMoveLengthPerSlide;
			if(m_nSliderCurMoveLength > m_nSliderCanMoveLength)
			{
				m_nSliderCurMoveLength = m_nSliderCanMoveLength;
			}

			Invalidate();
			SendMsgToParentWnd();
		}
	}
	else
	{
		if(m_nSliderCurMoveLength > 0)
		{
			m_nSliderCurMoveLength -= c_nSliderMoveLengthPerSlide;
			if(m_nSliderCurMoveLength < 0)
			{
				m_nSliderCurMoveLength = 0;
			}

			Invalidate();
			SendMsgToParentWnd();

			SetTimer(TIMERID_TOPBTNHOVER, 30, NULL);
		}
	}
	return TRUE;
}