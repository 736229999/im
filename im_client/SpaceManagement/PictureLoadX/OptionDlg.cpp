// OptionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PictureLoadX.h"
#include "OptionDlg.h"
#include ".\optiondlg.h"


// COptionDlg �Ի���

IMPLEMENT_DYNAMIC(COptionDlg, CDialog)
COptionDlg::COptionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COptionDlg::IDD, pParent)
	, m_bAlwaysTopMainDlg(TRUE)
	, m_nIntervalMainDlg(3)
	, m_bEmptyPictures(TRUE)
{
}

COptionDlg::~COptionDlg()
{
}

void COptionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_CHECK_TOP, m_bAlwaysTopMainDlg);
	DDX_Text(pDX, IDC_EDIT_INTERVAL, m_nIntervalMainDlg);
	//	DDV_MinMaxInt(pDX, m_nIntervalMainDlg, 1, 10);
	DDX_Check(pDX, IDC_CHECK_EMPTY_ALL_PICTURES, m_bEmptyPictures);
}


BEGIN_MESSAGE_MAP(COptionDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_CANCLE, OnBnClickedButtonCancle)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_INTERVAL, OnDeltaposSpinInterval)
END_MESSAGE_MAP()


// COptionDlg ��Ϣ�������

void COptionDlg::OnBnClickedButtonCancle()
{
	UpdateData(FALSE);
	
	COptionDlg::OnCancel();
}

void COptionDlg::OnBnClickedButtonOk()
{
	
	UpdateData(TRUE);
	if(m_nIntervalMainDlg>=1&&m_nIntervalMainDlg<=10)
	{
		::SendMessage(GetParent()->m_hWnd,WM_DATA_UPDATE,0,0);   //֪ͨ���������ݸ�����;
		COptionDlg::OnOK();
	}	
	else
	this->MessageBox("������һ��1��10֮�������!","��ʾ");
	
}

BOOL  COptionDlg::IsEmptyPictures()
{
	return  m_bEmptyPictures;
}

BOOL  COptionDlg::IsAlwaysTop()
{
	return  m_bAlwaysTopMainDlg;
}
int  COptionDlg::GetInterval()
{
	return  m_nIntervalMainDlg;
}
void COptionDlg::OnDeltaposSpinInterval(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	
	if(-1==pNMUpDown->iDelta)   m_nIntervalMainDlg++;
	else						m_nIntervalMainDlg--;
	if(m_nIntervalMainDlg>10)	m_nIntervalMainDlg=10;
	if(m_nIntervalMainDlg<1)   m_nIntervalMainDlg=1;
	SetDlgItemInt(IDC_EDIT_INTERVAL,m_nIntervalMainDlg,TRUE);	
}
void COptionDlg::PreSubclassWindow()
{
	
	CDialog::PreSubclassWindow();
}

BOOL COptionDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	GetDlgItem(IDC_EDIT_INTERVAL)->ModifyStyle(0,ES_NUMBER);
	return TRUE;  
}
