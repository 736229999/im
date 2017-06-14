#include "stdafx.h"
#include "SearchListWnd.h"
#include "im\impublic.h"
#include "im\WebProcess.h"

using namespace std;

extern CWebUserHeadIconProcess g_webUserHeadIconProcess;
extern string CutRoundImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE);


CSearchListWnd::CSearchListWnd(const vector<int> &vecFriendID)
{
	m_vecFriendID = vecFriendID;
}

void CSearchListWnd::InitWindow()
{
	DuiLib::WindowImplBase::InitWindow();

	Update(m_vecFriendID);
}

void CSearchListWnd::Notify(DuiLib::TNotifyUI& msg)
{
	if(msg.sType == DUI_MSGTYPE_ITEMCLICK)
	{
		::PostMessage(GetParent(GetHWND()), WM_MSG_ITEMCLICK, GetCurSel(), 0);
	}
	DuiLib::WindowImplBase::Notify(msg);
}

// 刷新列表
void CSearchListWnd::Update(const std::vector<int> &vecFriendID)
{
	m_vecFriendID = vecFriendID;

	m_pList = (DuiLib::CListUI*)m_PaintManager.FindControl("lsSearchResult");
	m_pTab = (DuiLib::CTabLayoutUI*)m_PaintManager.FindControl("tabMain");
	m_pList->RemoveAll();
	if(m_vecFriendID.size() == 0)
	{
		m_pTab->SelectItem(1);
		
		CRect rc;
		GetWindowRect(GetHWND(), &rc);

		rc.bottom = rc.top+32;
		MoveWindow(GetHWND(), rc.left, rc.top, rc.Width(), rc.Height(), TRUE);
	}
	else
	{
		m_pTab->SelectItem(0);

		for(int i = 0; i < (int)vecFriendID.size(); ++i)
		{
			FRIEND_INFO *pFriend = FindFriend(vecFriendID[i]);
			CString strName = UserInfoToStr(pFriend);
			DuiLib::CListLabelElementUI *pItem = new DuiLib::CListLabelElementUI();
			pItem->SetTag(vecFriendID[i]);
			pItem->SetText(strName);
			pItem->SetFixedHeight(32);
			CString strFile = g_webUserHeadIconProcess.GetFaceFile(pFriend);
			if ( FileExist(strFile) )
			{
				string strHeadIcon = CutRoundImg(strFile, 28, 28);
				CString strBk;
				strBk.Format("file='%s' dest='3,2,31,30'", strHeadIcon.c_str());
				pItem->SetBkImage(strBk);
			}
			m_pList->Add(pItem);
		}

		CRect rc;
		GetWindowRect(GetHWND(), &rc);

		rc.bottom = rc.top+(vecFriendID.size() > 5 ? 5 : vecFriendID.size())*32;
		MoveWindow(GetHWND(), rc.left, rc.top, rc.Width(), rc.Height(), TRUE);

		m_pList->SelectItem(0);
	}

	m_PaintManager.SetFocus(m_pList);
}

// 获取当前选择
int CSearchListWnd::GetCurSel()
{
	if(m_pTab == NULL || m_pTab->GetCurSel() != 0)
	{
		return -1;
	}
	else
	{
		return m_pList->GetCurSel();
	}
}

// 获取item关联的数据
DWORD CSearchListWnd::GetItemData(int nIndex)
{
	DuiLib::CControlUI *pItem = m_pList->GetItemAt(nIndex);
	if(pItem != NULL)
	{
		return pItem->GetTag();
	}

	return 0;
}

LRESULT CSearchListWnd::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	return DuiLib::WindowImplBase::HandleMessage(uMsg, wParam, lParam); 
}