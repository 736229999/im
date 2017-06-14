#include "stdafx.h"
#include "SellerMobileAuth.h"
#include "Im\MessageBox.h"

extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);

std::tr1::shared_ptr<CSellerMobileAuthDlg> CSellerMobileAuthDlg::pInstance = NULL;
std::tr1::shared_ptr<CSellerMobileAuthDlg> CSellerMobileAuthDlg::Show(HWND parent)
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
	}
	else
	{
		pInstance = tr1::shared_ptr<CSellerMobileAuthDlg>(new CSellerMobileAuthDlg());	
		pInstance->Create(parent, "�ֻ���", UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);

		pInstance->CenterWindow();
		pInstance->ShowModal();
	}

	UpdateBk();

	return pInstance;
}

DuiLib::UILIB_RESOURCETYPE CSellerMobileAuthDlg::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LRESULT CSellerMobileAuthDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(WM_TIMER == uMsg)	OnTimer(wParam);
	return DuiLib::WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

LPCTSTR CSellerMobileAuthDlg::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CSellerMobileAuthDlg::InitWindow()
{
	DuiLib::WindowImplBase::InitWindow();
	SetIcon(IDR_MAINFRAME);
	UpdateBk();
}

void CSellerMobileAuthDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if(msg.sType == "click")
	{
		if(msg.pSender->GetName() == "btnGetAuthCode")
		{	// ��ȡ��֤��
			// У���ֻ���
			DuiLib::CControlUI *pCtrlMobile = m_PaintManager.FindControl("editmobil");
			CString strMobile = pCtrlMobile->GetText().GetData();
			if(strMobile.GetLength() != 11)
			{
				CMessageBox::Model(GetHWND(), "�ֻ��Ŵ���");
				return;
			}

			KillTimer(GetHWND(), TIMERID_REGETAUTHCODE);
			m_nDescend = 60;
			SetTimer(GetHWND(), TIMERID_REGETAUTHCODE, 1000, NULL);
			msg.pSender->SetEnabled(false);

			msg.pSender->SetText("���·���(60��)");

			DuiLib::CControlUI *pCtrlTip = m_PaintManager.FindControl("edittip");
			pCtrlTip->SetText("��������֤��");
		}
	}
}

void CSellerMobileAuthDlg::UpdateBk()
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

void CSellerMobileAuthDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent) 
	{
	case TIMERID_REGETAUTHCODE:
		{
			DuiLib::CControlUI *pCtrl = m_PaintManager.FindControl("btnGetAuthCode");
			m_nDescend--;
			if(m_nDescend == 0)
			{
				KillTimer(GetHWND(), TIMERID_REGETAUTHCODE);
				pCtrl->SetEnabled(true);
				pCtrl->SetText("��ȡ��֤��");

				DuiLib::CControlUI *pCtrlTip = m_PaintManager.FindControl("edittip");
				pCtrlTip->SetText("���ȡ��֤��");
			}
			else
			{
				CString strText;
				strText.Format("���·���(%d��)", m_nDescend);
				pCtrl->SetText(strText);
			}
		}
		break;
	default:
		break;
	}
}