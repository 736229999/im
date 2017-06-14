#include "stdafx.h"
#include "GroupSendAddOtherDlg.h"
#include "im\WebProcess.h"
#include "TabWndSearch.h"
#include "SearchListWnd.h"
#include "MainFrame.h"
#include "DuiMfFriendList.h"

extern CWebUserHeadIconProcess g_webUserHeadIconProcess;
extern string CutRoundImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE);
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
static long s_SearchEditStartChar = 0;
CDuiSelFriendList::CDuiSelFriendList(DuiLib::CPaintManagerUI& paint_manager)
	: DuiLib::CTreeViewUI()
	, paint_manager_(paint_manager)
{

}

// 判断好友列表中的item是否为分组
BOOL CDuiSelFriendList::IsGroupItem(DuiLib::CTreeNodeUI *pItem)
{
	return pItem->FindSubControl("itemArea") == NULL;
}

BOOL CDuiSelFriendList::AddGroup_Interface(int nGroupId, CString strTitle)
{
	int nIndex = g_GroupInfo.FindItemIndex(nGroupId);
	if(nIndex == -1)
		return FALSE;

	int nFarentGroupID = g_GroupInfo.group[nIndex].group_father;

	DuiLib::CTreeNodeUI* pListElement = NULL;
	DuiLib::CDialogBuilder dlgBuilder;
	pListElement = static_cast<DuiLib::CTreeNodeUI*>(dlgBuilder.Create(_T("GroupSendAddOther_FriendGroup.xml"), (UINT)0, NULL, &paint_manager_));
	if (pListElement == NULL)
		return FALSE;

	DuiLib::CCheckBoxUI *pCheck = static_cast<DuiLib::CCheckBoxUI*>(pListElement->FindSubControl("checkSel"));
	pCheck->OnNotify += MakeDelegate(this,&CDuiSelFriendList::OnCheckBoxChanged);

	pListElement->SetFixedHeight(26);
	pListElement->SetTag(nGroupId);

	// 找到父类
	if(nFarentGroupID == -1)
	{
		Add(pListElement);
		pListElement->SetVisibleFolderBtn(true);
		pListElement->GetFolderButton()->SetText(strTitle);
	}
	else
	{
		DuiLib::CTreeNodeUI *pParent = FindGroupNode(nFarentGroupID);
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
		DuiLib::CDuiRect rcPadding = pParent->GetTreeNodeHoriznotal()->GetPadding();
		rcPadding.left += 19;
		pListElement->GetTreeNodeHoriznotal()->SetPadding(rcPadding);

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

	return TRUE;
}

// 查找组节点
DuiLib::CTreeNodeUI* CDuiSelFriendList::FindGroupNode(int nGroupId)
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

// 查找好友node
DuiLib::CTreeNodeUI* CDuiSelFriendList::FindItem_Interface(int nGroupId, DWORD dwData)
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

// 好友列表添加好友
BOOL CDuiSelFriendList::AddItem_Interface(FRIEND_INFO *pFriend)
{
	// 查找所属组
	DuiLib::CTreeNodeUI *pParent = FindGroupNode(pFriend->group_index);
	if(pParent == NULL)
	{
		return FALSE;
	}

	DuiLib::CTreeNodeUI* pListElement = NULL;
	DuiLib::CDialogBuilder dlgBuilder;
	pListElement = static_cast<DuiLib::CTreeNodeUI*>(dlgBuilder.Create(_T("GroupSendAddOther_FriendItem.xml"), (UINT)0, NULL, &paint_manager_));
	if (pListElement == NULL)
		return FALSE;

	DuiLib::CCheckBoxUI *pCheck = static_cast<DuiLib::CCheckBoxUI*>(pListElement->FindSubControl("checkSel"));
	pCheck->OnNotify += MakeDelegate(this,&CDuiSelFriendList::OnCheckBoxChanged);

	pListElement->SetFixedHeight(28);
	pListElement->SetTag((DWORD)(0x80000000+pFriend->GetId()));

	DuiLib::CDuiRect rcPadding = pParent->GetTreeNodeHoriznotal()->GetPadding();
	rcPadding.left += 22;
	pListElement->GetTreeNodeHoriznotal()->SetPadding(rcPadding);
	if( !pParent->AddChildNode(pListElement) )
	{
		return FALSE;
	}

	return UpdateItem(pListElement, pFriend);
}

// 更新好友节点内容
BOOL CDuiSelFriendList::UpdateItem(DuiLib::CTreeNodeUI *pNode, FRIEND_INFO *pFriend)
{
	// 是否有备注名称
	CString sName = UserInfoToStr(pFriend);

	pNode->SetFixedHeight(28);
	pNode->SetTag((DWORD)(0x80000000+pFriend->GetId()));

	DuiLib::CButtonGifUI *pIcon = static_cast<DuiLib::CButtonGifUI*>(pNode->FindSubControl("lbHeaderIcon"));
	DuiLib::CLabelUI *pNickName = static_cast<DuiLib::CLabelUI*>(pNode->FindSubControl("lbNickName"));
	DuiLib::CHorizontalLayoutUI *pArea = static_cast<DuiLib::CHorizontalLayoutUI*>(pNode->FindSubControl("itemArea"));
	if(pIcon == NULL || pNickName == NULL || pArea == NULL)
		return FALSE;

	CString strFile = g_webUserHeadIconProcess.GetFaceFile(pFriend);
	CString strGifFile = "";
	CString strFaceFile = pFriend->face_url;
    if(strFaceFile.Find(".gif")!=-1)
	{
		strGifFile =  strFile + ".gif";////文件改名
		if ( !FileExist(strGifFile) )////不存在则创建
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
				string strHeadIcon = CutRoundImg(strFile, 26, 26, !bOnline);////是否在线的判断
				pIcon->SetBkImage(strHeadIcon.c_str());
				pIcon->SetNormalGifFile("");
			}
		}
		else
		{
			string strHeadIcon = CutRoundImg(strFile, 26, 26, !bOnline);
			pIcon->SetBkImage(strHeadIcon.c_str());
			pIcon->SetNormalGifFile("");
		}
	}

	pNickName->SetText(sName);
}

void CDuiSelFriendList::Notify(DuiLib::TNotifyUI& msg)
{
}

bool CDuiSelFriendList::OnCheckBoxChanged( void* param )
{
	DuiLib::TNotifyUI* pMsg = (DuiLib::TNotifyUI*)param;
	if(pMsg->sType == _T("selectchanged"))
	{
		DuiLib::CCheckBoxUI* pCheckBox = (DuiLib::CCheckBoxUI*)pMsg->pSender;
		DuiLib::CTreeNodeUI* pItem = (DuiLib::CTreeNodeUI*)pCheckBox->GetParent()->GetParent();
		SetItemCheckBox(pCheckBox->GetCheck(),pItem);

		if(pItem->GetParentNode() != NULL)  //edit by:Redrain  2014.12.11
			IsAllChildChecked(pItem->GetParentNode());

		return true;
	}
	return true;
}

void CDuiSelFriendList::IsAllChildChecked(DuiLib::CTreeNodeUI *pNode)
{	
	bool bIsAllChildChecked = true;
	bool bIsAllChildUncheck = true;
	int nCount = pNode->GetCountChild();
	if(nCount > 0)
	{			
		DuiLib::CCheckBoxUI *pCheck = static_cast<DuiLib::CCheckBoxUI*>(pNode->FindSubControl("checkSel"));
		for(int nIndex = 0;nIndex < nCount;nIndex++)
		{
			DuiLib::CTreeNodeUI* pItem = pNode->GetChildNode(nIndex);
			DuiLib::CCheckBoxUI *pCheckItem = static_cast<DuiLib::CCheckBoxUI*>(pItem->FindSubControl("checkSel"));
			if(!pCheckItem->IsSelected())
			{
				bIsAllChildChecked = false;
			}
			else
			{
				bIsAllChildUncheck = false;
			}
		}		
		/*if(!bIsAllChildChecked && !bIsAllChildUncheck)
		{
			pCheck->SetSelectedImage("SysBtn\\radiobox_pressed.png");
			pCheck->SetSelectedHotImage("SysBtn\\radioboxed_hover.png");
			pCheck->SetNormalImage("SysBtn\\radiobox_pressed.png");
			pCheck->SetHotImage("SysBtn\\radioboxed_hover.png");
		}
		else
		{
			pCheck->SetSelectedImage("SysBtn\\check_box_pressed.png");
			pCheck->SetSelectedHotImage("SysBtn\\checked_box_hover.png");
			pCheck->SetNormalImage("SysBtn\\check_box_normal.png");
			pCheck->SetHotImage("SysBtn\\check_box_hover.png");
		}*/

		if (bIsAllChildChecked && !pCheck->IsSelected())
		{
			pCheck->Selected(true);
			return;
		}
		else if (bIsAllChildUncheck && pCheck->IsSelected())
		{
			pCheck->Selected(false);
			return;
		}

		
	}
}

bool CDuiSelFriendList::SetItemCheckBox( bool _Selected,DuiLib::CTreeNodeUI* _TreeNode /*= NULL*/ )
{
	if(_TreeNode)
	{
		if(_TreeNode->GetCountChild() > 0)
		{
			int nCount = _TreeNode->GetCountChild();
			for(int nIndex = 0;nIndex < nCount;nIndex++)
			{
				DuiLib::CTreeNodeUI* pItem = _TreeNode->GetChildNode(nIndex);
				DuiLib::CCheckBoxUI *pCheck = static_cast<DuiLib::CCheckBoxUI*>(pItem->FindSubControl("checkSel"));
				pCheck->Selected(_Selected);
				if(pItem->GetCountChild())
					SetItemCheckBox(_Selected,pItem);
			}
		}
		return true;
	}
	else
	{
		int nIndex = 0;
		int nCount = GetCount();
		while(nIndex < nCount)
		{
			DuiLib::CTreeNodeUI* pItem = (DuiLib::CTreeNodeUI*)GetItemAt(nIndex);
			DuiLib::CCheckBoxUI *pCheck = static_cast<DuiLib::CCheckBoxUI*>(pItem->FindSubControl("checkSel"));
			pCheck->Selected(_Selected);
			if(pItem->GetCountChild())
				SetItemCheckBox(_Selected,pItem);

			nIndex++;
		}
		return true;
	}
	return false;
}

// 获取现在的好友
void CDuiSelFriendList::GetSelFriend(vector<int> &vecFriendID)
{
	vecFriendID.clear();
	int nCount = GetCount();
	for(int i = 0; i < nCount; ++i)
	{
		DuiLib::CTreeNodeUI *pNode = (DuiLib::CTreeNodeUI*)GetItemAt(i);
		if(!IsGroupItem(pNode))
		{
			DuiLib::CCheckBoxUI *pCheck = static_cast<DuiLib::CCheckBoxUI*>(pNode->FindSubControl("checkSel"));
			if(pCheck->IsSelected())
			{
				int nID = pNode->GetTag() - 0x80000000;
				if(nID != g_ImUserInfo.GetId())
				{
					vecFriendID.push_back(nID);
				}
			}
		}
	}
}

// 获取所有的好友
void CDuiSelFriendList::GetAllFriend(vector<int> &vecFriendID)
{
	vecFriendID.clear();
	int nCount = GetCount();
	for(int i = 0; i < nCount; ++i)
	{
		DuiLib::CTreeNodeUI *pNode = (DuiLib::CTreeNodeUI*)GetItemAt(i);
		if(!IsGroupItem(pNode))
		{
			DuiLib::CCheckBoxUI *pCheck = static_cast<DuiLib::CCheckBoxUI*>(pNode->FindSubControl("checkSel"));

			int nID = pNode->GetTag() - 0x80000000;
			if(nID != g_ImUserInfo.GetId())
			{
				vecFriendID.push_back(nID);
			}
		}
	}
}

// 选中好友
void CDuiSelFriendList::SelFriend(int nFriendID)
{
	int nCount = GetCount();
	for(int i = 0; i < nCount; ++i)
	{
		DuiLib::CTreeNodeUI *pNode = (DuiLib::CTreeNodeUI*)GetItemAt(i);
		if(!IsGroupItem(pNode))
		{
			if(pNode->GetTag() - 0x80000000 == nFriendID)
			{	
				DuiLib::CCheckBoxUI *pCheck = static_cast<DuiLib::CCheckBoxUI*>(pNode->FindSubControl("checkSel"));
				pCheck->Selected(true);

				DuiLib::CTreeNodeUI *pParentNode = pNode->GetParentNode();
				pParentNode->GetFolderButton()->Selected(true);

				pNode->Select(true);
				
				// 确保在可视范围内
				EnsureVisible(GetItemIndex(pNode));

				break;
			}
		}
	}
}

// 勾选好友
void CDuiSelFriendList::CheckFriend(int nFriendID, BOOL bCheck)
{
	int nCount = GetCount();
	for(int i = 0; i < nCount; ++i)
	{
		DuiLib::CTreeNodeUI *pNode = (DuiLib::CTreeNodeUI*)GetItemAt(i);
		if(!IsGroupItem(pNode))
		{
			if(pNode->GetTag() - 0x80000000 == nFriendID)
			{	
				DuiLib::CCheckBoxUI *pCheck = static_cast<DuiLib::CCheckBoxUI*>(pNode->FindSubControl("checkSel"));
				pCheck->Selected(bCheck);

				DuiLib::CTreeNodeUI *pParentNode = pNode->GetParentNode();
				pParentNode->GetFolderButton()->Selected(true);

				pNode->Select(true);

				// 确保在可视范围内
				EnsureVisible(GetItemIndex(pNode));

				break;
			}
		}
	}
}

std::tr1::shared_ptr<CGroupSendAddOtherDlg> CGroupSendAddOtherDlg::pInstance = NULL;
vector<int> CGroupSendAddOtherDlg::g_SelFriendID;
std::tr1::shared_ptr<CGroupSendAddOtherDlg> CGroupSendAddOtherDlg::Show(HWND hParend, vector<int> &vecSelFriendID, CPoint pt)
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
	}
	else
	{
		pInstance = std::tr1::shared_ptr<CGroupSendAddOtherDlg>(new CGroupSendAddOtherDlg());
		pInstance->Create(GetDesktopWindow(), _T("选择好友"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE, pt.x, pt.y, CW_USEDEFAULT, CW_USEDEFAULT, NULL);
		pInstance->ShowWindow(true);
		pInstance->m_hParent = hParend;
		pInstance->CheckFriend(vecSelFriendID);
	}

	return pInstance;
}

void CGroupSendAddOtherDlg::CloseDlg()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		pInstance->Close(IDOK);
	}
}

CGroupSendAddOtherDlg::CGroupSendAddOtherDlg()
{
	m_pSearchListWnd = NULL;
}

CGroupSendAddOtherDlg::~CGroupSendAddOtherDlg()
{
}

DuiLib::UILIB_RESOURCETYPE CGroupSendAddOtherDlg::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LRESULT CGroupSendAddOtherDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(WM_TIMER == uMsg)	OnTimer(wParam);
	else if(WM_MOUSEWHEEL == uMsg)
	{	// 鼠标滚轮消息
		DuiLib::CEditUI *pEdit = (DuiLib::CEditUI*)m_PaintManager.FindControl("editView");
		if(m_PaintManager.GetFocus() == pEdit && m_pSearchListWnd != NULL && ::IsWindow(m_pSearchListWnd->GetHWND()))
		{
			m_pSearchListWnd->HandleMessage(uMsg, wParam, lParam);
		}
	}
	else if(WM_KEYDOWN == uMsg && (wParam == VK_UP || wParam == VK_DOWN))
	{
		DuiLib::CEditUI *pEdit = (DuiLib::CEditUI*)m_PaintManager.FindControl("editView");
		if(m_PaintManager.GetFocus() == pEdit && m_pSearchListWnd != NULL && ::IsWindow(m_pSearchListWnd->GetHWND()))
		{
			m_pSearchListWnd->HandleMessage(uMsg, wParam, lParam);

			// 搜索框重新获取焦点
			m_PaintManager.SetFocus(NULL);
			SetTimer(GetHWND(), TIMER_SERCHEDIT_SETFOCUS, 20, NULL);
		}
	}
	else if(WM_MSG_ITEMCLICK == uMsg)
	{	// 单元选择
		int nIdx = wParam;
		int nFriendID = m_pSearchListWnd->GetItemData(nIdx);
		m_pSelFriendList->SelFriend(nFriendID);

		m_pSearchListWnd->Close(IDOK);

		DuiLib::CEditUI *pEdit = (DuiLib::CEditUI*)m_PaintManager.FindControl("editView");
		pEdit->SetText("");
	}
	else if(WM_LBUTTONUP == uMsg)
	{
		DuiLib::CEditUI *pEdit = (DuiLib::CEditUI*)m_PaintManager.FindControl("editView");
		DuiLib::CControlUI *pFocus = m_PaintManager.GetFocus();
		if(pEdit != pFocus && m_pSearchListWnd != NULL && ::IsWindow(m_pSearchListWnd->GetHWND()))
		{
			m_pSearchListWnd->Close(IDOK);
			pEdit->SetText("");
		}
	}
	else if(WM_ACTIVATE == uMsg && WA_INACTIVE == wParam)
	{
		DuiLib::CEditUI *pEdit = (DuiLib::CEditUI*)m_PaintManager.FindControl("editView");
		if(m_pSearchListWnd != NULL && ::IsWindow(m_pSearchListWnd->GetHWND()) && lParam != (LPARAM)m_pSearchListWnd->GetHWND())
		{
			m_pSearchListWnd->Close(IDOK);
			pEdit->SetText("");
		}
	}
	else if(WM_MOVE)
	{
		//列表跟着动
		if(m_pSearchListWnd != NULL && m_pSearchListWnd->GetHWND() != NULL && IsWindowVisible(m_pSearchListWnd->GetHWND()))
		{
 				CRect rcList;
 				::GetWindowRect(m_pSearchListWnd->GetHWND(), &rcList);
				DuiLib::CEditUI *pEdit = (DuiLib::CEditUI*)m_PaintManager.FindControl("editView");
				CRect rc = pEdit->GetPos();
				CPoint pt(rc.left-24, rc.bottom+3);
				ClientToScreen(GetHWND(), &pt);
				::MoveWindow(m_pSearchListWnd->GetHWND(), pt.x, pt.y, rc.Width()+24, rcList.Height(), TRUE);
		}
	}
	return DuiLib::WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

LPCTSTR CGroupSendAddOtherDlg::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CGroupSendAddOtherDlg::InitWindow()
{
	DuiLib::WindowImplBase::InitWindow();
	SetIcon(IDR_MAINFRAME);
	UpdateBk();
	m_pSelFriendList = static_cast<CDuiSelFriendList*>(m_PaintManager.FindControl("listFriend"));

	// 根据好友列表来创建自己的列表
	CDuiFriendList *pMainFriendList = CMainFrame::GetInstance()->GetFriendList();
	int nItemCount = pMainFriendList->GetCount();
	for(int i = 0; i < nItemCount; ++i)
	{
		DuiLib::CTreeNodeUI *pItem = (DuiLib::CTreeNodeUI *)pMainFriendList->GetItemAt(i);
		BOOL bGroup = pMainFriendList->IsGroupItem(pItem);
		if(bGroup)
		{	// 添加分组
			int nGroupID = pItem->GetTag();
			int nGroupIndex = g_GroupInfo.FindItemIndex(nGroupID);
			if(nGroupID != 1 && nGroupID != 2)
				m_pSelFriendList->AddGroup_Interface(nGroupID, g_GroupInfo.group[nGroupIndex].name);
		}
		else
		{	// 添加好友
			FRIEND_INFO *pFriend = (FRIEND_INFO *)pItem->GetTag();
			if(pFriend != NULL)
			{
				if(pFriend->group_index != 1 && pFriend->group_index != 2 && pFriend->GetId() != g_ImUserInfo.GetId())
					m_pSelFriendList->AddItem_Interface(pFriend);
			}
		}
	}
}

DuiLib::CControlUI* CGroupSendAddOtherDlg::CreateControl(LPCTSTR pstrClass)
{
	if(strcmp(pstrClass, "FriendTreeView") == 0)
	{
		return new CDuiSelFriendList(m_PaintManager);
	}
	return NULL;
}

void CGroupSendAddOtherDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if(msg.sType == "click")
	{
		if(msg.pSender->GetName() == "btnAdd" )
		{	// 收集选择的好友id
			m_pSelFriendList->GetSelFriend(g_SelFriendID);
			::SendMessage(m_hParent, WM_OK, 0, 0);
			//Close(IDOK);		// 点添加按钮时不关闭窗口
		}
	}
	else if(msg.sType == DUI_MSGTYPE_TEXTCHANGED)
	{
		if(msg.pSender->GetName() == "editView")
		{
			SearchTextChange(msg.pSender->GetText().GetData());			
		}
	}
	else if(msg.sType == DUI_MSGTYPE_RETURN)
	{
		if(msg.pSender->GetName() == "editView")
		{
			::PostMessage(GetHWND(), WM_MSG_ITEMCLICK, m_pSearchListWnd->GetCurSel(), 0);
		}
	}
}

void CGroupSendAddOtherDlg::UpdateBk()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			::UpdateBk(background, pInstance->GetHWND());
		}
	}
}

void CGroupSendAddOtherDlg::CheckFriend(vector<int> &vecFriendID)
{
	int nCount = m_pSelFriendList->GetCount();
	for(int i = 0; i < nCount; ++i)
	{
		DuiLib::CTreeNodeUI *pNode = (DuiLib::CTreeNodeUI*)m_pSelFriendList->GetItemAt(i);
		if(!m_pSelFriendList->IsGroupItem(pNode))
		{
			DuiLib::CCheckBoxUI *pCheck = static_cast<DuiLib::CCheckBoxUI*>(pNode->FindSubControl("checkSel"));
			int nID = pNode->GetTag() - 0x80000000;
			if(std::find(vecFriendID.begin(), vecFriendID.end(), nID) != vecFriendID.end())
			{
				if(!pCheck->IsSelected())
				{
					pCheck->Selected(true);
				}
			}
		}
	}
}

// 通过关键字查找好友
void CGroupSendAddOtherDlg::SearchFriend(const char *pKey, vector<int> &vecMeetFriend)
{
	vecMeetFriend.clear();
	if(pKey == NULL || strcmp(pKey, "") == 0)
	{
		return;
	}

	// 在好友列表中查找包含关键字的
	vector<int> vecAllFriend;
	m_pSelFriendList->GetAllFriend(vecAllFriend);
	for(int i = 0; i < (int)vecAllFriend.size(); ++i)
	{
		FRIEND_INFO *pFriend = FindFriend(vecAllFriend[i]);
		if(pFriend == NULL)
		{
			continue;
		}

		CString strName = UserInfoToStr(pFriend);
		BOOL bMeet = CTabWndSearch::KeySearch(pKey, strName);

		if(bMeet)
		{
			vecMeetFriend.push_back(vecAllFriend[i]);
		}
	}
}

// 查找关键字改变
void CGroupSendAddOtherDlg::SearchTextChange(const char *pKey)
{
	//if(strcmp(pKey, "") == 0)
	//{
	//	if(m_pSearchListWnd != NULL && ::IsWindow(m_pSearchListWnd->GetHWND()))
	//		m_pSearchListWnd->Close(IDOK);

	//	// 搜索框重新获取焦点
	//	SetTimer(GetHWND(), TIMER_SERCHEDIT_SETFOCUS, 20, NULL);

	//	return;
	//}

	// 查找出的好友
	vector<int> vecMeetFriend;
	SearchFriend(pKey, vecMeetFriend);

	// 弹出列表
	DuiLib::CEditUI *pEdit = (DuiLib::CEditUI*)m_PaintManager.FindControl("editView");
	long nEndChar = 0;
	pEdit->GetSel(s_SearchEditStartChar, nEndChar);

	if(m_pSearchListWnd == NULL || !::IsWindow(m_pSearchListWnd->GetHWND()))
	{
		m_pSearchListWnd = std::tr1::shared_ptr<CSearchListWnd>(new CSearchListWnd(vecMeetFriend));
		m_pSearchListWnd->Create(m_PaintManager.GetPaintWindow(), NULL, WS_POPUP, WS_EX_TOOLWINDOW /*| WS_EX_TOPMOST*/, DuiLib::CDuiRect());
	}
	else
	{
		m_pSearchListWnd->Update(vecMeetFriend);
	}
	m_PaintManager.SetFocus(NULL);
	// HACK: Don't deselect the parent's caption
	HWND hWndParent = m_pSearchListWnd->GetHWND();
	while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);

	// 计算列表的位置
	CRect rcList;
	GetWindowRect(m_pSearchListWnd->GetHWND(), &rcList);
	
	CRect rc = pEdit->GetPos();
	CPoint pt(rc.left-24, rc.bottom+3);
	ClientToScreen(GetHWND(), &pt);
	::MoveWindow(m_pSearchListWnd->GetHWND(), pt.x, pt.y, rc.Width()+24, rcList.Height(), TRUE);
	m_pSearchListWnd->ShowWindow(true);
	::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);

	// 搜索框重新获取焦点
	SetTimer(GetHWND(), TIMER_SERCHEDIT_SETFOCUS, 20, NULL);
}

void CGroupSendAddOtherDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent) 
	{
	case TIMER_SERCHEDIT_SETFOCUS:
		{
			DuiLib::CEditUI *pEdit = (DuiLib::CEditUI*)m_PaintManager.FindControl("editView");
			KillTimer(GetHWND(), TIMER_SERCHEDIT_SETFOCUS);
			m_PaintManager.SetFocus(pEdit);
			pEdit->SetSel(s_SearchEditStartChar,s_SearchEditStartChar);
		}
		break;
	default:
		break;
	}
}

// 勾选好友
void CGroupSendAddOtherDlg::CheckFriend(int nFriendID, BOOL bCheck)
{
	m_pSelFriendList->CheckFriend(nFriendID, bCheck);
}