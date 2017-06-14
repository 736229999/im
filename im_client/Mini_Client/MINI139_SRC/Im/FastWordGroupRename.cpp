#include "StdAfx.h"
#include "FastWordGroupRename.h"
#include "LocalData.h"
using namespace DuiLib;
std::tr1::shared_ptr<CFastWordGroupRename> CFastWordGroupRename::pInstance = NULL;
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
CFastWordGroupRename::CFastWordGroupRename(void)
{
	//m_parent = NULL;
}


CFastWordGroupRename::~CFastWordGroupRename(void)
{
}

LRESULT CFastWordGroupRename::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CFastWordGroupRename::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	m_parent = NULL;

	CEditUI* pEditGroupName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editFastWordGroupName")));
	pEditGroupName->SetMaxChar(10);
	SetIcon(IDR_MAINFRAME);
	UpdateBk();
}

void CFastWordGroupRename::UpdateBk()
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

void CFastWordGroupRename::Notify(DuiLib::TNotifyUI& msg)
{
	if(msg.sType == _T("click")) //点击咨询弹出菜单
	{
		if(msg.pSender->GetName() == _T("closebtn")) 
		{

		}
		else if(msg.pSender->GetName() == _T("btncancel"))
		{
			Close();
		}
		else if(msg.pSender->GetName() == _T("btnok"))
		{
			CEditUI* pEditGroupName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editFastWordGroupName")));
			CString strChangedGroupName = pEditGroupName->GetText();

			if(strChangedGroupName == "")
			{
				CMessageBox::Model(GetHWND(), "组名不能为空！", "提示", MB_OK);
				return;
			}

			vector<FastWordGroupInfo> vecFastWordGroupInfo = g_LocalData.GetFastWordInfo();
			vector<FastWordGroupInfo>::iterator it = std::find(vecFastWordGroupInfo.begin(), vecFastWordGroupInfo.end(), strChangedGroupName);
			if(it != vecFastWordGroupInfo.end())
			{
				CMessageBox::Model(GetHWND(), "检测到重复的组名！", "提示", MB_OK);
				return;
			}

			char* pChangedGroupName = (LPSTR)(LPCSTR)strChangedGroupName;
			CString strGroupName = m_strGroupName;
			char* pGroupName = (LPSTR)(LPCSTR)strGroupName;
			::SendMessage(m_parent, WM_FASTWORD_GROUP_RENAME, (DWORD)pChangedGroupName, (DWORD)pGroupName);
			Close();
		}
	}
	DuiLib::WindowImplBase::Notify(msg);
}

std::tr1::shared_ptr<CFastWordGroupRename> CFastWordGroupRename::NewDlg(HWND wnd, CString strGroupName)
{
	// 	if(pInstance2 != NULL && pInstance2->m_hWnd != NULL)
	// 	{
	// 		pInstance2->Close();
	// 		pInstance2 = NULL;
	// 	}
	if(pInstance == NULL || pInstance->m_hWnd == NULL)
	{
		pInstance = tr1::shared_ptr<CFastWordGroupRename>(new CFastWordGroupRename());
		pInstance->Create(wnd, "重命名组", UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
	}

	CEditUI* pEditGroupName = static_cast<CEditUI*>(pInstance->m_PaintManager.FindControl(_T("editFastWordGroupName")));
	if(pEditGroupName != NULL)
		pEditGroupName->SetText(strGroupName);
	
	pInstance->CenterWindow();
	::BringWindowToTop(*pInstance);
	pInstance->SetParentHwnd(wnd);
	pInstance->SetChangedGroupName(strGroupName);
	return pInstance;
}

void CFastWordGroupRename::SetParentHwnd(HWND pDlg)
{
	m_parent = pDlg;
}

void CFastWordGroupRename::SetChangedGroupName(CString strGroupName)
{
	m_strGroupName = strGroupName;
}
