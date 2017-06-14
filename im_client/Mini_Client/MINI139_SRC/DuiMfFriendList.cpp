#include "stdafx.h"
#include "DuiMfFriendList.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "im\WebProcess.h"
#include "im\imfriendlistctrl.h"
#include "im\LocalData.h"
#pragma warning (disable: 4018)

const DWORD CLR_MULSEL_BAK=RGB(220,220,220);
#define CLR_TEXT  RGB(0,0,0)


#define ITEM_H 26
#define SEL_ITEM_H 0
#define X_SPACE 8
#define GRP_BTN_W 12
#define GRP_BTN_H 12
#define LIST_START_X 10
#define ICON_X 24

#define TIMER_GROUPEDIT_SETFOCUS 0x01

extern CWebUserHeadIconProcess g_webUserHeadIconProcess;
extern string CutRoundImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE);
extern  CLocalData	g_LocalData;

void ShowMessage(CString strMessage,UINT nType/* =MB_OK|MB_ICONINFORMATION */);

void GetOnlineCount(FL_GROUP_INFO *pGroup, int& nFrdCnt, int& nFrdOnCnt)
{ 
	//子组内成员

	for(int i=0; i<pGroup->vecSubGroup.size(); i++)
	{
		FL_GROUP_INFO* pSubGroup = pGroup->vecSubGroup[i];
		GetOnlineCount(pSubGroup,nFrdCnt,nFrdOnCnt);
	}

 	//组内成员
	for ( int iFriend=0; iFriend<pGroup->vecItem.size(); iFriend++)
	{
		FRIEND_INFO *pFriend = (FRIEND_INFO *)pGroup->vecItem[iFriend]->dwData;
		
		if ( pFriend )
		{
			nFrdCnt++;
			if(pFriend->status != XTREAM_HIDE && pFriend->status != XTREAM_OFFLINE)
				nFrdOnCnt++;
 		}
	}
 }

CDuiGroupEdit::CDuiGroupEdit(CDuiFriendList *pList)
{
	m_pList = pList;
}

void CDuiGroupEdit::DoEvent(DuiLib::TEventUI& event)
{
	DuiLib::CEditUI::DoEvent(event);

	if(event.Type == DuiLib::UIEVENT_KILLFOCUS)
	{
		if(IsVisible())
			m_pList->OnGroupNameEdit((WPARAM)this, GetTag());
	}
}

CDuiFriendList::CDuiFriendList(HWND hParent, DuiLib::CPaintManagerUI& paint_manager)
	: DuiLib::CTreeViewUI()
	, paint_manager_(paint_manager)
	, m_hParent(hParent)
{
	m_nHeight = -1;
	m_nWidth = -1;
	m_bRedraw = true;

	m_clrBkGround = RGB(248,248,248);//0xFFD49E;//
/*
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
*/
	m_OriginY = 0;
	m_nClientRealHeigh = 0;
	
	m_bBeginDrag       = false;

	m_bInvalidateLock  = false;

	m_bMouseAlbumDown  = false;
	m_bMouseArticleDown = false;
	m_bMouseInfoDown   = false;
	m_bMouseBlogDown   = false ;
	m_bMouseIconDown   = false ;
	m_bMouseWebInfoDown = false;
	m_bMouseVedioDown  = false;
	m_bMouseMobileDown = false;
//	m_bMouseFocusDown  = false;

	m_bRefreshFriendListFlag = false;

	m_bMouseOnIcon    = false;
	m_bMouseOnBlog    = false;
	m_bMouseOnInfo    = false;
	m_bMouseOnAlbum	  = false;
	m_bMouseOnWebInfo = false;
//	m_bMouseOnFocus   = false;
	m_bMouseOnArticle = false;
	m_bMouseOnVedio   = false;
	m_bMouseOnMobile  = false;


	m_cLastFindChar=0;
	m_nLastFindGroup=0;
	m_nLastFindItem=0;
	m_posItemSelected.nGroupID=-1;
	m_posItemSelected.nItem=-1;

	m_bShiftDown = FALSE;
	m_bAltDown	 = FALSE;
	m_bCtrlDown	 = FALSE;

	m_bmpWebPicture.LoadBitmap(IDB_WEBPIC);
	m_bmpWebArticle.LoadBitmap(IDB_WEBARTICLE);
	//m_bmpFocusOn.LoadBitmap(IDB_FOCUSON);
	//m_bmpFocusOff.LoadBitmap(IDB_FOCUSOFF);
	m_bTracking = FALSE;
}

CDuiFriendList::~CDuiFriendList()
{
	DeleteItems(m_vecGroup);
}

//BEGIN_MESSAGE_MAP(CDuiFriendList, CIMListCtrl)
//	ON_WM_ERASEBKGND()
//	ON_WM_MOUSEMOVE()
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONUP()
//	ON_WM_RBUTTONDOWN()
//	ON_WM_RBUTTONUP()
//	ON_WM_LBUTTONDBLCLK()
//	ON_WM_PAINT()
//	ON_WM_CREATE()
//	ON_WM_VSCROLL()
//	ON_WM_MOUSEWHEEL()
//	ON_WM_SIZE()
//	ON_MESSAGE(WM_GROUPNAME_EDIT,OnGroupNameEdit)
//	ON_WM_SETCURSOR()
//	ON_WM_TIMER()
//	ON_WM_KEYDOWN()
//	ON_WM_KEYUP()
//	ON_WM_MOUSEHOVER()
//END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////


//按照ID  查找组(子组),  zhangmin
FL_GROUP_INFO * CDuiFriendList::GetGroup(vector<FL_GROUP_INFO *> vecGroup, int nGroupID )
{
	if(nGroupID > 100)
		nGroupID = nGroupID;
	for( int i=0; i<vecGroup.size(); i++)
	{
		if( vecGroup[i]->dwData == nGroupID)
		{
			return vecGroup[i];
		}
		else
		{
			FL_GROUP_INFO* pSubGroup =  GetGroup(vecGroup[i]->vecSubGroup, nGroupID);
			if(pSubGroup != NULL)
				return pSubGroup;
		}
	}
	//ASSERT(1);
	return NULL;

}

//按照ID  查找好友zhangmin
FL_ITEM_INFO * CDuiFriendList::GetItem(vector<FL_GROUP_INFO *> vecGroup, int nGroupID, int nItemID)
{
	
	for(int i=0; i<vecGroup.size();i++)
	{
		if( vecGroup[i]->dwData == nGroupID)
		{
			if(nItemID < vecGroup[i]->vecItem.size())
				return vecGroup[i]->vecItem[nItemID];
			else 
				return NULL;
		}
		else
		{
			FL_ITEM_INFO * pItem =  GetItem(vecGroup[i]->vecSubGroup, nGroupID, nItemID);
			if(pItem != NULL)
				return pItem;
		}
	}
	ASSERT(1);
	return NULL;


}


//按照friendID  查找好友zhangmin
FL_ITEM_INFO * CDuiFriendList::GetItem(FRIEND_INFO *pFriend)
{

	FL_GROUP_INFO* pGroup =	GetGroup(m_vecGroup, pFriend->group_index);

	if(pGroup == NULL)
		return NULL;

	for(int i=0;i<pGroup->vecItem.size(); i++)
	{

		FL_ITEM_INFO* pItem = pGroup->vecItem[i];
		if(pFriend == (FRIEND_INFO *)pItem->dwData)
			return pItem;
	}

	ASSERT(1);
	return NULL;


}

int CDuiFriendList::DelGroup(int nParentGroupId, int nGroupId)
{
	//if ( GroupExist(nGroup)==false )
		//return ;
	//没有子组和成员的组才能删除成功
	vector<FL_GROUP_INFO *> *pVecGroup;
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroupId);
	ASSERT(pGroup != NULL);
	vector<FL_GROUP_INFO *> vecpSubGroup = pGroup->vecSubGroup;
	for(int i = 0; i < (int)vecpSubGroup.size(); ++i)
	{
		DelGroup(pGroup->dwData, vecpSubGroup[i]->dwData);
	}

	if(nParentGroupId != -1)
	{
		FL_GROUP_INFO *pParentGroup = GetGroup(m_vecGroup, nParentGroupId);
		if(pParentGroup == NULL)
			return -1;
		pVecGroup = &(pParentGroup->vecSubGroup);
	}
	else
	{
		pVecGroup = &(m_vecGroup);
	}

	vector<FL_GROUP_INFO *>::iterator it = find(pVecGroup->begin(), pVecGroup->end(), pGroup);
	ASSERT( it != pVecGroup->end());
	(*pVecGroup).erase(it);
	delete pGroup;

	DelGroup_Interface(nGroupId);

	return 1;
}

// 判断好友列表中的item是否为分组
BOOL CDuiFriendList::IsGroupItem(DuiLib::CTreeNodeUI *pItem)
{
	return pItem->FindSubControl("itemArea") == NULL;
}

// 界面上删除组
void CDuiFriendList::DelGroup_Interface(int nGroupId)
{
	for(int i = 0; i < GetCount(); ++i)
	{
		DuiLib::CTreeNodeUI *pItem = (DuiLib::CTreeNodeUI *)GetItemAt(i);
		if(!IsGroupItem(pItem))
		{
			continue;
		}
		if(pItem->GetTag() == nGroupId)
		{
			/*if(pItem->GetCountChild() == 0)
			{*/
				Remove(pItem);
			//}
			
			break;
		}
	}

	UpdateAllGroup();
}

// 界面插入组
BOOL CDuiFriendList::InserGroup_Interface(int nNextGroupID, int nGroupId, CString strTitle)
{
	DuiLib::CTreeNodeUI *pNextGroup = FindGroupNode(nNextGroupID);
	if(pNextGroup == NULL)
		return FALSE;

	int nIndex = g_GroupInfo.FindItemIndex(nGroupId);
	if(nIndex == -1)
		return FALSE;

	int nFarentGroupID = g_GroupInfo.group[nIndex].group_father;

	DuiLib::CTreeNodeUI* pListElement = NULL;
	DuiLib::CDialogBuilder dlgBuilder;
	pListElement = static_cast<DuiLib::CTreeNodeUI*>(dlgBuilder.Create(_T("FriendPage_Group.xml"), (UINT)0, this, &paint_manager_));
	if (pListElement == NULL)
		return FALSE;

	pListElement->SetFixedHeight(26);
	pListElement->SetTag(nGroupId);

	// 找到父类
	DuiLib::CTreeNodeUI *pParent = NULL;
	if(nFarentGroupID == -1)
	{
		AddAt(pListElement, pNextGroup);
		pListElement->SetVisibleFolderBtn(true);
		pListElement->GetFolderButton()->SetText(strTitle);

		CRect rcList = GetPos();
		CRect rcNextGroup = pNextGroup->GetPos();
		int nScrollY = rcNextGroup.bottom + 26 - rcList.bottom;
		if(nScrollY > 0)
		{
			SIZE sz = GetScrollPos();
			sz.cy += nScrollY;
			SetScrollPos(sz);
		}
	}
	else
	{
		return FALSE;
	}

	UpdateAllGroup();

	return TRUE;
}


int CDuiFriendList::InsertGroup(int nParentGroup,int nGroupId, CString strTitle)
{
	FL_GROUP_INFO * Group = new FL_GROUP_INFO();
	Group->strTitle = strTitle;
	Group->clrText = CLR_TEXT;
	Group->dwData = nGroupId;
	Group->state  = FLS_CLOSE;

	if(nParentGroup == -1)
	{
		assert(m_vecGroup.size()>=3 );
		//我的好友，插在陌生人前面
		m_vecGroup.insert(m_vecGroup.end()-2,Group);

		InserGroup_Interface(m_vecGroup[m_vecGroup.size()-2]->dwData, nGroupId, strTitle);
	}
	else
	{
		FL_GROUP_INFO * pParentGroup = GetGroup(m_vecGroup, nParentGroup);
		if(pParentGroup != NULL)
		{
			pParentGroup->vecSubGroup.push_back(Group);
			AddGroup_Interface(nGroupId, strTitle);
		}
	}

	return 1;
}

bool CDuiFriendList::FindFriend(FRIEND_INFO*pFriend,int&nGroup,int&nItem)
{
	vector<FL_GROUP_INFO *> vecGroup;
	int nGroupCount = GetAllGroup(vecGroup);

	nGroup=-1;
	nItem =-1;

	for ( int i=0; i<nGroupCount; i++ )
	{
		FL_GROUP_INFO *group = vecGroup[i];
		int nGroupID = group->dwData;
		int nItems = group->vecItem.size();

		for ( int j=0; j<nItems; j++ )
		{
			FRIEND_INFO * pInfo =(FRIEND_INFO *)GetItemData(nGroupID,j);

			if ( pInfo->GetId()==pFriend->GetId() )
			{
				nGroup = nGroupID, nItem=j;
				return true;
			}
		}
	}
	return false;
}

BOOL CDuiFriendList::AddGroup_Interface(int nGroupId, CString strTitle)
{
	int nIndex = g_GroupInfo.FindItemIndex(nGroupId);
	if(nIndex == -1)
		return FALSE;

	int nFarentGroupID = g_GroupInfo.group[nIndex].group_father;

	DuiLib::CTreeNodeUI* pListElement = NULL;
	DuiLib::CDialogBuilder dlgBuilder;
	pListElement = static_cast<DuiLib::CTreeNodeUI*>(dlgBuilder.Create(_T("FriendPage_Group.xml"), (UINT)0, this, &paint_manager_));
	if (pListElement == NULL)
		return FALSE;

	pListElement->SetFixedHeight(26);
	pListElement->SetTag(nGroupId);

	// 找到父类
	DuiLib::CTreeNodeUI *pParent = NULL;
	if(nFarentGroupID == -1)
	{
		Add(pListElement);
		pListElement->SetVisibleFolderBtn(true);
		pListElement->GetFolderButton()->SetText(strTitle);
	}
	else
	{
		for(int i = 0; i < GetCount(); ++i)
		{
			DuiLib::CControlUI *pItem = GetItemAt(i);
			if(pItem->GetTag() == nFarentGroupID)
			{
				pParent = (DuiLib::CTreeNodeUI*)pItem;
				break;
			}
		}

		if(pParent == NULL)
		{
			return FALSE;
		}

		// 获取子节点中第一个好友节点所在的索引
		int nChildCount = pParent->GetCountChild();
		int nFriendItemIndex = -1;
		for(int i = 0; i < nChildCount; ++i)
		{
			DuiLib::CTreeNodeUI *pChildNode = pParent->GetChildNode(i);
			if( !IsGroupItem(pChildNode) )
			{
				nFriendItemIndex = i;
				break;
			}
		}
		if(nFriendItemIndex == -1)
			pParent->AddChildNode(pListElement);
		else
			pParent->AddAt(pListElement, nFriendItemIndex);
		pListElement->SetVisibleFolderBtn(true);
		pListElement->GetFolderButton()->SetText(strTitle);
		DuiLib::CDuiRect rcPadding = pParent->GetFolderButton()->GetPadding();
		rcPadding.left += 20;
		pListElement->GetFolderButton()->SetPadding(rcPadding);

		if(pParent != NULL)
		{
			CRect rcList = GetPos();
			CRect rcParent = pParent->GetPos();
			int nCount = pParent->GetCountChild();
			int nScrollY = rcParent.bottom + 26 * nCount - rcList.bottom;
			if(nScrollY > 0)
			{
				SIZE sz = GetScrollPos();
				sz.cy += nScrollY;
				SetScrollPos(sz);
			}
		}
	}

	UpdateAllGroup();

	return TRUE;
}


int CDuiFriendList::AddGroup(int nGroupId, CString strTitle)
{
	FL_GROUP_INFO * Group;
	Group = GetGroup(m_vecGroup, nGroupId);
	//先判断是否已经存在
	if(Group == NULL)
	{
		FL_GROUP_INFO * Group=new FL_GROUP_INFO();
		Group->nFrdCnt = 0;
		Group->strTitle=strTitle;
		Group->clrText = CLR_TEXT;
		Group->dwData = nGroupId;

		if(!AddGroup_Interface(nGroupId, strTitle))
		{
			delete Group;
			Group = NULL;
			return -1;
		}
		
		m_vecGroup.push_back(Group);
	}

	return 1;
}

//AddGroup是在m_vecGroup尾部添加，好像是废话
int CDuiFriendList::AddSubGroup(int nParentGroupId, int nSubGroupId, CString strTitle)
{
	FL_GROUP_INFO* pGroup = GetGroup(m_vecGroup, nSubGroupId);

	if(pGroup != NULL)
		return -1;


	vector<FL_GROUP_INFO *>* pVecGroup;

	if(nParentGroupId == -1)
	{
		pVecGroup = &m_vecGroup;
	}
	else
	{
		FL_GROUP_INFO* pParentGroup = GetGroup(m_vecGroup, nParentGroupId);
		if(pParentGroup == NULL)
			return -1;
		pVecGroup = &(pParentGroup->vecSubGroup);
	}

	// 创建子组
	FL_GROUP_INFO * pSubGroup = new FL_GROUP_INFO();
	if(pSubGroup == NULL)
		return -1;
	pSubGroup->nFrdCnt = 0;
	pSubGroup->strTitle=strTitle;
	pSubGroup->clrText = CLR_TEXT;
	pSubGroup->dwData = nSubGroupId;

	if(!AddGroup_Interface(nSubGroupId, strTitle))
	{
		delete pSubGroup;
		pSubGroup = NULL;
		return -1;
	}

 	(*pVecGroup).push_back(pSubGroup);

	return 1;
}

// 查找组节点
DuiLib::CTreeNodeUI* CDuiFriendList::FindGroupNode(int nGroupId)
{
	for(int i = 0; i < GetCount(); ++i)
	{
		DuiLib::CTreeNodeUI *pItem = (DuiLib::CTreeNodeUI *)GetItemAt(i);
		if(!IsGroupItem(pItem))
		{
			continue;
		}
		if(pItem->GetTag() == nGroupId)
		{
			return pItem;
		}
	}

	return NULL;
}

void CDuiFriendList::SetGroupTextColor(int nGroup, COLORREF clrText)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup,nGroup);
	if(pGroup != NULL)
		pGroup->clrText = clrText;

	DuiLib::CTreeNodeUI *pGroupNode = FindGroupNode(nGroup);
	if(pGroupNode != NULL)
	{
		pGroupNode->GetFolderButton()->SetTextColor(clrText);
	}
}

void CDuiFriendList::SetGroupState(int nGroup, FLS_STATE state)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroup);
	if(pGroup != NULL)
		pGroup->state = state;

	DuiLib::CTreeNodeUI *pGroupNode = FindGroupNode(nGroup);
	if(pGroupNode != NULL)
	{
		pGroupNode->GetFolderButton()->Selected(state == FLS_EXPAND);
	}
}

void CDuiFriendList::SetGroupName(int nGroup, CString strTitle)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroup);
	if(pGroup != NULL)
	{
		strTitle.Trim();
		pGroup->strTitle = strTitle;
	}
	
	DuiLib::CTreeNodeUI *pGroupNode = FindGroupNode(nGroup);
	if(pGroupNode != NULL)
	{
		pGroupNode->GetFolderButton()->SetText(strTitle);
		UpdateGroup(nGroup);
	}
}

CString CDuiFriendList::GetGroupName(int nGroup)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroup);
	if(pGroup == NULL)
		return "";
	return  pGroup->strTitle;
}

// 查找好友node
DuiLib::CTreeNodeUI* CDuiFriendList::FindItem_Interface(int nGroupId, DWORD dwData)
{
	DuiLib::CTreeNodeUI *pGroupNode = FindGroupNode(nGroupId);
	for(int i = 0; i < pGroupNode->GetCountChild(); ++i)
	{
		DuiLib::CTreeNodeUI *pChildNode = pGroupNode->GetChildNode(i);
		if(!IsGroupItem(pChildNode))
		{
			if(pChildNode->GetTag() == dwData)
			{
				return pChildNode;
			}
		}
	}

	return NULL;
}

// 界面上删除好友
void CDuiFriendList::DelItem_Interface(int nGroupId, DWORD dwData)
{
	DuiLib::CTreeNodeUI *pChildNode = FindItem_Interface(nGroupId, dwData);
	if(pChildNode != NULL)
	{
		Remove(pChildNode);
	}

	UpdateAllGroup();
}

void CDuiFriendList::DeleteItem(int nGroup, int nItem)
{
/*
	if ( ItemExist(nGroup,nItem)==false )
		return ;
*/
	FL_GROUP_INFO * pGroup = GetGroup(m_vecGroup, nGroup);
	FL_ITEM_INFO * pItem = GetItem(m_vecGroup, nGroup, nItem);
	DWORD nData = pItem->dwData;
	//释放Item
	delete pItem;
	//清空组内的Item指针
	pGroup->vecItem.erase(pGroup->vecItem.begin() + nItem);

	DelItem_Interface(nGroup, nData);
}

void CDuiFriendList::DeleteItem(FRIEND_INFO *pFriend)
{
	
	FL_ITEM_INFO* pItem =  GetItem(pFriend);
	FL_GROUP_INFO* pGroup = GetGroup(m_vecGroup, pFriend->group_index);

	//清空组内的Item指针
	vector<_FL_ITEM_INFO *>::iterator it = find(pGroup->vecItem.begin(), pGroup->vecItem.end(), pItem);
	ASSERT( it != pGroup->vecItem.end() );
	if(it != pGroup->vecItem.end())
	{
		pGroup->vecItem.erase(it);
	}
	
	//释放Item
	delete pItem;
	
	DelItem_Interface(pFriend->group_index, (DWORD)pFriend);
}


void CDuiFriendList::DeleteItems(vector<FL_GROUP_INFO *>& vecGroup)
{
	for(int i=0; i<vecGroup.size(); i++)
	{
		//删除本组中的Item
		for ( int j=0; j<vecGroup[i]->vecItem.size(); j++ )
		{
			delete vecGroup[i]->vecItem[j];
		}
		vecGroup[i]->vecItem.clear();
		
		//删除子组中的Item
		DeleteItems(vecGroup[i]->vecSubGroup);

		//释放内存
		delete vecGroup[i];
		
	}
	//清空组信息
	vecGroup.clear();
}


void CDuiFriendList::DeleteAllItems()
{

	DeleteItems(m_vecGroup);
	
	m_OriginY = 0;
	m_nClientRealHeigh = 0;

	m_posItemSelected.nGroupID = -1;
	m_posItemSelected.nItem = -1;

	RemoveAll();
}


int CDuiFriendList::AddItem(int iGroup, CString strText, DWORD dwData)
{
	FL_ITEM_INFO *pItem = GetItem((FRIEND_INFO*)dwData);
	if(pItem != NULL)
	{
		DuiLib::CTreeNodeUI *pNode = FindItem_Interface(iGroup, dwData);
		ASSERT(pNode != NULL);
		UpdateItem(pNode, (FRIEND_INFO*)dwData);
		return 1;
	}
	else
	{
		FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, iGroup);
		if(pGroup == NULL)
			return -1;
		FL_ITEM_INFO * item = new FL_ITEM_INFO();
		item->strText = strText;	
		item->dwData  = dwData;
		item->clrText =  CLR_TEXT;

		pGroup->vecItem.push_back(item);

		AddItem_Interface((FRIEND_INFO*)dwData);

		return 1;
	}
}


int CDuiFriendList::SubGroupAddItem(FL_GROUP_INFO *tmp, int iGroup, CString strText, DWORD dwData)
{
	//组内成员
	if(tmp->dwData == iGroup)
	{
		FL_ITEM_INFO * item = new FL_ITEM_INFO();

		item->strText = strText;	
		item->dwData  = dwData;
		tmp->vecItem.push_back(item);
		return 1;
	}
	else
	{
		for(int i=0; i< tmp->vecSubGroup.size(); i++)
		{	
			//子组内成员
			if( SubGroupAddItem(tmp->vecSubGroup[i], iGroup, strText, dwData) )
				return 1;
		}
	}
	return -1;

}

// 好友列表添加好友
BOOL CDuiFriendList::AddItem_Interface(FRIEND_INFO *pFriend)
{
	// 查找所属组
	DuiLib::CTreeNodeUI *pParent = FindGroupNode(pFriend->group_index);
	if(pParent == NULL)
	{
		return FALSE;
	}

	DuiLib::CTreeNodeUI* pListElement = NULL;
	DuiLib::CDialogBuilder dlgBuilder;
	pListElement = static_cast<DuiLib::CTreeNodeUI*>(dlgBuilder.Create(_T("FriendPage_Item.xml"), (UINT)0, NULL, &paint_manager_));
	if (pListElement == NULL)
		return FALSE;
	pListElement->SetFixedHeight(34);
	pListElement->SetTag((DWORD)pFriend);

	DuiLib::CHorizontalLayoutUI *pArea = static_cast<DuiLib::CHorizontalLayoutUI*>(pListElement->FindSubControl("itemArea"));
	if(pArea == NULL)
		return FALSE;

	DuiLib::CDuiRect rcPadding = pParent->GetFolderButton()->GetPadding();
	rcPadding.left += 28;
	pArea->SetPadding(rcPadding);
	
	if( !pParent->AddAt(pListElement, 0))//pParent->AddChildNode(pListElement) )
	{
		return FALSE;
	}

	return UpdateItem(pListElement, pFriend);
}

// 更新好友节点内容
BOOL CDuiFriendList::UpdateItem(DuiLib::CTreeNodeUI *pNode, FRIEND_INFO *pFriend)
{
	// 是否有备注名称
	CString sName = UserInfoToStr(pFriend);

	pNode->SetFixedHeight(34);
	pNode->SetTag((DWORD)pFriend);

    pIcon = static_cast<DuiLib::CButtonGifUI*>(pNode->FindSubControl("lbHeaderIcon"));
	DuiLib::CLabelUI *pNickName = static_cast<DuiLib::CLabelUI*>(pNode->FindSubControl("lbNickName"));
	DuiLib::CLabelUI *pNote = static_cast<DuiLib::CLabelUI*>(pNode->FindSubControl("note"));
	DuiLib::CHorizontalLayoutUI *pArea = static_cast<DuiLib::CHorizontalLayoutUI*>(pNode->FindSubControl("itemArea"));
	DuiLib::CLabelUI *pStatus = static_cast<DuiLib::CLabelUI*>(pNode->FindSubControl("lbStatus"));
	if(pNickName == NULL || pNote == NULL || pArea == NULL)
		return FALSE;

	CString strFile = g_webUserHeadIconProcess.GetFaceFile(pFriend);
	CString strGifFile =  pFriend->face_url;
	if(strGifFile.Find(".gif") != -1)
	{
		strGifFile =  strFile + ".gif";
	    if ( !FileExist(strGifFile) )///不存在时创建
		{
		   CopyFile(strFile,strGifFile,FALSE);
		}
	}
	if ( FileExist(strFile) )
	{
		BOOL bOnline = TRUE;
		if(pFriend->GetId() == g_ImUserInfo.GetId())
		{
			bOnline = TRUE;
		}
		else if(pFriend->status == XTREAM_OFFLINE || pFriend->status == XTREAM_HIDE)
		{
			bOnline = FALSE;
		}
		if ( FileExist(strGifFile))/////GIF在线
		{
			if(bOnline)
			{
				pIcon->SetBkImage("");
				pIcon->SetNormalGifFile(strGifFile);
			}
			else//不在线
			{
				string strHeadIcon = CutRoundImg(strFile, 30, 30, !bOnline);////是否在线的判断
				pIcon->SetBkImage(strHeadIcon.c_str());
				pIcon->SetNormalGifFile("");
			}
		}
		else///非GIF
		{
	    	string strHeadIcon = CutRoundImg(strFile, 30, 30, !bOnline);////是否在线的判断
	    	pIcon->SetBkImage(strHeadIcon.c_str());
			pIcon->SetNormalGifFile("");
		}
	}

	pNickName->SetText(sName);
	int nWidth = pNickName->CalcRealWidth();
	pNickName->SetFixedWidth(nWidth + 10);
	pNote->SetText(pFriend->user_sign);

	// 如果好友为自己的ID则把状态该为自己的状态
	if(pFriend->GetId() == g_ImUserInfo.GetId())
	{
		pFriend->status = g_ImUserInfo.status;
	}

	pStatus->SetBkImage("");
	switch(pFriend->status)
	{
	case XTREAM_LEAVE:
		{
			pStatus->SetBkImage("FriendStatus\\status_leave_small.png");
		}
		break;
	case XTREAM_BUSY:
		{
			pStatus->SetBkImage("FriendStatus\\status_busy_small.png");
		}
		break;
	case XTREAM_HIDE:
		{
			if(pFriend->GetId() == g_ImUserInfo.GetId())
			{
				pStatus->SetBkImage("FriendStatus\\status_stealth_small.png");
			}
		}
		break;
	}

	UpdateAllGroup();
}

void CDuiFriendList::SetItem(int nGroup, int nIndex, CString strText, DWORD dwData /* = 0 */)
{
	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup, nIndex);

	if(pItem != NULL)
	{
		DuiLib::CTreeNodeUI *pNode = FindItem_Interface(nGroup, pItem->dwData);
		pItem->strText = strText;
		pItem->dwData = dwData;

		if(pNode != NULL)
		{
			UpdateItem(pNode, (FRIEND_INFO*)dwData);
		}
	}
}

void CDuiFriendList::SwapItem(int nGroup1, int nItem1, int nGroup2, int nItem2)
{
	FL_ITEM_INFO *pItem1 = GetItem(m_vecGroup, nGroup1, nItem1);	
	FL_ITEM_INFO *pItem2 = GetItem(m_vecGroup, nGroup2, nItem2);

	DuiLib::CTreeNodeUI *pNode1 = FindItem_Interface(nGroup1, pItem1->dwData);
	DuiLib::CTreeNodeUI *pNode2 = FindItem_Interface(nGroup2, pItem2->dwData);

	FL_ITEM_INFO *pItemTmp = pItem1;
	pItem1 = pItem2;
	pItem2 = pItemTmp;

	UpdateItem(pNode1, (FRIEND_INFO*)pItem1->dwData);
	UpdateItem(pNode2, (FRIEND_INFO*)pItem2->dwData);
}

bool ItemSortByState(FL_ITEM_INFO *pItem1, FL_ITEM_INFO *pItem2 )
{
	FRIEND_INFO *pFriend1, *pFriend2;
	pFriend1 = (FRIEND_INFO*)pItem1->dwData;
	pFriend2 = (FRIEND_INFO*)pItem2->dwData;

	if(g_ImUserInfo.GetId() == ((FRIEND_INFO*)pFriend1)->GetId())
		return /*pFriend1->status > pFriend2->status*/true;
	else if(g_ImUserInfo.GetId() == ((FRIEND_INFO*)pFriend2)->GetId())
		return /*pFriend1->status < pFriend2->status*/false;

	return pFriend1->status < pFriend2->status;
}

bool ItemSortByText(FL_ITEM_INFO *pItem1, FL_ITEM_INFO *pItem2 )
{
	FRIEND_INFO *pFriend1, *pFriend2;
	pFriend1 = (FRIEND_INFO*)pItem1->dwData;
	pFriend2 = (FRIEND_INFO*)pItem2->dwData;

	if(g_ImUserInfo.GetId() == ((FRIEND_INFO*)pFriend1)->GetId())
		return /*pFriend1->status > pFriend2->status*/true;
	else if(g_ImUserInfo.GetId() == ((FRIEND_INFO*)pFriend2)->GetId())
		return /*pFriend1->status < pFriend2->status*/false;

	bool bRet = ( strcmp(pItem1->strText,pItem2->strText) < 0 ) ? true : false;
	return bRet;
}

// 更新一个组所有的节点
BOOL CDuiFriendList::UpdateGroupAllItem(int nGroupId)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroupId);
	if(pGroup == NULL)
		return FALSE;

	DuiLib::CTreeNodeUI *pGroupNode = FindGroupNode(nGroupId);
	if(pGroupNode == NULL)
		return FALSE;

	// 获取非分组子节点
	vector<int> vecIdx;
	int nChildNodeCount = pGroupNode->GetCountChild();
	for(int i = 0; i < nChildNodeCount; ++i)
	{
		DuiLib::CTreeNodeUI *pChildNode = pGroupNode->GetChildNode(i);
		if(!IsGroupItem(pChildNode))
		{
			vecIdx.push_back(i);
		}
	}

	if(pGroup->vecItem.size() != vecIdx.size())
	{
		return FALSE;
	}

	// 更新节点
	for(int i = 0; i < (int)vecIdx.size(); ++i)
	{
		DuiLib::CTreeNodeUI *pChildNode = pGroupNode->GetChildNode(vecIdx[i]);
		FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroupId, i);
		UpdateItem(pChildNode, (FRIEND_INFO*)(pItem->dwData));
	}

	UpdateAllGroup();
}

void CDuiFriendList::SortGroupByState(int nGroupId)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroupId);
	if(pGroup == NULL)
		return ;

	//按状态排序
	std::sort(pGroup->vecItem.begin(),pGroup->vecItem.end(),ItemSortByState);

	//按名字排序
	vector<FL_ITEM_INFO *>::iterator it;
	int nOnlineCnt = 0;
	for(it=pGroup->vecItem.begin();it<pGroup->vecItem.end(); it++)
	{
		FRIEND_INFO* pFriend = (FRIEND_INFO*)(*it)->dwData;
		if(pFriend->status == XTREAM_OFFLINE)
			break;
		nOnlineCnt++;
	}

	std::sort(pGroup->vecItem.begin(),it,ItemSortByText);
	std::sort(it,pGroup->vecItem.end(),ItemSortByText);

#ifdef _VER_INTERNAL_			// 内部版，按照职位排序
	//vector<FL_ITEM_INFO *> vecpGM;			// 总经理
	//vector<FL_ITEM_INFO *> vecpDeputyGM;		// 副总经理
	//vector<FL_ITEM_INFO *> vecpSeniorDirector;	// 高级总监
	//vector<FL_ITEM_INFO *> vecpDirector;	// 总监
	//vector<FL_ITEM_INFO *> vecpDeputyDirector;	// 副总监
	//vector<FL_ITEM_INFO *> vecpSeniorManager;	// 高级经理
	//vector<FL_ITEM_INFO *> vecpArchitect;	// 应用架构师
	//vector<FL_ITEM_INFO *> vecpManager;	// 经理
	//vector<FL_ITEM_INFO *> vecpOther;	// 其他
	//for(int i = 0; i < pGroup->vecItem.size(); ++i)
	//{
	//	FRIEND_INFO *pFriend = (FRIEND_INFO*)pGroup->vecItem[i]->dwData;
	//	
	//	// 解析出职务，分机号
	//	string strJob, strExt;
	//	SplitExtAndJob(pFriend->flash_url, strExt, strJob);
	//	if(!strJob.empty())
	//	{
	//		if(strJob == "总经理")	vecpGM.push_back(pGroup->vecItem[i]);
	//		else if(strJob == "副总经理")	vecpDeputyGM.push_back(pGroup->vecItem[i]);
	//		else if(strJob == "高级总监")	vecpSeniorDirector.push_back(pGroup->vecItem[i]);
	//		else if(strJob == "总监")	vecpDirector.push_back(pGroup->vecItem[i]);
	//		else if(strJob == "副总监")	vecpDeputyDirector.push_back(pGroup->vecItem[i]);
	//		else if(strJob == "高级经理")	vecpSeniorManager.push_back(pGroup->vecItem[i]);
	//		else if(strJob == "应用架构师")	vecpArchitect.push_back(pGroup->vecItem[i]);
	//		else if(strJob == "经理")	vecpManager.push_back(pGroup->vecItem[i]);
	//		else vecpOther.push_back(pGroup->vecItem[i]);
	//	}
	//	else
	//	{
	//		vecpOther.push_back(pGroup->vecItem[i]);
	//	}
	//}
	//vector<FL_ITEM_INFO *>* arrLs[] = {&vecpGM, &vecpDeputyGM, &vecpDirector, &vecpDeputyDirector, &vecpSeniorManager, &vecpArchitect, &vecpManager, &vecpOther};
	//pGroup->vecItem.clear();
	//int nCount = sizeof(arrLs)/sizeof(vector<FL_ITEM_INFO *>*);
	//for(int i = 0; i < nCount; ++i)
	//{
	//	pGroup->vecItem.insert(pGroup->vecItem.end(), arrLs[i]->begin(), arrLs[i]->end());
	//}
#endif
	
	
	/*
	for(int i=0; i<pGroup->vecItem.size(); i++)
	{
		pItem1 = pGroup->vecItem[i];
		pFriend1 = (FRIEND_INFO*)pItem1->dwData;

		if((DWORD)pFriend1 == -1)
			return;

		for ( int j=i+1; j<pGroup->vecItem.size(); j++ )
		{
			pItem2 = pGroup->vecItem[j];
			pFriend2 =  (FRIEND_INFO*)pItem2->dwData;
			if((DWORD)pFriend2 == -1)
				return;

			if ( pFriend1->status > pFriend2->status )
			{	
				pGroup->vecItem[i] = pItem2;
				pGroup->vecItem[j] = pItem1;

				pFriend1 = pFriend2;
			}
		}
	}
	*/
	UpdateGroupAllItem(nGroupId);
}


void CDuiFriendList::SetItemText(int nGroup, int nIndex, CString strText)
{
	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup, nIndex);
	
	if(pItem != NULL)
	{
		pItem->strText = strText;
	
		DuiLib::CTreeNodeUI *pNode = FindItem_Interface(nGroup, pItem->dwData);
		if(pNode != NULL)
		{
			UpdateItem(pNode, (FRIEND_INFO*)pItem->dwData);
		}
	}
}

void CDuiFriendList::SetItemText(FRIEND_INFO *pFriend, CString strText)
{
	FL_ITEM_INFO *pItem = GetItem(pFriend);
	if(pItem != NULL)
	{
		pItem->strText = strText;
		
		DuiLib::CTreeNodeUI *pNode = FindItem_Interface(pFriend->group_index, pItem->dwData);
		if(pNode != NULL)
		{
			UpdateItem(pNode, (FRIEND_INFO*)pItem->dwData);
		}
	}
}


CString CDuiFriendList::GetItemText(int nGroup, int nIndex)
{
	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup, nIndex);
		return "";
	if(pItem != NULL)
		return pItem->strText;
}

void CDuiFriendList::UpdateItemImage(FRIEND_INFO *pFriend)
{
	FL_ITEM_INFO *pItem = GetItem(pFriend);

	if( pFriend != NULL && pItem != NULL)
	{
		DuiLib::CTreeNodeUI *pNode = FindItem_Interface(pFriend->group_index, pItem->dwData);
		if(pNode != NULL)
		{
			UpdateItem(pNode, (FRIEND_INFO*)pItem->dwData);
		}
	}
}

// 分组是否展开
BOOL CDuiFriendList::IsExpand(int nGroupID)
{
	DuiLib::CTreeNodeUI *pGroupNode = FindGroupNode(nGroupID);
	if(pGroupNode == NULL || !IsGroupItem(pGroupNode))
	{
		return FALSE;
	}

	return pGroupNode->GetFolderButton()->IsSelected();
}

// 展开或关闭分组
void CDuiFriendList::ExpandGroup(int nGroupID, BOOL expand)
{
	DuiLib::CTreeNodeUI *pGroupNode = FindGroupNode(nGroupID);
	if(pGroupNode == NULL || !IsGroupItem(pGroupNode))
	{
		return;
	}
	pGroupNode->GetFolderButton()->Selected(expand);
}

// 选中组/或取消选中
void CDuiFriendList::SelectGroup(int nGroupID, BOOL bSelect)
{
	DuiLib::CTreeNodeUI *pGroupNode = FindGroupNode(nGroupID);
	if(pGroupNode == NULL || !IsGroupItem(pGroupNode))
	{
		return;
	}
	pGroupNode->Select(bSelect);
}

void CDuiFriendList::SetCurSel(int nGroupID,int nItem)
{
	if ( nGroupID==-1 )
		return ;

	m_posItemSelected.nGroupID = nGroupID;
	m_posItemSelected.nItem  = nItem;

	FL_GROUP_INFO *group = GetGroup(m_vecGroup, m_posItemSelected.nGroupID);
	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroupID, nItem);
	if(group != NULL && pItem != NULL)
	{
		if ( !IsExpand(nGroupID) )
		{
			group->state = FLS_EXPAND;
			ExpandGroup(nGroupID, TRUE);
		}

		DuiLib::CTreeNodeUI *pNode = FindItem_Interface(nGroupID, pItem->dwData);
		if(pNode != NULL)
		{
			pNode->Select(true);
		}
	}
}

void CDuiFriendList::SetCurSel(FRIEND_INFO *pFriend)
{	
	FL_GROUP_INFO* pGroup =	GetGroup(m_vecGroup, pFriend->group_index);

	if(pGroup == NULL)
		return;
	m_posItemSelected.nGroupID =  pFriend->group_index;

	for(int i=0;i<pGroup->vecItem.size(); i++)
	{
		FL_ITEM_INFO* pItem = pGroup->vecItem[i];
		if(pFriend == (FRIEND_INFO *)pItem->dwData)
		{
			m_posItemSelected.nItem = i;
			break;
		}
	}
	
	SetCurSel(m_posItemSelected.nGroupID, m_posItemSelected.nItem);
}


FL_ITEM_POS CDuiFriendList::GetCurSel()
{
	return m_posItemSelected;
}

DWORD CDuiFriendList::GetItemData(int nGroup, int nIndex)
{
	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup, nIndex);
	if(pItem != NULL)
		return pItem->dwData;
	return 0;
}

void CDuiFriendList::SetItemData(int nGroup, int nIndex, DWORD dwData)
{
	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup, nIndex);
	if(pItem != NULL)
		pItem->dwData = dwData;
}

int CDuiFriendList::MoveItem(int nGroupSrc, int nItemSrc, int nGroupDest)
{

	FL_GROUP_INFO* pGroupSrc = GetGroup(m_vecGroup, nGroupSrc);
	FL_GROUP_INFO* pGroupDest = GetGroup(m_vecGroup, nGroupDest);
	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroupSrc, nItemSrc);

	ASSERT(pGroupSrc != NULL && pGroupDest != NULL && pItem != NULL);
	if(pGroupSrc == NULL || pGroupDest == NULL || pItem == NULL)
		return -1;
	
	ASSERT(nItemSrc < pGroupSrc->vecItem.size());

	if(nItemSrc >= pGroupSrc->vecItem.size())
		return -1;

	//Dest添加
	pGroupDest->vecItem.push_back(pGroupSrc->vecItem[nItemSrc]);
	pGroupDest->state = FLS_EXPAND;

	//Src  清除
	pGroupSrc->vecItem.erase(pGroupSrc->vecItem.begin() + nItemSrc );



	m_posItemSelected.nGroupID = nGroupDest;
	m_posItemSelected.nItem  = pGroupDest->vecItem.size()-1;

	// 界面删除
	DuiLib::CTreeNodeUI *pNode = FindItem_Interface(nGroupSrc, pItem->dwData);
	Remove(pNode);
	// 界面添加
	AddItem_Interface((FRIEND_INFO*)pItem->dwData);

	return 1;
}

//void CDuiFriendList::OnRButtonDown(UINT nFlags, CPoint point)
//{
//	// 没有多选的时候
//	if(m_multSelList.Size()==0)
//	{
//		int nHotGroup, nHotItem;
//		HitTest(m_vecGroup, point, nHotGroup, nHotItem);
//
//		point.y+=m_OriginY;
//
//		m_posItemSelected.nGroupID = nHotGroup;
//		m_posItemSelected.nItem  = nHotItem;
//		InvalidateList();
//
//
//		::PostMessage( GetParent()->m_hWnd,
//			WM_IMFRIEND_LIST_NOTIFY,
//			NM_FL_RBUTTONDOWN,
//			MAKEWORD(nHotGroup,nHotItem));
//	}
//
//	
//	CIMListCtrl::OnRButtonDown(nFlags, point);
//}

//void CDuiFriendList::OnRButtonUp(UINT nFlags, CPoint point)
//{
//	if(m_multSelList.Size()>0)
//	{
//		::PostMessage( GetParent()->m_hWnd,
//			WM_IMFRIEND_LIST_NOTIFY,
//			NM_FL_RBUTTONUP_M,
//			(LPARAM)&m_multSelList);
//	}
//	else
//	{
//		int nHotGroup, nHotItem;
//		HitTest(m_vecGroup,point, nHotGroup, nHotItem);
//
//		::PostMessage( GetParent()->m_hWnd,
//			WM_IMFRIEND_LIST_NOTIFY,
//			NM_FL_RBUTTONUP,
//			MAKEWORD(nHotGroup,nHotItem));
//	}
//	
//	CIMListCtrl::OnRButtonUp(nFlags, point);
//}

//void CDuiFriendList::OnLButtonDblClk(UINT nFlags, CPoint point)
//{
//	int nHotGroup, nHotItem;
//	HitTest(m_vecGroup, point, nHotGroup, nHotItem);
//
//	m_posItemSelected.nGroupID = nHotGroup;
//	m_posItemSelected.nItem  = nHotItem;
//	InvalidateList();
//
//	if ( nHotGroup>-1 && nHotItem>-1 )
//	{	
//		FL_ITEM_INFO *pItem = GetItem(m_vecGroup,nHotGroup ,nHotItem);
//
//		if ( pItem->rcBtnBlog.PtInRect(point) )//|| pItem->rcIcon.PtInRect(point) )
//			return ;
//	}
//
//	::PostMessage( GetParent()->m_hWnd,
//		WM_IMFRIEND_LIST_NOTIFY,
//		NM_FL_LBUTTONDBLCLICK,
//		0);
//
//	CIMListCtrl::OnLButtonDblClk(nFlags,point);
//}

//void CDuiFriendList::HitTest(vector<FL_GROUP_INFO *>vecGroup,const POINT& pt, int& nGroup, int& nItem)
//{
//	nGroup = nItem = -1;
//	CPoint point(pt);
//	point.y += m_OriginY;
//	
//	for (int iGroup = 0; iGroup < vecGroup.size(); iGroup++)
//	{
//		FL_GROUP_INFO *pGroup=  vecGroup[iGroup];
//		
//		if (pGroup->rcBar.PtInRect(point))
//		{
//			nGroup = pGroup->dwData;
//			return;
//		}
//		
//		if (pGroup->state == FLS_EXPAND)
//		{
//			HitTest(pGroup->vecSubGroup, pt, nGroup,nItem);
//			if(nGroup != -1 || nItem != -1)
//				return;
//			for (int i = 0; i < pGroup->vecItem.size(); i++)
//			{
//				FL_ITEM_INFO *pItem= pGroup->vecItem[i];
//				
//				if (pItem->rcItem.PtInRect(point))
//				{
//					nGroup = pGroup->dwData;
//					nItem = i;
//					return ;
//				}
//			}
//		}
//	}
//#if 0
//	if ( nItem==-1 && nGroup==-1)
//	{
//		CRect rc=m_vecGroup[0]->vecItem[0]->rcItem;
//		TRACE("(%d,%d,%d,%d),(%d,%d)\n",rc.left,
//			rc.top,
//			rc.right,
//			rc.bottom,
//			point.x,
//			point.y);
//	}
//#endif
//
//}

int CDuiFriendList::GetItemCount(int nGroupID)
{
	vector<FL_GROUP_INFO *> vecGroup;
	int nGroupCount = GetAllGroup(vecGroup);

	for(int i = 0; i < nGroupCount; ++i)
	{
		FL_GROUP_INFO *group = vecGroup[i];
		if(group->dwData == nGroupID)
		{
			return group->vecItem.size();
		}
	}
	return 	-1;
}

void CDuiFriendList::SetGroupData(int nGroup,DWORD dwData)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroup);
	pGroup->dwData=dwData;
}

void CDuiFriendList::SetGroupFriendsCount(int nGroup,int nCount)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroup);
	pGroup->nFrdCnt = nCount;
}

FL_GROUP_INFO* CDuiFriendList::GetGroupData(int nGroup)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroup);
	
	if (pGroup !=NULL)
		return 	pGroup;
	return NULL;
}

int CDuiFriendList::GetGroupState(int nGroup)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroup);
	
	if (pGroup !=NULL)
		return 	pGroup->state;
	return -1;
}

void CDuiFriendList::OnBeginDrag()
{
	for ( int i=0; i<m_vecGroup.size(); i++ )
	{
		m_vecGroup[i]->state_before_drag = m_vecGroup[i]->state;
		m_vecGroup[i]->state = FLS_CLOSE;
	}
}

void CDuiFriendList::OnEndDrag()
{
	for ( int i=0; i<m_vecGroup.size(); i++ )
	{
		m_vecGroup[i]->state = m_vecGroup[i]->state_before_drag;
	}
}

//void CDuiFriendList::OnTimer(UINT nIDEvent)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	switch(nIDEvent)
//	{
//	case 0x0F:
//		{
//			CRect rect;
//			GetClientRect(&rect);
//			ClientToScreen(&rect);
//
//			CPoint pt;
//			GetCursorPos(&pt);
//
//			if ( !rect.PtInRect(pt) )
//			{
//				m_bMouseDown    = false;
//				m_bBeginDrag    = false;
//
//				m_bMouseAlbumDown  = false;
//				m_bMouseInfoDown   = false;
//				m_bMouseBlogDown   = false ;
//				m_bMouseIconDown   = false ;
//				m_bMouseWebInfoDown = false;
//				m_bMouseVedioDown   = false;
//				m_bMouseMobileDown  = false;
////				m_bMouseFocusDown = false;
//				m_bMouseArticleDown = false;
//
//				m_bMouseOnIcon = m_bMouseOnBlog = m_bMouseOnInfo = m_bMouseOnAlbum = m_bMouseOnVedio = m_bMouseOnMobile = false;
//
//				if ( m_bBeginDrag )
//				{
//					m_bBeginDrag = false;
//					OnEndDrag();
//				}
//
//				InvalidateList();
//
//				KillTimer(0x0F);
//			}
//
//			if ( m_bMouseDown==false )
//			{
//				KillTimer(0x0F);
//			}
//		}
//		break;
//	case 0x0E:
//		{
//			g_strDot += ".";
//
//			if (g_iDotCount++>2)
//			{
//				g_iDotCount  = 0  ;
//				g_strDot     = "" ;
//			}
//			
//			FL_GROUP_INFO *pGroup = m_vecGroup[0];
//
//			CRect rcBar = pGroup->rcBar;
//			rcBar.top-=m_OriginY;
//			rcBar.bottom-=m_OriginY;
//
//			this->m_bRedraw=true;
//			this->InvalidateRect(&rcBar);
//		}
//		break;
//	}
//
//	CIMListCtrl::OnTimer(nIDEvent);
//}

void	CDuiFriendList::CloseAllGroup()
{
	vector<FL_GROUP_INFO *>::iterator it;
	FL_GROUP_INFO* pGroupInfo;
	
	for( it = m_vecGroup.begin();it!=m_vecGroup.end();++it)
	{
		pGroupInfo = *it;
		pGroupInfo->state = FLS_CLOSE;
		ExpandGroup(pGroupInfo->dwData, FALSE);
	}
}


LRESULT CDuiFriendList::OnGroupNameEdit(WPARAM w,LPARAM l)
{
	CDuiGroupEdit *pEdit = (CDuiGroupEdit*)w;
	CString strGroupName = pEdit->GetText().GetData();
	pEdit->SetVisible(false);
	int     nGroup       = l;
	strGroupName.Trim();
	// 去掉全角空格
	TrimSpace2(strGroupName);
	
	if (strGroupName=="")
	{
		ShowMessage("组名称不可为空！",MB_OK|MB_ICONWARNING);
		return 1;
	}

	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroup);
	if(pGroup->strTitle == strGroupName)
		return 1;	// 组名没变

	for ( int i=0; i<g_GroupInfo.count; i++ )
	{
		//不能有重名
		if ( strGroupName==g_GroupInfo.group[i].name )
		{
			CString strMsg;
			strMsg.Format("组 %s 已经存在! 请重新设置一个不同的组名,如: %s_%d",
				strGroupName,strGroupName,rand()%255 );

			ShowMessage(strMsg,MB_OK|MB_ICONWARNING);

			return 1;
		}
	}

	if( strGroupName.GetLength() >= MAX_GROUPNAME_LEN )
	{
		strGroupName = strGroupName.Left(MAX_GROUPNAME_LEN );
	}


	this->SetGroupName(l,strGroupName);
	

	m_posItemSelected.nGroupID = l;
	m_posItemSelected.nItem = -1;

	::PostMessage(m_hParent,
		WM_IMFRIEND_LIST_NOTIFY,
		NM_FL_GROUPNAME,
		l);
	return 1;
}

//void CDuiFriendList::AddRefreshFlag()
//{
//	m_bRefreshFriendListFlag = true;
//	SetTimer(0x0E,355,NULL);
//}
//
//void CDuiFriendList::RemoveRefreshFlag()
//{
//	if ( m_bRefreshFriendListFlag )
//	{
//		m_bRefreshFriendListFlag = false;
//		KillTimer(0x0E);
//	}
//}

int CDuiFriendList::FindGroup(int nGroupID)
{
	ASSERT(0);
	//待删除
	return -1;
	int nC=m_vecGroup.size();
	int nID=0;
	for(int i=0;i<nC;i++)
	{
		nID=m_vecGroup[i]->dwData;
		if(nID==nGroupID)
			return i;
	}
	return -1;
}

// 获取所有组（循环遍历后的结果，不需要自行再遍历）
int CDuiFriendList::GetAllGroup(vector<FL_GROUP_INFO *> &vecGroup)
{
	vecGroup.clear();

	for(int i = 0; i < m_vecGroup.size(); ++i)
	{
		FL_GROUP_INFO *group = m_vecGroup[i];
		vecGroup.push_back(group);

		vector<FL_GROUP_INFO *> vecSubGourp;
		group->GetSubGroup(vecSubGourp);

		if(vecSubGourp.size() != 0)
		{
			vecGroup.insert(vecGroup.end(), vecSubGourp.begin(), vecSubGourp.end());
		}
	}

	return vecGroup.size();
}

//// 鼠标悬停消息
//void CDuiFriendList::OnMouseHover(UINT nFlags, CPoint point)
//{
//	m_bTracking = FALSE;
//	static int s_nGroup = -1;
//	static int s_nItem = -1;
//
//	CIMListCtrl::OnMouseHover(nFlags, point);
//	int nGroup, nItem;
//	HitTest(m_vecGroup, point, nGroup, nItem);
//
//	if ( nGroup==-1 || nItem==-1 )
//	{
//		s_nGroup = s_nItem = -1;
//		return;
//	}
//
//	CPoint ptReal(point.x,point.y);
//	ptReal.y += m_OriginY;
//	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup, nItem);
//
//	if ( pItem->rcIcon.PtInRect(ptReal) && (nGroup != s_nGroup || nItem != s_nItem))
//	{
//		s_nGroup = nGroup;
//		s_nItem = nItem;
//		m_posDragTo.nGroupID = nGroup;
//		m_posDragTo.nItem  = nItem;
//		GetParent()->SendMessage(WM_IMFRIEND_LIST_NOTIFY,NM_FL_ITEM_OVER,(LPARAM)&m_posDragTo);
//	}
//
//	if ( !pItem->rcIcon.PtInRect(ptReal) )
//	{	// 发送离开悬停消息
//		s_nGroup = s_nItem = -1;
//	}
//}

// 更新界面组节点
void CDuiFriendList::UpdateGroup(int nGroupID)
{
	DuiLib::CTreeNodeUI *pGroupNode = FindGroupNode(nGroupID);
	if(pGroupNode == NULL)
		return;

	//好友成员数量
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroupID);
	if(pGroup == NULL)
		return;

	pGroup->nFrdCnt = 0, pGroup->nFrdOnCnt = 0;
	CString strGroup ;
	//会不会有性能问题?  zhangmin
	GetOnlineCount(pGroup, pGroup->nFrdCnt, pGroup->nFrdOnCnt);
	if(pGroup->nFrdCnt > 0)
		strGroup.Format("%s %d/%d",pGroup->strTitle,pGroup->nFrdOnCnt,pGroup->nFrdCnt);
	else
		strGroup = pGroup->strTitle;

	pGroupNode->GetFolderButton()->SetText(strGroup);
}

// 更新界面所有组节点
void CDuiFriendList::UpdateAllGroup()
{
	for(int i = 0; i < g_GroupInfo.count; ++i)
	{
		UpdateGroup(g_GroupInfo.group[i].group_index);
	}
}

DuiLib::CControlUI* CDuiFriendList::CreateControl(LPCTSTR pstrClass) 
{
	if (_tcsicmp(pstrClass, "GroupEdit") == 0)
	{
		return	new CDuiGroupEdit(this);
	}
	else if (_tcsicmp(pstrClass, _T("ButtonGif")) == 0)
	{
		return	new DuiLib::CButtonGifUI;
	}
	return NULL;
}

void CDuiFriendList::DoEvent(DuiLib::TEventUI& event)
{
	if( event.Type == DuiLib::UIEVENT_RBUTTONDOWN )
	{
		return;
	}

	if( event.Type == DuiLib::UIEVENT_TIMER )
		OnTimer( (UINT_PTR)event.wParam );

	CTreeViewUI::DoEvent(event);
}

bool CDuiFriendList::SelectItem(int iIndex, bool bTakeFocus /*= false*/)
{
	FL_ITEM_POS posOld = m_posItemSelected;

	m_posItemSelected.nGroupID = -1;
	m_posItemSelected.nItem = -1;

	bool bSuccess = CTreeViewUI::SelectItem(iIndex, bTakeFocus);
	if(bSuccess)
	{
		int nSel = CTreeViewUI::GetCurSel();
		if(nSel == -1)
			return true;

		DuiLib::CTreeNodeUI *pNode = (DuiLib::CTreeNodeUI *)CTreeViewUI::GetItemAt(nSel);
		if(IsGroupItem(pNode))
		{	// 为组节点
			m_posItemSelected.nGroupID = pNode->GetTag();
			m_posItemSelected.nItem = -1;
		}
		else
		{
			FRIEND_INFO *pFriend = (FRIEND_INFO*)pNode->GetTag();
			if(pFriend == NULL)
			{
				g_LocalData.WriteLog("CDuiFriendList::SelectItem pFriend 为空");
				return false;
			}
			m_posItemSelected.nGroupID = pFriend->group_index;
			FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, m_posItemSelected.nGroupID);
			if(pGroup == NULL)
			{
				g_LocalData.WriteLog("CDuiFriendList::SelectItem pGroup 为空");
				return false;
			}
			for(int i = 0; i < pGroup->vecItem.size(); ++i)
			{
				if(pGroup->vecItem[i]->dwData == pNode->GetTag())
				{
					m_posItemSelected.nItem = i;
					break;
				}
			}
		}

		m_posLast = posOld;
	}

	return bSuccess;
}

void CDuiFriendList::ShowEditBox(int nGroup)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroup);
	if(pGroup == NULL)
		return;

	DuiLib::CTreeNodeUI *pGroupNode = FindGroupNode(nGroup);
	if(pGroupNode == NULL)
		return;

	pGroupNode->Select(false);
	RECT rcNode = pGroupNode->GetPos();
	RECT rcFolder = pGroupNode->GetFolderButton()->GetPos();
	CDuiGroupEdit *pEdit = static_cast<CDuiGroupEdit*>(pGroupNode->FindSubControl("editGroupName"));
	rcFolder.left += 28;
	rcFolder.right = rcNode.right;
	if(GetVerticalScrollBar()->IsVisible())
	{
		rcFolder.right -= 2;
	}
	pEdit->SetPos(rcFolder);
	pEdit->SetText(pGroup->strTitle);
	pEdit->SetVisible(true);
	pEdit->SetTag(pGroupNode->GetTag());
	paint_manager_.SetTimer(this, TIMER_GROUPEDIT_SETFOCUS, 20);
}

void CDuiFriendList::OnTimer( UINT_PTR idEvent )
{
	if(idEvent == TIMER_GROUPEDIT_SETFOCUS)
	{
		paint_manager_.KillTimer(this, idEvent);
		DuiLib::CTreeNodeUI *pGroupNode = FindGroupNode(m_posItemSelected.nGroupID);
		if(pGroupNode == NULL)
			return;

		CDuiGroupEdit *pEdit = static_cast<CDuiGroupEdit*>(pGroupNode->FindSubControl("editGroupName"));
		pEdit->SetFocus();
		pEdit->SetSel(0,-1);
	}
}

//设置我的团队中好友未读消息红色标识
void CDuiFriendList::SetItemUmMsg(FRIEND_INFO *pFriend, bool bIsUnMsg)
{
	// 先设置好友的红点
	FL_ITEM_INFO *pItem = GetItem(pFriend);
	if(pItem == NULL)
		return;
	DuiLib::CTreeNodeUI *pNode = FindItem_Interface(pFriend->group_index, pItem->dwData);

	DuiLib::CLabelUI *pLabelUnMsg = static_cast<DuiLib::CLabelUI*>(pNode->FindSubControl("lbUnMsg"));

	CRect rc1 = pLabelUnMsg->GetPos();
	if(rc1.Width() == 0 && rc1.Height() == 0)
	{
		DuiLib::CButtonGifUI* pHeadIcon = static_cast<DuiLib::CButtonGifUI*>(pNode->FindSubControl("lbHeaderIcon"));
		CRect rcUnMsgHead = pHeadIcon->GetPadding();
		
		rcUnMsgHead.left = rcUnMsgHead.left + 24;
		rcUnMsgHead.right = rcUnMsgHead.left + 6;
		rcUnMsgHead.top	= rcUnMsgHead.top + 4;
		rcUnMsgHead.bottom = rcUnMsgHead.top + 6;
		pLabelUnMsg->SetPos(rcUnMsgHead);
	}

	pLabelUnMsg->SetVisible(bIsUnMsg);
	pLabelUnMsg->SetInternVisible(bIsUnMsg);//加上SetInternVisible之后才能获取显示隐藏状态

	//再设置好友的父节点的红点
	DuiLib::CTreeNodeUI* pNodeGroup = pNode->GetParentNode();
	int nLevel = 0;
	while (pNodeGroup != NULL)
	{
		nLevel++;
		pNodeGroup = pNodeGroup->GetParentNode();
	}
	pNodeGroup = pNode->GetParentNode();
	int nLeft = 0;
	while (pNodeGroup != NULL)
	{
		DuiLib::CLabelUI *pLabelUnMsgGroup = static_cast<DuiLib::CLabelUI*>(pNodeGroup->FindSubControl("lbUnMsg"));
		
		CRect rc2 = pLabelUnMsgGroup->GetPos();
		if(rc2.Width() == 0 && rc2.Height() == 0)
		{
			CRect rcNode = pNodeGroup->GetFolderButton()->GetTextPadding();
			nLevel--;
			nLeft = rcNode.left + nLevel * 20;
			CRect rcUmMsg = pLabelUnMsgGroup->GetPos();
			rcUmMsg.left = 0 + nLeft - 6;
			rcUmMsg.right = 6 + rcUmMsg.left;
			rcUmMsg.top	 = 2;
			rcUmMsg.bottom = 8;
			pLabelUnMsgGroup->SetPos(rcUmMsg);
		}
		if(!bIsUnMsg)
		{
			if(!GetUnMsgVisble(pNodeGroup))//如果好友的父节点上有其他好友存在未读消息则红点不隐藏
			{
				pLabelUnMsgGroup->SetVisible(bIsUnMsg);
				pLabelUnMsgGroup->SetInternVisible(bIsUnMsg);
			}
		}
		else
		{
			pLabelUnMsgGroup->SetVisible(bIsUnMsg);
			pLabelUnMsgGroup->SetInternVisible(bIsUnMsg);
		}
		
		pNodeGroup = pNodeGroup->GetParentNode();
	}
}

//判断好友的父节点上是否有其他好友存在未读消息
BOOL CDuiFriendList::GetUnMsgVisble(DuiLib::CTreeNodeUI* pNodeGroup)
{
	long lChildCount = pNodeGroup->GetCountChild();
	for(int i = 0; i < lChildCount; i++)
	{
		DuiLib::CTreeNodeUI* pNode = pNodeGroup->GetChildNode(i);
		if(pNode->IsHasChild())
		{
			if(GetUnMsgVisble(pNode))
				return TRUE;
		}
		else
		{
			DuiLib::CLabelUI *pLabelUnMsg = static_cast<DuiLib::CLabelUI*>(pNode->FindSubControl("lbUnMsg"));
			if(pLabelUnMsg != NULL && pLabelUnMsg->IsVisible())
				return TRUE;
		}
	}
	return FALSE;
}

BOOL CDuiFriendList::GetFriendUnMsgVisable(FRIEND_INFO *pFriend)
{
	FL_ITEM_INFO *pItem = GetItem(pFriend);
	if(pItem == NULL)
		return FALSE;
	DuiLib::CTreeNodeUI *pNode = FindItem_Interface(pFriend->group_index, pItem->dwData);

	DuiLib::CLabelUI *pLabelUnMsg = static_cast<DuiLib::CLabelUI*>(pNode->FindSubControl("lbUnMsg"));
	if(pLabelUnMsg->IsVisible())
		return TRUE;
	return FALSE;
}