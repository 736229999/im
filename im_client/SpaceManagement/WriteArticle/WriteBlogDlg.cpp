#include "..\myarticles\writeblogdlg.h"
// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteArticle.h"
#include "writeblogdlg.h"
#include "imagefiledialog.h"
#include "createdirdlg.h"

#include "formprogress.h"
#include "webdataprocess.h"
#include "webpictureprocess.h"
#include "WebInterfaceWrapper.h"

#include "webfacedlg.h"

#include "TestWebPicDlg.h"
#include "webpicdlg.h"
#include "createdirdlg.h"
#include ".\writeblogdlg.h"

#define IDM_CUT		0xA001
#define IDM_COPY	0xA002
#define IDM_PASTE   0xA003
#define IDM_UNDO    0xA004//撤销
#define IDM_REDO	0xA005//恢复
#define IDM_ALL		0xA006
#define IDM_RELOAD  0xA007
#define IDR_MAINMENU 0x1234

#define TAG_HINT_STR "<请在这里输入TAG,用逗号“,”隔开,最多10个>"

#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0 )
static 	HBRUSH              g_hBackground	= NULL;
CWebInterfaceWrapper         g_WebInterFace;

CString MakeGuid(const char *szText)
{
	MD5_CTX md5T;
	unsigned char digest[16];

	md5T.MD5Update ((unsigned char*)szText, strlen(szText) );
	md5T.MD5Final (digest);

	CString str;
	str.Format("%x-%x-%x-%x",
		(((long *)digest))[0],
		(((long *)digest))[1],
		(((long *)digest))[2],
		(((long *)digest))[3]);

	return str;
}
// CWriteBlogPanel 对话框
IMPLEMENT_DYNAMIC(CWriteBlogPanel, CDialog)
CWriteBlogPanel::CWriteBlogPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CWriteBlogPanel::IDD, pParent)
{
	CString strFaceUrl = g_pUserInfo->GetFaceUrl();
	CString strFaceFile ;
	strFaceFile.Format("%sdata\\%s\\webicon\\%s",g_spaceData.GetPath(),(LPCSTR)g_pUserInfo->GetUserName(),MakeGuid((LPCSTR)strFaceUrl) );

	m_pBmpHead = CBitmapFromFile(strFaceFile,40,40);
}

CWriteBlogPanel::~CWriteBlogPanel()
{
	if ( m_pBmpHead )
	{
		delete m_pBmpHead;
		m_pBmpHead = NULL;
	}
}

void CWriteBlogPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TAG,m_comboTag);
	DDX_Control(pDX, IDC_COMBO_DIR,m_comboDir);
	DDX_Control(pDX, IDC_COMBO_VIEW,m_comboView);
	DDX_Control(pDX, IDC_CHECK_TOP, m_btnCheckTop);
	DDX_Control(pDX, IDC_EDIT1, m_editDir);
	DDX_Control(pDX, IDC_BTN_CREATEDIR, m_btnCreateDir);
	DDX_Control(pDX, IDC_STATIC_TAG, m_staticTag);
	DDX_Control(pDX, IDC_STATIC_APPARTS, m_staticAppArts);
	DDX_Control(pDX, IDC_STATIC_VIEWTYPE, m_staticViewType);
	DDX_Control(pDX, IDC_STATIC_MYSPACE,m_staticNickName);
	DDX_Control(pDX, IDC_STATIC_VIEWSPACE,m_btnHome);
	DDX_Control(pDX, IDC_STATIC_LOCAL1,m_btnLocal1);
	DDX_Control(pDX, IDC_STATIC_LOCAL2,m_btnLocal2);
	DDX_Control(pDX, IDC_STATIC_LOCAL3,m_btnLocal3);
	DDX_Control(pDX, IDC_BTN_DEL1,m_btnDel1);
	DDX_Control(pDX, IDC_BTN_DEL2,m_btnDel2);
	DDX_Control(pDX, IDC_BTN_DEL3,m_btnDel3);
	DDX_Control(pDX, IDC_STATIC_LINK,m_btnInsertLink);
	DDX_Control(pDX, IDC_STATIC_LOCALFILE,m_btnInsertLocal);
	DDX_Control(pDX, IDC_STATIC_WEB,m_btnInsertWeb);
	DDX_Control(pDX, IDC_STATIC_MORE,m_btnMore);
	DDX_Control(pDX, IDC_STATIC_WOKU,m_btnInsertWoku);

}


BEGIN_MESSAGE_MAP(CWriteBlogPanel, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_CBN_DROPDOWN(IDC_COMBO_DIR, OnCbnDropdownComboDir)
	ON_CBN_DROPDOWN(IDC_COMBO_TAG, OnCbnDropdownComboTag)
	ON_STN_CLICKED(IDC_STATIC_LINK2, OnStnClickedStaticLink2)
	ON_WM_PAINT()	
	
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CWriteBlogDlg, CXSkinDlg)
	ON_EVENT(CWriteBlogDlg, IDC_DHTMLEDIT_BLOG, 7, onmouseupDhtmleditBlog, VTS_NONE)	
	ON_EVENT(CWriteBlogDlg, IDC_DHTMLEDIT_BLOG, 12, onkeydownDhtmleditBlog, VTS_NONE)	
	ON_EVENT(CWriteBlogDlg, IDC_DHTMLEDIT_BLOG, 13, onkeypressDhtmleditBlog, VTS_NONE)
	ON_EVENT(CWriteBlogDlg, IDC_DHTMLEDIT_BLOG, 14, onkeyupDhtmleditBlog, VTS_NONE)
	ON_EVENT(CWriteBlogDlg, IDC_DHTMLEDIT_BLOG, 5, onmousedownDhtmleditBlog, VTS_NONE)
END_EVENTSINK_MAP()

BOOL CWriteBlogPanel::OnEraseBkgnd(CDC* pDC)
{
	CRect r;
	GetClientRect(&r);
	pDC->FillSolidRect(&r,RGB(255,255,255));
	pDC->Draw3dRect(&r,GetSysColor(COLOR_3DDKSHADOW),GetSysColor(COLOR_3DLIGHT));
	return FALSE;
}


CWriteBlogDlg *       CWriteBlogDlg::pInstance = NULL;
map<CString,CString>  CWriteBlogDlg::m_mapFile;
map<CString,long>     CWriteBlogDlg::m_mapFileSize;

// CWriteBlogDlg 对话框

IMPLEMENT_DYNAMIC(CWriteBlogDlg, CXSkinDlg)
CWriteBlogDlg::CWriteBlogDlg(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CWriteBlogDlg::IDD, pParent),
	m_wndBlogPanel(this)
{
	m_bFontBold   = false;	
	m_bFontItalic = false;
	m_bFontUnderLine = false;
//	m_wndEmotion.SetImageList(&g_imgEmotionList);

	m_hThreadPub   = NULL;
	m_hThreadInit  = NULL;
	m_hThreadArticleBook = NULL;
	m_hContent     = NULL;
	m_pWndProgress = NULL;

	m_dwSavedID    = 0;
	m_dwArticleID  = 0;
	m_hContent     = 0;

	m_articleType  = FreshArticle;
	m_bInitBookCombo = false;

	m_pEditBlog = NULL;

	pInstance = this;

	g_hBackground  = CreateSolidBrush(IM_SPACE_COLOR);
}

CWriteBlogDlg::~CWriteBlogDlg()
{
	m_mapFileSize.clear();
	m_mapFile.clear();

	if ( m_pEditBlog )
	{
		m_pEditBlog->DestroyWindow();
		delete m_pEditBlog;
		m_pEditBlog=NULL;
	}

	DeleteObject(g_hBackground);
	g_hBackground = NULL;

	if ( m_hContent )
	{
		try
		{
			TerminateThread(m_hContent,0);
			CloseHandle(m_hContent);
		}
		catch(...)
		{
		}

		m_hContent = NULL;
	}


	if ( m_hThreadPub )
	{
		try
		{
			TerminateThread(m_hThreadPub,0);
			CloseHandle(m_hThreadPub);
		}
		catch(...)
		{
		}

		m_hThreadPub=NULL;
	}

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

		m_hThreadInit=NULL;
	}


	if ( m_hThreadArticleBook )
	{
		try
		{
			TerminateThread(m_hThreadArticleBook,0);
			CloseHandle(m_hThreadArticleBook);
		}
		catch(...)
		{
		}

		m_hThreadArticleBook=NULL;
	}


	if ( m_pWndProgress )
	{
		m_pWndProgress->DestroyWindow();
		delete m_pWndProgress;
	}

	m_pWndProgress = NULL;
	
	pInstance = NULL;
}

void CWriteBlogDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_TITLE, m_edtTitle);
	DDX_Control(pDX, IDC_STATIC_HIDE, m_wndHide);
	DDX_Control(pDX, IDC_BUTTON_RESET,  m_buttonNew);
	DDX_Control(pDX, IDC_BUTTON_OPEN,  m_buttonOpen);
	DDX_Control(pDX, IDC_BUTTON_SAVE,  m_buttonSave);
	DDX_Control(pDX, IDC_BUTTON_POST,  m_buttonPost);
	DDX_Control(pDX, IDC_BUTTON_MYARTICLE, m_buttonMyArts);
	DDX_Control(pDX, IDC_BUTTON_INSERTLOCAL,m_btnInsertLocal);
	DDX_Control(pDX, IDC_BUTTON_INSERTWEB,m_btnInsertWeb);
	DDX_Control(pDX, IDC_BUTTON_INSERTWOKU,m_btnInsertWoku);
	DDX_Control(pDX, IDC_BUTTON_INSERTLINK,m_btnInsertLink);
}


BEGIN_MESSAGE_MAP(CWriteBlogDlg, CXSkinDlg)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_NCDESTROY()
	ON_MESSAGE(WM_WEBDATA_NOTIFY, OnWebDataNotify)
	ON_CBN_SELCHANGE(ID_BLG_FONTSIZE, OnCbnSelchangeComboFontsize)
	ON_CBN_SELCHANGE(ID_BLG_FONTNAME, OnCbnSelchangeComboFontname)
	ON_COMMAND(ID_BLG_BOLD,OnFontBold)
	ON_UPDATE_COMMAND_UI(ID_BLG_BOLD, OnUpdateBold)
	ON_COMMAND(ID_BLG_ITALIC,OnFontItalic)
	ON_UPDATE_COMMAND_UI(ID_BLG_ITALIC, OnUpdateItalic)
	ON_COMMAND(ID_BLG_UNDERLINE,OnFontUnderLine)
	ON_UPDATE_COMMAND_UI(ID_BLG_UNDERLINE, OnUpdateUnderLine)
	ON_COMMAND(ID_BLG_COLORMENU,OnFontColor)
	ON_COMMAND(ID_BLG_NUMBER,OnFormatNumber)
	ON_COMMAND(ID_BLG_BULLETS,OnFormatBullets)
	ON_COMMAND(ID_FORMAT_ALIGN_LEFT,OnFormatAlignLeft)
	ON_COMMAND(ID_FORMAT_ALIGN_RIGHT,OnFormatAlignRight)
	ON_COMMAND(ID_FORMAT_ALIGN_CENTER,OnFormatAlignCenter)
	ON_COMMAND(ID_WEBFACE,OnInsertEmotion)
	ON_COMMAND(ID_LINK,OnInsertLink)
	ON_COMMAND(IDC_BTN_CREATEDIR,OnCreateDir)
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BUTTON_RESET, OnBnClickedButtonReset)
	ON_BN_CLICKED(IDC_BUTTON_POST, OnBnClickedButtonPost)
	ON_BN_CLICKED(IDC_BUTTON_LOCALFILE, OnBnClickedButtonLocalfile)
	ON_MESSAGE(WM_CANCEL_NOTIFY,OnCancelUpload)
	ON_MESSAGE(WM_THREAD_NOTIFY,OnThreadNotify)
	ON_CBN_SELCHANGE(IDC_COMBO_TAG, OnCbnSelchangeComboMytag)
	ON_BN_CLICKED(IDC_BUTTON_WEBPIC, OnBnClickedButtonWebPic)	
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)	
	ON_CBN_KILLFOCUS(IDC_COMBO_TAG, OnCbnKillfocusComboTag)
	ON_CBN_SETFOCUS(IDC_COMBO_TAG, OnCbnSetfocusComboTag)
	ON_BN_CLICKED(IDC_STATIC_LINK,OnBnClickedMyArtilce)
	ON_BN_CLICKED(IDC_BUTTON_INSERTWOKU, OnBnClickedButton139pic)
	ON_STN_CLICKED(IDC_STATIC_WOKU, OnBnClickedButton139pic)
	ON_WM_MEASUREITEM()
	ON_CBN_KILLFOCUS(IDC_COMBO_DIR, OnCbnKillfocusComboDir)
	ON_CBN_SETFOCUS(IDC_COMBO_DIR, OnCbnSetfocusComboDir)
	ON_MESSAGE(WM_WEBFACE_NOTIFY, OnWebFaceNotify)
	ON_MESSAGE(WM_WEBPIC_NOTIFY,OnWebPicNotify)
	ON_MESSAGE(WM_CLOSEWRITEBLOGDLG,OnCloseDlg)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_WINDOWPOSCHANGING()
	ON_CBN_SELCHANGE(IDC_COMBO_DIR, OnCbnSelchangeComboDir)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_COMMAND(ID_SAVE_ARTICLE, OnMenuSave)
	ON_STN_CLICKED(IDC_BUTTON_WEBP, OnStnClickedButtonWebpic)
	ON_STN_CLICKED(IDC_BUTTON_LOCALFILE, OnStnClickedButtonLocalfile)
	ON_COMMAND(ID_NEW_ARTICLES, OnNewArticles)
	ON_STN_CLICKED(IDC_STATIC_ARTS, OnStnClickedStaticArts)
	ON_STN_CLICKED(ID_LINK, OnStnClickedLink)
	ON_STN_CLICKED(IDC_STATIC_WEBARTS, OnStnClickedStaticWebarts)
	
	ON_COMMAND(ID_NEW_ARTICLE, OnMenuNewArticle)
	ON_COMMAND(ID_EXIT,OnMenuExit)
	ON_COMMAND(ID_HYPERLINK, OnPicLink)
	ON_COMMAND(ID_LOCAL_PIC, OnInsertLocalPic)
	ON_COMMAND(ID_WEB_PIC, OnInsertWebPic)
	ON_COMMAND(ID_WOKU_PIC, OnBnClickedButton139pic)
	ON_COMMAND(ID_POST,OnBnClickedButtonPost)
	ON_STN_CLICKED(IDC_STATIC_MORE, OnOpen)
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_STN_CLICKED(IDC_STATIC_VIEWSPACE, OnStnClickedStaticViewspace)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_OPEN, OnOpen)
	ON_COMMAND(IDC_BUTTON_OPEN, OnOpen)
	ON_BN_CLICKED(IDC_BUTTON_MYARTICLE, OnBnClickedButtonMyarticle)
	ON_BN_CLICKED(IDC_BTN_DEL1, OnBnClickedBtnDel1)
	ON_BN_CLICKED(IDC_BTN_DEL2, OnBnClickedBtnDel2)
	ON_BN_CLICKED(IDC_BTN_DEL3, OnBnClickedBtnDel3)	
	ON_STN_CLICKED(IDC_STATIC_LOCAL1, OnStnClickedStaticLocal1)
	ON_STN_CLICKED(IDC_STATIC_LOCAL2, OnStnClickedStaticLocal2)
	ON_STN_CLICKED(IDC_STATIC_LOCAL3, OnStnClickedStaticLocal3)
	ON_STN_CLICKED(IDC_STATIC_LOCALFILE, OnStnClickedStaticLocalfile)
	ON_BN_CLICKED(IDC_BUTTON_INSERTLOCAL, OnStnClickedStaticLocalfile)
	ON_STN_CLICKED(IDC_STATIC_WEB, OnStnClickedStaticWeb)
	ON_BN_CLICKED(IDC_BUTTON_INSERTWEB, OnStnClickedStaticWeb)
	ON_STN_CLICKED(IDC_STATIC_LINK, OnStnClickedStaticLink)
	ON_BN_CLICKED(IDC_BUTTON_INSERTLINK, OnStnClickedStaticLink)
	ON_COMMAND(ID_CUT, OnCut)
	ON_COMMAND(ID_REDO, OnRedo)
	ON_COMMAND(ID_UNDO, OnUndo)
	ON_COMMAND(ID_SELALL, OnSelall)
	ON_COMMAND(ID_PASTE, OnPaste)
	ON_COMMAND(ID_COPY, OnCopy)
	
	ON_COMMAND(ID_VIEW_MYARTICLES, OnViewMyarticles)
	ON_COMMAND(ID_VIEW_WEBARTICLES, OnViewWebarticles)
END_MESSAGE_MAP()


// CWriteBlogDlg 消息处理程序

BOOL CWriteBlogDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetDlgStyle(ALL);
	EnableHelp(true);

	CString strTitle;
	strTitle.Format("创建文章 - %s",(LPCSTR)g_pUserInfo->GetNickName());
	SetWindowText(strTitle);
	SetTitleIcon(GetResDLLHandle(),IDI_WRITEBLOG);
	
	if ( m_pEditBlog==NULL )
	{
		try
		{
			m_pEditBlog = new CDHtmlEditCtrl();
			CRect rect(0,0,0,0);
			m_pEditBlog->Create("article editor",WS_VISIBLE|WS_CHILD,rect,this,IDC_DHTMLEDIT_BLOG);
		}
		catch(...)
		{
			m_pEditBlog = NULL;
		}
	}

	bool bEditOk = true;

	if ( m_pEditBlog==NULL )
	{
		bEditOk = false;
	}
	else
	if ( m_pEditBlog->m_hWnd==NULL )
	{
		bEditOk = false;
	}

	if ( bEditOk==false )
	{
		MessageBox("由于 Windows Vista 操作系统不再提供DHTML编辑器控件，您需要额外安装该控件。点击确定进行安装。谢谢。","Mini 发表文章",MB_OK|MB_ICONINFORMATION);

		CString strDHtmlSetup = GetModulePath()+"DhtmlEd.msi";
		ShellExecute(NULL,"open",strDHtmlSetup,"","",SW_SHOW);
		
		CXSkinDlg::OnCancel();
		return TRUE;
	}

	m_edtTitle.SetBorderColor(RGB(255,255,255));

	m_wndBlogPanel.Create(CWriteBlogPanel::IDD,this);
	m_wndBlogPanel.ShowWindow(SW_SHOW);
	m_wndBlogPanel.m_editDir.ShowWindow(SW_SHOW);
	m_wndBlogPanel.m_editDir.SetWindowText("正在加载文集...");
	m_wndBlogPanel.m_editDir.SetReadOnly(TRUE);
	m_wndBlogPanel.m_comboDir.ShowWindow(SW_HIDE);
	m_wndBlogPanel.m_staticNickName.SetWindowText(g_pUserInfo->GetNickName());
	m_wndBlogPanel.m_btnHome.SetWindowText(CString("http://")+(LPCSTR)g_pUserInfo->GetUserName()+".woku.com");

	m_FontTitle.CreateFont(
		16,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                   // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"宋体");                 // lpszFacename
	

	m_FontTitle1.CreateFont(
		14,                        // nHeight
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
		"宋体");                 // lpszFacename

	m_edtTitle.SetFont(&m_FontTitle);
	m_edtTitle.SetWindowText("在此输入标题");

	ComboInitViewProp(&m_wndBlogPanel.m_comboView);

	InitBlogToolBar();
	InitMainMenuBar();
	InitToolButtons();
	//InitSetToolBar();
	EnableToolTips();

	InitLocalArticle();

	m_wndBlogPanel.m_comboDir.SetWindowText("正在加载文集...");
	m_wndBlogPanel.m_comboTag.SetWindowText("");
	
	DWORD dwThreadID;
	m_hThreadInit  = CreateThread(NULL,0,
					  (LPTHREAD_START_ROUTINE)InitProc,
					  (LPVOID)this,
					  0,
					  &dwThreadID);
	
	UpdateArticle(FALSE);
	InitArticleParams();

	m_articleInfo2 = m_articleInfo;
	m_edtTitle.LimitText(100);

	SetTimer(0x01,500,NULL);
	SetTimer(0x02,3*60*1000,NULL);

	AdjustUI();

	SetMinMaxInfo(860, 560,0,0);

	//CenterWindow();
	//ShowWindow(SW_MAXIMIZE);

	return  TRUE;
}

bool CompareArticleDate(const TWebArticleInfo *p1, const TWebArticleInfo *p2)
{
	if (p1->dwCreateDate>p2->dwCreateDate)
		return true;
	else
		return false;
}

LRESULT CWriteBlogDlg::OnWebDataNotify(WPARAM wParam,LPARAM lParam)
{
	CWebDataProcess::tagThreadResult * result = (CWebDataProcess::tagThreadResult *)lParam;	
	return 0;
}


void CWriteBlogDlg::OnSize(UINT nType, int cx, int cy)
{
	CXSkinDlg::OnSize(nType, cx, cy);

	AdjustUI();
}

void CWriteBlogDlg::AdjustUI()
{
	if ( m_wndHide.m_hWnd==NULL )
		return ;

	CRect rc;
	GetClientRect(&rc);

	int cx = rc.Width();
	int cy = rc.Height();

	CRect rc1;
	m_edtTitle.GetWindowRect(&rc1);
	rc1.left  =6;
	rc1.right = cx-212;
	rc1.top=111;
	rc1.bottom=rc1.top+24;
	m_edtTitle.MoveWindow(&rc1,FALSE);


	//排版工具栏
	rc1.left   = 2;
	rc1.top    = 80-2;
	rc1.bottom = 105;
	m_toolBlog.MoveWindow(&rc1);

	static int nPreWidth = 0;

	if ( nPreWidth>cx )
	{
		//编辑框
		rc1.top    = 139;
		rc1.left   = 6;
		rc1.right  = cx-212;
		rc1.bottom = cy-6;
		m_pEditBlog->MoveWindow(&rc1);

		rc1.left   = cx-210;
		rc1.right  = cx-5;
		rc1.top    = 110;
		rc1.bottom = cy-5;;

		m_wndBlogPanel.MoveWindow(&rc1);
	}
	else
	{
		rc1.left   = cx-210;
		rc1.right  = cx-5;
		rc1.top    = 110;
		rc1.bottom = cy-5;;

		m_wndBlogPanel.MoveWindow(&rc1);

		//编辑框
		rc1.top    = 139;
		rc1.left   = 6;
		rc1.right  = cx-212;
		rc1.bottom = cy-6;
		m_pEditBlog->MoveWindow(&rc1);

	}

	nPreWidth = cx;


	CRgn rgn;
	GetRepaintRgn(rgn);
	InvalidateRgn(&rgn);
}

void CWriteBlogDlg::OnClose()
{
	UpdateArticle(FALSE);

	if ( IsArticleModified() )
	{	
		SaveArticleToLocal();
	}

	CXSkinDlg::OnCancel();
}

void CWriteBlogDlg::OnNcDestroy()
{
	CWebPicDlg::Destory();
	CXSkinDlg::OnNcDestroy();
}

int CWriteBlogDlg::InitMainMenuBar()
{
	CRect rcClient,rcTitleBar;
	GetClientRect(&rcClient);
	rcTitleBar.left=2;
	rcTitleBar.top=24;
	rcTitleBar.right=rcClient.right;
	rcTitleBar.bottom=44;
	

	m_menuTitleBar.SetDHtmlCtrl(m_pEditBlog);
	m_menuTitleBar.Create(this,rcTitleBar,WS_CHILD|WS_VISIBLE,IDR_MAINMENU);
	m_menuTitleBar.AddTitle(IDR_MENU1,"文件(F)");
	m_menuTitleBar.AddTitle(IDR_MENU2,"编辑(E)");
	m_menuTitleBar.AddTitle(IDR_MENU3,"插入(I)");
	m_menuTitleBar.AddTitle(IDR_MENU5,"查看(V)");

	//m_menuTitleBar.AddTitle(IDR_MENU4,"帮助(H)");


	return 1;
}
int CWriteBlogDlg::InitBlogToolBar()
{
	if (!m_toolBlog.CreateEx(this, TBSTYLE_FLAT/*|TBSTYLE_EX_HIDECLIPPEDBUTTONS*/, WS_CHILD | WS_VISIBLE | CBRS_TOP 
		| CBRS_TOOLTIPS |  CBRS_HIDE_INPLACE) ||
		!m_toolBlog.LoadToolBar(IDR_TOOLBAR_BLOG))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	SIZE sz;
	sz.cx = 16; 
	sz.cy = 16;

	m_toolBlog.InitToolBar(sz,IDB_TOOL_BLOG,IDB_TOOL_BLOG);

	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	CreateFontNameBox();
	CreateFontSizeBox();

	CDC *pDC = GetDC();

	::EnumFontFamilies(pDC->m_hDC, (LPTSTR) NULL, 
		(FONTENUMPROC)EnumFontNameProc, (LPARAM)&(m_comboFontName));

	ReleaseDC(pDC);
	SetTimer(0x01,500,NULL);

	m_toolBlog.AddToolTip(ID_WEBFACE,"插入表情");
	m_toolBlog.AddToolTip(ID_LINK,"插入超级链接");

	return 0;
}
int CWriteBlogDlg::InitSetToolBar()
{
	if (!m_toolSet.CreateEx(this, TBSTYLE_FLAT/*|TBSTYLE_EX_HIDECLIPPEDBUTTONS*/, WS_CHILD | WS_VISIBLE | CBRS_TOP 
		| CBRS_TOOLTIPS |  CBRS_HIDE_INPLACE) ||
		!m_toolSet.LoadToolBar(IDR_TOOL_SET))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	SIZE sz;
	sz.cx = 16; 
	sz.cy = 16;
	m_toolSet.InitToolBar(sz,IDB_TOOL_BLOG,IDB_TOOL_BLOG);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);



}

int CWriteBlogDlg::InitToolButtons()
{	
	m_buttonNew.SetFlatStyle(CFlatButton::FS_OFFICE);
    m_buttonNew.SetBitmap(IDB_ARTS_NEW,IDB_ARTS_NEW);

	m_buttonPost.SetFlatStyle(CFlatButton::FS_OFFICE);
    m_buttonPost.SetBitmap(IDB_ARTS_POST,IDB_ARTS_POST);

	m_buttonOpen.SetFlatStyle(CFlatButton::FS_OFFICE);
	m_buttonOpen.SetBitmap(IDB_ARTS_OPEN,IDB_ARTS_OPEN);

	m_buttonMyArts.SetFlatStyle(CFlatButton::FS_OFFICE);
	m_buttonMyArts.SetBitmap(IDB_MY_ARTICLE,IDB_MY_ARTICLE);

	m_buttonSave.SetFlatStyle(CFlatButton::FS_OFFICE);
	m_buttonSave.SetBitmap(IDB_ARTS_SAVE,IDB_ARTS_SAVE);

	m_btnInsertLocal.SetFlatStyle(CFlatButton::FS_OFFICE);
	m_btnInsertLocal.SetBitmap(IDB_ARTS_INSERT_LOCAL_IMAGE,IDB_ARTS_INSERT_LOCAL_IMAGE);

	m_btnInsertWeb.SetFlatStyle(CFlatButton::FS_OFFICE);
	m_btnInsertWeb.SetBitmap(IDB_ARTS_INSERT_INTERNET_IMAGE,IDB_ARTS_INSERT_INTERNET_IMAGE);

	m_btnInsertWoku.SetFlatStyle(CFlatButton::FS_OFFICE);
	m_btnInsertWoku.SetBitmap(IDB_ARTS_INSERT_WOKU_IMAGE,IDB_ARTS_INSERT_WOKU_IMAGE);

	m_btnInsertLink.SetFlatStyle(CFlatButton::FS_OFFICE);
	m_btnInsertLink.SetBitmap(IDB_ARTS_INSERT_LINK,IDB_ARTS_INSERT_LINK);


	return 0;
}



void CWriteBlogDlg::CreateFontNameBox()
{
	int nIndex=0;
	while (m_toolBlog.GetItemID(nIndex)!=ID_BLG_FONTNAME)
	{
		nIndex++;
	}

	CRect rcFont;

	m_toolBlog.SetButtonInfo(nIndex,ID_BLG_FONTNAME,TBBS_SEPARATOR,170);
	m_toolBlog.GetItemRect(nIndex,&rcFont);
	rcFont.bottom+=200;
	rcFont.top++;
	rcFont.InflateRect(-2,0);

	CRect rcText = rcFont;
	rcText.right = rcText.left+30;
	rcText.top+=4;

//	m_wndText1.Create("字体",WS_CHILD|WS_VISIBLE,rcText,&m_toolBlog);
//	m_wndText1.SetFont( GetFont() );

	rcFont.left =3;

	m_comboFontName.Create(WS_CHILD|WS_VSCROLL|WS_VISIBLE|CBS_DISABLENOSCROLL|CBS_SORT|CBS_DROPDOWNLIST
		|CBS_HASSTRINGS,rcFont,&m_toolBlog,ID_BLG_FONTNAME);

	
	m_comboFontName.SetFont(GetFont());
}


void CWriteBlogDlg::CreateFontSizeBox()
{
	int nIndex=0;
	while (m_toolBlog.GetItemID(nIndex)!=ID_BLG_FONTSIZE)
	{
		nIndex++;
	}

	CRect rcFont;

	m_toolBlog.SetButtonInfo(nIndex,ID_BLG_FONTSIZE,TBBS_SEPARATOR,95);
	m_toolBlog.GetItemRect(nIndex,&rcFont);
	rcFont.bottom+=200;
	rcFont.top++;
	rcFont.InflateRect(-2,0);

	CRect rcText = rcFont;
	rcText.top   +=4;
	rcText.left  +=5;
	rcText.right = rcText.left+30;

//	m_wndText2.Create("大小",WS_CHILD|WS_VISIBLE,rcText,&m_toolBlog);
//	m_wndText2.SetFont( GetFont() );
	rcFont.left +=5;


	m_comboFontSize.Create(WS_CHILD|WS_VSCROLL|WS_VISIBLE|CBS_AUTOHSCROLL|CBS_DROPDOWNLIST
		|CBS_HASSTRINGS,rcFont,&m_toolBlog,ID_BLG_FONTSIZE);
	
	m_comboFontSize.AddString("8");
	m_comboFontSize.AddString("10");
	m_comboFontSize.AddString("12");
	m_comboFontSize.AddString("14");
	m_comboFontSize.AddString("18");
	m_comboFontSize.AddString("24");
	m_comboFontSize.AddString("36");

	m_comboFontSize.SetFont( GetFont() );
}

void CWriteBlogDlg::OnCbnSelchangeComboFontsize()
{
	VARIANT var;
	VariantInit(&var);

	var.vt = VT_I4;
	var.lVal = m_comboFontSize.GetCurSel()+1;  // -2 through +4

	try
	{
		m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_SETFONTSIZE, OLECMDEXECOPT_DONTPROMPTUSER,  &var);
	}
	catch(...)
	{
	}
}

void CWriteBlogDlg::OnCbnSelchangeComboFontname()
{
	CString str;
	m_comboFontName.GetLBText( m_comboFontName.GetCurSel(), str );

	VARIANT var;
	VariantInit(&var);
	var.vt = VT_BSTR;
	var.bstrVal = str.AllocSysString();

	try
	{
		m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_SETFONTNAME, OLECMDEXECOPT_DONTPROMPTUSER,  &var);
	}
	catch(...)
	{
	}
}

void CWriteBlogDlg::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		KillTimer(0x01);
		m_comboFontName.SelectString(0,"宋体");
		m_comboFontSize.SetCurSel(1);

		OnCbnSelchangeComboFontname();
		OnCbnSelchangeComboFontsize();
	}

	if ( nIDEvent==0x02 )
	{
		if ( m_hThreadPub==NULL )//正在发表文章
		{
			UpdateArticle(FALSE);

			CString strError;

			if (CheckLog(strError) )
			{
				SaveArticleToLocal();
			}
		}		
	}

	if ( nIDEvent==0x03 )
	{
		if ( m_bInitBookCombo )
		{
			DWORD dwThreadId;
			m_hThreadArticleBook = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)InitArticleBookProc,
					0,
					0,
					&dwThreadId);

			KillTimer(0x03);
		}
	}

	CXSkinDlg::OnTimer(nIDEvent);
}

BOOL CWriteBlogDlg::PreTranslateMessage(MSG* pMsg)
{
	if ( m_toolBlog.m_hWnd )
		m_toolBlog.SendMessage( WM_IDLEUPDATECMDUI, WPARAM(TRUE), 0);

	if ( pMsg->message==WM_COMMAND )
	{
		switch( pMsg->wParam)
		{
		case IDM_CUT:
			m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_CUT,OLECMDEXECOPT_DODEFAULT,NULL); 
			break;
		case IDM_ALL:
			m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_SELECTALL,OLECMDEXECOPT_DODEFAULT,NULL); 
			break;
		case IDM_COPY:
			m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_COPY,OLECMDEXECOPT_DODEFAULT,NULL); 
			break;
		case IDM_PASTE:
			m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_PASTE,OLECMDEXECOPT_DODEFAULT,NULL);
			break;
		case IDM_UNDO:
			m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_UNDO,OLECMDEXECOPT_DODEFAULT,NULL);
			break;
		case IDM_REDO:
			m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_REDO,OLECMDEXECOPT_DODEFAULT,NULL);
			break;
		case IDM_RELOAD:
			{
				DWORD dwThreadID ;
				m_hContent =  CreateThread(NULL,0,
					(LPTHREAD_START_ROUTINE)LoadArticleContent,
					(LPVOID)m_dwArticleID,
					0,
					&dwThreadID);

				CFormProgress2 *pProgress = new CFormProgress2("正在重载文章内容...",this);
				pProgress->CenterWindow(this);
				pProgress->ShowWindow(SW_SHOW);
				m_pWndProgress = pProgress;
				LockOperation();
			}
			break;
		default:
			break;
		} 
	}


	return CXSkinDlg::PreTranslateMessage(pMsg);
}

void CWriteBlogDlg::OnFontBold()
{
	VARIANT var;
	m_bFontBold = !m_bFontBold;

	VariantInit(&var);
	var.vt = VT_BOOL;
	var.boolVal = m_bFontBold;

	try
	{
		m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_BOLD, OLECMDEXECOPT_DONTPROMPTUSER,  &var);
	}
	catch(...)
	{
	}
}

void CWriteBlogDlg::OnFontItalic()
{
	VARIANT var;
	m_bFontItalic = !m_bFontItalic;

	VariantInit(&var);
	var.vt = VT_BOOL;
	var.boolVal = m_bFontItalic;

	try
	{
		m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_ITALIC, OLECMDEXECOPT_DONTPROMPTUSER,  &var);
	}
	catch(...)
	{
	}
}

void CWriteBlogDlg::InitArticleParams()
{
	if ( m_dwArticleID==0 && m_dwSavedID==0 )//new article
	{
		m_wndBlogPanel.m_comboView.SetCurSel(2);
		m_wndBlogPanel.m_btnCheckTop.SetCheck(0);
		m_wndBlogPanel.m_comboDir.SetWindowText("");
		m_wndBlogPanel.m_comboTag.SetWindowText("");

		OnCbnKillfocusComboTag();
		OnCbnKillfocusComboDir();
	}
}

void CWriteBlogDlg::InitMyTagList()
{
	vector<TTagInfo>     info;
	CWebInterfaceWrapper objWI;

	objWI.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());
	objWI.GetUserArticleTagList(info);

	int n = info.size();
	m_wndBlogPanel.m_comboTag.ResetContent();

	for (int i=0;i<n;i++ )
	{
		m_wndBlogPanel.m_comboTag.InsertString(-1,info[i].szTagName );
	}
}

void CWriteBlogDlg::InitLocalArticle()
{
	m_dwLocal1 = m_dwLocal2 = m_dwLocal3 = 0;

	m_wndBlogPanel.m_btnLocal1.SetWindowText("空文档");
	m_wndBlogPanel.m_btnLocal2.SetWindowText("空文档");
	m_wndBlogPanel.m_btnLocal3.SetWindowText("空文档");

	TSavedArticleList al;
	g_spaceData.LoadMySavedArticleList(al);

	for ( int i=0; i<al.GetCount(); i++ )
	{
		TSavedArticleItem item =al.GetAt( al.FindIndex(i) );

		if ( i==0 )
		{
			m_wndBlogPanel.m_btnLocal1.SetWindowText(item.szTitle);
			m_dwLocal1 = item.dwSavedID;
		}

		if ( i==1 )
		{
			m_wndBlogPanel.m_btnLocal2.SetWindowText(item.szTitle);
			m_dwLocal2 = item.dwSavedID;
		}

		if ( i==2 )
		{
			m_wndBlogPanel.m_btnLocal3.SetWindowText(item.szTitle);
			m_dwLocal3 = item.dwSavedID;
		}

		if ( i>2 )
		{
			break;
		}
	}
}

void CWriteBlogDlg::InitDirList()
{
	CWebInterfaceWrapper objWI;
	m_bookInfo.uID = g_pUserInfo->GetUserID();

	objWI.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());
	objWI.GetUserBookList(g_pUserInfo->GetUserID(),m_bookInfo);

	PostMessage(WM_THREAD_NOTIFY,2,0);
}

void CWriteBlogDlg::OnUpdateBold(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bFontBold);
}

void CWriteBlogDlg::OnUpdateItalic(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bFontItalic);
}

void CWriteBlogDlg::OnUpdateUnderLine(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bFontUnderLine);
}

void CWriteBlogDlg::OnFontUnderLine()
{
	VARIANT var;
	m_bFontUnderLine = !m_bFontUnderLine;

	VariantInit(&var);
	var.vt = VT_BOOL;
	var.boolVal = m_bFontUnderLine;

	try
	{
		m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_UNDERLINE, OLECMDEXECOPT_DONTPROMPTUSER,  &var);
	}
	catch(...)
	{
	}
}

BOOL CWriteBlogDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);

	int cx = rc.Width();
	int cy = rc.Height();

	int nSaveDC = pDC->SaveDC();

	pDC->FillSolidRect(&rc,IM_SPACE_COLOR);

	pDC->Draw3dRect(3,45,cx-6,1,RGB(255,255,255),RGB(200,200,248));
	pDC->Draw3dRect(3,72,cx-6,1,RGB(255,255,255),RGB(200,200,248));
	pDC->Draw3dRect(3,106,cx-6,1,RGB(255,255,255),RGB(200,200,248));
	
	pDC->Draw3dRect(5,110,cx-201-5,26,IM_LINE_COLOR,IM_LINE_COLOR);
	pDC->Draw3dRect(5,138,cx-201-5,cy-5-138,IM_LINE_COLOR,IM_LINE_COLOR);

	pDC->RestoreDC(nSaveDC);

	return FALSE;
}

void CWriteBlogDlg::OnFontColor()
{
	VARIANT var;
	VariantInit(&var);

	CColorDialog  dlg(var.lVal,0,this);

	if ( IDOK==dlg.DoModal() )
	{
		var.vt = VT_I4;
		var.lVal = dlg.GetColor();

		try
		{
			m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_SETFORECOLOR, OLECMDEXECOPT_DONTPROMPTUSER, &var);
		}
		catch(...)
		{
		}
	}
}

void CWriteBlogDlg::OnFormatBullets()
{
	try
	{
		m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_UNORDERLIST,OLECMDEXECOPT_DODEFAULT,NULL); 
	}
	catch(...)
	{
	}
}

void CWriteBlogDlg::OnFormatNumber()
{
	try
	{
		m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_ORDERLIST,OLECMDEXECOPT_DODEFAULT,NULL);
	}
	catch(...)
	{
	}
}

void CWriteBlogDlg::OnFormatAlignCenter()
{
	m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_JUSTIFYCENTER,OLECMDEXECOPT_DODEFAULT,NULL); 
}

void CWriteBlogDlg::OnFormatAlignLeft()
{
	m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_JUSTIFYLEFT,OLECMDEXECOPT_DODEFAULT,NULL); 
}

void CWriteBlogDlg::OnFormatAlignRight()
{
	m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_JUSTIFYRIGHT,OLECMDEXECOPT_DODEFAULT,NULL); 
}

HBRUSH CWriteBlogDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	int nCtrlID = pWnd->GetDlgCtrlID();

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		hbr = g_hBackground;

		if ( nCtrlID==IDC_STATIC_TIP1)
			pDC->SetTextColor(RGB(255,0,0));

		pDC->SetBkMode(TRANSPARENT);
	}

	return hbr;
}

void CWriteBlogDlg::OnBnClickedMyArtilce()
{
	/*BSTR strUserName;
	g_pUserInfo->get_UserName(&strUserName);
	ShowArticle((char *)(LPCSTR)WideStrToStr(strUserName));*/
	OnPicLink();
}

void CWriteBlogDlg::OnBnClickedButtonWebPic()
{
	CTestWebPicDlg dlg(this);
	if ( dlg.DoModal()==IDOK )
	{
		CString strUrl = dlg.GetPictureUrl();

		VARIANT strPic;
		VariantInit(&strPic);
		strPic.vt = VT_BSTR;
		strPic.bstrVal = strUrl.AllocSysString();

		m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_IMAGE,OLECMDEXECOPT_DONTPROMPTUSER,&strPic);
		//	m_pEditBlog->QueryStatus(CDHtmlEditCtrl::DECMD_IMAGE);
		::SysFreeString(strPic.bstrVal);

		FocusEditor();
	}

}

void CWriteBlogDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 780;//最小宽度
	lpMMI->ptMinTrackSize.y = 250;//最小高度

	//CXSkinDlg::OnGetMinMaxInfo(lpMMI);
}

//void CWriteBlogDlg::OnStnClickedStaticEmotion()
//{
	//int  nSelFace = m_wndEmotion.GetSel();
	//
	//if ( nSelFace==-1 )
	//	return ;
	//
	//CString strImg;
	//strImg.Format("%sImage\\WebFace\\face%02d.gif",theApp.GetModulePath(),nSelFace);

	//VARIANT strPic;
	//VariantInit(&strPic);
	//strPic.vt = VT_BSTR;
	//strPic.bstrVal = strImg.AllocSysString();

	//m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_IMAGE,OLECMDEXECOPT_DONTPROMPTUSER,&strPic);
	//m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_LOCK_ELEMENT,OLECMDEXECOPT_DONTPROMPTUSER,NULL);
	
	//	m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_SELECTALL,OLECMDEXECOPT_DONTPROMPTUSER,NULL);
	//SendMessage(WM_KEYDOWN,0x0027,0x014d0001);
	//Sleep(100);
	//SendMessage(WM_KEYUP,0x0027,0x014d0001);
	//CPoint pt;
	//CRect rc;
	//m_pEditBlog->GetWindowRect(&rc);
	//pt.x = rc.right-2;
	//pt.y = rc.bottom - 2;
	//m_pEditBlog->ScreenToClient(&pt);

	//m_pEditBlog->PostMessage(WM_LBUTTONDOWN,1,MAKELPARAM(pt.y,pt.x));
	//m_pEditBlog->SetFocus();

	//SysFreeString(strPic.bstrVal);
//}

void CWriteBlogDlg::OnBnClickedButtonReset()
{
	ResetArticle();
}

void CWriteBlogDlg::ResetArticle()
{
	m_wndBlogPanel.m_comboTag.SetWindowText("");
	m_edtTitle.SetWindowText("");

	m_comboFontName.SelectString(0,"宋体");
	m_comboFontSize.SetCurSel(1);

	OnCbnSelchangeComboFontname();
	OnCbnSelchangeComboFontsize();
	
	m_dwSavedID   = 0;
	m_dwArticleID = 0;

	m_pEditBlog->NewDocument();

	InitArticleParams();

	UpdateArticle(FALSE);

	m_articleType  = FreshArticle;
	m_articleInfo2 = m_articleInfo;
}

//CString CWriteBlogDlg::ProcessWebFace(CString strDocument)
//{
//	CString strServerName = "www.woku.com";//GetServerName(g_ImUserInfo.web_location);
//	CString strModulePath = GetModulePath();
//
//	CString strImg;
//	CString strWebFace;
//
//	for ( int iFace=1; iFace<17; iFace++ )
//	{
//		strImg.Format("%sImage\\WebFace\\face%02d.gif", strModulePath, iFace);
//		strWebFace.Format("http://%s/images/face/face%02d.gif",strServerName,iFace);
//		strDocument.Replace(strImg,strWebFace);
//	}
//
//	strImg.Format("%sImage\\WebFace\\face17.gif", strModulePath);
//	strWebFace.Format("http://%s/images/face/hn2_1.gif",strServerName);
//	strDocument.Replace(strImg,strWebFace);
//
//	strImg.Format("%sImage\\WebFace\\face00.gif", strModulePath);
//	strWebFace.Format("http://%s/images/face/face.gif",strServerName);
//	strDocument.Replace(strImg,strWebFace);
//
//	return strDocument;
//}

UINT CWriteBlogDlg::InitArticleBookProc(LPVOID param)
{
	try
	{
		CWebInterfaceWrapper objWI1;

		objWI1.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());
		objWI1.GetArticleBookList(pInstance->m_articleBook.uArticleId,pInstance->m_articleBook);
		
	}
	catch(...)
	{
	}

	pInstance->PostMessage(WM_THREAD_NOTIFY,4,0);

	return 0;
}

UINT CWriteBlogDlg::InitProc(LPVOID param)
{
	CWriteBlogDlg * pClass = ( CWriteBlogDlg *)param;

	::CoInitialize(0);

	pClass->InitMyTagList();
	pClass->InitDirList();

	::CoUninitialize();
	CloseHandle(pClass->m_hThreadInit);
	pClass->m_hThreadInit = NULL;

	pClass->m_wndBlogPanel.m_comboDir.EnableWindow(TRUE);
	pClass->m_wndBlogPanel.m_comboTag.EnableWindow(TRUE);

	pClass->InitArticleParams();

	return 0;
}

UINT CWriteBlogDlg::PubLogProc(LPVOID param)
{
	CWriteBlogDlg * pClass = ( CWriteBlogDlg *)param;

	int iViewProp = ComboGetViewProp(&pClass->m_wndBlogPanel.m_comboView);
	int iError    = 0;

	CWebInterfaceWrapper objWI;
	objWI.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());
    
	pClass->m_strPubLogErrorInfo="";

	map<CString,CString>::iterator it;

	CString strContent = pClass->m_articleInfo.strContent;

	for ( it=m_mapFile.begin(); it!=m_mapFile.end(); it++ )
	{
		CString strLocalFile = it->first;
		CString strWebFile   = it->second;
		CString strError; 

		strWebFile.Trim();
	
		bool    bUploadPicture = false;

		if (strContent.Find(strLocalFile,0)>-1  )
		{
			if ( strWebFile=="" )
			{
				bUploadPicture = true;
			}
			else
			{
				long nFileSize = GetFileSize(strLocalFile);
				map<CString,long>::iterator itSize = m_mapFileSize.find(strLocalFile);
				if ( itSize!=m_mapFileSize.end() )
				{
					if ( nFileSize!=itSize->second )
					{
						bUploadPicture = true;	
					}
				}
			}

			if ( bUploadPicture )
			{
				if ( objWI.UploadPicture(strLocalFile,"","","",iViewProp,2,strWebFile,strError) )
				{
					CString strFileName = GetWebFileName(strWebFile);

					CString strTempFileName = strFileName;
					strTempFileName.MakeLower();


					if (strTempFileName.Right(3)=="gif" )
					{
						strWebFile = "http://"+strWebFile;
					}
					else
					{
						int i=strFileName.Find(".");
						CString strNewFileName;
						strNewFileName.Format("%s_500.jpg",strFileName.Left(i));
						strWebFile.Replace(strFileName,strNewFileName);

						strWebFile = "http://"+strWebFile;
					}
				
					it->second = strWebFile;
					
					long nFileSize = GetFileSize(strLocalFile);
					map<CString,long>::iterator itSize;

					itSize = m_mapFileSize.find(strLocalFile);

					if ( itSize==m_mapFileSize.end() )
						m_mapFileSize.insert(map<CString,long>::value_type(strLocalFile,nFileSize));
					else
						itSize->second = nFileSize;
				}
				else
				{
					CString &strErrorRef =pClass->m_strPubLogErrorInfo;

					strErrorRef = strErrorRef + "\n";
					strErrorRef = strErrorRef + "  图片 \""+it->first+"\" 上传失败！失败原因:"+strError;
				
					iError = 1;
				}
			}
		}

		strContent.Replace(strLocalFile,strWebFile);
	}

	if ( iError==0 )
	{
		long nReturnArticleID = 0;
		
		if (pClass->m_dwArticleID==0)//发表新文章
		{
			nReturnArticleID = objWI.CreateArticle( 0,//create new file
													0,
													pClass->m_articleInfo.strTitle,
													strContent,
													0,//pClass->m_articleInfo.dwDirID,
													pClass->m_articleInfo.iViewType,
													pClass->m_articleInfo.strTag,
													pClass->m_articleInfo.iTopFlag,
													pClass->m_strPubLogErrorInfo);
		}
		else
		{
			nReturnArticleID = objWI.CreateArticle( 1,//modify article
													pClass->m_dwArticleID,
													pClass->m_articleInfo.strTitle,
													strContent,
													0,//pClass->m_articleInfo.dwDirID,
													pClass->m_articleInfo.iViewType,
													pClass->m_articleInfo.strTag,
													pClass->m_articleInfo.iTopFlag,
													pClass->m_strPubLogErrorInfo);
		}
		
		if(nReturnArticleID!=0)
		{				
			CString strError;
			CString strIds = "";
	
			for ( int i=0; i<pClass->m_wndBlogPanel.m_comboDir.GetCount(); i++ )
			{
				if ( pClass->m_wndBlogPanel.m_comboDir.GetCheck(i) )
				{
					if ( strIds!="" )
					{
						strIds =  strIds + ",";
					}

					strIds = strIds+IntToStr(pClass->m_wndBlogPanel.m_comboDir.GetItemData(i));
				}
			}

			if ( strIds!="" )
				objWI.PutArticleToBook(nReturnArticleID,strIds,strError);

			::PostMessage(pClass->m_hWnd,WM_THREAD_NOTIFY,3,nReturnArticleID);

		}
		else
		{
			iError = 2;
		}

		

	}

	::PostMessage(pClass->m_hWnd,WM_THREAD_NOTIFY,0,iError);

	return 0;	
}

void CWriteBlogDlg::FocusEditor()
{
	CPoint ptSave;
	GetCursorPos(&ptSave); 

	CRect rcEditor;
	m_pEditBlog->GetWindowRect(&rcEditor);

	int nScrollSz = GetSystemMetrics(SM_CXVSCROLL);
	int nScrollHt = GetSystemMetrics(SM_CYVSCROLL);

	CPoint ptClick;
	
	int nBorder=3;

	ptClick.x = rcEditor.right-nScrollSz-nScrollHt-nBorder;
	ptClick.y = rcEditor.bottom-nBorder;

	SetCursorPos(ptClick.x,ptClick.y);

	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0); 
	mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 

	ptClick.x = rcEditor.right-nScrollSz-nBorder;
	ptClick.y = rcEditor.bottom - nScrollHt - nBorder;

	SetCursorPos(ptClick.x,ptClick.y);

	mouse_event(MOUSEEVENTF_LEFTDOWN,0,0,0,0); 
	mouse_event(MOUSEEVENTF_LEFTUP,0,0,0,0); 

	SetCursorPos(ptSave.x,ptSave.y);
}

LRESULT CWriteBlogDlg::OnCancelUpload(WPARAM wParam, LPARAM lParam)
{
	if ( m_hThreadPub )
	{
		try
		{
			TerminateThread(m_hThreadPub,0);
			CloseHandle(m_hThreadPub);
		}
		catch(...)
		{
		}

		if ( m_pWndProgress )
		{
			m_pWndProgress->DestroyWindow();
			delete m_pWndProgress;
		}

		m_pWndProgress = NULL;

		m_hThreadPub=NULL;
	}

	UnLockOperation();

	return 0;
}

void CWriteBlogDlg::OnHelpClick()
{
	OpenHtmlPage("www.feiniu.com");
}

LRESULT CWriteBlogDlg::OnThreadNotify(WPARAM wParam,LPARAM lParam)
{
	UnLockOperation();

	if ( wParam==0 )//thread publog notify
	{
		CloseHandle(m_hThreadPub);
		m_hThreadPub = NULL;

		if ( m_pWndProgress )
		{
			m_pWndProgress->DestroyWindow();
			delete m_pWndProgress;
		}

		m_pWndProgress = NULL;
		InvalidateButton();  

		if(lParam==0)
		{	
						
			if ( m_dwArticleID==0 )
			{
				MessageBox("你的文章已经发表成功了！","发表文章");
			}
			else
			{
				ShowMessage("你的文章已经修改成功了！");
			}
			

			if ( m_articleType==LocalSavedArticle )
			{
				//通知 我的文章管理组件
				if ( g_pMyArticles )
					g_pMyArticles->OnUploadLocalArticle(m_dwSavedID);

				g_spaceData.DeleteSavedArticle(m_dwSavedID);
			}

			if ( g_pMyArticles )
				g_pMyArticles->UpdateWebArticleList(m_dwArticleID);

			OnBnClickedButtonReset();	
		}
		else
		if ( lParam==1 )//日志中图片上传失败
		{
			CString strError;
			strError.Format("对不起！文章中的图片上传失败了。错误信息如下:\n%s\n我们对此给您造成的不便深表歉意，您可以多试几次看看，谢谢!",m_strPubLogErrorInfo);
			ShowMessage(strError);
		}
		else
		if ( lParam==2 )//日志内容发表失败
		{
			CString strError;

			if ( m_dwArticleID==0 )
				strError.Format("对不起！发表文章失败。错误信息如下:\n%s",m_strPubLogErrorInfo);
			else
				strError.Format("对不起！修改文章失败。错误信息如下:\n%s",m_strPubLogErrorInfo);

			ShowMessage(strError);
		}		
	}
	else
	if ( wParam==1 )//thread load article content 
	{
		if ( m_pWndProgress )
		{
			m_pWndProgress->DestroyWindow();
			delete m_pWndProgress;
		}

		m_pWndProgress = NULL;
		
		UpdateArticle(TRUE);
		InvalidateButton();

		m_articleInfo2 = m_articleInfo;
		
		CloseHandle(m_hContent);
		m_hContent = NULL;

		FocusEditor();

		SetTimer(0x03,100,NULL);
	}
	else
	if ( wParam==2 )
	{	
		//pInstance->m_articleBook.vecBook.clear();
		//objWI1.GetArticleBookList(dwArticleID,pInstance->m_articleBook);
		m_wndBlogPanel.m_editDir.ShowWindow(SW_HIDE);
		m_wndBlogPanel.m_comboDir.ShowWindow(SW_SHOW);

		for ( int i=0; i<m_bookInfo.vecBook.size(); i++ )
		{
			int n=m_wndBlogPanel.m_comboDir.AddString(m_bookInfo.vecBook[i].szBookName);
			m_wndBlogPanel.m_comboDir.SetItemData(n,m_bookInfo.vecBook[i].nBookId);
		}

		m_wndBlogPanel.m_comboDir.SetCheck(0,TRUE);
		
		m_bInitBookCombo = true;
	}
	else
	if ( wParam==3 )
	{
		CString strArtitlceUrl;
		strArtitlceUrl.Format("http://%s.woku.com/article/%u.html",(LPCSTR)g_pUserInfo->GetUserName(),lParam);

		MiniOpenUrl(strArtitlceUrl);
	}
	else
	if ( wParam==4 )
	{
		m_wndBlogPanel.m_comboDir.SelectAll(FALSE);

		if ( m_articleBook.uArticleId==m_dwArticleID )
		{
			for ( int i=0; i<m_articleBook.vecBook.size(); i++ )
			{
				for ( int j=0; j<m_wndBlogPanel.m_comboDir.GetCount(); j++ )
				{
					if ( m_articleBook.vecBook[i].nBookId==m_wndBlogPanel.m_comboDir.GetItemData(j) )
					{
						m_wndBlogPanel.m_comboDir.SetCheck(j,TRUE);
					}
				}
			}
		}

		CloseHandle(m_hThreadArticleBook);
		m_hThreadArticleBook = NULL;

	}
	
	return 0;
}

void CWriteBlogDlg::OnBnClickedButtonPost()
{
	CString strTitle;
	m_edtTitle.GetWindowText(strTitle);
	if ("在此输入标题"==strTitle) 
	{
		MessageBox("文章“标题”不能为空","发表文章");
		return ;
	}
	UpdateArticle(FALSE);

	CString strError;

	if ( CheckLog(strError)==false )
	{
		ShowMessage(strError);
		return ;
	}

	CFormProgress3 *pProgress = new CFormProgress3(m_hWnd,this);
	pProgress->ShowWindow(SW_SHOW);
	pProgress->CenterWindow(this);
	pProgress->SetTip("正在发表文章...");

	m_pWndProgress = pProgress;

	DWORD dwThreadID;
	m_hThreadPub  = CreateThread(NULL,0,
		(LPTHREAD_START_ROUTINE)PubLogProc,
		(LPVOID)this,
		0,
		&dwThreadID);

	LockOperation();	
}

void CWriteBlogDlg::OnBnClickedButtonLocalfile()
{
	CImageFileDialog dlg("","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,this);


	if ( IDOK==dlg.DoModal() )
	{
		if (dlg.GetSelFileSize()<2048 )
		{
			CString strFile = dlg.GetPathName();
			VARIANT strPic;
			VariantInit(&strPic);
			strPic.vt = VT_BSTR;
			strPic.bstrVal = strFile.AllocSysString();

			map<CString,CString>::iterator it=m_mapFile.find(strFile);

			if ( it==m_mapFile.end() )
				m_mapFile.insert(map<CString,CString>::value_type(strFile,""));

			m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_IMAGE,OLECMDEXECOPT_DONTPROMPTUSER,&strPic);
		//	m_pEditBlog->QueryStatus(CDHtmlEditCtrl::DECMD_IMAGE);
			::SysFreeString(strPic.bstrVal);

			FocusEditor();

		}
		else
		{
			ShowMessage("对不起，您只能设置大小小于 2M 的图片！",MB_ICONINFORMATION|MB_OK);
		}
	}
}

bool CWriteBlogDlg::CheckLog(CString &sError)
{
	CString str;
	m_edtTitle.GetWindowText(str);
	str.Trim();

	if ( str=="" )
	{
		sError = "文章“标题”不能为空！";
		return false;
	}

	return true;
}

void CWriteBlogDlg::OnCbnSelchangeComboMytag()
{
	CString strTag ;
	m_wndBlogPanel.m_comboTag.GetWindowText(strTag);

	if ( strTag=="正在加载Tag..." )
	{
		m_wndBlogPanel.m_comboTag.SetCurSel(-1);
		m_wndBlogPanel.m_comboTag.SetWindowText("");
	}

	//if ( nSel!=-1 )
	//{
	//	CString s;
	//	m_wndBlogPanel.m_comboTag.GetLBText(nSel,s);

	//	if ( strTag.Find(s)<0 )
	//	{
	//		if ( strTag!="" )
	//			strTag+=",";

	//		strTag+=s;

	//		m_wndBlogPanel.m_comboTag.SetWindowText(strTag);			
	//	}
	//}
}


BOOL CWriteBlogPanel::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_btnDel1.SetBitmap(IDB_ARTS_DEL_H,IDB_ARTS_DEL_N,IDB_ARTS_DEL_D,IDB_ARTS_DEL_D);
	m_btnDel2.SetBitmap(IDB_ARTS_DEL_H,IDB_ARTS_DEL_N,IDB_ARTS_DEL_D,IDB_ARTS_DEL_D);
	m_btnDel3.SetBitmap(IDB_ARTS_DEL_H,IDB_ARTS_DEL_N,IDB_ARTS_DEL_D,IDB_ARTS_DEL_D);
	m_btnLocal1.SetNormalColor(RGB(39,135,161));
	m_btnLocal2.SetNormalColor(RGB(39,135,161));
	m_btnLocal3.SetNormalColor(RGB(39,135,161));
	m_btnLocal1.SetLinkColor(IM_SPACE_COLOR);
	m_btnLocal2.SetLinkColor(IM_SPACE_COLOR);
	m_btnLocal3.SetLinkColor(IM_SPACE_COLOR);
	m_btnLocal1.SetAutoSize(false);
	m_btnLocal2.SetAutoSize(false);
	m_btnLocal3.SetAutoSize(false);

	return TRUE;  
}

HBRUSH CWriteBlogPanel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		hbr = g_hBackground;
		CBrush br;
		br.CreateSolidBrush(RGB(255,255,255));
		hbr=(HBRUSH)br.Detach();
		pDC->SetBkMode(TRANSPARENT);

		pDC->SetTextColor(RGB(1,161,255));

		if(pWnd->GetDlgCtrlID()==IDC_CHECK_TOP||pWnd->GetDlgCtrlID()==IDC_EDIT1)
		{
			pDC->SetTextColor(RGB(0,0,0));			
		}
	}
	
	return hbr;
}

void CWriteBlogPanel::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
}

void CWriteBlogDlg::GetRepaintRgn(CRgn &rgn)
{
	CRect rc;
	GetClientRect(&rc);

	rgn.CreateRectRgn(rc.left,rc.top,rc.right,rc.bottom);

	RemoveRgnOfWnd(rgn,&m_wndBlogPanel);
	RemoveRgnOfWnd(rgn,m_pEditBlog);
	RemoveRgnOfWnd(rgn,&m_edtTitle);
	RemoveRgnOfWnd(rgn,&m_toolBlog);
}

LRESULT CWriteBlogPanel::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( message==WM_COMMAND )
	{
		::PostMessage(GetParent()->m_hWnd,
			message,wParam,lParam);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CWriteBlogDlg::OnToolTipText(UINT, NMHDR* pNMHDR, LRESULT* pResult)
{
     ASSERT(pNMHDR->code == TTN_NEEDTEXTA || pNMHDR->code == TTN_NEEDTEXTW);

     // if there is a top level routing frame then let it handle the message
     if (GetRoutingFrame() != NULL) return FALSE;

     // to be thorough we will need to handle UNICODE versions of the message also !!
     TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
     TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
     TCHAR szFullText[512];
     CString strTipText;
     UINT nID = pNMHDR->idFrom;

     if (pNMHDR->code == TTN_NEEDTEXTA && (pTTTA->uFlags & TTF_IDISHWND) ||
         pNMHDR->code == TTN_NEEDTEXTW && (pTTTW->uFlags & TTF_IDISHWND))
     {
          // idFrom is actually the HWND of the tool 
          nID = ::GetDlgCtrlID((HWND)nID);
     }

     if (nID != 0) // will be zero on a separator
     {
          AfxLoadString(nID, szFullText);
          strTipText=szFullText;

#ifndef _UNICODE
          if (pNMHDR->code == TTN_NEEDTEXTA)
          {
               lstrcpyn(pTTTA->szText, strTipText, sizeof(pTTTA->szText));
          }
          else
          {
               _mbstowcsz(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
          }
#else
          if (pNMHDR->code == TTN_NEEDTEXTA)
          {
               _wcstombsz(pTTTA->szText, strTipText,sizeof(pTTTA->szText));
          }
          else
          {
               lstrcpyn(pTTTW->szText, strTipText, sizeof(pTTTW->szText));
          }
#endif

          *pResult = 0;

          // bring the tooltip window above other popup windows
          ::SetWindowPos(pNMHDR->hwndFrom, HWND_TOP, 0, 0, 0, 0,SWP_NOACTIVATE|
               SWP_NOSIZE|SWP_NOMOVE|SWP_NOOWNERZORDER); return TRUE;
     }
	 return TRUE;
} 

void CWriteBlogDlg::OnCbnSetfocusComboTag()
{
	CString s ;
	m_wndBlogPanel.m_comboTag.GetWindowText(s);

	s.Trim();

	m_wndBlogPanel.m_comboTag.SetTextColor(0);
	m_wndBlogPanel.m_comboTag.Invalidate();

	if ( s==TAG_HINT_STR )
	{
		m_wndBlogPanel.m_comboTag.SetWindowText("");
	}
}

void CWriteBlogDlg::OnCbnKillfocusComboTag()
{
	CString s ;
	m_wndBlogPanel.m_comboTag.GetWindowText(s);

    s.Trim();

	if ( s=="" )
	{
		m_wndBlogPanel.m_comboTag.SetTextColor(0xAAAAAA);
		m_wndBlogPanel.m_comboTag.SetWindowText(TAG_HINT_STR);
	}
}

void CWriteBlogDlg::OnInsertEmotion()
{
	int nIndex=0;
	
	while ( m_toolBlog.GetItemID(nIndex)!=ID_WEBFACE ) nIndex++;

	CRect rect;
	m_toolBlog.GetItemRect(nIndex,&rect);
	m_toolBlog.ClientToScreen(&rect);

	CWebFaceDlg::Show(rect.left,rect.bottom,m_hWnd);
}

void CWriteBlogDlg::OnInsertLink()
{
	m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_HYPERLINK,OLECMDEXECOPT_DONTPROMPTUSER,NULL);
}

void CWriteBlogDlg::OnBnClickedButtonSave()
{
	UpdateArticle(FALSE);

	CString strError;

	if (CheckLog(strError)==false)
	{
		ShowMessage(strError);
		return ;
	}
	CString strText;
	m_edtTitle.GetWindowText(strText);
	if ("在此输入标题"==strText) 
	{
		strText="文章“标题”不能为空！";
		ShowMessage(strText);
		return ;
	}

	SaveArticleToLocal();

	m_articleType = LocalSavedArticle;

	m_articleInfo2 = m_articleInfo;	

	CString str;
	str.Format("文章“%s”保存在你的电脑上了。",m_articleInfo.strTitle);
	MessageBox(str,"保存文章",MB_ICONINFORMATION|MB_OK);
	
	InitLocalArticle();
}

void CWriteBlogDlg::OnCreateDir()
{
	CCreateDirDlg dlg(this);
	dlg.m_pBookList = &m_bookInfo.vecBook;

	if ( dlg.DoModal()==IDOK )
	{
		TBookInfo bi = dlg.m_biCreated;

		m_bookInfo.vecBook.push_back(bi);
		int i=m_wndBlogPanel.m_comboDir.AddString(bi.szBookName);
		m_wndBlogPanel.m_comboDir.SetItemData(i,bi.nBookId);
		m_wndBlogPanel.m_comboDir.SetCheck(i,TRUE);
	}
}

void CWriteBlogDlg::UpdateArticle(BOOL bUpdated/* =TRUE */)
{
	if ( bUpdated )
	{
		m_edtTitle.SetWindowText(m_articleInfo.strTitle);
		m_wndBlogPanel.m_comboTag.SetWindowText(m_articleInfo.strTag);

		//bool b=false;
		//for ( int i=0; i<m_wndBlogPanel.m_comboDir.GetCount(); i++ )
		//{
		//	if  ( m_wndBlogPanel.m_comboDir.GetItemData(i)==m_articleInfo.iDirID )
		//	{
		//		m_wndBlogPanel.m_comboDir.SetCurSel(i);
		//		b=true;
		//		break;
		//	}
		//}
		//if (b==false)
		//{
		//	m_wndBlogPanel.m_comboDir.SetWindowText(m_articleInfo.strDirName);
		//}

		if ( m_articleInfo.iViewType<0 ||  m_articleInfo.iViewType>3 )
			 m_articleInfo.iViewType = 3;

		ComboSelViewProp(&m_wndBlogPanel.m_comboView,m_articleInfo.iViewType);

		if ( m_articleInfo.iTopFlag )
			m_wndBlogPanel.m_btnCheckTop.SetCheck(1);
		else
			m_wndBlogPanel.m_btnCheckTop.SetCheck(0);

		m_pEditBlog->put_DocumentHTML(m_articleInfo.strContent);
        
		OnCbnKillfocusComboDir();
		OnCbnKillfocusComboTag();
	}
	else
	{
		m_edtTitle.GetWindowText(m_articleInfo.strTitle);
		m_wndBlogPanel.m_comboTag.GetWindowText(m_articleInfo.strTag);
		m_articleInfo.strTag.Trim();
		if ( m_articleInfo.strTag==TAG_HINT_STR )
		{
			m_articleInfo.strTag ="";
		}	

		//m_articleInfo.iDirID	 = -1;
		//m_articleInfo.strDirName = "";

		//int nSel = m_wndBlogPanel.m_comboDir.GetCurSel();
		//if ( nSel!=-1 )
		//	m_articleInfo.iDirID = m_wndBlogPanel.m_comboDir.GetItemData(nSel);
		else
		{
			CString strTemp;
            m_wndBlogPanel.m_comboDir.GetWindowText(strTemp);
			strTemp.Trim();


				//int n = m_wndBlogPanel.m_comboDir.GetCount();
				//for ( int i=0; i<n; i++ )
				//{
				//	CString s;
				//	m_wndBlogPanel.m_comboDir.GetLBText(i,s);
				//	s.Trim();
				//	if ( s==strTemp )
				//	{
				//		//m_articleInfo.iDirID = m_wndBlogPanel.m_comboDir.GetItemData(i);
				//		break;
				//	}
				//}

				//if ( m_articleInfo.iDirID==-1 )
				//{
				//	m_articleInfo.strDirName = strTemp;
				//}
			//}
		}

		m_articleInfo.iViewType  = ComboGetViewProp(&m_wndBlogPanel.m_comboView);//查看权限获取

		try
		{
			m_articleInfo.strContent = m_pEditBlog->get_DocumentHTML();
		}
		catch(...)
		{
		}

		if ( m_articleInfo.iViewType==-1 )
			m_articleInfo.iViewType=3;

		if ( m_wndBlogPanel.m_btnCheckTop.GetCheck() )
			m_articleInfo.iTopFlag=1;
		else
			m_articleInfo.iTopFlag=0;
	}
}

void CWriteBlogDlg::EditWebArticle(const TWebArticleInfo &info)
{
	UpdateArticle(FALSE);

	if ( IsArticleModified() )
	{
		SaveArticleToLocal();
	}

	m_dwArticleID            = info.dwArticleID;
	//m_articleInfo.iDirID     = info.dwDirID;
	m_articleInfo.iTopFlag   = info.dwTopFlag;
	m_articleInfo.iViewType  = info.dwViewType;
	m_articleInfo.strContent = "";
	m_articleInfo.strTag     = "";
	m_articleInfo.strTitle   = info.szTitle;
	m_dwSavedID              = 0;

	//		
	if ( m_pWndProgress )
	{
		m_pWndProgress->DestroyWindow();
		delete m_pWndProgress;
		m_pWndProgress = NULL;
	}

	if ( m_hContent )
	{
		try
		{
			TerminateThread(m_hContent,0);
			CloseHandle(m_hContent);
		}
		catch(...)
		{
		}

		m_hContent=0;
	}


	DWORD dwThreadID ;
	m_hContent =  CreateThread(NULL,0,
					(LPTHREAD_START_ROUTINE)LoadArticleContent,
					(LPVOID)info.dwArticleID,
					0,
					&dwThreadID);

	CFormProgress2 *pProgress = new CFormProgress2("正在加载文章内容...",this);
	pProgress->CenterWindow(this);
	pProgress->ShowWindow(SW_SHOW);
	m_pWndProgress = pProgress;

	m_articleType = WebArticle;

	UpdateArticle(TRUE);

	LockOperation();

	SetWindowText("修改已发表的文章");
}

void CWriteBlogDlg::NewArticle()
{
	UpdateArticle(FALSE);

	if ( IsArticleModified() )
	{
		//SaveArticleToLocal();
		ResetArticle();
	}
}

void CWriteBlogDlg::EditLocalSavedArticle(const TLocalSavedArticleInfo &info, DWORD dwSavedID)
{
	if ( m_articleType==LocalSavedArticle && dwSavedID==m_dwSavedID )
	{
		return ;
	}

	UpdateArticle(FALSE);

	if ( IsArticleModified() )
	{
		//SaveArticleToLocal();
	}

	m_dwArticleID            = info.dwArticleID ;
	m_articleInfo.iTopFlag    = info.dwTopFlag;
	m_articleInfo.iViewType   = info.dwViewType;
	m_articleInfo.strContent  = info.strContent;
	m_articleInfo.strTag      = info.strTag;
	m_articleInfo.strTitle   = info.strTitle;
	m_dwSavedID              = dwSavedID;

	m_articleType = LocalSavedArticle;

	UpdateArticle(TRUE);
	
	CStringList slImages;
	GetHtmlImageList(m_articleInfo.strContent,slImages);
	int n = slImages.GetCount();
	for ( int i=0; i<n; i++ )
	{
		CString strFile = slImages.GetAt( slImages.FindIndex(i) );
		if ( strFile.Find("://")==-1 )
		{
			map<CString,CString>::iterator it = m_mapFile.find(strFile);
			if ( it==m_mapFile.end() )
				m_mapFile.insert(map<CString,CString>::value_type(strFile,""));
		}
	}

	m_articleInfo2 = m_articleInfo;

	SetWindowText("编辑未发表的文章");
}

UINT CWriteBlogDlg::LoadArticleContent(LPVOID p)
{
	DWORD dwArticleID = (DWORD)p;

	try
	{
		CWebInterfaceWrapper objWI1;

		objWI1.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());
	
		pInstance->m_articleInfo.strContent = objWI1.GetArticleContent(dwArticleID);
		pInstance->m_articleBook.uArticleId = dwArticleID;
	}
	catch(...)
	{
	}

	pInstance->PostMessage(WM_THREAD_NOTIFY,1,0);

	return 0;
}

bool CWriteBlogDlg::IsArticleModified()
{
	if ( m_articleInfo.strTitle=="" && m_articleInfo2.strTitle=="" )
	{
		return false;
	}
	else
	{
		return ( m_articleInfo.strContent!=m_articleInfo2.strContent ||
			//m_articleInfo.strDirName!=m_articleInfo2.strDirName ||
			m_articleInfo.strTag!=m_articleInfo2.strTag ||
			m_articleInfo.strTitle!=m_articleInfo.strTitle||
			//m_articleInfo.iDirID!=m_articleInfo.iDirID ||
			m_articleInfo.iTopFlag!=m_articleInfo.iTopFlag ||
			m_articleInfo.iViewType!=m_articleInfo.iViewType );
	}
}

void CWriteBlogDlg::SaveArticleToLocal()
{
	TSavedArticleItem item;

	if ( m_dwSavedID==0 )//此文章还未保存到本地
	{
		m_dwSavedID      = time(0);
	}
	else
	{
	}

	item.dwSavedID = m_dwSavedID;

	TLocalSavedArticleInfo article;
	article.dwArticleID   = m_dwArticleID;
	//article.dwDirID       = m_articleInfo.iDirID;
	article.dwTopFlag     = m_articleInfo.iTopFlag;
	article.dwViewType    = m_articleInfo.iViewType;
	article.strContent    = m_articleInfo.strContent;

	CTime tm = CTime::GetCurrentTime();
	article.strCreateDate = tm.Format("%Y-%m-%d %H:%M:%S");
	//article.strDirName    = m_articleInfo.strDirName;
	article.strTag        = m_articleInfo.strTag;
	article.strTitle      = m_articleInfo.strTitle;
	article.strTitle.Trim();

	if ( article.strTitle=="" || article.strTitle=="在此输入标题" )
		article.strTitle.Format("无标题-%s",(LPCSTR)article.strCreateDate);

	if ( g_pMyArticles )
		g_pMyArticles->UpdateLocalArticleList(item.dwSavedID,(LPCSTR)article.strTitle);

	strcpy(item.szTitle,(LPCSTR)article.strTitle);

	g_spaceData.SaveMySavedArticle(article,item.dwSavedID);
	g_spaceData.UpdateSavedArticle(item);

}

void CWriteBlogDlg::OnBnClickedButton139pic()
{
	CRect rect;
	m_btnInsertWoku.GetWindowRect(&rect);	
	CWebPicDlg::Show(rect.left,rect.bottom,m_hWnd);
}

void CWriteBlogDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CXSkinDlg::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
	CMenuEx::MeasureItem(lpMeasureItemStruct);
}

void CWriteBlogDlg::OnCbnKillfocusComboDir()
{
	CString s ;
	m_wndBlogPanel.m_comboDir.GetWindowText(s);

	//s.Trim();

	//if ( s=="" )
	//{
	//	m_wndBlogPanel.m_comboDir.SetTextColor(0xAAAAAA);
	//	m_wndBlogPanel.m_comboDir.SetWindowText(DIR_HINT_STR);
	//}
}

void CWriteBlogDlg::OnCbnSetfocusComboDir()
{
	CString s ;
	m_wndBlogPanel.m_comboDir.GetWindowText(s);

	s.Trim();

	m_wndBlogPanel.m_comboDir.SetTextColor(0);
	m_wndBlogPanel.m_comboDir.Invalidate();

	//if ( s==DIR_HINT_STR )
	//{
	//	m_wndBlogPanel.m_comboDir.SetWindowText("");
	//}
}

LRESULT CWriteBlogDlg::OnWebPicNotify(WPARAM wParam, LPARAM lParam)
{
	CString strUrl  = (char *)wParam;

	VARIANT strPic;
	VariantInit(&strPic);
	strPic.vt = VT_BSTR;
	USES_CONVERSION;
	strPic.bstrVal = strUrl.AllocSysString();

	m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_IMAGE,OLECMDEXECOPT_DONTPROMPTUSER,&strPic);
	//
	m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_SEND_BACKWARD,OLECMDEXECOPT_DONTPROMPTUSER,NULL);
	SysFreeString(strPic.bstrVal);


	m_wndBlogPanel.m_comboTag.SetFocus();
	m_pEditBlog->SetFocus();

	FocusEditor();

	return 0;
}

LRESULT CWriteBlogDlg::OnWebFaceNotify(WPARAM wParam, LPARAM lParam)
{
	CString strUrl  = (char *)wParam;
	
	VARIANT strPic;
	VariantInit(&strPic);
	strPic.vt = VT_BSTR;
	USES_CONVERSION;
	strPic.bstrVal = strUrl.AllocSysString();

	m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_IMAGE,OLECMDEXECOPT_DONTPROMPTUSER,&strPic);
	//
	m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_SEND_BACKWARD,OLECMDEXECOPT_DONTPROMPTUSER,NULL);
	SysFreeString(strPic.bstrVal);


	m_wndBlogPanel.m_comboTag.SetFocus();
	m_pEditBlog->SetFocus();

	FocusEditor();

	return 0;
}

void CWriteBlogDlg::PreSubclassWindow()
{
	CXSkinDlg::PreSubclassWindow();
}

void CWriteBlogDlg::OnWindowPosChanged(WINDOWPOS* lpwndpos)
{
	CXSkinDlg::OnWindowPosChanged(lpwndpos);//SWP_NOSENDCHANGING 
	// TODO: 在此处添加消息处理程序代码
	TRACE("Win pos changed %x\n",lpwndpos->flags);
}

void CWriteBlogDlg::OnWindowPosChanging(WINDOWPOS* lpwndpos)
{
	CXSkinDlg::OnWindowPosChanging(lpwndpos);

	TRACE("Win pos changing %x\n",lpwndpos->flags);
}

void CWriteBlogPanel::OnCbnDropdownComboDir()
{
	m_comboDir.SetTextColor(0);
	m_comboDir.Invalidate();
}

void CWriteBlogPanel::OnCbnDropdownComboTag()
{
	m_comboTag.SetTextColor(0);
	m_comboTag.Invalidate();
}

void CWriteBlogDlg::OnCbnSelchangeComboDir()
{
	CString strDir ;
	m_wndBlogPanel.m_comboDir.GetWindowText(strDir);

	if ( strDir=="正在加载文集..." )
	{
		m_wndBlogPanel.m_comboDir.SetCurSel(-1);
		m_wndBlogPanel.m_comboDir.SetWindowText("");
	}
}

void CWriteBlogPanel::OnStnClickedStaticLink2()
{
	if ( g_pMyArticles )
		g_pMyArticles->Show();
	//CMyZoneDlg::Show();
}


LRESULT CWriteBlogDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CXSkinDlg::WindowProc(message, wParam, lParam);
}


void CWriteBlogDlg::onkeydownDhtmleditBlog()
{
	//if ( KEYDOWN(0x13) )
}

void CWriteBlogDlg::onmouseupDhtmleditBlog()
{
	SHORT sValue=GetAsyncKeyState(VK_RBUTTON);

	if ( sValue )
	{	
		CMenuEx menu, *popmenu;
		CMenuEx menuedit, *pmenuedit;
		CPoint  pt;
		GetCursorPos(&pt);
		
		menuedit.LoadMenu(IDR_MENU2);
		pmenuedit=menuedit.GetSubMenu(0);
		
		menu.LoadMenu(IDR_MENU_TEMPLATE);
		popmenu = menu.GetSubMenu(0);
		popmenu->RemoveMenu(0,MF_BYPOSITION);

		popmenu->AppendMenu(MF_STRING,IDM_CUT,"剪切(&X)");
		popmenu->AppendMenu(MF_STRING,IDM_COPY,"复制(&C)");
		popmenu->AppendMenu(MF_STRING,IDM_PASTE,"粘贴(&P)");
		popmenu->AppendMenu(MF_STRING,IDM_ALL,"全选(&A)");
		popmenu->AppendMenu(MF_SEPARATOR,0,"");
		popmenu->AppendMenu(MF_STRING,IDM_UNDO,"撤销(&U)");
		popmenu->AppendMenu(MF_STRING,IDM_REDO,"恢复(&R)");

		if ( m_articleType==WebArticle )
		{
			popmenu->AppendMenu(MF_SEPARATOR,0,"");
			popmenu->AppendMenu(MF_STRING,IDM_RELOAD,"重新加载文章");
		}

		if ( m_pEditBlog->QueryStatus(CDHtmlEditCtrl::DECMD_CUT)!=CDHtmlEditCtrl::DECMDF_ENABLED )
		{
			popmenu->EnableMenuItem(IDM_CUT,MF_DISABLED);
			pmenuedit->EnableMenuItem(ID_CUT,MF_DISABLED);
		}	
		if ( m_pEditBlog->QueryStatus(CDHtmlEditCtrl::DECMD_COPY)!=CDHtmlEditCtrl::DECMDF_ENABLED )
		{
			popmenu->EnableMenuItem(IDM_COPY,MF_DISABLED);
		}	
		if ( m_pEditBlog->QueryStatus(CDHtmlEditCtrl::DECMD_PASTE)!=CDHtmlEditCtrl::DECMDF_ENABLED )
		{
			popmenu->EnableMenuItem(IDM_PASTE,MF_DISABLED);
		}
		if ( m_pEditBlog->QueryStatus(CDHtmlEditCtrl::DECMD_SELECTALL)!=CDHtmlEditCtrl::DECMDF_ENABLED )
		{
			popmenu->EnableMenuItem(IDM_ALL,MF_DISABLED);
		}
		if ( m_pEditBlog->QueryStatus(CDHtmlEditCtrl::DECMD_UNDO)!=CDHtmlEditCtrl::DECMDF_ENABLED )
		{
			popmenu->EnableMenuItem(IDM_UNDO,MF_DISABLED);
		}
		if ( m_pEditBlog->QueryStatus(CDHtmlEditCtrl::DECMD_REDO)!=CDHtmlEditCtrl::DECMDF_ENABLED )
		{
			popmenu->EnableMenuItem(IDM_REDO,MF_DISABLED);
		}
		popmenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
	}
}


void CWriteBlogDlg::onkeypressDhtmleditBlog()
{

}


void CWriteBlogDlg::onkeyupDhtmleditBlog()
{

}


LRESULT CWriteBlogDlg::OnCloseDlg(WPARAM,LPARAM)
{
	CXSkinDlg::OnCancel();
	return 0;
}
void CWriteBlogDlg::OnMenuExit()
{
	// TODO: 在此添加命令处理程序代码
	CXSkinDlg::OnCancel();
}

void CWriteBlogDlg::onmousedownDhtmleditBlog()
{
	// TODO: 在此处添加消息处理程序代码
	CString str;
	m_edtTitle.GetWindowText(str);
	if(str=="")
	{
		m_edtTitle.SetWindowText("在此输入标题");
	}
}


void CWriteBlogDlg::OnBnClickedButtonTop()
{

}

void CWriteBlogDlg::OnStnClickedButtonWebpic()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButtonWebPic();

}

void CWriteBlogDlg::OnStnClickedButtonLocalfile()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedButtonLocalfile();
	
}

void CWriteBlogDlg::OnStnClickedStaticLink3()
{
	// TODO: 在此添加控件通知处理程序代码

}

//void CWriteBlogDlg::OnCbnDropdownComboTag()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	this->OnCbnDropdownComboTag();
//}

void CWriteBlogDlg::OnNewArticles()
{
	// TODO: 在此添加命令处理程序代码
	OnCreateDir();

}

void CWriteBlogDlg::OnStnClickedStaticArts()
{

}

void CWriteBlogDlg::OnStnClickedLink()
{
	// TODO: 在此添加控件通知处理程序代码
	OnBnClickedMyArtilce();
}

void CWriteBlogDlg::OnStnClickedStaticWebarts()
{
	// TODO: 在此添加控件通知处理程序代码
	

}




void CWriteBlogDlg::OnMenuNewArticle()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedButtonReset();

}

void CWriteBlogDlg::OnMenuSave()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedButtonSave();
}



void CWriteBlogDlg::OnPicLink()
{
	// TODO: 在此添加命令处理程序代码
	OnInsertLink();
}

void CWriteBlogDlg::OnInsertLocalPic()
{
	// TODO: 在此添加命令处理程序代码
		OnBnClickedButtonLocalfile();
}

void CWriteBlogDlg::OnInsertWebPic()
{
	// TODO: 在此添加命令处理程序代码
	OnBnClickedButtonWebPic();
}

void CWriteBlogDlg::OnViewArticle()
{
}

void CWriteBlogDlg::OnViewWebArticles()
{
	OnStnClickedStaticViewspace();
}

 void CWriteBlogDlg::InvalidateButton()
{
	m_buttonNew.Invalidate();
	m_buttonPost.Invalidate();
	m_buttonSave.Invalidate();
	m_buttonOpen.Invalidate();	
	m_buttonMyArts.Invalidate();
	m_btnInsertLocal.Invalidate();
	m_btnInsertWeb.Invalidate();
	m_btnInsertLink.Invalidate();	
	m_btnInsertWoku.Invalidate();
}

void CWriteBlogDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CXSkinDlg::OnLButtonDown(nFlags, point);
}

void CWriteBlogDlg::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认	
	CXSkinDlg::OnMButtonDown(nFlags, point);
}

void CWriteBlogDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CXSkinDlg::OnRButtonUp(nFlags, point);
}

void CWriteBlogDlg::OnStnClickedStaticViewspace()
{	
	CString strName=g_pUserInfo->GetUserName();
    CString url="http://"+strName+".woku.com";
   ShellExecute(NULL,NULL,url,NULL,NULL,SW_HIDE);

}

void CWriteBlogDlg::OnOpen()
{
	COpenArticlesDlg dlg(this);

	if ( dlg.DoModal() )
	{
		if ( dlg.m_nArticleType==0 )//local 
		{
			TLocalSavedArticleInfo info;
			g_spaceData.LoadMySavedArticle(info,dlg.m_dwArticleID);
			this->EditLocalSavedArticle(info,dlg.m_dwArticleID);
		}
		else
		if ( dlg.m_nArticleType==1 )
		{
			TUserWebArticleInfo info(g_pUserInfo->GetUserID());
			g_spaceData.LoadUserWebArticles(g_pUserInfo->GetUserID(),info);
			for( int i=0; i<info.vecArticles.size(); i++ )
			{
				if ( info.vecArticles[i]->dwArticleID==dlg.m_dwArticleID )
				{
                    EditWebArticle(*info.vecArticles[i]);
				}

				//delete info.vecArticles[i];
			}
		}


	}
}

void CWriteBlogDlg::OnBnClickedButtonMyarticle()
{
	g_pMyArticles->Show();
}

void CWriteBlogDlg::OnBnClickedBtnDel1()
{
	if ( m_dwLocal1!=0 )
	{
		CString strTitle;
		m_wndBlogPanel.m_btnLocal1.GetWindowText(strTitle);

		CString strTip;
		strTip.Format("确定要删除草稿 \"%s\" 吗?",strTitle);
		if ( MessageBox(strTip,"发表文章",MB_YESNO|MB_ICONQUESTION)==IDYES )
		{
			g_spaceData.DeleteSavedArticle(m_dwLocal1);
			InitLocalArticle();
		}
	}
}

void CWriteBlogDlg::OnBnClickedBtnDel2()
{
	if ( m_dwLocal2!=0 )
	{
		CString strTitle;
		m_wndBlogPanel.m_btnLocal2.GetWindowText(strTitle);

		CString strTip;
		strTip.Format("确定要删除草稿 \"%s\" 吗?",strTitle);
		if ( MessageBox(strTip,"发表文章",MB_YESNO|MB_ICONQUESTION)==IDYES )
		{
			g_spaceData.DeleteSavedArticle(m_dwLocal2);
			InitLocalArticle();
		}
	}
}

void CWriteBlogDlg::OnBnClickedBtnDel3()
{
	if ( m_dwLocal3!=0 )
	{
		CString strTitle;
		m_wndBlogPanel.m_btnLocal3.GetWindowText(strTitle);

		CString strTip;
		strTip.Format("确定要删除草稿 \"%s\" 吗?",strTitle);
		if ( MessageBox(strTip,"发表文章",MB_YESNO|MB_ICONQUESTION)==IDYES )
		{
			g_spaceData.DeleteSavedArticle(m_dwLocal3);
			InitLocalArticle();
		}
	}
}

void CWriteBlogDlg::OnStnClickedStaticLocal1()
{
	int dwSaveID = m_dwLocal1;

	if ( dwSaveID!=0 )
	{
		TLocalSavedArticleInfo info;
		g_spaceData.LoadMySavedArticle(info,dwSaveID);
		this->EditLocalSavedArticle(info,dwSaveID);
	}
}

void CWriteBlogDlg::OnStnClickedStaticLocal2()
{
	int dwSaveID = m_dwLocal2;

	if ( dwSaveID!=0 )
	{
		TLocalSavedArticleInfo info;
		g_spaceData.LoadMySavedArticle(info,dwSaveID);
		this->EditLocalSavedArticle(info,dwSaveID);
	}
}

void CWriteBlogDlg::OnStnClickedStaticLocal3()
{
	int dwSaveID = m_dwLocal3;

	if ( dwSaveID!=0 )
	{
		TLocalSavedArticleInfo info;
		g_spaceData.LoadMySavedArticle(info,dwSaveID);
		this->EditLocalSavedArticle(info,dwSaveID);
	}
}

void CWriteBlogPanel::OnPaint()
{
	CPaintDC dc(this); 

	CWnd *pWnd ;
	CRect rc;

	pWnd = GetDlgItem(IDC_ICON_LINK);

	if ( pWnd )
	{
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		CResBitmap bmp;
		bmp.LoadBitmap(IDB_ARTS_INSERT_LINK);
		::DrawBitmap(&dc,rc.left,rc.top,&bmp,RGB(0,255,0));
	}

	pWnd = GetDlgItem(IDC_ICON_WOKU);

	if ( pWnd )
	{
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		CResBitmap bmp;
		bmp.LoadBitmap(IDB_ARTS_INSERT_WOKU_IMAGE);
		::DrawBitmap(&dc,rc.left,rc.top,&bmp,RGB(0,255,0));
	}

	pWnd = GetDlgItem(IDC_ICON_WEB);

	if ( pWnd )
	{
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		CResBitmap bmp;
		bmp.LoadBitmap(IDB_ARTS_INSERT_INTERNET_IMAGE);
		::DrawBitmap(&dc,rc.left,rc.top,&bmp,RGB(0,255,0));
	}


	pWnd = GetDlgItem(IDC_ICON_LOCAL);

	if ( pWnd )
	{
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		CResBitmap bmp;
		bmp.LoadBitmap(IDB_ARTS_INSERT_LOCAL_IMAGE);
		::DrawBitmap(&dc,rc.left,rc.top,&bmp,RGB(0,255,0));
	}

	pWnd = GetDlgItem(IDC_STATIC_PIC);

	if ( pWnd )
	{
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		if ( m_pBmpHead )
		{
			DrawBitmap(&dc,rc.left,rc.top,m_pBmpHead,0);
		}

		dc.Draw3dRect(rc.left,rc.top,40,40,IM_LINE_COLOR,IM_LINE_COLOR);
	}
}

void CWriteBlogDlg::OnStnClickedStaticLocalfile()
{
	OnBnClickedButtonLocalfile();
}

void CWriteBlogDlg::OnStnClickedStaticWeb()
{
	OnInsertWebPic();
}

void CWriteBlogDlg::OnStnClickedStaticLink()
{
	OnPicLink();
}

void CWriteBlogDlg::OnCut()
{
	m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_CUT,OLECMDEXECOPT_DODEFAULT,NULL); 
}

void CWriteBlogDlg::OnRedo()
{
	m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_REDO,OLECMDEXECOPT_DODEFAULT,NULL);
}

void CWriteBlogDlg::OnUndo()
{
	m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_UNDO,OLECMDEXECOPT_DODEFAULT,NULL);
}

void CWriteBlogDlg::OnSelall()
{
	m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_SELECTALL,OLECMDEXECOPT_DODEFAULT,NULL); 
}

void CWriteBlogDlg::OnPaste()
{
	m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_PASTE,OLECMDEXECOPT_DODEFAULT,NULL);
}

void CWriteBlogDlg::OnCopy()
{
	m_pEditBlog->ExecCommand(CDHtmlEditCtrl::DECMD_COPY,OLECMDEXECOPT_DODEFAULT,NULL); 
}


void CWriteBlogDlg::OnViewMyarticles()
{
	g_pMyArticles->Show();
}

void CWriteBlogDlg::OnViewWebarticles()
{
	MiniOpenUrl((LPCSTR)g_pWebConfig->GetWebPageUrl(USER_ARTICLE,g_pUserInfo->GetUserName()));
}
