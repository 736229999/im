// CExitWarning.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "CheckServiceLogWarn.h"
#include "afxdialogex.h"
#include "MainFrame.h"
#include "CExitWarning.h"
#include "Im\MessageHandleWnd.h"
#include "Im\LocalData.h"

extern map<FRIEND_INFO *,UINT>  g_mapMsgUnRead;////����δ����Ϣ�ĺ���ID��δ������
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
		pInstance->Create(wnd,_T("��ܰ��ʾ"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
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
	m_pLabelWarn = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelWarn"));//��ʾ��Ϣ
	m_pBtnAdmin= static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("okbtn"));   ////ȷ��
	m_pBtnCancel= static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("cancelbtn"));//ȡ��
}

void CheckServiceLogWarn::ReSetWindow(int nTag)
{
	m_wTag = nTag;
	switch(nTag)
	{
	case UNSAVED_SERVICELOG://δ����
		{
			m_pLabelWarn->SetText("������δ����ķ���С��,���ȱ��棿");
		}
		break;
	case SAVED_SERVICELOG://�ѱ���
		{
            m_pLabelWarn->SetText("����С���ѱ���,�Ƿ�رջỰ����?");
		}
		break;
	case UNCOMPLETED_SERVICELOG://���治����
		{
			m_pLabelWarn->SetText("���ķ���С��δ��д����,������д");
		}
		break;
	case IS_BUYER_TRAN://�Ѿ�ת�ӵ����
		{
			m_pLabelWarn->SetText("���Ŀͻ��ѱ�ת��,�Ƿ�رյ�ǰ�Ự����?");
		}
		break;
	default:
		break;
	}
   	SetWindowTop();
}

LRESULT CheckServiceLogWarn::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// ��ֹ�������
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
			m_bClickedBtnOK = TRUE;//�����ȷ����ť
            Close(IDOK);//�رմ���
		}
		else if(msg.pSender->GetName() == "cancelbtn")//ȡ��
		{
			m_bClickedBtnOK = FALSE;
			Close(IDCANCEL);//�رմ���
		}
		else if(msg.pSender->GetName() == "closebtn")//�ر�
		{	
			m_bClickedBtnOK = FALSE;
            Close(IDCANCEL);//�رմ���
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