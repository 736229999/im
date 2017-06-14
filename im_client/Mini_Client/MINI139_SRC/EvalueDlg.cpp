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
	//////�����ɫ����///////////////
	m_colorBackground = RGB(255, 255, 255);
	m_colorBorder = RGB(255, 255, 255)/*CHAT_BKG_COLOR*/;
	DeleteObject(m_hBkGround);
	m_hBkGround = ::CreateSolidBrush(m_colorBackground);

	SetDlgStyle(CXSkinDlg::CLOSE);

	// ��ʼ��
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

	SetWindowText("�������۱�");

	// ���ÿؼ�����
	CWnd* pChild = GetNextWindow(GW_CHILD);
	while (pChild != NULL)
	{
		pChild->SetFont(&g_font12);
		pChild = pChild->GetNextWindow(GW_HWNDNEXT);
	}

	// ���ñ༭��߿���ɫ
	m_editSuggest.SetBorderColor(RGB(145, 145, 145));

	return TRUE;
}

void CEvalueDlg::OnPaint()
{
	CPaintDC dc(this);
	CXSkinDlg::DrawCaptionBar();
	CXSkinDlg::DrawBorder();
}

// ��Ӧ�ؼ���ɫ�����¼���������Ʊ���ɫ����ԭ����ɫ������
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

// ����ύ��ť
void CEvalueDlg::OnBnClickedSubmit()
{
	UpdateData(TRUE);
	CXSkinDlg::OnOK();
}

// ��ȡ���۵ȼ�
CString CEvalueDlg::GetEvalueGrade()
{
	map<int, CString> mapGrade;
	mapGrade[0] = "�ǳ�����";
	mapGrade[1] = "����";
	mapGrade[2] = "һ��";
	mapGrade[3] = "������";
	mapGrade[4] = "�ǳ�������";

	return mapGrade[m_nEvalueGrade];
}

// ��ȡ����ȼ�
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