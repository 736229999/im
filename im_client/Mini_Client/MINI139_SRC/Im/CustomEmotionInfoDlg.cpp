// im\CustomEmotionInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\customemotioninfodlg.h"


// CCustomEmotionInfoDlg �Ի���

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



// CCustomEmotionInfoDlg ��Ϣ�������

BOOL CCustomEmotionInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CString str;
	str.Format("�Է������߻�������ݷ�ʽ\"%s\"����ת����Զ�����飡",
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
