#pragma once

#include "..\..\..\..\DuiLib\UIlib.h"
#include "im\impublic.h"

enum FLS_STATE
{
	FLS_EXPAND=0x0,
	FLS_CLOSE=0x01,
};

typedef struct _FL_ITEM_POS
{
	int nGroupID;
	int nItem;

	_FL_ITEM_POS()
	{
		nGroupID   = -1;
		nItem	   = -1;
	}

} FL_ITEM_POS;

typedef struct _FL_ITEM_INFO
{
	DWORD         dwData;
	CString       strText;

	COLORREF      clrText ;
	CBitmap     * bitmap20  ;
	CBitmap     * bitmap40  ;

	//CString		  strBmp;

	CRect         rcItem;
	CRect         rcIcon;
	CRect         rcText;
	CRect         rcBtnInfo, rcBtnPic, rcBtnArticle;
	CRect         rcBtnBlog;
	CRect		  rcBtnWebInfo;//��վ����
	CRect		  rcMobile, rcVedio;

	//	CRect         rcBtnFocus;//��ע

	_FL_ITEM_INFO()
	{
		dwData		= 0;
		clrText     = RGB(0,0,0);
		bitmap20    = NULL;
		bitmap40    = NULL;
		//	rcBtnFocus.SetRect(0,0,0,0);
		rcBtnWebInfo.SetRect(0,0,0,0);
		rcMobile.SetRect(0,0,0,0);
		rcVedio.SetRect(0,0,0,0);
	}

	~_FL_ITEM_INFO()
	{
		//if ( bitmap20 )
		//	delete bitmap20;

		//if ( bitmap40 )
		//	delete bitmap40;
	}

} FL_ITEM_INFO;

typedef struct _FL_GROUP_INFO
{		
	FLS_STATE		state;
	FLS_STATE       state_before_drag;
	CString			strTitle;
	COLORREF		clrText;

	DWORD			dwData;	// �����ͱ��
	int				nFrdCnt;	// ���ڳ�Ա��������������ĳ�Ա
	int 			nFrdOnCnt;   //���߳�Ա����

	CRect			rcBar;
	CRect			rcButton;
	CRect			rcText;

	vector<_FL_GROUP_INFO *> vecSubGroup; //���飬�ݹ鶨�壬�ݹ���ʾ

	vector<_FL_ITEM_INFO *> vecItem;

	// ��ȡ���飬������������
	int GetSubGroup(vector<_FL_GROUP_INFO *> &vecTempSubGroup)
	{
		vecTempSubGroup.clear();
		list<_FL_GROUP_INFO*> listGroup;
		listGroup.push_back(this);
		while (!listGroup.empty())
		{
			list<_FL_GROUP_INFO*>::iterator it = listGroup.begin();
			vecTempSubGroup.push_back(*it);
			for (int i = 0; i < (*it)->vecSubGroup.size(); ++i)
			{
				listGroup.push_back((*it)->vecSubGroup[i]);
			}

			listGroup.pop_front();
		}
		vecTempSubGroup.erase(vecTempSubGroup.begin());

		return vecTempSubGroup.size();
	}	

	_FL_GROUP_INFO()
	{
		dwData		= 0;
		state	    = FLS_EXPAND;
		state_before_drag = FLS_EXPAND;
		clrText     = RGB(0,0,0);
	}

	~_FL_GROUP_INFO()
	{
		for ( int i=0; i<vecItem.size(); i++ )
			delete vecItem[i];

		vecItem.clear();

		// ���ر��������е�������delete����Ϊ����������Ĭ�ϵݹ�ɾ������
		for(int i = 0; i < vecSubGroup.size(); ++i)
		{
			delete vecSubGroup[i];
			vecSubGroup[i] = NULL;
		}

		vecSubGroup.clear();
	}
} FL_GROUP_INFO;

class CDuiFriendList;
class CDuiGroupEdit : public DuiLib::CEditUI
{
public:
	CDuiGroupEdit(CDuiFriendList *pList);
	void DoEvent(DuiLib::TEventUI& event);

private:
	CDuiFriendList *m_pList;
};

class CDuiFriendList : public DuiLib::CTreeViewUI, public DuiLib::IDialogBuilderCallback
{
public:
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass);
	void DoEvent(DuiLib::TEventUI& event);
	bool SelectItem(int iIndex, bool bTakeFocus = false);

public: 
	CDuiFriendList(HWND hParent, DuiLib::CPaintManagerUI& paint_manager);	
	virtual ~CDuiFriendList();

	// Implementation
public:
	/*bool IsInvalidateLock() { return m_bInvalidateLock; }
	void SetInvalidateLock(bool bLock) {
		m_bInvalidateLock=bLock; 										
		if ( bLock==false )											 
			InvalidateList(); 
	}*/

	//��ʾ�༭������edit box
	//void ShowEditBox(int nGroup);
	int DelGroup(int nParentGroup,int nGroup);
	int AddGroup(int nGroupId, CString strTitle);
	int AddSubGroup(int nGroupId, int nSubGroupId, CString strTitle);
	void SetGroupTextColor(int nGroup, COLORREF clrText);
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
	//void		AddRefreshFlag();
	//void		RemoveRefreshFlag();
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
	// ��ʾ�༭����
	void ShowEditBox(int nGroup);
	LRESULT OnGroupNameEdit(WPARAM w,LPARAM l);
	// �����Ƿ�չ��
	BOOL IsExpand(int nGroupID);
	// չ����رշ���
	void ExpandGroup(int nGroupID, BOOL expand);
	// ѡ����/��ȡ��ѡ��
	void SelectGroup(int nGroupID, BOOL bSelect);
	// �жϺ����б��е�item�Ƿ�Ϊ����
	BOOL IsGroupItem(DuiLib::CTreeNodeUI *pItem);
	// ������ڵ�
	DuiLib::CTreeNodeUI* FindGroupNode(int nGroup);

protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel (UINT nFlags, short zDelta, CPoint pt);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	// �����ͣ��Ϣ
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);

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

	// ������ɾ����
	void DelGroup_Interface(int nGroupId);
	// �����������
	BOOL AddGroup_Interface(int nGroupId, CString strTitle);
	// ���������
	BOOL InserGroup_Interface(int nIndex, int nGroupId, CString strTitle);
	// ���Һ���node
	DuiLib::CTreeNodeUI* FindItem_Interface(int nGroupId, DWORD dwData);
	// ������ɾ������
	void DelItem_Interface(int nGroupId, DWORD dwData);
	// ���º��ѽڵ�����
	BOOL UpdateItem(DuiLib::CTreeNodeUI *pNode, FRIEND_INFO *pFriend);
	// ����һ�������еĽڵ�
	BOOL UpdateGroupAllItem(int nGroupId);
	// �����б���Ӻ���
	BOOL AddItem_Interface(FRIEND_INFO *pFriend);
	// ���½�����ڵ�
	void UpdateGroup(int nGroupID);
	// ���½���������ڵ�
	void UpdateAllGroup();
	void OnTimer( UINT_PTR idEvent );
	
public:
	void SetItemUmMsg(FRIEND_INFO *pFriend, bool bIsUnMsg);//�����ҵ��Ŷ��к���δ����Ϣ��ɫ��ʶ
	BOOL GetFriendUnMsgVisable(FRIEND_INFO *pFriend);//��ȡ�ҵ��Ŷ��к���δ����Ϣ��ɫ��ʶ
	BOOL GetUnMsgVisble(DuiLib::CTreeNodeUI* pNodeGroup);//�жϺ��ѵĸ��ڵ����Ƿ����������Ѵ���δ����Ϣ
private:
	DuiLib::CPaintManagerUI& paint_manager_;
	BOOL			m_bAltDown;		// alt���Ƿ���
	BOOL			m_bShiftDown;	// shift���Ƿ���
	BOOL			m_bCtrlDown;	// ctrl���Ƿ���
	FL_ITEM_POS		m_posLast;		// �ϴ�ѡ��Ľڵ�

	FL_ITEM_POS		m_posItemSelected;
	FL_ITEM_POS		m_posDragTo;

	COLORREF		m_clrBkGround;

	int				m_nItemHeight;
	int             m_nTextHeight;
	int             m_nSelItemHeight;

	vector<FL_GROUP_INFO *> m_vecGroup;

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

	HWND m_hParent;
	DuiLib::CButtonGifUI *pIcon;
};