// MagicFaceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MagicFace.h"
#include "MagicFaceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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


// CMagicFaceDlg 对话框




CMagicFaceDlg::CMagicFaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMagicFaceDlg::IDD, pParent)
{
	m_pDlgShow = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMagicFaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SHOCKWAVEFLASH1, m_FlashOcx);
}

BEGIN_MESSAGE_MAP(CMagicFaceDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMagicFaceDlg 消息处理程序

BOOL CMagicFaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

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
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW|WS_EX_LAYERED|WS_EX_TRANSPARENT);
	//SetLayeredWindowAttributes(RGB(255,255,255), 0, LWA_COLORKEY);
	
//	ModifyStyleEx(WS_EX_APPWINDOW,WS_EX_TOOLWINDOW);
	SetLayeredWindowAttributes(0, 0, LWA_ALPHA);

	m_FlashOcx.put_BackgroundColor(0xAAAAAA);

	m_FlashOcx.put_Movie(g_strMagicFace);
	m_FlashOcx.Play();

	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	CRect rc;
	GetClientRect(&rc);
	m_FlashOcx.MoveWindow(0,0,rc.Width(),rc.Height());

	CenterWindow();

	SetTimer(0x01,10,NULL);
	SetTimer(0x02,33,NULL);

	m_pDlgShow = new CShowDlg(GetDesktopWindow());
	m_pDlgShow->MoveWindow(0,0,rc.Width(),rc.Height(),FALSE);
	m_pDlgShow->CenterWindow();
	m_pDlgShow->ShowWindow(SW_SHOW);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMagicFaceDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CMagicFaceDlg::OnPaint()
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
//
HCURSOR CMagicFaceDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CMagicFaceDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if ( m_FlashOcx.m_hWnd )
		m_FlashOcx.MoveWindow(0,0,cx,cy);
}
BEGIN_EVENTSINK_MAP(CMagicFaceDlg, CDialog)
	ON_EVENT(CMagicFaceDlg, IDC_SHOCKWAVEFLASH1, 1958, CMagicFaceDlg::OnProgressShockwaveflash1, VTS_I4)
	ON_EVENT(CMagicFaceDlg, IDC_SHOCKWAVEFLASH1, 150, CMagicFaceDlg::FSCommandShockwaveflash1, VTS_BSTR VTS_BSTR)
	ON_EVENT(CMagicFaceDlg, IDC_SHOCKWAVEFLASH1, 197, CMagicFaceDlg::FlashCallShockwaveflash1, VTS_BSTR)
	ON_EVENT(CMagicFaceDlg, IDC_SHOCKWAVEFLASH1, DISPID_READYSTATECHANGE, CMagicFaceDlg::OnReadyStateChangeShockwaveflash1, VTS_I4)
END_EVENTSINK_MAP()

void CMagicFaceDlg::OnProgressShockwaveflash1(long percentDone)
{
}

void CMagicFaceDlg::FSCommandShockwaveflash1(LPCTSTR command, LPCTSTR args)
{
	// TODO: 在此处添加消息处理程序代码
	if ( strcmp(command,"window")==0 )
	{
		CDialog::OnCancel();
	}	
}

void CMagicFaceDlg::FlashCallShockwaveflash1(LPCTSTR request)
{
}

void CMagicFaceDlg::OnReadyStateChangeShockwaveflash1(long newState)
{
}

void CMagicFaceDlg::OnTimer(UINT_PTR nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		static long nPreFrame=-1;
		long    nCurFrame = m_FlashOcx.get_FrameNum();

		if ( nPreFrame!=-1 )
		{
			if ( nPreFrame > nCurFrame )
			{
				KillTimer(0x01);
				CDialog::OnCancel();
			}		
		}

		nPreFrame = nCurFrame;
	}

	if ( nIDEvent==0x02 )
	{
		if ( m_pDlgShow )
			m_pDlgShow->ShowFrame(this);
	}

	CDialog::OnTimer(nIDEvent);
}

void CMagicFaceDlg::OnDestroy()
{
	if ( m_pDlgShow )
	{
		m_pDlgShow->DestroyWindow();
		delete m_pDlgShow;
		m_pDlgShow = NULL;
	}

	CDialog::OnDestroy();
}
