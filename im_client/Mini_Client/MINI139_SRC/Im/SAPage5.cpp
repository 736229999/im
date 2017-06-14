#include "StdAfx.h"
#include ".\sapage5.h"

#include "LocalData.h"


IMPLEMENT_DYNAMIC(CSAPage5, CSABasePage)
CSAPage5::CSAPage5(CWnd* pParent /*=NULL*/)
	: CSABasePage(CSAPage5::IDD, pParent)
{
	Create(CSAPage5::IDD,pParent);
}

CSAPage5::~CSAPage5()
{
}

void CSAPage5::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT_PATH,m_edtPath);
	DDX_Control(pDX,IDC_BUTTON_PATH,m_btnPath);
}


BEGIN_MESSAGE_MAP(CSAPage5, CSABasePage)
	ON_BN_CLICKED(IDC_BUTTON_PATH, OnBnClickedButtonPath)
END_MESSAGE_MAP()


// CSAPage5 ��Ϣ�������

BOOL CSAPage5::OnInitDialog()
{
	CSABasePage::OnInitDialog();

	FILE_SETTING setting;

	g_LocalData.GetUserRecvFileSetting(setting);

	m_edtPath.SetWindowText(setting.szRecvFilePath);

	return TRUE; 
}


int CSAPage5::SaveUserRecvFolder()
{
	FILE_SETTING setting;
	m_edtPath.GetWindowText(setting.szRecvFilePath,sizeof(setting.szRecvFilePath)-1);
	g_LocalData.SaveUserRecvFileSetting(setting);
	return 1;
}

void CSAPage5::OnBnClickedButtonPath()
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
