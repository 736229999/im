#pragma once

#include "imlistctrl.h"
#include "..\DuiMfFriendList.h"

#include <afxmt.h>
#include <vector>
using namespace std;

/************************************************************************ 
* 
* �ļ�����xtreamgrouplist.h 
* 
* �ļ�������xtream �����б�ؼ�,
* 
* �����ˣ� ѦС��,2005��8��3��
* 
* �汾�ţ�1.0 
* 
* �޸ļ�¼�� 
* 
************************************************************************/ 

#define NM_FL_LBUTTONDOWN         0x01
#define NM_FL_LBUTTONUP           0x02

#define NM_FL_RBUTTONDOWN         0x03
#define NM_FL_RBUTTONUP           0x04

#define NM_FL_LBUTTONDBLCLICK     0x05
#define NM_FL_DRAGDROP			  0x06

//�����Ʊ༭���֪ͨ
#define NM_FL_GROUPNAME			  0x07

#define NM_FL_BLOG_CLICK           0x08
#define NM_FL_HEADICON_CLICK       0x09
#define NM_FL_RBUTTONUP_M		   0x0A
#define NM_FL_INFO_CLICK           0x0B
#define NM_FL_ALBUM_CLICK          0x0C
#define NM_FL_WEBINFO_CLICK		   0x0D
#define NM_FL_FOCUS_CLICK		   0x0E
#define NM_FL_ARTICLE_CLICK        0x0F
#define NM_FL_VEDIO_CLICK		   0x10
#define NM_FL_MOBILE_CLICK         0x11
#define NM_FL_ITEM_OVER			   0x12
#define NM_FL_SEL_CHANGE           0x13


//enum FLS_STATE
//{
//	FLS_EXPAND=0x0,
//	FLS_CLOSE=0x01,
//};
//
//typedef struct _FL_ITEM_POS
//{
//	int nGroupID;
//	int nItem;
//
//	_FL_ITEM_POS()
//	{
//		nGroupID   = -1;
//		nItem	   = -1;
//	}
//
//} FL_ITEM_POS;
//
//typedef struct _FL_ITEM_INFO
//{
//	DWORD         dwData;
//	CString       strText;
//
//	COLORREF      clrText ;
//	CBitmap     * bitmap20  ;
//	CBitmap     * bitmap40  ;
//	
//	//CString		  strBmp;
//
//	CRect         rcItem;
//	CRect         rcIcon;
//	CRect         rcText;
//	CRect         rcBtnInfo, rcBtnPic, rcBtnArticle;
//	CRect         rcBtnBlog;
//	CRect		  rcBtnWebInfo;//��վ����
//	CRect		  rcMobile, rcVedio;
//
////	CRect         rcBtnFocus;//��ע
//	
//	_FL_ITEM_INFO()
//	{
//		dwData		= 0;
//		clrText     = RGB(0,0,0);
//		bitmap20    = NULL;
//		bitmap40    = NULL;
//	//	rcBtnFocus.SetRect(0,0,0,0);
//		rcBtnWebInfo.SetRect(0,0,0,0);
//		rcMobile.SetRect(0,0,0,0);
//		rcVedio.SetRect(0,0,0,0);
//	}
//
//	~_FL_ITEM_INFO()
//	{
//		//if ( bitmap20 )
//		//	delete bitmap20;
//
//		//if ( bitmap40 )
//		//	delete bitmap40;
//	}
//
//} FL_ITEM_INFO;
//
//typedef struct _FL_GROUP_INFO
//{		
//	FLS_STATE		state;
//	FLS_STATE       state_before_drag;
//	CString			strTitle;
//	COLORREF		clrText;
//
//	DWORD			dwData;	// �����ͱ��
//	int				nFrdCnt;	// ���ڳ�Ա��������������ĳ�Ա
//	int 			nFrdOnCnt;   //���߳�Ա����
//
//	CRect			rcBar;
//	CRect			rcButton;
//	CRect			rcText;
//
//	vector<_FL_GROUP_INFO *> vecSubGroup; //���飬�ݹ鶨�壬�ݹ���ʾ
//
//	vector<_FL_ITEM_INFO *> vecItem;
//
//	// ��ȡ���飬������������
//	int GetSubGroup(vector<_FL_GROUP_INFO *> &vecTempSubGroup)
//	{
//		vecTempSubGroup.clear();
//		list<_FL_GROUP_INFO*> listGroup;
//		listGroup.push_back(this);
//		while (!listGroup.empty())
//		{
//			list<_FL_GROUP_INFO*>::iterator it = listGroup.begin();
//			vecTempSubGroup.push_back(*it);
//			for (int i = 0; i < (*it)->vecSubGroup.size(); ++i)
//			{
//				listGroup.push_back((*it)->vecSubGroup[i]);
//			}
//
//			listGroup.pop_front();
//		}
//		vecTempSubGroup.erase(vecTempSubGroup.begin());
//
//		return vecTempSubGroup.size();
//	}	
//
//	_FL_GROUP_INFO()
//	{
//		dwData		= 0;
//		state	    = FLS_EXPAND;
//		state_before_drag = FLS_EXPAND;
//		clrText     = RGB(0,0,0);
//	}
//
//	~_FL_GROUP_INFO()
//	{
//		for ( int i=0; i<vecItem.size(); i++ )
//			delete vecItem[i];
//
//		vecItem.clear();
//
//		// ���ر��������е�������delete����Ϊ����������Ĭ�ϵݹ�ɾ������
//		for(int i = 0; i < vecSubGroup.size(); ++i)
//		{
//			delete vecSubGroup[i];
//			vecSubGroup[i] = NULL;
//		}
//
//		vecSubGroup.clear();
//	}
//} FL_GROUP_INFO;
//



class CGroupEdit : public CFlatEdit
{
	DECLARE_DYNAMIC(CGroupEdit)

public:
	CGroupEdit(int nGroupId);
	virtual ~CGroupEdit();

protected:
	virtual void PreSubclassWindow();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();

	DECLARE_MESSAGE_MAP()

private:
	int  m_nGroupId		;
};

class CIMFriendListCtrl;

//------------------------------------------------------
// ���Ѷ�ѡ�б�
//------------------------------------------------------

class MULTI_SEL_LST
{
	friend class CIMFriendListCtrl;

public:
	// ����ĳ����λ�����Ƿ���ڶ�ѡ�б��У������ڷ���-1
	int		FindPos(FL_ITEM_POS pos)
	{
		for(int i=0;i<m_lst.size();i++)
		{
			if(pos.nGroupID==m_lst[i].nGroupID &&pos.nItem==m_lst[i].nItem)
				return i;
		}
		return -1;
	};
	// ���һ������λ�����ѡ�б�
	void	AddPos(FL_ITEM_POS pos)
	{
		if(FindPos(pos)==-1)
		{
			m_lst.push_back(pos);
		}
	}
	// ��Ӷ������λ�õ���ѡ�б�
	void	AddPos(const MULTI_SEL_LST& posList)
	{
		int nS=posList.Size();
		for(int i=0;i<nS;i++)
		{
			AddPos(posList.m_lst[i]);
		}
	}
	// ɾ��һ������λ��������λ�ò����ڣ�����-1
	int		DelPos(FL_ITEM_POS pos)
	{
		int nPos=FindPos(pos);	
		if(nPos==-1)
			return -1;
		m_lst.erase(m_lst.begin()+nPos);//&m_lst[nPos]);
		return nPos;
	}
	// ���λ������
	int		Size() const
	{
		return m_lst.size();
	}
	// ����б�
	void	Clear()
	{ m_lst.clear(); };
	// ������һ��ѡ�����
	bool	GetLastPos(FL_ITEM_POS &pos)
	{ 
		if(m_lst.size()<1) 
			return false;
		else
			pos=m_lst[m_lst.size()-1];
	};
	// ���λ��
	const FL_ITEM_POS & Get(int nIdx) const
	{ ASSERT(nIdx>=0); return m_lst[nIdx];};

protected:
	// �������ѡ����
	void	PushBack(const FL_ITEM_POS& pos)
	{ m_lst.push_back(pos); };

private:
	vector<FL_ITEM_POS>	m_lst;
};


//------------------------------------------------------
// �����б�ؼ�
//------------------------------------------------------

class CIMFriendListCtrl : public CIMListCtrl
{
public: 
	CIMFriendListCtrl();	
	virtual ~CIMFriendListCtrl();

// Implementation
public:
	BOOL Create(CWnd *pParentWnd,const CRect &rect, DWORD dwStyle=WS_VISIBLE|WS_CHILDWINDOW|WS_CLIPCHILDREN,UINT nID=0);

	bool IsInvalidateLock() { return m_bInvalidateLock; }
	void SetInvalidateLock(bool bLock) {
		m_bInvalidateLock=bLock; 										
		if ( bLock==false )											 
			InvalidateList(); 
	}

	//��ʾ�༭������edit box
	void ShowEditBox(int nGroup);

	void ClearGroup(int nGroup);
	int DelGroup(int nParentGroup,int nGroup);
	int AddGroup(int nGroupId, CString strTitle);
	int AddSubGroup(int nGroupId, int nSubGroupId, CString strTitle);
	void SetGroupTextColor(int nGroup, COLORREF clrText);
	void SetItemTextColor(FRIEND_INFO *pFriend, COLORREF clrText);
	void SetGroupName(int nGroup, CString strTitle);
	void SetGroupState(int nGroup, FLS_STATE state);
	CString GetGroupName(int nGroup);

	
	void DeleteItem(int nGroup, int nItem);
	void DeleteAllItems();

	inline bool ItemExist(int nGroup, int nItem) {
		if ( nGroup>-1 && nGroup<m_vecGroup.size() )
		{
			if ( nItem>-1 && nItem<m_vecGroup[nGroup]->vecItem.size() )
			{
				return true;
			}
		}

		//ASSERT(FALSE);
		return false;
	}

	inline bool GroupExist(int nGroup)
	{
		if ( nGroup>-1 && nGroup<m_vecGroup.size() )
		{
			return true;
		}

		ASSERT(FALSE);
		return false;
	}
	
	int AddItem(int iGroup, CString strText, DWORD dwData);
	int SubGroupAddItem(FL_GROUP_INFO *tmp, int iGroup, CString strText, DWORD dwData);
	
	CString GetItemText(int nGroup, int nIndex);
	void SetItem(int iGroup, int nIndex, CString strText, DWORD dwData = 0);
	void SetItemText(int nGroup, int nIndex, CString strText);
	void DelItemImage(int nGroup, int nItem );
	void UpdateItemImage(FRIEND_INFO *pFriend);
	DWORD GetItemData(int nGroup, int nIndex);
	CRect GetItemRect(int nGroup, int nIndex);
	CRect GetItemIconRect(int nGroup, int nIndex);
	void SetItemData(int nGroup, int nIndex, DWORD dwData);
	void SetItemTextColor(int nGroup, int nIndex, COLORREF clrText);

	void  SwapItem(int nGroup1, int nItem1, int nGroup2, int nItem2);
	int  MoveItem(int nGroupSrc, int nItemSrc, int nGroupDest);

	void  SetGroupData(int nGroup,DWORD dwData);
	// ��ø��������id
	FL_GROUP_INFO* GetGroupData(int nGroup);
	int   GetGroupState(int nGroup);
	int   GetItemCount(int nGroup);
	FL_ITEM_POS GetCurSel();
	void SetCurSel(int nGroupID,int nItem);
	void HitTest(vector<FL_GROUP_INFO *>vecGroup,const POINT& pt, int& nGroup, int& nItem);
	void		AddRefreshFlag();
	void		RemoveRefreshFlag();
	void		InvalidateList();
	//------------------------------------------------------
	// ��ö�ѡ�ĺ����б�
	//------------------------------------------------------
	const MULTI_SEL_LST& GetMultSel() const
	{ return m_multSelList; };
	//------------------------------------------------------
	// ���Һ������б��е�λ�ã�û�в��ԣ�
	//------------------------------------------------------
	bool		FindFriend(FRIEND_INFO*pFriend,int&nGroup,int&nItem);
	//------------------------------------------------------
	// ѡ��һ�����ѣ�û�в��ԣ�
	//------------------------------------------------------
	void		SelItem(FRIEND_INFO*pFriend);
	//------------------------------------------------------
	// ���һ�����ѣ�û�в��ԣ�
	//------------------------------------------------------
	void		AddFriend(FRIEND_INFO*pFriend);
	//------------------------------------------------------
	// ������id��ø������б��е�����λ��
	//------------------------------------------------------
	int			FindGroup(int nGroupID);
	//------------------------------------------------------
	// �����ṩ������ĸ��ѯ���������ĵ�һ���û�
	//------------------------------------------------------
	void		FindUser(char cC);	
	//------------------------------------------------------
	// ���ϴβ�ѯ�������ʼ��ѯ��һ���û�
	//------------------------------------------------------
	void		FindUser(char cC,int nLastGroup,int nLastItem);
	//------------------------------------------------------
	// ��ö�ѡ��ѡ�е���
	//------------------------------------------------------
	void		GetMultSel(FL_ITEM_POS beg,FL_ITEM_POS end,MULTI_SEL_LST & rst);
	//------------------------------------------------------
	// ��յ�ǰѡ��ĺ���
	//------------------------------------------------------
	void		ClearMultSel()
	{ m_multSelList.Clear(); InvalidateList(); };
	//------------------------------------------------------
	// 
	//------------------------------------------------------
	void		SetGroupFriendsCount(int nGroup,int nCount);
	//------------------------------------------------------
	// 
	//------------------------------------------------------
	void		IncreaseFriend(int nFolder);

	//�۵�������
	void	CloseAllGroup();
	
	void SetItemText(FRIEND_INFO *pFriend, CString strText);
	
	void DeleteItem(FRIEND_INFO *pFriend);
	void SortGroupByState(int nGroupId);
	void SetCurSel(FRIEND_INFO *pFriend);
	int InsertGroup(int nParentGroup,int nGroupId, CString strTitle);

	// ��ȡ�����飨ѭ��������Ľ��������Ҫ�����ٱ�����
	int GetAllGroup(vector<FL_GROUP_INFO *> &vecGroup);
	
protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

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
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// �����ͣ��Ϣ
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

	LRESULT OnGroupNameEdit(WPARAM w,LPARAM l);

private:

	void InvalidateItem(int nGroup, int nIndex, BOOL bErase = TRUE);
	void DrawGroup(FL_GROUP_INFO* Group, CDC *pDC);
	void DrawFriend(FL_ITEM_INFO *pItem, CDC *pDC , bool bSelItem);

	void DrawStatusFoot(CDC *pDC, CRect rcIcon, bool bSelItem, int status );
	void OnBeginDrag();
	void OnEndDrag();
	void CalcScrollInfo();
	void RecalcLayout();
	void SetSubGroupHeight(FL_GROUP_INFO *pParentGroup , int& nPreGroupHeight);
		
	
	FL_GROUP_INFO* GetGroup(vector<FL_GROUP_INFO *> vecGroup, int nGroupID );
	
	FL_ITEM_INFO* GetItem(vector<FL_GROUP_INFO *> vecGroup, int nGroupID, int nItemID);
	FL_ITEM_INFO* GetItem(FRIEND_INFO *pFriend);
	
	void DeleteItems(vector<FL_GROUP_INFO *>& vecGroup);

private:

	
	BOOL			m_bAltDown;		// alt���Ƿ���
	BOOL			m_bShiftDown;	// shift���Ƿ���
	BOOL			m_bCtrlDown;	// ctrl���Ƿ���
	MULTI_SEL_LST	m_multSelList;	// ��ѡ�б�
	FL_ITEM_POS		m_posLast;		// �ϴ�ѡ��Ľڵ�

	FL_ITEM_POS		m_posItemSelected;
	FL_ITEM_POS		m_posDragTo;

	COLORREF		m_clrBkGround;

	int				m_nItemHeight;
	int             m_nTextHeight;
	int             m_nSelItemHeight;

	vector<FL_GROUP_INFO *> m_vecGroup;

	//CFont		m_Font;

	int			m_OriginY;
	int			m_nClientRealHeigh;

	bool        m_bBeginDrag; 
	bool        m_bInvalidateLock;

	bool        m_bMouseAlbumDown    ;
	bool		m_bMouseArticleDown  ;
	bool        m_bMouseInfoDown     ;
	bool        m_bMouseBlogDown     ;
	bool        m_bMouseIconDown     ;
	bool		m_bMouseWebInfoDown  ;
	//bool        m_bMouseFocusDown;
	bool		m_bMouseVedioDown, m_bMouseMobileDown;

	bool        m_bMouseOnIcon;
	bool		m_bMouseOnBlog;
	bool		m_bMouseOnInfo;
	bool		m_bMouseOnAlbum;
	bool		m_bMouseOnArticle;
	bool		m_bMouseOnWebInfo;
	bool		m_bMouseOnVedio, m_bMouseOnMobile;

	bool        m_bRefreshFriendListFlag;

	CImageList	m_ButtonImages;
	
	CBitmap		m_canvas;			// ����
	int			m_nWidth;			// �������
	int			m_nHeight;			// �����߶�
	bool		m_bRedraw;			// �Ƿ���Ҫ�ػ�

	char		m_cLastFindChar;	// �ϴβ�ѯ����ĸ
	int			m_nLastFindGroup;	// �ϴβ�ѯ����ĸ���ڵ���
	int			m_nLastFindItem;	// �ϴβ�ѯ����ĸ���ڵ���
	CResBitmap  m_bmpWebArticle, m_bmpWebPicture;
	//CResBitmap  m_bmpFocusOn, m_bmpFocusOff;

	// ���������ͣ��Ϣ
	TRACKMOUSEEVENT m_TMS;
	BOOL m_bTracking;
};

