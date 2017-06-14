
// xTreamaMainWnd.cpp : implementation file
//
#include "stdafx.h"
#include "..\IMClient.h"
#include "..\resource.h"

#include "p2pSendmsgDlg.h"
#include "..\FriendInfoDlg.h"
#include "SAConfig.h"
#include "localdata.h"
#include "FSDialog.h"
#include "hintdlg.h"
#include "MsgDlg.h"
#include "impublic.h"
#include "xtpacket.h"
#include "UserAuthInfoDlg.h"
#include "UserAuthDlg.h"
#include "MessageHandleWnd.h"
#include ".\floorpagefriend.h"
#include "webprocess.h"

#include "..\utils\Function.h"
#include "selectgroupdlg.h"
#include "ModfCommNameDlg.h"
#include "msgobj.h"
#include "emotiondlg.h"
#include "usercarddlg.h"
#include "SMSDlg.h"
#include "AddBlackDlg.h"
#include "../ImTest/ImTest.h"
#include <deque>

#include "../json/json/json.h"
#include "../MsgManageDlg.h"

#include "..\tone.h"
#include "..\..\HmgSkin\HmgSkin\FlatPopupMenu.h"

//�����б���ʹ�õĲ˵�
#define	ID_SENDMSG				0x0101
#define	ID_FRIENDINFO			0x0102
#define	ID_SEARCH				0x0103
#define	ID_DELFRIEND			0x0104
#define	ID_ADDGROUP				0x0105
#define	ID_DELGROUP				0x0106
#define	ID_MSGHISTORY			0x0107
#define	ID_RENAMEGROUP			0x0108
#define	ID_REFRESH_FRIENDLIST	0x0109
#define	ID_SHOWALL				0x0110
#define	ID_HIDEOFFLINE			0x0111
#define	ID_FRIENDALBUM			0x0112
#define	ID_FRIENDBLOG			0x0113
#define	ID_DELMFRIEND			0x0114	// ɾ���������
#define ID_MODF_COMMNAME		0x0115	// �޸ĺ��ѱ�ע����
#define ID_CREATE_TEMPCLUSTER	0x0116 
#define	ID_FRIENDARTICLE		0x0117
#define	ID_SEND_GROUP_MESSAGE	0x0118
#define	ID_ADD_CGROUP			0x0119
#define	ID_ADD_FRIEND			0x0120
#define	ID_RENAME_GROUP			0x0121
#define ID_REMOVE_BLACKLIST		0x0122
#define ID_ADD_BLACKLIST		0x0123

#define  IDC_XT_FRIENDTREE  0x05001//�����б�ؼ�ID

// �����ʱ�ܹ��������С��ID��0�����ҵĺ��ѣ�1���ڡ�İ���ˡ���2���ڡ���������
const int c_minGroupID = 3;	
// �����ʱ�ܹ�����������ID
const int c_maxGroupID = MY_TEAM_GROUP_ID - 1;

CFloorPageFriend *g_FloorPageFriend = NULL;
HWND    g_friendFloor = NULL;

#include "..\FriendInfoDlg.h"

#include "DuiCounsel.h"
extern CDuiCounsel *g_pDuiCounsel;

//#define	SEQ_TEMP_FAMILY	0x0001	// ��ʱȺ����
// const char *szProductID = NULL ��ƷID��ֻ�ڴ�������Ի����ʱ����
CP2PSendMsgDlg * ShowMessageDlg(FRIEND_INFO *pFriend, const char *szProductID = NULL)
{
	CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend, szProductID);

	if ( pDlg->m_hWnd==NULL )
	{
		pDlg->Create(CP2PSendMsgDlg::IDD,CWnd::GetDesktopWindow());
		pDlg->NotifyServer_OpenedMsgDlg();
	}

	if( g_pDuiCounsel != NULL && ::IsWindowVisible(*g_pDuiCounsel) && pFriend->GetId() >= 5000000)
	{
		g_pDuiCounsel->ShowSelectP2PSendMsgDlg(pDlg);
	}
	else
	{
		if ( pDlg->IsWindowVisible()==FALSE )
			pDlg->ShowWindow(SW_SHOW);
	}

	if (pDlg->IsIconic() )
	{
		pDlg->ShowWindow(SW_RESTORE);
	}

	CRect rcDlg;
	pDlg->GetWindowRect(&rcDlg);
	::SetWindowPos(pDlg->GetSafeHwnd(), HWND_TOPMOST, rcDlg.left, rcDlg.top, rcDlg.Width(), rcDlg.Height(),SWP_SHOWWINDOW);
	::SetWindowPos(pDlg->GetSafeHwnd(), HWND_NOTOPMOST, rcDlg.left, rcDlg.top, rcDlg.Width(), rcDlg.Height(),SWP_SHOWWINDOW);

	pDlg->SetForegroundWindow();
	pDlg->BringWindowToTop();
	if( g_pDuiCounsel != NULL )
	{
		CRect rcCounsel;
		::GetWindowRect(*g_pDuiCounsel, &rcCounsel);
		::SetWindowPos(*g_pDuiCounsel, pDlg->GetSafeHwnd(), rcCounsel.left, rcCounsel.top, rcCounsel.Width(), rcCounsel.Height(),SWP_NOACTIVATE);
	}
	return pDlg;
}

IMPLEMENT_DYNAMIC(CFloorPageFriend, CFloorPage)
IMPLEMENT_HMG_SCROLLBAR(CFloorPageFriend)

CFloorPageFriend::CFloorPageFriend(CWnd* pParent /*=NULL*/)
: CFloorPage(CFloorPageFriend::IDD, pParent)
{
	m_bShowAllFriend=TRUE;
	m_bDisconnectFlag  = false;
	m_nLastHeartSeq = 0;
	m_bIsHideTick = false;
}

//Destructor
CFloorPageFriend::~CFloorPageFriend()
{
	ClearFriendList();
}


BEGIN_MESSAGE_MAP(CFloorPageFriend, CFloorPage)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_IMFRIEND_LIST_NOTIFY, OnGroupListNotify)
	ON_COMMAND(ID_SENDMSG, OnSendMsg)
	ON_COMMAND(ID_FRIENDINFO,OnFriendInfo)
	ON_COMMAND(ID_FRIENDALBUM,OnFriendAlbum)
	ON_COMMAND(ID_FRIENDARTICLE,OnFriendArticle)
	ON_COMMAND(ID_FRIENDBLOG,OnFriendBlog)
	ON_COMMAND(ID_DELFRIEND,OnDelFriend)
	ON_COMMAND(ID_DELMFRIEND,OnDelMFriend)
	ON_COMMAND(ID_ADDGROUP,OnAddGroup)
	ON_COMMAND(ID_DELGROUP,OnDelGroup)
	ON_COMMAND(ID_RENAMEGROUP,OnRenameGroup)
	ON_COMMAND(ID_MSGHISTORY,OnMsgHistory)
	ON_COMMAND(ID_SEARCH,OnSearchXtream)
	ON_COMMAND(ID_REFRESH_FRIENDLIST, OnRefreshFriendList)
	ON_COMMAND(ID_SHOWALL,OnShowAll)
	ON_COMMAND(ID_HIDEOFFLINE,OnHideOffline)
	ON_COMMAND(ID_MODF_COMMNAME,OnModfCommName)
	ON_COMMAND(ID_CREATE_TEMPCLUSTER,OnCreateTempCluster)
	ON_COMMAND(ID_SEND_GROUP_MESSAGE,OnSendGroupMessage)
	ON_COMMAND(ID_ADD_CGROUP,OnAddSubGroup)
	ON_COMMAND(ID_ADD_FRIEND,OnBnClickedButtonAdd)
	ON_COMMAND(ID_RENAME_GROUP,OnRenameGroup)
	ON_COMMAND(ID_REMOVE_BLACKLIST,OnRemoveBlackList)
	ON_COMMAND(ID_ADD_BLACKLIST,OnAddBlackList)
	ON_MESSAGE(WM_HINTDLG_CLICK, OnHintDlgClick)
	SET_HMG_SCROLLBAR(IDC_XT_FRIENDTREE)
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_XT_MESSAGE()
	ON_WM_ERASEBKGND()
	ON_WM_INITMENUPOPUP()
	ON_WM_MEASUREITEM()
	ON_MESSAGE(WM_USER_HEAD_NOTIFY,OnWebIconNotify)
	ON_MESSAGE(WM_CHANGE_FRIENDGROUP,OnChangeFriendGroup)
	ON_MESSAGE(WM_THREAD_NOTIFY,OnThreadNotify)
	ON_COMMAND_RANGE(ID_GROUP_CHOOSE_BEG,ID_GROUP_CHOOSE_END,OnGroupChoose)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_WEBDATA_NOTIFY,OnWebDataNotify)
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CFloorPageFriend)
	// �û���֤
	ON_XT_COMMAND(CMD_USERAUTH_IND,OnUserAuthInd)
	// ��֤����
	ON_XT_COMMAND(CMD_USERAUTH_ACK,OnUserAuthAck)
	// �޸ĺ��ѷ���
	ON_XT_COMMAND(CMD_FRIENDGROUP_MOD_ACK,OnFriendGroupModAck)
	// ����ϵͳ��Ϣ
	ON_XT_COMMAND(CMD_SERVERMSG_IND,OnRecvSvrMsg)
	// ����
	ON_XT_COMMAND(CMD_DIRECTORY_ACK,OnHeartBeat)
	// ����״̬�ı�֪ͨ
	ON_XT_COMMAND(CMD_FRIEND_STATUS_IND,OnFriendStatusChange)
	// �յ�������Ϣ
	ON_XT_COMMAND(CMD_P2PMSG_RECV,OnRecvP2PMsg)
	// p2p����Ӧ��
	ON_XT_COMMAND(CMD_P2PMSG_ACK,OnP2PMsgAck)
	// �����б�
	ON_XT_COMMAND(CMD_FRIENDLIST_ACK,OnFriendListAck)
	// ��ȡ������Ϣ
	ON_XT_COMMAND(CMD_GROUPINFO_GET_ACK,OnGroupInfoGet)
	// ��ȡ������ϸ����
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoGet)
	ON_XT_COMMAND(CMD_STATUS_GET_ACK,OnStatusGetAck)
	// �޸�����״̬Ӧ��
	ON_XT_COMMAND(CMD_STATUS_MOD_ACK,OnStatusModAck)
	// �յ�Ⱥ������Ϣ
	ON_XT_COMMAND(CMD_CLUSTERMSG_RECV,OnRecvClusterMsg)
	// �޸���
	ON_XT_COMMAND(CMD_GROUPINFO_MOD_ACK,OnGroupModAck)
	// �޸ĺ��ѱ�ע
	ON_XT_COMMAND(CMD_MOD_FCNAME_ACK,OnModFCNameAck)
	// ��ȡ���ѻ�������
	ON_XT_COMMAND(CMD_GET_FRIEND_BASICINFO_ACK,OnBasicInfoAck)
	// ���յ�p2p����
	ON_XT_COMMAND(CMD_P2P_TEST_REQ,OnRecvP2pTest)
	// ���ܵ����ص�p2p����Ӧ��
	ON_XT_COMMAND(CMD_P2P_TEST_ACK,OnRecvP2pTestAck)
	// ����Ҫ��ɾ���Լ�
	ON_XT_COMMAND(XT_SERVER_TRANSMIT::TYPE_FRIEND_DEL,OnDelFriendReq)
	// �����ļ�֪ͨ
	ON_XT_COMMAND(CMD_SEND_FILE_REQ,OnRecvSendFileReq)
	// web֪ͨ
	ON_XT_COMMAND(CMD_WEB_IND,OnWebInd)
	// ���º�����Ϣ
	ON_XT_COMMAND(CMD_FRIEND_INFO_IND, OnFriendInfoInd);
END_XT_MESSAGE_MAP()

void CFloorPageFriend::OnGroupChoose(UINT nID)
{
	int nIdx = nID - ID_GROUP_CHOOSE_BEG;
	if(nIdx > g_GroupInfo.count)
		return;
	PostMessage( WM_IMFRIEND_LIST_NOTIFY,NM_FL_DRAGDROP,MAKELPARAM(nIdx,0));
}

void CFloorPageFriend::OnRemoveBlackList()
{
	//int nIdx = GROUPID_BLACKLIST;
	int nIdx = g_GroupInfo.count - 1;
	if(nIdx > g_GroupInfo.count)
		return;
	PostMessage( WM_IMFRIEND_LIST_NOTIFY,NM_FL_DRAGDROP,MAKELPARAM(nIdx,0));
}

void CFloorPageFriend::OnAddBlackList()
{
	CString str;
	FRIEND_INFO* pFriend = NULL;
	//CWnd* pStatic;
	FL_ITEM_POS pos;

	pos = m_wndGroupList.GetCurSel();


	pFriend = (FRIEND_INFO *)m_wndGroupList.GetItemData(pos.nGroupID,pos.nItem);
	if(pFriend->group_index == GROUPID_STRANGER)
	{
		str.Format("����ϵ����İ������,��Ϊ����,����İ������ɾ��");
	}
	else if (pFriend->group_index == GROUPID_BLACKLIST)
	{
		str.Format("����ϵ���ں�������,��Ϊ����,���Ӻ�������ɾ��");
	}
	if(IDYES==AfxMessageBox(str,MB_YESNO|MB_ICONWARNING))
	{
		CAddBlackDlg *pDlg = new CAddBlackDlg(pFriend,this);
		pDlg->Create(CAddBlackDlg::IDD,GetDesktopWindow());
		pDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		;
	}
}

void CFloorPageFriend::OnDelFriendReq(const char * data,WORD wRet)
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

void CFloorPageFriend::DoDataExchange(CDataExchange* pDX)
{
	__super::DoDataExchange(pDX);
	//	DDX_Control(pDX,IDC_STATIC_NICKNAME,m_btnName);
}


BOOL CFloorPageFriend::OnInitDialog()
{
	__super::OnInitDialog();
	// �ж��ϴ������Ƿ���ʾ���к���
	m_bShowAllFriend = g_LocalData.IsShowAllFriend();

	CRect rc(0,0,40,40);
	m_wndGroupList.Create(this,rc,WS_VISIBLE|WS_CHILDWINDOW|WS_CLIPCHILDREN|WS_TABSTOP);
	m_wndGroupList.SetDlgCtrlID(IDC_XT_FRIENDTREE);

	//m_btnSearch.SetBitmap(IDB_BTN_SEARCH,IDB_BTN_SEARCH);
	//m_btnCluster.SetBitmap(IDB_BTN_SEARCH,IDB_BTN_SEARCH);
	g_pProtocol->RegistCmd(CMD_USERINFO_MOD_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_USERAUTH_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_FRIENDGROUP_MOD_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SERVERMSG_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_P2PMSG_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_P2PMSG_RECV,m_hWnd);
	g_pProtocol->RegistCmd(CMD_FRIEND_STATUS_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_FRIENDLIST_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_DIRECTORY_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GROUPINFO_GET_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_STATUS_GET_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_STATUS_MOD_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CLUSTERMSG_RECV,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GROUPINFO_MOD_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_MOD_FCNAME_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_FRIEND_BASICINFO_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_P2P_TEST_REQ,m_hWnd);
	g_pProtocol->RegistCmd(CMD_P2P_TEST_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_USERAUTH_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SEND_FILE_REQ,m_hWnd);
	g_pProtocol->RegistCmd(CMD_WEB_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_FRIEND_INFO_IND,m_hWnd);
	g_pProtocol->RegistCmd(XT_SERVER_TRANSMIT::TYPE_FRIEND_DEL,m_hWnd);

	g_FloorPageFriend = this;
	AttachHMGScrollbar(m_wndGroupList.m_hWnd);
	m_wndGroupList.SetFocus();

	g_friendFloor = GetSafeHwnd();
	return TRUE;
}

#if 0
BOOL CFloorPageFriend::Create(CWnd *pParentWnd, CRect rect)
{
	__super::Create(CFloorPageFriend::IDD,pParentWnd);

	MoveWindow(&rect);
	ShowWindow(SW_SHOW);

	return TRUE;
}
#endif
//OnWM_DESTROY
void CFloorPageFriend::OnDestroy() 
{
	KillTimer(TIMER_BEAT);
	m_wndGroupList.DeleteAllItems();
	g_pProtocol->UnRegistCmd(m_hWnd);

	__super::OnDestroy();
}

LRESULT CFloorPageFriend::OnWebDataNotify(WPARAM wParam, LPARAM lParam)
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
LRESULT CFloorPageFriend::OnGroupListNotify(WPARAM wParam, LPARAM lParam)
{
	if(wParam == NM_FL_ITEM_OVER)		// ��ͣ��Ϣ��������
	{
		//KillTimer(TIMER_USER_CARD);
		m_hotItem.pos  = *(FL_ITEM_POS *)lParam;

		if ( m_hotItem.pos.nGroupID!=-1 && m_hotItem.pos.nItem!=-1 )
		{
			m_hotItem.rect = m_wndGroupList.GetItemIconRect(m_hotItem.pos.nGroupID,m_hotItem.pos.nItem);
			m_wndGroupList.ClientToScreen(&m_hotItem.rect);
			CRect rcItem = m_hotItem.rect;
			FRIEND_INFO * pFriend = (FRIEND_INFO *) m_wndGroupList.GetItemData(m_hotItem.pos.nGroupID,m_hotItem.pos.nItem);

			if ( pFriend )
			{
				if ( pFriend->pUserWebArticleInfo==NULL )
				{
					pFriend->pUserWebArticleInfo = new TUserWebArticleInfo(pFriend->GetId());
					g_spaceData.LoadUserWebArticles(pFriend->GetId(),*pFriend->pUserWebArticleInfo);
				}

				if ( pFriend->pUserWebPictureInfo==NULL )
				{
					pFriend->pUserWebPictureInfo = new TUserWebPictureInfo(pFriend->GetId());
					g_spaceData.LoadUserWebPictures(pFriend->GetId(),*pFriend->pUserWebPictureInfo);
				}

				g_webDataProcess.GetWebPictureList(IntToStr(pFriend->GetId()),pFriend->pUserWebPictureInfo->szLastUpdateDate,"",this);
				g_webDataProcess.GetWebArticleList(IntToStr(pFriend->GetId()),pFriend->pUserWebArticleInfo->szLastUpdateDate,"",this);

				CRect rcFriendPage;
				GetWindowRect(&rcFriendPage);

				int nLeft = rcFriendPage.left-240;

				if ( nLeft<0 )
				{							
					nLeft = rcFriendPage.right+10;
				}

				CUserCardDlg::Show(pFriend,nLeft,rcItem.top,m_hotItem.rect);
			}	
			//SetTimer(TIMER_USER_CARD,100,NULL);
		}
		return 0;
	}

	FL_ITEM_POS pos=m_wndGroupList.GetCurSel();
	int nGroup = pos.nGroupID;
	if(nGroup == -1)//�������Ժ����б����Ϣ������
		return 0;
	int nItem  = pos.nItem;


	FRIEND_INFO * pFriend = NULL;
	
	if ( nGroup>-1 && nItem>-1 )
		pFriend=(FRIEND_INFO *)m_wndGroupList.GetItemData(pos.nGroupID,pos.nItem);

	switch ( wParam )
	{
	case NM_FL_WEBINFO_CLICK:
		{
			FL_ITEM_POS *pPos = (FL_ITEM_POS *)lParam;
			pFriend = (FRIEND_INFO *)m_wndGroupList.GetItemData(pPos->nGroupID,pPos->nItem);
			
			bool bShowPicture = false;
			bool bShowArticle = false;

			if ( pFriend->webInfo & 0x02 )
			{
				bShowPicture=true;
				pFriend->webInfo &= ( ~0x02 );
			}
			else
			if ( pFriend->webInfo & 0x01 )
			{
				bShowArticle=true;
				pFriend->webInfo &= ( ~0x01 );
			}
		

			if ( bShowArticle && (g_pWebConfig != NULL) )
			{
				MiniOpenUrl((LPCSTR)g_pWebConfig->GetWebPageUrl(USER_ARTICLE,StrToWideStr(pFriend->GetUserName().c_str())));

				::RegUserOperation(0x0020);
			}

			if ( bShowPicture && (g_pWebConfig != NULL) )
			{
				MiniOpenUrl((LPCSTR)g_pWebConfig->GetWebPageUrl(USER_PHOTO,StrToWideStr(pFriend->GetUserName().c_str())));

				::RegUserOperation(0x0021);
			}
		}
		break;

	case NM_FL_RBUTTONUP_M:
		OnNCFlRbum();
		break;
	case NM_FL_LBUTTONDOWN:
		if ( pFriend &&  pFriend->status!=XTREAM_OFFLINE )
		{
			CString strFile = g_webUserHeadIconProcess.GetFaceFile(pFriend);
			if ( strFile=="" ||  FileExist(strFile)==FALSE )
				g_webUserHeadIconProcess.GetUserHeadIcon(pFriend,this);

			//TRACE("NM_FL_LBUTTONDOWN\n");
		}
		break;
	// �Ҽ�˫��
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
			FRIEND_INFO *pFriend = (FRIEND_INFO *)m_wndGroupList.GetItemData(nGroup,nItem);
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
					if ( IDYES==MessageBox(str,IM_CAPTION,MB_YESNO|MB_ICONWARNING) )
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
					if ( IDYES==MessageBox(str,IM_CAPTION,MB_YESNO|MB_ICONWARNING) )
					{
						// ֱ��ɾ���ú���
						pFriend->group_index = nDestGroupId;
						pFriend->bInviting = false;
						m_wndGroupList.MoveItem(nGroup,nItem,nDestGroupId);

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
					m_wndGroupList.MoveItem(nGroup,nItem,nDestGroupId);
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
				req.action = GROUP_MODIFY;
				req.uid    = g_ImUserInfo.GetId();
				req.fid    = pFriend->GetId();
				req.group_id = nDestGroupId;
				g_pProtocol->SendReq(req);
				//m_wndGroupList.MoveItem(nGroup,nItem,nGroupDest);
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
				m_wndGroupList.MoveItem(nGroup,nItem,nDestGroupId);
				SortGroupList(nDestGroupId);

				m_wndGroupList.SetCurSel(pFriend);
				
			}

			UpdateFriendCnt();
		}
		break;
	case NM_FL_GROUPNAME://�������޸�
		{
			int nGroupId = (m_wndGroupList.GetGroupData(lParam))->dwData;
			CString strGroupName = m_wndGroupList.GetGroupName(lParam);
			
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
					
					CString strGroup = m_wndGroupList.GetGroupName(nGroup);
					if(strGroup != "İ����" && strGroup!= "������")
					{
						if(pFriend->GetId() != g_ImUserInfo.GetId())
						{
							strItem.Format("���ͼ�ʱ��Ϣ");
							popmenu->AppendMenu(MF_STRING,ID_SENDMSG,strItem);
							popmenu->AppendMenu(MF_SEPARATOR,0,"");
						}

						strItem.Format("�޸ı�ע��");
						popmenu->AppendMenu(MF_STRING,ID_MODF_COMMNAME,strItem);
						
						if(pFriend->GetId() != g_ImUserInfo.GetId())
						{
							popmenu->AppendMenu(MF_SEPARATOR,0,"");
						}

						//��ţ����֯�ܹ��еĲ������ƶ�
						if(nGroup < FEINIU_GROUP_ID && nGroup > -1)
						{
							strItem.Format("ɾ����ϵ��");
							popmenu->AppendMenu(MF_STRING,ID_DELFRIEND,strItem);
							popmenu->AppendMenu(MF_SEPARATOR,0,"");
							
							CMenu menuGroup;
							menuGroup.CreatePopupMenu();
							//��ţ����֯�ܹ��еĲ������ƶ�
							for(int i=0; i<g_GroupInfo.count;i++)//////�޸��ж��߼�����֯�ܹ�������䶯 2014-11-12
							{
								if(g_GroupInfo.group[i].group_index < FEINIU_GROUP_ID)
								{
									//////İ���˺ͺ�����ȥ��
									////if(g_GroupInfo.group[i].group_index!=1&&g_GroupInfo.group[i].group_index!=2)
									{
				                       menuGroup.AppendMenu(MF_STRING,ID_GROUP_CHOOSE_BEG + i,g_GroupInfo.group[i].name);
									}
								}
							}
							popmenu->AppendMenu(MF_POPUP, (UINT)menuGroup.m_hMenu, "�Ѻ����ƶ���...");
							popmenu->AppendMenu(MF_SEPARATOR,0,"");

								
							strItem.Format("�ƶ���������");
							popmenu->AppendMenu(MF_STRING,ID_REMOVE_BLACKLIST,strItem);
							popmenu->AppendMenu(MF_SEPARATOR,0,"");
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
						popmenu->AppendMenu(MF_SEPARATOR,0,"");

						popmenu->AppendMenu(MF_STRING,ID_ADD_BLACKLIST,"��Ϊ����");
						popmenu->AppendMenu(MF_SEPARATOR,0,"");

						popmenu->AppendMenu(MF_STRING,ID_FRIENDINFO,"�鿴����");
					}
				}
				else//��ڵ�
				{
					if ( g_ImUserInfo.status!=XTREAM_OFFLINE )
					{
						CString strGroup = m_wndGroupList.GetGroupName(nGroup);

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
									popmenu->AppendMenu(MF_SEPARATOR, 0, "");
									popmenu->AppendMenu(MF_STRING, ID_ADDGROUP, "�����");
									popmenu->AppendMenu(MF_SEPARATOR, 0, "");
									popmenu->AppendMenu(MF_STRING, ID_ADD_CGROUP, "�������");
									popmenu->AppendMenu(MF_SEPARATOR, 0, "");
									popmenu->AppendMenu(MF_STRING, ID_ADD_FRIEND, "��Ӻ���");

									//�ҵĺ��Ѳ�����༭
									if (nGroup != 0)
									{
										popmenu->AppendMenu(MF_SEPARATOR, 0, "");
										popmenu->AppendMenu(MF_STRING, ID_RENAME_GROUP, "��������");
										popmenu->AppendMenu(MF_SEPARATOR, 0, "");
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
				CFlatPopupMenu flatmenu;
				flatmenu.Create(AfxGetInstanceHandle());
				flatmenu.AppendMenu(popmenu);
				UINT id = flatmenu.Track(point.x, point.y, GetSafeHwnd(), true);
				// ��Ӧѡ��˵�����Ϣ��Ӧ
				DWORD wP = MAKEWPARAM(id, 0);
				SendMessage(WM_COMMAND, wP, 0);
			}			
		}
		break;
		/*case NM_FL_ITEM_OVER:
		{
		KillTimer(TIMER_USER_CARD);
		m_hotItem.pos  = *(FL_ITEM_POS *)lParam;

		if ( m_hotItem.pos.nGroupID!=-1 && m_hotItem.pos.nItem!=-1 )
		{
		m_hotItem.rect = m_wndGroupList.GetItemRect(m_hotItem.pos.nGroupID,m_hotItem.pos.nItem);
		m_wndGroupList.ClientToScreen(&m_hotItem.rect);

		SetTimer(TIMER_USER_CARD,100,NULL);
		}
		}
		break;*/
	case NM_FL_HEADICON_CLICK:
		{
			FL_ITEM_POS *p = (FL_ITEM_POS *)lParam;
			pFriend = (FRIEND_INFO *) m_wndGroupList.GetItemData(p->nGroupID,p->nItem);
		}
		break;
	case NM_FL_BLOG_CLICK :
		{
			FL_ITEM_POS *p = (FL_ITEM_POS *)lParam;
			pFriend = (FRIEND_INFO *) m_wndGroupList.GetItemData(p->nGroupID,p->nItem);
			ShowBlogSite((char*)pFriend->GetUserName().c_str());
		}
		break;
	case NM_FL_INFO_CLICK:
		{
			if ( pFriend )
				CFriendInfoDlg::Show(pFriend);
		}
		break;
	case NM_FL_ALBUM_CLICK:
		{
			if ( pFriend && (g_pWebConfig != NULL))
			{
				MiniOpenUrl((LPCSTR)g_pWebConfig->GetWebPageUrl(USER_PHOTO,StrToWideStr(pFriend->GetUserName().c_str())));
			//	g_pUserPictures->Show(pFriend->id,(LPCSTR)pFriend->username,(LPCSTR)pFriend->nickname);
			//	CFriendPictureViewDlg::Show(pFriend->id,0);
			}
		}
		break;
	case NM_FL_ARTICLE_CLICK:
		{
			if ( pFriend &&(g_pWebConfig != NULL))
			{
				MiniOpenUrl((LPCSTR)g_pWebConfig->GetWebPageUrl(USER_ARTICLE,StrToWideStr(pFriend->GetUserName().c_str())));
		
				//g_pUserArticles->Show(pFriend->id,(LPCSTR)pFriend->username,(LPCSTR)pFriend->nickname);
			}
		}
		break;
	case NM_FL_VEDIO_CLICK:
		{
			FL_ITEM_POS *pPos = (FL_ITEM_POS *)lParam;
			pFriend = (FRIEND_INFO *)m_wndGroupList.GetItemData(pPos->nGroupID,pPos->nItem);

			if ( pFriend )
			{
				CP2PSendMsgDlg *pDlg = ShowMessageDlg(pFriend);
				pDlg->OnBnClickedBtnVedio();
			}
		}
		break;
	case NM_FL_MOBILE_CLICK:
		{
			FL_ITEM_POS *pPos = (FL_ITEM_POS *)lParam;
			pFriend = (FRIEND_INFO *)m_wndGroupList.GetItemData(pPos->nGroupID,pPos->nItem);

			CSMSDlg::Show(pFriend);
			CUserCardDlg::Close();
			//KillTimer(TIMER_USER_CARD);
		}
		break;
	default:break;
	}

	return 0;
}

void CFloorPageFriend::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	if ( m_wndGroupList.m_hWnd )
	{
		m_wndGroupList.MoveWindow(0,0, cx, cy-0);
	}

	
	// TODO: �ڴ˴������Ϣ����������
}

void CFloorPageFriend::OnSendMsg()
{
	FL_ITEM_POS pos = m_wndGroupList.GetCurSel();

	if ( pos.nItem!=-1 && pos.nGroupID!=-1 )
	{
        FRIEND_INFO *pFriend =
			(FRIEND_INFO *)m_wndGroupList.GetItemData(pos.nGroupID,pos.nItem);

		//�Լ����ܸ��Լ�����Ϣ
		if(pFriend->GetId() == g_ImUserInfo.GetId())
			return;

		CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
		
		if ( pDlg->m_hWnd==NULL )
		{
			pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
			pDlg->NotifyServer_OpenedMsgDlg();
		}

		if ( pDlg->IsWindowVisible()==FALSE )
			pDlg->ShowWindow(SW_SHOW);

		if (pDlg->IsIconic())
			pDlg->ShowWindow(SW_RESTORE);

		pDlg->BringWindowToTop();
	}
}

void CFloorPageFriend::OnFriendInfo()
{
	int nGroup = m_wndGroupList.GetCurSel().nGroupID;
	int nItem  = m_wndGroupList.GetCurSel().nItem;

	if ( nGroup==-1 || nItem==-1 )
	{
		return ;
	}

	FRIEND_INFO *pFriend = (FRIEND_INFO *)m_wndGroupList.GetItemData(nGroup,nItem);
	/*CFriendInfoDlg *pDlg = */CFriendInfoDlg::Show(pFriend);	
	//CFriendInfoDlg *pDlg = CFriendInfoDlg::Show(pFriend);	
}

void CFloorPageFriend::OnFriendBlog()
{
	int nGroup = m_wndGroupList.GetCurSel().nGroupID;
	int nItem  = m_wndGroupList.GetCurSel().nItem;

	if ( nGroup==-1 || nItem==-1 )
	{
		return ;
	}

	RegUserOperation(0x0020);
	
	FRIEND_INFO *pFriend = (FRIEND_INFO *)m_wndGroupList.GetItemData(nGroup,nItem);
	::ShowBlogSite((char*)pFriend->GetUserName().c_str());
}

void CFloorPageFriend::OnFriendArticle()
{
	int nGroup = m_wndGroupList.GetCurSel().nGroupID;
	int nItem  = m_wndGroupList.GetCurSel().nItem;

	if ( nGroup==-1 || nItem==-1 )
	{
		return ;
	}

	RegUserOperation(0x001c);

	FRIEND_INFO *pFriend = (FRIEND_INFO *)m_wndGroupList.GetItemData(nGroup,nItem);
	::ShowArticle((char*)pFriend->GetUserName().c_str());
}

void CFloorPageFriend::OnFriendAlbum()
{
	int nGroup = m_wndGroupList.GetCurSel().nGroupID;
	int nItem  = m_wndGroupList.GetCurSel().nItem;

	if ( nGroup==-1 || nItem==-1 )
	{
		return ;
	}

	RegUserOperation(0x001d);

	FRIEND_INFO *pFriend = (FRIEND_INFO *)m_wndGroupList.GetItemData(nGroup,nItem);
	::ShowAlbum((char*)pFriend->GetUserName().c_str());
}

void CFloorPageFriend::OnDelFriend()
{
	int nGroup = m_wndGroupList.GetCurSel().nGroupID;
	int nItem  = m_wndGroupList.GetCurSel().nItem;

	if ( nGroup==-1 || nItem==-1 )
	{
		return ;
	}

	FRIEND_INFO *pFriend = (FRIEND_INFO *)m_wndGroupList.GetItemData(nGroup,nItem);

	if(pFriend->group_index > FEINIU_GROUP_ID)
	{
		MessageBox(_T("��ţ���еĺ��Ѳ���ɾ����Ŷ~~"));
	}

	
	CString str;
	str.Format("ȷ��Ҫɾ�� %s ��?",UserInfoToStr(pFriend));

	if (MessageBox(str,IM_CAPTION,MB_YESNO|MB_ICONWARNING)!=IDYES)
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

void CFloorPageFriend::OnSearchXtream()
{
	tr1::shared_ptr<CAddFriendOrClusterDlg> pDlg = CAddFriendOrClusterDlg::Show();
	pDlg->SetSearchType(FS_XTREAM);

}

// ��ȡ�µ���ID���Զ������ʱ�õ���������-1��ʾʧ��
int CFloorPageFriend::GetNewGroupID()
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

	return nGroupId;
}

void CFloorPageFriend::CreateGroup(int nParentGroupId, int& nGroupId, CString& strGroupName)
{
	if ( g_GroupInfo.count > 50 )
	{
		ShowMessage("���ֻ�ܽ�50���Զ����飡",MB_OK|MB_ICONINFORMATION);
		return ;
	}

	// ��ȡ����ID
	nGroupId = GetNewGroupID();

	//�����Ƿ�������������	����߼�����bug��
	//֧��ͬ�������֣�����ɶ?
	
	int n = 1;
	for(int i=0; i<g_GroupInfo.count; i++)
	{
		CString str = m_wndGroupList.GetGroupName(g_GroupInfo.group[i].group_index);
		
		if ( strGroupName == str )
		{
			strGroupName.Format("%s%d",str,n++);
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
void CFloorPageFriend::OnAddGroup()
{
	//��ǰѡ����鼰�丸��
	int nSelGroupId = m_wndGroupList.GetCurSel().nGroupID;
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
		m_wndGroupList.InsertGroup(nParentGroupId , nNewGroupId, strNewGroupName);
		m_wndGroupList.SetCurSel(nNewGroupId, -1);
		m_wndGroupList.ShowEditBox(nNewGroupId);

		ImAddGroup(nParentGroupId, nNewGroupId, strNewGroupName);
	}
}



//�������
void CFloorPageFriend::OnAddSubGroup()
{
	//��ǰѡ����鼰�丸��
	int nSelGroupId = m_wndGroupList.GetCurSel().nGroupID;
	int nParentGroupId = nSelGroupId;

	//�½���
	int nNewGroupId = -1;
	CString strNewGroupName = "�½�����";
	CreateGroup(nParentGroupId, nNewGroupId, strNewGroupName);

	if(nNewGroupId != -1)
	{
		m_wndGroupList.InsertGroup(nParentGroupId , nNewGroupId, strNewGroupName);
		//�����
		m_wndGroupList.SetGroupState(nParentGroupId,FLS_EXPAND);
		m_wndGroupList.SetCurSel(nNewGroupId, -1);
		m_wndGroupList.ShowEditBox(nNewGroupId);

		ImAddGroup(nParentGroupId, nNewGroupId, strNewGroupName);
	}

	
}


void CFloorPageFriend::OnRenameGroup()
{
	int nGroup = m_wndGroupList.GetCurSel().nGroupID;
	m_wndGroupList.ShowEditBox(nGroup);
}

//����Ա����Ϣ
void CFloorPageFriend::OnSendGroupMessage()
{
	OnCreateTempCluster();
	return;

	/*
	FL_ITEM_POS pos = m_wndGroupList.GetCurSel();

	if ( pos.nItem!=-1 && pos.nGroup!=-1 )
	{
        FRIEND_INFO *pFriend =
			(FRIEND_INFO *)m_wndGroupList.GetItemData(pos.nGroup,pos.nItem);

		CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
		
		if ( pDlg->m_hWnd==NULL )
			pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());

		if ( pDlg->IsWindowVisible()==FALSE )
			pDlg->ShowWindow(SW_SHOW);

		if (pDlg->IsIconic())
				pDlg->ShowWindow(SW_RESTORE);

		pDlg->BringWindowToTop();
	}
	
	CClusterMemberSelectDlg dlg(&m_clusterInfo,this);
	dlg.SetP2pInviteFriend(&m_FriendInfo);

	if ( IDOK==dlg.DoModal() )
	{
		dlg.GetClusterInfo(m_clusterInfo);

		if ( m_pmP2PMode!=pmMultiTalk )
		{
			::SendMessage(g_pMessageHandleWnd->m_hWnd,WM_P2P_TO_GROUP,0,(LPARAM)&m_FriendInfo);
		}

		m_pmP2PMode = pmMultiTalk;
		UpdateP2PPanel();

		m_wndUserList.SetClusterInfo(&m_clusterInfo);
		m_wndUserList.UpdateClusterList();

		SetBtnFolderBitmap();

		ShowDlgTitle();

		m_bClusterInit = true;
	}
	*/
}



void CFloorPageFriend::OnDelGroup()
{
	int group_id = m_wndGroupList.GetCurSel().nGroupID;
	if( -1 == group_id )
	{
		return;
	}

	FL_GROUP_INFO* pGroup = m_wndGroupList.GetGroupData(group_id);

	if(pGroup == NULL)
		return;

	if(group_id > FEINIU_GROUP_ID)
	{
		MessageBox(_T("��ţ���еķ��鲻��ɾ����Ŷ~~"));
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
		if ( IDYES == MessageBox(str, IM_CAPTION, MB_YESNO|MB_ICONWARNING) )
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
			if(bDelError || iParentGroupId == -2 || m_wndGroupList.DelGroup(iParentGroupId, group_id) == -1)
			{
				ShowMessage("ɾ����ʧ��!~");
				return;
			}
						
			//g_GroupInfo�汾��
			g_LocalData.SaveGroupInfo(g_GroupInfo);
		}
	}
}

void CFloorPageFriend::OnMsgHistory()
{
	FL_ITEM_POS pos = m_wndGroupList.GetCurSel();

	if ( pos.nGroupID!=-1 && pos.nItem!=-1 )
	{
		FRIEND_INFO *pFriend =(FRIEND_INFO *)m_wndGroupList.GetItemData(pos.nGroupID,pos.nItem);
		CMsgDlg * pMsg = CMsgDlg::GetInstance();

		pMsg->ShowWindow(SW_SHOW);
		pMsg->BringWindowToTop();
		pMsg->SetDefaultFriend(pFriend->GetId());
	}
}

//�ӱ����ļ��л�ȡ������Ϣ
void CFloorPageFriend::LoadLocalFriends()
{
	::ClearFriendList();

	memset(&g_GroupInfo,0,sizeof(g_GroupInfo));
	
	g_GroupInfo.dwUserId = g_ImUserInfo.GetId();

	// ���Ұ�ġ��ҵ��Ŷӡ����������ǰ��������ͨ���鷽ʽ��ӵģ�����Ĭ�ϵģ�������ע�͵���
	// ����Ժ��ΪĬ�Ϸ�������ȡ��ע��
//#ifdef _BUSINESS_VESION
//	g_GroupInfo.count = 4;
//	g_GroupInfo.group[0].group_index = 0;
//	g_GroupInfo.group[0].group_father = -1;
//	g_GroupInfo.group[0].group_state = FLS_CLOSE;
//	strcpy(g_GroupInfo.group[0].name, "�ҵ��Ŷ�");
//
//	g_GroupInfo.group[1].group_index = 1;
//	g_GroupInfo.group[1].group_father = -1;
//	g_GroupInfo.group[1].group_state = FLS_CLOSE;
//	strcpy(g_GroupInfo.group[1].name, "�ҵĺ���");
//
//	g_GroupInfo.group[2].group_index = 2;
//	g_GroupInfo.group[2].group_father = -1;
//	g_GroupInfo.group[2].group_state = FLS_CLOSE;
//	strcpy(g_GroupInfo.group[2].name, "İ����");
//
//	g_GroupInfo.group[3].group_index = 3;
//	g_GroupInfo.group[3].group_father = -1;
//	g_GroupInfo.group[3].group_state = FLS_CLOSE;
//	strcpy(g_GroupInfo.group[3].name, "������");
//#else
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
//#endif

	g_LocalData.GetGroupInfo(g_GroupInfo);
	g_LocalData.GetFriendList(g_lsFriend);	
}

void CFloorPageFriend::ChangeUserStatus(uint8 status)
{
	if ( status==g_ImUserInfo.status)
	{
		return ;
	}
	else if (status==XTREAM_OFFLINE)
	{	
		KillTimer(TIMER_BEAT);	
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
		XT_STATUS_MOD_REQ req;

		req.id     = g_ImUserInfo.GetId();
		req.status = status;

		g_pProtocol->SendReq(req);
	}
}

//����״̬����֪ͨ
void CFloorPageFriend::OnFriendStatusChange(const char *data,WORD wRet)
{
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
			if ( ind->status == XTREAM_ONLINE )
			{
#ifdef _VER_INTERNAL_	// �ڲ���
#else
				PlayTone(EVOICE_FRIEND_ONLINE);
				ShowUserHintDlg(pFriend,UserInfoToStr(pFriend), "������", m_hWnd);
#endif
				NotifyVideoEquipment(pFriend);				
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
				m_wndGroupList.UpdateItemImage(pFriend);
				
				m_wndGroupList.SetInvalidateLock(true);

				/*
				if ( ind->status==XTREAM_OFFLINE )
					m_wndGroupList.DelItemImage(nGroup,nItem);
				*/
				UpdateFriend(pFriend);


				m_wndGroupList.SetInvalidateLock(false);
			}

			RemoveHideTick(pFriend->GetId());
		}
	}
}

void CFloorPageFriend::OnStatusModAck(const char *data,WORD wRet)
{
	LPXT_STATUS_MOD_ACK resp = (LPXT_STATUS_MOD_ACK)data;
	
	if ( wRet==0 )
	{
		g_ImUserInfo.status = resp->status;
		
		FRIEND_INFO *pFriend = FindFriend(g_ImUserInfo.GetId());
		if(pFriend != NULL)
		{
			pFriend->status = resp->status;
			m_wndGroupList.SetInvalidateLock(true);
			UpdateFriend(pFriend);
			m_wndGroupList.SetInvalidateLock(false);
		}
		

		/*if ( resp->status==XTREAM_OFFLINE )
		{
		}*/
	}	
}

void CFloorPageFriend::OnFriendGroupModAck(const char *data,WORD wRet)
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
				sAllowIni.Format("%sdata\\%s\\allow.ini",(LPCTSTR)g_LocalData.GetPath(),g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());
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
				if(pFriend == NULL)
				{
					
					pFriend = new FRIEND_INFO();
					pFriend->SetId(ack->fid);
					pFriend->status = XTREAM_OFFLINE;
					pFriend->group_index = 0;

					XT_USERINFO_GET_REQ userinfo_req;
					userinfo_req.fid = ack->fid;
					g_pProtocol->SendReq(userinfo_req);
					/*
					FRIEND_INFO *pOldFriend = FindFriend(pFriend->id);
					if ( pOldFriend && pOldFriend->group_index!=pFriend->group_index )
					{
						DelFriend(pOldFriend->id);					
					}
					*/
					AddFriend(pFriend,true);
					//SelItem(pFriend);
					ImGetUserStatus(pFriend->GetId());
					if ( m_mapNewFriend.find(pFriend->GetId())==m_mapNewFriend.end() )
						m_mapNewFriend.insert(map<uint32,FRIEND_INFO *>::value_type(pFriend->GetId(),pFriend));	

					//ѡ�����
					{

						CAddBlackDlg* m_AddBlackDlg = new CAddBlackDlg(pFriend,this);
						m_AddBlackDlg->Create(CAddBlackDlg::IDD,GetDesktopWindow());
						m_AddBlackDlg->ShowWindow(SW_SHOW);
/*
							m_pSecifySendDlg = new CSpecifySendDlg();
	m_pSecifySendDlg->Create(this);
	m_pSecifySendDlg->CenterWindow(GetDesktopWindow());
	*/
					}

				}
				else
				{
					pFriend->group_index = ack->group_id;
					pFriend->bInviting = false;
					UpdateFriendList();
				}
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

void CFloorPageFriend::OnGroupModAck(const char *data,WORD wRet)
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

void CFloorPageFriend::OnUserAuthAck(const char *data,WORD wRet)
{
	if(wRet != 0)
	{
		ShowMessage(g_pProtocol->GetLastError());
	}
}

//������֤����\ͨ��\�ܾ�
void CFloorPageFriend::OnUserAuthInd(const char *data,WORD wRet)
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
			sAllowIni.Format("%sdata\\%s\\allow.ini",(LPCTSTR)g_LocalData.GetPath(),g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());
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

void CFloorPageFriend::OnUserInfoGet(const char *data,WORD wRet)
{
	CString sCommName;

	if ( wRet==0 )
	{
		LPXT_USERINFO_GET_ACK ack = (LPXT_USERINFO_GET_ACK)data;

		FRIEND_INFO *pFriend  = NULL;

		if ( ack->GetId()!=g_ImUserInfo.GetId() )
		{
			pFriend = FindFriend(ack->GetId());			
	
			if ( pFriend )
			{	
				FL_ITEM_POS posSel = m_wndGroupList.GetCurSel();

				//m_wndGroupList.DelItemImage(nGroup,nItem);
				// ע�ⱸע���Ʋ�Ҫ������
				sCommName=pFriend->comment_name;
				//memcpy(pFriend,(XT_USER_INFO *)ack,sizeof(XT_USER_INFO));
				UpdateUserFullInfo(pFriend,ack);
				strncpy(pFriend->comment_name,sCommName,sizeof(pFriend->comment_name)-1);

				UpdateUserBitmap(pFriend);
				m_wndGroupList.UpdateItemImage(pFriend);
				m_wndGroupList.SetInvalidateLock(true);

				pFriend->bRefreshed = true;
				UpdateFriend(pFriend);

				//if ( pFriend->status!=XTREAM_OFFLINE )
				//{
					// ���ظ��û����Զ���ͷ��ͼƬ
					g_webUserHeadIconProcess.GetUserHeadIcon(ack,this);
				//}
				if(posSel.nGroupID >= 0 && posSel.nItem >= 0)
					m_wndGroupList.SetCurSel(posSel.nGroupID,posSel.nItem);

				m_wndGroupList.SetInvalidateLock(false);
			}

		}		
	}
}

void CFloorPageFriend::OnGroupInfoGet(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		CRecvPacket rp((char *)data,MAX_RECV_BUFFERS);
		uint8 group_count;
		uint8 group_id,group_pid;
		rp>>group_count;

		GROUP_INFO tmp_GroupInfo = g_GroupInfo;

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
	if (AddGroup_MyTeam())
	{	// �ɹ���ӣ��ٴθ��·����б�
		Sleep(2000);		// ��ӳɹ����һ���ٸ����б�������Ӻ����ظ�����
		ImGetGroupInfo();
	}
#endif
}


void CFloorPageFriend::OnFriendListAck(const char *data,WORD wRet)
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

				itoa(f.friendId,pFriend->nickname,10);
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
			ImGetFriendBasicInfo(addiFriends);
			
			m_wndGroupList.RemoveRefreshFlag();
			UpdateFriendList();

			m_mapWebInfo.clear();

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

			CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
			if ( pDlg->m_hWnd==NULL )
			{
				pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
				pDlg->NotifyServer_OpenedMsgDlg();
			}
			if ( pDlg->IsWindowVisible()==FALSE )
				pDlg->ShowWindow(SW_SHOW);
			if (pDlg->IsIconic())
					pDlg->ShowWindow(SW_RESTORE);
			pDlg->BringWindowToTop();

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

		m_wndGroupList.RemoveRefreshFlag();

	//	theApp.m_pMainWnd->PostMessage(WM_IM_OK,1,(LPARAM)error);
	}
}

void CFloorPageFriend::StartHeartBeart()
{
	SetTimer(TIMER_BEAT,1000,NULL);
	m_dwPrevBeatTick = GetTickCount();
	ImHeartBeat();
}

void CFloorPageFriend::InitializeIm()
{
	// �����ļ��м��غ����б�
	LoadLocalFriends();
	UpdateFriendList();

	// ���ѷ�������
	ImGetGroupInfo();
	// ���ͻ�ȡ�����б������
	ImGetFriendList(0,FLS_TOTAL);
}

void CFloorPageFriend::SaveLocalFriends()
{
	if ( g_ImUserInfo.GetId() != 0 )
	{
		// groupList��State >> groupInfo
		for(int i=0; i<g_GroupInfo.count; i++ )
		{
			int nSubGroupId = g_GroupInfo.group[i].group_index;
			int nGroupState =  m_wndGroupList.GetGroupState(nSubGroupId);
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

void CFloorPageFriend::OnHeartBeat(const char * data,WORD wRet)
{
	LPXT_DIRECTORY_ACK resp = (LPXT_DIRECTORY_ACK)data;
	if(wRet == 0)
	{
		m_dwPrevBeatTick = GetTickCount();
	}

}

bool CFloorPageFriend::AddFriend(FRIEND_INFO *pFriend,bool bSel)
{
	if (FindFriend(pFriend->GetId()))
	{
		return false;
	}

	g_lsFriend.push_back(pFriend);
	
	m_wndGroupList.AddItem(pFriend->group_index, UserInfoToStr(pFriend),(DWORD)pFriend );
	m_wndGroupList.SetItemTextColor(pFriend,StatusToColor(pFriend->status));
	if(bSel)
		m_wndGroupList.SetCurSel(pFriend);

	return true;
}

void CFloorPageFriend::DelFriend(FRIEND_INFO *pFriend)
{

	g_pMessageHandleWnd->CloseP2PSendMsgDlg(pFriend->GetId());
	
	//m_wndGroupList��ɾ��
	m_wndGroupList.DeleteItem(pFriend);

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
	UpdateFriendCnt();

	//���º�������
	XT_USERINFO_GET_REQ req;
	req.fid = g_ImUserInfo.GetId();
	g_pProtocol->SendReq(req);
}


void CFloorPageFriend::DelFriend(uint32 nFriendId)
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


void CFloorPageFriend::ImGetUserStatus(uint32 uid)
{
	XT_STATUS_GET_REQ req;
	req.id = g_ImUserInfo.GetId();
	req.fid = uid;

	g_pProtocol->SendReq(req);
}

void CFloorPageFriend::ImSetUserGroup(uint32 fid, int nGroupId, uint8 action)
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

void CFloorPageFriend::ImAddGroup(int nParentGroupId,int nGroupId,CString strGroupName)
{
	XT_GROUPINFO_MOD_REQ req;

	req.action   = GROUP_ADD;
	req.group_pid = (nParentGroupId==-1 ? 0xFF : nParentGroupId);
	req.group_id = nGroupId;
	req.uid      = g_ImUserInfo.GetId();
	sprintf(req.name,strGroupName);

	g_pProtocol->SendReq(req);
}

void CFloorPageFriend::ImRenameGroup( int nGroupId, CString strGroupName)
{
	XT_GROUPINFO_MOD_REQ req;
	req.action = GROUP_MODIFY;
	req.group_pid = -2;//��̨����ʹ��
	req.group_id = nGroupId;
	sprintf(req.name,strGroupName);
	req.uid   = g_ImUserInfo.GetId();

	g_pProtocol->SendReq(req);
}

void CFloorPageFriend::ImDelGroup( int nGroupId)
{
	XT_GROUPINFO_MOD_REQ req;

	req.action   = GROUP_DEL;
	req.group_pid = -2;//��̨����ʹ��
	req.group_id = nGroupId;
	sprintf(req.name," ");
	req.uid      = g_ImUserInfo.GetId();

	g_pProtocol->SendReq(req);	
}

void CFloorPageFriend::IMSetFoucs(uint32 fid,uint8 focus)
{
	XT_MODIFY_FRIENDFLAG_REQ req;
	req.friendid = fid;
	req.flag     = focus;
	g_pProtocol->SendReq(req);
}

void CFloorPageFriend::ImGetGroupInfo()
{
	XT_GROUPINFO_GET_REQ req;
	req.group_count = 0;
	req.id = g_ImUserInfo.GetId();
	req.merchant_id = g_ImUserInfo.merchant_id;

	g_pProtocol->SendReq(req);
}

void CFloorPageFriend::ImSaveUserOperations()
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

void CFloorPageFriend::ImHeartBeat()
{
	XT_DIRECTORY_REQ req;
	req.id = g_ImUserInfo.GetId();
	req.local_port = g_pProtocol->GetUdpCommPort();
	req.local_ip   = g_pProtocol->GetUdpCommIp();
	// ��ȡ�������������к�
	m_nLastHeartSeq = g_pProtocol->GetCurSendSequnceId();
	g_pProtocol->SendReq(req);
	
}

void CFloorPageFriend::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent) 
	{
	/*case TIMER_USER_CARD:
		{
			KillTimer(TIMER_USER_CARD);

			FL_ITEM_POS p = m_hotItem.pos;

			if ( p.nGroupID!=-1 && p.nItem!=-1 )
			{
				CPoint ptCursor;
				GetCursorPos(&ptCursor);

				CRect rcItem = m_hotItem.rect;

				if ( rcItem.PtInRect(ptCursor) )
				{
					FRIEND_INFO * pFriend = (FRIEND_INFO *) m_wndGroupList.GetItemData(p.nGroupID,p.nItem);

					if ( pFriend )
					{
						if ( pFriend->pUserWebArticleInfo==NULL )
						{
							pFriend->pUserWebArticleInfo = new TUserWebArticleInfo(pFriend->id);
							g_spaceData.LoadUserWebArticles(pFriend->id,*pFriend->pUserWebArticleInfo);
						}

						if ( pFriend->pUserWebPictureInfo==NULL )
						{
							pFriend->pUserWebPictureInfo = new TUserWebPictureInfo(pFriend->id);
							g_spaceData.LoadUserWebPictures(pFriend->id,*pFriend->pUserWebPictureInfo);
						}

						g_webDataProcess.GetWebPictureList(IntToStr(pFriend->id),pFriend->pUserWebPictureInfo->szLastUpdateDate,"",this);
						g_webDataProcess.GetWebArticleList(IntToStr(pFriend->id),pFriend->pUserWebArticleInfo->szLastUpdateDate,"",this);

						int nLeft = rcItem.left-330;

						if ( nLeft<0 )
						{							
							nLeft = rcItem.left+150;
						}
		
						CUserCardDlg::Show(pFriend,nLeft,rcItem.top,rcItem);
					}		
				}
			}
		}
		break;*/
	case TIMER_BEAT:
		{
			static int timer=0;
			static int operation=0;

			if ( ++timer>30 )
			{
				ImHeartBeat();
				timer=0;
			}

			if ( ++operation>600 )//
			{
				ImSaveUserOperations();
				operation = 0;
			}

			CheckHideTick();
			
			if(m_dwPrevBeatTick < CIMProtocol::s_nHeartBeat)
			{
				m_dwPrevBeatTick = CIMProtocol::s_nHeartBeat;
			}

			if ( !InternetGetConnectedState(NULL, NULL) || (GetTickCount() - m_dwPrevBeatTick ) > 180000)	
			{
				TRACE("�������ʧȥ��ϵ");
				//������ʱ
				KillTimer(TIMER_BEAT);
				SetTimer(TIMER_RELOGIN,1000,NULL);
				m_bDisconnectFlag = true;
				m_nPreOnlineStatus = g_ImUserInfo.status;
				theApp.m_pMainWnd->PostMessage(WM_HEART_TIMEOUT,0,0);
			}
			if( !InternetGetConnectedState(NULL, NULL) )
			{	// �������Ͽ��ˣ���3s���һ�����磬������������ʱ�����µ�¼
				SetTimer(TIMER_CHECK_NETWORD,3000,NULL);				
			}

		break;
		}
	case TIMER_RELOGIN:
		{
			static int timer = 0;
			static int count = 0;

			if ( ++timer > (60+count*90) )
			{
				if ( g_ImUserInfo.status==XTREAM_OFFLINE )
				{
					ChangeUserStatus(XTREAM_ONLINE);
					timer=0;
					count++;
				}
				else
				{
					KillTimer(TIMER_RELOGIN);
					count=0;
					timer=0;
				}
			}
		}
		break;
	case TIMER_CHECK_NETWORD:
		{	// ������������ʱ����������
			if ( g_ImUserInfo.status==XTREAM_OFFLINE && InternetGetConnectedState(NULL, NULL) )
			{
				ChangeUserStatus(XTREAM_ONLINE);
				KillTimer(TIMER_CHECK_NETWORD);
			}
		}
		break;
	case TIMER_SORTLIST:
		{
			KillTimer(TIMER_SORTLIST);
			UpdateFriendList();
		}
		break;
	case TIMER_INVALIDATELIST:
		{
			KillTimer(TIMER_INVALIDATELIST);
			m_wndGroupList.InvalidateList();
		}
	default:
		break;
	}

	__super::OnTimer(nIDEvent);
}

void CFloorPageFriend::SetUserOffLineFlag()
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

void CFloorPageFriend::ImGetFriendList(int start_pos, int type)
{
	XT_FRIENDLIST_REQ req;
	req.merchant_id = g_ImUserInfo.merchant_id;
	req.id			= g_ImUserInfo.GetId();
	req.start_pos	= start_pos;

	// �տ�ʼ��ȡ�����б�
	if ( start_pos==0 )
	{
		m_arrFriend.clear();
		m_vecPos.clear();
	}
	g_pProtocol->SendReq(req);
}

void CFloorPageFriend::SortGroupList(int nGroup)
{
	

	m_wndGroupList.SetInvalidateLock(true);

	m_wndGroupList.SortGroupByState(nGroup);

	/*�����������Ȳ�����
	for ( i=0; i<nItemCount; i++ )
	{
		FRIEND_INFO * f1 =(FRIEND_INFO *)m_wndGroupList.GetItemData(nGroup,i);
		if((DWORD)f1==-1)
			return;
		if ( f1->status!=XTREAM_OFFLINE )
			nOnline++;
	}

	QuickSortGroupList(nGroup,0,nOnline-1);
	QuickSortGroupList(nGroup,nOnline,nItemCount-1);
*/
	
	m_wndGroupList.SetInvalidateLock(false);
	
	m_wndGroupList.Invalidate();
}

void CFloorPageFriend::QuickSortGroupList(int nGroup, int iLo, int iHi)
{
	if ( iLo<iHi )
	{
		char szKey[255];
		strncpy(szKey,((FRIEND_INFO *)m_wndGroupList.GetItemData(nGroup,iLo))->nickname,sizeof(szKey)-1);

		int lo = iLo;
		int hi = iHi;

		while (lo<hi)
		{
			FRIEND_INFO *fhi = (FRIEND_INFO *)m_wndGroupList.GetItemData(nGroup,hi);

			while ( lo<hi && strcmp(fhi->nickname,szKey)>=0 )
			{
				--hi;
				fhi = (FRIEND_INFO *)m_wndGroupList.GetItemData(nGroup,hi);
			}

			m_wndGroupList.SwapItem(nGroup,lo,nGroup,hi);

			FRIEND_INFO *flo = (FRIEND_INFO *)m_wndGroupList.GetItemData(nGroup,lo);

			while ( lo<hi && strcmp(flo->nickname,szKey)<0)
			{
				++lo;
				flo = (FRIEND_INFO *)m_wndGroupList.GetItemData(nGroup,lo);
			}

			m_wndGroupList.SwapItem(nGroup,lo,nGroup,hi);
		}

		QuickSortGroupList(nGroup,iLo,lo-1);
		QuickSortGroupList(nGroup,lo+1,iHi);
	}
}

//������ʾ�����б�
void CFloorPageFriend::UpdateFriendList()
{
	FL_ITEM_POS pos = m_wndGroupList.GetCurSel();

	// groupList��State >> groupInfo
	/*
	for(int i=0; i<g_GroupInfo.count; i++ )
	{
		int nSubGroupId = g_GroupInfo.group[i].group_index;
		int nGroupState =  m_wndGroupList.GetGroupState(nSubGroupId);
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
	*/
	
	//�������group ������Item
	m_wndGroupList.DeleteAllItems();

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
			m_wndGroupList.AddSubGroup(nParentGroupId, nSubGroupId, g_GroupInfo.group[nSubGroupIndex].name);
			//��״̬
			m_wndGroupList.SetGroupState(nSubGroupId,(FLS_STATE)g_GroupInfo.group[nSubGroupIndex].group_state);
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

		if(m_bShowAllFriend == FALSE)
		{
			if(pFriend->status == XTREAM_OFFLINE || XTREAM_HIDE == pFriend->status)
			{
				continue;
			}
		}
	
		// �Ƿ��б�ע����
		CString sName;
		if(pFriend->comment_name[0] == 0)
			sName = pFriend->nickname;
		else
			sName = pFriend->comment_name;
		UpdateUserBitmap(pFriend);
		
		int ret = m_wndGroupList.AddItem(pFriend->group_index, sName, (DWORD)(pFriend));
		//����ʧ�ܣ�����"�ҵĺ���"��
		/*�޲���
		if(ret == -1)
		{
			ret = m_wndGroupList.AddItem2(0, sName, (DWORD)(pFriend));
			ASSERT(ret == 1);
		}
		*/
		//int nItem = m_wndGroupList.AddItem(folder, sName, (DWORD)(pInfo) );
		m_wndGroupList.UpdateItemImage(pFriend);
		m_wndGroupList.SetItemTextColor(pFriend,StatusToColor(pFriend->status));
	}

	


	//���ں�������
	for(int i=0; i<g_GroupInfo.count; i++ )
	{
		int nSubGroupId = g_GroupInfo.group[i].group_index;
		SortGroupList(nSubGroupId);
	}
	//groupInfo >> ״̬>> groupList
	/*
	int k = 0;
	for ( i=0; i<m_wndGroupList.GetGroupCount(); i++ )
	{

		if ( m_wndGroupList.GetItemCount(i)==0 )
		{
			int nGroupIndex = m_wndGroupList.GetGroupData(i);
			int nItemIndex  = g_GroupInfo.FindItemIndex(nGroupIndex);
			if(nItemIndex > 5)
				nItemIndex = nItemIndex;

			if ( nItemIndex!=-1 )
			{
				m_wndGroupList.SetGroupState(i, (FLS_STATE)g_GroupInfo.group[nItemIndex].group_state);
			}
			else
			{
				m_wndGroupList.SetGroupState(i, FLS_CLOSE);
			}
		}
		k++;
		SortGroupList(i);
	}
	*/

	if(pos.nGroupID >= 0 && pos.nItem >= 0)
		m_wndGroupList.SetCurSel(pos.nGroupID,pos.nItem);

	
	UpdateFriendCnt();
	
}

void CFloorPageFriend::SelItem(FRIEND_INFO * pUser)
{
	int nGroup, nItem;
	FindItem(pUser,nGroup,nItem);

	if ( nGroup!=-1 && nItem!=-1 )
	{
		m_wndGroupList.SetCurSel(nGroup,nItem);
	}
}


void CFloorPageFriend::FindItem(FRIEND_INFO *pFriend, int &nGroup, int& nItem )
{
	m_wndGroupList.FindFriend(pFriend, nGroup, nItem);
}



void CFloorPageFriend::UpdateFriend(FRIEND_INFO *pFriend,BOOL bSort)
{
	if(pFriend->comment_name[0]==0)
		m_wndGroupList.SetItemText(pFriend,UserInfoToStr(pFriend));		
	else
		m_wndGroupList.SetItemText(pFriend,pFriend->comment_name);
		
	m_wndGroupList.SetItemTextColor(pFriend,StatusToColor(pFriend->status));

	if ( bSort )
		SortGroupList(pFriend->group_index);

	UpdateFriendCnt();

	//�����б���º������״̬Ҳ����
	CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->FindP2PSendMsgDlgById(pFriend->GetId());
	if(pDlg != NULL && pDlg->GetSafeHwnd() != NULL && pDlg->IsWindowVisible())
		pDlg->ChangeHeadStatus(pFriend);
}


void CFloorPageFriend::UpdateFriendCnt()
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
	}
	else
	{
		m_sFriendCntInfo = "";
	}
}





LRESULT CFloorPageFriend::OnWebIconNotify(WPARAM wParam, LPARAM lParam)
{
	return 0;
	////��ɾ��
	//uint32 fid=wParam;
	//char * szFile = (char *)lParam;
	//int nGroupCount=m_wndGroupList.GetGroupCount();

	//for ( int nGroup=0; nGroup<nGroupCount; nGroup++ )
	//{
	//	int nItemCount=m_wndGroupList.GetItemCount(nGroup);

	//	for ( int nItem=0; nItem<nItemCount; nItem++ )
	//	{
	//		FRIEND_INFO *pData = (FRIEND_INFO *)m_wndGroupList.GetItemData(nGroup,nItem);
	//		if ( pData->id==fid )
	//		{
	//			if ( szFile )
	//			{
	//				UpdateUserBitmap(pData);
	//				m_wndGroupList.UpdateItemImage(nGroup, nItem);
	//			}
	//			else
	//			{
	//				m_wndGroupList.DelItemImage(nGroup,nItem);
	//			}

	//			return 0;
	//		}
	//	}
	//}

	//return 0;
}

LRESULT CFloorPageFriend::OnHintDlgClick(WPARAM wParam, LPARAM lParam)
{
	FRIEND_INFO *pFriend = (FRIEND_INFO *)(lParam);
	CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
	
	if(pFriend->GetId() == g_ImUserInfo.GetId())
		return 1;

	if ( pDlg->m_hWnd==NULL )
		pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());

	if ( pDlg->IsWindowVisible()==FALSE )
		pDlg->ShowWindow(SW_SHOW);

	if ( pDlg->IsIconic() )
	{
		pDlg->ShowWindow(SW_RESTORE);
	}

//	pDlg->FlashWindowEx(FLASHW_TRAY|FLASHW_TIMERNOFG,20,500);
	pDlg->SetForegroundWindow();
	pDlg->BringWindowToTop();

	return 1;
}

//action=0,����
//action=1,ͬ��
//action=2,�ܾ�
//aciton=3,����Ϊ����֪ͨ
void CFloorPageFriend::ShowUserAuthInfo(uint32 fid,uint16 action, CString strVerify,uint32 nGroupId)
{

	FRIEND_INFO *pFriend;
	CString strUserInfo;
	pFriend = FindFriend(fid);
	if(pFriend)
		strUserInfo = UserInfoToStr(pFriend);
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

	CUserAuthInfoDlg * pDlg = new CUserAuthInfoDlg(this,GetDesktopWindow());
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

void CFloorPageFriend::OnStatusGetAck(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_STATUS_GET_ACK ack = (LPXT_STATUS_GET_ACK)data;
		if(m_bIsHideTick)
		{
			if ( ack )
			{
				if(ack->status!=XTREAM_OFFLINE && ack->status!=XTREAM_HIDE)
				{
					list<tagHideTick>::iterator itr = m_lsHideTick.begin();
					for(; itr != m_lsHideTick.end(); itr++)
					{
						if(itr->fid == /*pFriend->GetId()*/ack->id)
						{
							m_lsHideTick.erase(itr);
							break;
						}
					}
				}
			}
			m_bIsHideTick = false;
		}
		else
		{
			if ( ack )
			{
				FRIEND_INFO *pFriend = FindFriend(ack->id);
				if ( pFriend )
				{
					uint8 statusFriend = pFriend->status;
					uint8 statusAck = ack->status;
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


					if(ack->status!=XTREAM_OFFLINE && ack->status!=XTREAM_HIDE)
					{
						list<tagHideTick>::iterator itr = m_lsHideTick.begin();
						for(; itr != m_lsHideTick.end(); itr++)
						{
							if(itr->fid == pFriend->GetId())
							{
								m_lsHideTick.erase(itr);
								break;
							}
						}
					}
				}
			}
		}
	}
}


void CFloorPageFriend::OnWebInd(const char * data, WORD wRet)
{
	LPXT_WEB_MSG msg = (LPXT_WEB_MSG)data;

	if ( msg->msg_type==XT_WEB_MSG::USER_UPDATE )
	{
		FRIEND_INFO *pFriend = FindFriend(msg->sender_id);

		if ( pFriend )
		{
			switch (msg->sub_type)
			{
			case XT_WEB_MSG::BLOG:
				pFriend->webInfo = pFriend->webInfo | 0x01;
				break;
			case XT_WEB_MSG::PIC:
				pFriend->webInfo = pFriend->webInfo | 0x02;
				break;
			case XT_WEB_MSG::MUSIC:
				pFriend->webInfo = pFriend->webInfo | 0x04;
				break;
			case XT_WEB_MSG::VEDIO:
				pFriend->webInfo = pFriend->webInfo | 0x08;
				break;
			case XT_WEB_MSG::FILE:
				pFriend->webInfo = pFriend->webInfo | 0x10;
				break;
			}

			KillTimer(TIMER_INVALIDATELIST);
			SetTimer(TIMER_INVALIDATELIST,200,NULL);
		}		
	}
}

void CFloorPageFriend::OnRecvSendFileReq(const char *data,WORD wRet)
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

		pFriend->SetId(req.friendId);
		pFriend->group_index = 1; //İ����
		pFriend->status = XTREAM_ONLINE;

		//��ӵ�İ����
		AddFriend(pFriend,true);
		//SelItem(pFriend);
		ImGetUserStatus(pFriend->GetId());	

#ifdef _BUSINESS_VESION
		((CMainFrame *)AfxGetMainWnd())->Hangup();
#endif
	}

	if ( pFriend->status==XTREAM_OFFLINE )
	{
		m_bIsHideTick = true;
		ImGetUserStatus(pFriend->GetId());
		pFriend->status=XTREAM_ONLINE;
		UpdateFriend(pFriend);

		AddHideTick(pFriend->GetId());
	}

	//���κ�����
	if ( pFriend->group_index==2 )
	{
		return ;
	}
	else
	{
		((CRecentListCtrl *)g_pRecentListCtrl)->AddUser(pFriend);
	}
	
	bool bFindDlg = g_pMessageHandleWnd->FindP2PSendMsgDlg(req.friendId);

	CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);

	if ( pDlg->m_hWnd == NULL )
	{
		pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());

		pDlg->ShowWindow(SW_MINIMIZE);
		pDlg->ShowWindow(SW_SHOW);	
	}

	CString strTitle;	
	strTitle.Format("%s�����ļ�:",UserInfoToStr(pFriend));
	ShowUserHintDlg(pFriend, strTitle,req.name,m_hWnd);
	pDlg->FlashWindow(TRUE);
	pDlg->RecvSendFileReq(data,wRet);
}

// ���յ������û�����������Ϣ
void CFloorPageFriend::OnRecvP2PMsg(const char *data,WORD wRet)
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
			m_wndGroupList.InvalidateList();

			
			CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
			
			if ( pDlg != NULL && pDlg->m_hWnd != NULL )
			{
				if( pDlg->m_hWnd != NULL)
				{
					pDlg->UpdateFriendInfo(pFriend);
					pDlg->UpdateSignature();
				}
			}
			
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

			m_wndGroupList.InvalidateList();
		}

		return ;
	}

#ifdef _BUSINESS_VESION
	if ( msg->data_type==XT_MSG::OPEN_P2PSENDMSG_DLG )
	{
		FRIEND_INFO *pUser = FindFriend(msg->from_id);
		if(pUser == NULL ||
			(pUser != NULL && pUser->group_index == 1))
		{	// Ϊİ����ʱ�Զ��Ӵ�
			AutoAccept(msg->from_id);
		}

		if ( pUser==NULL )
		{
			pUser = new FRIEND_INFO;

			pUser->SetId(msg->from_id);
			pUser->group_index = 1;//İ����
			pUser->status = XTREAM_ONLINE;
			sprintf(pUser->nickname,msg->from_nickname);
/*
			Json::Reader jReader;
			Json::Value jvRecv;
			string strProductID = "";
			if (jReader.parse(msg->data, jvRecv))
			{ 
				string strGuid = jvRecv["guid"].asString();
				pUser->SetGuid(strGuid.c_str());

				strProductID = jvRecv["productID"].asString(); 
			}
*/
			pUser->SetGuid(msg->data);
			pUser->SetUserName("");
			pUser->SetPassword("");

			//��ӵ�İ����
			AddFriend(pUser,true);
			SelItem(pUser);
			ImGetUserStatus(pUser->GetId());		
			// �����촰��
			ShowMessageDlg(pUser/*, strProductID.c_str()*/);
		}
		else
		{
			ShowMessageDlg(pUser);
		}
			// ���ù���״̬
			((CMainFrame *)AfxGetMainWnd())->Hangup();

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
		ImGetUserStatus(pFriend->GetId());		

#ifdef _BUSINESS_VESION
		((CMainFrame *)AfxGetMainWnd())->Hangup();
#endif
	}
	else
	{
		strcpy(msg->from_nickname, pFriend->nickname);//����ǳ�
	}

	if ( pFriend->status==XTREAM_OFFLINE )
	{
		m_bIsHideTick = true;
		ImGetUserStatus(pFriend->GetId());
		pFriend->status=XTREAM_ONLINE;
		UpdateFriend(pFriend);

		AddHideTick(pFriend->GetId());
	}

	// ����Զ��ظ�����Ϊ������������̨����Ϣ���������У����Կ��߳��������Զ��ظ�����Ȼ����ֽ��ն��ղ�����Ϣ�������
	if(msg->data_type == XT_MSG::IM_MSG && (g_ImUserInfo.status == XTREAM_LEAVE || g_ImUserInfo.status == XTREAM_BUSY))
	{
#ifdef _BUSINESS_VESION
		if(pFriend->group_index == 1)
		{	// Ϊİ����ʱ�Զ��Ӵ�
#ifdef _PRESS_TEST
			AutoAcceptPress(*msg);
#else
			AutoAccept(msg->from_id);
#endif
		}
		else
#endif
		{	// ��İ�����Զ��ظ�
			unsigned threadID;
			_beginthreadex(NULL, 0, AutoReply, (void*)msg->from_id, 0, &threadID);
		}
		
	}



	//���κ�����
	if ( pFriend->group_index==2 )
	{
		return ;
	}
	else
	{
		((CRecentListCtrl *)g_pRecentListCtrl)->AddUser(pFriend);
	}
	

	bool bFindDlg = g_pMessageHandleWnd->FindP2PSendMsgDlg(pFriend->GetId());

	CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);

	if ( pDlg->m_hWnd==NULL )
	{
		pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());

		pDlg->ShowWindow(SW_HIDE);
		if( g_pDuiCounsel != NULL && ::IsWindowVisible(*g_pDuiCounsel) && pFriend->GetId() >= 5000000)
			g_pDuiCounsel->ShowWindow(false);
		else
		{
			pDlg->ShowWindow(SW_MINIMIZE);
			pDlg->ShowWindow(SW_SHOW);	
		}
	}

	vector<DOWNLOAD_OBJ> arrDownloadTask;
	CString sShowMsg;
	if(msg->data_type == 0 && TranslateMsg(msg->data,sShowMsg,arrDownloadTask))
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

	CString strTitle;

	//����Ҳ�����ǰ��SendMessageDlg,�򵯳�hint dlg
	switch ( msg->data_type )
	{
	case XT_MSG::IM_MSG://������Ϣ
		{
		PlayTone(EVOICE_RECEIVE_MSG);
		if( g_pDuiCounsel != NULL && pFriend->GetId() >= 5000000)
			g_pDuiCounsel->ShowWindow(true);
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
				pDlg->FlashWindow(TRUE);
				if(g_pDuiCounsel != NULL && g_pDuiCounsel->m_vecP2PSendMsgDlg.size() <= 1 && pFriend->GetId() >= 5000000)
					pDlg->ShowWindow(SW_SHOW);
			}
		}
		else
		{	
			strTitle.Format("%s˵:",UserInfoToStr(pFriend));
			
			if ( pDlg->m_hWnd==NULL )
			{
				pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());

				pDlg->ShowWindow(SW_MINIMIZE);
				pDlg->ShowWindow(SW_SHOW);
			
				ShowUserHintDlg(pFriend, strTitle, msg->data, m_hWnd);
			}
			else
			{	
				if(msg->msg_type != 10)
				{
					if ( pDlg->IsIconic() )
					{
						ShowUserHintDlg(pFriend, strTitle, msg->data, m_hWnd);
					}
				}
				pDlg->FlashWindowEx(FLASHW_TRAY|FLASHW_TIMERNOFG,20,500);
			}

			pDlg->FlashWindow(TRUE);
			if(g_pDuiCounsel != NULL && g_pDuiCounsel->m_vecP2PSendMsgDlg.size() <= 1 && pFriend->GetId() >= 5000000)
				pDlg->ShowWindow(SW_SHOW);
		}

		{
			CRecentListCtrl *pRecentList=(CRecentListCtrl *)g_pRecentListCtrl;
			pRecentList->AddUser(pFriend);
		}

		if (pDlg->m_hWnd)
			g_LocalData.SaveTalkMsg(*msg,msg->from_id);

		if(g_pDuiCounsel != NULL && pFriend->GetId() >= 5000000)
		{
			g_pDuiCounsel->SetFriendListElement(pFriend, false, false, false, false, true);
		}
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
				
				if ( pDlg->m_hWnd && pDlg->IsIconic()==FALSE )
				{
					/*if(g_LocalData.GetBasicSystemSetting().bPlayMagicFace )
						pDlg->ShowMagicFace(face_name);*/

					pDlg->BringWindowToTop();
				}
				else
				{
					bShowHint = true;
				}

				if ( bFindDlg==false )
				{
					if ( pDlg->m_hWnd==NULL )
					{
						pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
						pDlg->ShowWindow(SW_MINIMIZE);
						pDlg->ShowWindow(SW_SHOW);
					}
					
					//pDlg->FlashWindowEx(FLASHW_TRAY|FLASHW_TIMERNOFG,20,500);
					pDlg->FlashWindow(TRUE);

				}
				else
				{	if ( pDlg->m_hWnd==NULL )
					{
						pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
						pDlg->ShowWindow(SW_MINIMIZE);
						pDlg->ShowWindow(SW_SHOW);
						pDlg->FlashWindow(TRUE);

						bShowHint=true;
					}
					else
					{
						if ( pDlg->IsIconic() )
						{
							bShowHint = true;
						}

						pDlg->FlashWindowEx(FLASHW_TRAY|FLASHW_TIMERNOFG,20,500);
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
			pDlg->FlashWindow(TRUE);
		}
		break;
	case XT_MSG::MM_REQ:
		{
			PlayTone(EVOICE_VOICE_CALL, TRUE);
			if ( pDlg->m_hWnd==NULL )
			{
				pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());

				pDlg->ShowWindow(SW_MINIMIZE);
				pDlg->ShowWindow(SW_SHOW);	
			}

			CString strTitle;	
			strTitle.Format("%s:",UserInfoToStr(pFriend));

			uint32 mmID;
			uint8 mmType;
			CRecvPacket rp(msg->data,sizeof(msg->data));
			rp>>mmID>>mmType;

			ShowUserHintDlg(pFriend, strTitle, mmType==0 ? "����������Ƶ����" : "����������������",m_hWnd);
			pDlg->FlashWindow(TRUE);
			pDlg->MM_OnRecvRequest(*msg);
		}
		break;
	case XT_MSG::MM_CANCEL:
		{
			StopTone();
		}
		break;
	case XT_MSG::WIN_SHAKE:
		{	// ���ڶ���
			WORD nVK = 0;
			g_LocalData.GetShakeWinConfig(nVK);
			if(nVK == 0)////֧������
			{
				PlayTone(EVOICE_SHOCK_SCREEN);
			}
		}
		break;
	default:break;
	}			
}

//
void CFloorPageFriend::OnP2PMsgAck(const char *data,WORD wRet)
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

//����ϵͳ��Ϣ
void CFloorPageFriend::OnRecvSvrMsg(const char *data,WORD wRet)
{
	LPXT_SERVERMSG sysmsg = (LPXT_SERVERMSG)data;
	
	if ( wRet==0 )
	{	
		if ( sysmsg->type==1 )//�ص�¼��ʾ��Ϣ
		{
			KillTimer(TIMER_BEAT);
			KillTimer(TIMER_RELOGIN);		
		}		
	}
}

void CFloorPageFriend::RemoveHideTick(uint32 fid)
{
	list<tagHideTick>::iterator it;
	
	for (it=m_lsHideTick.begin(); it!=m_lsHideTick.end(); it++ )
	{
		m_lsHideTick.erase(it);
		break;
	}
}

void CFloorPageFriend::OnNetWorkError()
{
	TRACE("ERROR:������󣡣�\n");
	KillTimer(TIMER_BEAT);
	SetTimer(TIMER_RELOGIN,1000,NULL);

	this->m_bDisconnectFlag = true;
	this->m_nPreOnlineStatus = g_ImUserInfo.status;
	//theApp.m_pMainWnd->PostMessage(WM_HEART_TIMEOUT,0,0);
}

void CFloorPageFriend::OnLogOut()
{
	KillTimer(TIMER_BEAT);
	KillTimer(TIMER_RELOGIN);
	m_mapWebInfo.clear();
	m_mapNewFriend.clear();
	ImSaveUserOperations();

	SaveUserWebData();
	SaveLocalFriends();
}

void CFloorPageFriend::AddHideTick(uint32 fid)
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

void CFloorPageFriend::CheckHideTick()
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

BOOL CFloorPageFriend::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
		case 0x0D://enter
			m_wndGroupList.SetFocus();	
			return TRUE;
		default:
			break;
		}
	}

	return __super::PreTranslateMessage(pMsg);
}

void CFloorPageFriend::OnRecvClusterMsg(const char *data,WORD wRet)
{
	LPXT_MSG msg = (LPXT_MSG)data;

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

BOOL CFloorPageFriend::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CFloorPageFriend::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	__super::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	CMenuEx::InitPopupMenu(pPopupMenu,nIndex,bSysMenu);
}

void CFloorPageFriend::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	__super::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
	CMenuEx::MeasureItem(lpMeasureItemStruct);
}

void CFloorPageFriend::OnRefreshFriendList()
{
	static DWORD dwTickPreGet=(DWORD)-1;
	DWORD dwTick = GetTickCount();

	if ( (dwTickPreGet-dwTick)>60 )
	{
		ImGetFriendList();
		m_wndGroupList.AddRefreshFlag();
		dwTickPreGet = dwTick;
	}
}

void CFloorPageFriend::OnShowAll()
{
	m_bShowAllFriend=TRUE;
	g_LocalData.SetShowAllFriend(m_bShowAllFriend);
	UpdateFriendList();
}

void CFloorPageFriend::OnHideOffline()
{
	m_bShowAllFriend=FALSE;
	g_LocalData.SetShowAllFriend(m_bShowAllFriend);
	UpdateFriendList();
}

LRESULT CFloorPageFriend::OnChangeFriendGroup(WPARAM wParam, LPARAM lParam)
{
	int nFriendID=(int)wParam;
	int nNewGroupID=(int)lParam;
	// ���ҵ��ú���
	FRIEND_INFO* pFriend = FindFriend(nFriendID);
	if(pFriend==NULL)
		return -1;
	m_wndGroupList.SetCurSel(pFriend);

	// ��Ӧ�ƶ����ѵ���Ϣ����
	int nGroupIndex = g_GroupInfo.FindItemIndex(nNewGroupID);
	OnGroupListNotify(NM_FL_DRAGDROP, nGroupIndex);

	return 0;
}

void CFloorPageFriend::MakeTempCluster(FL_GROUP_INFO* pGroup)
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
			CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(&pTempClusterInfo->members[0]);
			if ( pDlg->m_hWnd==NULL )
				pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
			pDlg->ShowWindow(SW_SHOW);
		}
		else
		{
			// Ӧ�����ж��Ƿ���ڸ�Ⱥ�ĶԻ���
			CP2PSendMsgDlg *pDlg =NULL;
			pDlg=g_pMessageHandleWnd->GetTempGroupSendMsgDlg(nGroupIndex);
			if(pDlg==NULL)
			{
				// ��������ڸöԻ����򴴽�֮
				pDlg= g_pMessageHandleWnd->GetP2PSendMsgDlg(&pTempClusterInfo->members[0]);
				if ( pDlg->m_hWnd==NULL )
				{
					pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
				}
				pDlg->CreateTempCluster(*pTempClusterInfo,nGroupIndex);
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


void CFloorPageFriend::OnNCFlRbum()
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
	popmenu->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this);
}

// ɾ���������
void CFloorPageFriend::OnDelMFriend()
{
	const MULTI_SEL_LST& lst=m_wndGroupList.GetMultSel();
	FL_ITEM_POS pos;
	vector<FRIEND_INFO*> delUsers;
	int i;
	int nC = lst.Size();
	//int nGroup,nItem;
	if ( nC == 0 )
	{
		return ;
	}
	FRIEND_INFO *pFriend ;
	CString str;
	
	str.Format("ȷ��Ҫɾ��ѡ��ĺ�����?");
	if (MessageBox(str,IM_CAPTION,MB_YESNO|MB_ICONWARNING)!=IDYES)
	{
		return ;
	}


	for(i=0;i<nC;i++)
	{
		pos=lst.Get(i);
		pFriend = (FRIEND_INFO *)m_wndGroupList.GetItemData(pos.nGroupID,pos.nItem);
		delUsers.push_back(pFriend);
	}

	for(i=0;i<nC;i++)
	{
		pFriend=delUsers[i];
		if(pFriend->group_index != 1)
		{
			//ɾ���û�
			XT_FRIENDGROUP_MOD_REQ req;
			req.action = GROUP_DEL;
			req.group_id = 0;
			req.fid = pFriend->GetId();
			req.uid = g_ImUserInfo.GetId();
			//���û���id
			g_pProtocol->SendReq(req);
		}

		//ɾ���û�
		DelFriend(pFriend);
	}

	m_wndGroupList.ClearMultSel();
}

void CFloorPageFriend::OnModfCommName()
{
	int nGroup = m_wndGroupList.GetCurSel().nGroupID;
	int nItem  = m_wndGroupList.GetCurSel().nItem;

	if ( nGroup==-1 || nItem==-1 )
	{
		return ;
	}
	FRIEND_INFO *pFriend = (FRIEND_INFO *)m_wndGroupList.GetItemData(nGroup,nItem);
	if(pFriend==NULL)
		return;
	CModfCommNameDlg *pDlg=CModfCommNameDlg::NewDlg(this,pFriend,GetDesktopWindow());
	pDlg->CenterWindow();
	pDlg->ShowWindow(SW_SHOW);
}



void CFloorPageFriend::SetCommName( FRIEND_INFO* pFriend, const CString& sCommName)
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
	m_wndGroupList.SetItemText(pFriend, s);
}


void CFloorPageFriend::OnCreateTempCluster()
{
	FL_ITEM_POS pos=m_wndGroupList.GetCurSel();

	int nGroup = pos.nGroupID;
	int nItem  = pos.nItem;
	if(nGroup>-1)
	{
		FL_GROUP_INFO* pGroup = m_wndGroupList.GetGroupData(nGroup);
		MakeTempCluster(pGroup);
	}
}

void CFloorPageFriend::ResetName(uint32 fid)
{
	vector<FL_GROUP_INFO *> vecGroup;
	int nGroupCount = m_wndGroupList.GetAllGroup(vecGroup);

	for(int i = 0; i < nGroupCount; ++i)
	{
		FL_GROUP_INFO *group = vecGroup[i];
		int nGroup = group->dwData;
		int nItems = group->vecItem.size();

		for ( int nItem=0; nItem<nItems; nItem++)
		{
			FRIEND_INFO *pFriend = (FRIEND_INFO *)m_wndGroupList.GetItemData(nGroup,nItem);

			if ( fid==pFriend->GetId())
			{
				m_wndGroupList.SetItemText(nGroup,nItem,pFriend->nickname);

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

void CFloorPageFriend::ModifyName(XT_MOD_FRIEND_COMMNAME_ACK *pAck)
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
	int nGroupCount = m_wndGroupList.GetAllGroup(vecGroup);

	for(int i = 0; i < nGroupCount; ++i)
	{
		FL_GROUP_INFO *group = vecGroup[i];
		int nGroup = group->dwData;
		int nItems = group->vecItem.size();

		for ( int nItem=0; nItem<nItems; nItem++)
		{
			FRIEND_INFO *pFriend = (FRIEND_INFO *)m_wndGroupList.GetItemData(nGroup,nItem);

			if ( pAck->fid==pFriend->GetId())
			{
				m_wndGroupList.SetItemText(nGroup,nItem,UserInfoToStr(pFriend));
				break;
			}
		}
	}
}


void CFloorPageFriend::OnModFCNameAck(const char* data,WORD wRet)
{
	XT_MOD_FRIEND_COMMNAME_ACK *pAck=(XT_MOD_FRIEND_COMMNAME_ACK*)data;
	if(wRet==0)
	{
		// �޸ı�ע�ɹ�
		CMessageBox::Prompt("�޸ı�ע�ɹ����������Ѿ��������ύ����Ϣ��");
		ModifyName(pAck);
		////////////////////////////////////////////////////////////////////
	}
	else
	{
		// �޸ı�עʧ��
		CMessageBox::Prompt(g_pProtocol->GetLastError(),"�޸ı�עʧ��");
		ResetName(pAck->fid);
	}
}


void CFloorPageFriend::GetDiffFriends(const FRIEND_LIST & oldFriends,
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

void CFloorPageFriend::ImGetFriendBasicInfo(const vector<uint32> & arrFriend) const
{
	const int maxids = 200;
	char buff[MAX_SEND_BUFFERS];
	LPXT_GET_FRIEND_BASICINFO_REQ req = (LPXT_GET_FRIEND_BASICINFO_REQ)buff;
	req->type = XT_GET_FRIEND_BASICINFO_REQ::TYPE_FRIEND;
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
				// id�б��Ѿ���д���
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


void CFloorPageFriend::OnBasicInfoAck(const char * data,WORD wRet)
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
		
		if ( bExist )
		{
			// ��������ں����б��������,����ȡwebͷ��
			if ( pFriend->status!=XTREAM_OFFLINE )
			{
				// ���ظ��û����Զ���ͷ��ͼƬ
				g_webUserHeadIconProcess.GetUserHeadIcon(pFriend,this);
			}
		//	UpdateFriend(pFriend,FALSE);
		}
		else
		{
			// �ú����ڱ��ػ����в�����
			;
		}

		CRecentListCtrl *pRecentList = (CRecentListCtrl *)g_pRecentListCtrl;
		pRecentList->UpdateUser(pFriend);
	}


	// ���˻�ȡȺ���ѻ������ϵ����
	//if( (pAck->count) > 0 && (pAck->count != nGroupFriend))
	//{
	//	// ����ˢ���б�
	//	UpdateFriendList();
	//}
	m_wndGroupList.InvalidateList();
	KillTimer(TIMER_SORTLIST);
	SetTimer(TIMER_SORTLIST,100,NULL);
	
}

// �յ������û��Ĳ������ݰ�
void CFloorPageFriend::OnRecvP2pTest(const char * data,WORD wRet)
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

void CFloorPageFriend::OnRecvP2pTestAck(const char * data, WORD wRet)
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

void CFloorPageFriend::FindOldFriends(const FRIEND_LIST & oldFriends, const vector<FRIENDITEM> & newFriends, vector<uint32> & rst)
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

LRESULT CFloorPageFriend::OnThreadNotify(WPARAM wParam, LPARAM lParam)
{
	//if ( wParam==1 )
	//{
	//	if ( m_vecThreadWebData.size()==0 )
	//	{
	//		m_wndGroupList.InvalidateList();
	//	}
	//}
	//else
	//if ( wParam==2 )
	//{
	//	m_seThFriendWebRun.Lock();

	//	for ( int i=0; i<m_vecThFriendWebRun.size(); i++ )
	//	{
	//		if ( (DWORD)m_vecThFriendWebRun[i]==(DWORD)lParam )
	//		{
	//			CloseHandle(m_vecThFriendWebRun[i]->hThread);
	//			delete m_vecThFriendWebRun[i];
	//			m_vecThFriendWebRun.erase(m_vecThFriendWebRun.begin()+i);
	//			break;
	//		}
	//	}

	//	if ( m_vecThFriendWebWait.size()>0 )
	//	{
	//		DWORD dwThreadID;
	//		tagThreadFriendWebParam *pTh=m_vecThFriendWebWait[0];
	//		pTh->hThread =  CreateThread(NULL,0,
	//							(LPTHREAD_START_ROUTINE)LoadFriendWebInfoProc,
	//								pTh,
	//								0,
	//								&dwThreadID);

	//		m_vecThFriendWebRun.push_back(pTh);
	//		m_vecThFriendWebWait.erase(m_vecThFriendWebWait.begin());
	//	}

	//	m_seThFriendWebRun.Unlock();

	//}

	return 0;
}

void CFloorPageFriend::OnPaint()
{
	CPaintDC dc(this);
/*
	CRect rc;
	GetClientRect(&rc);
	rc.bottom=rc.top+23;

	CResBitmap bmp;
	bmp.LoadBitmap(IDB_HEADER_BACK);
	HorizStretchBlt(&dc,0,0,rc.Width(),&bmp);
	*/
}

void CFloorPageFriend::OnBnClickedButtonAdd()
{
	CAddFriendOrClusterDlg::Show();
}

void CFloorPageFriend::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFloorPage::OnShowWindow(bShow, nStatus);

	if (bShow)
		m_wndGroupList.SetFocus();
}

#ifdef _BUSINESS_VESION
// ����ҵ��Ŷӷ��飬����Ѿ����ھͲ����
static BOOL g_isAddMyTeam = FALSE;	// ȷ��ֻ�����һ�Ρ��ҵ��Ŷӡ�
BOOL CFloorPageFriend::AddGroup_MyTeam()
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

// �Զ��ظ��������߻���æµʱ��
unsigned __stdcall CFloorPageFriend::AutoReply(void * pParam)
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

	msg.send_time   = time(0);

	msg.to_id		= pFriend->GetId();
	msg.dest_ip		= pFriend->ip;
	msg.dest_port	= pFriend->port;
	msg.msg_type    = 0;		

	msg.data_type   = 0;//��ʱ������Ϣ

	CString text(item.strText);
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

void CFloorPageFriend::OnFriendInfoInd(const char *data,WORD wRet)
{
	// ��ȡ������Ϣ
	XT_INFO_IND *pFriendInd = (XT_INFO_IND *)data;

	unsigned threadID;
	_beginthreadex(NULL, 0, UpdateFriend, (void*)pFriendInd->fid, 0, &threadID);
}

// ���º�����Ϣ
unsigned __stdcall CFloorPageFriend::UpdateFriend(void * pParam)
{
	Sleep(200);

	XT_USERINFO_GET_REQ req;
	req.fid = (uint32)pParam;
	g_pProtocol->SendReq(req);

	return 0;
}
