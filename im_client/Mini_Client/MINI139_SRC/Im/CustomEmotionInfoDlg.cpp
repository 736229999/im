// im\CustomEmotionInfoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\customemotioninfodlg.h"


// CCustomEmotionInfoDlg 对话框

IMPLEMENT_DYNAMIC(CCustomEmotionInfoDlg, CDialog)
CCustomEmotionInfoDlg::CCustomEmotionInfoDlg(CString strShortCut, CWnd* pParent /*=NULL*/)
	: CDialog(CCustomEmotionInfoDlg::IDD, pParent)
{
	m_strShortCut   = strShortCut;
	m_bInfoNextTime = false;
}

CCustomEmotionInfoDlg::~CCustomEmotionInfoDlg()
{
}

void CCustomEmotionInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCustomEmotionInfoDlg, CDialog)
	ON_BN_CLICKED(IDC_CHECK1, OnBnClickedCheck1)
END_MESSAGE_MAP()



// CCustomEmotionInfoDlg 消息处理程序

BOOL CCustomEmotionInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString str;
	str.Format("对方不在线或隐身，快捷方式\"%s\"不能转义成自定义表情！",
		m_strShortCut );
	SetDlgItemText(IDC_STATIC_INFO,str);

	((CButton *)GetDlgItem(IDC_CHECK1))->SetCheck(1);
	return TRUE;  
}

void CCustomEmotionInfoDlg::OnBnClickedCheck1()
{
	CButton * pBtn = (CButton *)GetDlgItem(IDC_CHECK1);
	m_bInfoNextTime = !pBtn->GetCheck();
}
