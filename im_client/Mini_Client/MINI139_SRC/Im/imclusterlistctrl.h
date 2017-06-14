#pragma once

#include "imlistctrl.h"
#include <afxmt.h>
#include <vector>
#include "..\DuiMfClusterList.h"
using namespace std;


#include "resgdiobj.h"

/************************************************************************ 
* 
* 文件名：xtreamgrouplist.h 
* 
* 文件描述：xtream 好友列表控件,
* 
* 创建人： 薛小峰,2005年8月3日
* 
* 版本号：1.0 
* 
* 修改记录： 
* 
************************************************************************/ 
//
//#define NM_CL_LBUTTONDOWN         0x01
//#define NM_CL_LBUTTONUP           0x02
//
//#define NM_CL_RBUTTONDOWN         0x03
//#define NM_CL_RBUTTONUP           0x04
//
//#define NM_CL_LBUTTONDBLCLICK     0x05
//#define NM_CL_EXPAND              0x06
//
//#define NM_CL_HOME_CLICK			0x07
//#define NM_CL_REFUSE_CLICK			0x08
//#define NM_CL_PARTY_CLICK			0x09
//#define NM_CL_SETTING_CLICK         0x0A
//#define NM_CL_ALBUM_CLICK           0x0B
//#define NM_CL_ARTICLE_CLICK			0x0C
//#define NM_CL_WRITE_ARTICLE_CLICK	0x0D
//#define NM_CL_UPLOAD_PIC_CLICK		0x0E
//#define NM_CL_DISCUSS_CLICK         0x0F  
//#define NM_CL_TITLE_CLICK		    0x10
//
//enum CLS_STATE
//{
//	CLS_EXPAND,
//	CLS_CLOSE,
//};
//
//typedef struct _CL_GROUP_POS
//{
//	int nRoot;
//	int nGroup;
//
//	_CL_GROUP_POS()
//	{
//		nRoot  = -1;
//		nGroup = -1;
//	}
//
//	bool operator ==(_CL_GROUP_POS &g)
//	{
//		if ( nRoot==g.nRoot && nGroup==g.nGroup )
//			return true;
//		else
//			return false;
//	}
//
//	bool IsNull()
//	{
//		if ( nRoot==-1 || nGroup==-1 )
//		{
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	}
//
//} CL_GROUP_POS;
//
typedef struct _CL_ITEM_POS
{
	int nRoot;
	int nGroup;
	int nItem;

	_CL_ITEM_POS() 
	{
		nRoot    = -1;
		nGroup   = -1;
		nItem    = -1;
	}

	operator CL_GROUP_POS() const
	{
		CL_GROUP_POS pos;
		pos.nRoot  = nRoot;
		pos.nGroup = nGroup;
		return pos;
	}

	bool operator ==(_CL_ITEM_POS &i)
	{
		if ( nRoot==i.nRoot && nGroup==i.nGroup && nItem==i.nItem )
			return true;
		else
			return false;
	}

	bool IsNull()
	{
		if ( nRoot==-1 || nGroup==-1 || nItem==-1 )
		{
			return true;
		}
		else
		{
			return false;
		}
	}

} CL_ITEM_POS;

typedef struct _CL_ITEM_INFO
{
	DWORD         dwData;
	char          szText[50];

	COLORREF      clrText;

	CRect         rcItem;
	CRect         rcIcon;
	CRect         rcText;

	_CL_ITEM_INFO()
	{
		dwData		= 0;
		clrText     = RGB(0,0,0);
	}

	~_CL_ITEM_INFO()
	{
	}

} CL_ITEM_INFO;
//
//typedef struct _CL_GROUP_INFO
//{		
//	CLS_STATE		state		;
//
//	char            szTitle[50]	;
//
//	COLORREF		clrText		;
//
//	DWORD			dwData		;
//
//	CRect			rcBar		;// 组的长条范围大小
//	CRect			rcFace		;// 族徽
//	CRect			rcText		;// 文本范围
//
//	CRect			rcBtnHome	;// 博客主页按钮范围
//	CRect			rcBtnRefuse	;// 阻止按钮范围
//	//CRect			rcBtnAlbum;
//	//CRect			rcBtnUploadPic;
//	//CRect			rcBtnArticle;
//	//CRect			rcBtnWriteArticle;
//	//CRect			rcBtnSetting;
//	//CRect			rcBtnParty;//讨论区
//
//	CRect			rcBtnExpand ;//+/- button
//
//	CBitmap		*   pBitmap40		; //族徽
//	CBitmap     *   pBitmap20       ; //族徽
//    
//	bool            bLoadingMembers	;
//	bool			bFlash			;//是否闪烁
//	int				iFlash			;//no use other
//
//	vector<_CL_ITEM_INFO *> vecItem;
//
//	_CL_GROUP_INFO()
//	{
//		dwData		    = 0;
//		state	        = CLS_CLOSE;
//		bLoadingMembers = false;
//		bFlash          = false;
//		iFlash          = 0;
//		pBitmap40		= NULL;
//		pBitmap20       = NULL;
//		clrText         = RGB(0,0,0);
//	}
//
//	~_CL_GROUP_INFO()
//	{
//		for ( int i=0; i<vecItem.size(); i++ )
//		{
//			delete vecItem[i];
//		}
//
//		vecItem.clear();
//	}
//
//} CL_GROUP_INFO;
//
//
//typedef struct _CL_ROOT_INFO
//{
//	CLS_STATE  state;
//	CString    strTitle;
//
//	CRect      rcBar   ;
//	CRect      rcButton;
//	CRect      rcText;
//
//	vector<CL_GROUP_INFO *> vecGroup;
//
//	_CL_ROOT_INFO()
//	{
//		state=CLS_EXPAND;
//	}
//
//} CL_ROOT_INFO;

class CIMClusterListCtrl : public CIMListCtrl
{
public:
	CIMClusterListCtrl();	
	virtual ~CIMClusterListCtrl();
	
// Implementation
public:
	BOOL Create(CWnd *pParentWnd,const CRect &rect, DWORD dwStyle=WS_VISIBLE|WS_CHILDWINDOW|WS_CLIPCHILDREN,UINT nID=0);

	bool IsInvalidateLock() { return m_bInvalidateLock; }
	void SetInvalidateLock(bool bLock) {
		m_bInvalidateLock=bLock; 										
		if ( bLock==false )											 
			InvalidateList(); 
	}

	int AddRoot(CString strRoot);

	CL_GROUP_POS FindCluster(int nClusterId);

	void ClearGroup(CL_GROUP_POS posGroup);
	void DelGroup(CL_GROUP_POS posGroup);
	int  InsertGroup(CL_GROUP_POS posGroup,CString strTitle,DWORD dwData=0);
	CL_GROUP_POS AddGroup(int nRoot,CString strTitle);
	void SetGroupTextColor(CL_GROUP_POS posGroup, COLORREF clrText);
	void SetGroupName(CL_GROUP_POS posGroup, CString strTitle);
	void SetGroupState(CL_GROUP_POS posGroup, CLS_STATE state);
	void UpdateGroupImage(CL_GROUP_POS posGroup);
	CString GetGroupName(CL_GROUP_POS posGroup);
	
	void DeleteItem(CL_ITEM_POS posItem);
	void DeleteAllItems();

	//int AddItem(int iGroup, CString strText, LPCSTR lpszBmp, DWORD dwData=0 );
	CL_ITEM_POS AddItem(CL_GROUP_POS posGroup, CString strText, DWORD dwData = 0);
	CString GetItemText(CL_ITEM_POS posItem);
	int	 GetItemImage(CL_ITEM_POS posItem);
	void SetItem(CL_ITEM_POS posItem,CString strText, DWORD dwData = 0);
	void SetItemText(CL_ITEM_POS posItem,CString strText);
	DWORD GetItemData(CL_ITEM_POS posItem);
	void SetItemData(CL_ITEM_POS posItem,DWORD dwData);
	void SetItemTextColor(CL_ITEM_POS posItem, COLORREF clrText);

	void  SwapItem(CL_ITEM_POS pos1,CL_ITEM_POS pos2);
	void  SwapGroup(CL_GROUP_POS pos1, CL_GROUP_POS pos2);
	int  MoveItem(CL_ITEM_POS posItem, CL_GROUP_POS posGroup);

	void  SetGroupData(CL_GROUP_POS posGroup,DWORD dwData);
	DWORD GetGroupData(CL_GROUP_POS posGroup);
	CL_GROUP_INFO * GetGroupInfo(CL_GROUP_POS posGroup);
	int   GetRootCount();
	int   GetGroupCount();
	int   GetGroupCount(int nRoot);
	int   GetItemCount(CL_GROUP_POS posGroup);
	CL_ITEM_POS GetCurSel();
	void SetCurSel(CL_ITEM_POS);
	void ClearCurSel();//清空当前选中的位置

	void HitTest(const POINT& pt, CL_ITEM_POS &posItem);

	void AddLoadFlag(CL_GROUP_POS posGroup);
	void RemoveLoadFlag(CL_GROUP_POS posGroup);

	void AddFlashFlag(CL_GROUP_POS posGroup);
	void RemoveFlashFlag(CL_GROUP_POS posGroup);

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
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

protected:
    void InvalidateItem( CL_ITEM_POS posItem, bool bErase = TRUE);
	void InvalidateGroup( CL_GROUP_POS posGroup );
	void DrawRoot(int nRoot, CDC *pDC);
	void DrawGroup(int nRoot, int nGroup, CDC *pDC);
	//void DrawGroupBar(CDC *pDC,CRect rcBar, CPoint ptCursor,bool bSelected,bool bExpand);
	void CalcScrollInfo();
	void RecalcLayout();
	void InvalidateList();
		
private:
	int		m_nItemHeight;
	int		m_nTextHeight;
	int     m_nSelItemHeight;

	CL_ITEM_POS m_posItemSelected;

	COLORREF m_clrBkGround;

	vector<CL_ROOT_INFO *>  m_vecRoot;

	//CFont		m_Font;

	int			m_OriginY;
	int			m_nClientRealHeigh;

	HCURSOR     m_hCursor;

	CImageList	 m_ButtonImages   ;

	CBitmap		m_canvas;			// 画布
	int			m_nWidth;			// 画布宽度
	int			m_nHeight;			// 画布高度
	bool		m_bRedraw;			// 是否需要重绘

	bool		m_bMouseOnTitle     ;
	bool		m_bMouseOnGroupIcon		;
	bool		m_bMouseOnHome		;
	bool		m_bMouseOnRefuse	;	
	//bool		m_bMouseOnAlbum		;
	//bool		m_bMouseOnArticle	;
	//bool		m_bMouseOnWriteArticle;
	//bool		m_bMouseOnUploadPic   ;
	//bool		m_bMouseOnSetting	;
	//bool		m_bMouseOnParty		;
	bool		m_bMouseOnExpand	;

	bool		m_bBtnGroupIconDown		;
	bool		m_bBtnHomeDown		;
	bool		m_bBtnRefuseDown	;
	//bool		m_bBtnArticleDown   ;
	//bool		m_bBtnAlbumDown     ;
	//bool		m_bBtnWriteArticleDown	;
	//bool		m_bBtnUploadPicDown		;
	//bool		m_bBtnSettingDown		;
	//bool		m_bBtnPartyDown			;

	bool		m_bMouseOver            ;

	CResBitmap	m_bmpFamilyHead			;//默认族徽
	CResBitmap  m_bmpGroupTool			;
	CResBitmap  m_bmpGroupStatus        ;

	bool		m_bInvalidateLock;
};

