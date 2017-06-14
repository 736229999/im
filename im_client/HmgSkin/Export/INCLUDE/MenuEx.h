#pragma once

#include "HmgSkinPublic.h"
class CMenuEx;

typedef struct AFX_EXT_CLASS tagMENUITEM
{
	CString		strText;			// menu item text
	UINT		nID;				// menu item command ID
	// separator's ID always be 0
	// popupmenu's ID always be -1

	CSize		itemSize;			// menu item's size,don't include image's size

	CImageList* pImageList;			// imagelist for normal menu item
	CImageList* pDisabledImageList;	// imagelist for disabled(grayed) menu item
	CImageList* pHotImageList;		// imagelist for selected(hot) menu item
	UINT		nImageIndex;		// index of imagelist,-1 means no image

	BOOL		bIsSubMenu;		// normal menu item if FALSE
	// TRUE means this item include submenu
	CMenuEx*	pSubMenu;			// always be NULL,when bIsPopupMenu is FALSE
	// if bIsPopupMenu is TRUE,contain popupmenu's pointor
	BOOL        bDrawTitle;   // 是否画竖着的标题栏

} MENUITEM,*LPMENUITEM;


///////////////////////////////////////////
class AFX_EXT_CLASS CMenuEx : public CMenu
{
	DECLARE_DYNAMIC( CMenuEx )

// Constructor
public:	
	CMenuEx();	
	virtual ~CMenuEx();
	virtual BOOL DestroyMenu();

public:
	static int s_nTitleGroup[10]; 
	static int s_nTitleIndex;

// Operation
public:
	// initialize menu operation
	BOOL LoadMenu(UINT nIDResource);
	BOOL LoadMenu(LPCTSTR lpszResourceName);
	BOOL LoadMenu(HMENU hMenu);
	BOOL LoadMenu(CMenu & Menu);

	// menu item operation
	BOOL AppendMenu(UINT nFlags, UINT nIDNewItem = 0,LPCTSTR lpszNewItem = NULL);
	BOOL InsertMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL );
	BOOL ModifyMenu(UINT nPosition, UINT nFlags, UINT nIDNewItem = 0, LPCTSTR lpszNewItem = NULL );	
	BOOL RemoveMenu(UINT nPosition, UINT nFlags);	

	// load image operation
	void SetImageIndex(const UINT* nIDResource,UINT nIDCount);
	void LoadToolBar(const CToolBar* pToolBar);

	// menu item operation
	UINT  GetMenuItemSize() const;
	LPMENUITEM GetMenuItem(UINT nPosition);	

	//submenu operation
	CMenuEx* GetSubMenu(int nPosition);
	CMenuEx* FindPopupMenuFromID(UINT nID);

// Attributes
protected:
	// top-level menu indicator
	BOOL m_bPopupMenu;
	//static BOOL m_bDrawTitle;   // 是否画竖着的标题栏
	BOOL m_bDrawTitle;   // 是否画竖着的标题栏

	static BOOL m_bDrawLeft;    // 是否画左边的条


	// separator height
	int m_nSeparator;
	
	// color for ownerdrawn
	COLORREF m_crBackground;			// menu background color	
	COLORREF m_crTextSelected;			// menu selected text color
	COLORREF m_crText;					// menu normal text color
	COLORREF m_crLeft;					// color on menu left side
	COLORREF m_crSelectedBroder;		// menu selected rectangle border color
	COLORREF m_crSelectedFill;			// menu selected rectangle fill color

	// all imagelist should be same size 	
	CSize m_szImage;

	// menu item image for ownerdrawn
	CImageList* m_pImageList;			// menu item's normal imagelist
	CImageList* m_pDisabledImageList;	// menu item's disabled imagelist
	CImageList* m_pHotImageList;		// menu item's selected imagelist

	static CBitmap     m_bmpTitle;
protected:
	// contain all menu item of this menu
	CArray<LPMENUITEM,LPMENUITEM> m_MenuItemArr;

public:
	// color attribute operation
	void SetTextSelectedColor(COLORREF color);
	void SetBackgroundColor(COLORREF color);
	void SetTextColor(COLORREF color);
	void SetLeftColor(COLORREF color);
	void SetSelectedBroderColor(COLORREF color);
	void SetSelectedFillColor(COLORREF color);
	
	// set imagelist operation
	void SetImageList(CImageList* pImageList);
	void SetDisabledImageList(CImageList* pImageList);
	void SetHotImageList(CImageList* pImageList);	

	//set 
	void SetTitleImage(DWORD hInst, UINT nBitmap);
	void SetDrawLeft(BOOL bDraw)   { m_bDrawLeft  = bDraw; }
	void SetDrawTitle(BOOL bDraw)  { m_bDrawTitle = bDraw; }
	
	// set top-level menu indicator
	void SetPopupMenu(BOOL bPopupMenu);
// Implementation
public:
	// virtual drawitem operation,call by MFC frameworks
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDIS);
	
	// update all menu item to owerdrawn style
	// because frameworks insert some non-ownerdrawn menu item some times
	void UpdatePopupMenu();

protected:
	// drawn menu item helper operation
	void DrawBackground(CDC* pDC,CRect rect);
	void DrawMenuImage(CDC* pDC,CRect rect,LPDRAWITEMSTRUCT lpDIS);
	void DrawMenuText(CDC*  pDC,CRect rect,LPDRAWITEMSTRUCT lpDIS);
	void DrawSelected(CDC*  pDC,CRect rect,LPDRAWITEMSTRUCT lpDIS);

// Static Member
public:
	// call this fuction in mainframe's OnMeasureItem() message mapping handler
	// measure all ownerdrawn menu item's size
	static void MeasureItem(LPMEASUREITEMSTRUCT lpMIS);
	
	// call this fuction in mainframe's OnInitPopupMenu() message mapping handler
	// update non-ownerdrawn to ownerdrawn style
	static void InitPopupMenu(CMenu* pPopupMenu,UINT nIndex,BOOL bSystem);

};