#include "stdafx.h"
#include <afxwin.h>
#include <afxcmn.h>

#include "impublic.h"

#pragma warning (disable: 4018)


void ShowMessage(CString strMessage,UINT nType/* =MB_OK|MB_ICONINFORMATION */);

#define ItemExistsAsserte(nGroup, nIndex) \
	_ASSERTE(nGroup >= 0 && nGroup < m_vecGroup.size() && \
		nIndex >= 0 && nIndex < m_vecGroup[nGroup]->vecItem.size());

/////////////////////////////////////////////////////////////////////////////
// CIMAddressListCtrl
#include "imaddresslistctrl.h"
#include "..\Resource.h"


CIMAddressListCtrl::CIMAddressListCtrl()
{
	m_nItemHeight = 18;

	m_clrBkGround	   = RGB(255,255,255);//0xFFD49E;//
	
	m_Font.CreateFont(12, 
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

	m_OriginY = 0;
	m_nClientRealHeigh = 0;
	m_pImageList  = NULL;

	m_hCursor = ::LoadCursor(NULL,IDC_ARROW);

	m_pTrueParent =  NULL;

	m_bmpMini.LoadBitmap(IDB_CONTACT_MINI);
	m_bmpOutlook.LoadBitmap(IDB_CONTACT_OUTLOOK);
	m_bmpOutlook2.LoadBitmap(IDB_CONTACT_OUTLOOK1);
	m_bmpSendMail.LoadBitmap(IDB_CONTACT_SENDMAIL);
	m_bMouseDown = false;
	m_bBeginDrag = false;

	m_bMouseMailDown = false;
	m_bMouseOnMail   = false;
}

CIMAddressListCtrl::~CIMAddressListCtrl()
{
	DeleteAllItems();
}

BEGIN_MESSAGE_MAP(CIMAddressListCtrl, CIMListCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIMAddressListCtrl message handlers
BOOL CIMAddressListCtrl::Create(CWnd *pParentWnd,const CRect &rect, DWORD dwStyle/* =WS_VISIBLE|WS_BORDER|WS_CHILD */,UINT nID/* =0 */)
{
	m_pTrueParent = pParentWnd;
	return CIMListCtrl::Create( AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW),
						"xTreamTreeClass",
						dwStyle,
						rect,pParentWnd,
						nID);
}

void CIMAddressListCtrl::ClearGroup(int nGroup)
{
	int count = m_vecGroup[nGroup]->vecItem.size();
	for (int i=0;i<count; i++ )
	{
		delete m_vecGroup[nGroup]->vecItem[i];
	}

	m_vecGroup[nGroup]->vecItem.clear();
	InvalidateList();
}

void CIMAddressListCtrl::DelGroup(int nGroup)
{
	if (nGroup>-1 && nGroup<m_vecGroup.size())
	{
		int count = m_vecGroup[nGroup]->vecItem.size();
		for (int i=0;i<count; i++ )
		{
			delete m_vecGroup[nGroup]->vecItem[i];
		}
		m_vecGroup[nGroup]->vecItem.clear();

		delete m_vecGroup[nGroup];
		m_vecGroup.erase(m_vecGroup.begin()+nGroup);
	}
	InvalidateList();
}

int CIMAddressListCtrl::InsertGroup(int nGroup,CString strTitle,DWORD dwData/* =0 */)
{
	AL_GROUP_INFO * Group = new AL_GROUP_INFO();
	Group->strTitle = strTitle;
	Group->clrText  = RGB(0,0,180);
	Group->dwData   = dwData;
	Group->state    = ALS_CLOSE;

	if ( nGroup>=0 && nGroup<m_vecGroup.size() )
	{
		m_vecGroup.insert(
			m_vecGroup.begin()+nGroup,
			Group);
	}
	else
	{
        nGroup = m_vecGroup.size();
		m_vecGroup.push_back(Group);
	}

	m_nClientRealHeigh += m_nItemHeight ;

	CalcScrollInfo();

	if ( m_hWnd )
	{
		OnPaint();
	}

	return nGroup;
}

int CIMAddressListCtrl::AddGroup(CString strTitle)
{
	AL_GROUP_INFO * Group = new AL_GROUP_INFO();

	Group->strTitle=strTitle;
	Group->clrText = RGB(0,0,180);
	
	m_vecGroup.push_back(Group);

	int nGroup = m_vecGroup.size()-1;

	m_nClientRealHeigh += m_nItemHeight ;

	CalcScrollInfo();

	return nGroup;
}

void CIMAddressListCtrl::SetGroupTextColor(int nGroup, COLORREF clrText)
{
	_ASSERTE(nGroup >= 0 && nGroup < m_vecGroup.size());

	m_vecGroup[nGroup]->clrText = clrText;

	InvalidateList();
}

void CIMAddressListCtrl::SetGroupState(int nGroup, ALS_STATE state)
{
	m_vecGroup[nGroup]->state = state;

	InvalidateList();
}

void CIMAddressListCtrl::SetGroupName(int nGroup, CString strTitle)
{
	_ASSERTE(nGroup >= 0 && nGroup < m_vecGroup.size());

	strTitle.Trim();

	m_vecGroup[nGroup]->strTitle = strTitle;
	
	InvalidateList();
}

CString CIMAddressListCtrl::GetGroupName(int nGroup)
{
	_ASSERTE(nGroup >= 0 && nGroup < m_vecGroup.size());
	CString strName = m_vecGroup[nGroup]->strTitle;

	return strName;
}

void CIMAddressListCtrl::DeleteItem(int nGroup, int nItem)
{
	delete m_vecGroup[nGroup]->vecItem[nItem];

	m_vecGroup[nGroup]->vecItem.erase(
		m_vecGroup[nGroup]->vecItem.begin()
		+ nItem);

	if ( m_hWnd )
	{
		InvalidateList();
	}
}

void CIMAddressListCtrl::DeleteAllItems()
{
	for (int i = 0; i < m_vecGroup.size(); i++)
	{
		int c=m_vecGroup[i]->vecItem.size();

		for ( int j=0; j<c; j++ )
		{
			delete m_vecGroup[i]->vecItem[j];
		}

		m_vecGroup[i]->vecItem.clear();

		delete m_vecGroup[i];
	}
	
	m_vecGroup.clear();
	m_OriginY = 0;
	m_nClientRealHeigh = 0;

	m_posItemSelected.nGroup = -1;
	m_posItemSelected.nItem = -1;
	CalcScrollInfo();
	InvalidateList();
}
/*
int CIMAddressListCtrl::AddItem(int iGroup, CString strText, LPCSTR lpszBmp, DWORD dwData )
{
	_ASSERTE(iGroup >= 0 && iGroup < m_vecGroup.size());


	AL_ITEM_INFO item;	
	item.strText = strText;	
	item.strBmp = lpszBmp;	
	item.dwData = dwData;

	m_vecGroup[iGroup]->vecItem.push_back(item);
	m_nClientRealHeigh += m_nItemHeight;

	CalcScrollInfo();

	if (IsWindow(m_hWnd))
	{
		InvalidateList();
	}

	return m_vecGroup[iGroup]->vecItem.size() - 1;
}
*/

int CIMAddressListCtrl::AddItem(int iGroup, CString strText, int nImage, DWORD dwData)
{
	_ASSERTE(iGroup >= 0 && iGroup < m_vecGroup.size());

	AL_ITEM_INFO  * item = new AL_ITEM_INFO();

	item->strText = strText;	
	item->iImage  = nImage;
	item->dwData  = dwData;

	m_vecGroup[iGroup]->vecItem.push_back(item);
	m_nClientRealHeigh += m_nItemHeight;

	CalcScrollInfo();
	
	InvalidateList();

	return m_vecGroup[iGroup]->vecItem.size() - 1;
}

void CIMAddressListCtrl::SetItem(int nGroup, int nIndex, CString strText, int nImage /* = -1 */, DWORD dwData /* = 0 */)
{
	ItemExistsAsserte(nGroup, nIndex);

	AL_ITEM_INFO * Item = m_vecGroup[nGroup]->vecItem[nIndex];
	Item->strText = strText;
	Item->iImage = nImage;
	Item->dwData = dwData;
	
	InvalidateList();
}

void CIMAddressListCtrl::SwapItem(int nGroup1, int nItem1, int nGroup2, int nItem2)
{
	ItemExistsAsserte(nGroup1, nItem1);
	ItemExistsAsserte(nGroup2, nItem2);

	AL_ITEM_INFO * item = m_vecGroup[nGroup1]->vecItem[nItem1];
	m_vecGroup[nGroup1]->vecItem[nItem1] = m_vecGroup[nGroup2]->vecItem[nItem2];
	m_vecGroup[nGroup2]->vecItem[nItem2] = item;

	InvalidateList();
}

void CIMAddressListCtrl::SetItemText(int nGroup, int nIndex, CString strText)
{
	ItemExistsAsserte(nGroup, nIndex);
	m_vecGroup[nGroup]->vecItem[nIndex]->strText = strText;

	InvalidateList();
}

CString CIMAddressListCtrl::GetItemText(int nGroup, int nIndex)
{
	ItemExistsAsserte(nGroup, nIndex);
	return m_vecGroup[nGroup]->vecItem[nIndex]->strText;
}

void CIMAddressListCtrl::SetItemBitmap(int nGroup, int nIndex, CBitmap *pBitmap)
{
	ItemExistsAsserte(nGroup, nIndex);

	if ( m_vecGroup[nGroup]->vecItem[nIndex]->bitmap )
		delete m_vecGroup[nGroup]->vecItem[nIndex]->bitmap;

	m_vecGroup[nGroup]->vecItem[nIndex]->bitmap = pBitmap;	

	InvalidateList();
}

void CIMAddressListCtrl::SetItemImage(int nGroup, int nIndex, int iImage)
{
	ItemExistsAsserte(nGroup, nIndex);
	m_vecGroup[nGroup]->vecItem[nIndex]->iImage = iImage;	

	InvalidateList();
}

int CIMAddressListCtrl::GetItemImage(int nGroup, int nIndex)
{
	ItemExistsAsserte(nGroup, nIndex);
	return m_vecGroup[nGroup]->vecItem[nIndex]->iImage;
}

void CIMAddressListCtrl::SetCurSel(int nGroup,int nItem)
{
	m_posItemSelected.nGroup = nGroup;
	m_posItemSelected.nItem  = nItem;

	if ( m_vecGroup[nGroup]->state!=ALS_EXPAND )
	{
		m_vecGroup[nGroup]->state = ALS_EXPAND;
	}

    if ( m_hWnd )
	{
		OnPaint();

		CRect rcItem;

		if ( nGroup>-1 && nItem>-1 )
		{
			rcItem = m_vecGroup[nGroup]->vecItem[nItem]->rcItem;
		}

		if ( nGroup>-1 && nItem<0 )
		{
			rcItem = m_vecGroup[nGroup]->rcBar;
		}

		rcItem.top    -= m_OriginY;
		rcItem.bottom -= m_OriginY;
		CRect rcClient;
		GetClientRect(&rcClient);

		CRect rcView = rcClient;
		rcView.top    += m_nItemHeight;
		rcView.bottom -= m_nItemHeight;

		CRect rcInter ;
		rcInter.IntersectRect(rcItem,rcView);
		
		if ( rcInter.IsRectEmpty() )
		{
			int nPos  = GetScrollPos(SB_VERT);
			int nMove = 0;
			if ( rcItem.top>rcClient.bottom )
			{
				nMove= (rcItem.top-rcClient.bottom + m_nItemHeight );
				m_OriginY += nMove;
				nPos += nMove;
				
			}
			else
			{
				nMove = (rcClient.bottom - rcItem.top + m_nItemHeight );
				m_OriginY -= nMove;
				nPos-=nMove;
			}

			CIMListCtrl::SetScrollPos(SB_VERT, nPos);
			InvalidateList();
		}
	}
}

AL_ITEM_POS CIMAddressListCtrl::GetCurSel()
{
	if ( m_posItemSelected.nGroup>=m_vecGroup.size() )
		m_posItemSelected.nGroup=-1;

	if ( m_posItemSelected.nGroup!=-1 )
	{
		if ( m_posItemSelected.nItem>=m_vecGroup[m_posItemSelected.nGroup]->vecItem.size() )
			m_posItemSelected.nItem=-1;
	}

	return m_posItemSelected;
}

DWORD CIMAddressListCtrl::GetItemData(int nGroup, int nIndex)
{
	if( nGroup >= 0 && nGroup < m_vecGroup.size() && \
		nIndex >= 0 && nIndex < m_vecGroup[nGroup]->vecItem.size())
	{
		return m_vecGroup[nGroup]->vecItem[nIndex]->dwData;
	}
	return NULL;
}

void CIMAddressListCtrl::SetItemData(int nGroup, int nIndex, DWORD dwData)
{
	ItemExistsAsserte(nGroup, nIndex);

	m_vecGroup[nGroup]->vecItem[nIndex]->dwData = dwData;
}

void CIMAddressListCtrl::SetItemTextColor(int nGroup, int nIndex, COLORREF clrText)
{
	ItemExistsAsserte(nGroup, nIndex);

	FRIEND_INFO *pf=(FRIEND_INFO *)m_vecGroup[nGroup]->vecItem[nIndex]->dwData;

	m_vecGroup[nGroup]->vecItem[nIndex]->clrText = clrText;

	InvalidateList();
}

void CIMAddressListCtrl::SetImageList(CImageList * pImageList)
{
	m_pImageList = pImageList;
}

int CIMAddressListCtrl::MoveItem(int nGroupSrc, int nItemSrc, int nGroupDest)
{
	ItemExistsAsserte(nGroupSrc, nItemSrc);

	AL_ITEM_INFO * item = m_vecGroup[nGroupSrc]->vecItem[nItemSrc];
	m_vecGroup[nGroupSrc]->vecItem.erase( 
		m_vecGroup[nGroupSrc]->vecItem.begin()+nItemSrc );

	m_vecGroup[nGroupDest]->vecItem.push_back(item);
	m_vecGroup[nGroupDest]->state = ALS_EXPAND;

	m_posItemSelected.nGroup = nGroupDest;
	m_posItemSelected.nItem = m_vecGroup[nGroupDest]->vecItem.size()-1;
	
	InvalidateList();

	return m_vecGroup[nGroupDest]->vecItem.size()-1;
}

void CIMAddressListCtrl::RecalcLayout()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int nPreGroupHeight = 0;
	
	for (int iGroup = 0; iGroup < m_vecGroup.size(); iGroup++)
	{
		CRect rcGroup;
		
		rcGroup.left   = rcClient.left;
		rcGroup.top    = nPreGroupHeight ;
		rcGroup.right  = rcClient.right - 2;
		rcGroup.bottom = rcGroup.top + m_nItemHeight;
		
		nPreGroupHeight = rcGroup.bottom + (m_vecGroup[iGroup]->state == ALS_EXPAND ?
			m_vecGroup[iGroup]->vecItem.size() * m_nItemHeight : 0);
		
		m_vecGroup[iGroup]->rcBar = rcGroup;
	}
	
	if (m_nClientRealHeigh != nPreGroupHeight) 
	{
		m_nClientRealHeigh = max(nPreGroupHeight, rcClient.Height());
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

void CIMAddressListCtrl::OnPaint() 
{
	CPaintDC dc(this);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap memBmp;

	CRect rcClient;
	GetClientRect(&rcClient);

	int iBmpHeight = max( rcClient.Height(), m_nClientRealHeigh );
	rcClient.bottom = iBmpHeight;

	RecalcLayout();

	memBmp.CreateCompatibleBitmap(&dc, rcClient.Width(), iBmpHeight);
	CBitmap* pBitmap = memDC.SelectObject(&memBmp);

	CBrush brushBack(m_clrBkGround);
	memDC.FillRect(&rcClient, &brushBack);

	CFont* pFont = memDC.SelectObject(&m_Font);
	
	for (int iGroup = 0; iGroup < m_vecGroup.size(); iGroup++)
	{		
		DrawGroup(m_vecGroup[iGroup], &memDC, m_posItemSelected.nGroup == iGroup);
	}

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, m_OriginY, SRCCOPY);
	memDC.SelectObject(pBitmap);
	memDC.SelectObject(pFont);
}

void CIMAddressListCtrl::DrawGroup(AL_GROUP_INFO* Group, CDC *pDC, BOOL bSelectedGroup)
{	
	POINT ptCursor;

	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);

	ptCursor.y+=m_OriginY;

	int nSavedDC = pDC->SaveDC();

	const int nLeftMargin = 14;
	
	const int nTextTop  = ( m_nItemHeight-m_nTextHeight)/2;


	bool bDragFlag = false; 

	if ( m_bMouseDown && 
		m_posItemSelected.nGroup>-1 && 
		m_posItemSelected.nItem>-1 &&
		m_posDragTo.nGroup!=m_posItemSelected.nGroup
		)
	{
		if ( Group== m_vecGroup[m_posDragTo.nGroup] )
			bDragFlag=true;
	}

	CRect rc = Group->rcBar;

	POINT pt;
	pt.x = rc.left +2;
	pt.y = rc.top + (rc.Height() - HEAD_SZ) / 2+1;
	Group->rcButton.SetRect(pt.x,pt.y,pt.x+HEAD_SZ,pt.y+HEAD_SZ);

	m_ButtonImages.Draw(pDC, Group->state, pt, ILD_NORMAL);
	
	pDC->SetBkMode(TRANSPARENT);

	CFont* pOldFont = pDC->SelectObject(&m_Font);

	CString strGroup ;

	if ( Group->vecItem.size()>0 )
	{
		strGroup.Format("%s",Group->strTitle);
	}
	else
	{
		strGroup = Group->strTitle;
	}

	if ( bDragFlag )
	{
		pDC->SetTextColor(RGB(255,0,0));
	}
	else
	{
		pDC->SetTextColor(Group->clrText);
	}

	if ( strGroup=="" )
	{
		strGroup="-";
	}

	strGroup= strGroup+ " ("+IntToStr(Group->vecItem.size())+")";

	CSize  szGroupText =pDC->GetTextExtent(strGroup);
	CRect  rcGroupText;

	rcGroupText.left = rc.left + 18;
	rcGroupText.top  = rc.top + nTextTop-1;
	rcGroupText.bottom = rcGroupText.top+szGroupText.cy+2;
	rcGroupText.right  = rcGroupText.left+szGroupText.cx+7;

	Group->rcText = rcGroupText;
	Group->rcText.InflateRect(1,1);

	CBrush brushBack(RGB(247,247,247));
	CPen   pen(PS_SOLID,1,RGB(233,233,233));
	CBrush* pOldBrush = pDC->SelectObject(&brushBack);
	CPen* pOldPen = pDC->SelectObject(&pen);

	if ( bSelectedGroup && m_posItemSelected.nItem==-1 )
		pDC->Rectangle(rcGroupText);
		
	pDC->TextOut( rcGroupText.left+4, rcGroupText.top+1, strGroup);

	if (Group->state==ALS_EXPAND)
	{
		for ( int ii=0; ii<Group->vecItem.size(); ii++ )
		{
			AL_ITEM_INFO * pItem = Group->vecItem[ii];
			ContactPersonInfo  * pUser = (ContactPersonInfo *)pItem->dwData;
			bool bSelItem = false;

			int nItemTop = rc.bottom + ii * m_nItemHeight;
			pItem->rcItem.SetRect(rc.left + 3 ,
								  nItemTop,
								  rc.right - 2, 
								  nItemTop + m_nItemHeight);

			if ( bSelectedGroup && ii==m_posItemSelected.nItem)
			{
				pDC->Rectangle(pItem->rcItem);
			}

			//begin draw head icon
			pt.x = pItem->rcItem.left+nLeftMargin+16, pt.y = pItem->rcItem.top+3;

			CRect rcIcon(pt.x, pt.y, pt.x + HEAD_SZ, pt.y + HEAD_SZ);

			if ( pUser->nType==1 )
			{
				if ( pUser->nInvited==0 )
                    ::DrawBitmap(pDC,pt.x,pt.y,&m_bmpOutlook,RGB(0,255,0));
				else
					::DrawBitmap(pDC,pt.x,pt.y,&m_bmpOutlook2,RGB(0,255,0));
			}
			else
			if ( pUser->nType==0 )
			{
				::DrawBitmap(pDC,pt.x,pt.y,&m_bmpMini,RGB(0,255,0));
			}

			pItem->rcIcon = rcIcon;
			rcIcon.InflateRect(1,1);
			pt.x = pItem->rcItem.right-20, pt.y=pItem->rcItem.top+(m_nItemHeight-16)/2;

			pDC->SetTextColor(pItem->clrText);

			CSize  size1 =pDC->GetTextExtent(pItem->strText);
			//CSize  size2 =pDC->GetTextExtent(strStatus);

			CRect  rcItemText;

			rcItemText.left = pItem->rcItem.left + nLeftMargin + 40;

			rcItemText.top    = pItem->rcItem.top + nTextTop;
			rcItemText.bottom = rcItemText.top  + size1.cy;
			rcItemText.right  = rcItemText.left + size1.cx;
			
			if ( pUser->nType==1 )
			{
				if ( rcItemText.right>( pItem->rcItem.right-18 ) )
					rcItemText.right = pItem->rcItem.right-18;

				pItem->rcBtnSendMail.top=pItem->rcItem.top+3;
				pItem->rcBtnSendMail.left=pItem->rcItem.right-16;
				pItem->rcBtnSendMail.bottom=pItem->rcBtnSendMail.top+16;
				pItem->rcBtnSendMail.right=pItem->rcBtnSendMail.left+16;

				::DrawBitmap(pDC,pItem->rcBtnSendMail.left+1,pItem->rcBtnSendMail.top+1,&m_bmpSendMail,RGB(0,255,0));
				DrawPtInRect(ptCursor,pItem->rcBtnSendMail,pDC,0,m_bMouseMailDown);
			}
			//else
			//{
			//	if ( rcItemText.right>(pItem->rcItem.right-2) )
			//		rcItemText.right=pItem->rcItem.right-2;
			//}

			pItem->rcText = rcItemText;
			
			pDC->DrawText(pItem->strText,&rcItemText,DT_TOP|DT_LEFT|DT_END_ELLIPSIS);

			pDC->SetTextColor(RGB(247,148,28));


			//pDC->TextOut(rcItemText.right+2,rcItemText.top,strStatus);
		}

	}

	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldBrush);

	pDC->RestoreDC(nSavedDC);	
}

BOOL CIMAddressListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	/*CRect rcClient;
	//GetClientRect(&rcClient);
	//pDC->FillSolidRect(0, 0, rcClient.Width(), m_nClientRealHeigh, RGB(255, 255, 255));
	//return 1;

	BYTE r1 = GetRValue(m_clrBackBegin),
		 g1 = GetGValue(m_clrBackBegin),
		 b1 = GetBValue(m_clrBackBegin);
	
    BYTE r2 = GetRValue(m_clrBackEnd),
		 g2 = GetGValue(m_clrBackEnd),
		 b2 = GetBValue(m_clrBackEnd);
	// 渐变背景
	for (int nRow = rcClient.left; nRow <= rcClient.Height(); nRow++)
	{
		int r,g,b;
		r = r1 + (nRow * (r2-r1) / rcClient.Width());
		g = g1 + (nRow * (g2-g1) / rcClient.Width());
		b = b1 + (nRow * (b2-b1) / rcClient.Width());

		pDC->FillSolidRect(rcClient.left, nRow + m_OriginY, rcClient.Width(), 1, RGB(r, g, b));
	}
	
	//////////////////////////////////////////////////////////////////////////	
	// 立体边缘
	CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(0, m_OriginY - 1);
	pDC->LineTo(rcClient.Width(), m_OriginY - 1); // 上线
	pDC->MoveTo(0, m_OriginY - 1);
	//pDC->LineTo(0, rcClient.Height() + m_OriginY - 1); // 左线
	
	CPen pen1(PS_SOLID, 1, RGB(54, 108, 222));
	pDC->SelectObject(&pen1);
	pDC->MoveTo(1, rcClient.Height() + m_OriginY - 1);
	pDC->LineTo(rcClient.Width() - 1, rcClient.Height() + m_OriginY - 1); // 下线
	if (m_nClientRealHeigh <= rcClient.Height()) 
	{
		//pDC->LineTo(rcClient.Width() - 1, -1 + m_OriginY - 1); // 右线
	}
	
	pDC->SelectObject(pOldPen); */

	return FALSE;
}

void CIMAddressListCtrl::PreSubclassWindow() 
{
	CDC *pDC = GetDC();

	CFont *pOldfont=pDC->SelectObject(&m_Font);

	m_nTextHeight =pDC->GetTextExtent("H我").cy;

	m_nItemHeight = ( m_nTextHeight>HEAD_SZ ? m_nTextHeight+5 : (HEAD_SZ+5) );

	pDC->SelectObject(pOldfont);

	ReleaseDC(pDC);

	this->EnableToolTips(TRUE);

	CIMListCtrl::PreSubclassWindow();
}

void CIMAddressListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	static int sGroupIndex=-2, sItemIndex=-2;
	static bool IsMouseOnIcon=false;

	int nGroup, nItem;
	HitTest(point,nGroup,nItem);
	m_posDragTo.nGroup = nGroup;
	m_posDragTo.nItem  = nItem;

	if ( nGroup==-1 && nItem==-1 )
		return ;

	if ( m_bMouseDown && nGroup!=sGroupIndex || nItem!=sItemIndex )
	{
		InvalidateList();
	}

	CPoint ptReal(point.x,point.y+m_OriginY);

	bool bInvalidate = false;

	HCURSOR   hCursor = LoadCursor(NULL,IDC_ARROW);

	if ( m_bMouseDown )
	{
		if ( m_posItemSelected.nGroup>0 && m_posItemSelected.nItem>-1 )
		{
			HINSTANCE hInst   = AfxGetInstanceHandle();

			if ( (m_posDragTo.nGroup>-1 && m_posDragTo.nGroup!=m_posItemSelected.nGroup) && nGroup>0 )
			{
				hCursor = ResLoadCursor(IDC_DROP);
			}
			else
				hCursor = ResLoadCursor(IDC_NODROP);
		}

		if ( m_bBeginDrag==false && m_posItemSelected.nGroup!=nGroup || m_posItemSelected.nItem!=nItem && m_posItemSelected.nGroup>0 )
		{
			bInvalidate = true;
			m_bBeginDrag =true;

			//OnBeginDrag();
			for ( int i=0; i<m_vecGroup.size(); i++ )
			{
				m_vecGroup[i]->state_before_drag = m_vecGroup[i]->state;
				m_vecGroup[i]->state = ALS_CLOSE;
			}
		}
	}

	CString sTip;

	if ( nItem>-1)
	{
		AL_ITEM_INFO *pItem = m_vecGroup[nGroup]->vecItem[nItem];

		bool bMouseOnMail = pItem->rcBtnSendMail.PtInRect(ptReal);

		if ( bMouseOnMail )
		{
			if ( m_bMouseOnMail==false )
			{			
				bInvalidate    = true;
				m_bMouseOnMail = true;
			}

			sTip = "点击发送邀请邮件";
		}
		else
		{
			if ( m_bMouseOnMail )
			{
				bInvalidate=true;
				m_bMouseOnMail=false;
			}
			
			sTip = "";
		}
	}

	sGroupIndex = nGroup;
	sItemIndex  = nItem;

	if ( bInvalidate )
		InvalidateList();

	SetCursor(hCursor);

	ShowToolTip(sTip);

	CIMListCtrl::OnMouseMove(nFlags,point);
}

void CIMAddressListCtrl::InvalidateItem(int nGroup, int nIndex, BOOL bErase /* = TRUE */)
{
	_ASSERTE(nGroup >= 0);
	
	CRect rcItem = (nIndex >= 0 ? m_vecGroup[nGroup]->vecItem[nIndex]->rcItem : m_vecGroup[nGroup]->rcBar);
	rcItem.OffsetRect(0, -m_OriginY);
	
	CRect rcClient;
	GetClientRect(&rcClient);
	if (rcItem.top >= rcClient.top || rcItem.bottom <= rcClient.bottom) 
	{
		InvalidateRect(&rcItem, bErase);
	}
}

void CIMAddressListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	int nHotGroup, nHotItem;
	HitTest(point, nHotGroup, nHotItem);

	m_posItemSelected.nGroup = nHotGroup;
	m_posItemSelected.nItem  = nHotItem;

	InvalidateList();


	::PostMessage( m_pTrueParent->m_hWnd,
		WM_CLUSTER_LIST_NOTIFY,
		NM_AL_RBUTTONDOWN,
		MAKEWORD(nHotGroup,nHotItem));

	CIMListCtrl::OnRButtonDown(nFlags, point);
}

void CIMAddressListCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	int nHotGroup, nHotItem;
	HitTest(point, nHotGroup, nHotItem);

	::PostMessage( m_pTrueParent->m_hWnd,
		WM_CLUSTER_LIST_NOTIFY,
		NM_AL_RBUTTONUP,
		MAKEWORD(nHotGroup,nHotItem));

	m_bMouseDown=false;

	CIMListCtrl::OnRButtonUp(nFlags, point);
}

void CIMAddressListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	int nHotGroup, nHotItem;
	HitTest(point, nHotGroup, nHotItem);

	m_bMouseDown = true;

	if ( nHotGroup>-1 && nHotItem>-1 )
	{
		AL_ITEM_INFO *pItem = this->m_vecGroup[nHotGroup]->vecItem[nHotItem];
		CPoint ptReal = point;
		ptReal.y += m_OriginY;
		m_bMouseMailDown  = pItem->rcBtnSendMail.PtInRect(ptReal);
	}

	if ( m_bMouseMailDown ) 
	{
		m_bMouseDown=false;
	}
	else
	{
		m_posItemSelected.nGroup = nHotGroup;
		m_posItemSelected.nItem  = nHotItem;
		m_bMouseDown = true;
	
		SetTimer(0x0F,20,NULL);
	}

	InvalidateList();
	SetFocus();

	::PostMessage( m_pTrueParent->m_hWnd,
				   WM_CLUSTER_LIST_NOTIFY,
				   NM_AL_LBUTTONDOWN,
				   MAKEWORD(nHotGroup,nHotItem));

	CIMListCtrl::OnLButtonDown(nFlags, point);
}

void CIMAddressListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int nHotGroup, nHotItem;
	HitTest(point, nHotGroup, nHotItem);

	AL_ITEM_POS posHot;
	posHot.nGroup = nHotGroup;
	posHot.nItem  = nHotItem;

	CPoint ptReal = point;
	ptReal.y += m_OriginY;

	SetCursor(LoadCursor(NULL,IDC_ARROW));

	m_bMouseDown =false;

	if ( nHotItem>-1 && nHotGroup>-1 )
	{
		AL_ITEM_INFO *pItem = this->m_vecGroup[nHotGroup]->vecItem[nHotItem];


		bool bMailClick = pItem->rcBtnSendMail.PtInRect(ptReal);

		if (  bMailClick )
		{
			if ( bMailClick && m_bMouseMailDown )
			{
				::SendMessage( GetParent()->m_hWnd,
					WM_CLUSTER_LIST_NOTIFY,
					NM_AL_MAIL_CLICK,
					(LPARAM)&posHot);

				InvalidateList();
				goto END;
			}
		}
	}


	if ( m_bBeginDrag )
	{
		m_bBeginDrag = false;
		for ( int i=0; i<m_vecGroup.size(); i++ )
		{
			m_vecGroup[i]->state = m_vecGroup[i]->state_before_drag;
		}
	}

	if ( nHotItem==m_posItemSelected.nItem && nHotGroup==m_posItemSelected.nGroup )
	{
		if (nHotItem == -1 && nHotGroup >= 0) // 点击了分组栏，需要收放分组成员
		{
			m_vecGroup[nHotGroup]->state = m_vecGroup[nHotGroup]->state == ALS_EXPAND ? ALS_CLOSE : ALS_EXPAND;
			InvalidateList();
		}
	}
	else
	{
		m_posDragTo.nItem  = nHotItem;
		m_posDragTo.nGroup = nHotGroup;

		if ( nHotGroup>-1 && 
			m_posItemSelected.nGroup>-1 &&
			m_posItemSelected.nItem>-1 )
		{
			::PostMessage( m_pTrueParent->m_hWnd,
				WM_CLUSTER_LIST_NOTIFY,
				NM_AL_DRAGDROP,
				MAKELPARAM(nHotGroup,nHotItem));
			InvalidateList();
		}

	}

	::PostMessage( m_pTrueParent->m_hWnd,
		WM_CLUSTER_LIST_NOTIFY,
		NM_AL_LBUTTONUP,
		MAKEWORD(nHotGroup,nHotItem));

END:
	{
		m_bMouseMailDown  = false;;
	}

	CIMListCtrl::OnLButtonUp(nFlags, point);
}

void CIMAddressListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int nHotGroup, nHotItem;
	HitTest(point, nHotGroup, nHotItem);

	m_posItemSelected.nGroup = nHotGroup;
	m_posItemSelected.nItem  = nHotItem;

	InvalidateList();


	::PostMessage( m_pTrueParent->m_hWnd,
		WM_CLUSTER_LIST_NOTIFY,
		NM_AL_LBUTTONDBLCLICK,
		MAKEWORD(nHotGroup,nHotItem));

	CIMListCtrl::OnLButtonDblClk(nFlags,point);
}

void CIMAddressListCtrl::HitTest(const POINT& pt, int& nGroup, int& nItem)
{
	nGroup = nItem = -1;
	CPoint point(pt);
	point.y += m_OriginY;
	
	for (int iGroup = 0; iGroup < m_vecGroup.size(); iGroup++)
	{
		AL_GROUP_INFO *pGroup= m_vecGroup[iGroup];
		
		if (pGroup->rcBar.PtInRect(point))
		{
			nGroup = iGroup;
			return;
		}
		
		if (pGroup->state == ALS_EXPAND)
		{
			for (int i = 0; i < pGroup->vecItem.size(); i++)
			{
				AL_ITEM_INFO *pItem= pGroup->vecItem[i];
				
				if (pItem->rcItem.PtInRect(point))
				{
					nGroup = iGroup;
					nItem = i;
					return ;
				}
			}
		}
	}
#if 0
	if ( nItem==-1 && nGroup==-1)
	{
		CRect rc=m_vecGroup[0]->vecItem[0]->rcItem;
		TRACE("(%d,%d,%d,%d),(%d,%d)\n",rc.left,
			rc.top,
			rc.right,
			rc.bottom,
			point.x,
			point.y);
	}
#endif

}

int CIMAddressListCtrl::GetItemCount(int nGroup)
{
	return 	m_vecGroup[nGroup]->vecItem.size();
}

void CIMAddressListCtrl::SetGroupData(int nGroup,DWORD dwData)
{
	m_vecGroup[nGroup]->dwData=dwData;
}

DWORD CIMAddressListCtrl::GetGroupData(int nGroup)
{
	return 	m_vecGroup[nGroup]->dwData;
}

int CIMAddressListCtrl::GetGroupCount()
{
	return m_vecGroup.size();
}

BOOL CIMAddressListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	OnVScroll(zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, 0, 0);	
	return TRUE;
}

void CIMAddressListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
		if (m_OriginY > 0) 
		{
			CIMListCtrl::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - m_nItemHeight);
			m_OriginY -= m_nItemHeight;
			if (m_OriginY < 0) 
			{
				m_OriginY = 0;
			}
			
			InvalidateList();
		}
		break;
	case SB_LINEDOWN:
		if (m_OriginY + rcClient.Height() < m_nClientRealHeigh) 
		{
			CIMListCtrl::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + m_nItemHeight);
			m_OriginY += m_nItemHeight;
			
			InvalidateList();
		}
		break;
	case SB_PAGEDOWN:
		if (m_OriginY + rcClient.Height() < m_nClientRealHeigh) 
		{
			CIMListCtrl::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + rcClient.Height());
			m_OriginY += rcClient.Height();

			InvalidateList();
		}
		break;
	case SB_PAGEUP:
		if (m_OriginY > 0) 
		{
			CIMListCtrl::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - rcClient.Height());
			m_OriginY -= rcClient.Height();
			if (m_OriginY < 0) 
			{
				m_OriginY = 0;
			}

			InvalidateList();
		}
		break;
	case SB_THUMBPOSITION:
		CIMListCtrl::SetScrollPos(SB_VERT, nPos);
		m_OriginY = m_OriginY / rcClient.Height() + nPos;		
		InvalidateList();
		break;
	case SB_THUMBTRACK:
		if (nPos < m_nClientRealHeigh) 
		{
			m_OriginY = nPos;
			//m_OriginY += m_nItemHeight;
			InvalidateList();
		}
		
		CIMListCtrl::SetScrollPos(SB_VERT, nPos);
		break;
	default:
		;
	}
	
	//CIMListCtrl::ShowScrollBar(SB_VERT, FALSE);
}

int CIMAddressListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	m_ButtonImages.Create(12, 12, ILC_MASK | ILC_COLOR24, 2, 1);
	HBITMAP hbmp = ResLoadBitmap(IDB_GROUPLIST_BUTTON);//LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GROUPLIST_BUTTON));
	ImageList_AddMasked(m_ButtonImages.m_hImageList, hbmp, RGB(0, 255, 0));
	DeleteObject((HGDIOBJ)hbmp);	
	CalcScrollInfo();

	return 0;
}

void CIMAddressListCtrl::CalcScrollInfo()
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
			ShowScrollBar(SB_VERT, FALSE);
		}
		else
		{
			CIMListCtrl::SetScrollInfo(SB_VERT, &si);
		}
	}
}

void CIMAddressListCtrl::OnSize(UINT nType, int cx, int cy)
{
	if (nType != SIZE_MINIMIZED && nType != SIZE_MAXHIDE) 
	{
		CalcScrollInfo();
	}
}

void CIMAddressListCtrl::InvalidateList()
{
	if ( IsWindow(m_hWnd) )
	{
		Invalidate();
	}
}

void CIMAddressListCtrl::OnTimer(UINT nIDEvent)
{
	CRect rect;
	GetClientRect(&rect);
	ClientToScreen(&rect);

	CPoint pt;
	GetCursorPos(&pt);

	if ( !rect.PtInRect(pt) )
	{
		m_bMouseDown     = false;
		m_bMouseOnMail   = false;
		m_bMouseMailDown = false;

		m_hCursor = LoadCursor(NULL,IDC_ARROW);
		InvalidateList();

		KillTimer(0x0F);
	}

	if ( m_bMouseDown==false )
	{
		KillTimer(0x0F);
	}

	CIMListCtrl::OnTimer(nIDEvent);
}

void CIMAddressListCtrl::InitToolTip()
{
	CIMListCtrl::InitToolTip();
}