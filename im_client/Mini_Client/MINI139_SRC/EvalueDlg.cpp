// EvalueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EvalueDlg.h"
#include "afxdialogex.h"
#include "Resource.h"

#include <map>
using namespace std;
// CEvalueDlg dialog

IMPLEMENT_DYNAMIC(CEvalueDlg, CXSkinDlg)

CEvalueDlg::CEvalueDlg(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(IDD_DLG_EVALUE, pParent)
{
	//////添加颜色设置///////////////
	m_colorBackground = RGB(255, 255, 255);
	m_colorBorder = RGB(255, 255, 255)/*CHAT_BKG_COLOR*/;
	DeleteObject(m_hBkGround);
	m_hBkGround = ::CreateSolidBrush(m_colorBackground);

	SetDlgStyle(CXSkinDlg::CLOSE);

	// 初始化
	m_nEvalueGrade = 0;
	m_strSuggest = "";
}

CEvalueDlg::~CEvalueDlg()
{
}

void CEvalueDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SUGGEST, m_editSuggest);
	DDX_Control(pDX, IDC_BTN_SUBMIT, m_btnSubmit);
	DDX_Radio(pDX, IDC_RADIO_Very_satisfied, m_nEvalueGrade);
	DDX_Text(pDX, IDC_EDIT_SUGGEST, m_strSuggest);
}


BEGIN_MESSAGE_MAP(CEvalueDlg, CXSkinDlg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_SUBMIT, OnBnClickedSubmit)
END_MESSAGE_MAP()


// CEvalueDlg message handlers
BOOL CEvalueDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetWindowText("服务评价表");

	// 设置控件字体
	CWnd* pChild = GetNextWindow(GW_CHILD);
	while (pChild != NULL)
	{
		pChild->SetFont(&g_font12);
		pChild = pChild->GetNextWindow(GW_HWNDNEXT);
	}

	// 设置编辑框边框颜色
	m_editSuggest.SetBorderColor(RGB(145, 145, 145));

	return TRUE;
}

void CEvalueDlg::OnPaint()
{
	CPaintDC dc(this);
	CXSkinDlg::DrawCaptionBar();
	CXSkinDlg::DrawBorder();
}

// 相应控件颜色绘制事件，解决控制背景色保持原有颜色的问题
HBRUSH CEvalueDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		DWORD ID = GetWindowLong(pWnd->GetSafeHwnd(), GWL_ID);
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBkGround;
	}

	////////////////////////////////////////////////////////////////////
	return hbr;
}

// 点击提交按钮
void CEvalueDlg::OnBnClickedSubmit()
{
	UpdateData(TRUE);
	CXSkinDlg::OnOK();
}

// 获取评价等级
CString CEvalueDlg::GetEvalueGrade()
{
	map<int, CString> mapGrade;
	mapGrade[0] = "非常满意";
	mapGrade[1] = "满意";
	mapGrade[2] = "一般";
	mapGrade[3] = "不满意";
	mapGrade[4] = "非常不满意";

	return mapGrade[m_nEvalueGrade];
}

// 获取服务等级
EN_EVALUA_GRADE CEvalueDlg::GetEvalueGradeEn()
{
	map<int, EN_EVALUA_GRADE> mapGrade;
	mapGrade[0] = eVerySatisfied;
	mapGrade[1] = eSatisfied;
	mapGrade[2] = eGeneral;
	mapGrade[3] = eDissatisfied;
	mapGrade[4] = eVeryDissatisfied;	

	return mapGrade[m_nEvalueGrade];
}