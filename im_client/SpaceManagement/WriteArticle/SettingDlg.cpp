// SettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WriteArticle.h"
#include "SettingDlg.h"
#include ".\settingdlg.h"
#include "WriteBlogDlg.h"


// CSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CSettingDlg, CDialog)
CSettingDlg::CSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDlg::IDD, pParent)
{
}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TAG, m_tagComboBox);
	DDX_Control(pDX, IDC_COMBO_VIEW, m_comBoBoxView);
	//DDX_Control(pDX, IDC_COMBO_DIR, m_comboBoxArticles);


	DDX_Control(pDX, IDC_COMBO_DIR, m_comboBoxArts);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialog)
	ON_WM_CTLCOLOR()
//	ON_CBN_SELCHANGE(IDC_COMBO_TAG, OnCbnSelchangeComboTag)
END_MESSAGE_MAP()


// CSettingDlg ��Ϣ�������

HBRUSH CSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	//if (nCtlColor==CTLCOLOR_DLG)
	//{
	//	
	//	CBrush  br;
	//	br.CreateSolidBrush(IM_SPACE_COLOR);

	//	hbr=(HBRUSH)br.Detach();
	//}
	//if (nCtlColor==CTLCOLOR_STATIC) 
	//{
	//	pDC->SetBkColor(IM_SPACE_COLOR);
	//}

	CBrush  br;
	br.CreateSolidBrush(IM_SPACE_COLOR);
	hbr=(HBRUSH)br.Detach();
	//pDC->SetTextColor(RGB(1,161,255));
	pDC->SetBkMode(TRANSPARENT);

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

//void CSettingDlg::OnCbnSelchangeComboTag()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	
//	
//}

BOOL CSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
