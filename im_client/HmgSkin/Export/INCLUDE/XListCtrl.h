// XListCtrl.h  Version 1.3
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This software is released into the public domain.
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#ifndef XLISTCTRL_H
#define XLISTCTRL_H

#include "XHeaderCtrl.h"
#include  "ButtonEx.h"

extern AFX_EXT_CLASS UINT NEAR WM_XLISTCTRL_COMBO_SELECTION;
extern AFX_EXT_CLASS UINT NEAR WM_XLISTCTRL_CHECKBOX_CLICKED;


#include <map>
using namespace std;

typedef map<int,CButtonEx*>button_map;
#define  USE_TOPINDEX_BUTTON
///////////////////////////////////////////////////////////////////////////////
// Style bits for use with SetExtendedStyleX()

// none defined yet

///////////////////////////////////////////////////////////////////////////////
// CXListCtrl data

struct AFX_EXT_CLASS XLISTCTRLDATA
{
	// ctor
	XLISTCTRLDATA()
	{
		bEnabled             = TRUE;
		crText               = ::GetSysColor(COLOR_WINDOWTEXT);
		crBackground         = ::GetSysColor(COLOR_WINDOW);
		bShowProgress        = FALSE;
		nProgressPercent     = 0;
		strProgressMessage   = _T("");
		bShowProgressMessage = TRUE;
		nCheckedState        = -1;
		bBold                = FALSE;
		nImage               = -1;
		strToolTip           = _T("");
		bCombo               = FALSE;
		bComboIsClicked      = FALSE;
		nComboListHeight     = 10;
		nInitialComboSel     = 0;
		psa                  = NULL;
		dwItemData           = 0;
	}

	BOOL			bEnabled;				// TRUE = enabled, FALSE = disabled (gray text)
	BOOL			bBold;					// TRUE = display bold text
	int				nImage;					// index in image list, else -1
	CString			strToolTip;				// tool tip text for cell

	// for combo
	BOOL			bCombo;					// TRUE = display combobox
	BOOL			bComboIsClicked;		// TRUE = downarrow is clicked
	CStringArray *	psa;					// pointer to string array for combo listbox
	int				nComboListHeight;		// combo listbox height (in rows)
	int				nInitialComboSel;		// initial combo listbox selection (0 = first)

	// for color
	COLORREF	crText;
	COLORREF	crBackground;

	// for progress
	BOOL		bShowProgress;				// true = show progress control
	int			nProgressPercent;			// 0 - 100
	CString		strProgressMessage;			// custom message for progress indicator -
											// MUST INCLUDE %d
	BOOL		bShowProgressMessage;		// TRUE = display % message, or custom message
											// if one is supplied
	// for checkbox
	int			nCheckedState;				// -1 = don't show, 0 = unchecked, 1 = checked

	DWORD		dwItemData;					// pointer to app's data
};


///////////////////////////////////////////////////////////////////////////////
// CXListCtrl class
enum AFX_EXT_CLASS XColumnStatus 
{
	xcsNormal = 0,
	xcsArrowUp ,
	xcsArrowDown,
};

class AFX_EXT_CLASS CXListCtrl : public CListCtrl
{
// Construction
public:
	CXListCtrl();
	virtual ~CXListCtrl();

// Attributes
public:

// new Operations , by miniking
	void    SwapLine(int nLine1, int nLine2, int nColumns);
// Operations
public:
	void    SetColumnStatus(int nCol, XColumnStatus status);
	XColumnStatus GetColumnStatus(int nCol);
	int		CountCheckedItems(int nSubItem);
	BOOL	DeleteAllItems();
	BOOL	DeleteItem(int nItem);
	void	DeleteProgress(int nItem, int nSubItem);
	BOOL	GetBold(int nItem, int nSubItem);
	int		GetCheckbox(int nItem, int nSubItem);
	int		GetColumns();
	CString	GetComboText(int iItem, int iSubItem);
	int		GetCurSel();
	BOOL	GetEnabled(int nItem);
	DWORD	GetExtendedStyleX() { return m_dwExtendedStyleX; }
	int		GetHeaderCheckedState(int nSubItem);
	DWORD	GetItemData(int nItem);
	BOOL	GetSubItemRect(int iItem, int iSubItem, int nArea, CRect& rect);
	int		InsertItem(int nItem, LPCTSTR lpszItem);
	int		InsertItem(int nItem, 
					   LPCTSTR lpszItem, 
					   COLORREF crText, 
					   COLORREF crBackground);
	int		InsertItem(const LVITEM* pItem);
	BOOL	SetBold(int nItem, int nSubItem, BOOL bBold);
	BOOL	SetComboBox(int nItem, 
						int nSubItem, 
						BOOL bEnableCombo, 
						CStringArray *psa,
						int nComboListHeight,
						int nInitialComboSel);
	BOOL	SetCheckbox(int nItem, int nSubItem, int nCheckedState);
	void    CreateItemButton( int nItem, int nSubItem, HWND hMain );
	BOOL	SetCurSel(int nItem);
	BOOL	SetEnabled(int nItem, BOOL bEnable);

	DWORD	SetExtendedStyleX(DWORD dwNewStyle) 
	{
		DWORD dwOldStyle = m_dwExtendedStyleX;
		m_dwExtendedStyleX = dwNewStyle;
		return dwOldStyle;
	}

	BOOL	SetHeaderCheckedState(int nSubItem, int nCheckedState);
	int		SetItem(const LVITEM* pItem);
	BOOL	SetItemData(int nItem, DWORD dwData);
	BOOL	SetItemImage(int nItem, int nSubItem, int nImage);
	BOOL	SetItemTextColor(int nItem, int nSubItem, COLORREF crText);
	BOOL    SetItemBackColor(int nItem, int nSubItem, COLORREF crBackground);
	BOOL	SetItemText(int nItem, int nSubItem, LPCTSTR lpszText); 
	BOOL	SetItemText(int nItem, 
						int nSubItem, 
						LPCTSTR lpszText,
						COLORREF crText, 
						COLORREF crBackground);
	BOOL	SetProgress(int nItem, 
						int nSubItem, 
						BOOL bShowProgressText = TRUE, 
						LPCTSTR lpszProgressText = NULL);
	void	UpdateProgress(int nItem, int nSubItem, int nPercent);
	void	UpdateSubItem(int nItem, int nSubItem);

	void DeleteAllToolTips();
	BOOL SetItemToolTipText(int nItem, int nSubItem, LPCTSTR lpszToolTipText);
	CString GetItemToolTipText(int nItem, int nSubItem);
	virtual int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
	void release();
	void updateListCtrlButtonPos();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXListCtrl)
public:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	CXHeaderCtrl	m_HeaderCtrl;
	CImageList		m_cImageList;	// Image list for the header control
	button_map      m_mButton;
	WORD            m_uID;
protected:
	void DrawCheckbox(int nItem, 
					  int nSubItem, 
					  CDC *pDC, 
					  COLORREF crText,
					  COLORREF crBkgnd,
					  CRect& rect, 
					  XLISTCTRLDATA *pCLD);
	int DrawImage(int nItem, 
				  int nSubItem, 
				  CDC* pDC, 
				  COLORREF crText,
				  COLORREF crBkgnd,
				  CRect rect,
				  XLISTCTRLDATA *pXLCD);
	void DrawProgress(int nItem, 
					  int nSubItem, 
					  CDC *pDC, 
					  COLORREF crText,
					  COLORREF crBkgnd,
					  CRect& rect, 
					  XLISTCTRLDATA *pCLD);
	void DrawText(int nItem, 
				  int nSubItem, 
				  CDC *pDC, 
				  COLORREF crText,
				  COLORREF crBkgnd,
				  CRect& rect, 
				  XLISTCTRLDATA *pCLD);
	//void GetDrawColors(int nItem,
	//				   int nSubItem,
	//				   COLORREF& colorText,
	//				   COLORREF& colorBkgnd);
	void SubclassHeaderControl();

	BOOL			m_bHeaderIsSubclassed;
	DWORD			m_dwExtendedStyleX;

	COLORREF		m_cr3DFace;
	COLORREF		m_cr3DHighLight;
	COLORREF		m_cr3DShadow;
	COLORREF		m_crBtnFace;
	COLORREF		m_crBtnShadow;
	COLORREF		m_crBtnText;
	COLORREF		m_crGrayText;
	COLORREF		m_crHighLight;
	COLORREF		m_crHighLightText;
	COLORREF		m_crWindow;
	COLORREF		m_crWindowText;


	// Generated message map functions
protected:
	//{{AFX_MSG(CXListCtrl)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSysColorChange();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	virtual afx_msg BOOL OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult);

	DECLARE_MESSAGE_MAP()

protected:
	HBITMAP   m_hArrowUp, m_hArrowDown;
	
};

#endif //XLISTCTRL_H
