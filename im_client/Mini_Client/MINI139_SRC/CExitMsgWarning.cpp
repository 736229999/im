// CExitWarning.cpp : 实现文件
//
#include "stdafx.h"
#include "CExitMsgWarning.h"
#include "afxdialogex.h"
#include "MainFrame.h"
#include "CExitWarning.h"
#include "Im\MessageHandleWnd.h"
#include "Im\LocalData.h"

extern map<FRIEND_INFO *,UINT>  g_mapMsgUnRead;////保存未读消息的好友ID和未读条数
extern CMessageHandleWnd *g_pMessageHandleWnd;
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
//
std::tr1::shared_ptr<CExitMsgWarningDlg> CExitMsgWarningDlg::pInstance = NULL;
std::tr1::shared_ptr<CExitMsgWarningDlg> CExitMsgWarningDlg::Show(int nTag,HWND wnd)
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
		pInstance->ReSetWindow(nTag);
	}
	else
	{
		pInstance = tr1::shared_ptr<CExitMsgWarningDlg>(new CExitMsgWarningDlg());
		if(nTag == EXIT_APP || nTag == LOGIN_AGAIN|| nTag == MOD_USER)
		{
		    pInstance->Create(GetDesktopWindow(),_T("关闭会话"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);///无聊天框时，居中显示
		}
		else
		{
		    pInstance->Create(wnd,_T("关闭会话"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);////有聊天框时弹出，禁止父窗口操作
		}
		pInstance->ReSetWindow(nTag);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}
	pInstance->SetParentHwnd(wnd);
	return pInstance;
}

void CExitMsgWarningDlg::SetParentHwnd(HWND pDlg)
{
	m_parent = pDlg;
}

DuiLib::UILIB_RESOURCETYPE CExitMsgWarningDlg::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LPCTSTR CExitMsgWarningDlg::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CExitMsgWarningDlg::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	UpdateBk();
	m_parent = NULL;
	m_bUnReadMsgShow = FALSE;
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));

	SetIcon(IDR_MAINFRAME);
	
	m_pBtnClose = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("closebtn"));
	m_pLabelTitle = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelTitle"));
	/*m_pLabelIcon = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("IconWarn"));*/
	m_pLabelWarn = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelWarn"));
	m_pLabelHide = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelHide"));
	m_pTabLayoutA = static_cast<DuiLib::CTabLayoutUI*>(m_PaintManager.FindControl("tabA"));
	m_pCheckMsgBuf   = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("MsgBuf"));

	m_pBtnAdmin= static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("okbtn"));   ////确定
	m_pBtnCancel= static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("cancelbtn"));//取消
}

void CExitMsgWarningDlg::ReSetWindow(int nTag)
{
	m_wTag = nTag;
	switch(nTag)
	{
	   case CLOSE_TALK://关闭一个咨询窗口时
		   {
			    m_pLabelTitle->SetText("关闭提醒");
			   /* m_pLabelIcon->SetBkImage("SysBtn\\icon_tips_warning.png");*/
			    m_pLabelWarn->SetText("您有未处理的消息,确认要关闭聊天窗口吗？");
				m_pTabLayoutA->SelectItem(1);
				m_pCheckMsgBuf->SetText("下次打开聊天窗口恢复未读消息联系人");
				m_pCheckMsgBuf->Selected(true);
		   }
		   break;
	   case EXIT_APP:
	   case MOD_USER:
		
		   {
			   m_pLabelTitle->SetText("关闭提醒");
			  /* m_pLabelIcon->SetBkImage("SysBtn\\icon_tips_warning.png");*/
			   m_pLabelWarn->SetText("您有未处理的消息,确认要退出哞哞吗？");
			   m_pTabLayoutA->SelectItem(1);
			   m_pCheckMsgBuf->SetMaxWidth(200);
			   m_pCheckMsgBuf->SetText("下次登录时恢复未读消息联系人");
			   m_pCheckMsgBuf->Selected(true);
		   }
		   break;
	   case LOGIN_AGAIN:
		   {
			   m_pLabelTitle->SetText("登录哞哞提示");
		/*	   m_pLabelIcon->SetBkImage("SysBtn\\icon_tips_warning.png");*/
			   m_pLabelWarn->SetText("您上次退出前有未处理完的即时消息,是否需要");
			   m_pTabLayoutA->SelectItem(0);
			   m_pLabelHide->SetText("打开之前的消息？");

		   }
		   break;
	   case CLOSE_MUL_TALK:////关闭所有或者关闭当前窗口
		   {
			   m_pLabelTitle->SetText("关闭提醒");
			/*   m_pLabelIcon->SetBkImage("SysBtn\\icon_tips_warning.png");*/
			   m_pLabelWarn->SetText("您有未处理的消息,确认要关闭聊天窗口吗？");
			   m_pTabLayoutA->SelectItem(1);
			   m_pCheckMsgBuf->SetText("下次打开聊天窗口恢复未读消息联系人");
			 //  m_pTabLayoutB->SelectItem(0);////切换到0窗口

			   m_pBtnAdmin->SetText("关闭所有");
			   m_pBtnCancel->SetText("关闭当前");
			   m_pCheckMsgBuf->Selected(true);
		   }
		   break;
	  default:
		 break;
	}
	SetWindowTop();
    
}

LRESULT CExitMsgWarningDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}
	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CExitMsgWarningDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);

	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender->GetName() == "okbtn")
		{
			CMainFrame::GetInstance()->ReleaseDlg();///放开
			if(m_wTag == CLOSE_TALK)
			{
				////兼容聊天框和咨询框关闭一个窗口
                SaveCloseTalkCfg();
			}	
			else  if(m_wTag == LOGIN_AGAIN)////下次登录提醒配置
			{
				OnBnClickedBtnExit();
				WORD nUnReadWnd = 0;
				g_LocalData.GetUnReadWndCnt(nUnReadWnd);//未读窗口总数
				if(g_mapMsgUnRead.size()>0)
				{
	               g_mapMsgUnRead.clear();//下次登录先清空g_mapMsgUnRead
				}
				if(nUnReadWnd>0)///未读窗口数
				{
					DWORD dwFid = 0;
					DWORD dwCnt = 0;
					for(int nCnt = 0;nCnt<nUnReadWnd;nCnt++)
					{
						g_LocalData.GetUnReadParamsCfg(dwFid,dwCnt,nCnt);
						FRIEND_INFO *pFriend = GetFriendInfo(dwFid);
						if(pFriend!= NULL)
						{
							g_mapMsgUnRead[pFriend] = dwCnt;////存储未读消息
						}							
					}
					CMainFrame::GetInstance()->PopUnReadMsgDlg();
				}  
			}
			else  if(m_wTag == EXIT_APP || m_wTag == MOD_USER)
			{
				SaveMsgUnReadCfg();
				WORD nUnReadWnd = g_mapMsgUnRead.size();
				g_LocalData.SaveUnReadWndCnt(nUnReadWnd);
				int nIndex = 0;
				////未读消息保存//////////////////////////////////////////////////
				if(nUnReadWnd>0)///未读窗口数
				{
					CString strFid = _T("");
					CString strCnt = _T("");
					DWORD dwFid = 0;
					DWORD dwCnt = 0;
					map<FRIEND_INFO *,UINT>::iterator it = g_mapMsgUnRead.begin();
					for(;it!=g_mapMsgUnRead.end();it++)
					{
						nIndex++;
						CMainFrame::GetInstance()->SaveUnReadMsgLog(it->first,it->second,nIndex,TRUE);////保存未读消息					
					}
					WORD nExitWarn = 0;
					g_LocalData.GetCloseTalkWarnConfig(nExitWarn);//当前退出是否提示
					if(nExitWarn==0)///提示未读
					{
						g_mapMsgUnRead.clear();
					}
				}

				if(m_wTag == EXIT_APP)
				{
					CMainFrame::GetInstance()->ExitApp();
				}
				else
				{
					g_ImUserInfo.bAutoLogin = false;
					CMainFrame::GetInstance()->SaveLoginAccount();
					//得到当前程序的路径；
					char strPath[100]; 
					GetModuleFileName(NULL,strPath,100);
					//创建守护进程，在启用新进程成功后发送WM_QUIT消息结束原来的进程；
					STARTUPINFO startInfo;
					PROCESS_INFORMATION processInfo;
					ZeroMemory(&startInfo, sizeof(STARTUPINFO));
					startInfo.cb = sizeof(STARTUPINFO);
					if(CreateProcess(NULL,(LPTSTR)(LPCTSTR)strPath, NULL, NULL,FALSE, 0, NULL, NULL, &startInfo, &processInfo))
					{
						CMainFrame::GetInstance()->CloseDlg();
					}
					CLoginDlg::Show();
				}
			}
			else  if(m_wTag == CLOSE_MUL_TALK)////关闭所有
			{
				
				SaveCloseTalkCfg(CLOSE_ALL);////关闭全部
				g_pMessageHandleWnd->CloseAllP2PSendMsgDlg();
			}
		}
		else if(msg.pSender->GetName() == "cancelbtn")
		{
			CMainFrame::GetInstance()->ReleaseDlg();///放开
			if(m_wTag == CLOSE_TALK)
			{
				m_bUnReadMsgShow = FALSE;
				Close(IDCANCEL);
			}	
			else  if(m_wTag == LOGIN_AGAIN)
			{
				WORD nVK = 0;
				g_LocalData.SaveLoginUnReadMsgCfg(nVK);///不提示
				if(g_mapMsgUnRead.size()>0)
				{
					g_mapMsgUnRead.clear();//下次登录不恢复未读消息则清空
				}
				Close(IDCANCEL);
			}
			else  if(m_wTag ==EXIT_APP ||m_wTag == MOD_USER)
			{
				Close(IDCANCEL);
			}
			else  if(m_wTag == CLOSE_MUL_TALK)////关闭当前
			{
                SaveCloseTalkCfg();
				::SendMessage(m_parent, WM_CLOSE_ONE, NULL, NULL);
			}
			
		}
		else if(msg.pSender->GetName() == "closebtn")
		{	
			CMainFrame::GetInstance()->ReleaseDlg();////打开对话框	
			Close(IDCANCEL);
		}
	}
}

////有未读消息，下次聊天提醒
void CExitMsgWarningDlg::SaveCloseTalkCfg(int nCode)
{
	if(CLOSE_TALK == nCode)////关闭一个窗口
	{
	    WORD nExitWarn = 0;
		if(m_pCheckMsgBuf->IsSelected())
		{
			m_bUnReadMsgShow = TRUE;
			nExitWarn = 1;
			g_LocalData.SaveCloseTalkWarnConfig(nExitWarn);//退出提示
		}
		else
		{
			m_bUnReadMsgShow = TRUE;
			nExitWarn = 0;
			g_LocalData.SaveCloseTalkWarnConfig(nExitWarn);//退出不提示
		}
		 pInstance->Close();
	}
	else if(CLOSE_ALL == nCode)  ////全部关闭
	{
		WORD nExitWarn = 0;
		if(m_pCheckMsgBuf->IsSelected())
		{
			m_bUnReadMsgShow = TRUE;
			nExitWarn = 1;
			g_LocalData.SaveCloseTalkWarnConfig(nExitWarn);//退出提示
		}
		else
		{
			m_bUnReadMsgShow = TRUE;
			nExitWarn = 0;
			g_LocalData.SaveCloseTalkWarnConfig(nExitWarn);//退出不提示
			nExitWarn = 0;
			g_LocalData.SaveLoginUnReadMsgCfg(nExitWarn);
			return ;
		}
		pInstance->Close();
	}
	
}

////有未读消息，下次登录提醒
void CExitMsgWarningDlg::OnBnClickedBtnExit()
{
	WORD nExitWarn = 0;
	if(m_pCheckMsgBuf->IsSelected())
	{
		nExitWarn = 1;
		g_LocalData.SaveExitMsgWarnConfig(nExitWarn);//未退出,弹出未读消息
	}
	else
	{
		nExitWarn = 0;
		g_LocalData.SaveExitMsgWarnConfig(nExitWarn);//不弹出未读消息
	}
	 pInstance->Close();
}

////下次登录以后，显示未读消息
void CExitMsgWarningDlg::SaveMsgUnReadCfg()
{
	WORD nExitWarn = 0;
	if(m_pCheckMsgBuf->IsSelected())///选中
	{
		nExitWarn = 1;
		g_LocalData.SaveLoginUnReadMsgCfg(nExitWarn);//下次登录提示
	}
	else///未选中
	{
		nExitWarn = 0;
		g_LocalData.SaveLoginUnReadMsgCfg(nExitWarn);//下次登录不提示
	}
	pInstance->Close();
}

void CExitMsgWarningDlg::IsExitWarningShow()
{
	WORD nWarn;
	g_LocalData.GetExitWarnConfig(nWarn); //退出告警,并且可以更改配置
	if(nWarn!=0) // 弹出框
	{
		// 弹出后确认再关闭	
		CExitWarningDlg::Show();
	}
	else
	{	// 不弹出提醒，直接关闭主窗体
		::PostMessage(CMainFrame::GetInstance()->GetHWND(),WM_EXIT_MAINFRM,1,1);//主界面响应
	}
}

void CExitMsgWarningDlg::UpdateBk()
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

void CExitMsgWarningDlg::SetWindowTop()
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