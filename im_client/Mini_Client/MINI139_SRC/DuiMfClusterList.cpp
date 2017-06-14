#include "stdafx.h"
#include "DuiMfClusterList.h"
#include <afxwin.h>
#include <afxcmn.h>

#pragma warning (disable: 4018)

#define TIMER_MOUSEMOVE  0x01
#define TIMER_LOADING    0x02
#define TIMER_FLASH		 0x03

#define ROOT_BAR_CY      26  /*我创建的群*/

#define LIST_START_X      10  /*我创建的群*/
#define GROUP_BAR_CY	 45  /*飞牛网(武汉)*/
#define TOOL_BTN_SIZE	 17
#define TOOL_BTN_SPACE   8
#define GRP_BTN_W 12
#define SPACE_X 8



static CString  g_strDot="";
static int      g_iDotCount=0;

void ShowMessage(CString strMessage,UINT nType/* =MB_OK|MB_ICONINFORMATION */);

#define ItemExistsAsserte(nGroup, nIndex) \
	_ASSERTE(nGroup >= 0 && nGroup < m_vecRoot[posGroup.nRoot]->vecGroup.size() && \
		nIndex >= 0 && nIndex < m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->vecItem.size());

/////////////////////////////////////////////////////////////////////////////
// CDuiClusterList
int GetOnlineCount(CL_GROUP_INFO *group)
{
	int n=0;
	CLUSTER_INFO *pClusterInfo = (CLUSTER_INFO*)(group->dwData);
	
	for ( int iFriend=0; iFriend<pClusterInfo->members.size(); iFriend++)
	{
		FRIEND_INFO *pFriend = (FRIEND_INFO *)&pClusterInfo->members[iFriend];
		if ( pFriend && pFriend->status!=XTREAM_OFFLINE )
		{
			n++;
		}
	}
	return n;
}

CDuiClusterList::CDuiClusterList(HWND hParent, DuiLib::CPaintManagerUI& paint_manager)
	: DuiLib::CTreeViewUI()
	, paint_manager_(paint_manager)
	, m_hParent(hParent)
{
}

void CDuiClusterList::DelMem()
{
	for(int nRoot = 0; nRoot < m_vecRoot.size(); nRoot++)
	{
		CL_ROOT_INFO* pRootInfo = m_vecRoot[nRoot];
		if(pRootInfo != NULL)
		{
			for(int nGroup = 0; nGroup < pRootInfo->vecGroup.size(); nGroup++)
			{
				CL_GROUP_INFO* pGroupInfo =  pRootInfo->vecGroup[nGroup];
				if(pGroupInfo != NULL)
				{
					delete pGroupInfo;
					pGroupInfo = NULL;
				}
			}
			delete pRootInfo;
			pRootInfo = NULL;
		}
	}
	m_vecRoot.clear();
}

CDuiClusterList::~CDuiClusterList()
{
	DelMem();
}

	//ON_WM_TIMER()

/////////////////////////////////////////////////////////////////////////////
BOOL CDuiClusterList::AddRoot_Interface(CString strTitle, int nRoot)
{
	DuiLib::CTreeNodeUI* pListElement = NULL;
	DuiLib::CDialogBuilder dlgBuilder;
	pListElement = static_cast<DuiLib::CTreeNodeUI*>(dlgBuilder.Create(_T("ClusterPage_Group.xml"), (UINT)0, this, &paint_manager_));
	if (pListElement == NULL)
		return FALSE;

	pListElement->SetFixedHeight(26);
	pListElement->SetTag(nRoot);
	pListElement->SetFixedWidth(150);

	DuiLib::CButtonUI *pLbCreateClusterIcon = static_cast<DuiLib::CButtonUI*>(pListElement->FindSubControl("btnCreateCluster"));
	if(nRoot != 0)
	{
		pLbCreateClusterIcon->SetVisible(false);
	}

	Add(pListElement);
	pListElement->SetVisibleFolderBtn(true);
	pListElement->GetFolderButton()->SetText(strTitle);

	return TRUE;
}

int CDuiClusterList::AddRoot(CString strRoot)
{
	CL_ROOT_INFO  *pR = new CL_ROOT_INFO();
	pR->strTitle = strRoot;
	m_vecRoot.push_back(pR);
	AddRoot_Interface(strRoot, (int)m_vecRoot.size()-1);
	return m_vecRoot.size()-1;
}

// 查找组节点
DuiLib::CTreeNodeUI* CDuiClusterList::FindCluster_Interface(int nData)
{
	int nCount = GetCount();
	for(int i = 0; nCount; ++i)
	{
		DuiLib::CTreeNodeUI *pItem = (DuiLib::CTreeNodeUI*)GetItemAt(i);
		if(!IsRootNote(pItem) && pItem->GetTag() == nData)
		{
			return (DuiLib::CTreeNodeUI*)pItem;
		}
	}

	return NULL;
}

// 界面上删除组节点
void CDuiClusterList::DelCluster_Interface(int nData)
{
	DuiLib::CTreeNodeUI *pChildNode = FindCluster_Interface(nData);
	if(pChildNode != NULL)
	{
		DuiLib::CTreeNodeUI *pParentNode = pChildNode->GetParentNode();
		Remove(pChildNode);

		pParentNode->GetCountChild();
		int nIndex = pParentNode->GetTag();
		CString strTitle = m_vecRoot[nIndex]->strTitle;

		CString strText;
		strText.Format("%s (%d)", strTitle, m_vecRoot[nIndex]->vecGroup.size());
		pParentNode->GetFolderButton()->SetText(strTitle);
	}
	UpdateRoot();
}


void CDuiClusterList::DelGroup(CL_GROUP_POS posGroup)
{
	vector<CL_GROUP_INFO *> & groups  = m_vecRoot[posGroup.nRoot]->vecGroup; 

	int nData = groups[posGroup.nGroup]->dwData;
	delete groups[posGroup.nGroup];
	groups.erase(groups.begin()+posGroup.nGroup);

	DelCluster_Interface(nData);
}

// 查找跟节点
DuiLib::CTreeNodeUI* CDuiClusterList::FindRoot_Interface(int nRoot)
{
	int nCount = GetCount();
	for(int i = 0; nCount; ++i)
	{
		DuiLib::CTreeNodeUI *pItem = (DuiLib::CTreeNodeUI*)GetItemAt(i);
		if(IsRootNote(pItem) &&  pItem->GetTag() == nRoot)
		{
			return (DuiLib::CTreeNodeUI*)pItem;
		}
	}

	return NULL;
}

// 检查节点是否为根节点
BOOL CDuiClusterList::IsRootNote(DuiLib::CTreeNodeUI *pNode)
{
	return pNode->FindSubControl("itemArea") == NULL;
}

// 界面添加组节点
BOOL CDuiClusterList::AddCluster_Interface(int nRoot, int nData)
{
	// 查找所属组
	DuiLib::CTreeNodeUI *pParent = FindRoot_Interface(nRoot);
	if(pParent == NULL)
	{
		return FALSE;
	}

	DuiLib::CTreeNodeUI* pListElement = NULL;
	DuiLib::CDialogBuilder dlgBuilder;
	pListElement = static_cast<DuiLib::CTreeNodeUI*>(dlgBuilder.Create(_T("ClusterPage_Item.xml"), (UINT)0, NULL, &paint_manager_));
	if (pListElement == NULL)
		return FALSE;
	pListElement->SetFixedHeight(46);
	pListElement->SetTag((DWORD)nData);

	DuiLib::CHorizontalLayoutUI *pArea = static_cast<DuiLib::CHorizontalLayoutUI*>(pListElement->FindSubControl("itemArea"));
	if(pArea == NULL)
		return FALSE;

	DuiLib::CDuiRect rcPadding = pParent->GetFolderButton()->GetPadding();
	rcPadding.left += 28;
	pArea->SetPadding(rcPadding);
	if( !pParent->AddChildNode(pListElement) )
	{
		return FALSE;
	}

	return UpdateCluster_Interface(pListElement, nData);
}

// 更新界面组节点
BOOL CDuiClusterList::UpdateCluster_Interface(DuiLib::CTreeNodeUI *pClusterNode, int nData)
{
	pClusterNode->SetFixedHeight(46);
	pClusterNode->SetTag((DWORD)nData);

	DuiLib::CLabelUI *pIcon = static_cast<DuiLib::CLabelUI*>(pClusterNode->FindSubControl("lbHeaderIcon"));
	DuiLib::CLabelUI *pName = static_cast<DuiLib::CLabelUI*>(pClusterNode->FindSubControl("lbName"));
	DuiLib::CLabelUI *pRemind = static_cast<DuiLib::CLabelUI*>(pClusterNode->FindSubControl("lbRemind"));
	DuiLib::CHorizontalLayoutUI *pArea = static_cast<DuiLib::CHorizontalLayoutUI*>(pClusterNode->FindSubControl("itemArea"));
	DuiLib::CLabelUI *pStatus = static_cast<DuiLib::CLabelUI*>(pClusterNode->FindSubControl("lbStatus"));
	if(pIcon == NULL || pName == NULL || pRemind == NULL || pArea == NULL)
		return FALSE;

	// 更新群头像（现在使用默认）

	// 群名字
	CLUSTER_INFO *pCluser = (CLUSTER_INFO*)nData;
	pName->SetText(pCluser->szFamilyName);
	int nWidth = pName->CalcRealWidth();
	pName->SetFixedWidth(nWidth);

	// 未读消息数
	if(pCluser->nMsgNum > 0)
	{
		pRemind->SetVisible(true);
		CString strCount;
		strCount.Format("%d", pCluser->nMsgNum);
		if(pCluser->nMsgNum > 99)
		{
			strCount = "99+";
		}
		pRemind->SetText(strCount);
		int nWidth = pRemind->CalcRealWidth();
		if(nWidth < 14)	nWidth = 14;
		pRemind->SetFixedWidth(nWidth + 4);
	}
	else
	{
		pRemind->SetVisible(false);
	}
	
	UpdateRoot();
}

void CDuiClusterList::UpdateGroupImage(CL_GROUP_POS posGroup)
{
	if ( posGroup.IsNull()==false )
	{
		CLUSTER_INFO * pCluster = (CLUSTER_INFO *)m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->dwData;

		if ( pCluster )
		{
			/*m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->pBitmap40 = GetClusterBitmap(pCluster,false);
			m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->pBitmap20 = GetClusterBitmap(pCluster,true);
			InvalidateList();*/
		}
	}
}

CL_GROUP_POS CDuiClusterList::AddGroup(int nRoot, CLUSTER_INFO *pCluster)
{
	CL_GROUP_POS posGroup;

	if ( nRoot>-1 && nRoot<m_vecRoot.size() )
	{
		CL_GROUP_INFO * Group = new CL_GROUP_INFO();

		strncpy(Group->szTitle,pCluster->szFamilyName,sizeof(Group->szTitle) - 1);
		Group->clrText = RGB(0,0,180);
		Group->dwData = (DWORD)pCluster;
		
		m_vecRoot[nRoot]->vecGroup.push_back(Group);

		posGroup.nRoot  = nRoot;
		posGroup.nGroup = m_vecRoot[nRoot]->vecGroup.size()-1;

		AddCluster_Interface(nRoot, (int)pCluster);
	}

	return posGroup;
}

void CDuiClusterList::SetGroupTextColor(CL_GROUP_POS posGroup, COLORREF clrText)
{
	if (posGroup.IsNull()==false)
	{
		m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->clrText = clrText;
		//InvalidateList();
	}
}

void CDuiClusterList::SwapGroup(CL_GROUP_POS pos1, CL_GROUP_POS pos2)
{
	if ( pos1==pos2 || pos1.IsNull() || pos2.IsNull() )
	{
		return ;
	}
	else
	{
		CL_GROUP_INFO * p1 = m_vecRoot[pos1.nRoot]->vecGroup[pos1.nGroup];
		CL_GROUP_INFO * p2 = m_vecRoot[pos2.nRoot]->vecGroup[pos2.nGroup];

		DuiLib::CTreeNodeUI *pNode1 = FindCluster_Interface(p1->dwData);
		DuiLib::CTreeNodeUI *pNode2 = FindCluster_Interface(p2->dwData);

		m_vecRoot[pos1.nRoot]->vecGroup[pos1.nGroup] = p2;
		m_vecRoot[pos2.nRoot]->vecGroup[pos2.nGroup] = p1;

		UpdateCluster_Interface(pNode1, p2->dwData);
		UpdateCluster_Interface(pNode2, p1->dwData);
	}
}

void CDuiClusterList::ClearCurSel()
{
	m_posGroupSelected.nGroup = -1;
	m_posGroupSelected.nRoot = -1;
}

CL_GROUP_POS CDuiClusterList::FindCluster(int nClusterId)
{
	CL_GROUP_POS posSel;

	int nRoot = m_vecRoot.size();
	for ( int n=0; n<nRoot; n++ )
	{
		vector<CL_GROUP_INFO *> & groups = m_vecRoot[n]->vecGroup;

		int nCGroup = groups.size();
		int i;
		CLUSTER_INFO * pInfo;
		for(i = 0;i < nCGroup;i++)
		{
			pInfo = (CLUSTER_INFO *)groups[i]->dwData;
			if(pInfo)
			{
				if(pInfo->nFamilyID == nClusterId)
				{
					posSel.nRoot = n;
					posSel.nGroup = i;
					return posSel;
				}
			}
		}
	}

	return posSel;
}

CL_GROUP_POS CDuiClusterList::GetCurSel()
{
	return m_posGroupSelected;
}

void CDuiClusterList::OnRButtonDown(UINT nFlags, CPoint point)
{
	/*CL_ITEM_POS posHot;
	HitTest(point, posHot);

	m_posGroupSelected = posHot;

	InvalidateList();


	::SendMessage( m_hParent,
	WM_CLUSTER_LIST_NOTIFY,
	NM_CL_RBUTTONDOWN,
	(LPARAM)&posHot);

	CIMListCtrl::OnRButtonDown(nFlags, point);*/
}

void CDuiClusterList::OnRButtonUp(UINT nFlags, CPoint point)
{
	/*CL_ITEM_POS posHot;
	HitTest(point, posHot);

	::SendMessage( m_hParent,
		WM_CLUSTER_LIST_NOTIFY,
		NM_CL_RBUTTONUP,
		(LPARAM)&posHot);

	m_bMouseDown=false;

	CIMListCtrl::OnRButtonUp(nFlags, point);*/
}

CL_GROUP_INFO * CDuiClusterList::GetGroupInfo(CL_GROUP_POS posGroup)
{
	if ( posGroup.IsNull()==false )
		return m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup];
	else
		return NULL;
}

DWORD CDuiClusterList::GetGroupData(CL_GROUP_POS posGroup)
{
	if ( posGroup.IsNull()==false )
		return 	m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->dwData;
	else
		return 0;
}

int CDuiClusterList::GetGroupCount(int nRoot)
{
	return m_vecRoot[nRoot]->vecGroup.size();
}

int CDuiClusterList::GetRootCount()
{
	return m_vecRoot.size();
}

int CDuiClusterList::GetGroupCount()
{
	int n=0;
	for ( int i=0; i<m_vecRoot.size(); i++ )
		n+=m_vecRoot[i]->vecGroup.size();

	return n;
}

//void CDuiClusterList::OnTimer(UINT nIDEvent)
//{
//	switch ( nIDEvent )
//	{
//	case TIMER_LOADING:
//		{
//			g_strDot += ".";
//
//			if (g_iDotCount++>2)
//			{
//				g_iDotCount  = 0  ;
//				g_strDot     = "" ;
//			}
//			
//			for ( int nRoot=0; nRoot<m_vecRoot.size(); nRoot++ )
//			{
//				if ( m_vecRoot[nRoot]->state==CLS_EXPAND )
//				{
//					int nGroup = m_vecRoot[nRoot]->vecGroup.size();
//
//					for ( int i=0; i<nGroup; i++ )
//					{
//						CL_GROUP_INFO *pGroup = m_vecRoot[nRoot]->vecGroup[i];
//
//						if ( pGroup->bLoadingMembers )
//						{
//							CRect rcBar = pGroup->rcBar;
//							rcBar.top    -= m_OriginY  ;
//							rcBar.bottom -= m_OriginY  ;
//							m_bRedraw=true;
//							InvalidateRect(&rcBar);
//						}
//					}		
//				}
//		}
//		}
//		break;
//	case TIMER_FLASH:
//		{
//			for ( int nRoot=0; nRoot<m_vecRoot.size(); nRoot++ )
//			{
//				BOOL bRootExpand = (m_vecRoot[nRoot]->state==CLS_EXPAND);
//		
//				int nGroup = m_vecRoot[nRoot]->vecGroup.size();
//
//				for ( int i=0; i<nGroup; i++ )
//				{
//					CL_GROUP_INFO *pGroup = m_vecRoot[nRoot]->vecGroup[i];
//
//					if ( pGroup->bFlash )
//					{
//						pGroup->iFlash++;
//						if ( bRootExpand )
//						{
//							CRect rcBar = pGroup->rcBar;
//							rcBar.top    -= m_OriginY  ;
//							rcBar.bottom -= m_OriginY  ;
//							m_bRedraw=true;
//							InvalidateRect(&rcBar);
//						}
//						else
//						{
//							break;
//						}
//					}
//				}
//						
//				if ( bRootExpand==FALSE )
//				{
//					CRect rcBar = m_vecRoot[nRoot]->rcBar;
//					rcBar.top    -= m_OriginY  ;
//					rcBar.bottom -= m_OriginY  ;
//					m_bRedraw=true;
//					InvalidateRect(&rcBar);
//				}
//			}
//		}
//		break;
//	case TIMER_MOUSEMOVE:
//		{
//			CPoint ptCursor;
//			CRect  rcClient;
//
//			GetCursorPos(&ptCursor);
//			GetWindowRect(&rcClient);
//
//			if ( rcClient.PtInRect(ptCursor)==FALSE )
//			{
//				m_bMouseOver=false;
//				InvalidateList();
//				KillTimer(TIMER_MOUSEMOVE);
//			}
//		}
//		break;
//	}
//
//	CIMListCtrl::OnTimer(nIDEvent);
//}
//
//void CDuiClusterList::AddFlashFlag(CL_GROUP_POS posGroup)
//{
//	if ( posGroup.IsNull()==false )
//	{
//		int  nCount = m_vecRoot[posGroup.nRoot]->vecGroup.size();
//		bool bFlag  = false;
//
//		for ( int i=0; i<nCount; i++ )
//		{
//			if ( m_vecRoot[posGroup.nRoot]->vecGroup[i]->bFlash )
//			{
//				bFlag=true;
//				break;
//			}
//		}
//
//		if ( bFlag==false )
//		{
//			KillTimer(TIMER_FLASH);
//			SetTimer(TIMER_FLASH, 355, NULL );
//		}
//
//		m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->bFlash = true;
//	}
//}
//
//void CDuiClusterList::AddLoadFlag(CL_GROUP_POS posGroup)
//{
//	if ( posGroup.IsNull()==false )
//	{
//		int  nCount = m_vecRoot[posGroup.nRoot]->vecGroup.size();
//		bool bFlag  = false;
//
//		for ( int i=0; i<nCount; i++ )
//		{
//			if ( m_vecRoot[posGroup.nRoot]->vecGroup[i]->bLoadingMembers )
//			{
//				bFlag=true;
//				break;
//			}
//		}
//
//		if ( bFlag==false )
//		{
//			KillTimer(TIMER_LOADING);
//			SetTimer(TIMER_LOADING, 355, NULL );
//			InvalidateGroup(posGroup);
//		}
//
//		m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->bLoadingMembers = true;
//	}
//}
//
//void CDuiClusterList::RemoveFlashFlag(CL_GROUP_POS posGroup)
//{
//	if ( posGroup.IsNull()==false )
//	{
//		m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->bFlash = false;
//
//		int  nCount = m_vecRoot[posGroup.nRoot]->vecGroup.size();
//		bool bFlag  = false;
//
//		for ( int i=0; i<nCount; i++ )
//		{
//			if ( m_vecRoot[posGroup.nRoot]->vecGroup[i]->bFlash )
//			{
//				bFlag=true;
//				break;
//			}
//		}
//
//		if ( bFlag==false )
//		{
//			KillTimer(TIMER_FLASH);
//			InvalidateGroup(posGroup);
//		}
//	}
//}
//
//void CDuiClusterList::RemoveLoadFlag(CL_GROUP_POS posGroup)
//{
//	if ( posGroup.IsNull()==false )
//	{
//		m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->bLoadingMembers = false;
//
//		int  nCount = m_vecRoot[posGroup.nRoot]->vecGroup.size();
//		bool bFlag  = false;
//
//		for ( int i=0; i<nCount; i++ )
//		{
//			if ( m_vecRoot[posGroup.nRoot]->vecGroup[i]->bLoadingMembers )
//			{
//				bFlag=true;
//				break;
//			}
//		}
//
//		if ( bFlag==false )
//		{
//			::g_strDot = "";
//			::g_iDotCount = 0;
//
//			KillTimer(TIMER_LOADING);
//		}
//	}
//}

bool CDuiClusterList::SelectItem(int iIndex, bool bTakeFocus /*= false*/)
{
	m_posGroupSelected.nRoot = -1;
	m_posGroupSelected.nGroup = -1;

	bool bSuccess = CTreeViewUI::SelectItem(iIndex, bTakeFocus);
	if(bSuccess)
	{
		int nSel = CTreeViewUI::GetCurSel();
		DuiLib::CTreeNodeUI *pNode = (DuiLib::CTreeNodeUI *)CTreeViewUI::GetItemAt(nSel);
		if(IsRootNote(pNode))
		{	// 为组节点
			m_posGroupSelected.nRoot = pNode->GetTag();
			m_posGroupSelected.nGroup = -1;
		}
		else
		{
			DuiLib::CTreeNodeUI *pParentNode = pNode->GetParentNode();
			m_posGroupSelected.nRoot = pParentNode->GetTag();

			for(int i = 0; i < m_vecRoot[m_posGroupSelected.nRoot]->vecGroup.size(); ++i)
			{
				if(m_vecRoot[m_posGroupSelected.nRoot]->vecGroup[i]->dwData == pNode->GetTag())
				{
					m_posGroupSelected.nGroup = i;
					break;
				}
			}
		}
	}

	return bSuccess;
}

void CDuiClusterList::SetCurSel(CL_GROUP_POS posGroup)
{
	if ( posGroup.IsNull() )
	{
		return;
	}

	DuiLib::CTreeNodeUI *pRootNode = FindRoot_Interface(posGroup.nRoot);
	if(pRootNode == NULL)
		return;

	DWORD dwData = m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->dwData;
	int nChileCount = pRootNode->GetCountChild();
	for(int i = 0; i < nChileCount; ++i)
	{
		if(pRootNode->GetChildNode(i)->GetTag() == dwData)
		{
			pRootNode->GetChildNode(i)->Select();
			return;
		}
	}
}

DuiLib::CControlUI* CDuiClusterList::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

// 更新根节点
void CDuiClusterList::UpdateRoot()
{
	for(int i = 0; i < m_vecRoot.size(); ++i)
	{
		DuiLib::CTreeNodeUI *pRoot = FindRoot_Interface(i);
		if(pRoot != NULL)
		{
			int nChildCount = pRoot->GetCountChild();
			if(nChildCount > 0)
			{
				CString strTit;
				strTit.Format("%s (%d)", m_vecRoot[i]->strTitle, nChildCount);
				pRoot->GetFolderButton()->SetText(strTit);
			}
		}
	}
}

// 更新组
void CDuiClusterList::UpdateGroup(CLUSTER_INFO *pCluster)
{
	DuiLib::CTreeNodeUI *pNode = FindCluster_Interface((int)pCluster);
	if(pNode != NULL)
	{
		UpdateCluster_Interface(pNode, (int)pCluster);
	}
}

// 展开节点
void CDuiClusterList::ExpandRoot(int nRoot, BOOL bExpand)
{
	DuiLib::CTreeNodeUI *pNode = FindRoot_Interface(nRoot);
	if(pNode != NULL)
	{
		pNode->GetFolderButton()->Selected(bExpand);
	}
}

// 根节点是否展开
BOOL CDuiClusterList::IsExpand(int nRoot)
{
	DuiLib::CTreeNodeUI *pNode = FindRoot_Interface(nRoot);
	if(pNode != NULL)
	{
		return pNode->GetFolderButton()->IsSelected();
	}

	return FALSE;
}