// im\ClusterInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\clustersettingdlg.h"
#include "FamilyFaceDlg.h"
#include "P2PInviteDlg.h"
#include "localdata.h"
#include "ClusterPage2.h"
#include "..\MainFrame.h"
// CClusterSettingDlg 对话框
#define TIMER_CHECK					0x01
#define TIMERID_REFRESH_DISABLE		0x02
#define TIMER_UPDATE_CLUSTER        0x03

CClusterSettingDlg * CClusterSettingDlg::pInstance = NULL;

IMPLEMENT_DYNAMIC(CClusterSettingDlg, CXSkinDlg)
IMPLEMENT_HMG_SCROLLBAR(CClusterSettingDlg)

CClusterSettingDlg::CClusterSettingDlg(CLUSTER_INFO * pInfo,CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CClusterSettingDlg::IDD, pParent)
{
	m_hBrushControl  = CreateSolidBrush(RGB(255,255,255));
	m_clusterInfo    = *pInfo;
	m_pProgress		 = NULL;
	m_pBmpFamilyHead = NULL;
	m_bmpDefaultFamilyHead.LoadBitmap(IDB_FAMILY_HEAD);

	Create(IDD,pParent);
}

CClusterSettingDlg::~CClusterSettingDlg()
{
	DeleteObject(m_hBrushControl);
	CloseProgress();

	pInstance = NULL;
}

void CClusterSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_ID, m_edtID);
	DDX_Control(pDX, IDC_EDIT_NAME, m_edtName);
	DDX_Control(pDX, IDC_EDIT_CREATOR, m_edtCreator);
	DDX_Control(pDX, IDC_EDIT_INTRO, m_edtIntro);
	DDX_Control(pDX, IDC_LIST_CLUSTER, m_lstCluster);
	DDX_Control(pDX, IDC_SETMEMBERS, m_btnSetmembers);
	DDX_Control(pDX, IDC_BTN_ADDFRIEND, m_btnAddfriend);
	DDX_Control(pDX, IDC_BTN_DELMEMBER, m_btnDelmember);
	DDX_Control(pDX, IDC_BTN_SETMANAGE, m_btnSetmanage);
	DDX_Control(pDX, IDC_BTN_DELMANAGE, m_btnDelmanage);
	DDX_Control(pDX, IDC_OK, m_btnOk);
	DDX_Control(pDX, IDC_ADVANCED_SETTING, m_btnAdvancedSetting);
	DDX_Control(pDX, IDC_CLOSE, m_btnCancel);
	DDX_Control(pDX, IDC_UPDATE, m_btnUpdate);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_combType);
	DDX_Control(pDX, IDC_RADIO1, m_btnRadio1);
	DDX_Control(pDX, IDC_RADIO2, m_btnRadio2);
	DDX_Control(pDX, IDC_RADIO3, m_btnRadio3);
	DDX_Control(pDX, IDC_CHECK_MAGIC, m_btnCheckMagic);
	DDX_Control(pDX, IDC_CLUSTER_OPR, m_btnOpr);
	DDX_Control(pDX, IDC_LINK_FAMILYHOME,m_btnLinkHome);
	DDX_Control(pDX, IDC_BTN_HEAD_MODIFY,m_btnFamilyHeadModify);
	DDX_Control(pDX, IDC_CHECK_REFUSE, m_btnCheckRefuse);
}


BEGIN_MESSAGE_MAP(CClusterSettingDlg, CXSkinDlg)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_CLOSE, OnBnClickedClose)
	ON_BN_CLICKED(IDC_OK, OnBnClickedOk)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_SETMEMBERS, OnBnClickedSetmembers)
	ON_XT_MESSAGE()
	ON_BN_CLICKED(IDC_BTN_ADDFRIEND, OnBnClickedBtnAddfriend)
	ON_BN_CLICKED(IDC_BTN_DELMEMBER, OnBnClickedBtnDelmember)
	ON_BN_CLICKED(IDC_BTN_DELMANAGE, OnBnClickedBtnDelmanage)
	ON_BN_CLICKED(IDC_BTN_SETMANAGE, OnBnClickedBtnSetmanage)
	SET_HMG_SCROLLBAR(IDC_LIST_CLUSTER)
	SET_HMG_SCROLLBAR(IDC_EDIT_INTRO)
	ON_BN_CLICKED(IDC_UPDATE, OnBnClickedUpdate)
	ON_WM_NCDESTROY()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_MESSAGE(WM_CANCEL_NOTIFY,OnCancelRefresh)
	ON_BN_CLICKED(IDC_CLUSTER_OPR, OnBnClickedClusterOpr)
	ON_STN_CLICKED(IDC_LINK_FAMILYHOME, OnStnClickedLinkFamilyhome)
	ON_WM_PAINT()
	ON_MESSAGE(WM_FAMILY_HEAD_NOTIFY,OnFamilyIconNotify)
	ON_BN_CLICKED(IDC_ADVANCED_SETTING, OnBnClickedAdvancedSetting)
	ON_BN_CLICKED(IDC_BTN_HEAD_MODIFY, OnBnClickedBtnHeadModify)
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CClusterSettingDlg)
	ON_XT_COMMAND(CMD_GET_CINFO_ACK,OnGetClusterInfoAck)
	ON_XT_COMMAND(CMD_SET_CINFO_ACK,OnSetClusterInfoAck)
	ON_XT_COMMAND(CMD_GET_CMEMBERS_ACK,OnClusterMembersAck)
	ON_XT_COMMAND(CMD_CMEMBERS_IND,OnCMemberInd)
	ON_XT_COMMAND(CMD_SET_CMEMBERS_ACK,OnSetCMemberAck)
	ON_XT_COMMAND(CMD_LOGOUT,OnLoginOut)
END_XT_MESSAGE_MAP()



// CClusterSettingDlg 消息处理程序

void CClusterSettingDlg::Show(CLUSTER_INFO *pCluster)
{
	if ( pInstance==NULL )
	{
		pInstance = new CClusterSettingDlg(pCluster,GetDesktopWindow());		
		pInstance->UpdateClusterInfo();
		pInstance->ShowWindow(SW_SHOW);
	}
	else
	{
		if (pInstance->IsIconic())
		{
			pInstance->ShowWindow(SW_RESTORE);
		}

		memcpy(&pInstance->m_clusterInfo,pCluster,sizeof(CLUSTER_INFO));

		pInstance->UnLockOperation();
		pInstance->CloseProgress();

		pInstance->UpdateClusterInfo();
		pInstance->BringWindowToTop();
	}
}

BOOL CClusterSettingDlg::OnEraseBkgnd(CDC* pDC)
{
	BOOL bRet = CXSkinDlg::OnEraseBkgnd(pDC);

	CRect rcLine;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_LINE);

	if ( pWnd )
	{
		pWnd->GetWindowRect(&rcLine);
		ScreenToClient(&rcLine);
	}

	CBrush brush;
	brush.Attach(m_hBrushControl);
	CPen   pen(PS_SOLID,1,IM_LINE_COLOR);

	CBrush *pOldbrush = pDC->SelectObject(&brush);
	CPen   *pOldpen   = pDC->SelectObject(&pen);

	pDC->Rectangle(&rcLine);
	brush.Detach();

	pDC->SelectObject(pOldbrush);
	pDC->SelectObject(pOldpen);

	return bRet;
}

void CClusterSettingDlg::OnBnClickedClose()
{
	DestroyWindow();
}

void CClusterSettingDlg::Real_UpdateClusterInfo()
{
	CLUSTER_INFO * pCluster=&this->m_clusterInfo;

	CString str;
	str.Format("群 %s",pCluster->szFamilyName);
	SetWindowText(str);

	int nType = m_clusterInfo.GetMemberType(g_ImUserInfo.GetId());

	for ( int i=0; i<pCluster->Count(); i++ )
	{
		FRIEND_INFO *pFriend = &pCluster->members[i];

		int nItem = m_lstCluster.AddString(UserInfoToStr(pFriend));
		m_lstCluster.SetItemData(nItem,(DWORD)pFriend);
	}

	//	m_edtBulitin.SetWindowText(pCluster->bulitin);

	FRIEND_INFO *pCreator = pCluster->GetCreator();

	if ( pCreator )
		m_edtCreator.SetWindowText(pCreator->nickname);
	else
		m_edtCreator.SetWindowText(UIntToStr(pCluster->nCreatorID));

	m_edtID.SetWindowText(UIntToStr(pCluster->nFamilyID));
	m_edtIntro.SetWindowText(pCluster->szFamilyDesc);
	m_edtName.SetWindowText(pCluster->szFamilyName);
	if(g_pWebConfig )
	{
		CString sUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(FAMILY_HOME,StrToWideStr(pCluster->szFamilyDomain));
		m_btnLinkHome.SetWindowText(sUrl);
	}

	m_btnLinkHome.SizeToContent();
	//m_combType.SetCurSel(pCluster->nFamilyClass);
	ComboSelFamilyClass(&m_combType,pCluster->nFamilyClass);

	CLUSTER_INFO *pRealCluster = FindClusterInfo(pCluster->nFamilyID);

	if ( pRealCluster )
	{
		m_btnCheckMagic.SetCheck(pRealCluster->bAutoShowMagic);
		m_btnCheckRefuse.SetCheck(pRealCluster->bRefused);
	}

	m_pBmpFamilyHead = GetClusterBitmap(pRealCluster,false);

	SetClusterAuthRadioBtn(pCluster->nJoinFlag);

	if ( nType==2 )
	{
		m_btnSetmanage.EnableWindow(TRUE);
		m_btnDelmanage.EnableWindow(TRUE);
	}
	else
	{
		m_btnSetmanage.EnableWindow(FALSE);
		m_btnDelmanage.EnableWindow(FALSE);
	}

	if ( nType==2 || nType==1 )//管理员和创建者
	{
		m_edtName.SetReadOnly(FALSE);
		m_edtIntro.SetReadOnly(FALSE);
		//		m_edtBulitin.SetReadOnly(FALSE);

		m_btnRadio1.EnableWindow(TRUE);
		m_btnRadio2.EnableWindow(TRUE);
		m_btnRadio3.EnableWindow(TRUE);

		m_combType.EnableWindow(TRUE);
		m_btnAdvancedSetting.EnableWindow(TRUE);
		//m_btnFamilyHeadModify.ShowWindow(SW_SHOW);
	}
	else
	{
		m_btnSetmembers.EnableWindow(FALSE);
		m_btnDelmember.EnableWindow(FALSE);
		m_btnRadio1.EnableWindow(FALSE);
		m_btnRadio2.EnableWindow(FALSE);
		m_btnRadio3.EnableWindow(FALSE);
		m_btnAdvancedSetting.EnableWindow(FALSE);

		//m_btnFamilyHeadModify.ShowWindow(SW_HIDE);
	}

	m_lstCluster.SortClusterList();
}

void CClusterSettingDlg::UpdateClusterInfo()
{
	m_lstCluster.ResetContent();
	SetTimer(TIMER_UPDATE_CLUSTER,10,NULL);	
}

BOOL CClusterSettingDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetDlgStyle(MINCLOSE);
	EnableHelp(true);

	AttachHMGScrollbar(this->m_lstCluster.m_hWnd);
	AttachHMGScrollbar(this->m_edtIntro.m_hWnd);

	CenterWindow(GetDesktopWindow());

	ComboInitFamilyClass(&m_combType);

	g_pProtocol->RegistCmd(CMD_GET_CINFO_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_GET_CMEMBERS_ACK,m_hWnd);
	//g_pProtocol->RegistCmd(CMD_SET_CINFO_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_CMEMBERS_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SET_CMEMBERS_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_LOGOUT,m_hWnd);

	this->m_edtCreator.SetBorderColor(IM_LINE_COLOR);
	this->m_edtID.SetBorderColor(IM_LINE_COLOR);
	this->m_edtIntro.SetBorderColor(IM_LINE_COLOR);
	this->m_edtName.SetBorderColor(IM_LINE_COLOR);
	this->m_edtName.SetBorderColor(IM_LINE_COLOR);
	this->m_combType.SetBorderColor(IM_LINE_COLOR);

	m_edtName.SetLimitText(30);

	int nType;// 2，群主，1群管理员，0普通成员
	nType = m_clusterInfo.GetMemberType(g_ImUserInfo.GetId());
	if(nType != 2)
		SetDlgItemText(IDC_CLUSTER_OPR,"退出群");
	else
		SetDlgItemText(IDC_CLUSTER_OPR,"解散群");

	g_webFamilyHeadIconProcess.RegistIconNotify(m_hWnd);
	m_btnFamilyHeadModify.ShowWindow(SW_HIDE);

	return TRUE;  
}

void CClusterSettingDlg::OnBnClickedOk()
{
	int nType = m_clusterInfo.GetMemberType(g_ImUserInfo.GetId());
	bool bCheckAutoMagic = this->m_btnCheckMagic.GetCheck();
	bool bCheckRefused   = this->m_btnCheckRefuse.GetCheck();

	bool bFlag = false;

	CLUSTER_INFO *pRealCluster = FindClusterInfo(m_clusterInfo.nFamilyID);

	if ( pRealCluster &&  bCheckAutoMagic!=pRealCluster->bAutoShowMagic )
	{
		pRealCluster->bAutoShowMagic = m_clusterInfo.bAutoShowMagic = bCheckAutoMagic;
		g_LocalData.SaveClusterConfig((char*)g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());

		bFlag = true;
	}

	if ( pRealCluster && bCheckRefused!=pRealCluster->bRefused )
	{
		pRealCluster->bRefused = bCheckRefused;
		g_LocalData.SaveClusterConfig((char*)g_ImUserInfo./*GetUserName*/GetUserFileName().c_str());

		bFlag = true;
	}

	if ( nType==1 || nType==2 )//更新
	{
		//修改群
		CString sName;
		m_edtIntro.GetWindowText(m_cInfoReq.szFamilyDesc,sizeof(m_cInfoReq.szFamilyDesc));
		m_edtName.GetWindowText(sName);

		sName.Trim();
		if(sName=="")
		{
			CMessageBox::Prompt("群名不能为空");
			return;
		}
		strcpy(m_cInfoReq.szFamilyName,sName);

		m_cInfoReq.nFamilyClass = ComboGetFamilyClass(&m_combType);
		m_cInfoReq.nJoinFlag    = GetClusterAuth();
		m_cInfoReq.nFamilyProp = PERM_FAMILY;
		m_cInfoReq.nFamilyID   = m_clusterInfo.nFamilyID;
		GetDlgItemText(IDC_EDIT_MAINPAGE,m_cInfoReq.szFamilyFaceUrl,sizeof(m_cInfoReq.szFamilyFaceUrl));

		CLUSTER_INFO * pCluster = FindClusterInfo(m_cInfoReq.nFamilyID);
		if(pCluster)
		{
			pCluster->nJoinFlag = m_cInfoReq.nJoinFlag;
			strcpy(pCluster->szFamilyName,m_cInfoReq.szFamilyName);
			strcpy(pCluster->szFamilyDesc,m_cInfoReq.szFamilyDesc);
			pCluster->nFamilyClass = m_cInfoReq.nFamilyClass;

		}

		g_pProtocol->SendReq(m_cInfoReq);

		this->LockOperation();
		this->ShowProgress("正在提交群资料...");
	}
	else
	{
		if ( bFlag==true )
		{
			ShowMessage("修改成功");
		}

		DestroyWindow();
	}
}

HBRUSH CClusterSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		UINT nID = pWnd->GetDlgCtrlID();

		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBrushControl;

		if (nID==IDC_STATIC_HOME)
		{
			hbr = m_hBkGround;
		}
	}

	return hbr;
}

void CClusterSettingDlg::OnBnClickedSetmembers()
{
	CClusterMemberSelectDlg dlg(&m_clusterInfo,this);
	
	if ( IDOK==dlg.DoModal() )
	{
		dlg.GetClusterInfo(m_clusterInfo);
		UpdateClusterInfo();
	}
}

void CClusterSettingDlg::OnSetClusterInfoAck(const char *data,WORD wRet)
{
	LPXT_SET_CINFO_ACK ack = (LPXT_SET_CINFO_ACK)data;

	UnLockOperation();
	CloseProgress();

    if ( wRet==0 )
	{
		if ( ack->nFamilyID==m_clusterInfo.nFamilyID )
		{
			strncpy(m_clusterInfo.szFamilyName,m_cInfoReq.szFamilyName,sizeof(m_clusterInfo.szFamilyName)-1);
			strncpy(m_clusterInfo.szFamilyDesc,m_cInfoReq.szFamilyDesc,sizeof(m_clusterInfo.szFamilyDesc)-1);

			m_clusterInfo.nFamilyClass = m_cInfoReq.nFamilyClass;		
			m_clusterInfo.nJoinFlag    = m_cInfoReq.nJoinFlag;
			UpdateClusterInfo();
			ShowMessage("修改成功!");
		}
	}
	else
	{
		if ( wRet==SERVER_TIMEOUT )
		{
			ShowMessage("服务器处理超时！");
		}
		else
		{
			ShowMessage(g_pProtocol->GetLastError());
			UpdateClusterInfo();
		}
	}
}


void CClusterSettingDlg::OnGetClusterInfoAck(const char *data,WORD wRet)
{
	LPXT_GET_CINFO_ACK ack = (LPXT_GET_CINFO_ACK)data;

	if ( wRet==0 )
	{
		if ( ack->nFamilyID==m_clusterInfo.nFamilyID &&
			 ack->nFamilyProp==m_clusterInfo.nFamilyProp )
		{
			memcpy(&m_clusterInfo,data,sizeof(XT_CLUSTER_INFO));

			SetClusterAuthRadioBtn(ack->nJoinFlag);

			SetTimer(TIMER_CHECK,60000,NULL);
		}
	}
	else
	{
		
		if ( wRet==SERVER_TIMEOUT )
		{
			ShowMessage("服务器处理超时!");
		}
		else
		{
			ShowMessage(g_pProtocol->GetLastError());
		}
		this->UnLockOperation();
		this->CloseProgress();
	}
}

void CClusterSettingDlg::OnCMemberInd(const char *data, WORD wRet)
{
	LPXT_CMEMBER_IND ind = (LPXT_CMEMBER_IND)data;

	if ( wRet==0 && ind->cluster_id==this->m_clusterInfo.nFamilyID && ind->cluster_type==PERM_FAMILY )
	{
		CLUSTER_INFO *pRealCluster = FindClusterInfo(ind->cluster_id);

		if ( pRealCluster )
		{
			m_clusterInfo = *pRealCluster;
			this->UpdateClusterInfo();
		}
	}
}

LRESULT CClusterSettingDlg::OnFamilyIconNotify(WPARAM wParam, LPARAM lParam)
{
	if ( wParam==m_clusterInfo.nFamilyID && lParam!=0 )
	{
		CLUSTER_INFO * pRealCluster = FindClusterInfo(m_clusterInfo.nFamilyID);
		m_pBmpFamilyHead = GetClusterBitmap(pRealCluster,false);
		InvalidateFamilyHead();
	}

	return 0;
}

LRESULT CClusterSettingDlg::OnCancelRefresh(WPARAM, LPARAM)
{
	this->CloseProgress();
	this->UnLockOperation();
	KillTimer(TIMER_CHECK);
	return 0;
}


void CClusterSettingDlg::OnBnClickedBtnAddfriend()
{
	FRIEND_INFO *pFriend = NULL;
	int nSel=m_lstCluster.GetCurSel();

	if ( nSel>-1 )
	{
		pFriend = (FRIEND_INFO *)m_lstCluster.GetItemData(nSel);
	}

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
	
		g_pProtocol->SendReq(req);

		RegUserOperation(0x00c2);
	}
	else
	{
		ShowMessage("请从查找结果中选中一个用户!");
	} 
}

void CClusterSettingDlg::OnLoginOut(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		DestroyWindow();
	}
}

void CClusterSettingDlg::OnBnClickedBtnDelmember()
{
	int nSel   = m_lstCluster.GetCurSel();
	int nCount = 0 ;

	CString sMsg;

	if ( nSel>-1 )
	{
		FRIEND_INFO *pFriend = (FRIEND_INFO *)m_lstCluster.GetItemData(nSel);


		if ( pFriend->GetId()==g_ImUserInfo.GetId() )
		{
			ShowMessage("您不能删除自己。");
			return ;
		}

		nCount=m_lstCluster.GetCount();
		
		// 如果成员小于规定个数,解散
		if(nCount<=MIN_CLUSTER_MEMBER)
		{
			sMsg.Format("群\"%s\"的成员个数如果少于%d个的话,将会被系统自动解散!确定要删除成员\"%s\"吗?",
				m_clusterInfo.szFamilyName,
				MIN_CLUSTER_MEMBER,
				pFriend->nickname);
			if ( CMessageBox::Model(GetSafeHwnd(),sMsg,IM_CAPTION,MB_OKCANCEL)==IDOK )
			{
				XT_CLUSTER_DISMISS_REQ req;
				req.nFamilyID = m_clusterInfo.nFamilyID;
				req.nFamilyProp = PERM_FAMILY;
				g_pProtocol->SendReq(req);

				// 删除主界面群列表对应的项
				::SendMessage(CMainFrame::GetInstance()->GetHWND(), WM_IMOPERATION,OPR_DEL_CLUSTER,(LPARAM)m_clusterInfo.nFamilyID);
				// 退出
				OnClose();
			}
		}
		else
		{
			sMsg.Format("确定要删除群成员\"%s\"吗？",pFriend->nickname);

			if (CMessageBox::Model(GetSafeHwnd(),sMsg,IM_CAPTION,MB_YESNO)==IDYES )
			{
				XT_SET_CMEMBERS_REQ req;

				req.nFamilyID = m_clusterInfo.nFamilyID;
				req.nAction     = CMEMBER_DEL;//
				req.nFamilyProp = m_clusterInfo.nFamilyProp;
				req.nCount = 1;
				req.nMemID[0] = pFriend->GetId();
			
				g_pProtocol->SendReq(req);	
				
				this->LockOperation();
				this->ShowProgress("删除中,请稍候...");
			}
		}
	}
}

void CClusterSettingDlg::OnClusterMembersAck(const char *data,WORD wRet)
{
	LPXT_GET_CMEMBERS_ACK ack = (LPXT_GET_CMEMBERS_ACK)data;
	char szTemp[20];
	if ( wRet==0 )
	{
		if ( ack->nFamilyID==m_clusterInfo.nFamilyID && ack->nFamilyProp==PERM_FAMILY )
		{
			for ( int i=0; i<ack->return_count; i++ )
			{
				if ( ack->members[i].id==0 )
				{
					continue;
				}

				FRIEND_INFO *pFriend = m_clusterInfo.GetFriendInfo(ack->members[i].id);

				if ( pFriend==NULL )
				{
					m_clusterInfo.members.push_back(FRIEND_INFO());
					pFriend = &m_clusterInfo.members[m_clusterInfo.Count() - 1];
					strcpy(pFriend->nickname,itoa(ack->members[i].id,szTemp,10));
				}
				pFriend->SetId(ack->members[i].id);
				pFriend->cluster_property = ack->members[i].prop;
				pFriend->status = ack->members[i].status;
				if ( pFriend->cluster_property==CMEMBER_CREATOR )//creator 
				{
					m_clusterInfo.nCreatorID = pFriend->GetId();
				}
			}//end for
	
			KillTimer(TIMER_CHECK);
			SetTimer(TIMER_CHECK,60000,NULL);

			if ( ack->next_pos==0xFFFF && ack->next_pos != 0)
			{	
				this->UnLockOperation();
				this->CloseProgress();

				if ( m_clusterInfo.bRefreshFlag )
				{
					CString str;
					str.Format("群 \"%s\" 更新完成!",m_clusterInfo.szFamilyName);
					ShowMessage(str);	
					m_clusterInfo.bRefreshFlag =false;
				}

				KillTimer(TIMER_CHECK);
			}

			UpdateClusterInfo();
		}
	}
	else
	{
		if ( ack->nFamilyID==m_clusterInfo.nFamilyID && ack->nFamilyProp==PERM_FAMILY )
		{

			CString str;
			str.Format("群 \"%s\" 更新失败!",m_clusterInfo.szFamilyName);
			ShowMessage(str);	
			this->UnLockOperation();
			this->CloseProgress();
		}
	}
}

void CClusterSettingDlg::OnBnClickedBtnDelmanage()
{
	int nSel = m_lstCluster.GetCurSel();

	if ( nSel>-1 )
	{
		FRIEND_INFO *pFriend = (FRIEND_INFO *)m_lstCluster.GetItemData(nSel);

		if ( pFriend->GetId()==g_ImUserInfo.GetId() )
		{
			ShowMessage("您不能更改自己的权限哦!");
			return ;
		}

		if ( pFriend->cluster_property!=1 )
		{
			CString str;
			str.Format("%s 不是本群的管理员!请选择管理员",pFriend->nickname);
			ShowMessage(str);
			return ;
		}

		XT_SET_CMEMBERS_REQ req;

		req.nFamilyID = m_clusterInfo.nFamilyID;
		req.nAction = CMEMBER_CANCEL_ADMIN;//CMEMBER_ADD
		req.nFamilyProp = m_clusterInfo.nFamilyProp;
		req.nCount = 1;
		req.nMemID[0] = pFriend->GetId();
	
		g_pProtocol->SendReq(req);

		this->LockOperation();
		this->ShowProgress("正在取消管理员资格...");

	}
}

void CClusterSettingDlg::OnBnClickedBtnSetmanage()
{
	int nSel = m_lstCluster.GetCurSel();

	if ( nSel>-1 )
	{
		FRIEND_INFO *pFriend = (FRIEND_INFO *)m_lstCluster.GetItemData(nSel);

		if ( pFriend->GetId()==g_ImUserInfo.GetId() )
		{
			ShowMessage("您是本群的最高长官!管理员是你的下属!");
			return ;
		}

		if ( pFriend->cluster_property>0 )
		{
			CString str;
			str.Format("%s 已经是本群的管理员了!",pFriend->nickname);
			ShowMessage(str);
			return ;
		}

		XT_SET_CMEMBERS_REQ req;

		req.nFamilyID = m_clusterInfo.nFamilyID;
		req.nAction = CMEMBER_SET_ADMIN;//CMEMBER_ADD
		req.nFamilyProp = m_clusterInfo.nFamilyProp;
		req.nCount = 1;
		req.nMemID[0] = pFriend->GetId();
	
		g_pProtocol->SendReq(req);

		this->LockOperation();
		this->ShowProgress("正在设置管理员...");
	}
}

void CClusterSettingDlg::SetClusterAuthRadioBtn(int nAuth)
{
	switch(nAuth)
	{
	case 0:
		m_btnRadio1.SetCheck(1);
		m_btnRadio2.SetCheck(0);
		m_btnRadio3.SetCheck(0);
		break;
	case 1:
		m_btnRadio1.SetCheck(0);
		m_btnRadio2.SetCheck(1);
		m_btnRadio3.SetCheck(0);
		break;
	case 2:
		m_btnRadio1.SetCheck(0);
		m_btnRadio2.SetCheck(0);
		m_btnRadio3.SetCheck(1);
		break;
	default:
		break;
	}
}

int CClusterSettingDlg::GetClusterAuth()
{
	int nAuth =0;

	if ( m_btnRadio1.GetCheck() )
		nAuth=0;
	else
	if ( m_btnRadio2.GetCheck() )
		nAuth=1;
	else
	if ( m_btnRadio3.GetCheck() )
		nAuth=2;

	return nAuth;
}

void CClusterSettingDlg::OnBnClickedUpdate()
{
	XT_GET_CINFO_REQ req;
	req.nFamilyID = m_clusterInfo.nFamilyID;
	req.nFamilyProp = PERM_FAMILY;	

	CLUSTER_INFO *pCluster = FindClusterInfo(m_clusterInfo.nFamilyID);

	if ( pCluster )
	{
		m_clusterInfo = *pCluster;
		m_clusterInfo.bRefreshFlag = true;
		pCluster->bRefreshFlag = true;
		pCluster->dwTickGet    = 0;
	}

	g_pProtocol->SendReq(req);

	this->LockOperation();
	this->ShowProgress("正在更新群信息...");

	SetTimer(TIMERID_REFRESH_DISABLE,3000,NULL);
	m_btnUpdate.EnableWindow(FALSE);
} 

void CClusterSettingDlg::OnSetCMemberAck(const char *data,WORD wRet)
{
	LPXT_SET_CMEMBERS_ACK ack = (LPXT_SET_CMEMBERS_ACK)data;

	if ( wRet==0 && ack->nFamilyID==this->m_clusterInfo.nFamilyID && ack->nFamilyProp==PERM_FAMILY )
	{
		if ( ack->nAction==CMEMBER_SET_ADMIN )//设为管理员
		{
			RegUserOperation(0x00b5);

			for ( int i=0; i<m_lstCluster.GetCount(); i++ )
			{
				FRIEND_INFO *pUser = (FRIEND_INFO *)m_lstCluster.GetItemData(i);

				if ( pUser->GetId()==ack->nMemberID )
				{
					pUser->cluster_property=1;
					m_lstCluster.InvalidateItem(i);
					break;
				}
			}
		}

		if ( ack->nAction==CMEMBER_CANCEL_ADMIN )//撤销管理员
		{
			RegUserOperation(0x00b4);

			for ( int i=0; i<m_lstCluster.GetCount(); i++ )
			{
				FRIEND_INFO *pUser = (FRIEND_INFO *)m_lstCluster.GetItemData(i);
				
				if ( pUser->GetId()==ack->nMemberID )
				{
					pUser->cluster_property=0;
					m_lstCluster.InvalidateItem(i);
					break;
				}
			}
		}

		if ( ack->nAction == CMEMBER_DEL )//删除成员
		{		
			RegUserOperation(0x00b2);

			for ( int i=0; i<m_lstCluster.GetCount(); i++ )
			{
				FRIEND_INFO *pUser = (FRIEND_INFO *)m_lstCluster.GetItemData(i);

				if ( pUser->GetId()==ack->nMemberID )
				{
					m_clusterInfo.DelMember(ack->nMemberID);
		
					m_lstCluster.ResetContent();

					for ( int i=0; i<m_clusterInfo.Count(); i++ )
					{
						FRIEND_INFO *pFriend = &m_clusterInfo.members[i];

						int nItem = m_lstCluster.AddString(UserInfoToStr(pFriend));
						m_lstCluster.SetItemData(nItem,(DWORD)pFriend);
					}

					m_lstCluster.SortClusterList();
					break;
				}
			}
		}
	}

	this->UnLockOperation();
	this->CloseProgress();
}

void CClusterSettingDlg::OnNcDestroy()
{
	g_pProtocol->UnRegistCmd(m_hWnd);
	g_webFamilyHeadIconProcess.UnRegistIconNotify(m_hWnd);

	CXSkinDlg::OnNcDestroy();

	delete this;
}

void CClusterSettingDlg::OnClose()
{
	DestroyWindow();
}

void CClusterSettingDlg::ShowProgress(CString strTip)
{
	CloseProgress();

	m_pProgress = new CFormProgress3(m_hWnd,this);
	m_pProgress->SetTip(strTip);
	m_pProgress->CenterWindow(this);
	m_pProgress->ShowWindow(SW_SHOW);
}

void CClusterSettingDlg::CloseProgress()
{
	if ( m_pProgress )
	{
		m_pProgress->DestroyWindow();
		delete m_pProgress;
		m_pProgress=NULL;
	}
}

void CClusterSettingDlg::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==TIMER_CHECK )
	{
		ShowMessage("更新群成员列表超时！");
		this->UnLockOperation();
		this->CloseProgress();
	}
	else if( nIDEvent == TIMERID_REFRESH_DISABLE )
	{
		if( !m_btnUpdate.IsWindowEnabled() )
		{
			m_btnUpdate.EnableWindow();
			KillTimer(TIMERID_REFRESH_DISABLE);
		}
	}
	else if ( nIDEvent==TIMER_UPDATE_CLUSTER )
	{
		KillTimer(nIDEvent);
		Real_UpdateClusterInfo();
	}

	CXSkinDlg::OnTimer(nIDEvent);
}

void CClusterSettingDlg::OnBnClickedClusterOpr()
{
	CString sMsg;
	int nType;// 2，群主，1群管理员，0普通成员
	nType = m_clusterInfo.GetMemberType(g_ImUserInfo.GetId());
	if(nType != 2)
		sMsg = "确定要退出该群吗?";
	else
		sMsg = "确定要解散该群吗?";
	if ( CMessageBox::Model(GetSafeHwnd(),sMsg,IM_CAPTION,MB_OKCANCEL) == IDOK )
	{
		if(nType != 2)
		{
			// 退出群
			XT_CLUSTER_APPLY_REQ req;
			req.nAction = 1;
			req.nFamilyID = m_clusterInfo.nFamilyID;
			req.nFamilyProp = PERM_FAMILY;
			req.nUserID = g_ImUserInfo.GetId();
			g_pProtocol->SendReq(req);
		}
		else
		{
			// 解散群
			XT_CLUSTER_DISMISS_REQ req;
			req.nFamilyID = m_clusterInfo.nFamilyID;
			req.nFamilyProp = PERM_FAMILY;
			g_pProtocol->SendReq(req);
		}
		// 删除主界面群列表对应的项
		::SendMessage(CMainFrame::GetInstance()->GetHWND(), WM_IMOPERATION,OPR_DEL_CLUSTER,(LPARAM)m_clusterInfo.nFamilyID);
		// 退出
		OnClose();
	}
}

void CClusterSettingDlg::OnStnClickedLinkFamilyhome()
{
	if(g_pWebConfig)
	{
		CString sUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(FAMILY_HOME,StrToWideStr(m_clusterInfo.szFamilyDomain));
		MiniOpenUrl(sUrl);
	}

}

void CClusterSettingDlg::InvalidateFamilyHead()
{
	CWnd *pWnd = GetDlgItem(IDC_STATIC_ICON);
	if ( pWnd && pWnd->m_hWnd )
	{
		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		InvalidateRect(&rc);
	}
}

void CClusterSettingDlg::OnHelpClick()
{
	OpenHtmlPage("www.feiniu.com");
}

void CClusterSettingDlg::OnPaint()
{
	CPaintDC dc(this); 
	CXSkinDlg::OnPaint();

	CWnd * pWnd = GetDlgItem(IDC_STATIC_ICON);

	if ( pWnd && pWnd->m_hWnd )
	{
		CRect rc;
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);
		rc.InflateRect(1,1);

		int cx = rc.Width();
		int cy = rc.Height();
	
		CBitmap * pHead ;

		if ( m_pBmpFamilyHead )
			pHead = m_pBmpFamilyHead;
		else
			pHead = &m_bmpDefaultFamilyHead;

		int nSavedDC = dc.SaveDC();

		DrawBitmap(&dc,rc.left+(cx-40)/2,rc.top+(cy-40)/2,pHead,0);

		CPen   pen(PS_SOLID,1,RGB(0,122,255));
		CPoint pt(3,3);
		dc.SelectObject(&pen);
		dc.SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));		
		dc.RoundRect(&rc,pt);

		dc.RestoreDC(nSavedDC);
	}
}

void CClusterSettingDlg::OnBnClickedAdvancedSetting()
{
	if(g_pWebConfig)
	{
		MiniOpenUrl( (LPCSTR)g_pWebConfig->GetWebPageUrl(FAMILY_SETTING,StrToWideStr(m_clusterInfo.szFamilyDomain)));
	}

}

void CClusterSettingDlg::OnBnClickedBtnHeadModify()
{
	CFamilyFaceDlg dlg(this->m_clusterInfo.nFamilyID,this);
	if (IDOK==dlg.DoModal())
	{
		uint16 faceid;
		CString strUrl;
		if ( dlg.GetFaceInfo(faceid,strUrl) )
		{
			if ( faceid==(uint16)0xFF )
			{

			}
			else
			{
			//	SetFamilyFace(&m_clusterInfo,faceid);
			}
		}
	}
}
