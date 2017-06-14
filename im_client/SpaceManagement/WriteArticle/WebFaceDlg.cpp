// im\WebFaceDlg.cpp : 实现文件
//
// im\MagicFaceDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteArticle.h"
#include ".\webfacedlg.h"
#include "webdataprocess.h"
#include "WebPictureProcess.h"

// CWebFaceDlg 对话框
CWebFaceDlg * CWebFaceDlg::pInstance = NULL;

IMPLEMENT_DYNAMIC(CWebFaceDlg, CDialog)
CWebFaceDlg::CWebFaceDlg(CWnd* pParent /*=NULL*/)
: CDialog(CWebFaceDlg::IDD, pParent)
{
	m_iUserId     = 0;

	Create(IDD,pParent);
}

CWebFaceDlg::~CWebFaceDlg()
{
	pInstance=NULL;
}

void CWebFaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_explorer);
}


BEGIN_MESSAGE_MAP(CWebFaceDlg, CDialog)
	ON_WM_CLOSE()
	ON_WM_NCDESTROY()
	ON_WM_KILLFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_ACTIVATE()
	ON_WM_NCPAINT()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CWebFaceDlg 消息处理程序
void CWebFaceDlg::Show(int x, int y, HWND hWndSel)
{
	int screen_cx = GetSystemMetrics(SM_CXSCREEN);
	int screen_cy = GetSystemMetrics(SM_CYSCREEN)-25;

	int cx=250, cy=100;

	if ( (x+cx)>screen_cx ) 
		x = screen_cx-cx;

	if ( (y+cy)>screen_cy )
		y = screen_cy-cy;

	if ( pInstance == NULL )
	{
		pInstance = new CWebFaceDlg(GetDesktopWindow());
		pInstance->m_hFaceSelWnd = hWndSel;
	}
	else
	{
		pInstance->m_hFaceSelWnd = hWndSel;
	}

	pInstance->MoveWindow(x,y,cx,cy);	
	pInstance->ShowWindow(SW_SHOW);
	pInstance->BringWindowToTop();


}

void CWebFaceDlg::Destory()
{
	if ( pInstance )
	{
		pInstance->DestroyWindow();
	}
}

void CWebFaceDlg::OnClose()
{
	ShowWindow(SW_HIDE);
}

void CWebFaceDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	delete this;
}

BOOL CWebFaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	ModifyStyleEx(0,WS_EX_TOOLWINDOW);	
	SetTimer(0x01,10,NULL);
	return TRUE; 
}

void CWebFaceDlg::OnKillFocus(CWnd* pNewWnd)
{
	ShowWindow(SW_HIDE);
}

BOOL CWebFaceDlg::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CWebFaceDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	if ( nState==WA_INACTIVE )
	{
		ShowWindow(SW_HIDE);
	}
}

BEGIN_EVENTSINK_MAP(CWebFaceDlg, CDialog)
	ON_EVENT(CWebFaceDlg, IDC_EXPLORER1, 250, BeforeNavigate2Explorer1, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
END_EVENTSINK_MAP()

void CWebFaceDlg::BeforeNavigate2Explorer1(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	// TODO: 在此处添加消息处理程序代码
	USES_CONVERSION;
	CString strUrl = W2A(URL->bstrVal);

	strUrl.Trim();

	int iPos = strUrl.Find("#");

	if ( iPos>-1 )
	{
		static CString strEmotionUrl="";

		strEmotionUrl = strUrl.Mid(iPos+1,strUrl.GetLength()-iPos-1);
		if ( strEmotionUrl.Find("://")==-1 )
		{
			strEmotionUrl = CString("http://")+(LPCSTR)g_pUserInfo->GetUserName()+".woku.com"+strEmotionUrl;
		}

		::PostMessage(this->m_hFaceSelWnd,WM_WEBFACE_NOTIFY,(WPARAM)(LPCSTR)strEmotionUrl,0);
		*Cancel = TRUE;

		ShowWindow(SW_HIDE);
	}
}

void CWebFaceDlg::OnNcPaint()
{
	CDC *pDC = GetWindowDC();
	CRect rc ;
	GetWindowRect(&rc);
	rc.right-=rc.left;
	rc.bottom-=rc.top;
	rc.left = rc.top = 0;
	pDC->Draw3dRect(rc,IM_LINE_COLOR,IM_LINE_COLOR);
	ReleaseDC(pDC);
}

void CWebFaceDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if ( this->m_explorer.m_hWnd )
	{
		this->m_explorer.MoveWindow(-3,-3,cx+6,cy+6);
	}
}

void CWebFaceDlg::OnTimer(UINT nIDEvent)
{
	CString url = (LPCSTR)g_pWebConfig->GetWebPageUrl(ARTICLE_EMOTION,(char *)(LPCSTR)g_pUserInfo->GetUserName());

	m_explorer.Navigate(url,NULL,NULL,NULL,NULL);

	KillTimer(nIDEvent);

	CDialog::OnTimer(nIDEvent);
}
