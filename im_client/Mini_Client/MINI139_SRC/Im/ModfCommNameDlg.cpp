// im\ModfCommNameDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ModfCommNameDlg.h"
#include "floorPagefriend.h"
#include ".\modfcommnamedlg.h"
#include "..\MainFrame.h"
using namespace DuiLib;

extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);

CModfCommNameDlg::	CModfCommNameDlg()
{
	m_pFriend = NULL;
}
CModfCommNameDlg::~CModfCommNameDlg()
{

	if(m_pFriend = NULL)
	{
	   delete m_pFriend;
	   m_pFriend = NULL;
	}

}
std::tr1::shared_ptr<CModfCommNameDlg> CModfCommNameDlg::pInstance = NULL;
std::tr1::shared_ptr<CModfCommNameDlg> CModfCommNameDlg::Show(FRIEND_INFO* pFriend)
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		BringWindowToTop(pInstance->GetHWND());
		pInstance->UpdateFriendInfo(pFriend);
	}
	else
	{
		pInstance = tr1::shared_ptr<CModfCommNameDlg>(new CModfCommNameDlg());
		pInstance->Create(GetDesktopWindow(), _T(""), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
	    pInstance->UpdateFriendInfo(pFriend);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}
	return pInstance;
}

void CModfCommNameDlg::UpdateFriendInfo(FRIEND_INFO *pFriend)
{
	m_pFriend = pFriend;
	if(m_pFriend)
	{
		m_pEditName->SetText(m_pFriend->comment_name);
		m_pEditName->SetSel(0,-1);
		m_pEditName->SetSel(-1,-1);
	}
}

void CModfCommNameDlg::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));

	SetIcon(IDR_MAINFRAME);

	//////////获取控件变量
	m_pEditName = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl("BackNameEdit"));

	m_pBtnMin = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnmin"));
	m_pBtnClose = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnclose"));
	m_pBtnOK = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnOK"));
	m_pBtnCancel = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnCancel"));

	m_pEditName->SetMaxChar(8);

	UpdateBk();

	m_pEditName->SetFont(12);
	m_pEditName->SetFocus();
}

// 消息
void CModfCommNameDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender == m_pBtnMin) ////最小化
		{
			SendMessage(WM_SYSCOMMAND,SC_MINIMIZE,NULL);
		}
		if(msg.pSender == m_pBtnClose) ////关闭
		{
			OnCancel();
		}
		else if(msg.pSender == m_pBtnOK) ////确定
		{
			OnOK();
		}
		else if(msg.pSender == m_pBtnCancel) ////取消
		{
			OnCancel();
		}
	}
}


LRESULT CModfCommNameDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CModfCommNameDlg::UpdateBk()
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

void CModfCommNameDlg::OnOK()
{
		ASSERT(m_pFriend);
		if(m_pFriend==NULL)
		{
			pInstance->Close(IDOK);
			return;
		}
	
		CString sCommName = m_pEditName->GetText();
		sCommName.Trim();
		if(sCommName==m_pFriend->comment_name)
		{
			pInstance->Close(IDOK);
			return;
		}
		// 陌生人不发送修改到服务器
		if(m_pFriend->group_index != 1)
		{
			//修改用户备注名称
			XT_MOD_FRIEND_COMMNAME_REQ req;
			req.fid = m_pFriend->GetId();
			req.id=g_ImUserInfo.GetId();
			strcpy(req.commName,sCommName);
			// 发送操作
			g_pProtocol->SendReq(req);
		}
	
		//CMainFrame::GetInstance()->SetCommName(m_pFriend,sCommName);	// 主界面已经处理了修改备注的消息
	
		////更新好友资料
		//XT_USERINFO_GET_REQ req;
		//req.fid = g_ImUserInfo.GetId();
		//g_pProtocol->SendReq(req);
	
		pInstance->Close(IDOK);
		//::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_MOD_BACKNAME, 0, 0);	
}

void CModfCommNameDlg::OnCancel()
{
	pInstance->Close(IDOK);
	::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_MOD_BACKNAME, 0, 0);	
}


//
//CModfCommNameDlg* CModfCommNameDlg::NewDlg(CMainFrame* pIMPage,FRIEND_INFO* pFriend,CWnd* pParent /* = NULL */)
//{
//	CModfCommNameDlg* pWnd=new CModfCommNameDlg(pIMPage,pFriend,pParent);
//	return pWnd;
//}



//void CModfCommNameDlg::OnShowWindow(BOOL bShow, UINT nStatus)
//{
//	CXSkinDlg::OnShowWindow(bShow, nStatus);
//	m_edtCommName.SetFocus();
//}

////void CModfCommNameDlg::DoDataExchange(CDataExchange* pDX)
////{
////	CXSkinDlg::DoDataExchange(pDX);
////	DDX_Control(pDX, IDOK, m_btnOK);
////	DDX_Control(pDX, IDCANCEL, m_btnCancel);
////	DDX_Control(pDX, IDC_EDIT1, m_edtCommName);
////}
////
////
////BEGIN_MESSAGE_MAP(CModfCommNameDlg, CXSkinDlg)
////	ON_BN_CLICKED(IDOK, OnBnClickedOk)
////	ON_WM_NCDESTROY()
////	ON_WM_SHOWWINDOW()
////END_MESSAGE_MAP()
////
////
////
////
////void CModfCommNameDlg::OnBnClickedOk()
////{
////	ASSERT(m_pIMPage);
////	ASSERT(m_pFriend);
////	if(m_pFriend==NULL || m_pIMPage==NULL)
////	{
////		OnOK();
////		return;
////	}
////
////	CString sCommName;
////	m_edtCommName.GetWindowText(sCommName);
////	sCommName.Trim();
////	if(sCommName==m_pFriend->comment_name)
////	{
////		OnOK();
////		return;
////	}
////	// 陌生人不发送修改到服务器
////	if(m_pFriend->group_index != 1)
////	{
////		//修改用户备注名称
////		XT_MOD_FRIEND_COMMNAME_REQ req;
////		req.fid = m_pFriend->GetId();
////		req.id=g_ImUserInfo.GetId();
////		strcpy(req.commName,sCommName);
////		// 发送操作
////		g_pProtocol->SendReq(req);
////	}
////	m_pIMPage->SetCommName(m_pFriend,sCommName);
////
////	//更新好友资料
////	XT_USERINFO_GET_REQ req;
////	req.fid = g_ImUserInfo.GetId();
////	g_pProtocol->SendReq(req);
////
////	OnOK();
////}
////
////void CModfCommNameDlg::OnNcDestroy()
////{
////	CXSkinDlg::OnNcDestroy();
////
////	delete this;
////}
////
////BOOL CModfCommNameDlg::OnInitDialog()
////{
////	CXSkinDlg::OnInitDialog();
////	SetDlgStyle(MINCLOSE);
////	m_edtCommName.LimitText(8);
////	if(m_pFriend)
////	{
////		m_edtCommName.SetWindowText(m_pFriend->comment_name);
////		m_edtCommName.SetSel(0,-1);
////		m_edtCommName.SetSel(-1,-1);
////	}
////
////	SetWindowText("修改备注名称");
////	return TRUE;  
////
////}
////
////void CModfCommNameDlg::OnShowWindow(BOOL bShow, UINT nStatus)
////{
////	CXSkinDlg::OnShowWindow(bShow, nStatus);
////	m_edtCommName.SetFocus();
////	
////}