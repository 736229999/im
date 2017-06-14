// SAPage2.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SAPage2.h"
#include ".\sapage2.h"


// CSAPage2 �Ի���

IMPLEMENT_DYNAMIC(CSAPage2, CSABasePage)
CSAPage2::CSAPage2(CWnd* pParent /*=NULL*/)
	: CSABasePage(CSAPage2::IDD, pParent)
{
	Create(CSAPage2::IDD,pParent);
}

CSAPage2::~CSAPage2()
{
}

void CSAPage2::DoDataExchange(CDataExchange* pDX)
{
	CSABasePage::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT_MOBILE,m_editMobile);
	DDX_Control(pDX,IDC_EDIT_EMAIL,m_editEmail);
	DDX_Control(pDX,IDC_EDIT_ADDRESS,m_editAddress);
	DDX_Control(pDX,IDC_STATIC_BIND,m_btnBind);
}


BEGIN_MESSAGE_MAP(CSAPage2, CSABasePage)
	ON_STN_CLICKED(IDC_STATIC_BIND, OnStnClickedStaticBind)
	ON_BN_CLICKED(IDC_RADIO_PUBLIC, &CSAPage2::OnBnClickedRadioPublic)
END_MESSAGE_MAP()


// CSAPage2 ��Ϣ�������

BOOL CSAPage2::OnInitDialog()
{
	CSABasePage::OnInitDialog();

	m_editAddress.LimitText(39);
	m_editEmail.LimitText(59);
	m_editMobile.LimitText(15);
	m_btnBind.SetNormalColor(RGB(0,0,255));

	if ( g_ImUserInfo.bind_mobile )
	{
		//m_btnBind.ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_B2)->ShowWindow(SW_HIDE);
		SetDlgItemText(IDC_STATIC_B1,"����ֻ��Ѿ���");
	}

	return TRUE; 
}

void CSAPage2::SetUserInfo(const XT_USER_INFO &info)
{
	m_editMobile.SetWindowText(info.mobile);
	m_editAddress.SetWindowText(info.address);
	m_editEmail.SetWindowText(info.email);
	CButton *pRadio1 = (CButton *)GetDlgItem(IDC_RADIO_PUBLIC);
	pRadio1->SetCheck(false);
	CButton *pRadio2 = (CButton *)GetDlgItem(IDC_RADIO_PRIVATE);
	pRadio2->SetCheck(false);
	CButton *pRadio3 = (CButton *)GetDlgItem(IDC_RADIO_FRIEND);
	pRadio3->SetCheck(false);
	// ˽��
	if(info.linkpop == 0)
	{
		pRadio2->SetCheck(true);
	}
	// ���ѿɼ�
	else if(info.linkpop == 1)
	{
		pRadio3->SetCheck(true);
	}
	// ����
	else
	{
		pRadio1->SetCheck(true);
	}
}

int  CSAPage2::GetUserInfo(XT_USER_INFO &info)
{
	if(!IsModified())
		return 1;
	CString strEmail,error,sAddress,sMobile;

	m_editEmail.GetWindowText(strEmail);
	strEmail.Trim();
	/*if ( strEmail=="" )
	{
		ShowMessage("Email��ַ��ʽ����ȷ!");
		m_editEmail.SetFocus();
		return 0;
	}
	else */
	if ( !CheckEmail(strEmail,error) )
	{
		ShowMessage(error);
		m_editEmail.SetFocus();
		return 0;
	}
	
	m_editMobile.GetWindowText(sMobile);
	m_editAddress.GetWindowText(sAddress);
	
	strncpy(info.address,sAddress,sizeof(info.address)-1);
	strncpy(info.email,strEmail,sizeof(info.email)-1);
	strncpy(info.mobile,sMobile,sizeof(info.mobile)-1);
	
	CButton *pRadioPublic = (CButton *)GetDlgItem(IDC_RADIO_PUBLIC);
	CButton *pRadioPrivate = (CButton *)GetDlgItem(IDC_RADIO_PRIVATE);
	CButton *pRadioFriend = (CButton *)GetDlgItem(IDC_RADIO_FRIEND);
	// 0 ˽��;1 ���ѿɼ���3 ����
	if ( pRadioPublic->GetCheck() )
		info.linkpop = 3;
	if ( pRadioPrivate->GetCheck() )
		info.linkpop = 0;//

	if ( pRadioFriend->GetCheck() )
		info.linkpop = 1;//

	//this->m_bModified = false;
	return 1;
}

void CSAPage2::OnStnClickedStaticBind()
{
	MiniOpenUrl("http://www.feiniu.com");
}


void CSAPage2::OnBnClickedRadioPublic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}
