// CExitWarning.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "CExitMsgWarning.h"
#include "afxdialogex.h"
#include "MainFrame.h"
#include "CExitWarning.h"
#include "Im\MessageHandleWnd.h"
#include "Im\LocalData.h"

extern map<FRIEND_INFO *,UINT>  g_mapMsgUnRead;////����δ����Ϣ�ĺ���ID��δ������
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
		    pInstance->Create(GetDesktopWindow(),_T("�رջỰ"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);///�������ʱ��������ʾ
		}
		else
		{
		    pInstance->Create(wnd,_T("�رջỰ"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);////�������ʱ��������ֹ�����ڲ���
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

	m_pBtnAdmin= static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("okbtn"));   ////ȷ��
	m_pBtnCancel= static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("cancelbtn"));//ȡ��
}

void CExitMsgWarningDlg::ReSetWindow(int nTag)
{
	m_wTag = nTag;
	switch(nTag)
	{
	   case CLOSE_TALK://�ر�һ����ѯ����ʱ
		   {
			    m_pLabelTitle->SetText("�ر�����");
			   /* m_pLabelIcon->SetBkImage("SysBtn\\icon_tips_warning.png");*/
			    m_pLabelWarn->SetText("����δ�������Ϣ,ȷ��Ҫ�ر����촰����");
				m_pTabLayoutA->SelectItem(1);
				m_pCheckMsgBuf->SetText("�´δ����촰�ڻָ�δ����Ϣ��ϵ��");
				m_pCheckMsgBuf->Selected(true);
		   }
		   break;
	   case EXIT_APP:
	   case MOD_USER:
		
		   {
			   m_pLabelTitle->SetText("�ر�����");
			  /* m_pLabelIcon->SetBkImage("SysBtn\\icon_tips_warning.png");*/
			   m_pLabelWarn->SetText("����δ�������Ϣ,ȷ��Ҫ�˳�������");
			   m_pTabLayoutA->SelectItem(1);
			   m_pCheckMsgBuf->SetMaxWidth(200);
			   m_pCheckMsgBuf->SetText("�´ε�¼ʱ�ָ�δ����Ϣ��ϵ��");
			   m_pCheckMsgBuf->Selected(true);
		   }
		   break;
	   case LOGIN_AGAIN:
		   {
			   m_pLabelTitle->SetText("��¼������ʾ");
		/*	   m_pLabelIcon->SetBkImage("SysBtn\\icon_tips_warning.png");*/
			   m_pLabelWarn->SetText("���ϴ��˳�ǰ��δ������ļ�ʱ��Ϣ,�Ƿ���Ҫ");
			   m_pTabLayoutA->SelectItem(0);
			   m_pLabelHide->SetText("��֮ǰ����Ϣ��");

		   }
		   break;
	   case CLOSE_MUL_TALK:////�ر����л��߹رյ�ǰ����
		   {
			   m_pLabelTitle->SetText("�ر�����");
			/*   m_pLabelIcon->SetBkImage("SysBtn\\icon_tips_warning.png");*/
			   m_pLabelWarn->SetText("����δ�������Ϣ,ȷ��Ҫ�ر����촰����");
			   m_pTabLayoutA->SelectItem(1);
			   m_pCheckMsgBuf->SetText("�´δ����촰�ڻָ�δ����Ϣ��ϵ��");
			 //  m_pTabLayoutB->SelectItem(0);////�л���0����

			   m_pBtnAdmin->SetText("�ر�����");
			   m_pBtnCancel->SetText("�رյ�ǰ");
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
	// ��ֹ�������
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
			CMainFrame::GetInstance()->ReleaseDlg();///�ſ�
			if(m_wTag == CLOSE_TALK)
			{
				////������������ѯ��ر�һ������
                SaveCloseTalkCfg();
			}	
			else  if(m_wTag == LOGIN_AGAIN)////�´ε�¼��������
			{
				OnBnClickedBtnExit();
				WORD nUnReadWnd = 0;
				g_LocalData.GetUnReadWndCnt(nUnReadWnd);//δ����������
				if(g_mapMsgUnRead.size()>0)
				{
	               g_mapMsgUnRead.clear();//�´ε�¼�����g_mapMsgUnRead
				}
				if(nUnReadWnd>0)///δ��������
				{
					DWORD dwFid = 0;
					DWORD dwCnt = 0;
					for(int nCnt = 0;nCnt<nUnReadWnd;nCnt++)
					{
						g_LocalData.GetUnReadParamsCfg(dwFid,dwCnt,nCnt);
						FRIEND_INFO *pFriend = GetFriendInfo(dwFid);
						if(pFriend!= NULL)
						{
							g_mapMsgUnRead[pFriend] = dwCnt;////�洢δ����Ϣ
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
				////δ����Ϣ����//////////////////////////////////////////////////
				if(nUnReadWnd>0)///δ��������
				{
					CString strFid = _T("");
					CString strCnt = _T("");
					DWORD dwFid = 0;
					DWORD dwCnt = 0;
					map<FRIEND_INFO *,UINT>::iterator it = g_mapMsgUnRead.begin();
					for(;it!=g_mapMsgUnRead.end();it++)
					{
						nIndex++;
						CMainFrame::GetInstance()->SaveUnReadMsgLog(it->first,it->second,nIndex,TRUE);////����δ����Ϣ					
					}
					WORD nExitWarn = 0;
					g_LocalData.GetCloseTalkWarnConfig(nExitWarn);//��ǰ�˳��Ƿ���ʾ
					if(nExitWarn==0)///��ʾδ��
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
					//�õ���ǰ�����·����
					char strPath[100]; 
					GetModuleFileName(NULL,strPath,100);
					//�����ػ����̣��������½��̳ɹ�����WM_QUIT��Ϣ����ԭ���Ľ��̣�
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
			else  if(m_wTag == CLOSE_MUL_TALK)////�ر�����
			{
				
				SaveCloseTalkCfg(CLOSE_ALL);////�ر�ȫ��
				g_pMessageHandleWnd->CloseAllP2PSendMsgDlg();
			}
		}
		else if(msg.pSender->GetName() == "cancelbtn")
		{
			CMainFrame::GetInstance()->ReleaseDlg();///�ſ�
			if(m_wTag == CLOSE_TALK)
			{
				m_bUnReadMsgShow = FALSE;
				Close(IDCANCEL);
			}	
			else  if(m_wTag == LOGIN_AGAIN)
			{
				WORD nVK = 0;
				g_LocalData.SaveLoginUnReadMsgCfg(nVK);///����ʾ
				if(g_mapMsgUnRead.size()>0)
				{
					g_mapMsgUnRead.clear();//�´ε�¼���ָ�δ����Ϣ�����
				}
				Close(IDCANCEL);
			}
			else  if(m_wTag ==EXIT_APP ||m_wTag == MOD_USER)
			{
				Close(IDCANCEL);
			}
			else  if(m_wTag == CLOSE_MUL_TALK)////�رյ�ǰ
			{
                SaveCloseTalkCfg();
				::SendMessage(m_parent, WM_CLOSE_ONE, NULL, NULL);
			}
			
		}
		else if(msg.pSender->GetName() == "closebtn")
		{	
			CMainFrame::GetInstance()->ReleaseDlg();////�򿪶Ի���	
			Close(IDCANCEL);
		}
	}
}

////��δ����Ϣ���´���������
void CExitMsgWarningDlg::SaveCloseTalkCfg(int nCode)
{
	if(CLOSE_TALK == nCode)////�ر�һ������
	{
	    WORD nExitWarn = 0;
		if(m_pCheckMsgBuf->IsSelected())
		{
			m_bUnReadMsgShow = TRUE;
			nExitWarn = 1;
			g_LocalData.SaveCloseTalkWarnConfig(nExitWarn);//�˳���ʾ
		}
		else
		{
			m_bUnReadMsgShow = TRUE;
			nExitWarn = 0;
			g_LocalData.SaveCloseTalkWarnConfig(nExitWarn);//�˳�����ʾ
		}
		 pInstance->Close();
	}
	else if(CLOSE_ALL == nCode)  ////ȫ���ر�
	{
		WORD nExitWarn = 0;
		if(m_pCheckMsgBuf->IsSelected())
		{
			m_bUnReadMsgShow = TRUE;
			nExitWarn = 1;
			g_LocalData.SaveCloseTalkWarnConfig(nExitWarn);//�˳���ʾ
		}
		else
		{
			m_bUnReadMsgShow = TRUE;
			nExitWarn = 0;
			g_LocalData.SaveCloseTalkWarnConfig(nExitWarn);//�˳�����ʾ
			nExitWarn = 0;
			g_LocalData.SaveLoginUnReadMsgCfg(nExitWarn);
			return ;
		}
		pInstance->Close();
	}
	
}

////��δ����Ϣ���´ε�¼����
void CExitMsgWarningDlg::OnBnClickedBtnExit()
{
	WORD nExitWarn = 0;
	if(m_pCheckMsgBuf->IsSelected())
	{
		nExitWarn = 1;
		g_LocalData.SaveExitMsgWarnConfig(nExitWarn);//δ�˳�,����δ����Ϣ
	}
	else
	{
		nExitWarn = 0;
		g_LocalData.SaveExitMsgWarnConfig(nExitWarn);//������δ����Ϣ
	}
	 pInstance->Close();
}

////�´ε�¼�Ժ���ʾδ����Ϣ
void CExitMsgWarningDlg::SaveMsgUnReadCfg()
{
	WORD nExitWarn = 0;
	if(m_pCheckMsgBuf->IsSelected())///ѡ��
	{
		nExitWarn = 1;
		g_LocalData.SaveLoginUnReadMsgCfg(nExitWarn);//�´ε�¼��ʾ
	}
	else///δѡ��
	{
		nExitWarn = 0;
		g_LocalData.SaveLoginUnReadMsgCfg(nExitWarn);//�´ε�¼����ʾ
	}
	pInstance->Close();
}

void CExitMsgWarningDlg::IsExitWarningShow()
{
	WORD nWarn;
	g_LocalData.GetExitWarnConfig(nWarn); //�˳��澯,���ҿ��Ը�������
	if(nWarn!=0) // ������
	{
		// ������ȷ���ٹر�	
		CExitWarningDlg::Show();
	}
	else
	{	// ���������ѣ�ֱ�ӹر�������
		::PostMessage(CMainFrame::GetInstance()->GetHWND(),WM_EXIT_MAINFRM,1,1);//��������Ӧ
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
	::GetWindowRect(pInstance->GetHWND(),rcDlgs);   //�õ��Ի����Rect �Ի���Ĵ�С
	int cx = GetSystemMetrics(SM_CXSCREEN);  //�����Ļ�ķֱ���
	int cy = GetSystemMetrics(SM_CYSCREEN); 
	SetWindowPos(pInstance->GetHWND(),CWnd::wndTopMost,//����λ��
		cx/2-rcDlgs.Width()/2,
		cy/2-rcDlgs.Height()/2,//λ��
		rcDlgs.Width(),
		rcDlgs.Height(),//��С
		SWP_SHOWWINDOW);//SWP_SHOWWINDOW����ʾ���ڡ�
}