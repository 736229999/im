#include "AlbumFolder.h"
// AlbumFolder.cpp : 实现文件
//

#include "stdafx.h"
#include "AlbumFolder.h"

static const int ITEM_HEIGHT = 85;
static const int ITEM_WIDTH = 85;
// CAlbumFolder

IMPLEMENT_DYNAMIC(CAlbumFolder, CButtonBase)

CAlbumFolder::CAlbumFolder()
{
	m_Font.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL, // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"宋体");
	m_nCurSel  = -1;
	m_nHotItem = -1;
	m_bMouseDown = false;
	m_nOriginIndex = 0;
}

CAlbumFolder::~CAlbumFolder()
{
}


BEGIN_MESSAGE_MAP(CAlbumFolder, CButtonBase)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CAlbumFolder 消息处理程序
void CAlbumFolder::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CButtonBase::PreSubclassWindow();
	EnableToolTips(TRUE);
	ModifyStyle(0, BS_OWNERDRAW);
}


BOOL CAlbumFolder::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

BOOL CAlbumFolder::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if ( m_bShowScrollBtn )
	{
		if ( zDelta>0 )
		{
			m_nOriginIndex--;
			if ( m_nOriginIndex<0 )
				m_nOriginIndex=0;
		}
		else
		if ( zDelta<0 )
		{
			m_nOriginIndex++;
			if ( m_nOriginIndex>(m_vecItem.size()-1) )
			{
				m_nOriginIndex--;
			}
		}

		Invalidate();
	}

	return TRUE;
}

int CAlbumFolder::AddItem(CString strItemText, UINT nBitmap, DWORD dwData, int nIndex)
{
	tagItemInfo ti;
	ti.dwData      = dwData;
	ti.nBitmap     = nBitmap;
	ti.strItemText = strItemText;

	if ( nIndex>-1 && nIndex<m_vecItem.size() )
	{
		m_vecItem.insert(m_vecItem.begin()+nIndex,ti);
		if ( m_hWnd )
			Invalidate();
		return nIndex;
	}
	else
	{
		m_vecItem.push_back(ti);
		if ( m_hWnd )
			Invalidate();
		return m_vecItem.size()-1;
	}
}

void CAlbumFolder::SetItemText(int nItem, CString strItemText)
{
	if ( nItem>-1 && nItem<m_vecItem.size() )
	{
		m_vecItem[nItem].strItemText = strItemText;

		if ( m_hWnd )
			Invalidate();
	}
}

int CAlbumFolder::HitTest(CPoint point)
{
	if ( m_bShowScrollBtn )
	{
		if ( m_rcBtnUp.PtInRect(point) )
			return -2;
		
		if ( m_rcBtnDown.PtInRect(point) )
			return -3;
	}

	int n = m_vecItem.size();

	for ( int i=m_nOriginIndex; i<n; i++ )
	{
		if ( m_vecItem[i].rcItem.PtInRect(point) )
		{
			return i;
		}
	}

	return -1;
}

int CAlbumFolder::GetCurSel()
{
	return m_nCurSel;
}

DWORD CAlbumFolder::GetItemData(int nItem)
{
	if ( nItem>-1 && nItem<m_vecItem.size() )
		return m_vecItem[nItem].dwData;

	return 0;
}

int CAlbumFolder::GetCount()
{
	return m_vecItem.size();
}

void CAlbumFolder::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	CClientDC dc(this); 

	CRect rc;
	GetClientRect(&rc);

	int nCx    = rc.Width();
	int nCy    = rc.Height();

	int nCount = m_vecItem.size();

	int nTextHeight = 22;
	int nBtnHeight  = 14;
	
	CDC dcMem;
	CBitmap bmpMem;

	dcMem.CreateCompatibleDC(&dc);
	bmpMem.CreateCompatibleBitmap(&dc,nCx,nCy);

	int nSaveDC = dcMem.SaveDC();
	
	dcMem.SelectObject(&bmpMem);

	dcMem.FillSolidRect(0,0,nCx,nCy,IM_SPACE_COLOR);
	dcMem.SelectObject(&m_Font);
	dcMem.SetBkMode(TRANSPARENT);
	dcMem.SetTextAlign(TA_CENTER);

	int  nSpace = 4;

	int nRealItemHeight = (ITEM_HEIGHT+nTextHeight)*nCount + nSpace;

	if ( nRealItemHeight>nCy )
		m_bShowScrollBtn = true;
	else
		m_bShowScrollBtn = false;

	CRect rcAlbumList;

	if ( m_bShowScrollBtn )
	{
		CResBitmap bmpUp, bmpDown;
		bmpUp.LoadBitmap(IDB_IM_FOLDER_CLOSE);
		bmpDown.LoadBitmap(IDB_IM_FOLDER_EXPAND);

		//draw up down button
		m_rcBtnUp.left = nSpace;
		m_rcBtnUp.top  = nSpace;
		m_rcBtnUp.right = nCx-nSpace;
		m_rcBtnUp.bottom = m_rcBtnUp.top+nBtnHeight;

		DrawBitmap(&dcMem,m_rcBtnUp.left+(m_rcBtnUp.Width()-9)/2,
			m_rcBtnUp.top+(m_rcBtnUp.Height()-8)/2,&bmpUp,RGB(255,255,255) );

		if ( m_nHotItem==-2 )
		{
			if ( m_bMouseDown )
				dcMem.Draw3dRect(m_rcBtnUp,GetSysColor(COLOR_3DSHADOW),GetSysColor(COLOR_3DLIGHT));
			else
				dcMem.Draw3dRect(m_rcBtnUp,GetSysColor(COLOR_3DLIGHT),GetSysColor(COLOR_3DSHADOW));
		}


		m_rcBtnDown.left = nSpace;
		m_rcBtnDown.bottom  = nCy-nSpace;
		m_rcBtnDown.right = nCx-nSpace;
		m_rcBtnDown.top = m_rcBtnDown.bottom-nBtnHeight;

		DrawBitmap(&dcMem,m_rcBtnDown.left+(m_rcBtnDown.Width()-9)/2,
			m_rcBtnDown.top+(m_rcBtnDown.Height()-8)/2,&bmpDown,RGB(255,255,255) );

		if ( m_nHotItem==-3 )
		{
			if ( m_bMouseDown )
				dcMem.Draw3dRect(m_rcBtnDown,GetSysColor(COLOR_3DSHADOW),GetSysColor(COLOR_3DLIGHT));
			else
				dcMem.Draw3dRect(m_rcBtnDown,GetSysColor(COLOR_3DLIGHT),GetSysColor(COLOR_3DSHADOW));
		}

		rcAlbumList.left   = nSpace;
		rcAlbumList.top    = m_rcBtnUp.bottom+nSpace;
		rcAlbumList.right  = nCx-nSpace;
		rcAlbumList.bottom = m_rcBtnDown.top-nSpace;
	}
	else
	{		
		m_nOriginIndex = 0;

		rcAlbumList.left = nSpace;
		rcAlbumList.top  = nSpace;
		rcAlbumList.right = nCx-nSpace;
		rcAlbumList.bottom = nCy-nSpace;
	}

	CRgn rgnAlbum;
	rgnAlbum.CreateRectRgn(rcAlbumList.left,rcAlbumList.top,rcAlbumList.right,rcAlbumList.bottom);
	dcMem.SelectObject(&rgnAlbum);

	for ( int n=m_nOriginIndex; n<nCount; n++ )
	{
		CRect & rcItem = m_vecItem[n].rcItem;

		rcItem.left   = rcAlbumList.left;
		rcItem.right  = rcAlbumList.right;
		rcItem.top    = (n-m_nOriginIndex)*(ITEM_HEIGHT+nTextHeight)+rcAlbumList.top;
		rcItem.bottom = rcItem.top+ITEM_HEIGHT+nTextHeight;

		CResBitmap bmp;
		bmp.LoadBitmap(m_vecItem[n].nBitmap);

		int nLeft = (nCx-ITEM_WIDTH)/2;
		DrawBitmap(&dcMem,nLeft,rcItem.top+2,&bmp,RGB(0,255,0));

		dcMem.SetTextColor(RGB(0,0,0));

		if ( n==m_nCurSel )
		{
			dcMem.SetTextColor(RGB(0,0,255));
			dcMem.Draw3dRect(rcItem,GetSysColor(COLOR_3DSHADOW),GetSysColor(COLOR_3DLIGHT));
		}
		else
		if ( n==m_nHotItem )
		{
			dcMem.Draw3dRect(rcItem,GetSysColor(COLOR_3DLIGHT),GetSysColor(COLOR_3DSHADOW));
		}

		dcMem.TextOut(nCx/2,rcItem.top+ITEM_HEIGHT,m_vecItem[n].strItemText);
	}

	CRgn rgnClient;
	rgnClient.CreateRectRgn(0,0,nCx,nCy);
	dcMem.SelectObject(&rgnClient);

	dcMem.Draw3dRect(&rc,GetSysColor(COLOR_3DSHADOW),GetSysColor(COLOR_3DLIGHT));
	rc.InflateRect(-1,-1);
	dcMem.Draw3dRect(&rc,GetSysColor(COLOR_3DDKSHADOW),GetSysColor(COLOR_3DHILIGHT));
	
	dc.BitBlt(0,0,nCx,nCy,&dcMem,0,0,SRCCOPY);

	dcMem.RestoreDC(nSaveDC);
}

void CAlbumFolder::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		CPoint pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);

		int nHotItem = HitTest(pt);
		if ( nHotItem==-1 )
		{
			m_nHotItem=-1;
			KillTimer(0x01);
			Invalidate();
		}
	}

	if ( nIDEvent==0x02 && m_bMouseDown )
	{
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		KillTimer(0x02);

		int nSel = HitTest(point);

		if ( nSel==-2 )
		{
			m_nOriginIndex--;
			if ( m_nOriginIndex<0 )
				m_nOriginIndex=0;
		}

		if ( nSel==-3 )
		{
			m_nOriginIndex++;
			if ( m_nOriginIndex>(m_vecItem.size()-1) )
			{
				m_nOriginIndex--;
			}
		}

		if ( nSel!=m_nCurSel )
		{
			if ( nSel>-1 )
				m_nCurSel=nSel;
		}

		if ( m_bMouseDown )
			SetTimer(0x02,300,NULL);
		Invalidate();
	}

	CButtonBase::OnTimer(nIDEvent);
}

void CAlbumFolder::OnMouseMove(UINT nFlags, CPoint point) 
{
	int nHotItem = HitTest(point);

	if ( nHotItem!=m_nHotItem )
	{
		if ( m_nHotItem==-1 )
		{
			SetTimer(0x01,100,NULL);
		}
		m_nHotItem = nHotItem;
		Invalidate();
	}

	CButtonBase::OnMouseMove(nFlags, point);
}

void CAlbumFolder::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bMouseDown = true;

	SetTimer(0x02,350,NULL);

	int nSel = HitTest(point);

	if ( nSel==-2 )
	{
		m_nOriginIndex--;
		if ( m_nOriginIndex<0 )
			m_nOriginIndex=0;
	}

	if ( nSel==-3 )
	{
		m_nOriginIndex++;
		if ( m_nOriginIndex>(m_vecItem.size()-1) )
		{
			m_nOriginIndex--;
		}
	}

	if ( nSel!=m_nCurSel )
	{
		if ( nSel>-1 )
			m_nCurSel=nSel;
	}

	Invalidate();

	CButtonBase::OnLButtonDown(nFlags,point);
}

void CAlbumFolder::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bMouseDown = false;
	KillTimer(0x02);

	Invalidate();

	CButtonBase::OnLButtonUp(nFlags,point);
}