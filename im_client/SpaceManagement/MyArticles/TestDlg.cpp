// TestDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyArticles.h"
#include "TestDlg.h"
#include ".\testdlg.h"


// CTestDlg �Ի���

IMPLEMENT_DYNAMIC(CTestDlg, CDialog)
CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
}

CTestDlg::~CTestDlg()
{
}

void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CTestDlg ��Ϣ�������

void CTestDlg::OnBnClickedOk()
{
	
	BSTR bstr;
	g_pUserInfo->get_NickName(&bstr);
	
	USES_CONVERSION;
	MessageBox(W2A(bstr));
	OnOK();
}
