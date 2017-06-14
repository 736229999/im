// im\ClusterPage1.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "ClusterPage1.h"
#include ".\clusterpage1.h"
#include "WebProcess.h"


// CClusterPage1 �Ի���

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


// CClusterPage1 ��Ϣ�������

BOOL CClusterPage1::OnInitDialog()
{
	CWizardPage::OnInitDialog();

	//ComboInitFamilyClass(&m_comboType);
	InitClusterType(&m_comboType);///Ⱥ����

	//m_edtName.LimitText(20);//20��
	//m_edtIntro.LimitText(50);//50��
	m_edtIntro.SetWindowText("�����벻����50���ֵ�Ⱥ���,����Ϣ��ȫ������");

	m_edtName.SetBorderColor(RGB(126,157,185));
	m_edtIntro.SetBorderColor(RGB(126,157,185));
	//m_edtDomain.SetBorderColor(RGB(126,157,185));

	m_comboType.SetCurSel(0);
	m_comboType.SetBorderColor(RGB(126,157,185));

	//page 3 �Ƶ�page1
	((CButton *)GetDlgItem(IDC_RADIO_VERIFY))->SetCheck(TRUE);

	SetTimer(0X01,500,NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

bool CClusterPage1::GetClusterInfo(CLUSTER_INFO &info)
{
	CString str;
	m_edtName.GetWindowText(str);
	str.Trim();

	// ȥ��ȫ�ǿո�
	TrimSpace2(str);

	if ( str=="" )
	{
		//MessageBox("Ⱥ���Ʋ���Ϊ�գ�",IM_CAPTION,MB_OK|MB_ICONWARNING);
		CMessageBox::Prompt("Ⱥ���Ʋ���Ϊ�գ�");
		m_edtName.SetFocus();
		return false;
	}
/*
	m_edtDomain.GetWindowText(str);

	str.Trim();

	if ( str=="" )
	{
		//MessageBox("Ⱥ�������ܿգ�",IM_CAPTION,MB_OK|MB_ICONWARNING);
		CMessageBox::Prompt("Ⱥ�������ܿգ�");
		m_edtDomain.SetFocus();
		return false;
	}

	int l=str.GetLength();
	str.MakeLower();


	if ( l<3 )
	{
		//MessageBox("Ⱥ��������̫���ˣ�����3����ĸ��",IM_CAPTION,MB_OK|MB_ICONERROR);
		CMessageBox::Prompt("Ⱥ��������̫���ˣ�����3����ĸ��");
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
			//MessageBox("Ⱥ����ֻ������ĸ�����ֵĽ�� �� :name123",IM_CAPTION,MB_OK|MB_ICONERROR);
			ShowMessage("Ⱥ����ֻ������ĸ�����ֵĽ�� �� :name123");
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

	//page3�Ƶ�page1
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
			ShowMessage("Ⱥ�������ܰ����»���,����ֻ������ĸ�����ֵĽ�� �� :name123.",MB_ICONERROR|MB_OK );
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString strTxt = _T("");
	if ( nIDEvent==0X01 )
	{
		m_edtName.GetWindowText(strTxt);
		if ( strTxt=="" )
		{
			//m_pHintPass = new CHintDlg2(IDB_LOGIN_PASSHINT,rc.left,rc.top,this);
			m_edtName.SetWindowText("������20��");
			m_edtName.SetSel(0,-1);				
		}			

		/*Ⱥ����Ϊ��
		m_edtIntro.GetWindowText(strTxt);
		if ( strTxt=="" )
		{
			//m_pHintPass = new CHintDlg2(IDB_LOGIN_PASSHINT,rc.left,rc.top,this);
			m_edtIntro.SetWindowText("�����벻����50���ֵ�Ⱥ���,����Ϣ��ȫ������");
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
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CWizardPage::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_NAME);
	DWORD dwSel = pEdit->GetSel();//��ȡ��ǰ���λ��
	int maxSize =20;
	CString strName,allName;
	CString tmp;
	pEdit->GetWindowText(strName);	
	int iLen =0;//���ָ���
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
				pEdit->SetSel(dwSel);//���������ı���β���޷��ص��ı���ͷ
			}
			break;
		}
	}	
}


void CClusterPage1::OnChangeEditIntro()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CWizardPage::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CEdit *pEdit=(CEdit*)GetDlgItem(IDC_EDIT_INTRO);
	DWORD dwSel = pEdit->GetSel();//��ȡ��ǰ���λ��
	int maxSize =50;
	CString strName,allName;
	CString tmp;
	pEdit->GetWindowText(strName);	
	int iLen =0;//���ָ���
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
				pEdit->SetSel(dwSel);//���������ı���β���޷��ص��ı���ͷ
			}
			break;
		}
	}	
}
