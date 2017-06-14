// SpaceManagementDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SpaceManagement.h"
#include "SpaceManagementDlg.h"
#include "WebInterfaceWrapper.h"
#include ".\spacemanagementdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CString strFile;

IUserInfoX * m_pUserInfo;


UINT Thread1(LPVOID p)
{
	IWebInterfaceX *pX = (IWebInterfaceX *)p;
	BSTR strPicturUrl=L"";
	BSTR strError=L"";
	long nFileSize;
	pX->UploadPicture((LPCSTR)strFile,&nFileSize,&strPicturUrl,&strError);
//	pX->UploadPicture((LPCSTR)strFile,0,0,"AA",&strPicturUrl,&strError);
	return 0;
}
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框
void InitUserInfoX(IUserInfoX *pUserInfo)
{
	pUserInfo->PutAge(10);
	pUserInfo->PutEmail("xuexf@staff.woku.com");
	pUserInfo->PutFaceID(1);
	pUserInfo->PutSex(0);
	pUserInfo->PutGoldMoney(122);
	pUserInfo->PutMessageCount(2);
	pUserInfo->PutNickName("迷你丸子");
	pUserInfo->PutUserName("miniking");
	pUserInfo->PutOnlineTime(22222);
	pUserInfo->PutPassword("222222");
	pUserInfo->PutUserID(610789);
}

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CSpaceManagementDlg 对话框



CSpaceManagementDlg::CSpaceManagementDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpaceManagementDlg::IDD, pParent)
{
	CoCreateInstance(CLSID_CUserInfoX,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IUserInfoX,
			(void **)&m_pUserInfo);//CreateInstance(CLSID_CUserInfoX);

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSpaceManagementDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

UINT CSpaceManagementDlg::ThreadProc(LPVOID lpParam)
{

	CSpaceManagementDlg *pDlg = (CSpaceManagementDlg *)lpParam;
	long nID=m_pUserInfo->GetUserID();

	return 0;
}

BEGIN_MESSAGE_MAP(CSpaceManagementDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, OnBnClickedButton4)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON5, OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON12, OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON16, OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON15, OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON17, OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON18, OnBnClickedButton18)
END_MESSAGE_MAP()


// CSpaceManagementDlg 消息处理程序

BOOL CSpaceManagementDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将\“关于...\”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);	

	HRESULT hr;

		m_pMiniPictureBroadCast.CreateInstance(CLSID_IMiniPictureBroadCastX);
	try
	{
		m_pMyArticles.CreateInstance(CLSID_MyArticlesX);
		m_pWriteArticle.CreateInstance(CLSID_WriteArticleX);
	
		hr = m_pMyPictures.CreateInstance(CLSID_MyPicturesX);
		m_pMiniMusicPlayer.CreateInstance(CLSID_MiniMusicPlayer);


		m_pWebInterface.CreateInstance(CLSID_WebInterfaceX);
		InitUserInfoX((IUserInfoX *)m_pWriteArticle->GetUserInfo());
		InitUserInfoX((IUserInfoX *)m_pMyArticles->GetUserInfo());
		InitUserInfoX((IUserInfoX *)m_pMyPictures->GetUserInfo());
		//InitUserInfoX((IUserInfoX *)m_pWebInterface->GetUserInfo());
		m_pMyArticles->SelectWriteArticleX((long)(IWriteArticleX *)m_pWriteArticle);
		//m_pWriteArticle->SelectMyArticlesX((long)(IMyArticlesX *)m_pMyArticles);
		m_pHeadPhoto.CreateInstance(CLSID_HeadPhotoX);	
	}
	catch(_com_error e)
	{
		MessageBox(e.ErrorMessage());
	}

	// 设置小图标

	// TODO: 在此添加额外的初始化代码
	//m_pUserInfo->PutUserID(100);

	AfxBeginThread((AFX_THREADPROC)ThreadProc,this);
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}

void CSpaceManagementDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
void CSpaceManagementDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标显示。
HCURSOR CSpaceManagementDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSpaceManagementDlg::OnBnClickedButton1()
{
	m_pMyArticles->Show();
}

void CSpaceManagementDlg::OnBnClickedButton2()
{
	//IWebConfigXPtr p;
	//p.CreateInstance(CLSID_WebConfigX);
	//AspxUrlType type=atMagicFace;
	//CString str = p->GetAspxUrl(type,"");
	//MessageBox(str);
}

void CSpaceManagementDlg::OnBnClickedButton3()
{
	//IWebConfigXPtr p;
	//p.CreateInstance(CLSID_WebConfigX);
	//AspxUrlType type=atMagicFace;
	//p->GetAspxUrl(type,"");
}

void CSpaceManagementDlg::OnBnClickedButton4()
{
	IWriteArticleX *pp;
	pp=(IWriteArticleX *)m_pWriteArticle;
	pp->Show();
}

void CSpaceManagementDlg::OnDestroy()
{
	m_pWriteArticle->Close();
	m_pWriteArticle.Release();
	m_pMyArticles->Close();
	m_pMyArticles.Release();
	m_pHeadPhoto->Close();
	m_pHeadPhoto.Release();
	m_pMyPictures->Close();
	m_pMyPictures.Release();
	m_pWebInterface.Release();
	m_pMiniPictureBroadCast->Close();
	m_pMiniPictureBroadCast.Release();
	m_pMiniMusicPlayer.Release();

	CDialog::OnDestroy();
}

void CSpaceManagementDlg::OnBnClickedButton5()
{
	InitUserInfoX((IUserInfoX *)m_pHeadPhoto->GetUserInfo());
	m_pHeadPhoto->Show();
}

void CSpaceManagementDlg::OnBnClickedButton6()
{
	m_pMyPictures->Show();	
}

void CSpaceManagementDlg::OnBnClickedButton7()
{
}

void CSpaceManagementDlg::OnBnClickedButton8()
{
}

void CSpaceManagementDlg::OnBnClickedButton9()
{
	
}

void CSpaceManagementDlg::OnBnClickedButton10()
{

}

void CSpaceManagementDlg::OnBnClickedButton11()
{
	CFileDialog f(TRUE);

	CString strPicturUrl;
	CString strError;


	if ( f.DoModal()==IDOK )
	{
		strFile = f.GetPathName();
		CWebInterfaceWrapper obj;
		obj.InitInterface(610789,"miniking","迷你丸子");
		obj.UploadPicture(strFile,"","","",3,2,strPicturUrl,strError);
		//AfxBeginThread((AFX_THREADPROC)Thread1,(LPVOID)(IWebInterfaceX *)m_pWebInterface);
	}
}

void CSpaceManagementDlg::OnBnClickedButton12()
{
	CWebInterfaceWrapper objW;
	objW.InitInterface(610789,"miniking","迷你丸子");
	objW.MiniOnline();
}

void CSpaceManagementDlg::OnBnClickedButton13()
{
	long nSize;
	CWebInterfaceWrapper objW;
	objW.InitInterface(610789,"miniking","迷你丸子");
	nSize = objW.GetSpaceSize();
}

void CSpaceManagementDlg::OnBnClickedButton14()
{
	long nSize;
	CWebInterfaceWrapper objW;
	objW.InitInterface(610789,"miniking","迷你丸子");
	objW.SyncPicture();
}

void CSpaceManagementDlg::OnBnClickedButton16()
{
	CFileDialog f(TRUE);

	CString strPicturUrl;
	CString strError;


	if ( f.DoModal()==IDOK )
	{
		strFile = f.GetPathName();
		CWebInterfaceWrapper obj;
		obj.InitInterface(610789,"miniking","迷你丸子");
		obj.UploadFace(strFile,strPicturUrl,strError);
		obj.SyncPicture();
		//AfxBeginThread((AFX_THREADPROC)Thread1,(LPVOID)(IWebInterfaceX *)m_pWebInterface);
	}
}

void CSpaceManagementDlg::OnBnClickedButton15()
{
	
/*	
	
	m_pMiniPictureBroadCast->AddUser(168265);
	m_pMiniPictureBroadCast->AddUser(7192016);	
	m_pMiniPictureBroadCast->AddUser(2353437);
	m_pMiniPictureBroadCast->AddUser(610789);
	
	
	m_pMiniPictureBroadCast->SetWindowTop(TRUE);
	m_pMiniPictureBroadCast->SetInterval(3);*/	
	try
	{
		m_pMiniPictureBroadCast->SetMessageWndHandle(0);
	}
	catch( _com_error e)
	{
		MessageBox(e.ErrorMessage());
	}
	//m_pMiniPictureBroadCast->Show(7192016,"wangtaoblog","wang_tao","D:\\");
	

	
}

void CSpaceManagementDlg::OnBnClickedButton17()
{
	vector<TWebAudioInfo *> vecAudio;
	CWebInterfaceWrapper obj;
	obj.InitInterface(610789,"miniking","迷你丸子");
	//obj.InitInterface(446767,"xuexiaofeng","薛小峰＠ＷＯＫＵ");
	CString strServerDate;
	obj.GetUserAudioList(610789,"","",vecAudio,strServerDate);
	
}

void CSpaceManagementDlg::OnBnClickedButton18()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_pMiniMusicPlayer->AddFriend(446767,"xuexiaofeng","薛小峰＠ＷＯＫＵ",0);
	//m_pMiniMusicPlayer->AddFriend(168265,"hamenny","阿凉",0);
	//m_pMiniMusicPlayer->AddFriend(1642598,"ddmor","这个家伙不够狠",0);
	m_pMiniMusicPlayer->Show();
	/*for (..)
		m_pMiniMusicPlayer->AddFriend(..->)*/
}
