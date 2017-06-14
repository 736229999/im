// ClusterParamSetting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClusterParamSetting.h"
#include "afxdialogex.h"
#include "xskindlg.h"
#include "ModHeadIconDlg.h"
#include "MainFrame.h"

enum RADIO_CHECKED
{
    RADIO_ALL_CHECKED = 0,
	RADIO_VERIFY_CHECKED,
	RADIO_NONE_CHECKED,
};

#define VALUE_COLOR  RGB(0,0,0)
#define MAX_FACE_SHOW_SIZE 300
// CClusterParamSetting �Ի���
#define  TIMER_UPDATE_CLUSTER_INFO 1
CClusterParamSetting * CClusterParamSetting::pInstance = NULL;
IMPLEMENT_DYNAMIC(CClusterParamSetting, CXSkinDlg)


ThParam m_thParam;




CClusterParamSetting::CClusterParamSetting(CLUSTER_INFO *pInfo, CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CClusterParamSetting::IDD, pParent)
{
	//////�����ɫ����///////////////
	m_colorBackground = RGB(235, 235, 235);
	m_colorBorder = CHAT_BKG_COLOR;
	DeleteObject(m_hBkGround);
	m_hBkGround = ::CreateSolidBrush(m_colorBackground);
	SetDlgStyle(CXSkinDlg::CLOSE);

	m_clusterInfo = pInfo;
	m_pComboBox   = NULL;

	m_font.CreateFont(
		19,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("΢���ź�"));

	// tab����
	m_colMainTab.SetDirectMode(CColumnCtrl::EN_Y_DIRE);
	m_colMainTab.SetFont(&m_font);
	m_colMainTab.AddItem("Ⱥ����");
	m_colMainTab.AddItem("Ⱥ��Ա");
	m_colMainTab.AddItem("��������");
	m_colMainTab.AddItem("��ȫ����");
	m_colMainTab.SetBackgroundColor(RGB(235, 235, 235), RGB(255, 255, 255), RGB(245, 245, 245));
	m_colMainTab.SetItemTextColor(RGB(0,0,0), RGB(0,0,0), RGB(0,0,0));
	m_colMainTab.SetFrameModol(FRAME_DRAW_NULL);

	m_bSecurityCfg = TRUE;
	m_pFacePic = NULL;
	m_pModHeadDlg = NULL;
}

CClusterParamSetting::~CClusterParamSetting()
{
	if(m_pFacePic != NULL)
	{
		delete m_pFacePic;
		m_pFacePic = NULL;
	}
	if(m_pClusterInfo != NULL)
	{
		if(::IsWindow(m_pClusterInfo->GetSafeHwnd()))
		{
			m_pClusterInfo->DestroyWindow();
		}
		delete m_pClusterInfo;
		m_pClusterInfo = NULL;
	}
	if(m_pClusterMember != NULL)
	{
		if(::IsWindow(m_pClusterMember->GetSafeHwnd()))
		{
			m_pClusterMember->DestroyWindow();
		}
		delete m_pClusterMember;
		m_pClusterMember = NULL;
	}
	if(m_pClusterProfile != NULL)
	{
		if(::IsWindow(m_pClusterProfile->GetSafeHwnd()))
		{
			m_pClusterProfile->DestroyWindow();
		}
		delete m_pClusterProfile;
		m_pClusterProfile = NULL;
	}
	if(m_pClusterSecurity != NULL)
	{
		if(::IsWindow(m_pClusterSecurity->GetSafeHwnd()))
		{
			m_pClusterSecurity->DestroyWindow();
		}
		delete m_pClusterSecurity;
		m_pClusterSecurity = NULL;
	}
	if(m_pModHeadDlg != NULL)
	{
		delete m_pModHeadDlg;
		m_pModHeadDlg = NULL;
	}
}

void CClusterParamSetting::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_PHOTO, m_btnPhoto);
	DDX_Control(pDX, IDC_OK, m_btnOK);
	DDX_Control(pDX, IDC_CLOSE, m_btnCancel);
	DDX_Control(pDX, IDC_STATIC_MAIN_TAB, m_colMainTab);
	//DDX_Control(pDX, IDC_STATIC_ICON, m_ctrlPicture);
}


BEGIN_MESSAGE_MAP(CClusterParamSetting, CXSkinDlg)
	ON_BN_CLICKED(IDC_CLOSE, &CClusterParamSetting::OnBnClickedClose)
	ON_BN_CLICKED(IDC_BTN_PHOTO, &CClusterParamSetting::OnBnClickedBtnPhoto)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_OK, &CClusterParamSetting::OnBnClickedBtnOk)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_COLUMNCTRL_SEL_CHANGE, OnColumnctrlSelChange)
	ON_MESSAGE(WM_MOD_HEAD_DLG_MSG, OnModHeadDlgMsg)
END_MESSAGE_MAP()
//
BEGIN_XT_MESSAGE_MAP(CClusterParamSetting)
//	ON_XT_COMMAND(CMD_GET_CINFO_ACK,OnGetClusterInfoAck)
//	//ON_XT_COMMAND(CMD_GET_CMEMBERS_ACK,OnClusterMembersAck)
//	//ON_XT_COMMAND(CMD_CMEMBERS_IND,OnCMemberInd)
	ON_XT_COMMAND(CMD_SET_CINFO_ACK, OnSetClusterInfoAck)
	ON_XT_COMMAND(CMD_SET_CMEMBERS_ACK,OnSetCMemberAck)
	ON_XT_COMMAND(CMD_SET_PROFILE_NAME_ACK, OnGetCMProfileAck)
	
//	//ON_XT_COMMAND(CMD_LOGOUT,OnLoginOut)
END_XT_MESSAGE_MAP()
// CClusterParamSetting ��Ϣ�������

BOOL CClusterParamSetting::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	// ���������ӿؼ������ÿؼ�����
	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
	while(hWndChild)  
	{  
		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
	}

	m_btnPhoto.SetBitmap(IDB_BMP_GRAY_BTN_HOVER_L, IDB_BMP_GRAY_BTN_NORMAL_L, IDB_BMP_GRAY_BTN_PRESSED_L, IDB_BMP_GRAY_BTN_PRESSED_L);
	m_btnPhoto.SetTextColor(RGB(0,0,0));
	m_btnPhoto.ShowWindow(SW_HIDE);		// �����޸�Ⱥͷ����
	m_btnOK.SetBitmap(IDB_BMP_SENIOR_BTN_HOVER, IDB_BMP_SENIOR_BTN_NORMAL, IDB_BMP_SENIOR_BTN_PRESSED, IDB_BMP_SENIOR_BTN_PRESSED);
	m_btnOK.SetTextColor(RGB(255, 255, 255));
	m_btnCancel.SetPng(IDB_PNG_GRAYBTN_S_HOVER, IDB_PNG_GRAYBTN_S_NORMAL, IDB_PNG_GRAYBTN_S_PRESSED, IDB_PNG_GRAYBTN_S_PRESSED);
	m_btnCancel.SetTextColor(RGB(0,0,0));

 	g_pProtocol->RegistCmd(CMD_SET_CMEMBERS_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SET_CINFO_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SET_PROFILE_NAME_ACK, m_hWnd);
	g_pProtocol->RegistCmd(CMD_CDISMISS_ACK, m_hWnd);

	// �����ؼ�λ��
	CRect rcClient;
	GetClientRect(&rcClient);

	CRect rcMainTab;
	m_colMainTab.GetWindowRect(&rcMainTab);
	// ͷ��ؼ�
	m_pFacePic = new CRountFaceCtrl(IDB_PNG_BIG_BROUND_NORMAL, IDB_PNG_BIG_BROUND_NORMAL);
	m_pFacePic->Create(this, 50001);
	CRect rcPic;
	m_pFacePic->GetWindowRect(&rcPic);
	ScreenToClient(&rcPic);
	CRect rcPicNew;
	int nC = (rcMainTab.Width()- rcPic.Width())/2;
	rcPicNew.left = nC;
	rcPicNew.right = rcPicNew.left + rcPic.Width();
	rcPicNew.top = nC + TITLE_BAR_CY;
	rcPicNew.bottom = rcPicNew.top + rcPic.Height();
	m_pFacePic->MoveWindow(&rcPicNew);
	m_pFacePic->ShowWindow(SW_SHOW);
	// ��ť
	CRect rcBtn;
	m_btnPhoto.GetWindowRect(&rcBtn);
	ScreenToClient(&rcBtn);
	int nHeight = rcBtn.Height();
	int nWidth = rcBtn.Width();
	rcBtn.left = nC;
	rcBtn.right = rcBtn.left + nWidth;
	rcBtn.top = rcMainTab.Width() + TITLE_BAR_CY;
	rcBtn.bottom = rcBtn.top + nHeight;
	m_btnPhoto.MoveWindow(&rcBtn);
	// tab��
	ScreenToClient(&rcMainTab);
	nWidth = rcMainTab.Width();
	rcMainTab.top = rcBtn.bottom + nC;
	rcMainTab.bottom = rcClient.bottom - 1;
	rcMainTab.left = 1;
	rcMainTab.right = rcMainTab.left + nWidth;
	m_colMainTab.MoveWindow(&rcMainTab);
	// ��ҳ������
	CRect rcWorkArea;
	GetDlgItem(IDC_STATIC_FRAME_VIEW)->GetWindowRect(&rcWorkArea);
	ScreenToClient(&rcWorkArea);
	rcWorkArea.top = TITLE_BAR_CY + 1;
	rcWorkArea.left = rcMainTab.right;
	rcWorkArea.right = rcClient.right - 1;
	GetDlgItem(IDC_STATIC_FRAME_VIEW)->MoveWindow(&rcWorkArea);
	return TRUE;
}

void CClusterParamSetting::CreateDialogBox(CLUSTER_INFO *pInfo)
{
	CRect rect;
	GetDlgItem(IDC_STATIC_FRAME_VIEW)->GetClientRect(&rect);
	GetDlgItem(IDC_STATIC_FRAME_VIEW)->MapWindowPoints(this, rect);

	m_pClusterInfo = new CClusterInfo;
	m_pClusterInfo->Create(IDD_CLUSTER_INFO, this);
	m_pClusterInfo->MoveWindow(rect);
	m_pClusterInfo->ShowWindow(SW_SHOW);

	m_pClusterMember  = new CClusterMember;
	m_pClusterMember->Create(IDD_CLUSTER_MEMBER, this);
	m_pClusterMember->MoveWindow(rect);
	m_pClusterMember->ShowWindow(SW_HIDE);

	m_pClusterProfile  = new CClusterProfile(pInfo,GetDesktopWindow());
	m_pClusterProfile->Create(IDD_CLUSTER_PROFILE, this);
	m_pClusterProfile->MoveWindow(rect);
	m_pClusterProfile->ShowWindow(SW_HIDE);

	m_pClusterSecurity = new CClusterSecurity;
	m_pClusterSecurity->Create(IDD_CLUSTER_SECURITY, this);
	m_pClusterSecurity->MoveWindow(rect);
	m_pClusterSecurity->ShowWindow(SW_HIDE);

	m_colMainTab.SetCurSel(0);	
}

void CClusterParamSetting::OnBnClickedBtnOk()
{
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�޸�Ⱥ��Ϣ���ύ��Ⱥ��Ϣ��Ⱥ��֤���ύ
	////////////////////////////////////////////////////////////////////
	CString strTxt = _T("");
	(m_pClusterInfo->m_ctlClusterName).GetWindowText(strTxt);//Ⱥ����
	strTxt.Trim();
	if(strTxt=="")
	{
		CMessageBox::Prompt("Ⱥ���Ʋ���Ϊ��");
		m_colMainTab.SetCurSel(0);
		return;
	}

	// �ж�Ⱥ��Ƭ
	string strProfileName = m_pClusterProfile->GetProfileName();
	/*
	if(strProfileName == "������10��")
		strProfileName = "";
	*/
	XT_SET_CINFO_REQ m_cInfoReq;
	strcpy(m_cInfoReq.szFamilyName,strTxt);
	(m_pClusterInfo->m_ctlClusterInfo).GetWindowText(m_cInfoReq.szFamilyDesc,sizeof(m_cInfoReq.szFamilyDesc));//Ⱥ����
	(m_pClusterInfo->m_ctlClusterSign).GetWindowText(m_cInfoReq.szFamilySign,sizeof(m_cInfoReq.szFamilySign));//Ⱥǩ��
	strncpy(m_cInfoReq.szFamilyFaceUrl, strProfileName.c_str(), 10);
	

	m_cInfoReq.nFamilyClass = ComboGetFamilyClass(&(m_pClusterInfo->m_ctlComboKinds));////Ⱥ����
	if(m_bSecurityCfg)////ֻ��Ⱥ����Ȩ�޸İ�ȫ����
	{
	  m_cInfoReq.nJoinFlag    = m_pClusterSecurity->GetClusterRadioBtnChecked();///Ⱥ��֤
	}
	else
	{
       m_cInfoReq.nJoinFlag   = m_clusterInfo->nJoinFlag;///Ĭ������
	}

	m_cInfoReq.nFamilyProp = PERM_FAMILY;

	(m_pClusterInfo->m_ctlClusterId).GetWindowText(strTxt);//ȺID
	int nFamilyID = _ttol(strTxt);
	m_cInfoReq.nFamilyID   =  nFamilyID;
		
	CLUSTER_INFO * pCluster = FindClusterInfo(m_cInfoReq.nFamilyID);
	if(pCluster)
	{
		strcpy(m_cInfoReq.szFamilyNotice,pCluster->szFamilyNotice);////����

		pCluster->nJoinFlag = m_cInfoReq.nJoinFlag;
		strcpy(pCluster->szFamilyName,m_cInfoReq.szFamilyName);
		strcpy(pCluster->szFamilyDesc,m_cInfoReq.szFamilyDesc);
		strcpy(pCluster->szFamilySign,m_cInfoReq.szFamilySign);///Ⱥǩ��
		//strcpy(pCluster->szFamilyFaceUrl,m_cInfoReq.szFamilyFaceUrl);///�洢����Ⱥ��Ƭ
		pCluster->nFamilyClass = m_cInfoReq.nFamilyClass;///Ⱥ����
	}
	extern CIMProtocol * g_pProtocol;
	g_pProtocol->SendReq(m_cInfoReq);
	this->LockOperation();
	Sleep(50);
	//////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////////////////////////
	///////Ⱥ���촰�ڴӷ��������շ�����Ϣ//////////////////////////////////////////////////
	//extern HWND  g_hClusterSendMSG;
	//::PostMessage(g_hClusterSendMSG,WM_SET_HEAD_TITLE,1,(LPARAM)pCluster);

	///�޸�������Ⱥ������
	::SendMessage(CMainFrame::GetInstance()->GetHWND(), WM_IMOPERATION,OPR_MODIFY_CLUSTER_NAME,(LPARAM)pCluster->nFamilyID);
	// �˳�
	//////////////////////////////////////////////////////////////

	////���±�Ⱥ�и��˵�Ⱥ��Ƭ2015-03-03//////////////////////////////////////////////////////
	int nMemberCnt  =   pCluster->Count();
	for(int j = 0;j<nMemberCnt;j++)
	{
		FRIEND_INFO *pFriend = &pCluster->members[j];
		if ( pFriend->GetId()==g_ImUserInfo.GetId() )////���˵�Ⱥ��Ƭ�޸�
		{
			if(strProfileName != pFriend->profile_name)
			{	// Ⱥ��Ƭ���޸�
				XT_PROFILENAME_SET_REQ req;
				req.nFamilyID = pCluster->nFamilyID;/////ȺID
				req.nMemID = pFriend->GetId();         ////����ID
				strncpy(req.ProfileName, strProfileName.c_str(), 10);/////Ⱥ��Ƭ
				// ���Ͳ���
				g_pProtocol->SendReq(req);

				Sleep(50);
			}
			break;
		}
	}

	//����Ⱥ��Ƭ��ȡ����
	XT_PROFILENAME_GET_REQ reqMem;
	reqMem.nFamilyID   = pCluster->nFamilyID;
	g_pProtocol->SendReq(reqMem);
	Sleep(50);
	DestroyWindow();
}


void CClusterParamSetting::OnBnClickedBtnClusterinfo()
{
	m_pClusterInfo->ShowWindow(SW_SHOW);
	m_pClusterMember->ShowWindow(SW_HIDE);
	m_pClusterProfile->ShowWindow(SW_HIDE);
	m_pClusterSecurity->ShowWindow(SW_HIDE);
}


void CClusterParamSetting::OnBnClickedBtnClustermember()
{
	m_pClusterInfo->ShowWindow(SW_HIDE);
	m_pClusterMember->ShowWindow(SW_SHOW);
	m_pClusterProfile->ShowWindow(SW_HIDE);
	m_pClusterSecurity->ShowWindow(SW_HIDE);
	////m_pClusterMember->CreateListInfo(m_clusterInfo);
}

void CClusterParamSetting::OnBnClickedBtnProfile()
{
	m_pClusterInfo->ShowWindow(SW_HIDE);
	m_pClusterMember->ShowWindow(SW_HIDE);
	m_pClusterProfile->ShowWindow(SW_SHOW);
	m_pClusterSecurity->ShowWindow(SW_HIDE);
}


void CClusterParamSetting::OnBnClickedBtnSecurity()
{
	m_pClusterInfo->ShowWindow(SW_HIDE);
	m_pClusterMember->ShowWindow(SW_HIDE);
	m_pClusterProfile->ShowWindow(SW_HIDE);
	m_pClusterSecurity->ShowWindow(SW_SHOW);
}

void CClusterParamSetting::OnBnClickedClose()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	DestroyWindow();
}

void CClusterParamSetting::OnBnClickedBtnPhoto()
{
	if(m_pModHeadDlg != NULL)
	{
		delete m_pModHeadDlg;
		m_pModHeadDlg = NULL;
	}

	m_pModHeadDlg = new CModHeadIconDlg();
	m_pModHeadDlg->Create(this);
}

// �޸�ͷ�񴰿ڵ��Զ�����Ϣ
LRESULT CClusterParamSetting::OnModHeadDlgMsg(WPARAM wP, LPARAM lP)
{
	UINT id = (UINT)wP;
	if(id == IDOK)
	{
		string strHeadUrl = m_pModHeadDlg->GetHeadUrl();
		if(!strHeadUrl.empty())
		{
			/*FRIEND_INFO user = g_ImUserInfo;
			strcpy(user.face_url, strHeadUrl.c_str());
			g_pProtocol->SendReq((XT_USERINFO_MOD_REQ)user);*/
		}

	}

	if(id == IDOK || id == IDCANCEL)
	{
		delete m_pModHeadDlg;
		m_pModHeadDlg = NULL;
	}	

	return 0;
}

BOOL CClusterParamSetting::AdjustImgSize(CImage& img,CString& strNewFile)
{
	CImage imgNew;
	imgNew.Create(MAX_FACE_SHOW_SIZE,MAX_FACE_SHOW_SIZE,32);

	CRect rtSource(0,0,img.GetWidth(),img.GetHeight()) ;
	CRect rtDest(0,0,MAX_FACE_SHOW_SIZE,MAX_FACE_SHOW_SIZE);


	if( img.GetWidth()> img.GetHeight())
	{
		rtSource.left += (img.GetWidth()-img.GetHeight())/2;
		rtSource.right-= (img.GetWidth()-img.GetHeight())/2;
	}
	else
	{
		rtSource.top	+= (img.GetHeight()-img.GetWidth()	)/2;
		rtSource.bottom	-= (img.GetHeight()-img.GetWidth()	)/2;
	}

	//::SetStretchBltMode(img.GetDC(),STRETCH_HALFTONE);
	HDC hDc = imgNew.GetDC();
	::SetStretchBltMode( hDc,STRETCH_HALFTONE);
	img.StretchBlt(hDc,rtDest,rtSource);
	imgNew.ReleaseDC();

	CString strPhotoPath;
	char szRecentPhotoPath[MAX_PATH];
	GetTempPath(sizeof(szRecentPhotoPath) , szRecentPhotoPath );
	strPhotoPath = szRecentPhotoPath;

	if( strPhotoPath.Right(1) != '\\' )
	{
		strPhotoPath += '\\';
	}

	HRESULT hResult;
	static int i=0;
	strNewFile.Format("%sMini139Upload%d.jpg",( LPCTSTR)strPhotoPath,++i);
	::DeleteFile(strNewFile);
	hResult = imgNew.Save(strNewFile );

	if( FAILED(hResult))
	{
		return FALSE;
	}

	return TRUE;
}

CClusterParamSetting *CClusterParamSetting::Show(CLUSTER_INFO *pCluster)
{
	//if ( pInstance==NULL )
	//{
	//	pInstance = new CClusterParamSetting(pCluster,GetDesktopWindow());		
	//	//pInstance->UpdateClusterInfo(pInstance->m_clusterInfo);
	//	pInstance->ShowWindow(SW_SHOW);
	//}
	//else
	//{
	//	if (pInstance->IsIconic())
	//	{
	//		pInstance->ShowWindow(SW_RESTORE);
	//	}

	//	//memcpy(&pInstance->m_clusterInfo,pCluster,sizeof(CLUSTER_INFO));

	//	pInstance->UnLockOperation();
	//	//pInstance->CloseProgress();

	//	//pInstance->UpdateClusterInfo(pInstance->m_clusterInfo);
	//	pInstance->BringWindowToTop();
	//}
///////////////////////////////////////////////////////////////////////////
	DestroyDlg();

	pInstance = new CClusterParamSetting(pCluster,GetDesktopWindow());
	pInstance->Create(CClusterParamSetting::IDD,GetDesktopWindow());
	pInstance->ShowWindow(SW_SHOW);
	pInstance->CreateDialogBox(pCluster);
	pInstance->UpdateClusterInfo(pCluster);
	return pInstance;
/////////////////////////////////////////////////////////////////////////////
}


// ���ٴ���
void CClusterParamSetting::DestroyDlg()
{
	if(pInstance != NULL)
	{
		if(::IsWindow(pInstance->GetSafeHwnd()))
		{	// PostNcDestroyWindow�л��ͷŴ���ָ��
			pInstance->DestroyWindow();
		}
		else
		{
			delete pInstance;
			pInstance = NULL;
		}		
	}
}

void CClusterParamSetting::UpdateClusterInfo(CLUSTER_INFO *pCluster)
{
	/////�����ڱ�������
	CString strTitleTxt = _T("");
	strTitleTxt.Format(_T("Ⱥ����-%s(Ⱥ�ţ�%u)"),m_clusterInfo->szFamilyName, m_clusterInfo->nFamilyID);
	this->SetWindowText(strTitleTxt);

	/////�ӶԻ���Ⱥ��Ϣ��ʾ����
	CString strTxt = _T("");
	strTxt.Format(_T("%u"),pCluster->nFamilyID);
	(m_pClusterInfo->m_ctlClusterId).SetWindowText(strTxt);  //Ⱥ��
	
	FRIEND_INFO *pCreator = pCluster->GetCreator();

	if ( pCreator )
	{
		(m_pClusterInfo->m_ctlCluserOwner).SetWindowText(pCreator->nickname);//Ⱥ������
	}
	else
	{
		(m_pClusterInfo->m_ctlCluserOwner).SetWindowText(UIntToStr(m_clusterInfo->nCreatorID));
	}

	(m_pClusterInfo->m_ctlClusterName).SetWindowText(m_clusterInfo->szFamilyName);//Ⱥ����
	(m_pClusterInfo->m_ctlClusterSign).SetWindowText(m_clusterInfo->szFamilySign);//Ⱥǩ��
	(m_pClusterInfo->m_ctlClusterInfo).SetWindowText(m_clusterInfo->szFamilyDesc);//Ⱥ����

	m_pClusterInfo->UpdateComboBox(m_clusterInfo);//Ⱥ����
	m_pClusterProfile->OnUpdateUI(pCluster);
	//(m_pClusterProfile->m_edtClusterNickname).SetWindowText(m_clusterInfo->szFamilyFaceUrl);//Ⱥ��Ƭ

	///////////Ⱥ��Ƭ�޸�
	////////�ӶԻ���Ⱥ��Ա����
	m_pClusterMember->CreateListInfo(pCluster);

	////////�ӶԻ���Ⱥ��֤����
	int nPos = pCluster->nJoinFlag;//Ⱥ��֤
	if(RADIO_ALL_CHECKED == nPos)
	{
		m_pClusterSecurity->m_btnRadioAll.SetCheck(TRUE);
	}
	if(RADIO_VERIFY_CHECKED == nPos)
	{
         m_pClusterSecurity->m_btnRadioVerify.SetCheck(TRUE);
	}
	if(RADIO_NONE_CHECKED  == nPos)
	{
        m_pClusterSecurity->m_btnRadioNone.SetCheck(TRUE);
	}	
	switch(pCluster->dwClusterPageShow)////�����жϱ�ʶ
	{
		case 1:
			m_colMainTab.SetCurSel(0);
			//OnBnClickedBtnClusterinfo();//��Ϣ
			break;
		case 2:
			m_colMainTab.SetCurSel(1);
			//OnBnClickedBtnClustermember();//��Ա
			break;
		case 3:
			m_colMainTab.SetCurSel(2);
			//OnBnClickedBtnProfile();//����
			break;
		case 4:
			m_colMainTab.SetCurSel(3);
			//OnBnClickedBtnSecurity();//��ȫ
			break;
		default:
			break;
	}

    ///////�жϵ�ǰ�û���Ȩ�ޣ�Ⱥ��/����Ա/��Ա)
	extern BOOL g_bCreateFlag;
	if(g_bCreateFlag)
	{
		g_bCreateFlag = FALSE;
		m_pClusterInfo->m_ctlClusterName.SetReadOnly(FALSE);//�����޸�Ⱥ��Ϣ
		m_pClusterInfo->m_ctlClusterSign.SetReadOnly(FALSE);
		m_pClusterInfo->m_ctlClusterInfo.SetReadOnly(FALSE);
		m_pClusterInfo->m_ctlComboKinds.EnableWindow(TRUE);

		m_pClusterMember->m_btnSetAdmin.EnableWindow(TRUE);//Ⱥ��Ա����
		m_pClusterMember->m_btnCancelAdmin.EnableWindow(TRUE);
		m_pClusterMember->m_btnDelMember.EnableWindow(TRUE);
		m_pClusterMember->m_btnModifyInfo.EnableWindow(TRUE);

		//m_btnClusterSecurity.EnableWindow(TRUE);////��ȫ����
	}
	else
	{
		int nType = pCluster->GetMemberType(g_ImUserInfo.GetId());// 2��Ⱥ����1Ⱥ����Ա��0��ͨ��Ա
		if(nType!= 2)//����Ⱥ�����޸���Ȩ��
		{
			m_pClusterInfo->m_ctlClusterName.SetReadOnly(TRUE);//�鿴Ⱥ��Ϣ
			m_pClusterInfo->m_ctlClusterSign.SetReadOnly(TRUE);
			m_pClusterInfo->m_ctlClusterInfo.SetReadOnly(TRUE);
			m_pClusterInfo->m_ctlComboKinds.EnableWindow(FALSE);

			m_pClusterMember->m_btnSetAdmin.EnableWindow(FALSE);//Ⱥ��Ա����
			m_pClusterMember->m_btnCancelAdmin.EnableWindow(FALSE);
			m_pClusterMember->m_btnDelMember.EnableWindow(FALSE);
			m_pClusterMember->m_btnModifyInfo.EnableWindow(FALSE);
			//���г�Ա���������Ⱥ��Ա
			m_pClusterMember->m_btnAddMember.EnableWindow(TRUE);

			m_pClusterMember->m_btnSetAdmin.SetTextColor(VALUE_COLOR);//��ɫ����
			m_pClusterMember->m_btnCancelAdmin.SetTextColor(VALUE_COLOR);
			m_pClusterMember->m_btnDelMember.SetTextColor(VALUE_COLOR);
			m_pClusterMember->m_btnModifyInfo.SetTextColor(VALUE_COLOR);
			m_pClusterMember->m_btnAddMember.SetTextColor(VALUE_COLOR);
        
			m_bSecurityCfg = FALSE;
			m_pClusterSecurity->EnableWindow(FALSE);////��ȫ����Ϊ��
			//m_btnClusterSecurity.SetTextColor(VALUE_COLOR);////��ȫ����
		}
		else//�����޸�Ⱥ��Ϣ
		{
			m_pClusterInfo->m_ctlClusterName.SetReadOnly(FALSE);
			m_pClusterInfo->m_ctlClusterSign.SetReadOnly(FALSE);
			m_pClusterInfo->m_ctlClusterInfo.SetReadOnly(FALSE);
			m_pClusterInfo->m_ctlComboKinds.EnableWindow(TRUE);

			m_pClusterMember->m_btnSetAdmin.EnableWindow(TRUE);//Ⱥ��Ա����
			m_pClusterMember->m_btnCancelAdmin.EnableWindow(TRUE);
			m_pClusterMember->m_btnDelMember.EnableWindow(TRUE);
			m_pClusterMember->m_btnModifyInfo.EnableWindow(TRUE);

			//m_btnClusterSecurity.EnableWindow(TRUE);////��ȫ����
		}
	}
	

}

void CClusterParamSetting::OnPaint()
{
	CXSkinDlg::OnPaint();
}

void CClusterParamSetting::OnSetClusterInfoAck(const char *data,WORD wRet)
{
	LPXT_SET_CINFO_ACK ack = (LPXT_SET_CINFO_ACK)data;

	//UnLockOperation();
	//CloseProgress();

	if ( wRet==0 )
	{
		if ( ack->nFamilyID==m_clusterInfo->nFamilyID )
		{
			strncpy(m_clusterInfo->szFamilyName,m_cInfoReq.szFamilyName,sizeof(m_clusterInfo->szFamilyName)-1);
			strncpy(m_clusterInfo->szFamilyDesc,m_cInfoReq.szFamilyDesc,sizeof(m_clusterInfo->szFamilyDesc)-1);

			m_clusterInfo->nFamilyClass = m_cInfoReq.nFamilyClass;		
			m_clusterInfo->nJoinFlag    = m_cInfoReq.nJoinFlag;
		    UpdateClusterInfo(m_clusterInfo);
			ShowMessage("�޸ĳɹ�!");
		}
	}
	else
	{
		if ( wRet==SERVER_TIMEOUT )
		{
			ShowMessage("����������ʱ��");
		}
		else
		{
			ShowMessage(g_pProtocol->GetLastError());
			//UpdateClusterInfo();
		}
	}
}


HBRUSH CClusterParamSetting::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	UINT nId = pWnd->GetDlgCtrlID();
	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBkGround;
	}
	////////////////////////////////////////////////////////////////////
	return hbr;
}

/////Ⱥ��Ա����
void CClusterParamSetting::OnSetCMemberAck(const char *data, WORD wRet)
{
	LPXT_SET_CMEMBERS_ACK ack = (LPXT_SET_CMEMBERS_ACK)data;

	//if (wRet == 0 && ack->nFamilyID == this->m_clusterInfo.nFamilyID && ack->nFamilyProp == PERM_FAMILY)
	//{
	//	if (ack->nAction == CMEMBER_SET_ADMIN)//��Ϊ����Ա
	//	{
	//		RegUserOperation(0x00b5);

	//		for (int i = 0; i<m_lstCluster.GetCount(); i++)
	//		{
	//			FRIEND_INFO *pUser = (FRIEND_INFO *)m_lstCluster.GetItemData(i);

	//			if (pUser->id == ack->nMemberID)
	//			{
	//				pUser->cluster_property = 1;
	//				m_lstCluster.InvalidateItem(i);
	//				break;
	//			}
	//		}
	//	}

	//	if (ack->nAction == CMEMBER_CANCEL_ADMIN)//��������Ա
	//	{
	//		RegUserOperation(0x00b4);

	//		for (int i = 0; i<m_lstCluster.GetCount(); i++)
	//		{
	//			FRIEND_INFO *pUser = (FRIEND_INFO *)m_lstCluster.GetItemData(i);

	//			if (pUser->id == ack->nMemberID)
	//			{
	//				pUser->cluster_property = 0;
	//				m_lstCluster.InvalidateItem(i);
	//				break;
	//			}
	//		}
	//	}

	//	if (ack->nAction == CMEMBER_DEL)//ɾ����Ա
	//	{
	//		RegUserOperation(0x00b2);

	//		for (int i = 0; i<m_lstCluster.GetCount(); i++)
	//		{
	//			FRIEND_INFO *pUser = (FRIEND_INFO *)m_lstCluster.GetItemData(i);

	//			if (pUser->id == ack->nMemberID)
	//			{
	//				m_clusterInfo.DelMember(ack->nMemberID);

	//				m_lstCluster.ResetContent();

	//				for (int i = 0; i<m_clusterInfo.Count(); i++)
	//				{
	//					FRIEND_INFO *pFriend = &m_clusterInfo.members[i];

	//					int nItem = m_lstCluster.AddString(UserInfoToStr(pFriend));
	//					m_lstCluster.SetItemData(nItem, (DWORD)pFriend);
	//				}

	//				m_lstCluster.SortClusterList();
	//				break;
	//			}
	//		}
	//	}
	//}
}


////�޸ı���Ⱥ��Ƭ
void CClusterParamSetting::OnGetCMProfileAck(const char *data, WORD wRet)
{
	LPXT_PROFILENAME_SET_ACK ack = (LPXT_PROFILENAME_SET_ACK)data;

	if (wRet == 0 && ack->nFamilyID == this->m_clusterInfo->nFamilyID)
	{
		m_pClusterProfile->OnUpdateUI(m_clusterInfo);
		//(m_pClusterProfile->m_edtClusterNickname).SetWindowText(m_clusterInfo->szFamilyFaceUrl);//Ⱥ��Ƭ
	}
}

// �˵���ѡ�Ÿı��¼�
LRESULT CClusterParamSetting::OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam)
{
	CColumnCtrl *pCtrl = (CColumnCtrl*)wParam;
	int nSel = pCtrl->GetCurSel();
	switch(nSel)
	{
	case 0:
		{
			OnBnClickedBtnClusterinfo();
		}
		break;
	case 1:
		{
			OnBnClickedBtnClustermember();
		}
		break;
	case 2:
		{
			OnBnClickedBtnProfile();
		}
		break;
	case 3:
		{
			OnBnClickedBtnSecurity();
		}
		break;
	}

	return 0;
}


BOOL CClusterParamSetting::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message==WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			{
			}
			return TRUE;
			break;
		case VK_ESCAPE:
			return TRUE;
			break;
		default:
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CClusterParamSetting::PostNcDestroy()
{
	CXSkinDlg::PostNcDestroy();
	delete pInstance;
	pInstance = NULL;
}