#include "stdafx.h"
#include "MainFrame.h"

// im\TabPageCluster.cpp : 实现文件
//
#include "DuiMfClusterList.h"
#include "stdafx.h"
#include "..\IMClient.h"
#include "Im\floorpagecluster.h"
#include "Im\p2pSendmsgDlg.h"
#include "Im\DuiP2PSendMsgDlg.h"
#include "Im\MessageHandleWnd.h"
#include "FriendInfoDlg.h"
#include "Im\ClusterCreateDlg.h"
#include "Im\ClustersettingDlg.h"
#include "Im\LocalData.h"
#include "Im\xtpacket.h"
#include "Im\DialogClusterAuth.h"
#include "Im\DailogClusterCheck.h"
#include "Im\FSDialog.h"
#include "Im\MsgDlg.h"
#include "Im\sysmsgdlg.h"
#include "Im\webprocess.h"
#include "..\HmgSkin\HmgSkin\FlatPopupMenu.h"
#include "DuiMfClusterList.h"
#include "..\..\Common\Common.h"
#include "DuiMfRecentList.h"

#define IDC_CLUSTER_LIST        0x05002

extern BOOL AppendDuiMenu(DuiLib::CMenuWnd* pDuiMenu, CMenu *pMenu);
//
//BEGIN_MESSAGE_MAP(CFloorPageCluster, CFloorPage)
//	ON_MESSAGE(WM_IMOPERATION,OnImOperation)
//	ON_MESSAGE(WM_CLUSTER_LIST_NOTIFY, OnGroupListNotify)
//	ON_COMMAND(ID_CLUSTER_CREATE,OnClusterCreate)
//	ON_COMMAND(IDC_BUTTON_CREATE,OnClusterCreate)
//	ON_COMMAND(ID_CLUSTER_FRIENDINFO, OnFriendInfo)
//	ON_COMMAND(ID_CLUSTER_SENDMSG, OnSendClusterMsg)
//	ON_COMMAND(ID_FRIEND_SENDMSG, OnSendP2PMsg)
//	ON_COMMAND(ID_CLUSTER_MODIFY, OnClusterInfo)
//	ON_COMMAND(ID_CLUSTER_HOME,OnClusterHome)
//	ON_COMMAND(ID_CLUSTER_FORUM,OnClusterForum)
//	ON_COMMAND(ID_CLUSTER_ALBUM,OnClusterAlbum)
//	ON_COMMAND(ID_CLUSTER_DISMISS, OnClusterDismiss)
//	ON_COMMAND(ID_CLUSTER_QUIT,OnQuitCluster)
//	ON_COMMAND(ID_MEMBER_DEL,OnMemberDel)
//	ON_COMMAND(ID_CLUSTER_MSGHISTORY,OnClusterMsgHistory)
//	ON_COMMAND(ID_FRIEND_MSGHISTORY,OnFriendMsgHistory)
//	ON_COMMAND(ID_SEARCH,OnSearchCluster)
//	ON_COMMAND(IDC_BUTTON_ADD,OnSearchCluster)
//	ON_COMMAND(ID_ADD2FRIEND,OnAdd2Friend)
//	ON_COMMAND(ID_CLUSTER_REFUSE,OnClusterRefuse)
//	ON_COMMAND(ID_USER_BLOG,OnUserBlog)
//	ON_COMMAND(ID_USER_ALBUM,OnUserAlbum)
//	ON_COMMAND(ID_CINFO_REFRESH,OnCInfoRefresh)
//	ON_COMMAND(ID_CMEMBER_LOADING,OnCMemberLoad)
//	ON_COMMAND(ID_CLIST_REFRESH,OnCListRefresh)
//	ON_MESSAGE(WM_HINTDLG_CLICK, OnHintDlgClick)
//	ON_MESSAGE(WM_FAMILY_HEAD_NOTIFY,OnFamilyIconNotify)
//END_MESSAGE_MAP()


// CFloorPageCluster 消息处理程序

//处理来自好友列表的鼠标右键
LRESULT CMainFrame::OnGroupListNotify(WPARAM wParam, LPARAM lParam)
{
	CL_GROUP_POS pos=m_pClusterList->GetCurSel();

	switch ( wParam )
	{
		// 群成员列表中双击
	case NM_CL_RBUTTONUP:
		{
			CMenuEx menu,*popmenu;
			menu.LoadMenu(IDR_MENU_TEMPLATE);
			popmenu = menu.GetSubMenu(0);
			popmenu->RemoveMenu(0,MF_BYPOSITION);

			CPoint point;
			GetCursorPos(&point);

			if ( pos.nGroup>-1 )
			{
				CLUSTER_INFO *pCluster =(CLUSTER_INFO *) m_pClusterList->GetGroupData(pos);

				int nType;//2，群主，1群管理员，0普通成员
				nType = pCluster->GetMemberType(g_ImUserInfo.GetId());

				CString str;
				if ( g_ImUserInfo.status!=XTREAM_OFFLINE )
				{
					popmenu->AppendMenu(MF_STRING,ID_CLUSTER_SENDMSG,"进入群聊天室");
					////popmenu->AppendMenu(MF_SEPARATOR,0);

					//if ( nType==0 )
					////popmenu->AppendMenu(MF_STRING,ID_CLUSTER_MODIFY,"查看群资料");
					//else
					////popmenu->AppendMenu(MF_STRING,ID_CLUSTER_MODIFY,"设置/查看群资料");

					////popmenu->AppendMenu(MF_STRING,ID_CLUSTER_HOME,"群主页");
					////popmenu->AppendMenu(MF_STRING,ID_CLUSTER_FORUM,"群讨论区");
					////popmenu->AppendMenu(MF_STRING,ID_CLUSTER_ALBUM,"群相册");

					////popmenu->AppendMenu(MF_STRING,ID_CINFO_REFRESH,"更新群信息");
					popmenu->AppendMenu(MF_STRING,ID_CLUSTER_REFUSE,pCluster->bRefused? "解除阻止..." :"阻止群消息");
					popmenu->AppendMenu(MF_STRING,ID_CLUSTER_MSGHISTORY,"查看聊天记录");
					////popmenu->AppendMenu(MF_STRING,ID_CLUSTER_CREATE,"创建群...");
					popmenu->AppendMenu(MF_STRING,ID_SEARCH_CLUSTER,"搜索并加入群...");

					if ( pCluster->bMemberInit || pCluster->Count() > 0 )
					{
						if ( nType==2 )//群主
						{
							popmenu->AppendMenu(MF_STRING,ID_CLUSTER_DISMISS,"解散群");
						}
						else//群管理员
							if ( nType==1 )
							{
								popmenu->AppendMenu(MF_STRING,ID_CLUSTER_QUIT,"退出群");
							}
							else//if ( nType==0 )//群成员
							{
								popmenu->AppendMenu(MF_STRING,ID_CLUSTER_QUIT,"退出群");
							}				
					}

					popmenu->AppendMenu(MF_STRING,ID_CLIST_REFRESH,"刷新群列表");

					//popmenu->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this);
					DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
					DuiLib::CPoint duiPoint(point.x,point.y);
					//ClientToScreen(m_hWnd, &point);
					pMenu->Init(NULL, _T("loginstatus.xml"), point, &m_PaintManager);
					AppendDuiMenu(pMenu, popmenu);
				}
			}
			else
			{
				if ( g_ImUserInfo.status!=XTREAM_OFFLINE )
				{
					popmenu->AppendMenu(MF_STRING,ID_CLIST_REFRESH,"刷新群列表");
					popmenu->AppendMenu(MF_STRING,ID_CLUSTER_CREATE,"创建群...");
					popmenu->AppendMenu(MF_STRING,ID_SEARCH_CLUSTER,"搜索并加入群...");	
					//popmenu->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this);

					DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
					DuiLib::CPoint duiPoint(point.x,point.y);
					//ClientToScreen(m_hWnd, &point);
					pMenu->Init(NULL, _T("loginstatus.xml"), point, &m_PaintManager);
					AppendDuiMenu(pMenu, popmenu);
				}
			}
		}
		break;
	}
	return 0;
}

void CMainFrame::SortGroupList()
{
	int nRoot = m_pClusterList->GetRootCount();

	uint32 nSelFamilyID = (uint32)-1;
	uint32 nSelUserID   = (uint32)-1;

	CL_GROUP_POS posSel = m_pClusterList->GetCurSel();

	if ( posSel.nGroup!=-1 )
	{
		CLUSTER_INFO * pSelCulster = (CLUSTER_INFO *) m_pClusterList->GetGroupData(posSel);
		nSelFamilyID = pSelCulster->nFamilyID;
	}

	for ( int n=0; n<nRoot; n++ )
	{
		int nGroups = m_pClusterList->GetGroupCount(n);

		int nMyGroupCount = 0;
		int i, j;

		CL_GROUP_POS posGroupI;
		posGroupI.nRoot = n;
	
		CL_GROUP_POS posGroupJ;
		posGroupJ.nRoot = n;

		for ( i=0; i<nGroups-1; i++ )
		{
			posGroupI.nGroup  = i;
			CL_GROUP_INFO *c1 = (CL_GROUP_INFO *)m_pClusterList->GetGroupInfo(posGroupI);

			for ( j=i+1; j<nGroups; j++ )
			{
				posGroupJ.nGroup  = j;
				CL_GROUP_INFO *c2 = (CL_GROUP_INFO *)m_pClusterList->GetGroupInfo(posGroupJ);

				if ( c1->bFlash!=c2->bFlash )
				{
					if ( c2->bFlash )
					{
						m_pClusterList->SwapGroup(posGroupI,posGroupJ);
						c1 = c2;
					}
				}
			}
		}

		int nFlashGroups = 0;

		for ( i=0; i<nGroups-1; i++ )
		{
			posGroupI.nGroup  = i;

			CL_GROUP_INFO *c1 = (CL_GROUP_INFO *)m_pClusterList->GetGroupInfo(posGroupI);
			if ( c1->bFlash )
				nFlashGroups++;
		}

		//sort other groups
		for ( i=nFlashGroups; i<nGroups-1; i++ )
		{
			posGroupI.nGroup = i;
			CLUSTER_INFO *c1 = (CLUSTER_INFO *)m_pClusterList->GetGroupData(posGroupI);

			for ( j=i+1; j<nGroups; j++ )
			{
				posGroupJ.nGroup  = j;
				CLUSTER_INFO * c2 = (CLUSTER_INFO *)m_pClusterList->GetGroupData(posGroupJ);

				if ( strcmp(c1->szFamilyName, c2->szFamilyName)>0 )
				{
					m_pClusterList->SwapGroup(posGroupI,posGroupJ);
					c1 = c2;
				}
			}
		}
	}
	//m_pClusterList->SetCurSel(posSel.nGroup,posSel.nItem);
}

void CMainFrame::BuildCluster(uint32 cid)
{
	CLUSTER_INFO *pCluster = FindClusterInfo(cid);

	if ( pCluster )
	{

		CL_GROUP_POS posGroup;
		posGroup = m_pClusterList->FindCluster(cid);

		FRIEND_INFO *pCreator = pCluster->GetCreator();

		if ( posGroup.IsNull()==false )
		{
			m_pClusterList->UpdateGroup(pCluster);
		}
		else
		{
			//区分"我创建的群"和"我加入的群"
			if ( pCreator && pCreator->GetId() == g_ImUserInfo.GetId() )
			{
				posGroup=m_pClusterList->AddGroup(0,pCluster);				
			}
			else
			{
				posGroup=m_pClusterList->AddGroup(1,pCluster);				
			}
		}
	}
}

void CMainFrame::OnSendClusterMsg()
{
	CL_GROUP_POS pos = (CL_GROUP_POS)m_pClusterList->GetCurSel();

	if ( pos.nGroup!=-1 )
	{
		CLUSTER_INFO *pCluster =
			(CLUSTER_INFO *)m_pClusterList->GetGroupData(pos);

		if ( pCluster )
		{
			CClusterSendMsgDlg *pDlg = g_pMessageHandleWnd->GetClusterSendMsgDlg(pCluster);

			if ( pDlg->m_hWnd==NULL )
				pDlg->Create(CClusterSendMsgDlg::IDD,CWnd::FromHandle(GetDesktopWindow()));

			pDlg->ShowWindow(SW_SHOW);

			if ( pDlg->IsIconic() )
				pDlg->ShowWindow(SW_RESTORE);

			pDlg->BringWindowToTop();
		}
	}

	if( pos.nGroup == -1 && pos.nRoot != -1 )
	{
		BOOL bExpand = m_pClusterList->IsExpand(pos.nRoot);
		m_pClusterList->ExpandRoot(pos.nRoot, !bExpand);
	}
}

void CMainFrame::OnSearchCluster()
{
	tr1::shared_ptr<CAddFriendOrClusterDlg> pDlg = CAddFriendOrClusterDlg::Show();
	pDlg->SetSearchType(FS_CLUSTER);
}

void CMainFrame::OnClusterCreate()
{
	CCreateClusterDlg::Show();
}

// 弹出群聊天记录对话框
void CMainFrame::OnClusterMsgHistory()
{  
	CL_GROUP_POS pos=(CL_GROUP_POS)m_pClusterList->GetCurSel();

	if ( pos.IsNull()==false )
	{
		CLUSTER_INFO *pInfo = (CLUSTER_INFO *)m_pClusterList->GetGroupData(pos);

		if ( pInfo )
		{
			std::tr1::shared_ptr<CDuiMsgDlg> pDlg = CDuiMsgDlg::Show();
			pDlg->SetDefaultCluster(pInfo->nFamilyID);
		}
	}
}

void CMainFrame::OnClusterInfo()
{
	CL_GROUP_POS pos = (CL_GROUP_POS)m_pClusterList->GetCurSel();

	if ( pos.IsNull()==false )
	{
		CLUSTER_INFO *pInfo = (CLUSTER_INFO *)m_pClusterList->GetGroupData(pos);

		if ( pInfo )
		{
			CClusterSettingDlg::Show(pInfo);
		}
	}
}

void CMainFrame::OnClusterAuthInd(const char *data,WORD wRet)
{
	LPXT_CLUSTER_AUTH_IND ind = (LPXT_CLUSTER_AUTH_IND)data;

	if ( wRet==0 )
	{
		if ( ind->action==0 )//申请
		{
		//	CDailogClusterCheck dlg(ind->nFamilyID,ind->uid,ind->info,GetDesktopWindow());
		//	dlg.DoModal();
			CDailogClusterCheck::Show(ind);
		}

		if (ind->action==1 )//同意
		{
			CString str ;
			str.Format("您被列为群　%u 的成员",ind->cluster_id);	
			ShowMessage(str,MB_OK|MB_ICONINFORMATION);

			CLUSTER_INFO * pCluster = FindClusterInfo(ind->cluster_id);

			if ( pCluster==NULL )
			{
				pCluster=new CLUSTER_INFO();
				pCluster->nFamilyID = ind->cluster_id;
				pCluster->nFamilyProp = PERM_FAMILY;

				g_vecCluster.push_back(pCluster);
			}

			ImGetClusterInfo(ind->cluster_id);
		}

		if ( ind->action==2 )
		{
			CString str;
			str.Format("群 %u 拒绝你的加入！",ind->cluster_id);
			//CMessageBox::Model(GetDesktopWindow()->m_hWnd,str,IM_CAPTION,MB_OK|MB_ICONINFORMATION);
			CMessageBox::Prompt(str);
		}
	}
}

void CMainFrame::OnClusterDismissInd(const char *data,WORD wRet)
{
	LPXT_CLUSTER_DISMISS_IND ind = (LPXT_CLUSTER_DISMISS_IND)data;

	if ( wRet==0 )
	{
		if ( ind->cluster_type==PERM_FAMILY )
		{
			CLUSTER_INFO *pCluster =  FindClusterInfo(ind->cluster_id);
			char szTemp[20];
			if ( pCluster )
			{
				FRIEND_INFO * pUser = pCluster->GetFriendInfo(ind->destructor_id);
				//这里先注释掉，防止解散群时群名字变成了群ID
// 				if(pCluster->bBasicInfo == false)
// 					strcpy(pCluster->szFamilyName,itoa(pCluster->nFamilyID,szTemp,10));
				CString str;

				if ( pUser )
				{
					if(g_ImUserInfo.GetId() == pCluster->GetCreator()->GetId())
						str.Format("群【%s】被您解散了",pCluster->szFamilyName/*,pUser->GetUserName().c_str()*//*nickname*/);
					else
						str.Format("群【%s】被群主【%s】解散了",pCluster->szFamilyName,pCluster->GetCreator()->GetUserName().c_str()/*nickname*/);
				}
				else
					str.Format("群【%s】被解散了",pCluster->szFamilyName/*,ind->destructor_id*/);
				ShowUserHintDlg(str);

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
				msgSys.nClusterId = ind->cluster_id;
				sprintf(msgSys.szClusterName, pCluster->szFamilyName);
				g_LocalData.AddClusterSystemMsg(g_ImUserInfo.GetId(),msgSys);

				DelCluster(pCluster->nFamilyID);
			}

		}
	}
}

// 请求加入族的返回数据处理
void CMainFrame::OnClusterApplyAck(const char *data,WORD wRet)
{
	LPXT_CLUSTER_APPLY_ACK ack = (LPXT_CLUSTER_APPLY_ACK)data;

	if ( wRet==0 )
	{
		if ( ack->nAction==0 && ack->nFamilyProp==PERM_FAMILY )//Add
		{
			if ( ack->nFlag==0 )
			{
				ShowMessage("加入群成功！",MB_OK|MB_ICONINFORMATION);

				CLUSTER_INFO * pCluster = FindClusterInfo(ack->nFamilyProp);

				if ( pCluster==NULL )
				{
					pCluster=new CLUSTER_INFO();
					pCluster->nFamilyID = ack->nFamilyID;
					pCluster->nFamilyProp = PERM_FAMILY;

					g_vecCluster.push_back(pCluster);
				}
			
				ImGetClusterInfo(ack->nFamilyID);
			}
			else
			if ( ack->nFlag==1 )//需要验证
			{
				CDialogClusterAuth::Show(ack->nFamilyID);
			}
			else
			if ( ack->nFlag==2 )//拒绝
			{
				CString str;
				str.Format("群 %u 拒绝你的加入！",ack->nFamilyID);
				ShowMessage(str,MB_OK|MB_ICONINFORMATION);
			}
		}
	}
	else
	if ( wRet==1 )
	{
		ShowMessage(g_pProtocol->GetLastError());
	}
}

// 处理返回的群列表的数据
void CMainFrame::OnClusterListAck(const char *data,WORD wRet)
{
	LPXT_GET_CLIST_ACK ack = (LPXT_GET_CLIST_ACK)data;
	char szTemp[20];

	if ( wRet==0 )
	{
		for ( int i=0; i<ack->nCount; i++ )
		{
			CLUSTER_INFO *pCluster;
			pCluster = FindClusterInfo(ack->nFamilyID[i]);
			if ( pCluster == NULL)
			{
				pCluster = new CLUSTER_INFO();
				pCluster->nFamilyID = ack->nFamilyID[i];
				g_vecCluster.push_back(pCluster);
			}
			// 如果该群的基本信息还没有获取则向服务器请求
			if (pCluster->bBasicInfo == false)
			{
				strcpy(pCluster->szFamilyName ,itoa(pCluster->nFamilyID,szTemp,10));
				// 获得每个群的信息
				ImGetClusterInfo(ack->nFamilyID[i]);
			}
		}// end for

		//更新群列表，清除已经退出或者解散的群
		int nClusters            = g_vecCluster.size();

		if ( nClusters>0 )
		{
			uint32  * arryClusterIds = new uint32[nClusters];
			for ( int n=0; n<nClusters; n++ )
			{
				arryClusterIds[n]=g_vecCluster[n]->nFamilyID;
			}

			for ( int n=0; n<nClusters; n++ )
			{
				bool flag=false;
				for ( int i=0; i<ack->nCount; i++ )
				{
					if ( arryClusterIds[n]==ack->nFamilyID[i] )
					{
						flag = true;
						break;
					}
				}

				if ( flag==false )
				{
					this->DelCluster(arryClusterIds[n]);
				}
			}

			delete [] arryClusterIds;
		}

		//加载每个群的设置信息(是否被阻止,是否自动播放魔法表情...)
		g_LocalData.LoadClusterConfig((char*)g_ImUserInfo.GetUserName().c_str());

		g_LocalData.LoadClusterExitConfig((char*)g_ImUserInfo.GetUserName().c_str());
		// 加载最近联系人名单
		m_pRecentList->Load();

		//向IM服务器获取历史聊天记录
		ImGetOfflineMsg();
	}
	else
	{
		//加载每个群的设置信息(是否被阻止,是否自动播放魔法表情...)
		g_LocalData.LoadClusterConfig((char*)g_ImUserInfo.GetUserName().c_str());
		g_LocalData.LoadClusterExitConfig((char*)g_ImUserInfo.GetUserName().c_str());
		//ShowMessage("获取群列表失败，请重试。");
	}
}

void CMainFrame::ImGetClusterList()
{
	XT_GET_CLIST_REQ req;

	req.nFamilyProp = PERM_FAMILY;
	req.nCount=0;

	g_pProtocol->SendReq(req);
}

void CMainFrame::ImGetClusterInfo(uint32 cid)
{
	XT_GET_CINFO_REQ req;

	req.nFamilyID   = cid;
	req.nFamilyProp = PERM_FAMILY;

	g_pProtocol->SendReq(req);
}

void CMainFrame::ImGetOfflineMsg()
{
	XT_OFFLINEMSG_REQ req;

	req.id = g_ImUserInfo.GetId();
	g_pProtocol->SendReq(req);
}

void CMainFrame::SetClusterOfflineFlag()
{
	for ( int i=0; i<::g_vecCluster.size(); i++ )
	{
		for ( int j=0; j<g_vecCluster[i]->Count(); j++ )
		{
			g_vecCluster[i]->members[j].status=XTREAM_OFFLINE;
		}
	}

	this->BuildClusterList();
}


void CMainFrame::ImGetClusterMember(uint32 cid)
{
	XT_GET_CMEMBERS_REQ req;

	CLUSTER_INFO *pCluster = FindClusterInfo(cid);
	m_arrNeedGetBasicInfo.clear();
	m_memberList.clear();
	if ( pCluster )
	{
		req.nFamilyID   = cid;
		req.nFamilyProp = PERM_FAMILY;
		req.req_count = 0;
		req.online_type  = 0;
		req.userstatus   = g_ImUserInfo.status;
		g_pProtocol->SendReq(req);
	}

	////发送群名片获取请求
	XT_PROFILENAME_GET_REQ reqMem;
	reqMem.nFamilyID   =cid;
	g_pProtocol->SendReq(reqMem);
}

// 处理接收的群信息
void CMainFrame::OnClusterInfoAck(const char *data,WORD wRet)
{
	LPXT_GET_CINFO_ACK ack = (LPXT_GET_CINFO_ACK)data;

	if ( wRet==0 )
	{
		CLUSTER_INFO *pCluster = FindClusterInfo(ack->nFamilyID);

		if ( pCluster )
		{
			pCluster->bBasicInfo = true;

			memcpy(pCluster,ack,sizeof(XT_CLUSTER_INFO));

			//?
			if ( pCluster->bRefreshFlag == true )
			{
				ImGetClusterMember(ack->nFamilyID);	
			}

			g_webFamilyHeadIconProcess.GetFamilyHeadIcon(pCluster,CWnd::FromHandle(GetHWND()));
		
			BuildCluster(pCluster->nFamilyID);
		}//end if ( pCluster )

		KillTimer(GetHWND(), TIMER_SORTGROUP);
		SetTimer(GetHWND(), TIMER_SORTGROUP, 300, NULL);
	}
	else if(wRet == SERVER_TIMEOUT)
	{
	}
}

void CMainFrame::BuildClusterList()
{
	m_pClusterList->RemoveAll();
	//不区分"我创建的群"和"我加入的群"
	m_pClusterList->AddRoot("我创建的群");	
	m_pClusterList->AddRoot("我加入的群");
	//m_pClusterList->AddRoot("群 组");

	for ( int i=0; i<g_vecCluster.size(); i++ )
	{
		BuildCluster(g_vecCluster[i]->nFamilyID);
	}	

	for ( int r=0; r<m_pClusterList->GetRootCount(); r++ )
	{
		for ( int g=0; g<m_pClusterList->GetGroupCount(r); g++ )
		{
			CL_GROUP_POS posGroup;
			posGroup.nRoot  = r;
			posGroup.nGroup = g;
			m_pClusterList->UpdateGroupImage(posGroup);
		}
	}
}

void CMainFrame::DelClusterMember(uint32 cid, uint32 mid)
{
	CLUSTER_INFO *pCluster = FindClusterInfo(cid);

	if ( mid==g_ImUserInfo.GetId() )
	{
		DelCluster(cid);
	}
	else
	{
		if( pCluster )
		{
			pCluster->DelMember(mid);
			this->BuildCluster(cid);
		}
	}
}

void CMainFrame::DelCluster(uint32 cid)
{
	int i;
	
	CL_GROUP_POS posGroup = m_pClusterList->FindCluster(cid);

	if ( posGroup.IsNull()==false )
	{
		m_pClusterList->DelGroup(posGroup);

		for ( i=0; i<g_vecCluster.size(); i++)
		{
			if ( g_vecCluster[i]->nFamilyID==cid )
			{
				CLUSTER_INFO pTemp = *g_vecCluster[i];
				g_vecClusterExit.push_back(&pTemp);/////退出或者解散的群

				delete g_vecCluster[i];
				g_vecCluster.erase(g_vecCluster.begin()+i);
				break;
			}
		}
	}

	g_pMessageHandleWnd->CloseClusterSendMsgDlg(cid);
}

// 族成员变化事件,加入,撤销
void CMainFrame::OnCMemberInd(const char *data, WORD wRet)
{
	LPXT_CMEMBER_IND ind = (LPXT_CMEMBER_IND)data;
	char szTemp[20];
	if ( wRet==0 )
	{
		CLUSTER_INFO *pCluster = FindClusterInfo(ind->cluster_id);

		if ( pCluster==NULL && ind->action==0 && ind->cluster_type==PERM_FAMILY )//被列为群的成员
		{
			if ( ind->member_id==g_ImUserInfo.GetId() )
			{
				CString str; 
				str.Format("您被列为群【%s】的成员！",ind->clustername );
				CLUSTER_INFO * pCluster = FindClusterInfo(ind->cluster_id);

				if ( pCluster==NULL )
				{
					pCluster=new CLUSTER_INFO();
					pCluster->nFamilyID = ind->cluster_id;
					pCluster->nFamilyProp = PERM_FAMILY;

					g_vecCluster.push_back(pCluster);					
				}
				// 弹出提示窗体
				ShowClusterHintDlg(pCluster,"群消息",str,m_hWnd);
				ImGetClusterInfo(ind->cluster_id);
				
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
				msgSys.nClusterId = ind->cluster_id;
				sprintf(msgSys.szClusterName, pCluster->szFamilyName);
				g_LocalData.AddClusterSystemMsg(g_ImUserInfo.GetId(),msgSys);
				return ;
			}
		}

		if ( ind->cluster_type==PERM_FAMILY && pCluster )
		{
			FRIEND_INFO* pFriendInfo = pCluster->GetFriendInfo(ind->member_id);/*GetFriendInfo(ind->member_id)*/;
			FRIEND_INFO* pCreater = pCluster->GetCreator();
			if(pFriendInfo)
			{
				strncpy(ind->nickname,pFriendInfo->nickname,sizeof(ind->nickname)-1);
			}
			else//无法获取到昵称先用ID代替
				strcpy(ind->nickname,itoa(ind->member_id,szTemp,10));

			CString str;
			CString strClusterName = pCluster->szFamilyName;		// 保存一份群的名字，避免群被删除后程序崩溃
			if ( ind->action==0 )//添加成员通知
			{
				if ( pCluster->GetFriendInfo(ind->member_id)==NULL )
				{
					FRIEND_INFO member;
					member.SetId(ind->member_id);
					member.status = XTREAM_OFFLINE;
					member.cluster_property = CMEMBER_NORMAL;//putong 成员
					//strcpy(ind->nickname,itoa(ind->member_id,szTemp,10));不需要把成员ID赋给nickname
					strcpy(member.nickname,ind->nickname);
					pCluster->members.push_back(member);

					// 获取该成员状态
					XT_STATUS_GET_REQ req;
					req.id = g_ImUserInfo.GetId();
					req.fid = ind->member_id;
					g_pProtocol->SendReq(req);
					// 获取该成员的详细资料
					XT_USERINFO_GET_REQ getInfo;
					getInfo.fid = ind->member_id;
					g_pProtocol->SendReq(getInfo);

					BuildCluster(ind->cluster_id);	
					//str.Format("群 \"%s\" 有新成员加入。他(她)叫: %s！",pCluster->szFamilyName,ind->nickname);
					str.Format("【%s】加入群【%s】",/*pFriendInfo->username*/ind->nickname,pCluster->szFamilyName);

					//写群系统消息
					/*SYSTEM_MSG msgSys;
					msgSys.dwTime = time(NULL);
					msgSys.iType  = SYSTEM_MSG::SYSMSG_CLUSTER;
					sprintf(msgSys.szMessage,str);
					g_LocalData.AddSystemMsg(g_ImUserInfo.GetId(),msgSys);*/
					
					// 新成员加入就不再出提示框
					//return;
				}	
			}
			else if ( ind->action==1 )//退出,删除
			{
				if ( ind->member_id==g_ImUserInfo.GetId() )
				{
					str.Format("您退出了群【%s】",pCluster->szFamilyName);
					DelCluster(ind->cluster_id);
				}
				else
				{
					pCluster->DelMember(ind->member_id);
					str.Format("【%s】退出了群【%s】",/*pFriendInfo->username*/ind->nickname,pCluster->szFamilyName);
				}

				DelClusterMember(ind->cluster_id,ind->member_id);
			}
			else if ( ind->action==2 )//设置为管理员通知
			{
				if ( ind->member_id==g_ImUserInfo.GetId() )
				{
					str.Format("您被群【%s】的群主【%s】设置成管理员",pCluster->szFamilyName,pCreater->GetUserName().c_str());
				}
				else
				{
					

					CString str;
					str.Format("【%s】被群【%s】的群主【%s】设置成管理员",/*pFriendInfo->username*/ind->nickname,ind->clustername,pCreater->GetUserName().c_str());

					//str.Format("【%s】成为群【%s】的管理员",pFriendInfo->username/*ind->nickname*/,pCluster->szFamilyName);
				}

				FRIEND_INFO *pFriend =	pCluster->GetFriendInfo(ind->member_id);
				if ( pFriend )
					pFriend->cluster_property = 1;

			}
			else if ( ind->action==3)// 撤销管理员
			{
				if ( ind->member_id==g_ImUserInfo.GetId() )
				{
					str.Format("您被群【%s】的群主【%s】取消了管理员权限",pCluster->szFamilyName,pCreater->GetUserName().c_str());
				}
				else
				{
					str.Format("【%s】被群【%s】的群主【%s】取消了管理员权限",/*pFriendInfo->username*/ind->nickname,pCluster->szFamilyName,pCreater->GetUserName().c_str());
				}

				FRIEND_INFO *pFriend =	pCluster->GetFriendInfo(ind->member_id);
				if ( pFriend )
					pFriend->cluster_property = 0;
			}
// 			else if(ind->action == 4)//更改名片
// 			{
// 
// 			}
			else if(ind->action == CMEMBER_INVITE_REFUSE)//被邀请方拒绝入群
			{
				FRIEND_INFO* pFriendInfo = GetFriendInfo(ind->member_id);
				str.Format("【%s】拒绝加入群【%s】",/*pFriendInfo->username*/ind->nickname,ind->clustername);
			}
			else if(ind->action == CMEMBER_CREATOR_REFUSE)//群主拒绝被邀请方入群
			{
				FRIEND_INFO* pFriendInfo = GetFriendInfo(ind->member_id);
				FRIEND_INFO* pCreator = pCluster->GetCreator();
				str.Format("【%s】拒绝【%s】加入群【%s】",pCreator->GetUserName().c_str(),/*pFriendInfo->username*/ind->nickname,ind->clustername);
			}
			else
			{
				return ;
			}


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
			msgSys.nClusterId = ind->cluster_id;
			sprintf(msgSys.szClusterName, strClusterName);
			g_LocalData.AddClusterSystemMsg(g_ImUserInfo.GetId(),msgSys);

			//弹提示框
			ShowUserHintDlg(str);
		}
	}
}

// 邀请好友入群
void CMainFrame::OnClusterInviteInd(const char *data, WORD wRet)
{
	LPXT_CLUSTER_INVITE_IND ind = (LPXT_CLUSTER_INVITE_IND)data;
	char szTemp[20];
	if ( wRet==0 )
	{
		CLUSTER_INFO *pCluster = FindClusterInfo(ind->cluster_id);

		if ( ind->cluster_type==PERM_FAMILY )//被列为群的成员
		{
// 			if ( ind->cluster_id==g_ImUserInfo.GetId() )
// 			{
				CString str; 
				//str.Format("你被列为群 %s 的成员！",ind->clustername );
				str.Format("【%s】邀请您加入群【%s】", FindFriend(ind->invite_id)->nickname, ind->clustername );
				CLUSTER_INFO * pCluster = FindClusterInfo(ind->cluster_id);
				if ( pCluster==NULL )
				{
					pCluster=new CLUSTER_INFO();
					pCluster->nFamilyID = ind->cluster_id;
					pCluster->nFamilyProp = PERM_FAMILY;
					strcpy(pCluster->szFamilyName, ind->clustername);
					//g_vecCluster.push_back(pCluster);					
				}
				// 弹出提示窗体
				ShowClusterAgreeDlg(pCluster,FindFriend(ind->invite_id),"群消息",/*str*/ ind->clustername, m_hWnd);
				//ImGetClusterInfo(ind->cluster_id);
				//写群系统消息
				//SYSTEM_MSG msgSys;
				//msgSys.dwTime = time(NULL);
				//msgSys.iType  = SYSTEM_MSG::SYSMSG_CLUSTER;
				//sprintf(msgSys.szMessage,str);
				//g_LocalData.AddSystemMsg(g_ImUserInfo.GetId(),msgSys);

				CLUSTER_SYSTEM_MSG msgSys;
				msgSys.dwTime = time(NULL);
				sprintf(msgSys.szMessage,str);
				msgSys.nInviteId = ind->invite_id;
				sprintf(msgSys.szNickName, FindFriend(ind->invite_id)->nickname);
				msgSys.nClusterId = ind->cluster_id;
				sprintf(msgSys.szClusterName, ind->clustername);
				g_LocalData.AddClusterSystemMsg(g_ImUserInfo.GetId(),msgSys);

				return ;
//			}
		}
	}
}

// 好友是否入群
void CMainFrame::OnClusterAgreeInd(const char *data, WORD wRet)
{
	LPXT_CLUSTER_AGREE_IND ind = (LPXT_CLUSTER_AGREE_IND)data;
	char szTemp[20];
	if ( wRet==0 )
	{
		if(ind->action == 0)//好友同意入群
		{
			if(g_ImUserInfo.GetId() == ind->invite_id)//如果是群主邀请则直接入群
			{
				char buff[MAX_SEND_BUFFERS];
				XT_SET_CMEMBERS_REQ  & req = *((LPXT_SET_CMEMBERS_REQ)buff);
				req.nFamilyID   = ind->cluster_id;
				req.nAction     = CMEMBER_ADD;
				req.nFamilyProp = PERM_FAMILY;
				req.nCount      = 1;
				req.nMemID[0]=ind->friend_id;
				req.nInviteID=ind->invite_id;
				g_pProtocol->SendReq(req);
			}
			else//不是群主则需要通过群主的验证
			{
				char buff[MAX_SEND_BUFFERS];
				XT_CLUSTER_AUTH_REQ req;// = *((LPXT_CLUSTER_AUTH_REQ)buff);
				req.nAction = 0;
				req.nFamilyProp = ind->cluster_type;
				req.nFamilyID = ind->cluster_id;
				req.nRequestID = ind->friend_id;
				req.nManagerID = 0;
				req.send_time  = time(NULL);
				g_pProtocol->SendReq(req);
			}
		}
		else if(ind->action == 1)//好友不同意入群
		{
			//群消息通知
			char buff[MAX_SEND_BUFFERS];
			XT_SET_CMEMBERS_REQ  & req = *((LPXT_SET_CMEMBERS_REQ)buff);
			req.nFamilyID   = ind->cluster_id;
			req.nAction     = CMEMBER_INVITE_REFUSE;
			req.nFamilyProp = PERM_FAMILY;
			req.nCount      = 1;
			req.nMemID[0]=ind->friend_id;
			req.nInviteID=ind->invite_id;
			g_pProtocol->SendReq(req);
		}
	}
}

void CMainFrame::OnQuitCluster()
{
	CL_GROUP_POS pos = m_pClusterList->GetCurSel();

	if ( pos.nGroup==-1 )
	{
		return ;
	}

	CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_pClusterList->GetGroupData(pos);

	if ( pCluster )
	{
		CString str;
		str.Format("确定要退出该群吗？");

		if (CMessageBox::Model(GetHWND(),str,IM_CAPTION,MB_YESNO|MB_ICONWARNING)!=IDYES)
		{
			return ;
		}
		// 关闭族聊天对话框
		g_pMessageHandleWnd->CloseClusterSendMsgDlg(pCluster->nFamilyID );
		XT_CLUSTER_APPLY_REQ req;
		req.nAction     = 1;
		req.nFamilyID   = pCluster->nFamilyID;
		req.nFamilyProp = PERM_FAMILY;
		req.nUserID     = g_ImUserInfo.GetId();
		g_pProtocol->SendReq(req);

		m_pClusterList->DelGroup((CL_GROUP_POS)pos);

		//退出群当前选中的群则把选中位置初始化
		m_pClusterList->ClearCurSel();
		for ( int i=0; i<g_vecCluster.size(); i++)
		{
			if ( g_vecCluster[i]->nFamilyID==pCluster->nFamilyID )
			{
				CLUSTER_INFO pTemp = *(pCluster);
				g_vecClusterExit.push_back(&pTemp);/////退出或者解散的群

				delete g_vecCluster[i];
				g_vecCluster.erase(g_vecCluster.begin()+i);
				break;
			}
		}		
	}
}

// 解散群 
void CMainFrame::OnClusterDismiss()
{
	CL_GROUP_POS pos = (CL_GROUP_POS)m_pClusterList->GetCurSel();

	if ( pos.IsNull() )
		return ;

	CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_pClusterList->GetGroupData(pos);

	if ( pCluster )
	{
		CString str;
		str.Format("确定要解散该群吗？");

		if (CMessageBox::Model(GetHWND(), str,IM_CAPTION,MB_YESNO)!=IDYES)
		{
			return ;
		}

		CString strCluterName = pCluster->szFamilyName;
		//CString strMsg = "您已经解散了群\"" + strCluterName + "\"";
		CString strMsg;
		strMsg.Format("群【%s】被您解散了", strCluterName);
		//写群系统消息
		//SYSTEM_MSG msgSys;
		//msgSys.dwTime = time(NULL);
		//msgSys.iType  = SYSTEM_MSG::SYSMSG_CLUSTER;
		//sprintf(msgSys.szMessage,/*str*/strMsg);
		//g_LocalData.AddSystemMsg(g_ImUserInfo.GetId(),msgSys);

		CLUSTER_SYSTEM_MSG msgSys;
		msgSys.dwTime = time(NULL);
		sprintf(msgSys.szMessage,strMsg);
		msgSys.nInviteId = 0;
		msgSys.nClusterId = pCluster->nFamilyID;
		sprintf(msgSys.szClusterName, pCluster->szFamilyName);
		g_LocalData.AddClusterSystemMsg(g_ImUserInfo.GetId(),msgSys);

		ShowUserHintDlg(strMsg);
		XT_CLUSTER_DISMISS_REQ req;
		req.nFamilyID = pCluster->nFamilyID;
		req.nFamilyProp = PERM_FAMILY;
		g_pProtocol->SendReq(req);

		// 关闭族聊天对话框
		g_pMessageHandleWnd->CloseClusterSendMsgDlg(pCluster->nFamilyID );
	
		for ( int i=0; i<g_vecCluster.size(); i++)
		{
			if ( g_vecCluster[i]->nFamilyID==pCluster->nFamilyID )
			{ 
				CLUSTER_INFO pTemp = *pCluster;
				g_vecClusterExit.push_back(&pTemp);/////退出或者解散的群

				delete g_vecCluster[i];
				g_vecCluster.erase(g_vecCluster.begin()+i);
             
				break ;
			}
		}

		m_pClusterList->DelGroup(pos);
	}
}

// 
void CMainFrame::OnSetCMemberAck(const char *data,WORD wRet)
{
	LPXT_SET_CMEMBERS_ACK ack = (LPXT_SET_CMEMBERS_ACK)data;

	if ( wRet==0 )
	{
		if ( ack->nFamilyProp==PERM_FAMILY  )
		{
			CLUSTER_INFO *pCluster = FindClusterInfo(ack->nFamilyID);

			if ( pCluster!=NULL )//新创建的群
			{
				CLUSTER_INFO * pCluster = FindClusterInfo(ack->nFamilyID);

				switch (ack->nAction)
				{
				case CMEMBER_SET_ADMIN:
					if ( pCluster )
					{
						FRIEND_INFO *pUser = pCluster->GetFriendInfo(ack->nMemberID);
						if ( pUser )
						{
							pUser->cluster_property = CMEMBER_ADMIN;
							this->BuildCluster(ack->nFamilyID);
						}
					}
					break;
				case CMEMBER_CANCEL_ADMIN:
					if ( pCluster )
					{
						FRIEND_INFO *pUser = pCluster->GetFriendInfo(ack->nMemberID);
						if ( pUser )
						{
							pUser->cluster_property = CMEMBER_NORMAL;
							this->BuildCluster(ack->nFamilyID);
						}
					}
					break;
				case CMEMBER_DEL:
					if ( pCluster )
					{
						//pCluster->DelMember(ack->nMemberID);
						this->BuildCluster(ack->nFamilyID);
					}
					break;
				case CMEMBER_ADD:
					{
						ImGetClusterMember(ack->nFamilyID);
						FRIEND_INFO* pFriendInfo = GetFriendInfo(ack->nMemberID);
						CString strMsg;
						strMsg.Format("【%s】加入群【%s】", pFriendInfo->nickname/*username*/, pCluster->szFamilyName);
						//SYSTEM_MSG msgSys;
						//msgSys.dwTime = time(NULL);
						//msgSys.iType  = SYSTEM_MSG::SYSMSG_CLUSTER;
						//sprintf(msgSys.szMessage,/*str*/strMsg);
						//g_LocalData.AddSystemMsg(g_ImUserInfo.GetId(),msgSys);

						CLUSTER_SYSTEM_MSG msgSys;
						msgSys.dwTime = time(NULL);
						sprintf(msgSys.szMessage,strMsg);
						msgSys.nInviteId = 0;
						msgSys.nClusterId = ack->nFamilyID;
						sprintf(msgSys.szClusterName, pCluster->szFamilyName);
						g_LocalData.AddClusterSystemMsg(g_ImUserInfo.GetId(),msgSys);
						//弹提示框
						ShowUserHintDlg(strMsg);
					}
					break;
				//case CMEMBER_INVITE_REFUSE:
				//	{
				//		CString str;
				//		FRIEND_INFO* pFriendInfo = GetFriendInfo(ack->nMemberID);
				//		str.Format("【%s】拒绝加入群【%s】",pFriendInfo->username/*ind->nickname*/,pCluster->szFamilyName);
				//		//写群系统消息
				//		SYSTEM_MSG msgSys;
				//		msgSys.dwTime = time(NULL);
				//		msgSys.iType  = SYSTEM_MSG::SYSMSG_CLUSTER;
				//		sprintf(msgSys.szMessage,str);
				//		g_LocalData.AddSystemMsg(g_ImUserInfo.GetId(),msgSys);
				//		//弹提示框
				//		ShowUserHintDlg(str);
				//	}
				//	break;
				//case CMEMBER_CREATOR_REFUSE:
				//	{
				//		CString str;
				//		FRIEND_INFO* pCreator = pCluster->GetCreator();
				//		FRIEND_INFO* pFriendInfo = GetFriendInfo(ack->nMemberID);
				//		str.Format("【%s】拒绝【%s】加入群【%s】",pCreator->username,pFriendInfo->username/*ind->nickname*/,pCluster->szFamilyName);

				//		//写群系统消息
				//		SYSTEM_MSG msgSys;
				//		msgSys.dwTime = time(NULL);
				//		msgSys.iType  = SYSTEM_MSG::SYSMSG_CLUSTER;
				//		sprintf(msgSys.szMessage,str);
				//		g_LocalData.AddSystemMsg(g_ImUserInfo.GetId(),msgSys);

				//		//弹提示框
				//		ShowUserHintDlg(str);
				//	}
				//	break;
				}
		//		ImGetClusterMember(ack->nFamilyID);
			}
		}
	}
	else
	if ( wRet==SERVER_TIMEOUT )
	{
		//if ( ack->nFamilyProp==TEMP_FAMILY )
		//	ShowMessage("邀请多人聊天出现意外,服务器处理超时或您的网络有问题.请稍候再试.!",MB_OK|MB_ICONINFORMATION);
		//else
		//if ( ack->nFamilyProp==PERM_FAMILY )
		//{
		//	ShowMessage("设置群成员时出现现意外,服务器处理超时或您的网络有问题.请稍候再试.!",MB_OK|MB_ICONINFORMATION);
		//}
	}
	else
	{
		ShowMessage(g_pProtocol->GetLastError());
	}
}

void CMainFrame::OnCListRefresh()
{
	static DWORD dwPreRefreshTick = (DWORD)0;
	DWORD dwTick = GetTickCount();
	ImGetClusterList();
	dwPreRefreshTick = dwTick;
//#ifndef _DEBUG
//	if ( (dwTick-dwPreRefreshTick)>60000 )
//	{
//		this->
//	}
//#endif
}

void CMainFrame::OnCInfoRefresh()
{
	CL_GROUP_POS pos = this->m_pClusterList->GetCurSel();
	
	if ( pos.IsNull()==false )
	{
		CLUSTER_INFO *pCluster     = (CLUSTER_INFO *)m_pClusterList->GetGroupData(pos);
		CLUSTER_INFO *pRealCluster = FindClusterInfo(pCluster->nFamilyID);

		if ( pRealCluster )
		{
			pRealCluster->bRefreshFlag = true;
		
			XT_GET_CINFO_REQ req;
			req.nFamilyID   = pCluster->nFamilyID;
			req.nFamilyProp = PERM_FAMILY;
		
			g_pProtocol->SendReq(req);
		}
		else
		{
			// 如果该群在全局群中不存在,则删除之
			//DelCluster(pCluster->nFamilyID);
		}

	}
}

void CMainFrame::OnSetClusterInfoAck(const char *data,WORD wRet)
{
	LPXT_SET_CINFO_ACK ack = (LPXT_SET_CINFO_ACK)data;

    if ( wRet==0 )
	{
		CLUSTER_INFO *pCluster = FindClusterInfo(ack->nFamilyID);

		if ( pCluster && CClusterSettingDlg::pInstance )
		{
			XT_SET_CINFO_REQ * pInfo = &CClusterSettingDlg::pInstance->m_cInfoReq;
			
			sprintf(pCluster->szFamilyName,pInfo->szFamilyName);
			sprintf(pCluster->szFamilyDesc,pInfo->szFamilyDesc);
			sprintf(pCluster->szFamilySign,pInfo->szFamilySign);
			sprintf(pCluster->szFamilyNotice,pInfo->szFamilyNotice);
			pCluster->nFamilyClass = pInfo->nFamilyClass;
		
			pCluster->nJoinFlag         = pInfo->nJoinFlag;

			this->BuildCluster(pCluster->nFamilyID);
		}
	}	
}

LRESULT CMainFrame::OnImOperation(WPARAM wParam,LPARAM lParam)
{
	int nCmd = (int)wParam;
	switch(nCmd)
	{
	case OPR_DEL_CLUSTER:
		DelCluster((int)lParam);
		break;
	case OPR_MODIFY_CLUSTER_NAME:
		BuildCluster((int)lParam);
		break;
	}
	return 0;
}

LRESULT CMainFrame::OnDismissCluster(WPARAM wParam, LPARAM lParam)
{
	int nID=(int)wParam;

	CL_GROUP_POS posGroup = m_pClusterList->FindCluster(nID);

	if ( posGroup.IsNull()==false )
	{
		CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_pClusterList->GetGroupData(posGroup);
		// 如果列表中不存在待解散的组，则返回
		if(pCluster==NULL)
		{
			return -1;
		}
		// 关闭族聊天对话框
		g_pMessageHandleWnd->CloseClusterSendMsgDlg(pCluster->nFamilyID );

		// 清除全局族列表中的该族
		for ( int i=0; i<g_vecCluster.size(); i++)
		{
			if ( g_vecCluster[i]->nFamilyID==pCluster->nFamilyID )
			{
				CLUSTER_INFO pTemp = *(pCluster);
				g_vecClusterExit.push_back(&pTemp);/////退出或者解散的群

				delete g_vecCluster[i];
				g_vecCluster.erase(g_vecCluster.begin()+i);
				break ;
			}
		}
		m_pClusterList->DelGroup(posGroup);
	}

	return 0;
}

LRESULT CMainFrame::OnHintClick_ClusterPage(WPARAM wParam, LPARAM lParam)
{
	CLUSTER_INFO *pCluster = (CLUSTER_INFO *)(lParam);
	ASSERT(pCluster);
	CClusterSendMsgDlg *pDlg = g_pMessageHandleWnd->GetClusterSendMsgDlg(pCluster);
	if ( pDlg->m_hWnd==NULL )
		pDlg->Create(CClusterSendMsgDlg::IDD,CWnd::FromHandle(GetDesktopWindow()));

	if ( pDlg->IsWindowVisible()==FALSE )
		pDlg->ShowWindow(SW_SHOW);

	if ( pDlg->IsIconic() )
	{
		pDlg->ShowWindow(SW_RESTORE);
	}

	pDlg->FlashWindowEx(FLASHW_TRAY|FLASHW_TIMERNOFG,20,500);
	pDlg->SetForegroundWindow();
	pDlg->BringWindowToTop();
	return 0;
}

void CMainFrame::LoadLoacalClusterList()
{
	g_LocalData.LoadClusterList((char*)g_ImUserInfo.GetUserName().c_str());
	//g_LocalData.LoadClusterExitList(g_ImUserInfo.GetUserName().c_str());
	BuildClusterList();
	SortGroupList();
}

void CMainFrame::FindOldMembers(
						const vector<FRIEND_INFO> & oldList, 
						const vector<uint32> & newList, 
						vector<uint32> & oldMembers )
{
	vector<FRIEND_INFO>::const_iterator it,itE;
	size_t i,c;
	oldMembers.clear();
	itE = oldList.end();
	c = newList.size();
	for(it = oldList.begin();it != itE; it++)
	{
		for(i = 0; i < c; i++)
		{
			if(((FRIEND_INFO*)(&(*it)))->GetId() == newList[i])
				break;
		}
		// 如果新列表中没有，则为垃圾数据，需要删除
		if(i == c)
		{
			oldMembers.push_back(((FRIEND_INFO*)(&(*it)))->GetId());
		}
	}
}

void CMainFrame::OnBnClickedButtonCreate()
{
	// TODO: 在此添加控件通知处理程序代码
}


//////向服务器请求获取群名片信息
void CMainFrame::OnGetCMProfileAck(const char *data, WORD wRet)
{
	//CL_GROUP_POS posGroup = m_pClusterList->FindCluster(ack->nFamilyID);
	//if ( posGroup.IsNull()==false )
	//{
	//	CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_pClusterList->GetGroupData(posGroup);
	//	LPXT_PROFILENAME_GET_ACK ack = (LPXT_PROFILENAME_GET_ACK)data;
	//	if (wRet == 0 && ack->nFamilyID == pCluster->nFamilyID)
	//	{
	//		//CLUSTER_INFO * pCluster = FindClusterInfo(m_clusterInfo.nFamilyID);/////获取群信息
	//		for ( int i=0; i<ack->nCount; i++ )
	//		{
	//			XT_CMEM_IFNO   &info = ack->MemInfo[i];
	//			FRIEND_INFO *pUser = pCluster->GetFriendInfo(info.nMemID);
	//			if (pUser)/////本群中含有该成员信息
	//			{
	//				strcpy(pUser->profile_name,info.ProfileName);/////赋值群名片
	//			}
	//		}
	//		this->m_wndClusterPanel.UpdateClusterProfile();  //更新群名片显示
	//	}

	//}
}

// 处理获得族成员列表数据
void CMainFrame::OnClusterMembersAck(const char *data,WORD wRet)
{
	LPXT_GET_CMEMBERS_ACK ack = (LPXT_GET_CMEMBERS_ACK)data;
	bool bExist = true;
	bool bNeedUpdate = true;
	uint32 id;

	if ( wRet==0 && ack->nFamilyProp==PERM_FAMILY )
	{
		CLUSTER_INFO *pCluster = FindClusterInfo(ack->nFamilyID);
		FRIEND_INFO member;
		if ( pCluster )
		{
			for ( int i=0; i<ack->return_count; i++ )
			{
				id = ack->members[i].id;
				if ( id == 0 )
				{
					continue;
				}
				m_memberList.push_back(id);
				bExist = true;
				bNeedUpdate = true;
				// 判断该用户是否是自己的好友，以便从好友列表缓存中获取基本资料
				// 否则从服务器获得
				FRIEND_INFO * pCache = GetFriendInfo(id);
				FRIEND_INFO *pFriend = pCluster->GetFriendInfo(id);
				if ( pFriend == NULL )
				{
					member.SetId(ack->members[i].id);
					member.status = ack->members[i].status;
					// 暂时使用id作昵称
					itoa(member.GetId(),member.nickname,10);
					pCluster->members.push_back(member);
					pFriend = &pCluster->members[pCluster->Count()-1];

					if ( pCache==NULL )
					{
						// 该好友在缓存中不存在
						bExist = false;
					}
					else
					{
						// 保存群名片不变
						string strProfileName = pFriend->profile_name;
						*pFriend = *pCache;
						strncpy(pFriend->profile_name, strProfileName.c_str(), 10);
					}
				}
				else if(pCache)
				{
					// 保存群名片不变
					string strProfileName = pFriend->profile_name;
					*pFriend = *pCache;
					strncpy(pFriend->profile_name, strProfileName.c_str(), 10);
				}
				// 好友在缓存中存在
				if( bExist )
				{
					if( pFriend->bBasicInfo )
					{
						bNeedUpdate = false;	// 已经存在该好友的基本资料不需要更新
					}
				}

				if(bNeedUpdate)
				{
					// 如果需要从服务器获取基本信息，则先保存到列表
					m_arrNeedGetBasicInfo.push_back(id);
				}
				pFriend->cluster_property = ack->members[i].prop;
				pFriend->status           = ack->members[i].status;
				if ( pFriend->cluster_property == CMEMBER_CREATOR )
				{
					pCluster->nCreatorID = pFriend->GetId();
				}
			}

			if ( ack->next_pos != 0xFFFF && ack->next_pos != 0)
			{
				XT_GET_CMEMBERS_REQ req;
				req.nFamilyID   = pCluster->nFamilyID;
				req.nFamilyProp = PERM_FAMILY;
				req.req_count     = 0;
				req.start_pos = ack->next_pos;
				req.online_type = 0;//所有
				req.userstatus  = g_ImUserInfo.status;
				g_pProtocol->SendReq(req);
			}
			else
			{
				int i;
				int mid,cid;
				vector<uint32> oldMembers;
				// 群成员列表获取完毕
				if ( pCluster->bRefreshFlag )
				{
					pCluster->bRefreshFlag=false ;
				}
				// 从服务器获取好友基本资料
				m_arrFriendsID_Basic = m_arrNeedGetBasicInfo;
				ImGetFriendBasicInfo();
				// 删除过时的群成员
				FindOldMembers(pCluster->members,m_memberList,oldMembers);

				for( i = 0; i < oldMembers.size();i++)
				{
					//DelClusterMember(pCluster->nFamilyID,oldMembers[i]);
					cid = pCluster->nFamilyID;
					mid = oldMembers[i];
					if ( mid==g_ImUserInfo.GetId() )
					{
						DelCluster(cid);
					}
					else
					{
						if( pCluster )
						{
							pCluster->DelMember(mid);
						}
					}
				}
				m_arrNeedGetBasicInfo.clear();
				m_memberList.clear();
				pCluster->bMemberInit = true;


				BuildCluster(ack->nFamilyID);

				KillTimer(GetHWND(), TIMER_SORTGROUP);
				SetTimer(GetHWND(), TIMER_SORTGROUP,300,NULL);
			}
		}
	}
}

// 阻止群消息或解除阻止
void CMainFrame::OnClusterRefuse()
{
	CL_GROUP_POS pos = (CL_GROUP_POS)m_pClusterList->GetCurSel();

	if ( pos.nGroup!=-1 )
	{
		CLUSTER_INFO *pCluster =
			(CLUSTER_INFO *)m_pClusterList->GetGroupData(pos);

		if ( pCluster )
		{
			if(!pCluster->bRefused)
			{
				pCluster->radio = CLUSTER_INFO::NORECV;
				pCluster->bRefused = true;
				pCluster->nMsgNum = 0;
			}
			else
			{
				pCluster->radio = CLUSTER_INFO::RECV_SHOW;
				pCluster->bRefused = false;
				pCluster->nMsgNum = 0;
			}
		}
	}

	if( pos.nGroup == -1 && pos.nRoot != -1 )
	{
		BOOL bExpand = m_pClusterList->IsExpand(pos.nRoot);
		m_pClusterList->ExpandRoot(pos.nRoot, !bExpand);
	}
}