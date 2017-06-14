#include "stdafx.h"
#include "MainFrame.h"
#include "im\xtpacket.h"
#include "im\P2PSendMsgDlg.h"
#include "IMClient.h"
#include "im\MessageHandleWnd.h"
#include "im\UserCardDlg.h"
#include "im\SMSDlg.h"
#include "im\FSDialog.h"
#include "FriendInfoDlg.h"
#include "tone.h"
#include "im\MsgDlg.h"
#include "im\UserAuthDlg.h"
#include "im\AddBlackDlg.h"
#include "im\DuiCounsel.h"
#include "im\SelectGroupDlg.h"
#include "im\UserAuthInfoDlg.h"
#include "im\ModfCommNameDlg.h"
#include "..\HmgSkin\HmgSkin\FlatPopupMenu.h"
#include "DuiMfFriendList.h"
#include "..\..\Common\Common.h"
#include "DuiMfRecentList.h"
#include "im\MsgObj.h"
#include "im\EmotionDlg.h"
#include "GroupSendDlg.h"
#include "im\LocalData.h"
#include "im\DuiMsgNotice.h"
#ifdef _CS_PLAT_EX_
#include "CSPlatDlg.h"
#endif

extern map<FRIEND_INFO *,UINT> g_mapMsgUnRead;

/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg * ShowMessageDlg(FRIEND_INFO *pFriend, StruExtraInfo struExtInfo = StruExtraInfo(), BOOL bIsShow = TRUE)
{
#ifdef _CS_PLAT_EX_
	if (g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != NULL && g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg->GetHWND() != NULL)
	{
		CRect rc;
		::GetWindowRect(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg->GetHWND(), &rc);
		if(::IsMinimized(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg->GetHWND()) || (rc.left < 0 && rc.top < 0 && rc.bottom < 0 && rc.right < 0))
		{
			::ShowWindow(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg->GetHWND(), SW_RESTORE);
		}
	}
#endif	

	/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend, struExtInfo);
	if ( pDlg->GetHWND()==NULL )
	{
		//pDlg->Create(CP2PSendMsgDlg::IDD,CWnd::GetDesktopWindow());

		pDlg->NotifyServer_OpenedMsgDlg();
	}

	if(!bIsShow)
	{
		pDlg->ShowWindow(false);
		return pDlg;
	}
#ifdef _CS_PLAT_EX_
	if( CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()) && CMainFrame::IsBuyer(pFriend->GetId()))
	{
		CDuiCSPlatDlg::GetInstance()->m_bIsUpdate = TRUE;
		CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
	}
#else
	if( CDuiCounsel::GetInstance() != NULL && ::IsWindowVisible(CDuiCounsel::GetInstance()->GetHWND()) && CMainFrame::IsBuyer(pFriend->GetId()))
	{
		CDuiCounsel::GetInstance()->m_bIsUpdate = TRUE;
		CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
	}
#endif
	else
	{
		if ( /*pDlg->*/::IsWindowVisible(pDlg->GetHWND())==FALSE )
			pDlg->ShowWindow(SW_SHOW);
	}

	if (/*pDlg->IsIconic()*/::IsMinimized(pDlg->GetHWND()) )
	{
		pDlg->ShowWindow(SW_RESTORE);
	}

	::SetForegroundWindow(pDlg->GetHWND());
	::BringWindowToTop(pDlg->GetHWND());
#ifdef _CS_PLAT_EX_
	g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg = pDlg;//设置当前显示的聊天窗口
#else
	CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg = pDlg;
	g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg = pDlg;
#endif
	return pDlg;
}

bool TranslateMsg(const CString & sMsg,CString & sShowMsg,vector<DOWNLOAD_OBJ> & arrDownloadTask)
{
	arrDownloadTask.clear();
	CMsgObj msgObj;
	CString sMD5;
	CString sFile;
	vector<string> arrImg;
	vector<string> arrShow;
	msgObj.Load(sMsg);
	msgObj.GetWebImage(arrImg);
	// 先判断这些图片链接是否存在于自定义表情中
	size_t nC = arrImg.size();
	if(nC == 0)
		return false;
	arrShow.resize(nC);

	EMOTION_ID id;
	size_t i;
	for( i = 0;i < nC;i++)
	{
		// 查询本地表情库中是否有该链接的定义
		id = FindFaceUseUrl(arrImg[i].c_str());
		// 本地表情中没有该图片，需要下载
		if( id.nGroup == -1)
		{
			// 先获得该文件的md5名
			GetMD5WebFileName(arrImg[i].c_str(),sMD5);		
			// 判断该文件是否已经下载
			if(FileExist(g_LocalData.GetImageCanchePath()+"\\"+sMD5))
			{
				// 该文件已经下载,则直接使用它的文件名显示
				arrShow[i] = sMD5;
			}
			else
			{
				arrShow[i] = sMD5;
				// 下载任务
				DOWNLOAD_OBJ param;
				param.handle = NULL;
				param.sSaved = g_LocalData.GetImageCanchePath()+"\\"+sMD5;
				param.sUrl = arrImg[i].c_str();
				param.timer = 0;
				param.state = DS_DOWNLOADING;
				arrDownloadTask.push_back(param);
			}
		}
		// 本地有该文件缓存，不需要下载
		else
		{
			sFile = g_LocalData.GetImageCanchePath() +"\\"+ g_customEmotionDB[id.nGroup][id.nItem].szFile;
			if(!FileExist(sFile))
			{
				CopyFile(g_LocalData.GetCustomFacePath()+"\\" + g_customEmotionDB[id.nGroup][id.nItem].szFile,sFile,false);
			}
			arrShow[i] = g_customEmotionDB[id.nGroup][id.nItem].szFile;
		}
	}
	msgObj.ReplaceWebImage(arrShow);
	sShowMsg = msgObj.ToMsg().c_str();
	return true;
}

extern BOOL AppendDuiMenu(DuiLib::CMenuWnd* pDuiMenu, CMenu *pMenu);
extern CWebUserHeadIconProcess g_webUserHeadIconProcess;
extern string CutRoundImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE);

////好友列表中使用的菜单
//#define	ID_SENDMSG				0x0101
//#define	ID_FRIENDINFO			0x0102
//#define	ID_SEARCH				0x0103
//#define	ID_DELFRIEND			0x0104
//#define	ID_ADDGROUP				0x0105
//#define	ID_DELGROUP				0x0106
//#define	ID_MSGHISTORY			0x0107
//#define	ID_RENAMEGROUP			0x0108
//#define	ID_REFRESH_FRIENDLIST	0x0109
//#define	ID_SHOWALL				0x0110
//#define	ID_HIDEOFFLINE			0x0111
//#define	ID_FRIENDALBUM			0x0112
//#define	ID_FRIENDBLOG			0x0113
//#define	ID_DELMFRIEND			0x0114	// 删除多个好友
//#define ID_MODF_COMMNAME		0x0115	// 修改好友备注名称
//#define ID_CREATE_TEMPCLUSTER	0x0116 
//#define	ID_FRIENDARTICLE		0x0117
//#define	ID_SEND_GROUP_MESSAGE	0x0118
//#define	ID_ADD_CGROUP			0x0119
//#define	ID_ADD_FRIEND			0x0120
//#define	ID_RENAME_GROUP			0x0121
//#define ID_REMOVE_BLACKLIST		0x0122
//#define ID_ADD_BLACKLIST		0x0123

// 添加组时能够分配的最小组ID，0用于我的好友，1用于“陌生人”，2用于“黑名单”
const int c_minGroupID = 3;	
// 添加组时能够分配的最大组ID
const int c_maxGroupID = MY_TEAM_GROUP_ID - 1;

//从本地文件中获取好友信息
void CMainFrame::LoadLocalFriends()
{
	::ClearFriendList();

	memset(&g_GroupInfo,0,sizeof(g_GroupInfo));

	g_GroupInfo.dwUserId = g_ImUserInfo.GetId();

#ifdef _CS_PLAT_EX_  //飞牛客服工作台(去掉好友组)
	g_GroupInfo.count = 2;
	g_GroupInfo.group[0].group_index = 1;
	g_GroupInfo.group[0].group_father = -1;
	g_GroupInfo.group[0].group_state = FLS_CLOSE;
	strcpy(g_GroupInfo.group[0].name, "最近联系客户");


	g_LocalData.GetGroupInfo(g_GroupInfo);
	g_LocalData.GetFriendList(g_lsFriend);	
#else
	g_GroupInfo.count = 3;
	g_GroupInfo.group[0].group_index = 0;
	g_GroupInfo.group[0].group_father = -1;
	g_GroupInfo.group[0].group_state = FLS_CLOSE;
	strcpy(g_GroupInfo.group[0].name, "我的好友");

	g_GroupInfo.group[1].group_index = 1;
	g_GroupInfo.group[1].group_father = -1;
	g_GroupInfo.group[1].group_state = FLS_CLOSE;
	strcpy(g_GroupInfo.group[1].name, "陌生人");

	g_GroupInfo.group[2].group_index = 2;
	g_GroupInfo.group[2].group_father = -1;
	g_GroupInfo.group[2].group_state = FLS_CLOSE;
	strcpy(g_GroupInfo.group[2].name, "黑名单");

	g_LocalData.GetGroupInfo(g_GroupInfo);
	g_LocalData.GetFriendList(g_lsFriend);	
#endif
	
}

void CMainFrame::ImGetFriendList(int start_pos, int type)
{
	XT_FRIENDLIST_REQ req;
#ifndef _WORK_EX_
	req.merchant_id = g_ImUserInfo.merchant_id;
#endif
	req.id			= g_ImUserInfo.GetId();
	req.start_pos	= start_pos;
#ifdef _CS_PLAT_EX_
	req.nType = XT_LOGIN_REQ::CS;
	req.merchant_id = -1;
#else
	req.nType = XT_LOGIN_REQ::SELLER;
#endif

	// 刚开始获取好友列表
	if ( start_pos==0 )
	{
		m_arrFriend.clear();
		m_vecPos.clear();
	}
	g_pProtocol->SendReq(req);
}

//更新显示好友列表
void CMainFrame::UpdateFriendList()
{
	FL_ITEM_POS pos = m_pFriendList->GetCurSel();
	
	//清空所有group 和组中Item
	m_pFriendList->DeleteAllItems();

	//groupInfo >> groupList， groupID最多到500
	bool bGroupAdd[500];
	memset(bGroupAdd, 0, sizeof(bGroupAdd));

	for ( int i=0; i<g_GroupInfo.count; i++ )
	{
		int nSubGroupId, nParentGroupId, nSubGroupIndex;
		deque<int> dequeId;
		
		nSubGroupId = g_GroupInfo.group[i].group_index;
		nParentGroupId = g_GroupInfo.group[i].group_father;
		//须父组先创建
		do
		{
			dequeId.push_front(nSubGroupId);
			nSubGroupIndex = g_GroupInfo.FindItemIndex(nSubGroupId);
			if(nSubGroupIndex != -1)
			{
				nSubGroupId = g_GroupInfo.group[nSubGroupIndex].group_father;
			}
			else//找不到父组
			{
				dequeId.clear();
				break;
			}
		}while(nSubGroupId != -1 && !bGroupAdd[nSubGroupId] && dequeId.size()<10);//这里有风险

		while( !dequeId.empty())
		{
			nSubGroupId = dequeId.front();
			nSubGroupIndex = g_GroupInfo.FindItemIndex(nSubGroupId);
			nParentGroupId = g_GroupInfo.group[nSubGroupIndex].group_father;
			m_pFriendList->AddSubGroup(nParentGroupId, nSubGroupId, g_GroupInfo.group[nSubGroupIndex].name);
			//组状态
			m_pFriendList->SetGroupState(nSubGroupId,(FLS_STATE)g_GroupInfo.group[nSubGroupIndex].group_state);
			bGroupAdd[nSubGroupId] = true;
			dequeId.pop_front();
		}
	}

	//好友信息>> groupList
	FRIEND_LIST::iterator it;
	FRIEND_INFO * pFriend ;
	for ( it=g_lsFriend.begin(); it!=g_lsFriend.end(); it++ )
	{
		pFriend = (*it);
		if(pFriend == NULL)
		{
			ASSERT(pFriend);
			continue;
		}
	
		// 是否有备注名称
		CString sName;
		if(pFriend->comment_name[0] == 0)
			sName = pFriend->nickname;
		else
			sName = pFriend->comment_name;
		
		m_pFriendList->AddItem(pFriend->group_index, sName, (DWORD)(pFriend));
		//设置自己的组ID
		if(pFriend->GetId() == g_ImUserInfo.GetId())
			g_ImUserInfo.group_index = pFriend->group_index;
	}

	//组内好友排序
	for(int i=0; i<g_GroupInfo.count; i++ )
	{
		int nSubGroupId = g_GroupInfo.group[i].group_index;
		SortGroupList(nSubGroupId);
	}

	if(pos.nGroupID >= 0 && pos.nItem >= 0)
		m_pFriendList->SetCurSel(pos.nGroupID,pos.nItem);

	UpdateFriendCnt();
}

void CMainFrame::InitializeIm()
{
	// 本地文件中加载好友列表
	LoadLocalFriends();
	UpdateFriendList();

	// 好友分组请求
	ImGetGroupInfo();
	// 发送获取好友列表的请求
	ImGetFriendList(0,FLS_TOTAL);

	m_pipeCallMe.Initialize(g_ImUserInfo.GetId(),m_hWnd);	

	ShowUserInfo();	
	StartMiniWebSession();
}

void CMainFrame::OnGroupInfoGet(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		CRecvPacket rp((char *)data,MAX_RECV_BUFFERS);
		uint8 group_count;
		uint8 group_id,group_pid;
		rp>>group_count;

		GROUP_INFO tmp_GroupInfo = g_GroupInfo;

#ifdef _CS_PLAT_EX_  //飞牛客服工作台(去掉好友组)
		g_GroupInfo.count = group_count;
		g_GroupInfo.dwUserId = g_ImUserInfo.GetId();

		for (int i=0; i<group_count; i++ )
		{
			rp>>group_pid;
			rp>>group_id;
			rp>>g_GroupInfo.group[i].name;

			g_GroupInfo.group[i].group_father = (group_pid==0xFF) ? -1 : group_pid;
			g_GroupInfo.group[i].group_index = group_id;
			g_GroupInfo.group[i].group_state = FLS_CLOSE;

		}


		g_GroupInfo.Sort();
#else
		g_GroupInfo.count = group_count + 3;
		g_GroupInfo.dwUserId = g_ImUserInfo.GetId();
		g_GroupInfo.group[0].group_index = 0;
		g_GroupInfo.group[0].group_father = -1;
		g_GroupInfo.group[0].group_state = FLS_CLOSE;
		sprintf(g_GroupInfo.group[0].name,"我的好友");

		g_GroupInfo.group[1].group_index = 1;
		g_GroupInfo.group[1].group_father = -1;
		g_GroupInfo.group[2].group_state = FLS_CLOSE;
		sprintf(g_GroupInfo.group[1].name,"陌生人");

		g_GroupInfo.group[2].group_index = 2;
		g_GroupInfo.group[2].group_father = -1;
		g_GroupInfo.group[2].group_state = FLS_CLOSE;
		sprintf(g_GroupInfo.group[2].name,"黑名单");

		for (int i=0; i<group_count; i++ )
		{
			rp>>group_pid;
			rp>>group_id;
			rp>>g_GroupInfo.group[i+3].name;

			g_GroupInfo.group[i+3].group_father = (group_pid==0xFF) ? -1 : group_pid;
			g_GroupInfo.group[i+3].group_index = group_id;
			g_GroupInfo.group[i+3].group_state = FLS_CLOSE;

		}

		g_GroupInfo.Sort();
#endif
		//分组开合状态
		for(int i=0; i<g_GroupInfo.count; i++)
		{
			int nGroupId = g_GroupInfo.group[i].group_index;
			int idx = tmp_GroupInfo.FindItemIndex(nGroupId);
			if( idx != -1 && idx < tmp_GroupInfo.count)
			{
				g_GroupInfo.group[i].group_state = tmp_GroupInfo.group[idx].group_state;
			}
			else
			{
				g_GroupInfo.group[i].group_state = FLS_CLOSE;
			}
		}
		UpdateFriendList();
	}

	// 卖家版中判断分组中是否有我的团队分组，如果没有就添加
#ifdef _BUSINESS_VESION	
#if !defined(_CS_PLAT_EX_) && !defined(_WORK_EX_)
	if (AddGroup_MyTeam())
	{	// 成功添加，再次更新分组列表
		Sleep(2000);		// 添加成功后等一会再更新列表，以免添加函数重复调用
		ImGetGroupInfo();
	}
#endif
#endif
}

#ifdef _BUSINESS_VESION
// 添加我的团队分组，如果已经存在就不添加
static BOOL g_isAddMyTeam = FALSE;	// 确保只添加了一次“我的团队”
BOOL CMainFrame::AddGroup_MyTeam()
{
	if (g_isAddMyTeam)
		return FALSE;
	g_isAddMyTeam = TRUE;

	// 查找团队
	BOOL bFind = FALSE;
	for(int i = 0; i < g_GroupInfo.count; ++i)
	{
		if (g_GroupInfo.group[i].group_index == MY_TEAM_GROUP_ID)
		{
			bFind = TRUE;
			break;
		}
	}

	if (!bFind)
	{	// 没有“我的团队”，给后台发送消息添加
		ImAddGroup(-1, MY_TEAM_GROUP_ID, "我的团队");
		return TRUE;
	}

	return FALSE;
}
#endif

void CMainFrame::OnFriendListAck(const char *data,WORD wRet)
{
	FRIENDLIST_SEG & ack = *(FRIENDLIST_SEG *)data;
	size_t i;
	size_t nC ;
	FRIEND_INFO * pFriend = NULL;
	if ( wRet==0 )
	{
		nC = ack.size();
		for ( i = 0; i<m_vecPos.size(); i++ )
		{
			if ( m_vecPos[i]==ack.m_nNextPos )
			{
				return ;
			}
		}
		m_vecPos.push_back(ack.m_nNextPos);

		for ( i = 0; i < nC; i++ )
		{
			m_arrFriend.push_back(ack[i]);

			FRIENDITEM & f = ack[i];
			//如果对方是隐身则把状态设置为离开，
			if ( f.status==XTREAM_HIDE )
			{
				f.status=XTREAM_OFFLINE;
			}
			pFriend = FindFriend(f.friendId);
			if(pFriend != NULL)
			{
				pFriend->group_index = f.groupId;
				pFriend->status = f.status;
				UpdateFriend(pFriend,FALSE);
			}
			else
			{
				pFriend = new FRIEND_INFO();
				pFriend->SelfEncrypt();
				pFriend->SetId(f.friendId);
				pFriend->group_index = f.groupId;
				pFriend->status = f.status;
				// 暂时使用id做名称
				// 添加到好友列表中去
				//ASSERT(FALSE);
				strcpy(pFriend->nickname, UserInfoToStr(pFriend));
				if(!AddFriend(pFriend,false))
					delete pFriend;
			}
		}

		// 判断好友列表是否获取完毕
		if ( ack.m_nNextPos!=0xFFFF )
		{
			// 继续获取好友列表
			ImGetFriendList(ack.m_nNextPos);
			UpdateFriendList();
		}
		// 获取好友列表完毕
		else
		{
			// 与本地缓存比较,判断是否有好友需要更新
			// 此时全局好友列表中为本地缓存加载的内容
			vector<uint32> addiFriends;
			vector<uint32> oldFriends;
			int j,c;
			GetDiffFriends(g_lsFriend,m_arrFriend,addiFriends);
			// 删除在好友列表中不存在的旧好友
			FindOldFriends(g_lsFriend,m_arrFriend,oldFriends);
			c = oldFriends.size();
			for(j = 0; j < c;j++)
			{
				DelFriend(oldFriends[j]);	
			}
			// 获取本地缓存中没有资料的好友的基本资料
			m_arrFriendsID_Basic = addiFriends;
			ImGetFriendBasicInfo();

			//m_pFriendList->RemoveRefreshFlag();
			UpdateFriendList();

			//m_mapWebInfo.clear();

#ifdef _IM_UNIT_TEST
			IMTEST->StartTest();
			// 		CTestDialog dlg(NULL, true);
			// 		dlg.DoModal();
#endif
		}


		//在获取好友列表后ok后，打开与客服的对话框
		if(!theApp.m_strCustomerName.empty() && !theApp.m_strSubMerchantName.empty()
			&& theApp.m_nMerchantID != 0  && theApp.m_nSubMerchantID != 0)
		{
			FRIEND_INFO* pFriend= NULL;
			int friendid = theApp.m_nSubMerchantID;
			pFriend = ::FindFriend(friendid);
			if( pFriend)
			{
			}
			else
			{
				pFriend = new FRIEND_INFO;
				pFriend->SetId(theApp.m_nSubMerchantID);
				pFriend->SetUserName("");
				pFriend->SetGuid("");
				pFriend->SetPassword("");
				strcpy(pFriend->nickname,theApp.m_strSubMerchantName.c_str());
				pFriend->group_index = 1;//陌生人
				g_lsFriend.push_back(pFriend);

				XT_STATUS_GET_REQ reqStatus;
				reqStatus.fid = friendid;
				reqStatus.id  = g_ImUserInfo.GetId();
				g_pProtocol->SendReq(reqStatus);

				//XT_USERINFO_GET_REQ reqUserInfo;
				//reqUserInfo.fid = friendid;
				//g_pProtocol->SendReq(reqUserInfo);	
			}

			CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
			if ( pDlg->GetHWND()==NULL )
			{
				pDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, CWnd::FromHandle(GetDesktopWindow()));
				pDlg->NotifyServer_OpenedMsgDlg();
			}
			if ( pDlg->GetHWND()==NULL )
				//pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
				pDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, CWnd::FromHandle(GetDesktopWindow()));

			if ( /*pDlg->IsWindowVisible()*/::IsWindowVisible(pDlg->GetHWND())==FALSE )
				pDlg->ShowWindow(/*SW_SHOW*/true);

			if (/*pDlg->IsIconic()*/::IsMinimized(pDlg->GetHWND()))
				pDlg->ShowWindow(/*SW_RESTORE*/true);

			/*pDlg->BringWindowToTop()*/::BringWindowToTop(pDlg->GetHWND());
		}

		// 判断好有列表是否获取完毕
		if ( ack.m_nNextPos==0xFFFF )
		{
			ImGetClusterList();

			if(g_LocalData.GetBasicSystemSetting()->bShowMiniPictureBroadCast )
				ShowMiniPictureBroadCast();
		}
	}
	else
	{
		static char error[255];

		if ( wRet==SERVER_TIMEOUT )
		{
			strcpy(error,"服务器处理好友列表请求超时！");			
		}
		else
		{
			strcpy(error,g_pProtocol->GetLastError());
		}

		//m_pFriendList->RemoveRefreshFlag();

		//	theApp.m_pMainWnd->PostMessage(WM_IM_OK,1,(LPARAM)error);
	}
}

void CMainFrame::GetDiffFriends(const FRIEND_LIST & oldFriends,
	const vector<FRIENDITEM> & newFriends,
	vector<uint32> & addiFriends )
{
	FRIEND_LIST::const_iterator it,itE;
	size_t i,nC;
	uint32 id;
	bool bExist;
	FRIEND_INFO * pFriend = NULL;
	addiFriends.clear();
	itE = oldFriends.end();
	nC = newFriends.size();

	//
	// 遍历新好友列表中的成员是否在旧好友列表中存在
	//

	for( i = 0; i < nC; i++ )
	{
		id = newFriends[i].friendId;
		bExist = false;
		for( it = oldFriends.begin(); it != itE; it++ )
		{
			if( (*it)->GetId() == id)
			{
				pFriend = (*it);
				bExist = true;
				break;
			}
		}
		bExist = false;
		if( !bExist )
		{
			addiFriends.push_back(id);
		}
		// 判断好友资料是否有效，如果无效则需获取
		else if(pFriend)
		{
			if(pFriend->bBasicInfo == false)
				addiFriends.push_back(id);
		}
	}
}

void CMainFrame::FindOldFriends(const FRIEND_LIST & oldFriends, const vector<FRIENDITEM> & newFriends, vector<uint32> & rst)
{
	FRIEND_LIST::const_iterator it,itE;
	size_t i,c;
	rst.clear();
	itE = oldFriends.end();
	c = newFriends.size();
	for(it = oldFriends.begin();it != itE; it++)
	{
		// 不删除陌生人
		if((*it)->group_index == 1)
		{
			continue;
		}

		for(i = 0; i < c; i++)
		{
			if((*it)->GetId() == newFriends[i].friendId)
				break;
		}
		// 如果新列表中没有，则为垃圾数据，需要删除
		if(i == c)
		{
			rst.push_back((*it)->GetId());
		}
	}
}

void CMainFrame::DelFriend(FRIEND_INFO *pFriend)
{

	g_pMessageHandleWnd->CloseP2PSendMsgDlg(pFriend->GetId());

	//m_wndGroupList中删除
	m_pFriendList->DeleteItem(pFriend);

	//g_lsFriend中删除
	FRIEND_LIST::iterator it = find(g_lsFriend.begin(), g_lsFriend.end(), pFriend);
	ASSERT(it != g_lsFriend.end());
	if(it != g_lsFriend.end())
	{
		//清空群成员的备注名
		for(int nCluster = 0; nCluster < g_vecCluster.size(); nCluster++)
		{
			CLUSTER_INFO* pCluster = g_vecCluster[nCluster];
			for(int nFriend = 0; nFriend < pCluster->members.size(); nFriend++)
			{
				FRIEND_INFO& pFriendInfo = pCluster->members[nFriend];
				if(pFriendInfo.GetId() == pFriend->GetId())
					memset(pFriendInfo.comment_name, 0, sizeof(pFriendInfo.comment_name));
			}
		}
		g_lsFriend.erase(it);
		delete pFriend;
		pFriend = NULL;
	}
	//UpdateFriendCnt();

	//更新好友资料
	XT_USERINFO_GET_REQ req;
	req.fid = g_ImUserInfo.GetId();
	g_pProtocol->SendReq(req);
}

void CMainFrame::DelFriend(uint32 nFriendId)
{

	FRIEND_INFO *pFriend = NULL;

	FRIEND_LIST::iterator it;
	for ( it=g_lsFriend.begin();it!=g_lsFriend.end(); it++ )
	{

		if ( (*it)->GetId() == nFriendId )
		{
			delete pFriend;
			pFriend = NULL;
			g_lsFriend.erase(it);
			return;
			//pFriend = *it;
			//break;
		}
	}	
	//ASSERT(pFriend != NULL);
	//if(pFriend != NULL)
	//DelFriend(nFriendId);
}

bool CMainFrame::AddFriend(FRIEND_INFO *pFriend,bool bSel)
{
	if (FindFriend(pFriend->GetId()))
	{
		return false;
	}

#ifndef _CS_PLAT_EX_
	DeleteStranger(pFriend->GetId());//商家版清空陌生人列表
#endif

	g_lsFriend.push_back(pFriend);

	m_pFriendList->AddItem(pFriend->group_index, UserInfoToStr(pFriend),(DWORD)pFriend );

	if(bSel)
		m_pFriendList->SetCurSel(pFriend);

	return true;
}

//商家版清空陌生人列表
void CMainFrame::DeleteStranger(int nAddId)
{
	DuiLib::CTreeNodeUI *pNode = m_pFriendList->FindGroupNode(1);	
	if(pNode == NULL)
		return;

	int nIdx = pNode->GetCountChild();
	if(nIdx < 50)
		return;

	DuiLib::CTreeNodeUI *pLastNode = pNode->GetChildNode(nIdx-1);
	if(pLastNode == NULL)
		return;

	FRIEND_INFO* pFriend = (FRIEND_INFO*)pLastNode->GetTag();

	if(pFriend != NULL && pFriend->GetId() == nAddId)
	{
		pLastNode = pNode->GetChildNode(nIdx-2);
		if(pLastNode == NULL)
			return;
		pFriend = (FRIEND_INFO*)pLastNode->GetTag();
	}
	if(pFriend == NULL)
		return;
	int nId = pFriend->GetId();

	pNode->RemoveAt(pLastNode);

	m_pRecentList->DeleteUser(nId);
	list<FRIEND_INFO *>::iterator itr = g_lsFriend.begin();
	for(; itr != g_lsFriend.end(); itr++)
	{
		if((*itr)->GetId() == nId)
		{
			g_lsFriend.erase(itr);
			break;
		}
	}
	DeleteStranger(nAddId);
}

void CMainFrame::ImGetFriendBasicInfo() const
{
	const int maxids = 15;
	char buff[MAX_SEND_BUFFERS];
	LPXT_GET_FRIEND_BASICINFO_REQ req = (LPXT_GET_FRIEND_BASICINFO_REQ)buff;
	req->type = XT_GET_FRIEND_BASICINFO_REQ::TYPE_FRIEND;
	size_t i = 0;
	size_t n = 0;
	int c = min(m_arrFriendsID_Basic.size(),maxids);
	if(c == 0)
	{
		return;
	}

	for(int i = 0; i < c; ++i)
	{
		req->FriendID[i] = m_arrFriendsID_Basic[i];
	}
	req->count = c;
	g_pProtocol->SendReq(*req);
}

void CMainFrame::OnGroupChoose(UINT nID)
{
	int nIdx = nID - ID_GROUP_CHOOSE_BEG;
	if(nIdx > g_GroupInfo.count)
		return;
	PostMessage( WM_IMFRIEND_LIST_NOTIFY,NM_FL_DRAGDROP,MAKELPARAM(nIdx,0));
}

void CMainFrame::OnRemoveBlackList()
{
	//int nIdx = GROUPID_BLACKLIST;
	int nIdx = g_GroupInfo.count - 1;
	if(nIdx > g_GroupInfo.count)
		return;
	PostMessage( WM_IMFRIEND_LIST_NOTIFY,NM_FL_DRAGDROP,MAKELPARAM(nIdx,0));
}

void CMainFrame::OnAddBlackList()
{
#ifndef _CS_PLAT_EX_ //飞牛客服工作台
	CString str;
	FRIEND_INFO* pFriend = NULL;
	//CWnd* pStatic;
	FL_ITEM_POS pos;

	pos = m_pFriendList->GetCurSel();


	pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(pos.nGroupID,pos.nItem);
	if(pFriend->group_index == GROUPID_STRANGER)
	{
		str.Format("此联系人在陌生人中,加为好友,将从陌生人中删除");
	}
	else if (pFriend->group_index == GROUPID_BLACKLIST)
	{
		str.Format("此联系人在黑名单中,加为好友,将从黑名单中删除");
	}
	if(IDYES==CMessageBox::Model(GetHWND(), str, IM_CAPTION, MB_YESNO|MB_ICONWARNING))
	{
		CAddBlackDlg::Show(pFriend->GetId());
	}
#endif
}

void CMainFrame::OnDelFriendReq(const char * data,WORD wRet)
{
	uint32 uid = (uint32)data;
	if(wRet == 0)
	{
		CString sInfo;
		FRIEND_INFO * pFriend = GetFriendInfo(uid);
		if(pFriend)
		{
			sInfo.Format("用户\"%s\"已经将你列入黑名单",pFriend->nickname);
		}
		else
		{
			sInfo.Format("用户%d已经将你列入黑名单",uid);
		}
		ShowMessage(sInfo);
		DelFriend(pFriend);	
	}
}

LRESULT CMainFrame::OnWebDataNotify(WPARAM wParam, LPARAM lParam)
{
	CWebDataProcess::tagThreadResult * result = (CWebDataProcess::tagThreadResult *)lParam;

	uint32 fid = atol((LPCSTR)result->strID);

	FRIEND_INFO *pFriend  = FindFriend(fid);

	if ( pFriend )		
	{
		if ( wParam==1)
		{
			vector<TWebArticleInfo *> info;
			CString strServerDate;

			try
			{
				ParaseWebArticles(result->strWebResult,info,strServerDate,100);
			}
			catch(...)
			{
			}

			int n = info.size();

			TUserWebArticleInfo * u = pFriend->pUserWebArticleInfo;
			n--;

			if (u)
			{
				while(n>-1)
				{
					strncpy(u->szLastUpdateDate,(LPCSTR)strServerDate,sizeof(u->szLastUpdateDate)-1);

					if ( u->Push(info[n])==false )
					{
						delete info[n];
					}	

					n--;
				}

			}

			info.clear();
		}
		else
		{	
			vector<TWebPictureInfo *> info;
			CString strServerDate;

			try
			{
				ParaseWebPictures(result->strWebResult,info,strServerDate,100);
			}
			catch(...)
			{
			}

			int n = info.size();

			n--;

			TUserWebPictureInfo * u = pFriend->pUserWebPictureInfo;

			if (u)
			{		
				strncpy(u->szLastUpdateDate,(LPCSTR)strServerDate,sizeof(u->szLastUpdateDate)-1);

				while(n>-1)
				{
					if ( u->Push(info[n])==false )
					{
						delete info[n];
					}
									
					n--;
				}
			}
		}

		CUserCardDlg::Refresh(pFriend);
	}


	return 0;
}

//处理来自好友列表的鼠标右键
LRESULT CMainFrame::OnFriendListNotify(WPARAM wParam, LPARAM lParam)
{
	FL_ITEM_POS pos=m_pFriendList->GetCurSel();
	int nGroup = pos.nGroupID;
	if(nGroup == -1)//不是来自好友列表的消息不处理
		return 0;
	int nItem  = pos.nItem;

	FRIEND_INFO * pFriend = NULL;
	
	if ( nGroup>-1 && nItem>-1 )
		pFriend=(FRIEND_INFO *)m_pFriendList->GetItemData(pos.nGroupID,pos.nItem);

	switch ( wParam )
	{
	case NM_FL_LBUTTONDOWN:
		if ( pFriend &&  pFriend->status!=XTREAM_OFFLINE )
		{
			CString strFile = g_webUserHeadIconProcess.GetFaceFile(pFriend);
			if ( strFile=="" ||  FileExist(strFile)==FALSE )
				g_webUserHeadIconProcess.GetUserHeadIcon(pFriend, CWnd::FromHandle(GetHWND()));

			//TRACE("NM_FL_LBUTTONDOWN\n");
		}
		break;
	// 左键双击
	case NM_FL_LBUTTONDBLCLICK:
		// 当前选定了某个好友
		if(pFriend)
		{
			//自己不能给自己发消息
			if(g_ImUserInfo.status!=XTREAM_OFFLINE && pFriend->GetId() != g_ImUserInfo.GetId())
				ShowMessageDlg(pFriend);
		}

		//KillTimer(TIMER_USER_CARD);
		break;
	case NM_FL_DRAGDROP://拖放
		if (g_ImUserInfo.status!=XTREAM_OFFLINE)
		{
			int nMenuIndx = LOBYTE(lParam) ;
			int nDestGroupId = g_GroupInfo.group[nMenuIndx].group_index;
			if(nGroup == nDestGroupId)
				break;
			FRIEND_INFO *pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,nItem);
			if(pFriend == NULL)
			{
				return 0;
			}
			ASSERT(pFriend);
			bool bMove = false;
			bool bBlack = false;
			// 将其他组的好友拖到黑名单
			if ( nDestGroupId == GROUPID_BLACKLIST && pFriend->group_index != GROUPID_STRANGER )
			{
				if( pFriend->group_index == GROUPID_BLACKLIST)
				{
					bMove = true;
				}
				else
				{
					CString str;
					str.Format("确认将 %s 加入黑名单吗",UserInfoToStr(pFriend));
					if ( IDYES==CMessageBox::Model(GetHWND(), str,IM_CAPTION,MB_YESNO|MB_ICONWARNING) )
					{
						/*str.Format("是否同时将自己从%s的好友列表中移除?",UserInfoToStr(pFriend));
						if ( IDYES == MessageBox(str,IM_CAPTION,MB_YESNO|MB_ICONWARNING) )*/
						{
							bBlack = true;
						}
						bMove = true;
					}
				}
			}
			// 从其他组拖动到陌生人组
			else if(nDestGroupId == GROUPID_STRANGER )
			{
				if(  pFriend->group_index == GROUPID_STRANGER )
				{
					bMove = true;
				}
				else
				{
					CString str;
					str.Format("确认将 %s 加入陌生人吗",UserInfoToStr(pFriend));
					if ( IDYES==CMessageBox::Model(GetHWND(),str,IM_CAPTION,MB_YESNO|MB_ICONWARNING) )
					{
						// 直接删除该好友
						pFriend->group_index = nDestGroupId;
						pFriend->bInviting = false;
						m_pFriendList->MoveItem(nGroup,nItem,nDestGroupId);

						XT_FRIENDGROUP_MOD_REQ req;
						req.action = GROUP_MODIFY;
						req.group_id = GROUPID_STRANGER;
						req.fid = pFriend->GetId();
						req.uid = g_ImUserInfo.GetId();
						g_pProtocol->SendReq(req);
					}
				}
			}
			// 从陌生人拖动到其他组（添加好友）
			else if(pFriend->group_index == GROUPID_STRANGER )
			{
				XT_FRIENDGROUP_MOD_REQ req;
				req.action = GROUP_ADD;
				req.uid    = g_ImUserInfo.GetId();
				req.fid    = pFriend->GetId();
				req.group_id = nDestGroupId;
				g_pProtocol->SendReq(req);
				// 陌生人拖动到黑名单
				if(nDestGroupId == GROUPID_BLACKLIST)
				{
					pFriend->bInviting = false;
					pFriend->group_index = nDestGroupId;
					m_pFriendList->MoveItem(nGroup,nItem,nDestGroupId);
					SortGroupList(nDestGroupId);
				}
				else
				{
					pFriend->bInviting = true;
				}
				
			}
			// 黑名单到好友
			else if( pFriend->group_index == GROUPID_BLACKLIST  )
			{
				XT_FRIENDGROUP_MOD_REQ req;
				req.action = GROUP_ADD;
				req.uid    = g_ImUserInfo.GetId();
				req.fid    = pFriend->GetId();
				req.group_id = nDestGroupId;
				g_pProtocol->SendReq(req);
				//m_pFriendList->MoveItem(nGroup,nItem,nGroupDest);
				pFriend->bInviting = true;
				//SortGroupList(nGroupDest);
			}
			else
				bMove = true;
			// 修改用户组
			if(bMove)
			{
				if(bBlack)
				{	
					// 黑名单
					ImSetUserGroup(pFriend->GetId(),nDestGroupId,XT_FRIENDGROUP_MOD_REQ::ACTION_BLACK);			
				}
				else
				{
					//修改用户组
					ImSetUserGroup(pFriend->GetId(),nDestGroupId);
				}
				
				pFriend->group_index = nDestGroupId;
				m_pFriendList->MoveItem(nGroup,nItem,nDestGroupId);
				SortGroupList(nDestGroupId);

				m_pFriendList->SetCurSel(pFriend);
				
			}

			UpdateFriendCnt();
		}
		break;
	case NM_FL_GROUPNAME://组名称修改
		{
			int nGroupId = (m_pFriendList->GetGroupData(lParam))->dwData;
			CString strGroupName = m_pFriendList->GetGroupName(lParam);
			
			int i,index=-1;
			for (  i=0; i<g_GroupInfo.count; i++ )
			{
				if ( nGroupId==g_GroupInfo.group[i].group_index )
				{
					index=i;
				}	
				else
				{
					//不能有重名
					if ( strGroupName==g_GroupInfo.group[i].name )
					{
						index=-1;
						break;
					}
				}
			}

			if ( index!=-1 && strGroupName!=g_GroupInfo.group[index].name )
			{
				sprintf(g_GroupInfo.group[index].name,strGroupName);
				ImRenameGroup(nGroupId,strGroupName);
			}
		}
		break;
	case NM_FL_RBUTTONUP:
		{
			CMenuEx menu, *popmenu;

			menu.LoadMenu(IDR_MENU_TEMPLATE);
			popmenu = menu.GetSubMenu(0);
			popmenu->RemoveMenu(0,MF_BYPOSITION);

			CPoint point;
			GetCursorPos(&point);

			TRACE("NM_FL_RBUTTONUP\n");

			// 根据不同情况来显示弹出菜单的内容

			// 判断当前是否有选中的组
			if ( nGroup > -1 )
			{
				if ( nItem > -1 ) //好友节点
				{
					CString strItem;
					
					CString strGroup = m_pFriendList->GetGroupName(nGroup);
#ifdef _CS_PLAT_EX_
					if(strGroup != "最近联系客户" && strGroup!= "黑名单")
#else
					if(strGroup != "陌生人" && strGroup!= "黑名单")
#endif
					{
						if(pFriend->GetId() != g_ImUserInfo.GetId())
						{
							strItem.Format("发送即时消息");
							popmenu->AppendMenu(MF_STRING,ID_P2PSENDMSG,strItem);
							//popmenu->AppendMenu(MF_SEPARATOR,0,"");
						}

						strItem.Format("修改备注名");
						popmenu->AppendMenu(MF_STRING,ID_MODF_COMMNAME,strItem);
						
						if(pFriend->GetId() != g_ImUserInfo.GetId())
						{
							//popmenu->AppendMenu(MF_SEPARATOR,0,"");
						}

						//飞牛网组织架构中的不允许移动
						if(nGroup < FEINIU_GROUP_ID && nGroup > -1)
						{
							strItem.Format("删除联系人");
							popmenu->AppendMenu(MF_STRING,ID_DELFRIEND,strItem);
							//popmenu->AppendMenu(MF_SEPARATOR,0,"");
							
							CMenu menuGroup;
							menuGroup.CreatePopupMenu();
							//飞牛网组织架构中的不允许移动
							for(int i=0; i<g_GroupInfo.count;i++)//////修改判断逻辑，组织架构不允许变动 2014-11-12
							{
								if(g_GroupInfo.group[i].group_index < FEINIU_GROUP_ID)
								{
									//陌生人和黑名单去掉
									if(/*g_GroupInfo.group[i].group_index!=1&&*/g_GroupInfo.group[i].group_index!=2)
									{
				                       menuGroup.AppendMenu(MF_STRING,ID_GROUP_CHOOSE_BEG + i,g_GroupInfo.group[i].name);
									}
								}
							}
							popmenu->AppendMenu(MF_POPUP, (UINT)menuGroup.m_hMenu, "把好友移动到...");
							//popmenu->AppendMenu(MF_SEPARATOR,0,"");

								
							strItem.Format("移动至黑名单");
							popmenu->AppendMenu(MF_STRING,ID_REMOVE_BLACKLIST,strItem);
							//popmenu->AppendMenu(MF_SEPARATOR,0,"");
						}

						
						if(pFriend->GetId() != g_ImUserInfo.GetId())
						{
							strItem.Format("查看资料");
							popmenu->AppendMenu(MF_STRING,ID_FRIENDINFO,strItem);
						}
					}
					else
					{
						popmenu->AppendMenu(MF_STRING,ID_DELFRIEND,"从该组删除");
						//popmenu->AppendMenu(MF_SEPARATOR,0,"");
                #ifndef  _CS_PLAT_EX_
						popmenu->AppendMenu(MF_STRING,ID_ADD_BLACKLIST,"加为好友");
						//popmenu->AppendMenu(MF_SEPARATOR,0,"");
                #endif
						popmenu->AppendMenu(MF_STRING,ID_FRIENDINFO,"查看资料");
					}
				}
				else//组节点
				{
					if ( g_ImUserInfo.status!=XTREAM_OFFLINE )
					{
						CString strGroup = m_pFriendList->GetGroupName(nGroup);

						if (nGroup == MY_TEAM_GROUP_ID)
						{	// 我的团队分组只有"向组员群发消息"
							popmenu->AppendMenu(MF_STRING, ID_SEND_GROUP_MESSAGE, "向组员群发消息");
						}
						else
						{
							if (strGroup != "陌生人" && strGroup != "黑名单")
							{
								popmenu->AppendMenu(MF_STRING, ID_SEND_GROUP_MESSAGE, "向组员群发消息");
								//飞牛网组织架构中的不允许移动
								if (nGroup < 100 && nGroup > -1)
								{
                         
                            #ifndef _CS_PLAT_EX_
									//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
									popmenu->AppendMenu(MF_STRING, ID_ADDGROUP, "添加组");
									//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
									popmenu->AppendMenu(MF_STRING, ID_ADD_CGROUP, "添加子组");
									//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
									popmenu->AppendMenu(MF_STRING, ID_ADD_FRIEND, "添加好友");
                            #endif  
									//我的好友不允许编辑
									if (nGroup != 0)
									{
										//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
										popmenu->AppendMenu(MF_STRING, ID_RENAME_GROUP, "重命名组");
										//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
										popmenu->AppendMenu(MF_STRING, ID_DELGROUP, "删除组");
									}
								}
							}
						}					
					}
				}
			}
			if(popmenu->GetMenuItemCount() > 0)
			{
				DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
				DuiLib::CPoint duiPoint(point.x,point.y);
				//ClientToScreen(m_hWnd, &point);
				pMenu->Init(NULL, _T("loginstatus.xml"), point, &m_PaintManager);
				AppendDuiMenu(pMenu, popmenu);

				//CFlatPopupMenu flatmenu;
				//flatmenu.Create(AfxGetInstanceHandle());
				//flatmenu.AppendMenu(popmenu);
				//UINT id = flatmenu.Track(point.x, point.y, GetHWND(), true);
				//// 相应选择菜单的消息相应
				//DWORD wP = MAKEWPARAM(id, 0);
				//SendMessage(WM_COMMAND, wP, 0);
			}			
		}
		break;
		/*case NM_FL_ITEM_OVER:
		{
		KillTimer(TIMER_USER_CARD);
		m_hotItem.pos  = *(FL_ITEM_POS *)lParam;

		if ( m_hotItem.pos.nGroupID!=-1 && m_hotItem.pos.nItem!=-1 )
		{
		m_hotItem.rect = m_pFriendList->GetItemRect(m_hotItem.pos.nGroupID,m_hotItem.pos.nItem);
		m_pFriendList->ClientToScreen(&m_hotItem.rect);

		SetTimer(TIMER_USER_CARD,100,NULL);
		}
		}
		break;*/
		/*case NM_FL_HEADICON_CLICK:
		{
		FL_ITEM_POS *p = (FL_ITEM_POS *)lParam;
		pFriend = (FRIEND_INFO *) m_pFriendList->GetItemData(p->nGroupID,p->nItem);
		}
		break;
		case NM_FL_BLOG_CLICK :
		{
		FL_ITEM_POS *p = (FL_ITEM_POS *)lParam;
		pFriend = (FRIEND_INFO *) m_pFriendList->GetItemData(p->nGroupID,p->nItem);
		ShowBlogSite((char*)pFriend->GetUserName().c_str());
		}
		break;*/
		/*case NM_FL_INFO_CLICK:
		{
		if ( pFriend )
		CFriendInfoDlg::Show(pFriend);
		}
		break;*/
	//case NM_FL_ALBUM_CLICK:
	//	{
	//		if ( pFriend && (g_pWebConfig != NULL))
	//		{
	//			MiniOpenUrl((LPCSTR)g_pWebConfig->GetWebPageUrl(USER_PHOTO,StrToWideStr(pFriend->GetUserName().c_str())));
	//		//	g_pUserPictures->Show(pFriend->id,(LPCSTR)pFriend->username,(LPCSTR)pFriend->nickname);
	//		//	CFriendPictureViewDlg::Show(pFriend->id,0);
	//		}
	//	}
	//	break;
	//case NM_FL_ARTICLE_CLICK:
	//	{
	//		if ( pFriend &&(g_pWebConfig != NULL))
	//		{
	//			MiniOpenUrl((LPCSTR)g_pWebConfig->GetWebPageUrl(USER_ARTICLE,StrToWideStr(pFriend->GetUserName().c_str())));
	//	
	//			//g_pUserArticles->Show(pFriend->id,(LPCSTR)pFriend->username,(LPCSTR)pFriend->nickname);
	//		}
	//	}
	//	break;
	//case NM_FL_VEDIO_CLICK:
	//	{
	//		FL_ITEM_POS *pPos = (FL_ITEM_POS *)lParam;
	//		pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(pPos->nGroupID,pPos->nItem);

	//		if ( pFriend )
	//		{
	//			CDuiP2PSendMsgDlg *pDlg = ShowMessageDlg(pFriend);
	//			pDlg->OnBnClickedBtnVedio();
	//		}
	//	}
	//	break;
	//case NM_FL_MOBILE_CLICK:
	//	{
	//		FL_ITEM_POS *pPos = (FL_ITEM_POS *)lParam;
	//		pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(pPos->nGroupID,pPos->nItem);

	//		CSMSDlg::Show(pFriend);
	//		CUserCardDlg::CloseWindwow();
	//		//KillTimer(TIMER_USER_CARD);
	//	}
	//	break;
	default:break;
	}

	return 0;
}

void CMainFrame::OnSendMsg()
{
	FL_ITEM_POS pos = m_pFriendList->GetCurSel();

	if ( pos.nItem!=-1 && pos.nGroupID!=-1 )
	{
		////////////////////////////////////////////////////////////////////////////////
		////首先弹出未读消息联系人
		/*WORD nExitWarn = 0;
		g_LocalData.GetCloseTalkWarnConfig(nExitWarn);///读取配置是否弹出未读消息窗口*/

		FRIEND_INFO *pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(pos.nGroupID,pos.nItem);
		OpenP2PMsgDlg(pFriend);	
	}
	else if(pos.nGroupID != -1)////展开组
	{
		BOOL bExpand = m_pFriendList->IsExpand(pos.nGroupID);
		m_pFriendList->ExpandGroup(pos.nGroupID, !bExpand);
	}
}

void CMainFrame::PopUnReadMsgDlg(FRIEND_INFO* pShowFriend)
{
	////显示未读消息窗口
	map<FRIEND_INFO *,uint32>::iterator it;
	if(g_mapMsgUnRead.size() == 0)
	{
		return ;
	}

	for(it = g_mapMsgUnRead.begin();it!=g_mapMsgUnRead.end();it++)
	{
		FRIEND_INFO *pFriend = it->first;
		int nUnReadCnt =  it->second;
		CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend,StruExtraInfo(),nUnReadCnt);
		if ( pDlg->GetHWND()==NULL )////没有打开窗口
		{
			pDlg->NotifyServer_OpenedMsgDlg();			
		}
#ifdef _CS_PLAT_EX_
		if( CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()) && CMainFrame::IsBuyer(pFriend->GetId()))//有买家和卖家的对话最前展示
		{
			CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg,nUnReadCnt);
		}
		else if(CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()) && !CMainFrame::IsBuyer(pFriend->GetId()))
		{
			g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);//创建未读消息的窗口
			CDuiCSPlatDlg::GetInstance()->SetFriendUnMsg(pFriend, true);//设置好友未读消息标记
		}
#else
		if( CDuiCounsel::GetInstance() != NULL && ::IsWindowVisible(CDuiCounsel::GetInstance()->GetHWND()) && CMainFrame::IsBuyer(pFriend->GetId()))//有买家和卖家的对话最前展示
		{
			CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg,nUnReadCnt);
		}
#endif
		else////没有咨询框最前展示
		{
			if ( /*pDlg->*/::IsWindowVisible(pDlg->GetHWND())==FALSE )
			{
				pDlg->CenterWindow();
				pDlg->ShowWindow(SW_SHOW);
			}
		}

		if (/*pDlg->IsIconic()*/::IsMinimized(pDlg->GetHWND()) )
		{
			pDlg->ShowWindow(SW_RESTORE);
		}

		//CRect rcDlg;
		///*pDlg->*/::GetWindowRect(pDlg->GetHWND(), &rcDlg);
		//::SetWindowPos(pDlg->GetHWND(), HWND_TOPMOST, rcDlg.left, rcDlg.top, rcDlg.Width(), rcDlg.Height(),SWP_SHOWWINDOW);
		//::SetWindowPos(pDlg->GetHWND(), HWND_NOTOPMOST, rcDlg.left, rcDlg.top, rcDlg.Width(), rcDlg.Height(),SWP_SHOWWINDOW);
#ifdef _CS_PLAT_EX_
		if( CDuiCSPlatDlg::GetInstance() != NULL && CMainFrame::IsBuyer(pFriend->GetId()))
		{
			CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg,nUnReadCnt);
		}
		/*pDlg->*/::SetForegroundWindow(pDlg->GetHWND());
		/*pDlg->*/::BringWindowToTop(pDlg->GetHWND());
		if( CDuiCSPlatDlg::GetInstance() != NULL && CMainFrame::IsBuyer(pFriend->GetId()))
		{
			CRect rcCounsel;
			::GetWindowRect(CDuiCSPlatDlg::GetInstance()->GetHWND(), &rcCounsel);
			::SetWindowPos(CDuiCSPlatDlg::GetInstance()->GetHWND(), pDlg->GetHWND(), rcCounsel.left, rcCounsel.top, rcCounsel.Width(), rcCounsel.Height(),SWP_NOACTIVATE);
			if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() > 0)
				CDuiCSPlatDlg::GetInstance()->ShowWindow(true);
		}
#elif defined _WORK_EX_
		if( CDuiCounsel::GetInstance() != NULL && (CMainFrame::IsSeller(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())))
		{
			CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg,nUnReadCnt);
		}
		/*pDlg->*/::SetForegroundWindow(pDlg->GetHWND());
		/*pDlg->*/::BringWindowToTop(pDlg->GetHWND());
		if( CDuiCounsel::GetInstance() != NULL && (CMainFrame::IsSeller(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())))
		{
			CRect rcCounsel;
			::GetWindowRect(CDuiCounsel::GetInstance()->GetHWND(), &rcCounsel);
			::SetWindowPos(CDuiCounsel::GetInstance()->GetHWND(), pDlg->GetHWND(), rcCounsel.left, rcCounsel.top, rcCounsel.Width(), rcCounsel.Height(),SWP_NOACTIVATE);
			if(CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() > 0)
				CDuiCounsel::GetInstance()->ShowWindow(true);
		}
#else
		if( CDuiCounsel::GetInstance() != NULL && (CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())))
		{
			CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg,nUnReadCnt);
		}
		/*pDlg->*/::SetForegroundWindow(pDlg->GetHWND());
		/*pDlg->*/::BringWindowToTop(pDlg->GetHWND());
		if( CDuiCounsel::GetInstance() != NULL && (CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())))
		{
			CRect rcCounsel;
			::GetWindowRect(CDuiCounsel::GetInstance()->GetHWND(), &rcCounsel);
			::SetWindowPos(CDuiCounsel::GetInstance()->GetHWND(), pDlg->GetHWND(), rcCounsel.left, rcCounsel.top, rcCounsel.Width(), rcCounsel.Height(),SWP_NOACTIVATE);
			if(CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() > 0)
				CDuiCounsel::GetInstance()->ShowWindow(true);
		}
#endif
	}

	if(pShowFriend != NULL)
	{
		map<FRIEND_INFO *,uint32>::iterator itrShow;
		itrShow = g_mapMsgUnRead.begin();
		for(; itrShow != g_mapMsgUnRead.end(); itrShow++)
		{
			if(itrShow->first->GetId() == pShowFriend->GetId())
			{
#ifdef _CS_PLAT_EX_
				CDuiCSPlatDlg::GetInstance()->SetFriendListElement(itrShow->first, false, false, false, false, true);//true-->false
#else
				CDuiCounsel::GetInstance()->SetFriendListElement(itrShow->first, false, false, false, false, true);//true-->false
#endif
				break;
			}
		}
	}

	if(g_mapMsgUnRead.size()>0)
	{
	   g_mapMsgUnRead.clear();////重启或者再次打开完成后清空
	   WORD nVK = 0;
	   g_LocalData.SaveLoginUnReadMsgCfg(nVK);///设置未读消息数为0
	}
}


void CMainFrame::OnFriendInfo()
{
	int nGroup = m_pFriendList->GetCurSel().nGroupID;
	int nItem  = m_pFriendList->GetCurSel().nItem;

	if ( nGroup==-1 || nItem==-1 )
	{
		return ;
	}

	FRIEND_INFO *pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,nItem);
	
	CFriendInfoDlg::NewDlg(pFriend);
}

void CMainFrame::OnFriendBlog()
{
	int nGroup = m_pFriendList->GetCurSel().nGroupID;
	int nItem  = m_pFriendList->GetCurSel().nItem;

	if ( nGroup==-1 || nItem==-1 )
	{
		return ;
	}

	RegUserOperation(0x0020);
	
	FRIEND_INFO *pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,nItem);
	::ShowBlogSite((char*)pFriend->GetUserName().c_str());
}

void CMainFrame::OnFriendArticle()
{
	int nGroup = m_pFriendList->GetCurSel().nGroupID;
	int nItem  = m_pFriendList->GetCurSel().nItem;

	if ( nGroup==-1 || nItem==-1 )
	{
		return ;
	}

	RegUserOperation(0x001c);

	FRIEND_INFO *pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,nItem);
	::ShowArticle((char*)pFriend->GetUserName().c_str());
}

void CMainFrame::OnFriendAlbum()
{
	int nGroup = m_pFriendList->GetCurSel().nGroupID;
	int nItem  = m_pFriendList->GetCurSel().nItem;

	if ( nGroup==-1 || nItem==-1 )
	{
		return ;
	}

	RegUserOperation(0x001d);

	FRIEND_INFO *pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,nItem);
	::ShowAlbum((char*)pFriend->GetUserName().c_str());
}

void CMainFrame::OnDelFriend()
{
	int nGroup = m_pFriendList->GetCurSel().nGroupID;
	int nItem  = m_pFriendList->GetCurSel().nItem;

	if ( nGroup==-1 || nItem==-1 )
	{
		return ;
	}

	FRIEND_INFO *pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,nItem);

	if(pFriend == NULL)
		return;

#ifndef _CS_PLAT_EX_
	if(g_pMessageHandleWnd != NULL)
	{
		for(int nDlg = 0; g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
		{
			CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg];
			if(pDlg != NULL)
			{
				if(pDlg->GetFriendInfo()->GetId() == pFriend->GetId())
				{
					CMessageBox::Model(GetHWND(), _T("存在和该用户的会话，请关闭会话再删除~"), IM_CAPTION, MB_ICONWARNING);
					return;
				}
			}
			
		}
	}
	
#endif

	if(pFriend->group_index > FEINIU_GROUP_ID)
	{
		CMessageBox::Model(GetHWND(), _T("飞牛网中的好友不能删除的哦~~"), IM_CAPTION, MB_ICONWARNING);
	}

	
	CString str;
	str.Format("确定要删除 %s 吗?",UserInfoToStr(pFriend));

	if (CMessageBox::Model(GetHWND(),str,IM_CAPTION,MB_YESNO|MB_ICONWARNING)!=IDYES)
	{
		return ;
	}

	//删除用户
	XT_FRIENDGROUP_MOD_REQ req;

	req.action = GROUP_DEL;
	req.group_id = 0;
	req.fid = pFriend->GetId();
	req.uid = g_ImUserInfo.GetId();

	if(pFriend->group_index != 1)
	{
		//用用户组id
		g_pProtocol->SendReq(req);
	}
	//删除用户
	::RegUserOperation(0x00c3);

	DelFriend(pFriend);
}

void CMainFrame::OnSearchXtream()
{
	tr1::shared_ptr<CAddFriendOrClusterDlg> pDlg = CAddFriendOrClusterDlg::Show();
	pDlg->SetSearchType(FS_XTREAM);

}

// 获取新的组ID（自定义分组时用到），返回-1表示失败
int CMainFrame::GetNewGroupID()
{
	int nGroupId = c_minGroupID;

	//查找能够使用的groupId
	list<int> listGroupId;
	for (int i = 0; i < g_GroupInfo.count; i++)
	{
		listGroupId.push_back(g_GroupInfo.group[i].group_index);
	}
	listGroupId.sort();
	list<int>::iterator it;
	int nLastId = -1;
	for (it = listGroupId.begin(); it != listGroupId.end(); it++)
	{
		if (*it != nLastId + 1)
		{
			break;
		}
		nLastId = *it;
	}
	nGroupId = nLastId + 1;

	if (nGroupId <= c_minGroupID)
	{	// 控制自定义生成的组ID不要占用默认分组的ID
		nGroupId = c_minGroupID;
	}
	if (nGroupId > c_maxGroupID)
	{	// 分组ID越界
		return -1;
	}

	// 避免组id重复
	while(std::find(listGroupId.begin(), listGroupId.end(), nGroupId) != listGroupId.end())
	{
		nGroupId += 1;
	}
	if (nGroupId > c_maxGroupID)
	{	// 分组ID越界
		return -1;
	}

	return nGroupId;
}

void CMainFrame::CreateGroup(int nParentGroupId, int& nGroupId, CString& strGroupName)
{
	if ( g_GroupInfo.count > 50 )
	{
		ShowMessage("最多只能建50个自定义组！",MB_OK|MB_ICONINFORMATION);
		return ;
	}

	// 获取新组ID
	nGroupId = GetNewGroupID();
	if(nGroupId == -1)
	{	// 创建组id失败
		ShowMessage("创建自定义分组失败！",MB_OK|MB_ICONINFORMATION);
		return;
	}

	//查找是否有重命名的组	这个逻辑是有bug的
	//支持同样的名字，又怕啥?
	if(g_GroupInfo.FindItemIndex(strGroupName) != -1)
	{
		BOOL bGet = FALSE;
		int nCount = 0;
		while(!bGet && nCount <10)
		{
			for(int i = 0; i < 10; i++)
			{
				CString strNewName;
				strNewName.Format("%s%d", strGroupName, i);
				if(g_GroupInfo.FindItemIndex(strNewName) == -1)
				{
					strGroupName = strNewName;
					bGet = TRUE;
					break;
				}
			}

			if(!bGet)
			{
				for(int i = 'A'; i < 'z'+1; i++)
				{
					CString strNewName;
					strNewName.Format("%s%c", strGroupName, i);
					if(g_GroupInfo.FindItemIndex(strNewName) == -1)
					{
						strGroupName = strNewName;
						bGet = TRUE;
						break;
					}
				}
			}

			nCount++;
		}
	}
	
	//在g_GroupInfo中添加新组
	int nIndex = g_GroupInfo.count;
	g_GroupInfo.group[nIndex].group_index = nGroupId;
	g_GroupInfo.group[nIndex].group_father = nParentGroupId;
	sprintf(g_GroupInfo.group[nIndex].name,strGroupName);
	g_GroupInfo.count++;
	g_GroupInfo.Sort();	
	
}
void CMainFrame::OnAddGroup()
{
	//当前选择的组及其父组
	int nSelGroupId = m_pFriendList->GetCurSel().nGroupID;
	int nParentGroupId = -1;
	for(int i=0; i<g_GroupInfo.count; i++)
	{
		if(nSelGroupId == g_GroupInfo.group[i].group_index)
		{
			nParentGroupId = g_GroupInfo.group[i].group_father;
			break;
		}
	}
	
	//新建组
	int nNewGroupId = -1;
	CString strNewGroupName = "新建组";
	CreateGroup(nParentGroupId, nNewGroupId, strNewGroupName);

	//获取当前选择组的父组为新建组的父组
	
	if(nNewGroupId != -1)
	{
		m_pFriendList->InsertGroup(nParentGroupId , nNewGroupId, strNewGroupName);
		m_pFriendList->SetCurSel(nNewGroupId, -1);
		SetTimer(GetHWND(), TIMER_EDITGOUP, 20, NULL);

		ImAddGroup(nParentGroupId, nNewGroupId, strNewGroupName);
	}

	m_pFriendList->SelectGroup(nSelGroupId, FALSE);
}



//添加子组
void CMainFrame::OnAddSubGroup()
{
	//当前选择的组及其父组
	int nSelGroupId = m_pFriendList->GetCurSel().nGroupID;
	int nParentGroupId = nSelGroupId;

	//新建组
	int nNewGroupId = -1;
	CString strNewGroupName = "新建子组";
	CreateGroup(nParentGroupId, nNewGroupId, strNewGroupName);

	if(nNewGroupId != -1)
	{
		m_pFriendList->InsertGroup(nParentGroupId , nNewGroupId, strNewGroupName);
		//父组打开
		m_pFriendList->SetGroupState(nParentGroupId,FLS_EXPAND);
		m_pFriendList->SetCurSel(nNewGroupId, -1);
		SetTimer(GetHWND(), TIMER_EDITGOUP, 20, NULL);

		ImAddGroup(nParentGroupId, nNewGroupId, strNewGroupName);
	}

	m_pFriendList->SelectGroup(nSelGroupId, FALSE);
}


void CMainFrame::OnRenameGroup()
{
	int nGroup = m_pFriendList->GetCurSel().nGroupID;
	m_pFriendList->ShowEditBox(nGroup);
}

//向组员发消息
void CMainFrame::OnSendGroupMessage()
{
	int nGroup = m_pFriendList->GetCurSel().nGroupID;
	FL_GROUP_INFO *pGroup = m_pFriendList->GetGroupData(nGroup);
	if(pGroup == NULL)
		return;

	// 同时将子组的好友也添加到聊天窗口
	vector<FL_GROUP_INFO*> vecSubGroup;
	pGroup->GetSubGroup(vecSubGroup);
	int nGroupIndex = pGroup->dwData;
	FRIEND_LIST::iterator it,itE;
	it=g_lsFriend.begin();
	itE=g_lsFriend.end();
	// 如果选择的是陌生人和黑名单则不处理
	if(nGroupIndex == 1 || nGroupIndex == 2)
	{
		return ;
	}
	// 找到该组内的好友
	DuiLib::CTreeNodeUI *pGroupNode = m_pFriendList->FindGroupNode(pGroup->dwData);
	vector<int> vecFriendID;
	int nChildCount = pGroupNode->GetCountChild();
	for(int i = 0; i < nChildCount; ++i)
	{
		DuiLib::CTreeNodeUI *pChildNode = pGroupNode->GetChildNode(i);
		if(!m_pFriendList->IsGroupItem(pChildNode))
		{
			FRIEND_INFO *pFriend = (FRIEND_INFO*)pChildNode->GetTag();
			if(pFriend->GetId() != g_ImUserInfo.GetId())
			{
				vecFriendID.push_back(pFriend->GetId());
			}
		}
	}

	CGroupSendDlg::Show(vecFriendID);
}



void CMainFrame::OnDelGroup()
{
	int group_id = m_pFriendList->GetCurSel().nGroupID;
	if( -1 == group_id )
	{
		return;
	}

	FL_GROUP_INFO* pGroup = m_pFriendList->GetGroupData(group_id);

	if(pGroup == NULL)
		return;

	if(group_id > FEINIU_GROUP_ID)
	{
		CMessageBox::Model(GetHWND(), _T("飞牛网中的分组不能删除的哦~~"), IM_CAPTION, MB_ICONWARNING);
	}
	//判断组内是否有好友
	if(pGroup->nFrdCnt > 0)
	{
		ShowMessage("需要把组及其子组中的好友清空或移动到其它组，才能删除这个组");
	}
	else
	{
		CString str;
		str.Format("您确定要删除此组吗？如果有子组，也会被删除");
		if ( IDYES == CMessageBox::Model(GetHWND(), str, IM_CAPTION, MB_YESNO|MB_ICONWARNING) )
		{

			//获取父组id
			int iGroupInd = -2,iParentGroupId = -2;
			iGroupInd = g_GroupInfo.FindItemIndex(group_id);
			iParentGroupId = g_GroupInfo.group[iGroupInd].group_father;

			//获取所有子组，其实是需要所有子组id
			vector<FL_GROUP_INFO*> vecSubGroup;
			pGroup->GetSubGroup(vecSubGroup);
			//把自己放进去
			//删除g_GroupInfo 中该组及其所有子组
			bool bDelError = false;	

			vector<_FL_GROUP_INFO*>::iterator it = vecSubGroup.begin();
			for (; it < vecSubGroup.end(); it++)
			{
				assert((*it)->nFrdCnt == 0);
				if(g_GroupInfo.DelGroup((*it)->dwData) == -1)
				{
					bDelError = true;
					assert(bDelError == false);
					break;
				}
				ImDelGroup((*it)->dwData);
				//sleep(200);
			}
		
			if(!bDelError && g_GroupInfo.DelGroup(group_id) != -1)
			{
				ImDelGroup(group_id);
			}
			else
			{
				bDelError = true;
			}
			if(bDelError || iParentGroupId == -2 || m_pFriendList->DelGroup(iParentGroupId, group_id) == -1)
			{
				ShowMessage("删除组失败!~");
				return;
			}
						
			//g_GroupInfo存本地
			g_LocalData.SaveGroupInfo(g_GroupInfo);
		}
	}

	m_pFriendList->SelectGroup(group_id, FALSE);
}

void CMainFrame::OnMsgHistory()
{
	FL_ITEM_POS pos = m_pFriendList->GetCurSel();

	if ( pos.nGroupID!=-1 && pos.nItem!=-1 )
	{
		FRIEND_INFO *pFriend =(FRIEND_INFO *)m_pFriendList->GetItemData(pos.nGroupID,pos.nItem);
		std::tr1::shared_ptr<CDuiMsgDlg> pDlg = CDuiMsgDlg::Show();
		pDlg->SetDefaultFriend(pFriend->GetId());
	}
}

void CMainFrame::ChangeUserStatus(uint8 status)
{
	if ( status==g_ImUserInfo.status)
	{
		return ;
	}
	else if (status==XTREAM_OFFLINE)
	{	
		KillTimer(GetHWND(), TIMER_BEAT);	
		SetUserOffLineFlag();
		return ;
	}

	//上线
	if ( g_ImUserInfo.status==XTREAM_OFFLINE )
	{
		if ( g_pProtocol->IsRunning()==false )
		{
			g_pProtocol->Start();
		}
	
		XT_LOGIN_REQ loginInfo;	

		if ( m_bDisconnectFlag )
		{
			loginInfo.status = m_nPreOnlineStatus;
			m_bDisconnectFlag = false;
		}
		else
		{
			loginInfo.status     = status;
		}

		loginInfo.login_flag |= CLoginDlg::m_LoginFlag; //// 保留登录的初始状态 
		////loginInfo.login_flag |= XT_LOGIN_REQ::LOGINBYNAME;
		strcpy(loginInfo.uLoginKey.username ,g_ImUserInfo.GetUserName().c_str());
		// 密码解密
		strcpy(loginInfo.password,g_ImUserInfo.GetPassword().c_str());		
		g_pProtocol->SendReq(loginInfo);
	}
	else
	{
		if(status != g_ImUserInfo.status)
		{
			XT_STATUS_MOD_REQ req;

			req.id     = g_ImUserInfo.GetId();
			req.status = status;

			g_pProtocol->SendReq(req);
		}		
	}
}

//好友状态更改通知
void CMainFrame::OnFriendStatusChange(const char *data,WORD wRet)
{
	RecentList_UserStatusInd(data, wRet);
	if ( wRet==0)
	{
		LPXT_STATUS_IND ind =(LPXT_STATUS_IND)data;
		FRIEND_INFO *pFriend = FindFriend(ind->fid);

		//如果是隐身的话，则状态图标和离开一样
		if ( ind->status == XTREAM_HIDE )
		{
			ind->status = XTREAM_OFFLINE;
		}

		if ( pFriend )
		{
			pFriend->ver = ind->version;
            //好友上线
			if ( ind->status == XTREAM_ONLINE && pFriend->status != XTREAM_ONLINE)
			{
#ifdef _VER_INTERNAL_	// 内部版
#else			
  #ifdef  _CS_PLAT_EX_///飞牛客服工作台
  #else
				PlayTone(EVOICE_FRIEND_ONLINE);
				ShowUserHintDlg(pFriend,UserInfoToStr(pFriend), "上线了", m_hWnd);
  #endif
#endif
				//NotifyVideoEquipment(pFriend);				
			}

			if ( ind->status != pFriend->status )
			{
			/*
				if ( pFriend->status==XTREAM_OFFLINE )
					g_webUserHeadIconProcess.GetUserHeadIcon(pFriend,this);	
			*/
				// 更新状态
				pFriend->status = ind->status;
				// 状态改变后需要更新列表的显示
				UpdateUserBitmap(pFriend);
				m_pFriendList->UpdateItemImage(pFriend);

				//m_pFriendList->SetInvalidateLock(true);

				/*
				if ( ind->status==XTREAM_OFFLINE )
					m_pFriendList->DelItemImage(nGroup,nItem);
				*/
#ifdef _CS_PLAT_EX_
				if(CDuiCSPlatDlg::GetInstance() != NULL && IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()) && CDuiCSPlatDlg::GetInstance()->m_vecP2PSendMsgDlg.size() > 0)
				{
					CDuiCSPlatDlg::GetInstance()->UpdateFriendList(pFriend);
				}
#endif
				UpdateFriend(pFriend);


				//m_pFriendList->SetInvalidateLock(false);
			}

			RemoveHideTick(pFriend->GetId());
		}
	}
}

void CMainFrame::OnFriendGroupModAck(const char *data,WORD wRet)
{
	LPXT_FRIENDGROUP_MOD_ACK ack = (LPXT_FRIENDGROUP_MOD_ACK)data;
	char szTemp[20];
	FRIEND_INFO * pFriend;
	if ( wRet==0 )
	{
		//修改组
		if ( ack->action==GROUP_MODIFY )
		{
			//从黑名单->我得好友,自定义组
			if ( ack->flag==0x01 )	//需要验证
			{

				pFriend = GetFriendInfo(ack->fid);
				if(pFriend)
				{
					CUserAuthReqDlg::Show(pFriend,ack->group_id);
				}
				else
				{
					FRIEND_INFO friendInfo;
					friendInfo.SetId(ack->fid);
					friendInfo.SetUserName(itoa(friendInfo.GetId(),szTemp,10));
					strcpy(friendInfo.nickname, friendInfo.GetUserName().c_str());
					CUserAuthReqDlg::Show(&friendInfo,ack->group_id);
				}			
			}
			else if ( ack->flag==0x02 ) //拒绝
			{
				ShowUserAuthInfo(ack->fid,0x02,"",ack->group_id);
			}
			else if( ack->flag == 0x00 )	//允许任何人加为好友
			{
				pFriend = GetFriendInfo(ack->fid);
				if(pFriend)
				{
					pFriend->group_index = ack->group_id;
					// 邀请成功
					pFriend->bInviting = false;
					UpdateFriendList();
				}
			}

		//	if( ack->flag != 0x00 )	//修改组还没有成功,所以要恢复先前好友拖放时界面提前的组修改
		//	{
		//		UpdateFriendList();
		//	}


		}

		if( ack->action == GROUP_DEL )
		{

			pFriend = GetFriendInfo(ack->fid);
			if(pFriend)
			{
				pFriend->group_index = GROUPID_STRANGER;
			}
		}


		/*实际上是添加好友ack，zhangmin*/
		if ( ack->action==GROUP_ADD )
		{
			//添加好友需要验证
			if ( ack->flag==0x01 )
			{
				CString sAllowIni;
				CString sID;
				BOOL bAddFriend = FALSE;
				sAllowIni.Format("%sdata\\%s\\allow.ini",(LPCTSTR)g_LocalData.GetPath(),g_ImUserInfo.GetUserName().c_str());
				sID.Format("%d",ack->fid);
				bAddFriend = ::GetPrivateProfileInt("AddFriends",sID,0,sAllowIni);
				if(bAddFriend)
				{
					// 假如设置了加对方为好友，就直接发送验证信息
					XT_USERAUTH_REQ req;
					req.from_id = g_ImUserInfo.GetId();
					req.groupId = ack->group_id;
					req.to_id   = ack->fid;
					req.action  = XT_USERAUTH_REQ::ACTION_REQUEST;// 请求
					req.send_time = time(NULL);
					strcpy(req.info,"");
					g_pProtocol->SendReq(req);
					::WritePrivateProfileString("AddFriends",sID,"0",sAllowIni);
				}
				else
				{
					// 显示验证提示框
					pFriend = GetFriendInfo(ack->fid);
					if(pFriend)
					{
						CUserAuthReqDlg::Show(pFriend,ack->group_id);
					}
					else
					{
						FRIEND_INFO friendInfo;
						friendInfo.SetId(ack->fid);
						friendInfo.SetUserName(itoa(friendInfo.GetId(),szTemp,10));
						strcpy(friendInfo.nickname, friendInfo.GetUserName().c_str());
						CUserAuthReqDlg::Show(&friendInfo,ack->group_id);
					}
				}
				
			}
			//添加好友被直接拒绝
			else if ( ack->flag==0x02 )
			{
				ShowUserAuthInfo(ack->fid,0x02,"",ack->group_id);
			}
			//添加好友成功
			else if ( ack->flag==0x00 )//成功
			{
				FRIEND_INFO * pFriend ;
				// 判断是否为新添加好友
				pFriend = ::FindFriend(ack->fid);
				ASSERT(pFriend != NULL);

				pFriend->group_index = ack->group_id;
				pFriend->bInviting = false;
				UpdateFriendList();

				ImGetUserStatus(pFriend->GetId());
				if ( m_mapNewFriend.find(pFriend->GetId())==m_mapNewFriend.end() )
					m_mapNewFriend.insert(map<uint32,FRIEND_INFO *>::value_type(pFriend->GetId(),pFriend));	
			}

			if ( ack->flag!=0x00 )
			{
				UpdateFriendList();
			}
		}
	}	
	else
	{
		UpdateFriendList();
		ShowMessage(g_pProtocol->GetLastError());
	}
}

void CMainFrame::OnGroupModAck(const char *data,WORD wRet)
{
	if ( wRet!=0 && wRet!=SERVER_TIMEOUT )
	{
		LPXT_GROUPINFO_MOD_ACK ack = (LPXT_GROUPINFO_MOD_ACK)data;
		if (ack->action==GROUP_MODIFY )
		{
			ShowMessage(g_pProtocol->GetLastError());	
		}
		else if (ack->action==GROUP_ADD)	
		{
			ShowMessage(g_pProtocol->GetLastError());	
		}
	}
}

void CMainFrame::OnUserAuthAck(const char *data,WORD wRet)
{
	if(wRet != 0)
	{
		ShowMessage(g_pProtocol->GetLastError());
	}
}

//好友验证请求\通过\拒绝
void CMainFrame::OnUserAuthInd(const char *data,WORD wRet)
{
	LPXT_USERAUTH_REQ req = (LPXT_USERAUTH_REQ)data;

	if ( wRet==0 && req )
	{
		if ( req->action==0x01 )//同意
		{
			FRIEND_INFO *pFriend;
			pFriend = FindFriend(req->from_id);
			if(pFriend)
			{
				pFriend->group_index = req->groupId;
				pFriend->bInviting = false;
				UpdateFriendList();

			}
			else
			{
				pFriend = new FRIEND_INFO();
				pFriend->SelfEncrypt();
				pFriend->SetId(req->from_id);
				pFriend->status = XTREAM_OFFLINE;
				pFriend->group_index = req->groupId;

				AddFriend(pFriend,false);
				//SelItem(pFriend);

				ImGetUserStatus(pFriend->GetId());

				if ( m_mapNewFriend.find(pFriend->GetId())==m_mapNewFriend.end() )
					m_mapNewFriend.insert(map<uint32,FRIEND_INFO *>::value_type(pFriend->GetId(),pFriend));

				// 获得该好友的详细信息
				XT_USERINFO_GET_REQ userinfo_req;
				userinfo_req.fid = req->from_id;
				g_pProtocol->SendReq(userinfo_req);
			}

		}
		else if(req->action == 0)
		{
			CString sAllowIni;
			CString sID;
			sAllowIni.Format("%sdata\\%s\\allow.ini",(LPCTSTR)g_LocalData.GetPath(),g_ImUserInfo.GetUserName().c_str());
			sID.Format("%d",req->from_id);
			BOOL bAllow;
			bAllow = ::GetPrivateProfileInt("AllowFriends",sID,0,sAllowIni);
			if(bAllow)
			{
				// 收到对方的验证请求，如果之前设置有允许对方加我为好友的设置，则可以立刻同意对方的验证
				XT_USERAUTH_REQ reqAuth;
				reqAuth.action = 0x01;//同意
				reqAuth.from_id = g_ImUserInfo.GetId();
				reqAuth.to_id   = req->from_id;
				reqAuth.groupId = req->groupId;
				reqAuth.send_time = time(NULL);
				strcpy(reqAuth.info,"-");
				g_pProtocol->SendReq(reqAuth);
				::WritePrivateProfileString("AllowFriends",sID,"0",sAllowIni);
				return;
			}
		}

        ShowUserAuthInfo(req->from_id,req->action,req->info,req->groupId);
	}
}

void CMainFrame::StartHeartBeart()
{
	SetTimer(GetHWND(), TIMER_BEAT,1000,NULL);
	m_dwPrevBeatTick = GetTickCount();
	ImHeartBeat();
}

void CMainFrame::SaveLocalFriends()
{
	if ( g_ImUserInfo.GetId() != 0 )
	{
		// groupList中State >> groupInfo
		for(int i=0; i<g_GroupInfo.count; i++ )
		{
			int nSubGroupId = g_GroupInfo.group[i].group_index;
			int nGroupState =  m_pFriendList->GetGroupState(nSubGroupId);
			//组存在
			if(nGroupState != -1)
			{
				g_GroupInfo.group[i].group_state = nGroupState;
			}
			else
			{
				g_GroupInfo.group[i].group_state = FLS_CLOSE;
			}
		}

		g_LocalData.SaveGroupInfo(g_GroupInfo);
		g_LocalData.SaveFriendList(g_lsFriend);
	}
}

void CMainFrame::OnHeartBeat(const char * data,WORD wRet)
{
	LPXT_DIRECTORY_ACK resp = (LPXT_DIRECTORY_ACK)data;
	if(wRet == 0)
	{
		m_dwPrevBeatTick = GetTickCount();
#if _DEBUG
		SYSTEMTIME tm;
		GetLocalTime(&tm);
		CString sTime;
		sTime.Format("%d:%d:%d\n",tm.wHour,tm.wMinute,tm.wSecond);
		TRACE(sTime);
#endif
	}
	
	if(resp->ret == 0xAA)
	{	// 异常，重新登录
		m_bDisconnectFlag = true;
		m_nPreOnlineStatus = g_ImUserInfo.status;

		g_ImUserInfo.status = XTREAM_OFFLINE;
		m_bLogined=false;
		m_nLoginStatus=0;

		g_pProtocol->Stop();

		KillTimer(GetHWND(), TIMER_BEAT);
		SetTimer(GetHWND(), TIMER_RELOGIN,100,NULL);
#ifdef _CS_PLAT_EX_
		g_pMessageHandleWnd->CloseAllMonitor();
#endif
	}
}

void CMainFrame::ImGetUserStatus(uint32 uid)
{
	XT_STATUS_GET_REQ req;
	req.id = g_ImUserInfo.GetId();
	req.fid = uid;

	g_pProtocol->SendReq(req);
}

void CMainFrame::ImSetUserGroup(uint32 fid, int nGroupId, uint8 action)
{
	//修改用户组
	XT_FRIENDGROUP_MOD_REQ req;
	//FRIEND_INFO *pFriend = FindFriend(fid);
	req.action    = action;
	req.group_id  = nGroupId;
	req.uid       = g_ImUserInfo.GetId();
	req.fid = fid;
	g_pProtocol->SendReq(req);
}

void CMainFrame::ImAddGroup(int nParentGroupId,int nGroupId,CString strGroupName)
{
	XT_GROUPINFO_MOD_REQ req;

	req.action   = GROUP_ADD;
	req.group_pid = (nParentGroupId==-1 ? 0xFF : nParentGroupId);
	req.group_id = nGroupId;
	req.uid      = g_ImUserInfo.GetId();
	sprintf(req.name,strGroupName);

	g_pProtocol->SendReq(req);
}

void CMainFrame::ImRenameGroup( int nGroupId, CString strGroupName)
{
	XT_GROUPINFO_MOD_REQ req;
	req.action = GROUP_MODIFY;
	req.group_pid = -2;//后台不会使用
	req.group_id = nGroupId;
	sprintf(req.name,strGroupName);
	req.uid   = g_ImUserInfo.GetId();

	g_pProtocol->SendReq(req);
}

void CMainFrame::ImDelGroup( int nGroupId)
{
	XT_GROUPINFO_MOD_REQ req;

	req.action   = GROUP_DEL;
	req.group_pid = -2;//后台不会使用
	req.group_id = nGroupId;
	sprintf(req.name," ");
	req.uid      = g_ImUserInfo.GetId();

	g_pProtocol->SendReq(req);	
}

void CMainFrame::IMSetFoucs(uint32 fid,uint8 focus)
{
	XT_MODIFY_FRIENDFLAG_REQ req;
	req.friendid = fid;
	req.flag     = focus;
	g_pProtocol->SendReq(req);
}

void CMainFrame::ImGetGroupInfo()
{
	XT_GROUPINFO_GET_REQ req;
	req.group_count = 0;
	req.id = g_ImUserInfo.GetId();
#ifdef _CS_PLAT_EX_
	req.nType = XT_LOGIN_REQ::CS;
#endif

	g_pProtocol->SendReq(req);
}

void CMainFrame::ImSaveUserOperations()
{
	XT_USER_OPERATION_REQ req  ;

	req.count = 0;
	req.uid   = g_ImUserInfo.GetId();
	
	map<uint16,uint16>::iterator it;
	for ( it=g_mapUserOper.begin();
		it!=g_mapUserOper.end();
		it++ )
	{
		req.oper_ids[req.count]=it->first;
		req.oper_cnt[req.count]=it->second;
		req.count++;
	}

	if ( req.count>0 )
	{
		g_mapUserOper.clear();
		g_pProtocol->SendReq(req);
	}
}

void CMainFrame::ImHeartBeat()
{
	XT_DIRECTORY_REQ req;
	req.id = g_ImUserInfo.GetId();
	req.local_port = g_pProtocol->GetUdpCommPort();
	req.local_ip   = g_pProtocol->GetUdpCommIp();
	// 获取发送心跳的序列号
	m_nLastHeartSeq = g_pProtocol->GetCurSendSequnceId();
	g_pProtocol->SendReq(req);
	
}

void CMainFrame::SetUserOffLineFlag()
{
	//SetTrayIcon(XTREAM_OFFLINE);
	g_ImUserInfo.status = XTREAM_OFFLINE;

	FRIEND_LIST::iterator it;
	for ( it=g_lsFriend.begin(); it!=g_lsFriend.end(); it++)
	{
		(*it)->status=XTREAM_OFFLINE;
		UpdateFriend(*it);
	}

	m_lsHideTick.clear();
}

void CMainFrame::SortGroupList(int nGroup)
{
	

	//m_pFriendList->SetInvalidateLock(true);

	m_pFriendList->SortGroupByState(nGroup);

	/*按名字排序，先不整了
	for ( i=0; i<nItemCount; i++ )
	{
		FRIEND_INFO * f1 =(FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,i);
		if((DWORD)f1==-1)
			return;
		if ( f1->status!=XTREAM_OFFLINE )
			nOnline++;
	}

	QuickSortGroupList(nGroup,0,nOnline-1);
	QuickSortGroupList(nGroup,nOnline,nItemCount-1);
*/
	
	//m_pFriendList->SetInvalidateLock(false);
	
	m_pFriendList->Invalidate();
}

void CMainFrame::QuickSortGroupList(int nGroup, int iLo, int iHi)
{
	if ( iLo<iHi )
	{
		char szKey[255];
		strncpy(szKey,((FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,iLo))->nickname,sizeof(szKey)-1);

		int lo = iLo;
		int hi = iHi;

		while (lo<hi)
		{
			FRIEND_INFO *fhi = (FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,hi);

			while ( lo<hi && strcmp(fhi->nickname,szKey)>=0 )
			{
				--hi;
				fhi = (FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,hi);
			}

			m_pFriendList->SwapItem(nGroup,lo,nGroup,hi);

			FRIEND_INFO *flo = (FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,lo);

			while ( lo<hi && strcmp(flo->nickname,szKey)<0)
			{
				++lo;
				flo = (FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,lo);
			}

			m_pFriendList->SwapItem(nGroup,lo,nGroup,hi);
		}

		QuickSortGroupList(nGroup,iLo,lo-1);
		QuickSortGroupList(nGroup,lo+1,iHi);
	}
}

void CMainFrame::SelItem(FRIEND_INFO * pUser)
{
	int nGroup, nItem;
	FindItem(pUser,nGroup,nItem);

	if ( nGroup!=-1 && nItem!=-1 )
	{
		m_pFriendList->SetCurSel(nGroup,nItem);
	}
}


void CMainFrame::FindItem(FRIEND_INFO *pFriend, int &nGroup, int& nItem )
{
	m_pFriendList->FindFriend(pFriend, nGroup, nItem);
}



void CMainFrame::UpdateFriend(FRIEND_INFO *pFriend,BOOL bSort)
{
	if(pFriend->comment_name[0]==0)
		m_pFriendList->SetItemText(pFriend,UserInfoToStr(pFriend));		
	else
		m_pFriendList->SetItemText(pFriend,pFriend->comment_name);
		
	//m_pFriendList->SetItemTextColor(pFriend,StatusToColor(pFriend->status));

	if ( bSort )
		SortGroupList(pFriend->group_index);

	UpdateFriendCnt();

	//好友列表更新后聊天框状态也更新
	CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->FindP2PSendMsgDlgById(pFriend->GetId());
	if(pDlg != NULL && pDlg->GetHWND() != NULL && ::IsWindowVisible(pDlg->GetHWND()))
		pDlg->ChangeHeadStatus(pFriend);
}


void CMainFrame::UpdateFriendCnt()
{
	FRIEND_LIST::iterator it;
	FRIEND_INFO* pFriend;
	int nFriendOnlineCnt = 0, nFriendAllCnt = 0;
	for ( it=g_lsFriend.begin(); it!=g_lsFriend.end(); it++ )
	{
		pFriend = (*it);
		if(pFriend == NULL)
		{
			ASSERT(pFriend);
			continue;
		}
		//统计所有好友数量和在线好友数量(排除黑名单和陌生人)
		if(pFriend->group_index != 1 && pFriend->group_index != 2)
		{
			if(pFriend->status != XTREAM_OFFLINE && pFriend->status != XTREAM_HIDE )
			{
				nFriendOnlineCnt++;
			}
			nFriendAllCnt++;
		}
		
	}
	
	if(nFriendAllCnt > 0)
	{
		m_sFriendCntInfo.Format("%d/%d",nFriendOnlineCnt,nFriendAllCnt);
		m_pFriendPage->SetToolTip(m_sFriendCntInfo);
	}
	else
	{
		m_sFriendCntInfo = "";
	}
}





LRESULT CMainFrame::OnWebIconNotify(WPARAM wParam, LPARAM lParam)
{
	return 0;
	////待删除
	//uint32 fid=wParam;
	//char * szFile = (char *)lParam;
	//int nGroupCount=m_pFriendList->GetGroupCount();

	//for ( int nGroup=0; nGroup<nGroupCount; nGroup++ )
	//{
	//	int nItemCount=m_pFriendList->GetItemCount(nGroup);

	//	for ( int nItem=0; nItem<nItemCount; nItem++ )
	//	{
	//		FRIEND_INFO *pData = (FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,nItem);
	//		if ( pData->id==fid )
	//		{
	//			if ( szFile )
	//			{
	//				UpdateUserBitmap(pData);
	//				m_pFriendList->UpdateItemImage(nGroup, nItem);
	//			}
	//			else
	//			{
	//				m_pFriendList->DelItemImage(nGroup,nItem);
	//			}

	//			return 0;
	//		}
	//	}
	//}

	//return 0;
}

//action=0,请求
//action=1,同意
//action=2,拒绝
//aciton=3,被列为好友通知
void CMainFrame::ShowUserAuthInfo(uint32 fid,uint16 action, CString strVerify,uint32 nGroupId)
{

	FRIEND_INFO *pFriend;
	CString strUserInfo;
	pFriend = FindFriend(fid);
	if(pFriend)
	{
		strUserInfo = UserInfoToStr(pFriend);
		m_pRecentList->UpdateUser(pFriend);
		RECENTLIST_OP(UpdateUser(pFriend))
	}
	else
		strUserInfo.Format("%d",fid);

	CString strMsg;
	switch(action)
	{
		case 0:
			strMsg = strUserInfo + "请求添加您为好友";
			break;
		case 1:
			strMsg = strUserInfo + "接受了您的添加请求";
			break;
		case 2:
			strMsg = strUserInfo + "拒绝了您的添加请求";
			break;
		case 3:
			strMsg = strUserInfo + "成功添加了您为好友";
			break;
		default:
			break;
	}

	
	if( !strMsg.IsEmpty() )
		strMsg += "\r\n 附加信息:" + strVerify;

	SYSTEM_MSG msgSys;
	msgSys.dwTime = time(NULL);
	msgSys.iType  = SYSTEM_MSG::SYSMSG_FRINED;
	sprintf(msgSys.szMessage,strMsg);
	g_LocalData.AddSystemMsg(g_ImUserInfo.GetId(),msgSys);

	CUserAuthInfoDlg * pDlg = new CUserAuthInfoDlg(CWnd::FromHandle(GetHWND()), CWnd::FromHandle(GetDesktopWindow()));
	pDlg->SetUserAuthInfo(fid, action, strVerify,nGroupId);
	pDlg->ShowWindow(SW_HIDE);
	if ( action==0x03 )
	{
		if ( FindFriend(fid) )
		{
			pDlg->m_btnOk.ShowWindow(SW_HIDE);
		}
	}
}

void CMainFrame::OnStatusGetAck(const char *data,WORD wRet)
{
	RecentList_StatusGetAck(data, wRet);
	if ( wRet==0 )
	{
		LPXT_STATUS_GET_ACK ack = (LPXT_STATUS_GET_ACK)data;

		if ( ack )
		{
			FRIEND_INFO *pFriend = FindFriend(ack->id);
			if ( pFriend )
			{
				if ( ack->status==XTREAM_HIDE )
				{
					ack->status=XTREAM_OFFLINE;
				}			
				pFriend->ip     = ack->ip;
				pFriend->port   = ack->port;
				pFriend->localIP = ack->lanip;
				pFriend->localPort = ack->lanport;
				pFriend->status = ack->status;
				pFriend->ver    = ack->ver;
				memcpy(pFriend->key,ack->p2pkey,16);
				if ( pFriend->status==XTREAM_OFFLINE && ack->status!=XTREAM_OFFLINE )
				{
					map<uint32,FRIEND_INFO *>::iterator it;

					it = m_mapNewFriend.find(pFriend->GetId());

					if ( it!=m_mapNewFriend.end() )
					{
						m_mapNewFriend.erase(it);

						CSelectGroupDlg::NewDlg(pFriend->GetId(),CString(pFriend->nickname),m_hWnd);
					}
				}

				pFriend->status = ack->status;

				UpdateFriend(pFriend);
			}
		}
	}
}

void CMainFrame::OnRecvSendFileReq(const char *data,WORD wRet)
{
	P2P_SENDFILE_REQ  & req = *(LPP2P_SENDFILE_REQ)data;
	
	FRIEND_INFO *pFriend = NULL;
	if ( req.friendId == 0 || wRet!=0 )
	{
		return ;
	}

	if ( wRet==0 )
		pFriend=FindFriend(req.friendId);

	if ( pFriend==NULL )
	{
		pFriend = new FRIEND_INFO;
		pFriend->SelfEncrypt();

		pFriend->SetId(req.friendId);
		pFriend->group_index = 1; //陌生人
		pFriend->status = XTREAM_ONLINE;

		//添加到陌生人
		AddFriend(pFriend,true);
		//SelItem(pFriend);
		//ImGetUserStatus(pFriend->GetId());	

#ifdef _BUSINESS_VESION
		CMainFrame::GetInstance()->Hangup();
#endif
	}

	if ( pFriend->status==XTREAM_OFFLINE )
	{
		pFriend->status=XTREAM_ONLINE;
		UpdateFriend(pFriend);

		AddHideTick(pFriend->GetId());
	}

	//屏蔽黑名单
	if ( pFriend->group_index==2 )
	{
		return ;
	}
	
	bool bFindDlg = g_pMessageHandleWnd->FindP2PSendMsgDlg(req.friendId);

	CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);

	if ( pDlg->GetHWND() == NULL )
	{
		pDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, CWnd::FromHandle(GetDesktopWindow()));

		pDlg->ShowWindow(true);	
	}

	CString strTitle;	
	strTitle.Format("%s发送文件:",UserInfoToStr(pFriend));
	ShowUserHintDlg(pFriend, strTitle,req.name,m_hWnd);
	FlashWindow(pDlg->GetHWND(), TRUE);
	pDlg->RecvSendFileReq(data,wRet);

	m_pRecentList->AddUser(pFriend);
	RECENTLIST_OP(AddUser(pFriend))
}

// 接收到其他用户发过来的消息
void CMainFrame::OnRecvP2PMsg(const char *data,WORD wRet)
{
	LPXT_MSG msg = (LPXT_MSG)data;

#ifdef _MSG_JSON_
	 //即时消息的时候尝试Json解析
	if(msg->data_type == XT_MSG::IM_MSG )
	{
		Json::Reader jReader;
		Json::Value jvRecv;
		
		jReader.parse(msg->data,jvRecv);
		std::string strMsgData = jvRecv["txt"].asString();
		uint32 p_id = jvRecv["pid"].asUInt();
		
		//json格式解析成功
		if(strMsgData.length()>0)  
		{
			strcpy(msg->data,strMsgData.c_str());
			msg->data_len = strMsgData.size();
		}
	}
#endif

	FRIEND_INFO *pFriend = NULL;

	if ( msg->from_id==0 )
	{
		return ;
	}

	//以发消息的方式发送自营的订单,一般为转接会话或者主管接手之后转到新客服
	if(msg->data_type==XT_MSG::IM_MSG_ORDER_SEQ)
	{
		CString strOrder = msg->data;//订单格式为 ID:订单号的加密字符串
		int nIndex = strOrder.Find(":");
		CString strId = strOrder.Left(nIndex);
		int nId = atoi(strId);
		CString strOderSeq = strOrder.Right(strOrder.GetLength() - nIndex - 1);
		extern map<int, CString> g_mapFriendIdToOrder;
		g_mapFriendIdToOrder[nId] = strOderSeq;

		for(int nDlg = 0; nDlg < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
		{
			CDuiP2PSendMsgDlg* pDlg = g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg];
			if(pDlg->GetFriendInfo()->GetId() == nId)
			{
				pDlg->UpdateUrl();
				break;
			}
		}
		return;
	}

	if(msg->data_type == XT_MSG::CUST_SER_TRAN_REQ)
	{
		CString strTran = msg->data;
		if(strTran.Find("tranReq") != -1)//转接请求
		{
			strTran = strTran.Right(strTran.GetLength() - 8);
			CString strCusterName = strTran.Left(strTran.Find(":"));
			CString strCusterId = strTran.Right(strTran.GetLength() - strTran.ReverseFind(':') - 1);
			//弹出接收或者拒绝提示框

			FRIEND_INFO* pFromFriend = GetFriendInfo(msg->from_id);
			if(pFromFriend != NULL)
			{
				TranMsg* tranMsgInfo = new TranMsg;
				tranMsgInfo->strCusterName = strCusterName;
				tranMsgInfo->nCusterId = atoi(strCusterId);
				tranMsgInfo->nFromId = msg->from_id;
				tranMsgInfo->strFromName = pFromFriend->nickname;//UserInfoToStr(pFromFriend);
				tranMsgInfo->nTranState = 1;
				tranMsgInfo->time = g_LocalData.GetTimeNow();
				CDuiMsgNotice::Show(tranMsgInfo);
				g_LocalData.SaveTranMsg(tranMsgInfo);
			}
		}

		if(strTran.Find("tranAck") != -1)//转接应答
		{
			int nAccept = atoi(strTran.Right(1));
			strTran = strTran.Mid(8, strTran.GetLength() - 10);
			int nCusterId = atoi(strTran);

			BOOL bCanTran = FALSE;
			CDuiP2PSendMsgDlg* pDlg = NULL;
			for(int nDlg = 0; nDlg < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
			{
				pDlg = g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg];
				if(pDlg->GetFriendInfo()->GetId() == nCusterId && pDlg->CanTran())
				{
					bCanTran = TRUE;
					break;
				}
			}
			if(!bCanTran)//不可以接受
			{
				XT_MSG msgTran;
				memset(&msgTran,0,sizeof(msgTran));
				msgTran.ver         = XTREAM_CLIENT_VERSION;
				msgTran.from_id     = g_ImUserInfo.GetId();
				msgTran.send_time   = /*time(0)*/g_LocalData.GetTimeNow();
				msgTran.to_id		= msg->from_id;
				msgTran.data_type   = XT_MSG::CUST_SER_TRAN_REQ;
				CString text;
				text.Format("tranFail:%d", nCusterId);//接收
				strcpy(msgTran.data, text);
				msgTran.data_len=strlen(text)+1;
				//SF_SetTransMode(CIMProtocol::SMT_SERVER);
				g_pProtocol->SendP2PMsg(msgTran,CIMProtocol::SMT_SERVER);
			}
			else
			{
				if(nAccept == 1)//接受
				{
					pDlg->TranCuster(msg->from_id);
				}
				else if(nAccept == 2)//拒绝
				{
					FRIEND_INFO* pFromFriend = GetFriendInfo(msg->from_id);
					FRIEND_INFO* pCuster = GetFriendInfo(nCusterId);
					TranMsg* tranMsgInfo = new TranMsg;
					tranMsgInfo->strCusterName = UserInfoToStr(pCuster);
					tranMsgInfo->nCusterId = nCusterId;
					tranMsgInfo->nFromId = msg->from_id;
					tranMsgInfo->strFromName = pFromFriend->nickname;//UserInfoToStr(pFromFriend);
					tranMsgInfo->nTranState = 2;
					tranMsgInfo->time = g_LocalData.GetTimeNow();
					CDuiMsgNotice::Show(tranMsgInfo);
					g_LocalData.SaveTranMsg(tranMsgInfo);
					CDuiP2PSendMsgDlg* pDlg = NULL;
					for(int nDlg = 0; nDlg < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
					{
						pDlg = g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg];
						if(pDlg->GetFriendInfo()->GetId() == nCusterId)
						{
							pDlg->AddTranRefuse(tranMsgInfo->strFromName);
							pDlg->SetTranPre(false);
						}
					}
				}
			}
		}

		if(strTran.Find("tranFail") != -1)
		{
			//该会话已完结
			int nCuster = atoi(strTran.Right(strTran.GetLength() - 9));
			if(CDuiMsgNotice::GetInstance() != NULL && IsWindowVisible(CDuiMsgNotice::GetInstance()->GetHWND()))
				CDuiMsgNotice::GetInstance()->DeleteTranMsg(nCuster);
			CMessageBox::Model(NULL, "该会话已完结", NULL, MB_OK);
		}
		return;
	}
	if(msg->data_type == XT_MSG::DEVICE_REPLY)
	{
		int nFriendId = msg->to_id;
		FRIEND_INFO* pFriend = ::GetFriendInfo(nFriendId);
		CDuiP2PSendMsgDlg *pDlg = NULL;
		if( pFriend)
		{
			pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
		}
		else
		{
			FRIEND_INFO friendinfo;
			friendinfo.SetId(nFriendId);

			pFriend = new FRIEND_INFO;
			*pFriend = friendinfo;
			pFriend->group_index = 1;//陌生人
			AddFriend(pFriend, false);
			pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
		}	
		::BringWindowToTop(pDlg->GetHWND());
#ifdef _CS_PLAT_EX_
		CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
#elif defined _WORK_EX_
		CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
#else
		if(CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId()))
			CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
#endif
		pDlg->AddTalkMsg(*msg, false, 0);
		g_LocalData.SaveTalkMsg(*msg,msg->from_id);
		return;
	}
	if ( msg->data_type>XT_MSG::MAX_TYPE)
	{
		return ;
	}

	if ( msg->data_type==XT_MSG::USER_SIGN )
	{
		FRIEND_INFO *pFriend = FindFriend(msg->from_id);
		if ( pFriend )
		{
			strcpy(pFriend->user_sign,msg->data);
			//m_pFriendList->InvalidateList();
			Sleep(100);
			XT_USERINFO_GET_REQ req;
			req.fid = msg->from_id;
			g_pProtocol->SendReq(req);

			//map<uint32,/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *>::iterator it;
			//it = g_pMessageHandleWnd->m_mapP2PSendMsgDlg.find(msg->from_id);

			/////*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *pDlg = NULL;
			//if(it != g_pMessageHandleWnd->m_mapP2PSendMsgDlg.end())
			//{
			//	CDuiP2PSendMsgDlg *pDlg = it->second;

			//	if ( pDlg != NULL && pDlg->GetHWND() != NULL )
			//	{
			//		pDlg->UpdateFriendInfo(pFriend);
			//		pDlg->UpdateSignature();
			//	}
			//}
		}

		return ;
	}

	if ( msg->data_type==XT_MSG::VEDIO_EQUIPMENT )
	{
		FRIEND_INFO *pUser = FindFriend(msg->from_id);
		if ( pUser )
		{
			CRecvPacket rp((char *)msg->data,sizeof(msg->data));
		
			uint8 n ;
			rp>>n;

			if ( n>0 )
			{
				pUser->equipment = pUser->equipment | 0x01;
			}
			else
			if ( n==0 )
			{
				pUser->equipment = pUser->equipment & (~((uint8)0x01) );
			}

			//m_pFriendList->InvalidateList();
		}

		return ;
	}

#ifdef _BUSINESS_VESION
	if ( msg->data_type==XT_MSG::OPEN_P2PSENDMSG_DLG )
	{
#ifdef _CS_PLAT_EX_
		// 修改排队队列
		if(CMainFrame::IsBuyer(msg->from_id))
		{
			CDuiCSPlatDlg::DelWaitFriend(msg->from_id);
		}
#endif

		////网页分流消息
		FRIEND_INFO *pUser = FindFriend(msg->from_id);
		if(pUser == NULL ||
			(pUser != NULL && CMainFrame::IsBuyer(pUser->GetId())))
		{	// 为买家时自动接待，工作版不自动接待
#ifndef _WORK_EX_
			AutoAccept(msg->from_id);
#endif
		}

		StruExtraInfo struExtInfo;
		BOOL bIsShow = FALSE;
		if ( pUser==NULL )
		{
			pUser = new FRIEND_INFO;

			pUser->SetId(msg->from_id);
			pUser->group_index = 1;//陌生人
			pUser->status = XTREAM_ONLINE;
			sprintf(pUser->nickname,msg->from_nickname);

			Json::Reader jReader;
			Json::Value jvRecv;
			
			if (jReader.parse(msg->data, jvRecv))
			{ 
				/*				if(jvRecv.isMember("guid"))
				{
				string strGuid = jvRecv["guid"].asString();
				pUser->SetGuid(strGuid.c_str());
				}			*/	
				pUser->SetGuid("");

				if(jvRecv.isMember("productID"))
				{
					struExtInfo.strProductID = jvRecv["productID"].asString(); 
				}
				
				if(jvRecv.isMember("orderSeq"))
				{
					struExtInfo.strOrderSeq = jvRecv["orderSeq"].asString(); 	
					extern map<int, CString> g_mapFriendIdToOrder;
					g_mapFriendIdToOrder[msg->from_id] = struExtInfo.strOrderSeq.c_str();
				}
			}

			pUser->SetUserName("");
			pUser->SetPassword("");

			//添加到陌生人
			AddFriend(pUser,true);
			SelItem(pUser);
			//ImGetUserStatus(pUser->GetId());
#ifdef _CS_PLAT_EX_
			if(CDuiCSPlatDlg::GetInstance() != NULL && CDuiCSPlatDlg::GetInstance()->m_vecP2PSendMsgDlg.size() > 1 && CMainFrame::IsBuyer(pUser->GetId()))
				bIsShow = FALSE;
			else
				bIsShow = TRUE;
#else
			// 打开聊天窗口
			if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() > 1 && CMainFrame::IsBuyer(pUser->GetId()))
				bIsShow = FALSE;
			else
				bIsShow = TRUE;
#endif

			// 新用户，获取好友资料
			//unsigned threadID = 0;
			//_beginthreadex(NULL, 0, UpdateFriendInfo, (void*)msg->from_id, 0, &threadID);
		}
		else
		{
			Json::Reader jReader;
			Json::Value jvRecv;
			if (jReader.parse(msg->data, jvRecv))
			{ 
				/*				if(jvRecv.isMember("guid"))
				{
				string strGuid = jvRecv["guid"].asString();
				pUser->SetGuid(strGuid.c_str());
				}	*/			
				pUser->SetGuid("");

				if(jvRecv.isMember("productID"))
				{
					struExtInfo.strProductID = jvRecv["productID"].asString(); 
				}

				if(jvRecv.isMember("orderSeq"))
				{
					struExtInfo.strOrderSeq = jvRecv["orderSeq"].asString(); 	
					extern map<int, CString> g_mapFriendIdToOrder;
					g_mapFriendIdToOrder[msg->from_id] = struExtInfo.strOrderSeq.c_str();
				}
			}
#ifdef _CS_PLAT_EX_
			if(CDuiCSPlatDlg::GetInstance() != NULL && CDuiCSPlatDlg::GetInstance()->m_vecP2PSendMsgDlg.size() > 0 && CMainFrame::IsBuyer(pUser->GetId()) &&
				(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != NULL && g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg->GetFriendInfo()->GetId() != pUser->GetId()) )
					bIsShow = FALSE;
			else
				bIsShow = TRUE;
#else
			if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() > 0 && CMainFrame::IsBuyer(pUser->GetId()) &&
				(CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg != NULL && CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg->GetFriendInfo()->GetId() != pUser->GetId()) )
				bIsShow = FALSE;
			else
				bIsShow = TRUE;
#endif
		}

		////显示未读消息
		if(g_mapMsgUnRead.size()>0)
		{
			PopUnReadMsgDlg(pUser);
		}
		CDuiP2PSendMsgDlg* pDlg = NULL;
		if(!bIsShow)
		{
			pDlg = ShowMessageDlg(pUser, struExtInfo, FALSE);
#ifdef _CS_PLAT_EX_
			if(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != NULL)
				FlashWindow(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg->GetHWND(), TRUE);
#else
			if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg != NULL && (CMainFrame::IsBuyer(pUser->GetId()) || CMainFrame::IsWork(pUser->GetId())) )
				FlashWindow(CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg->GetHWND(), TRUE);
#endif
		}
		else
			pDlg = ShowMessageDlg(pUser, struExtInfo);
		pDlg->SetExtInfo(struExtInfo);
		//pDlg->UpdateFriendState();//分流之后设置用户状态为在线
		// 客户端显示自动接待内容
		if(pUser == NULL ||
			(pUser != NULL && pUser->GetId() >= 5000000))
		{	
			AUTO_REPLY acceptMsg;
			g_LocalData.GetAutoAccept_Online(acceptMsg);
			XT_MSG msgAcceptAuto;
			memset(&msgAcceptAuto,0,sizeof(msgAcceptAuto));
			msgAcceptAuto.ver         = XTREAM_CLIENT_VERSION;
			msgAcceptAuto.from_id     = g_ImUserInfo.GetId();
			msgAcceptAuto.fontColor   = acceptMsg.font.fontColor;
			msgAcceptAuto.fontSize    = acceptMsg.font.fontSize;
			msgAcceptAuto.fontStyle   = acceptMsg.font.flags;
			strcpy(msgAcceptAuto.fontName,acceptMsg.font.fontName);
			strcpy(msgAcceptAuto.from_nickname,g_ImUserInfo.nickname);
			msgAcceptAuto.send_time   = /*msg->send_time*/g_LocalData.GetTimeNow();;
			msgAcceptAuto.to_id		= msg->to_id;
			msgAcceptAuto.msg_type    = 0;		
			msgAcceptAuto.data_type   = XT_MSG::AUTO_REPLY;	// 自动接待消息

			CString text(acceptMsg.strText);
#ifdef _CS_PLAT_EX_
			if(text.IsEmpty())
			{	
				text.Format("您好，飞牛客服\"%s\"很高兴为您服务。", g_ImUserInfo.GetUserName().c_str());
			}
#endif
			strcpy(msgAcceptAuto.data,"[自动接待]" + text);

			msgAcceptAuto.data_len=strlen(text)+1;
			pDlg->AddTalkMsg(msgAcceptAuto, false);
			g_LocalData.SaveTalkMsg(msgAcceptAuto, pUser->GetId());
		}
#if defined (_BUSINESS_VESION) && !defined(_CS_PLAT_EX_) && !defined(_WORK_EX_) //商家版
		// 设置挂起状态
		CMainFrame::GetInstance()->Hangup();
#endif
		//pDlg->UpdateUrl();
#ifdef _CS_PLAT_EX_
		g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg->SetEditFocus();
#else
		CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg->SetEditFocus();
#endif
		return;
	}
#endif

	if ( wRet==0 )
		pFriend=FindFriend(msg->from_id);

	if ( pFriend==NULL )
	{
		pFriend = new FRIEND_INFO;

		pFriend->SetId(msg->from_id);
		pFriend->group_index = 1;//陌生人
		pFriend->status = XTREAM_ONLINE;
		sprintf(pFriend->nickname,msg->from_nickname);
		pFriend->SetUserName("");
		pFriend->SetGuid("");
		pFriend->SetPassword("");

		//添加到陌生人
		AddFriend(pFriend,true);
		SelItem(pFriend);
		//ImGetUserStatus(pFriend->GetId());		

#ifdef _BUSINESS_VESION
		CMainFrame::GetInstance()->Hangup();
#endif

		// 新用户，获取好友资料
		//unsigned threadID = 0;
		//_beginthreadex(NULL, 0, UpdateFriendInfo, (void*)msg->from_id, 0, &threadID);
	}
	else
	{
		strcpy(msg->from_nickname, pFriend->nickname);//添加昵称
	}

	if ( pFriend->status==XTREAM_OFFLINE )
	{
		if((msg->data_type != XT_MSG::SF_FILE_ACK && msg->data_type != XT_MSG::SF_PACKET_ACK))
			pFriend->status=XTREAM_ONLINE;

		UpdateFriend(pFriend);

		AddHideTick(pFriend->GetId());
	}

	// 添加自动回复（因为现在正处於与后台的消息交互过程中，所以开线程来处理自动回复，不然会出现接收端收不到消息的情况）
	if(msg->data_type == XT_MSG::IM_MSG && (g_ImUserInfo.status == XTREAM_LEAVE || g_ImUserInfo.status == XTREAM_BUSY))
	{
		unsigned threadID;
		_beginthreadex(NULL, 0, AutoReply, (void*)msg->from_id, 0, &threadID);
	}

	//屏蔽黑名单
	if ( pFriend->group_index==2 )
	{
		return ;
	}
	
	bool bFindDlg = g_pMessageHandleWnd->FindP2PSendMsgDlg(pFriend->GetId());

	if((msg->data_type == XT_MSG::SF_FILE_ACK || msg->data_type == XT_MSG::SF_PACKET_ACK)
		&& CGroupSendDlg::GetInstance() != NULL && ::IsWindow(CGroupSendDlg::GetInstance()->GetHWND()))
	{	// 群发过程中，发送方发送图片过程中的相关协议
		if(msg->data_type == XT_MSG::SF_FILE_ACK)
		{
			CGroupSendDlg::GetInstance()->SF_RecvPfileAck(*msg);
		}
		if(msg->data_type == XT_MSG::SF_PACKET_ACK)
		{
			CGroupSendDlg::GetInstance()->SF_RecvPfilePacketAck(*msg);
		}
		return;
	}

	if(msg->data_type == XT_MSG::WIN_SHAKE)		// 振屏消息，判断是否允许
	{
		WORD nVK;
		g_LocalData.GetShakeWinConfig(nVK);
		if(nVK == 0)
		{	// 不支持振屏，不弹出聊天窗口
			return;
		}
	}

	CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
	if ( pDlg->GetHWND()==NULL )   
	{
		pDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, CWnd::FromHandle(GetDesktopWindow()));
	}
	else
	{
		if(msg->data_type==XT_MSG::OPEN_P2PSENDMSG_DLG)
		{
			pDlg->UpdateUrl();
			pDlg->UpdateFriendState();//分流之后设置用户状态为在线
#ifdef _CS_PLAT_EX_
			pDlg->UpdateIsTran();
#endif
		}
	}

#ifndef _CS_PLAT_EX_
	if ( ::IsWindowVisible(pDlg->GetHWND())==FALSE )
		pDlg->ShowWindow(true);

	if (::IsMinimized(pDlg->GetHWND()))
		pDlg->ShowWindow(true);

	//::BringWindowToTop(pDlg->GetHWND());
#endif

#ifdef _CS_PLAT_EX_
	//显示好友的红点
	if(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != NULL && g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != pDlg && !CMainFrame::IsBuyer(pFriend->GetId()))
		CDuiCSPlatDlg::GetInstance()->SetFriendUnMsg(pFriend, true);
	if( CDuiCSPlatDlg::GetInstance() != NULL && CMainFrame::IsBuyer(pFriend->GetId()))
	{
		CDuiCSPlatDlg::GetInstance()->SetCustUnMsgVisable(true);//显示客户标签的红点
		//改为如果和当前客户聊天时，有新的聊天窗口不弹到前面
		if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() <= 1)
		{
			//CDuiCSPlatDlg::GetInstance()->m_bIsUpdate = TRUE;
			//CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
		}
	}
	else if(CDuiCSPlatDlg::GetInstance() != NULL && !CMainFrame::IsBuyer(pFriend->GetId()))
	{
		CDuiCSPlatDlg::GetInstance()->SetTeamUnMsgVisable(true);//显示我的团队标签的红点
	}
	if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() > 1 && g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != pDlg)
	{
		pDlg->ShowWindow(false);
	}
	else
	{
		g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg = pDlg;//设置当前显示的聊天窗口
		if(!CMainFrame::IsBuyer(pFriend->GetId()))
		{
			CDuiCSPlatDlg::GetInstance()->SetListCurselNull();//设置不选中客户窗口
		}
	}
#elif defined _WORK_EX_
	if( CDuiCounsel::GetInstance() != NULL)
	{
		//改为如果和当前客户聊天时，有新的聊天窗口不弹到前面
		if(CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() <= 1)
		{
			//CDuiCounsel::GetInstance()->m_bIsUpdate = TRUE;
			//CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
		}
		if(!CDuiCounsel::GetInstance()->IsCurrentShowDlg(pDlg))
		{
			pDlg->ShowWindow(false);
		}
	}
#else
	if( CDuiCounsel::GetInstance() != NULL && (CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())) )
	{
		//改为如果和当前客户聊天时，有新的聊天窗口不弹到前面
		if(CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() <= 1)
		{
			//CDuiCounsel::GetInstance()->m_bIsUpdate = TRUE;
			//CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
		}
		if(!CDuiCounsel::GetInstance()->IsCurrentShowDlg(pDlg))
		{
			pDlg->ShowWindow(false);
		}
	}
#endif

	vector<DOWNLOAD_OBJ> arrDownloadTask;
	CString sShowMsg;
	if(msg->data_type == 0 && TranslateMsg(msg->data,sShowMsg,arrDownloadTask))
	{
		// 翻译出应该显示的消息
		strncpy(msg->data,sShowMsg,sizeof(msg->data)-1);
		// 开始下载任务
		if(pDlg && pDlg->GetHWND() != NULL)
		{
			pDlg->Download(arrDownloadTask);
		}
	}

	pDlg->ProcessMsg(*msg);

	CString strTitle;

	//如果找不到当前的SendMessageDlg,则弹出hint dlg
	switch ( msg->data_type )
	{
	case XT_MSG::IM_MSG://聊天消息
		{
		//////显示未读消息
		//if(g_mapMsgUnRead.size()>0)
		//{
		//	PopUnReadMsgDlg(pFriend);
		//}
		PlayTone(EVOICE_RECEIVE_MSG);
#ifndef _CS_PLAT_EX_
		if( CDuiCounsel::GetInstance() != NULL && CMainFrame::IsBuyer(pFriend->GetId()))
			CDuiCounsel::GetInstance()->ShowWindow(true);
#endif
    	if ( bFindDlg==false )
		{
			if(msg->msg_type == 10)
			{
				// 离线留言不用提示框;
			}
			else
			{
				strTitle.Format("%s说:",UserInfoToStr(pFriend));
				ShowUserHintDlg(pFriend, strTitle, msg->data, m_hWnd);
				::FlashWindow(pDlg->GetHWND(), TRUE);
#ifdef _CS_PLAT_EX_
				if(CDuiCSPlatDlg::GetInstance() != NULL && g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() <= 1)
					pDlg->ShowWindow(SW_SHOW);
				else
				{
					if(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != NULL)
						::FlashWindow(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg->GetHWND(), TRUE);
				}
#elif defined _WORK_EX_
				if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() <= 1 && (CMainFrame::IsSeller(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())))
					pDlg->ShowWindow(SW_SHOW);
				else
				{
					if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg != NULL && (CMainFrame::IsSeller(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())))
						::FlashWindow(CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg->GetHWND(), TRUE);
				}
#else
				if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() <= 1 && (CMainFrame::IsBuyer(pFriend->GetId())|| CMainFrame::IsWork(pFriend->GetId())))
					pDlg->ShowWindow(SW_SHOW);
				else
				{
					if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg != NULL && (CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())))
						::FlashWindow(CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg->GetHWND(), TRUE);
				}
#endif
			}
		}
		else
		{	
			strTitle.Format("%s说:",UserInfoToStr(pFriend));
			
			if ( pDlg->GetHWND()==NULL )
			{
				pDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, CWnd::FromHandle(GetDesktopWindow()));

				pDlg->ShowWindow(SW_MINIMIZE);
				pDlg->ShowWindow(SW_SHOW);
			
				ShowUserHintDlg(pFriend, strTitle, msg->data, m_hWnd);
			}
			else
			{	
				if(msg->msg_type != 10)
				{
					if ( ::IsIconic(pDlg->GetHWND()) )
					{
						ShowUserHintDlg(pFriend, strTitle, msg->data, m_hWnd);
					}
				}
				//FlashWindowEx(pDlg->GetHWND(), FLASHW_TRAY|FLASHW_TIMERNOFG,20,500);
			}

			FlashWindow(pDlg->GetHWND(), TRUE);
#ifdef _CS_PLAT_EX_
			if(CDuiCSPlatDlg::GetInstance() != NULL && g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() <= 1)
				pDlg->ShowWindow(SW_SHOW);
			else
			{
				if(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != NULL)
					::FlashWindow(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg->GetHWND(), TRUE);
			}
#elif defined _WORK_EX_
			if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() <= 1 && (CMainFrame::IsSeller(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())))
				pDlg->ShowWindow(SW_SHOW);
			else
			{
				if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg != NULL && (CMainFrame::IsSeller(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())))
					::FlashWindow(CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg->GetHWND(), TRUE);
			}
#else
			if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() <= 1 && (CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())))
				pDlg->ShowWindow(SW_SHOW);
			else
			{
				if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg != NULL && (CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())))
					::FlashWindow(CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg->GetHWND(), TRUE);
			}
#endif
		}

		if (pDlg->GetHWND())
			g_LocalData.SaveTalkMsg(*msg,msg->from_id);

		m_pRecentList->AddUser(pFriend);
		RECENTLIST_OP(AddUser(pFriend))

#ifdef _BUSINESS_VESION	// 更新最近联系客户到后台
		if (g_ImUserInfo.status!=XTREAM_OFFLINE/* && !m_bHasUpdateNearCommCust*/)
		{
			// 统计需要更新到后台的最近联系客户信息
			CMainFrame::GetInstance()->StatNeedUpdateCustInfo(pFriend);
			// 更新最近联系客户信息到后台
			CMainFrame::GetInstance()->UpdateCustInfoToServer();

			//return;
		}	
#endif	
#ifdef _CS_PLAT_EX_
		BOOL bIsShow = FALSE;
		if(CDuiCSPlatDlg::GetInstance() != NULL && CMainFrame::IsBuyer(pFriend->GetId()))
		{
			if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() == 1)
				bIsShow = TRUE;
		}
		////显示未读消息
		if(g_mapMsgUnRead.size()>0)
		{
			PopUnReadMsgDlg(pFriend);
		}
		if(bIsShow)
		{
			CDuiCSPlatDlg::GetInstance()->SetFriendListElement(pFriend, false, false, false, false, true);
			//CDuiCSPlatDlg::GetInstance()->m_bIsUpdate = TRUE;
			//CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
		}
		else
		{
			if(CDuiCSPlatDlg::GetInstance() != NULL)
			{
				CDuiCSPlatDlg::GetInstance()->SetFriendListElement(pFriend, false, false, false, false, true);
				g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg->SetEditFocus();
			}
		}
#else
		BOOL bIsShow = FALSE;
		if(CDuiCounsel::GetInstance() != NULL && CMainFrame::IsBuyer(pFriend->GetId()))
		{
			if(CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() == 1)
				bIsShow = TRUE;
		}
		////显示未读消息
		if(g_mapMsgUnRead.size()>0)
		{
			PopUnReadMsgDlg(pFriend);
		}
		if(bIsShow)
		{
			CDuiCounsel::GetInstance()->SetFriendListElement(pFriend, false, false, false, false, true);
			//CDuiCounsel::GetInstance()->m_bIsUpdate = TRUE;
			//CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
		}
		else
		{
			if(CDuiCounsel::GetInstance() != NULL)
			{
				CDuiCounsel::GetInstance()->SetFriendListElement(pFriend, false, false, false, false, true);
				CDuiCounsel::GetInstance()->m_pDlgSelectP2PSendMsgDlg->SetEditFocus();
			}
		}
#endif
		break;
		}
	case XT_MSG::MAGIC_FACE://魔法表情
		{
			PlayTone(EVOICE_RECEIVE_MSG);
			CRecvPacket rp((char *)msg->data,sizeof(msg->data));
			char   face_name[255];
			char   face_url[255];

			rp>>face_name>>face_url;

			CString strMessage;
			strMessage.Format("魔法表情\"%s\"",face_name);

			if (pDlg->RecvMagicFace(msg->from_id,face_name,face_url))
			{
				bool bShowHint = false;

				strTitle.Format("%s",UserInfoToStr(FindFriend(msg->from_id)));
				
				if ( pDlg->GetHWND() != NULL && ::IsIconic(pDlg->GetHWND())==FALSE )
				{
					/*if(g_LocalData.GetBasicSystemSetting().bPlayMagicFace )
						pDlg->ShowMagicFace(face_name);*/

					::BringWindowToTop(pDlg->GetHWND());
				}
				else
				{
					bShowHint = true;
				}

				if ( bFindDlg==false )
				{
					if ( pDlg->GetHWND()==NULL )
					{
						pDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, CWnd::FromHandle(GetDesktopWindow()));
						//pDlg->ShowWindow(SW_MINIMIZE);
						pDlg->ShowWindow(true);
					}
					
					//pDlg->FlashWindowEx(FLASHW_TRAY|FLASHW_TIMERNOFG,20,500);
					FlashWindow(pDlg->GetHWND(), TRUE);

				}
				else
				{	if ( pDlg->GetHWND()==NULL )
					{
						pDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, CWnd::FromHandle(GetDesktopWindow()));
						//pDlg->ShowWindow(SW_MINIMIZE);
						pDlg->ShowWindow(true);
						FlashWindow(pDlg->GetHWND(), TRUE);

						bShowHint=true;
					}
					else
					{
						if ( IsIconic(pDlg->GetHWND()) )
						{
							bShowHint = true;
						}

						//pDlg->FlashWindowEx(FLASHW_TRAY|FLASHW_TIMERNOFG,20,500);
					}
				}
				
//#ifdef _TEST
				if(g_LocalData.GetBasicSystemSetting()->bPlayMagicFace )
					pDlg->ShowMagicFace(face_name);
//#endif

				if ( bShowHint )
					ShowUserHintDlg(pFriend, strTitle,strMessage, m_hWnd);
			
			}
		}
		break;
	case XT_MSG::FILE_REQ://发文件请求
		{
			PlayTone(EVOICE_RECEIVE_MSG);
			strTitle.Format("%s发送文件:",UserInfoToStr(pFriend));
			CRecvPacket rp(msg->data,sizeof(msg->data));
			uint16 file_id;
			char   file_name[255];
			rp>>file_id>>file_name;

			ShowUserHintDlg(pFriend, strTitle,file_name,m_hWnd);
			FlashWindow(pDlg->GetHWND(), TRUE);
		}
		break;
	case XT_MSG::WIN_SHAKE:
		{	// 窗口抖动
			WORD nVK = 0;
			g_LocalData.GetShakeWinConfig(nVK);
			if(nVK == 1)////支持振屏
			{
				PlayTone(EVOICE_SHOCK_SCREEN);
			}
		}
		break;
	default:break;
	}			
	// 客户端显示自动回复内容
	if(msg->data_type == XT_MSG::IM_MSG && (g_ImUserInfo.status == XTREAM_LEAVE || g_ImUserInfo.status == XTREAM_BUSY))
	{
		vector<AUTO_REPLY> allItem;
		if( 0 != g_LocalData.GetAllAutoReplyItem(allItem) )
		{
			AUTO_REPLY item = allItem[0];

			XT_MSG msgReplyAuto;
			memset(&msgReplyAuto,0,sizeof(msgReplyAuto));
			msgReplyAuto.ver         = XTREAM_CLIENT_VERSION;
			msgReplyAuto.from_id     = g_ImUserInfo.GetId();
			msgReplyAuto.fontColor   = item.font.fontColor;
			msgReplyAuto.fontSize    = item.font.fontSize;
			msgReplyAuto.fontStyle   = item.font.flags;
			strcpy(msgReplyAuto.fontName,item.font.fontName);
			strcpy(msgReplyAuto.from_nickname,g_ImUserInfo.nickname);
			msgReplyAuto.send_time   = msg->send_time;
			msgReplyAuto.to_id		= msg->to_id;
			msgReplyAuto.msg_type    = 0;		
			msgReplyAuto.data_type   = XT_MSG::AUTO_REPLY;	// 自动接待消息

			CString text(item.strText);
			strcpy(msgReplyAuto.data,"[自动回复]" + text);
			msgReplyAuto.data_len=strlen(text)+1;
			pDlg->AddTalkMsg(msgReplyAuto, false);
			g_LocalData.SaveTalkMsg(msgReplyAuto, pFriend->GetId());
		}
	}
}

//
void CMainFrame::OnP2PMsgAck(const char *data,WORD wRet)
{
	if ( wRet==SERVER_TIMEOUT && g_ImUserInfo.status!=XTREAM_OFFLINE)//P2p发送超时
	{
		// 如果p2p发送失败则由服务器转发
		SendDataStruct *pSendData = (SendDataStruct *)data;
		
		CRecvPacket rp((char *)pSendData->data,MAX_RECV_BUFFERS);

		XT_HEAD hd;
		rp.GetXtHead(hd);
		XT_MSG msg;
		char buff[MAX_RECV_BUFFERS];
		// 找到发送目的好友
		FRIEND_INFO *pFriend = FindFriend(hd.did);
		// 如果不存在该好友，丢弃（已经删除了该好友?）
		if(pFriend == NULL)
		{
			return;
		}
		// 读出数据并解密
		long buff_len = rp.ReadPacket(buff,hd.len - sizeof(XT_HEAD));
        BOOL bDecryped = TEADecrypt(buff,buff_len,
				(char *)pFriend->key,
				buff,&buff_len);

		if ( bDecryped )
		{
			CRecvPacket cp(buff,buff_len);

			cp	>>msg.from_id
				>>msg.to_id
				>>msg.data_type
				>>msg.ver
				>>msg.send_time
				//>>msg.ms
				>>msg.recv_flag
				;
			//msg.send_time = time(0);
			//msg.ms = GetTickCount()%1000/4;
			
			if (msg.data_type==0)
			{			
				cp	>>msg.fontName
					>>msg.fontStyle
					>>msg.fontColor
					>>msg.fontSize;
			}

			msg.data_len = cp.ReadData(msg.data,sizeof(msg.data));
			cp>>msg.from_nickname ;
			// 聊天消息转发
			FRIEND_INFO *pFriend = FindFriend(msg.to_id);
			if ( pFriend )
			{
				//bool bFind = g_pMessageHandleWnd->FindP2PSendMsgDlg(pFriend->id);

				//if ( bFind )
				//{
				//	g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend)->OnP2pMsgTimeOut();
				//}
				TRACE("p2p发送失败，服务器帮助转发\n");
				// 如果发送失败,需要重新获取好友状态
				if(pFriend->connFlag == FRIEND_INFO::P2P)
				{
					pFriend->ip = 0;
					pFriend->port = 0;
					pFriend->connFlag = FRIEND_INFO::P2P_FAILED;
				}
				// p2p发送失败尝试服务器转发
				msg.recv_flag = CIMProtocol::SMT_SERVER;
//#ifndef _DEBUG
				g_pProtocol->SendP2PMsg(msg,CIMProtocol::SMT_SERVER);
//#endif
			}	
			
		}
		else
		{
		//	ShowMessage("聊天消息发送失败!");
		}
	}
}

void CMainFrame::RemoveHideTick(uint32 fid)
{
	list<tagHideTick>::iterator it;
	
	for (it=m_lsHideTick.begin(); it!=m_lsHideTick.end(); it++ )
	{
		m_lsHideTick.erase(it);
		break;
	}
}

void CMainFrame::OnNetWorkError()
{
	TRACE("ERROR:网络错误！！\n");
	KillTimer(GetHWND(), TIMER_BEAT);
	SetTimer(GetHWND(), TIMER_RELOGIN,1000,NULL);

	this->m_bDisconnectFlag = true;
	this->m_nPreOnlineStatus = g_ImUserInfo.status;
	//theApp.m_pMainWnd->PostMessage(WM_HEART_TIMEOUT,0,0);
}

void CMainFrame::AddHideTick(uint32 fid)
{
	list<tagHideTick>::iterator it;
	
	bool bflag=false; 
	for ( it=m_lsHideTick.begin();it!=m_lsHideTick.end(); it++ )
	{
		if ( (*it).fid==fid )
		{
			(*it).dwTick=GetTickCount();
			bflag=true;
			break;
		}
	}
	
	if ( bflag==false )
	{
		tagHideTick ht;

		ht.fid = fid;
		ht.dwTick = GetTickCount();

		m_lsHideTick.push_back(ht);
	}
}

void CMainFrame::CheckHideTick()
{
	list<tagHideTick>::iterator it;
	
	DWORD dwTick = GetTickCount();
	
	while ( 1 )
	{
		bool bFlag=false;

		for ( it=m_lsHideTick.begin();
			it!=m_lsHideTick.end();
			 it++ )
		{
			if ( (dwTick-(*it).dwTick)>30000 )
			{
				FRIEND_INFO *pFriend = FindFriend((*it).fid);

				if ( pFriend )
				{
					pFriend->status=XTREAM_OFFLINE;
					UpdateFriend(pFriend);

					m_lsHideTick.erase(it);
					bFlag = true;
					break;
				}
			}
		}

		if (bFlag==false)
			break;
	}
	
}

void CMainFrame::OnRecvClusterMsg(const char *data,WORD wRet)
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
					}
					else
					{						
						g_LocalData.SaveTalkMsg(*msg);
						pDlg->FlashWindow(FALSE);
					}

					msg->cluster_type = PERM_FAMILY;
					msg->msg_type     = 1;//群聊消息

					m_pRecentList->AddCluster(pCluster);
					RECENTLIST_OP(AddCluster(pCluster))
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
							m_pClusterList->UpdateGroup(pCluster);
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

	if ( msg->cluster_type == TEMP_FAMILY )//不是临时群，丢掉
	{
		if ( msg->from_id == g_ImUserInfo.GetId() )
		{
			return ;
		}

		bool bFind = g_pMessageHandleWnd->FindGroupTalkDlg(msg->cluster_id);

		CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetTempClusterSendMsgDlg(msg->cluster_id);
		vector<DOWNLOAD_OBJ> arrDownloadTask;
		CString sShowMsg;
		if(TranslateMsg(msg->data,sShowMsg,arrDownloadTask))
		{
			// 翻译出应该显示的消息
			strncpy(msg->data,sShowMsg,sizeof(msg->data)-1);
			// 开始下载任务
			if(pDlg && pDlg->m_hWnd != NULL)
			{
				pDlg->Download(arrDownloadTask);
			}
		}

		pDlg->ProcessMsg(*msg);

		switch ( msg->data_type)
		{
		case XT_MSG::IM_MSG:
			{
				if ( pDlg->IsWindowVisible()==false )
				{
					if ( bFind==false )
					{
						pDlg->ShowWindow(SW_HIDE);
						pDlg->ShowWindow(SW_MINIMIZE);
						pDlg->ShowWindow(SW_SHOW);
					}
					else
					{
						pDlg->ShowWindow(SW_SHOW);
					}						
				}
				if(pDlg->IsIconic() || !pDlg->IsWindowVisible())
					pDlg->ShowWindow(SW_RESTORE);
				pDlg->BringWindowToTop();
				pDlg->FlashWindowEx(FLASHW_TRAY|FLASHW_TIMERNOFG,20,500);
				pDlg->SaveClusterMsg(*msg);
				break;
			}
		case XT_MSG::MAGIC_FACE://magic face
			{
				CRecvPacket rp((char *)msg->data,sizeof(msg->data));
				char   face_name[255];
				char   face_url[255];

				rp>>face_name>>face_url;

				FRIEND_INFO *pFriend = GetFriendInfo(msg->from_id);

				if ( pFriend )
				{
					CString strMessage;
					strMessage.Format("%s 发送了魔法表情\"%s\"",pFriend->nickname,face_name);
					ShowUserHintDlg(strMessage);

					if ( pDlg->RecvMagicFace(msg->from_id, face_name,face_url) )
					{
						pDlg->ShowMagicFace(face_name);
					}
				}
				break;
			}
		default:
			break;

		}
		//	
	}
}

void CMainFrame::OnRefreshFriendList()
{
	static DWORD dwTickPreGet=(DWORD)-1;
	DWORD dwTick = GetTickCount();

	if ( (dwTickPreGet-dwTick)>60 )
	{
		ImGetFriendList();
		//m_pFriendList->AddRefreshFlag();
		dwTickPreGet = dwTick;
	}
}

void CMainFrame::OnShowAll()
{
	//m_bShowAllFriend=TRUE;
	//g_LocalData.SetShowAllFriend(m_bShowAllFriend);
	UpdateFriendList();
}

void CMainFrame::OnHideOffline()
{
	//m_bShowAllFriend=FALSE;
	//g_LocalData.SetShowAllFriend(m_bShowAllFriend);
	UpdateFriendList();
}

LRESULT CMainFrame::OnChangeFriendGroup(WPARAM wParam, LPARAM lParam)
{
	int nFriendID=(int)wParam;
	int nNewGroupID=(int)lParam;
	// 先找到该好友
	FRIEND_INFO* pFriend = FindFriend(nFriendID);
	if(pFriend==NULL)
		return -1;
	m_pFriendList->SetCurSel(pFriend);

	// 响应移动好友的消息函数
	int nGroupIndex = g_GroupInfo.FindItemIndex(nNewGroupID);
	//OnGroupListNotify(NM_FL_DRAGDROP, nGroupIndex);

	return 0;
}

void CMainFrame::MakeTempCluster(FL_GROUP_INFO* pGroup)
{
	// 同时将子组的好友也添加到聊天窗口
	vector<FL_GROUP_INFO*> vecSubGroup;
	pGroup->GetSubGroup(vecSubGroup);

	CLUSTER_INFO *pTempClusterInfo=new CLUSTER_INFO();
	int nGroupIndex = pGroup->dwData;
	CString strGroupName = pGroup->strTitle;///群组名称;
	int nLen = strGroupName.GetLength();
	if (nLen > 0)
	{
		memcpy(pTempClusterInfo->szFamilyName, (void*)strGroupName.GetBuffer(nLen), nLen);
		pTempClusterInfo->szFamilyName[nLen] = '\0';
	}
	int i=0;
	FRIEND_LIST::iterator it,itE;
	it=g_lsFriend.begin();
	itE=g_lsFriend.end();
	// 如果选择的是陌生人和黑名单则不处理
	if(nGroupIndex == 1 || nGroupIndex == 2)
	{
		delete pTempClusterInfo;
		return ;
	}
	// 找到该组内在线的好友
	for(;it!=itE;it++)
	{
		// 判断好友是否属于选定组或其子组
		BOOL bIsInGroup = FALSE;
		if ((*it)->group_index == nGroupIndex)
		{	// 好友属于选定组
			bIsInGroup = TRUE;
		}
		else
		{
			for (int i = 0; i < vecSubGroup.size(); ++i)
			{
				if ((*it)->group_index == vecSubGroup[i]->dwData)
				{	// 好友属于子组
					bIsInGroup = TRUE;
					break;
				}
			}
		}

		if (bIsInGroup)
		{
			if((*it)->GetId() != g_ImUserInfo.GetId())//如果组内有自己则不添加到临时群
				pTempClusterInfo->members.push_back(**it);
			// 临时群聊成员个数不能超过MAX_TEMPFAMILYMEM_NUM
			if(pTempClusterInfo->Count() >= (MAX_TEMPFAMILYMEM_NUM-1))
				break;
		}
	}
	// 如果有足够数量在线好友则建立群聊
	if(pTempClusterInfo->Count() > 0)
	{
		if(pTempClusterInfo->Count() == 1)
		{
			// 仅有一人，则建立单聊
			CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(&pTempClusterInfo->members[0]);
			// 			if ( pDlg->GetHWND()==NULL )
			// 				pDlg = CDuiP2PSendMsgDlg::NewDlg(&pTempClusterInfo->members[0],pmSingleTalk, CWnd::FromHandle(GetDesktopWindow()));
			// 			pDlg->ShowWindow(true);
		}
		else
		{
			// 应该先判断是否存在该群聊对话框
			CP2PSendMsgDlg *pDlg =NULL;
			pDlg=g_pMessageHandleWnd->GetTempGroupSendMsgDlg(nGroupIndex);

			if(pDlg==NULL)
			{
				// 如果不存在该对话框则创建之
				//CDuiP2PSendMsgDlg *pDlg2 = g_pMessageHandleWnd->GetP2PSendMsgDlg(&pTempClusterInfo->members[0]);
				// 				if ( pDlg2->m_hWnd==NULL )
				// 				{
				// 					//pDlg2->Create(CDuiP2PSendMsgDlg::IDD,CWnd::FromHandle(GetDesktopWindow()));
				// 					CDuiP2PSendMsgDlg::NewDlg(&pTempClusterInfo->members[0],pmSingleTalk, CWnd::FromHandle(GetDesktopWindow()));
				// 				}
				// 				pDlg=g_pMessageHandleWnd->GetTempClusterSendMsgDlg(nGroupIndex);
				// 				pDlg->CenterWindow();
				// 				pDlg->ShowWindow(SW_SHOW);
				// 				pDlg->BringWindowToTop();

				pDlg= g_pMessageHandleWnd->GetTempClusterSendMsgDlg2(nGroupIndex);

				// 				if ( pDlg->m_hWnd==NULL )
				// 				{
				// 					pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
				// 				}
				pDlg->CreateTempCluster(*pTempClusterInfo,nGroupIndex);

				pDlg->ShowWindow(SW_SHOW);
				pDlg->BringWindowToTop();

				// 				pDlg = new CP2PSendMsgDlg(&pTempClusterInfo->members[0],pmSingleTalk, CWnd::FromHandle(GetDesktopWindow()));
				// 				m_mapTempClusterMsgDlg.insert(
				// 					map<uint32,/*CP2PSendMsgDlg*/CP2PSendMsgDlg *>::value_type(fid,pDlg)
				// 					);
				// 				pDlg->Create(CP2PSendMsgDlg::IDD,CWnd::FromHandle(GetDesktopWindow()));
				// 				pDlg->CreateTempCluster(*pTempClusterInfo,nGroupIndex);
			}
			else
			{
				if ( pDlg->IsWindowVisible()==FALSE )
					pDlg->ShowWindow(SW_SHOW);

				if (pDlg->IsIconic() )
				{
					pDlg->ShowWindow(SW_RESTORE);
				}
				pDlg->BringWindowToTop();
			}	

#ifdef _BUSINESS_VESION	// 如果多人聊天窗口成员所属组为我的团队或者其子分组，则不显示“推送评价”按钮
			if (CheckSubGroupIsInMyTeam(nGroupIndex))
			{
				pDlg->ShowOrHideScoreBtn(FALSE);
			}			
#endif

		}

	}	
	if(pTempClusterInfo)
		delete pTempClusterInfo;
}



void CMainFrame::OnNCFlRbum()
{
	CMenuEx menu, *popmenu;
	menu.LoadMenu(IDR_MENU_TEMPLATE);
	popmenu = menu.GetSubMenu(0);
	popmenu->RemoveMenu(0,MF_BYPOSITION);
	CPoint point;
	GetCursorPos(&point);
	CString strItem;
	strItem="删除好友";
	popmenu->AppendMenu(MF_STRING,ID_DELMFRIEND,strItem);
	popmenu->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, CWnd::FromHandle(GetHWND()));
}


void CMainFrame::OnModfCommName()
{
	int nGroup = m_pFriendList->GetCurSel().nGroupID;
	int nItem  = m_pFriendList->GetCurSel().nItem;

	if ( nGroup==-1 || nItem==-1 )
	{
		return ;
	}
	FRIEND_INFO *pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,nItem);
	if(pFriend==NULL)
		return;

	tr1::shared_ptr<CModfCommNameDlg> pDlg = CModfCommNameDlg::Show(pFriend);
	pDlg->CenterWindow();
	pDlg->ShowWindow(SW_SHOW);
}



void CMainFrame::SetCommName( FRIEND_INFO* pFriend, const CString& sCommName)
{
	if(pFriend == NULL)
		return;
	CString s = sCommName;
	if(sCommName.GetLength()>8)
		s = sCommName.Mid(0,8);

	strcpy(pFriend->comment_name, s);
	// 是否有备注名称
	if(pFriend->comment_name[0] == 0)
		s = pFriend->nickname;
	else
		s = pFriend->comment_name;
	m_pFriendList->SetItemText(pFriend, s);
}


void CMainFrame::OnCreateTempCluster()
{
	FL_ITEM_POS pos=m_pFriendList->GetCurSel();

	int nGroup = pos.nGroupID;
	int nItem  = pos.nItem;
	if(nGroup>-1)
	{
		FL_GROUP_INFO* pGroup = m_pFriendList->GetGroupData(nGroup);
		MakeTempCluster(pGroup);
	}
}

void CMainFrame::ResetName(uint32 fid)
{
	vector<FL_GROUP_INFO *> vecGroup;
	int nGroupCount = m_pFriendList->GetAllGroup(vecGroup);

	for(int i = 0; i < nGroupCount; ++i)
	{
		FL_GROUP_INFO *group = vecGroup[i];
		int nGroup = group->dwData;
		int nItems = group->vecItem.size();

		for ( int nItem=0; nItem<nItems; nItem++)
		{
			FRIEND_INFO *pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,nItem);

			if ( fid==pFriend->GetId())
			{
				m_pFriendList->SetItemText(nGroup,nItem,pFriend->nickname);

				break;
			}
		}
	}

	FRIEND_LIST::iterator fIt;

	for ( fIt=g_lsFriend.begin();
		fIt!=g_lsFriend.end();
		fIt++ )
	{
		FRIEND_INFO *p=(*fIt);
		if ( p->GetId()==fid )
		{
			strcpy(p->comment_name,"");
			break;
		}
	}	
}

void CMainFrame::ModifyName(XT_MOD_FRIEND_COMMNAME_ACK *pAck)
{
	FRIEND_LIST::iterator fIt;

	for ( fIt=g_lsFriend.begin();
		fIt!=g_lsFriend.end();
		fIt++ )
	{
		FRIEND_INFO *p=(*fIt);
		if ( p->GetId()==pAck->fid )
		{
			strcpy(p->comment_name,pAck->commName);
			break;
		}
	}	

	vector<FL_GROUP_INFO *> vecGroup;
	int nGroupCount = m_pFriendList->GetAllGroup(vecGroup);

	for(int i = 0; i < nGroupCount; ++i)
	{
		FL_GROUP_INFO *group = vecGroup[i];
		int nGroup = group->dwData;
		int nItems = group->vecItem.size();

		for ( int nItem=0; nItem<nItems; nItem++)
		{
			FRIEND_INFO *pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(nGroup,nItem);

			if ( pAck->fid==pFriend->GetId())
			{
				m_pFriendList->SetItemText(nGroup,nItem,UserInfoToStr(pFriend));
				break;
			}
		}
	}
}


void CMainFrame::OnModFCNameAck(const char* data,WORD wRet)
{
	XT_MOD_FRIEND_COMMNAME_ACK *pAck=(XT_MOD_FRIEND_COMMNAME_ACK*)data;
	if(wRet==0)
	{
		// 修改备注成功
		//CMessageBox::Prompt("修改备注成功！服务器已经接受您提交的信息。");
		ModifyName(pAck);
		////////////////////////////////////////////////////////////////////
	}
	else
	{
		// 修改备注失败
		CMessageBox::Prompt(g_pProtocol->GetLastError(),"修改备注失败");
		//ResetName(pAck->fid);
	}
}

// 自动回复（当离线或者忙碌时）
unsigned __stdcall CMainFrame::AutoReply(void * pParam)
{
	Sleep(200);
	FRIEND_INFO *pFriend=FindFriend((uint32)pParam);
	if(pFriend == NULL)
		return 0;

	vector<AUTO_REPLY> allItem;
	if( 0 == g_LocalData.GetAllAutoReplyItem(allItem) )
	{
		return 0;
	}

	AUTO_REPLY item = allItem[0];

	XT_MSG msg;
	memset(&msg,0,sizeof(msg));
	msg.ver         = XTREAM_CLIENT_VERSION;
	msg.from_id     = g_ImUserInfo.GetId();
	msg.fontColor   = item.font.fontColor;
	msg.fontSize    = item.font.fontSize;
	msg.fontStyle   = item.font.flags;
	strcpy(msg.fontName,item.font.fontName);
	strcpy(msg.from_nickname,g_ImUserInfo.nickname);

	msg.send_time   = /*time(0)*/g_LocalData.GetTimeNow();

	msg.to_id		= pFriend->GetId();
	msg.dest_ip		= pFriend->ip;
	msg.dest_port	= pFriend->port;
	msg.msg_type    = 0;		

	msg.data_type   = 0;//即时聊天消息

	CString text("[自动回复]" + item.strText);//自动回复前面添加提示
	if ( text.IsEmpty() )
	{
		return 0;
	}

	if ( FindForbiddenWords(text) )
	{
		return 0;
	}

	strcpy(msg.data,text);
	msg.data_len=strlen(text)+1;

	int  type = CIMProtocol::SMT_SERVER;
	g_pProtocol->SendP2PMsg(msg,type);

	return 0;
}

void CMainFrame::OnFriendInfoInd(const char *data,WORD wRet)
{
	// 获取好友信息
	XT_INFO_IND *pFriendInd = (XT_INFO_IND *)data;

	unsigned threadID = 0;
	_beginthreadex(NULL, 0, UpdateFriendInfo, (void*)pFriendInd->fid, 0, &threadID);
}

// 更新好友信息
unsigned __stdcall CMainFrame::UpdateFriendInfo(void * pParam)
{
	Sleep(200);

	XT_USERINFO_GET_REQ req;
	req.fid = (uint32)pParam;
	g_pProtocol->SendReq(req);

	return 0;
}

void CMainFrame::OnRecvP2pTestAck(const char * data, WORD wRet)
{
	//LPXT_P2PTEST_ACK pAck = (LPXT_P2PTEST_ACK)data;
	//FRIEND_INFO * pFriend;
	//uint32 id;
	//if( wRet == 0 )
	//{
	//	pFriend = FindFriend(pAck->id);
	//	// 与该好友的连接成功
	//	if(pFriend)
	//	{
	//		if(pAck->testType == XT_P2PTEST_ACK::LAN)
	//		{
	//			pFriend->connFlag = FRIEND_INFO::LAN;
	//		}
	//		else
	//		{
	//			pFriend->connFlag = FRIEND_INFO::P2P;
	//			pFriend->p2pRetry = 5;
	//		}
	//		
	//	}
	//}
	//else if( wRet == SERVER_TIMEOUT )
	//{
	//	// 与该好友的连接测试失败
	//	CRecvPacket rp((char *)data,MAX_RECV_BUFFERS);
	//	XT_HEAD hd;
	//	rp.GetXtHead(hd);
	//	id = hd.did;
	//	pFriend = FindFriend(id);
	//	if(pFriend)
	//	{
	//		// 内网测试失败，继续公网p2p测试
	//		if(pFriend->connFlag == FRIEND_INFO::TRY_LAN)
	//		{
	//			pFriend->connFlag = FRIEND_INFO::TRY_P2P;
	//			// 公网p2p测试
	//			XT_P2PTEST_REQ req;
	//			req.dest_ip = pFriend->ip;
	//			req.dest_port = pFriend->port;
	//			req.ipType = XT_P2PTEST_REQ::P2P;
	//			req.id = g_ImUserInfo.GetId();
	//			req.toid = pFriend->id;
	//			g_pProtocol->SendP2pTestPacket(req);
	//		}
	//		// 公网p2p测试失败，则由服务器中转通讯
	//		else if(pFriend->connFlag == FRIEND_INFO::TRY_P2P)
	//		{
	//			pFriend->ip = 0;
	//			pFriend->port = 0;
	//			pFriend->connFlag = FRIEND_INFO::SERVER;
	//		}
	//	}
	//}

}

// 收到其他用户的测试数据包
void CMainFrame::OnRecvP2pTest(const char * data,WORD wRet)
{
	LPXT_P2PTEST_IND pInd = (LPXT_P2PTEST_IND)data;
	FRIEND_INFO * pFriend;
	if(wRet == 0)
	{
		// 接收成功，更新该好友的ip,key，以便同其连接
		pFriend = ::GetFriendInfo(pInd->id);
		if(pFriend)
		{
			memcpy(pFriend->key, pInd->key,16);
			if(pInd->ipType == XT_P2PTEST_IND::LAN)
			{
				pFriend->localIP = pInd->ip;
				pFriend->localPort = pInd->port;
				pFriend->connFlag = FRIEND_INFO::LAN;
			}
			else
			{
				pFriend->ip = pInd->ip;
				pFriend->port = pInd->port;
				pFriend->connFlag = FRIEND_INFO::P2P;
			}
		}		
	}
}

void CMainFrame::OnBasicInfoAck(const char * data,WORD wRet)
{
	LPXT_GET_FRIEND_BASICINFO_NOTIFY pAck = (LPXT_GET_FRIEND_BASICINFO_NOTIFY)data;
	int i;
	uint32 id;
	if(wRet != 0)
	{
		return;
	}
	int nGroupFriend = 0;
	for( i = 0;i < pAck->count ;i++ )
	{
		id = pAck->FriendInfo[i].nID;

		vector<uint32>::iterator it = std::find(m_arrFriendsID_Basic.begin(), m_arrFriendsID_Basic.end(), id);	
		if(it != m_arrFriendsID_Basic.end())
		{
			m_arrFriendsID_Basic.erase(it);
		}

		FRIEND_INFO * pFriend = FindFriend(id);
		bool bExist = true;
		if(pFriend == NULL)
		{
			nGroupFriend++;
			bExist = false;
			continue;
			//pFriend = new FRIEND_INFO();
			//pFriend->id = id;
		}

		pFriend->bBasicInfo = true;
		pFriend->sex  = pAck->FriendInfo[i].nSexy;
		pFriend->SetUserName(pAck->FriendInfo[i].szUserName);
		strcpy(pFriend->nickname,pAck->FriendInfo[i].szNickName);
		strcpy(pFriend->comment_name,pAck->FriendInfo[i].szCommentName);
		strcpy(pFriend->face_url,pAck->FriendInfo[i].szHeadUrl);
		strcpy(pFriend->user_sign,pAck->FriendInfo[i].szUserSign);
		pFriend->SetGuid(pAck->FriendInfo[i].szGUID);//新增自营GUID标识
		pFriend->bind_mobile = pAck->FriendInfo[i].nMobileStatus;
		pFriend->jifen = pAck->FriendInfo[i].LoveFlag;		// 服务端通过LoveFlag字段传过来主管权限

		if ( bExist )
		{
			// 如果好友在好友列表中则更新,并获取web头像
			if ( pFriend->status!=XTREAM_OFFLINE )
			{
				// 下载该用户的自定义头像图片
				g_webUserHeadIconProcess.GetUserHeadIcon(pFriend, CWnd::FromHandle(GetHWND()));
			}
			//	UpdateFriend(pFriend,FALSE);
		}
		else
		{
			// 该好友在本地缓存中不存在
			;
		}

		m_pRecentList->UpdateUser(pFriend);
	}


	// 过滤获取群好友基本资料的情况
	//if( (pAck->count) > 0 && (pAck->count != nGroupFriend))
	//{
	//	// 重新刷新列表
	//	UpdateFriendList();
	//}
	//m_pFriendList->InvalidateList();
	if(!m_arrFriendsID_Basic.empty())
	{
		ImGetFriendBasicInfo();
	}
	else
	{
		KillTimer(GetHWND(), TIMER_SORTLIST);
		SetTimer(GetHWND(),TIMER_SORTLIST,100,NULL);
	}
}

void CMainFrame::OnBnClickedButtonAdd()
{
	CAddFriendOrClusterDlg::Show();
}

// 打开聊天窗口，供外部调用，减少重复代码
void CMainFrame::OpenP2PMsgDlg(FRIEND_INFO *pFriend)
{
	if(g_mapMsgUnRead.size()>0)
	{
		PopUnReadMsgDlg(pFriend);
	}
	//自己不能给自己发消息
	if(pFriend->GetId() == g_ImUserInfo.GetId())
		return;

	CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
	if ( pDlg->GetHWND()==NULL )////没有打开窗口
	{
		pDlg->NotifyServer_OpenedMsgDlg();			
	}
#ifdef _CS_PLAT_EX_
	if(CDuiCSPlatDlg::GetInstance() != NULL)
	{
		if(CMainFrame::IsBuyer(pFriend->GetId()))
			CDuiCSPlatDlg::GetInstance()->SetLayoutPageCust();
		else
			CDuiCSPlatDlg::GetInstance()->SetLayoutPageTeam();
	}
	if(!CMainFrame::IsBuyer(pFriend->GetId()))
	{
		CDuiCSPlatDlg::GetInstance()->SetListCurselNull();
		
	}
	if( CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()) && CMainFrame::IsBuyer(pFriend->GetId()))//有买家和卖家的对话最前展示
	{
		CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
	}
#elif defined _WORK_EX_
	if( CDuiCounsel::GetInstance() != NULL && ::IsWindowVisible(CDuiCounsel::GetInstance()->GetHWND()))//有买家和卖家的对话最前展示
	{
		CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
	}
#else
	if( CDuiCounsel::GetInstance() != NULL && ::IsWindowVisible(CDuiCounsel::GetInstance()->GetHWND()) && (CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())) )//有买家和卖家的对话最前展示
	{
		CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
	}
#endif
	else////没有咨询框最前展示
	{
		if ( /*pDlg->*/::IsWindowVisible(pDlg->GetHWND())==FALSE )
		{
#ifndef _CS_PLAT_EX_
			pDlg->CenterWindow();
#endif
			pDlg->ShowWindow(SW_SHOW);
		}
	}

	if (/*pDlg->IsIconic()*/::IsMinimized(pDlg->GetHWND()) )
	{
		pDlg->ShowWindow(SW_RESTORE);
	}

#ifdef _CS_PLAT_EX_
	if (g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != NULL && g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg->GetHWND() != NULL &&
		::IsMinimized(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg->GetHWND()) )
	{
		g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg->ShowWindow(SW_RESTORE);
	}
	g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg = pDlg;
#endif

#ifdef _CS_PLAT_EX_
	for(int nDlg = 0; nDlg < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); nDlg++)
	{
		if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg] != pDlg)
		{
			g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[nDlg]->ShowWindow(false);
		}
	}
	if( CDuiCSPlatDlg::GetInstance() != NULL && CMainFrame::IsBuyer(pFriend->GetId()))
	{
		CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
	}
	::SetForegroundWindow(pDlg->GetHWND());
	::BringWindowToTop(pDlg->GetHWND());
#elif defined _WORK_EX_
	if( CDuiCounsel::GetInstance() != NULL )
	{
		CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
	}
	::SetForegroundWindow(pDlg->GetHWND());
	::BringWindowToTop(pDlg->GetHWND());
#else
	if( CDuiCounsel::GetInstance() != NULL && (CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())) )
	{
		CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
	}
	::SetForegroundWindow(pDlg->GetHWND());
	::BringWindowToTop(pDlg->GetHWND());
#endif
}