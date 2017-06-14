#include "StdAfx.h"
#include "P2PSendMsgDlgCloseHint.h"
#include "P2PSendMsgDlg.h"
#include "MessageHandleWnd.h"
extern CMessageHandleWnd *g_pMessageHandleWnd;
std::tr1::shared_ptr<CP2PSendMsgDlgCloseHint> CP2PSendMsgDlgCloseHint::pInstance = NULL;
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
CP2PSendMsgDlgCloseHint::CP2PSendMsgDlgCloseHint(void)
{
	//m_parent = NULL;
}


CP2PSendMsgDlgCloseHint::~CP2PSendMsgDlgCloseHint(void)
{
}

LRESULT CP2PSendMsgDlgCloseHint::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CP2PSendMsgDlgCloseHint::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();

	SetIcon(IDR_MAINFRAME);

	UpdateBk();
}

void CP2PSendMsgDlgCloseHint::UpdateBk()
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

void CP2PSendMsgDlgCloseHint::Notify(DuiLib::TNotifyUI& msg)
{
	if(msg.sType == _T("click")) //点击咨询弹出菜单
	{
		if(msg.pSender->GetName() == _T("closebtn")) 
		{
			CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
		}
		else if(msg.pSender->GetName() == _T("btncloseall"))
		{
			g_pMessageHandleWnd->CloseAllP2PSendMsgDlg();
			CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
			//Close();
		}
		else if(msg.pSender->GetName() == _T("btncloseone"))
		{
			::SendMessage(m_parent, WM_CLOSE_ONE, NULL, NULL);
			CMainFrame::GetInstance()->ReleaseDlg();////打开对话框
			//Close();
		}
	}
	DuiLib::WindowImplBase::Notify(msg);
}

std::tr1::shared_ptr<CP2PSendMsgDlgCloseHint> CP2PSendMsgDlgCloseHint::NewDlg(HWND wnd)
{
	// 	if(pInstance2 != NULL && pInstance2->m_hWnd != NULL)
	// 	{
	// 		pInstance2->Close();
	// 		pInstance2 = NULL;
	// 	}
	if(pInstance == NULL || pInstance->m_hWnd == NULL)
	{
		pInstance = tr1::shared_ptr<CP2PSendMsgDlgCloseHint>(new CP2PSendMsgDlgCloseHint());
		pInstance->Create(wnd, "关闭会话", UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
	}
	else
	{
		pInstance->Close(IDCANCEL);
		pInstance = tr1::shared_ptr<CP2PSendMsgDlgCloseHint>(new CP2PSendMsgDlgCloseHint());
		pInstance->Create(wnd, "关闭会话", UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
		//::BringWindowToTop(*pInstance);
	}
	
	pInstance->SetWindowTop();
	pInstance->SetParentHwnd(wnd);
	return pInstance;
}

void CP2PSendMsgDlgCloseHint::SetParentHwnd(HWND pDlg)
{
	m_parent = pDlg;
}

void CP2PSendMsgDlgCloseHint::SetWindowTop()
{
	CRect rcDlgs;
	::GetWindowRect(pInstance->GetHWND(),rcDlgs);   //得到对话框的Rect 对话框的大小
	int cx = GetSystemMetrics(SM_CXSCREEN);  //获得屏幕的分辨率
	int cy = GetSystemMetrics(SM_CYSCREEN); 
	SetWindowPos(pInstance->GetHWND(),CWnd::wndTopMost,//窗口位置
		cx/2-rcDlgs.Width()/2,
		cy/2-rcDlgs.Height()/2,//位置
		rcDlgs.Width(),
		rcDlgs.Height(),//大小
		SWP_SHOWWINDOW);//SWP_SHOWWINDOW：显示窗口。
}
