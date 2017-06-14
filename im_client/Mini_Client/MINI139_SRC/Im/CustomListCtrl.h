#pragma once

#include "impublic.h"
#include "..\..\..\SpaceManagement\public\public_controls\ResGdiObj.h"
#define MAX_RECENT_USER		20
#define TIMER_SORT			0x1000


// CClusterListCtrl
class CListCtrlBase : public CListBox
{
	DECLARE_DYNAMIC(CListCtrlBase)

public:
	CListCtrlBase();
	virtual ~CListCtrlBase();
	int				FindItem(int uid);
	virtual void	PopMenu(int nSel);
	int				FindUser(char cC);

protected:
	virtual void	DrawBitmap(CDC *pDC,int x, int y, UINT nIDBmp)			;
	virtual void	DrawHeadIcon(CDC *pDC, FRIEND_INFO *pUser, CPoint pt, bool bSmall)	;
	virtual void	DblClick(int nSel);
	
	void			DrawBitmap(CDC *pDC,int x, int y, CBitmap *pBitmap, bool bBlackWhite);

protected:
	void OnFriendInfo()		;
	void OnFriendAlbum()    ;
	void OnFriendBlog()     ;
	void OnFriendMsg()      ;
	void OnFriendAdd()      ;
	void OnP2PTalk()		;
	void SwapItem(int i, int j);
	void OnClearRecent();
	//afx_msg void OnLbnDblclk();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()

protected:
	int				m_cxExtent			;
	CImageList		m_ImageListHead		;
	CImageList		m_ImageListDefault	;
	int				m_nLastFindItem;

public:
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
};

class CClusterListCtrl : public CListCtrlBase
{
	DECLARE_DYNAMIC(CClusterListCtrl)

public:
	CClusterListCtrl();
	virtual ~CClusterListCtrl();

	void SetBorder(bool bShowBorder) { m_bShowBorder=bShowBorder; }
	void SetClusterInfo( CLUSTER_INFO * pCluster){ m_pCluster = pCluster;}
	void InvalidateItem(int  nItem);
	// 不会立刻刷新,会延时几秒才会执行
	void SortClusterList();
	void OnFriendDel();
	void PopMenu(int nSel);
	void ImGetClusterList();

protected:
	void SetInvalidateLock(BOOL bLock = TRUE)
	{ m_bInvalidateLock = bLock; };
	void SortClusterListReal();
	void ShowToolTip(const CString& sToolTip);
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
		
private:	
	CLUSTER_INFO *  m_pCluster		;
	bool            m_bShowBorder   ;
	int				m_nLastToolTip;
	CToolTipCtrl	m_toolTip;
	BOOL			m_bInvalidateLock;
};


class CRecentListCtrl : public CListCtrlBase
{
	DECLARE_DYNAMIC(CRecentListCtrl)

public:
	CRecentListCtrl();
	virtual ~CRecentListCtrl();

public:
	void Load();
	void Save();
	void UpdateUserList();

	void AddUser(FRIEND_INFO *pUser);
	void UpdateUser(FRIEND_INFO *pUser);

	void AddCluster(CLUSTER_INFO *pCluster);
	void UpdateCluster(CLUSTER_INFO *pCluster);

	// 获取最近联系人
	const FRIEND_LIST* GetRecentFriend();

protected:
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual void PopMenu(int nSel);
	virtual void DblClick(int nSel);

	afx_msg void OnDestroy();
	afx_msg void OnClusterTalk();
	afx_msg void OnClusterInfo();
	afx_msg void OnClusterPage();
	afx_msg void OnClusterMsg();
	afx_msg	void OnClearRecent();

	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

	void OnUserInfoGet(const char *data, WORD wRet);
	void OnUserStatusInd(const char *data, WORD wRet);

	// 获取好友状态
	void OnStatusGetAck(const char *data,WORD wRet);

	void ClearRecentList();
	void InvalidateItem(int nItem);
	bool FindUser(uint32 fid,FRIEND_INFO **ppFriend, int * pnItem);
	bool FindCluster(uint32 cid,FRIEND_INFO **ppFriend, int *pnItem);

private:
	struct tagCluster
	{
		uint32 cid;
		char   name[128];
		char   domain[24];
	};

	void GetCluster(FRIEND_INFO *pFriend,tagCluster &);
	void SetCluster(FRIEND_INFO *pFriend, const tagCluster & c);

	FRIEND_LIST		m_flRecent;
	int				m_nMaxCount;	// 设定最近联系人列表中的最大个数
	
public:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLbnDblclk();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

//////////////新建搜索联系人列表///////////////////////////////////////////////
class CSearchListCtrl : public CListCtrlBase
{
	DECLARE_DYNAMIC(CSearchListCtrl)

public:
	CSearchListCtrl();
	virtual ~CSearchListCtrl();

	void SetBorder(bool bShowBorder) { m_bShowBorder=bShowBorder; }
	void SetClusterInfo( CLUSTER_INFO * pCluster) { m_pCluster = pCluster; }
	void InvalidateItem(int  nItem);
	void PopMenu(int nSel);
	// 不会立刻刷新,会延时几秒才会执行

protected:
	void SetInvalidateLock(BOOL bLock = TRUE)
	{ m_bInvalidateLock = bLock; };
 ////void SortClusterListReal();
	void ShowToolTip(const CString& sToolTip);
	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

private:	
	CLUSTER_INFO *  m_pCluster		;
	bool            m_bShowBorder   ;
	int				m_nLastToolTip;
	CToolTipCtrl	m_toolTip;
	BOOL			m_bInvalidateLock;

private:
	struct tagCluster
	{
		uint32 cid;
		char   name[20];
		char   domain[20];
	};

public:
	afx_msg void OnLbnDblclk();
	void  DblClick(int nSel);
	void  GetCluster(FRIEND_INFO *pFriend, tagCluster & c);
	void  SetCluster(FRIEND_INFO *pFriend, const tagCluster & c);
	void  AddCluster(CLUSTER_INFO *pCluster);
	bool  FindCluster(uint32 cid, FRIEND_INFO **ppFriend, int *pnItem);

	afx_msg void OnClusterTalk();
	afx_msg void OnClusterInfo();
	afx_msg void OnClusterPage();
	afx_msg void OnClusterMsg();
	afx_msg void OnClearRecent();

    DWORD  m_dwCount;
	BOOL   m_bChange[3];
};
