// FormViewCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "FormViewCtrl.h"
#include ".\formviewctrl.h"
#include "personinfo.h"
#include "impublic.h"


#include "localdata.h"
// CFormViewCtrl

IMPLEMENT_DYNCREATE(CFormViewCtrl, CFormView)
IMPLEMENT_HMG_SCROLLBAR(CFormViewCtrl)

CFormViewCtrl::CFormViewCtrl()
	: CFormView(CFormViewCtrl::IDD)
	, m_strName(_T(""))
	, m_strCompany(_T(""))
	, m_strCareer(_T(""))
	, m_strAddress(_T(""))
	, m_strEmail(_T(""))
	, m_strPhone(_T(""))
	, m_strMSNNum(_T(""))
	, m_strBlog(_T(""))
	, m_strNote(_T(""))
	, m_strQQNum(_T(""))
	, m_strSchool(_T(""))
	, m_strBirthday(_T(""))
{
	m_bModified   = false;
	m_bModifyFlag = false;
}

CFormViewCtrl::~CFormViewCtrl()
{
}

void CFormViewCtrl::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_NAME, m_strName);
	DDX_Text(pDX, IDC_EDIT_COMPANY, m_strCompany);
	DDX_Text(pDX, IDC_EDIT1_CAREER, m_strCareer);
	DDX_Text(pDX, IDC_EDIT_ADDRESS, m_strAddress);
	DDX_Text(pDX, IDC_EDIT_EMAIL, m_strEmail);
	DDX_Text(pDX, IDC_EDIT_PHOTO, m_strPhone);
	DDX_Text(pDX, IDC_EDIT_MSN_NUM, m_strMSNNum);
	DDX_Text(pDX, IDC_EDIT_BLOG, m_strBlog);
	DDX_Text(pDX, IDC_EDIT_NOTE, m_strNote);
	DDX_Text(pDX, IDC_EDIT_QQNUM, m_strQQNum);
	DDX_Control(pDX, IDC_COMBO_SEX, m_wndComboSex);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_wndComboType);
	DDV_MaxChars(pDX, m_strNote, 4095);
	DDV_MaxChars(pDX, m_strBlog, 1000);
	DDV_MaxChars(pDX, m_strQQNum, 20);
	DDV_MaxChars(pDX, m_strMSNNum, 40);
	DDV_MaxChars(pDX, m_strPhone, 40);
	DDV_MaxChars(pDX, m_strEmail, 40);
	DDV_MaxChars(pDX, m_strAddress, 200);
	DDV_MaxChars(pDX, m_strCompany, 100);
	DDV_MaxChars(pDX, m_strCareer, 20);
	DDX_Text(pDX, IDC_EDIT_SCHOOL, m_strSchool);
	DDV_MaxChars(pDX, m_strSchool, 40);
	DDV_MaxChars(pDX, m_strName, 40);
	DDX_Control(pDX, IDC_EDIT_NAME, m_wndEditName);
	DDX_Control(pDX, IDC_EDIT_SCHOOL, m_wndEditSchool);
	DDX_Control(pDX, IDC_EDIT_COMPANY, m_wndEditCompany);
	DDX_Control(pDX, IDC_EDIT_ADDRESS, m_wndEditAddress);
	DDX_Control(pDX, IDC_EDIT_EMAIL, m_wndEditEmail);
	DDX_Control(pDX, IDC_EDIT_PHOTO, m_wndEditPhone);
	DDX_Control(pDX, IDC_EDIT_QQNUM, m_wndEditQQNum);
	DDX_Control(pDX, IDC_EDIT_MSN_NUM, m_wndEditMSNNum);
	DDX_Control(pDX, IDC_EDIT_BLOG, m_wndEditBlog);
	DDX_Control(pDX, IDC_EDIT_NOTE, m_wndEditNote);
	DDX_Control(pDX, IDC_EDIT1_CAREER, m_wndEditCareer);
	//DDX_Control(pDX, IDC_DATETIMEPICKER_BIRTHDAY, m_wndTimeBirthday);
	DDX_Control(pDX, IDC_EDIT_BIRTHDAY, m_wndEditBirthday);
	DDX_Text(pDX, IDC_EDIT_BIRTHDAY, m_strBirthday);
}

BEGIN_MESSAGE_MAP(CFormViewCtrl, CFormView)
	ON_WM_MOUSEACTIVATE()
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER_BIRTHDAY, OnDtnDatetimechangeDatetimepickerBirthday)
	SET_HMG_SCROLLBAR(IDC_EDIT_NOTE)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CFormViewCtrl ���

#ifdef _DEBUG
void CFormViewCtrl::AssertValid() const
{
	CFormView::AssertValid();
}

void CFormViewCtrl::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


BOOL CFormViewCtrl::CreateFromCtrl(UINT nID, CWnd* pParent)
{
	if (!pParent || !pParent->GetSafeHwnd())
		return FALSE;
	CWnd *pCtrl = pParent->GetDlgItem(nID);
	if (!pCtrl)
		return FALSE;
	CRect rcCtrl;
	pCtrl->GetWindowRect(rcCtrl);
	pParent->ScreenToClient(rcCtrl);
	UINT style = ::GetWindowLong(pCtrl->GetSafeHwnd(), GWL_STYLE);
	pCtrl->DestroyWindow();

	BOOL bResult = Create(NULL, NULL, style | WS_CHILD | WS_VISIBLE, rcCtrl, pParent, nID, NULL);
	if( bResult)
	{
		OnInitialUpdate();
	}

	return bResult;
}

void CFormViewCtrl::Reset()
{
	CTime timeTemp;

	m_strCompany = "";
	m_strCareer = "";
	m_strAddress ="";
	m_strPhone="";
	m_strMSNNum="";
	m_strBlog="";
	m_strNote="";
	m_strQQNum ="";
	m_strSchool ="";	
	m_strBirthday = "";

	if ( m_nType==0 )
	{	
		m_strEmail ="";
		m_wndComboType.SetWindowText("");
	}

	UpdateData(FALSE);
}
void CFormViewCtrl::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_wndComboSex.AddString("Ů");
	m_wndComboSex.AddString("��");
	m_wndComboSex.AddString("-");

	AttachHMGScrollbar(m_wndEditNote.m_hWnd);
    
}

BOOL CFormViewCtrl::GetContactInfo(ContactPersonInfo& personInfo,ContactGroupInfo &groupInfo)
{
	if ( UpdateData()==FALSE )
		return FALSE;

	TrimSpace2(m_strName);
	//����
	if(m_strName.GetLength()==0)
	{
		ShowMessage("��������Ϊ��!",MB_OK);
		return FALSE;
	}

	//�Ա�
	CString strSex ;
	m_wndComboSex.GetWindowText(strSex);

	personInfo.strAddress  = m_strAddress;
	personInfo.strBirthday = m_strBirthday;
	personInfo.strBlog     = m_strBlog;
	personInfo.strCareer   = m_strCareer;
	personInfo.strCompany  = m_strCompany;
	personInfo.strEmail    = m_strEmail;
	personInfo.strMSNNum   = m_strMSNNum;
	personInfo.strName     = m_strName;
	personInfo.strNote     = m_strNote;
	personInfo.strPhone    = m_strPhone  ;
	personInfo.strQQNum    = m_strQQNum  ;
	personInfo.strSchool   = m_strSchool ;
	personInfo.nSex        = m_wndComboSex.GetCurSel();
	personInfo.nType       = m_nType;
	personInfo.nInvited    = m_nInvited;

	CString strGroup;
	m_wndComboType.GetWindowText(strGroup);
	strGroup.Trim();

	if (strGroup=="")
		strGroup="����";

	if (m_nType==0 )
	{
		if ( strGroup=="Outlook��ַ��" )
		{
			ShowMessage("�Բ���Outlook��ַ���Ѿ���ʹ���ˣ�����������һ��������",MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		int iIndex = m_groupInfo.GetGroupIndex(strGroup);

		if ( iIndex==-1 )
		{
			iIndex = m_groupInfo.Add(strGroup);
			personInfo.nGroupIndex = iIndex;
		}
		else
		{
			personInfo.nGroupIndex = m_groupInfo.GetGroupIndex(strGroup);
		}

		groupInfo = m_groupInfo;
	}

	return TRUE;
}

BOOL CFormViewCtrl::SetContactInfo(const ContactPersonInfo & personInfo,const ContactGroupInfo &groupInfo)
{
	m_strAddress  = personInfo.strAddress;
	m_strBirthday = personInfo.strBirthday;
	m_strBlog     = personInfo.strBlog;
	m_strCareer   = personInfo.strCareer;
	m_strCompany  = personInfo.strCompany;
	m_strEmail    = personInfo.strEmail;
	m_strMSNNum   = personInfo.strMSNNum;
	m_strName     = personInfo.strName;
	m_strNote     = personInfo.strNote;
	m_strPhone    = personInfo.strPhone;
	m_strQQNum    = personInfo.strQQNum;
	m_strSchool   = personInfo.strSchool;

	m_wndComboSex.SetCurSel(personInfo.nSex);
	m_nType       = personInfo.nType;
	m_nInvited    = personInfo.nInvited;

	m_groupInfo   = groupInfo;
	m_wndComboType.ResetContent();

	for(int i=0; i<groupInfo.count; i++ )
	{
		m_wndComboType.AddString(groupInfo.items[i].szName);
	}

	m_wndComboType.SelectString(-1,m_groupInfo.GetGroupName(personInfo.nGroupIndex));

	if ( personInfo.nType==1 )//out look contact
	{
		m_wndComboType.SetWindowText("Outlook��ַ��");
		m_wndComboType.EnableWindow(FALSE);		
		m_wndEditEmail.SetReadOnly(TRUE);
		SetDlgItemText(IDC_STATIC_TYPE_TIP,"");
	}	
	else
	{		
		SetDlgItemText(IDC_STATIC_TYPE_TIP,"(ѡ����ϵ�˷����������һ��������)");
	}


	return UpdateData(FALSE);
}


void CFormViewCtrl::OnDtnDatetimechangeDatetimepickerBirthday(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMDATETIMECHANGE pDTChange = reinterpret_cast<LPNMDATETIMECHANGE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	m_bModified = true;

	CWnd *pParent = GetParent();
	if ( pParent )
		pParent->PostMessage(WM_MODIFY_NOTIFY,0,0);
}

BOOL CFormViewCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CRect rect;

	GetWindowRect(rect);
	ScreenToClient(rect);

	pDC->FillSolidRect(rect,RGB(255,255,255));
	pDC->Draw3dRect(rect,IM_LINE_COLOR,IM_LINE_COLOR);

	return TRUE;
	//return CFormView::OnEraseBkgnd(pDC);
}

HBRUSH CFormViewCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if( CTLCOLOR_STATIC == nCtlColor )
	{
		pDC->SetTextColor(RGB(0,84,166));
		pDC->SetBkMode(TRANSPARENT);
		return ::g_hWhiteBrush;
	}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

LRESULT CFormViewCtrl::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( m_hWnd )
	{
		if ( ( m_bModifyFlag==false ) &&
			( message>= WM_MOUSEFIRST && message<=WM_MOUSELAST ) ||
			( message>= WM_KEYFIRST && message<=WM_KEYLAST ) )
		{
			m_bModifyFlag=true;
		}

		if ( m_bModifyFlag && message==WM_COMMAND )
		{
			WORD w=HIWORD(wParam);

			switch (w)
			{
			case EN_CHANGE:
			case BN_CLICKED:
			case CBN_SELCHANGE:
			case CBN_EDITCHANGE:
				m_bModified=true;
				{
					CWnd *pParent = GetParent();
					if ( pParent )
						pParent->PostMessage(WM_MODIFY_NOTIFY,0,0);
				}
				break;
			}
		}
	}


	return CFormView::WindowProc(message, wParam, lParam);
}
