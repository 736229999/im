// im\ClusterList.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\clusterpanel.h"
#include "MessageHandleWnd.h"
#include "ClustersettingDlg.h"
#include "..\EditNoticeDlg.h"
//#include "..\..\..\..\im_client\HmgUploadPicture\stdafx.h"
// CClusterPanel �Ի���

#define TIMER_UPDATE_CLUSTER 0xee		// ����Ⱥ��Ϣ		

HWND g_hWndClusterPanel = NULL;
IMPLEMENT_DYNAMIC(CClusterPanel, CDialog)
IMPLEMENT_HMG_SCROLLBAR(CClusterPanel)

CClusterPanel::CClusterPanel(CLUSTER_INFO * pCluster, CWnd* pParent)
	: CDialog(CClusterPanel::IDD, pParent)
{
	m_nOnline = 0;
	m_pCluster           = pCluster;
	m_pBitmapFamilyFlag  = NULL;
	m_bmpDefaultFamilyFlag.LoadBitmap(IDB_FAMILY_HEAD);
	m_wndList.SetClusterInfo(m_pCluster);
	m_NoticeFont.fontSize = 10;
	m_NoticeFont.fontColor = RGB(95, 95, 95);
	m_pEditNoticeDlg  = NULL;
}

CClusterPanel::~CClusterPanel()
{
	m_pCluster = NULL;
	if(m_pEditNoticeDlg != NULL)
	{
		delete m_pEditNoticeDlg;
		m_pEditNoticeDlg = NULL;
	}
}

void CClusterPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CLUSTER, m_wndList);
	DDX_Control(pDX, IDC_STATIC_INFO,m_wndInfo);
	DDX_Control(pDX, IDC_EDIT_INTRO, m_edtIntro);
	DDX_Control(pDX, IDC_STATIC_NOTICE, m_btnNotice);
	DDX_Control(pDX, IDC_BTN_ADD, m_btnAddMember);
	DDX_Control(pDX, IDC_BTN_SEARCH, m_btnSearchMember);
	DDX_Control(pDX, IDC_EDIT_SEARCH, m_edtSearch);
}


BEGIN_MESSAGE_MAP(CClusterPanel, CDialog)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_LBN_DBLCLK(IDC_LIST_CLUSTER, OnLbnDblclkListCluster)
	SET_HMG_SCROLLBAR(IDC_LIST_CLUSTER)
	SET_HMG_SCROLLBAR(IDC_EDIT_INTRO)
	ON_WM_ERASEBKGND()
	ON_STN_CLICKED(IDC_STATIC_SETTING, OnStnClickedStaticSetting)
	ON_LBN_SELCHANGE(IDC_LIST_CLUSTER, OnLbnSelchangeListCluster)
	ON_WM_TIMER()
	//ON_STN_CLICKED(IDC_STATIC_REFRESH, OnStnClickedStaticRefresh)
	ON_XT_MESSAGE()
	ON_STN_CLICKED(IDC_STATIC_NOTICE, &CClusterPanel::OnStnClickedStaticNotice)
	ON_BN_CLICKED(IDC_BTN_ADD, &CClusterPanel::OnBnClickedBtnAdd)
	ON_BN_CLICKED(IDC_BTN_SEARCH, &CClusterPanel::OnBnClickedBtnSearch)
	ON_EN_SETFOCUS(IDC_EDIT_SEARCH, &CClusterPanel::OnEnSetfocusEditSearch)
	ON_EN_KILLFOCUS(IDC_EDIT_SEARCH, &CClusterPanel::OnEnKillfocusEditSearch)
	ON_MESSAGE(EDIT_NOTICE_SELF_MSG, OnEditNoticeMsg)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CClusterPanel)
	ON_XT_COMMAND(CMD_SET_CMEMBERS_ACK,OnSetCMemberAck)
	ON_XT_COMMAND(CMD_SET_CINFO_ACK,OnSetClusterInfoAck)
	ON_XT_COMMAND(CMD_GET_CINFO_ACK,OnClusterInfoAck)
	//ON_XT_COMMAND(CMD_STATUS_MOD_ACK,OnStatusModAck)
	// ����״̬�ı�֪ͨ
	ON_XT_COMMAND(CMD_FRIEND_STATUS_IND,OnFriendStatusChange)

	//Ⱥ��Ƭ�޸ĺ������������Ϣ
	ON_XT_COMMAND(CMD_SET_PROFILE_NAME_ACK, OnGetCMProfileAck)

	////////Ⱥ��Ϣ���շ�ʽ����
	//ON_XT_COMMAND(CMD_SET_CLUSTERMSG_RECV_ACK, OnSetClusterMsgRecAck)
	
END_XT_MESSAGE_MAP()

// CClusterPanel ��Ϣ�������

BOOL CClusterPanel::OnInitDialog()
{
	CDialog::OnInitDialog();
	g_hWndClusterPanel = GetSafeHwnd();

	m_wndInfo.SetTextColor(RGB(45,45,45));
	m_wndInfo.SetFont(&g_font12);

	m_btnNotice.SetNormalColor(RGB(88,163,210));
	m_btnNotice.SetLinkColor(RGB(88,163,210));
	m_btnNotice.SetFont(&g_font12B);
	
	m_btnAdd.SetNormalColor(RGB(0,0,255));
	m_btnAdd.SetLinkColor(RGB(200,100,122));
	
	this->m_wndList.SetBorder(false);

	m_btnSearchMember.SetPng(IDB_PNG_SEARCH_HOVER, IDB_PNG_SEARCH_NORMAL, IDB_PNG_SEARCH_PRESSED, IDB_PNG_SEARCH_DISABLE);
	m_btnAddMember.SetPng(IDB_PNG_ADD_GROUP_HOVER, IDB_PNG_ADD_GROUP_NORMAL, IDB_PNG_ADD_GROUP_PRESSED, IDB_PNG_ADD_GROUP_DISABLE);

	//������ϵ��
	m_edtSearch.SetLimitText(20);
	m_edtSearch.SetWindowText("����Ⱥ��Ա");
	m_edtSearch.SetTextColor(RGB(255,255,255));
	m_edtSearch.SetFont(&g_font12);///�޸�����g_font12
	m_edtSearch.SetBackColor(RGB(139,139,139));
	m_edtSearch.SetBorderColor(RGB(139,139,139));

	AttachHMGScrollbar(this->m_wndList.m_hWnd);
	AttachHMGScrollbar(this->m_edtIntro.m_hWnd);

	g_pProtocol->RegistCmd(CMD_SET_CINFO_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SET_CMEMBERS_ACK,m_hWnd);	
	g_pProtocol->RegistCmd(CMD_GET_CINFO_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SERVERMSG_IND,m_hWnd);
    g_pProtocol->RegistCmd(CMD_STATUS_MOD_ACK,m_hWnd);
    g_pProtocol->RegistCmd(CMD_SET_PROFILE_NAME_ACK,m_hWnd);

    g_pProtocol->RegistCmd(CMD_FRIEND_STATUS_IND,m_hWnd);
	//UpdateClusterList();

	//////��ȡ�б�λ��/////////////////
	CRect rc;
	CWnd* pItem = GetDlgItem(IDC_LIST_CLUSTER);/////�б��
	pItem->GetWindowRect(&rc);

	//////�����������/////////////////
	////////////
	m_edtSearch.SetFont(&g_font13);
	m_wndTabSearch.Create(CTabWndSearch::IDD,this);
	m_wndTabSearch.MoveWindow(&rc);

	m_wndTabSearch.ShowWindow(SW_HIDE);
	m_wndTabSearch.ShowTempSessionBtn(FALSE);
	m_wndList.ShowWindow(SW_SHOW);
	m_wndList.SetFont(&g_font12);

	GetDlgItem(IDC_STATIC_NOTICE_TEXT)->SetFont(&g_font12B);

	// �����ǰ�û�Ϊ�ǹ���Ա����ֹ�༭����
	if ( m_pCluster->nCreatorID != g_ImUserInfo.GetId() )
	{
		m_btnNotice.EnableWindow(FALSE);
	}

	// ����Ⱥ��Ϣ
	SetTimer(TIMER_UPDATE_CLUSTER, 50, NULL);

	return TRUE;  
}

void CClusterPanel::OnSetCMemberAck(const char *data,WORD wRet)
{
	KillTimer(0x01);////�رն�ʱ����ʾ
	LPXT_SET_CMEMBERS_ACK ack = (LPXT_SET_CMEMBERS_ACK)data;

	if (wRet == 0 && ack->nFamilyID == this->m_pCluster->nFamilyID && ack->nFamilyProp == PERM_FAMILY)
	{
		switch (ack->nAction)
		{
		case CMEMBER_SET_ADMIN:
			if (m_pCluster)
			{
				FRIEND_INFO *pUser = m_pCluster->GetFriendInfo(ack->nMemberID);
				if (pUser)
				{
					pUser->cluster_property = CMEMBER_ADMIN;
					UpdateClusterList();
					//this->BuildCluster(ack->nFamilyID);
				}
			}
			break;
		case CMEMBER_CANCEL_ADMIN:
			if (m_pCluster)
			{
				FRIEND_INFO *pUser = m_pCluster->GetFriendInfo(ack->nMemberID);
				if (pUser)
				{
					pUser->cluster_property = CMEMBER_NORMAL;
					UpdateClusterList();
					//this->BuildCluster(ack->nFamilyID);
				}
			}
			break;
		case CMEMBER_ADD:
			//ImGetClusterMember(ack->nFamilyID);
			break;

		case CMEMBER_DEL:    //ɾ����Ա
		{
			RegUserOperation(0x00b2);
			int  nItemCnt = m_wndList.GetCount();
			for (int i = 0; i < nItemCnt; i++)
			{
				FRIEND_INFO *pUser = (FRIEND_INFO *)m_wndList.GetItemData(i);

				if (pUser->GetId() == ack->nMemberID)
				{
					m_pCluster->DelMember(ack->nMemberID);

					m_wndList.ResetContent();

					for (int i = 0; i < m_pCluster->Count(); i++)
					{
						FRIEND_INFO *pFriend = &m_pCluster->members[i];

						int nItem = m_wndList.AddString(UserInfoToStr(pFriend));
						m_wndList.SetItemData(nItem, (DWORD)pFriend);
					}

					CString str;
					str.Format(" (%d/%d)", m_nOnline, m_pCluster->Count());
					m_wndInfo.SetWindowText("Ⱥ��Ա:", str);
					m_wndList.SortClusterList();
				}
			}
		}
		break;
		}
	}
}

void CClusterPanel::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	
	if ( m_wndList.m_hWnd )
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		CRect rcNotice;
		GetDlgItem(IDC_STATIC_NOTICE_TEXT)->GetWindowRect(&rcNotice);
		ScreenToClient(&rcNotice);
		CRect rcIntro;
		m_edtIntro.GetWindowRect(&rcIntro);
		ScreenToClient(&rcIntro);
		rcIntro.top = rcNotice.bottom + 1;
		rcIntro.left = rcClient.left + 5;
		rcIntro.right = rcClient.right - 1;
		m_edtIntro.MoveWindow(rcIntro);

		CRect rcList;
		m_wndList.GetWindowRect(&rcList);
		ScreenToClient(&rcList);
		rcList.left = rcClient.left + 1;
		rcList.right = rcClient.right - 1;
		rcList.bottom = rcClient.bottom - 1;
		m_wndList.MoveWindow(&rcList);
	}
}

void CClusterPanel::OnPaint()
{
	CPaintDC dc(this); 

	CRect rcClient;
	GetClientRect(&rcClient);

	int nSave = dc.SaveDC();

	// �����ϣ��ң��±߿�Ϊ��ɫ
	CPen penDrawTRB(PS_SOLID, 1, RGB(255, 255, 255));
	CPen *pOldPen = dc.SelectObject(&penDrawTRB);
	dc.MoveTo(0, 0);
	dc.LineTo(rcClient.right, 0);
	dc.LineTo(rcClient.right, rcClient.bottom);
	dc.LineTo(0, rcClient.bottom);
	dc.SelectObject(pOldPen);
	// ������߿���ɫ
	CPen penDrawL(PS_SOLID,1,CHAT_BKG_COLOR);
	pOldPen = dc.SelectObject(&penDrawL);
	dc.MoveTo(0, 0);
	dc.LineTo(0, rcClient.bottom);
	dc.SelectObject(pOldPen);

	// ��ť��
	CRect rcBtn;
	m_btnAddMember.GetWindowRect(&rcBtn);
	ScreenToClient(&rcBtn);
	CRect rcBottomBar;
	rcBottomBar.left = 1;
	rcBottomBar.right = rcClient.right - 1;
	rcBottomBar.top = rcBtn.top - 4;
	rcBottomBar.bottom = rcBtn.bottom + 4;
	dc.FillSolidRect(&rcBottomBar, RGB(233, 233, 233));

	dc.RestoreDC(nSave);
}

HBRUSH CClusterPanel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		if ( pWnd->GetDlgCtrlID()!=IDC_EDIT_BULITIN )
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr = g_hWhiteBrush;
		}
	}

	return hbr;
}

void CClusterPanel::OnLbnDblclkListCluster()
{
#if 0
	int nSel = m_wndList.GetCurSel();

	if ( nSel>-1 && nSel<m_wndList.GetCount() )
	{
		FRIEND_INFO *pInfo =(FRIEND_INFO *)m_wndList.GetItemData(nSel);

		if ( pInfo->id==g_ImUserInfo.GetId() )
		{
			ShowBlogSite(pInfo->username);
		}
		else
		{
			FRIEND_INFO *pInfo =(FRIEND_INFO *)m_wndList.GetItemData(nSel);

			if ( pInfo->id!=g_ImUserInfo.GetId() )
			{
				GetParent()->SendMessage(WM_THREAD_NOTIFY,2,(LPARAM)pInfo);
			}

			//CP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pInfo);

			//if ( pDlg->m_hWnd==NULL )
			//	pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());

			//if ( pDlg->IsWindowVisible()==FALSE )
			//	pDlg->ShowWindow(SW_SHOW);

			//if ( pDlg->IsIconic() )
			//	pDlg->ShowWindow(SW_RESTORE);

			//pDlg->BringWindowToTop();

			//GetParent()->SendMessage(WM_THREAD_NOTIFY,0,(LPARAM)pInfo);
		}
	}
#endif
}

BOOL CClusterPanel::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message==WM_KEYDOWN )
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			{
				if (GetDlgItem(IDC_EDIT_SEARCH) == GetFocus())
				{
					OnBnClickedBtnSearch();
				}
			}
			return TRUE;
		case VK_ESCAPE:
			return TRUE;
		default:
			break;
		}
	}
	 return CDialog::PreTranslateMessage(pMsg);
}



void CClusterPanel::UpdateClusterList()
{
	ASSERT(m_pCluster);
	CLUSTER_INFO *pRealCluster = FindClusterInfo(m_pCluster->nFamilyID);
	*m_pCluster = *pRealCluster;
	m_pBitmapFamilyFlag = GetClusterBitmap(m_pCluster,false);

	KillTimer(0x01);
	SetTimer(0x01,50,NULL);
}

void CClusterPanel::UpdateClusterProfile(CLUSTER_INFO* pClusterInfo)
{
	ASSERT(pClusterInfo);
	m_pCluster = pClusterInfo;
	m_pBitmapFamilyFlag = GetClusterBitmap(m_pCluster,false);

	KillTimer(0x02);
	SetTimer(0x02,50,NULL);
}

BOOL CClusterPanel::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(rc,RGB(255, 255, 255));

	return FALSE;
}

void CClusterPanel::OnStnClickedStaticSetting()
{
	ASSERT(m_pCluster);
	if(m_pCluster)
	{
		CLUSTER_INFO * pCluster = FindClusterInfo(m_pCluster->nFamilyID);
		ASSERT(pCluster);
		if(pCluster)
		{
			CClusterSettingDlg::Show(pCluster);
		}
	}	
}

void CClusterPanel::OnLbnSelchangeListCluster()
{
#if 0
	int nSel = m_wndList.GetCurSel();

	if ( nSel>-1 && nSel<m_wndList.GetCount() )
	{
		FRIEND_INFO *pInfo =(FRIEND_INFO *)m_wndList.GetItemData(nSel);

		if ( pInfo->id!=g_ImUserInfo.GetId() )
		{
			GetParent()->SendMessage(WM_THREAD_NOTIFY,1,(LPARAM)pInfo);
		}
	}
#endif
}

void CClusterPanel::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		KillTimer(0x01);
		m_wndList.SetClusterInfo(m_pCluster);

		if( strcmp(m_pCluster->szFamilyNotice, "") == 0 )
		{
			m_edtIntro.SetXFont(m_NoticeFont);
			m_edtIntro.SetText("������Ⱥ���棡");
		}
		else
		{
			m_NoticeFont.fontColor = RGB(0, 0, 0);
			m_edtIntro.SetXFont(m_NoticeFont);
			m_edtIntro.SetWindowText(m_pCluster->szFamilyNotice);///////Ⱥ����
		}

		m_wndList.ResetContent();
		///////Ⱥ��Ƭ����///////////////////////////////////////////////
		m_nOnline = 0;
		for ( int i=0; i < m_pCluster->Count(); i++ )
		{
			FRIEND_INFO *pFriend = &m_pCluster->members[i];

			if ( pFriend->status!=XTREAM_OFFLINE )
			{
				m_nOnline++;
			}
			FRIEND_INFO* pGroupFriend = GetFriendInfo(pFriend->GetId());
			if(pGroupFriend != NULL && pGroupFriend->comment_name != "")
			{
				strncpy(pFriend->comment_name,pGroupFriend->comment_name,sizeof(pGroupFriend->comment_name)-1);
			}
			CString strTxt = CMemberInfoToStr(pFriend);
			int nItem = m_wndList.AddString(strTxt);
			m_wndList.SetItemData(nItem,(DWORD)pFriend);
		}
	    m_wndList.SortClusterList();/////ˢ�����߳�Ա��ʾ
		CString str;
		str.Format(" (%d/%d)",m_nOnline,m_pCluster->Count());
		m_wndInfo.SetWindowText("Ⱥ��Ա:", str );
		//m_wndList.SortClusterList();
	}

	if(TIMER_UPDATE_CLUSTER == nIDEvent)
	{	// ����Ⱥ��Ϣ
		KillTimer(nIDEvent);

		// �����������Ⱥ��Ϣ
		XT_GET_CINFO_REQ req;
		req.nFamilyID = m_pCluster->nFamilyID;
		req.nFamilyProp = PERM_FAMILY;
		g_pProtocol->SendReq(req);
	}
    
	if ( nIDEvent==0x02 )//////ֻ�޸�Ⱥ��Ƭ��Ϣ
	{
		KillTimer(0x02);
		m_wndList.SetClusterInfo(m_pCluster);
		m_wndList.ResetContent();
		for ( int i=0; i < m_pCluster->Count(); i++ )
		{
			FRIEND_INFO *pFriend = &m_pCluster->members[i];
			
			FRIEND_INFO* pGroupFriend = NULL;
			pGroupFriend = GetFriendInfo(pFriend->GetId());
			if(pGroupFriend != NULL && pGroupFriend->comment_name != "")
			{
				strncpy(pFriend->comment_name,pGroupFriend->comment_name,sizeof(pGroupFriend->comment_name)-1);
			}
			CString strTxt = CMemberInfoToStr(pFriend);
			int nItem = m_wndList.AddString(strTxt);
			m_wndList.SetItemData(nItem,(DWORD)pFriend);
		}
		m_wndList.SortClusterList();/////ˢ�����߳�Ա��ʾ
	}
	CDialog::OnTimer(nIDEvent);
}

void CClusterPanel::InvalidateFamilyHead()
{
#if 0
	if ( m_hWnd && m_wndFlag.m_hWnd )
	{
		CRect rc;
		m_wndFlag.GetWindowRect(&rc);
		ScreenToClient(&rc);
		InvalidateRect(&rc);
	}
#endif
}

void CClusterPanel::SelUser(int uid)
{
	int nPos = m_wndList.FindItem(uid);
	if(nPos < 0)
		return;
	m_wndList.SetCurSel(nPos);
}

void CClusterPanel::ShowUserMenu(int uid)
{
	int nPos = m_wndList.FindItem(uid);
	if(nPos < 0)
		return;
	m_wndList.SetCurSel(nPos);
	m_wndList.PopMenu(nPos);
	
	
}
void CClusterPanel::OnRefresh()
{
	ASSERT(m_pCluster);
	if(m_pCluster)
	{
		CLUSTER_INFO * pCluster = FindClusterInfo(m_pCluster->nFamilyID);
		ASSERT(pCluster);
		
		if(pCluster)
		{
			XT_GET_CINFO_REQ req;
			req.nFamilyID = pCluster->nFamilyID;
			req.nFamilyProp = PERM_FAMILY;	
		
			pCluster->bRefreshFlag = true;
			pCluster->dwTickGet    = 0;

			g_pProtocol->SendReq(req);
		}
	}	

}
void CClusterPanel::OnStnClickedStaticNotice()
{
	CString strText;
	m_btnNotice.GetWindowText(strText);
	if(strText == "�༭����")
	{
		m_btnNotice.SetWindowText("ȡ���༭");
		if(m_pEditNoticeDlg != NULL)
		{
			delete m_pEditNoticeDlg;
			m_pEditNoticeDlg = NULL;
		}

		// �������Ĵ���λ��
		int x, y; 
		x = GetSystemMetrics(SM_CXSCREEN); //��Ļ��� 
		y = GetSystemMetrics(SM_CYSCREEN); //��Ļ�߶�  

		CRect rcParent;
		GetParent()->GetWindowRect(&rcParent);

		CPoint pt;
		if(rcParent.right + 250 < x)
		{
			pt.x = rcParent.right + 2;
			pt.y = rcParent.top;
		}
		else if(rcParent.left > 250)
		{
			pt.x = rcParent.left - 250;
			pt.y = rcParent.top;
		}
		else
		{
			pt.x = rcParent.right - 250;
			pt.y = rcParent.top;
		}

		CLUSTER_INFO *pRealCluster = FindClusterInfo(m_pCluster->nFamilyID);
		ASSERT(pRealCluster != NULL);
		m_pEditNoticeDlg = new CEditNoticeDlg(this, pRealCluster->szFamilyNotice, pt);
		m_pEditNoticeDlg->Create();
	}
	else
	{
		m_btnNotice.SetWindowText("�༭����");
		if(m_pEditNoticeDlg != NULL)
		{
			delete m_pEditNoticeDlg;
			m_pEditNoticeDlg = NULL;
		}
	}
}

void CClusterPanel::OnBnClickedBtnAdd()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CClusterMemberSelectDlg dlg(m_pCluster,this);
	if ( IDOK==dlg.DoModal() )
	{
		dlg.GetClusterInfo(*m_pCluster);
		//UpdateClusterInfo();
	}
	else
	{
		return ;
	}
}

void CClusterPanel::OnBnClickedBtnSearch()
{
	GetDlgItem(IDC_BTN_SEARCH)->SetFocus();
	CString strItem = _T("");
	((CEdit *)GetDlgItem(IDC_EDIT_SEARCH))->GetWindowText(strItem);
	m_wndList.ShowWindow(SW_HIDE);
	m_wndTabSearch.ShowWindow(SW_SHOW);
	m_wndTabSearch.UpdateSearchList(m_pCluster,strItem);

}

void CClusterPanel::UpdateNotice(LPCTSTR szNotice)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	/////////////////////////////////////////////////////////////////////////////
	XT_SET_CINFO_REQ    m_cInfoReq;
	strncpy_s(m_cInfoReq.szFamilyNotice, szNotice, MAX_FAMILYNAME_LEN);
	//strcpy(m_cInfoReq.szFamilyNotice,szNotice);/////Ⱥ��������

	m_cInfoReq.nFamilyProp = PERM_FAMILY;

	CLUSTER_INFO * pCluster = FindClusterInfo(m_pCluster->nFamilyID);
	if(pCluster)
	{
		m_cInfoReq.nFamilyID     =  pCluster->nFamilyID;////ȺID
		m_cInfoReq.nCreatorID    =  pCluster->nCreatorID;////������ID
		m_cInfoReq.nJoinFlag    =  pCluster->nJoinFlag;////��Ⱥ����
		strcpy(m_cInfoReq.szFamilyName,pCluster->szFamilyName);///Ⱥ����
		strcpy(m_cInfoReq.szFamilyDesc,pCluster->szFamilyDesc);///Ⱥ����
		strcpy(m_cInfoReq.szFamilySign,pCluster->szFamilySign);///Ⱥǩ��
		////strcpy(m_cInfoReq.szFamilyFaceUrl,pCluster->szFamilyFaceUrl);///Ⱥ������Ƭ
		strcpy(pCluster->szFamilyNotice,m_cInfoReq.szFamilyNotice);///Ⱥ������ʾ
	}
	extern CIMProtocol * g_pProtocol;
	g_pProtocol->SendReq(m_cInfoReq);
	////////////////////////////////////////////////////////////////////
}


void CClusterPanel::OnEnSetfocusEditSearch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_edtSearch.SetBackColor(RGB(0XFF,0XFF,0XFF));
	m_edtSearch.SetWindowText((_T("")));
	m_edtSearch.SetTextColor(RGB(0, 0, 0));
	m_wndList.ShowWindow(SW_SHOW);
	m_wndTabSearch.ShowWindow(SW_HIDE);
}


void CClusterPanel::OnEnKillfocusEditSearch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	m_edtSearch.SetBackColor(RGB(139,139,139));
	m_edtSearch.SetTextColor(RGB(255, 255, 255));
	CString strTxt = _T("");
	m_edtSearch.GetWindowText(strTxt);
	strTxt.Trim();//ȥ���ո�
	if(strTxt == _T(""))
	{
		m_edtSearch.SetWindowText(_T("����Ⱥ��Ա"));
		m_edtSearch.SetSel(0,-1);
	}
}

// �༭�����Զ�����Ϣ
LRESULT CClusterPanel::OnEditNoticeMsg(WPARAM wp, LPARAM lp)
{
	int nLP = (int)wp;
	if(nLP == IDOK)
	{	// ��ȷ��
		m_btnNotice.SetWindowText("�༭����");

		string strNotice = m_pEditNoticeDlg->GetText();
		UpdateNotice(strNotice.c_str());

		delete m_pEditNoticeDlg;
		m_pEditNoticeDlg = NULL;
	}

	if(nLP == IDCANCEL)
	{	// ��ȷ��
		m_btnNotice.SetWindowText("�༭����");
		delete m_pEditNoticeDlg;
		m_pEditNoticeDlg = NULL;
	}

	return 0;
}

// ����Ⱥ��������Ӧ��Ϣ
void CClusterPanel::OnSetClusterInfoAck(const char *data,WORD wRet)
{
	LPXT_SET_CINFO_ACK ack = (LPXT_SET_CINFO_ACK)data;

	if ( wRet==0 )
	{
		if(ack->nFamilyID == m_pCluster->nFamilyID)
		{
			// ����Ⱥ��Ϣ
			SetTimer(TIMER_UPDATE_CLUSTER, 50, NULL);
		}
		/*CLUSTER_INFO *pCluster = FindClusterInfo(ack->nFamilyID);

		if ( pCluster != NULL )
		{
		m_edtIntro.SetText(pCluster->szFamilyNotice);
		}*/
	}	
}

// ��ȡȺ��Ϣ
void CClusterPanel::OnClusterInfoAck(const char *data,WORD wRet)
{
	LPXT_GET_CINFO_ACK ack = (LPXT_GET_CINFO_ACK)data;
	if ( wRet==0 )
	{
		UpdateClusterList();
	}	
}

// ����ϵͳ��Ϣ
void CClusterPanel::OnRecvSvrMsg(const char *data,WORD wRet)
{
	LPXT_SERVERMSG sysmsg = (LPXT_SERVERMSG)data;
	
	if ( wRet==0 )
	{	
		if ( sysmsg->type==CMD_SET_CINFO_REQ )
		{
			// ����Ⱥ��Ϣ
			SetTimer(TIMER_UPDATE_CLUSTER, 50, NULL);
		}
	}
}

// ��������
void CClusterPanel::OnDestroy()
{
	g_pProtocol->UnRegistCmd(m_hWnd);
	CDialog::OnDestroy();
}

////�û�״̬�ı�ʱ����Ϣ
void CClusterPanel::OnFriendStatusChange(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_STATUS_IND ind =(LPXT_STATUS_IND)data;
		//���������Ļ�����״̬ͼ����뿪һ��
		if ( ind->status==XTREAM_HIDE )
		{
			ind->status=XTREAM_OFFLINE;
		}

		FRIEND_INFO *pFriend = m_pCluster->GetFriendInfo(ind->fid);
		UpdateClusterList();
	}
}

////�޸�Ⱥ��Ƭ��Ⱥ�Ի����г�Ա������Ӧ�仯
void CClusterPanel::OnGetCMProfileAck(const char *data, WORD wRet)
{

 //   m_wndList.ResetContent();
	//LPXT_PROFILENAME_SET_ACK ack = (LPXT_PROFILENAME_SET_ACK)data;
	//if (wRet == 0 && ack->nFamilyID == this->m_pCluster->nFamilyID)
	//{
	//	//RegUserOperation(0x00b1);
	//	int nMemberCnt = m_pCluster->Count();
	//	for (int j = 0; j<nMemberCnt; j++)
	//	{
	//		FRIEND_INFO *pUser = &(m_pCluster->members[j]);
	//		if (pUser->id == ack->nMemID)//////����Ⱥ��ԱID
	//		{
	//			strcpy(pUser->profile_name,ack->ProfileName);
	//		}
	//		CString strTxt = CMemberInfoToStr(pUser);
	//		int nItem = m_wndList.AddString(strTxt);
	//		m_wndList.SetItemData(nItem,(DWORD)pUser);
	//	}
	//}
}

///////Ⱥ��Ϣ�������ú󷵻���Ϣ
void CClusterPanel::OnSetClusterMsgRecAck(const char *data, WORD wRet)
{
	LPXT_CLUSTERMSG_RECV_SET_ACK ack = (LPXT_CLUSTERMSG_RECV_SET_ACK)data;
	if (wRet == 0 && ack->nFamilyID == this->m_pCluster->nFamilyID)
	{
		int nMemberCnt = m_pCluster->Count();
		for (int j = 0; j<nMemberCnt; j++)
		{
			FRIEND_INFO *pUser = &(m_pCluster->members[j]);
			if (pUser->GetId() == ack->nMemID)//////����Ⱥ��ԱID
			{
			   UpdateClusterList();
			}
		}
	}
}

// �ƶ�����༭����
void CClusterPanel::MoveNoticeDlg()
{
	if(m_pEditNoticeDlg != NULL && ::IsWindow(m_pEditNoticeDlg->GetSafeHwnd()))
	{
		// �������Ĵ���λ��
		int x, y; 
		x = GetSystemMetrics(SM_CXSCREEN); //��Ļ��� 
		y = GetSystemMetrics(SM_CYSCREEN); //��Ļ�߶�  

		CRect rcParent;
		GetParent()->GetWindowRect(&rcParent);

		CPoint pt;
		if(rcParent.right + 250 < x)
		{
			pt.x = rcParent.right + 2;
			pt.y = rcParent.top;
		}
		else if(rcParent.left > 250)
		{
			pt.x = rcParent.left - 250;
			pt.y = rcParent.top;
		}
		else
		{
			pt.x = rcParent.right - 250;
			pt.y = rcParent.top;
		}

		CRect rcNoticeDlg;
		m_pEditNoticeDlg->GetWindowRect(&rcNoticeDlg);
		m_pEditNoticeDlg->MoveWindow(pt.x, pt.y, rcNoticeDlg.Width(), rcNoticeDlg.Height());
	}
}