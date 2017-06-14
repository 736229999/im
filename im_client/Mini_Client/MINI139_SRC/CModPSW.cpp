// CExitWarning.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "CModPSW.h"
#include "afxdialogex.h"
#include "MainFrame.h"

extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
std::tr1::shared_ptr<CModPassWordDlg> CModPassWordDlg::pInstance = NULL;
std::tr1::shared_ptr<CModPassWordDlg> CModPassWordDlg::Show()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
	}
	else
	{
		pInstance = tr1::shared_ptr<CModPassWordDlg>(new CModPassWordDlg());
		pInstance->Create(GetDesktopWindow(), _T("�رջỰ"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}
	return pInstance;
}

DuiLib::UILIB_RESOURCETYPE CModPassWordDlg::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LPCTSTR CModPassWordDlg::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

BEGIN_XT_MESSAGE_MAP(CModPassWordDlg)
	ON_XT_COMMAND(CMD_MOD_PSW_ACK,OnModPswAck)
END_XT_MESSAGE_MAP()

void CModPassWordDlg::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));
	SetIcon(IDR_MAINFRAME);

	m_pEditNewPSW = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl("EditNewPSW"));  ////������
	m_pLabelInputTips = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("LabelInputTips"));
	m_pEditConfirmPSW = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl("EditConfirmPSW")); ////ȷ������
	m_pLabelCheckError = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("LabelCheckError"));
	m_pEditNewPSW->SetFocus();
	m_pLabelCheckError->SetText("");
	UpdateBk();
	////ע��ȷ����Ϣ
	g_pProtocol->RegistCmd(CMD_MOD_PSW_ACK,m_hWnd);
}

LRESULT CModPassWordDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// ��ֹ�������
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}
	// �����̨������Ϣ
	else if(WM_XT_CMD_NOTIFY == uMsg)
	{
		OnXtMessage(wParam, lParam);
	}
	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CModPassWordDlg::Notify(DuiLib::TNotifyUI& msg)
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

void CModPassWordDlg::UpdateBk()
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

void CModPassWordDlg::OnBnClickedBtnAdmin()
{
	////��������Ƿ�Ϸ���6~18�ַ�),���������Ƿ���ͬ
	CString strNewPsw = m_pEditNewPSW->GetText().GetData();      ////������
	CString strConfirmPsw = m_pEditConfirmPSW->GetText().GetData();//ȷ������
	int nLenPsw = strlen(strNewPsw);
	if(nLenPsw<MIN_PSW_LEN || nLenPsw>MAX_PSW_LEN) ///���볤�Ȳ��Ϸ�
	{
		m_pLabelInputTips->SetTextColor(0XFF0000);////��ɫ��ʶ
		m_pLabelInputTips->SetText("X����6~18�ַ�,������ĸ�����ֺͷ������");
		m_pEditNewPSW->SetFocus();
		m_pLabelCheckError->SetText("");
		return ;
	}
	else if(strcmp(strNewPsw,strConfirmPsw)!=0) ///���߲���
	{
		if(0 == strlen(strConfirmPsw))//ȷ��Ϊ��
		{
			 m_pLabelInputTips->SetText("");
			 m_pLabelCheckError->SetText("X������ȷ������");
			 m_pEditConfirmPSW->SetFocus();
			 return ;
		}
		else
		{
			m_pLabelInputTips->SetText("");
	        m_pLabelCheckError->SetText("X�������벻һ��");
			m_pEditConfirmPSW->SetFocus();
		    return ;
		}
	}
	else   ////Э�鷢�͵���̨�޸����루�û�g_ImUserInfo��������)
	{
		XT_MOD_PSW_REQ  ReqModPsw;
		memset(&ReqModPsw,0,sizeof(XT_MOD_PSW_REQ));
		ReqModPsw.uid = g_ImUserInfo.GetId();
		sprintf(ReqModPsw.szPsw,strNewPsw);
		extern CIMProtocol * g_pProtocol;
		g_pProtocol->SendReq(ReqModPsw);
	    pInstance->ShowWindow(false);
	}
}

////�޸����������ȷ��
void CModPassWordDlg::OnModPswAck(const char *data, WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_MOD_PSW_ACK ack = (LPXT_MOD_PSW_ACK)data;
		if ( ack->uid == g_ImUserInfo.GetId() ) ////���ص�ǰ�û�����Ϣ
		{  
			ShowMessage("�����޸ĳɹ�!",MB_OK|MB_ICONINFORMATION);
			pInstance->Close(IDOK);
		}
		else
		{
			ShowMessage("�����޸�ʧ��!",MB_OK|MB_ICONINFORMATION);
			pInstance->Close(IDOK);
		}
	}
	else
	{
	   ShowMessage("�����޸�ʧ��!",MB_OK|MB_ICONINFORMATION);
	   pInstance->Close(IDOK);
	}
}

void CModPassWordDlg::CloseDlg()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		pInstance->Close(IDOK);
	}
}
