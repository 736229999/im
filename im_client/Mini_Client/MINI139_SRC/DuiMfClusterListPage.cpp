#include "stdafx.h"
#include "MainFrame.h"

// im\TabPageCluster.cpp : ʵ���ļ�
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


// CFloorPageCluster ��Ϣ�������

//�������Ժ����б������Ҽ�
LRESULT CMainFrame::OnGroupListNotify(WPARAM wParam, LPARAM lParam)
{
	CL_GROUP_POS pos=m_pClusterList->GetCurSel();

	switch ( wParam )
	{
		// Ⱥ��Ա�б���˫��
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

				int nType;//2��Ⱥ����1Ⱥ����Ա��0��ͨ��Ա
				nType = pCluster->GetMemberType(g_ImUserInfo.GetId());

				CString str;
				if ( g_ImUserInfo.status!=XTREAM_OFFLINE )
				{
					popmenu->AppendMenu(MF_STRING,ID_CLUSTER_SENDMSG,"����Ⱥ������");
					////popmenu->AppendMenu(MF_SEPARATOR,0);

					//if ( nType==0 )
					////popmenu->AppendMenu(MF_STRING,ID_CLUSTER_MODIFY,"�鿴Ⱥ����");
					//else
					////popmenu->AppendMenu(MF_STRING,ID_CLUSTER_MODIFY,"����/�鿴Ⱥ����");

					////popmenu->AppendMenu(MF_STRING,ID_CLUSTER_HOME,"Ⱥ��ҳ");
					////popmenu->AppendMenu(MF_STRING,ID_CLUSTER_FORUM,"Ⱥ������");
					////popmenu->AppendMenu(MF_STRING,ID_CLUSTER_ALBUM,"Ⱥ���");

					////popmenu->AppendMenu(MF_STRING,ID_CINFO_REFRESH,"����Ⱥ��Ϣ");
					popmenu->AppendMenu(MF_STRING,ID_CLUSTER_REFUSE,pCluster->bRefused? "�����ֹ..." :"��ֹȺ��Ϣ");
					popmenu->AppendMenu(MF_STRING,ID_CLUSTER_MSGHISTORY,"�鿴�����¼");
					////popmenu->AppendMenu(MF_STRING,ID_CLUSTER_CREATE,"����Ⱥ...");
					popmenu->AppendMenu(MF_STRING,ID_SEARCH_CLUSTER,"����������Ⱥ...");

					if ( pCluster->bMemberInit || pCluster->Count() > 0 )
					{
						if ( nType==2 )//Ⱥ��
						{
							popmenu->AppendMenu(MF_STRING,ID_CLUSTER_DISMISS,"��ɢȺ");
						}
						else//Ⱥ����Ա
							if ( nType==1 )
							{
								popmenu->AppendMenu(MF_STRING,ID_CLUSTER_QUIT,"�˳�Ⱥ");
							}
							else//if ( nType==0 )//Ⱥ��Ա
							{
								popmenu->AppendMenu(MF_STRING,ID_CLUSTER_QUIT,"�˳�Ⱥ");
							}				
					}

					popmenu->AppendMenu(MF_STRING,ID_CLIST_REFRESH,"ˢ��Ⱥ�б�");

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
					popmenu->AppendMenu(MF_STRING,ID_CLIST_REFRESH,"ˢ��Ⱥ�б�");
					popmenu->AppendMenu(MF_STRING,ID_CLUSTER_CREATE,"����Ⱥ...");
					popmenu->AppendMenu(MF_STRING,ID_SEARCH_CLUSTER,"����������Ⱥ...");	
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
			//����"�Ҵ�����Ⱥ"��"�Ҽ����Ⱥ"
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

// ����Ⱥ�����¼�Ի���
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
		if ( ind->action==0 )//����
		{
		//	CDailogClusterCheck dlg(ind->nFamilyID,ind->uid,ind->info,GetDesktopWindow());
		//	dlg.DoModal();
			CDailogClusterCheck::Show(ind);
		}

		if (ind->action==1 )//ͬ��
		{
			CString str ;
			str.Format("������ΪȺ��%u �ĳ�Ա",ind->cluster_id);	
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
			str.Format("Ⱥ %u �ܾ���ļ��룡",ind->cluster_id);
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
				//������ע�͵�����ֹ��ɢȺʱȺ���ֱ����ȺID
// 				if(pCluster->bBasicInfo == false)
// 					strcpy(pCluster->szFamilyName,itoa(pCluster->nFamilyID,szTemp,10));
				CString str;

				if ( pUser )
				{
					if(g_ImUserInfo.GetId() == pCluster->GetCreator()->GetId())
						str.Format("Ⱥ��%s��������ɢ��",pCluster->szFamilyName/*,pUser->GetUserName().c_str()*//*nickname*/);
					else
						str.Format("Ⱥ��%s����Ⱥ����%s����ɢ��",pCluster->szFamilyName,pCluster->GetCreator()->GetUserName().c_str()/*nickname*/);
				}
				else
					str.Format("Ⱥ��%s������ɢ��",pCluster->szFamilyName/*,ind->destructor_id*/);
				ShowUserHintDlg(str);

				//дȺϵͳ��Ϣ
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

// ���������ķ������ݴ���
void CMainFrame::OnClusterApplyAck(const char *data,WORD wRet)
{
	LPXT_CLUSTER_APPLY_ACK ack = (LPXT_CLUSTER_APPLY_ACK)data;

	if ( wRet==0 )
	{
		if ( ack->nAction==0 && ack->nFamilyProp==PERM_FAMILY )//Add
		{
			if ( ack->nFlag==0 )
			{
				ShowMessage("����Ⱥ�ɹ���",MB_OK|MB_ICONINFORMATION);

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
			if ( ack->nFlag==1 )//��Ҫ��֤
			{
				CDialogClusterAuth::Show(ack->nFamilyID);
			}
			else
			if ( ack->nFlag==2 )//�ܾ�
			{
				CString str;
				str.Format("Ⱥ %u �ܾ���ļ��룡",ack->nFamilyID);
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

// �����ص�Ⱥ�б������
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
			// �����Ⱥ�Ļ�����Ϣ��û�л�ȡ�������������
			if (pCluster->bBasicInfo == false)
			{
				strcpy(pCluster->szFamilyName ,itoa(pCluster->nFamilyID,szTemp,10));
				// ���ÿ��Ⱥ����Ϣ
				ImGetClusterInfo(ack->nFamilyID[i]);
			}
		}// end for

		//����Ⱥ�б�����Ѿ��˳����߽�ɢ��Ⱥ
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

		//����ÿ��Ⱥ��������Ϣ(�Ƿ���ֹ,�Ƿ��Զ�����ħ������...)
		g_LocalData.LoadClusterConfig((char*)g_ImUserInfo.GetUserName().c_str());

		g_LocalData.LoadClusterExitConfig((char*)g_ImUserInfo.GetUserName().c_str());
		// ���������ϵ������
		m_pRecentList->Load();

		//��IM��������ȡ��ʷ�����¼
		ImGetOfflineMsg();
	}
	else
	{
		//����ÿ��Ⱥ��������Ϣ(�Ƿ���ֹ,�Ƿ��Զ�����ħ������...)
		g_LocalData.LoadClusterConfig((char*)g_ImUserInfo.GetUserName().c_str());
		g_LocalData.LoadClusterExitConfig((char*)g_ImUserInfo.GetUserName().c_str());
		//ShowMessage("��ȡȺ�б�ʧ�ܣ������ԡ�");
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

	////����Ⱥ��Ƭ��ȡ����
	XT_PROFILENAME_GET_REQ reqMem;
	reqMem.nFamilyID   =cid;
	g_pProtocol->SendReq(reqMem);
}

// ������յ�Ⱥ��Ϣ
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
	//������"�Ҵ�����Ⱥ"��"�Ҽ����Ⱥ"
	m_pClusterList->AddRoot("�Ҵ�����Ⱥ");	
	m_pClusterList->AddRoot("�Ҽ����Ⱥ");
	//m_pClusterList->AddRoot("Ⱥ ��");

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
				g_vecClusterExit.push_back(&pTemp);/////�˳����߽�ɢ��Ⱥ

				delete g_vecCluster[i];
				g_vecCluster.erase(g_vecCluster.begin()+i);
				break;
			}
		}
	}

	g_pMessageHandleWnd->CloseClusterSendMsgDlg(cid);
}

// ���Ա�仯�¼�,����,����
void CMainFrame::OnCMemberInd(const char *data, WORD wRet)
{
	LPXT_CMEMBER_IND ind = (LPXT_CMEMBER_IND)data;
	char szTemp[20];
	if ( wRet==0 )
	{
		CLUSTER_INFO *pCluster = FindClusterInfo(ind->cluster_id);

		if ( pCluster==NULL && ind->action==0 && ind->cluster_type==PERM_FAMILY )//����ΪȺ�ĳ�Ա
		{
			if ( ind->member_id==g_ImUserInfo.GetId() )
			{
				CString str; 
				str.Format("������ΪȺ��%s���ĳ�Ա��",ind->clustername );
				CLUSTER_INFO * pCluster = FindClusterInfo(ind->cluster_id);

				if ( pCluster==NULL )
				{
					pCluster=new CLUSTER_INFO();
					pCluster->nFamilyID = ind->cluster_id;
					pCluster->nFamilyProp = PERM_FAMILY;

					g_vecCluster.push_back(pCluster);					
				}
				// ������ʾ����
				ShowClusterHintDlg(pCluster,"Ⱥ��Ϣ",str,m_hWnd);
				ImGetClusterInfo(ind->cluster_id);
				
				//дȺϵͳ��Ϣ
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
			else//�޷���ȡ���ǳ�����ID����
				strcpy(ind->nickname,itoa(ind->member_id,szTemp,10));

			CString str;
			CString strClusterName = pCluster->szFamilyName;		// ����һ��Ⱥ�����֣�����Ⱥ��ɾ����������
			if ( ind->action==0 )//��ӳ�Ա֪ͨ
			{
				if ( pCluster->GetFriendInfo(ind->member_id)==NULL )
				{
					FRIEND_INFO member;
					member.SetId(ind->member_id);
					member.status = XTREAM_OFFLINE;
					member.cluster_property = CMEMBER_NORMAL;//putong ��Ա
					//strcpy(ind->nickname,itoa(ind->member_id,szTemp,10));����Ҫ�ѳ�ԱID����nickname
					strcpy(member.nickname,ind->nickname);
					pCluster->members.push_back(member);

					// ��ȡ�ó�Ա״̬
					XT_STATUS_GET_REQ req;
					req.id = g_ImUserInfo.GetId();
					req.fid = ind->member_id;
					g_pProtocol->SendReq(req);
					// ��ȡ�ó�Ա����ϸ����
					XT_USERINFO_GET_REQ getInfo;
					getInfo.fid = ind->member_id;
					g_pProtocol->SendReq(getInfo);

					BuildCluster(ind->cluster_id);	
					//str.Format("Ⱥ \"%s\" ���³�Ա���롣��(��)��: %s��",pCluster->szFamilyName,ind->nickname);
					str.Format("��%s������Ⱥ��%s��",/*pFriendInfo->username*/ind->nickname,pCluster->szFamilyName);

					//дȺϵͳ��Ϣ
					/*SYSTEM_MSG msgSys;
					msgSys.dwTime = time(NULL);
					msgSys.iType  = SYSTEM_MSG::SYSMSG_CLUSTER;
					sprintf(msgSys.szMessage,str);
					g_LocalData.AddSystemMsg(g_ImUserInfo.GetId(),msgSys);*/
					
					// �³�Ա����Ͳ��ٳ���ʾ��
					//return;
				}	
			}
			else if ( ind->action==1 )//�˳�,ɾ��
			{
				if ( ind->member_id==g_ImUserInfo.GetId() )
				{
					str.Format("���˳���Ⱥ��%s��",pCluster->szFamilyName);
					DelCluster(ind->cluster_id);
				}
				else
				{
					pCluster->DelMember(ind->member_id);
					str.Format("��%s���˳���Ⱥ��%s��",/*pFriendInfo->username*/ind->nickname,pCluster->szFamilyName);
				}

				DelClusterMember(ind->cluster_id,ind->member_id);
			}
			else if ( ind->action==2 )//����Ϊ����Ա֪ͨ
			{
				if ( ind->member_id==g_ImUserInfo.GetId() )
				{
					str.Format("����Ⱥ��%s����Ⱥ����%s�����óɹ���Ա",pCluster->szFamilyName,pCreater->GetUserName().c_str());
				}
				else
				{
					

					CString str;
					str.Format("��%s����Ⱥ��%s����Ⱥ����%s�����óɹ���Ա",/*pFriendInfo->username*/ind->nickname,ind->clustername,pCreater->GetUserName().c_str());

					//str.Format("��%s����ΪȺ��%s���Ĺ���Ա",pFriendInfo->username/*ind->nickname*/,pCluster->szFamilyName);
				}

				FRIEND_INFO *pFriend =	pCluster->GetFriendInfo(ind->member_id);
				if ( pFriend )
					pFriend->cluster_property = 1;

			}
			else if ( ind->action==3)// ��������Ա
			{
				if ( ind->member_id==g_ImUserInfo.GetId() )
				{
					str.Format("����Ⱥ��%s����Ⱥ����%s��ȡ���˹���ԱȨ��",pCluster->szFamilyName,pCreater->GetUserName().c_str());
				}
				else
				{
					str.Format("��%s����Ⱥ��%s����Ⱥ����%s��ȡ���˹���ԱȨ��",/*pFriendInfo->username*/ind->nickname,pCluster->szFamilyName,pCreater->GetUserName().c_str());
				}

				FRIEND_INFO *pFriend =	pCluster->GetFriendInfo(ind->member_id);
				if ( pFriend )
					pFriend->cluster_property = 0;
			}
// 			else if(ind->action == 4)//������Ƭ
// 			{
// 
// 			}
			else if(ind->action == CMEMBER_INVITE_REFUSE)//�����뷽�ܾ���Ⱥ
			{
				FRIEND_INFO* pFriendInfo = GetFriendInfo(ind->member_id);
				str.Format("��%s���ܾ�����Ⱥ��%s��",/*pFriendInfo->username*/ind->nickname,ind->clustername);
			}
			else if(ind->action == CMEMBER_CREATOR_REFUSE)//Ⱥ���ܾ������뷽��Ⱥ
			{
				FRIEND_INFO* pFriendInfo = GetFriendInfo(ind->member_id);
				FRIEND_INFO* pCreator = pCluster->GetCreator();
				str.Format("��%s���ܾ���%s������Ⱥ��%s��",pCreator->GetUserName().c_str(),/*pFriendInfo->username*/ind->nickname,ind->clustername);
			}
			else
			{
				return ;
			}


			//дȺϵͳ��Ϣ
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

			//����ʾ��
			ShowUserHintDlg(str);
		}
	}
}

// ���������Ⱥ
void CMainFrame::OnClusterInviteInd(const char *data, WORD wRet)
{
	LPXT_CLUSTER_INVITE_IND ind = (LPXT_CLUSTER_INVITE_IND)data;
	char szTemp[20];
	if ( wRet==0 )
	{
		CLUSTER_INFO *pCluster = FindClusterInfo(ind->cluster_id);

		if ( ind->cluster_type==PERM_FAMILY )//����ΪȺ�ĳ�Ա
		{
// 			if ( ind->cluster_id==g_ImUserInfo.GetId() )
// 			{
				CString str; 
				//str.Format("�㱻��ΪȺ %s �ĳ�Ա��",ind->clustername );
				str.Format("��%s������������Ⱥ��%s��", FindFriend(ind->invite_id)->nickname, ind->clustername );
				CLUSTER_INFO * pCluster = FindClusterInfo(ind->cluster_id);
				if ( pCluster==NULL )
				{
					pCluster=new CLUSTER_INFO();
					pCluster->nFamilyID = ind->cluster_id;
					pCluster->nFamilyProp = PERM_FAMILY;
					strcpy(pCluster->szFamilyName, ind->clustername);
					//g_vecCluster.push_back(pCluster);					
				}
				// ������ʾ����
				ShowClusterAgreeDlg(pCluster,FindFriend(ind->invite_id),"Ⱥ��Ϣ",/*str*/ ind->clustername, m_hWnd);
				//ImGetClusterInfo(ind->cluster_id);
				//дȺϵͳ��Ϣ
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

// �����Ƿ���Ⱥ
void CMainFrame::OnClusterAgreeInd(const char *data, WORD wRet)
{
	LPXT_CLUSTER_AGREE_IND ind = (LPXT_CLUSTER_AGREE_IND)data;
	char szTemp[20];
	if ( wRet==0 )
	{
		if(ind->action == 0)//����ͬ����Ⱥ
		{
			if(g_ImUserInfo.GetId() == ind->invite_id)//�����Ⱥ��������ֱ����Ⱥ
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
			else//����Ⱥ������Ҫͨ��Ⱥ������֤
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
		else if(ind->action == 1)//���Ѳ�ͬ����Ⱥ
		{
			//Ⱥ��Ϣ֪ͨ
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
		str.Format("ȷ��Ҫ�˳���Ⱥ��");

		if (CMessageBox::Model(GetHWND(),str,IM_CAPTION,MB_YESNO|MB_ICONWARNING)!=IDYES)
		{
			return ;
		}
		// �ر�������Ի���
		g_pMessageHandleWnd->CloseClusterSendMsgDlg(pCluster->nFamilyID );
		XT_CLUSTER_APPLY_REQ req;
		req.nAction     = 1;
		req.nFamilyID   = pCluster->nFamilyID;
		req.nFamilyProp = PERM_FAMILY;
		req.nUserID     = g_ImUserInfo.GetId();
		g_pProtocol->SendReq(req);

		m_pClusterList->DelGroup((CL_GROUP_POS)pos);

		//�˳�Ⱥ��ǰѡ�е�Ⱥ���ѡ��λ�ó�ʼ��
		m_pClusterList->ClearCurSel();
		for ( int i=0; i<g_vecCluster.size(); i++)
		{
			if ( g_vecCluster[i]->nFamilyID==pCluster->nFamilyID )
			{
				CLUSTER_INFO pTemp = *(pCluster);
				g_vecClusterExit.push_back(&pTemp);/////�˳����߽�ɢ��Ⱥ

				delete g_vecCluster[i];
				g_vecCluster.erase(g_vecCluster.begin()+i);
				break;
			}
		}		
	}
}

// ��ɢȺ 
void CMainFrame::OnClusterDismiss()
{
	CL_GROUP_POS pos = (CL_GROUP_POS)m_pClusterList->GetCurSel();

	if ( pos.IsNull() )
		return ;

	CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_pClusterList->GetGroupData(pos);

	if ( pCluster )
	{
		CString str;
		str.Format("ȷ��Ҫ��ɢ��Ⱥ��");

		if (CMessageBox::Model(GetHWND(), str,IM_CAPTION,MB_YESNO)!=IDYES)
		{
			return ;
		}

		CString strCluterName = pCluster->szFamilyName;
		//CString strMsg = "���Ѿ���ɢ��Ⱥ\"" + strCluterName + "\"";
		CString strMsg;
		strMsg.Format("Ⱥ��%s��������ɢ��", strCluterName);
		//дȺϵͳ��Ϣ
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

		// �ر�������Ի���
		g_pMessageHandleWnd->CloseClusterSendMsgDlg(pCluster->nFamilyID );
	
		for ( int i=0; i<g_vecCluster.size(); i++)
		{
			if ( g_vecCluster[i]->nFamilyID==pCluster->nFamilyID )
			{ 
				CLUSTER_INFO pTemp = *pCluster;
				g_vecClusterExit.push_back(&pTemp);/////�˳����߽�ɢ��Ⱥ

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

			if ( pCluster!=NULL )//�´�����Ⱥ
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
						strMsg.Format("��%s������Ⱥ��%s��", pFriendInfo->nickname/*username*/, pCluster->szFamilyName);
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
						//����ʾ��
						ShowUserHintDlg(strMsg);
					}
					break;
				//case CMEMBER_INVITE_REFUSE:
				//	{
				//		CString str;
				//		FRIEND_INFO* pFriendInfo = GetFriendInfo(ack->nMemberID);
				//		str.Format("��%s���ܾ�����Ⱥ��%s��",pFriendInfo->username/*ind->nickname*/,pCluster->szFamilyName);
				//		//дȺϵͳ��Ϣ
				//		SYSTEM_MSG msgSys;
				//		msgSys.dwTime = time(NULL);
				//		msgSys.iType  = SYSTEM_MSG::SYSMSG_CLUSTER;
				//		sprintf(msgSys.szMessage,str);
				//		g_LocalData.AddSystemMsg(g_ImUserInfo.GetId(),msgSys);
				//		//����ʾ��
				//		ShowUserHintDlg(str);
				//	}
				//	break;
				//case CMEMBER_CREATOR_REFUSE:
				//	{
				//		CString str;
				//		FRIEND_INFO* pCreator = pCluster->GetCreator();
				//		FRIEND_INFO* pFriendInfo = GetFriendInfo(ack->nMemberID);
				//		str.Format("��%s���ܾ���%s������Ⱥ��%s��",pCreator->username,pFriendInfo->username/*ind->nickname*/,pCluster->szFamilyName);

				//		//дȺϵͳ��Ϣ
				//		SYSTEM_MSG msgSys;
				//		msgSys.dwTime = time(NULL);
				//		msgSys.iType  = SYSTEM_MSG::SYSMSG_CLUSTER;
				//		sprintf(msgSys.szMessage,str);
				//		g_LocalData.AddSystemMsg(g_ImUserInfo.GetId(),msgSys);

				//		//����ʾ��
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
		//	ShowMessage("������������������,����������ʱ����������������.���Ժ�����.!",MB_OK|MB_ICONINFORMATION);
		//else
		//if ( ack->nFamilyProp==PERM_FAMILY )
		//{
		//	ShowMessage("����Ⱥ��Աʱ����������,����������ʱ����������������.���Ժ�����.!",MB_OK|MB_ICONINFORMATION);
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
			// �����Ⱥ��ȫ��Ⱥ�в�����,��ɾ��֮
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
		// ����б��в����ڴ���ɢ���飬�򷵻�
		if(pCluster==NULL)
		{
			return -1;
		}
		// �ر�������Ի���
		g_pMessageHandleWnd->CloseClusterSendMsgDlg(pCluster->nFamilyID );

		// ���ȫ�����б��еĸ���
		for ( int i=0; i<g_vecCluster.size(); i++)
		{
			if ( g_vecCluster[i]->nFamilyID==pCluster->nFamilyID )
			{
				CLUSTER_INFO pTemp = *(pCluster);
				g_vecClusterExit.push_back(&pTemp);/////�˳����߽�ɢ��Ⱥ

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
		// ������б���û�У���Ϊ�������ݣ���Ҫɾ��
		if(i == c)
		{
			oldMembers.push_back(((FRIEND_INFO*)(&(*it)))->GetId());
		}
	}
}

void CMainFrame::OnBnClickedButtonCreate()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}


//////������������ȡȺ��Ƭ��Ϣ
void CMainFrame::OnGetCMProfileAck(const char *data, WORD wRet)
{
	//CL_GROUP_POS posGroup = m_pClusterList->FindCluster(ack->nFamilyID);
	//if ( posGroup.IsNull()==false )
	//{
	//	CLUSTER_INFO *pCluster = (CLUSTER_INFO *)m_pClusterList->GetGroupData(posGroup);
	//	LPXT_PROFILENAME_GET_ACK ack = (LPXT_PROFILENAME_GET_ACK)data;
	//	if (wRet == 0 && ack->nFamilyID == pCluster->nFamilyID)
	//	{
	//		//CLUSTER_INFO * pCluster = FindClusterInfo(m_clusterInfo.nFamilyID);/////��ȡȺ��Ϣ
	//		for ( int i=0; i<ack->nCount; i++ )
	//		{
	//			XT_CMEM_IFNO   &info = ack->MemInfo[i];
	//			FRIEND_INFO *pUser = pCluster->GetFriendInfo(info.nMemID);
	//			if (pUser)/////��Ⱥ�к��иó�Ա��Ϣ
	//			{
	//				strcpy(pUser->profile_name,info.ProfileName);/////��ֵȺ��Ƭ
	//			}
	//		}
	//		this->m_wndClusterPanel.UpdateClusterProfile();  //����Ⱥ��Ƭ��ʾ
	//	}

	//}
}

// ���������Ա�б�����
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
				// �жϸ��û��Ƿ����Լ��ĺ��ѣ��Ա�Ӻ����б����л�ȡ��������
				// ����ӷ��������
				FRIEND_INFO * pCache = GetFriendInfo(id);
				FRIEND_INFO *pFriend = pCluster->GetFriendInfo(id);
				if ( pFriend == NULL )
				{
					member.SetId(ack->members[i].id);
					member.status = ack->members[i].status;
					// ��ʱʹ��id���ǳ�
					itoa(member.GetId(),member.nickname,10);
					pCluster->members.push_back(member);
					pFriend = &pCluster->members[pCluster->Count()-1];

					if ( pCache==NULL )
					{
						// �ú����ڻ����в�����
						bExist = false;
					}
					else
					{
						// ����Ⱥ��Ƭ����
						string strProfileName = pFriend->profile_name;
						*pFriend = *pCache;
						strncpy(pFriend->profile_name, strProfileName.c_str(), 10);
					}
				}
				else if(pCache)
				{
					// ����Ⱥ��Ƭ����
					string strProfileName = pFriend->profile_name;
					*pFriend = *pCache;
					strncpy(pFriend->profile_name, strProfileName.c_str(), 10);
				}
				// �����ڻ����д���
				if( bExist )
				{
					if( pFriend->bBasicInfo )
					{
						bNeedUpdate = false;	// �Ѿ����ڸú��ѵĻ������ϲ���Ҫ����
					}
				}

				if(bNeedUpdate)
				{
					// �����Ҫ�ӷ�������ȡ������Ϣ�����ȱ��浽�б�
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
				req.online_type = 0;//����
				req.userstatus  = g_ImUserInfo.status;
				g_pProtocol->SendReq(req);
			}
			else
			{
				int i;
				int mid,cid;
				vector<uint32> oldMembers;
				// Ⱥ��Ա�б��ȡ���
				if ( pCluster->bRefreshFlag )
				{
					pCluster->bRefreshFlag=false ;
				}
				// �ӷ�������ȡ���ѻ�������
				m_arrFriendsID_Basic = m_arrNeedGetBasicInfo;
				ImGetFriendBasicInfo();
				// ɾ����ʱ��Ⱥ��Ա
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

// ��ֹȺ��Ϣ������ֹ
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