// im\ClusterInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ClusterInfoDlg.h"
#include ".\clusterinfodlg.h"
#include "WebProcess.h"


#define TIMER_GET_INFO	0x100
// CClusterInfoDlg �Ի���

std::tr1::shared_ptr<CClusterInfoDlg> CClusterInfoDlg::pInstance2 = NULL;
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);

BEGIN_XT_MESSAGE_MAP(CClusterInfoDlg)
	ON_XT_COMMAND(CMD_GET_CINFO_ACK,OnClusterInfoAck)
END_XT_MESSAGE_MAP()

LRESULT CClusterInfoDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// ��ֹ�������
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	// �����̨������Ϣ
	if(WM_XT_CMD_NOTIFY == uMsg)
	{
		OnXtMessage(wParam, lParam);
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CClusterInfoDlg::OnFinalMessage(HWND hWnd)
{
	g_pProtocol->UnRegistCmd(m_hDlgWnd);
	DuiLib::WindowImplBase::OnFinalMessage(hWnd);
}

void CClusterInfoDlg::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();

	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));

	SetIcon(IDR_MAINFRAME);

	m_labelClusterName = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("clustername"));
	m_labelClusterNum = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("clusternum"));
	m_labelclusterType = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("clustertype"));
	m_editFamilyDesc = static_cast<DuiLib::CRichEditUI*>(m_PaintManager.FindControl("clusterdesc"));
	m_hDlgWnd = GetHWND();
	UpdateBk();
	// ע����Ϣ
	g_pProtocol->RegistCmd(CMD_GET_CINFO_ACK, m_hDlgWnd);
}

DuiLib::CControlUI* CClusterInfoDlg::CreateControl(LPCTSTR pstrClass)
{
	return NULL;
}

// ��Ϣ
void CClusterInfoDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
}

std::tr1::shared_ptr<CClusterInfoDlg> CClusterInfoDlg::NewDlg(XT_CLUSTER_INFO* pCluster)
{
// 	if(pInstance2 != NULL && pInstance2->m_hWnd != NULL)
// 	{
// 		pInstance2->Close();
// 		pInstance2 = NULL;
// 	}
   	if(pInstance2 == NULL || pInstance2->m_hWnd == NULL)
   	{
		pInstance2 = tr1::shared_ptr<CClusterInfoDlg>(new CClusterInfoDlg());
		pInstance2->Create(GetDesktopWindow(), "Ⱥ����", UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance2->CenterWindow();
	}
	else
	{
		::BringWindowToTop(*pInstance2);
	}
	pInstance2->SetCluster(pCluster);
	return pInstance2;
}

void CClusterInfoDlg::SetCluster(XT_CLUSTER_INFO* pCluster)
{
	m_pCluster = pCluster;
}

void CClusterInfoDlg::OnClusterInfoAck(const char *data,WORD wRet)
{
	LPXT_GET_CINFO_ACK ack = (LPXT_GET_CINFO_ACK)data;
	if(wRet==0)
	{
		ASSERT(m_pCluster);
		if ( ack->nFamilyID==m_pCluster->nFamilyID && ack->nFamilyProp==PERM_FAMILY
			)
		{
			memcpy(m_pCluster,ack,sizeof(XT_CLUSTER_INFO));
			m_labelClusterName->SetText(m_pCluster->szFamilyName);
			CString str;
			str.Format("%d", m_pCluster->nFamilyID);
			m_labelClusterNum->SetText(str);

			str = GetClusterClass(m_pCluster->nFamilyClass);
			m_labelclusterType->SetText(str);
			if(strcmp(m_pCluster->szFamilyDesc, "") == 0 || strcmp(m_pCluster->szFamilyDesc, "�����벻����50���ֵ�Ⱥ���,����Ϣ��ȫ������") == 0)
				m_editFamilyDesc->SetText("");
			else
				m_editFamilyDesc->SetText(m_pCluster->szFamilyDesc);
			if(!IsWindowVisible(*this))
				ShowWindow(true);
		}
	}
}

void CClusterInfoDlg::UpdateBk()
{
	if(pInstance2 != NULL && ::IsWindow(pInstance2->GetHWND()))
	{
		DuiLib::CControlUI* background = pInstance2->m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			::UpdateBk(background, pInstance2->GetHWND());
		}
	}
}
