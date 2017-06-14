// im\ClusterPage3.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "ClusterPage3.h"
#include ".\clusterpage3.h"


// CClusterPage3 对话框

IMPLEMENT_DYNAMIC(CClusterPage3, CWizardPage)
CClusterPage3::CClusterPage3(CWnd* pParent /*=NULL*/)
	: CWizardPage(CClusterPage3::IDD, pParent)
{
}

CClusterPage3::~CClusterPage3()
{
}

void CClusterPage3::DoDataExchange(CDataExchange* pDX)
{
	CWizardPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CClusterPage3, CWizardPage)
END_MESSAGE_MAP()


// CClusterPage3 消息处理程序

BOOL CClusterPage3::OnInitDialog()
{
	CWizardPage::OnInitDialog();

	((CButton *)GetDlgItem(IDC_RADIO1))->SetCheck(1);

	return TRUE;   
}

bool CClusterPage3::GetClusterInfo(CLUSTER_INFO &info)
{
	CButton *pR1 = (CButton *)GetDlgItem(IDC_RADIO1);
	CButton *pR2 = (CButton *)GetDlgItem(IDC_RADIO2);
	CButton *pR3 = (CButton *)GetDlgItem(IDC_RADIO3);

	info.nJoinFlag=0;

	if ( pR2->GetCheck() )
		info.nJoinFlag=1;
	else
	if ( pR3->GetCheck() )
		info.nJoinFlag=2;

	return true;
}
