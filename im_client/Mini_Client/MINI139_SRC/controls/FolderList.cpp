// FolderList.cpp : implementation file
//

#include "stdafx.h"
#include "FolderList.h"
#include "..\resource.h"
#include ".\folderlist.h"

#include "resgdiobj.h"

#include "..\im\impublic.h"
/////////////////////////////////////////////////////////////////////////////
// CFolderList

CFolderList::CFolderList()
{
	m_NormalFont.CreateFont(12, 
		0,
		0, 
		90,
		FW_NORMAL,
		0,
		0,
		0,
		DEFAULT_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY,
		(DEFAULT_PITCH|FF_MODERN),
		"宋体");

	m_BoldFont.CreateFont(12, 
		0,
		0, 
		90,
		FW_BOLD,
		0,
		0,
		0,
		DEFAULT_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY,
		(DEFAULT_PITCH|FF_MODERN),
		"宋体");

	m_nItemHeight=20;
	m_nCurSel = -1;
	m_dwStyle = 0 ;

	m_bmpCheck.LoadBitmap(IDB_CHECK);
	m_bmpUnCheck.LoadBitmap(IDB_UNCHECK);

	m_pImageList  = NULL;
	m_nItemIndent = 0;
	m_nClientRealHeigh = 0;
	m_nOriginY         = 0;
}

CFolderList::~CFolderList()
{
	ClearAll();
}


BEGIN_MESSAGE_MAP(CFolderList, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFolderList message handlers
int CFolderList::AddItem(CString strItem, int iFolder, bool bCheck, int iImageIndex,bool bTail)
{
	tagItem *item     = new tagItem;
	item->strText     = strItem;
	item->isChecked   = bCheck;
	item->iImageIndex = iImageIndex;

	m_vecItem.push_back(item);

	if ( bTail )
		m_vecFolder[iFolder]->vecItem.push_back(item);	
	else
		m_vecFolder[iFolder]->vecItem.insert(m_vecFolder[iFolder]->vecItem.begin(),item);

	CalcScrollInfo();

	if ( m_hWnd )
	{
		Invalidate();
	}

	return m_vecItem.size()-1;
}

CString CFolderList::GetItemText(int nIndex)
{
	return m_vecItem[nIndex]->strText;
}

bool CFolderList::GetItemCheck(int nIndex)
{
	return m_vecItem[nIndex]->isChecked;
}

DWORD CFolderList::GetItemData(int nIndex)
{
	return m_vecItem[nIndex]->dwData;
}

void CFolderList::SetItemImage(int nIndex,int iImageIndex)
{
	m_vecItem[nIndex]->iImageIndex = iImageIndex;
	if ( m_hWnd )
		Invalidate();
}

void CFolderList::SetItemCheck(int nIndex, bool bCheck)
{
	m_vecItem[nIndex]->isChecked = bCheck;
	if ( m_hWnd )
		Invalidate();
}

void CFolderList::SetItemData(int nIndex, DWORD dwData)
{
	m_vecItem[nIndex]->dwData=dwData;
}

BOOL CFolderList::OnEraseBkgnd(CDC* pDC) 
{	
	return FALSE;
}

void CFolderList::DrawFolder(const tagFolder* folder,CDC *pDC,const CRect rc)
{
	CResBitmap bmpBar;

	CDC *pFolderDC = GetDC();

	int iDotAjust=(m_nItemHeight-12)/2;

	bmpBar.LoadBitmap(IDB_IM_FOLDER);
	CDC memDC;
	memDC.CreateCompatibleDC(pFolderDC);
	memDC.SelectObject(&bmpBar);

	BITMAP bmpInfo;
	bmpBar.GetBitmap(&bmpInfo);

	TransparentBlt(pDC->m_hDC,rc.left,rc.top,rc.Width(),m_nItemHeight,
		memDC.m_hDC,0,0,
		bmpInfo.bmWidth,bmpInfo.bmHeight,
		RGB(0,255,0));

	CDC stateDC;
	stateDC.CreateCompatibleDC(pFolderDC);
	CResBitmap stateBmp;
	
	if 	( folder->state==FOLDER_EXPAND )
	{
		stateBmp.LoadBitmap(IDB_IM_FOLDER_EXPAND);
	}
	else
	{
		stateBmp.LoadBitmap(IDB_IM_FOLDER_CLOSE);
	}
	
	stateDC.SelectObject(&stateBmp);
		
	BITMAP statebmpInfo;
	
	stateBmp.GetBitmap(&statebmpInfo);

	int x=rc.left+2;
	int y=rc.top+(m_nItemHeight-statebmpInfo.bmHeight)/2+1;
	
	TransparentBlt(pDC->m_hDC, x,y, statebmpInfo.bmWidth,
			statebmpInfo.bmHeight,stateDC.m_hDC,0,0,
			statebmpInfo.bmWidth,statebmpInfo.bmHeight,
			RGB(255,255,255));


	int nTextColor = pDC->GetTextColor();

	CFont *pOldfont = pDC->SelectObject(&m_BoldFont);
	
	pDC->SetTextColor(RGB(0,84,166));	
	pDC->SetBkMode(TRANSPARENT);

	pDC->TextOut(rc.left+15,rc.top+iDotAjust,folder->title);

	bool bCheck = m_dwStyle & FS_CHECK;
	bool bImage = m_dwStyle & FS_IMAGE;

	if (  folder->state==FOLDER_EXPAND )
	{
		for ( int ii=0; ii<folder->vecItem.size(); ii++ )
		{
			tagItem *pItem=folder->vecItem[ii];

			pItem->rcItem.left=rc.left+1;
			pItem->rcItem.top=rc.bottom+ii*m_nItemHeight;
			pItem->rcItem.right=rc.right-1;
			pItem->rcItem.bottom=pItem->rcItem.top+m_nItemHeight;

			int iItemLeft = pItem->rcItem.left+m_nItemIndent;

			pDC->SetTextColor(RGB(0,0,0));
			pDC->SelectObject(&m_NormalFont);

			if ( pItem->isMouseOver )
			{
				pDC->FillSolidRect(&pItem->rcItem,RGB(200,218,244));
				pDC->SetTextColor(RGB(21,123,217));
			}
			
			if ( pItem->isSelected )
			{
				pDC->FillSolidRect(&pItem->rcItem,RGB(64,154,220));
				pDC->SetTextColor(RGB(255,255,255));
			}

			if ( bCheck )
			{
				pItem->rcCheckBox.left   = iItemLeft;
				pItem->rcCheckBox.top    = pItem->rcItem.top+iDotAjust;
				pItem->rcCheckBox.right  = pItem->rcCheckBox.left+12;
				pItem->rcCheckBox.bottom = pItem->rcCheckBox.top+12;

				DrawBitmap(pDC,iItemLeft,pItem->rcCheckBox.top,pItem->isChecked? &m_bmpCheck : &m_bmpUnCheck,0);
				iItemLeft += 15;
			}

			if ( bImage && m_pImageList )
			{
				//this->m_pImageList->Draw(pDC,pItem->iImageIndex,CPoint(iItemLeft,pItem->rcItem.top+1),ILD_NORMAL);
				g_ImgFaceFrame.Draw(pDC->m_hDC, iItemLeft, pItem->rcItem.top+1, 18,18);
				g_ImgIcon.Draw(pDC->m_hDC,iItemLeft,pItem->rcItem.top+1, 18, 18);
				iItemLeft += 19;
			}
			
			pDC->TextOut(iItemLeft+2,pItem->rcItem.top+iDotAjust,pItem->strText);
		}

	}

	pDC->SelectObject(pOldfont);
	ReleaseDC(pFolderDC);
}

void CFolderList::SetCurSel(int nIndex)
{
	if ( m_nCurSel!=-1 && m_nCurSel!=nIndex )
		m_vecItem[m_nCurSel]->isSelected=false;

	if( nIndex>=m_vecItem.size())
		return ;

	m_vecItem[nIndex]->isSelected=true;
	m_nCurSel=nIndex;

	if ( m_hWnd )
	{
		Invalidate();
	}
}

int CFolderList::GetCurSel()
{
	return m_nCurSel;
}

void CFolderList::PreSubclassWindow() 
{
	SetFont(&m_NormalFont);

	CDC *pDC = GetDC();
	CFont *pOld=pDC->SelectObject(&m_NormalFont);
	m_nTextHeight=pDC->GetTextExtent("Hello").cy;
	pDC->SelectObject(pOld);
	pDC->DeleteDC();

	RecalcLayout();

	CWnd::PreSubclassWindow();
}

void CFolderList::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CPoint pt=point;
	pt.y+=m_nOriginY;

	for ( int iFolder=0; iFolder<m_vecFolder.size(); iFolder++ )
	{
		tagFolder *pFolder=m_vecFolder[iFolder];

		if ( pFolder->rcBar.PtInRect(pt))
		{
			::SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(32649)));
			pFolder->isMouseOver=true;
		}
		else
		{
			pFolder->isMouseOver=false;
		}

		if ( pFolder->state==FOLDER_EXPAND )
		{
			for ( int i=0; i<pFolder->vecItem.size(); i++ )
			{
				tagItem *pItem=pFolder->vecItem[i];

				if ( pItem->rcItem.PtInRect(pt) )
				{
					pItem->isMouseOver=true;
				}
				else
				{
					pItem->isMouseOver=false;
				}
			}
		}
	}


	Invalidate();

	SetTimer(1,10,NULL);
	
	CWnd::OnMouseMove(nFlags, point);
}

void CFolderList::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CPoint pt = point;
	pt.y+=m_nOriginY;

	for ( int iFolder=0; iFolder<m_vecFolder.size(); iFolder++ )
	{
		tagFolder *pFolder=m_vecFolder[iFolder];
		
		if ( pFolder->rcBar.PtInRect(pt) )
		{
			if ( pFolder->state==FOLDER_EXPAND )
			{
				pFolder->state=FOLDER_CLOSE;
			}
			else
			{
				pFolder->state=FOLDER_EXPAND;
			}
		}

		if ( pFolder->state==FOLDER_EXPAND )
		{
			for ( int i=0; i<pFolder->vecItem.size(); i++ )
			{
				tagItem *pItem=pFolder->vecItem[i];
				
				if ( pItem->rcItem.PtInRect(pt) )
				{
					if ( m_nCurSel!=-1 )
					{
						m_vecItem[m_nCurSel]->isSelected=false;
					}

					if ( (m_dwStyle&FS_CHECK) && pItem->rcCheckBox.PtInRect(pt) )
					{
						pItem->isChecked = !pItem->isChecked;
					}

					pItem->isSelected=true;
					m_nCurSel=GetItemIndex(pItem);

					////////////将选中消息发送到 ListAdd列表中去处理/////////////////////////////////////
					{
						extern HWND g_hWndListAdd;
						if(0 == ::PostMessage(g_hWndListAdd,WM_ADD_FRIEND_TO_LIST,1,0/*(LPARAM)pItem)*/))
						{
							/////pItem = NULL;
						}
					}
					////////////将选中消息发送到 ListAdd列表中去处理/////////////////////////////////////
				}
				else
				{
					pItem->isMouseOver=false;
				}
			}
		}
	}

	Invalidate();
	
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CFolderList::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CRect rcWindow;
	CPoint ptCursor;

	GetWindowRect(&rcWindow);
	GetCursorPos(&ptCursor);

	if ( !rcWindow.PtInRect(ptCursor))
	{
		ScreenToClient(&ptCursor);
		OnMouseMove(0,ptCursor);
		KillTimer(1);
	}
	
	CWnd::OnTimer(nIDEvent);
}


int  CFolderList::GetItemIndex(tagItem *pItem)
{

	for ( int i=0; i<m_vecItem.size(); i++ )
	{
		if ( pItem==m_vecItem[i] )
		{
			return i;
		}
	}

	return -1;
}

BOOL CFolderList::Create(CWnd *pParentWnd,const CRect &rect, DWORD dwStyle/* =WS_VISIBLE|WS_BORDER|WS_CHILD */,UINT nID/* =0 */)
{
	return CWnd::Create( AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS),
						"xFolderList",
						dwStyle,
						rect,pParentWnd,
						nID);
}

int CFolderList::AddFolder(CString strTitle,FOLDER_STATE state /* = FOLDER_EXPAND */)
{
	tagFolder *pFolder=new tagFolder;
	pFolder->title=strTitle;
	pFolder->state = state;
	m_vecFolder.push_back(pFolder);
	CalcScrollInfo();
	return m_vecFolder.size()-1;
}

void CFolderList::ClearAll()
{	
	int i;

	for ( i=0; i<m_vecItem.size(); i++ )
	{
		delete m_vecItem[i];
	}

	m_vecItem.clear();

	for ( i=0; i<m_vecFolder.size(); i++ )
	{
		delete m_vecFolder[i];
	}
	
	m_nCurSel=-1;
	m_vecFolder.clear();	
}

void CFolderList::OnPaint()
{
	CPaintDC dc(this); 

	RecalcLayout();

	CRect rcClient,rcWindow;
	
	GetClientRect(&rcClient);
	GetWindowRect(&rcWindow);

	CDC *pDC = &dc;

	CDC memDc;
	memDc.CreateCompatibleDC(pDC);

	int nSavedDC = memDc.SaveDC();

	CBitmap memBmp;
	memBmp.CreateCompatibleBitmap(pDC,rcClient.Width(),this->m_nClientRealHeigh);
	memDc.SelectObject(&memBmp);

	CRect rcDraw;
	rcDraw.left   = rcDraw.top = 0;
	rcDraw.right  = rcClient.Width();
	rcDraw.bottom = m_nClientRealHeigh;

	CBrush brushWhite(RGB(255,255,255));
	memDc.FillRect(rcDraw,&brushWhite);

	int height = rcClient.Height();
	int width  = rcClient.Width();

	int nPreFolderHeight=0;

	for ( int iFolder=0; iFolder<m_vecFolder.size(); iFolder++ )
	{
		CRect rcFolder;

		rcFolder.left   = rcClient.left+1;
		rcFolder.top    = nPreFolderHeight;
		rcFolder.right  = rcClient.right-1;
		rcFolder.bottom = rcFolder.top+m_nItemHeight;

		tagFolder *pFolder=m_vecFolder[iFolder];	

		int nVacum=0;

		if ( pFolder->vecItem.size()>0 )
			nVacum=10;

		nPreFolderHeight = rcFolder.bottom+ (pFolder->state==FOLDER_EXPAND ? pFolder->vecItem.size()*m_nItemHeight+nVacum : 0 );
		
		pFolder->rcBar=rcFolder;
		DrawFolder(pFolder,&memDc,rcFolder); 		
	}


	pDC->BitBlt(0,0,rcClient.Width(),rcClient.Height(),&memDc,0,m_nOriginY,SRCCOPY);

	memDc.RestoreDC(nSavedDC);
}

void CFolderList::RecalcLayout()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int nPreHeight = 0;
	
	int nFolders = m_vecFolder.size();
	for ( int iFolder =0; iFolder<nFolders; iFolder++ )
	{
		int nItems = m_vecFolder[iFolder]->vecItem.size();
	
		if ( m_vecFolder[iFolder]->state==FOLDER_EXPAND )
		{
			nPreHeight += (nItems*m_nItemHeight);
		}

		nPreHeight+=m_nItemHeight;

		if ( nItems>0 )
			nPreHeight+=10;
	}

	if (m_nClientRealHeigh != nPreHeight) 
	{
		m_nClientRealHeigh = max(nPreHeight, rcClient.Height());
		CalcScrollInfo();
	}	
	
	if (m_nClientRealHeigh - m_nOriginY < rcClient.Height()) 
	{
		m_nOriginY = m_nClientRealHeigh - rcClient.Height();
	}
	
	if (m_nOriginY < 0) 
	{
		m_nOriginY = 0;
	}
}


void CFolderList::CalcScrollInfo()
{
	if (IsWindow(m_hWnd)) 
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		
		if (m_nClientRealHeigh == 0) 
		{
			m_nClientRealHeigh = rcClient.Height();
		}
		
		m_nClientRealHeigh = max(m_nClientRealHeigh, rcClient.Height());

		SCROLLINFO si;
		memset(&si, 0, sizeof(si));
		si.cbSize = sizeof(si);
		si.fMask  = SIF_PAGE | SIF_RANGE;
		si.nPage  = rcClient.Height();
		si.nMin = 0;
		si.nMax = m_nClientRealHeigh - 1;
		if (si.nPage == si.nMax) 
		{
			CWnd::ShowScrollBar(SB_VERT, FALSE);
		}
		else
		{
			CWnd::SetScrollInfo(SB_VERT, &si);
		}
	}
}

BOOL CFolderList::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	OnVScroll(zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, 0, 0);	
	return TRUE;
}

void CFolderList::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	SetFocus();

	switch (nSBCode) 
	{
	case SB_TOP:
		OnVScroll(SB_LINEUP, nPos, pScrollBar);
		break;
	case  SB_BOTTOM:
		OnVScroll(SB_LINEDOWN, nPos, pScrollBar);
		break;
	case SB_LINEUP:
		if (m_nOriginY > 0) 
		{
			SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - m_nItemHeight);
			m_nOriginY -= m_nItemHeight;
			if (m_nOriginY < 0) 
			{
				m_nOriginY = 0;
			}
			
			Invalidate();
		}
		break;
	case SB_LINEDOWN:
		if (m_nOriginY + rcClient.Height() < m_nClientRealHeigh) 
		{
			SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + m_nItemHeight);
			m_nOriginY += m_nItemHeight;
			
			Invalidate();
		}
		break;
	case SB_PAGEDOWN:
		if (m_nOriginY + rcClient.Height() < m_nClientRealHeigh) 
		{
			SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + rcClient.Height());
			m_nOriginY += rcClient.Height();

			Invalidate();
		}
		break;
	case SB_PAGEUP:
		if (m_nOriginY > 0) 
		{
			SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - rcClient.Height());
			m_nOriginY -= rcClient.Height();
			if (m_nOriginY < 0) 
			{
				m_nOriginY = 0;
			}

			Invalidate();
		}
		break;
	case SB_THUMBPOSITION:
		SetScrollPos(SB_VERT, nPos);
		m_nOriginY = m_nOriginY / rcClient.Height() + nPos;		
		Invalidate();
		break;
	case SB_THUMBTRACK:
		if (nPos < m_nClientRealHeigh) 
		{
			m_nOriginY = nPos;
			//m_nOriginY += m_nItemHeight;
			Invalidate();
		}
		
		SetScrollPos(SB_VERT, nPos);
		break;
	default:
		;
	}

	//CWnd::OnVScroll(nSBCode,nPos,pScrollBar);
}


void CFolderList::OnLButtonUp(UINT nFlags, CPoint point)
{
	GetParent()->PostMessage(WM_COMMAND,MAKEWPARAM(this->GetDlgCtrlID(),1),0);

	CWnd::OnLButtonUp(nFlags, point);
}
