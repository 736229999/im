// im\P2PUserList.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "P2PUserList.h"
#include ".\p2puserlist.h"


// CP2PUserList 对话框

IMPLEMENT_DYNAMIC(CP2PUserList, CDialog)
IMPLEMENT_HMG_SCROLLBAR(CP2PUserList)

CP2PUserList::CP2PUserList(CWnd* pParent /*=NULL*/)
	: CDialog(CP2PUserList::IDD, pParent)
{
	m_pClusterInfo = NULL;
}

CP2PUserList::~CP2PUserList()
{
	if(::IsWindow(GetSafeHwnd()))
	{
		DestroyWindow();
	}
}

void CP2PUserList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLUSTER, m_wndList);
}


BEGIN_MESSAGE_MAP(CP2PUserList, CDialog)
	ON_WM_SIZE()
	SET_HMG_SCROLLBAR(IDC_LIST_CLUSTER)
	ON_LBN_SELCHANGE(IDC_LIST_CLUSTER, OnLbnSelchangeListCluster)
	ON_LBN_DBLCLK(IDC_LIST_CLUSTER, OnLbnDblclkListCluster)
END_MESSAGE_MAP()


void CP2PUserList::UpdateClusterList()
{

	if(m_pClusterInfo == NULL)
		return;
	
	if ( m_wndList.m_hWnd==NULL )
	{
		return ;
	}

	m_wndList.ResetContent();

	int nItem = -1;
	size_t i;
	size_t c;
	c = m_pClusterInfo->members.size();
	for (  i = 0; i < c; i++ )
	{
		if ( m_pClusterInfo->members[i].GetId()==g_ImUserInfo.GetId() )
		{
			m_pClusterInfo->members[i].cluster_property=2;
		}
		else
		{
			m_pClusterInfo->members[i].cluster_property=0;
		}
	}
	m_wndList.SetClusterInfo(m_pClusterInfo);

	try
	{
		for ( i = 0;i < c; i++)
		{
			if (m_pClusterInfo->members[i].GetId()==g_ImUserInfo.GetId())
			{
				nItem=i;
			}
		}
	}
	catch(...)
	{
		return ;
	}

	if ( nItem!=-1 && c > 1 )
	{
		FRIEND_INFO temp=m_pClusterInfo->members[0];
		m_pClusterInfo->members[0]=m_pClusterInfo->members[nItem];
		m_pClusterInfo->members[nItem]=temp;
	}
	
	for ( i=0; i < c; i++ )
	{
		FRIEND_INFO *pFriend = &m_pClusterInfo->members[i];
		nItem = m_wndList.AddString(UserInfoToStr(pFriend));
		m_wndList.SetItemData(nItem,(DWORD)pFriend);
	}

	
}

void CP2PUserList::SetClusterInfo(CLUSTER_INFO * pCluster)
{
	m_pClusterInfo = pCluster;
}

// CP2PUserList 消息处理程序

void CP2PUserList::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if ( m_wndList.m_hWnd )
	{
		m_wndList.MoveWindow(0,0,cx,cy);
	}
}

BOOL CP2PUserList::OnInitDialog()
{
	CDialog::OnInitDialog();

	AttachHMGScrollbar(m_wndList.m_hWnd);
	
	return TRUE; 
}

void CP2PUserList::OnLbnSelchangeListCluster()
{
}

void CP2PUserList::OnLbnDblclkListCluster()
{
	int nSel = m_wndList.GetCurSel();

	if ( nSel>-1 && nSel<m_wndList.GetCount() )
	{
		FRIEND_INFO *pInfo =(FRIEND_INFO *)m_wndList.GetItemData(nSel);

		if ( pInfo->GetId()==g_ImUserInfo.GetId() )
		{
			ShowBlogSite((char*)pInfo->GetUserName().c_str());
		}
		else
		{
			GetParent()->SendMessage(WM_THREAD_NOTIFY,0,(LPARAM)pInfo);
		}
	}
}

BOOL CP2PUserList::PreTranslateMessage(MSG* pMsg)
{
	if  (  pMsg->message  ==  WM_KEYDOWN  )  
	{  
		switch(  pMsg->wParam  )  
		{  
		case 0x0D:
		case VK_ESCAPE:
			GetParent()->PostMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
			return TRUE;
		default:
			;
		}  
	}

	return CDialog::PreTranslateMessage(pMsg);
}
