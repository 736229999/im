// SAPage3.cpp : 实现文件
//

#include "stdafx.h"
#include "SAPage3.h"
#include ".\sapage3.h"


// CSAPage3 对话框

IMPLEMENT_DYNAMIC(CSAPage3, CSABasePage)
CSAPage3::CSAPage3(CWnd* pParent /*=NULL*/)
	: CSABasePage(CSAPage3::IDD, pParent)
{
	Create(CSAPage3::IDD,pParent);
}

CSAPage3::~CSAPage3()
{
}

void CSAPage3::DoDataExchange(CDataExchange* pDX)
{
	CSABasePage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_PASS1, m_edtPass1);
	DDX_Control(pDX, IDC_EDIT_PASS2, m_edtPass2);
	DDX_Control(pDX, IDC_EDIT_PASS3, m_edtPass3);
}


BEGIN_MESSAGE_MAP(CSAPage3, CSABasePage)
	ON_BN_CLICKED(IDC_CHECK_PSWD, OnBnClickedCheckPswd)
	ON_EN_CHANGE(IDC_EDIT_PASS2, OnEnChangeEditPass2)
	ON_EN_CHANGE(IDC_EDIT_PASS3, OnEnChangeEditPass3)
END_MESSAGE_MAP()


void CSAPage3::SetUserInfo(const XT_USER_INFO &info)
{
	CButton *pRadio1 = (CButton *)GetDlgItem(IDC_RADIO1);
	pRadio1->SetCheck(false);
	CButton *pRadio2 = (CButton *)GetDlgItem(IDC_RADIO2);
	pRadio2->SetCheck(false);
	CButton *pRadio3 = (CButton *)GetDlgItem(IDC_RADIO3);
	pRadio3->SetCheck(false);
	CButton * pCheck = (CButton *)GetDlgItem(IDC_CHECK_FAMILY_FLAG);
	pCheck->SetCheck(info.familyflag);

	switch ( info.rule )
	{
	case 1:
		pRadio2->SetCheck(1);
		break;
	case 2:
		pRadio3->SetCheck(1);
		break;
	default:
		pRadio1->SetCheck(1);
	}
}

void CSAPage3::ResetModifyFlag()
{
	m_edtPass1.SetWindowText("");
	m_edtPass2.SetWindowText("");
	m_edtPass3.SetWindowText("");

	CSABasePage::ResetModifyFlag();
}

int  CSAPage3::GetUserInfo(XT_USER_INFO &info)
{
	CButton *pRadio1 = (CButton *)GetDlgItem(IDC_RADIO1);
	CButton *pRadio2 = (CButton *)GetDlgItem(IDC_RADIO2);
	CButton *pRadio3 = (CButton *)GetDlgItem(IDC_RADIO3);
	CButton * pCheck = (CButton *)GetDlgItem(IDC_CHECK_FAMILY_FLAG);

	CString pass1,pass2,pass3;

	GetDlgItem(IDC_EDIT_PASS1)->GetWindowText(pass1);//旧密码
	GetDlgItem(IDC_EDIT_PASS2)->GetWindowText(pass2);//新密码,
	GetDlgItem(IDC_EDIT_PASS3)->GetWindowText(pass3);//新密码确认

	bool bCheckPwd = ((CButton *)GetDlgItem(IDC_CHECK_PSWD))->GetCheck();

	if ( bCheckPwd )
	{
		if (pass1!=g_ImUserInfo.GetPassword().c_str())
		{
			CMessageBox::Prompt("旧密码错误!");
			GetDlgItem(IDC_EDIT_PASS1)->SetFocus();
			return 0;
		}

		if (pass2.Find(' ',0)>-1 )
		{
			CMessageBox::Prompt("密码中不能包含空格！");
			GetDlgItem(IDC_EDIT_PASS2)->SetFocus();
			return 0;
		}

		if (pass2.GetLength()<6 )
		{
			//MessageBox("密码长度太短了!至少6个字符.",IM_CAPTION,MB_OK|MB_ICONWARNING);
			CMessageBox::Prompt("密码长度太短了!至少6个字符.");
			GetDlgItem(IDC_EDIT_PASS2)->SetFocus();
			return 0;
		}

		if (pass3 != pass2 )
		{
			//MessageBox("新密码有误！",IM_CAPTION,MB_OK|MB_ICONWARNING);
			CMessageBox::Prompt("新密码有误！");
			GetDlgItem(IDC_EDIT_PASS1)->SetFocus();
			return 0;
		}

		info.SetPassword((LPCSTR)pass2);
	}
	else
	{
		info.SetPassword(g_ImUserInfo.GetPassword().c_str());
	}

	if ( pRadio1->GetCheck() )
		info.rule = 0;//允许

	if ( pRadio2->GetCheck() )
		info.rule = 1;//需要验证

	if ( pRadio3->GetCheck() )
		info.rule = 2;//拒绝任何人
	
	if( pCheck->GetCheck())
	{
		// 拒绝
		info.familyflag = 1;
	}
	else
	{
		// 接受
		info.familyflag = 0;
	}

	//this->m_bModified = false;

	return 1;
}

void CSAPage3::OnBnClickedCheckPswd()
{
	bool bCheck = ((CButton *)GetDlgItem(IDC_CHECK_PSWD))->GetCheck();

	GetDlgItem(IDC_EDIT_PASS1)->EnableWindow(bCheck);
	GetDlgItem(IDC_EDIT_PASS2)->EnableWindow(bCheck);
	GetDlgItem(IDC_EDIT_PASS3)->EnableWindow(bCheck);
	
	m_edtPass1.SetWindowText("");
	m_edtPass2.SetWindowText("");
	m_edtPass3.SetWindowText("");
}

BOOL CSAPage3::OnInitDialog()
{
	CSABasePage::OnInitDialog();

	m_edtPass1.LimitText(15);
	m_edtPass2.LimitText(15);
	m_edtPass3.LimitText(15);

	((CButton *)GetDlgItem(IDC_CHECK_PSWD))->SetCheck(0);

	return TRUE; 
}

void ProcessEditPass(CEdit *pEditPass)
{

	CString s;
	pEditPass->GetWindowText(s);

	LPCSTR pstr = (LPCSTR)s;

	int l = strlen(pstr);

	bool bChinese=false;

	for ( int i=0; i<l; i++ )
	{
		int nChar = (int)pstr[i];
		if ( nChar<0 )
		{
			bChinese=true;
			break;
		}
	}


	if ( bChinese )
		pEditPass->SetWindowText("");
}

void CSAPage3::OnEnChangeEditPass2()
{
	ProcessEditPass(&m_edtPass2);
}

void CSAPage3::OnEnChangeEditPass3()
{
	ProcessEditPass(&m_edtPass3);
}
