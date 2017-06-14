// UploadPicShellExtDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HmgUploadPicture.h"
#include "UploadPicShellExtDlg.h"
#include ".\uploadpicshellextdlg.h"


// CUploadPicShellExtDlg 对话框

IMPLEMENT_DYNAMIC(CUploadPicShellExtDlg, CDialog)
CUploadPicShellExtDlg::CUploadPicShellExtDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CUploadPicShellExtDlg::IDD, pParent)
{
}

CUploadPicShellExtDlg::~CUploadPicShellExtDlg()
{
}

void CUploadPicShellExtDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_DLG_UPLOADPICSHELLEXT_UPFILELISTCTRL, m_wndUpFileListCtrl);
}


BEGIN_MESSAGE_MAP(CUploadPicShellExtDlg, CDialog)
END_MESSAGE_MAP()


// CUploadPicShellExtDlg 消息处理程序

BOOL CUploadPicShellExtDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_strWindowText.Format(_T("上传到Woku个人空间中..."));
	SetWindowText(m_strWindowText);
	


	ShowUpFileNumInfo();
	return TRUE; 
}

void CUploadPicShellExtDlg::ShowUpFileNumInfo()
{
	CString strInfo;
	
	strInfo.Format(_T("当前已选文件数:%d 大小:%d"), m_wndUpFileListCtrl.GetItemCount(), 0);

	//SetDlgItemText(IDC_DLG_UPLOADPICSHELLEXT_STATIC_FILENUMINFO, strInfo);
}

void CUploadPicShellExtDlg::SetParamInfo(const CString & strParamInfo)
{
}