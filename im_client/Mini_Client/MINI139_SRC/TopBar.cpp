#include "stdafx.h"
#include "TopBar.h"

static int s_nID = 0;
std::map<int, std::tr1::shared_ptr<CTopBarDlg>> CTopBarDlg::g_mapDlg;

void CTopBarDlg::Show(HWND hWnd, const char *szText, POINT pt, int nWidth)
{
	std::tr1::shared_ptr<CTopBarDlg> pInstance = std::tr1::shared_ptr<CTopBarDlg>(new CTopBarDlg());
	g_mapDlg[pInstance->m_nID] = pInstance;
	pInstance->Create(hWnd, _T(""), WS_POPUP , WS_EX_TOOLWINDOW | WS_EX_TOPMOST, pt.x, pt.y, nWidth, 24, NULL);
	HWND hWndParent = pInstance->m_hWnd;
	while( ::GetParent(hWndParent) != NULL ) hWndParent = ::GetParent(hWndParent);

	// 加载数据
	DuiLib::CLabelUI *pText = static_cast<DuiLib::CLabelUI*>(pInstance->m_PaintManager.FindControl("lbText"));
	pText->SetText(szText);

	::ShowWindow(pInstance->m_hWnd, SW_SHOW);
	::SendMessage(hWndParent, WM_NCACTIVATE, TRUE, 0L);
}

CTopBarDlg::CTopBarDlg()
{
	m_nID = s_nID++;
}

CTopBarDlg::~CTopBarDlg()
{
}

DuiLib::UILIB_RESOURCETYPE CTopBarDlg::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LRESULT CTopBarDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(WM_TIMER == uMsg)
	{
		if(wParam == TIMER_AUTO_CLOSE)
		{
			KillTimer(GetHWND(), wParam);
			Close();
		}
	}
	else if(uMsg == WM_KILLFOCUS)
	{
		Close();
	}
	return DuiLib::WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

LPCTSTR CTopBarDlg::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CTopBarDlg::InitWindow()
{
	DuiLib::WindowImplBase::InitWindow();

	// 5s后自动关闭窗口
	SetTimer(GetHWND(), TIMER_AUTO_CLOSE, 5000, NULL);
}

void CTopBarDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if(msg.sType == "click")
	{
		if(msg.pSender->GetName() == "btnClose")
		{	// 关闭窗口
			Close();
		}
	}
}

void CTopBarDlg::OnFinalMessage( HWND hWnd )
{
	g_mapDlg.erase(m_nID);
}