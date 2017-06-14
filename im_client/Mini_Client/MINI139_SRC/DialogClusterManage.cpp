// DialogClusterManage.cpp : 实现文件
//

#include "stdafx.h"
#include "DialogClusterManage.h"
#include "afxdialogex.h"


// CDialogClusterManage 对话框
BOOL g_bCreateFlag = FALSE;
std::tr1::shared_ptr<CDialogClusterManage> CDialogClusterManage::pInstance = NULL;
IMPLEMENT_DYNAMIC(CDialogClusterManage, CDialog)

CDialogClusterManage::CDialogClusterManage(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogClusterManage::IDD, pParent)
{
	m_pCluster = NULL;
}

CDialogClusterManage::~CDialogClusterManage()
{
}

void CDialogClusterManage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_GROUP, m_btnClusterSetting);
}


BEGIN_MESSAGE_MAP(CDialogClusterManage, CDialog)
	ON_MESSAGE(WM_CLUSTER_MANAGE_MSG,&CDialogClusterManage::OnSetClusterInfo)
	ON_STN_CLICKED(IDC_STATIC_GROUP, &CDialogClusterManage::OnStnClickedStaticCluster)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CDialogClusterManage 消息处理程序
HWND g_hClusterManage  = NULL;
BOOL CDialogClusterManage::OnInitDialog()
{
	CDialog::OnInitDialog();
	g_hClusterManage = GetSafeHwnd();

	m_btnClusterSetting.SetNormalColor(RGB(0,0,255));
	m_btnClusterSetting.SetLinkColor(RGB(200,100,122));
	return TRUE;
}

std::tr1::shared_ptr<CDialogClusterManage> CDialogClusterManage::Show()
{
	if ( pInstance==NULL)
	{
		pInstance = tr1::shared_ptr<CDialogClusterManage>(new CDialogClusterManage(GetDesktopWindow()));
		pInstance->Create(CDialogClusterManage::IDD,GetDesktopWindow());
		pInstance->ShowWindow(SW_SHOW);
	}
	else
	{
		if ( pInstance->IsWindowVisible()==false )
		{
			pInstance->ShowWindow(SW_SHOW);
		}

		if (pInstance->IsIconic())
		{
			pInstance->ShowWindow(SW_RESTORE);
		}

		pInstance->BringWindowToTop();
	}

	return pInstance;
}

LRESULT CDialogClusterManage::OnSetClusterInfo(WPARAM wParam,LPARAM lParam)
{
	m_pCluster = (CLUSTER_INFO *)lParam;
	m_ClusterInfoClassFlag.nFamilyClass = m_pCluster->nFamilyClass;
	m_ClusterInfoClassFlag.nJoinFlag    = m_pCluster->nJoinFlag;
	CString strTxt = _T("");
	strTxt.Format(_T("恭喜你成功新建群"));
	strTxt.AppendFormat(_T("\r\n"));
	strTxt.AppendFormat(_T("%s (群号: %u)"),m_pCluster->szFamilyName,m_pCluster->nFamilyID);
	((CStatic *)GetDlgItem(IDC_STATIC_CLUSTER))->SetWindowText(strTxt);
	return S_OK;
}


void CDialogClusterManage::OnStnClickedStaticCluster()
{
	// TODO: 在此添加控件通知处理程序代码
	//m_pCluster->nFamilyID = m_pCreateClusterInfo->nCid;
	ASSERT(m_pCluster);
	if(m_pCluster)
	{
		CLUSTER_INFO * pCluster = FindClusterInfo(m_pCluster->nFamilyID);
		ASSERT(pCluster);
		if(pCluster)
		{
			//////////////////将群组信息通过消息传递到各个窗口中显示////////
			////////////////////////////////////////////////////////////////
			//CClusterParamSetting * pDlg = new CClusterParamSetting;
			//pDlg->Show();
			//delete pDlg;
			pCluster->nFamilyClass =  m_ClusterInfoClassFlag.nFamilyClass;
			pCluster->nJoinFlag    =  m_ClusterInfoClassFlag.nJoinFlag;
			g_bCreateFlag  =  TRUE;
			CClusterParamSetting::Show(pCluster);
		}
	}	

	ShowWindow(SW_HIDE);
	//DestroyWindow();
}

HBRUSH CDialogClusterManage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		UINT nId = pWnd->GetDlgCtrlID();

		if ( nId==IDC_EDIT_USERNAME || nId==IDC_EDIT_NICKNAME || nId==IDC_EDIT_EMAIL )
		{

		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr = g_hWhiteBrush;
		}

		if ( nId==IDC_STATIC_GROUP )
		{
			pDC->SetTextColor(RGB(255,0,0));//（255,0，0） //(74,123,211)
		}

	}

	if ( nCtlColor==CTLCOLOR_DLG  )
	{
		hbr = g_hWhiteBrush;
	}

	return hbr;
}