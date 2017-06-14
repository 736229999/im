#pragma once

#include "imlistctrl.h"

#include <afxmt.h>
#include <vector>
using namespace std;


#define NM_AL_LBUTTONDOWN         0x01
#define NM_AL_LBUTTONUP           0x02

#define NM_AL_RBUTTONDOWN         0x03
#define NM_AL_RBUTTONUP           0x04

#define NM_AL_LBUTTONDBLCLICK     0x05
#define NM_AL_DRAGDROP			  0x06
#define NM_AL_MAIL_CLICK          0x07

enum ALS_STATE
{
	ALS_EXPAND,
	ALS_CLOSE,
};

typedef struct _AL_ITEM_POS
{
	int nGroup;
	int nItem;

	_AL_ITEM_POS()
	{
		nGroup   = -1;
		nItem    = -1;
	}

} AL_ITEM_POS;

typedef struct _AL_ITEM_INFO
{
	DWORD         dwData;
	CString       strText;

	COLORREF      clrText;
	int		      iImage;
	CBitmap     * bitmap;

	CRect         rcItem;
	CRect         rcIcon;
	CRect         rcText;
	CRect		  rcBtnSendMail;

	_AL_ITEM_INFO()
	{
		dwData		= 0;
		clrText     = RGB(0,0,0);
		iImage		= -1;
		bitmap      = NULL;
		rcBtnSendMail.SetRect(0,0,0,0);
	}

	~_AL_ITEM_INFO()
	{
		if ( bitmap )
			delete bitmap;
		bitmap=NULL;
	}

} AL_ITEM_INFO;

typedef struct _AL_GROUP_INFO
{		
	ALS_STATE		state     ;
	ALS_STATE       state_before_drag;
	CString			strTitle  ;
	COLORREF		clrText   ;

	DWORD			dwData    ;

	CBitmap    *    bitmap    ;//ืๅปี

	CRect			rcBar     ;
	CRect			rcButton  ;
	CRect			rcText    ;

	vector<_AL_ITEM_INFO *> vecItem;

	_AL_GROUP_INFO()
	{
		dwData		= 0;
		state	    = ALS_CLOSE;
		clrText     = RGB(0,0,0);
	}

	~_AL_GROUP_INFO()
	{
		for ( int i=0; i<vecItem.size(); i++ )
		{
			delete vecItem[i];
		}

		vecItem.clear();
	}

} AL_GROUP_INFO;


class CIMAddressListCtrl : public CIMListCtrl
{
public:
	CIMAddressListCtrl();	
	virtual ~CIMAddressListCtrl();

// Implementation
public:
	BOOL Create(CWnd *pParentWnd,const CRect &rect, DWORD dwStyle=WS_VISIBLE|WS_CHILDWINDOW|WS_CLIPCHILDREN,UINT nID=0);
	void SetImageList(CImageList *pImageList);

	void ClearGroup(int nGroup);
	void DelGroup(int nGroup);
	int  InsertGroup(int nGroup,CString strTitle,DWORD dwData=0);
	int  AddGroup(CString strTitle);
	void SetGroupTextColor(int nGroup, COLORREF clrText);
	void SetGroupName(int nGroup, CString strTitle);
	void SetGroupState(int nGroup, ALS_STATE state);
	CString GetGroupName(int nGroup);
	
	void DeleteItem(int nGroup, int nItem);
	void DeleteAllItems();

	void InvalidateItem(int nGroup, int nIndex, BOOL bErase = TRUE);

	//int AddItem(int iGroup, CString strText, LPCSTR lpszBmp, DWORD dwData=0 );
	int AddItem(int iGroup, CString strText, int nImage, DWORD dwData = 0);
	CString GetItemText(int nGroup, int nIndex);
	int	 GetItemImage(int nGroup, int nIndex);
	CBitmap * GetItemBitmap(int nGroup, int nIndex);
	void SetItem(int iGroup, int nIndex, CString strText, int nImage = -1, DWORD dwData = 0);
	void SetItemText(int nGroup, int nIndex, CString strText);
	void SetItemImage(int nGroup, int nIndex, int iImage);
	void SetItemBitmap(int nGroup, int nIndex, CBitmap *pBitmap);
	//void SetItemImage(int nGroup, int nIndex, CString strBmp);
	DWORD GetItemData(int nGroup, int nIndex);
	void SetItemData(int nGroup, int nIndex, DWORD dwData);
	void SetItemTextColor(int nGroup, int nIndex, COLORREF clrText);

	void  SwapItem(int nGroup1, int nItem1, int nGroup2, int nItem2);
	int  MoveItem(int nGroupSrc, int nItemSrc, int nGroupDest);

	void  SetGroupData(int nGroup,DWORD dwData);
	DWORD GetGroupData(int nGroup);
	int   GetGroupCount();
	int   GetItemCount(int nGroup);
	AL_ITEM_POS GetCurSel();
	void SetCurSel(int nGroup,int nItem);

	void HitTest(const POINT& pt, int& nGroup, int& nItem);

protected:
	virtual void PreSubclassWindow();

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnVScroll (UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel (UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSize (UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	virtual void InitToolTip();

	DECLARE_MESSAGE_MAP()

private:
	void DrawGroup(AL_GROUP_INFO * Group,CDC *pDC,BOOL bSelectedGroup);
	void CalcScrollInfo();
	void RecalcLayout();
	void InvalidateList();
		
private:
	int		m_nItemHeight;
	int		m_nTextHeight;
	int     m_nSelItemHeight;

	AL_ITEM_POS m_posItemSelected;

	COLORREF m_clrBkGround;

	vector<AL_GROUP_INFO *> m_vecGroup;

	CFont		m_Font;

	int			m_OriginY;
	int			m_nClientRealHeigh;

	HCURSOR     m_hCursor;

	CImageList	 * m_pImageList;
	CImageList	 m_ButtonImages;

	CWnd*		m_pTrueParent;

	bool        m_bBeginDrag; 
	AL_ITEM_POS		m_posDragTo;

	bool			m_bMouseMailDown;
	bool			m_bMouseOnMail;


	CResBitmap  m_bmpMini, m_bmpOutlook, m_bmpOutlook2, m_bmpSendMail;

};

