// SelfInfoPreviewDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SelfInfoPreviewDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "XmlAddress.h"
#include "../../../Common/Common.h"

// CString g_cCareers[] = 
// 		{"ʳƷ","�Ҿ�","�ٻ�","��ױ","�������",
// 		"�ҵ�","����","ͼ������","����","����"};

CString g_cCareers[] = 
	{
		"ʳƷ","����","����","�ҵ�","�ճ�����",
		"ĸӤ","ͼ������","��ױ","�������",
		"����","��Ȥ����","��������","����",
		"����"};
// CSelfInfoPreviewDlg �Ի���

IMPLEMENT_DYNAMIC(CSelfInfoPreviewDlg, CDialog)

CSelfInfoPreviewDlg::CSelfInfoPreviewDlg(CWnd* pParent)
	: CDialog(IDD_DLG_SELF_INFO_PREVIEW, pParent)
{
	m_hBkGround = ::CreateSolidBrush(RGB(235, 235, 235));
}

CSelfInfoPreviewDlg::~CSelfInfoPreviewDlg()
{
	DeleteObject(m_hBkGround);
}

void CSelfInfoPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_EDIT, m_btnEdit);
}


BEGIN_MESSAGE_MAP(CSelfInfoPreviewDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_EDIT, OnBnClickedEdit)
END_MESSAGE_MAP()


// CSelfInfoPreviewDlg ��Ϣ�������
BOOL CSelfInfoPreviewDlg::Create()
{
	BOOL bRlt = CDialog::Create(IDD_DLG_SELF_INFO_PREVIEW, m_pParentWnd);
	ShowWindow(SW_SHOW);

	return bRlt;
}

BOOL CSelfInfoPreviewDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���������ӿؼ������ÿؼ�����
	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
	while(hWndChild)  
	{  
		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
	}

	// ҳ������
	Refresh();	

	// �༭��ť
	m_btnEdit.SetPng(IDB_PNG_GRAYBTN_S_HOVER, IDB_PNG_GRAYBTN_S_NORMAL, IDB_PNG_GRAYBTN_S_PRESSED, IDB_PNG_GRAYBTN_S_PRESSED);
	m_btnEdit.SetTextColor(0x2e2e2e);

#ifdef BUSINESS_VESION
	GetDlgItem(IDC_STATIC_INDUSTRY_VALUE)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_CAREER)->ShowWindow(SW_HIDE);
#endif
	return TRUE;
}

void CSelfInfoPreviewDlg::OnPaint()
{
	CDialog::OnPaint();

	CPaintDC dc(this);	

}

BOOL CSelfInfoPreviewDlg::OnEraseBkgnd(CDC* pDC)
{
	CDialog::OnEraseBkgnd(pDC);

	// ���ñ���ɫ
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient,RGB(235, 235, 235));	

	return FALSE;
}

// ���ÿؼ���ɫ
HBRUSH CSelfInfoPreviewDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBkGround;
	}

	return hbr;
}

// ����˱༭��ť���򸸴��ڷ���Ϣ
void CSelfInfoPreviewDlg::OnBnClickedEdit()
{
	::SendMessage(GetParent()->m_hWnd, WM_SELF_INFO_PREVIEW_DLG_MSG, IDC_BTN_EDIT, 0);
}

// �����û���Ϣ
void CSelfInfoPreviewDlg::Refresh()
{
	SetDlgItemText(IDC_STATIC_NAME_VALUE, g_ImUserInfo.nickname);
	SetDlgItemText(IDC_STATIC_SEX_VALUE, SexToStr(g_ImUserInfo.sex));
	// �ϳɵ���
	map<int, string> mapP;
	GetAllProvince(mapP);
	map<int, string> mapC;
	map<int, string> mapD;
	string strAddress;
	int nP = 1;
	int nC = 1;
	if(g_ImUserInfo.province > 0)
	{
		strAddress = mapP[g_ImUserInfo.province];
		nP = g_ImUserInfo.province;
	}
	else
	{
		strAddress = mapP[nP];
	}
	strAddress += "  ";
	GetAllCity(nP, mapC);
	if(g_ImUserInfo.city > 0)
	{
		strAddress += mapC[g_ImUserInfo.city];
		nC = g_ImUserInfo.city;
	}
	else
	{
		strAddress += mapC[nC];
	}
	strAddress += "  ";
	GetAllDistrict(nP, nC, mapD);
	if(g_ImUserInfo.country > 0)
	{
		strAddress += mapD[g_ImUserInfo.country];
	}
	else
	{
		strAddress += mapD[1];
	}
	SetDlgItemText(IDC_STATIC_LOCAL_VALUE, strAddress.c_str());
	// ����
	if(strcmp(g_ImUserInfo.birthday, "") != 0)
	{
		int nY, nM, nD;
		sscanf(g_ImUserInfo.birthday, "%4d%2d%2d", &nY, &nM, &nD); 
		CString strBirthday;
		strBirthday.Format("%d��%d��%d��", nY, nM, nD);
		SetDlgItemText(IDC_STATIC_BIRTHDAY, strBirthday);
	}	
	// �ֻ�
	SetDlgItemText(IDC_STATIC_MOBILE_VALUE, g_ImUserInfo.mobile);
	// �绰
	//SetDlgItemText(IDC_STATIC_TEL_VALUE, g_ImUserInfo.);
	//�绰(����)
	#ifdef _VER_INTERNAL_
	string strJob, strExt;
	SplitExtAndJob(g_ImUserInfo.flash_url, strExt, strJob);
	SetDlgItemText(IDC_STATIC_TEL_VALUE,strExt.c_str());
	#endif
	// ����
	SetDlgItemText(IDC_STATIC_EMAIL_VALUE, g_ImUserInfo.email);
	// ��ҵ
	CString strCareer = "";
	if(g_ImUserInfo.career > 0 && g_ImUserInfo.career < 10 + 1)
	{
		strCareer = g_cCareers[g_ImUserInfo.career-1];
	}
	SetDlgItemText(IDC_STATIC_INDUSTRY_VALUE, strCareer);
}
