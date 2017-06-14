#include "StdAfx.h"
#include "DuiCounselNone.h"
std::tr1::shared_ptr<CDuiCounselNone> CDuiCounselNone::pInstance = NULL;
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
CDuiCounselNone::CDuiCounselNone(void)
{
}


CDuiCounselNone::~CDuiCounselNone(void)
{
}

LRESULT CDuiCounselNone::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CDuiCounselNone::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();

	SetIcon(IDR_MAINFRAME);

	UpdateBk();
}

void CDuiCounselNone::UpdateBk()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			::UpdateBk(background, pInstance->GetHWND());
		}
	}
}

void CDuiCounselNone::Notify(DuiLib::TNotifyUI& msg)
{
	if(msg.sType == _T("click")) //点击咨询弹出菜单
	{
		if(msg.pSender->GetName() == _T("btnok"))
		{
			ShowWindow(false);
		}
	}
	DuiLib::WindowImplBase::Notify(msg);
}

std::tr1::shared_ptr<CDuiCounselNone> CDuiCounselNone::NewDlg()
{
	// 	if(pInstance2 != NULL && pInstance2->m_hWnd != NULL)
	// 	{
	// 		pInstance2->Close();
	// 		pInstance2 = NULL;
	// 	}
	if(pInstance == NULL || pInstance->m_hWnd == NULL)
	{
		pInstance = tr1::shared_ptr<CDuiCounselNone>(new CDuiCounselNone());
		pInstance->Create(GetDesktopWindow(), "提示", UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
	}
	
	//::BringWindowToTop(pInstance->GetHWND());
	pInstance->ShowWindow(true);
	CRect rcDlg;
	::GetWindowRect(pInstance->GetHWND(), &rcDlg);
	::SetWindowPos(pInstance->GetHWND(), HWND_TOPMOST, rcDlg.left, rcDlg.top, rcDlg.Width(), rcDlg.Height(),SWP_SHOWWINDOW);
	//::SetWindowPos(pInstance->GetHWND(), HWND_NOTOPMOST, rcDlg.left, rcDlg.top, rcDlg.Width(), rcDlg.Height(),SWP_SHOWWINDOW);
	
	return pInstance;
}