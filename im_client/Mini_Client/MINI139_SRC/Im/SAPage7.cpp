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


// CSAPage7 消息处理程序

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
	char pszDirName[256];//存放选择的目录名
	LPITEMIDLIST pidl;
	bi.hwndOwner = GetSafeHwnd();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = pszDirName;
	bi.lpszTitle = _T("选择要备份文件所在的目录");
	bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	char szCurDir[255];
	::GetCurrentDirectory(255,szCurDir);
	if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)//调用选择目录对话框
	{
		if (::SHGetPathFromIDList(pidl, pszDirName))//获得所选择的目录
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


			//测试一下该目录是否可写
			CString strTestFile;
			strTestFile.Format("%sMini_图片",pszDirName);

			::RemoveDirectory(strTestFile);

			if( 0 == ::CreateDirectory(strTestFile,NULL) )
			{
				CMessageBox::Model(GetSafeHwnd(),"该目录不可写，请重新选择一个可存放的目录");
				return;
			}

			::RemoveDirectory(strTestFile);

			m_edtPath.SetWindowText(pszDirName);	
		}
	}
	::SetCurrentDirectory(szCurDir);


}
