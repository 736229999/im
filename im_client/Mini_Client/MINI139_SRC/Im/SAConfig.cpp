// SAConfig.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\saconfig.h"
#include "..\MainFrame.h"
#include "SelfFaceDlg.h"

// CSAConfig 对话框
#define TIMER_USERINFO_MOD 0x01

CSAConfig * CSAConfig::pInstance = NULL;

IMPLEMENT_DYNAMIC(CSAConfig, CXSkinDlg)
CSAConfig::CSAConfig(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CSAConfig::IDD, pParent)
{
	m_pPage1 = NULL;
	m_pPage2 = NULL;
	m_pPage3 = NULL;
	m_pPage4 = NULL;
	m_pPage6 = NULL;
	m_pPage7 = NULL;
	m_pPageVedio = NULL;
	m_pPageHotKey = NULL;
	m_pPage5 = NULL;

	m_nStartPage  = 0;
	m_nPreSelPage = -1;

	m_hBrushCaption = CreateSolidBrush(RGB(255,255,233));
	m_hBackGround   = CreateSolidBrush(IM_SPACE_COLOR);

	m_Font.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"宋体");

	Create(CSAConfig::IDD,pParent);
}

CSAConfig::~CSAConfig()
{
	delete m_pPage1;
	delete m_pPage2;
	delete m_pPage3;
	delete m_pPage4;
	delete m_pPage6;
	delete m_pPage7;
	delete m_pPageHotKey;
	delete m_pPageBasicSet;
	delete m_pPage5;
	delete m_pPageVedio;
    
	DeleteObject(m_hBrushCaption);
	DeleteObject(m_hBackGround);

	pInstance = NULL;
}

CSAConfig *  CSAConfig::Show(int nStartPage)
{
	if ( pInstance==NULL )
	{
		bool bInit=false;

		DWORD dwError = 0;
		while(bInit==false)
		{
			try
			{
				pInstance=new CSAConfig(GetDesktopWindow());
				pInstance->SetStartPage(nStartPage);
				pInstance->ShowWindow(SW_SHOW);
				bInit=true;
			}
			catch(...)
			{
				bInit=false;
			}
		}
	}
	else
	{
		pInstance->SetStartPage(nStartPage);
		if ( pInstance->IsWindowVisible()==false )
		{
			pInstance->ShowWindow(SW_SHOW);
		}

		if ( pInstance->IsIconic() )
		{
			pInstance->ShowWindow(SW_RESTORE);
		}
		
		pInstance->BringWindowToTop();
	}

	pInstance->UpdateUserInfo();
	return pInstance;
}

void CSAConfig::Close()
{
	if ( pInstance )
		pInstance->DestroyWindow();
}

void CSAConfig::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_STATIC_PAGE,m_wndFrame);
	DDX_Control(pDX, IDC_OK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CSAConfig, CXSkinDlg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_OK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_FOLDER, OnBnClickedFolder)
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_NCDESTROY()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_XT_MESSAGE()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_FACESEL_NOTIFY,OnFaceSelNotify)
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CSAConfig)
	ON_XT_COMMAND(CMD_USERINFO_MOD_ACK,OnUserInfoModified)
	ON_XT_COMMAND(CMD_LOGOUT,OnLogOut)
END_XT_MESSAGE_MAP()

// CSAConfig 消息处理程序

HBRUSH CSAConfig::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	UINT nID = pWnd->GetDlgCtrlID();

	HBRUSH hbr = CDialog::OnCtlColor(pDC,pWnd,nCtlColor);

	if ( nID==IDC_STATIC_INFO ||
		 nID==IDC_STATIC_PAGE )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBackGround;
	}

	if ( nCtlColor==CTLCOLOR_DLG )
	{
		hbr = m_hBackGround;
	}

	return hbr;
}

void CSAConfig::OnBnClickedOk()
{
	bool bCloseWindow = true;
	bool bFlag        = false;
	bool bMessageFlag = false;

	if ( g_ImUserInfo.status!=XTREAM_OFFLINE && (m_pPage1->IsModified() || m_pPage2->IsModified() || m_pPage3->IsModified()) )
	{
		//个人资料已经修改
		//
		if ( m_pPage1->GetUserInfo(m_UserInfo) &&
			 m_pPage2->GetUserInfo(m_UserInfo) &&
			 m_pPage3->GetUserInfo(m_UserInfo) )
		{
			
			if ( strcmpi(g_ImUserInfo.nickname,m_UserInfo.nickname)==0 )
			{
				strcpy(m_UserInfo.nickname,"");
				g_pProtocol->SendReq(m_UserInfo);
				strcpy(m_UserInfo.nickname,g_ImUserInfo.nickname);
			}
			else
			{
				g_pProtocol->SendReq(m_UserInfo);
			}


			this->m_btnOk.EnableWindow(false);
			m_strInfo = "正在向服务器提交个人资料";

			bFlag = true;

			SetTimer(TIMER_USERINFO_MOD,350,NULL);
		}
		bCloseWindow = false;
	}


	if ( m_pPage4->IsModified() )
	{
		if ( m_pPage4->SaveLoginSetting() )
		{
			bMessageFlag = true;	

			if ( bFlag )
			{
				bCloseWindow =false;
			}
		}
		else
		{
			ShowPage(3);
			bCloseWindow = false;
		}
	}

	if( m_pPageHotKey->IsModified() )
	{
		m_pPageHotKey->SaveConfig();

		bMessageFlag=true;
	}

	//接收文件路径
	if ( m_pPage5->IsModified() )
	{
		m_pPage5->SaveUserRecvFolder();

		bMessageFlag=true;
	}

	if(m_pPageBasicSet->IsModified())
	{
		m_pPageBasicSet->SaveConfig();

		bMessageFlag=true;
	}

	if ( m_pPage7->IsModified() )//个人空间设置 - 文章
	{
		m_pPage7->SaveZoneSetting();
		bMessageFlag = true;
	}

	if ( m_pPageVedio->IsModified() )
	{
		m_pPageVedio->SaveVedioSetting();
		bMessageFlag = true;
	}

	if (bCloseWindow)
	{
		if ( bMessageFlag )
		{
			ShowMessage("设置成功！");
		}

		DestroyWindow();
	}
}

void CSAConfig::OnHelpClick()
{
	OpenHtmlPage("www.feiniu.com");
}

LRESULT CSAConfig::OnFaceSelNotify(WPARAM wParam, LPARAM lParam)
{
	//if ( lParam==IDOK )
	//{
	//	OnBnClickedOk();
	//}
	//else
	//if ( lParam==IDCANCEL )
	//{
	//	OnBnClickedCancel();
	//}

	return 0;
}

BOOL CSAConfig::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetWindowText("哞哞 - 个人设置");

	SetDlgStyle(MINCLOSE);
	EnableHelp(true);

	CenterWindow(GetDesktopWindow());

	CRect rect;
	GetDlgItem(IDC_FOLDER)->GetWindowRect(&rect);
	ScreenToClient(&rect);

	this->m_wndFolderList.Create(this,rect);
	this->m_wndFolderList.SetDlgCtrlID(IDC_FOLDER);


	//m_wndFolderList.AddFolder("系统设置");

	m_pPage1 = new CSAPage1(&m_wndFrame);
	m_pPage2 = new CSAPage2(&m_wndFrame);
	m_pPage3 = new CSAPage3(&m_wndFrame);
	m_pPage4 = new CSAPage4(&m_wndFrame);
//	m_pPage5 = new CSAPage5(&m_wndFrame);//个人形象
	m_pPage6 = new CSAPage6(&m_wndFrame);
	m_pPage7 = new CSAPage7(&m_wndFrame);
	m_pPageVedio = new CSAPageVedio(&m_wndFrame);
	m_pPageHotKey = new CSAPageHotkey(&m_wndFrame);
	m_pPage5 = new CSAPage5(&m_wndFrame);
	m_pPageBasicSet = new CSAPageBasicSet(&m_wndFrame);
	m_pPageClusterMsgSet = new CSAPageClusterMsgSet(&m_wndFrame);



	m_btnCancel.SetBitmap(IDB_BTN1_HOT,IDB_BTN1_NORMAL,IDB_BTN1_DOWN,IDB_BTN1_DISABLE);
	m_btnOk.SetBitmap(IDB_BTN1_HOT,IDB_BTN1_NORMAL,IDB_BTN1_DOWN,IDB_BTN1_DISABLE);

	m_wndFrame.SetFont(&m_Font);

	g_pProtocol->RegistCmd(CMD_USERINFO_MOD_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_LOGOUT,m_hWnd);
    
	return TRUE; 
}

void CSAConfig::UpdateUserInfo()
{
	m_UserInfo = g_ImUserInfo;

	m_pPage1->SetUserInfo(m_UserInfo);
	m_pPage2->SetUserInfo(m_UserInfo);
	m_pPage3->SetUserInfo(m_UserInfo);
}

void CSAConfig::AddPage(CString strPage, CDialog *pPage, int iFolder)
{
	int nItem = m_wndFolderList.AddItem(strPage,iFolder);
	m_wndFolderList.SetItemData(nItem,(DWORD)pPage);

	CRect rc;
	m_wndFrame.GetClientRect(&rc);
	
	rc.InflateRect(-1,-1);

	pPage->MoveWindow(rc.left+1,rc.top+22,rc.Width()-3,rc.Height()-30);
	pPage->ShowWindow(SW_HIDE);
}


void CSAConfig::OnBnClickedFolder()
{
	int nCurSel = m_wndFolderList.GetCurSel();
	ShowPage(nCurSel);
}

void CSAConfig::SetStartPage(int nPage)
{
	m_nStartPage=nPage;
}

void CSAConfig::ShowPage(int nPage)
{
	CDialog *pPage=NULL;

	if ( m_nPreSelPage==nPage)
	{
		return ;
	}

	if ( m_nPreSelPage!=-1 )
	{
		pPage = (CDialog *)m_wndFolderList.GetItemData(m_nPreSelPage);
		pPage->ShowWindow(SW_HIDE);
	}

	pPage= (CDialog *)m_wndFolderList.GetItemData(nPage);
	pPage->ShowWindow(SW_SHOW);
	//pPage->BringWindowToTop();
	CString strCaption = m_wndFolderList.GetItemText(nPage);
	m_wndFrame.SetWindowText(strCaption);
	m_wndFrame.Invalidate(pPage);

	m_nPreSelPage=nPage;
}


void CSAConfig::OnDestroy()
{
	CXSkinDlg::OnDestroy();

	for ( int i=0; i<m_wndFolderList.GetItemCount(); i++ )
	{
		CDialog *pDlg = (CDialog *)m_wndFolderList.GetItemData(i);
		pDlg->DestroyWindow();
	}
}

BOOL CSAConfig::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	CRgn rgnClient;
	rgnClient.CreateRectRgn(rcClient.left,rcClient.top,rcClient.right,rcClient.bottom);

	if ( this->m_wndFolderList.m_hWnd  )
		this->RemoveRgnOfWnd(rgnClient,&m_wndFolderList);

	if ( this->m_wndFrame.m_hWnd )
		this->RemoveRgnOfWnd(rgnClient,&m_wndFrame);

	pDC->SelectObject(&rgnClient);

	pDC->FillSolidRect(&rcClient,IM_SPACE_COLOR);

	if ( m_wndFolderList.m_hWnd  )
	{
		CRect rc;
		m_wndFolderList.GetWindowRect(&rc);
		ScreenToClient(&rc);
		rc.InflateRect(1,1);
		pDC->Draw3dRect(&rc,IM_LINE_COLOR,IM_LINE_COLOR);
	}
	//return CXSkinDlg::OnEraseBkgnd(pDC);
	return FALSE;
}

void CSAConfig::OnNcDestroy()
{
	m_pPage1->DestroyWindow();
	m_pPage2->DestroyWindow();
	m_pPage3->DestroyWindow();
	m_pPage4->DestroyWindow();
	m_pPage6->DestroyWindow();
	m_pPage7->DestroyWindow();
	m_pPageVedio->DestroyWindow();
	m_pPageHotKey->DestroyWindow();
	m_pPageBasicSet->DestroyWindow();
	m_pPage5->DestroyWindow();
	g_pProtocol->UnRegistCmd(m_hWnd);

	CXSkinDlg::OnNcDestroy();
	delete pInstance;
}

INT_PTR CSAConfig::DoModal()
{
	// TODO: 在此添加专用代码和/或调用基类

	return CXSkinDlg::DoModal();
}

void CSAConfig::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	DestroyWindow();
}

void CSAConfig::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_USERINFO_MOD:
		{
			static int count=0;
			static CString strDot=".";

			CString strInfo=m_strInfo+strDot;

			SetDlgItemText(IDC_STATIC_INFO,strInfo);

			count++;
			strDot+=".";

			if (count>4)
			{
				count=0;
				strDot="";
			}
		}
		break;
	}

	CXSkinDlg::OnTimer(nIDEvent);
}

void CSAConfig::OnUserInfoModified(const char *data,WORD wRet)
{
	KillTimer(TIMER_USERINFO_MOD);
	SetDlgItemText(IDC_STATIC_INFO,"");

	LPXT_USERINFO_MOD_ACK resp = (LPXT_USERINFO_MOD_ACK)data;

	this->m_btnOk.EnableWindow(true);

	if ( wRet==0 )
	{
		memcpy(&g_ImUserInfo,&m_UserInfo,sizeof(XT_USER_INFO));
		// 保存新的登录信息
		CMainFrame::SaveLoginAccount();
		ShowMessage("服务器已经接受了您的新资料",MB_OK|MB_ICONINFORMATION);
		
		DestroyWindow();

		RegUserOperation(0x00c0);
	}
	else
	if ( wRet==SERVER_TIMEOUT)
	{
		ShowMessage("服务器处理超时或者您的网络连接出现故障",MB_OK|MB_ICONINFORMATION);
	}
	else
	{
		ShowMessage(resp->error,MB_OK|MB_ICONINFORMATION);
	}
}

void CSAConfig::OnBnClickedCancel()
{
//	this->m_pPage5->StopThread();
	DestroyWindow();
}

BOOL CSAConfig::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
		case 0x0D:
			return TRUE;
		default:
			break;
		}
	}

	return CXSkinDlg::PreTranslateMessage(pMsg);
}

void CSAConfig::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CXSkinDlg::OnShowWindow(bShow, nStatus);

	if ( bShow )
	{
		for ( int i=0; i<m_wndFolderList.GetItemCount(); i++ )
		{
			CDialog *pDlg = (CDialog *)m_wndFolderList.GetItemData(i);
			pDlg->ShowWindow(SW_HIDE);
		}
		m_wndFolderList.ClearAll();
		m_nPreSelPage = -1;

		m_wndFolderList.AddFolder("个人设置");
		// 同时添加一个系统设置
		m_wndFolderList.AddFolder("系统设置",FOLDER_EXPAND);

		//m_wndFolderList.AddFolder("群设置",FOLDER_CLOSE);

		//m_wndFolderList
		
		// 个人设置
		if ( g_ImUserInfo.status!=XTREAM_OFFLINE )
		{
			AddPage("基本资料",m_pPage1,0);
//			AddPage("个人形象",m_pPage5,0);
//			AddPage("闪  秀", m_pPage6,0);
			AddPage("联系方式",m_pPage2,0);
			AddPage("下载目录",m_pPage7,0);
		}

		// 系统设置
//		AddPage("基本设置",m_pPageBasicSet,1);
		AddPage("热键设置",m_pPageHotKey,1);
		AddPage("登录设置",m_pPage4,1);

		// 群设置
		//AddPage("消息设置",m_pPageClusterMsgSet,2);
		
		if( g_ImUserInfo.status!=XTREAM_OFFLINE )
		{
			AddPage("网络安全",m_pPage3,1);
		}
	
		AddPage("文件传输",m_pPage5,1);
		AddPage("选择摄像头",m_pPageVedio,1);

		if ( m_nStartPage==0 )
			m_pPage1->SetTimer(0x01,100,NULL);

		if ( m_nStartPage==-1 )
			m_nStartPage=0;

		if ( m_nStartPage>=0 && m_nStartPage<m_wndFolderList.GetItemCount() )
			m_wndFolderList.SetCurSel(m_nStartPage);

        ShowPage(m_nStartPage);
	}
}

LRESULT CSAConfig::OnUploadNotify(WPARAM wParam, LPARAM lParam)
{
	KillTimer(TIMER_USERINFO_MOD);
	SetDlgItemText(IDC_STATIC_INFO,"");

//	switch ( wParam )
//	{
//	case 0:
//		{	//更改个人形象成功
//			sprintf(g_ImUserInfo.face_url,(LPCSTR)lParam);
//			
//			theApp.m_pMainWnd->PostMessage(WM_139WEB_NOTIFY,1,0);
//
//			this->m_wndFolderList.EnableWindow(TRUE);
////			this->m_pPage5->EnableWindow(TRUE);
//			this->m_btnOk.EnableWindow(TRUE);
//
//			ShowMessage("139个人形象更改成功！");
//
//			RegUserOperation(0x0015);
//		}
//		break;
//	case 1:
//		{
//			//更改个人形象失败
//			ShowMessage((LPCSTR)lParam);
//            
//			this->m_wndFolderList.EnableWindow(TRUE);
////			this->m_pPage5->EnableWindow(TRUE);
//			this->m_btnOk.EnableWindow(TRUE);
//		}
//		break;
//	case 2://begin to upload pic
//		{
//			this->m_strInfo = "正在上传个人形象";
//			this->m_btnOk.EnableWindow(false);
//			SetTimer(TIMER_USERINFO_MOD,350,NULL);
//
//			this->m_wndFolderList.EnableWindow(FALSE);
////			this->m_pPage5->EnableWindow(FALSE);
//			this->m_btnOk.EnableWindow(FALSE);
//		}
//		break;
//	case 3:
//		{
//			if ( lParam==0 )//begin refresh 
//			{
//				this->m_strInfo = "正在刷新个人形象";
//				this->m_btnOk.EnableWindow(false);
//				SetTimer(TIMER_USERINFO_MOD,350,NULL);
//
//				this->m_wndFolderList.EnableWindow(FALSE);
////				this->m_pPage5->EnableWindow(FALSE);
//			}
//			else
//			{
//				this->m_btnOk.EnableWindow(true);
//				this->m_wndFolderList.EnableWindow(TRUE);
//		//		this->m_pPage5->EnableWindow(TRUE);
//
//				if ( lParam==1 )// 刷新完成
//				{
//					theApp.m_pMainWnd->PostMessage(WM_139WEB_NOTIFY,0,0);
//				}
//			}
//		}
//	}

	return 0;
}

void CSAConfig::OnLogOut(const char *data,WORD wRet)
{
	CSelfFaceDlg::Close();
}
