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
	g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg = pDlg;//���õ�ǰ��ʾ�����촰��
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
	// ���ж���ЩͼƬ�����Ƿ�������Զ��������
	size_t nC = arrImg.size();
	if(nC == 0)
		return false;
	arrShow.resize(nC);

	EMOTION_ID id;
	size_t i;
	for( i = 0;i < nC;i++)
	{
		// ��ѯ���ر�������Ƿ��и����ӵĶ���
		id = FindFaceUseUrl(arrImg[i].c_str());
		// ���ر�����û�и�ͼƬ����Ҫ����
		if( id.nGroup == -1)
		{
			// �Ȼ�ø��ļ���md5��
			GetMD5WebFileName(arrImg[i].c_str(),sMD5);		
			// �жϸ��ļ��Ƿ��Ѿ�����
			if(FileExist(g_LocalData.GetImageCanchePath()+"\\"+sMD5))
			{
				// ���ļ��Ѿ�����,��ֱ��ʹ�������ļ�����ʾ
				arrShow[i] = sMD5;
			}
			else
			{
				arrShow[i] = sMD5;
				// ��������
				DOWNLOAD_OBJ param;
				param.handle = NULL;
				param.sSaved = g_LocalData.GetImageCanchePath()+"\\"+sMD5;
				param.sUrl = arrImg[i].c_str();
				param.timer = 0;
				param.state = DS_DOWNLOADING;
				arrDownloadTask.push_back(param);
			}
		}
		// �����и��ļ����棬����Ҫ����
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

////�����б���ʹ�õĲ˵�
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
//#define	ID_DELMFRIEND			0x0114	// ɾ���������
//#define ID_MODF_COMMNAME		0x0115	// �޸ĺ��ѱ�ע����
//#define ID_CREATE_TEMPCLUSTER	0x0116 
//#define	ID_FRIENDARTICLE		0x0117
//#define	ID_SEND_GROUP_MESSAGE	0x0118
//#define	ID_ADD_CGROUP			0x0119
//#define	ID_ADD_FRIEND			0x0120
//#define	ID_RENAME_GROUP			0x0121
//#define ID_REMOVE_BLACKLIST		0x0122
//#define ID_ADD_BLACKLIST		0x0123

// �����ʱ�ܹ��������С��ID��0�����ҵĺ��ѣ�1���ڡ�İ���ˡ���2���ڡ���������
const int c_minGroupID = 3;	
// �����ʱ�ܹ�����������ID
const int c_maxGroupID = MY_TEAM_GROUP_ID - 1;

//�ӱ����ļ��л�ȡ������Ϣ
void CMainFrame::LoadLocalFriends()
{
	::ClearFriendList();

	memset(&g_GroupInfo,0,sizeof(g_GroupInfo));

	g_GroupInfo.dwUserId = g_ImUserInfo.GetId();

#ifdef _CS_PLAT_EX_  //��ţ�ͷ�����̨(ȥ��������)
	g_GroupInfo.count = 2;
	g_GroupInfo.group[0].group_index = 1;
	g_GroupInfo.group[0].group_father = -1;
	g_GroupInfo.group[0].group_state = FLS_CLOSE;
	strcpy(g_GroupInfo.group[0].name, "�����ϵ�ͻ�");


	g_LocalData.GetGroupInfo(g_GroupInfo);
	g_LocalData.GetFriendList(g_lsFriend);	
#else
	g_GroupInfo.count = 3;
	g_GroupInfo.group[0].group_index = 0;
	g_GroupInfo.group[0].group_father = -1;
	g_GroupInfo.group[0].group_state = FLS_CLOSE;
	strcpy(g_GroupInfo.group[0].name, "�ҵĺ���");

	g_GroupInfo.group[1].group_index = 1;
	g_GroupInfo.group[1].group_father = -1;
	g_GroupInfo.group[1].group_state = FLS_CLOSE;
	strcpy(g_GroupInfo.group[1].name, "İ����");

	g_GroupInfo.group[2].group_index = 2;
	g_GroupInfo.group[2].group_father = -1;
	g_GroupInfo.group[2].group_state = FLS_CLOSE;
	strcpy(g_GroupInfo.group[2].name, "������");

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

	// �տ�ʼ��ȡ�����б�
	if ( start_pos==0 )
	{
		m_arrFriend.clear();
		m_vecPos.clear();
	}
	g_pProtocol->SendReq(req);
}

//������ʾ�����б�
void CMainFrame::UpdateFriendList()
{
	FL_ITEM_POS pos = m_pFriendList->GetCurSel();
	
	//�������group ������Item
	m_pFriendList->DeleteAllItems();

	//groupInfo >> groupList�� groupID��ൽ500
	bool bGroupAdd[500];
	memset(bGroupAdd, 0, sizeof(bGroupAdd));

	for ( int i=0; i<g_GroupInfo.count; i++ )
	{
		int nSubGroupId, nParentGroupId, nSubGroupIndex;
		deque<int> dequeId;
		
		nSubGroupId = g_GroupInfo.group[i].group_index;
		nParentGroupId = g_GroupInfo.group[i].group_father;
		//�븸���ȴ���
		do
		{
			dequeId.push_front(nSubGroupId);
			nSubGroupIndex = g_GroupInfo.FindItemIndex(nSubGroupId);
			if(nSubGroupIndex != -1)
			{
				nSubGroupId = g_GroupInfo.group[nSubGroupIndex].group_father;
			}
			else//�Ҳ�������
			{
				dequeId.clear();
				break;
			}
		}while(nSubGroupId != -1 && !bGroupAdd[nSubGroupId] && dequeId.size()<10);//�����з���

		while( !dequeId.empty())
		{
			nSubGroupId = dequeId.front();
			nSubGroupIndex = g_GroupInfo.FindItemIndex(nSubGroupId);
			nParentGroupId = g_GroupInfo.group[nSubGroupIndex].group_father;
			m_pFriendList->AddSubGroup(nParentGroupId, nSubGroupId, g_GroupInfo.group[nSubGroupIndex].name);
			//��״̬
			m_pFriendList->SetGroupState(nSubGroupId,(FLS_STATE)g_GroupInfo.group[nSubGroupIndex].group_state);
			bGroupAdd[nSubGroupId] = true;
			dequeId.pop_front();
		}
	}

	//������Ϣ>> groupList
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
	
		// �Ƿ��б�ע����
		CString sName;
		if(pFriend->comment_name[0] == 0)
			sName = pFriend->nickname;
		else
			sName = pFriend->comment_name;
		
		m_pFriendList->AddItem(pFriend->group_index, sName, (DWORD)(pFriend));
		//�����Լ�����ID
		if(pFriend->GetId() == g_ImUserInfo.GetId())
			g_ImUserInfo.group_index = pFriend->group_index;
	}

	//���ں�������
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
	// �����ļ��м��غ����б�
	LoadLocalFriends();
	UpdateFriendList();

	// ���ѷ�������
	ImGetGroupInfo();
	// ���ͻ�ȡ�����б������
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

#ifdef _CS_PLAT_EX_  //��ţ�ͷ�����̨(ȥ��������)
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
		sprintf(g_GroupInfo.group[0].name,"�ҵĺ���");

		g_GroupInfo.group[1].group_index = 1;
		g_GroupInfo.group[1].group_father = -1;
		g_GroupInfo.group[2].group_state = FLS_CLOSE;
		sprintf(g_GroupInfo.group[1].name,"İ����");

		g_GroupInfo.group[2].group_index = 2;
		g_GroupInfo.group[2].group_father = -1;
		g_GroupInfo.group[2].group_state = FLS_CLOSE;
		sprintf(g_GroupInfo.group[2].name,"������");

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
		//���鿪��״̬
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

	// ���Ұ����жϷ������Ƿ����ҵ��Ŷӷ��飬���û�о����
#ifdef _BUSINESS_VESION	
#if !defined(_CS_PLAT_EX_) && !defined(_WORK_EX_)
	if (AddGroup_MyTeam())
	{	// �ɹ���ӣ��ٴθ��·����б�
		Sleep(2000);		// ��ӳɹ����һ���ٸ����б�������Ӻ����ظ�����
		ImGetGroupInfo();
	}
#endif
#endif
}

#ifdef _BUSINESS_VESION
// ����ҵ��Ŷӷ��飬����Ѿ����ھͲ����
static BOOL g_isAddMyTeam = FALSE;	// ȷ��ֻ�����һ�Ρ��ҵ��Ŷӡ�
BOOL CMainFrame::AddGroup_MyTeam()
{
	if (g_isAddMyTeam)
		return FALSE;
	g_isAddMyTeam = TRUE;

	// �����Ŷ�
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
	{	// û�С��ҵ��Ŷӡ�������̨������Ϣ���
		ImAddGroup(-1, MY_TEAM_GROUP_ID, "�ҵ��Ŷ�");
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
			//����Է����������״̬����Ϊ�뿪��
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
				// ��ʱʹ��id������
				// ��ӵ������б���ȥ
				//ASSERT(FALSE);
				strcpy(pFriend->nickname, UserInfoToStr(pFriend));
				if(!AddFriend(pFriend,false))
					delete pFriend;
			}
		}

		// �жϺ����б��Ƿ��ȡ���
		if ( ack.m_nNextPos!=0xFFFF )
		{
			// ������ȡ�����б�
			ImGetFriendList(ack.m_nNextPos);
			UpdateFriendList();
		}
		// ��ȡ�����б����
		else
		{
			// �뱾�ػ���Ƚ�,�ж��Ƿ��к�����Ҫ����
			// ��ʱȫ�ֺ����б���Ϊ���ػ�����ص�����
			vector<uint32> addiFriends;
			vector<uint32> oldFriends;
			int j,c;
			GetDiffFriends(g_lsFriend,m_arrFriend,addiFriends);
			// ɾ���ں����б��в����ڵľɺ���
			FindOldFriends(g_lsFriend,m_arrFriend,oldFriends);
			c = oldFriends.size();
			for(j = 0; j < c;j++)
			{
				DelFriend(oldFriends[j]);	
			}
			// ��ȡ���ػ�����û�����ϵĺ��ѵĻ�������
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


		//�ڻ�ȡ�����б��ok�󣬴���ͷ��ĶԻ���
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
				pFriend->group_index = 1;//İ����
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

		// �жϺ����б��Ƿ��ȡ���
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
			strcpy(error,"��������������б�����ʱ��");			
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
	// �����º����б��еĳ�Ա�Ƿ��ھɺ����б��д���
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
		// �жϺ��������Ƿ���Ч�������Ч�����ȡ
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
		// ��ɾ��İ����
		if((*it)->group_index == 1)
		{
			continue;
		}

		for(i = 0; i < c; i++)
		{
			if((*it)->GetId() == newFriends[i].friendId)
				break;
		}
		// ������б���û�У���Ϊ�������ݣ���Ҫɾ��
		if(i == c)
		{
			rst.push_back((*it)->GetId());
		}
	}
}

void CMainFrame::DelFriend(FRIEND_INFO *pFriend)
{

	g_pMessageHandleWnd->CloseP2PSendMsgDlg(pFriend->GetId());

	//m_wndGroupList��ɾ��
	m_pFriendList->DeleteItem(pFriend);

	//g_lsFriend��ɾ��
	FRIEND_LIST::iterator it = find(g_lsFriend.begin(), g_lsFriend.end(), pFriend);
	ASSERT(it != g_lsFriend.end());
	if(it != g_lsFriend.end())
	{
		//���Ⱥ��Ա�ı�ע��
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

	//���º�������
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
	DeleteStranger(pFriend->GetId());//�̼Ұ����İ�����б�
#endif

	g_lsFriend.push_back(pFriend);

	m_pFriendList->AddItem(pFriend->group_index, UserInfoToStr(pFriend),(DWORD)pFriend );

	if(bSel)
		m_pFriendList->SetCurSel(pFriend);

	return true;
}

//�̼Ұ����İ�����б�
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
#ifndef _CS_PLAT_EX_ //��ţ�ͷ�����̨
	CString str;
	FRIEND_INFO* pFriend = NULL;
	//CWnd* pStatic;
	FL_ITEM_POS pos;

	pos = m_pFriendList->GetCurSel();


	pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(pos.nGroupID,pos.nItem);
	if(pFriend->group_index == GROUPID_STRANGER)
	{
		str.Format("����ϵ����İ������,��Ϊ����,����İ������ɾ��");
	}
	else if (pFriend->group_index == GROUPID_BLACKLIST)
	{
		str.Format("����ϵ���ں�������,��Ϊ����,���Ӻ�������ɾ��");
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
			sInfo.Format("�û�\"%s\"�Ѿ��������������",pFriend->nickname);
		}
		else
		{
			sInfo.Format("�û�%d�Ѿ��������������",uid);
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

//�������Ժ����б������Ҽ�
LRESULT CMainFrame::OnFriendListNotify(WPARAM wParam, LPARAM lParam)
{
	FL_ITEM_POS pos=m_pFriendList->GetCurSel();
	int nGroup = pos.nGroupID;
	if(nGroup == -1)//�������Ժ����б����Ϣ������
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
	// ���˫��
	case NM_FL_LBUTTONDBLCLICK:
		// ��ǰѡ����ĳ������
		if(pFriend)
		{
			//�Լ����ܸ��Լ�����Ϣ
			if(g_ImUserInfo.status!=XTREAM_OFFLINE && pFriend->GetId() != g_ImUserInfo.GetId())
				ShowMessageDlg(pFriend);
		}

		//KillTimer(TIMER_USER_CARD);
		break;
	case NM_FL_DRAGDROP://�Ϸ�
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
			// ��������ĺ����ϵ�������
			if ( nDestGroupId == GROUPID_BLACKLIST && pFriend->group_index != GROUPID_STRANGER )
			{
				if( pFriend->group_index == GROUPID_BLACKLIST)
				{
					bMove = true;
				}
				else
				{
					CString str;
					str.Format("ȷ�Ͻ� %s �����������",UserInfoToStr(pFriend));
					if ( IDYES==CMessageBox::Model(GetHWND(), str,IM_CAPTION,MB_YESNO|MB_ICONWARNING) )
					{
						/*str.Format("�Ƿ�ͬʱ���Լ���%s�ĺ����б����Ƴ�?",UserInfoToStr(pFriend));
						if ( IDYES == MessageBox(str,IM_CAPTION,MB_YESNO|MB_ICONWARNING) )*/
						{
							bBlack = true;
						}
						bMove = true;
					}
				}
			}
			// ���������϶���İ������
			else if(nDestGroupId == GROUPID_STRANGER )
			{
				if(  pFriend->group_index == GROUPID_STRANGER )
				{
					bMove = true;
				}
				else
				{
					CString str;
					str.Format("ȷ�Ͻ� %s ����İ������",UserInfoToStr(pFriend));
					if ( IDYES==CMessageBox::Model(GetHWND(),str,IM_CAPTION,MB_YESNO|MB_ICONWARNING) )
					{
						// ֱ��ɾ���ú���
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
			// ��İ�����϶��������飨��Ӻ��ѣ�
			else if(pFriend->group_index == GROUPID_STRANGER )
			{
				XT_FRIENDGROUP_MOD_REQ req;
				req.action = GROUP_ADD;
				req.uid    = g_ImUserInfo.GetId();
				req.fid    = pFriend->GetId();
				req.group_id = nDestGroupId;
				g_pProtocol->SendReq(req);
				// İ�����϶���������
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
			// ������������
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
			// �޸��û���
			if(bMove)
			{
				if(bBlack)
				{	
					// ������
					ImSetUserGroup(pFriend->GetId(),nDestGroupId,XT_FRIENDGROUP_MOD_REQ::ACTION_BLACK);			
				}
				else
				{
					//�޸��û���
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
	case NM_FL_GROUPNAME://�������޸�
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
					//����������
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

			// ���ݲ�ͬ�������ʾ�����˵�������

			// �жϵ�ǰ�Ƿ���ѡ�е���
			if ( nGroup > -1 )
			{
				if ( nItem > -1 ) //���ѽڵ�
				{
					CString strItem;
					
					CString strGroup = m_pFriendList->GetGroupName(nGroup);
#ifdef _CS_PLAT_EX_
					if(strGroup != "�����ϵ�ͻ�" && strGroup!= "������")
#else
					if(strGroup != "İ����" && strGroup!= "������")
#endif
					{
						if(pFriend->GetId() != g_ImUserInfo.GetId())
						{
							strItem.Format("���ͼ�ʱ��Ϣ");
							popmenu->AppendMenu(MF_STRING,ID_P2PSENDMSG,strItem);
							//popmenu->AppendMenu(MF_SEPARATOR,0,"");
						}

						strItem.Format("�޸ı�ע��");
						popmenu->AppendMenu(MF_STRING,ID_MODF_COMMNAME,strItem);
						
						if(pFriend->GetId() != g_ImUserInfo.GetId())
						{
							//popmenu->AppendMenu(MF_SEPARATOR,0,"");
						}

						//��ţ����֯�ܹ��еĲ������ƶ�
						if(nGroup < FEINIU_GROUP_ID && nGroup > -1)
						{
							strItem.Format("ɾ����ϵ��");
							popmenu->AppendMenu(MF_STRING,ID_DELFRIEND,strItem);
							//popmenu->AppendMenu(MF_SEPARATOR,0,"");
							
							CMenu menuGroup;
							menuGroup.CreatePopupMenu();
							//��ţ����֯�ܹ��еĲ������ƶ�
							for(int i=0; i<g_GroupInfo.count;i++)//////�޸��ж��߼�����֯�ܹ�������䶯 2014-11-12
							{
								if(g_GroupInfo.group[i].group_index < FEINIU_GROUP_ID)
								{
									//İ���˺ͺ�����ȥ��
									if(/*g_GroupInfo.group[i].group_index!=1&&*/g_GroupInfo.group[i].group_index!=2)
									{
				                       menuGroup.AppendMenu(MF_STRING,ID_GROUP_CHOOSE_BEG + i,g_GroupInfo.group[i].name);
									}
								}
							}
							popmenu->AppendMenu(MF_POPUP, (UINT)menuGroup.m_hMenu, "�Ѻ����ƶ���...");
							//popmenu->AppendMenu(MF_SEPARATOR,0,"");

								
							strItem.Format("�ƶ���������");
							popmenu->AppendMenu(MF_STRING,ID_REMOVE_BLACKLIST,strItem);
							//popmenu->AppendMenu(MF_SEPARATOR,0,"");
						}

						
						if(pFriend->GetId() != g_ImUserInfo.GetId())
						{
							strItem.Format("�鿴����");
							popmenu->AppendMenu(MF_STRING,ID_FRIENDINFO,strItem);
						}
					}
					else
					{
						popmenu->AppendMenu(MF_STRING,ID_DELFRIEND,"�Ӹ���ɾ��");
						//popmenu->AppendMenu(MF_SEPARATOR,0,"");
                #ifndef  _CS_PLAT_EX_
						popmenu->AppendMenu(MF_STRING,ID_ADD_BLACKLIST,"��Ϊ����");
						//popmenu->AppendMenu(MF_SEPARATOR,0,"");
                #endif
						popmenu->AppendMenu(MF_STRING,ID_FRIENDINFO,"�鿴����");
					}
				}
				else//��ڵ�
				{
					if ( g_ImUserInfo.status!=XTREAM_OFFLINE )
					{
						CString strGroup = m_pFriendList->GetGroupName(nGroup);

						if (nGroup == MY_TEAM_GROUP_ID)
						{	// �ҵ��Ŷӷ���ֻ��"����ԱȺ����Ϣ"
							popmenu->AppendMenu(MF_STRING, ID_SEND_GROUP_MESSAGE, "����ԱȺ����Ϣ");
						}
						else
						{
							if (strGroup != "İ����" && strGroup != "������")
							{
								popmenu->AppendMenu(MF_STRING, ID_SEND_GROUP_MESSAGE, "����ԱȺ����Ϣ");
								//��ţ����֯�ܹ��еĲ������ƶ�
								if (nGroup < 100 && nGroup > -1)
								{
                         
                            #ifndef _CS_PLAT_EX_
									//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
									popmenu->AppendMenu(MF_STRING, ID_ADDGROUP, "�����");
									//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
									popmenu->AppendMenu(MF_STRING, ID_ADD_CGROUP, "�������");
									//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
									popmenu->AppendMenu(MF_STRING, ID_ADD_FRIEND, "��Ӻ���");
                            #endif  
									//�ҵĺ��Ѳ�����༭
									if (nGroup != 0)
									{
										//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
										popmenu->AppendMenu(MF_STRING, ID_RENAME_GROUP, "��������");
										//popmenu->AppendMenu(MF_SEPARATOR, 0, "");
										popmenu->AppendMenu(MF_STRING, ID_DELGROUP, "ɾ����");
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
				//// ��Ӧѡ��˵�����Ϣ��Ӧ
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
		////���ȵ���δ����Ϣ��ϵ��
		/*WORD nExitWarn = 0;
		g_LocalData.GetCloseTalkWarnConfig(nExitWarn);///��ȡ�����Ƿ񵯳�δ����Ϣ����*/

		FRIEND_INFO *pFriend = (FRIEND_INFO *)m_pFriendList->GetItemData(pos.nGroupID,pos.nItem);
		OpenP2PMsgDlg(pFriend);	
	}
	else if(pos.nGroupID != -1)////չ����
	{
		BOOL bExpand = m_pFriendList->IsExpand(pos.nGroupID);
		m_pFriendList->ExpandGroup(pos.nGroupID, !bExpand);
	}
}

void CMainFrame::PopUnReadMsgDlg(FRIEND_INFO* pShowFriend)
{
	////��ʾδ����Ϣ����
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
		if ( pDlg->GetHWND()==NULL )////û�д򿪴���
		{
			pDlg->NotifyServer_OpenedMsgDlg();			
		}
#ifdef _CS_PLAT_EX_
		if( CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()) && CMainFrame::IsBuyer(pFriend->GetId()))//����Һ����ҵĶԻ���ǰչʾ
		{
			CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg,nUnReadCnt);
		}
		else if(CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()) && !CMainFrame::IsBuyer(pFriend->GetId()))
		{
			g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);//����δ����Ϣ�Ĵ���
			CDuiCSPlatDlg::GetInstance()->SetFriendUnMsg(pFriend, true);//���ú���δ����Ϣ���
		}
#else
		if( CDuiCounsel::GetInstance() != NULL && ::IsWindowVisible(CDuiCounsel::GetInstance()->GetHWND()) && CMainFrame::IsBuyer(pFriend->GetId()))//����Һ����ҵĶԻ���ǰչʾ
		{
			CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg,nUnReadCnt);
		}
#endif
		else////û����ѯ����ǰչʾ
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
	   g_mapMsgUnRead.clear();////���������ٴδ���ɺ����
	   WORD nVK = 0;
	   g_LocalData.SaveLoginUnReadMsgCfg(nVK);///����δ����Ϣ��Ϊ0
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
					CMessageBox::Model(GetHWND(), _T("���ں͸��û��ĻỰ����رջỰ��ɾ��~"), IM_CAPTION, MB_ICONWARNING);
					return;
				}
			}
			
		}
	}
	
#endif

	if(pFriend->group_index > FEINIU_GROUP_ID)
	{
		CMessageBox::Model(GetHWND(), _T("��ţ���еĺ��Ѳ���ɾ����Ŷ~~"), IM_CAPTION, MB_ICONWARNING);
	}

	
	CString str;
	str.Format("ȷ��Ҫɾ�� %s ��?",UserInfoToStr(pFriend));

	if (CMessageBox::Model(GetHWND(),str,IM_CAPTION,MB_YESNO|MB_ICONWARNING)!=IDYES)
	{
		return ;
	}

	//ɾ���û�
	XT_FRIENDGROUP_MOD_REQ req;

	req.action = GROUP_DEL;
	req.group_id = 0;
	req.fid = pFriend->GetId();
	req.uid = g_ImUserInfo.GetId();

	if(pFriend->group_index != 1)
	{
		//���û���id
		g_pProtocol->SendReq(req);
	}
	//ɾ���û�
	::RegUserOperation(0x00c3);

	DelFriend(pFriend);
}

void CMainFrame::OnSearchXtream()
{
	tr1::shared_ptr<CAddFriendOrClusterDlg> pDlg = CAddFriendOrClusterDlg::Show();
	pDlg->SetSearchType(FS_XTREAM);

}

// ��ȡ�µ���ID���Զ������ʱ�õ���������-1��ʾʧ��
int CMainFrame::GetNewGroupID()
{
	int nGroupId = c_minGroupID;

	//�����ܹ�ʹ�õ�groupId
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
	{	// �����Զ������ɵ���ID��Ҫռ��Ĭ�Ϸ����ID
		nGroupId = c_minGroupID;
	}
	if (nGroupId > c_maxGroupID)
	{	// ����IDԽ��
		return -1;
	}

	// ������id�ظ�
	while(std::find(listGroupId.begin(), listGroupId.end(), nGroupId) != listGroupId.end())
	{
		nGroupId += 1;
	}
	if (nGroupId > c_maxGroupID)
	{	// ����IDԽ��
		return -1;
	}

	return nGroupId;
}

void CMainFrame::CreateGroup(int nParentGroupId, int& nGroupId, CString& strGroupName)
{
	if ( g_GroupInfo.count > 50 )
	{
		ShowMessage("���ֻ�ܽ�50���Զ����飡",MB_OK|MB_ICONINFORMATION);
		return ;
	}

	// ��ȡ����ID
	nGroupId = GetNewGroupID();
	if(nGroupId == -1)
	{	// ������idʧ��
		ShowMessage("�����Զ������ʧ�ܣ�",MB_OK|MB_ICONINFORMATION);
		return;
	}

	//�����Ƿ�������������	����߼�����bug��
	//֧��ͬ�������֣�����ɶ?
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
	
	//��g_GroupInfo���������
	int nIndex = g_GroupInfo.count;
	g_GroupInfo.group[nIndex].group_index = nGroupId;
	g_GroupInfo.group[nIndex].group_father = nParentGroupId;
	sprintf(g_GroupInfo.group[nIndex].name,strGroupName);
	g_GroupInfo.count++;
	g_GroupInfo.Sort();	
	
}
void CMainFrame::OnAddGroup()
{
	//��ǰѡ����鼰�丸��
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
	
	//�½���
	int nNewGroupId = -1;
	CString strNewGroupName = "�½���";
	CreateGroup(nParentGroupId, nNewGroupId, strNewGroupName);

	//��ȡ��ǰѡ����ĸ���Ϊ�½���ĸ���
	
	if(nNewGroupId != -1)
	{
		m_pFriendList->InsertGroup(nParentGroupId , nNewGroupId, strNewGroupName);
		m_pFriendList->SetCurSel(nNewGroupId, -1);
		SetTimer(GetHWND(), TIMER_EDITGOUP, 20, NULL);

		ImAddGroup(nParentGroupId, nNewGroupId, strNewGroupName);
	}

	m_pFriendList->SelectGroup(nSelGroupId, FALSE);
}



//�������
void CMainFrame::OnAddSubGroup()
{
	//��ǰѡ����鼰�丸��
	int nSelGroupId = m_pFriendList->GetCurSel().nGroupID;
	int nParentGroupId = nSelGroupId;

	//�½���
	int nNewGroupId = -1;
	CString strNewGroupName = "�½�����";
	CreateGroup(nParentGroupId, nNewGroupId, strNewGroupName);

	if(nNewGroupId != -1)
	{
		m_pFriendList->InsertGroup(nParentGroupId , nNewGroupId, strNewGroupName);
		//�����
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

//����Ա����Ϣ
void CMainFrame::OnSendGroupMessage()
{
	int nGroup = m_pFriendList->GetCurSel().nGroupID;
	FL_GROUP_INFO *pGroup = m_pFriendList->GetGroupData(nGroup);
	if(pGroup == NULL)
		return;

	// ͬʱ������ĺ���Ҳ��ӵ����촰��
	vector<FL_GROUP_INFO*> vecSubGroup;
	pGroup->GetSubGroup(vecSubGroup);
	int nGroupIndex = pGroup->dwData;
	FRIEND_LIST::iterator it,itE;
	it=g_lsFriend.begin();
	itE=g_lsFriend.end();
	// ���ѡ�����İ���˺ͺ������򲻴���
	if(nGroupIndex == 1 || nGroupIndex == 2)
	{
		return ;
	}
	// �ҵ������ڵĺ���
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
		CMessageBox::Model(GetHWND(), _T("��ţ���еķ��鲻��ɾ����Ŷ~~"), IM_CAPTION, MB_ICONWARNING);
	}
	//�ж������Ƿ��к���
	if(pGroup->nFrdCnt > 0)
	{
		ShowMessage("��Ҫ���鼰�������еĺ�����ջ��ƶ��������飬����ɾ�������");
	}
	else
	{
		CString str;
		str.Format("��ȷ��Ҫɾ����������������飬Ҳ�ᱻɾ��");
		if ( IDYES == CMessageBox::Model(GetHWND(), str, IM_CAPTION, MB_YESNO|MB_ICONWARNING) )
		{

			//��ȡ����id
			int iGroupInd = -2,iParentGroupId = -2;
			iGroupInd = g_GroupInfo.FindItemIndex(group_id);
			iParentGroupId = g_GroupInfo.group[iGroupInd].group_father;

			//��ȡ�������飬��ʵ����Ҫ��������id
			vector<FL_GROUP_INFO*> vecSubGroup;
			pGroup->GetSubGroup(vecSubGroup);
			//���Լ��Ž�ȥ
			//ɾ��g_GroupInfo �и��鼰����������
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
				ShowMessage("ɾ����ʧ��!~");
				return;
			}
						
			//g_GroupInfo�汾��
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

	//����
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

		loginInfo.login_flag |= CLoginDlg::m_LoginFlag; //// ������¼�ĳ�ʼ״̬ 
		////loginInfo.login_flag |= XT_LOGIN_REQ::LOGINBYNAME;
		strcpy(loginInfo.uLoginKey.username ,g_ImUserInfo.GetUserName().c_str());
		// �������
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

//����״̬����֪ͨ
void CMainFrame::OnFriendStatusChange(const char *data,WORD wRet)
{
	RecentList_UserStatusInd(data, wRet);
	if ( wRet==0)
	{
		LPXT_STATUS_IND ind =(LPXT_STATUS_IND)data;
		FRIEND_INFO *pFriend = FindFriend(ind->fid);

		//���������Ļ�����״̬ͼ����뿪һ��
		if ( ind->status == XTREAM_HIDE )
		{
			ind->status = XTREAM_OFFLINE;
		}

		if ( pFriend )
		{
			pFriend->ver = ind->version;
            //��������
			if ( ind->status == XTREAM_ONLINE && pFriend->status != XTREAM_ONLINE)
			{
#ifdef _VER_INTERNAL_	// �ڲ���
#else			
  #ifdef  _CS_PLAT_EX_///��ţ�ͷ�����̨
  #else
				PlayTone(EVOICE_FRIEND_ONLINE);
				ShowUserHintDlg(pFriend,UserInfoToStr(pFriend), "������", m_hWnd);
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
				// ����״̬
				pFriend->status = ind->status;
				// ״̬�ı����Ҫ�����б����ʾ
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
		//�޸���
		if ( ack->action==GROUP_MODIFY )
		{
			//�Ӻ�����->�ҵú���,�Զ�����
			if ( ack->flag==0x01 )	//��Ҫ��֤
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
			else if ( ack->flag==0x02 ) //�ܾ�
			{
				ShowUserAuthInfo(ack->fid,0x02,"",ack->group_id);
			}
			else if( ack->flag == 0x00 )	//�����κ��˼�Ϊ����
			{
				pFriend = GetFriendInfo(ack->fid);
				if(pFriend)
				{
					pFriend->group_index = ack->group_id;
					// ����ɹ�
					pFriend->bInviting = false;
					UpdateFriendList();
				}
			}

		//	if( ack->flag != 0x00 )	//�޸��黹û�гɹ�,����Ҫ�ָ���ǰ�����Ϸ�ʱ������ǰ�����޸�
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


		/*ʵ��������Ӻ���ack��zhangmin*/
		if ( ack->action==GROUP_ADD )
		{
			//��Ӻ�����Ҫ��֤
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
					// ���������˼ӶԷ�Ϊ���ѣ���ֱ�ӷ�����֤��Ϣ
					XT_USERAUTH_REQ req;
					req.from_id = g_ImUserInfo.GetId();
					req.groupId = ack->group_id;
					req.to_id   = ack->fid;
					req.action  = XT_USERAUTH_REQ::ACTION_REQUEST;// ����
					req.send_time = time(NULL);
					strcpy(req.info,"");
					g_pProtocol->SendReq(req);
					::WritePrivateProfileString("AddFriends",sID,"0",sAllowIni);
				}
				else
				{
					// ��ʾ��֤��ʾ��
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
			//��Ӻ��ѱ�ֱ�Ӿܾ�
			else if ( ack->flag==0x02 )
			{
				ShowUserAuthInfo(ack->fid,0x02,"",ack->group_id);
			}
			//��Ӻ��ѳɹ�
			else if ( ack->flag==0x00 )//�ɹ�
			{
				FRIEND_INFO * pFriend ;
				// �ж��Ƿ�Ϊ����Ӻ���
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

//������֤����\ͨ��\�ܾ�
void CMainFrame::OnUserAuthInd(const char *data,WORD wRet)
{
	LPXT_USERAUTH_REQ req = (LPXT_USERAUTH_REQ)data;

	if ( wRet==0 && req )
	{
		if ( req->action==0x01 )//ͬ��
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

				// ��øú��ѵ���ϸ��Ϣ
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
				// �յ��Է�����֤�������֮ǰ����������Է�����Ϊ���ѵ����ã����������ͬ��Է�����֤
				XT_USERAUTH_REQ reqAuth;
				reqAuth.action = 0x01;//ͬ��
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
		// groupList��State >> groupInfo
		for(int i=0; i<g_GroupInfo.count; i++ )
		{
			int nSubGroupId = g_GroupInfo.group[i].group_index;
			int nGroupState =  m_pFriendList->GetGroupState(nSubGroupId);
			//�����
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
	{	// �쳣�����µ�¼
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
	//�޸��û���
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
	req.group_pid = -2;//��̨����ʹ��
	req.group_id = nGroupId;
	sprintf(req.name,strGroupName);
	req.uid   = g_ImUserInfo.GetId();

	g_pProtocol->SendReq(req);
}

void CMainFrame::ImDelGroup( int nGroupId)
{
	XT_GROUPINFO_MOD_REQ req;

	req.action   = GROUP_DEL;
	req.group_pid = -2;//��̨����ʹ��
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
	// ��ȡ�������������к�
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

	/*�����������Ȳ�����
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

	//�����б���º������״̬Ҳ����
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
		//ͳ�����к������������ߺ�������(�ų���������İ����)
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
	////��ɾ��
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

//action=0,����
//action=1,ͬ��
//action=2,�ܾ�
//aciton=3,����Ϊ����֪ͨ
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
			strMsg = strUserInfo + "���������Ϊ����";
			break;
		case 1:
			strMsg = strUserInfo + "�����������������";
			break;
		case 2:
			strMsg = strUserInfo + "�ܾ��������������";
			break;
		case 3:
			strMsg = strUserInfo + "�ɹ��������Ϊ����";
			break;
		default:
			break;
	}

	
	if( !strMsg.IsEmpty() )
		strMsg += "\r\n ������Ϣ:" + strVerify;

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
		pFriend->group_index = 1; //İ����
		pFriend->status = XTREAM_ONLINE;

		//��ӵ�İ����
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

	//���κ�����
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
	strTitle.Format("%s�����ļ�:",UserInfoToStr(pFriend));
	ShowUserHintDlg(pFriend, strTitle,req.name,m_hWnd);
	FlashWindow(pDlg->GetHWND(), TRUE);
	pDlg->RecvSendFileReq(data,wRet);

	m_pRecentList->AddUser(pFriend);
	RECENTLIST_OP(AddUser(pFriend))
}

// ���յ������û�����������Ϣ
void CMainFrame::OnRecvP2PMsg(const char *data,WORD wRet)
{
	LPXT_MSG msg = (LPXT_MSG)data;

#ifdef _MSG_JSON_
	 //��ʱ��Ϣ��ʱ����Json����
	if(msg->data_type == XT_MSG::IM_MSG )
	{
		Json::Reader jReader;
		Json::Value jvRecv;
		
		jReader.parse(msg->data,jvRecv);
		std::string strMsgData = jvRecv["txt"].asString();
		uint32 p_id = jvRecv["pid"].asUInt();
		
		//json��ʽ�����ɹ�
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

	//�Է���Ϣ�ķ�ʽ������Ӫ�Ķ���,һ��Ϊת�ӻỰ�������ܽ���֮��ת���¿ͷ�
	if(msg->data_type==XT_MSG::IM_MSG_ORDER_SEQ)
	{
		CString strOrder = msg->data;//������ʽΪ ID:�����ŵļ����ַ���
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
		if(strTran.Find("tranReq") != -1)//ת������
		{
			strTran = strTran.Right(strTran.GetLength() - 8);
			CString strCusterName = strTran.Left(strTran.Find(":"));
			CString strCusterId = strTran.Right(strTran.GetLength() - strTran.ReverseFind(':') - 1);
			//�������ջ��߾ܾ���ʾ��

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

		if(strTran.Find("tranAck") != -1)//ת��Ӧ��
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
			if(!bCanTran)//�����Խ���
			{
				XT_MSG msgTran;
				memset(&msgTran,0,sizeof(msgTran));
				msgTran.ver         = XTREAM_CLIENT_VERSION;
				msgTran.from_id     = g_ImUserInfo.GetId();
				msgTran.send_time   = /*time(0)*/g_LocalData.GetTimeNow();
				msgTran.to_id		= msg->from_id;
				msgTran.data_type   = XT_MSG::CUST_SER_TRAN_REQ;
				CString text;
				text.Format("tranFail:%d", nCusterId);//����
				strcpy(msgTran.data, text);
				msgTran.data_len=strlen(text)+1;
				//SF_SetTransMode(CIMProtocol::SMT_SERVER);
				g_pProtocol->SendP2PMsg(msgTran,CIMProtocol::SMT_SERVER);
			}
			else
			{
				if(nAccept == 1)//����
				{
					pDlg->TranCuster(msg->from_id);
				}
				else if(nAccept == 2)//�ܾ�
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
			//�ûỰ�����
			int nCuster = atoi(strTran.Right(strTran.GetLength() - 9));
			if(CDuiMsgNotice::GetInstance() != NULL && IsWindowVisible(CDuiMsgNotice::GetInstance()->GetHWND()))
				CDuiMsgNotice::GetInstance()->DeleteTranMsg(nCuster);
			CMessageBox::Model(NULL, "�ûỰ�����", NULL, MB_OK);
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
			pFriend->group_index = 1;//İ����
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
		// �޸��ŶӶ���
		if(CMainFrame::IsBuyer(msg->from_id))
		{
			CDuiCSPlatDlg::DelWaitFriend(msg->from_id);
		}
#endif

		////��ҳ������Ϣ
		FRIEND_INFO *pUser = FindFriend(msg->from_id);
		if(pUser == NULL ||
			(pUser != NULL && CMainFrame::IsBuyer(pUser->GetId())))
		{	// Ϊ���ʱ�Զ��Ӵ��������治�Զ��Ӵ�
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
			pUser->group_index = 1;//İ����
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

			//��ӵ�İ����
			AddFriend(pUser,true);
			SelItem(pUser);
			//ImGetUserStatus(pUser->GetId());
#ifdef _CS_PLAT_EX_
			if(CDuiCSPlatDlg::GetInstance() != NULL && CDuiCSPlatDlg::GetInstance()->m_vecP2PSendMsgDlg.size() > 1 && CMainFrame::IsBuyer(pUser->GetId()))
				bIsShow = FALSE;
			else
				bIsShow = TRUE;
#else
			// �����촰��
			if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() > 1 && CMainFrame::IsBuyer(pUser->GetId()))
				bIsShow = FALSE;
			else
				bIsShow = TRUE;
#endif

			// ���û�����ȡ��������
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

		////��ʾδ����Ϣ
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
		//pDlg->UpdateFriendState();//����֮�������û�״̬Ϊ����
		// �ͻ�����ʾ�Զ��Ӵ�����
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
			msgAcceptAuto.data_type   = XT_MSG::AUTO_REPLY;	// �Զ��Ӵ���Ϣ

			CString text(acceptMsg.strText);
#ifdef _CS_PLAT_EX_
			if(text.IsEmpty())
			{	
				text.Format("���ã���ţ�ͷ�\"%s\"�ܸ���Ϊ������", g_ImUserInfo.GetUserName().c_str());
			}
#endif
			strcpy(msgAcceptAuto.data,"[�Զ��Ӵ�]" + text);

			msgAcceptAuto.data_len=strlen(text)+1;
			pDlg->AddTalkMsg(msgAcceptAuto, false);
			g_LocalData.SaveTalkMsg(msgAcceptAuto, pUser->GetId());
		}
#if defined (_BUSINESS_VESION) && !defined(_CS_PLAT_EX_) && !defined(_WORK_EX_) //�̼Ұ�
		// ���ù���״̬
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
		pFriend->group_index = 1;//İ����
		pFriend->status = XTREAM_ONLINE;
		sprintf(pFriend->nickname,msg->from_nickname);
		pFriend->SetUserName("");
		pFriend->SetGuid("");
		pFriend->SetPassword("");

		//��ӵ�İ����
		AddFriend(pFriend,true);
		SelItem(pFriend);
		//ImGetUserStatus(pFriend->GetId());		

#ifdef _BUSINESS_VESION
		CMainFrame::GetInstance()->Hangup();
#endif

		// ���û�����ȡ��������
		//unsigned threadID = 0;
		//_beginthreadex(NULL, 0, UpdateFriendInfo, (void*)msg->from_id, 0, &threadID);
	}
	else
	{
		strcpy(msg->from_nickname, pFriend->nickname);//����ǳ�
	}

	if ( pFriend->status==XTREAM_OFFLINE )
	{
		if((msg->data_type != XT_MSG::SF_FILE_ACK && msg->data_type != XT_MSG::SF_PACKET_ACK))
			pFriend->status=XTREAM_ONLINE;

		UpdateFriend(pFriend);

		AddHideTick(pFriend->GetId());
	}

	// ����Զ��ظ�����Ϊ������������̨����Ϣ���������У����Կ��߳��������Զ��ظ�����Ȼ����ֽ��ն��ղ�����Ϣ�������
	if(msg->data_type == XT_MSG::IM_MSG && (g_ImUserInfo.status == XTREAM_LEAVE || g_ImUserInfo.status == XTREAM_BUSY))
	{
		unsigned threadID;
		_beginthreadex(NULL, 0, AutoReply, (void*)msg->from_id, 0, &threadID);
	}

	//���κ�����
	if ( pFriend->group_index==2 )
	{
		return ;
	}
	
	bool bFindDlg = g_pMessageHandleWnd->FindP2PSendMsgDlg(pFriend->GetId());

	if((msg->data_type == XT_MSG::SF_FILE_ACK || msg->data_type == XT_MSG::SF_PACKET_ACK)
		&& CGroupSendDlg::GetInstance() != NULL && ::IsWindow(CGroupSendDlg::GetInstance()->GetHWND()))
	{	// Ⱥ�������У����ͷ�����ͼƬ�����е����Э��
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

	if(msg->data_type == XT_MSG::WIN_SHAKE)		// ������Ϣ���ж��Ƿ�����
	{
		WORD nVK;
		g_LocalData.GetShakeWinConfig(nVK);
		if(nVK == 0)
		{	// ��֧�����������������촰��
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
			pDlg->UpdateFriendState();//����֮�������û�״̬Ϊ����
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
	//��ʾ���ѵĺ��
	if(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != NULL && g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != pDlg && !CMainFrame::IsBuyer(pFriend->GetId()))
		CDuiCSPlatDlg::GetInstance()->SetFriendUnMsg(pFriend, true);
	if( CDuiCSPlatDlg::GetInstance() != NULL && CMainFrame::IsBuyer(pFriend->GetId()))
	{
		CDuiCSPlatDlg::GetInstance()->SetCustUnMsgVisable(true);//��ʾ�ͻ���ǩ�ĺ��
		//��Ϊ����͵�ǰ�ͻ�����ʱ�����µ����촰�ڲ�����ǰ��
		if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() <= 1)
		{
			//CDuiCSPlatDlg::GetInstance()->m_bIsUpdate = TRUE;
			//CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
		}
	}
	else if(CDuiCSPlatDlg::GetInstance() != NULL && !CMainFrame::IsBuyer(pFriend->GetId()))
	{
		CDuiCSPlatDlg::GetInstance()->SetTeamUnMsgVisable(true);//��ʾ�ҵ��Ŷӱ�ǩ�ĺ��
	}
	if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() > 1 && g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != pDlg)
	{
		pDlg->ShowWindow(false);
	}
	else
	{
		g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg = pDlg;//���õ�ǰ��ʾ�����촰��
		if(!CMainFrame::IsBuyer(pFriend->GetId()))
		{
			CDuiCSPlatDlg::GetInstance()->SetListCurselNull();//���ò�ѡ�пͻ�����
		}
	}
#elif defined _WORK_EX_
	if( CDuiCounsel::GetInstance() != NULL)
	{
		//��Ϊ����͵�ǰ�ͻ�����ʱ�����µ����촰�ڲ�����ǰ��
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
		//��Ϊ����͵�ǰ�ͻ�����ʱ�����µ����촰�ڲ�����ǰ��
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
		// �����Ӧ����ʾ����Ϣ
		strncpy(msg->data,sShowMsg,sizeof(msg->data)-1);
		// ��ʼ��������
		if(pDlg && pDlg->GetHWND() != NULL)
		{
			pDlg->Download(arrDownloadTask);
		}
	}

	pDlg->ProcessMsg(*msg);

	CString strTitle;

	//����Ҳ�����ǰ��SendMessageDlg,�򵯳�hint dlg
	switch ( msg->data_type )
	{
	case XT_MSG::IM_MSG://������Ϣ
		{
		//////��ʾδ����Ϣ
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
				// �������Բ�����ʾ��;
			}
			else
			{
				strTitle.Format("%s˵:",UserInfoToStr(pFriend));
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
			strTitle.Format("%s˵:",UserInfoToStr(pFriend));
			
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

#ifdef _BUSINESS_VESION	// ���������ϵ�ͻ�����̨
		if (g_ImUserInfo.status!=XTREAM_OFFLINE/* && !m_bHasUpdateNearCommCust*/)
		{
			// ͳ����Ҫ���µ���̨�������ϵ�ͻ���Ϣ
			CMainFrame::GetInstance()->StatNeedUpdateCustInfo(pFriend);
			// ���������ϵ�ͻ���Ϣ����̨
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
		////��ʾδ����Ϣ
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
		////��ʾδ����Ϣ
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
	case XT_MSG::MAGIC_FACE://ħ������
		{
			PlayTone(EVOICE_RECEIVE_MSG);
			CRecvPacket rp((char *)msg->data,sizeof(msg->data));
			char   face_name[255];
			char   face_url[255];

			rp>>face_name>>face_url;

			CString strMessage;
			strMessage.Format("ħ������\"%s\"",face_name);

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
	case XT_MSG::FILE_REQ://���ļ�����
		{
			PlayTone(EVOICE_RECEIVE_MSG);
			strTitle.Format("%s�����ļ�:",UserInfoToStr(pFriend));
			CRecvPacket rp(msg->data,sizeof(msg->data));
			uint16 file_id;
			char   file_name[255];
			rp>>file_id>>file_name;

			ShowUserHintDlg(pFriend, strTitle,file_name,m_hWnd);
			FlashWindow(pDlg->GetHWND(), TRUE);
		}
		break;
	case XT_MSG::WIN_SHAKE:
		{	// ���ڶ���
			WORD nVK = 0;
			g_LocalData.GetShakeWinConfig(nVK);
			if(nVK == 1)////֧������
			{
				PlayTone(EVOICE_SHOCK_SCREEN);
			}
		}
		break;
	default:break;
	}			
	// �ͻ�����ʾ�Զ��ظ�����
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
			msgReplyAuto.data_type   = XT_MSG::AUTO_REPLY;	// �Զ��Ӵ���Ϣ

			CString text(item.strText);
			strcpy(msgReplyAuto.data,"[�Զ��ظ�]" + text);
			msgReplyAuto.data_len=strlen(text)+1;
			pDlg->AddTalkMsg(msgReplyAuto, false);
			g_LocalData.SaveTalkMsg(msgReplyAuto, pFriend->GetId());
		}
	}
}

//
void CMainFrame::OnP2PMsgAck(const char *data,WORD wRet)
{
	if ( wRet==SERVER_TIMEOUT && g_ImUserInfo.status!=XTREAM_OFFLINE)//P2p���ͳ�ʱ
	{
		// ���p2p����ʧ�����ɷ�����ת��
		SendDataStruct *pSendData = (SendDataStruct *)data;
		
		CRecvPacket rp((char *)pSendData->data,MAX_RECV_BUFFERS);

		XT_HEAD hd;
		rp.GetXtHead(hd);
		XT_MSG msg;
		char buff[MAX_RECV_BUFFERS];
		// �ҵ�����Ŀ�ĺ���
		FRIEND_INFO *pFriend = FindFriend(hd.did);
		// ��������ڸú��ѣ��������Ѿ�ɾ���˸ú���?��
		if(pFriend == NULL)
		{
			return;
		}
		// �������ݲ�����
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
			// ������Ϣת��
			FRIEND_INFO *pFriend = FindFriend(msg.to_id);
			if ( pFriend )
			{
				//bool bFind = g_pMessageHandleWnd->FindP2PSendMsgDlg(pFriend->id);

				//if ( bFind )
				//{
				//	g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend)->OnP2pMsgTimeOut();
				//}
				TRACE("p2p����ʧ�ܣ�����������ת��\n");
				// �������ʧ��,��Ҫ���»�ȡ����״̬
				if(pFriend->connFlag == FRIEND_INFO::P2P)
				{
					pFriend->ip = 0;
					pFriend->port = 0;
					pFriend->connFlag = FRIEND_INFO::P2P_FAILED;
				}
				// p2p����ʧ�ܳ��Է�����ת��
				msg.recv_flag = CIMProtocol::SMT_SERVER;
//#ifndef _DEBUG
				g_pProtocol->SendP2PMsg(msg,CIMProtocol::SMT_SERVER);
//#endif
			}	
			
		}
		else
		{
		//	ShowMessage("������Ϣ����ʧ��!");
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
	TRACE("ERROR:������󣡣�\n");
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

	if ( msg->cluster_type==PERM_FAMILY )//���ǹ̶�Ⱥ������
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
			if ( pCluster->bRefused==false )////���ղ���ʾȺ��Ϣ
			{
				bool bFind = g_pMessageHandleWnd->FindClusterSendMsgDlg(pCluster->nFamilyID);

				CClusterSendMsgDlg *pDlg = g_pMessageHandleWnd->GetClusterSendMsgDlg(pCluster);
				if(TranslateMsg(msg->data,sShowMsg,arrDownloadTask))
				{
					// �����Ӧ����ʾ����Ϣ
					strncpy(msg->data,sShowMsg,sizeof(msg->data)-1);
					// ��ʼ��������
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
					msg->msg_type     = 1;//Ⱥ����Ϣ

					m_pRecentList->AddCluster(pCluster);
					RECENTLIST_OP(AddCluster(pCluster))
					break;

				case 9://ħ������
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
							// �����Ӧ����ʾ����Ϣ
							strncpy(msg->data,sShowMsg,sizeof(msg->data)-1);
						}
						g_LocalData.SaveTalkMsg(*msg);

						if(pCluster->radio == CLUSTER_INFO::SHOW_NUM)
						{	// ��ӦȺ��Ŀ��ʾ����
							pCluster->nMsgNum++;
							m_pClusterList->UpdateGroup(pCluster);
						}
					}
					else if(pCluster->radio == CLUSTER_INFO::NORECV)/////�ܾ�����
					{
						;//����ʾ��Ϣ
					}
				}
			}
		}//end if ( pCluster)
	}

	if ( msg->cluster_type == TEMP_FAMILY )//������ʱȺ������
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
			// �����Ӧ����ʾ����Ϣ
			strncpy(msg->data,sShowMsg,sizeof(msg->data)-1);
			// ��ʼ��������
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
					strMessage.Format("%s ������ħ������\"%s\"",pFriend->nickname,face_name);
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
	// ���ҵ��ú���
	FRIEND_INFO* pFriend = FindFriend(nFriendID);
	if(pFriend==NULL)
		return -1;
	m_pFriendList->SetCurSel(pFriend);

	// ��Ӧ�ƶ����ѵ���Ϣ����
	int nGroupIndex = g_GroupInfo.FindItemIndex(nNewGroupID);
	//OnGroupListNotify(NM_FL_DRAGDROP, nGroupIndex);

	return 0;
}

void CMainFrame::MakeTempCluster(FL_GROUP_INFO* pGroup)
{
	// ͬʱ������ĺ���Ҳ��ӵ����촰��
	vector<FL_GROUP_INFO*> vecSubGroup;
	pGroup->GetSubGroup(vecSubGroup);

	CLUSTER_INFO *pTempClusterInfo=new CLUSTER_INFO();
	int nGroupIndex = pGroup->dwData;
	CString strGroupName = pGroup->strTitle;///Ⱥ������;
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
	// ���ѡ�����İ���˺ͺ������򲻴���
	if(nGroupIndex == 1 || nGroupIndex == 2)
	{
		delete pTempClusterInfo;
		return ;
	}
	// �ҵ����������ߵĺ���
	for(;it!=itE;it++)
	{
		// �жϺ����Ƿ�����ѡ�����������
		BOOL bIsInGroup = FALSE;
		if ((*it)->group_index == nGroupIndex)
		{	// ��������ѡ����
			bIsInGroup = TRUE;
		}
		else
		{
			for (int i = 0; i < vecSubGroup.size(); ++i)
			{
				if ((*it)->group_index == vecSubGroup[i]->dwData)
				{	// ������������
					bIsInGroup = TRUE;
					break;
				}
			}
		}

		if (bIsInGroup)
		{
			if((*it)->GetId() != g_ImUserInfo.GetId())//����������Լ�����ӵ���ʱȺ
				pTempClusterInfo->members.push_back(**it);
			// ��ʱȺ�ĳ�Ա�������ܳ���MAX_TEMPFAMILYMEM_NUM
			if(pTempClusterInfo->Count() >= (MAX_TEMPFAMILYMEM_NUM-1))
				break;
		}
	}
	// ������㹻�������ߺ�������Ⱥ��
	if(pTempClusterInfo->Count() > 0)
	{
		if(pTempClusterInfo->Count() == 1)
		{
			// ����һ�ˣ���������
			CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(&pTempClusterInfo->members[0]);
			// 			if ( pDlg->GetHWND()==NULL )
			// 				pDlg = CDuiP2PSendMsgDlg::NewDlg(&pTempClusterInfo->members[0],pmSingleTalk, CWnd::FromHandle(GetDesktopWindow()));
			// 			pDlg->ShowWindow(true);
		}
		else
		{
			// Ӧ�����ж��Ƿ���ڸ�Ⱥ�ĶԻ���
			CP2PSendMsgDlg *pDlg =NULL;
			pDlg=g_pMessageHandleWnd->GetTempGroupSendMsgDlg(nGroupIndex);

			if(pDlg==NULL)
			{
				// ��������ڸöԻ����򴴽�֮
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

#ifdef _BUSINESS_VESION	// ����������촰�ڳ�Ա������Ϊ�ҵ��Ŷӻ������ӷ��飬����ʾ���������ۡ���ť
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
	strItem="ɾ������";
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
	// �Ƿ��б�ע����
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
		// �޸ı�ע�ɹ�
		//CMessageBox::Prompt("�޸ı�ע�ɹ����������Ѿ��������ύ����Ϣ��");
		ModifyName(pAck);
		////////////////////////////////////////////////////////////////////
	}
	else
	{
		// �޸ı�עʧ��
		CMessageBox::Prompt(g_pProtocol->GetLastError(),"�޸ı�עʧ��");
		//ResetName(pAck->fid);
	}
}

// �Զ��ظ��������߻���æµʱ��
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

	msg.data_type   = 0;//��ʱ������Ϣ

	CString text("[�Զ��ظ�]" + item.strText);//�Զ��ظ�ǰ�������ʾ
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
	// ��ȡ������Ϣ
	XT_INFO_IND *pFriendInd = (XT_INFO_IND *)data;

	unsigned threadID = 0;
	_beginthreadex(NULL, 0, UpdateFriendInfo, (void*)pFriendInd->fid, 0, &threadID);
}

// ���º�����Ϣ
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
	//	// ��ú��ѵ����ӳɹ�
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
	//	// ��ú��ѵ����Ӳ���ʧ��
	//	CRecvPacket rp((char *)data,MAX_RECV_BUFFERS);
	//	XT_HEAD hd;
	//	rp.GetXtHead(hd);
	//	id = hd.did;
	//	pFriend = FindFriend(id);
	//	if(pFriend)
	//	{
	//		// ��������ʧ�ܣ���������p2p����
	//		if(pFriend->connFlag == FRIEND_INFO::TRY_LAN)
	//		{
	//			pFriend->connFlag = FRIEND_INFO::TRY_P2P;
	//			// ����p2p����
	//			XT_P2PTEST_REQ req;
	//			req.dest_ip = pFriend->ip;
	//			req.dest_port = pFriend->port;
	//			req.ipType = XT_P2PTEST_REQ::P2P;
	//			req.id = g_ImUserInfo.GetId();
	//			req.toid = pFriend->id;
	//			g_pProtocol->SendP2pTestPacket(req);
	//		}
	//		// ����p2p����ʧ�ܣ����ɷ�������תͨѶ
	//		else if(pFriend->connFlag == FRIEND_INFO::TRY_P2P)
	//		{
	//			pFriend->ip = 0;
	//			pFriend->port = 0;
	//			pFriend->connFlag = FRIEND_INFO::SERVER;
	//		}
	//	}
	//}

}

// �յ������û��Ĳ������ݰ�
void CMainFrame::OnRecvP2pTest(const char * data,WORD wRet)
{
	LPXT_P2PTEST_IND pInd = (LPXT_P2PTEST_IND)data;
	FRIEND_INFO * pFriend;
	if(wRet == 0)
	{
		// ���ճɹ������¸ú��ѵ�ip,key���Ա�ͬ������
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
		pFriend->SetGuid(pAck->FriendInfo[i].szGUID);//������ӪGUID��ʶ
		pFriend->bind_mobile = pAck->FriendInfo[i].nMobileStatus;
		pFriend->jifen = pAck->FriendInfo[i].LoveFlag;		// �����ͨ��LoveFlag�ֶδ���������Ȩ��

		if ( bExist )
		{
			// ��������ں����б��������,����ȡwebͷ��
			if ( pFriend->status!=XTREAM_OFFLINE )
			{
				// ���ظ��û����Զ���ͷ��ͼƬ
				g_webUserHeadIconProcess.GetUserHeadIcon(pFriend, CWnd::FromHandle(GetHWND()));
			}
			//	UpdateFriend(pFriend,FALSE);
		}
		else
		{
			// �ú����ڱ��ػ����в�����
			;
		}

		m_pRecentList->UpdateUser(pFriend);
	}


	// ���˻�ȡȺ���ѻ������ϵ����
	//if( (pAck->count) > 0 && (pAck->count != nGroupFriend))
	//{
	//	// ����ˢ���б�
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

// �����촰�ڣ����ⲿ���ã������ظ�����
void CMainFrame::OpenP2PMsgDlg(FRIEND_INFO *pFriend)
{
	if(g_mapMsgUnRead.size()>0)
	{
		PopUnReadMsgDlg(pFriend);
	}
	//�Լ����ܸ��Լ�����Ϣ
	if(pFriend->GetId() == g_ImUserInfo.GetId())
		return;

	CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
	if ( pDlg->GetHWND()==NULL )////û�д򿪴���
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
	if( CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindowVisible(CDuiCSPlatDlg::GetInstance()->GetHWND()) && CMainFrame::IsBuyer(pFriend->GetId()))//����Һ����ҵĶԻ���ǰչʾ
	{
		CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
	}
#elif defined _WORK_EX_
	if( CDuiCounsel::GetInstance() != NULL && ::IsWindowVisible(CDuiCounsel::GetInstance()->GetHWND()))//����Һ����ҵĶԻ���ǰչʾ
	{
		CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
	}
#else
	if( CDuiCounsel::GetInstance() != NULL && ::IsWindowVisible(CDuiCounsel::GetInstance()->GetHWND()) && (CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())) )//����Һ����ҵĶԻ���ǰչʾ
	{
		CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
	}
#endif
	else////û����ѯ����ǰչʾ
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