#include "stdafx.h"
#include <afxwin.h>
#include <afxcmn.h>
#include "impublic.h"
//#include "../../../Common/Common.h"
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

void ShowMessage(CString strMessage,UINT nType/* =MB_OK|MB_ICONINFORMATION */);


/////////////////////////////////////////////////////////////////////////////
// CIMFriendListCtrl
#include "imfriendlistctrl.h"
#include "..\Resource.h"
#include ".\imfriendlistctrl.h"

static CString  g_strDot="";
static int      g_iDotCount=0;

IMPLEMENT_DYNAMIC(CGroupEdit, CEdit)
CGroupEdit::CGroupEdit( int nGroupId)
{
	m_nGroupId		= nGroupId	;
}

CGroupEdit::~CGroupEdit()
{
}

BEGIN_MESSAGE_MAP(CGroupEdit, CFlatEdit)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
END_MESSAGE_MAP()

void CGroupEdit::OnKillFocus(CWnd* pNewWnd)
{
	CFlatEdit::OnKillFocus(pNewWnd);
	DestroyWindow();
}

void CGroupEdit::OnNcDestroy()
{
	CString str;
	GetWindowText(str);

	::SendMessage(GetParent()->m_hWnd,WM_GROUPNAME_EDIT,m_nGroupId,
		(LPARAM)(LPCSTR)str);

	CFlatEdit::OnNcDestroy();

	delete this;
}

void CGroupEdit::PreSubclassWindow()
{
	CFlatEdit::PreSubclassWindow();
	this->SetTextColor(RGB(0,84,166));
}


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

CIMFriendListCtrl::CIMFriendListCtrl()
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
	m_bMouseDown  = false;
	
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

CIMFriendListCtrl::~CIMFriendListCtrl()
{
	DeleteAllItems(); 
}

BEGIN_MESSAGE_MAP(CIMFriendListCtrl, CIMListCtrl)
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
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
	ON_MESSAGE(WM_GROUPNAME_EDIT,OnGroupNameEdit)
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIMFriendListCtrl message handlers
BOOL CIMFriendListCtrl::Create(CWnd *pParentWnd,const CRect &rect, DWORD dwStyle/* =WS_VISIBLE|WS_BORDER|WS_CHILD */,UINT nID/* =0 */)
{
	return CIMListCtrl::Create( AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS),
						"xTreamTreeClass",
						dwStyle,
						rect,pParentWnd,
						nID);
}



//按照ID  查找组(子组),  zhangmin
FL_GROUP_INFO * CIMFriendListCtrl::GetGroup(vector<FL_GROUP_INFO *> vecGroup, int nGroupID )
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
FL_ITEM_INFO * CIMFriendListCtrl::GetItem(vector<FL_GROUP_INFO *> vecGroup, int nGroupID, int nItemID)
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
FL_ITEM_INFO * CIMFriendListCtrl::GetItem(FRIEND_INFO *pFriend)
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



void CIMFriendListCtrl::ClearGroup(int nGroup)
{
	ASSERT(0);
	//待删除
	return;
	if ( GroupExist(nGroup)==false )
		return ;

	vector<_FL_ITEM_INFO *>* pItemInfo = &m_vecGroup[nGroup]->vecItem;

	vector<_FL_ITEM_INFO *>::iterator ItemIter;
	for( ItemIter =pItemInfo->begin() ; ItemIter != pItemInfo->end();ItemIter++)
	{
		delete *ItemIter;
	}
	pItemInfo->clear();
	InvalidateList();
}

int CIMFriendListCtrl::DelGroup(int nParentGroupId, int nGroupId)
{
	//if ( GroupExist(nGroup)==false )
		//return ;
	//没有子组和成员的组才能删除成功
	vector<FL_GROUP_INFO *> *pVecGroup;
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroupId);
	ASSERT(pGroup != NULL);
	ASSERT( pGroup->vecItem.size() == 0);
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
	InvalidateList();
	return 1;
}



int CIMFriendListCtrl::InsertGroup(int nParentGroup,int nGroupId, CString strTitle)
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
	}
	else
	{
		FL_GROUP_INFO * pParentGroup = GetGroup(m_vecGroup, nParentGroup);
		if(pParentGroup != NULL)
		{

			pParentGroup->vecSubGroup.push_back(Group);
			
		}
	}

	InvalidateList();

	return 1;
}



bool CIMFriendListCtrl::FindFriend(FRIEND_INFO*pFriend,int&nGroup,int&nItem)
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


int CIMFriendListCtrl::AddGroup(int nGroupId, CString strTitle)
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
		
		m_vecGroup.push_back(Group);

		InvalidateList();
	}

	return 1;
}

//AddGroup是在m_vecGroup尾部添加，好像是废话
int CIMFriendListCtrl::AddSubGroup(int nParentGroupId, int nSubGroupId, CString strTitle)
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

	
		//创建子组
	FL_GROUP_INFO * pSubGroup = new FL_GROUP_INFO();
	if(pSubGroup == NULL)
		return -1;
	pSubGroup->nFrdCnt = 0;
	pSubGroup->strTitle=strTitle;
	pSubGroup->clrText = CLR_TEXT;
	pSubGroup->dwData = nSubGroupId;
 	(*pVecGroup).push_back(pSubGroup);
 
	InvalidateList();
	return 1;
}

void CIMFriendListCtrl::SetGroupTextColor(int nGroup, COLORREF clrText)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup,nGroup);
	if(pGroup != NULL)
		pGroup->clrText = clrText;
	InvalidateList();
}

void CIMFriendListCtrl::SetGroupState(int nGroup, FLS_STATE state)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroup);
	if(pGroup != NULL)
		pGroup->state = state;
	InvalidateList();
}

void CIMFriendListCtrl::SetGroupName(int nGroup, CString strTitle)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroup);
	if(pGroup != NULL)
	{
		strTitle.Trim();
		pGroup->strTitle = strTitle;
	}
	InvalidateList();
}

CString CIMFriendListCtrl::GetGroupName(int nGroup)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroup);
	if(pGroup == NULL)
		return "";
	return  pGroup->strTitle;
}


void CIMFriendListCtrl::DeleteItem(int nGroup, int nItem)
{
/*
	if ( ItemExist(nGroup,nItem)==false )
		return ;
*/
	FL_GROUP_INFO * pGroup = GetGroup(m_vecGroup, nGroup);
	FL_ITEM_INFO * pItem = GetItem(m_vecGroup, nGroup, nItem);
	//释放Item
	delete pItem;
	//清空组内的Item指针
	pGroup->vecItem.erase(pGroup->vecItem.begin() + nItem);
	if ( m_hWnd )
	{
		InvalidateList();
	}
}

void CIMFriendListCtrl::DeleteItem(FRIEND_INFO *pFriend)
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
	
	if ( m_hWnd )
	{
		InvalidateList();
	}
}


void CIMFriendListCtrl::DeleteItems(vector<FL_GROUP_INFO *>& vecGroup)
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


void CIMFriendListCtrl::DeleteAllItems()
{

	DeleteItems(m_vecGroup);
	
	m_OriginY = 0;
	m_nClientRealHeigh = 0;

	m_posItemSelected.nGroupID = -1;
	m_posItemSelected.nItem = -1;
	InvalidateList();
}


int CIMFriendListCtrl::AddItem(int iGroup, CString strText, DWORD dwData)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, iGroup);
	if(pGroup == NULL)
		return -1;
	FL_ITEM_INFO * item = new FL_ITEM_INFO();
	item->strText = strText;	
	item->dwData  = dwData;
	item->clrText =  CLR_TEXT;
	
	pGroup->vecItem.push_back(item);

	return 1;
		
}


int CIMFriendListCtrl::SubGroupAddItem(FL_GROUP_INFO *tmp, int iGroup, CString strText, DWORD dwData)
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


void CIMFriendListCtrl::SetItem(int nGroup, int nIndex, CString strText, DWORD dwData /* = 0 */)
{

	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup, nIndex);

	if(pItem != NULL)
	{
		pItem->strText = strText;
		pItem->dwData = dwData;
		InvalidateList();
	}
}

void CIMFriendListCtrl::SwapItem(int nGroup1, int nItem1, int nGroup2, int nItem2)
{
	FL_ITEM_INFO *pItem1 = GetItem(m_vecGroup, nGroup1, nItem1);	
	FL_ITEM_INFO *pItem2 = GetItem(m_vecGroup, nGroup2, nItem2);

	FL_ITEM_INFO *pItemTmp = pItem1;
	pItem1 = pItem2;
	pItem2 = pItemTmp;

	InvalidateList();
}

bool ItemSortByState(FL_ITEM_INFO *pItem1, FL_ITEM_INFO *pItem2 )
{
	FRIEND_INFO *pFriend1, *pFriend2;
	pFriend1 = (FRIEND_INFO*)pItem1->dwData;
	pFriend2 = (FRIEND_INFO*)pItem2->dwData;
	return pFriend1->status < pFriend2->status;
}

bool ItemSortByText(FL_ITEM_INFO *pItem1, FL_ITEM_INFO *pItem2 )
{
	bool bRet = ( strcmp(pItem1->strText,pItem2->strText) < 0 ) ? true : false;
	return bRet;
}

void CIMFriendListCtrl::SortGroupByState(int nGroupId)
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
	InvalidateList();
}


void CIMFriendListCtrl::SetItemText(int nGroup, int nIndex, CString strText)
{
	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup, nIndex);
	if(pItem != NULL)
	{
		pItem->strText = strText;
		InvalidateList();
	}
}

void CIMFriendListCtrl::SetItemText(FRIEND_INFO *pFriend, CString strText)
{
	FL_ITEM_INFO *pItem = GetItem(pFriend);
	if(pItem != NULL)
	{
		pItem->strText = strText;
		InvalidateList();
	}
}


CString CIMFriendListCtrl::GetItemText(int nGroup, int nIndex)
{
	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup, nIndex);
		return "";
	if(pItem != NULL)
		return pItem->strText;
}

void CIMFriendListCtrl::DelItemImage(int nGroup, int nItem )
{
	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup, nItem);
	if(pItem != NULL)
	{
		pItem->bitmap20 = NULL;
		pItem->bitmap40 = NULL;
		InvalidateList();
	}
}

void CIMFriendListCtrl::UpdateItemImage(FRIEND_INFO *pFriend)
{


	FL_ITEM_INFO *pItem = GetItem(pFriend);

	if( pFriend != NULL && pItem != NULL)
	{
		CBitmap * p20 = GetUserBitmap(pFriend,true);
		pItem->bitmap20 = p20;
		
		InvalidateList();
	}
}

void CIMFriendListCtrl::SetCurSel(int nGroupID,int nItem)
{
	if ( nGroupID==-1 )
		return ;

	m_posItemSelected.nGroupID = nGroupID;
	m_posItemSelected.nItem  = nItem;

	FL_GROUP_INFO *group = GetGroup(m_vecGroup, m_posItemSelected.nGroupID);
	if(group == NULL)
		return;

	if ( group->state!=FLS_EXPAND )
	{
		group->state = FLS_EXPAND;
	}

	if ( m_hWnd )
	{
		RecalcLayout();

		CRect rcItem;
		int nTop;

		if ( nItem>-1 && nItem<group->vecItem.size() )
		{
			rcItem = group->rcBar;
			nTop = group->rcBar.top + (nItem + 1) * m_nItemHeight;
			rcItem.top = nTop;
			rcItem.bottom = nTop +  m_nSelItemHeight;
		}

		if ( nItem<0 )
		{
			rcItem = group->rcBar;
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
			SCROLLINFO si;
			CIMListCtrl::GetScrollInfo(SB_VERT,&si);
			int nPos ;

			int nMove = 0;
			if ( rcItem.top>0 )
			{
				nMove = (rcItem.top - rcClient.bottom  + m_nItemHeight );
				m_OriginY += nMove;

				if ( m_nClientRealHeigh!=0)
					nPos = nMove*si.nMax/m_nClientRealHeigh;
				else
					nPos = 0;

				si.nPos+=nPos;
			}
			else
			{
				nMove = (m_nItemHeight - rcItem.top );
				m_OriginY -= nMove;

				if (m_nClientRealHeigh!=0)
					nPos = nMove*si.nMax/this->m_nClientRealHeigh;
				else
					nPos = 0;
				si.nPos-=nPos;
			}

			CIMListCtrl::SetScrollPos(SB_VERT, si.nPos);
		}
		InvalidateList();
	}
	else
	{
		RecalcLayout();
	}

}

void CIMFriendListCtrl::SetCurSel(FRIEND_INFO *pFriend)
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
	
	return;
}


FL_ITEM_POS CIMFriendListCtrl::GetCurSel()
{
	return m_posItemSelected;
}



CRect CIMFriendListCtrl::GetItemRect(int nGroup, int nIndex)
{

	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup, nIndex);
	CRect rcItem;

	if( pItem != NULL)
	{
		rcItem = pItem->rcItem;
		rcItem.top -= this->m_OriginY;
		rcItem.bottom -= m_OriginY;
	}

	return rcItem;
}

CRect CIMFriendListCtrl::GetItemIconRect(int nGroup, int nIndex)
{

	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup, nIndex);
	CRect rcItem;

	if( pItem != NULL)
	{
		rcItem = pItem->rcIcon;
		rcItem.top -= this->m_OriginY;
		rcItem.bottom -= m_OriginY;
	}

	return rcItem;
}

DWORD CIMFriendListCtrl::GetItemData(int nGroup, int nIndex)
{
	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup, nIndex);
	if(pItem != NULL)
		return pItem->dwData;
	return 0;
}


void CIMFriendListCtrl::SetItemData(int nGroup, int nIndex, DWORD dwData)
{
	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup,nGroup);
	if(pItem != NULL)
		pItem->dwData = dwData;
}

void CIMFriendListCtrl::SetItemTextColor(int nGroup, int nIndex, COLORREF clrText)
{
	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup,nIndex);
	if(pItem != NULL)
	{
		pItem->clrText = clrText;
		InvalidateList();
	}
}

void CIMFriendListCtrl::SetItemTextColor(FRIEND_INFO *pFriend, COLORREF clrText)
{
	FL_ITEM_INFO *pItem = GetItem(pFriend);
	if(pItem != NULL)
	{
		pItem->clrText = clrText;
		InvalidateList();
	}
}


int CIMFriendListCtrl::MoveItem(int nGroupSrc, int nItemSrc, int nGroupDest)
{

	FL_GROUP_INFO* pGroupSrc = GetGroup(m_vecGroup, nGroupSrc);
	FL_GROUP_INFO* pGroupDest = GetGroup(m_vecGroup, nGroupDest);

	ASSERT(pGroupSrc != NULL && pGroupDest != NULL);
	if(pGroupSrc == NULL || pGroupDest == NULL)
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
	InvalidateList();

	return 1;
}

void CIMFriendListCtrl::SetSubGroupHeight(FL_GROUP_INFO *pParentGroup , int& nPreGroupHeight)
{	
	if( pParentGroup->state == FLS_EXPAND )
	{
#ifdef _VER_INTERNAL_		// 内部版中，好友列表显示在子分组列表之上
		//确定好友列表的位置
		for(int i=0; i<pParentGroup->vecItem.size(); i++)
		{
			FL_ITEM_INFO* pItem = pParentGroup->vecItem[i];
			pItem->rcItem.left = pParentGroup->rcBar.left;
			pItem->rcItem.top = nPreGroupHeight;
			pItem->rcItem.right = pParentGroup->rcBar.right;
			nPreGroupHeight += m_nItemHeight;
			if(m_posItemSelected.nGroupID == pParentGroup->dwData && m_posItemSelected.nItem == i)
				nPreGroupHeight += m_nSelItemHeight;
			pItem->rcItem.bottom = nPreGroupHeight;
		}

		for(int i=0; i<pParentGroup->vecSubGroup.size();i++)
		{

			FL_GROUP_INFO * pSubGroup = pParentGroup->vecSubGroup[i];
			pSubGroup->rcBar.left= pParentGroup->rcBar.left + X_SPACE + GRP_BTN_W;
			pSubGroup->rcBar.top = nPreGroupHeight;
			pSubGroup->rcBar.right= pParentGroup->rcBar.right;
			pSubGroup->rcBar.bottom = pSubGroup->rcBar.top + m_nItemHeight;

			nPreGroupHeight = pSubGroup->rcBar.bottom;

			//递归设置子组的位置(主要是高度)
			SetSubGroupHeight(pSubGroup, nPreGroupHeight);
		}
#else
		for(int i=0; i<pParentGroup->vecSubGroup.size();i++)
		{

			FL_GROUP_INFO * pSubGroup = pParentGroup->vecSubGroup[i];
			pSubGroup->rcBar.left= pParentGroup->rcBar.left + X_SPACE + GRP_BTN_W;
			pSubGroup->rcBar.top = nPreGroupHeight;
			pSubGroup->rcBar.right= pParentGroup->rcBar.right;
			pSubGroup->rcBar.bottom = pSubGroup->rcBar.top + m_nItemHeight;


			nPreGroupHeight = pSubGroup->rcBar.bottom;

			//递归设置子组的位置(主要是高度)
			SetSubGroupHeight(pSubGroup, nPreGroupHeight);
		}

		//确定好友列表的位置
		for(int i=0; i<pParentGroup->vecItem.size(); i++)
		{
			FL_ITEM_INFO* pItem = pParentGroup->vecItem[i];
			pItem->rcItem.left = pParentGroup->rcBar.left;
			pItem->rcItem.top = nPreGroupHeight;
			pItem->rcItem.right = pParentGroup->rcBar.right;
			nPreGroupHeight += m_nItemHeight;
			if(m_posItemSelected.nGroupID == pParentGroup->dwData && m_posItemSelected.nItem == i)
				nPreGroupHeight += m_nSelItemHeight;
			pItem->rcItem.bottom = nPreGroupHeight;
		}
#endif

		/*
		//好友列表高度
		nPreGroupHeight += pParentGroup->vecItem.size() * m_nItemHeight;
		
		//增加被选中的好友高度
		if(m_posItemSelected.nGroup == pParentGroup->dwData && m_posItemSelected.nItem != -1)
		{
			nPreGroupHeight += m_nSelItemHeight;
		}
		*/
	}

}

void CIMFriendListCtrl::RecalcLayout()
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int nPreGroupHeight = 0;
	for (int iGroup = 0; iGroup < m_vecGroup.size(); iGroup++)
	{
		CRect rcGroup;
		
		rcGroup.left   = rcClient.left + LIST_START_X;
		rcGroup.top    = nPreGroupHeight ;
		rcGroup.right  = rcClient.right - 2;
		rcGroup.bottom = rcGroup.top + m_nItemHeight;
		m_vecGroup[iGroup]->rcBar = rcGroup;
		nPreGroupHeight = rcGroup.bottom;
		SetSubGroupHeight(m_vecGroup[iGroup], nPreGroupHeight);
		
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

void CIMFriendListCtrl::OnPaint() 
{
	CPaintDC dc(this); 
	
	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CRect rcClient;
	// 获得列表窗体的大小
	GetClientRect(&rcClient);
	// 调整画布的大小
	if(m_nHeight != rcClient.Height() || m_nWidth != rcClient.Width())
	{
		m_nHeight = rcClient.Height();
		m_nWidth = rcClient.Width();
		m_canvas.DeleteObject();
		m_canvas.CreateCompatibleBitmap(&dc, m_nWidth, m_nHeight);
		m_bRedraw = true;
	}
	// 选择画布
	CBitmap* pOldbmp = memDC.SelectObject(&m_canvas);
	if(m_bRedraw)
	{
		CBrush brushBack(m_clrBkGround);
		memDC.FillRect(&rcClient, &brushBack);
		CFont* pFont = memDC.SelectObject(&g_font12);	
		// 重新计算列表中各项的位置
		RecalcLayout();
		for (int iGroup = 0; iGroup < m_vecGroup.size(); iGroup++)
		{		
			FL_GROUP_INFO *pGroup = m_vecGroup[iGroup];
			DrawGroup( pGroup, &memDC);
		}
		memDC.SelectObject(pFont);
		m_bRedraw = false;
	}
	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldbmp);
}

void CIMFriendListCtrl::DrawStatusFoot(CDC *pDC, CRect rcIcon, bool bSelItem, int status)
{
	if ( status!=XTREAM_LEAVE && status!=XTREAM_HIDE )
	{
		return ;
	}

	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pDC);

	CResBitmap bmpTemp;

	int x, y, width, height;
	int top;
/*
	if ( bSelItem )
	{
		x = rcIcon.right-19;
		y = rcIcon.bottom-9;

		width  = 18;
		height = 10;

		if ( status==XTREAM_LEAVE )
		{
			top = 0;
		}
		else
		{
			top = 9;
		}

		bmpTemp.LoadBitmap(IDB_STATUS_FOOT40);
	}
	else
*/
	{
		x = rcIcon.right-14;
		y = rcIcon.bottom-9;

		width  = 13;
		height = 8;

		if ( status==XTREAM_LEAVE )
		{
			top = 0;
		}
		else
		{
			top = 8;
		}

		bmpTemp.LoadBitmap(IDB_STATUS_FOOT20);
	}

	CBitmap *pbmpOld = dcTemp.SelectObject(&bmpTemp);

	pDC->BitBlt(x,y,width,height,&dcTemp,0,top,SRCCOPY);

	dcTemp.SelectObject(pbmpOld);

}

void CIMFriendListCtrl::DrawFriend(FL_ITEM_INFO *pItem, CDC *pDC , bool bSelItem)
{
	FRIEND_INFO  * pUser = (FRIEND_INFO *)pItem->dwData;
	const int nTextTop	= ( m_nItemHeight-m_nTextHeight)/2;
	/*
	if(bSelItem)
		nTextTop = 11;
		*/
	
	//const int nTextTop  = ( m_nItemHeight-m_nTextHeight)/2;
	bool bDragFlag = false; 

	//鼠标位置
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	ptCursor.y+=m_OriginY;


	//选中阴影
	if(bSelItem)
		pDC->Rectangle(pItem->rcItem);

	//判断是否超出范围
	CRect rcView;
	GetClientRect(&rcView);
	rcView.top += m_OriginY;
	rcView.bottom += m_OriginY;
	if(pItem->rcItem.bottom < rcView.top)
		return;
	else if(pItem->rcItem.top > rcView.bottom)
		return;

	
	if(pItem->rcItem.bottom < rcView.top)
		return;//continue;
	else if(pItem->rcItem.top > rcView.bottom)
		return; //break;
	
	//begin draw head icon
	POINT pt;
	pt.x = pItem->rcItem.left + GRP_BTN_W  + X_SPACE;
	pt.y = pItem->rcItem.top ;
	
	CRect rcIcon;
	
	CBitmap * pBitmap = NULL;

	/*
	if ( bSelItem )
	{
		rcIcon.SetRect(pt.x, pt.y, pt.x + 40, pt.y + 40);
		pBitmap = pItem->bitmap40;
	}
	else
	*/
	{
		rcIcon.SetRect(pt.x, pt.y, pt.x + HEAD_SZ, pt.y + HEAD_SZ);
		pBitmap = pItem->bitmap20;
	}
	
	//是否有web更新？
	//0 null, 1图片only,2文章only,3 both图片文章
	if ( pUser->webInfo>0 )
	{			
		CResBitmap * pBitmap =NULL;
	
		if ( pUser->webInfo & 0x02 )
			pBitmap = &m_bmpWebPicture;
		else
		if ( pUser->webInfo & 0x01 )
			pBitmap = &m_bmpWebArticle;
	
		if ( pBitmap )
		{
			pItem->rcBtnWebInfo.left = rcIcon.left-16;
			pItem->rcBtnWebInfo.top  = rcIcon.top+3;
			pItem->rcBtnWebInfo.right = pItem->rcBtnWebInfo.left + 12;
			pItem->rcBtnWebInfo.bottom = pItem->rcBtnWebInfo.top + 12;
	
	
			::DrawBitmap(pDC,pItem->rcBtnWebInfo.left,pItem->rcBtnWebInfo.top,pBitmap,RGB(0,255,0));
			//DrawBitmap(pDC,pItem->rcBtnWebInfo.left,pItem->rcBtnWebInfo.top,&m_bmpWebInfo);
			CRect rcLine = pItem->rcBtnWebInfo;
			rcLine.InflateRect(2,2);
			DrawPtInRect(ptCursor,rcLine,pDC,0,this->m_bMouseWebInfoDown);
		}
	}
	
	
	pItem->rcIcon = rcIcon;
	
	if ( pBitmap )
	{
		// 计算y坐标
		BITMAP bmp;
		pBitmap->GetBitmap(&bmp);
		int nHeight=bmp.bmHeight; 
		if(pItem->rcItem.Height() > nHeight)
		{
			pt.y = pItem->rcItem.top + (pItem->rcItem.Height()-nHeight)/2;
		}
		pItem->rcIcon.top = pt.y;
		pItem->rcIcon.right = pItem->rcIcon.left + bmp.bmWidth;
		pItem->rcIcon.bottom = pItem->rcIcon.top + bmp.bmHeight;

		// 绘制自定义139形象头像
		if (  pUser->status!=XTREAM_OFFLINE )
			DrawBitmap(pDC,pt.x,pt.y,pBitmap);
		else
			DrawBitmap(pDC,pt.x,pt.y,pBitmap,true);

		if(pUser->status == XTREAM_ONLINE || pUser->status == XTREAM_BUSY || pUser->status == XTREAM_LEAVE)
		{
			int nSmallHeight = g_ImgStatusBusy.GetHeight();
			CPoint ptSmallIcon(pt.x + nHeight-nSmallHeight, pt.y + nHeight-nSmallHeight);

			switch(pUser->status)
			{
			case XTREAM_BUSY:
				g_ImgStatusBusy.Draw(pDC->m_hDC, ptSmallIcon.x, ptSmallIcon.y, nSmallHeight, nSmallHeight);
				break;
			case XTREAM_LEAVE:
				g_ImgStatusLeave.Draw(pDC->m_hDC, ptSmallIcon.x, ptSmallIcon.y, nSmallHeight, nSmallHeight);
				break;
			default:
				break;
			}
		}

		if(pUser->status == XTREAM_HIDE && pUser->GetId() == g_ImUserInfo.GetId())
		{	// 是本人时，绘制隐身小图标
			int nSmallHeight = g_ImgStatusHide.GetHeight();
			CPoint ptSmallIcon(pt.x + nHeight-nSmallHeight, pt.y + nHeight-nSmallHeight);
			g_ImgStatusHide.Draw(pDC->m_hDC, ptSmallIcon.x, ptSmallIcon.y, nSmallHeight, nSmallHeight);
		}
	
		//pDC->Draw3dRect(pItem->rcIcon,0xBBCCBB,0xBBCCBB);
	}
	else
	{
		// 绘制系统自带头像
		/*
		if ( bSelItem )
			g_imgStatus40.Draw(pDC,GetDefaultImage(pUser->sex,pUser->status),pt,ILD_TRANSPARENT);
		else
		*/
		{
				
			if(pUser->status == XTREAM_ONLINE || pUser->status == XTREAM_BUSY || pUser->status == XTREAM_LEAVE)
			{
			
				g_ImgFaceFrame.Draw(pDC->m_hDC, pt.x, pt.y, 26, 26);
				g_ImgIcon.Draw(pDC->m_hDC, pt.x, pt.y, 26, 26);
				switch(pUser->status)
				{
					case XTREAM_BUSY:
						g_ImgStatusBusy.Draw(pDC->m_hDC, pt.x + 20, pt.y + 20, 6, 6);
						break;
					case XTREAM_LEAVE:
						g_ImgStatusLeave.Draw(pDC->m_hDC, pt.x + 20, pt.y + 20, 6, 6);
						break;
					default:
						break;
				}
			
			}
			else
				g_ImgIconGray.Draw(pDC->m_hDC, pt.x, pt.y, 26, 26);
			//g_imgStatus24.Draw(pDC,GetDefaultImage(pUser->sex,pUser->status),pt,ILD_TRANSPARENT);
		}
	}
	//画状态图标，如隐身、离开等
	//DrawStatusFoot(pDC, rcIcon, bSelItem, pUser->status);  //zhangmin
	
	rcIcon.InflateRect(1,1);

	//画框框
	//DrawPtInRect(ptCursor,rcIcon,pDC,0,this->m_bMouseIconDown);
	
	pt.x = pItem->rcItem.right-20, pt.y=pItem->rcItem.top+(m_nItemHeight-16)/2;
	
	CRect rcBlog;	//博客按钮位置
	
	//博客按钮
	rcBlog.SetRect(pt.x,pt.y,pt.x+16,pt.y+16);
	rcBlog.InflateRect(1,1);
	pItem->rcBtnBlog = rcBlog;
	
	//this->DrawBitmap(pDC,pt.x,pt.y,IDB_FRIEND_BLOG);
	
	// 绘制按钮被激活时候的轮廓
	//DrawPtInRect(ptCursor,rcBlog,pDC,0,this->m_bMouseBlogDown);
	
	
	int nSignTextRight = rcBlog.left-2;
	
	//手机绑定
	if ( pUser->bind_mobile )
	{
		CRect rcMobile = pItem->rcBtnBlog;			
		
		rcMobile.left -=19;
		rcMobile.right-=19;
	
		pItem->rcMobile = rcMobile;
		nSignTextRight -=19;
	
		//DrawBitmap(pDC,rcMobile.left+1,rcMobile.top+1,IDB_FRIEND_MOBILE);
	
		// 绘制按钮被激活时候的轮廓
		//DrawPtInRect(ptCursor,rcMobile,pDC,0,this->m_bMouseMobileDown);
	}
	
	
	//视频
	if ( pUser->equipment & 0x01 )
	{
		CRect rcVedio ;
	
		if ( pUser->bind_mobile )
			rcVedio = pItem->rcMobile;
		else
			rcVedio = pItem->rcBtnBlog;
		
		rcVedio.left -=19;
		rcVedio.right-=19;
	
		pItem->rcVedio = rcVedio;
		nSignTextRight -=19;
	
		//DrawBitmap(pDC,rcVedio.left+1,rcVedio.top+1,IDB_FRIEND_VEDIO);
	
		// 绘制按钮被激活时候的轮廓
		//DrawPtInRect(ptCursor,rcVedio,pDC,0,this->m_bMouseVedioDown);
	}
	
	if ( bDragFlag )
		pDC->SetTextColor(RGB(255,0,0));
	else
		pDC->SetTextColor(pItem->clrText);
	
	// draw item text
	CString strLabel;//个性签名
	
	if ( pUser->bInviting )
	{
		strLabel = "<该好友还未通过验证>";
	}
	else
	{
		if ( strcmp(pUser->user_sign,"")!=0 )
			strLabel.Format("  %s",pUser->user_sign);
		else
			strLabel = "";
	}


	//昵称显示
	if ( pItem->strText=="" )
		pItem->strText=" ";
	
	CSize  sizeText =pDC->GetTextExtent(pItem->strText);
	CRect  rcItemText;
	
	rcItemText.left   = pItem->rcItem.left + GRP_BTN_W + ICON_X + X_SPACE*2;
	rcItemText.top	  = pItem->rcItem.top + nTextTop;
	rcItemText.bottom = rcItemText.top	+ sizeText.cy;
	rcItemText.right  = rcItemText.left + sizeText.cx;
	pItem->rcText	  = rcItemText;
	pDC->DrawText(pItem->strText,&rcItemText,DT_TOP|DT_LEFT|DT_END_ELLIPSIS);
	
	if ( pUser->status==XTREAM_OFFLINE )
	{
		pDC->SetTextColor(RGB(180,180,180));
	}
	else
	{
		pDC->SetTextColor(RGB(247,148,28));
	}
	
	//开始画个性签名
	CRect rcLabelText;
	rcLabelText.top = rcItemText.top;
	rcLabelText.left = rcItemText.right+2;
	rcLabelText.bottom = rcItemText.bottom;
	rcLabelText.right  = nSignTextRight;
	
	if ( rcLabelText.right>rcLabelText.left )
		pDC->DrawText(strLabel,&rcLabelText,DT_TOP|DT_LEFT|DT_END_ELLIPSIS);
	
	// 如果当前项被选择，则显示详细资料等功能
	if ( bSelItem )
	{
		/*
		CString  strTemp ;
		CSize	 szTemp  ;
		CRect	 rcTemp  ;
		COLORREF clr	 ;
	
		CFont	 fontLink  ; 
		LOGFONT  lf 	   ;
		
		g_fontText.GetLogFont(&lf);
		lf.lfUnderline=true;
	
		fontLink.CreateFontIndirect(&lf);
	
		strTemp = "详细资料"; 
	
		szTemp = pDC->GetTextExtent(strTemp);
	
		rcTemp.SetRect(rcItemText.left,rcItemText.bottom+5,
			rcItemText.left+szTemp.cx,rcItemText.bottom+2+szTemp.cy);
	
		if ( rcTemp.PtInRect(ptCursor)) 
		{
			CFont *pOldfont = pDC->SelectObject(&fontLink);
			clr=pDC->SetTextColor(RGB(99,133,88));
			pDC->TextOut(rcTemp.left,rcTemp.top,strTemp);
			pDC->SelectObject(pOldfont);
		}
		else
		{
			clr=pDC->SetTextColor(RGB(180,180,180));
			pDC->TextOut(rcTemp.left,rcTemp.top,strTemp);
		}
	
		rcTemp.InflateRect(1,1);
		pItem->rcBtnInfo = rcTemp;
	
		pDC->SetTextColor(clr);
	*/
		/*
		int nPos1 = rcTemp.right+3;
	
		strTemp = "图片"; 
	
		szTemp = pDC->GetTextExtent(strTemp);
	
		rcTemp.SetRect(nPos1,rcItemText.bottom+5,
			nPos1+szTemp.cx,rcItemText.bottom+2+szTemp.cy);
	
		if ( rcTemp.PtInRect(ptCursor)) 
		{
			CFont *pOldfont = pDC->SelectObject(&fontLink);
			clr=pDC->SetTextColor(RGB(99,133,88));
			pDC->TextOut(rcTemp.left,rcTemp.top,strTemp);
			pDC->SelectObject(pOldfont);
		}
		else
		{
			clr=pDC->SetTextColor(RGB(180,180,180));
			pDC->TextOut(rcTemp.left,rcTemp.top,strTemp);
		}
	
		rcTemp.InflateRect(1,1);
		pItem->rcBtnPic= rcTemp;
	
		nPos1 = rcTemp.right+3;
	
		strTemp = "文章";
	
		szTemp = pDC->GetTextExtent(strTemp);
	
		rcTemp.SetRect(nPos1,rcItemText.bottom+5,
			nPos1+szTemp.cx,rcItemText.bottom+2+szTemp.cy);
	
		if ( rcTemp.PtInRect(ptCursor)) 
		{
			CFont *pOldfont = pDC->SelectObject(&fontLink);
			clr=pDC->SetTextColor(RGB(99,133,88));
			pDC->TextOut(rcTemp.left,rcTemp.top,strTemp);
			pDC->SelectObject(pOldfont);
		}
		else
		{
			clr=pDC->SetTextColor(RGB(180,180,180));
			pDC->TextOut(rcTemp.left,rcTemp.top,strTemp);
		}
		
		pItem->rcBtnArticle = rcTemp;
		pDC->SetTextColor(clr); 			
	
		*/
	}
	else
	{
		pItem->rcBtnPic.SetRect(0,0,0,0);
		pItem->rcBtnArticle.SetRect(0,0,0,0);
		pItem->rcBtnInfo.SetRect(0,0,0,0);
	}


}


void CIMFriendListCtrl::DrawGroup(FL_GROUP_INFO* Group, CDC *pDC )
{	
	POINT ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	
	// 得到当前列表控件的区域
	CRect rcView;
	GetClientRect(&rcView);
	rcView.top+=m_OriginY;
	rcView.bottom+=m_OriginY;
	ptCursor.y+=m_OriginY;

	int nSavedDC = pDC->SaveDC();
	pDC->SetWindowOrg(0,m_OriginY);

	const int nTextTop	= ( m_nItemHeight-m_nTextHeight)/2;

	bool bDragFlag = false; 


	//判断是否被拖拽
	if ( m_bMouseDown && 
		 m_posItemSelected.nGroupID>-1 && 
		 m_posItemSelected.nItem>-1 &&
		 m_posDragTo.nGroupID>-1 &&
		 m_posDragTo.nGroupID!=m_posItemSelected.nGroupID
		)
	{
		FL_GROUP_INFO *group = GetGroup(m_vecGroup, m_posDragTo.nGroupID);
		if ( Group == group )
			bDragFlag=true;
	}

	CRect rc = Group->rcBar;

	//btn按钮
	POINT pt;
	pt.x = rc.left;
	pt.y = rc.top + (rc.Height() - GRP_BTN_H) / 2;
	Group->rcButton.SetRect(pt.x, pt.y, pt.x + GRP_BTN_W, pt.y + GRP_BTN_H);
	m_ButtonImages.Draw(pDC, Group->state, pt, ILD_NORMAL);
	
	pDC->SetBkMode(TRANSPARENT);
	CFont* pOldFont = pDC->SelectObject(&g_font12);

	
	//好友成员数量
	Group->nFrdCnt = 0, Group->nFrdOnCnt = 0;
	CString strGroup ;
	//会不会有性能问题?  zhangmin
	GetOnlineCount(Group, Group->nFrdCnt, Group->nFrdOnCnt);
	if(Group->nFrdCnt > 0)
		strGroup.Format("%s %d/%d",Group->strTitle,Group->nFrdOnCnt,Group->nFrdCnt);
	else
		strGroup = Group->strTitle;

	if ( bDragFlag )
		pDC->SetTextColor(RGB(255,0,0));
	else
		pDC->SetTextColor(Group->clrText);

	if ( strGroup=="" )
	{
		strGroup="-";
	}

    //显示组名
	CSize  szGroupText =pDC->GetTextExtent(strGroup);
	CRect  rcGroupText;

	rcGroupText.left = rc.left + GRP_BTN_W + X_SPACE;
	rcGroupText.top  = rc.top + nTextTop;
	rcGroupText.bottom = rcGroupText.top + szGroupText.cy + 2;
	rcGroupText.right  = rcGroupText.left + szGroupText.cx + 7;

	Group->rcText = rcGroupText;
	Group->rcText.InflateRect(1,1);

	CBrush	 brushBack(0xECECEC);
	CBrush	 brushMulBack(CLR_MULSEL_BAK);
	CPen	 pen(PS_SOLID,1,RGB(0xEC,0xEC,0xEC));
	CBrush * pOldBrush	 = pDC->SelectObject(&brushBack);
	CPen *	 pOldPen	 = pDC->SelectObject(&pen);
/*
	if ( bSelectedGroup && m_posItemSelected.nItem==-1 )
		pDC->Rectangle(rcGroupText);
	*/
	//选中组
	/*
	if(m_posItemSelected.nGroup == Group->dwData && m_posItemSelected.nItem == -1)
		pDC->Rectangle(rcGroupText);
	*/
	pDC->TextOut( rcGroupText.left, rcGroupText.top, strGroup);


	//绘制子组和组内好友	
	if (Group->state == FLS_EXPAND)
	{

		if(Group->dwData == 1)
			Group->dwData = Group->dwData;
		//int nItemTop = rc.bottom;
		int nItemCount = Group->vecItem.size();
		for(int ii=0; ii< Group->vecSubGroup.size();ii++)
		{
			FL_GROUP_INFO* pSubGroup = Group->vecSubGroup[ii]; 

			DrawGroup( pSubGroup, pDC);
		}
/*		if(Group->vecSubGroup.size() > 0)
		{
			int id1 = Group->vecSubGroup.size()-1;
			if(Group->vecSubGroup[id1]->vecItem.size() > 0 && Group->vecSubGroup[id1]->state == FLS_EXPAND)
			{
				int id2 = Group->vecSubGroup[id1]->vecItem.size()-1;
				
				nItemTop = Group->vecSubGroup[id1]->vecItem[id2]->rcItem.bottom;
			}
			else
			{
				nItemTop = Group->vecSubGroup[id1]->rcBar.bottom;
			}
		}
		
*/
		// 绘制组内好友
		// 绘制组内好友
		for ( int ii=0; ii<nItemCount; ii++ )
		{
			int nItemHeight = m_nItemHeight;
			bool bSelItem = false;
			FL_ITEM_INFO *pItem = Group->vecItem[ii];
			ASSERT(pItem != NULL);
			// 判断是否有被选择的项，用来确定项高度
			// 判断是否有被选择的项，用来确定项高度
			 if(Group->dwData == 1 && ii == 0)
				 ii = ii;
			if(m_posItemSelected.nGroupID == Group->dwData && m_posItemSelected.nItem == ii)
			{
				bSelItem = true;
			}
			if( bSelItem ) 
			{
				nItemHeight += m_nSelItemHeight;
			}
			//设置Item的rect
			//Group->vecItem[ii]->rcItem.SetRect(rc.left + 3 ,nItemTop, rc.right - 2, nItemTop + nItemHeight);

			//绘制好友
			//绘制好友
			DrawFriend(Group->vecItem[ii], pDC, bSelItem);
			//更新高度
			//nItemTop += nItemHeight;
			//nItemTop += nItemHeight;
		}	
	}

	pDC->SelectObject(pOldBrush);
	pDC->SelectObject(pOldPen);
	pDC->SelectObject(pOldFont);
	pDC->RestoreDC(nSavedDC);
}



BOOL CIMFriendListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return FALSE;
}

void CIMFriendListCtrl::PreSubclassWindow() 
{
	m_TMS.cbSize = sizeof(TRACKMOUSEEVENT);
	m_TMS.dwFlags = TME_HOVER;
	m_TMS.hwndTrack = GetSafeHwnd();
	m_TMS.dwHoverTime = 1;
	TrackMouseEvent(&m_TMS); 

	CDC *pDC = GetDC();

	CFont *pOldfont=pDC->SelectObject(&g_font12);

	m_nTextHeight =pDC->GetTextExtent("H我").cy;

	m_nItemHeight = ITEM_H;

	m_nSelItemHeight = SEL_ITEM_H;

	pDC->SelectObject(pOldfont);

	ReleaseDC(pDC);

	this->EnableToolTips(TRUE);

	CIMListCtrl::PreSubclassWindow();
}

void CIMFriendListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	static int  sGroupIndex=-2, sItemIndex=-2;
	
	// 处理鼠标悬停
	if(!m_bTracking)
	{
		m_TMS.hwndTrack = GetSafeHwnd();
		m_bTracking = TrackMouseEvent(&m_TMS);
	} 
	else
	{	
		m_bTracking = FALSE;
	}

	int nGroup, nItem;
	HitTest(m_vecGroup, point, nGroup, nItem);

	m_posDragTo.nGroupID = nGroup;
	m_posDragTo.nItem  = nItem;

	if ( nGroup==-1 && nItem==-1 )
		return ;

	bool bInvalidate = false;

	if ( m_bMouseDown && nGroup!=sGroupIndex || nItem!=sItemIndex )
	{
		bInvalidate = true;
	}

	CPoint ptReal(point.x,point.y+m_OriginY);

	HCURSOR   hCursor = LoadCursor(NULL,IDC_ARROW);

	if ( m_bMouseDown )
	{
		if ( m_posItemSelected.nGroupID>-1 && m_posItemSelected.nItem>-1 )
		{
			HINSTANCE hInst   = AfxGetInstanceHandle();

			if ( m_posDragTo.nGroupID>-1 && m_posDragTo.nGroupID!=m_posItemSelected.nGroupID )
			{
				hCursor = ResLoadCursor(IDC_DROP);
			}
			else
				hCursor = ResLoadCursor(IDC_NODROP);
		}

		if ( m_bBeginDrag==false && m_posItemSelected.nGroupID!=nGroup || m_posItemSelected.nItem!=nItem )
		{
			bInvalidate = true;
			m_bBeginDrag =true;
			
			OnBeginDrag();
		}
	}

	//if ( nGroup!=sGroupIndex || nItem!=sItemIndex )
	//{
	//	if ( /*m_bMouseDown==false &&*/ nGroup!=-1 && nItem!=-1 )
	//	{
	//		//FL_ITEM_INFO *pItem = m_vecGroup[nGroup]->vecItem[nItem];
	//		
	//		FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup, nItem);
	//		CRect rc = pItem->rcItem;
	//		rc.right = rc.left+50;

	//		if ( rc.PtInRect(ptReal) )
	//			GetParent()->SendMessage(WM_IMFRIEND_LIST_NOTIFY,NM_FL_ITEM_OVER,(LPARAM)&m_posDragTo);
	//	}
	//}

	if ( nItem>-1)
	{
		//FL_ITEM_INFO *pItem = m_vecGroup[nGroup]->vecItem[nItem];
		FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup, nItem);

		if ( nItem==m_posItemSelected.nItem && nGroup==m_posItemSelected.nGroupID )
		{
			bool bMouseOnAlbum = pItem->rcBtnPic.PtInRect(ptReal);
			bool bMouseOnInfo  = pItem->rcBtnInfo.PtInRect(ptReal);
			bool bMouseOnArticle = pItem->rcBtnArticle.PtInRect(ptReal);

			if ( bMouseOnInfo )
			{
				hCursor=LoadCursor(NULL,IDC_HAND);
				//InvalidateRect(pItem->rcItem);

				if ( m_bMouseOnInfo==false )
				{			
					bInvalidate = true;
					m_bMouseOnInfo = true;
				}	


				goto END;
			}
			else
			{
				if ( m_bMouseOnInfo==true )
				{			
					bInvalidate = true;
					m_bMouseOnInfo = false;
				}	
			}

			if ( bMouseOnAlbum )
			{
				hCursor=LoadCursor(NULL,IDC_HAND);
				//InvalidateRect(pItem->rcItem);

				if ( m_bMouseOnAlbum==false )
				{			
					bInvalidate = true;
					m_bMouseOnAlbum = true;
				}	


				goto END;
			}
			else
			{
				if ( m_bMouseOnAlbum==true )
				{			
					bInvalidate = true;
					m_bMouseOnAlbum = false;
				}	
			}


			if ( bMouseOnArticle )
			{
				hCursor=LoadCursor(NULL,IDC_HAND);
				//InvalidateRect(pItem->rcItem);

				if ( m_bMouseOnArticle==false )
				{			
					bInvalidate = true;
					m_bMouseOnArticle= true;
				}	

				goto END;
			}
			else
			{
				if ( m_bMouseOnArticle==true )
				{			
					bInvalidate = true;
					m_bMouseOnArticle = false;
				}	
			}		
		}

		bool bMouseOnIcon  = pItem->rcIcon.PtInRect(ptReal);
		bool bMouseOnBlog  = pItem->rcBtnBlog.PtInRect(ptReal);
		bool bMouseOnWebInfo = pItem->rcBtnWebInfo.PtInRect(ptReal);
		bool bMouseOnVedio  = pItem->rcVedio.PtInRect(ptReal);
		bool bMouseOnMobile = pItem->rcMobile.PtInRect(ptReal);
//		bool bMouseOnFocus  = pItem->rcBtnFocus.PtInRect(ptReal);

		CString strTip;

		if ( bMouseOnIcon || bMouseOnBlog || bMouseOnWebInfo || bMouseOnVedio || bMouseOnMobile)
		{
			if ( bMouseOnBlog )
				hCursor=LoadCursor(NULL,IDC_HAND);
			
			m_bMouseOnIcon=bMouseOnIcon;
			m_bMouseOnBlog=bMouseOnBlog;
			m_bMouseOnWebInfo = bMouseOnWebInfo;
			m_bMouseOnVedio  = bMouseOnVedio;
			m_bMouseOnMobile = bMouseOnMobile;
//			m_bMouseOnFocus   = bMouseOnFocus;

			
			bInvalidate = true;
			
			//InvalidateRect(pItem->rcItem);
			FRIEND_INFO *pFriend=(FRIEND_INFO *)pItem->dwData;

			if ( bMouseOnIcon )
			{
				if ( strcmp(pFriend->user_sign,"")==0 )
					strTip.Format("%s 的头像",pFriend->nickname);
				else
					strTip.Format("%s - %s",pFriend->nickname,pFriend->user_sign);
			}

			if ( bMouseOnBlog )
			{
				strTip.Format("%s 的商友主页",pFriend->nickname);
			}

			if ( bMouseOnWebInfo )
			{
				if ( pFriend->webInfo & 0x02 )
					strTip.Format("%s上传了新的图片,点击这里去看看吧",pFriend->nickname);
				else
				if ( pFriend->webInfo & 0x01 )
					strTip.Format("%s上传了新的文章,点击这里去看看吧",pFriend->nickname);
			}

			if ( bMouseOnVedio )
			{
				strTip.Format("和%s视频聊天",pFriend->nickname);
			}

			if ( bMouseOnMobile )
			{
				strTip.Format("发手机短信给%s",pFriend->nickname);
			}

			ShowToolTip(strTip);			
		}
		else
		{
			ShowToolTip("");
		}
	}

END:
	sGroupIndex = nGroup;
	sItemIndex  = nItem;

	if ( bInvalidate )
	{
		InvalidateList();
	}
	SetCursor(hCursor);

	CIMListCtrl::OnMouseMove(nFlags,point);
}

void CIMFriendListCtrl::InvalidateItem(int nGroup, int nIndex, BOOL bErase /* = TRUE */)
{
	if ( ItemExist(nGroup,nIndex)==false )
		return ;
	
	CRect rcItem = (nIndex >= 0 ? m_vecGroup[nGroup]->vecItem[nIndex]->rcItem : m_vecGroup[nGroup]->rcBar);
	rcItem.OffsetRect(0, -m_OriginY);
	
	CRect rcClient;
	GetClientRect(&rcClient);
	if (rcItem.top >= rcClient.top || rcItem.bottom <= rcClient.bottom) 
	{
		InvalidateRect(&rcItem, bErase);
	}
}

void CIMFriendListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// 没有多选的时候
	if(m_multSelList.Size()==0)
	{
		int nHotGroup, nHotItem;
		HitTest(m_vecGroup, point, nHotGroup, nHotItem);

		point.y+=m_OriginY;

		m_posItemSelected.nGroupID = nHotGroup;
		m_posItemSelected.nItem  = nHotItem;
		InvalidateList();


		::PostMessage( GetParent()->m_hWnd,
			WM_IMFRIEND_LIST_NOTIFY,
			NM_FL_RBUTTONDOWN,
			MAKEWORD(nHotGroup,nHotItem));
	}

	
	CIMListCtrl::OnRButtonDown(nFlags, point);
}

void CIMFriendListCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	if(m_multSelList.Size()>0)
	{
		::PostMessage( GetParent()->m_hWnd,
			WM_IMFRIEND_LIST_NOTIFY,
			NM_FL_RBUTTONUP_M,
			(LPARAM)&m_multSelList);
	}
	else
	{
		int nHotGroup, nHotItem;
		HitTest(m_vecGroup,point, nHotGroup, nHotItem);

		::PostMessage( GetParent()->m_hWnd,
			WM_IMFRIEND_LIST_NOTIFY,
			NM_FL_RBUTTONUP,
			MAKEWORD(nHotGroup,nHotItem));
	}
	
	CIMListCtrl::OnRButtonUp(nFlags, point);
}

void CIMFriendListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	int			nPos;
	//bool		bB;

	// 先判断鼠标落在好友分组中的位置
	FL_ITEM_POS posCur2;  // 当前选择的项
	HitTest(m_vecGroup, point , posCur2.nGroupID, posCur2.nItem );


	TRACE("   new[%d %d]\n",  posCur2.nGroupID, posCur2.nItem );

	// 校正鼠标的绝对坐标
	point.y+=m_OriginY; 
	// 如果有被选中的好友项，则判断鼠标按下位置是否在该好友内的各热点区域内
	if ( posCur2.nItem>-1 && posCur2.nGroupID>-1 )
	{
		FL_ITEM_INFO *pItem = GetItem(m_vecGroup,posCur2.nGroupID ,posCur2.nItem);
		m_bMouseIconDown  = pItem->rcIcon.PtInRect(point);
		m_bMouseBlogDown  = pItem->rcBtnBlog.PtInRect(point);
		m_bMouseAlbumDown = pItem->rcBtnPic.PtInRect(point);
		m_bMouseArticleDown = pItem->rcBtnArticle.PtInRect(point);
		m_bMouseInfoDown  = pItem->rcBtnInfo.PtInRect(point);
		m_bMouseWebInfoDown = pItem->rcBtnWebInfo.PtInRect(point);
		m_bMouseVedioDown  = pItem->rcVedio.PtInRect(point);
		m_bMouseMobileDown = pItem->rcMobile.PtInRect(point);
//		m_bMouseFocusDown   = pItem->rcBtnFocus.PtInRect(point);
	}

	BOOL bCtrlDown  = KEYDOWN(VK_CONTROL);
	BOOL bShiftDown = KEYDOWN(VK_SHIFT);

	// 如果ctrl和shift都没有按下，则取消多选
	if(m_bCtrlDown==FALSE && m_bShiftDown==FALSE)
	{
		m_multSelList.Clear();

		if ( m_bMouseBlogDown || m_bMouseWebInfoDown || m_bMouseMobileDown || m_bMouseVedioDown )//|| m_bMouseFocusDown )//m_bMouseIconDown || 
		{
			m_bMouseDown=false;
		}
		else
		{
			// 只要不是博客主页按钮选中就设置当前选择的好友项

			m_posItemSelected = posCur2;

			GetParent()->PostMessage(WM_IMFRIEND_LIST_NOTIFY,NM_FL_SEL_CHANGE,0);
			m_bMouseDown = true;
			// 设置托拽操作
			SetTimer(0x0F,20,NULL);
		}
		// 保存最近一次选择项
		m_posLast=m_posItemSelected;
	}
	
	if( bCtrlDown == TRUE && m_bCtrlDown==TRUE )
	{
		// 判断上次是否有单选
		if(m_posItemSelected.nGroupID>-1 && m_posItemSelected.nItem>-1)
		{
			m_multSelList.AddPos(m_posItemSelected);
		}
		// 取消单选
		m_posItemSelected.nGroupID=-1;
		m_posItemSelected.nItem=-1;
		// 如果有ctrl按下则判断是连选还是反选
		nPos = m_multSelList.FindPos(posCur2);
		if(nPos == -1)
		{
			// 如果之前没有选择该项
			m_multSelList.AddPos(posCur2);
		}
		else
		{
			// 如果已经选择则反选
			m_multSelList.DelPos(posCur2);
		}
	}
	
	if( bShiftDown == TRUE && m_bShiftDown==TRUE )
	{
		// 取消单选
		m_posItemSelected.nGroupID=-1;
		m_posItemSelected.nItem=-1;
		
		// 跨组连选
		GetMultSel(m_posLast,posCur2,m_multSelList);
		 
	}

	m_bShiftDown = bShiftDown;
	m_bCtrlDown  = bCtrlDown;
	// 重绘列表
	InvalidateList();
	SetFocus();

	::PostMessage( GetParent()->m_hWnd,
				   WM_IMFRIEND_LIST_NOTIFY,
				   NM_FL_LBUTTONDOWN,
				   MAKEWORD(posCur2.nGroupID,posCur2.nItem));

	CIMListCtrl::OnLButtonDown(nFlags, point);
}

void CIMFriendListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	static FL_ITEM_POS posHot;
	int nHotGroup, nHotItem;
	
	// 判断当前鼠标位置
	HitTest(m_vecGroup, point, nHotGroup, nHotItem);

	point.y+=m_OriginY;

	m_bMouseDown = false;

	bool bInvalide = false;

	// 如果按下ctrl和shift键则忽略鼠键回落
	if(m_bCtrlDown || m_bShiftDown)
	{
		goto END;
	}
	
	posHot.nGroupID = nHotGroup, posHot.nItem = nHotItem;

	SetCursor(LoadCursor(NULL,IDC_ARROW));

	if ( nHotItem > -1 && nHotGroup > -1 )
	{
	
		FL_ITEM_INFO *pItem = GetItem(m_vecGroup,nHotGroup ,nHotItem);

		bool bIconClick = pItem->rcIcon.PtInRect(point);
		bool bBlogClick = pItem->rcBtnBlog.PtInRect(point);
		bool bAlbumClick = pItem->rcBtnPic.PtInRect(point);
		bool bArticleClick = pItem->rcBtnArticle.PtInRect(point);
		bool bInfoClick = pItem->rcBtnInfo.PtInRect(point);
		bool bWebInfoClick = pItem->rcBtnWebInfo.PtInRect(point);
		bool bVideoClick = pItem->rcVedio.PtInRect(point);
		bool bMobileClick = pItem->rcMobile.PtInRect(point);

		if (  bIconClick || bBlogClick || bInfoClick || bAlbumClick || bWebInfoClick || bArticleClick || bVideoClick || bMobileClick )
		{
			bInvalide    = true;

			if ( bIconClick && m_bMouseIconDown )
			{
				::PostMessage( GetParent()->m_hWnd,
						WM_IMFRIEND_LIST_NOTIFY,
						NM_FL_HEADICON_CLICK,
						(LPARAM)&posHot);
				goto END;
			}


			if ( bBlogClick && m_bMouseBlogDown )
			{
				::PostMessage( GetParent()->m_hWnd,
						WM_IMFRIEND_LIST_NOTIFY,
						NM_FL_BLOG_CLICK,
						(LPARAM)&posHot);
				goto END;
			}

			if ( bAlbumClick && m_bMouseAlbumDown )
			{
				::PostMessage( GetParent()->m_hWnd,
						WM_IMFRIEND_LIST_NOTIFY,
						NM_FL_ALBUM_CLICK,
						(LPARAM)&posHot);

				goto END;
			}

			if ( bArticleClick && m_bMouseArticleDown )
			{
				::PostMessage( GetParent()->m_hWnd,
					WM_IMFRIEND_LIST_NOTIFY,
					NM_FL_ARTICLE_CLICK,
					(LPARAM)&posHot);

				goto END;
			}

			if ( bInfoClick && m_bMouseInfoDown )
			{
				::PostMessage( GetParent()->m_hWnd,
						WM_IMFRIEND_LIST_NOTIFY,
						NM_FL_INFO_CLICK,
						(LPARAM)&posHot);

				goto END;
			}

			if ( bWebInfoClick && m_bMouseWebInfoDown )
			{
				::PostMessage( GetParent()->m_hWnd,
						WM_IMFRIEND_LIST_NOTIFY,
						NM_FL_WEBINFO_CLICK,
						(LPARAM)&posHot);

				goto END;
			}

			if ( bVideoClick && m_bMouseVedioDown )
			{
				::PostMessage( GetParent()->m_hWnd,
						WM_IMFRIEND_LIST_NOTIFY,
						NM_FL_VEDIO_CLICK,
						(LPARAM)&posHot);

				goto END;
			}

			if ( bMobileClick && m_bMouseMobileDown )
			{
				::PostMessage( GetParent()->m_hWnd,
						WM_IMFRIEND_LIST_NOTIFY,
						NM_FL_MOBILE_CLICK,
						(LPARAM)&posHot);

				goto END;
			}

			//if ( bFocusClick && m_bMouseFocusDown )
			//{
			//	::PostMessage( GetParent()->m_hWnd,
			//			WM_IMFRIEND_LIST_NOTIFY,
			//			NM_FL_FOCUS_CLICK,
			//			(LPARAM)&posHot);

			//	goto END;
			//}
		}
	}

	if ( m_bBeginDrag )
	{
		bInvalide    = true;
		m_bBeginDrag = false;
		OnEndDrag();
	}

	if ( nHotItem == m_posItemSelected.nItem && nHotGroup  == m_posItemSelected.nGroupID )
	{
		// 设置组的收合状态
		if (nHotItem  == -1 && nHotGroup >= 0) // 点击了分组栏，需要收放分组成员
		{
			int state = GetGroupState(nHotGroup);
			state = (state == FLS_EXPAND ? FLS_CLOSE : FLS_EXPAND);
			SetGroupState(nHotGroup, (FLS_STATE)state);

			//groupInfo保存开合
			int nSubGroupIndex = g_GroupInfo.FindItemIndex(nHotGroup);
			g_GroupInfo.group[nSubGroupIndex].group_state = state;
			
			bInvalide = true;
		}
	}

	
/*
	if ( nHotItem==m_posItemSelected.nItem && nHotGroup==m_posItemSelected.nGroup )
	{
		// 设置组的收合状态
		if (nHotItem == -1 && nHotGroup >= 0) // 点击了分组栏，需要收放分组成员
		{
			m_vecGroup[nHotGroup]->state = m_vecGroup[nHotGroup]->state == FLS_EXPAND ? FLS_CLOSE : FLS_EXPAND;
			bInvalide    = true;
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
			::PostMessage( GetParent()->m_hWnd,
				WM_IMFRIEND_LIST_NOTIFY,
				NM_FL_DRAGDROP,
				MAKELPARAM(nHotGroup,nHotItem));

			goto END;
		}
	}
	*/
	::PostMessage( GetParent()->m_hWnd,
		WM_IMFRIEND_LIST_NOTIFY,
		NM_FL_LBUTTONUP,
		(LPARAM)&posHot);

END:

	{
		m_bMouseAlbumDown  = false;
		m_bMouseInfoDown   = false;
		m_bMouseBlogDown   = false ;
		m_bMouseIconDown   = false ;
		m_bMouseWebInfoDown = false;
		m_bMouseVedioDown  = false;
		m_bMouseMobileDown = false;
//		m_bMouseFocusDown = false;
		m_bMouseArticleDown = false;
	}
	if ( bInvalide )
	{
		InvalidateList();
	}
	CIMListCtrl::OnLButtonUp(nFlags, point);
}

void CIMFriendListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	int nHotGroup, nHotItem;
	HitTest(m_vecGroup, point, nHotGroup, nHotItem);

	m_posItemSelected.nGroupID = nHotGroup;
	m_posItemSelected.nItem  = nHotItem;
	InvalidateList();

	if ( nHotGroup>-1 && nHotItem>-1 )
	{	
		FL_ITEM_INFO *pItem = GetItem(m_vecGroup,nHotGroup ,nHotItem);

		if ( pItem->rcBtnBlog.PtInRect(point) )//|| pItem->rcIcon.PtInRect(point) )
			return ;
	}

	::PostMessage( GetParent()->m_hWnd,
		WM_IMFRIEND_LIST_NOTIFY,
		NM_FL_LBUTTONDBLCLICK,
		0);

	CIMListCtrl::OnLButtonDblClk(nFlags,point);
}

void CIMFriendListCtrl::HitTest(vector<FL_GROUP_INFO *>vecGroup,const POINT& pt, int& nGroup, int& nItem)
{
	nGroup = nItem = -1;
	CPoint point(pt);
	point.y += m_OriginY;
	
	for (int iGroup = 0; iGroup < vecGroup.size(); iGroup++)
	{
		FL_GROUP_INFO *pGroup=  vecGroup[iGroup];
		
		if (pGroup->rcBar.PtInRect(point))
		{
			nGroup = pGroup->dwData;
			return;
		}
		
		if (pGroup->state == FLS_EXPAND)
		{
			HitTest(pGroup->vecSubGroup, pt, nGroup,nItem);
			if(nGroup != -1 || nItem != -1)
				return;
			for (int i = 0; i < pGroup->vecItem.size(); i++)
			{
				FL_ITEM_INFO *pItem= pGroup->vecItem[i];
				
				if (pItem->rcItem.PtInRect(point))
				{
					nGroup = pGroup->dwData;
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




void CIMFriendListCtrl::ShowEditBox(int nGroup)
{
	m_bRedraw=true;
	OnPaint();

	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroup);

	CRect rcBox =  pGroup->rcText;
	
	rcBox.right = pGroup->rcBar.right;
	rcBox.top    -= m_OriginY;
	rcBox.bottom -= m_OriginY;

	CGroupEdit * pEdit = new CGroupEdit(pGroup->dwData);
	pEdit->Create(WS_BORDER|WS_VISIBLE,rcBox,this,0);
	pEdit->SetFont(&g_font12);
	pEdit->SetWindowText(pGroup->strTitle);
	pEdit->SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	pEdit->BringWindowToTop();
	pEdit->SetFocus();
	//pEdit->SetSel(pGroup->strTitle.GetLength(),-1);
	pEdit->SetSel(0,-1);
	pEdit->LimitText(40-1);
}

int CIMFriendListCtrl::GetItemCount(int nGroupID)
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

void CIMFriendListCtrl::SetGroupData(int nGroup,DWORD dwData)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroup);
	pGroup->dwData=dwData;
}

void CIMFriendListCtrl::SetGroupFriendsCount(int nGroup,int nCount)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroup);
	pGroup->nFrdCnt = nCount;
}

void CIMFriendListCtrl::IncreaseFriend(int nFolder)
{
	ASSERT(0);
	//待删除
	return;
	m_vecGroup[nFolder]->nFrdCnt++;
}



FL_GROUP_INFO* CIMFriendListCtrl::GetGroupData(int nGroup)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroup);
	
	if (pGroup !=NULL)
		return 	pGroup;
	return NULL;
}

int CIMFriendListCtrl::GetGroupState(int nGroup)
{
	FL_GROUP_INFO *pGroup = GetGroup(m_vecGroup, nGroup);
	
	if (pGroup !=NULL)
		return 	pGroup->state;
	return -1;
}

BOOL CIMFriendListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	OnVScroll(zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, 0, 0);	
	return TRUE;
}

void CIMFriendListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
			m_bRedraw = true;
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
			InvalidateList();
		}
		
		CIMListCtrl::SetScrollPos(SB_VERT, nPos);
		break;
	default:
		;
	}
	
	//CIMListCtrl::ShowScrollBar(SB_VERT, FALSE);
}

void CIMFriendListCtrl::OnBeginDrag()
{
	for ( int i=0; i<m_vecGroup.size(); i++ )
	{
		m_vecGroup[i]->state_before_drag = m_vecGroup[i]->state;
		m_vecGroup[i]->state = FLS_CLOSE;
	}
}

void CIMFriendListCtrl::OnEndDrag()
{
	for ( int i=0; i<m_vecGroup.size(); i++ )
	{
		m_vecGroup[i]->state = m_vecGroup[i]->state_before_drag;
	}
}

int CIMFriendListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	m_ButtonImages.Create(12, 12, ILC_MASK | ILC_COLOR24, 2, 1);//+/-
	HBITMAP hbmp = ResLoadBitmap(IDB_GROUPLIST_BUTTON);//LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GROUPLIST_BUTTON));
	ImageList_AddMasked(m_ButtonImages.m_hImageList, hbmp, RGB(0, 255, 0));
	DeleteObject((HGDIOBJ)hbmp);	

	return 0;
}

void CIMFriendListCtrl::CalcScrollInfo()
{
	if (IsWindow(m_hWnd)) 
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		
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

#pragma warning (default: 4018)

BOOL CIMFriendListCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	//SetCursor(m_hCursor);
	return CIMListCtrl::OnSetCursor(pWnd, nHitTest, message);
}


void CIMFriendListCtrl::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 0x0F:
		{
			CRect rect;
			GetClientRect(&rect);
			ClientToScreen(&rect);

			CPoint pt;
			GetCursorPos(&pt);

			if ( !rect.PtInRect(pt) )
			{
				m_bMouseDown    = false;
				m_bBeginDrag    = false;

				m_bMouseAlbumDown  = false;
				m_bMouseInfoDown   = false;
				m_bMouseBlogDown   = false ;
				m_bMouseIconDown   = false ;
				m_bMouseWebInfoDown = false;
				m_bMouseVedioDown   = false;
				m_bMouseMobileDown  = false;
//				m_bMouseFocusDown = false;
				m_bMouseArticleDown = false;

				m_bMouseOnIcon = m_bMouseOnBlog = m_bMouseOnInfo = m_bMouseOnAlbum = m_bMouseOnVedio = m_bMouseOnMobile = false;

				if ( m_bBeginDrag )
				{
					m_bBeginDrag = false;
					OnEndDrag();
				}

				InvalidateList();

				KillTimer(0x0F);
			}

			if ( m_bMouseDown==false )
			{
				KillTimer(0x0F);
			}
		}
		break;
	case 0x0E:
		{
			g_strDot += ".";

			if (g_iDotCount++>2)
			{
				g_iDotCount  = 0  ;
				g_strDot     = "" ;
			}
			
			FL_GROUP_INFO *pGroup = m_vecGroup[0];

			CRect rcBar = pGroup->rcBar;
			rcBar.top-=m_OriginY;
			rcBar.bottom-=m_OriginY;

			this->m_bRedraw=true;
			this->InvalidateRect(&rcBar);
		}
		break;
	}

	CIMListCtrl::OnTimer(nIDEvent);
}

//void TrimSpace2(CString& sSrc)
//{
//	int i=0,j=0,nC=0;
//	nC=sSrc.GetLength();
//	if(nC==0)
//		return;
//	char* szBuf=new char[nC+1];
//	const char* p=sSrc;
//	char* pDest;
//	const char c=161;
//
//	// 去掉末尾的全角空格
//
//	p+=nC-1;
//	for(i=nC-1;i>=0;i--)
//	{
//		if( ((*p)==c) && ((*(p-1))==c) )
//		{
//			nC-=2;
//			p-=2;
//			continue;
//		}
//		else
//		{
//			break;
//		}
//	}
//
//	// 去掉首部的全角空格
//
//	p = sSrc;
//	pDest = szBuf;
//	for(i=0;i<nC;i++)
//	{
//		if( ((*p)==c) && ((*(p+1))==c) )
//		{
//			i++;
//			p+=2;
//			continue;
//		}
//		else
//		{
//			*pDest++ = *p++;
//		}
//	}
//	*pDest=0;
//	sSrc = szBuf;
//	delete [] szBuf;
//}


void	CIMFriendListCtrl::CloseAllGroup()
{
	vector<FL_GROUP_INFO *>::iterator it;
	FL_GROUP_INFO* pGroupInfo;
	
	for( it = m_vecGroup.begin();it!=m_vecGroup.end();++it)
	{
		pGroupInfo = *it;
		pGroupInfo->state = FLS_CLOSE;
	}
}


LRESULT CIMFriendListCtrl::OnGroupNameEdit(WPARAM w,LPARAM l)
{
	CString strGroupName = (LPCSTR)l;
	int     nGroup       = w;
	strGroupName.Trim();
	// 去掉全角空格
	TrimSpace2(strGroupName);
	

	if (strGroupName=="")
	{
		ShowMessage("组名称不可为空！",MB_OK|MB_ICONWARNING);
		return 1;
	}

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


	this->SetGroupName(w,strGroupName);

	m_posItemSelected.nGroupID = w;
	m_posItemSelected.nItem = -1;

	::PostMessage( GetParent()->m_hWnd,
		WM_IMFRIEND_LIST_NOTIFY,
		NM_FL_GROUPNAME,
		w);
	return 1;
}


void CIMFriendListCtrl::InvalidateList()
{
	if ( IsWindow(m_hWnd) && m_bInvalidateLock==false )
	{
		// 重新绘制标志
		m_bRedraw = true;
		Invalidate();
	}
}


BOOL CIMFriendListCtrl::PreTranslateMessage(MSG* pMsg)
{
	/*if ( pMsg->message==WM_KEYDOWN )
	{
		UINT nChar = pMsg->wParam;

		if ( nChar==VK_DOWN )
		{
			int nGroups = m_vecGroup.size();

			if ( nGroups>0 )
			{
				FL_ITEM_POS pos = this->GetCurSel();

				pos.nItem++;

				FL_GROUP_INFO *group = GetGroup(m_vecGroup, pos.nGroupID);
				if ( group == NULL )
				{
					pos.nGroupID=m_vecGroup[0]->dwData;
					pos.nItem=-1;
				}
				else
				{
					int nItems = group->vecItem.size();
					if ( pos.nItem<nItems )
					{
					}
					else
					{
						if ( nGroupIndex<(nGroups-1) )
						{
							pos.nGroupID = GetGroupIdByGroupIndex(++nGroupIndex);
							pos.nItem=-1;
						}
						else
						{
							pos.nItem--;
						}
					}

				}
		
	
				SetCurSel(pos.nGroupID,pos.nItem);
				GetParent()->PostMessage(WM_IMFRIEND_LIST_NOTIFY,NM_FL_SEL_CHANGE,0);
				return TRUE;
			}
		}

		if ( nChar==VK_UP )
		{
			int nGroups = m_vecGroup.size();

			if ( nGroups>0 )
			{
				FL_ITEM_POS pos = this->GetCurSel();

				pos.nItem--;

				int nGroupIndex = GetGroupIndexByGroupId(pos.nGroupID);
				if ( nGroupIndex==-1 )
				{
					pos.nGroupID=GetGroupIdByGroupIndex(0);
					pos.nItem=-1;
				}
				else
				{
					if ( pos.nItem>=0 )
					{
					}
					else
					{
						if ( nGroupIndex>0 )
						{
							pos.nGroupID = GetGroupIdByGroupIndex(--nGroupIndex);
							pos.nItem=m_vecGroup[nGroupIndex]->vecItem.size()-1;
						}
						else
						{
							if ( pos.nItem<0 )
								pos.nItem=-1;
						}
					}

				}
		
				SetCurSel(pos.nGroupID,pos.nItem);
				GetParent()->PostMessage(WM_IMFRIEND_LIST_NOTIFY,NM_FL_SEL_CHANGE,0);
				return TRUE;
			}
		}

	}*/

	return CIMListCtrl::PreTranslateMessage(pMsg);
}

void CIMFriendListCtrl::AddRefreshFlag()
{
	m_bRefreshFriendListFlag = true;
	SetTimer(0x0E,355,NULL);
}

void CIMFriendListCtrl::RemoveRefreshFlag()
{
	if ( m_bRefreshFriendListFlag )
	{
		m_bRefreshFriendListFlag = false;
		KillTimer(0x0E);
	}
}

int CIMFriendListCtrl::FindGroup(int nGroupID)
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






void CIMFriendListCtrl::FindUser(char cC)
{
	//待修改
	return;
	//int i,j;
	//char c;
	//int nIC;
	//int nGC;
	//nGC=GetGroupCount();
	//for(i=0;i<nGC;i++)
	//{
	//	nIC= GetItemCount(i);
	//	for(j=0;j<nIC;j++)
	//	{
	//		FRIEND_INFO*pFriend=(FRIEND_INFO*) GetItemData(i,j);
	//		c=GetFirstPinyin(pFriend->nickname);
	//		// 不区分大小写
	//		if(strnicmp(&cC,&c,1)==0)
	//		{
	//			m_cLastFindChar=c;
	//			m_nLastFindGroup=i;
	//			m_nLastFindItem=j;
	//			SetCurSel(i,j);
	//			return;
	//		}
	//	}
	//}
}

void CIMFriendListCtrl::FindUser(char cC,int nLastGroup,int nLastItem)
{
	//待修改
	return;

	//int i,j;
	//char c;
	//int nIC;
	//int nGC;
	//nGC=GetGroupCount();
	//// 从查询结果处向后查询
	//i=nLastGroup;
	//ASSERT(nLastGroup<=nGC);
	//nIC= GetItemCount(nLastGroup);
	//ASSERT(nLastItem<=nIC);
	//for(j=nLastItem+1;j<nIC;j++)
	//{
	//	FRIEND_INFO*pFriend=(FRIEND_INFO*) GetItemData(i,j);
	//	// 如果有备注名，优先备注名
	//	if(pFriend->comment_name[0]=='\0')
	//		c=GetFirstPinyin(pFriend->nickname);
	//	else
	//		c=GetFirstPinyin(pFriend->comment_name);
	//	// 不区分大小写
	//	if(strnicmp(&cC,&c,1)==0)
	//	{
	//		m_cLastFindChar=c;
	//		m_nLastFindGroup=i;
	//		m_nLastFindItem=j;
	//		SetCurSel(i,j);
	//		return;
	//	}
	//}
	//// 从上次查询结果所在的后一个组向后查询
	//for(i=nLastGroup+1;i<nGC;i++)
	//{
	//	nIC= GetItemCount(i);
	//	for(j=0;j<nIC;j++)
	//	{
	//		FRIEND_INFO*pFriend=(FRIEND_INFO*) GetItemData(i,j);
	//		// 如果有备注名，优先备注名
	//		if(pFriend->comment_name[0]=='\0')
	//			c=GetFirstPinyin(pFriend->nickname);
	//		else
	//			c=GetFirstPinyin(pFriend->comment_name);
	//		// 不区分大小写
	//		if(strnicmp(&cC,&c,1)==0)
	//		{
	//			m_cLastFindChar=c;
	//			m_nLastFindGroup=i;
	//			m_nLastFindItem=j;
	//			SetCurSel(i,j);
	//			return;
	//		}
	//	}
	//}
	//// 如果没有查询到，则返回起点重新查询
	//for(i=0;i<=nLastGroup;i++)
	//{
	//	nIC= GetItemCount(i);
	//	for(j=0;j<nIC;j++)
	//	{
	//		FRIEND_INFO*pFriend=(FRIEND_INFO*) GetItemData(i,j);
	//		// 如果有备注名，优先备注名
	//		if(pFriend->comment_name[0]=='\0')
	//			c=GetFirstPinyin(pFriend->nickname);
	//		else
	//			c=GetFirstPinyin(pFriend->comment_name);
	//		// 不区分大小写
	//		if(strnicmp(&cC,&c,1)==0)
	//		{
	//			m_cLastFindChar=c;
	//			m_nLastFindGroup=i;
	//			m_nLastFindItem=j;
	//			SetCurSel(i,j);
	//			return;
	//		}
	//	}
	//}

}


void CIMFriendListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nGC=0;
	int nIC=0;
	char cC=nChar;
	// 先判断是否有ctrl,shift键按下
	//if(nChar==VK_CONTROL)
	//{
	//	m_bCtrlDown = TRUE;
	//}
	//else if(nChar==VK_SHIFT)
	//{
	//	m_bShiftDown = TRUE;
	//}

/*
	if(nChar>=32 && nChar<127)
	{
		nGC=GetGroupCount();
		// 先判断上次是否查询了同样的名字
		if(strnicmp(&cC,&m_cLastFindChar,1)==0)
		{
			// 查找下一个相同首字母的用户
			if(m_nLastFindGroup>nGC)
			{
				// 如果组已经改变，则重新查找
				FindUser(cC);
				return;
			}
			nIC=GetItemCount(m_nLastFindGroup);
			if(m_nLastFindItem>nIC)
			{
				// 如果上次查询的用户所在组已经改变，则重新查找
				FindUser(cC);
				return;
			}
			// 从上次查询结果位置开始查询下一个用户
			FindUser(cC,m_nLastFindGroup,m_nLastFindItem);	
		}
		else
		{
			// 重新查找
			FindUser(cC);
		}

	}
*/
	CIMListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CIMFriendListCtrl::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// 判断是否有ctrl,shift键释放
	if(nChar==VK_CONTROL)
	{
		m_bCtrlDown = FALSE;
	}
	else if(nChar==VK_SHIFT)
	{
		m_bShiftDown = FALSE;
	}


	CIMListCtrl::OnKeyUp(nChar, nRepCnt, nFlags);
}

void CIMFriendListCtrl::GetMultSel(FL_ITEM_POS beg,FL_ITEM_POS end,MULTI_SEL_LST & rst)
{
	//待修改
	return;

	//rst.Clear();
	//if(beg.nGroup<0)
	//	beg.nGroup=0;
	//if(beg.nItem<0)
	//	beg.nItem=0;
	//if(end.nGroup<0)
	//	end.nGroup=0;
	//if(end.nItem<0)
	//	end.nItem=0;
	//FL_ITEM_POS temp;
	//// 确定起始终止点
	//if( (beg.nGroup>end.nGroup) ||
	//	((beg.nGroup==end.nGroup)&&(beg.nItem>end.nItem))
	//	)
	//{
	//	temp=beg;
	//	beg=end;
	//	end=temp;
	//}
	//int nGC=GetGroupCount();
	//int nIC=0;
	//int nGEnd;
	//int i,j;
	//int iC;
	//// 起始点超过当前总分组数，则返回
	//if(beg.nGroup>=nGC)
	//	return;
	//// 如果开始位置和结束位置在同一组，则判断它们之间的位置情况，考虑是否超出
	//if(beg.nGroup==end.nGroup)
	//{	
	//	temp.nGroup=beg.nGroup;
	//	nIC=GetItemCount(beg.nGroup);
	//	if(beg.nItem>=nIC)
	//		return;
	//	if(end.nGroup==beg.nGroup)
	//	{
	//		iC=end.nItem+1;
	//		if(iC>nIC)
	//			iC=nIC;
	//	}
	//	else
	//	{
	//		iC=nIC;
	//	}
	//	// 从开始位置到该组结尾（也可能为结束位置）
	//	for(i=beg.nItem;i<iC;i++)
	//	{
	//		temp.nItem=i;
	//		rst.PushBack(temp);
	//	}	
	//	return;
	//}
	//// 如果开始位置和结束位置在不同组
	//else
	//{
	//	// 先计算开始位置所在组
	//	temp.nGroup=beg.nGroup;
	//	nIC=GetItemCount(beg.nGroup);
	//	if(beg.nItem<nIC)
	//	{
	//		for(i=beg.nItem;i<nIC;i++)
	//		{
	//			temp.nItem=i;
	//			rst.PushBack(temp);
	//		}
	//	}
	//	// 再计算开始位置之后到结束位置之前的组
	//	nGEnd=end.nGroup-1;
	//	for(i=beg.nGroup+1;i<nGEnd;i++)
	//	{
	//		nIC=GetItemCount(i);
	//		temp.nGroup=i;
	//		for(j=0;j<nIC;j++)
	//		{
	//			temp.nItem=j;
	//			rst.PushBack(temp);
	//		}
	//	}
	//	// 计算结束位置所在的组
	//	temp.nGroup=end.nGroup;
	//	nIC=GetItemCount(end.nGroup);
	//	if(end.nItem>=nIC)
	//	{
	//		end.nItem=nIC-1;
	//	}
	//	for(i=0;i<=end.nItem;i++)
	//	{
	//		temp.nItem=i;
	//		rst.PushBack(temp);
	//	}
	//}	

}

// 获取所有组（循环遍历后的结果，不需要自行再遍历）
int CIMFriendListCtrl::GetAllGroup(vector<FL_GROUP_INFO *> &vecGroup)
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

// 鼠标悬停消息
void CIMFriendListCtrl::OnMouseHover(UINT nFlags, CPoint point)
{
	m_bTracking = FALSE;
	static int s_nGroup = -1;
	static int s_nItem = -1;

	CIMListCtrl::OnMouseHover(nFlags, point);
	int nGroup, nItem;
	HitTest(m_vecGroup, point, nGroup, nItem);

	if ( nGroup==-1 || nItem==-1 )
	{
		s_nGroup = s_nItem = -1;
		return;
	}

	CPoint ptReal(point.x,point.y);
	ptReal.y += m_OriginY;
	FL_ITEM_INFO *pItem = GetItem(m_vecGroup, nGroup, nItem);

	if ( pItem->rcIcon.PtInRect(ptReal) && (nGroup != s_nGroup || nItem != s_nItem))
	{
		s_nGroup = nGroup;
		s_nItem = nItem;
		m_posDragTo.nGroupID = nGroup;
		m_posDragTo.nItem  = nItem;
		GetParent()->SendMessage(WM_IMFRIEND_LIST_NOTIFY,NM_FL_ITEM_OVER,(LPARAM)&m_posDragTo);
	}

	if ( !pItem->rcIcon.PtInRect(ptReal) )
	{	// 发送离开悬停消息
		s_nGroup = s_nItem = -1;
	}
}

void CIMFriendListCtrl::OnSize(UINT nType, int cx, int cy)
{
	CIMListCtrl::OnSize(nType, cx, cy);
	m_nClientRealHeigh = 0;		// 改变m_nClientRealHeigh，size改变时让滚动条重新计算
	InvalidateList();
}