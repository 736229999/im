#include "stdafx.h"
#include "MenuEx.h"


IMPLEMENT_DYNAMIC( CMenuEx, CMenu )

#define SAFE_DEL(x)   {if(x) delete (x);(x)=NULL;}

#define MENU_ICON_WIDTH   20
#define MENU_TITLE_WIDTH  20

// 用来表示哪些MENU需要显示 TITLE BAR
int CMenuEx::s_nTitleGroup[10] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1}; 
int CMenuEx::s_nTitleIndex = 0;

CBitmap			CMenuEx::m_bmpTitle;
BOOL			CMenuEx::m_bDrawLeft;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CMenuEx::CMenuEx()
{
	m_bPopupMenu=FALSE;					// initialize this menu flag
										// FALSE indicate this menu is top-level menu
										// TRUE means this menu is popupmenu

	m_nSeparator = 10;					// separator menu item 's height

	// initialize color attribute213 235 245
	m_crBackground = RGB(255,255,255);	
	m_crText = RGB(0,84,166);
	m_crTextSelected = 0;
	m_crLeft= RGB(203,235,248);//RGB(36,165,241);	//RGB(255,235,211);//RGB(236,233,216);
	m_crSelectedBroder=RGB(125,167,217);//RGB(201,140,44);
	m_crSelectedFill=RGB(213,235,245);//RGB(254,233,198);//

	// no normal imagelist
	m_pImageList         = NULL;					
	m_pDisabledImageList = NULL;			
	m_pHotImageList      = NULL;		
	
	m_bDrawTitle = FALSE;
	m_bDrawLeft  = TRUE;
	// default menu image size
	m_szImage=CSize(20,20);				
}
/////////////////////////////////////////////////////////////////////////////
CMenuEx::~CMenuEx()
{	
	// destroy menu and free all variable
	DestroyMenu();	
}
/////////////////////////////////////////////////////////////////////////////

BOOL CMenuEx::DestroyMenu()
{	
	// delete all imagelist
	if(m_pImageList!=NULL)
		m_pImageList->DeleteImageList();

	if(m_pDisabledImageList!=NULL)
		m_pDisabledImageList->DeleteImageList();

	if(m_pHotImageList!=NULL)
		m_pHotImageList->DeleteImageList();		

	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		MENUITEM *pMenuItem = m_MenuItemArr.GetAt(i);
		
		// destroy submenu
		if(pMenuItem->bIsSubMenu)
		{
			pMenuItem->pSubMenu->DestroyMenu(); 		
			delete pMenuItem->pSubMenu;
		}

		// remove menu item from array
		m_MenuItemArr.RemoveAt(i);--i;
		
		// free menuitem struct
		delete pMenuItem;
	}	
	m_MenuItemArr.RemoveAll();

	for(int i =0;i<10;i++)
		s_nTitleGroup[i] = -1; 
	s_nTitleIndex = 0;

	return CMenu::DestroyMenu();
}
/////////////////////////////////////////////////////////////////////////////
BOOL CMenuEx::LoadMenu(UINT nIDResource)
{
	// load menu from menu editor created resouce
	CMenu Menu;
	Menu.LoadMenu(nIDResource);

	// empty all variable
	if(m_hMenu!=NULL)	
		DestroyMenu();	
		
	if(m_bPopupMenu)
	{
		// create popupmenu
		if(!CreatePopupMenu()) 
			return FALSE;	
	}
	else
	{
		// create top-level menu  
		if(!CreateMenu()) 
			return FALSE; 
	}		
	return LoadMenu(Menu);
}
/////////////////////////////////////////////////////////////////////////////
BOOL CMenuEx::LoadMenu(HMENU hMenu)
{
	// load menu from global HMENU handle
	CMenu Menu;
	Menu.Attach(hMenu);
	
	// empty all variable
	if(m_hMenu!=NULL)	
		DestroyMenu();	
	
	if(m_bPopupMenu)
	{
		// create popupmenu
		if(!CreatePopupMenu()) 
			return FALSE;	
	}
	else
	{
		// create top-level menu  
		if(!CreateMenu()) 
			return FALSE; 
	}	
	return LoadMenu(Menu);
}
/////////////////////////////////////////////////////////////////////////////
BOOL CMenuEx::LoadMenu(LPCTSTR lpszResourceName)
{
	// load menu from resource string
	CMenu Menu;
	Menu.LoadMenu(lpszResourceName);
	
	// empty all variable
	if(m_hMenu!=NULL)	
		DestroyMenu();	
	
	if(m_bPopupMenu)
	{
		// create popupmenu
		if(!CreatePopupMenu()) 
			return FALSE;	
	}
	else
	{
		// create top-level menu  
		if(!CreateMenu()) 
			return FALSE; 
	}	
	return LoadMenu(Menu);
}
/////////////////////////////////////////////////////////////////////////////
BOOL CMenuEx::LoadMenu(CMenu& Menu)
{	
	UINT nID;
	UINT nIndex;	
	UINT nState;
	CString strMenu;

	// fill this menu use temp menu
	INT SS = Menu.GetMenuItemCount();
	for(nIndex = 0; nIndex < Menu.GetMenuItemCount(); nIndex++)
	{		
		nID=Menu.GetMenuItemID(nIndex);					// get menu ID
		nState=Menu.GetMenuState(nIndex,MF_BYPOSITION);	// get menu state
		Menu.GetMenuString(nIndex,strMenu,MF_BYPOSITION);// get menu name
		
		if(nID==0)
		{	
			// insert separator
			if(!AppendMenu(MF_SEPARATOR))
				return FALSE;
		}
		else if(nID==-1)
		{
			// bypass popupmenu's item number	
			nState&=0x00FFL;	
			
			// insert popupmenu
			if(!AppendMenu(MF_POPUP|nState,(UINT)Menu.GetSubMenu(nIndex)->Detach(),strMenu))
				return FALSE;
		}
		else
		{		
			// insert normal menu item
			if(!AppendMenu(MF_STRING|nState,nID,strMenu))
				return FALSE;
		}
	}		
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////
BOOL CMenuEx::AppendMenu(UINT nFlags, UINT nIDNewItem, LPCTSTR lpszNewItem )
{	
	// append ownerdrawn style menu item to this menu
	// get last position of this menu
	UINT nIndex=m_MenuItemArr.GetSize();
	
	// insert menu item to last position
	// means append menu item
	return InsertMenu(nIndex,nFlags|MF_BYPOSITION,nIDNewItem,lpszNewItem);	
}

/////////////////////////////////////////////////////////////////////////////
BOOL CMenuEx::InsertMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem , LPCTSTR lpszNewItem )
{
	// insert menu item to specific position or command ID
	LPMENUITEM pMenuItem;
	UINT nIndex;
	CSize  size;	

	// mainframe's device context,use to get text size
	CDC	 *pDC =AfxGetMainWnd()->GetDC();

	if(nFlags & MF_BYPOSITION)
	{
		// insert menu item by position
		pMenuItem=new MENUITEM;

		if(nFlags&MF_SEPARATOR)
		{
			// insert separator
			pMenuItem->strText = "";						// no name
			pMenuItem->nID = 0;								// separator's ID
			pMenuItem->itemSize=CSize(0,m_nSeparator);		// separator's default size
			pMenuItem->pImageList=NULL;						// separator has no image
			pMenuItem->pDisabledImageList=NULL;
			pMenuItem->pHotImageList=NULL;
			pMenuItem->nImageIndex=-1;
			pMenuItem->bIsSubMenu=FALSE;					// separator must not be popupmenu
			pMenuItem->pSubMenu=NULL;				
			pMenuItem->bDrawTitle = m_bDrawTitle;
			// insert menu item to this menu
			if(!CMenu::InsertMenu(nPosition,nFlags,nIDNewItem,lpszNewItem))
			{
				SAFE_DEL(pMenuItem);
				AfxGetMainWnd()->ReleaseDC(pDC);
				return FALSE;
			}
			
			if(m_bPopupMenu)
			{			
				// modify menu item to ownerdrawn style
				// if this menu is not a top-level menu
				if(!CMenu::ModifyMenu(nPosition,MF_SEPARATOR|MF_BYPOSITION|MF_OWNERDRAW,0,(LPCTSTR)pMenuItem))
				{
					SAFE_DEL(pMenuItem);
					AfxGetMainWnd()->ReleaseDC(pDC);
					return FALSE;
				}
			}			
			// insert menu item to array
			m_MenuItemArr.InsertAt(nPosition,pMenuItem);
		}
		else if(nFlags&MF_POPUP)
		{
			// insert popupmenu
			CMenuEx* pSubMenu=new CMenuEx;
			//TRACE("in SDF\n");
			// ====================== 判断是否要画 TITLE BAR ====================
			for(int i = 0;i<10;i++)
			{
				if(s_nTitleGroup[s_nTitleIndex] == 1)
				{
					pSubMenu->SetDrawTitle(TRUE);
					TRACE("SDF\n");
					break;
				}
			}
			s_nTitleIndex++;
			// ==================================================================
			// create new popupmenu
			pSubMenu->SetPopupMenu(TRUE);

			// load popupmenu from specific handle
			pSubMenu->LoadMenu((HMENU)nIDNewItem);
			size=pDC->GetTextExtent(lpszNewItem);

			pMenuItem->strText = lpszNewItem;
			pMenuItem->nID = -1;	

			// specified menu item size,do not include image's size
			pMenuItem->itemSize.cx=size.cx+25;
			pMenuItem->itemSize.cy=2;
			
			// specified all imagelist for new menu item
			pMenuItem->pImageList=m_pImageList;		
			pMenuItem->pDisabledImageList=m_pDisabledImageList;
			pMenuItem->pHotImageList=m_pHotImageList;
			pMenuItem->nImageIndex=-1;
			pMenuItem->bIsSubMenu=TRUE;
			pMenuItem->pSubMenu=pSubMenu;		// popupmenu's pointor		
			pMenuItem->bDrawTitle = m_bDrawTitle;
			
			if(!CMenu::InsertMenu(nPosition,nFlags,(UINT)(pSubMenu->m_hMenu),lpszNewItem))
			{
				SAFE_DEL(pSubMenu);
				SAFE_DEL(pMenuItem);
				AfxGetMainWnd()->ReleaseDC(pDC);
				return FALSE;
			}
			
			if(m_bPopupMenu)
			{			
				if(!CMenu::ModifyMenu(nPosition,nFlags|MF_POPUP|MF_BYPOSITION|MF_OWNERDRAW,(UINT)(pSubMenu->m_hMenu),(LPCTSTR)pMenuItem))
				{
					SAFE_DEL(pSubMenu);
					SAFE_DEL(pMenuItem);
					AfxGetMainWnd()->ReleaseDC(pDC);
					return FALSE;
				}
			}			
			m_MenuItemArr.InsertAt(nPosition,pMenuItem);
		}
		else // if(nFlags & MF_STRING)
		{
			// insert normal menu item
			size=pDC->GetTextExtent(lpszNewItem);

			pMenuItem->strText = lpszNewItem;
			pMenuItem->nID = nIDNewItem;			
			pMenuItem->itemSize.cx=size.cx+10;
			pMenuItem->itemSize.cy=2;		
			pMenuItem->pImageList=m_pImageList;		
			pMenuItem->pDisabledImageList=m_pDisabledImageList;
			pMenuItem->pHotImageList=m_pHotImageList;
			pMenuItem->nImageIndex=-1;
			pMenuItem->bIsSubMenu=FALSE;
			pMenuItem->pSubMenu=NULL;
			pMenuItem->bDrawTitle = m_bDrawTitle;
			
			if(!CMenu::InsertMenu(nPosition,nFlags,nIDNewItem,lpszNewItem))
			{
				SAFE_DEL(pMenuItem);
				AfxGetMainWnd()->ReleaseDC(pDC);
				return FALSE;
			}			
			if(m_bPopupMenu)
			{			
				if(!CMenu::ModifyMenu(nPosition,MF_BYPOSITION|MF_OWNERDRAW,nIDNewItem,(LPCTSTR)pMenuItem))
				{
					SAFE_DEL(pMenuItem);
					AfxGetMainWnd()->ReleaseDC(pDC);
					return FALSE;
				}
			}
			m_MenuItemArr.InsertAt(nPosition,pMenuItem);
		}	
		
		AfxGetMainWnd()->ReleaseDC(pDC);
		return TRUE;
	}
	else if(nFlags & MF_BYCOMMAND)		// insert menu item by command ID
	{		
		for(nIndex = 0; nIndex < (UINT)m_MenuItemArr.GetSize(); nIndex++)
		{
			pMenuItem=m_MenuItemArr.GetAt(nIndex);
			
			// insert menu item with position
			// if the position match command ID
			if(pMenuItem->nID == nPosition)
			{
				AfxGetMainWnd()->ReleaseDC(pDC);
				return InsertMenu(nIndex,MF_BYPOSITION,nIDNewItem,lpszNewItem);					
			}
		}

		// do not found menu item in this menu
		CMenuEx* pSubMenu=FindPopupMenuFromID(nPosition);

		if(pSubMenu==NULL)
		{
			// not found 
			AfxGetMainWnd()->ReleaseDC(pDC);
			return FALSE;
		}
		else
		{
			// insert menu item to submenu
			AfxGetMainWnd()->ReleaseDC(pDC);
			return pSubMenu->InsertMenu(nPosition,nFlags,nIDNewItem ,lpszNewItem);
		}		
	}	

	AfxGetMainWnd()->ReleaseDC(pDC);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////

BOOL CMenuEx::RemoveMenu(UINT nPosition, UINT nFlags)
{	
	UINT nIndex;
	LPMENUITEM pMenuItem;

	// remove menu item by position
	if(nFlags & MF_BYPOSITION)
	{		
		// position out of range of menu item array
		if(nPosition>(UINT)(m_MenuItemArr.GetSize()-1))
			return FALSE;

		if(!CMenu::RemoveMenu(nPosition,nFlags))
			return FALSE;
		
		pMenuItem=m_MenuItemArr.GetAt(nPosition);
		// remove menu item from menu item array
		m_MenuItemArr.RemoveAt(nPosition);
				
		delete pMenuItem;
		// remvoe menu item from this menu		
		return TRUE;
	}
	else if(nFlags & MF_BYCOMMAND)
	{
		// remove menu item by command ID
		for(nIndex = 0; nIndex < (UINT)m_MenuItemArr.GetSize(); nIndex++)
		{
			pMenuItem = m_MenuItemArr.GetAt(nIndex);
			
			// remove menu item by position match command ID
			if(pMenuItem->nID == nPosition)			
				return RemoveMenu(nIndex,MF_BYPOSITION);						
		}	
		
		// found menu item from submenu of this menu
		CMenuEx* pSubMenu=FindPopupMenuFromID(nPosition);		
		if(pSubMenu==NULL)					
			return FALSE;		// not found
		else
			return pSubMenu->RemoveMenu(nIndex,MF_BYPOSITION);	// remove menu item from submenu		
	}
	
	return FALSE;	
}
/////////////////////////////////////////////////////////////////////////////
BOOL CMenuEx::ModifyMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem, LPCTSTR lpszNewItem)
{
	UINT nIndex;
	LPMENUITEM pMenuItem;
	CDC	 *pDC =AfxGetMainWnd()->GetDC();	
	CSize  size;

	// modify menu item by position
	if(nFlags & MF_BYPOSITION)
	{
		if(nPosition>(UINT)(m_MenuItemArr.GetSize()-1))
		{
			AfxGetMainWnd()->ReleaseDC(pDC);
			return FALSE;
		}

		if(!CMenu::ModifyMenu(nPosition,nFlags,nIDNewItem,lpszNewItem))
		{	
			// fail to modify menu
			AfxGetMainWnd()->ReleaseDC(pDC);
			return FALSE;
		}				
		pMenuItem=GetMenuItem(nPosition);
		
		if(pMenuItem==NULL)
		{
			AfxGetMainWnd()->ReleaseDC(pDC);
			return FALSE;
		}

		if(nFlags&MF_SEPARATOR)
		{
			// modify menu item to separator
			pMenuItem->strText = "";
			pMenuItem->nID = 0;		
			pMenuItem->itemSize=CSize(0,m_nSeparator);
			pMenuItem->pImageList=m_pImageList;		
			pMenuItem->pDisabledImageList=m_pDisabledImageList;
			pMenuItem->pHotImageList=m_pHotImageList;
			pMenuItem->nImageIndex=-1;
			pMenuItem->bIsSubMenu=FALSE;
			pMenuItem->pSubMenu=NULL;	
			pMenuItem->bDrawTitle = m_bDrawTitle;//m_bDrawTitle;
		}
		else if(nFlags&MF_POPUP)
		{
			// modify menu item to popupmenu item
			CMenuEx* pSubMenu=new CMenuEx;
			TRACE("in ModifyMenu\n");
			pSubMenu->SetPopupMenu(TRUE);
			pSubMenu->LoadMenu((HMENU)nIDNewItem);

			pMenuItem->strText = lpszNewItem;
			pMenuItem->nID = -1;
			size=pDC->GetTextExtent(lpszNewItem);
			pMenuItem->itemSize.cx=size.cx+25;
			pMenuItem->itemSize.cy=2;		
			pMenuItem->pImageList=m_pImageList;		
			pMenuItem->pDisabledImageList=m_pDisabledImageList;
			pMenuItem->pHotImageList=m_pHotImageList;
			pMenuItem->nImageIndex=-1;
			pMenuItem->bIsSubMenu=TRUE;
			pMenuItem->pSubMenu=pSubMenu;	
			pMenuItem->bDrawTitle = m_bDrawTitle;
		}
		else // MF_STRING
		{
			// modify menu item to normal menu item
			pMenuItem->strText = lpszNewItem;
			pMenuItem->nID = nIDNewItem;

			size=pDC->GetTextExtent(lpszNewItem);
			pMenuItem->itemSize.cx=size.cx+10;
			pMenuItem->itemSize.cy=2;	
			pMenuItem->pImageList=m_pImageList;		
			pMenuItem->pDisabledImageList=m_pDisabledImageList;
			pMenuItem->pHotImageList=m_pHotImageList;			
			pMenuItem->nImageIndex=-1;
			pMenuItem->bIsSubMenu=FALSE;
			pMenuItem->pSubMenu=NULL;
			pMenuItem->bDrawTitle = m_bDrawTitle;
		}			
		return TRUE;		
	}
	else if(nFlags & MF_BYCOMMAND)
	{
		// modify menu item by command ID
		for(nIndex = 0; nIndex < (UINT)m_MenuItemArr.GetSize(); nIndex++)
		{
			pMenuItem=m_MenuItemArr.GetAt(nIndex);	
			
			// modify ID matched menu item by position
			if(pMenuItem->nID == nPosition)	
			{
				AfxGetMainWnd()->ReleaseDC(pDC);
				return ModifyMenu(nIndex,MF_BYPOSITION,nIDNewItem,lpszNewItem);							
			}
		}
		
		// walk through all items, looking for ID matched popupmenu
		CMenuEx* pSubMenu=FindPopupMenuFromID(nPosition);
		
		if(pSubMenu==NULL)		
		{
			AfxGetMainWnd()->ReleaseDC(pDC);
			return FALSE;		// not found
		}
		else		
		{
			AfxGetMainWnd()->ReleaseDC(pDC);
			return pSubMenu->ModifyMenu(nPosition,nFlags,nIDNewItem,lpszNewItem);			
		}
	}		
	
	AfxGetMainWnd()->ReleaseDC(pDC);
	return FALSE;
}
/////////////////////////////////////////////////////////////////////////////
CMenuEx* CMenuEx::GetSubMenu(int nPosition)
{
	// get submenu by position based zero
	// return NULL if do not exist
	if((UINT)nPosition>(m_MenuItemArr.GetSize()-1))
		return NULL;
	
	return m_MenuItemArr.GetAt(nPosition)->pSubMenu;
}
/////////////////////////////////////////////////////////////////////////////
CMenuEx* CMenuEx::FindPopupMenuFromID(UINT nID)
{
	ASSERT_VALID(this);
	
	// walk through all items, looking for ID match
	UINT nItems = GetMenuItemSize();
	for (int iItem = 0; iItem < (int)nItems; iItem++)
	{
		CMenuEx* pPopup = GetSubMenu(iItem);
		if (pPopup != NULL)
		{
			// recurse to child popup
			pPopup = FindPopupMenuFromID(nID);
			// check popups on this popup
			if (pPopup != NULL)
				return pPopup;
		}
		else if (GetMenuItemID(iItem) == nID)
		{
			// it is a normal item inside this popup			
			return this;
		}
	}
	// not found
	return NULL;
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	// chech this is ownerdrawn menu or not
	if(lpDIS->CtlType!=ODT_MENU)
		return;	

	// drawn device context for draw menu
	CDC dc;
	dc.Attach(lpDIS->hDC);

	LPMENUITEM lpMenuItem=(LPMENUITEM)lpDIS->itemData;

	// draw background of menu item
	DrawBackground(&dc,lpDIS->rcItem);	
	
	// draw selected 	
	DrawSelected(&dc,lpDIS->rcItem,lpDIS);		
	
	// draw menu text
	DrawMenuText(&dc,lpDIS->rcItem,lpDIS);
	
	//draw menu imagelist
	DrawMenuImage(&dc,lpDIS->rcItem,lpDIS);		

	dc.Detach();
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::DrawBackground(CDC* pDC,CRect rect)
{
	rect.InflateRect(0,1);
	CRect  rcLeft=rect;	

	pDC->FillSolidRect(rect,m_crBackground);

	if (m_bDrawLeft)
	{
		rcLeft.right = rcLeft.left+m_szImage.cx+5;	

		if( m_bDrawTitle ) 
		{
			rcLeft.left += MENU_TITLE_WIDTH;
			pDC->FillSolidRect(rcLeft,m_crBackground);
		}
		else
		{
			pDC->FillSolidRect(rcLeft,m_crLeft);
		}
	}

	// draw mtitle
	if( m_bDrawTitle && m_bmpTitle.m_hObject )
	{
		CRect rcClip;
		pDC->GetClipBox(rcClip);
		BITMAP bm;
		m_bmpTitle.GetBitmap(&bm);

		CDC  mDC;mDC.CreateCompatibleDC(pDC);
		CBitmap* old = mDC.SelectObject(&m_bmpTitle);
		pDC->BitBlt(0,rect.top,MENU_TITLE_WIDTH,rect.Height(),&mDC,0,bm.bmHeight-(rcClip.bottom - rect.top),SRCCOPY);

		mDC.SelectObject(old);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::DrawMenuImage(CDC *pDC, CRect rect,LPDRAWITEMSTRUCT lpDIS)
{
	CRect rcImage=rect;
	rcImage.InflateRect(0,1);
	LPMENUITEM lpMenuItem=(LPMENUITEM)lpDIS->itemData;
	
	rcImage.left+=1;
	if( m_bDrawTitle ) rcImage.left+= MENU_TITLE_WIDTH;

	rcImage.right=rcImage.left+m_szImage.cx;

	rcImage.top+=1;
	rcImage.bottom=rcImage.top+m_szImage.cy;	

	// draw imagelist of menu item
	// if menu item has image(nImageIndex!=-1)
	if(lpMenuItem->nImageIndex !=-1)
	{			
		CImageList* pImageList=lpMenuItem->pImageList;			

		if((lpDIS->itemState&ODS_GRAYED)&&(lpMenuItem->pDisabledImageList!=NULL))
		{
			// draw disabled image
			pImageList=lpMenuItem->pDisabledImageList;
		}
		else if((lpDIS->itemState&ODS_SELECTED)&&(lpMenuItem->pHotImageList!=NULL))
		{	
			// draw selected image
			pImageList=lpMenuItem->pHotImageList;
		}							
			
		// draw NO.nImageIndex image of imagelist
		pImageList->Draw(pDC,lpMenuItem->nImageIndex,rcImage.TopLeft(),ILD_TRANSPARENT);		
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::DrawMenuText(CDC *pDC, CRect rect,LPDRAWITEMSTRUCT lpDIS)
{
	CRect rcText=rect;
	CFont* pOldFont = pDC->GetCurrentFont();
	LPMENUITEM lpMenuItem=(LPMENUITEM)lpDIS->itemData;
	if( m_bDrawTitle ) 
		rcText.left+= MENU_TITLE_WIDTH;
	
	if( m_bDrawLeft==FALSE )
		rcText.left-= MENU_TITLE_WIDTH;

	rcText.left = rcText.left+m_szImage.cx+2+8;	

	//this->GetMenuItemInfo(
	// not use background color when draw text
	pDC->SetBkMode(TRANSPARENT);
	
	if(lpMenuItem->nID==0)
	{
		// draw separator
		rcText.top =rcText.Height()/2+rcText.top-1;
		rcText.bottom =rcText.top +1;
		rcText.right-=3;

		if ( m_bDrawLeft && m_bDrawTitle )
			rcText.left-=MENU_TITLE_WIDTH;

		pDC->FillSolidRect(rcText,0xD8856B);//RGB(197,194,184));		
	}
	else
	{	
		// calculate text position
		CSize size=pDC->GetTextExtent(lpMenuItem->strText);	
		rcText.top=rcText.top+(rcText.Height()-size.cy)/2;

		// draw "√" instead at end
		if(lpDIS->itemState & ODS_CHECKED)
		{	
			CString check("√");
			
			CSize szCheck=pDC->GetTextExtent(check);

			CRect rcCheck=rect;
			rcCheck.left = rect.right-szCheck.cx-2;		

			if(lpDIS->itemState & ODS_GRAYED)
			{	
				// draw grayed "√"
				pDC->DrawState(rcCheck.TopLeft(),szCheck,check, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
			}
			else
			{	
				// set text color for different text style
				if(lpDIS->itemState & ODS_SELECTED)			
					pDC->SetTextColor(m_crTextSelected);			
				else			
					pDC->SetTextColor(m_crText);
				
				// draw normal "√"
				pDC->DrawText(check,rcCheck,DT_EXPANDTABS|DT_VCENTER|DT_SINGLELINE);
			}		
		//	pDC->SetBkMode(OPAQUE);
		}	


		if( (lpDIS->itemState & ODS_GRAYED) || (lpDIS->itemState & ODS_DISABLED) )
		{	
			// draw grayed text
			pDC->DrawState(rcText.TopLeft(), rcText.Size(), lpMenuItem->strText, DSS_DISABLED, TRUE, 0, (HBRUSH)NULL);
		}
		else
		{
			// set text color
			if(lpDIS->itemState & ODS_SELECTED)			
				pDC->SetTextColor(m_crTextSelected);			
			else			
				pDC->SetTextColor(m_crText);				
			
			// draw normal text
			pDC->DrawText(lpMenuItem->strText,rcText,DT_LEFT | DT_EXPANDTABS | DT_VCENTER);
		}
	}	

	pDC->SelectObject(pOldFont);
	pDC->SetBkMode(OPAQUE);
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::DrawSelected(CDC *pDC, CRect rect,LPDRAWITEMSTRUCT lpDIS)
{
	CRect rcText=rect;
	if( m_bDrawTitle ) 
		rcText.left+= MENU_TITLE_WIDTH;

	LPMENUITEM lpMenuItem=(LPMENUITEM)lpDIS->itemData;

	// separator,grayed menu item,disabled menu item
	// can not be selected
	if((lpMenuItem->nID==0)||(lpDIS->itemState & ODS_GRAYED)||(lpDIS->itemState & ODS_DISABLED))
		return;	
	// draw selected recatangle use special color
	// if current menu item is selected
	if(lpDIS->itemState & ODS_SELECTED )
	{
		CPen* pBorderPen=new CPen(PS_SOLID,1,m_crSelectedBroder);
		CBrush* pFillBrush=new CBrush(m_crSelectedFill);		
		CPen* pOldPen= pDC->SelectObject(pBorderPen);
		CBrush* pOldBrush= pDC->SelectObject(pFillBrush);
		
		CRect rcBack = rcText;
		rcBack.InflateRect(0,0);

		pDC->Rectangle(rcBack);		// draw selected rectangle

		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);

		delete pBorderPen;
		delete pFillBrush;
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::SetImageList(CImageList* pImageList)
{
	if(pImageList==m_pImageList)
		return;		

	IMAGEINFO info;
	LPMENUITEM lpMenuItem;	
	m_pImageList =pImageList;
	
	// get imagelist size
	pImageList->GetImageInfo(0,&info);	
	m_szImage.cx=info.rcImage.right-info.rcImage.left;	
	m_szImage.cy=info.rcImage.bottom-info.rcImage.top;

	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);

		// set normal imagelist for menu item
		lpMenuItem->pImageList=pImageList;
		
		// set submenu imagelist
		if(lpMenuItem->bIsSubMenu)		
			lpMenuItem->pSubMenu->SetImageList(pImageList);				
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::SetDisabledImageList(CImageList* pImageList)
{
	if(pImageList==m_pDisabledImageList)
		return;
	
	LPMENUITEM lpMenuItem;
	// update disabled imagelist
	m_pDisabledImageList =pImageList;
	
	if(m_pImageList==NULL&&m_pHotImageList==NULL)
	{		
		IMAGEINFO info;
		
		// only set disabled imagelist
		// get imagelist size
		pImageList->GetImageInfo(0,&info);		
		m_szImage.cx=info.rcImage.right-info.rcImage.left;	
		m_szImage.cy=info.rcImage.bottom-info.rcImage.top;
	}
	
	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);
		lpMenuItem->pDisabledImageList=pImageList;
		
		// set submenu disabled imagelis
		if(lpMenuItem->bIsSubMenu)		
			lpMenuItem->pSubMenu->SetDisabledImageList(pImageList);
				
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::SetHotImageList(CImageList* pImageList)
{
	if(pImageList==m_pHotImageList)
		return;
	
	LPMENUITEM lpMenuItem;
	m_pHotImageList =pImageList;
	
	if(m_pImageList==NULL&&m_pDisabledImageList==NULL)
	{		
		IMAGEINFO info;		

		// only hot imagelist is set
		// get imagelist size
		pImageList->GetImageInfo(0,&info);			
		m_szImage.cx=info.rcImage.right-info.rcImage.left;	
		m_szImage.cy=info.rcImage.bottom-info.rcImage.top;
	}

	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);
		lpMenuItem->pHotImageList=pImageList;
		
		// set submenu hot imagelis
		if(lpMenuItem->bIsSubMenu)		
			lpMenuItem->pSubMenu->SetHotImageList(pImageList);		
	}	
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::SetImageIndex(const UINT* nIDResource,UINT nIDCount)
{
	// set imagelist index from specific table "nIDResource[nIDCount]"
	// only can SetImageIndex after set one imagelist
	if(m_pImageList==NULL&&m_pHotImageList==NULL&&m_pDisabledImageList==NULL)
		return;

	LPMENUITEM lpMenuItem;
	for(UINT nIndex=0;nIndex<(UINT)m_MenuItemArr.GetSize();nIndex++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(nIndex);

		// call submenu 's loadtoolbar
		if(lpMenuItem->bIsSubMenu)		
			lpMenuItem->pSubMenu->SetImageIndex(nIDResource,nIDCount);		
		else
		{
			for(UINT i=0;i<nIDCount;i++)
			{
				// if table's commandID equal menu item's command ID
				// set nImageIndex to table's index
				if(lpMenuItem->nID==nIDResource[i])
					lpMenuItem->nImageIndex=i;
			}
		}
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::LoadToolBar(const CToolBar* pToolBar)
{
	
	// load all imagelist from pToolBar
	SetImageList(pToolBar->GetToolBarCtrl().GetImageList());
	SetDisabledImageList(pToolBar->GetToolBarCtrl().GetDisabledImageList());
	SetHotImageList(pToolBar->GetToolBarCtrl().GetHotImageList());
	
	// pToolBar don't have imagelist
	if(m_pImageList==NULL&&m_pDisabledImageList==NULL&&m_pHotImageList==NULL)
		return;

	LPMENUITEM pItem;

	for(UINT nIndex=0;nIndex<(UINT)m_MenuItemArr.GetSize();nIndex++)
	{
		pItem=m_MenuItemArr.GetAt(nIndex);		
		
		// load submenu toolbar
		if(pItem->bIsSubMenu)		
			pItem->pSubMenu->LoadToolBar(pToolBar);		
		else
		{	
			for(UINT i=0;i<(UINT)pToolBar->GetToolBarCtrl().GetButtonCount();i++)
			{				 
				if(pItem->nID==pToolBar->GetItemID(i))
				{
					// if toolbar's commandID equal menu item's command ID
					// set nImageIndex to toolbar's index
					pItem->nImageIndex=i;

					// set all imagelist to toolbar's imagelist
					pItem->pImageList=m_pImageList;
					pItem->pDisabledImageList=m_pDisabledImageList;
					pItem->pHotImageList=m_pHotImageList;						
				}
			}
		}		
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::SetTextColor(COLORREF color)
{
	LPMENUITEM lpMenuItem;
	// set normal menu text color
	m_crText =color;
	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);
		
		// set submenu text color
		if(lpMenuItem->bIsSubMenu)
			lpMenuItem->pSubMenu->SetTextColor(color);
	}
}
/////////////////////////////////////////////////////////////////////////////

void CMenuEx::SetBackgroundColor(COLORREF color)
{
	LPMENUITEM lpMenuItem;
	// set menu background color
	m_crBackground =color;
	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);
		if(lpMenuItem->bIsSubMenu)
			lpMenuItem->pSubMenu->SetBackgroundColor(color);
	}
}
/////////////////////////////////////////////////////////////////////////////

void CMenuEx::SetTextSelectedColor(COLORREF color)
{
	LPMENUITEM lpMenuItem;
	// set selected menu text color
	m_crTextSelected =color;
	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);
		if(lpMenuItem->bIsSubMenu)
			lpMenuItem->pSubMenu->SetTextSelectedColor(color);	
	}
}
/////////////////////////////////////////////////////////////////////////////

void CMenuEx::SetLeftColor(COLORREF color)
{
	LPMENUITEM lpMenuItem;
	// set menu left side color
	m_crLeft =color;
	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);
		if(lpMenuItem->bIsSubMenu)
			lpMenuItem->pSubMenu->SetLeftColor(color);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::SetSelectedBroderColor(COLORREF color)
{
	LPMENUITEM lpMenuItem;
	// set selected rectangle border color
	m_crSelectedBroder =color;
	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);
		if(lpMenuItem->bIsSubMenu)
			lpMenuItem->pSubMenu->SetSelectedBroderColor(color);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::SetSelectedFillColor(COLORREF color)
{
	LPMENUITEM lpMenuItem;
	// set selected rectangle fill color
	m_crSelectedFill =color;
	for(int i = 0; i < m_MenuItemArr.GetSize(); i++)
	{
		lpMenuItem=m_MenuItemArr.GetAt(i);
		if(lpMenuItem->bIsSubMenu)
			lpMenuItem->pSubMenu->SetSelectedFillColor(color);
	}
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::SetPopupMenu(BOOL bPopupMenu)
{
	// set top-level menu indicator	
	m_bPopupMenu=bPopupMenu;
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::UpdatePopupMenu()
{	
	// top-level menu do not need update popupmenu
	if(!m_bPopupMenu)
		return;
	
	LPMENUITEM pMenuItem;
	UINT nID,nState,nIndex;
	CSize  size;
	CString strMenu;
	UINT nSize=GetMenuItemCount();	
	CDC	 *pDC =AfxGetMainWnd()->GetDC();

	for(nIndex=0;nIndex<nSize;nIndex++)
	{
		nID=GetMenuItemID(nIndex);					// get menu ID
		nState=GetMenuState(nIndex,MF_BYPOSITION);	// get menu state
		GetMenuString(nIndex,strMenu,MF_BYPOSITION);// get menu name
		pMenuItem=GetMenuItem(nIndex);	
		
		if((pMenuItem==NULL)||(pMenuItem->nID!=nID))
		{
			pMenuItem=new MENUITEM;			
			
			if(nID==0)
			{	
				// insert separator
				pMenuItem->strText = "";						// no name
				pMenuItem->nID = 0;								// separator's ID
				pMenuItem->itemSize=CSize(0,m_nSeparator);		// separator's default size
				pMenuItem->pImageList=NULL;						// separator has no image
				pMenuItem->pDisabledImageList=NULL;
				pMenuItem->pHotImageList=NULL;
				pMenuItem->nImageIndex=-1;
				pMenuItem->bIsSubMenu=FALSE;					// separator must not be popupmenu
				pMenuItem->pSubMenu=NULL;
				pMenuItem->bDrawTitle = m_bDrawTitle;


				// modify menu item to ownerdrawn style
				// if this menu is not a top-level menu
				CMenu::ModifyMenu(nIndex,MF_SEPARATOR|MF_BYPOSITION|MF_OWNERDRAW,0,(LPCTSTR)pMenuItem);
				
				// insert menu item to array
				m_MenuItemArr.InsertAt(nIndex,pMenuItem);					
			}
			else if(nID==-1)
			{
				// bypass popupmenu's item number	
				nState&=0x00FFL;	
				// insert popupmenu
				CMenuEx* pSubMenu=new CMenuEx;	
				TRACE("in UpdatePopupMenu\n");
				// create new popupmenu
				pSubMenu->SetPopupMenu(TRUE);
				
				// load popupmenu from specific handle
				pSubMenu->LoadMenu((HMENU)(CMenu::GetSubMenu(nIndex)->Detach()));
				size=pDC->GetTextExtent(strMenu);
				
				pMenuItem->strText = strMenu;
				pMenuItem->nID = -1;					
				// specified menu item size,do not include image's size
				pMenuItem->itemSize.cx=size.cx+25;
				pMenuItem->itemSize.cy=2;				
				pMenuItem->pImageList=m_pImageList;		
				pMenuItem->pDisabledImageList=m_pDisabledImageList;
				pMenuItem->pHotImageList=m_pHotImageList;
				pMenuItem->nImageIndex=-1;
				pMenuItem->bIsSubMenu=TRUE;
				pMenuItem->pSubMenu=pSubMenu;		// popupmenu's pointor
				pMenuItem->bDrawTitle = m_bDrawTitle;
				
				CMenu::ModifyMenu(nIndex,nState|MF_POPUP|MF_BYPOSITION|MF_OWNERDRAW,(UINT)(pSubMenu->m_hMenu),(LPCTSTR)pMenuItem);
				
				m_MenuItemArr.InsertAt(nIndex,pMenuItem);
			}
			else
			{					
				// insert normal menu item
				size=pDC->GetTextExtent(strMenu);
				
				pMenuItem->strText = strMenu;
				pMenuItem->nID = nID;			
				pMenuItem->itemSize.cx=size.cx+10;
				pMenuItem->itemSize.cy=2;		
				pMenuItem->pImageList=m_pImageList;		
				pMenuItem->pDisabledImageList=m_pDisabledImageList;
				pMenuItem->pHotImageList=m_pHotImageList;
				pMenuItem->nImageIndex=-1;
				pMenuItem->bIsSubMenu=FALSE;
				pMenuItem->pSubMenu=NULL;
				pMenuItem->bDrawTitle = m_bDrawTitle;
				
				CMenu::ModifyMenu(nIndex,nState|MF_BYPOSITION|MF_OWNERDRAW,nID,(LPCTSTR)pMenuItem);
				
				m_MenuItemArr.InsertAt(nIndex,pMenuItem);					
			}							
		}
		else if((nID!=-1)&&((pMenuItem->strText==strMenu)||!(nState & MF_OWNERDRAW)))
		{	
			// system don't auto insert popup menu item
		    pMenuItem->strText=strMenu;
			CMenu::ModifyMenu(nIndex,nState|MF_BYPOSITION|MF_OWNERDRAW,nID,(LPCTSTR)pMenuItem);
		}
	}
	
	for(nIndex=nSize;nIndex<GetMenuItemSize();nIndex++)
	{
		pMenuItem=m_MenuItemArr.GetAt(nIndex);
		m_MenuItemArr.RemoveAt(nIndex);
		delete pMenuItem;
	}	

	AfxGetMainWnd()->ReleaseDC(pDC);	
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::InitPopupMenu(CMenu* pSubMenu,UINT nIndex,BOOL bSystem)
{
	//update pSubMenu's item to owerdrawn style
	if(bSystem)
		return;

	if(!pSubMenu->IsKindOf(RUNTIME_CLASS(CMenuEx)))
		return;
	
	((CMenuEx*)pSubMenu)->UpdatePopupMenu();	
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::MeasureItem(LPMEASUREITEMSTRUCT  lpMIS)
{
	// check measure ownerdraw menu or other controler
	if(lpMIS->CtlType!=ODT_MENU)		
		return;

	MENUITEM *lpMenuItem =(LPMENUITEM)lpMIS->itemData;
	
	// initialize width and height with itemSize
	lpMIS->itemWidth=lpMenuItem->itemSize.cx;
	if( lpMenuItem->bDrawTitle ) 
		lpMIS->itemWidth += MENU_TITLE_WIDTH;

	lpMIS->itemHeight=lpMenuItem->itemSize.cy;

	// not separator,may be popupmenu or normal menu item
	if(lpMenuItem->nID!=0)
	{		
		IMAGEINFO info;

		if(lpMenuItem->pImageList!=NULL)
		{	
			// first add normal imagelist size
			lpMenuItem->pImageList->GetImageInfo(0,&info);				
			lpMIS->itemWidth+=(info.rcImage.right-info.rcImage.left);
			lpMIS->itemHeight+=(info.rcImage.bottom-info.rcImage.top);			
		}
		else if(lpMenuItem->pDisabledImageList!=NULL)
		{
			// or add disabled imagelist size
			lpMenuItem->pDisabledImageList->GetImageInfo(0,&info);				
			lpMIS->itemWidth+=(info.rcImage.right-info.rcImage.left);
			lpMIS->itemHeight+=(info.rcImage.bottom-info.rcImage.top);
		}
		else if(lpMenuItem->pHotImageList!=NULL)
		{
			// or add hot imagelist size
			lpMenuItem->pHotImageList->GetImageInfo(0,&info);				
			lpMIS->itemWidth+=(info.rcImage.right-info.rcImage.left);
			lpMIS->itemHeight+=(info.rcImage.bottom-info.rcImage.top);
		}
		else
		{		
			// add default size,if do not have imagelist
			if ( m_bDrawLeft )
				lpMIS->itemWidth+=40;
			else
				lpMIS->itemWidth+=20;

			lpMIS->itemHeight+=20;
		}		
	}		
}
/////////////////////////////////////////////////////////////////////////////
UINT  CMenuEx::GetMenuItemSize() const 
{
	// get menu item counter
	return m_MenuItemArr.GetSize();
}
/////////////////////////////////////////////////////////////////////////////
LPMENUITEM CMenuEx::GetMenuItem(UINT nPosition)
{
	// get menu item by position
	if(nPosition>(UINT)m_MenuItemArr.GetUpperBound())
		return NULL;

	return m_MenuItemArr.GetAt(nPosition);
}
/////////////////////////////////////////////////////////////////////////////
void CMenuEx::SetTitleImage(DWORD hInst,UINT nBitmap)
{
	if ( m_bmpTitle.GetSafeHandle() )
		m_bmpTitle.DeleteObject();

	HBITMAP hBitmap = ::LoadBitmap((HINSTANCE)hInst,MAKEINTRESOURCE(nBitmap));
	
	if ( hBitmap )
	{
		m_bmpTitle.Attach(hBitmap);
	}
}
