// im\TabPageCluster.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\floorpagecluster.h"
#include "p2pSendmsgDlg.h"
#include "MessageHandleWnd.h"
#include "..\FriendInfoDlg.h"
#include "ClusterCreateDlg.h"
#include "ClustersettingDlg.h"
#include "LocalData.h"
#include "xtpacket.h"
#include "DialogClusterAuth.h"
#include "DailogClusterCheck.h"
#include "FSDialog.h"
#include "MsgDlg.h"
#include "sysmsgdlg.h"
#include "webprocess.h"
#include "..\..\HmgSkin\HmgSkin\FlatPopupMenu.h"

// CFloorPageCluster 对话框
#define ID_CLUSTER_MODIFY		10001
#define ID_CLUSTER_SENDMSG      10002
#define ID_CLUSTER_SEARCH		10003
#define ID_CLUSTER_CREATE		10004
#define ID_CLUSTER_DISMISS		10005
#define ID_CLUSTER_MEMBER		10006
#define ID_CLUSTER_MSGHISTORY	10007
#define ID_CLUSTER_FRIENDINFO	10008
#define ID_FRIEND_SENDMSG		10009
#define ID_CLUSTER_QUIT         10010
#define ID_MEMBER_DEL           10011
#define ID_SEARCH               10012
#define ID_FRIEND_MSGHISTORY    10013
#define ID_ADD2FRIEND           10014
#define ID_CLUSTER_REFUSE       10015
#define ID_USER_BLOG            10016
#define ID_USER_ALBUM           10017
#define ID_CINFO_REFRESH        10018
#define ID_CMEMBER_LOADING      10019
#define ID_CLIST_REFRESH        10020
#define ID_USER_ARTICLE         10021
#define ID_CLUSTER_FORUM        10022
#define ID_CLUSTER_ALBUM        10023
#define ID_CLUSTER_HOME         10024

#define IDC_CLUSTER_LIST        0x05002

#define TIMER_SORTGROUP			0x01


IMPLEMENT_DYNAMIC(CFloorPageCluster, CFloorPage)
IMPLEMENT_HMG_SCROLLBAR(CFloorPageCluster)

#define DESTROY_ICON(icon) if(icon) { DestroyIcon(icon); icon = NULL; }

CFloorPageCluster::CFloorPageCluster(CWnd* pParent /*=NULL*/)
	: CFloorPage(CFloorPageCluster::IDD, pParent)
{
	m_pProgressLoad = NULL;
	m_bNeedSort = false; 
}

CFloorPageCluster::~CFloorPageCluster()
{
}

void CFloorPageCluster::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BUTTON_CREATE,m_btnCreate);
	DDX_Control(pDX,IDC_BUTTON_ADD,m_btnSearch);
}


BEGIN_MESSAGE_MAP(CFloorPageCluster, CFloorPage)
	ON_WM_SIZE()
	ON_MESSAGE(WM_IMOPERATION,OnImOperation)
	//ON_MESSAGE(WM_DISMISS_CLUSTER,OnDismissCluster)
	ON_MESSAGE(WM_CLUSTER_LIST_NOTIFY, OnGroupListNotify)
	ON_COMMAND(ID_CLUSTER_CREATE,OnClusterCreate)
	ON_COMMAND(IDC_BUTTON_CREATE,OnClusterCreate)
	ON_COMMAND(ID_CLUSTER_FRIENDINFO, OnFriendInfo)
	ON_COMMAND(ID_CLUSTER_SENDMSG, OnSendClusterMsg)
	ON_COMMAND(ID_FRIEND_SENDMSG, OnSendP2PMsg)
	ON_COMMAND(ID_CLUSTER_MODIFY, OnClusterInfo)
	ON_COMMAND(ID_CLUSTER_HOME,OnClusterHome)
	ON_COMMAND(ID_CLUSTER_FORUM,OnClusterForum)
	ON_COMMAND(ID_CLUSTER_ALBUM,OnClusterAlbum)
	ON_COMMAND(ID_CLUSTER_DISMISS, OnClusterDismiss)
	ON_COMMAND(ID_CLUSTER_QUIT,OnQuitCluster)
	ON_COMMAND(ID_MEMBER_DEL,OnMemberDel)
	ON_COMMAND(ID_CLUSTER_MSGHISTORY,OnClusterMsgHistory)
	ON_COMMAND(ID_FRIEND_MSGHISTORY,OnFriendMsgHistory)
	ON_COMMAND(ID_SEARCH,OnSearchCluster)
	ON_COMMAND(IDC_BUTTON_ADD,OnSearchCluster)
	ON_COMMAND(ID_ADD2FRIEND,OnAdd2Friend)
	ON_COMMAND(ID_CLUSTER_REFUSE,OnClusterRefuse)
	ON_COMMAND(ID_USER_BLOG,OnUserBlog)
	ON_COMMAND(ID_USER_ALBUM,OnUserAlbum)
	ON_COMMAND(ID_CINFO_REFRESH,OnCInfoRefresh)
	ON_COMMAND(ID_CMEMBER_LOADING,OnCMemberLoad)
	ON_COMMAND(ID_CLIST_REFRESH,OnCListRefresh)
	ON_MESSAGE(WM_HINTDLG_CLICK, OnHintDlgClick)
	ON_XT_MESSAGE()
	ON_WM_DESTROY()
	SET_HMG_SCROLLBAR(IDC_CLUSTER_LIST)
	ON_WM_ERASEBKGND()
	ON_WM_INITMENUPOPUP()
	ON_WM_MEASUREITEM()
	ON_MESSAGE(WM_FAMILY_HEAD_NOTIFY,OnFamilyIconNotify)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CFloorPageCluster::OnBnClickedButtonCreate)
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CFloorPageCluster)
	ON_XT_COMMAND(CMD_GET_CLIST_ACK,OnClusterListAck)
	// 群基本信息获取
	ON_XT_COMMAND(CMD_GET_CINFO_ACK,OnClusterInfoAck)
	// 获取群成员列表
	ON_XT_COMMAND(CMD_GET_CMEMBERS_ACK,OnClusterMembersAck)
	// 设置群成员
    ON_XT_COMMAND(CMD_SET_CMEMBERS_ACK,OnSetCMemberAck)
	// 好友状态改变通知
	ON_XT_COMMAND(CMD_FRIEND_STATUS_IND,OnCMemberStatusChange)
	// 收到群聊信息
	ON_XT_COMMAND(CMD_CLUSTERMSG_RECV,OnRecvClusterMsg)
	ON_XT_COMMAND(CMD_CLUSTER_APPLY_ACK,OnClusterApplyAck)
	// 收到离线群聊消息
	ON_XT_COMMAND(CMD_COFFLINE_MSG_ACK,OnRecvCofflineMsg)
	// 收到群验证消息
	ON_XT_COMMAND(CMD_CLUSTER_AUTH_IND,OnClusterAuthInd)
	// 群成员变动通知
	ON_XT_COMMAND(CMD_CMEMBERS_IND,OnCMemberInd)
	//邀请好友入群通知
	ON_XT_COMMAND(CMD_CLUSTER_INVITE_IND,OnClusterInviteInd);
	//好友是否入群通知
	ON_XT_COMMAND(CMD_CLUSTER_AGREE_IND,OnClusterAgreeInd);
	// 获取群成员列表请求
	ON_XT_COMMAND(CMD_GET_CMEMBERS_REQ,OnGetCMemberReq)
	// 群解散通知
	ON_XT_COMMAND(CMD_CDISMISS_IND,OnClusterDismissInd)
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoGetAck)
	ON_XT_COMMAND(CMD_STATUS_MOD_ACK,OnStatusModAck)
	ON_XT_COMMAND(CMD_STATUS_GET_ACK,OnStatusGetAck)
	ON_XT_COMMAND(CMD_SET_CINFO_ACK,OnSetClusterInfoAck)
	// 获取好友基本资料
	ON_XT_COMMAND(CMD_GET_FRIEND_BASICINFO_ACK,OnGetFriendBasicInfoAck)

	//获取群名片信息
	ON_XT_COMMAND(CMD_GET_PROFILE_NAME_ACK, OnGetCMProfileAck)
END_XT_MESSAGE_MAP()

// CFloorPageCluster 消息处理程序

BOOL CFloorPageCluster::OnInitDialog()
{
	__super::OnInitDialog();

	CRect rc(0,0,40,40);
	m_wndClusterList.Create(this,rc,WS_VISIBLE|WS_CHILD|WS_TABSTOP);
	m_wndClusterList.SetDlgCtrlID(IDC_CLUSTER_LIST);

	g_pProtocol->RegistCmd(CMD_GET_CLIST_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_CMEMBERS_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_CINFO_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SET_CMEMBERS_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_FRIEND_STATUS_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CLUSTERMSG_RECV,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CLUSTER_APPLY_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_COFFLINE_MSG_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CLUSTER_AUTH_IND, m_hWnd);
	g_pProtocol->RegistCmd(CMD_CMEMBERS_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CLUSTER_INVITE_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CLUSTER_AGREE_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_CMEMBERS_REQ,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CDISMISS_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_STATUS_MOD_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_STATUS_GET_ACK,m_hWnd);	
	g_pProtocol->RegistCmd(CMD_SET_CINFO_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_FRIEND_BASICINFO_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_PROFILE_NAME_ACK,m_hWnd);/////向服务器请求获取群名片

	AttachHMGScrollbar(m_wndClusterList.m_hWnd);
	/*
	m_btnCreate.SetBitmap(IDB_BITMAP_SYS_ADD_NORMAL,IDB_BITMAP_SYS_ADD_PRESSED);
	m_btnSearch.SetBitmap(IDB_BTN_SEARCH,IDB_BTN_SEARCH);
	*/

	this->EnableToolTips();

	return TRUE; 
}

void CFloorPageCluster::OnUserInfoGetAck(const char *data,WORD wRet)
{
	LPXT_USERINFO_GET_ACK ack=(LPXT_USERINFO_GET_ACK)data;

	if ( wRet==0 )
	{
		for ( int i=0; i<g_vecCluster.size(); i++ )
		{
			CLUSTER_INFO *pCluster = g_vecCluster[i];

			FRIEND_INFO *pFriend = pCluster->GetFriendInfo(ack->GetId());
			if ( pFriend )
			{
				int nCluster_Property=pFriend->cluster_property;
				//memcpy(pFriend,ack,sizeof(XT_USER_INFO));
				UpdateUserFullInfo(pFriend,ack);
				pFriend->cluster_property=nCluster_Property;

				pFriend->bRefreshed = true;

				BuildCluster(pCluster->nFamilyID);
			}
		}
	}
}


void CFloorPageCluster::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if ( m_wndClusterList.m_hWnd )
	{
		m_wndClusterList.MoveWindow(0,20, cx, cy);////0-->20

		if ( m_pProgressLoad )
		{
			m_pProgressLoad->CenterWindow(&m_wndClusterList);
		}
	}
}

//处理来自好友列表的鼠标右键
LRESULT CFloorPageCluster::OnGroupListNotify(WPARAM wParam, LPARAM lParam)
{
	CL_ITEM_POS pos=m_wndClusterList.GetCurSel();

	switch ( wParam )
	{
	// 群成员列表中双击
	case NM_CL_LBUTTONDBLCLICK:
		if ( g_ImUserInfo.status!=XTREAM_OFFLINE )
		{
			CL_ITEM_POS  * posItem = (CL_ITEM_POS *)lParam;

			if ( posItem->nItem>-1 )//群成员，发送p2p聊天消息哦
			{
				FRIEND_INFO * pFriend = (FRIEND_INFO *)m_wndClusterList.GetItemData(*posItem);

				if ( pFriend->GetId()==g_ImUserInfo.GetId() )
				{
					ShowBlogSite((char*)pFriend->GetUserName().c_str());
				}
				else
				{
					CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);

					if ( pDlg->m_hWnd==NULL )
					{
						pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
					}

					pDlg->ShowWindow(SW_SHOW);
					
					if ( pDlg->IsIconic() )
					{
						pDlg->ShowWindow(SW_RESTORE);
					}
					
					pDlg->BringWindowToTop();
				}
			}
			//群，发送群聊天消息哦
			else if ( posItem->nGroup>-1 )
			{						
				CL_GROUP_POS posGroup;
				posGroup.nRoot  = posItem->nRoot;
				posGroup.nGroup = posItem->nGroup;

				CLUSTER_INFO * pCluster = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(posGroup);

				if ( pCluster )
				{
					if ( pCluster->bMemberInit==false )
					{
						m_wndClusterList.AddLoadFlag(posGroup);
						// 在群聊天对话框中会获取群成员列表因此屏蔽
						//ImGetClusterMember(pCluster->nFamilyID);
					}

					CClusterSendMsgDlg *pDlg = g_pMessageHandleWnd->GetClusterSendMsgDlg(pCluster);
					
					if ( pDlg->m_hWnd==NULL )
					{
						pDlg->Create(CClusterSendMsgDlg::IDD,GetDesktopWindow());
					}

					pDlg->ShowWindow(SW_SHOW);

					if ( pDlg->IsIconic() )
					{
						pDlg->ShowWindow(SW_RESTORE);
					}
					
					pDlg->BringWindowToTop();	
				}
			}
		}
		break;	
	case NM_CL_EXPAND:
		if ( g_ImUserInfo.status!=XTREAM_OFFLINE )
		{
			CL_ITEM_POS * pos = (CL_ITEM_POS *)lParam;

			if ( pos->nRoot>-1 && pos->nGroup>-1 && pos->nItem==-1 )
			{
				CL_GROUP_POS posGroup;
				posGroup.nGroup = pos->nGroup;
				posGroup.nRoot  = pos->nRoot;

				CLUSTER_INFO *pCluster =(CLUSTER_INFO *) m_wndClusterList.GetGroupData(posGroup);
				// 如果该群成员没有初始化，则获取
				if ( pCluster && pCluster->bMemberInit==false )
				{
					// 显示加载群成员的字符动画
					m_wndClusterList.AddLoadFlag(posGroup);
					ImGetClusterMember(pCluster->nFamilyID);
				}
			}
		}
		break;
	case NM_CL_RBUTTONUP:
		{
			CMenuEx menu,*popmenu;
			menu.LoadMenu(IDR_MENU_TEMPLATE);
			popmenu = menu.GetSubMenu(0);
			popmenu->RemoveMenu(0,MF_BYPOSITION);

			CPoint point;
			GetCursorPos(&point);

			CL_ITEM_POS * posItem = (CL_ITEM_POS *)lParam;

			if ( posItem->nGroup>-1 )
			{
				CL_GROUP_POS posGroup;
				posGroup.nGroup = posItem->nGroup;
				posGroup.nRoot  = posItem->nRoot;

				CLUSTER_INFO *pCluster =(CLUSTER_INFO *) m_wndClusterList.GetGroupData(posGroup);

				int nType;//2，群主，1群管理员，0普通成员
				nType = pCluster->GetMemberType(g_ImUserInfo.GetId());

				CString str;

				if ( posItem->nItem>-1 ) //好友节点
				{
					FRIEND_INFO *pUser = (FRIEND_INFO *)m_wndClusterList.GetItemData(*posItem);

					if ( g_ImUserInfo.status!=XTREAM_OFFLINE && pUser->GetId()!=g_ImUserInfo.GetId() )
					{
						str.Format("和 \"%s\" 单独聊天",pUser->nickname);
						popmenu->AppendMenu(MF_STRING,ID_FRIEND_SENDMSG,str);

						popmenu->AppendMenu(MF_SEPARATOR,0);
					}

					str.Format("查看 \"%s\" 的详细资料",pUser->nickname);
					popmenu->AppendMenu(MF_STRING,ID_CLUSTER_FRIENDINFO,str);
				
					/*str.Format("查看 \"%s\" 的主页",pUser->nickname);
					popmenu->AppendMenu(MF_STRING,ID_USER_BLOG,str);

					str.Format("查看 \"%s\" 的图片",pUser->nickname);
					popmenu->AppendMenu(MF_STRING,ID_USER_ALBUM,str);*/

					if ( pUser->GetId()!=g_ImUserInfo.GetId() && FindFriend(pUser->GetId())==NULL )
					{
						str.Format("加 \"%s\" 为好友",pUser->nickname);
						popmenu->AppendMenu(MF_STRING,ID_ADD2FRIEND,str);
					}

					popmenu->AppendMenu(MF_SEPARATOR,0);

					popmenu->AppendMenu(MF_STRING,ID_CLUSTER_MSGHISTORY,"群聊天记录");
						
					if ( nType==2 || nType==1 && pUser->GetId()!=g_ImUserInfo.GetId() )
					{
						popmenu->AppendMenu(MF_SEPARATOR,0);

						str.Format("删除 %s",pUser->nickname);
						popmenu->AppendMenu(MF_STRING,ID_MEMBER_DEL,str);
					}

					popmenu->AppendMenu(MF_SEPARATOR,0);
					popmenu->AppendMenu(MF_STRING,ID_SEARCH,"搜索并加入群...");		

					if ( pCluster->bMemberInit || pCluster->Count() > 0 )
					{
						if ( nType==2 )//群主
						{
							popmenu->AppendMenu(MF_STRING,ID_CLUSTER_DISMISS,"解散群");
						}
						else
						{
							popmenu->AppendMenu(MF_STRING,ID_CLUSTER_QUIT,"退出群");
						}				
					}
		
					//popmenu->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this);
					CFlatPopupMenu flatmenu;
					flatmenu.Create(AfxGetInstanceHandle());
					flatmenu.AppendMenu(popmenu);
					UINT id = flatmenu.Track(point.x, point.y, GetSafeHwnd(), true);
					// 相应选择菜单的消息相应
					DWORD wP = MAKEWPARAM(id, 0);
					SendMessage(WM_COMMAND, wP, 0);
				}
				else//组节点
				{
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

						popmenu->AppendMenu(MF_SEPARATOR,0);

						////popmenu->AppendMenu(MF_STRING,ID_CINFO_REFRESH,"更新群信息");
						popmenu->AppendMenu(MF_STRING,ID_CLUSTER_REFUSE,pCluster->bRefused? "解除阻止..." :"阻止群消息");
						popmenu->AppendMenu(MF_SEPARATOR,0);
						popmenu->AppendMenu(MF_STRING,ID_CLUSTER_MSGHISTORY,"查看聊天记录");

						popmenu->AppendMenu(MF_SEPARATOR,0);
						////popmenu->AppendMenu(MF_STRING,ID_CLUSTER_CREATE,"创建群...");
						popmenu->AppendMenu(MF_STRING,ID_SEARCH,"搜索并加入群...");
						
						if ( pCluster->bMemberInit || pCluster->Count() > 0 )
						{
							popmenu->AppendMenu(MF_SEPARATOR,0);

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
                        
						popmenu->AppendMenu(MF_SEPARATOR,0);
						popmenu->AppendMenu(MF_STRING,ID_CLIST_REFRESH,"刷新群列表");

						//popmenu->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this);
						CFlatPopupMenu flatmenu;
						flatmenu.Create(AfxGetInstanceHandle());
						flatmenu.AppendMenu(popmenu);
						UINT id = flatmenu.Track(point.x, point.y, GetSafeHwnd(), true);
						// 相应选择菜单的消息相应
						DWORD wP = MAKEWPARAM(id, 0);
						SendMessage(WM_COMMAND, wP, 0);
					}
				}
			}
			else
			{
				if ( g_ImUserInfo.status!=XTREAM_OFFLINE )
				{
					popmenu->AppendMenu(MF_STRING,ID_CLIST_REFRESH,"刷新群列表");
					popmenu->AppendMenu(MF_SEPARATOR,0);
				    popmenu->AppendMenu(MF_STRING,ID_CLUSTER_CREATE,"创建群...");
					popmenu->AppendMenu(MF_STRING,ID_SEARCH,"搜索并加入群...");	
					//popmenu->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this);

					CFlatPopupMenu flatmenu;
					flatmenu.Create(AfxGetInstanceHandle());
					flatmenu.AppendMenu(popmenu);
					UINT id = flatmenu.Track(point.x, point.y, GetSafeHwnd(), true);
					// 相应选择菜单的消息相应
					DWORD wP = MAKEWPARAM(id, 0);
					SendMessage(WM_COMMAND, wP, 0);
				}
			}
		}
		break;
		case NM_CL_REFUSE_CLICK:
		{
			CL_GROUP_POS pos = (CL_GROUP_POS)this->m_wndClusterList.GetCurSel();
			if ( pos.nGroup!=-1 )
			{
				CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(pos);
				pCluster->bRefused = !pCluster->bRefused;
				g_LocalData.SaveClusterConfig((char*)g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());
				m_wndClusterList.Invalidate();
			}			
		}
		break;
		case NM_CL_SETTING_CLICK:
		{
			OnClusterInfo();		
		}
		break;
		case NM_CL_TITLE_CLICK:
		{
			OnSendClusterMsg();
		}
		break;
		case NM_CL_PARTY_CLICK:
		{
			CL_GROUP_POS pos = (CL_GROUP_POS)m_wndClusterList.GetCurSel();
			if ( pos.nGroup!=-1 )
			{
				CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(pos);
				
				if(g_pWebConfig)
				{
					CString sUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(FAMILY_DISCUSS,StrToWideStr(pCluster->szFamilyDomain));

					RegUserOperation(0x0032);
					MiniOpenUrl(sUrl);
				}

			}	
		}
		break;
		case NM_CL_ALBUM_CLICK:
		{
			CL_GROUP_POS pos = (CL_GROUP_POS)m_wndClusterList.GetCurSel();
			if ( pos.nGroup!=-1 )
			{
				CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(pos);
				if(g_pWebConfig)
				{
					CString sUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(FAMILY_ALBUM,StrToWideStr(pCluster->szFamilyDomain));

					RegUserOperation(0x0032);
					MiniOpenUrl(sUrl);
				}

			}	
		}
		break;
		case NM_CL_HOME_CLICK:
		{
			CL_GROUP_INFO *p   = (CL_GROUP_INFO *)lParam   ;
			CLUSTER_INFO *pClu = (CLUSTER_INFO *)p->dwData ;

			ShowBlogFamilySite(pClu->szFamilyDomain) ;
			break;
		}
	}
	return 0;
}

LRESULT CFloorPageCluster::OnFamilyIconNotify(WPARAM wParam, LPARAM lParam)
{
	if (lParam)
	{
		CLUSTER_INFO * pCluster = FindClusterInfo(wParam);

		if ( pCluster )
		{
			UpdateClusterBitmap(pCluster);
			m_wndClusterList.UpdateGroupImage(m_wndClusterList.FindCluster(pCluster->nFamilyID));
		}
	}

	return 0;
}

void CFloorPageCluster::SortGroupList()
{
	int nRoot = m_wndClusterList.GetRootCount();

	uint32 nSelFamilyID = (uint32)-1;
	uint32 nSelUserID   = (uint32)-1;

	CL_ITEM_POS posSel = m_wndClusterList.GetCurSel();

	if ( posSel.nGroup!=-1 )
	{
		CL_GROUP_POS posGroup;
		posGroup.nGroup = posSel.nGroup;
		posGroup.nRoot  = posSel.nRoot;

		CLUSTER_INFO * pSelCulster = (CLUSTER_INFO *) m_wndClusterList.GetGroupData(posGroup);
		nSelFamilyID = pSelCulster->nFamilyID;

		if ( posSel.nItem!=-1 )
		{
			FRIEND_INFO *pSelUser = (FRIEND_INFO *)m_wndClusterList.GetItemData(posSel);
			nSelUserID=pSelUser->GetId();
		}
	}

	for ( int n=0; n<nRoot; n++ )
	{
		int nGroups = m_wndClusterList.GetGroupCount(n);

		int nMyGroupCount = 0;
		int i, j;

		CL_GROUP_POS posGroupI;
		posGroupI.nRoot = n;
	
		CL_GROUP_POS posGroupJ;
		posGroupJ.nRoot = n;

		for ( i=0; i<nGroups-1; i++ )
		{
			posGroupI.nGroup  = i;
			CL_GROUP_INFO *c1 = (CL_GROUP_INFO *)m_wndClusterList.GetGroupInfo(posGroupI);

			for ( j=i+1; j<nGroups; j++ )
			{
				posGroupJ.nGroup  = j;
				CL_GROUP_INFO *c2 = (CL_GROUP_INFO *)m_wndClusterList.GetGroupInfo(posGroupJ);

				if ( c1->bFlash!=c2->bFlash )
				{
					if ( c2->bFlash )
					{
						m_wndClusterList.SwapGroup(posGroupI,posGroupJ);
						c1 = c2;
					}
				}
			}
		}

		int nFlashGroups = 0;

		for ( i=0; i<nGroups-1; i++ )
		{
			posGroupI.nGroup  = i;

			CL_GROUP_INFO *c1 = (CL_GROUP_INFO *)m_wndClusterList.GetGroupInfo(posGroupI);
			if ( c1->bFlash )
				nFlashGroups++;
		}

		//sort other groups
		for ( i=nFlashGroups; i<nGroups-1; i++ )
		{
			posGroupI.nGroup = i;
			CLUSTER_INFO *c1 = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(posGroupI);

			for ( j=i+1; j<nGroups; j++ )
			{
				posGroupJ.nGroup  = j;
				CLUSTER_INFO * c2 = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(posGroupJ);

				if ( strcmp(c1->szFamilyName, c2->szFamilyName)>0 )
				{
					m_wndClusterList.SwapGroup(posGroupI,posGroupJ);
					c1 = c2;
				}
			}
		}
	}
	//m_wndClusterList.SetCurSel(posSel.nGroup,posSel.nItem);
	SelectUserOnList(nSelFamilyID,nSelUserID);
	
}

void CFloorPageCluster::SortClusterList(uint32 cid)
{
	KillTimer(cid);
	SetTimer(cid,200,NULL);
	m_bNeedSort = true;
}

void CFloorPageCluster::SortClusterListReal(uint32 cid)
{
	CLUSTER_INFO *pCluster = FindClusterInfo(cid);

	if ( pCluster==NULL )
	{
		return ;
	}

	CL_GROUP_POS posGroup = m_wndClusterList.FindCluster(cid);

	if ( posGroup.IsNull()==false )
	{
		int nItemCount = m_wndClusterList.GetItemCount(posGroup);

		m_wndClusterList.SetInvalidateLock(true);

		int nOnline=0;

		CL_ITEM_POS posI, posJ;

		posI.nRoot = posGroup.nRoot;
		posI.nGroup = posGroup.nGroup;

		posJ = posI;

		int i;

		for ( i=0; i<nItemCount-1; i++ )
		{
			posI.nItem = i;

			FRIEND_INFO * f1 =(FRIEND_INFO *)m_wndClusterList.GetItemData(posI);

			for ( int j=i+1;j<nItemCount;j++ )
			{
				posJ.nItem = j;

				FRIEND_INFO * f2 =(FRIEND_INFO *)m_wndClusterList.GetItemData(posJ);

				if ( f1->status>f2->status )
				{	
					m_wndClusterList.SwapItem(posI,posJ);
					f1=f2;
				}
			}
		}

		for ( i=0; i<nItemCount; i++ )
		{
			posI.nItem = i;

			FRIEND_INFO * f1 =(FRIEND_INFO *)m_wndClusterList.GetItemData(posI);
			if ( f1->status!=XTREAM_OFFLINE )
				nOnline++;
		}

		QuickSort(posGroup.nRoot,posGroup.nGroup,0,nOnline-1);
		QuickSort(posGroup.nRoot,posGroup.nGroup,nOnline,nItemCount-1);

		m_wndClusterList.SetInvalidateLock(false);

		KillTimer(cid);
	}
}

void CFloorPageCluster::QuickSort(int nRoot, int nGroup, int iLo, int iHi)
{
	if ( iLo<iHi )
	{
		char szKey[255];

		CL_ITEM_POS posLo;
		posLo.nRoot  = nRoot;
		posLo.nGroup = nGroup;
		posLo.nItem  = iLo;

		CL_ITEM_POS posHi;
		posHi.nRoot  = nRoot;
		posHi.nGroup = nGroup;
		posHi.nItem  = iHi;

		strncpy(szKey,((FRIEND_INFO *)m_wndClusterList.GetItemData(posLo))->nickname,255-1);

		while (posLo.nItem<posHi.nItem)
		{
			FRIEND_INFO *fhi = (FRIEND_INFO *)m_wndClusterList.GetItemData(posHi);

			while ( posLo.nItem<posHi.nItem && strcmp(fhi->nickname,szKey)>=0 )
			{
				posHi.nItem--;
				fhi = (FRIEND_INFO *)m_wndClusterList.GetItemData(posHi);
			}

			m_wndClusterList.SwapItem(posLo,posHi);

			FRIEND_INFO *flo = (FRIEND_INFO *)m_wndClusterList.GetItemData(posLo);

			while ( posLo.nItem<posHi.nItem && strcmp(flo->nickname,szKey)<0)
			{
				posLo.nItem++;
				flo = (FRIEND_INFO *)m_wndClusterList.GetItemData(posLo);
			}

			m_wndClusterList.SwapItem(posLo,posHi);
		}

		QuickSort(nRoot,nGroup,iLo,posLo.nItem-1);
		QuickSort(nRoot,nGroup,posLo.nItem+1,iHi);
	}
}

void CFloorPageCluster::BuildCluster(uint32 cid)
{
	CLUSTER_INFO *pCluster = FindClusterInfo(cid);

	if ( pCluster )
	{
		m_wndClusterList.SetInvalidateLock(true);

		CL_GROUP_POS posGroup;
		posGroup = m_wndClusterList.FindCluster(cid);

		//int nGroup=-1;
		//int i;
		//for ( i = 0; i < m_wndClusterList.GetGroupCount(); i++)
		//{
		//	CLUSTER_INFO *pData = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(i);

		//	if (pData && pData->nFamilyID==pCluster->nFamilyID )
		//	{
		//		nGroup=i;
		//		break;
		//	}
		//}

		FRIEND_INFO *pCreator = pCluster->GetCreator();

		if ( posGroup.IsNull()==false )
		{
			m_wndClusterList.ClearGroup(posGroup);
			m_wndClusterList.SetGroupName(posGroup,pCluster->szFamilyName);
		}
		else
		{
			//区分"我创建的群"和"我加入的群"
			if ( pCreator && pCreator->GetId() == g_ImUserInfo.GetId() )
			{
				posGroup=m_wndClusterList.AddGroup(0,pCluster->szFamilyName);				
				m_wndClusterList.SetGroupData(posGroup,(DWORD)pCluster);
			}
			else
			{
				posGroup=m_wndClusterList.AddGroup(1,pCluster->szFamilyName);				
				m_wndClusterList.SetGroupData(posGroup,(DWORD)pCluster);
			}
		}

		
		
		size_t ii,c;
		c = pCluster->Count();
		for ( ii = 0; ii < c; ii++ )
		{
			FRIEND_INFO *pMember = &pCluster->members[ii];

			if ( pMember->GetId()==g_ImUserInfo.GetId() && stricmp(g_ImUserInfo.nickname,"")!=0 )
			{
				// 群片名保存不变
				string strProfileName = pMember->profile_name;
				memcpy(pMember,&g_ImUserInfo,sizeof(XT_USER_INFO));
				strcpy(pMember->profile_name, strProfileName.c_str());
			}
			FRIEND_INFO* pGroupFriend = GetFriendInfo(pMember->GetId());
			if(pGroupFriend != NULL && pGroupFriend->comment_name != "")
			{
				strncpy(pMember->comment_name,pGroupFriend->comment_name,sizeof(pGroupFriend->comment_name)-1);
			}
			m_wndClusterList.AddItem(posGroup,/*UserInfoToStr*/CMemberInfoToStr(pMember),(DWORD)pMember);
		}

		SortClusterList(cid);

		m_wndClusterList.SetInvalidateLock(false);
	}

}

void CFloorPageCluster::OnSendClusterMsg()
{
	CL_GROUP_POS pos = (CL_GROUP_POS)m_wndClusterList.GetCurSel();

	if ( pos.nGroup!=-1 )
	{
		CLUSTER_INFO *pCluster =
			(CLUSTER_INFO *)m_wndClusterList.GetGroupData(pos);

		if ( pCluster )
		{
			CClusterSendMsgDlg *pDlg = g_pMessageHandleWnd->GetClusterSendMsgDlg(pCluster);

			if ( pDlg->m_hWnd==NULL )
				pDlg->Create(CClusterSendMsgDlg::IDD,GetDesktopWindow());

			pDlg->ShowWindow(SW_SHOW);

			if ( pDlg->IsIconic() )
				pDlg->ShowWindow(SW_RESTORE);

			pDlg->BringWindowToTop();
		}
	}
}

// 开始和族内成员聊天
void CFloorPageCluster::OnSendP2PMsg()
{
	// 获得当前被选择的成员
	CL_ITEM_POS posItem = m_wndClusterList.GetCurSel();
	FRIEND_INFO *pFriend = (FRIEND_INFO *)m_wndClusterList.GetItemData(posItem);
	if(pFriend==NULL)
		return;
	if ( pFriend->GetId()!=g_ImUserInfo.GetId() )
	{
		// 如果不是自己则弹出聊天对话框
        CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
	
		if ( pDlg->m_hWnd==NULL )
			pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());

		pDlg->ShowWindow(SW_SHOW);

		if ( pDlg->IsIconic() )
			pDlg->ShowWindow(SW_RESTORE);

		pDlg->BringWindowToTop();
	}
	else
	{
		// 如果是自己则转到自己的博客主页
		ShowBlogSite((char*)pFriend->GetUserName().c_str());
	}
}

void CFloorPageCluster::SelectUserOnList(uint32 cid, uint32 uid)
{
	CL_GROUP_POS posGroup = m_wndClusterList.FindCluster(cid);

	if ( posGroup.IsNull()==false )
	{		
		int nItems   = m_wndClusterList.GetItemCount(posGroup);
		int nSelUser = -1;

		CL_ITEM_POS posItem;
		posItem.nRoot = posGroup.nRoot;
		posItem.nGroup = posGroup.nGroup;

		for ( int f=0; f<nItems; f++ )
		{		
			posItem.nItem = f;	
			FRIEND_INFO *pUser = (FRIEND_INFO *)m_wndClusterList.GetItemData(posItem);
			if ( pUser->GetId()==uid )
			{		
				m_wndClusterList.SetCurSel(posItem);
				break;
			}
		}
	}

}

void CFloorPageCluster::SetClusterFlashFlag(uint32 cid, bool bFlash )
{
	CL_GROUP_POS posGroup = m_wndClusterList.FindCluster(cid);

	if ( posGroup.IsNull()==false )
	{
		if ( m_wndClusterList.GetGroupInfo(posGroup)->bFlash!=bFlash )
		{
			if (bFlash)
				m_wndClusterList.AddFlashFlag(posGroup);
			else
				m_wndClusterList.RemoveFlashFlag(posGroup);

			SortGroupList();
		}
	}
}

void CFloorPageCluster::OnSearchCluster()
{
	tr1::shared_ptr<CAddFriendOrClusterDlg> pDlg = CAddFriendOrClusterDlg::Show();
	pDlg->SetSearchType(FS_CLUSTER);
}

void CFloorPageCluster::OnClusterCreate()
{
	CCreateClusterDlg::Show();
}


void CFloorPageCluster::OnFriendMsgHistory()
{
	CL_ITEM_POS posItem = m_wndClusterList.GetCurSel();
	FRIEND_INFO *pInfo  = (FRIEND_INFO *)m_wndClusterList.GetItemData(posItem);

	if ( pInfo )
	{
		CMsgDlg * pDlg = CMsgDlg::GetInstance();
		pDlg->BringWindowToTop();

		pDlg->SetDefaultFriend(pInfo->GetId());
	}
}
// 弹出群聊天记录对话框
void CFloorPageCluster::OnClusterMsgHistory()
{  
	CL_GROUP_POS pos=(CL_GROUP_POS)m_wndClusterList.GetCurSel();

	if ( pos.IsNull()==false )
	{
		CLUSTER_INFO *pInfo = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(pos);

		if ( pInfo )
		{
			CMsgDlg * pDlg = CMsgDlg::GetInstance();
			pDlg->BringWindowToTop();

			pDlg->SetDefaultCluster(pInfo->nFamilyID);
		}
	}
}

void CFloorPageCluster::OnFriendInfo()
{
	CL_ITEM_POS pos    = m_wndClusterList.GetCurSel();

	if ( pos.IsNull()==false )
	{
		FRIEND_INFO *pInfo = (FRIEND_INFO *)m_wndClusterList.GetItemData(pos);

		if ( pInfo )
		{
			/*CFriendInfoDlg  *pDlg = */CFriendInfoDlg::Show(pInfo);
		}
	}
}

void CFloorPageCluster::OnClusterInfo()
{
	CL_GROUP_POS pos = (CL_GROUP_POS)m_wndClusterList.GetCurSel();

	if ( pos.IsNull()==false )
	{
		CLUSTER_INFO *pInfo = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(pos);

		if ( pInfo )
		{
			CClusterSettingDlg::Show(pInfo);
		}
	}
}

void CFloorPageCluster::OnClusterAuthInd(const char *data,WORD wRet)
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
			//::MessageBox(GetDesktopWindow()->m_hWnd,str,IM_CAPTION,MB_OK|MB_ICONINFORMATION);
			CMessageBox::Prompt(str);
		}
	}
}

void CFloorPageCluster::OnClusterDismissInd(const char *data,WORD wRet)
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

void CFloorPageCluster::OnGetFriendBasicInfoAck(const char *data,WORD wRet)
{
	LPXT_GET_FRIEND_BASICINFO_NOTIFY pAck = (LPXT_GET_FRIEND_BASICINFO_NOTIFY)data;
	int i,j;
	CLUSTER_INFO * pCluster = NULL;
	FRIEND_INFO *pFriend = NULL;
	bool bNeedUpdate = false;

	if(wRet != 0)
	{
		return;
	}
	for( i = 0;i < g_vecCluster.size() ;i++ )
	{
		bNeedUpdate = false;
		pCluster = g_vecCluster[i];
		for ( j = 0; j < pAck->count; j++ )
		{
			pFriend = pCluster->GetFriendInfo(pAck->FriendInfo[j].nID);
			if ( pFriend )
			{
				bNeedUpdate = true;
				pFriend->bBasicInfo = true;
				pFriend->sex = pAck->FriendInfo[j].nSexy;
				strcpy(pFriend->face_url,pAck->FriendInfo[j].szHeadUrl);
				pFriend->SetUserName(pAck->FriendInfo[j].szUserName);
				strcpy(pFriend->nickname,pAck->FriendInfo[j].szNickName);
				strcpy(pFriend->comment_name,pAck->FriendInfo[j].szCommentName);
				pFriend->SetGuid(pAck->FriendInfo[j].szGUID);//新增自营GUID标识
			}
		}
		if(bNeedUpdate)
		{
			BuildCluster(pCluster->nFamilyID);
			CClusterSendMsgDlg *pDlg = g_pMessageHandleWnd->FindClusterSendMsgDlg(pCluster->nFamilyID);
			if( pDlg )
			{
				pDlg->UpdateMemberList(pCluster);
			}
		}
		
	}
	
}

void CFloorPageCluster::OnGetCMemberReq(const char *data,WORD wRet)
{
	XT_GET_CMEMBERS_REQ * req = (XT_GET_CMEMBERS_REQ *)data;

	if ( req->nFamilyProp==PERM_FAMILY )//添加输出群成员的时候,刷新群列表
	{
		CLUSTER_INFO *pCluster = FindClusterInfo(req->nFamilyID );
		//?
		if ( pCluster && req->start_pos==0 )
		{
			map<uint32,tagPos *>::iterator it = m_mapPos.find(pCluster->nFamilyID);

			if (it!=m_mapPos.end())
			{
				it->second->vecPos.clear();
				delete it->second;

				m_mapPos.erase(it);
			}
		}
	}
}

// 请求加入族的返回数据处理
void CFloorPageCluster::OnClusterApplyAck(const char *data,WORD wRet)
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
void CFloorPageCluster::OnClusterListAck(const char *data,WORD wRet)
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
		g_LocalData.LoadClusterConfig((char*)g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());

		g_LocalData.LoadClusterExitConfig((char*)g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());
		//加载最近联系人名单
		((CRecentListCtrl *)g_pRecentListCtrl)->Load();
		((CRecentListCtrl *)g_pRecentListCtrl)->UpdateUserList();

		//向IM服务器获取历史聊天记录
		ImGetOfflineMsg();
	}
	else
	{
		//加载每个群的设置信息(是否被阻止,是否自动播放魔法表情...)
		g_LocalData.LoadClusterConfig((char*)g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());
		g_LocalData.LoadClusterExitConfig((char*)g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());
		ShowMessage("获取群列表失败，请重试。");
	}

	if ( m_pProgressLoad )
	{
		m_pProgressLoad->DestroyWindow();
		delete m_pProgressLoad;
		m_pProgressLoad=NULL;
	}
}

void CFloorPageCluster::ImGetClusterList()
{
	XT_GET_CLIST_REQ req;

	req.nFamilyProp = PERM_FAMILY;
	req.nCount=0;

	g_pProtocol->SendReq(req);
}

void CFloorPageCluster::ImGetClusterInfo(uint32 cid)
{
	XT_GET_CINFO_REQ req;

	req.nFamilyID   = cid;
	req.nFamilyProp = PERM_FAMILY;

	g_pProtocol->SendReq(req);
}

//
//void CFloorPageCluster::ImGetClusterMemberBasicInfo(uint32 cid, bool bAll/* =true  */)
//{
//	CLUSTER_INFO *pCluster = FindClusterInfo(cid);
//
//	if ( pCluster )
//	{
//		char buff[MAX_SEND_BUFFERS];
//		LPXT_GET_FRIEND_BASICINFO_REQ req = (LPXT_GET_FRIEND_BASICINFO_REQ)buff;
//
//		int  n = 0;
//		int  i = 0;
//		const int maxids = 200;
//
//		while ( i<pCluster->count )
//		{
//			i++;
//
//			if ( n<maxids )
//			{
//				if  (bAll )
//				{
//					req->FriendID[n]=pCluster->members[i-1].id;
//					n++;
//				}
//				else
//				if ( pCluster->members[i-1].status!=XTREAM_OFFLINE )
//				{
//					req->FriendID[n]=pCluster->members[i-1].id;
//					n++;
//				}
//
//				if ( i==pCluster->count )
//				{
//					req->count = n;
//					g_pProtocol->SendReq(*req);
//				}
//			}
//			else
//			{
//				i--;
//				req->count=maxids;
//				g_pProtocol->SendReq(*req);
//				n = 0;
//			}
//		}
//	}
//}

void CFloorPageCluster::ImGetFriendBasicInfo(const vector<uint32> & arrFriend) 
{
	const int maxids = 300;
	char buff[MAX_SEND_BUFFERS];
	LPXT_GET_FRIEND_BASICINFO_REQ req = (LPXT_GET_FRIEND_BASICINFO_REQ)buff;
	req->type = XT_GET_FRIEND_BASICINFO_REQ::TYPE_OTHER;
	size_t i = 0;
	size_t n = 0;
	size_t c = arrFriend.size();

	while ( i< c )
	{
		if ( n < maxids )
		{
			req->FriendID[n] = arrFriend[i];
			n++;
			if( i >= c - 1 )
			{
				// id列表已经填写完毕
				req->count = n;
				g_pProtocol->SendReq(*req);
				break;
			}
		}
		else
		{
			req->FriendID[n] = arrFriend[i];
			req->count = n;
			g_pProtocol->SendReq(*req);
			n = 0;
		}
		i++;
	}
}


void CFloorPageCluster::ImGetOfflineMsg()
{
	XT_OFFLINEMSG_REQ req;

	req.id = g_ImUserInfo.GetId();
	g_pProtocol->SendReq(req);
}

void CFloorPageCluster::SetClusterOfflineFlag()
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


void CFloorPageCluster::ImGetClusterMember(uint32 cid)
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
		ClearMembersPos(cid);
	}

	////发送群名片获取请求
	XT_PROFILENAME_GET_REQ reqMem;
	reqMem.nFamilyID   =cid;
	g_pProtocol->SendReq(reqMem);
}

// 处理接收的群信息
void CFloorPageCluster::OnClusterInfoAck(const char *data,WORD wRet)
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

			g_webFamilyHeadIconProcess.GetFamilyHeadIcon(pCluster,this);
		
			BuildCluster(pCluster->nFamilyID);
		}//end if ( pCluster )

		KillTimer(TIMER_SORTGROUP);
		SetTimer(TIMER_SORTGROUP,300,NULL);
	}
	else if(wRet == SERVER_TIMEOUT)
	{
	}
}

void CFloorPageCluster::BuildClusterList()
{
	m_wndClusterList.DeleteAllItems();
	//不区分"我创建的群"和"我加入的群"
	m_wndClusterList.AddRoot("我创建的群");	
	m_wndClusterList.AddRoot("我加入的群");
	//m_wndClusterList.AddRoot("群 组");

	for ( int i=0; i<g_vecCluster.size(); i++ )
	{
		BuildCluster(g_vecCluster[i]->nFamilyID);
		MakeClusterBitmap(g_vecCluster[i]);
	}	

	for ( int r=0; r<m_wndClusterList.GetRootCount(); r++ )
	{
		for ( int g=0; g<m_wndClusterList.GetGroupCount(r); g++ )
		{
			CL_GROUP_POS posGroup;
			posGroup.nRoot  = r;
			posGroup.nGroup = g;
			m_wndClusterList.UpdateGroupImage(posGroup);
		}
	}
}

// 处理群成员状态改变事件
void CFloorPageCluster::OnCMemberStatusChange(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_STATUS_IND ind =(LPXT_STATUS_IND)data;

		if ( ind->status==XTREAM_HIDE )
		{
			ind->status=XTREAM_OFFLINE;
		}

		for ( int i=0; i<g_vecCluster.size(); i++ )
		{
			FRIEND_INFO *pFriend = g_vecCluster[i]->GetFriendInfo(ind->fid);

			//如果是隐身的话，则状态图标和离开一样
			if ( pFriend )
			{
				pFriend->ver  = ind->version;
				pFriend->status = ind->status;	

				SortClusterList(g_vecCluster[i]->nFamilyID);
			}
		}
	}
}

void CFloorPageCluster::DelClusterMember(uint32 cid, uint32 mid)
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

void CFloorPageCluster::DelCluster(uint32 cid)
{
	int i;
	
	CL_GROUP_POS posGroup = m_wndClusterList.FindCluster(cid);

	if ( posGroup.IsNull()==false )
	{
		m_wndClusterList.DelGroup(posGroup);

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
void CFloorPageCluster::OnCMemberInd(const char *data, WORD wRet)
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
void CFloorPageCluster::OnClusterInviteInd(const char *data, WORD wRet)
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
void CFloorPageCluster::OnClusterAgreeInd(const char *data, WORD wRet)
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

// 处理获得族成员列表数据
void CFloorPageCluster::OnClusterMembersAck(const char *data,WORD wRet)
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
			map<uint32,tagPos *>::iterator it = m_mapPos.find(pCluster->nFamilyID);
			tagPos *pPos ;

			if ( it==m_mapPos.end() )
			{
				pPos = new tagPos();
				pPos->vecPos.push_back(ack->next_pos);
				m_mapPos.insert(map<uint32,tagPos*>::value_type(pCluster->nFamilyID,pPos));
			}
			else
			{
				pPos = it->second;
				for ( int i = 0; i<pPos->vecPos.size(); i++ )
				{
					if ( pPos->vecPos[i] == ack->next_pos )
					{
						return ;
					}
				}

				pPos->vecPos.push_back(ack->next_pos);
			}
			
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
				ImGetFriendBasicInfo(m_arrNeedGetBasicInfo);
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

				CL_GROUP_POS posGroup = m_wndClusterList.FindCluster(ack->nFamilyID);

                if ( posGroup.IsNull()==false )
					m_wndClusterList.RemoveLoadFlag(posGroup);

				KillTimer(TIMER_SORTGROUP);
				SetTimer(TIMER_SORTGROUP,300,NULL);
			}
		}
	}
	else
	{
//		ShowMessage("获取群成员数据失败");
		int n = m_wndClusterList.GetRootCount();

		for ( int i=0; i<n; i++ )
		{
			for ( int g=0; g<m_wndClusterList.GetGroupCount(i); g++ )
			{
				CL_GROUP_POS posGroup;

				posGroup.nGroup = g;
				posGroup.nRoot  = i;

				CLUSTER_INFO *p=(CLUSTER_INFO *)m_wndClusterList.GetGroupData(posGroup);
				m_wndClusterList.RemoveLoadFlag(posGroup);
			}
		}
	}
}

void CFloorPageCluster::OnRecvCofflineMsg(const char *data,WORD wRet)
{
	///*if ( wRet==0 )
	//{
	//	uint16 next_pos     ;
	//	uint8  ret_count    ;
	//	uint32 nFamilyID   ;
	//	uint8  nFamilyProp ;
	//	uint8  message_type ;

	//	CRecvPacket rp((char *)data,MAX_RECV_BUFFERS);

	//	rp>>next_pos>>ret_count>>nFamilyID>>nFamilyProp>>message_type;

	//	CLUSTER_INFO * pCluster = FindClusterInfo(nFamilyID);

	//	if ( pCluster )
	//	{
	//		CClusterSendMsgDlg *pDlg = g_pMessageHandleWnd->GetClusterSendMsgDlg(pCluster);

	//		if ( pDlg->IsWindowVisible()==false )
	//		{
	//			pDlg->ShowWindow(SW_SHOW);
	//			pDlg->BringWindowToTop();
	//			pDlg->FlashWindow(TRUE);
	//		}

	//		for ( int i=0; i<ret_count; i++ )
	//		{
	//			XT_MSG msg;

	//			rp >>msg.nFamilyID>>msg.to_id>>msg.ver
	//				>>msg.nFamilyProp>>msg.send_time
	//				>>msg.fontName>>msg.fontStyle
	//				>>msg.fontColor>>msg.fontSize
	//				>>msg.message;

	//			pDlg->AddMsg(msg,false);			
	//			g_LocalData.AddMsg(g_ImUserInfo.GetId(),msg);
	//		}

	//		if ( next_pos!=0xFFFF )
	//		{
	//			XT_COFFLINE_MSG_REQ req;
	//			req.nFamilyID = nFamilyID;
	//			req.count=0;
	//			req.uid=g_ImUserInfo.GetId();
	//			req.start_pos=next_pos;
	//			g_pProtocol->SendReq(req);
	//		}
	//	}
	//}*/
	
}

void CFloorPageCluster::OnRecvClusterMsg(const char *data,WORD wRet)
{
	LPXT_MSG msg = (LPXT_MSG)data;

	if ( msg->cluster_type==PERM_FAMILY )//不是固定群，丢掉
	{
		if ( msg->from_id==g_ImUserInfo.GetId() )
		{
			return ;
		}

		CLUSTER_INFO * pCluster = FindClusterInfo(msg->cluster_id);

		if ( pCluster )
		{
			if ( pCluster->bMemberInit==false )
			{
			}

			vector<DOWNLOAD_OBJ> arrDownloadTask;
			CString sShowMsg;
			if ( pCluster->bRefused==false )////接收并显示群消息
			{
				bool bFind = g_pMessageHandleWnd->FindClusterSendMsgDlg(pCluster->nFamilyID);

				CClusterSendMsgDlg *pDlg = g_pMessageHandleWnd->GetClusterSendMsgDlg(pCluster);
				if(TranslateMsg(msg->data,sShowMsg,arrDownloadTask))
				{
					// 翻译出应该显示的消息
					strncpy(msg->data,sShowMsg,sizeof(msg->data)-1);
					// 开始下载任务
					if(pDlg)// && pDlg->m_hWnd != NULL)
					{
						pDlg->Download(arrDownloadTask);
					}
				}
				pDlg->ProcessMsg(*msg);
				FRIEND_INFO *pUser = NULL;

				switch ( msg->data_type )
				{
				case 0:
					if ( pDlg->m_hWnd==NULL )
					{
						g_pMessageHandleWnd->OnRecvClusterMsg(pCluster);	
						SetClusterFlashFlag(pCluster->nFamilyID,true);
					}
					else
					{						
						g_LocalData.SaveTalkMsg(*msg);
						pDlg->FlashWindow(FALSE);
					}

					msg->cluster_type = PERM_FAMILY;
					msg->msg_type     = 1;//群聊消息

					((CRecentListCtrl *)g_pRecentListCtrl)->AddCluster(pCluster);
					break;
					
				case 9://魔法表情
					{
						CRecvPacket rp((char *)msg->data,sizeof(msg->data));
						char   face_name[255];
						char   face_url[255];

						rp>>face_name>>face_url;

						if ( pDlg->m_hWnd == NULL || pDlg->IsWindowVisible()==false )
						{	
							::g_pMessageHandleWnd->OnRecvClusterMsg(pCluster);					
						}
						else
						{
							pDlg->FlashWindow(TRUE);
						}

						if ( pDlg->RecvMagicFace(msg->from_id, face_name,face_url) )
						{
							if ( pDlg->m_hWnd )
							{
								if ( pCluster->bAutoShowMagic )
									pDlg->ShowMagicFace(face_name);

								if ( pDlg->IsIconic() )
									pDlg->ShowWindow(SW_RESTORE);

								pDlg->BringWindowToTop();
							}
						}
					}
					break;
				default:break;		
				}//end switch
			}//end if ( pCluster->bRefused )
			else
			{
				if(msg->data_type == 0)
				{
					if(pCluster->radio == CLUSTER_INFO::RECV_NOSHOW
						|| pCluster->radio == CLUSTER_INFO::SHOW_NUM)
					{
						if(TranslateMsg(msg->data,sShowMsg,arrDownloadTask))
						{
							// 翻译出应该显示的消息
							strncpy(msg->data,sShowMsg,sizeof(msg->data)-1);
						}
						g_LocalData.SaveTalkMsg(*msg);

						if(pCluster->radio == CLUSTER_INFO::SHOW_NUM)
						{	// 对应群栏目显示数字
							pCluster->nMsgNum++;
							m_wndClusterList.SetInvalidateLock(false);
						}
					}
					else if(pCluster->radio == CLUSTER_INFO::NORECV)/////拒绝接收
					{
						;//不显示消息
					}
				}
			}
		}//end if ( pCluster)
	}
}

void CFloorPageCluster::OnQuitCluster()
{
	CL_ITEM_POS pos = m_wndClusterList.GetCurSel();

	if ( pos.nGroup==-1 )
	{
		return ;
	}

	CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_wndClusterList.GetGroupData((CL_GROUP_POS)pos);

	if ( pCluster )
	{
		CString str;
		str.Format("确定要退出该群吗？");

		if (MessageBox(str,IM_CAPTION,MB_YESNO|MB_ICONWARNING)!=IDYES)
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

		m_wndClusterList.DelGroup((CL_GROUP_POS)pos);

		//退出群当前选中的群则把选中位置初始化
		m_wndClusterList.ClearCurSel();
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

void CFloorPageCluster::OnClusterAlbum()
{
	CL_GROUP_POS pos = (CL_GROUP_POS)m_wndClusterList.GetCurSel();

	if ( pos.IsNull() )
		return ;

	CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(pos);

	if ( pCluster && (g_pWebConfig != NULL))
	{
		MiniOpenUrl((LPCSTR)g_pWebConfig->GetWebPageUrl(FAMILY_ALBUM,StrToWideStr(pCluster->szFamilyDomain)));
	}
}

void CFloorPageCluster::OnClusterForum()
{
	CL_GROUP_POS pos = (CL_GROUP_POS)m_wndClusterList.GetCurSel();

	if ( pos.IsNull() )
		return ;

	CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(pos);

	if ( pCluster && (g_pWebConfig != NULL))
	{
		MiniOpenUrl((LPCSTR)g_pWebConfig->GetWebPageUrl(FAMILY_DISCUSS,StrToWideStr(pCluster->szFamilyDomain)));
	}
}

void CFloorPageCluster::OnClusterHome()
{
	CL_GROUP_POS pos = (CL_GROUP_POS)m_wndClusterList.GetCurSel();

	if ( pos.IsNull() )
		return ;

	CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(pos);

	if ( pCluster && (g_pWebConfig != NULL))
	{
		MiniOpenUrl((LPCSTR)g_pWebConfig->GetWebPageUrl(FAMILY_HOME,StrToWideStr(pCluster->szFamilyDomain)));
	}
}

// 解散群 
void CFloorPageCluster::OnClusterDismiss()
{
	CL_GROUP_POS pos = (CL_GROUP_POS)m_wndClusterList.GetCurSel();

	if ( pos.IsNull() )
		return ;

	CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(pos);

	if ( pCluster )
	{
		CString str;
		str.Format("确定要解散该群吗？");

		if (MessageBox(str,IM_CAPTION,MB_YESNO|MB_ICONWARNING)!=IDYES)
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

		m_wndClusterList.DelGroup(pos);
	}
}

// 
void CFloorPageCluster::OnSetCMemberAck(const char *data,WORD wRet)
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

void CFloorPageCluster::OnAdd2Friend()
{
	CL_ITEM_POS pos = m_wndClusterList.GetCurSel();

	if ( pos.IsNull()==false )
	{
		FRIEND_INFO *pFriend = (FRIEND_INFO *)m_wndClusterList.GetItemData(pos);

		if ( pFriend )
		{
			if ( pFriend->GetId()==g_ImUserInfo.GetId() )
			{
				ShowMessage("您已经是自己的朋友了！");
				return ;
			}

			if ( FindFriend(pFriend->GetId()) )
			{
				CString str;
				str.Format("%s 已经是您的朋友了！" , UserInfoToStr(pFriend));
				ShowMessage(str);
				return ;
			}

			XT_FRIENDGROUP_MOD_REQ req;

			req.action = GROUP_ADD;
			req.uid    = g_ImUserInfo.GetId();
			req.fid    = pFriend->GetId();
			req.group_id = 0;

			RegUserOperation(0x00c2);
			
			g_pProtocol->SendReq(req);
		}
	}
}

void CFloorPageCluster::OnMemberDel()
{
	CL_ITEM_POS pos = m_wndClusterList.GetCurSel();

	if ( pos.IsNull()==false )
	{
		CString sMsg;
		FRIEND_INFO *pFriend = (FRIEND_INFO *)m_wndClusterList.GetItemData(pos);
		CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_wndClusterList.GetGroupData((CL_GROUP_POS)pos);

		if(pFriend==NULL || pCluster==NULL)
			return;
		if ( pFriend->GetId()==g_ImUserInfo.GetId() )
		{
			ShowMessage("错了，你不能删除自己！",MB_ICONINFORMATION|MB_OK);
			return ;
		}
		else
		{
			sMsg.Format("确定要删除群成员\"%s\"吗？",pFriend->nickname);

			// 如果群成员个数小于规定个数，则解散群

			if (MessageBox(sMsg,IM_CAPTION,MB_YESNO|MB_ICONINFORMATION)==IDYES )
			{
				// 如果成员小于规定个数,解散
				if(pCluster->Count()<=MIN_CLUSTER_MEMBER)
				{
					sMsg.Format("群\"%s\"的成员个数如果少于%d个的话,将会被系统自动解散!确定要删除成员\"%s\"吗?",
						pCluster->szFamilyName,
						MIN_CLUSTER_MEMBER,
						pFriend->nickname);

					if ( MessageBox(sMsg,IM_CAPTION,MB_OKCANCEL|MB_ICONWARNING)==IDOK )
					{
						XT_CLUSTER_DISMISS_REQ req;
						req.nFamilyID = pCluster->nFamilyID;
						req.nFamilyProp = PERM_FAMILY;
						g_pProtocol->SendReq(req);
						// 删除主界面群列表对应的项
						SendMessage(WM_IMOPERATION,OPR_DEL_CLUSTER,(LPARAM)pCluster->nFamilyID);
					}
				}
				else
				{	
					// 删除一个成员
					XT_SET_CMEMBERS_REQ req;

					req.nFamilyID = pCluster->nFamilyID;
					req.nAction = CMEMBER_DEL;//
					req.nFamilyProp =  PERM_FAMILY;
					req.nCount = 1;
					req.nMemID[0]=pFriend->GetId();
					
					g_pProtocol->SendReq(req);	

					RegUserOperation(0x00b3);
				}			
			}
		}
	}
}

void CFloorPageCluster::OnDestroy()
{
	g_pProtocol->UnRegistCmd(m_hWnd);

	__super::OnDestroy();
	m_wndClusterList.DeleteAllItems();

	ClearClusterList();
	ClearMembersPos();

	if ( m_pProgressLoad )
	{
		m_pProgressLoad->DestroyWindow();
		delete m_pProgressLoad;
		m_pProgressLoad=NULL;
	}		  
}

void CFloorPageCluster::OnStatusModAck(const char *data,WORD wRet)
{
	LPXT_STATUS_MOD_ACK resp = (LPXT_STATUS_MOD_ACK)data;
	size_t i,j,c;
	if ( wRet==0 )
	{
		if ( resp->status==XTREAM_OFFLINE )
		{
			for ( i = 0; i < g_vecCluster.size(); i++ )
			{
				c = g_vecCluster[i]->Count();
				for ( j = 0; j < c; j++ )
				{
					g_vecCluster[i]->members[j].status=XTREAM_OFFLINE;
				}
			}
	
			BuildClusterList();
		}
		else
		{
			for ( int i=0; i<g_vecCluster.size(); i++ )
			{
				FRIEND_INFO *p=g_vecCluster[i]->GetFriendInfo(g_ImUserInfo.GetId());
				if ( p )
					p->status=resp->status;

				SortClusterList(g_vecCluster[i]->nFamilyID);
			}

			this->m_wndClusterList.Invalidate();
		}
	}	
}

BOOL CFloorPageCluster::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CFloorPageCluster::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	__super::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	CMenuEx::InitPopupMenu(pPopupMenu,nIndex,bSysMenu);
}

void CFloorPageCluster::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	__super::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
	CMenuEx::MeasureItem(lpMeasureItemStruct);
}

void CFloorPageCluster::ClearMembersPos(uint32 cid)
{
	map<uint32,tagPos *>::iterator it=m_mapPos.find(cid);

	if ( it!=m_mapPos.end() )
	{
		it->second->vecPos.clear();
		delete it->second;
		m_mapPos.erase(it);
	}
}

void CFloorPageCluster::ClearMembersPos()
{
	map<uint32,tagPos *>::iterator it ;

	for ( it=m_mapPos.begin(); it!=m_mapPos.end(); it++ )
	{
		it->second->vecPos.clear();
		delete it->second;
		//m_mapPos.erase(it);
	}

	m_mapPos.clear();
}

void CFloorPageCluster::InitializeList()
{
	m_wndClusterList.DeleteAllItems();
	ClearClusterList();
	ClearMembersPos();
}

void CFloorPageCluster::OnLogOut()
{
	g_LocalData.SaveClusterConfig((char*)g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());
	g_LocalData.SaveClusterList((char*)g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());

	g_LocalData.SaveClusterExitConfig((char*)g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());
	g_LocalData.SaveClusterExitList((char*)g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());

	m_wndClusterList.DeleteAllItems();
	ClearClusterList();
	ClearMembersPos();
}

void CFloorPageCluster::OnUserBlog()
{
	CL_ITEM_POS pos = this->m_wndClusterList.GetCurSel();
	
	if ( pos.IsNull()==false )
	{
		FRIEND_INFO *pUser = (FRIEND_INFO *)m_wndClusterList.GetItemData(pos);
		ShowBlogSite((char*)pUser->GetUserName().c_str());
	}
}

void CFloorPageCluster::OnCMemberLoad()
{
	CL_ITEM_POS pos = this->m_wndClusterList.GetCurSel();
	
	if ( pos.IsNull()==false )
	{
		CL_GROUP_POS posGroup = (CL_GROUP_POS)pos;

		CLUSTER_INFO * pCluster = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(posGroup);

		if ( m_wndClusterList.GetItemCount(posGroup)==0 && pCluster)
		{
			m_wndClusterList.AddLoadFlag(posGroup);
			ImGetClusterMember(pCluster->nFamilyID);
		}
	}
}

void CFloorPageCluster::OnCListRefresh()
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

void CFloorPageCluster::OnCInfoRefresh()
{
	CL_ITEM_POS pos = this->m_wndClusterList.GetCurSel();
	
	if ( pos.IsNull()==false )
	{
		CL_GROUP_POS posGroup      = (CL_GROUP_POS)pos;

		CLUSTER_INFO *pCluster     = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(posGroup);
		CLUSTER_INFO *pRealCluster = FindClusterInfo(pCluster->nFamilyID);

		if ( pRealCluster )
		{
			pRealCluster->bRefreshFlag = true;
			m_wndClusterList.AddLoadFlag(posGroup);
		
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

void CFloorPageCluster::OnUserAlbum()
{
	CL_ITEM_POS pos = this->m_wndClusterList.GetCurSel();
	
	if (pos.IsNull()==false)
	{
		FRIEND_INFO *pUser = (FRIEND_INFO *)m_wndClusterList.GetItemData(pos);
		ShowAlbum((char*)pUser->GetUserName().c_str());
	}
}

void CFloorPageCluster::OnStatusGetAck(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_STATUS_GET_ACK ack = (LPXT_STATUS_GET_ACK)data;

		if ( ack )
		{
			if ( ack->status==XTREAM_HIDE )
			{
				ack->status=XTREAM_OFFLINE;
			}
	
			int nRoot = m_wndClusterList.GetRootCount();

			for ( int i=0; i<nRoot; i++ )
			{
				int nGroup = m_wndClusterList.GetGroupCount(i);

				for ( int j=0; j<nGroup; j++ )
				{
					CL_GROUP_POS posGroup;

					posGroup.nRoot  = i;
					posGroup.nGroup = j;

					CLUSTER_INFO * pCluster = (CLUSTER_INFO * )m_wndClusterList.GetGroupData(posGroup);
					FRIEND_INFO  * pFriend  = pCluster->GetFriendInfo(ack->id);

					if ( pFriend )
					{
						pFriend->ip     = ack->ip;
						pFriend->port   = ack->port;
						pFriend->status = ack->status;					
					}

					if ( pFriend )
						this->BuildCluster(pCluster->nFamilyID);
				}
			}			
		}
	}
}

void CFloorPageCluster::OnSetClusterInfoAck(const char *data,WORD wRet)
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

void CFloorPageCluster::OnClusterRefuse()
{
	CL_GROUP_POS pos =  (CL_GROUP_POS)m_wndClusterList.GetCurSel();
	if ( pos.IsNull()==false )
	{
		CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(pos);
		pCluster->bRefused=!pCluster->bRefused;

		m_wndClusterList.Invalidate();
	}
}

int CFloorPageCluster::FindClusterMember(CL_GROUP_POS posGroup, uint32 mid)
{
	if ( posGroup.IsNull()==false )
	{
		CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(posGroup);

		int n = m_wndClusterList.GetItemCount(posGroup);

		for ( int i=0; i<n; i++)
		{
			CL_ITEM_POS posItem;

			posItem.nRoot  = posGroup.nRoot;
			posItem.nGroup = posGroup.nGroup;
			posItem.nItem  = i;
			FRIEND_INFO *p = (FRIEND_INFO *)m_wndClusterList.GetItemData(posItem);

			if ( p->GetId()==mid)
				return i;
		}
	}

    return -1;
}

void CFloorPageCluster::AddClusterMemberLoadFlag(uint32 cid)
{
	CL_GROUP_POS posGroup = m_wndClusterList.FindCluster(cid);

	if ( posGroup.IsNull()==false )
	{
		CLUSTER_INFO *p=(CLUSTER_INFO *)m_wndClusterList.GetGroupData(posGroup);
		m_wndClusterList.AddLoadFlag(posGroup);
	}
}

LRESULT CFloorPageCluster::OnImOperation(WPARAM wParam,LPARAM lParam)
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

//void CFloorPageCluster::DelCluster(int nClusterId)
//{
//	int i;
//	int nC;
//	int nGroup=0;
//	nC=m_wndClusterList.GetGroupCount();
//	CLUSTER_INFO *pCluster=NULL;
//	for(i=0;i<nC;i++)
//	{
//		pCluster = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(i);
//		if(pCluster->nFamilyID == nClusterId)
//		{
//			nGroup=i;
//			break;
//		}
//	}
//	// 如果列表中不存在待解散的组，则返回
//	if(pCluster==NULL)
//	{
//		return ;
//	}
//	// 关闭族聊天对话框
//	g_pMessageHandleWnd->CloseClusterSendMsgDlg(pCluster->nFamilyID );
//
//	// 清除全局族列表中的该族
//	for ( int i=0; i<g_vecCluster.size(); i++)
//	{
//		if ( g_vecCluster[i]->nFamilyID==pCluster->nFamilyID )
//		{
//			delete g_vecCluster[i];
//			g_vecCluster.erase(g_vecCluster.begin()+i);
//			break ;
//		}
//	}
//	m_wndClusterList.DelGroup(nGroup);
//
//}


LRESULT CFloorPageCluster::OnDismissCluster(WPARAM wParam, LPARAM lParam)
{
	int nID=(int)wParam;

	CL_GROUP_POS posGroup = m_wndClusterList.FindCluster(nID);

	if ( posGroup.IsNull()==false )
	{
		CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(posGroup);
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
		m_wndClusterList.DelGroup(posGroup);
	}

	return 0;
}

LRESULT CFloorPageCluster::OnHintDlgClick(WPARAM wParam, LPARAM lParam)
{
	CLUSTER_INFO *pCluster = (CLUSTER_INFO *)(lParam);
	ASSERT(pCluster);
	CClusterSendMsgDlg *pDlg = g_pMessageHandleWnd->GetClusterSendMsgDlg(pCluster);
	if ( pDlg->m_hWnd==NULL )
		pDlg->Create(CClusterSendMsgDlg::IDD,GetDesktopWindow());

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

void CFloorPageCluster::LoadLoacalClusterList()
{
	g_LocalData.LoadClusterList((char*)g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());
	//g_LocalData.LoadClusterExitList(g_ImUserInfo.GetUserName().c_str());
	BuildClusterList();
	SortGroupList();

	if ( m_wndClusterList.GetGroupCount()==0 )
	{
		if ( m_pProgressLoad==NULL )
		{
			m_pProgressLoad = new CFormProgress2("正在获取群列表",&m_wndClusterList);
			m_pProgressLoad->CenterWindow(&m_wndClusterList);
			m_pProgressLoad->ShowWindow(SW_SHOW);
		}
	}
}

void CFloorPageCluster::FindOldMembers(
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

void CFloorPageCluster::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_SORTGROUP:
		KillTimer(TIMER_SORTGROUP);
		SortGroupList();
		break;
	default:
		if(m_bNeedSort)
		{
			try
			{
				SortClusterListReal(nIDEvent);
			}
			catch(...)
			{
			}
		}
		break;
	}

	CFloorPage::OnTimer(nIDEvent);
}

void CFloorPageCluster::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);
	rc.bottom=rc.top+23;
	CResBitmap bmp;
	bmp.LoadBitmap(IDB_HEADER_BACK);
	HorizStretchBlt(&dc,0,0,rc.Width(),&bmp);
}

void CFloorPageCluster::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFloorPage::OnShowWindow(bShow, nStatus);

	if ( bShow )
		m_wndClusterList.SetFocus();
}


void CFloorPageCluster::OnBnClickedButtonCreate()
{
	// TODO: 在此添加控件通知处理程序代码
}


//////向服务器请求获取群名片信息
void CFloorPageCluster::OnGetCMProfileAck(const char *data, WORD wRet)
{
	//CL_GROUP_POS posGroup = m_wndClusterList.FindCluster(ack->nFamilyID);
	//if ( posGroup.IsNull()==false )
	//{
	//	CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_wndClusterList.GetGroupData(posGroup);
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
