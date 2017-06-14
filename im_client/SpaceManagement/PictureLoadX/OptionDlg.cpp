// OptionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PictureLoadX.h"
#include "OptionDlg.h"
#include ".\optiondlg.h"


// COptionDlg 对话框

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


// COptionDlg 消息处理程序

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
		::SendMessage(GetParent()->m_hWnd,WM_DATA_UPDATE,0,0);   //通知主窗口数据更新了;
		COptionDlg::OnOK();
	}	
	else
	this->MessageBox("请输入一个1—10之间的整数!","提示");
	
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
