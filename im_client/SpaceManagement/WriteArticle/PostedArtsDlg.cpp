// PostedArtsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WriteArticle.h"
#include "PostedArtsDlg.h"
#include ".\postedartsdlg.h"


// CPostedArtsDlg �Ի���

IMPLEMENT_DYNAMIC(CPostedArtsDlg, CDialog)
CPostedArtsDlg::CPostedArtsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPostedArtsDlg::IDD, pParent)
{
}

CPostedArtsDlg::~CPostedArtsDlg()
{
}

void CPostedArtsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_ARTCOUNT, m_listCount);
}


BEGIN_MESSAGE_MAP(CPostedArtsDlg, CDialog)
	//ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()


// CPostedArtsDlg ��Ϣ�������

LRESULT CPostedArtsDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message==WM_COMMAND)
	{

		GetParent()->PostMessage(WM_COMMAND,wParam,lParam);
	}

	return CDialog::WindowProc(message, wParam, lParam);

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CPostedArtsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect  rcClient;
	GetClientRect(&rcClient);
	m_ListArticles.Create(this,CRect(10,42,rcClient.Width()-10,rcClient.Height()-50));

	m_listCount.AddString("10");
	m_listCount.AddString("15");
	m_listCount.AddString("20");
	m_listCount.AddString("25");
	m_listCount.AddString("ȫ�� ");

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
