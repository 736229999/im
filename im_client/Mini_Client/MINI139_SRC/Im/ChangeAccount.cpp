// im\ChangeAccount.cpp : 实现文件
//

#include "stdafx.h"
#include ".\changeaccount.h"
#include "impublic.h"
#include "WebProcess.h"

// CChangeAccountDlg 对话框

IMPLEMENT_DYNAMIC(CChangeAccountDlg, CXSkinDlg)
CChangeAccountDlg::CChangeAccountDlg(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CChangeAccountDlg::IDD, pParent)
{
	Create(CChangeAccountDlg::IDD,pParent);
}

CChangeAccountDlg::~CChangeAccountDlg()
{

}

void CChangeAccountDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_exp);
}


BEGIN_MESSAGE_MAP(CChangeAccountDlg, CXSkinDlg)
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_WM_SIZING()
END_MESSAGE_MAP()


// CChangeAccountDlg 消息处理程序

void CChangeAccountDlg::OnDestroy()
{
	CXSkinDlg::OnDestroy();
	delete this;
}


void CChangeAccountDlg::NewWnd()
{
	CChangeAccountDlg * pInstance = new CChangeAccountDlg(GetDesktopWindow());
	pInstance->ShowWindow(SW_SHOW);
	pInstance->BringWindowToTop();
}

BOOL CChangeAccountDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetDlgStyle(ALL);
	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	CRect rct;
	GetWindowRect(&rct);
	ScreenToClient(&rct);
	rct.InflateRect(-10,-30,-10,-10);
	m_exp.MoveWindow(&rct);
	SetWindowText("更改用户名");

	if(g_pWebConfig)
	{
		CString sFixUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(FIX_USERNAME,L"");
		CString sUrl = GetMiniUrl(sFixUrl);
		m_exp.Navigate(sUrl,NULL,NULL,NULL,NULL);
	}


	return TRUE;  

}

void CChangeAccountDlg::OnPaint()
{
	CPaintDC dc(this); 
	CXSkinDlg::OnPaint();
}

void CChangeAccountDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CXSkinDlg::OnSizing(fwSide, pRect);
	CRect rct;
	GetWindowRect(&rct);
	ScreenToClient(&rct);
	rct.InflateRect(-10,-30,-10,-10);
	m_exp.MoveWindow(&rct);
}
