// im\AboutBoxDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\aboutboxdlg.h"


// CAboutBoxDlg �Ի���
CAboutBoxDlg * CAboutBoxDlg::pInstance = NULL;

IMPLEMENT_DYNAMIC(CAboutBoxDlg, CDialog)
CAboutBoxDlg::CAboutBoxDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutBoxDlg::IDD, pParent)
{
	Create(CAboutBoxDlg::IDD,pParent);
}

CAboutBoxDlg::~CAboutBoxDlg()
{
	pInstance = NULL;
}

void CAboutBoxDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_STATIC_LINK,m_btn1);
}


BEGIN_MESSAGE_MAP(CAboutBoxDlg, CDialog)
	ON_STN_CLICKED(IDC_STATIC_LINK, OnStnClickedStaticLink)
	ON_WM_NCDESTROY()
	ON_WM_CLOSE()
	ON_WM_KILLFOCUS()
	ON_BN_CLICKED(IDOK, &CAboutBoxDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CAboutBoxDlg ��Ϣ�������

void CAboutBoxDlg::Show()
{
	if ( pInstance==NULL )
	{
		pInstance = new CAboutBoxDlg(GetDesktopWindow());
		pInstance->ShowWindow(SW_SHOW);
	}
	else
	{
		pInstance->BringWindowToTop();
	}
}

void CAboutBoxDlg::Close()
{
	if ( pInstance )
		pInstance->DestroyWindow();
}

void CAboutBoxDlg::OnStnClickedStaticLink()
{
	OpenHtmlPage("http://www.feiniu.com");
}

void CAboutBoxDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	delete this;
}

void CAboutBoxDlg::OnClose()
{
	DestroyWindow();
}

void CAboutBoxDlg::OnKillFocus(CWnd* pNewWnd)
{
	
}

BOOL CAboutBoxDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString strVer;
	strVer.Format("���� V %X.%s",CIMProtocol::s_nCurVer,g_pProtocol->m_pUdpComm->GetVersion());
	SetDlgItemText(IDC_STATIC_VER,strVer);

	return TRUE;  
}

void CAboutBoxDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
