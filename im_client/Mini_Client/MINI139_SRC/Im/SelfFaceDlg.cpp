// im\SelfFaceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\imclient.h"
#include ".\SelfFaceDlg.h"
#include "ImageFileDialog.h"
#include "localdata.h"
#include "webprocess.h"


#define IDC_FACESEL_CTRL 0x10A1

// CSelfFaceDlg 对话框


CSelfFaceDlg* CSelfFaceDlg::m_pInstance = NULL;

IMPLEMENT_DYNAMIC(CSelfFaceDlg, CXSkinDlg)
CSelfFaceDlg::CSelfFaceDlg(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CSelfFaceDlg::IDD, pParent)
{
	m_hUploadFace  = NULL;
	m_pWndProgress = NULL;
	m_hSAPageWnd   = NULL;
	m_pInstance    = this;

	Create(CSelfFaceDlg::IDD,pParent);
}

CSelfFaceDlg::~CSelfFaceDlg()
{
	m_pInstance = NULL;
}


CSelfFaceDlg * CSelfFaceDlg::Show()
{
	if ( m_pInstance )
	{
		if ( m_pInstance->IsWindowVisible()==false )
		{
			m_pInstance->ShowWindow(SW_SHOW);
		}

		if ( m_pInstance->IsIconic() )
		{
			m_pInstance->ShowWindow(SW_RESTORE);
		}

		m_pInstance->BringWindowToTop();
	}
	else
	{
		m_pInstance = new CSelfFaceDlg(GetDesktopWindow());
		m_pInstance->ShowWindow(SW_SHOW);

	}

	return m_pInstance;
}

void CSelfFaceDlg::Close()
{
	if( m_pInstance )
	{
		if(m_pInstance->m_hUploadFace )
		{
			::TerminateThread(m_pInstance->m_hUploadFace,0);
		}

		m_pInstance->DestroyWindow();
	}
}

void CSelfFaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BUTTON_PIC,m_btnUpload);
	DDX_Control(pDX,IDC_BUTTON_BIGHEAD,m_btnBigHead);
	DDX_Control(pDX,IDOK,m_btnOk);
	DDX_Control(pDX,IDCANCEL,m_btnCancel);
	DDX_Control(pDX,IDC_STATIC_FACE,m_wndFaceIconCtrl);
}


BEGIN_MESSAGE_MAP(CSelfFaceDlg, CXSkinDlg)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_USER_HEAD_NOTIFY,OnWebIconNotify)
	ON_BN_CLICKED(IDC_BUTTON_PIC, OnBnClickedButtonPic)
	ON_MESSAGE(WM_CANCEL_NOTIFY,OnCancelUpload)
	ON_MESSAGE(WM_THREAD_NOTIFY,OnThreadNotify)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_BIGHEAD, OnBnClickedButtonBighead)
	ON_WM_NCDESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CSelfFaceDlg)

	// 登出
	ON_XT_COMMAND(CMD_LOGOUT,OnLogOut)

END_XT_MESSAGE_MAP()



// CSelfFaceDlg 消息处理程序

BOOL CSelfFaceDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetDlgStyle(CLOSE);
	SetWindowText("修改个人头像");
	EnableHelp(true);

	CRect rect;
	GetDlgItem(IDC_STATIC_PIC)->GetWindowRect(&rect);
	ScreenToClient(&rect);
	m_wndFaceSelCtrl.Create(this,rect,WS_VISIBLE|WS_CHILDWINDOW|WS_CLIPCHILDREN,IDC_FACESEL_CTRL);
	m_wndFaceSelCtrl.InitData();
	g_webUserHeadIconProcess.GetUserHeadIcon(&g_ImUserInfo,this);

	m_wndFaceIconCtrl.SetCursor( ::LoadCursor(NULL,MAKEINTRESOURCE(IDC_ARROW)) );
	m_wndFaceSelCtrl.SetDlgCtrlID(IDC_FACESEL_CTRL);

	m_btnBigHead.SetButtonImage(GetResDLLHandle(),IDB_BIGHEAD);
	m_btnUpload.SetButtonImage(GetResDLLHandle(),IDB_UPLOAD_PIC);

	
	g_pProtocol->RegistCmd(CMD_LOGOUT,m_hWnd);

	return TRUE;  
}

void CSelfFaceDlg::OnHelpClick()
{
	OpenHtmlPage("www.feiniu.com");
}

void CSelfFaceDlg::OnLogOut(const char *data, WORD wRet)
{
	if (wRet==0 )
	{
		//m_hUploadFace
		if(m_hUploadFace )
		{
			::TerminateThread(m_hUploadFace,0);
		}
			
		DestroyWindow();
	}
}

HBRUSH CSelfFaceDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetTextColor(RGB(0,84,166));
		pDC->SetBkMode(TRANSPARENT);
		hbr = this->m_hBkGround;
	}

	return hbr;
}

LRESULT CSelfFaceDlg::OnThreadNotify(WPARAM wParam, LPARAM lParam)
{
	if ( m_hUploadFace )
	{
		CloseHandle(m_hUploadFace);
		m_hUploadFace=NULL;
	}

	if ( m_pWndProgress )
	{
		m_pWndProgress->DestroyWindow();
		delete m_pWndProgress;
		m_pWndProgress=NULL;
	}

	UnLockOperation();

	if ( wParam==0)//ok
	{
		//个人形象上传成功
		m_btnOk.EnableWindow(FALSE);
		m_wndFaceSelCtrl.ClearData();
		m_wndFaceSelCtrl.InitData();
		m_wndFaceSelCtrl.SetSel(FACE_HISTORY,(char *)lParam);
	}
	else
	if ( wParam==1)//failed
	{
		ShowMessage((LPCSTR)lParam);
	}

	return 0;
}

LRESULT CSelfFaceDlg::OnCancelUpload(WPARAM wParam,LPARAM lParam)
{
	if ( m_hUploadFace )
	{
		try
		{
			TerminateThread(m_hUploadFace,0);
			CloseHandle(m_hUploadFace);
		}
		catch(...)
		{
		}

		m_hUploadFace = NULL;
	}

	if ( m_pWndProgress )
	{
		m_pWndProgress->DestroyWindow();
		delete m_pWndProgress;
		m_pWndProgress=NULL;
	}

	UnLockOperation();

	return 0;
}

LRESULT CSelfFaceDlg::OnWebIconNotify(WPARAM wParam,LPARAM lParam)
{
	char * szFileIcon = (char *)lParam;

	if ( wParam==g_ImUserInfo.GetId() )
	{
		if ( szFileIcon )
			this->m_wndFaceIconCtrl.SetFaceFile(szFileIcon,XTREAM_ONLINE);
	}

	return 0;
}

void CSelfFaceDlg::OnBnClickedButtonPic()
{
	m_thParam.pDlg = this;
	CImageFileDialog dlg("","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,this);

	if ( IDOK==dlg.DoModal() )
	{
		HRESULT hResult;
		CImage imgLocal;
		CString strFile = dlg.GetPathName();
		hResult = imgLocal.Load(strFile);
		if( FAILED(hResult))
		{
			ShowMessage("打开图片失败");
			return;
		}

		if( imgLocal.GetHeight() > MAX_FACE_SHOW_SIZE ||
			imgLocal.GetWidth() > MAX_FACE_SHOW_SIZE )
		{
			if( !AdjustImgSize(imgLocal,m_thParam.strFile))
			{
				m_thParam.strFile = dlg.GetFileName();

				CStdioFile fUploadFile;
				fUploadFile.Open(m_thParam.strFile,CFile::modeRead);

				if( fUploadFile.GetLength() > 1024*1024*2 )
				{
					ShowMessage("对不起，您只能设置大小小于 2M 的图片！",MB_ICONINFORMATION|MB_OK);
					return ;
				}
				
			}
		}
		else
		{
			m_thParam.strFile = dlg.GetFileName();
		}

		LockOperation();

		DWORD dwThreadId ;

		m_hUploadFace = CreateThread(NULL,0,
				(LPTHREAD_START_ROUTINE)UploadFaceProc,
				this,
				0,
				&dwThreadId);

		m_pWndProgress = new CFormProgress3(m_hWnd,this);
		m_pWndProgress->SetTip("正在上传形象...");
		m_pWndProgress->ShowWindow(SW_SHOW);
		m_pWndProgress->CenterWindow(this);
	}	
}


BOOL CSelfFaceDlg::AdjustImgSize(CImage& img,CString& strNewFile)
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

UINT CSelfFaceDlg::UploadFaceProc(LPVOID p)
{
	CSelfFaceDlg *pDlg = (CSelfFaceDlg *)p;

	CString strUrl;
	CString strError("功能待开发，敬请期待!!");

	CWebInterfaceWrapper objWI;
	objWI.InitInterface(g_ImUserInfo.GetId(),g_ImUserInfo.GetUserName().c_str(),g_ImUserInfo.nickname);

	if ( objWI.UploadFace(pDlg->m_thParam.strFile,strUrl,strError) )//.UploadPicture(pDlg->m_thParam.strFile,&g_ImUserInfo,0,4,strUrl,strError)
	{
		CString strFaceUrl = strUrl;
		strFaceUrl.MakeLower();
		strFaceUrl.Replace("http://","");
		strFaceUrl.Trim();

		CString strFile = g_webUserHeadIconProcess.Url2File((char *)(LPCSTR)strFaceUrl);

		CopyFile(pDlg->m_thParam.strFile,strFile,FALSE);

		g_LocalData.SaveHistoryFaceUrl(strFaceUrl);
		pDlg->SendMessage(WM_THREAD_NOTIFY,0,(LPARAM)(LPCSTR)strFaceUrl);
	}
	else
	{
		pDlg->SendMessage(WM_THREAD_NOTIFY,1,(LPARAM)(LPCSTR)strError);
	}

	::CloseHandle(pDlg->m_hUploadFace);
	pDlg->m_hUploadFace = NULL;

	return 0;
}

extern CFont g_fontText;

BOOL CSelfFaceDlg::OnEraseBkgnd(CDC* pDC)
{
	bool bRet = CXSkinDlg::OnEraseBkgnd(pDC);

	CRect rc;
	GetDlgItem(IDC_STATIC1)->GetWindowRect(&rc);
	ScreenToClient(&rc);

	int nSavedDC = pDC->SaveDC();

	pDC->FillSolidRect(&rc,0xFFFFFF);
	pDC->SetBkColor(0xFFFFFF);
	pDC->SetTextAlign(TA_CENTER);
	pDC->SelectObject(&g_fontText);
	rc.InflateRect(2,2);
	pDC->TextOut(rc.left+rc.Width()/2,rc.top+4,"预览");
	pDC->Draw3dRect(&rc,IM_LINE_COLOR,IM_LINE_COLOR);

	this->m_wndFaceSelCtrl.GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.InflateRect(2,2);
	pDC->Draw3dRect(&rc,IM_LINE_COLOR,IM_LINE_COLOR);

	pDC->RestoreDC(nSavedDC);

	return bRet;
}

BOOL CSelfFaceDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message==WM_COMMAND )
	{
		if ( pMsg->wParam==IDC_FACESEL_CTRL )
		{
			if ( m_btnOk.IsWindowEnabled()==FALSE )
			{
				m_btnOk.EnableWindow(TRUE);
			}

			int iType;
			CString strUrl;
			m_wndFaceSelCtrl.GetSel(iType,strUrl);

			XT_USER_INFO info;

			if ( iType==FACE_DEFAULT )
			{
				info.SetId(g_ImUserInfo.GetId());
				strcpy(info.face_url, strUrl );
    		}
			else
			if ( iType==FACE_HISTORY )
			{
				info.SetId(g_ImUserInfo.GetId());
				strcpy(info.face_url, HistoryFaceUrl2WebUrl(strUrl) );
			}
			
			g_webUserHeadIconProcess.GetUserHeadIcon(&info,this);
		}
	}

	return CXSkinDlg::PreTranslateMessage(pMsg);
}


UINT CSelfFaceDlg::LoadWebHeadListProc(LPVOID param)
{
	//CSelfFaceDlg *pDlg = (CSelfFaceDlg *)param;

	//::CoInitialize(0);

	//CMiniPublic p(g_webUrlProcess.GetServiceUrl(CWebUrlProcess::wtMiniPublic,g_ImUserInfo.GetUserName().c_str()));
	//BSTR ret=L"";
	//p.GetUserDefaultFace(&ret);
	//g_mapUserFace.clear();
	//CString str ;
	//USES_CONVERSION;
	//str = W2A(ret);
	//str.Trim();

	//CString strFile = g_LocalData.GetPath()+"face.xml";

	//if ( str=="" )
	//{
	//	CStdioFile f;
	//	if ( f.Open(strFile,CFile::modeRead|CFile::typeText) )
	//	{
	//		CString s;
	//		while ( f.ReadString(s) )
	//		{
	//			str = str+s;
	//		}
	//		f.Close();
	//	}
	//}
	//else
	//{
	//	DeleteFile(strFile);
	//	CStdioFile f;
	//	if ( f.Open(strFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
	//	{
	//		f.WriteString(str);
	//		f.Close();
	//	}
	//}

	//ParaseFaceList(str,g_mapUserFace);

	//MAX_USER_HEAD_COUNT = g_mapUserFace.size()+1;

	//::CoUninitialize();

	//if ( pFrame->m_bImOk )
	//{
	//	g_webUserHeadIconProcess.GetUserHeadIcon(&g_ImUserInfo,pFrame);
	//}

	//CloseHandle(pFrame->m_hLoadUserFaceList);
	//pFrame->m_hLoadUserFaceList=NULL;

	//if ( pFrame->m_bFriendListGet )
	//{
	//	pFrame->m_pPage1->m_pageIM.UpdateUserFace();
	//	pFrame->m_pPage1->m_pageCluster.UpdateUserFace();
	//}	

	return 0;
}



bool CSelfFaceDlg::GetFaceUrl(CString &strUrl)
{
	int iFaceType;
	CString strPicUrl;

	m_wndFaceSelCtrl.GetSel(iFaceType,strUrl);

	return true;
}

void CSelfFaceDlg::OnBnClickedButtonBighead()
{
	g_pHeadPhoto->Show();
}

void CSelfFaceDlg::OnNcDestroy()
{
	CXSkinDlg::OnNcDestroy();
	delete this;
}

void CSelfFaceDlg::OnClose()
{
	DestroyWindow();
}

void CSelfFaceDlg::OnBnClickedOk()
{
	CString strFaceUrl;

	GetFaceUrl(strFaceUrl);

	strFaceUrl.MakeLower();
	strFaceUrl.Replace("http://","");
	strFaceUrl.Trim();

	strcpy(g_ImUserInfo.face_url,strFaceUrl);

	CWebInterfaceWrapper objWI;
	objWI.InitInterface(g_ImUserInfo.GetId(),g_ImUserInfo.GetUserName().c_str(),g_ImUserInfo.nickname);

	CString strPicUrl;
	CString strError;

	strPicUrl=strFaceUrl;

	CWaitCursor w;
	objWI.SetUserFace(strPicUrl,strError);

	theApp.m_pMainWnd->PostMessage(WM_139WEB_NOTIFY,1,0);

	if ( m_hSAPageWnd )
		::SendMessage(m_hSAPageWnd,WM_FACESEL_NOTIFY,0,IDOK);

	DestroyWindow();
}

void CSelfFaceDlg::OnBnClickedCancel()
{
	if ( m_hSAPageWnd )
		::SendMessage(m_hSAPageWnd,WM_FACESEL_NOTIFY,0,IDCANCEL);

	DestroyWindow();
}
