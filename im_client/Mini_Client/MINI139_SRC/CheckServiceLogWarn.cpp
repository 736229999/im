// CExitWarning.cpp : 实现文件
//
#include "stdafx.h"
#include "CheckServiceLogWarn.h"
#include "afxdialogex.h"
#include "MainFrame.h"
#include "CExitWarning.h"
#include "Im\MessageHandleWnd.h"
#include "Im\LocalData.h"

extern map<FRIEND_INFO *,UINT>  g_mapMsgUnRead;////保存未读消息的好友ID和未读条数
extern CMessageHandleWnd *g_pMessageHandleWnd;
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
//
std::tr1::shared_ptr<CheckServiceLogWarn> CheckServiceLogWarn::pInstance = NULL;
std::tr1::shared_ptr<CheckServiceLogWarn> CheckServiceLogWarn::Show(int nTag,HWND wnd)
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
		pInstance->ReSetWindow(nTag);
	}
	else
	{
		pInstance = tr1::shared_ptr<CheckServiceLogWarn>(new CheckServiceLogWarn());
		pInstance->Create(wnd,_T("温馨提示"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->ReSetWindow(nTag);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}
	pInstance->SetParentHwnd(wnd);
	pInstance->SetWindowTop();

	return pInstance;
}

void CheckServiceLogWarn::SetParentHwnd(HWND pDlg)
{
	m_parent = pDlg;
}

DuiLib::UILIB_RESOURCETYPE CheckServiceLogWarn::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LPCTSTR CheckServiceLogWarn::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CheckServiceLogWarn::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	UpdateBk();
	m_parent = NULL;
	m_bClickedBtnOK = FALSE;
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));

	SetIcon(IDR_MAINFRAME);

	m_pBtnClose = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("closebtn"));
	m_pLabelWarn = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelWarn"));//提示信息
	m_pBtnAdmin= static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("okbtn"));   ////确定
	m_pBtnCancel= static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("cancelbtn"));//取消
}

void CheckServiceLogWarn::ReSetWindow(int nTag)
{
	m_wTag = nTag;
	switch(nTag)
	{
	case UNSAVED_SERVICELOG://未保存
		{
			m_pLabelWarn->SetText("您存在未保存的服务小记,请先保存？");
		}
		break;
	case SAVED_SERVICELOG://已保存
		{
            m_pLabelWarn->SetText("服务小记已保存,是否关闭会话窗口?");
		}
		break;
	case UNCOMPLETED_SERVICELOG://保存不完整
		{
			m_pLabelWarn->SetText("您的服务小记未填写完整,请检查填写");
		}
		break;
	case IS_BUYER_TRAN://已经转接的买家
		{
			m_pLabelWarn->SetText("您的客户已被转接,是否关闭当前会话窗口?");
		}
		break;
	default:
		break;
	}
   	SetWindowTop();
}

LRESULT CheckServiceLogWarn::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}
	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CheckServiceLogWarn::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);

	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender->GetName() == "okbtn")
		{
			m_bClickedBtnOK = TRUE;//点击了确定按钮
            Close(IDOK);//关闭窗口
		}
		else if(msg.pSender->GetName() == "cancelbtn")//取消
		{
			m_bClickedBtnOK = FALSE;
			Close(IDCANCEL);//关闭窗口
		}
		else if(msg.pSender->GetName() == "closebtn")//关闭
		{	
			m_bClickedBtnOK = FALSE;
            Close(IDCANCEL);//关闭窗口
		}
	}
}

void CheckServiceLogWarn::UpdateBk()
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


void CheckServiceLogWarn::SetWindowTop()
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