#include "StdAfx.h"
#include "seolistctrl.h"
#include "WriteArticle.h"
#include ".\seolistctrl.h"

#define  ITEM_HEIGHT  35
#define  SCROLL_MAX_HEIGHT 3000
// CSEOListCtrl
void DrawBitmap(CDC *pDC,int nDestX, int nDestY, CBitmap *pBitmap, int nSrcX, int nSrcY, int nWidth, int nHeight, COLORREF clrTrans)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	CBitmap *pOld=dcMem.SelectObject(pBitmap);

	TransparentBlt(pDC->m_hDC,nDestX,nDestY,nWidth,nHeight,
		dcMem.m_hDC,nSrcX,nSrcY,
		nWidth,
		nHeight,
		clrTrans);

	dcMem.SelectObject(pOld);

	dcMem.DeleteDC();
}

IMPLEMENT_DYNAMIC(CSEOListCtrl, CWnd)
CSEOListCtrl::CSEOListCtrl()
{
	m_ToolTip.m_hWnd   = NULL;
	m_nCurSel          = -1;
	m_nClientRealHeigh = 0;
	m_OriginY          = 0;
	m_strPreTip        = "";

	//m_Font.CreateFont(
	//	16,                        // nHeight
	//	0,                         // nWidth
	//	0,                         // nEscapement
	//	0,                         // nOrientation
	//	FW_NORMAL,                  // nWeight
	//	FALSE,                     // bItalic
	//	FALSE,                     // bUnderline
	//	0,                         // cStrikeOut
	//	ANSI_CHARSET,              // nCharSet
	//	OUT_DEFAULT_PRECIS,        // nOutPrecision
	//	CLIP_DEFAULT_PRECIS,       // nClipPrecision
	//	DEFAULT_QUALITY,           // nQuality
	//	DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	//	"ËÎÌå");
	m_Font.CreateFont(
		15,                        // nHeight
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
		"ËÎÌå");                 // lpszFacename

	m_Font1.CreateFont(
		12,                        // nHeight
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
		"ËÎÌå");  

	m_hCursor = ::LoadCursor(NULL,IDC_ARROW);
	//m_bmpStar.LoadBitmap(IDB_STAR);
	//m_bmpLight.LoadBitmap(IDB_LIGHT);

	m_nCurSel = -1;
	m_nHotItem = -1;
	m_nFocusItem = -1;

	m_nFirstVisibleItem=m_nLastVisibleItem=0;
}

CSEOListCtrl::~CSEOListCtrl()
{
}


BEGIN_MESSAGE_MAP(CSEOListCtrl, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

int CSEOListCtrl::HitTest(CPoint pt)
{
	if ( m_vecItems.size()>0 )
	{
        pt.y+=m_OriginY;

		for ( int i=m_nFirstVisibleItem; i<=m_nLastVisibleItem; i++ )
		{
			if ( m_vecItems[i].rcItem.PtInRect(pt) )
				return i;
		}
	}

	return -1;
}

void CSEOListCtrl::ShowItemTip(int nItem)
{
	if ( nItem>-1 && (UINT)nItem<m_vecItems.size() )
	{
		tagItem &ti = m_vecItems[nItem];
		CString strTip;
		strTip.Format("%d¡¢%s\n%s",nItem+1,ti.strTitle,ti.strDate);
		ShowToolTip(strTip);
	}
}

void CSEOListCtrl::ShowToolTip(CString strTip)
{
	if ( strTip == m_strPreTip )
	{
		return ;
	}

	InitToolTip();
	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rc;
		GetClientRect(&rc);
		m_ToolTip.AddTool(this, strTip, rc, 1);
	} // if


	m_ToolTip.UpdateTipText(strTip ,this,1);

	m_strPreTip = strTip ;

	m_ToolTip.Activate(TRUE);
}

void CSEOListCtrl::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		m_ToolTip.Create(this,TTS_ALWAYSTIP);
		m_ToolTip.Activate(TRUE);
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 350);
	} 
} 

//nStyle
void CSEOListCtrl::DrawPtInRect(const CPoint &ptCursor,const CRect & rcRect,CDC *pDC, int nStyle, bool bMouseDown)
{
	if (rcRect.PtInRect(ptCursor) )
	{
		if ( nStyle==0 )
		{
			if ( bMouseDown )
				pDC->Draw3dRect(&rcRect,RGB(122,122,122),RGB(233,233,233));
			else
				pDC->Draw3dRect(&rcRect,RGB(233,233,233),RGB(122,122,122));
		}
		else
		if ( nStyle==1 )
		{
			CRect rcTemp = rcRect;

			if ( bMouseDown )
			{
				rcTemp.InflateRect(-1,-1);
			}

			CPoint pt(3,3);

			CPen   p(PS_SOLID,1,RGB(0,188,199));
			CPen   *pp = pDC->SelectObject(&p);

			pDC->RoundRect(rcTemp,pt);

			pDC->FillSolidRect(rcTemp.left+1,rcTemp.top+1,
				rcTemp.Width()-2,rcTemp.Height()-2,
					RGB(200,222,255));

			pDC->SelectObject(pp);
		}
	}
}


BOOL CSEOListCtrl::PreTranslateMessage(MSG* pMsg)
{
	if ( m_ToolTip.m_hWnd )
		m_ToolTip.RelayEvent(pMsg);

	return CWnd::PreTranslateMessage(pMsg);
}

BOOL CSEOListCtrl::Create(CWnd *pParentWnd,const CRect &rect, DWORD dwStyle/* =WS_VISIBLE|WS_BORDER|WS_CHILD */,UINT nID/* =0 */)
{
	return CWnd::Create( AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS),
						"SEOListCtrl",
						dwStyle,
						rect,pParentWnd,
						nID);
}

void CSEOListCtrl::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();
	SetFont(&m_Font);
}

BOOL CSEOListCtrl::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CSEOListCtrl::OnPaint()
{
	CPaintDC dc(this); 	
    
	CRect rc;
	GetClientRect(&rc);

	RecalcLayout();
	Draw(&dc, rc);
}

void CSEOListCtrl::AddItem(CString strTitle,CString strDate, DWORD dwData)
{
	tagItem ti;

	ti.dwData = dwData;
	ti.strDate = strDate;
	ti.strTitle = strTitle;
	
	m_vecItems.push_back(ti);
}

void CSEOListCtrl::DrawItem(int nItem,tagItem &item, CDC *pDC, CRect rcDraw)
{
	CString strItem;
	int nDrawTop = rcDraw.top+2;
	
	COLORREF clrText ;

	if ( nItem==m_nCurSel )
	{
		pDC->FillSolidRect(&rcDraw,RGB(49,106,197));
		clrText = RGB(255,255,255);
	}
	else
	{
		clrText = RGB(0,0,0);
	}


	CResBitmap bmpItem;
	bmpItem.LoadBitmap(IDB_ARTS_ITEM);
	DrawBitmap(pDC,rcDraw.left+5,nDrawTop+2,&bmpItem,0);

	pDC->SetTextColor(clrText);

	strItem.Format("%s",item.strTitle);
	pDC->SelectObject(&m_Font);
	
	CRect rcItemText = rcDraw;
	rcItemText.top   = nDrawTop;
	rcItemText.left  = rcDraw.left+25;
	pDC->DrawText(strItem,rcItemText,DT_TOP|DT_LEFT|DT_END_ELLIPSIS);

	nDrawTop += pDC->GetTextExtent("A").cy+2;


	pDC->SelectObject(&m_Font1);

	strItem.Format("%s",item.strDate);
	pDC->TextOut(rcDraw.left+26,nDrawTop,strItem);

	CPen penLine(PS_SOLID,1,RGB(233,233,188));
	CPen *pOldpen = pDC->SelectObject(&penLine);
	pDC->MoveTo(rcDraw.left,rcDraw.bottom-1);
	pDC->LineTo(rcDraw.right,rcDraw.bottom-1);
	pDC->SelectObject(pOldpen);
}

void CSEOListCtrl::Draw(CDC *pDC, CRect rcDraw)
{
	CRect	rectFill;
	CDC		dcMem;
	CBitmap dcBitmap;

	rectFill.top    = rcDraw.top+m_OriginY;
	rectFill.left   = rcDraw.left;
	rectFill.bottom = rectFill.top+rcDraw.Height();
	rectFill.right  = rcDraw.right;

	dcMem.CreateCompatibleDC(pDC);
	dcBitmap.CreateCompatibleBitmap(pDC,rcDraw.Width(),rcDraw.Height());

	int nSaveDC = dcMem.SaveDC();

	dcMem.SelectObject(&dcBitmap);
	dcMem.FillSolidRect(0,0,rcDraw.Width(),rcDraw.Height(),SEO_NORMALCOLOR);
	dcMem.SelectObject(&m_Font);
	dcMem.SetBkMode(TRANSPARENT);

	int nItems = m_vecItems.size();

	int  nOffset = 0;
	bool bFirst  = false;

	m_nLastVisibleItem = nItems-1;

	for ( int i=0; i<nItems; i++ )
	{
		CString strItem;
		tagItem & ti = m_vecItems[i];
	
		CRect rcInter;
		rcInter.IntersectRect(ti.rcItem,rectFill);

		if ( rcInter.IsRectNull()==FALSE )
		{
			CRect rcItemDraw = ti.rcItem;

			rcItemDraw.top -= m_OriginY;
			rcItemDraw.bottom -= m_OriginY;

			if ( bFirst==false )
			{
				m_nFirstVisibleItem = i;

				if ( rcItemDraw.top<0 )
				{
					nOffset-=rcItemDraw.top;
				}

				bFirst = true;
			}

			rcItemDraw.top+=nOffset;
			rcItemDraw.bottom+=nOffset;

			DrawItem(i,ti,&dcMem,rcItemDraw);
		}
		else
		{
			if ( bFirst )
			{
				m_nLastVisibleItem = i-1;
				break;
			}
		}
	}

	m_OriginY-=nOffset;

	CRect rc = rcDraw;
	dcMem.Draw3dRect(&rc,GetSysColor(COLOR_3DSHADOW),GetSysColor(COLOR_3DLIGHT));
	rc.InflateRect(-1,-1);
	dcMem.Draw3dRect(&rc,GetSysColor(COLOR_3DDKSHADOW),GetSysColor(COLOR_3DHILIGHT));
	rc.InflateRect(1,1);

	pDC->BitBlt(0,0,rcDraw.Width(),rcDraw.Height(),&dcMem,0,0,SRCCOPY);

	dcMem.RestoreDC(nSaveDC);
}

void CSEOListCtrl::RecalcLayout()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int nItems			= m_vecItems.size();
	int nPreItemHeight	= 0;

	for ( int nItem = 0; nItem < nItems; nItem++ )
	{
		tagItem &ti = m_vecItems[nItem];

		ti.rcItem.left = rcClient.left;
		ti.rcItem.top  = nPreItemHeight;
		ti.rcItem.right  = rcClient.right - 2;
		ti.rcItem.bottom = ti.rcItem.top + ITEM_HEIGHT;
		
		nPreItemHeight = ti.rcItem.bottom;

		if ( nItem==m_nCurSel )
		{
			/*ti.rcItem.bottom+=25;
			nPreItemHeight+=25;*/
		}
	}

	if ( nItems>0 )
		nPreItemHeight+=ITEM_HEIGHT;
	
	if (m_nClientRealHeigh != nPreItemHeight) 
	{
		m_nClientRealHeigh = max(nPreItemHeight, rcClient.Height());
		CalcScrollInfo();
	}	
	
	if (m_nClientRealHeigh - m_OriginY < rcClient.Height()) 
	{
		m_OriginY = m_nClientRealHeigh - rcClient.Height();
	}
	
	if (m_OriginY < 0) 
	{
		m_OriginY = 0;
	}
}

void CSEOListCtrl::CalcScrollInfo()
{
				
	if (IsWindow(m_hWnd) && m_vecItems.size()>0 ) 
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		int nClientHeight = rcClient.Height();
		double fPercent   = SCROLL_MAX_HEIGHT/(double)m_nClientRealHeigh;

		
		SCROLLINFO si;
		memset(&si, 0, sizeof(si));
		si.cbSize = sizeof(si);
		si.fMask  = SIF_PAGE | SIF_RANGE;
		si.nPage  = rcClient.Height()*fPercent;
		si.nMin = 0;

		si.nMax = SCROLL_MAX_HEIGHT;//m_nClientRealHeigh - 1;

		
		if (nClientHeight>=m_nClientRealHeigh ) 
		{
			m_OriginY = 0;
			ShowScrollBar(SB_VERT, FALSE);
		}
		else
		{
			m_nScrollStep = ITEM_HEIGHT*fPercent;
			m_fScrollPercent = fPercent;
			ShowScrollBar(SB_VERT,TRUE);
			CWnd::SetScrollInfo(SB_VERT, &si);
		}
	}
}
 void CSEOListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	SetFocus();

	switch (nSBCode) 
	{
	case SB_TOP:
		OnVScroll(SB_LINEUP, nPos, pScrollBar);
		TRACE("\nSB_TOP");
		break;
	case  SB_BOTTOM:
		OnVScroll(SB_LINEDOWN, nPos, pScrollBar);
		TRACE("\nSB_BOTTOM");
		break;
	case SB_LINEUP:
		if (m_OriginY > 0) 
		{
			CWnd::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - m_nScrollStep );
		}
		TRACE("\nSB_LINEUP");
		break;
	case SB_LINEDOWN:
		if (m_OriginY + rcClient.Height() < m_nClientRealHeigh) 
		{
			CWnd::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + m_nScrollStep);
		}
		TRACE("\nSB_LINEDOWN");
		break;
	case SB_PAGEDOWN:
		if (m_OriginY + rcClient.Height() < m_nClientRealHeigh) 
		{
			CWnd::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + rcClient.Height()*m_fScrollPercent);
		}
		TRACE("\nSB_PAGEDOWN");
		break;
	case SB_PAGEUP:
		if (m_OriginY > 0) 
		{
			CWnd::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - rcClient.Height()*m_fScrollPercent);
		}
		TRACE("\nSB_PAGEUP");
		break;
	case SB_THUMBPOSITION:
		CWnd::SetScrollPos(SB_VERT, nPos);
		TRACE("\nSB_THUMBPOSITION");
		break;
	case SB_THUMBTRACK:
		CWnd::SetScrollPos(SB_VERT, nPos);
		TRACE("\nSB_THUMBTRACK");
		break;
	default:
		;
	}	

	TRACE("\nPOS=%d",nPos);

	int nScrollPos = GetScrollPos(SB_VERT);
	m_OriginY = this->m_nClientRealHeigh*((double)nScrollPos/SCROLL_MAX_HEIGHT);
	if ( m_OriginY<0 )
		m_OriginY=0;
	if ( m_OriginY>m_nClientRealHeigh )
		m_OriginY=m_nClientRealHeigh;

	Invalidate();

	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CSEOListCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	::SetCursor(m_hCursor);
	return TRUE;
	//return CWnd::OnSetCursor(pWnd, nHitTest, message);
}

BOOL CSEOListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	OnVScroll(zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, 0, 0);	
	return TRUE;
}

void CSEOListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	::SetCapture(m_hWnd);
	SetFocus();

	int nSel = HitTest(point);

	ShowToolTip("");

	TRACE("\nClick %d, CurSel=%d",nSel,m_nCurSel);

	if ( m_nCurSel!=nSel )
	{
		m_nCurSel=nSel;
		GetParent()->PostMessage(WM_SEOLIST_NOTIFY,0,SEO_CMD_SELCHANGE);
		Invalidate();
	}

	CWnd::OnLButtonDown(nFlags, point);
}

void CSEOListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();

	int nSel = HitTest(point);
	if ( nSel!=-1 )
		ShowItemTip(nSel);

	CWnd::OnLButtonUp(nFlags, point);
}

void CSEOListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	int nHotItem = HitTest(point);

	if ( m_nHotItem!=nHotItem )
	{
		if ( nHotItem!=-1 )
		{
			ShowItemTip(nHotItem);
		}

		m_nHotItem = nHotItem;
	}

	CWnd::OnMouseMove(nFlags, point);
}

DWORD CSEOListCtrl::GetItemData(int nItem)
{
	return m_vecItems[nItem].dwData;
}

void CSEOListCtrl::SetFocusItem(int nItem)
{
	m_nFocusItem=nItem;
	Invalidate();
}

void CSEOListCtrl::SetCurSel(int nSel)
{
	if ( nSel!=m_nCurSel && nSel>-1 )
	{
		m_nCurSel = nSel;

		RecalcLayout();

		if ( m_hWnd )
		{
			bool bUpdateScrollPos=false;

			if ( nSel<m_nFirstVisibleItem )
			{
				m_OriginY = m_vecItems[nSel].rcItem.top;
				bUpdateScrollPos=true;
			}

			if ( nSel>m_nLastVisibleItem )
			{
				int nCount = nSel-m_nLastVisibleItem;
				m_OriginY = m_vecItems[m_nFirstVisibleItem+nCount].rcItem.top;
				bUpdateScrollPos=true;
			}

			if ( bUpdateScrollPos )
			{				
				SCROLLINFO si;
				memset(&si, 0, sizeof(si));
				si.cbSize = sizeof(si);

				GetScrollInfo(SB_VERT,&si);
				si.nPos =SCROLL_MAX_HEIGHT*(double)m_OriginY/m_nClientRealHeigh;

				CWnd::SetScrollInfo(SB_VERT, &si);
			}
		}
		
		Invalidate();
	}
}

int CSEOListCtrl::GetCurSel()
{
	return m_nCurSel;
}

void CSEOListCtrl::DeleteItem(int nCurSel)
{
	m_vecItems.erase(m_vecItems.begin()+nCurSel);
	m_nCurSel=nCurSel;        //111111
	if ( m_nCurSel>=m_vecItems.size() )
		m_nCurSel = m_vecItems.size()-1;

	Invalidate();
}



void CSEOListCtrl::RemoveAllItems()
{
	m_nCurSel=-1;
	m_vecItems.clear();
	m_nFirstVisibleItem=m_nLastVisibleItem=0;
	m_nCurSel          = -1;
	m_nHotItem         = -1;
	m_nFocusItem       = -1;
	m_nClientRealHeigh = 0;
	m_OriginY          = 0;

	if ( m_hWnd )
	{
		CWnd::SetScrollPos(SB_VERT, 0 );
		Invalidate();
	}
}

int CSEOListCtrl::GetItemCount()
{
	return m_vecItems.size();
}