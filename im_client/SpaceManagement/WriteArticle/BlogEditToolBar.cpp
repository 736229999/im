// im\BlogEditToolBar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WriteArticle.h"
#include "BlogEditToolBar.h"

// CBlogEditToolBar �Ի���

IMPLEMENT_DYNAMIC(CBlogEditToolBar, CDialog)
CBlogEditToolBar::CBlogEditToolBar(CDHtmlEditCtrl *pDHtmlEditCtrl, CWnd* pParent /*=NULL*/)
	: CDialog(CBlogEditToolBar::IDD, pParent)
{
	m_pDHtmlEditCtrl = pDHtmlEditCtrl;
}

CBlogEditToolBar::~CBlogEditToolBar()
{
}

void CBlogEditToolBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_COMBO_FONTNAME,m_comboFontName);
}


BEGIN_MESSAGE_MAP(CBlogEditToolBar, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_FONTNAME, OnCbnSelchangeComboFontname)
	ON_BN_CLICKED(IDC_BUTTON_U, OnBnClickedButtonU)
	ON_BN_CLICKED(IDC_BUTTON_B, OnBnClickedButtonB)
END_MESSAGE_MAP()


// CBlogEditToolBar ��Ϣ�������

BOOL CBlogEditToolBar::OnInitDialog()
{
	CDialog::OnInitDialog();

	CDC *pDC = GetDC();
	::EnumFontFamilies(pDC->m_hDC, (LPTSTR) NULL, 
		(FONTENUMPROC)EnumFontNameProc, (LPARAM)&(m_comboFontName));

	ReleaseDC(pDC);

	m_comboFontName.SelectString(0,"����");

	VARIANT inVar;
	VariantInit(&inVar);

	inVar.vt = VT_BSTR;
	inVar.bstrVal = L"����";
	this->m_pDHtmlEditCtrl->ExecCommand(CDHtmlEditCtrl::DECMD_SETFONTNAME, OLECMDEXECOPT_DONTPROMPTUSER,  &inVar);

		VARIANT strFontSize;
	strFontSize.vt = VT_I4;
	strFontSize.iVal = 330;
	m_pDHtmlEditCtrl->ExecCommand(CDHtmlEditCtrl::DECMD_SETFONTSIZE,
		OLECMDEXECOPT_DODEFAULT, &strFontSize);

	return TRUE;  
}

void CBlogEditToolBar::OnCbnSelchangeComboFontname()
{
	CString strFontName;
	int nSel = m_comboFontName.GetCurSel();

	m_comboFontName.GetLBText(nSel,strFontName);


	VARIANT inVar;
	inVar.vt = VT_BSTR;
	inVar.bstrVal = strFontName.AllocSysString();
	this->m_pDHtmlEditCtrl->ExecCommand(CDHtmlEditCtrl::DECMD_SETFONTNAME, OLECMDEXECOPT_DONTPROMPTUSER, &inVar);
	::SysFreeString(inVar.bstrVal);
}

void CBlogEditToolBar::OnBnClickedButtonU()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void CBlogEditToolBar::OnBnClickedButtonB()
{

	//m_pDHtmlEditCtrl->ExecCommand(CDHtmlEditCtrl::DECMD_BOLD,0,&inVar);
}
