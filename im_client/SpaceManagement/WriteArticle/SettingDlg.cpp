// SettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteArticle.h"
#include "SettingDlg.h"
#include ".\settingdlg.h"
#include "WriteBlogDlg.h"


// CSettingDlg 对话框

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


// CSettingDlg 消息处理程序

HBRUSH CSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性
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

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//void CSettingDlg::OnCbnSelchangeComboTag()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	
//	
//}

BOOL CSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
