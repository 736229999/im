// im\testWebPicDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteArticle.h"
#include ".\testWebPicDlg.h"

// CTestWebPicDlg 对话框

IMPLEMENT_DYNAMIC(CTestWebPicDlg, CXSkinDlg)
CTestWebPicDlg::CTestWebPicDlg(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CTestWebPicDlg::IDD, pParent)
	, m_strUrl(_T(""))
{
}

CTestWebPicDlg::~CTestWebPicDlg()
{
}

void CTestWebPicDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER1, m_explore1);
	DDX_Text(pDX,IDC_EDIT_URL,m_strUrl);
}


BEGIN_MESSAGE_MAP(CTestWebPicDlg, CXSkinDlg)
	ON_BN_CLICKED(IDTEST, OnBnClickedTest)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CTestWebPicDlg 消息处理程序

void CTestWebPicDlg::OnBnClickedTest()
{
	UpdateData();
	m_strUrl.Trim();

	if ( m_strUrl!="" )
		m_explore1.Navigate(m_strUrl,NULL,NULL,NULL,NULL);
}

BOOL CTestWebPicDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	m_explore1.Navigate(g_spaceData.GetPath()+"Data\\default.htm",NULL,NULL,NULL,NULL);
	
	m_edtUrl.SubclassWindow( GetDlgItem(IDC_EDIT_URL)->m_hWnd);
	m_btn1.SubclassWindow( GetDlgItem(IDTEST)->m_hWnd);
	m_btn2.SubclassWindow( GetDlgItem(IDOK)->m_hWnd);
	m_btn3.SubclassWindow( GetDlgItem(IDCANCEL)->m_hWnd);

	SetWindowText("插入网络图片");
	SetDlgStyle(CLOSE);

	return TRUE;  
}

CString CTestWebPicDlg::GetPictureUrl()
{
	return m_strUrl;
}

HBRUSH CTestWebPicDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetTextColor(RGB(0,84,166));
		pDC->SetBkMode(TRANSPARENT);
		hbr = this->m_hBkGround;
	}

	return hbr;
}
