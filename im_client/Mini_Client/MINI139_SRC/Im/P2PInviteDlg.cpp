// im\P2PInviteDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\p2pinvitedlg.h"
#include "LocalData.h"
#include "Impublic.h"
// CClusterMemberSelectDlg ???

//#define MAX_COMM_CLUSTER_COUNT	100
//#define MAX_VIP_CLUSTER_COUNT	300
HWND g_hWndListAdd = NULL;
bool compareF(FRIEND_INFO *p1, FRIEND_INFO *p2)
{
	if ( strcmp(p1->nickname,p2->nickname)>0 )
		return true;
	else
		return false;
}

IMPLEMENT_DYNAMIC(CClusterMemberSelectDlg, CDialog)
//IMPLEMENT_IM_SB(CClusterMemberSelectDlg)

CClusterMemberSelectDlg::CClusterMemberSelectDlg(CLUSTER_INFO *pInfo, CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CClusterMemberSelectDlg::IDD, pParent)
{
	//////添加颜色设置///////////////
	m_colorBackground = CHAT_BKG_COLOR;
	m_colorBorder = CHAT_BKG_COLOR;
	DeleteObject(m_hBkGround);
	m_hBkGround = ::CreateSolidBrush(m_colorBackground);

	m_nItemIndex   = 0;
	m_clusterInfo.nFamilyProp = pInfo->nFamilyProp;
	m_clusterInfo.nFamilyType = pInfo->nFamilyType;
	m_clusterInfo.nFamilyID   = pInfo->nFamilyID;
	m_clusterInfo.nJoinFlag   = pInfo->nJoinFlag;
	m_clusterInfo.nViewFlag   = pInfo->nViewFlag;
	strcpy(m_clusterInfo.szFamilyDesc,pInfo->szFamilyDesc);
	strcpy(m_clusterInfo.szFamilyName,pInfo->szFamilyName);
	strcpy(m_clusterInfo.szFamilyDomain,pInfo->szFamilyDomain);
	
	int n=pInfo->members.size();
	for ( int i=0; i<n; i++ )
		m_clusterInfo.members.push_back(pInfo->members[i]);

	m_pP2pInviteFriend     = NULL;
	m_nClusterId = 0;
	m_nPos= -1;
//	m_nMemberNumLimit = MAX_COMM_CLUSTER_COUNT;
}

CClusterMemberSelectDlg::~CClusterMemberSelectDlg()
{

}

void CClusterMemberSelectDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	//DDX_Control(pDX, IDC_BUTTON_ALL,m_btnAll);
	//DDX_Control(pDX, IDC_BUTTON_NONE, m_btnNone);
	DDX_Control(pDX, IDC_BTN_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_BTN_DEL, m_btnDel);
	DDX_Control(pDX, IDC_LIST_ADD, m_ctlListAdd);
	//DDX_Control(pDX, IDC_BTN_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnSearch);
	DDX_Control(pDX, IDC_BTN_ALL, m_btnResume);
	DDX_Control(pDX, IDC_EDIT_CONTENT, m_edtSearch);
	DDX_Control(pDX, IDC_LIST_FRIEND, m_frame);
	DDX_Control(pDX, IDC_BTN_RIGHT, m_btnRight);
	DDX_Control(pDX, IDC_STATIC_TEXT, m_btnNote);
}


BEGIN_MESSAGE_MAP(CClusterMemberSelectDlg, CXSkinDlg)
	ON_WM_TIMER()
	ON_XT_MESSAGE()
	ON_WM_CLOSE()
	//ON_BN_CLICKED(IDC_BUTTON_ALL, OnBnClickedButtonAll)
	//ON_BN_CLICKED(IDC_BUTTON_NONE, OnBnClickedButtonNone)
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDOK, &CClusterMemberSelectDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CClusterMemberSelectDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_ADD, &CClusterMemberSelectDlg::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_DEL, &CClusterMemberSelectDlg::OnBnClickedBtnDel)
	ON_LBN_SELCHANGE(IDC_LIST_ADD, &CClusterMemberSelectDlg::OnLbnSelchangeListAdd)
	//ON_BN_CLICKED(IDC_BTN_SEARCH, &CClusterMemberSelectDlg::OnBnClickedBtnSearch)
	//ON_BN_CLICKED(IDC_BTN_RESUME, &CClusterMemberSelectDlg::OnBnClickedBtnResume)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CClusterMemberSelectDlg::OnBnClickedBtnSearch)
	ON_BN_CLICKED(IDC_BTN_ALL, &CClusterMemberSelectDlg::OnBnClickedBtnAll)
	ON_EN_SETFOCUS(IDC_EDIT_CONTENT, &CClusterMemberSelectDlg::OnEnSetfocusEditContent)
	ON_EN_KILLFOCUS(IDC_EDIT_CONTENT, &CClusterMemberSelectDlg::OnEnKillfocusEditContent)
	ON_STN_CLICKED(IDC_STATIC_INFO, &CClusterMemberSelectDlg::OnStnClickedStaticInfo)
	ON_BN_CLICKED(IDC_BTN_RIGHT, &CClusterMemberSelectDlg::OnBnClickedBtnRight)
	ON_MESSAGE(WM_ADD_FRIEND_TO_LIST, &CClusterMemberSelectDlg::OnAddFriendToCluster)
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CClusterMemberSelectDlg)
	ON_XT_COMMAND(CMD_CREATE_CLUSTER_ACK,OnCreateClusterAck)
	ON_XT_COMMAND(CMD_SET_CMEMBERS_ACK,OnCMembersSetAck)
	ON_XT_COMMAND(CMD_USERFAMILYFLAG_GET_ACK,OnUserFamilyFlagAck)
END_XT_MESSAGE_MAP()


// CClusterMemberSelectDlg 消息处理程序
BOOL CClusterMemberSelectDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();
	g_hWndListAdd = GetSafeHwnd();
	SetDlgStyle(CLOSE);
	EnableHelp(true);

	InitPos();
	m_edtSearch.SetFont(&g_font16);
    m_edtSearch.SetBackColor(RGB(0XFF,0XFF,0XFF));
	m_edtSearch.SetBorderColor(RGB(0X60,0XAD,0XDD));

	
	//m_btnSearch.SetBitmap(IDB_BITMAP_SEARCH_MEMBER,IDB_BITMAP_SEARCH_MEMBER);
    m_btnSearch.SetBitmap(IDB_BTN_SEARCH, IDB_BTN_SEARCH, RGB(0, 255, 0));
	m_btnResume.SetBitmap(IDB_BITMAP_CLEAR_CONTENTS,IDB_BITMAP_CLEAR_CONTENTS);
	m_edtSearch.SetWindowText("搜索好友");
	//m_edtSearch.SetBackColor(RGB(0X98,0X98,0X98));
	m_edtSearch.SetBorderColor(RGB(0X60,0XAD,0XDD));
	m_edtSearch.SetFont(&g_font12);

	m_btnNote.SetTextColor(RGB(0,0,255));
	if ( m_pP2pInviteFriend )
	{
		SetWindowText("邀请");
	}
	else
	{
		SetWindowText("群成员设置");
	}

	CRect rect;
	GetDlgItem(IDC_LIST_FRIEND)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	SetTimer(0x02,1000,NULL);
	//m_btnAddTo.SetBitmap(IDB_BITMAP_LIST_HOVER,IDB_BITMAP_LIST_NORMAL,IDB_BITMAP_LIST_PRESSED,IDB_BITMAP_LIST_PRESSED);
	/*m_btnAdd.SetTextColor(RGB(0,0,0));
	m_btnDel.SetTextColor(RGB(0,0,0));*/

	m_wndListFriend.Create(this,rect);
	m_wndListFriend.SetDlgCtrlID(0x05400+1);
	m_wndListFriend.SetFolderListStyle(/*FS_CHECK|*/FS_IMAGE);
	m_wndListFriend.SetImageList(&g_imgStatus30);
	m_wndListFriend.SetItemIndent(10);

	//m_wndListFriend.Create(this,rect);
	/*m_ctlListAdd.SetDlgCtrlID(0x05401+1);*/
	//m_ctlListAdd.SetFolderListStyle(/*FS_CHECK|*/FS_IMAGE);
	//m_ctlListAdd.SetImageList(&g_imgStatus30);
	//m_ctlListAdd.SetItemIndent(10);

	int nGroupCount = g_GroupInfo.count;
	int nGroup;
	int nIdx;
	for ( nGroup = 0; nGroup< nGroupCount; nGroup++ )
	{
		nIdx = g_GroupInfo.group[nGroup].group_index;
		if(nIdx == 1 || nIdx == 2)
			continue;
		m_wndListFriend.AddFolder(g_GroupInfo.group[nGroup].name);
		m_mapGroup[nIdx] = nGroup;//g_GroupInfo.group[nGroup].name;
	}

	FRIEND_LIST::iterator itFriend;
	

	//int nItem =0;
	int nFolder = 0;
	bool bTail;
	int nItemIndex;

	g_lsFriend.sort(compareF);

	for ( itFriend=g_lsFriend.begin(); itFriend!=g_lsFriend.end(); itFriend++ )
	{
		FRIEND_INFO *pFriend = (*itFriend);
		// 过滤黑名单和陌生人
		if(pFriend->group_index == 1 || pFriend->group_index == 2)
		{
			continue;
		}

		// 过滤最初聊天的好友
		if ( m_pP2pInviteFriend && m_pP2pInviteFriend->GetId()==pFriend->GetId() )
		{
			continue;
		}
		// 过滤已经被邀请的好友
		if ( m_clusterInfo.GetFriendInfo(pFriend->GetId()) )
		{
			continue;
		}
		// 过滤陌生人和黑名单中的好友
		//itGroup = mapGroup.find((int)pFriend->group_index);
		
		// 邀请好友群聊天，显示在线好友
		if ( m_pP2pInviteFriend )
		{
			// 只邀请在线好友
			if ( pFriend->status!=XTREAM_OFFLINE )				
			{
				/*CString strItem;
				strItem.Format("%s",pFriend->nickname);
				m_wndListFriend.InsertItem(nItem,strItem,GetDefaultImage(pFriend->sex,pFriend->status));
				m_wndListFriend.SetItemText(nItem,1,mapGroup[pFriend->group_index]);
				m_wndListFriend.SetItemData(nItem,(DWORD)pFriend);
				nItem++;*/
				nFolder = m_mapGroup[pFriend->group_index];
				bTail = pFriend->status==XTREAM_OFFLINE ? true : false;
				nItemIndex = m_wndListFriend.AddItem(
					UserInfoToStr(pFriend),
					nFolder,
					false,///false
					GetDefaultImage(pFriend->sex,pFriend->status),
					bTail);
				m_wndListFriend.SetItemData(nItemIndex,(DWORD)pFriend);
			}
		}
		// 设置群成员，显示所有好友
		else
		{
			/*CString strItem;
			strItem.Format("%s",pFriend->nickname);
			m_wndListFriend.InsertItem(nItem,strItem,GetDefaultImage(pFriend->sex,pFriend->status));
			m_wndListFriend.SetItemText(nItem,1,mapGroup[pFriend->group_index]);
			m_wndListFriend.SetItemData(nItem,(DWORD)pFriend);
			nItem++;*/
			nFolder = m_mapGroup[pFriend->group_index];
			bTail = pFriend->status==XTREAM_OFFLINE ? true : false;
			nItemIndex   = m_wndListFriend.AddItem(
				UserInfoToStr(pFriend),
				nFolder,
				false,///false
				GetDefaultImage(pFriend->sex,pFriend->status),
				bTail);
			m_wndListFriend.SetItemData(nItemIndex,(DWORD)pFriend);
		}
	}

	g_pProtocol->RegistCmd(CMD_CREATE_CLUSTER_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SET_CMEMBERS_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_USERFAMILYFLAG_GET_ACK,m_hWnd);

	return TRUE;  
}

void CClusterMemberSelectDlg::InitPos()
{
	CRect  rect;
	GetClientRect(&rect);
	long x = rect.left+10;
	long y = rect.top+30;
	//m_edtSearch.MoveWindow(x,y,220,25);
	//m_btnSearch.MoveWindow(x+200,y,20,25);
	//m_frame.MoveWindow(x,y+35,220,370);
	
	//m_ctlListAdd.MoveWindow(x+220+40,y+35,220,370);

	//m_btnRight.MoveWindow(x+220+10,y+190,10,10);
	m_btnRight.SetBitmap(IDB_BMP_ARROW_RIGHT,IDB_BMP_ARROW_RIGHT);

}
void CClusterMemberSelectDlg::OnHelpClick()
{
	if ( this->m_pP2pInviteFriend )
		OpenHtmlPage("www.feiniu.com");
	else
		OpenHtmlPage("www.feiniu.com");

}

void CClusterMemberSelectDlg::OnOK()
{
	// 获得用户选择的成员
	m_vecFriends.clear();

	for ( int nItem=0; nItem<m_wndListFriend.GetItemCount(); nItem++ )
	{
		if ( m_wndListFriend.GetItemCheck(nItem) )
		{
			FRIEND_INFO *pFriend =(FRIEND_INFO *)m_wndListFriend.GetItemData(nItem);	
			m_vecFriends.push_back(pFriend);
		}
	}
	
	int nMemberCount = m_vecFriends.size();
	
	CString sPrompt;

	if( m_clusterInfo.nFamilyProp == PERM_FAMILY)
	{
		if(m_clusterInfo.nFamilyType == FAMILY_VIP)
		{
		//	nLimit = MAX_VIP_CLUSTER_COUNT - m_clusterInfo.Count() ;
		}
		else
		{
		//	nLimit = MAX_COMM_CLUSTER_COUNT - m_clusterInfo.Count(); 
		}
	}
	else
	{
	//	nLimit = MAX_TEMPFAMILYMEM_NUM - m_clusterInfo.Count(); 
	}


	//if ( nMemberCount > nLimit)
	//{
	//	sPrompt="您选择的人数过多，临时群最多10个成员！";
	//	ShowMessage(sPrompt,MB_OK|MB_ICONINFORMATION);
	//}
	//else
	if ( nMemberCount>0 )
	{
		if ( m_pP2pInviteFriend )
		{
			if(m_pP2pInviteFriend->GetId() != 0)
				m_vecFriends.push_back(m_pP2pInviteFriend);
		}
		else
		{
		}

		if ( m_clusterInfo.nFamilyID==(uint32)-1 )
		{
			XT_CREATE_CLUSTER_REQ req;
			req.nFamilyProp  = TEMP_FAMILY;
			g_pProtocol->SendReq(req);
		}
		else
		{
			SetClusterMember(m_clusterInfo.nFamilyID);
		}

		GetDlgItem(IDOK)->EnableWindow(FALSE);
		GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		SetTimer(0x01,350,NULL);
	}
	else
	{
		CDialog::OnCancel();
	}
}

void CClusterMemberSelectDlg::OnTimer(UINT nIDEvent)
{
    if ( nIDEvent==0x01 )
	{
		static int count=0;
		static CString strDot="";

		if ( count++>5 )
		{
			strDot="";
			count=0;
		}
		else
		{
			strDot+=".";
		}

		SetDlgItemText(IDC_STATIC_INFO,strDot);
	}
	else if ( nIDEvent==0x02 )
	{
		CString strTxt = _T("");
		m_edtSearch.GetWindowText(strTxt);
		if(strTxt == _T(""))
		{
			//m_edtSearch.SetWindowText(_T("搜索好友"));
			//m_edtSearch.SetSel(0,-1);
		}
	}
	CXSkinDlg::OnTimer(nIDEvent);
}

void CClusterMemberSelectDlg::OnCMembersSetAck(const char *data, WORD wRet)
{
	LPXT_SET_CMEMBERS_ACK ack = (LPXT_SET_CMEMBERS_ACK)data;

	GetDlgItem(IDOK)->EnableWindow(TRUE);
	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

	if (wRet==0 && ack->nFamilyID==m_clusterInfo.nFamilyID )
	{
		KillTimer(0x01);
		if ( ack->nAction==CMEMBER_ADD )
		{
			RegUserOperation(0x00B1);

			for ( int i=0; i<m_vecFriends.size(); i++ )
			{	
				uint32 fid = m_vecFriends[i]->GetId();

				if ( m_clusterInfo.GetFriendInfo(fid)==NULL )
				{
					m_clusterInfo.members.push_back(*m_vecFriends[i]);	
					m_clusterInfo.members[m_clusterInfo.Count()-1].cluster_property=CMEMBER_NORMAL;
				}
			}

			if ( m_clusterInfo.GetFriendInfo(g_ImUserInfo.GetId())==NULL )
			{
				m_clusterInfo.members.push_back(g_ImUserInfo);
				m_clusterInfo.members[m_clusterInfo.Count()-1].cluster_property=CMEMBER_CREATOR;//族长
			}

			CDialog::OnOK();
		}		
	}
	else if(wRet != 0 && ack->nFamilyID==m_clusterInfo.nFamilyID)
	{
		KillTimer(0x01);
		SetDlgItemText(IDC_STATIC_INFO,g_pProtocol->GetLastError());
	}

}

void CClusterMemberSelectDlg::OnCreateClusterAck(const char *data, WORD wRet)
{
	LPXT_CREATE_CLUSTER_ACK ack = (LPXT_CREATE_CLUSTER_ACK)data;

	if ( wRet==0 )
	{
		m_clusterInfo.nFamilyID = ack->nFamilyID;
		SetClusterMember(ack->nFamilyID);
	}
	else
	if ( wRet==SERVER_TIMEOUT )
	{
		KillTimer(0x01);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
		SetDlgItemText(IDC_STATIC_INFO,"");
	}
	else
	{
		KillTimer(0x01);
		GetDlgItem(IDOK)->EnableWindow(TRUE);
		GetDlgItem(IDCANCEL)->EnableWindow(TRUE);

		SetDlgItemText(IDC_STATIC_INFO,g_pProtocol->GetLastError());
		//MessageBox(ack->error);
		CMessageBox::Prompt(g_pProtocol->GetLastError());
	}
}

void CClusterMemberSelectDlg::OnClose()
{
	g_pProtocol->UnRegistCmd(m_hWnd);
	CXSkinDlg::OnClose();
}


void CClusterMemberSelectDlg::OnUserFamilyFlagAck(const char *data, WORD wRet)
{
	XT_USERFAMILYFLAG_GET_ACK *pAck = (XT_USERFAMILYFLAG_GET_ACK*)data;
	if(wRet == 0)
	{
		char buff[MAX_SEND_BUFFERS];
		int nC = pAck->count;
		int nAccept = 0;
		// 
		int nRefuse = 0;
		uint32 id;
		
		XT_SET_CMEMBERS_REQ  & req = *((LPXT_SET_CMEMBERS_REQ)buff);
		int i;
		req.nFamilyID   = m_nClusterId;
		req.nAction     = CMEMBER_ADD;
		req.nFamilyProp = m_clusterInfo.nFamilyProp;
		
		for ( i = 0; i< nC; i++ )
		{
			if(pAck->JoinFlag[i].flag == 0)
			{
				req.nMemID[nAccept] = pAck->JoinFlag[i].uID;
				nAccept++;
			}
			else
			{
				id = pAck->JoinFlag[i].uID;
				nRefuse++;
			}
		}
		// 判断是否有人拒绝加入群
		if(nRefuse == 1)
		{
			CString sMsg;
			FRIEND_INFO * pUser = ::GetFriendInfo(id);
			if(pUser)
			{
				sMsg.Format("用户\"%s\"拒绝加入群",pUser->nickname);
			}
			else
			{
				sMsg.Format("用户%d拒绝加入群",id);
			}
			ShowMessage(sMsg);
		}
		else if(nRefuse > 1)
		{
			CString sMsg;
			sMsg.Format("有%d个用户拒绝加入群",nRefuse);
			ShowMessage(sMsg);
		}
		if(nAccept > 0)
		{
			req.nCount = nAccept;
			g_pProtocol->SendReq(req);
		}
		else
		{
			// 退出
			CDialog::OnOK();
		}
	}
}

void CClusterMemberSelectDlg::SetClusterMember(uint32 clusterId)
{
	m_nClusterId = clusterId;
	if(m_pP2pInviteFriend == NULL)
	{
		// 在设置固定群成员前，先查询这些成员是否愿意加入群
// 		XT_USERFAMILYFLAG_GET_REQ & req = *((LPXT_USERFAMILYFLAG_GET_REQ)buff);
// 		req.count = m_vecFriends.size();
// 		int i = 0;
// 		for ( i = 0; i<req.count; i++ )
// 		{
// 			req.userID[i]=m_vecFriends[i]->id;
// 		}
// 		g_pProtocol->QueryFriendFamilyFlag(req);
		char buff[MAX_SEND_BUFFERS];
		XT_CLUSTER_INVITE_REQ & req = *((LPXT_CLUSTER_INVITE_REQ)buff);
		strcpy(req.clustername, m_clusterInfo.szFamilyName);
		//strcpy(req.nickname, m_clusterInfo.szFamilyName);//nickname长度只有30，群成名设置可以超过30，所以暂时不赋值
		req.count = m_vecFriends.size();
		req.cluster_id = clusterId;
		req.cluster_type = m_clusterInfo.nFamilyProp;
		int i = 0;
		for ( i = 0; i<req.count; i++ )
		{
			req.userID[i]=m_vecFriends[i]->GetId();

			CString str; 
			str.Format("您邀请【%s】加入群【%s】", FindFriend(m_vecFriends[i]->GetId())->nickname, m_clusterInfo.szFamilyName );

			//写群系统消息
			//SYSTEM_MSG msgSys;
			//msgSys.dwTime = time(NULL);
			//msgSys.iType  = SYSTEM_MSG::SYSMSG_CLUSTER;
			//sprintf(msgSys.szMessage,str);
			//g_LocalData.AddSystemMsg(g_ImUserInfo.GetId(),msgSys);

			CLUSTER_SYSTEM_MSG msgSys;
			msgSys.dwTime = time(NULL);
			sprintf(msgSys.szMessage,str);
			msgSys.nInviteId = 0;
			msgSys.nClusterId = clusterId;
			sprintf(msgSys.szClusterName, m_clusterInfo.szFamilyName);
			g_LocalData.AddClusterSystemMsg(g_ImUserInfo.GetId(),msgSys);
		}
		g_pProtocol->ClusterInvite(req);
	}
	else
	{
		char buff[MAX_SEND_BUFFERS];
		XT_SET_CMEMBERS_REQ  & req = *((LPXT_SET_CMEMBERS_REQ)buff);

		req.nFamilyID   = clusterId;
		req.nAction     = CMEMBER_ADD;//
		req.nFamilyProp = m_clusterInfo.nFamilyProp;
		req.nCount      = m_vecFriends.size();

		for ( int i=0; i<req.nCount; i++ )
		{
			req.nMemID[i]=m_vecFriends[i]->GetId();
		}

		g_pProtocol->SendReq(req);
	}
	
}

void CClusterMemberSelectDlg::OnBnClickedButtonAll()
{

	int iCount=0;
	for ( int nItem=0; nItem<m_wndListFriend.GetItemCount(); nItem++ )
	{
		//m_wndListFriend.SetCheck(nItem,FALSE);

		//if ( iCount<10 )
		//{
		//	iCount++;
			m_wndListFriend.SetItemCheck(nItem);
		//}
	}

	if ( m_wndListFriend.GetItemCount()>10 )
	{
		//ShowMessage("一次最多选择10个成员！",MB_OK|MB_ICONINFORMATION);
	}
}

void CClusterMemberSelectDlg::OnBnClickedButtonNone()
{
	for ( int nItem=0; nItem<m_wndListFriend.GetItemCount(); nItem++ )
	{
		m_wndListFriend.SetItemCheck(nItem,0);
	}
}

HBRUSH CClusterMemberSelectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = g_hBackBrush;
	}

	return hbr;
}

void CClusterMemberSelectDlg::OnPaint()
{
	
	CPaintDC dc(this); 
	if(m_wndListFriend.m_hWnd)
	{
		CRect rc;
		m_wndListFriend.GetWindowRect(rc);
		ScreenToClient(&rc);
		rc.InflateRect(1,1);
		rc.right++;
		dc.Draw3dRect(rc,IM_LINE_COLOR,IM_LINE_COLOR);
	}
	CXSkinDlg::OnPaint();
}
	
void CClusterMemberSelectDlg::GetClusterInfo(CLUSTER_INFO & l)
{
	l.members.clear();
	l.nFamilyID = m_clusterInfo.nFamilyID;

	int n= m_clusterInfo.members.size();
	for ( int i=0; i<n; i++ )
		l.members.push_back(m_clusterInfo.members[i]);
}

void CClusterMemberSelectDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	m_vecFriends.clear();
	for ( int nItem=0; nItem<m_ctlListAdd.GetCount(); nItem++ )////选择了哪些好友
	{
		{
			FRIEND_INFO *pFriend =(FRIEND_INFO *)m_ctlListAdd.GetItemData(nItem);	
			m_vecFriends.push_back(pFriend);
		}
	}
	int nMemberCount = m_vecFriends.size();
	CString sPrompt;
	//m_vecFriends.swap(m_vecAddFriends);
	if( m_clusterInfo.nFamilyProp == PERM_FAMILY)
	{
		if(m_clusterInfo.nFamilyType == FAMILY_VIP)
		{
			//	nLimit = MAX_VIP_CLUSTER_COUNT - m_clusterInfo.Count() ;
		}
		else
		{
			//	nLimit = MAX_COMM_CLUSTER_COUNT - m_clusterInfo.Count(); 
		}
	}
	else
	{
		//	nLimit = MAX_TEMPFAMILYMEM_NUM - m_clusterInfo.Count(); 
	}


	//if ( nMemberCount > nLimit)
	//{
	//	sPrompt="您选择的人数过多，临时群最多10个成员！";
	//	ShowMessage(sPrompt,MB_OK|MB_ICONINFORMATION);
	//}
	//else
	if ( nMemberCount>0 )
	{
		//如果为组内群发则直接设置入群
		if(m_clusterInfo.nFamilyProp != TEMP_FAMILY)
		{
			if ( m_pP2pInviteFriend )
			{
				if(m_pP2pInviteFriend->GetId() != 0)
					m_vecFriends.push_back(m_pP2pInviteFriend);
				////m_vecAddFriends.push_back(m_pP2pInviteFriend);
			}
			else
			{
			}

			if ( m_clusterInfo.nFamilyID==(uint32)-1 )
			{
				XT_CREATE_CLUSTER_REQ req;
				req.nFamilyProp  = TEMP_FAMILY;
				g_pProtocol->SendReq(req);
			}
			else
			{
				SetClusterMember(m_clusterInfo.nFamilyID);
			}
		}
		else
		{
			//组内群发成员不能超过10个
			if(m_vecFriends.size() > MAX_TEMPFAMILYMEM_NUM - m_clusterInfo.Count())
			{
				CMessageBox::Model(GetSafeHwnd(),"群发成员超过30个，添加失败！", "温馨提示", MB_OK|MB_ICONWARNING);
				CDialog::OnOK();
				return;
			}
			char buff[MAX_SEND_BUFFERS];
			XT_SET_CMEMBERS_REQ  & req = *((LPXT_SET_CMEMBERS_REQ)buff);

			req.nFamilyID   = m_clusterInfo.nFamilyID;
			req.nAction     = CMEMBER_ADD;
			req.nFamilyProp = m_clusterInfo.nFamilyProp;
			req.nCount      = m_vecFriends.size();

			for ( int i=0; i<req.nCount; i++ )
			{
				req.nMemID[i]=m_vecFriends[i]->GetId();
			}
			req.nInviteID=g_ImUserInfo.GetId();

			g_pProtocol->SendReq(req);

		}

		//GetDlgItem(IDOK)->EnableWindow(FALSE);
		//GetDlgItem(IDCANCEL)->EnableWindow(FALSE);
		//
		//m_btnOk.SetTextColor(RGB(0,0,0));
		//m_btnCancel.SetTextColor(RGB(0,0,0));

		//SetTimer(0x01,350,NULL);
		CDialog::OnOK();
	}
	else
	{
		CDialog::OnCancel();
	}
}


void CClusterMemberSelectDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialog::OnCancel();
}

void CClusterMemberSelectDlg::OnBnClickedBtnAdd()
{
	//// TODO: 在此添加控件通知处理程序代码
	//CString strTxt = _T("");
	//int nItemCnt = m_wndListFriend.GetItemCount();
	//for ( int nItem=0; nItem<m_wndListFriend.GetItemCount(); nItem++ )
	//{
	//	if ( m_wndListFriend.GetItemCheck(nItem) )///选中了哪些好友
	//	{
	//		FRIEND_INFO *pFriend =(FRIEND_INFO *)m_wndListFriend.GetItemData(nItem);	
	//		int nFolder = m_mapGroup[pFriend->group_index];/////好友Index确认属于哪个好友分组
	//		
	//		int bTail = pFriend->status==XTREAM_OFFLINE ? true : false;
	//		int nItemIndex   = m_wndListFriend.AddItem(
	//			UserInfoToStr(pFriend),
	//			nFolder,
	//			false,
	//			GetDefaultImage(pFriend->sex,pFriend->status),
	//			bTail);
	//		m_wndListFriend.SetItemData(nItemIndex,(DWORD)pFriend);
	//		//m_vecFriends.push_back(pFriend);
	//		int i = 0;
	//		//////
	//	}
	//}
	///////////////////////////////////////////////////////////////////////////////////////////////
	//bool bChecked = FALSE;
	//int i = 0;
	//for ( int nItem=0; nItem<nItemCnt; nItem++ )
	//{
	//	bChecked =  m_wndListFriend.GetItemCheck(nItem);//判断是否选中
	//	if(bChecked)
	//	{
	//		strTxt = m_wndListFriend.GetItemText(nItem);
	//		////m_wndListFriend.SetItemCheck(nItem,0);//此项不选中
	//		m_ctlListAdd.AddString(strTxt);////需要过滤算法处理
	//	}
	//	else
	//	{
	//		i++;//未选中
	//	}
	//}
	//if(i == nItemCnt)
	//{
 //       MessageBox(_T("您没有选中任何项目！"));
	//}
}


void CClusterMemberSelectDlg::OnBnClickedBtnDel()
{
	//// TODO: 在此添加控件通知处理程序代码
	////CString strTxt = _T("");
	//if (m_nPos >= 0)
	//{   
	//	m_ctlListAdd.DeleteString(m_nPos);
	//	m_wndListFriend.SetItemCheck(m_nPos,0);//列表对应索引行去掉
	//}
	//else
	//{
	//	MessageBox(_T("您没有选中任何项目！"));
	//}
	//m_nPos = -1;//防止重复删除
}

//
BOOL CClusterMemberSelectDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message==WM_KEYDOWN )
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			{
				if (GetDlgItem(IDC_EDIT_CONTENT) == GetFocus())
				{
					OnBnClickedBtnSearch();
				}
			}
			break;
	/*	case VK_ESCAPE:
			return TRUE;*/
		default:
			break;
		}
	 }
	 return CXSkinDlg::PreTranslateMessage(pMsg);
}


void CClusterMemberSelectDlg::OnLbnSelchangeListAdd()
{
	// TODO: 在此添加控件通知处理程序代码
	int nPos = m_ctlListAdd.GetCurSel();
	FRIEND_INFO *pFriend =(FRIEND_INFO *)m_ctlListAdd.GetItemData(nPos);	
	int nChosed = pFriend->reserved;

	m_ctlListAdd.DeleteString(nPos);
	m_friend_id_map.erase(nChosed);
	m_nItemIndex--;/////插入位置减1
}

void CClusterMemberSelectDlg::OnBnClickedBtnSearch()
{
	CString strTxt =_T("");
	m_edtSearch.GetWindowText(strTxt);
	if(strTxt == _T("")||strTxt == _T("搜索好友"))
	{
		CMessageBox::Model(GetSafeHwnd(),_T("请输入正确的好友名称！"));
		return ;
	}
	if(m_wndListFriend.GetItemCount()<=0)
	{
		return ;
	}
	m_wndListFriend.ClearAll();
	m_mapGroup.clear();
	int bFlag = FALSE;
	int    i = 0;
	int nCnt = 0;
	int nFolder = 0;
	int nItemIndex = 0;
	bool bTail;
	CString strItem = _T("");
	((CEdit *)GetDlgItem(IDC_EDIT_CONTENT))->GetWindowText(strItem);
	FRIEND_LIST::iterator it;
	
	for(it=g_lsFriend.begin();it!=g_lsFriend.end();it++)
	{
		FRIEND_INFO *pFriend = (*it);
		strTxt.Format("%s",pFriend->nickname);
		if(strTxt.Find(strItem) !=-1 && strItem!=_T(""))////找到了
		{
			nCnt++;
			int nGroupCount = g_GroupInfo.count;
			int nGroup;
			int nIdx;
			if(!bFlag)
			{
				bFlag = TRUE;
				for ( nGroup = 0; nGroup< nGroupCount; nGroup++ )
				{
					nIdx = g_GroupInfo.group[nGroup].group_index;
					if(nIdx == 1 || nIdx == 2)
						continue;
					m_wndListFriend.AddFolder(g_GroupInfo.group[nGroup].name);
					m_mapGroup[nIdx] = nGroup;
				}
			}
	
			if(pFriend->group_index == 1 || pFriend->group_index == 2)
			{
				continue;
			}
			if ( m_pP2pInviteFriend && m_pP2pInviteFriend->GetId()==pFriend->GetId() )
			{
				continue;
			}
			if ( m_clusterInfo.GetFriendInfo(pFriend->GetId()) )
			{
				continue;
			}
	
			if ( m_pP2pInviteFriend )
			{
				if ( pFriend->status!=XTREAM_OFFLINE )				
				{
					nFolder = m_mapGroup[pFriend->group_index];
					bTail = pFriend->status==XTREAM_OFFLINE ? true : false;
					nItemIndex   = m_wndListFriend.AddItem(
						UserInfoToStr(pFriend),
						nFolder,
						true,//??
						GetDefaultImage(pFriend->sex,pFriend->status),
						bTail);
					m_wndListFriend.SetItemData(nItemIndex,(DWORD)pFriend);
				}
			}
			else
			{
				nFolder = m_mapGroup[pFriend->group_index];
				bTail = pFriend->status==XTREAM_OFFLINE ? true : false;
				nItemIndex   = m_wndListFriend.AddItem(
					UserInfoToStr(pFriend),
					nFolder,
					true,//默认选择
					GetDefaultImage(pFriend->sex,pFriend->status),
					bTail);
				m_wndListFriend.SetItemData(nItemIndex,(DWORD)pFriend);
			}
		}
	 }
	
	if(nCnt  == 0 && strTxt!=(""))////所有都没遍历到
	{
			CMessageBox::Model(GetSafeHwnd(),_T("请输入正确的好友名称！"));
			return ;
	}
}

void CClusterMemberSelectDlg::OnBnClickedBtnAll()
{
	 ////TODO: 在此添加控件通知处理程序代码
	   
		m_wndListFriend.ClearAll();
		int nFolder = 0;
		int nItemIndex = 0;
		bool bTail;
		FRIEND_LIST::iterator it;
		/////插入好友分组//////////////////////////////////////////////////////
		int nGroupCount = g_GroupInfo.count;
		int nGroup;
		for ( nGroup = 0; nGroup< nGroupCount; nGroup++ )
		{
			int nIdx = g_GroupInfo.group[nGroup].group_index;
			if(nIdx == 1 || nIdx == 2)/////陌生人或者黑名单
			{
				continue;
			}
			m_wndListFriend.AddFolder(g_GroupInfo.group[nGroup].name);
			m_mapGroup[nIdx] = nGroup;//g_GroupInfo.group[nGroup].name;
		}
	  ///////向每个组中插入好友成员///////////////////////////////////////////
		for(it=g_lsFriend.begin();it!=g_lsFriend.end();it++)
		{
			FRIEND_INFO *pFriend = (*it);
			int nGroupCount = g_GroupInfo.count;
			
			// ?????????
			if(pFriend->group_index == 1 || pFriend->group_index == 2)
			{
				continue;
			}
	
			// ?????????
			if ( m_pP2pInviteFriend && m_pP2pInviteFriend->GetId()==pFriend->GetId() )
			{
				continue;
			}
			// ??????????
			if ( m_clusterInfo.GetFriendInfo(pFriend->GetId()) )
			{
				continue;
			}
			/////////////////////////
			if ( m_pP2pInviteFriend )
			{
				// 在线
				if ( pFriend->status!=XTREAM_OFFLINE )				
				{
					nFolder = m_mapGroup[pFriend->group_index];
					bTail = pFriend->status==XTREAM_OFFLINE ? true : false;
					nItemIndex   = m_wndListFriend.AddItem(
							UserInfoToStr(pFriend),
							nFolder,
							false,
							GetDefaultImage(pFriend->sex,pFriend->status),
							bTail);
					m_wndListFriend.SetItemData(nItemIndex,(DWORD)pFriend);
				}
			}
			// 
			else
			{
				nFolder = m_mapGroup[pFriend->group_index];
				bTail = pFriend->status==XTREAM_OFFLINE ? true : false;
				nItemIndex   = m_wndListFriend.AddItem(
					UserInfoToStr(pFriend),
					nFolder,
					false,
					GetDefaultImage(pFriend->sex,pFriend->status),
					bTail);
				m_wndListFriend.SetItemData(nItemIndex,(DWORD)pFriend);
			}
		}
}


void CClusterMemberSelectDlg::OnEnSetfocusEditContent()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTxt =_T("");
	m_edtSearch.GetWindowText(strTxt);
	if(strTxt == _T("")||strTxt == _T("搜索好友"))
	{
		m_edtSearch.SetWindowText("");
	}
	
	//m_edtSearch.SetBackColor(RGB(0XFF,0XFF,0XFF));
	OnBnClickedBtnAll();
}


void CClusterMemberSelectDlg::OnEnKillfocusEditContent()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_edtSearch.SetBackColor(RGB(0X98,0X98,0X98));
	CString strTxt =_T("");
	m_edtSearch.GetWindowText(strTxt);
	if(strTxt == _T(""))
	{
		m_edtSearch.SetWindowText("搜索好友");
	}
}

void CClusterMemberSelectDlg::OnStnClickedStaticInfo()
{
	// TODO: 在此添加控件通知处理程序代码
}


void CClusterMemberSelectDlg::OnBnClickedBtnRight()
{
	// TODO: 在此添加控件通知处理程序代码
}

LRESULT CClusterMemberSelectDlg::OnAddFriendToCluster(WPARAM wParam, LPARAM lParam)
{
	/////////////////////链表处理重复数据插入/////////////////////////////////////
	map<DWORD,FRIEND_INFO>::iterator it;
	int nChosed = m_wndListFriend.GetCurSel();
	FRIEND_INFO *pFriend =(FRIEND_INFO *)m_wndListFriend.GetItemData(nChosed);///选中的项目的好友信息
	pFriend->reserved = nChosed;/////该字段存储初始列表中选中的项的位置
	////如果出现重复的ID和名称，丢弃	
	it = m_friend_id_map.find(nChosed);
	if (it==m_friend_id_map.end())////没找到，该节点为新增,若为已有节点，不插入
	{
		m_friend_id_map[nChosed] = *pFriend;/////map赋值
		m_ctlListAdd.AddString(UserInfoToStr(pFriend));
		m_ctlListAdd.SetItemData(m_nItemIndex,(DWORD)pFriend);
		m_nItemIndex++;
	
	}
	else
	{
		;
	}
	return S_OK;
}

