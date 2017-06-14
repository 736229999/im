// im\ArticleBackupDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MyArticles.h"
#include "ArticleBackupDlg.h"
#include ".\articlebackupdlg.h"


// CArticleBackupDlg �Ի���

IMPLEMENT_DYNAMIC(CArticleBackupDlg, CDialog)
CArticleBackupDlg::CArticleBackupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CArticleBackupDlg::IDD, pParent)
{
}

CArticleBackupDlg::~CArticleBackupDlg()
{
}

void CArticleBackupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PATH, m_edtPath);
	DDX_Control(pDX, IDC_EDIT_NAME, m_edtName);
}


BEGIN_MESSAGE_MAP(CArticleBackupDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_PATH, OnBnClickedButtonPath)
END_MESSAGE_MAP()


// CArticleBackupDlg ��Ϣ�������

BOOL CArticleBackupDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString str;
	CTime tm = CTime::GetCurrentTime();
	str.Format("woku.com_%s",tm.Format("%Y_%m_%d"));
	m_edtName.SetWindowText(str);

	return TRUE; 
}

void CArticleBackupDlg::OnBnClickedButtonPath()
{
	BROWSEINFO	bi;
	memset(&bi,0,sizeof(bi));
	char pszDirName[256];//���ѡ���Ŀ¼��
	LPITEMIDLIST pidl;
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = pszDirName;
	bi.lpszTitle = _T("ѡ��Ҫ�����ļ����ڵ�Ŀ¼");
	bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	char szCurDir[255];
	::GetCurrentDirectory(255,szCurDir);
	if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)//����ѡ��Ŀ¼�Ի���
	{
		if (::SHGetPathFromIDList(pidl, pszDirName))//�����ѡ���Ŀ¼
		{ 
			m_edtPath.SetWindowText(pszDirName);	
		}
	}
	::SetCurrentDirectory(szCurDir);
}
