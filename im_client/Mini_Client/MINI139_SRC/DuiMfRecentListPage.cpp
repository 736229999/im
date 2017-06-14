#include "stdafx.h"
#include "MainFrame.h"
#include "DuiMfRecentList.h"
#ifdef _CS_PLAT_EX_
#include "CSPlatDlg.h"
#endif

extern BOOL AppendDuiMenu(DuiLib::CMenuWnd* pDuiMenu, CMenu *pMenu);

void CMainFrame::OnRecentListRButtonDown()
{
	CPoint pt;
	GetCursorPos(&pt);

	int nSel = m_pRecentList->GetCurSel();
	if ( nSel>-1 && nSel<m_pRecentList->GetCount() )
	{
		DuiLib::CListContainerElementUI *pNode = (DuiLib::CListContainerElementUI *)m_pRecentList->GetItemAt(nSel);
		FRIEND_INFO *pItem = NULL;
		CLUSTER_INFO *pCluster = NULL;
		CDuiRecentList::GetRealValue(pNode->GetTag(), pItem, pCluster);

		if ( pItem != NULL )
		{
			CMenuEx menu, *popmenu;
			menu.LoadMenu(IDR_MENU_TEMPLATE);
			popmenu = menu.GetSubMenu(0);
			popmenu->RemoveMenu(0,MF_BYPOSITION);

			CString str;

			FRIEND_INFO *pUser = (FRIEND_INFO *)pItem;

			CPoint pt;
			GetCursorPos(&pt);

			if ( pUser->GetId()!=g_ImUserInfo.GetId() )
			{
				str.Format("与 \"%s\" 单独聊天",(pUser->nickname));
				popmenu->AppendMenu(MF_STRING,ID_SEND_P2PMSG,str);	

				str.Format("查看 \"%s\" 的详细资料",(pUser->nickname));
				popmenu->AppendMenu(MF_STRING,ID_FRIEND_INFO,str);	

				if ( FindFriend(pUser->GetId()) )
				{
					str.Format("和 \"%s\" 的聊天记录",(pUser->nickname));
					popmenu->AppendMenu(MF_STRING,ID_FRIEND_MSG,str);	
				}
				if(pUser->group_index == 1)
				{	
#ifndef _CS_PLAT_EX_
					// 陌生人
					str.Format("我要加 \"%s\" 为好友",(pUser->nickname));
					popmenu->AppendMenu(MF_STRING,ID_FRIEND_ADD,str);	
#endif
				}
			}

			DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
			DuiLib::CPoint duiPoint(pt.x,pt.y);
			pMenu->Init(NULL, _T("loginstatus.xml"), pt, &m_PaintManager);
			AppendDuiMenu(pMenu, popmenu);
		}
		if(pCluster != NULL)
		{
			CMenuEx menu, *popmenu;
			menu.LoadMenu(IDR_MENU_TEMPLATE);
			popmenu = menu.GetSubMenu(0);
			popmenu->RemoveMenu(0,MF_BYPOSITION);

			CString str;
			str.Format("进入群 \"%s\" 的聊天室",pCluster->szFamilyName);
			popmenu->AppendMenu(MF_STRING,ID_CLUSTER_TALK,str);	

			str.Format("查看群 \"%s\" 资料",pCluster->szFamilyName);
			popmenu->AppendMenu(MF_STRING,ID_CLUSTER_INFO,str);	

			popmenu->AppendMenu(MF_STRING,ID_CLUSTER_MSG,"历史聊天记录");	

			DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
			DuiLib::CPoint duiPoint(pt.x,pt.y);
			pMenu->Init(NULL, _T("loginstatus.xml"), pt, &m_PaintManager);
			AppendDuiMenu(pMenu, popmenu);
		}
	}
}

void CMainFrame::RecentList_UserInfoGet(const char *data,WORD wRet)
{
	m_pRecentList->OnUserInfoGet(data, wRet);
	RECENTLIST_OP(OnUserInfoGet(data, wRet))
}

void CMainFrame::RecentList_UserStatusInd(const char *data,WORD wRet)
{
	m_pRecentList->OnUserStatusInd(data, wRet);
	RECENTLIST_OP(OnUserStatusInd(data, wRet))
}

void CMainFrame::RecentList_StatusGetAck(const char *data,WORD wRet)
{
	m_pRecentList->OnStatusGetAck(data, wRet);
	RECENTLIST_OP(OnStatusGetAck(data, wRet))
}