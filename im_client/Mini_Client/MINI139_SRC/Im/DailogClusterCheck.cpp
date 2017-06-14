// im\DailogClusterCheck.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\dailogclustercheck.h"
#include "..\FriendInfoDlg.h"
#include "LocalData.h"
#include "MessageHandleWnd.h"
vector<CDailogClusterCheck::tagCF> CDailogClusterCheck::m_vecCF;
// CDailogClusterCheck 对话框

IMPLEMENT_DYNAMIC(CDailogClusterCheck, CXSkinDlg)
IMPLEMENT_HMG_SCROLLBAR(CDailogClusterCheck)

CDailogClusterCheck::CDailogClusterCheck(XT_CLUSTER_AUTH_IND *ind, CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CDailogClusterCheck::IDD, pParent)
{
	m_clusterAuthInd = (*ind);

	m_cf.cid = ind->cluster_id;
	m_cf.fid = ind->uid;
	m_cf.pDlg = this;
	m_hBkGround = CreateSolidBrush(RGB(255,255,255));

	m_vecCF.push_back(m_cf);
//	m_mapClusterAuthId.insert(map<uint32,void *>::value_type(cid,(void *)this));
}

CDailogClusterCheck::~CDailogClusterCheck()
{
	for ( int i=0; i<m_vecCF.size(); i++ )
	{
		if ( m_vecCF[i].pDlg == this )
		{
			m_vecCF.erase(m_vecCF.begin()+i);
			break;
		}
	}
	DeleteObject(m_hBkGround);
}

void CDailogClusterCheck::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX,IDOK,m_btnOk);
	DDX_Control(pDX,IDCANCEL,m_btnCancel);
	DDX_Control(pDX,IDC_EDIT_AUTH,m_editAuth);
	DDX_Control(pDX, IDC_STATIC_FACE, m_faceIcon);
	DDX_Control(pDX, IDC_LBL_MORE, m_btnFullInfo);
}


BEGIN_MESSAGE_MAP(CDailogClusterCheck, CXSkinDlg)
	ON_XT_MESSAGE()
	ON_WM_CLOSE()
	ON_WM_NCDESTROY()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CTLCOLOR()
	SET_HMG_SCROLLBAR(IDC_EDIT_AUTH)
	ON_STN_CLICKED(IDC_LBL_MORE, OnStnClickedLblMore)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


BEGIN_XT_MESSAGE_MAP(CDailogClusterCheck)
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoAck)
END_XT_MESSAGE_MAP()

// CDailogClusterCheck 消息处理程序


void CDailogClusterCheck::OnUserInfoAck(const char *data,WORD wRet)
{
	LPXT_USERINFO_GET_ACK ack = (LPXT_USERINFO_GET_ACK)data;
	if(wRet == 0)
	{	
		if ( ack->GetId() == m_cf.fid )
		{
			ShowWindow(SW_SHOW);
			FlashWindow(TRUE);

			//memcpy(&m_friendInfo,ack,sizeof(XT_USER_INFO));
			
			UpdateUserFullInfo(&m_friendInfo,ack);
			m_friendInfo.bRefreshed = true;
			SetInfo();
			ShowWindow(SW_SHOW);

			CString str;
			str.Format("【%s】申请加入群【%s】", m_friendInfo.nickname, m_sCluster);
			CLUSTER_SYSTEM_MSG msgSys;
			msgSys.dwTime = time(NULL);
			sprintf(msgSys.szMessage,str);
			msgSys.nInviteId = m_cf.fid;
			sprintf(msgSys.szNickName, m_friendInfo.nickname);
			msgSys.nClusterId = m_cf.cid;
			sprintf(msgSys.szClusterName, m_sCluster);
			g_LocalData.AddClusterSystemMsg(g_ImUserInfo.GetId(),msgSys);
		}
	}
	else
	{

	}
}


void CDailogClusterCheck::SetInfo()
{
	
	CString str;

	str = "请求加入群 - " + m_sCluster;
	SetWindowText(str);
	SetDlgItemText(IDC_STATIC_INFO,str);

	//str.Format("用户名: %s",m_friendInfo.username);
	SetDlgItemText(IDC_LBL_USERNAME,m_friendInfo.GetUserName().c_str());

	//str.Format("昵称: %s",m_friendInfo.nickname);
	SetDlgItemText(IDC_LBL_NICKNAME,m_friendInfo.nickname);

	str.Format("性别: %s", SexToStr(m_friendInfo.sex)) ;
	SetDlgItemText(IDC_LBL_SEX,str);

	m_faceIcon.SetUserInfo(&m_friendInfo);

}

BOOL CDailogClusterCheck::OnInitDialog()
{

	CXSkinDlg::OnInitDialog();
	SetDlgStyle(MINCLOSE);
	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,m_hWnd);
	
	
	
	char szBuf[256];

	FRIEND_INFO * pFriend = GetFriendInfo(m_cf.fid);
	CLUSTER_INFO * pCluster = FindClusterInfo(m_cf.cid);

	if(pCluster == NULL)
	{
		m_sCluster = itoa(m_cf.cid,szBuf,10);
	}
	else
	{
		m_sCluster = pCluster->szFamilyName;
	}
	
	if( pFriend )
	{
		m_friendInfo = *pFriend;
		SetInfo();
		ShowWindow(SW_SHOW);

		CString str;
		str.Format("【%s】申请加入群【%s】", m_friendInfo.nickname, m_sCluster);
		CLUSTER_SYSTEM_MSG msgSys;
		msgSys.dwTime = time(NULL);
		sprintf(msgSys.szMessage,str);
		msgSys.nInviteId = m_cf.fid;
		sprintf(msgSys.szNickName, m_friendInfo.nickname);
		msgSys.nClusterId = m_cf.cid;
		sprintf(msgSys.szClusterName, m_sCluster);
		g_LocalData.AddClusterSystemMsg(g_ImUserInfo.GetId(),msgSys);

	}
	else
	{
		// 如果没有好友信息则需要获取
		XT_USERINFO_GET_REQ userinfo_req;
		userinfo_req.fid = m_cf.fid;
		g_pProtocol->SendReq(userinfo_req);
		ShowWindow(SW_HIDE);
	}

	SetDlgItemText(IDC_EDIT_AUTH,m_clusterAuthInd.info);

	AttachHMGScrollbar(m_editAuth.m_hWnd);

	CenterWindow(GetDesktopWindow());

	return TRUE; 
}

void CDailogClusterCheck::OnOK()
{
	XT_SET_CMEMBERS_REQ req;

	req.nAction = CMEMBER_ADD;
	req.nFamilyID = m_cf.cid;
	req.nFamilyProp =PERM_FAMILY;
	req.nCount = 1;
	req.nMemID[0]=m_cf.fid;
	req.nInviteID=g_ImUserInfo.GetId();
	g_pProtocol->SendReq(req);

	// 同意加入群时，写入系统消息
	CLUSTER_INFO * pCluster = FindClusterInfo(m_cf.cid);
	if(pCluster != NULL)
	{
		//CString str;
		//str.Format("群 \"%s\" 有新成员加入。他(她)叫: %s！",pCluster->szFamilyName, m_friendInfo.nickname);
		//str.Format("【%s】加入群", m_friendInfo.username);
		//写群系统消息
		//SYSTEM_MSG msgSys;
		//msgSys.dwTime = time(NULL);
		//msgSys.iType  = SYSTEM_MSG::SYSMSG_CLUSTER;
		//strcpy(msgSys.szMessage, str);
		//g_LocalData.AddSystemMsg(g_ImUserInfo.GetId(),msgSys);

		//CLUSTER_SYSTEM_MSG msgSys;
		//msgSys.dwTime = time(NULL);
		//sprintf(msgSys.szMessage,str);
		//msgSys.nInviteId = 0;
		//msgSys.nClusterId = m_cf.cid;
		//sprintf(msgSys.szClusterName, pCluster->szFamilyName);
		//g_LocalData.AddClusterSystemMsg(g_ImUserInfo.GetId(),msgSys);
	}	

	DestroyWindow();
}

void CDailogClusterCheck::Close()
{
	vector<CDailogClusterCheck *> vecDlg;

	for ( int i=0; i<m_vecCF.size(); i++ )
		vecDlg.push_back(m_vecCF[i].pDlg);

	for ( int i=0; i<vecDlg.size(); i++ )
		vecDlg[i]->DestroyWindow();
}

CDailogClusterCheck * CDailogClusterCheck::Show(XT_CLUSTER_AUTH_IND *ind)
{
	CDailogClusterCheck *pDlg = NULL;

	for ( int i=0; i<m_vecCF.size(); i++ )
	{
		if ( m_vecCF[i].cid==ind->cluster_id && m_vecCF[i].fid==ind->uid )
		{
			pDlg = m_vecCF[i].pDlg;
			break;
		}
	}

	if ( pDlg == NULL )
	{
		pDlg = new CDailogClusterCheck(ind,GetDesktopWindow());
		pDlg->Create(CDailogClusterCheck::IDD,GetDesktopWindow());
		//pDlg->ShowWindow(SW_HIDE);
		
	}
	else
	{
		if (pDlg->IsIconic())
			pDlg->ShowWindow(SW_RESTORE);
		else
			pDlg->ShowWindow(SW_SHOW);

		pDlg->BringWindowToTop();
	}

	return pDlg;
}

void CDailogClusterCheck::OnClose()
{
	DestroyWindow();
}

void CDailogClusterCheck::OnNcDestroy()
{
	CXSkinDlg::OnNcDestroy();
	delete this;
}

void CDailogClusterCheck::OnBnClickedCancel()
{
	XT_SET_CMEMBERS_REQ req;

	req.nAction = CMEMBER_CREATOR_REFUSE;
	req.nFamilyID = m_cf.cid;
	req.nFamilyProp =PERM_FAMILY;
	req.nCount = 1;
	req.nMemID[0]=m_cf.fid;
	req.nInviteID=g_ImUserInfo.GetId();
	g_pProtocol->SendReq(req);

	DestroyWindow();
}


HBRUSH CDailogClusterCheck::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBkGround;
	}

	return hbr;
}

void CDailogClusterCheck::OnStnClickedLblMore()
{
	CFriendInfoDlg::NewDlg(&m_friendInfo);
}

BOOL CDailogClusterCheck::OnEraseBkgnd(CDC* pDC)
{
	BOOL bRt = CXSkinDlg::OnEraseBkgnd(pDC);
	CRect rcFrame;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_LINE);

	if ( pWnd )
	{
		pWnd->GetWindowRect(&rcFrame);
		ScreenToClient(&rcFrame);

		pDC->FillSolidRect(&rcFrame,RGB(255,255,255));
		pDC->Draw3dRect(&rcFrame,RGB(0,122,255),RGB(0,122,255));
	}


	return bRt;
}
