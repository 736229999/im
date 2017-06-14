#include "StdAfx.h"
#include ".\sapage7.h"

#include "LocalData.h"


IMPLEMENT_DYNAMIC(CSAPage7, CSABasePage)
CSAPage7::CSAPage7(CWnd* pParent /*=NULL*/)
	: CSABasePage(CSAPage7::IDD, pParent)
{
	Create(CSAPage7::IDD,pParent);
}

CSAPage7::~CSAPage7()
{
}

void CSAPage7::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT_PATH,m_edtPath);
	DDX_Control(pDX,IDC_BUTTON_PATH,m_btnPath);
}


BEGIN_MESSAGE_MAP(CSAPage7, CSABasePage)
	ON_BN_CLICKED(IDC_BUTTON_PATH, OnBnClickedButtonPath)
END_MESSAGE_MAP()


// CSAPage7 ��Ϣ�������

BOOL CSAPage7::OnInitDialog()
{
	CSABasePage::OnInitDialog();

	ZONE_SETTING setting;
	g_LocalData.GetZoneSetting(setting);
	m_edtPath.SetWindowText(setting.szBackupPath);

	return TRUE; 
}


int CSAPage7::SaveZoneSetting()
{
	ZONE_SETTING setting;
	m_edtPath.GetWindowText(setting.szBackupPath,sizeof(setting.szBackupPath));
	g_LocalData.SetZoneSetting(setting);

	return 1;
}

void CSAPage7::OnBnClickedButtonPath()
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
			int nLength = strlen(pszDirName);
			if( nLength > sizeof(pszDirName))
			{
				return;
			}

			if( pszDirName[nLength-1] != '\\' )
			{
				strcat(pszDirName,"\\");
			}


			//����һ�¸�Ŀ¼�Ƿ��д
			CString strTestFile;
			strTestFile.Format("%sMini_ͼƬ",pszDirName);

			::RemoveDirectory(strTestFile);

			if( 0 == ::CreateDirectory(strTestFile,NULL) )
			{
				CMessageBox::Model(GetSafeHwnd(),"��Ŀ¼����д��������ѡ��һ���ɴ�ŵ�Ŀ¼");
				return;
			}

			::RemoveDirectory(strTestFile);

			m_edtPath.SetWindowText(pszDirName);	
		}
	}
	::SetCurrentDirectory(szCurDir);


}
