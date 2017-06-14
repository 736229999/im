// im\CreateDirDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "WriteArticle.h"
#include "CreateDirDlg.h"
#include "WebDataProcess.h"
#include "WebInterfaceWrapper.h"


// CCreateDirDlg 对话框

IMPLEMENT_DYNAMIC(CCreateDirDlg, CDialog)
CCreateDirDlg::CCreateDirDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCreateDirDlg::IDD, pParent)
{
	m_pBookList = NULL;
}

CCreateDirDlg::~CCreateDirDlg()
{
}

void CCreateDirDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOk);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CCreateDirDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CCreateDirDlg 消息处理程序

BOOL CCreateDirDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CListBox *pBox = (CListBox *)GetDlgItem(IDC_LIST_DIR);

	for ( int i=0; i<m_pBookList->size(); i++ )
	{
		pBox->AddString( (*m_pBookList)[i].szBookName );
	}

	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_DIR);
	pEdit->LimitText(20);

	return TRUE;  
}

void CCreateDirDlg::OnBnClickedOk()
{
	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_DIR);

	CString strDir;
	pEdit->GetWindowText(strDir);

	strDir.Trim();
	if ( strDir=="" )
	{
		ShowMessage("请输入文集名。",MB_OK|MB_ICONINFORMATION);
		pEdit->SetFocus();
		return ;
	}

	CWaitCursor			 w;
	CWebInterfaceWrapper objWI;
	CString              strError;

	objWI.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());
	long nCreateBookID = objWI.CreateBook(0,0,strDir,3,strError);

	if( nCreateBookID!=0 )
	{
		this->m_biCreated.nBookId = nCreateBookID;
		strncpy(m_biCreated.szBookName,strDir,sizeof(m_biCreated.szBookName)-1);
		ShowMessage("文集创建成功！");
		CDialog::OnOK();
	}
	else
	{
		ShowMessage("创建文集失败！请多试几次看看。");
	}		
}
