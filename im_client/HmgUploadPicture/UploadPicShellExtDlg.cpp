// UploadPicShellExtDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HmgUploadPicture.h"
#include "UploadPicShellExtDlg.h"
#include ".\uploadpicshellextdlg.h"


// CUploadPicShellExtDlg �Ի���

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


// CUploadPicShellExtDlg ��Ϣ�������

BOOL CUploadPicShellExtDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_strWindowText.Format(_T("�ϴ���Woku���˿ռ���..."));
	SetWindowText(m_strWindowText);
	


	ShowUpFileNumInfo();
	return TRUE; 
}

void CUploadPicShellExtDlg::ShowUpFileNumInfo()
{
	CString strInfo;
	
	strInfo.Format(_T("��ǰ��ѡ�ļ���:%d ��С:%d"), m_wndUpFileListCtrl.GetItemCount(), 0);

	//SetDlgItemText(IDC_DLG_UPLOADPICSHELLEXT_STATIC_FILENUMINFO, strInfo);
}

void CUploadPicShellExtDlg::SetParamInfo(const CString & strParamInfo)
{
}