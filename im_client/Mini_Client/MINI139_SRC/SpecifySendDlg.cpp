// SpecifySendDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SpecifySendDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "..\..\include\ClientProtocol.h"
#include "im\xtpacket.h"
#include "im\P2PSendMsgDlg.h"
#include "im\DuiP2PSendMsgDlg.h"
#include "im\MessageHandleWnd.h"
#include "CSPlatDlg.h"
#include "im\DuiCounsel.h"
using namespace DuiLib;
// CSpecifySendDlg 对话框
////
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
void CSpecifySendDlg::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));

	SetIcon(IDR_MAINFRAME);

	//////////获取控件变量
	m_pEditName = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl("usernameEdit"));

	m_pBtnClose = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnclose"));
	m_pBtnOK = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnOK"));
	m_pBtnCancel = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnCancel"));
	
	UpdateBk();

	m_pEditName->SetFont(12);
	m_pEditName->SetFocus();
	m_bSearch = FALSE;
	g_pProtocol->RegistCmd(CMD_XTREAM_SEARCH_ACK,GetHWND());
}


std::tr1::shared_ptr<CSpecifySendDlg> CSpecifySendDlg::pInstance = NULL;
std::tr1::shared_ptr<CSpecifySendDlg> CSpecifySendDlg::Show()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		BringWindowToTop(pInstance->GetHWND());
    }
	else
	{
		pInstance = tr1::shared_ptr<CSpecifySendDlg>(new CSpecifySendDlg());
	    pInstance->Create(GetDesktopWindow(), _T(""), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}
	return pInstance;
}

void CSpecifySendDlg::CloseDlg()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		pInstance->Close();
	}
}

// 消息
void CSpecifySendDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
	    if(msg.pSender == m_pBtnClose) ////关闭
	    {
		    OnCancel();
	    }
		if(msg.pSender == m_pBtnOK) ////确定
		{
			OnOK();
		}
		else if(msg.pSender == m_pBtnCancel) ////取消
		{
			OnCancel();
		}
	}
}

BEGIN_XT_MESSAGE_MAP(CSpecifySendDlg)
	ON_XT_COMMAND(CMD_XTREAM_SEARCH_ACK, OnSearchUser)
END_XT_MESSAGE_MAP()


LRESULT CSpecifySendDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	// 处理后台交互消息
	if(WM_XT_CMD_NOTIFY == uMsg)
	{
		OnXtMessage(wParam, lParam);
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CSpecifySendDlg::OnFinalMessage(HWND hWnd)
{
	g_pProtocol->UnRegistCmd(GetHWND());
	DuiLib::WindowImplBase::OnFinalMessage(hWnd);
}


void CSpecifySendDlg::UpdateBk()
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

void CSpecifySendDlg::OnOK()
{
	m_pBtnOK->SetEnabled(FALSE);
	CString strName = m_pEditName->GetText();

	m_bSearch = TRUE;
	XT_XTREAM_SEARCH_REQ req;
	req.search_type = XTREAM_SEARCH_PRECISION;
	strcpy(req.user_name, strName);

	g_pProtocol->SendReq(req);
}

// 查找用户
void CSpecifySendDlg::OnSearchUser(const char *data, int nRetCode)
{
	if(!m_bSearch)
		return;
	m_bSearch = FALSE;
	if ( nRetCode==0 )
	{
		CRecvPacket rp((char *)data,MAX_RECV_BUFFERS);

		int i;
		uint16	next_pos;
		uint32	total;
		int		nTotal;
		uint8	count;
		uint8	realCount = 0;
		rp>>count>>next_pos>>total;
		// 总在线人数
		nTotal = total;
		
		for ( i=0; i<count; i++ )
		{
			FRIEND_INFO friendinfo;
			friendinfo.SetGuid("");
			friendinfo.SetPassword("");

			uint32 id = 0;
			rp  >>id;
			friendinfo.SetId(id);
			rp	>>friendinfo.sex
				>>friendinfo.status
				>>friendinfo.rule;
			char username[MAX_USERNAME_LEN+1] = {0};	
			rp	>>username;
			friendinfo.SetUserName(username);
			rp	>>friendinfo.nickname;

			if ( (friendinfo.GetId() != g_ImUserInfo.GetId()) )
			{
				FRIEND_INFO *pFriend = NULL;
				/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *pDlg;
				pFriend = ::FindFriend(friendinfo.GetId());
				if( pFriend)
				{
					pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
				}
				else
				{
					pFriend = new FRIEND_INFO;
					*pFriend = friendinfo;
					pFriend->group_index = 1;//陌生人
					CMainFrame::GetInstance()->AddFriend(pFriend, false);
					pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
				}	
				if ( pDlg->GetHWND()==NULL )
				{
					pDlg->NotifyServer_OpenedMsgDlg();
				}

				pDlg->ShowWindow(SW_SHOW);

				if (::IsMinimized(pDlg->GetHWND()) )
				{
					pDlg->ShowWindow(SW_RESTORE);
				}
			    ::BringWindowToTop(pDlg->GetHWND());
#ifdef _CS_PLAT_EX_
				CDuiCSPlatDlg::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
#elif defined _WORK_EX_
				CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
#else
				if(CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId()))
					CDuiCounsel::GetInstance()->ShowSelectP2PSendMsgDlg(pDlg);
#endif
			    Close(IDOK);
				////::SendMessage(GetParent()->GetSafeHwnd(), WM_SPEC_SEND_DLG_MSG, IDOK, 0);
			}
			else
			{	// 不能和自己聊天
				CMessageBox::Model(GetHWND(),"不能和自己聊天！",0,IDOK);
				m_pEditName->SetFocus();
			}
			break;
		}
		
	}
	else
	{	// 查找联系人失败
		CMessageBox::Model(GetHWND(),"未查找到联系人！",0,IDOK);
		m_pEditName->SetFocus();
	}

	m_pBtnOK->SetEnabled(TRUE);

}

void CSpecifySendDlg::OnCancel()
{
    Close(IDOK);
////	::SendMessage(GetParent()->GetSafeHwnd(), WM_SPEC_SEND_DLG_MSG, IDCANCEL, 0);
}



////IMPLEMENT_DYNAMIC(CSpecifySendDlg, CXSkinDlg)
////
////CSpecifySendDlg::CSpecifySendDlg()
////	: CXSkinDlg(IDD_DLG_SPECIFY_SEND, NULL)
////{
////	m_colorBackground = RGB(255, 255, 255);
////	m_colorBorder = CHAT_BKG_COLOR;
////	DeleteObject(m_hBkGround);
////	m_hBkGround = ::CreateSolidBrush(m_colorBackground);
////
////	m_bSearch = FALSE;
////	SetDlgStyle(CXSkinDlg::CLOSE);
////}
////
////CSpecifySendDlg::~CSpecifySendDlg()
////{
////	g_pProtocol->UnRegistCmd(GetSafeHwnd());
////}
////
////void CSpecifySendDlg::DoDataExchange(CDataExchange* pDX)
////{
////	CXSkinDlg::DoDataExchange(pDX);
////	DDX_Control(pDX, IDOK, m_btnOK);
////	DDX_Control(pDX, IDCANCEL, m_btnCancel);
////	DDX_Control(pDX, IDC_EDIT_NAME, m_pEditName->);
////}
////
////
////BEGIN_MESSAGE_MAP(CSpecifySendDlg, CXSkinDlg)
////	ON_WM_ERASEBKGND()
////	ON_WM_CTLCOLOR()
////	ON_XT_MESSAGE()
////END_MESSAGE_MAP()
////
////BEGIN_XT_MESSAGE_MAP(CSpecifySendDlg)
////	ON_XT_COMMAND(CMD_XTREAM_SEARCH_ACK, OnSearchUser)
////END_XT_MESSAGE_MAP()
////
////
////// CSpecifySendDlg 消息处理程序
////BOOL CSpecifySendDlg::OnInitDialog()
////{
////	CXSkinDlg::OnInitDialog();
////	SetWindowText("指定发送");
////	GetDlgItem(IDC_STATIC_NAME)->SetFont(&g_font12);
////	m_pEditName->.SetBorderColor(RGB(145,145,145));
////	g_pProtocol->RegistCmd(CMD_XTREAM_SEARCH_ACK,m_hWnd);
////	m_pEditName->.SetFocus();
////	return FALSE;
////}
////
////// 创建
////BOOL CSpecifySendDlg::Create(CWnd* pParent)
////{
////	BOOL bRlt = CXSkinDlg::Create(IDD_DLG_SPECIFY_SEND, pParent);
////	if(bRlt)
////	{
////		CenterWindow(pParent);
////		ShowWindow(SW_SHOW);
////	}
////
////	return bRlt;
////}
////
////BOOL CSpecifySendDlg::OnEraseBkgnd(CDC* pDC)
////{
////	CXSkinDlg::OnEraseBkgnd(pDC);
////
////	return FALSE;
////}
////
////HBRUSH CSpecifySendDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
////{
////	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
////	if ( nCtlColor==CTLCOLOR_STATIC )
////	{
////		pDC->SetBkMode(TRANSPARENT);
////		hbr = (HBRUSH)m_hBkGround;
////	}
////	return hbr;
////}
////
////void CSpecifySendDlg::OnOK()
////{
////	m_btnOK.EnableWindow(FALSE);
////	Invalidate(TRUE);
////	CString strName;
////	m_pEditName->.GetWindowText(strName);
////	m_bSearch = TRUE;
////	XT_XTREAM_SEARCH_REQ req;
////	req.search_type = XTREAM_SEARCH_PRECISION;
////	strcpy(req.user_name, strName);
////
////	g_pProtocol->SendReq(req);
////}
////
////// 查找用户
////void CSpecifySendDlg::OnSearchUser(const char *data, int nRetCode)
////{
////	if(!m_bSearch)
////		return;
////	m_bSearch = FALSE;
////	if ( nRetCode==0 )
////	{
////		CRecvPacket rp((char *)data,MAX_RECV_BUFFERS);
////
////		int i;
////		uint16	next_pos;
////		uint32	total;
////		int		nTotal;
////		uint8	count;
////		uint8	realCount = 0;
////		rp>>count>>next_pos>>total;
////		// 总在线人数
////		nTotal = total;
////		
////		for ( i=0; i<count; i++ )
////		{
////			FRIEND_INFO friendinfo;
////			friendinfo.SetGuid("");
////			friendinfo.SetPassword("");
////
////			uint32 id = 0;
////			rp  >>id;
////			friendinfo.SetId(id);
////			rp	>>friendinfo.sex
////				>>friendinfo.status
////				>>friendinfo.rule;
////			char username[MAX_USERNAME_LEN+1] = {0};	
////			rp	>>username;
////			friendinfo.SetUserName(username);
////			rp	>>friendinfo.nickname;
////
////			if ( (friendinfo.GetId() != g_ImUserInfo.GetId()) )
////			{
////				FRIEND_INFO *pFriend = NULL;
////				/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *pDlg;
////				pFriend = ::FindFriend(friendinfo.GetId());
////				if( pFriend)
////				{
////					pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
////				}
////				else
////				{
////					pFriend = new FRIEND_INFO;
////					*pFriend = friendinfo;
////					pFriend->group_index = 1;//陌生人
////					((CMainFrame *)AfxGetMainWnd())->AddFriend(pFriend, false);
////					pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);
////				}	
////				if ( pDlg->GetHWND()==NULL )
////				{
////					//pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
////					pDlg->NotifyServer_OpenedMsgDlg();
////				}
////
////				pDlg->ShowWindow(SW_SHOW);
////
////				if ( /*pDlg->IsIconic()*/::IsMinimized(pDlg->GetHWND()) )
////					pDlg->ShowWindow(SW_RESTORE);
////
////				/*pDlg->*/::BringWindowToTop(pDlg->GetHWND());
////				CXSkinDlg::OnOK();
////				::SendMessage(GetParent()->GetSafeHwnd(), WM_SPEC_SEND_DLG_MSG, IDOK, 0);
////			}
////			else
////			{	// 不能和自己聊天
////				MessageBox("不能和自己聊天！");
////				m_pEditName->.SetFocus();
////			}
////			break;
////		}
////		
////	}
////	else
////	{	// 查找联系人失败
////		MessageBox("未查找到联系人！");
////		m_pEditName->.SetFocus();
////	}
////
////	m_btnOK.EnableWindow(TRUE);
////	Invalidate(TRUE);
////}
////
////void CSpecifySendDlg::OnCancel()
////{
////	CXSkinDlg::OnCancel();
////	::SendMessage(GetParent()->GetSafeHwnd(), WM_SPEC_SEND_DLG_MSG, IDCANCEL, 0);
////}