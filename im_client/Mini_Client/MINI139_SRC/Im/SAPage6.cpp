// im\SAPage6.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "SAPage6.h"
#include ".\sapage6.h"
#include "webprocess.h"

// CSAPage6 对话框

IMPLEMENT_DYNAMIC(CSAPage6, CSABasePage)
CSAPage6::CSAPage6(CWnd* pParent /*=NULL*/)
	: CSABasePage(CSAPage6::IDD, pParent)
{

	try
	{
		m_pWndAvatar = new CShockwaveflash();
	}
	catch(...)
	{
		m_pWndAvatar = NULL;
	}

	Create(CSAPage6::IDD,pParent);
}

CSAPage6::~CSAPage6()
{
	if ( m_pWndAvatar )
	{
		m_pWndAvatar->DestroyWindow();
		delete m_pWndAvatar;
		m_pWndAvatar = NULL;
	}
}

void CSAPage6::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BUTTON_REFRESH,m_btnRefresh);
}


BEGIN_MESSAGE_MAP(CSAPage6, CSABasePage)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnBnClickedButtonRefresh)
	ON_WM_PAINT()
	ON_WM_SHOWWINDOW()
	ON_MESSAGE(WM_WEBFLASH_NOTIFY,OnWebFlashNotify)
END_MESSAGE_MAP()


// CSAPage6 消息处理程序

BOOL CSAPage6::OnInitDialog()
{
	CSABasePage::OnInitDialog();

	CRect rect;
	rect.SetRect(5,2,150,252);

	if ( m_pWndAvatar->Create("ShockwaveFlash.Object",WS_VISIBLE|WS_CHILD|WS_CLIPCHILDREN,rect,this,1203) )
	{
		m_pWndAvatar->ShowWindow(SW_SHOW);
		m_pWndAvatar->put_Movie(g_webFlashProcess.GetFlashFile(&g_ImUserInfo));
		m_pWndAvatar->Play();
	}
	else
	{
		delete m_pWndAvatar;
		m_pWndAvatar = NULL;
	}

	return TRUE; 
}

void CSAPage6::ResetModifyFlag()
{
	CSABasePage::ResetModifyFlag();
}

void CSAPage6::OnSize(UINT nType, int cx, int cy)
{
	CSABasePage::OnSize(nType, cx, cy);
}

void CSAPage6::OnBnClickedButtonRefresh()
{
	if ( m_pWndAvatar )
	{
		m_pWndAvatar->Stop();
		m_pWndAvatar->put_Movie(g_webFlashProcess.GetFlashFile(&g_ImUserInfo));
		m_pWndAvatar->Play();
	}
	//g_webFlashProcess.GetUserFlashShow(&g_ImUserInfo,this);
}

void CSAPage6::OnPaint()
{
	CPaintDC dc(this);

	CRect rect;
	rect.SetRect(5,2,150,252);

	if ( m_pWndAvatar==NULL )
	{
		dc.SetBkMode(TRANSPARENT);
		dc.DrawText("由于你没有安装Flash控件，无法显示闪秀",rect,DT_END_ELLIPSIS|DT_TOP|DT_LEFT|DT_WORDBREAK);
	}
	rect.InflateRect(2,2);
	dc.Draw3dRect(rect,IM_LINE_COLOR,IM_LINE_COLOR);	
}

void CSAPage6::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CSABasePage::OnShowWindow(bShow, nStatus);
}

LRESULT CSAPage6::OnWebFlashNotify(WPARAM wParam, LPARAM lParam)
{
	return 0;
}