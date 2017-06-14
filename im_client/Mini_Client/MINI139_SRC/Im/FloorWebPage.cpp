// im\FloorWebPage.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\floorwebpage.h"


// CFloorWebPage 对话框

IMPLEMENT_DYNAMIC(CFloorWebPage, CDialog)
CFloorWebPage::CFloorWebPage(CWnd* pParent /*=NULL*/)
	: CDialog(CFloorWebPage::IDD, pParent)
{
	m_bNavigated = false;
}

CFloorWebPage::~CFloorWebPage()
{
}

void CFloorWebPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_wndExplore);
	DDX_Control(pDX, IDC_STATIC_PROGRESS,m_wndProgress);
}


BEGIN_MESSAGE_MAP(CFloorWebPage, CDialog)
	ON_WM_SIZE()
	ON_WM_SHOWWINDOW()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CFloorWebPage 消息处理程序

void CFloorWebPage::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if ( m_bNavigated==false )
	{
		if ( m_wndProgress.m_hWnd )
		{
			CRect rc;
			m_wndProgress.GetWindowRect(&rc);
			ScreenToClient(&rc);
			m_wndProgress.MoveWindow( (cx-rc.Width())/2,rc.top,rc.Width(),rc.Height());
		}
	}
	else
	{
		if ( m_wndExplore.m_hWnd )
		{
			CWnd *pWnd = GetDlgItem(IDC_STATIC_FRAME);
			if ( pWnd && pWnd->m_hWnd )
				pWnd->MoveWindow(0,0,cx,cy);

			m_wndExplore.MoveWindow(-2,-2,cx+4,cy+4);
		}
	}
}

void CFloorWebPage::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
}

void CFloorWebPage::InitWebPage()
{
 	if ( m_hWnd )
	{
		if (m_bNavigated==false )
		{
			if(g_pWebConfig)
			{
				CString strPageUrl=(LPCSTR)g_pWebConfig->GetWebPageUrl(MINI_WEB_CHANEL,L"");
				strPageUrl.Trim();

				//pWebConfig.Release();

				m_wndExplore.Navigate(strPageUrl,NULL,NULL,NULL,NULL);
			}




			if ( m_wndProgress.Load(GetResDLLHandle(),MAKEINTRESOURCE(IDR_GIF_LOGIN),"GIF") )
			{
				m_wndProgress.Draw();
			}
		}
		else
		if ( m_bNavigated )
		{
			m_wndExplore.SetFocus();
		}
	}
}

BOOL CFloorWebPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_STATIC_FRAME)->ShowWindow(SW_HIDE);
	m_wndExplore.MoveWindow(0,0,0,0);
	return TRUE; 
}


BEGIN_EVENTSINK_MAP(CFloorWebPage, CDialog)
	ON_EVENT(CFloorWebPage, IDC_EXPLORER1, 252, NavigateComplete2Explorer1, VTS_DISPATCH VTS_PVARIANT)
END_EVENTSINK_MAP()

void CFloorWebPage::NavigateComplete2Explorer1(LPDISPATCH pDisp, VARIANT* URL)
{
	m_wndProgress.UnLoad();
	m_wndProgress.ShowWindow(SW_HIDE);

	CRect rc;
	GetClientRect(&rc);

	int cx = rc.Width();
	int cy = rc.Height();

	CWnd *pWnd = GetDlgItem(IDC_STATIC_FRAME);
	if ( pWnd && pWnd->m_hWnd )
		pWnd->MoveWindow(0,0,cx,cy);

	pWnd->ShowWindow(SW_SHOW);

	m_wndExplore.SetParent(pWnd);
	m_wndExplore.MoveWindow(-2,-2,cx+4,cy+4);

	m_wndExplore.SetFocus();

	m_bNavigated=true;
}

BOOL CFloorWebPage::OnEraseBkgnd(CDC* pDC)
{
	if ( m_bNavigated==false )
	{
		CRect r;
		GetClientRect(&r);
		pDC->FillSolidRect(&r,0xFFFFFF);
	}

	return FALSE;
}
