
// UpLoadPhotoDlg.cpp : 实现文件
//

#include "..\stdafx.h"
#include "UpLoadPhotoDlg.h"
#include ".\uploadphotodlg.h"

#include "webprocess.h"
#include "Enablewindow/EnableWindow.h"

#include <comutil.h>

float g_RemainSpace;	// MB
float g_fltUsedSpaceTotal;

#define WM_OVER		WM_USER + 1
#define WM_SHOWSTAT WM_USER + 2
#define WM_SHOWPLAN WM_USER + 3

#define MIN_WINDOW_WIDTH_X 958
#define MIN_WINDOW_HEIGHT_Y	610

IMPLEMENT_DYNAMIC(CUpLoadPhotoDlg, CXSkinDlg)

void CUpLoadPhotoDlg::GetLastUploadPicFolder(CString & strFolder)
{
	strFolder =	AfxGetApp() ->GetProfileString(version, _T("last_folder"), "");

	bool bValidePath = true;

	if(strFolder.GetLength() < 1)
	{
		// strFolder.Format("c:\\windows");
		bValidePath = false;		
	}
	else
	if ( strFolder.Left(2)=="\\\\" )
	{
		bValidePath = false;
	}

	if ( bValidePath==false )
	{
		GetWindowsDirectory(strFolder.GetBufferSetLength(MAX_PATH), MAX_PATH);
	}

	
}

void CUpLoadPhotoDlg::SaveLastUploadPicFolder(const CString & strFolder)
{
	BOOL bRet = AfxGetApp()->WriteProfileString(version, _T("last_folder"), strFolder);
}

void CUpLoadPhotoDlg::ShowAlbum(LPCSTR lpszUserName, int nAlbumID)
{
	CString strUrl;
	strUrl.Format("http://%s.woku.com/album/%d.html ", lpszUserName,nAlbumID);
	// 私有时
//	if((m_bIsMiniClinet && g_AppConfigInfo.upfileprop.iViewProp == 0) || (bMiniOpen == TRUE))
	{
		MiniOpenUrl(strUrl);		return ;	
	}

//	OpenHtmlPage(strUrl);
}

void CUpLoadPhotoDlg::MiniOpenUrl(const CString & strUrl)
{
	CString strOpen;
	if (g_AppConfigInfo.userinfo.bIsMiniClinet)
	{
		strOpen.Format("http://passport.woku.com/MiniOpen.aspx?info=%s&key=%s&url=%s",g_AppConfigInfo.userinfo.strParamInfo,g_AppConfigInfo.userinfo.strParamKey,strUrl);		

	}
	else
	{
		if (strOpen.Find("http://"))
			strOpen = strUrl;
		else
			strOpen.Format("http://%s", strUrl);
	}
	::OpenHtmlPage(strOpen);
}


//CString MD5Password(char* pPassword)
//{
//	unsigned char digest[16];
//
//	char password[255] = {0};
//	sprintf(password,"%sasdfds!@#$1234@!#$ASDFasdfQ@#4345asDF",pPassword);
//
//	MD5_CTX md5T;
//	md5T.MD5Update ((unsigned char*)password,(uint32)strlen(password) );
//	md5T.MD5Final (digest);
//
//	return ToHexString(digest,16);
//}

CString UrlGetStr(LPCSTR lpszUrl)
{
	HINTERNET hFile, hInet;
	char buffer[255];
	DWORD dwRead, dwSize;

	CString strResult="";

	hInet = InternetOpen("Mozilla/4.0+(compatible;+MSIE+6.0;+Windows+NT+5.2;+SV1;+FunWebProducts;+Maxthon;+.NET+CLR+1.1.4322)",INTERNET_OPEN_TYPE_PRECONFIG,"",NULL,0);

	if ( hInet!=NULL )
	{
		hFile = InternetOpenUrl(hInet, lpszUrl, NULL, 0, 
			INTERNET_FLAG_RELOAD | 
			INTERNET_FLAG_NO_CACHE_WRITE |
			INTERNET_FLAG_EXISTING_CONNECT,// | INTERNET_FLAG_NO_COOKIES,
			0
			);

		if ( hFile!=NULL )
		{
			try
			{
				dwSize = sizeof(buffer)-1;

				while ( InternetReadFile(hFile, buffer, dwSize, &dwRead ) && dwRead>0 )
				{
					buffer[dwRead]=0;
					strResult=strResult+buffer;
				}
			}
			catch(...)
			{
			}
			InternetCloseHandle(hFile);
		}
		InternetCloseHandle(hInet);
	}
 
	return strResult;
}

CUpLoadPhotoDlg::CUpLoadPhotoDlg(CWnd* pParent /*=NULL*/)
: CXSkinDlg(CUpLoadPhotoDlg::IDD, pParent)
, m_strSelectDir(_T(""))
, m_bSetSize(FALSE)
{
	m_hThreadUpload   = NULL;
	m_hThreadInit     = NULL;
	m_pwndProgressBar = NULL;
	m_hIcon     = ::LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_MAIN));//AfxGetApp()->LoadIcon(IDR_MAINFRAME1);


	m_bDragging		= FALSE;
	m_nDragIndex = -1;
	m_nDropIndex = -1;
	m_pDragImage = NULL;
	m_pDropWnd	 = NULL;

	m_hNoDragCursor = NULL;
	m_hHyperLinkCursor = NULL;
	m_hHandleCursor = NULL;

	m_CursorType = CURSOR_TYPE_NORMAL;
	g_AppConfigInfo.userinfo.bNeedvalidate = TRUE;

	g_fltUsedSpaceTotal = 0.0;
	g_RemainSpace = MAX_SPACE_SIZE;

	m_bInitDialog = TRUE;
	g_bEnableContinueOperation = TRUE;

	m_bLoadFolder = FALSE;
	m_bValidateSucceed = FALSE;

	version.Format(_T("1.0"));

	m_bStopUploadThread = FALSE;
	m_nTimerID = 0;

	g_AppConfigInfo.LoadConfigInfo();

	g_bEnableLogInfo = g_AppConfigInfo.bEnableLogInfo;
	m_strMiniServerAddr.Format(_T("webservices.mini.woku.com"));
}

CUpLoadPhotoDlg::~CUpLoadPhotoDlg()
{
	if ( m_hThreadUpload )
	{
		try
		{
			TerminateThread(m_hThreadUpload,0);
			CloseHandle(m_hThreadUpload);
		}
		catch(...)
		{
		}
	}

	m_hThreadUpload = NULL;

	if ( m_hThreadInit )
	{
		try
		{
			TerminateThread(m_hThreadInit,0);
			CloseHandle(m_hThreadInit);
		}
		catch(...)
		{
		}
	}

	m_hThreadInit = NULL;
}

void CUpLoadPhotoDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_DIR_TREE, m_wndDirTree);
	DDX_Control(pDX, IDC_FILE_LIST, m_wndFileListCtrl);
	DDX_Control(pDX, IDC_UPLOADFILE_LIST, m_wndUpLoadFileListCtrl);

	//
	DDX_Control(pDX, IDC_DESCRIPTION_STATIC, m_strDescription);

	// Group box 
	DDX_Control(pDX, IDC_DLG_UPLOAD_STATIC_GROUP1, m_wndGroupBox1);
	DDX_Control(pDX, IDC_DLG_UPLOAD_STATIC_GROUP2, m_wndGroupBox2);
	DDX_Control(pDX, IDC_DLG_UPLOAD_STATIC_GROUP3, m_wndGroupBox3);
}


BEGIN_MESSAGE_MAP(CUpLoadPhotoDlg, CXSkinDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_GETMINMAXINFO()
	ON_NOTIFY(TVN_SELCHANGED, IDC_DIR_TREE, OnTvnSelchangedDirTree)
	ON_NOTIFY(TVN_DELETEITEM, IDC_DIR_TREE, OnTvnDeleteitemDirTree)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_DIR_TREE, OnTvnItemexpandingDirTree)

	ON_BN_CLICKED(IDC_ADDUPLOAD_BUTTON, OnBnClickedAdduploadButton)
	ON_BN_CLICKED(IDC_ADDALLUPLOADBUTTON, OnBnClickedAddalluploadbutton)
	ON_BN_CLICKED(IDC_DLG_UPLOADING_CANCEL, OnBnClickedCancelButton)
	
	ON_BN_CLICKED(IDC_REMOVEUPLOADFILE_BUTTON, OnBnClickedRemoveuploadfileButton)
	ON_BN_CLICKED(IDC_REMOVEALLUPLOADFILES_BUTTON, OnBnClickedRemovealluploadfilesButton)

	ON_BN_CLICKED(IDC_DLG_UPLOAD_BTN_STARTSEND, OnBnClickedUploadfileButton)
	ON_BN_CLICKED(IDC_DLG_UPLOAD_BTN_RESETINFO, &CUpLoadPhotoDlg::OnBtnResetInfo)
	ON_NOTIFY(NM_DBLCLK, IDC_FILE_LIST, OnNMDblclkFileList)
	
	ON_MESSAGE(WM_PREVIEW, OnPreview)
	ON_MESSAGE(WM_DESTROYPREVIEW, OnDestroyPreview)
	ON_MESSAGE(WM_NOTIFY_LOADFINISHED,OnLoadFinished)
	ON_MESSAGE(WM_CANCEL_NOTIFY,OnCancelUpload)
	ON_MESSAGE(WM_THREAD_NOTIFY,OnThreadNotify)
	ON_MESSAGE(WM_BRINGTOTOP_MSG,BringToTop)
//	ON_CBN_SELCHANGE(IDC_TAG_COMBO, OnCbnSelchangeTagCombo)
	ON_WM_DESTROY()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_FILE_LIST, OnFileListCtrlBeginDrag)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_UPLOADFILE_LIST, OnUploadListCtrlBeginDrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_UPLOADFILE_LIST, OnUploadListCtrlBeginLabelEdit)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_UPLOADFILE_LIST, OnUploadListCtrlEndLabelEdit)
	ON_WM_SETFOCUS()
	ON_WM_SETCURSOR()

	ON_NOTIFY(NM_DBLCLK, IDC_UPLOADFILE_LIST, &CUpLoadPhotoDlg::OnNMDblclkUploadfileList)

	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_TIMER()

	ON_COMMAND(IDC_DLG_UPLOAD_STATIC_HYPERLINK, OnBtnHyperLink)
END_MESSAGE_MAP()

BOOL CUpLoadPhotoDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetDlgStyle(ALL);
	EnableHelp(true);

	_delete_all_tmp_files();

	CString strTitle;
	strTitle.Format("WoKu图片上传工具 - %s", g_AppConfigInfo.userinfo.strNickName);
	SetWindowText(strTitle);

	bool bHasConfig = false;

	CRect rcTemp;

	m_wndUploadOperationsDlg.Create(IDD_DLG_OPERATIONS, this);
	m_wndUploadOperationsDlg.ShowWindow(SW_SHOW);
	m_wndUploadOperationsDlg.m_pParent = this;

	m_wndGroupBox2.GetWindowRect(&rcTemp);
	ScreenToClient(&rcTemp);
	m_wndUploadOperationsDlg.MoveWindow(&rcTemp);


	m_wndPanelDlg1.Create(m_wndPanelDlg1.IDD, this);
	m_wndPanelDlg1.ShowWindow(SW_SHOW);
	m_wndPanelDlg1.m_pParentWnd = this;

	m_wndGroupBox1.GetWindowRect(&rcTemp);
	ScreenToClient(&rcTemp);
	m_wndPanelDlg1.MoveWindow(&rcTemp);

	m_wndPanelDlg2.Create(m_wndPanelDlg2.IDD, this);
	m_wndPanelDlg2.ShowWindow(SW_SHOW);

	m_wndGroupBox3.GetWindowRect(&rcTemp);
	ScreenToClient(&rcTemp);
	m_wndPanelDlg2.MoveWindow(&rcTemp);

	CRect rc;

	m_wndFileListCtrl.m_pParentWnd = this;
	m_wndUpLoadFileListCtrl.m_pParentWnd = this;

	m_wndFileListCtrl.Initilization();
	m_wndFileListCtrl.SetIconSpacing(100, 110);
//	m_wndFileListCtrl.EnableToolTips(TRUE);

	m_hNoDragCursor = AfxGetApp() ->LoadCursor(IDC_NODRAG);
	m_hHandleCursor = AfxGetApp() ->LoadCursor(IDC_HANDLE);

	m_wndUpLoadFileListCtrl.Initilization();
	m_wndUpLoadFileListCtrl.SetIconSpacing(100, 110);

	m_wndDirTree.EnableImages();
	m_wndDirTree.PopulateTree();

	ShowFileMessage();

	int nItem =	m_wndPanelDlg1.m_wndComboACL.AddString("私有");			// 0
	m_wndPanelDlg1.m_wndComboACL.SetItemData(nItem, 0);

	nItem =	m_wndPanelDlg1.m_wndComboACL.AddString("好友可见");			// 1
	m_wndPanelDlg1.m_wndComboACL.SetItemData(nItem, 1);
	
//	this->m_wndComboACL.AddString("注册用户可见");
	nItem =	m_wndPanelDlg1.m_wndComboACL.AddString("公开");				// 3
	m_wndPanelDlg1.m_wndComboACL.SetItemData(nItem, 3);
	
	m_wndPanelDlg1.m_wndComboACL.SetCurSel(2);

	m_wndPanelDlg1.m_wndComboLimitSize.AddString("480");
	m_wndPanelDlg1.m_wndComboLimitSize.AddString("600");
	m_wndPanelDlg1.m_wndComboLimitSize.AddString("800");
	m_wndPanelDlg1.m_wndComboLimitSize.AddString("1024");

	m_wndPanelDlg1.m_wndComboLimitSize.SetCurSel(3);

	m_wndPanelDlg1.m_wndComboLimitSize.AddString("1280");
	m_wndPanelDlg1.m_wndComboLimitSize.AddString("1600");
	m_wndPanelDlg1.m_wndComboLimitSize.AddString("原图");
		

	m_br.CreateSolidBrush(IM_SPACE_COLOR);

//	m_wndPanelDlg1.m_wndComboAlbum.m_bEnableEditUpdate = FALSE;

	// 	AttachXtScrollbar(m_wndDirTree.m_hWnd);
	// 	AttachXtScrollbar(m_wndFileListCtrl.m_hWnd);
	// 	AttachXtScrollbar(m_wndUpLoadFileListCtrl.m_hWnd);
	// 	AttachXtScrollbar(m_FileDsp.m_hWnd);

	CString strFolder;
	GetLastUploadPicFolder(strFolder);
//	m_wndDirTree.TunnelTree(strFolder);

	m_wndDirTree.Select(m_wndDirTree.GetRootItem(), TVGN_CARET);
	m_wndDirTree.TunnelTree2(strFolder);

	
	CString strUrl;
	strUrl.Format("http://%s.woku.com/photo/list.php ", g_AppConfigInfo.userinfo.strUserName);
	m_wndPanelDlg2.m_wndStaticHyperLink.SetHyperLinkeString(strUrl);

	// load handle cursor
	{
		CString windir;
		GetWindowsDirectory(windir.GetBuffer(MAX_PATH), MAX_PATH);
		windir.ReleaseBuffer();
		windir += _T("\\winhlp32.exe");
		HMODULE hModule = LoadLibrary(windir);
		if (hModule) 
		{
			m_hHyperLinkCursor =
				CopyCursor(::LoadCursor(hModule, MAKEINTRESOURCE(106)));
		}

		FreeLibrary(hModule);
	}

	m_wndPanelDlg2.m_wndBmpBtnINFO.SetBitmap(IDB_BITMAP_HINT,IDB_BITMAP_HINT,IDB_BITMAP_HINT,IDB_BITMAP_HINT);
	m_wndPanelDlg2.m_wndBmpBtnINFO.SizeToContent();
	
	m_nTimerID = SetTimer(11, 30, NULL);

	m_bInitDialog = FALSE;
	LoadFolder(strFolder);
	
	DWORD dwThreadId=0;
	m_hThreadUpload = CreateThread(NULL,0,
		(LPTHREAD_START_ROUTINE)InitProc,
		this,
		0,
		&dwThreadId);

	UpdateData(FALSE);
	
	//WINDOWPLACEMENT wp;
	//wp.length = sizeof (WINDOWPLACEMENT);
	//GetWindowPlacement (&wp);

	//if (((wp.flags =
	//	AfxGetApp()->GetProfileInt (version, _T ("flags"), -1)) != -1) &&
	//	((wp.showCmd =
	//	AfxGetApp()->GetProfileInt (version, _T ("showCmd"), -1)) != -1) &&
	//	((wp.rcNormalPosition.left =
	//	AfxGetApp()->GetProfileInt (version, _T ("x1"), -1)) != -1) &&
	//	((wp.rcNormalPosition.top =
	//	AfxGetApp()->GetProfileInt (version, _T ("y1"), -1)) != -1) &&
	//	((wp.rcNormalPosition.right =
	//	AfxGetApp()->GetProfileInt (version, _T ("x2"), -1)) != -1) &&
	//	((wp.rcNormalPosition.bottom =
	//	AfxGetApp()->GetProfileInt (version, _T ("y2"), -1)) != -1)) 
	//{

	//		wp.rcNormalPosition.left = min (wp.rcNormalPosition.left,
	//			::GetSystemMetrics (SM_CXSCREEN) -
	//			::GetSystemMetrics (SM_CXICON));
	//		wp.rcNormalPosition.top = min (wp.rcNormalPosition.top,
	//			::GetSystemMetrics (SM_CYSCREEN) -
	//			::GetSystemMetrics (SM_CYICON));

	//		if(wp.rcNormalPosition.right - wp.rcNormalPosition.left < MIN_WINDOW_WIDTH_X)
	//			wp.rcNormalPosition.right = wp.rcNormalPosition.left + MIN_WINDOW_WIDTH_X;
	//		if(wp.rcNormalPosition.bottom - wp.rcNormalPosition.top < MIN_WINDOW_HEIGHT_Y)
	//			wp.rcNormalPosition.bottom = wp.rcNormalPosition.top + MIN_WINDOW_HEIGHT_Y;

	//		SetWindowPlacement (&wp);
	//}
	//else
	//{
	//	//MoveWindow(0, 0, MIN_WINDOW_WIDTH_X, MIN_WINDOW_HEIGHT_Y);
	//	CenterWindow(GetDesktopWindow());
	//}

	CenterWindow(GetDesktopWindow());

	m_wndFileListCtrl.SetFocus();
	ResizePostion();

	SetMinMaxInfo(MIN_WINDOW_WIDTH_X,MIN_WINDOW_HEIGHT_Y,0,0);

	return FALSE;
}

void CUpLoadPhotoDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = MIN_WINDOW_WIDTH_X;
	lpMMI->ptMinTrackSize.y = MIN_WINDOW_HEIGHT_Y;

	CXSkinDlg::OnGetMinMaxInfo(lpMMI);
}
void CUpLoadPhotoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	// 	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	// 	{
	// 		CAboutDlg dlgAbout;
	// 		dlgAbout.DoModal();
	// 	}
	// 	else
	// 	{
	// 		CXSkinDlg::OnSysCommand(nID, lParam);
	// 	}
}

void CUpLoadPhotoDlg::OnPaint() 
{
	CPaintDC dc(this); // 用于绘制的设备上下文
	CXSkinDlg::OnPaint();

#if 0
	CRect rc;

#if 0
	CWnd *pLine1 = GetDlgItem(IDC_STATIC_LINE1);
	pLine1->GetWindowRect(rc);
	ScreenToClient(&rc);
	dc.FillSolidRect(rc,0xFFFFFF);
	dc.Draw3dRect(rc,IM_LINE_COLOR,IM_LINE_COLOR);
#endif

#if 1
	CWnd *pLine2 = GetDlgItem(IDC_STATIC_LINE2);
	pLine2->GetWindowRect(rc);
	ScreenToClient(&rc);	
	dc.FillSolidRect(rc,0xFFFFFF);
	dc.Draw3dRect(rc,IM_LINE_COLOR,IM_LINE_COLOR);
#endif

	m_wndDirTree.GetWindowRect(&rc);
	ScreenToClient(&rc);					 
	rc.InflateRect(2,2);
	dc.Draw3dRect(rc,IM_LINE_COLOR,IM_LINE_COLOR);

	m_wndFileListCtrl.GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.InflateRect(2,2);
	dc.Draw3dRect(rc,IM_LINE_COLOR,IM_LINE_COLOR);

	m_wndUpLoadFileListCtrl.GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.InflateRect(2,2);
	dc.Draw3dRect(rc,IM_LINE_COLOR,IM_LINE_COLOR);

#if 0
	//	
	m_wndGroupBox2.GetWindowRect(&rc);
	ScreenToClient(&rc);
	dc.SelectObject(&CBrush(NULL_BRUSH));
//	dc.FillSolidRect(&rc, RGB(255, 0, 0));
	dc.RoundRect(&rc, CPoint(10, 10));
#endif 


#endif

}	

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CUpLoadPhotoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CUpLoadPhotoDlg::OnTvnSelchangedDirTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	//if(!g_bEnableContinueOperation)		return ; //modify by lwl at ver:1005

	CString strFolder;


	if(pNMTreeView ->itemNew.hItem != NULL)
	{
		CString strItemText =	m_wndDirTree.GetItemText(pNMTreeView ->itemNew.hItem);

	/*	LPITEMIDLIST  lpi=NULL;
		HRESULT hr =SHGetSpecialFolderLocation(m_hWnd, nFolder, &lpi);
		if(SUCCEEDED(hr))
		{

		}*/

		if(strItemText.Compare(_T("桌面")) == 0)
		{
			CString strDesktopFolerPath; 
			SHGetSpecialFolderPath(m_hWnd,strDesktopFolerPath.GetBufferSetLength(MAX_PATH), CSIDL_DESKTOPDIRECTORY,FALSE);
			
			strFolder.Format("%s\0", strDesktopFolerPath);
		}
		else
		{
			if(!m_wndDirTree.OnFolderSelected(pNMHDR,pResult,strFolder))
			{
				return	;
			}
			
		}
	}

	if(m_bInitDialog == TRUE)	return ;
	
	//g_bEnableContinueOperation = FALSE;

	//AfxGetApp() ->BeginWaitCursor();
	LoadFolder(strFolder);

	*pResult = 0;
}

void CUpLoadPhotoDlg::LoadFolder(const CString & strFolder)
{
	//CWaitCursor wait;
	//_auto_enable_ui_ ui(this);

	if(strFolder.GetLength() < 1)		return ;

	m_bLoadFolder = TRUE;
	m_wndFileListCtrl.Clear();
	m_strSelectDir = strFolder;

	m_wndFileListCtrl.LoadFolder(strFolder);

	FileInfoList::iterator it;
	for ( it=m_wndFileListCtrl.m_FileList.begin(); 	it!=m_wndFileListCtrl.m_FileList.end();	it++ )
	{
		it->bFlag = this->GetFileFlag( (FILEINFO *)&(*it));

		int nItemIndex = m_wndFileListCtrl.SetFileFlagEx(it ->dwFileID, it->bFlag);
	}

	g_bEnableContinueOperation = TRUE;
}

void CUpLoadPhotoDlg::OnTvnDeleteitemDirTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	m_wndDirTree.OnDeleteShellItem(pNMHDR, pResult);
	*pResult = 0;
}

void CUpLoadPhotoDlg::OnTvnItemexpandingDirTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	//CWaitCursor wait;
	m_wndDirTree.OnFolderExpanding(pNMHDR,pResult);
	*pResult = 0;
}

// 添加
void CUpLoadPhotoDlg::OnBnClickedAdduploadButton()
{
	CWaitCursor wait;
	if(!g_bEnableContinueOperation)	return ;

	_auto_enable_ui_ ui(this);

	int nIndex = 0;
	FileInfoList lst;

	const int nMaxSideSize = get_max_side_size();
	POSITION pos = m_wndFileListCtrl.GetFirstSelectedItemPosition();
	if (pos == NULL)	return ;
	else
	{
		while (pos)
		{
			nIndex = m_wndFileListCtrl.GetNextSelectedItem(pos);

			DWORD dwFileID = DWORD(m_wndFileListCtrl.GetItemData(nIndex));
			FILEINFO *selectFileInfo = m_wndFileListCtrl.GetFileData(dwFileID);
			if(selectFileInfo == NULL)				continue;

			if(selectFileInfo->bFlag == 0)
			{
				FILEINFO temInfo = *selectFileInfo;
				temInfo.dwFileID = selectFileInfo->dwFileID;//rand();

				if(!CheckFileSize((LPTSTR)(LPCTSTR)temInfo.strFileName, temInfo.ulFileSize))
				{
					break;	//	continue ;
				}

				temInfo.nLimittedMaxSize = nMaxSideSize;

		//		m_wndUpLoadFileListCtrl.AddFile(temInfo);

				m_wndFileListCtrl.SetFileFlagEx(dwFileID, 1);
				m_wndFileListCtrl.DrawFileFlag(nIndex, 1);

				lst.push_back(temInfo);
				AddSelFile(&temInfo);
			} // end if(selectFileInfo->bFlag == 0) 
		} // end while (pos)
	} // end if(pos) ...

	if(lst.size() > 0)
	{
		m_wndFileListCtrl.DeselectItem();

		ShowFileMessage();
		
		m_wndUpLoadFileListCtrl.AddFileList(&lst, nMaxSideSize);
		m_wndUpLoadFileListCtrl.SelectItem(lst);
	}
}

// 全部添加
void CUpLoadPhotoDlg::OnBnClickedAddalluploadbutton()
{
	CWaitCursor wait;
	if(!g_bEnableContinueOperation)	return ;
	
	_auto_enable_ui_ ui(this);

	
	EnableDlgUI(FALSE);
	FileInfoList addFileList;
	int nCount = m_wndFileListCtrl.GetItemCount();
	for(int i = 0; i < nCount; ++i)
	{
		DWORD dwFileID = (DWORD)m_wndFileListCtrl.GetItemData(i);
		FILEINFO *selectfileInfo = m_wndFileListCtrl.GetFileData(dwFileID);
		if (selectfileInfo == NULL)			continue;

		if(selectfileInfo->bFlag == 0)
		{
			FILEINFO temInfo = *selectfileInfo;
			temInfo.dwFileID = selectfileInfo->dwFileID;	//rand();

			if(!CheckFileSize((LPTSTR)(LPCTSTR)temInfo.strFileName, temInfo.ulFileSize))
			{	
				break;	//	continue;
			}

			addFileList.push_back(temInfo);
			AddSelFile(&temInfo);

			m_wndFileListCtrl.SetFileFlagEx(dwFileID, 1);
			m_wndFileListCtrl.DrawFileFlag(i, 1);
		} // end if(selectfileInfo->bFlag == 0)
	} // end for

	if ( addFileList.size() > 0 )
	{
		int nMaxSideSize = get_max_side_size();
		m_wndUpLoadFileListCtrl.AddFileList(&addFileList, nMaxSideSize);
	}

	ShowFileMessage();
}

void CUpLoadPhotoDlg::OnBnClickedCancelButton()
{
	CXSkinDlg::OnCancel();
}

// 开始上传
void CUpLoadPhotoDlg::OnBnClickedUploadfileButton()
{
	CWaitCursor wait;

	if(m_bValidateSucceed == FALSE)
	{
		MessageBox("对不起,WoKu图片上传工具可能和网站失去联系,请关闭后刷新页面重新启动WoKu上传工具(USER_VALIDATE_FAILED),谢谢! ^^",
			APP_NAME, MB_OK | MB_ICONINFORMATION);
		return ;
	}

	
	if ( m_wndUpLoadFileListCtrl.m_FileList.size()==0 )
	{
		MessageBox("请先选择需要上传的图片.", APP_NAME,MB_OK|MB_ICONINFORMATION);
		return ;
	}

	EnableDlgUI(FALSE);

	// 访问属性
	g_AppConfigInfo.upfileprop.iViewProp = 0;
	int nCurSleItem = m_wndPanelDlg1.m_wndComboACL.GetCurSel();
	if(nCurSleItem != -1)
	{
		DWORD dwData = m_wndPanelDlg1.m_wndComboACL.GetItemData(nCurSleItem);
		g_AppConfigInfo.upfileprop.iViewProp = dwData;
	}
	
	// 相册,如果相册不存在则增加
	g_AppConfigInfo.upfileprop.iAlbumDirId = 0;
	const int nSel = m_wndPanelDlg1.m_wndComboAlbum.GetCurSel();
	if ( nSel !=- 1)
	{
		m_wndPanelDlg1.m_wndComboAlbum.GetLBText(nSel,	g_AppConfigInfo.upfileprop.strAlbumName);
		g_AppConfigInfo.upfileprop.iAlbumDirId = m_wndPanelDlg1.m_wndComboAlbum.GetItemData(nSel);
	}
	else if(nSel == -1)
	{
		BOOL bCreateNewAlbum = TRUE;
		CString strNewText, strText;

		m_wndPanelDlg1.m_wndComboAlbum.GetWindowText(strNewText);
		
		strNewText.Trim();
		if(strNewText.GetLength() > 0 && strNewText == _T("<请在这里输入或者选择图集>"))
		{
			strNewText.Empty();
		}

		if(strNewText.GetLength() > 0)
		{
			for(int i = 0; i < m_wndPanelDlg1.m_wndComboAlbum.GetCount(); i ++)
			{
				m_wndPanelDlg1.m_wndComboAlbum.GetLBText(i, strText);
				if(strText == strNewText)
				{
					bCreateNewAlbum = FALSE;
					g_AppConfigInfo.upfileprop.iAlbumDirId = m_wndPanelDlg1.m_wndComboAlbum.GetItemData(i);
				}
			} // end for

			if(bCreateNewAlbum)
			{
				int nRet = CreateNewAlbum(strNewText);
				if(nRet == -1)
				{
					MessageBox("无法创建新图集！\n请选择已有图集上传！", APP_NAME,MB_OK|MB_ICONINFORMATION);
					EnableDlgUI(TRUE);
					return ;
				}
				
				int nItem = m_wndPanelDlg1.m_wndComboAlbum.AddString(strNewText);
				m_wndPanelDlg1.m_wndComboAlbum.SetItemData(nItem, (DWORD)nRet);

				g_AppConfigInfo.upfileprop.iAlbumDirId = nRet;
			}
		} // end if
	}
	
//	m_wndEditTag.GetWindowText(g_AppConfigInfo.upfileprop.strTag);
	m_wndPanelDlg1.m_wndComboTAG.GetWindowText(g_AppConfigInfo.upfileprop.strTag);	
	if(g_AppConfigInfo.upfileprop.strTag == _T("<请在这里输入TAG,用逗号\",\"隔开,最多10个>"))
	{
		g_AppConfigInfo.upfileprop.strTag = _T("");
	}

	StartUpload();
}

// 移除
void CUpLoadPhotoDlg::OnBnClickedRemoveuploadfileButton()
{
	CWaitCursor wait;
	if(!g_bEnableContinueOperation)	return ;

	_auto_enable_ui_ ui(this);

	try
	{
		POSITION pos = m_wndUpLoadFileListCtrl.GetFirstSelectedItemPosition();
		int iCount   = m_wndUpLoadFileListCtrl.GetSelectedCount();

		int nIndex;

		if (pos == NULL)
			return;
		else
		{
			int i = 0;
			int *pnItemIndexArr = new int[m_wndFileListCtrl.GetItemCount()];
			for(int i = 0; i < m_wndFileListCtrl.GetItemCount(); ++ i)
			{
				pnItemIndexArr[i] = -1;
			}

			int * iIndex  = new int[iCount];
			int   iLast   = iCount - 1;

			m_wndFileListCtrl.DeselectItem();
			while (pos)
			{
				nIndex = m_wndUpLoadFileListCtrl.GetNextSelectedItem(pos);
				iIndex[iLast--] = nIndex; 	

				DWORD dwFileID = (DWORD)m_wndUpLoadFileListCtrl.GetItemData(nIndex);
				FILEINFO * pInfo = m_wndUpLoadFileListCtrl.GetFileData(dwFileID);

				if(pInfo != NULL)
				{
					int nItem =	m_wndFileListCtrl.SetFileFlagEx(pInfo, 0);
					pnItemIndexArr[i ++] = nItem;

					RemoveSelFile(pInfo);
				}
			}

			for(int i = 0; i < iCount; ++i)
			{
				m_wndUpLoadFileListCtrl.DeleteItem(iIndex[i]);
			}

			delete [] iIndex;

			for(int i = 0; i < m_wndFileListCtrl.GetItemCount(); ++ i)
			{
				m_wndFileListCtrl.SelectItem(pnItemIndexArr[i]);
			}

			delete []pnItemIndexArr;	pnItemIndexArr = NULL;
		}

	}
	catch( ... )
	{
		// ...
	}

	ShowFileMessage();
}

// 移除全部
void CUpLoadPhotoDlg::OnBnClickedRemovealluploadfilesButton()
{
	CWaitCursor wait;
	if(!g_bEnableContinueOperation)	return ;
	
	_auto_enable_ui_ ui(this);
	m_wndFileListCtrl.TerminateLoadThread();
	m_wndUpLoadFileListCtrl.TerminateLoadThread();
	
	bool bSetAll = false;
	int nCount = m_wndUpLoadFileListCtrl.GetItemCount();
	for(int i = 0; i < nCount; ++i)
	{
		DWORD dwFileID = DWORD(m_wndUpLoadFileListCtrl.GetItemData(i));
		FILEINFO *pInfo =m_wndUpLoadFileListCtrl.GetFileData(dwFileID);
		if (pInfo)
		{
			m_wndFileListCtrl.SetFileFlagEx(pInfo, 0);
			RemoveSelFile(pInfo);
		}
		else
		{
			bSetAll = true;
		}
	}
	if (bSetAll)
		m_wndFileListCtrl.SetAllFileFlag(0);
	m_wndUpLoadFileListCtrl.Clear();
	
	ShowFileMessage();
	_delete_all_tmp_files();
}

// 双击添加
void CUpLoadPhotoDlg::OnNMDblclkFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(g_bEnableContinueOperation == FALSE)		return ;

	CWaitCursor wait;
	_auto_enable_ui_ ui(this);

	int nIndex = m_wndFileListCtrl.GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
	if(nIndex == -1)				return ;
	
	DWORD dwFileID = DWORD(m_wndFileListCtrl.GetItemData(nIndex));
	FILEINFO *SelectFileInfo = m_wndFileListCtrl.GetFileData(dwFileID);
	if(SelectFileInfo == NULL)		return;

	if(SelectFileInfo->bFlag==0)
	{
		FILEINFO temInfo = *SelectFileInfo;
		temInfo.dwFileID = dwFileID;//rand();

		temInfo.nLimittedMaxSize = get_max_side_size();

	// for test
		if(!CheckFileSize((LPTSTR)(LPCTSTR)temInfo.strFileName, temInfo.ulFileSize))
		{
			return;
		}
#if 0
		if(this->m_wndUpLoadFileListCtrl.GetItemCount() >= 12)
		{
			MessageBox("每次最多上传12张图片!",APP_NAME,MB_OK|MB_ICONINFORMATION);
			return;
		}
#endif
		m_wndFileListCtrl.SetFileFlagEx(dwFileID, 1);
		m_wndFileListCtrl.DrawFileFlag(nIndex, 1);

		AddSelFile(&temInfo);
		m_wndUpLoadFileListCtrl.AddFile(temInfo);
		
		ShowFileMessage();
	}

	m_wndUpLoadFileListCtrl.SetFocus();
	*pResult = 0;
}

LRESULT	CUpLoadPhotoDlg::OnDestroyPreview(WPARAM wParam, LPARAM lParam)
{
	return 0;
}



void CUpLoadPhotoDlg::OnSize(UINT nType, int cx, int cy)
{
	CXSkinDlg::OnSize(nType, cx, cy);

	ResizePostion();

	if(m_hWnd && m_wndFileListCtrl.GetSafeHwnd())
	{
		CRgn rgn;

		GetRepaintRgn(rgn);
		InvalidateRgn(&rgn);
	}
}

void CUpLoadPhotoDlg::ResizePostion()
{
	if(m_hWnd && m_wndFileListCtrl.GetSafeHwnd())
	{

	}
	else
	{
		return ;
	}

	CRect rcTemp,rcClient;
	int   nDx, nDy;

	GetClientRect(&rcClient);

	//计算提示信息文本位置
	CWnd * pWnd = GetDlgItem(IDC_STATIC_LISTCTRL_PROMPT);
	pWnd->GetWindowRect(&rcTemp);
	ScreenToClient(&rcTemp);
	nDx = rcClient.right - 5 - rcTemp.right;
	rcTemp.left  += nDx;
	rcTemp.right += nDx;  
	pWnd->MoveWindow(&rcTemp);

	//计算最外层线框位置
	pWnd = GetDlgItem(IDC_STATIC_LINE2);
	pWnd->GetWindowRect(&rcTemp);
	ScreenToClient(&rcTemp);
	nDx = rcClient.right - 8 - rcTemp.right;
	nDy = rcClient.bottom - 8 - rcTemp.bottom;
	rcTemp.bottom+= nDy;
	rcTemp.right += nDx;  
	pWnd->MoveWindow(&rcTemp);	


	//计算文件列表树控件位置
	m_wndDirTree.GetWindowRect(&rcTemp);
	ScreenToClient(&rcTemp);
	nDy = rcClient.bottom - 16 - rcTemp.bottom;
	rcTemp.bottom += nDy;
	m_wndDirTree.MoveWindow(&rcTemp);

	int nHeight;

	//计算待选文件列表控件位置
	m_wndFileListCtrl.GetWindowRect(&rcTemp);
	ScreenToClient(&rcTemp);
	nDx = rcClient.right - 16 - rcTemp.right;
	nHeight = (rcClient.Height()-rcTemp.top-80 - 33 - 20)/2;
	rcTemp.right += nDx;  
	rcTemp.bottom = rcTemp.top + nHeight;
	m_wndFileListCtrl.MoveWindow(&rcTemp);

	//计算工具条位置
	rcTemp.top = rcTemp.bottom+2;
	rcTemp.bottom = rcTemp.top+30;
	m_wndUploadOperationsDlg.MoveWindow(&rcTemp);
	//m_wndUploadOperationsDlg.Invalidate();

	//计算已选文件列表控件位置
	rcTemp.top = rcTemp.bottom+2;
	rcTemp.bottom = rcTemp.top+nHeight;
	m_wndUpLoadFileListCtrl.MoveWindow(&rcTemp);
	m_wndUpLoadFileListCtrl.Invalidate();

	int nTemp = rcTemp.bottom;

	//计算Panel1
	m_wndPanelDlg1.GetWindowRect(&rcTemp);
	ScreenToClient(&rcTemp);
	rcTemp.top = nTemp+2;
	rcTemp.bottom = rcTemp.top+80;
	m_wndPanelDlg1.MoveWindow(&rcTemp);
	m_wndPanelDlg1.Invalidate();

	//计算Panel2
	m_wndPanelDlg2.GetWindowRect(&rcTemp);
	ScreenToClient(&rcTemp);	
	nDx = rcClient.right - rcTemp.right - 12;
	rcTemp.top = nTemp+2;
	rcTemp.left+=nDx;
	rcTemp.right+=nDx;
	rcTemp.bottom=rcTemp.top+80;
	m_wndPanelDlg2.MoveWindow(&rcTemp);

}

void CUpLoadPhotoDlg::GetRepaintRgn(CRgn &rgn)
{
	CRect rect;
	GetClientRect(&rect);

	rect.top+=22;

	rgn.CreateRectRgn(rect.left,rect.top,rect.right,rect.bottom);

	this->RemoveRgnOfWnd(rgn,&m_wndFileListCtrl);
	this->RemoveRgnOfWnd(rgn,&m_wndUpLoadFileListCtrl);

	this->RemoveRgnOfWnd(rgn, &m_wndUploadOperationsDlg);

	RemoveRgnOfWnd(rgn, &m_wndPanelDlg1);
 	RemoveRgnOfWnd(rgn, &m_wndPanelDlg2);

	this->RemoveRgnOfWnd(rgn,GetDlgItem(IDC_STATIC_LISTCTRL_PROMPT));
	this->RemoveRgnOfWnd(rgn,GetDlgItem(IDC_STATIC_TREECTRL_PROMT));

	this->RemoveRgnOfWnd(rgn,&m_wndDirTree);
}

BYTE CUpLoadPhotoDlg::GetFileFlag(FILEINFO *pInfo)
{
//	CString strFile;
	
//	if(pInfo->strOldFilePathName.GetLength() > 0)
//		strFile = pInfo ->strOldFilePathName;
//	else
	//	strFile = pInfo->strFileDir+"\\"+pInfo->strFileName;

	CString	strFile = pInfo ->strSourceFileDir + "\\" + pInfo ->strSourceFileName;

	map<CString,BOOL>::iterator it;

	BYTE bFlag ;	//0 normal, 1 sel, 2 uploaded

	bFlag = 0;
	CSyncCriticalSectionLock _lock(m_SyncMapSelFile);

	it = m_mapSelFile.find(strFile);

	if(it != m_mapSelFile.end())
	{
		bFlag = 1;
	}

	it = m_mapUpdFile.find(strFile);
	if(it != m_mapUpdFile.end())
	{
		bFlag = 2;
	}

	return bFlag;
}

void CUpLoadPhotoDlg::SetParamInfo(const CString & strParam)
{
	CString str;

	g_AppConfigInfo.userinfo.bIsMiniClinet = FALSE;
	// for mini client call
	GetSectionValue(strParam, "<mini_client>","</mini_client>", str);
	if(str.GetLength() < 1)	str.Format("1");
	if(atoi(str) == 0)
	{
		g_AppConfigInfo.userinfo.bIsMiniClinet = TRUE;
		g_AppConfigInfo.userinfo.bNeedvalidate = FALSE;
	}
	
	//////////////////////////////////////////////////////////////////
	GetSectionValue(strParam, "<username>","</username>",str);
	g_AppConfigInfo.userinfo.strUserName = str;

	GetSectionValue(strParam,"<nickname>","</nickname>",str);
	g_AppConfigInfo.userinfo.strNickName = str;

	GetSectionValue(strParam, "<password>", "</password>", str);
	g_AppConfigInfo.userinfo.strPassword = str;

	GetSectionValue(strParam,"<uid>","</uid>",str);
	g_AppConfigInfo.userinfo.nUserId = atol((LPCSTR)str);

	GetSectionValue(strParam,"<ver>","</ver>",str);
	g_AppConfigInfo.userinfo.nControlVersion = atol((LPCSTR)str);

	GetSectionValue(strParam,"<validkey>","</validkey>",str);
	g_AppConfigInfo.userinfo.strAuthKey = str;

	GetSectionValue(strParam,"<url>","</url>",str);
	g_AppConfigInfo.userinfo.strAuthUrl = str;//

	GetSectionValue(strParam,"<picurl>","</picurl>",str);
	g_AppConfigInfo.userinfo.strPicUrl  = str;

	GetSectionValue(strParam,"<album>","</album>",str);
	g_AppConfigInfo.userinfo.strDefaultAlbum  =  str;

	//mini public service 服务器名
	GetSectionValue(strParam,"<wsminipublic>","</wsminipublic>",str);

	str.Trim();
	if ( str!="" )
	{
		m_strMiniServerAddr = str;
	}

	//im servicve 服务器名
	GetSectionValue(strParam,"<wsimservice>","</wsimservice>",str);



	str.Trim();
	if ( str!="" )
	{
		m_strIMServiceAddr = str;
	}
	GetSectionValue(strParam, "<paraminfo>", "</paraminfo>", str);
	g_AppConfigInfo.userinfo.strParamInfo = str;

	GetSectionValue(strParam, "<paramkey>", "</paramkey>", str);
	g_AppConfigInfo.userinfo.strParamKey = str;


}

void CUpLoadPhotoDlg::AddUploadedFile(FILEINFO *pInfo)
{
//	CString strFile = pInfo->strFileDir+"\\"+pInfo->strFileName;

	CString	strFile = pInfo ->strSourceFileDir + "\\" + pInfo ->strSourceFileName;

	map<CString,BOOL>::iterator it = m_mapUpdFile.find(strFile);

	if ( it==m_mapUpdFile.end() )
	{
		m_mapUpdFile.insert(map<CString,BOOL>::value_type(strFile,TRUE));
	}
}

void CUpLoadPhotoDlg::AddSelFile(FILEINFO *pInfo)
{
//	CString strFile = pInfo->strFileDir+"\\"+pInfo->strFileName;

	CString	strFile = pInfo ->strSourceFileDir + "\\" + pInfo ->strSourceFileName;

	CSyncCriticalSectionLock _lock(m_SyncMapSelFile);
	map<CString, BOOL>::iterator it = m_mapSelFile.find(strFile);

	if ( it == m_mapSelFile.end() )
	{
		m_mapSelFile.insert(map<CString,BOOL>::value_type(strFile, TRUE));
	}
}

void CUpLoadPhotoDlg::RemoveSelFile(FILEINFO *pInfo)
{
	if(pInfo == NULL)	return ;

//	CString strFile;
//	strFile.Format("%s\\%s", pInfo->strFileDir, pInfo->strFileName);
	CString	strFile = pInfo ->strSourceFileDir + "\\" + pInfo ->strSourceFileName;

	CSyncCriticalSectionLock _lock(m_SyncMapSelFile);
	map<CString,BOOL>::iterator it = m_mapSelFile.find(strFile);

	if(it != m_mapSelFile.end())
	{
		m_mapSelFile.erase(it);
	}
}

// not use 
int CUpLoadPhotoDlg::ReplaceSelFile(const CString & strOldFilePathName, const CString & strNewFilePathName)
{
	CSyncCriticalSectionLock _lock(m_SyncMapSelFile);

	map<CString,BOOL>::iterator it = m_mapSelFile.find(strOldFilePathName);
	if(it != m_mapSelFile.end())
	{
		m_mapSelFile.erase(it);
	}

	m_mapSelFile.insert(map<CString,BOOL>::value_type(strNewFilePathName, TRUE));

	return m_mapSelFile.size();
}


LRESULT	CUpLoadPhotoDlg::OnPreview(WPARAM wParam, LPARAM lParam)
{
	FILEINFO *temInfo = (FILEINFO*)lParam;
	CString strFileName = temInfo->strFileDir + "\\" + temInfo->strFileName;

	CPreviewDlg::Show(strFileName);

	return 0;
}

HBRUSH CUpLoadPhotoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		if(pWnd ->GetDlgCtrlID() != IDC_STATIC_LINE2)		pDC->SetBkMode(TRANSPARENT);
		
		if(pWnd ->GetDlgCtrlID() == IDC_STATIC_TREECTRL_PROMT)
		{
			pDC ->SetTextColor(RGB(0, 0, 0));
			pDC->SetBkMode(TRANSPARENT);
		}
		
		if(pWnd ->GetDlgCtrlID() == IDC_STATIC_LISTCTRL_PROMPT)
		{
			pDC ->SetTextColor(RGB(0, 0, 255));
			pDC->SetBkMode(TRANSPARENT);
		}

		if(pWnd ->GetDlgCtrlID() == IDC_STATIC_TREECTRL_PROMT || pWnd ->GetDlgCtrlID() == IDC_STATIC_LISTCTRL_PROMPT)
		{
			CFont *pFont = pWnd ->GetFont();

			LOGFONT lf;
			pFont ->GetLogFont(&lf);
			lf.lfWeight = FW_BOLD;
		
			CFont Font;
			Font.CreateFontIndirect(&lf);
			
			pDC ->SelectObject(&Font);

			return (HBRUSH) m_br.GetSafeHandle();
		}

		pDC->SetBkColor(IM_SPACE_COLOR);

	//	return (HBRUSH)	m_br.GetSafeHandle();	

		return  (HBRUSH)GetStockObject(NULL_BRUSH);	//使背景透明
	}

	return hbr;
}

void CUpLoadPhotoDlg::OnCbnSelchangeTagCombo()
{
}

UINT CUpLoadPhotoDlg::InitProc(LPVOID pParam)
{
	CUpLoadPhotoDlg *pDlg = (CUpLoadPhotoDlg *)pParam;

	try
	{
		::CoInitialize(0);

		pDlg->DoInit();

		::CoUninitialize();

		pDlg->m_wndPanelDlg1.m_wndComboAlbum.EnableWindow(TRUE);
		pDlg->m_wndPanelDlg1.m_wndComboTAG.EnableWindow(TRUE);

		CloseHandle(pDlg->m_hThreadInit);
		pDlg->m_hThreadInit=NULL;
	}
	catch( ... )
	{
	}

	return 0;
}

void CUpLoadPhotoDlg::DoInit()
{	
	try
	{
		//g_AppConfigInfo.userinfo.bNeedvalidate = TRUE;
		if(g_AppConfigInfo.userinfo.bNeedvalidate == TRUE)
		{
			CString strAuthGet;
			strAuthGet.Format("%s?userid=%u&validkey=%s", (LPCSTR)g_AppConfigInfo.userinfo.strAuthUrl, g_AppConfigInfo.userinfo.nUserId, (LPCSTR)g_AppConfigInfo.userinfo.strAuthKey);

			//AfxMessageBox(strAuthGet);
			CString strAuthResult = UrlGetStr(strAuthGet);

			strAuthResult.Trim();
			strAuthResult.MakeLower();

			if(strAuthResult == "ok")
			{
				m_bValidateSucceed = TRUE;
				PostMessage(WM_THREAD_NOTIFY, THREAD_NOTIFY_MSG_USER_VALIDATE, 0);
			} 
			else
			{
				PostMessage(WM_THREAD_NOTIFY, THREAD_NOTIFY_MSG_USER_VALIDATE, 1);
				return ;
			}
		}
		else	// 不需要到服务器上进行验证
		{
			m_bValidateSucceed  = TRUE;
		}

		GetSpaceFromServer();
		GetAlbumListFromServer();
		GetTagListFromServer();

		m_wndPanelDlg1.OnCbnKillfocusDlgUploadTagCombo();

		CString tem;

// 		CString strServiceUrl;
// 		strServiceUrl.Format("http://%s/imservice.asmx",m_strIMServiceAddr);
// 		CIMService imService(strServiceUrl);
// 
// 		BSTR ret = L"";
// 		USES_CONVERSION;
// 		imService.GetAlbumInfo(g_AppConfigInfo.userinfo.nUserId,A2W((LPCSTR)g_AppConfigInfo.userinfo.strUserName),0,L"",&ret);
// 		tagAlbumInfo albumInfo;
// 		ParseAlbumInfo(CString(CComBSTR(ret)),albumInfo);
// 
// 		CString strTmp;
// 
// 		vector<tagAlbumListItem>::iterator it = albumInfo.vecAlbumList.begin();
// 		for( ; it != albumInfo.vecAlbumList.end(); ++it)
// 		{
// 			strTmp =  (*it).szAlbumListName;
// 			int iItem = m_wndPanelDlg1.m_wndComboAlbum.InsertString(-1, strTmp);
// 			m_wndPanelDlg1.m_wndComboAlbum.SetItemData(iItem,(*it).nAlbumListId);
// 		}
// 
// 		m_wndPanelDlg1.m_wndComboAlbum.SetCurSel(-1);
// 
// 		m_wndPanelDlg1.m_wndComboTAG.ResetContent();
// 		//	m_wndPanelDlg1.m_wndComboTAG.AddString("我的Tag");
// 		vector<tagAlbumTagItem>::iterator iter = albumInfo.vecAlbumTag.begin();
// 		for (; iter != albumInfo.vecAlbumTag.end(); ++iter)
// 		{
// 			m_wndPanelDlg1.m_wndComboTAG.InsertString(-1, (*iter).szAlbumTagName);
// 		}
 
 		//m_wndPanelDlg1.m_wndComboTAG.SetCurSel(-1);
 		m_wndPanelDlg1.m_wndComboTAG.SetTextColor(0xAAAAAA);
 		m_wndPanelDlg1.m_wndComboTAG.SetWindowText("<请在这里输入TAG,用逗号\",\"隔开,最多10个>");
 		//m_wndPanelDlg1.m_wndComboAlbum.SetTextColor(0xAAAAAA);
 		//m_wndPanelDlg1.m_wndComboAlbum.SetWindowText("<请在这里输入或者选择图集>");

	}
	catch( ... )
	{
	}
}

int CUpLoadPhotoDlg::GetAlbumListFromServer(void)
{
	TUserAlbumInfo albumInfo;

	GetUserAlbumList(g_AppConfigInfo.userinfo.nUserId,albumInfo);

	m_wndPanelDlg1.m_wndComboAlbum.ResetContent();

	for( int i=0; i<albumInfo.vecAlbum.size(); i++ )
	{
		int iItem = m_wndPanelDlg1.m_wndComboAlbum.InsertString(-1, albumInfo.vecAlbum[i].szAlbumName);
		m_wndPanelDlg1.m_wndComboAlbum.SetItemData(iItem, albumInfo.vecAlbum[i].nAlbumId);
	}

	m_wndPanelDlg1.m_wndComboAlbum.EnableWindow(TRUE);

	int nCount = m_wndPanelDlg1.m_wndComboAlbum.GetCount() ;
	if( nCount> 0) 
	{
		m_wndPanelDlg1.m_bEnableAlumTips = TRUE;
		m_wndPanelDlg1.m_wndComboAlbum.SetCurSel(-1);

		for ( int n=0; n<nCount; n++ )
		{
			CString str;
			
			m_wndPanelDlg1.m_wndComboAlbum.GetLBText(n,str);
			
			if ( g_AppConfigInfo.userinfo.strDefaultAlbum==str )
			{
				m_wndPanelDlg1.m_wndComboAlbum.SetCurSel(n);
				break;
			}
		}

		m_wndPanelDlg1.OnCbnKillfocusDlgUploadAlbumCombo();
	}
	else
	{
		m_wndPanelDlg1.m_bEnableAlumTips = TRUE;
		m_wndPanelDlg1.OnCbnKillfocusDlgUploadAlbumCombo();
	}

	return 0;
}

int CUpLoadPhotoDlg::GetTagListFromServer(void)
{
	vector<TTagInfo> info;

	try
	{
		GetUserPictureTagList(info);
	}
	catch(...)
	{
	}

	m_wndPanelDlg1.m_wndComboTAG.ResetContent();

	for( int i=0; i<info.size(); i++ )
	{
		m_wndPanelDlg1.m_wndComboTAG.InsertString(-1, info[i].szTagName);		
	}

	m_wndPanelDlg1.m_wndComboTAG.EnableWindow(TRUE);
	if(m_wndPanelDlg1.m_wndComboTAG.GetCount() > 0) 
	{
		//m_wndPanelDlg1.m_bEnableAlumTips = TRUE;
		m_wndPanelDlg1.m_wndComboTAG.SetCurSel(-1);
		m_wndPanelDlg1.OnCbnKillfocusDlgUploadAlbumCombo();
	}
	else
	{
		//m_wndPanelDlg1.m = TRUE;
		m_wndPanelDlg1.OnCbnKillfocusDlgUploadAlbumCombo();
	}

	return 0;
}

int CUpLoadPhotoDlg::CreateNewAlbum(const CString & strAlbumName)
{
	CString strError;
	long nReturnAlbumId = CreateAlbum(0,0,strAlbumName,0,strError);

	if ( nReturnAlbumId!=0 )
	{	
		return nReturnAlbumId;
	}
	else
	{
		CString strText;
		strText.Format(_T("添加新的相册时操作失败,原因:%s."), strError); // UTF8_Convert( FALSE));

		MessageBox(strText, APP_NAME, MB_ICONERROR);
	}
	
	return 0;
}

CString CUpLoadPhotoDlg::ParseAlbumName(const CString & strAlbumName)
{
	CString str(strAlbumName);
	CString str_01("<![CDATA["), str_02("]]>");

	str = str.Right(str.GetLength() - str_01.GetLength());
	str = str.Left(str.GetLength() - 3);

	return str;
}

int _get_remain_space_size()
{
	float flt = MAX_SPACE_SIZE - g_fltUsedSpaceTotal;
	int nRet = Round(flt, 0);
	
	return nRet;
}

void CUpLoadPhotoDlg::ShowFileMessage()
{
	try
	{
		const long nFileCount = this->m_wndUpLoadFileListCtrl.m_FileList.size();
		long nTotolSize = 0;

		FileInfoList::iterator it;
		for(it=m_wndUpLoadFileListCtrl.m_FileList.begin(); it!=m_wndUpLoadFileListCtrl.m_FileList.end(); it++ )
		{
			nTotolSize+=it->ulFileSize;
		}

		float fltTotalMB = float(nTotolSize) / 1024.0 / 1024.0;
		
	//	SetCurPos(MAX_SPACE_SIZE - g_RemainSpace);
		SetCurPos(g_fltUsedSpaceTotal);

	//	write_log_info("ShowFileMessage", "UpFileCount = %d, fltTotalMB = %f.\n", nFileCount, fltTotalMB);
	//	SetCurPos2((MAX_SPACE_SIZE - g_RemainSpace) + fltTotalMB);

		if(fltTotalMB > 0.0)		SetCurPos2(g_fltUsedSpaceTotal + fltTotalMB);
		else if(fltTotalMB == 0.0)	SetCurPos2(0);

		CString strDescription1, strDescription2;
	//	strDescription.Format("已选择%d个文件,共%.2f兆;可用空间:%uMB.",nFileCount, fltTotalMB, _get_remain_space_size());

		float fp = 100.0f; 

		fp *= (float)(g_fltUsedSpaceTotal/MAX_SPACE_SIZE); 
		//fp /= (float)(_get_remain_space_size() - 0); 

		strDescription1.Format("已选择%d个图片", nFileCount);
		strDescription2.Format("已占用可用空间%.2f%%(空间总量是1G)", fp);
		m_wndUploadOperationsDlg.SetSelectFileINFO(strDescription1, strDescription2);
	}
	catch( ... )
	{
		AfxMessageBox("CUpLoadPhotoDlg::ShowFileMessage, unknown exception caught!", MB_ICONERROR);
	}
}
void CUpLoadPhotoDlg::SetCurPos(const float nPos)
{
	if(m_wndUploadOperationsDlg.GetSafeHwnd() && ::IsWindow(m_wndUploadOperationsDlg.GetSafeHwnd()))
	{
	//	write_log_info("CUpLoadPhotoDlg::SetCurPos", "nPos = %f.\n", nPos);
	//	m_wndUploadOperationsDlg.m_wndProgressCtrl.SetCurPos(nPos);
	}
}

void CUpLoadPhotoDlg::SetCurPos2(const float nPos)
{
	if(m_wndUploadOperationsDlg.GetSafeHwnd() && ::IsWindow(m_wndUploadOperationsDlg.GetSafeHwnd()))
	{
	//	write_log_info("CUpLoadPhotoDlg::SetCurPos2", "nPos = %f.\n", nPos);
	//	m_wndUploadOperationsDlg.m_wndProgressCtrl.SetCurPos2(nPos);
	}
}

bool CUpLoadPhotoDlg::CheckFileSize(const CString & strFileName, ULONGLONG ulFileSize)
{
#if 0	// disable it.
	if ((float) ulFileSize / 1024.0 / 1024.0 > RARFILELARGESIZE)
	{
		CString strInfo;
		strInfo.Format("图片 %s 超出限制大小：%.2f 兆!",strFileName, RARFILELARGESIZE);
		MessageBox( strInfo ,APP_NAME,MB_OK|MB_ICONINFORMATION);
		return false;
	}
#endif

	long nTotolSize = 0;
	FileInfoList::iterator it = this->m_wndUpLoadFileListCtrl.m_FileList.begin();
	for (; it != this->m_wndUpLoadFileListCtrl.m_FileList.end(); it ++)
	{
		nTotolSize += it->ulFileSize;
	}

	nTotolSize += ulFileSize;

	float fltTotolSizeMB = nTotolSize / 1024.0 / 1024.0;
	ULONGLONG ulRemainSize = _get_remain_space_size(); // * 1024.0 * 1024.0;
	if (fltTotolSizeMB > ulRemainSize)
	{
		CString strInfo;
		strInfo.Format("对不起当前所选文件已超出可用空间的限制%d兆!", _get_remain_space_size());
		MessageBox(strInfo, APP_NAME,MB_OK|MB_ICONINFORMATION);

		return false;
	}

	return true;
}

LRESULT CUpLoadPhotoDlg::OnLoadFinished(WPARAM wParam, LPARAM lParam)
{
	if ( lParam==(DWORD)&this->m_wndUpLoadFileListCtrl )
	{
		ShowFileMessage();
	}

	return 0;
}

#include "UploadingDlg.h"

void CUpLoadPhotoDlg::StartUpload()
{
	FileInfoList::iterator it;

	m_lstUpLoadFleINFO.clear();
	const int nLimittedMaxSideSize = get_max_side_size();
	g_AppConfigInfo.nLimittedMaxSideSize = nLimittedMaxSideSize;

	for ( it = m_wndUpLoadFileListCtrl.m_FileList.begin(); it != m_wndUpLoadFileListCtrl.m_FileList.end() && m_bStopUploadThread == FALSE; it++ )
	{
	
		tagUpFileInfo _UpFileINFO;

		_UpFileINFO.bRemoveIt = FALSE;
		_UpFileINFO.bIsChanged = FALSE;

		_UpFileINFO.strDescription = (*it).strDescription;

		_UpFileINFO.nImageHeight = (*it).nImageHeight;
		_UpFileINFO.nImageWidth	 = (*it).nImageWidth;

		_UpFileINFO.nSoruceImageHeight = (*it).nSoruceImageHeight;
		_UpFileINFO.nSoruceImageWidth  = (*it).nSoruceImageWidth;

		_UpFileINFO.strFileDir = (*it).strFileDir;
		_UpFileINFO.strFileName = (*it).strFileName;

		_UpFileINFO.ulFileSize = (*it).ulFileSize;

		_UpFileINFO.strSourceFileName = it ->strDescription;
		_UpFileINFO.strSourceFilePathName = it->strSourceFileDir + "\\" + it->strSourceFileName;

		m_lstUpLoadFleINFO.push_back(_UpFileINFO);
	} // end for

	CUploadingDlg UploadingDlg(m_lstUpLoadFleINFO,  this);
	
	UploadingDlg.DoModal();
	g_AppConfigInfo.SaveConfigInfo();

	_lstUpFileINFO_iterator	_it = m_lstUpLoadFleINFO.begin();
	for(; _it != m_lstUpLoadFleINFO.end();)
	{
		if((*_it).bRemoveIt == TRUE)
		{
			_it = m_lstUpLoadFleINFO.erase(_it);
		}
		else
		{
			_it ++;
		}
	}

	SendMessage(WM_THREAD_NOTIFY, 0, 0);
}

LRESULT CUpLoadPhotoDlg::OnCancelUpload(WPARAM wParam,LPARAM lParam)
{
	CWaitCursor wait;

	m_bStopUploadThread = TRUE;

	if ( m_hThreadUpload )
	{
		try
		{
			//DWORD dw = 5000; // INFINITE;
			//DWORD dwRet = WaitForSingleObject(m_hThreadUpload, dw);
			//if(dwRet != WAIT_OBJECT_0)
			//{
			//	TerminateThread(m_hThreadUpload, 0);
			//}
			/*CloseHandle(m_hThreadUpload);*/
		}
		catch(...)
		{
		}
	}

	Sleep(1000 * 3);
	m_hThreadUpload = NULL;

	if ( this->m_pwndProgressBar )
	{
		m_pwndProgressBar->DestroyWindow();
		delete m_pwndProgressBar;
	}

	m_pwndProgressBar = NULL;

	EnableDlgUI(TRUE);
//	this->RemoveUploadedFile();

#ifdef _USE_XSKING_DLG
	this->UnLockOperation();
#endif

	if (m_lstUpLoadFleINFO.size() != this->m_wndUpLoadFileListCtrl.GetItemCount() )
//	if ( m_strUploadFilelst.GetCount()!=this->m_wndUpLoadFileListCtrl.GetItemCount() )
	{
		//	ShowAlbum(g_AppConfigInfo.userinfo.strUserName);
	}

//	this->RemoveUploadedFile();
	return 0;
}

LRESULT CUpLoadPhotoDlg::OnThreadNotify(WPARAM wParam, LPARAM lParam)
{
	try
	{
		//CWaitCursor wait;
		switch(wParam)
		{	
		case 0:
			{
				if(m_lstUpLoadFleINFO.size() != this->m_wndUpLoadFileListCtrl.GetItemCount() )
				{
					if ( g_AppConfigInfo.userinfo.bIsMiniClinet )
						ShowAlbum(g_AppConfigInfo.userinfo.strUserName,g_AppConfigInfo.upfileprop.iAlbumDirId);
					else
						MessageBox("图片上传成功!","Woku上传图片",MB_OK|MB_ICONINFORMATION);
				}
				else
				{
				}
			
				EnableDlgUI(TRUE);		
				RemoveUploadedFile();
				GetSpaceFromServer();//	_delete_all_tmp_files();
			}
			break;
		case 1:
			{
				if ( this->m_pwndProgressBar )
				{
					CString str;
					str.Format("正在上传图片(%d/%d)...", lParam, m_lstUpLoadFleINFO.size());
			//		write_log_info("CUpLoadPhotoDlg::OnThreadNotify", str);
					m_pwndProgressBar->SetTip(str);
				}
			}
			break;
		case THREAD_NOTIFY_MSG_USER_VALIDATE:	//验证成功失败
			{
				if ( lParam == 0 )		//success
				{
					m_wndPanelDlg2.m_BeginUpLoadButton.EnableWindow(TRUE);
				}
				else
				{
					AfxMessageBox("对不起,WoKu图片上传工具可能和网站失去联系,请关闭后刷新页面重新启动WoKu上传工具(USER_VALIDATE_FAILED),谢谢! ^^ ",MB_OK|MB_ICONINFORMATION);
				}
			}
			break;
		case THREAD_NOTIFY_MSG_LOAD_FOLDER_FILES_FINISHED:
			{
				AfxGetApp() ->EndWaitCursor();
				g_bEnableContinueOperation = TRUE;	m_bLoadFolder = FALSE;
			}
			break;
		case THREAD_NOTIFY_MSG_COMPRESSING_FILE:
			{
				if ( this->m_pwndProgressBar )
				{
					CString str("正在压缩图片 ...");
			//		str.Format("正在压缩图片(%d/%d)...", lParam, m_lstUpLoadFleINFO.size());
			//		write_log_info("CUpLoadPhotoDlg::OnThreadNotify", str);
					m_pwndProgressBar->SetTip(str);
				}
			}
			break;
		case THREAD_NOTIFY_MSG_CLOSE_PROGRESSBAR:
			{
				if ( this->m_pwndProgressBar )
				{
					m_pwndProgressBar->DestroyWindow();

					delete m_pwndProgressBar;
				}

				m_pwndProgressBar = NULL;
			}
			break;
		} // end switch(wParam)
	}
	catch (...)
	{
	}

	return 0;
}

void CUpLoadPhotoDlg::RemoveUploadedFile()
{
	int nCount = this->m_wndUpLoadFileListCtrl.GetItemCount();
	int nItem  = 0;

	while ( nItem<nCount )
	{
		DWORD      dwFileId = m_wndUpLoadFileListCtrl.GetItemData(nItem);
		FILEINFO * pInfo    = m_wndUpLoadFileListCtrl.GetFileData(dwFileId);

		if ( pInfo && dwFileId > 0 )
		{
			CString	strFile = pInfo ->strSourceFileDir + "\\" + pInfo ->strSourceFileName;
			if(IsNotExistInUploadFileLst(strFile))
			{
				int nItemIndex = m_wndFileListCtrl.SetFileFlagEx(pInfo, 2);
				m_wndFileListCtrl.DrawFileFlag(nItemIndex , 2);

				RemoveSelFile(pInfo);
				AddUploadedFile(pInfo);

				m_wndUpLoadFileListCtrl.DeleteItem(nItem);

				nCount--;	nItem--;
			}
		} // end if

		nItem ++;
	} // end while

	ShowFileMessage();
}

BOOL CUpLoadPhotoDlg::IsNotExistInUploadFileLst(const CString & strFilePathName)
{
	BOOL bExist = TRUE;
	_lstUpFileINFO_iterator _it = m_lstUpLoadFleINFO.begin();

	for(; _it != m_lstUpLoadFleINFO.end(); ++ _it)
	{
		if(_it ->strSourceFilePathName == strFilePathName)
		{
			 bExist = FALSE;
		}
	} // end for

	return bExist;
}

void CUpLoadPhotoDlg::OnDestroy()
{
	try
	{
		if(m_wndFileListCtrl.GetSafeHwnd())
		{
			m_wndFileListCtrl.TerminateLoadThread();
		}

		if(m_wndUpLoadFileListCtrl.GetSafeHwnd())
		{
			m_wndUpLoadFileListCtrl.TerminateLoadThread();
		}

		if(m_nTimerID != 0)
		{
			KillTimer(m_nTimerID);
		}

		_delete_all_tmp_files();

		SaveLastUploadPicFolder(this->m_strSelectDir);

		WINDOWPLACEMENT wp;
		wp.length = sizeof (WINDOWPLACEMENT);
		GetWindowPlacement (&wp);

		CString version("1.0");
		AfxGetApp()->WriteProfileInt (version, _T ("flags"), wp.flags);
		AfxGetApp()->WriteProfileInt (version, _T ("showCmd"), wp.showCmd);
		AfxGetApp()->WriteProfileInt (version, _T ("x1"), wp.rcNormalPosition.left);
		AfxGetApp()->WriteProfileInt (version, _T ("y1"), wp.rcNormalPosition.top);
		AfxGetApp()->WriteProfileInt (version, _T ("x2"), wp.rcNormalPosition.right);
		AfxGetApp()->WriteProfileInt (version, _T ("y2"), wp.rcNormalPosition.bottom);

		if ( m_hThreadUpload )
		{
			try
			{
				TerminateThread(m_hThreadUpload, 0);
				CloseHandle(m_hThreadUpload);
			}
			catch(...)
			{
			}
		}

		m_hThreadUpload = NULL;

		if ( this->m_pwndProgressBar )
		{
			m_pwndProgressBar->DestroyWindow();
			delete m_pwndProgressBar;
		}

		m_pwndProgressBar = NULL;

		CPreviewDlg::UnShow();
	}
	catch ( ... )
	{
	}

	CXSkinDlg::OnDestroy();
}

// Not use at current version
void CUpLoadPhotoDlg::OnBnClickedBtnAlbum()
{
	ShowAlbum(g_AppConfigInfo.userinfo.strUserName);
}

void CUpLoadPhotoDlg::OnFileListCtrlBeginDrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(!g_bEnableContinueOperation)		return ;

	try
	{
#if 1
		const int nItem = pNMListView->iItem;
		DWORD dwFileID = (DWORD)m_wndFileListCtrl.GetItemData(nItem);
		FILEINFO *pFileINFO = m_wndFileListCtrl.GetFileData(dwFileID);
		if(pFileINFO == NULL)	return ;

		if(pFileINFO ->bFlag != 0)
		{
			m_CursorType = CURSOR_TYPE_NODRAG;
			PostMessage(WM_SETCURSOR);

			CRect rcItem;
			m_wndFileListCtrl.GetItemRect(nItem, &rcItem, LVIR_ICON);

			m_wndFileListCtrl.ClientToScreen(&rcItem);	
			//	ClipCursor(rcItem);

			SetCapture();
			return ;
		}

		m_CursorType = CURSOR_TYPE_HANDLE;	PostMessage(WM_SETCURSOR);

		m_wndFileListCtrl.m_bDraging = TRUE;
		StartDragDrop(pNMListView, m_wndFileListCtrl);
#endif
	}
	catch ( ... )
	{
	}
}

void CUpLoadPhotoDlg::OnUploadListCtrlBeginDrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(!g_bEnableContinueOperation)	return ;

	m_CursorType = CURSOR_TYPE_HANDLE;	PostMessage(WM_SETCURSOR);
	m_wndUpLoadFileListCtrl.m_bDraging = TRUE;
	StartDragDrop(pNMListView, m_wndUpLoadFileListCtrl);
}

int CUpLoadPhotoDlg::StartDragDrop(NM_LISTVIEW* pNMListView, CThumBnaiListCtrl & ListCtrl)
{
	if(!g_bEnableContinueOperation)	return 0;

	try
	{
		CRect rc;
		GetWindowRect(&rc);
		ClipCursor(&rc);

		m_nDragIndex = pNMListView->iItem;

		//// Create a drag image
		POINT pt;
		int nOffset = -10; //offset in pixels for drag image (positive is up and to the left; neg is down and to the right)
		if(ListCtrl.GetSelectedCount() > 1) //more than one item is selected
			pt.x = nOffset;
		pt.y = nOffset;

		LVITEM lv;
		ZeroMemory(&lv, sizeof(LVITEM));

		lv.iItem = m_nDragIndex;
		lv.mask = LVIF_IMAGE;
		ListCtrl.GetItem(&lv);

		CImageList *pImageLst =	ListCtrl.GetImageList(LVSIL_NORMAL);

		m_pDragImage = new 	CImageList;
		m_pDragImage ->Create(pImageLst);
		//	m_pDragImage = ListCtrl.CreateDragImage(m_nDragIndex, &pt);
		ASSERT(m_pDragImage); //make sure it was created
		//We will call delete later (in LButtonUp) to clean this up

		//	m_pDragImage ->SetBkColor(RGB(255, 255, 255));

//#if 0
//		CBitmap bitmap;
//		if(ListCtrl.GetSelectedCount() > 1) //more than 1 item in list is selected
//			bitmap.LoadBitmap(IDB_BITMAP_MULTI);
//			bitmap.LoadBitmap(IDB_DRAGDROP_MULTI);
//		else
//			bitmap.LoadBitmap(IDB_DRAGDROP_SINGLE);
//
//		m_pDragImage->Replace(0, &bitmap, &bitmap);
//#endif

		//// Change the cursor to the drag image
		////	(still must perform DragMove() in OnMouseMove() to show it moving)
		m_pDragImage->BeginDrag(lv.iImage, CPoint(nOffset, nOffset - 4));
		m_pDragImage->DragEnter(GetDesktopWindow(), pNMListView->ptAction);

		//// Set dragging flag and others
		ListCtrl.m_bDraging = m_bDragging = TRUE;	//we are in a drag and drop operation

		m_nDropIndex = -1;	//we don't have a drop index yet
		m_pDragList = &ListCtrl; //make note of which list we are dragging from
		m_pDropWnd = &ListCtrl;	//at present the drag list is the drop list

		//// Capture all mouse messages
		ReleaseCapture();	SetCapture ();
	}
	catch ( ... )
	{
	}

	return 0;
}

void CUpLoadPhotoDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	/*if(m_CursorType == CURSOR_TYPE_NODRAG)
	{
		CXSkinDlg::OnMouseMove(nFlags, point);
		return ;
	}*/
	

	try
	{
		//// If we are in a drag/drop procedure (m_bDragging is true)
		if (m_bDragging)
		{
			CRect rcFileListCtrl, rcOperationDlg, rcUpFileListCtrl;

			m_wndFileListCtrl.GetWindowRect(&rcFileListCtrl);
			m_wndUploadOperationsDlg.GetWindowRect(&rcOperationDlg);
			m_wndUpLoadFileListCtrl.GetWindowRect(&rcUpFileListCtrl);


			/*CRect rcLine2;
			rcLine2.SetRectEmpty();

			CWnd *pWnd = GetDlgItem(IDC_STATIC_LINE2);
			if(pWnd != NULL)	pWnd ->GetWindowRect(&rcLine2);
			
			CRect rect;
			rect.SubtractRect(rcLine2, rcDirTree);*/
			
			CRect rcDirTree;
			m_wndDirTree.GetWindowRect(&rcDirTree);

			CPoint pt_(point);
			ClientToScreen(&pt_);	

			if(rcFileListCtrl.PtInRect(pt_) || rcOperationDlg.PtInRect(pt_) || rcUpFileListCtrl.PtInRect(pt_))
		//	if(!rcDirTree.PtInRect(pt_))
			{
				m_CursorType = CURSOR_TYPE_HANDLE;
			}
			else
			{
				m_CursorType = CURSOR_TYPE_NODRAG;
			}
			
			PostMessage(WM_SETCURSOR);

			//// Move the drag image
			CPoint pt(point);	//get our current mouse coordinates
			ClientToScreen(&pt); //convert to screen coordinates

			int x = THUMBNAIL_WIDTH / 2;
			int y = THUMBNAIL_HEIGHT / 2;

			pt.x -= (x + 5);	pt.y -= y;
			m_pDragImage->DragMove(pt); //move the drag image to those coordinates

			// Unlock window updates (this allows the dragging image to be shown smoothly)
			//	m_pDragImage->DragShowNolock(FALSE);

			//// Get the CWnd pointer of the window that is under the mouse cursor
			CWnd* pDropWnd = WindowFromPoint (pt);
		//	ASSERT(pDropWnd); //make sure we have a window

			CRect rc;
			GetWindowRect(&rc);
			if(rc.PtInRect(pt) && pDropWnd != NULL && pDropWnd ->GetSafeHwnd() && ::IsWindow(pDropWnd ->GetSafeHwnd()))
			{
				// Save current window pointer as the CListCtrl we are dropping onto
				m_pDropWnd = pDropWnd;

				// Convert from screen coordinates to drop target client coordinates
				pDropWnd->ScreenToClient(&pt);
			}
		} // end if
	}
	catch ( ... )
	{
	}

	if(m_wndPanelDlg2.m_wndStaticHyperLink.GetSafeHwnd() != NULL)
	{
		CRect rc;
		m_wndPanelDlg2.m_wndStaticHyperLink.GetWindowRect(&rc);
	}

	CXSkinDlg::OnMouseMove(nFlags, point);
}

void CUpLoadPhotoDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	ClipCursor(NULL);


	// Release mouse capture, so that other controls can get control/messages
	ReleaseCapture ();

	CXSkinDlg::OnLButtonUp(nFlags, point);

	if(m_CursorType == CURSOR_TYPE_NODRAG)
	{
		// 在此释放当前光标
		m_CursorType = CURSOR_TYPE_NORMAL;	PostMessage(WM_SETCURSOR);
		
		if(m_pDragImage != NULL && m_bDragging)
		{
			m_bDragging = FALSE;
			m_pDragImage->EndDrag ();
			delete m_pDragImage; //must delete it because it was created at the beginning of the drag
		}

		return ;
	}

	try
	{
		if(!g_bEnableContinueOperation)		return ;

		if (m_bDragging && g_bEnableContinueOperation)
		{
			m_CursorType = CURSOR_TYPE_NORMAL;	PostMessage(WM_SETCURSOR);

			// Note that we are NOT in a drag operation

			m_bDragging = FALSE;

			m_wndFileListCtrl.m_bDraging = FALSE;
			m_wndUpLoadFileListCtrl.m_bDraging = FALSE;

		//	m_pDragImage ->DragShowNolock(TRUE);

			// End dragging image
		//	m_pDragImage->DragLeave (GetDesktopWindow ());
			m_pDragImage->EndDrag ();
			delete m_pDragImage; //must delete it because it was created at the beginning of the drag

			CPoint pt (point); //Get current mouse coordinates
			ClientToScreen (&pt); //Convert to screen coordinates
			// Get the CWnd pointer of the window that is under the mouse cursor
			CWnd* pDropWnd = WindowFromPoint (pt);
			ASSERT (pDropWnd); //make sure we have a window pointer
			// If window is CListCtrl, we perform the drop
			if (pDropWnd != NULL && pDropWnd ->GetSafeHwnd() && ::IsWindow(pDropWnd ->GetSafeHwnd())
				&& pDropWnd->IsKindOf(RUNTIME_CLASS(CListCtrl)) && (pDropWnd == &m_wndFileListCtrl || pDropWnd == &m_wndUpLoadFileListCtrl))
			{
				m_pDropList = (CListCtrl*)pDropWnd;		//Set pointer to the list we are dropping on
				DropItemOnList(m_pDragList, m_pDropList); //Call routine to perform the actual drop
			}
		} // end if
	}
	catch( ... )
	{
	}

//	CXSkinDlg::OnLButtonUp(nFlags, point);
}

void CUpLoadPhotoDlg::DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList)
{
	//Send:	pDragList = pointer to CListCtrl we dragged from,
	//		pDropList = pointer to CListCtrl we are dropping on.

	_EnableWindow en(m_hWnd);
	if(!g_bEnableContinueOperation)	return ;

	_auto_enable_ui_ ui(this);

	try
	{
		const int nIndex = m_nDragIndex;
		if(pDragList->GetSelectedCount() == 1)		// 添加 单个
		{
			if(m_pDragList != NULL && m_pDragList == &m_wndFileListCtrl && m_pDropList != NULL && m_pDropList == &m_wndUpLoadFileListCtrl)
			{
				DWORD dwFileID = DWORD(m_wndFileListCtrl.GetItemData(nIndex));
				FILEINFO *SelectFileInfo = m_wndFileListCtrl.GetFileData(dwFileID);
				if(SelectFileInfo == NULL)				return;

				if(SelectFileInfo->bFlag == 0)
				{
					FILEINFO temInfo = *SelectFileInfo;
					temInfo.dwFileID = dwFileID;//rand();

					if(!CheckFileSize((LPTSTR)(LPCTSTR)temInfo.strFileName, temInfo.ulFileSize))
					{
						return;
					}
#if 0	// for test
					if(this->m_wndUpLoadFileListCtrl.GetItemCount() >= 12)
					{
						MessageBox("每次最多上传12张图片!",APP_NAME,MB_OK|MB_ICONINFORMATION);
						return;
					}
#endif
					int nItem = m_wndFileListCtrl.SetFileFlagEx(dwFileID, 1);
					m_wndFileListCtrl.DrawFileFlag(nIndex, 1);
					m_wndFileListCtrl.DeselectItem(nIndex);

					temInfo.nLimittedMaxSize = get_max_side_size();

					AddSelFile(&temInfo);
					m_wndUpLoadFileListCtrl.AddFile(temInfo);
				} // end if(selectFileInfo->bFlag == 0)
			}
			// 拖回去
			else if(m_pDragList != NULL && m_pDragList == &m_wndUpLoadFileListCtrl  && m_pDropList != NULL && m_pDropList == &m_wndFileListCtrl)
			{
				DWORD dwFileID = (DWORD)m_wndUpLoadFileListCtrl.GetItemData(nIndex);
				FILEINFO * pInfo = m_wndUpLoadFileListCtrl.GetFileData(dwFileID);

				m_wndUpLoadFileListCtrl.DeselectItem();
				m_wndFileListCtrl.DeselectItem();  

				int nItem = m_wndFileListCtrl.SetFileFlagEx(pInfo, 0);
				m_wndFileListCtrl.SelectItem(nItem);

				m_wndFileListCtrl.SetFocus();

				RemoveSelFile(pInfo);
				m_wndUpLoadFileListCtrl.DeleteItem(nIndex);
			}

			ShowFileMessage();
		}
		else			//  当前选择了多个
		{
			if(m_pDragList != NULL && m_pDragList == &m_wndFileListCtrl && m_pDropList != NULL && m_pDropList == &m_wndUpLoadFileListCtrl)
			{
				OnBnClickedAdduploadButton();
				m_wndUpLoadFileListCtrl.SetFocus();
			}
			// 拖回去
			else if(m_pDragList != NULL && m_pDragList == &m_wndUpLoadFileListCtrl && m_pDropList != NULL && m_pDropList == &m_wndFileListCtrl)
			{
				OnBnClickedRemoveuploadfileButton();
				m_wndFileListCtrl.SetFocus();
			}

			ShowFileMessage();
		} // end if
	}
	catch(...)
	{
	}
}

void CUpLoadPhotoDlg::OnUploadListCtrlBeginLabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;

	// 保存下当前的数据
	if(pDispInfo != NULL && pDispInfo ->item.iItem != -1)
	{
		m_wndUpLoadFileListCtrl.GetEditControl() ->SetLimitText(100);

		CString strItemText;
		strItemText = m_wndUpLoadFileListCtrl.GetItemText(pDispInfo ->item.iItem, 0);
		m_wndUpLoadFileListCtrl.m_strOldItemText = strItemText;
	}
}

void CUpLoadPhotoDlg::OnUploadListCtrlEndLabelEdit(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	//NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
	LV_ITEM *plvItem = &pDispInfo ->item;

	DWORD dwFileID = DWORD(m_wndUpLoadFileListCtrl.GetItemData(plvItem ->iItem));
	FILEINFO *pFileInfo = m_wndUpLoadFileListCtrl.GetFileData(dwFileID);
	if(pFileInfo != NULL && plvItem ->pszText != NULL)
	{
		CString strItemText(plvItem ->pszText);
		strItemText.Trim();	strItemText.TrimLeft(), strItemText.TrimRight();

		if(strItemText.GetLength() < 1)
		{
			m_wndUpLoadFileListCtrl.SetItemText(plvItem ->iItem, 0, m_wndUpLoadFileListCtrl.m_strOldItemText);
		}
		else
		{
			if(strItemText.GetLength() > 50)
			{
				MessageBox("对不起,描述信息长度不能超过25个汉字,请重新输入!", APP_NAME, MB_ICONEXCLAMATION);
				m_wndUpLoadFileListCtrl.EditLabel(plvItem ->iItem);
				return ;
			}
			
			pFileInfo ->strDescription = strItemText;
			m_wndUpLoadFileListCtrl.SetItemText(plvItem ->iItem, 0, pFileInfo->strDescription);
		}
	} // end if
}

BOOL CUpLoadPhotoDlg::PreTranslateMessage(MSG* pMsg)
{
//	if(m_hAccel != NULL)	TranslateAccelerator(m_hWnd, m_hAccel, pMsg);

	if(pMsg ->message == WM_KEYDOWN)
	{
		if(pMsg ->wParam == VK_DELETE && GetFocus() == &m_wndUpLoadFileListCtrl)
		{
			if(g_bEnableContinueOperation == TRUE)
			{
				OnBnClickedRemoveuploadfileButton();
			}
		}
		else if(pMsg ->wParam == VK_F2 && GetFocus() == &m_wndUpLoadFileListCtrl)
		{
			if(g_bEnableContinueOperation == TRUE)
			{
				int nCurSel = m_wndUpLoadFileListCtrl.GetCurSel();
				if(nCurSel >= 0)
				{
					CEdit *pEdit = 	m_wndUpLoadFileListCtrl.EditLabel(nCurSel);
				}
			}
		} // end if
	} 

	return CXSkinDlg::PreTranslateMessage(pMsg);
}

void CUpLoadPhotoDlg::OnSetFocus(CWnd* pOldWnd)
{
	CXSkinDlg::OnSetFocus(pOldWnd);

	if(m_wndUpLoadFileListCtrl.GetSafeHwnd() && ::IsWindow(m_wndUpLoadFileListCtrl.GetSafeHwnd()))
	{
//		m_wndUpLoadFileListCtrl.SetFocus();
	}
}

BOOL CUpLoadPhotoDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if(g_bEnableContinueOperation == FALSE)
	{
		//SetCursor(AfxGetApp() ->LoadStandardCursor(MAKEINTRESOURCE(IDC_WAIT)));
		
		return TRUE;
	//	return CXSkinDlg::OnSetCursor(pWnd, nHitTest, message);
	}

	if(pWnd != NULL && pWnd == &m_wndPanelDlg2.m_wndStaticHyperLink)
	{
		SetCursor(m_hHyperLinkCursor);	return TRUE;
	}

	if(m_CursorType == CURSOR_TYPE_NODRAG)
	{
		SetCursor(m_hNoDragCursor);		return TRUE;
	}
	else if(m_CursorType == CURSOR_TYPE_HANDLE)
	{
		SetCursor(m_hHandleCursor);		return TRUE;
	}
	else if(pWnd != NULL && pWnd == &m_wndDirTree && m_bDragging == TRUE)
	{
		SetCursor(m_hNoDragCursor);		return TRUE;
	}
	
	return CXSkinDlg::OnSetCursor(pWnd, nHitTest, message);
}

int CUpLoadPhotoDlg::get_max_side_size()
{
	// 最长边
	CString strText;
	int nMaxSideSize = -1;

	if(m_wndPanelDlg1.m_wndComboLimitSize.GetCurSel() != -1)
	{
		m_wndPanelDlg1.m_wndComboLimitSize.GetLBText(m_wndPanelDlg1.m_wndComboLimitSize.GetCurSel(), strText);
		nMaxSideSize = atoi(strText);
	}

	return nMaxSideSize;
}

void CUpLoadPhotoDlg::OnBtnResetInfo(void)
{
	m_wndPanelDlg1.m_wndComboTAG.SetWindowText("");

	m_wndPanelDlg1.m_wndComboACL.SetCurSel(2);
	m_wndPanelDlg1.m_wndComboLimitSize.SetCurSel(3);
	
	m_wndPanelDlg1.m_wndComboAlbum.SetCurSel(-1);  // m_wndPanelDlg1.m_wndComboAlbum.GetCount() > 0 ? 0 : -1);

	m_wndPanelDlg1.OnCbnKillfocusDlgUploadTagCombo();
	m_wndPanelDlg1.OnCbnKillfocusDlgUploadAlbumCombo();
}

void CUpLoadPhotoDlg::OnNMDblclkUploadfileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	OnBnClickedRemoveuploadfileButton();
}

int CUpLoadPhotoDlg::GetSpaceFromServer(void)
{
	int nRemainSpaceTotal = GetSpaceSize();

	g_RemainSpace = nRemainSpaceTotal / 1024.0/1024.0;//MAX_SPACE_SIZE - g_fltUsedSpaceTotal;
	g_fltUsedSpaceTotal = MAX_SPACE_SIZE - g_RemainSpace;

	SetCurPos(g_fltUsedSpaceTotal);
	SetCurPos2(0);

	ShowFileMessage();

	return 0;
}

BOOL CUpLoadPhotoDlg::OnEraseBkgnd(CDC* pDC)
{

	CRect rect;
	GetClientRect(&rect);

	CRgn rgn, rgn_;
	GetRepaintRgn(rgn);

	pDC->SelectObject(&rgn);
	pDC->FillSolidRect(&rect, IM_SPACE_COLOR);

//	rgn_.CreateRectRgn(rect.left, rect.top, rect.right, rect.bottom);
//	pDC ->SelectObject(&rgn_);

	CDC & dc = *pDC;

	
	CRect rc;

#if 0
	CWnd *pLine2 = GetDlgItem(IDC_STATIC_LINE2);
	pLine2->GetWindowRect(rc);
	ScreenToClient(&rc);	
	dc.FillSolidRect(rc,0xFFFFFF);
	dc.Draw3dRect(rc,IM_LINE_COLOR,IM_LINE_COLOR);

	m_wndDirTree.GetWindowRect(&rc);
	ScreenToClient(&rc);					 
	rc.InflateRect(2,2);
	dc.Draw3dRect(rc,IM_LINE_COLOR,IM_LINE_COLOR);

	m_wndFileListCtrl.GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.InflateRect(2,2);
	dc.Draw3dRect(rc,IM_LINE_COLOR,IM_LINE_COLOR);

	m_wndUpLoadFileListCtrl.GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.InflateRect(2,2);
	dc.Draw3dRect(rc,IM_LINE_COLOR,IM_LINE_COLOR);
#endif
	return FALSE;
}

void CUpLoadPhotoDlg::RedrawUploadFileListPics(const int nMaxSideSize)
{	
	if(m_wndUpLoadFileListCtrl.GetSafeHwnd())
	{
		//CWaitCursor wait;
		m_wndUpLoadFileListCtrl.SetFocus();
#if 0
	//	m_wndUpLoadFileListCtrl.RedrawPics(nMaxSideSize);

		int nItemCount = m_wndUpLoadFileListCtrl.GetItemCount();
		for(int i = 0; i < nItemCount; ++ i)
		{
			DWORD dwFileID = (DWORD) m_wndUpLoadFileListCtrl.GetItemData(i);
			FILEINFO *pFile = m_wndUpLoadFileListCtrl.GetFileData(dwFileID);

			if(pFile == NULL)	continue;

				CompressFileIfNeed(pFile, nMaxSideSize);
		} // end for

		ShowFileMessage();
#endif
	}
}


void CUpLoadPhotoDlg::OnClose()
{
//	if(!g_bEnableContinueOperation)	return ;

	CXSkinDlg::OnClose();
}

void CUpLoadPhotoDlg::EnableDlgUI(const BOOL bEnable)
{
	try
	{
		if(m_wndPanelDlg2.GetSafeHwnd() && ::IsWindow(m_wndPanelDlg2.GetSafeHwnd()))
		{
			m_wndPanelDlg2.m_BeginUpLoadButton.EnableWindow(bEnable);
			m_wndPanelDlg2.m_wndStaticHyperLink.EnableWindow(bEnable);
			m_wndPanelDlg2.m_wndResetInfo.EnableWindow(bEnable);

			m_wndPanelDlg2.Invalidate();
		}

		if(m_wndPanelDlg1.GetSafeHwnd() && ::IsWindow(m_wndPanelDlg1.GetSafeHwnd()))
		{
			m_wndPanelDlg1.m_wndComboACL.EnableWindow(bEnable);
			m_wndPanelDlg1.m_wndComboAlbum.EnableWindow(bEnable);
			m_wndPanelDlg1.m_wndComboTAG.EnableWindow(bEnable);
			m_wndPanelDlg1.m_wndComboLimitSize.EnableWindow(bEnable);
		}

		if(m_wndUploadOperationsDlg.GetSafeHwnd() &&::IsWindow(m_wndUploadOperationsDlg.GetSafeHwnd()))
		{
			m_wndUploadOperationsDlg.m_AddButton.EnableWindow(bEnable);
			m_wndUploadOperationsDlg.m_AddAllButton.EnableWindow(bEnable);
			m_wndUploadOperationsDlg.m_RemoveButton.EnableWindow(bEnable);
			m_wndUploadOperationsDlg.m_RemoveAllButton.EnableWindow(bEnable);

			m_wndUploadOperationsDlg.EnableWindow(bEnable);

			m_wndUploadOperationsDlg.Invalidate();

			m_wndUpLoadFileListCtrl.SetFocus();
		}
	}
	catch(...)
	{
	}
}
void CUpLoadPhotoDlg::OnTimer(UINT_PTR nIDEvent)
{
	if(nIDEvent == 11)
	{
		static BOOL bEnable = TRUE;

		if(bEnable != g_bEnableContinueOperation)
		{
			bEnable = g_bEnableContinueOperation;
			EnableDlgUI(bEnable);
		}
	}

	if (nIDEvent == 20)
	{
		KillTimer(20);

		if (this->IsIconic())
		{
			this->ShowWindow(SW_RESTORE);
		}
		//AfxMessageBox("test");
		this->BringWindowToTop();
		this->SetActiveWindow();
	}


	CXSkinDlg::OnTimer(nIDEvent);
}

void CUpLoadPhotoDlg::OnBtnHyperLink()
{
	ShowAlbum(g_AppConfigInfo.userinfo.strUserName);
}

void CUpLoadPhotoDlg::OnCancel()
{
	CXSkinDlg::OnCancel();
}

void CUpLoadPhotoDlg::OpenHtmlPage(const CString & strUrl)
{
	CString strProgramFilePath, strIexploreFilePathName;
	SHGetSpecialFolderPath(m_hWnd,strProgramFilePath.GetBufferSetLength(MAX_PATH), CSIDL_PROGRAM_FILES,FALSE);

	strIexploreFilePathName.Format("%s\\Internet Explorer\\iexplore.exe", strProgramFilePath);
	ShellExecute(NULL,"open", strIexploreFilePathName,strUrl,"",SW_SHOW);
}

void CUpLoadPhotoDlg::OnHelpClick()
{
	OpenHtmlPage("www.feiniu.com");
}

LRESULT CUpLoadPhotoDlg::BringToTop(WPARAM wParam, LPARAM lParam)
{
	SetTimer(20,10,NULL);
	//AfxMessageBox("test");
// 	if (this->IsIconic())
// 	{
// 		this->ShowWindow(SW_RESTORE);
// 	}
// 	this->SetActiveWindow();
// 	this->BringWindowToTop();
	
	return 0;
}
//void CUpLoadPhotoDlg::AddUploadedFile(CStringList & strFileLst)
//{
//	
//}