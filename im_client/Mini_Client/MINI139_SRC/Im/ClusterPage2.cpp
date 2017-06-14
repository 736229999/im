// im\ClusterPage2.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\clusterpage2.h"
#include "LocalData.h"

// CClusterPage2 对话框

IMPLEMENT_DYNAMIC(CClusterPage2, CWizardPage)

CClusterPage2::CClusterPage2(CWnd* pParent /*=NULL*/)
	: CWizardPage(CClusterPage2::IDD, pParent)
{
}

CClusterPage2::~CClusterPage2()
{
}

void CClusterPage2::DoDataExchange(CDataExchange* pDX)
{
	CWizardPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ALL, m_btnAll);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_btnClear);
}


BEGIN_MESSAGE_MAP(CClusterPage2, CWizardPage)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_ALL, OnBnClickedButtonAll)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
//	ON_NOTIFY(NM_RCLICK, IDC_TREE1, OnNMRclickTree1)
//	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)
//	ON_NOTIFY(NM_THEMECHANGED, IDC_TREE1, OnNMThemeChangedTree1)
//	ON_NOTIFY(NM_CLICK, IDC_TREE1, OnNMClickTree1)
//	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE1, OnTvnItemexpandedTree1)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CClusterPage2 消息处理程序

BOOL CClusterPage2::OnInitDialog()
{
	CWizardPage::OnInitDialog();

	GROUP_INFO info=g_GroupInfo;
	
	CRect rect;
	GetDlgItem(IDC_LIST)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	m_lstFriend.Create(this,rect);
	m_lstFriend.SetDlgCtrlID(0x05400+1);
	m_lstFriend.SetFolderListStyle(FS_CHECK|FS_IMAGE);
	m_lstFriend.SetImageList(&g_imgGroupStatus30);
	m_lstFriend.SetItemIndent(10);

	map<int,int> mapGroup;//map<group index,folder index>

	info.Sort();

	info.count-=2;//除去陌生人和黑名单

	for ( int nGroup=0; nGroup<info.count; nGroup++ )
	{
		int n = m_lstFriend.AddFolder(info.group[nGroup].name);		
		mapGroup.insert(map<int,int>::value_type(
			info.group[nGroup].group_index,n));
	}

	FRIEND_LIST::iterator itFriend;
	map<int,int>::iterator itGroup;

	for ( itFriend=g_lsFriend.begin(); itFriend!=g_lsFriend.end(); itFriend++ )
	{
		FRIEND_INFO *pFriend = (*itFriend);

		itGroup = mapGroup.find((int)pFriend->group_index);

		if ( itGroup!=mapGroup.end() )
		{
			int nFolderIndex = itGroup->second;

			bool bTail = pFriend->status==XTREAM_OFFLINE ? true : false;

			int nItemIndex   = m_lstFriend.AddItem(UserInfoToStr(pFriend),
								nFolderIndex,
								false,
								GetDefaultImage(pFriend->sex,pFriend->status),
								bTail);

			m_lstFriend.SetItemData(nItemIndex,(DWORD)pFriend);
		}
	}

	return TRUE; 
}

HBRUSH CClusterPage2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CWizardPage::OnCtlColor(pDC, pWnd, nCtlColor);

	return hbr;
}

void CClusterPage2::OnBnClickedButtonAll()
{
	int n=m_lstFriend.GetItemCount();
	for  (int i=0; i<n; i++ )
		m_lstFriend.SetItemCheck(i);

}

void CClusterPage2::OnBnClickedButtonClear()
{
	int n=m_lstFriend.GetItemCount();
	for  (int i=0; i<n; i++ )
		m_lstFriend.SetItemCheck(i,false);

}

bool CClusterPage2::GetClusterInfo(CLUSTER_INFO &info)
{
	int count = m_lstFriend.GetItemCount();
	FRIEND_INFO  * members = new FRIEND_INFO[300];
	info.members.clear();
	int n=0;
	int i;
	for ( i = 0; i<count; i++ )
	{
		if ( m_lstFriend.GetItemCheck(i) )
		{
			FRIEND_INFO *pFriend = (FRIEND_INFO *)m_lstFriend.GetItemData(i);
			members[n]=(*pFriend);
			n++;

		}
	}

	bool bRet = false;

	if(n<(MIN_CLUSTER_MEMBER-1))
	{
		// 包括自己
		CString sMsg;
		sMsg.Format("群成员至少要%d个才能创建。",MIN_CLUSTER_MEMBER);
		ShowMessage(sMsg,MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		if(n < 100)
		{
			for(i = 0;i < n;i++)
			{
				info.members.push_back(members[i]);
			}
			bRet = true;
		}
		else
		{
			ShowMessage("群成员最多不能超过100个",MB_OK|MB_ICONINFORMATION);
		}
	}

	delete [] members;
	return bRet;
}

void CClusterPage2::OnPaint()
{
	CPaintDC dc(this); 

	if (m_lstFriend.m_hWnd)
	{
		CRect rc;
		m_lstFriend.GetWindowRect(rc);
		ScreenToClient(&rc);

		rc.InflateRect(1,1);
		rc.right++;
		dc.Draw3dRect(rc,IM_LINE_COLOR,IM_LINE_COLOR);
	}
}

void CClusterPage2::OnTvnItemexpandedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 1;
}
