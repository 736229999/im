// CExitWarning.cpp : 实现文件
//
#include "stdafx.h"
#include "CExitWarning.h"
#include "afxdialogex.h"
#include "MainFrame.h"

extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);

std::tr1::shared_ptr<CExitWarningDlg> CExitWarningDlg::pInstance = NULL;
std::tr1::shared_ptr<CExitWarningDlg> CExitWarningDlg::Show()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
	}
	else
	{
		pInstance = tr1::shared_ptr<CExitWarningDlg>(new CExitWarningDlg());
		pInstance->Create(GetDesktopWindow(), _T("关闭会话"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}

	return pInstance;
}

DuiLib::UILIB_RESOURCETYPE CExitWarningDlg::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LPCTSTR CExitWarningDlg::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CExitWarningDlg::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));

	SetIcon(IDR_MAINFRAME);
	UpdateBk();
	m_pCheckExitWarning = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("checkExitWarning"));

	WORD nExitWarn = 0;
	g_LocalData.GetExitWarnConfig(nExitWarn);//退出不提示
	if(nExitWarn!=0)
	{
		m_pCheckExitWarning->Selected(true);
	}
}

LRESULT CExitWarningDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CExitWarningDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);

	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender->GetName() == "okbtn")
		{
			OnBnClickedBtnAdmin();
		}
		else if(msg.pSender->GetName() == "cancelbtn")
		{
			Close(IDCANCEL);
		}
	}
}

void CExitWarningDlg::OnBnClickedBtnAdmin()
{
	WORD nExitWarn = 0;
	if(m_pCheckExitWarning->IsSelected())
	{
		nExitWarn = 1;
		g_LocalData.SaveExitWarnConfig(nExitWarn);//退出确认
		WPARAM wParam = 1;
		::PostMessage(CMainFrame::GetInstance()->GetHWND(),WM_EXIT_MAINFRM,wParam,1);//主界面响应
	}
	else
	{
		nExitWarn = 0;
		g_LocalData.SaveExitWarnConfig(nExitWarn);//退出不提示
		WPARAM wParam = 1;
		::PostMessage(CMainFrame::GetInstance()->GetHWND(), WM_EXIT_MAINFRM,wParam,1);//主界面响应
	}
	Close(IDOK);
}

void CExitWarningDlg::UpdateBk()
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