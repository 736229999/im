// im\ClusterPage1.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "ClusterPage1.h"
#include ".\clusterpage1.h"
#include "WebProcess.h"


// CClusterPage1 对话框

IMPLEMENT_DYNAMIC(CClusterPage1, CWizardPage)
CClusterPage1::CClusterPage1(CWnd* pParent /*=NULL*/)
	: CWizardPage(CClusterPage1::IDD, pParent)
{
}

CClusterPage1::~CClusterPage1()
{
}

void CClusterPage1::DoDataExchange(CDataExchange* pDX)
{
	CWizardPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NAME, m_edtName);
	DDX_Control(pDX, IDC_EDIT_INTRO, m_edtIntro);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_Control(pDX, IDC_RADIO_ALL, m_btnRadioAll);
	DDX_Control(pDX, IDC_RADIO_VERIFY, m_btnRadioVerify);
	DDX_Control(pDX, IDC_RADIO_NONE, m_btnRadioNone);

	//DDX_Control(pDX, IDC_EDIT_DOMAIN, m_edtDomain);
	//DDX_Control(pDX, IDC_STATIC_DOMAIN, m_btnDomain);
}


BEGIN_MESSAGE_MAP(CClusterPage1, CWizardPage)
//	ON_EN_CHANGE(IDC_EDIT_DOMAIN, OnEnChangeEditDomain)
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_NAME, &CClusterPage1::OnChangeEditName)
	ON_EN_CHANGE(IDC_EDIT_INTRO, &CClusterPage1::OnChangeEditIntro)
END_MESSAGE_MAP()


// CClusterPage1 消息处理程序

BOOL CClusterPage1::OnInitDialog()
{
	CWizardPage::OnInitDialog();

	//ComboInitFamilyClass(&m_comboType);
	InitClusterType(&m_comboType);///群分类

	//m_edtName.LimitText(20);//20字
	//m_edtIntro.LimitText(50);//50字
	m_edtIntro.SetWindowText("请输入不超过50个字的群简介,该信息完全公开！");

	m_edtName.SetBorderColor(RGB(126,157,185));
	m_edtIntro.SetBorderColor(RGB(126,157,185));
	//m_edtDomain.SetBorderColor(RGB(126,157,185));

	m_comboType.SetCurSel(0);
	m_comboType.SetBorderColor(RGB(126,157,185));

	//page 3 移到page1
	((CButton *)GetDlgItem(IDC_RADIO_VERIFY))->SetCheck(TRUE);

	SetTimer(0X01,500,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

bool CClusterPage1::GetClusterInfo(CLUSTER_INFO &info)
{
	CString str;
	m_edtName.GetWindowText(str);
	str.Trim();

	// 去掉全角空格
	TrimSpace2(str);

	if ( str=="" )
	{
		//MessageBox("群名称不能为空！",IM_CAPTION,MB_OK|MB_ICONWARNING);
		CMessageBox::Prompt("群名称不能为空！");
		m_edtName.SetFocus();
		return false;
	}
/*
	m_edtDomain.GetWindowText(str);

	str.Trim();

	if ( str=="" )
	{
		//MessageBox("群域名不能空！",IM_CAPTION,MB_OK|MB_ICONWARNING);
		CMessageBox::Prompt("群域名不能空！");
		m_edtDomain.SetFocus();
		return false;
	}

	int l=str.GetLength();
	str.MakeLower();


	if ( l<3 )
	{
		//MessageBox("群域名长度太短了，至少3个字母！",IM_CAPTION,MB_OK|MB_ICONERROR);
		CMessageBox::Prompt("群域名长度太短了，至少3个字母！");
		m_edtDomain.SetFocus();
		return false;
	}

	for ( int i=0; i<l; i++ )
	{
		char c=str[i];
		if ( ( c<='z' && c>='a' ) ||
			 ( c<='9' && c>='0' )  )
		{
		}
		else
		{
			//MessageBox("群域名只能是字母和数字的结合 如 :name123",IM_CAPTION,MB_OK|MB_ICONERROR);
			ShowMessage("群域名只能是字母和数字的结合 如 :name123");
			m_edtDomain.SetFocus();
			m_edtDomain.SetSel(0,-1);
			return false;
		}
	}
*/

	CString strName, strIntro, strDomain;
	m_edtName.GetWindowText(strName);
	m_edtIntro.GetWindowText(strIntro);
	//m_edtDomain.GetWindowText(strDomain);


	strName.Trim();
	strIntro.Trim();
	strDomain.Trim();
	strDomain.MakeLower();

	strcpy(info.szFamilyName,strName);
	strcpy(info.szFamilyDesc,strIntro);
	strcpy(info.szFamilyDomain,strDomain);
//	sprintf(info.bulitin,strBulitin);
//
	info.nFamilyClass = ComboGetFamilyClass(&m_comboType);
		info.nFamilyClass = ComboGetFamilyClass(&m_comboType);

	//page3移到page1
	CButton *pR1 = (CButton *)GetDlgItem(IDC_RADIO_ALL);
	CButton *pR2 = (CButton *)GetDlgItem(IDC_RADIO_VERIFY);
	CButton *pR3 = (CButton *)GetDlgItem(IDC_RADIO_NONE);

	info.nJoinFlag=0;

	if ( pR2->GetCheck() )
		info.nJoinFlag=1;
	else
	if ( pR3->GetCheck() )
		info.nJoinFlag=2;

	return true;
}
/*
void CClusterPage1::OnEnChangeEditDomain()
{
	CString str;
	m_edtDomain.GetWindowText(str);

	CString str2= str.MakeLower();
	str2.Trim();
	int l = str2.GetLength();

	for ( int i=0; i<l; i++ )
	{
		char c=str2[i];
		if ( ( c<='z' && c>='a' ) ||
			 ( c<='9' && c>='0' ) ||
			 (c=='_' ) )
		{
		}
		else
		{
			ShowMessage("群域名不能包含下划线,而且只能是字母和数字的结合 如 :name123.",MB_ICONERROR|MB_OK );
			break;
		}
	}

	if(g_pWebConfig)
	{
		CString strDomain = (LPCSTR)g_pWebConfig->GetWebPageUrl(FAMILY_HOME,str.AllocSysString());
		m_btnDomain.SetWindowText(strDomain);
	}


}
*/


void CClusterPage1::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strTxt = _T("");
	if ( nIDEvent==0X01 )
	{
		m_edtName.GetWindowText(strTxt);
		if ( strTxt=="" )
		{
			//m_pHintPass = new CHintDlg2(IDB_LOGIN_PASSHINT,rc.left,rc.top,this);
			m_edtName.SetWindowText("不超过20字");
			m_edtName.SetSel(0,-1);				
		}			

		/*群简介可为空
		m_edtIntro.GetWindowText(strTxt);
		if ( strTxt=="" )
		{
			//m_pHintPass = new CHintDlg2(IDB_LOGIN_PASSHINT,rc.left,rc.top,this);
			m_edtIntro.SetWindowText("请输入不超过50个字的群简介,该信息完全公开！");
			m_edtIntro.SetSel(0,-1);				
		}		
		*/
	}
	CWizardPage::OnTimer(nIDEvent);
}


int  CClusterPage1::GetClusterRadioBtnChecked()
{
	int nAuth =0;
	if ( m_btnRadioAll.GetCheck() )
	{
		nAuth=0;
	}
	else  if ( m_btnRadioVerify.GetCheck() )
	{
		nAuth=1;
	}
	else  if ( m_btnRadioNone.GetCheck() )
	{
		nAuth=2;
	}

	return nAuth;
}

void CClusterPage1::OnChangeEditName()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CWizardPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	
	// TODO:  在此添加控件通知处理程序代码
	CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_NAME);
	DWORD dwSel = pEdit->GetSel();//获取当前光标位置
	int maxSize =20;
	CString strName,allName;
	CString tmp;
	pEdit->GetWindowText(strName);	
	int iLen =0;//文字个数
	for (int i=0;strName[i];i++) {		
		if (strName[i] < 0){  			   
			tmp.Format("%c%c",strName[i],strName[i+1]);
			allName = allName +tmp;
			i++;   			
		}
		else{
			tmp.Format("%c",strName[i]);
			allName = allName +tmp;
		}
		iLen++;
		if (iLen>=maxSize){
			if (strName != allName)
			{
				pEdit->SetWindowText(allName);
				pEdit->SetSel(dwSel);//如果光标在文本结尾则无法回到文本开头
			}
			break;
		}
	}	
}


void CClusterPage1::OnChangeEditIntro()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CWizardPage::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
	CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_INTRO);
	DWORD dwSel = pEdit->GetSel();//获取当前光标位置
	int maxSize =50;
	CString strName,allName;
	CString tmp;
	pEdit->GetWindowText(strName);	
	int iLen =0;//文字个数
	for (int i=0;strName[i];i++) {		
		if (strName[i] < 0){  			   
			tmp.Format("%c%c",strName[i],strName[i+1]);
			allName = allName +tmp;
			i++;   			
		}
		else{
			tmp.Format("%c",strName[i]);
			allName = allName +tmp;
		}
		iLen++;
		if (iLen>=maxSize){
			if (strName != allName)
			{
				pEdit->SetWindowText(allName);
				pEdit->SetSel(dwSel);//如果光标在文本结尾则无法回到文本开头
			}
			break;
		}
	}	
}
