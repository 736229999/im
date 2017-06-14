// RegisterDlg.cpp : implementation file
//
#include "..\..\public_utils\Function.h"
#include "stdafx.h"
#include "..\IMClient.h"
#include "ClusterCreateDlg.h"
#include "..\MainFrame.h"

/////////////////////////////////////////////////////////////////////////////
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
std::tr1::shared_ptr<CCreateClusterDlg> CCreateClusterDlg::pInstance = NULL;

std::tr1::shared_ptr<CCreateClusterDlg> CCreateClusterDlg::Show()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
	}
	else
	{
		pInstance = tr1::shared_ptr<CCreateClusterDlg>(new CCreateClusterDlg());
		pInstance->Create(GetDesktopWindow(), _T("�½�Ⱥ"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}

	return pInstance;
}

BEGIN_XT_MESSAGE_MAP(CCreateClusterDlg)
	ON_XT_COMMAND(CMD_CREATE_CLUSTER_ACK,OnCreateAck)
END_XT_MESSAGE_MAP()

LRESULT CCreateClusterDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// ��ֹ�������
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	// �����̨������Ϣ
	if(WM_XT_CMD_NOTIFY == uMsg)
	{
		OnXtMessage(wParam, lParam);
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

// ��Ϣ
void CCreateClusterDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender == m_pBtnOK)
		{
			OnBnClickedOK();
		}
		else if(msg.pSender == m_pBtnCancel)
		{
			Close(1);
		}
	}
}

void CCreateClusterDlg::OnFinalMessage(HWND hWnd)
{
	g_pProtocol->UnRegistCmd(GetHWND());
	DuiLib::WindowImplBase::OnFinalMessage(hWnd);
}

void CCreateClusterDlg::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));

	SetIcon(IDR_MAINFRAME);

	m_pEditClusterName = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl("clusternameEdit"));
	m_pComboType = static_cast<DuiLib::CComboUI*>(m_PaintManager.FindControl("clustertypeCombo"));
	m_pREditBriefInfo = static_cast<DuiLib::CRichEditUI*>(m_PaintManager.FindControl("briefinfo"));
	m_pRedioJoinLevel_allowall = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("allowall"));
	m_pRedioJoinLevel_needverify = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("needverify"));
	m_pRedioJoinLevel_disablejoin = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("disablejoin"));
	m_pBtnOK = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("okbtn"));
	m_pBtnCancel = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("cancelbtn"));

	UpdateBk();

	//  ����ComboBox����
	static LPCSTR c_type [] = { 
		"ʳƷ","����","����","�ҵ�","�ճ�����",
		"ĸӤ","ͼ������","��ױ","�������",
		"����","��Ȥ����","��������","����",
		"����"};

	static int    c_id   [] = {
		101, 102, 103, 104,
		201, 202, 203, 204,
		301, 302, 303, 304,
		401	};

	int n = sizeof(c_id)/sizeof(int);

	for ( int i=0; i<n; i++ )
	{
		DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
		pListElement->SetAttribute("text", c_type[i]);
		pListElement->SetTag(c_id[i]);
		m_pComboType->Add(pListElement);
	}
	m_pComboType->SelectItem(0);	

	g_pProtocol->RegistCmd(CMD_CREATE_CLUSTER_ACK,GetHWND());
}

// ��ȷ����ť
void CCreateClusterDlg::OnBnClickedOK()
{
	// Ⱥ����
	CString strClusterName = m_pEditClusterName->GetText().GetData();
	strClusterName.Trim();
	if(strClusterName.IsEmpty())
	{
		CMessageBox::Model(GetHWND(), "������Ⱥ����", "�½�Ⱥ", MB_OK|MB_ICONWARNING);
		return;
	}
	// Ⱥ����
	int nIndex = m_pComboType->GetCurSel();
	m_nClusterType = m_pComboType->GetItemAt(nIndex)->GetTag();
	// Ⱥ���
	CString strClusterBriefInfo = m_pREditBriefInfo->GetText().GetData();
	strClusterBriefInfo.Trim();
	// �����Ⱥ����
	m_nLevel = -1;
	if(m_pRedioJoinLevel_allowall->IsSelected())
	{
		m_nLevel = 0;
	}
	else if(m_pRedioJoinLevel_needverify->IsSelected())
	{
		m_nLevel = 1;
	}
	else
	{
		m_nLevel = 2;
	}

	// ����Ⱥ
	XT_CREATE_CLUSTER_REQ req;
	req.nJoinFlag = m_nLevel;
	strcpy(req.szFamilyDesc,strClusterBriefInfo);
	strcpy(req.szFamilyName,strClusterName);
	strcpy(req.szFamilyDomain,"");
	req.nFamilyProp = PERM_FAMILY;//�̶�Ⱥ
	req.nFamilyClass = m_nClusterType;
	g_pProtocol->SendReq(req);
}

void CCreateClusterDlg::OnCreateAck(const char *data,int nRetCode)
{
	LPXT_CREATE_CLUSTER_ACK ack = (LPXT_CREATE_CLUSTER_ACK)data;

	if ( nRetCode==0 )
	{
		// ���½���
		CLUSTER_INFO *pCluster = new CLUSTER_INFO();
		pCluster->nFamilyID = ack->nFamilyID;
		pCluster->bMemberInit = false;
		pCluster->nCreatorID  = g_ImUserInfo.GetId();
		//////////Ⱥ�������֤��ʽ��ȡ/////////////////////////////////
		////pCluster->nFamilyClass =  ComboGetFamilyClass(&(m_pPage1->m_comboType));
		pCluster->nFamilyClass =  m_nClusterType;    //Ⱥ�����ȡ
		pCluster->nJoinFlag    =  m_nLevel;//��֤��ʽ

		// Ⱥ���
		CString strClusterBriefInfo = m_pREditBriefInfo->GetText().GetData();
		strClusterBriefInfo.Trim();
		strcpy(pCluster->szFamilyDesc, strClusterBriefInfo);

		g_vecCluster.push_back(pCluster);
		//delete pCluster;//���

		////////////////�������öԻ���2014-09-19//////////////////////////
		CDialogClusterManage::Show();
		////////////////////////////////////////////////////////////////////////////
		//XT_CREATE_CLUSTER_INFO *CreateClusterInfo = NULL;	
		//if((CreateClusterInfo = (XT_CREATE_CLUSTER_INFO*)malloc(sizeof(XT_CREATE_CLUSTER_INFO)))==NULL) 
		//{
		//	return ;
		//}
		//memset(CreateClusterInfo, 0, sizeof(CreateClusterInfo));
		//CreateClusterInfo->nCid = ack->nFamilyID; //Ⱥ��

		CString  szFamilyName = _T("");
		CString strClusterName = m_pEditClusterName->GetText().GetData();
		strClusterName.Trim();
		szFamilyName.Format(__T("%s"),strClusterName);//Ⱥ����
		int nLen = szFamilyName.GetLength();
		if (nLen > 0)
		{
			memcpy(pCluster->szFamilyName, (void*)szFamilyName.GetBuffer(nLen), nLen);
			pCluster->szFamilyName[nLen] = '\0';
		}
		extern HWND g_hClusterManage;
		if(0 == ::PostMessage(g_hClusterManage,WM_CLUSTER_MANAGE_MSG,1,(LPARAM)pCluster))
		{
			free(pCluster);
		}
		///////////////////////////////////////////////////////////////////////////

		///�޸�������Ⱥ������
		::SendMessage(CMainFrame::GetInstance()->GetHWND(), WM_IMOPERATION,OPR_MODIFY_CLUSTER_NAME,(LPARAM)pCluster->nFamilyID);

		//////////////////////////////////////////////////////////////////
		// ����Ⱥ��Աǰ�жϳ�Ա�Ƿ�Ը�����Ⱥ

		//// ������Ⱥ��Աǰ���Ȳ�ѯ��Щ��Ա�Ƿ�Ը�����Ⱥ
		//char buff[MAX_SEND_BUFFERS];
		//XT_USERFAMILYFLAG_GET_REQ & req = *((LPXT_USERFAMILYFLAG_GET_REQ)buff);
		//req.count = m_clusterInfo.Count();
		//int i = 0;
		//for ( i = 0; i<req.count; i++ )
		//{
		//	req.userID[i] = m_clusterInfo.members[i].GetId();
		//}
		//g_pProtocol->QueryFriendFamilyFlag(req);
	}
	else//��Ⱥʧ��
	{
		/*KillTimer(0x01);

		m_pPage2->EnableWindow(TRUE);
		m_pPage1->EnableWindow(TRUE);

		m_btnNext.EnableWindow(TRUE);
		m_btnPrev.EnableWindow(TRUE);	*/

		CString strError;

		if ( nRetCode==SERVER_TIMEOUT )
		{
		strError = "����Ⱥ - ����������ʱ!";
		}
		else
		{
		strError = g_pProtocol->GetLastError();
		}

		ShowMessage(strError,MB_ICONINFORMATION|MB_OK);
	}

	Close(0);
}

void CCreateClusterDlg::UpdateBk()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			::UpdateBk(background, pInstance->GetHWND());
		}
	}
}
//
//// CClusterCreateDlg dialog
//#define WAIT_TIMER 0x01
//
//CClusterCreateDlg * CClusterCreateDlg::pInstance = NULL;
//
//CClusterCreateDlg::CClusterCreateDlg(CWnd* pParent /*=NULL*/)
//: CXSkinDlg(CClusterCreateDlg::IDD, pParent)
//{
//	//////�����ɫ����///////////////
//	m_colorBackground = CHAT_BKG_COLOR;
//	m_colorBorder = CHAT_BKG_COLOR;
//	DeleteObject(m_hBkGround);
//	m_hBkGround = ::CreateSolidBrush(m_colorBackground);
//
//
//	m_nStep      = 0;
//	m_pPage1     = NULL;
//	m_pPage2     = NULL;
//	m_bmpLogo.LoadBitmap(IDB_IM_STATUS100);
//	pInstance    = this;
//	m_nNextPos   = -1;
//	m_nSetCMembersSeq = 0;
//	m_nCluster = 0;
//}
//
//CClusterCreateDlg::~CClusterCreateDlg()
//{
//	pInstance = NULL;
//}
//
//
//void CClusterCreateDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CXSkinDlg::DoDataExchange(pDX);
//
//	DDX_Control(pDX, IDC_CANCEL, m_btnCancel);
//	DDX_Control(pDX, IDC_PREV, m_btnPrev);
//	DDX_Control(pDX, IDC_NEXT, m_btnNext);
//	DDX_Control(pDX, IDC_STATIC_PAGE, m_wndFrame);
//}
//
//
//BEGIN_MESSAGE_MAP(CClusterCreateDlg, CXSkinDlg)
//	ON_WM_DESTROY()
//	ON_XT_MESSAGE()
//	ON_BN_CLICKED(IDC_NEXT, OnBnClickedNext)
//	ON_BN_CLICKED(IDC_PREV, OnBnClickedPrev)
//	ON_BN_CLICKED(IDC_CANCEL, OnBnClickedCancel)
//	ON_WM_CTLCOLOR()
//	ON_WM_TIMER()
//	ON_WM_CLOSE()
//	ON_WM_PAINT()
//END_MESSAGE_MAP()
//
//BEGIN_XT_MESSAGE_MAP(CClusterCreateDlg)
//	ON_XT_COMMAND(CMD_CREATE_CLUSTER_ACK,OnCreateAck)
//	ON_XT_COMMAND(CMD_SET_CMEMBERS_ACK,OnSetCMemberAck)
//	ON_XT_COMMAND(CMD_USERFAMILYFLAG_GET_ACK,OnUserFamilyFlagAck)
//END_XT_MESSAGE_MAP()
//
///////////////////////////////////////////////////////////////////////////////
//// CClusterCreateDlg message handlers
//
//
//BOOL CClusterCreateDlg::OnInitDialog() 
//{
//	CXSkinDlg::OnInitDialog();
//
//	EnableHelp(true);
//
//	CRect rcPage;
//	m_wndFrame.GetClientRect(&rcPage);
//
//	SetDlgStyle(MINCLOSE);
//
//	m_pPage1 = new CClusterPage1(&m_wndFrame);
//	m_pPage2 = new CClusterPage2(&m_wndFrame);
//	m_pPage3 = new CClusterPage3(&m_wndFrame);
//
//	m_pPage1->Create(IDD_WIZARD_CLUSTER_PAGE1,&m_wndFrame);
//	m_pPage1->MoveWindow(rcPage);
//	m_pPage1->ShowWindow(SW_HIDE);
//
//	m_pPage2->Create(IDD_WIZARD_CLUSTER_PAGE2,&m_wndFrame);
//	m_pPage2->MoveWindow(rcPage);
//	m_pPage2->ShowWindow(SW_HIDE);
//
//	m_pPage3->Create(IDD_WIZARD_CLUSTER_PAGE3,&m_wndFrame);
//	m_pPage3->MoveWindow(rcPage);
//	m_pPage3->ShowWindow(SW_HIDE);
//
//	//ȡ������һ��������ť�ݲ���ʾ
//	m_btnPrev.ShowWindow(SW_HIDE);
//	m_btnCancel.ShowWindow(SW_HIDE);
//
//	CenterWindow(GetDesktopWindow());
//
//	InitPara();
//
//	UpdatePage();
//
//	g_pProtocol->RegistCmd(CMD_CREATE_CLUSTER_ACK,m_hWnd);
//	g_pProtocol->RegistCmd(CMD_SET_CMEMBERS_ACK,m_hWnd);
//	g_pProtocol->RegistCmd(CMD_USERFAMILYFLAG_GET_ACK,m_hWnd);
//
//	return TRUE;  
//}
//
//void CClusterCreateDlg::OnHelpClick()
//{
//	OpenHtmlPage("www.feiniu.com");
//}
//
//void CClusterCreateDlg::InitPara() 
//{
//	m_nStep=0;
//	UpdatePage();
//}
//
////void CClusterCreateDlg::OnTimer(UINT_PTR nIDEvent)
////{
////	if ( nIDEvent==WAIT_TIMER )
////	{
////		static CString strDot = "";
////		static icount=0;
////
////		if (icount++>4)
////		{
////			icount=0;
////			strDot="";
////		}
////
////		strDot+=".";
////		CString strInfo ="ע����";
////		strInfo+=strDot;
////
////		SetDlgItemText(IDC_STATIC_INFO,strInfo);
////	}
////
////	CXSkinDlg::OnTimer(nIDEvent);
////}
//
//void CClusterCreateDlg::OnDestroy() 
//{
//	g_pProtocol->UnRegistCmd(CMD_REGIST_ACK,m_hWnd);
//	CXSkinDlg::OnDestroy();	
//
//	if ( m_pPage1 )
//	{
//		m_pPage1->DestroyWindow();
//		delete m_pPage1;
//		m_pPage1 = NULL;
//	}
//
//	if ( m_pPage2 )
//	{
//		m_pPage2->DestroyWindow();
//		delete m_pPage2;
//		m_pPage2 = NULL;
//	}
//
//	if ( m_pPage3 )
//	{
//		m_pPage3->DestroyWindow();
//		delete m_pPage3;
//		m_pPage3 = NULL;
//	}
//
//	if ( m_bModalDlg==false )
//	{
//		delete this;
//	}
//}
//
//BOOL CClusterCreateDlg::PreTranslateMessage(MSG* pMsg) 
//{
//	if (pMsg->message == WM_KEYDOWN)
//	{
//		switch (pMsg->wParam)
//		{
//		case VK_ESCAPE:
//		case 0x0D:
//			return TRUE;
//		default:
//			break;
//		}
//	}
//
//	return CXSkinDlg::PreTranslateMessage(pMsg);
//}
//
//void CClusterCreateDlg::OnBnClickedNext()
//{
//	switch ( m_nStep )
//	{
//	case 0:
//		if ( !m_pPage1->GetClusterInfo(m_clusterInfo) )
//		{
//			return ;
//		}
//		break;
//	case 1:
//		if ( !m_pPage2->GetClusterInfo(m_clusterInfo) )
//		{
//			return ;
//		}
//		break;
//	case 2:
//		if ( !m_pPage3->GetClusterInfo(m_clusterInfo) )
//		{
//			return ;
//		}
//		break;
//	default:
//		break;
//	}		
//		
//	//if ( m_nStep==2 )
//	if (m_nStep == 0)
//	{
//
//		//���
//
//		XT_CREATE_CLUSTER_REQ req;
//		req.nJoinFlag = m_clusterInfo.nJoinFlag;
//		strcpy(req.szFamilyDesc,m_clusterInfo.szFamilyDesc);
//		strcpy(req.szFamilyName,m_clusterInfo.szFamilyName);
//		strcpy(req.szFamilyDomain,m_clusterInfo.szFamilyDomain);
//		req.nFamilyProp = PERM_FAMILY;//�̶�Ⱥ
//		req.nFamilyClass = m_clusterInfo.nFamilyClass;
//
//
//		m_strTip = "���ڴ���Ⱥ";
//
//		g_pProtocol->SendReq(req);
//
//		SetTimer(0x01,350,NULL);
//
//		m_pPage2->EnableWindow(FALSE);
//		m_pPage1->EnableWindow(FALSE);
//
//		m_btnNext.EnableWindow(FALSE);
//		m_btnPrev.EnableWindow(FALSE);
//	}
//	else
//	{
//		m_nStep++;
//		UpdatePage();
//	}
//}
//
//void CClusterCreateDlg::OnBnClickedPrev()
//{
//	m_nStep--;
//	SetDlgItemText(IDC_STATIC_INFO,"");
//	UpdatePage();
//}
//
//void CClusterCreateDlg::OnBnClickedCancel()
//{
//	DestroyWindow();
//}
//
//void CClusterCreateDlg::OnSetCMemberAck(const char *data, int nRetCode)
//{
//	LPXT_SET_CMEMBERS_ACK ack = (LPXT_SET_CMEMBERS_ACK)data;
//	// ������ص�Ӧ���Ǳ����巢�������Ӧ���򷵻�
//	if(m_nSetCMembersSeq != g_pProtocol->GetLastAckSequenceId())
//		return;
//	if ( nRetCode==0 )
//	{
//		for ( int i=0; i<m_vecPos.size(); i++ )
//		{
//			if ( g_pProtocol->GetLastAckSequenceId()==m_vecPos[i] )
//			{
//				return ;
//			}
//		}
//
//		m_vecPos.push_back(g_pProtocol->GetLastAckSequenceId());
//
//		if (m_nNextPos != -1 && m_nNextPos<m_clusterInfo.Count() )
//		{
//			int nNextPos = m_nNextPos+10;
//			if ( nNextPos>m_clusterInfo.Count() )
//				nNextPos = m_clusterInfo.Count();
//
//			char buff[MAX_SEND_BUFFERS];
//			XT_SET_CMEMBERS_REQ  & req = *((LPXT_SET_CMEMBERS_REQ)buff);
//
//			req.nAction = CMEMBER_ADD         ;
//			req.nFamilyID = ack->nFamilyID ;
//			req.nFamilyProp = PERM_FAMILY  ;
//			req.nCount = nNextPos-m_nNextPos  ;
//			int j = 0;
//			for ( int i=m_nNextPos; i<nNextPos; i++,j++ )
//			{
//				req.nMemID[j]=m_clusterInfo.members[i].GetId();
//			}
//
//			m_clusterInfo.nFamilyID = ack->nFamilyID;
//
//			g_pProtocol->SendReq(req);
//			m_strTip.Format("������ӳ�Ա(%d/%d)",m_nNextPos,m_clusterInfo.Count());
//		
//			m_nNextPos = nNextPos;
//		}
//		else
//		{
//			KillTimer(0x01);
//
//			m_pPage2->EnableWindow(TRUE);
//			m_pPage1->EnableWindow(TRUE);
//
//			m_btnNext.EnableWindow(TRUE);
//			m_btnPrev.EnableWindow(TRUE);	
//
//			m_strTip.Format("Ⱥ %s �Ѿ��ɹ�����!",this->m_clusterInfo.szFamilyName);
//			SetDlgItemText(IDC_STATIC_INFO,m_strTip);
//		
//			CLUSTER_INFO * pCluster = FindClusterInfo(ack->nFamilyID);
//			if(pCluster)
//			{
//				pCluster->bMemberInit = false;
//				pCluster->nCreatorID  = g_ImUserInfo.GetId();
//			}
//
//			m_vecPos.clear();
//			DestroyWindow();
//		}
//	}
//	else
//	{
//		KillTimer(0x01);
//
//		m_nNextPos = -1;
//		m_vecPos.clear();
//
//		m_pPage2->EnableWindow(TRUE);
//		m_pPage1->EnableWindow(TRUE);
//
//		m_btnNext.EnableWindow(TRUE);
//		m_btnPrev.EnableWindow(TRUE);	
//
//		if ( nRetCode==SERVER_TIMEOUT )
//		{
//			SetDlgItemText(IDC_STATIC_INFO,"����������ʱ!");
//			ShowMessage("����������ʱ!");
//		}
//		else
//		{
//			SetDlgItemText(IDC_STATIC_INFO,g_pProtocol->GetLastError());
//			ShowMessage(g_pProtocol->GetLastError());
//		}
//	}
//
//	XT_GET_CINFO_REQ getCInf;
//	getCInf.nFamilyID   = ack->nFamilyID;
//	getCInf.nFamilyProp = PERM_FAMILY;
//	g_pProtocol->SendReq(getCInf);
//
//}
//
//void CClusterCreateDlg::OnCreateAck(const char *data,int nRetCode)
//{
//	LPXT_CREATE_CLUSTER_ACK ack = (LPXT_CREATE_CLUSTER_ACK)data;
//
//	if ( nRetCode==0 )
//	{
//		m_strTip.Format("Ⱥ %s �Ѿ��ɹ�����!",this->m_clusterInfo.szFamilyName);
//		SetDlgItemText(IDC_STATIC_INFO,m_strTip);
//		RegUserOperation(0x00B0);
//		
//		// ��¼��Ⱥ��id
//		m_nCluster = ack->nFamilyID;
//		// ���½���
//		CLUSTER_INFO *pCluster = new CLUSTER_INFO();
//		pCluster->nFamilyID = ack->nFamilyID;
//		pCluster->bMemberInit = false;
//		pCluster->nCreatorID  = g_ImUserInfo.GetId();
//		//////////Ⱥ�������֤��ʽ��ȡ/////////////////////////////////
//		////pCluster->nFamilyClass =  ComboGetFamilyClass(&(m_pPage1->m_comboType));
//		pCluster->nFamilyClass =  GetClusterType(&(m_pPage1->m_comboType));    //Ⱥ�����ȡ
//		pCluster->nJoinFlag    =  m_pPage1->GetClusterRadioBtnChecked();//��֤��ʽ
//		g_vecCluster.push_back(pCluster);
//		//delete pCluster;//���
//
//	    ////////////////�������öԻ���2014-09-19//////////////////////////
//		CDialogClusterManage *pDlg=new CDialogClusterManage;
//		pDlg->Show();
//		////////////////////////////////////////////////////////////////////////////
//		//XT_CREATE_CLUSTER_INFO *CreateClusterInfo = NULL;	
//		//if((CreateClusterInfo = (XT_CREATE_CLUSTER_INFO*)malloc(sizeof(XT_CREATE_CLUSTER_INFO)))==NULL) 
//		//{
//		//	return ;
//		//}
//		//memset(CreateClusterInfo, 0, sizeof(CreateClusterInfo));
//		//CreateClusterInfo->nCid = ack->nFamilyID; //Ⱥ��
//		
//		CString  szFamilyName = _T("");
//		szFamilyName.Format(__T("%s"),this->m_clusterInfo.szFamilyName);//Ⱥ����
//		int nLen = szFamilyName.GetLength();
//		if (nLen > 0)
//		{
//			memcpy(pCluster->szFamilyName, (void*)szFamilyName.GetBuffer(nLen), nLen);
//			pCluster->szFamilyName[nLen] = '\0';
//		}
//	    extern HWND g_hClusterManage;
//		if(0 == ::PostMessage(g_hClusterManage,WM_CLUSTER_MANAGE_MSG,1,(LPARAM)pCluster))
//		{
//		    free(pCluster);
//		}
//		///////////////////////////////////////////////////////////////////////////
//        delete pDlg;
//		//////////////////////////////////////////////////////////////////
//
//		if ( m_nNextPos == -1 )
//			m_nNextPos = 200;
//
//		if ( m_nNextPos > m_clusterInfo.Count() )
//			m_nNextPos = m_clusterInfo.Count();
//
//
//		// ����Ⱥ��Աǰ�жϳ�Ա�Ƿ�Ը�����Ⱥ
//
//		// ������Ⱥ��Աǰ���Ȳ�ѯ��Щ��Ա�Ƿ�Ը�����Ⱥ
//		char buff[MAX_SEND_BUFFERS];
//		XT_USERFAMILYFLAG_GET_REQ & req = *((LPXT_USERFAMILYFLAG_GET_REQ)buff);
//		req.count = m_clusterInfo.Count();
//		int i = 0;
//		for ( i = 0; i<req.count; i++ )
//		{
//			req.userID[i] = m_clusterInfo.members[i].GetId();
//		}
//		g_pProtocol->QueryFriendFamilyFlag(req);
//	}
//	else//��Ⱥʧ��
//	{
//		KillTimer(0x01);
//
//		m_pPage2->EnableWindow(TRUE);
//		m_pPage1->EnableWindow(TRUE);
//
//		m_btnNext.EnableWindow(TRUE);
//		m_btnPrev.EnableWindow(TRUE);	
//
//		CString strError;
//
//		if ( nRetCode==SERVER_TIMEOUT )
//		{
//			strError = "����Ⱥ - ����������ʱ!";
//		}
//		else
//		{
//			strError = g_pProtocol->GetLastError();
//		}
//
//		ShowMessage(strError,MB_ICONINFORMATION|MB_OK);
//		SetDlgItemText(IDC_STATIC_INFO,"");
//	}
//}
//
//void CClusterCreateDlg::UpdatePage()
//{
//	switch (m_nStep)
//	{
//	case 0:
//		SetWindowText("�½�Ⱥ");
//		m_btnPrev.EnableWindow(FALSE);
//		m_btnNext.SetWindowText("�ύ");
//		m_pPage3->ShowWindow(SW_HIDE);
//		m_pPage2->ShowWindow(SW_HIDE);
//		m_pPage1->ShowWindow(SW_SHOW);//��ʾ����Ⱥ�˵�
//		break;
//	case 1:
//		SetWindowText("Ⱥ -����Ա�б�");
//		m_btnPrev.EnableWindow(TRUE);
//		m_btnNext.SetWindowText("��һ��");
//		m_pPage1->ShowWindow(SW_HIDE);
//		m_pPage3->ShowWindow(SW_HIDE);
//		m_pPage2->ShowWindow(SW_SHOW);//��ʾȺ����Ա�б�
//		break;
//	case 2:
//		SetWindowText("Ⱥ -����֤����");
//		m_btnPrev.EnableWindow(TRUE);
//		m_btnNext.SetWindowText("���");
//		m_pPage1->ShowWindow(SW_HIDE);
//		m_pPage2->ShowWindow(SW_HIDE);
//		m_pPage3->ShowWindow(SW_SHOW);//��ʾȺ��֤����
//		break;
//	}
//}
//
//HBRUSH CClusterCreateDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	if ( nCtlColor==CTLCOLOR_STATIC )
//	{
//		hbr = m_hBkGround;
//		pDC->SetBkMode(TRANSPARENT);
//	}
//
//	return hbr;
//}
//
//void CClusterCreateDlg::OnTimer(UINT nIDEvent)
//{
//	if ( nIDEvent==0x01 )
//	{
//		static int count=0;
//		static CString strDot="";
//
//		strDot+=".";
//
//		if ( count++>4 )
//		{
//			strDot="";
//			count=0;
//		}
//
//		SetDlgItemText(IDC_STATIC_INFO,m_strTip+strDot);
//	}
//
//	CXSkinDlg::OnTimer(nIDEvent);
//}
//
//void CClusterCreateDlg::OnClose()
//{
//	DestroyWindow();
//}
//
//void CClusterCreateDlg::OnPaint()
//{
//	if( !::IsWindow(GetSafeHwnd()) )
//	{
//		return;
//	}
//
//	CPaintDC dc(this); 
//	CXSkinDlg::OnPaint();
//
//	if (m_wndFrame.m_hWnd)
//	{
//		CRect rc;
//		m_wndFrame.GetWindowRect(rc);
//		ScreenToClient(&rc);
//
//		rc.InflateRect(1,1);
//		dc.Draw3dRect(&rc,IM_LINE_COLOR,IM_LINE_COLOR);
//	}
//
//	CWnd *pWnd = GetDlgItem(IDC_STATIC_LOGO);
//
//	if ( pWnd )
//	{
//		CRect rc;
//		pWnd->GetWindowRect(rc);
//		ScreenToClient(&rc);
//
//		DrawBitmap(&dc,rc.left,rc.top,&m_bmpLogo);
//	}
//
//}
//
//void CClusterCreateDlg::OnUserFamilyFlagAck(const char *data, WORD wRet)
//{
//	XT_USERFAMILYFLAG_GET_ACK *pAck = (XT_USERFAMILYFLAG_GET_ACK*)data;
//	if(wRet == 0)
//	{
//		char buff[MAX_SEND_BUFFERS];
//		int nC = pAck->count;
//		int nAccept = 0;
//		// 
//		int nRefuse = 0;
//		uint32 id;
//
//		XT_SET_CMEMBERS_REQ  & req = *((LPXT_SET_CMEMBERS_REQ)buff);
//		int i;
//		req.nFamilyID   = m_nCluster;
//		req.nAction     = CMEMBER_ADD;
//		req.nFamilyProp = PERM_FAMILY;
//		
//		for ( i = 0; i< nC; i++ )
//		{
//			if(pAck->JoinFlag[i].flag == 0)
//			{
//				req.nMemID[nAccept] = pAck->JoinFlag[i].uID;
//				nAccept++;
//			}
//			else
//			{
//				id = pAck->JoinFlag[i].uID;
//				nRefuse++;
//			}
//		}
//		// �ж��Ƿ����˾ܾ�����Ⱥ
//		if(nRefuse == 1)
//		{
//			CString sMsg;
//			FRIEND_INFO * pUser = ::GetFriendInfo(id);
//			if(pUser)
//			{
//				sMsg.Format("�û�%s�ܾ�����Ⱥ",pUser->nickname);
//			}
//			else
//			{
//				sMsg.Format("�û�%d�ܾ�����Ⱥ",id);
//			}
//			ShowMessage(sMsg);
//		}
//		else if(nRefuse > 1)
//		{
//			CString sMsg;
//			sMsg.Format("��%d���û��ܾ�����Ⱥ",nRefuse);
//			ShowMessage(sMsg);
//		}
//		if(nAccept > 0)
//		{
//			req.nCount      = nAccept;
//			m_strTip.Format("������ӳ�Ա(0/%d)",m_clusterInfo.Count());
//			SetDlgItemText(IDC_STATIC_INFO,m_strTip);
//			m_nSetCMembersSeq = g_pProtocol->GetCurSendSequnceId();
//			g_pProtocol->SendReq(req);
//		}
//		else
//		{
//			XT_GET_CINFO_REQ getCInf;
//			getCInf.nFamilyID   = m_nCluster;
//			getCInf.nFamilyProp = PERM_FAMILY;
//			g_pProtocol->SendReq(getCInf);
//
//			DestroyWindow();
//		}
//	}
//}
