// uploadpictureDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "uploadpicture.h"
#include "uploadpictureDlg.h"
#include ".\uploadpicturedlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void GetSectionValue(const CString &strInfo, CString str1,CString str2, CString &strRet)
{
	int		iPos1 , iPos2, iLen;

	iPos1 = strInfo.Find(str1);
	iPos2 = strInfo.Find(str2);

	iLen  = str1.GetLength();

	if ( iPos2>iPos1 && iPos1>-1 )
	{
		strRet = strInfo.Mid(iPos1+iLen,iPos2-iPos1-iLen);
	}
	else
	{
		strRet = "";
	}
}

CuploadpictureDlg::CuploadpictureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CuploadpictureDlg::IDD, pParent)
{
	m_pUpload = NULL;
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CuploadpictureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CuploadpictureDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_ACTIVATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CuploadpictureDlg 消息处理程序

BOOL CuploadpictureDlg::OnInitDialog()
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
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	::ShowWindow(m_hWnd,SW_HIDE);

	m_pUpload.CreateInstance(CLSID_HmgUploadPictureX);

	if(g_bShellExt == TRUE)
	{
		SetWindowText("uploadpicturex.v2");
		m_pUpload->UploadPicShellExt(g_strParam.AllocSysString());
	}
	else
	{
		CString strUserName;
		GetSectionValue(g_strParam,"<username>","</username>",strUserName);

		CString strTitle;
		strTitle.Format("wokuuploadpicture_platform:%s",strUserName);

		SetWindowText(strTitle);

		m_pUpload->ShowUploadDlg(g_strParam.AllocSysString());
	}

	
	m_pUpload.Release();
	m_pUpload = NULL;

	PostQuitMessage(0);
	
	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}


// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CuploadpictureDlg::OnPaint() 
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
HCURSOR CuploadpictureDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CuploadpictureDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	if (nState==WA_ACTIVE)
	{
		if ( m_pUpload )
			m_pUpload->BringToTop();
	}
}

void CuploadpictureDlg::OnClose()
{
	CDialog::OnClose();
}
