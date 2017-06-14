// SelfInfoEditDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SelfInfoEditDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "XmlAddress.h"
#include "Im\MessageBox.h"
#include "UIControls\UIAddrComboBox.h"
#include "../../../Common/Common.h"

extern CString g_cCareers[];

// CSelfInfoEditDlg �Ի���
#define TIMER_REFRESH	0x01			// �����û���Ϣ
#define BURTH_START_YEAR 1901
IMPLEMENT_DYNAMIC(CSelfInfoEditDlg, CDialog)

CSelfInfoEditDlg::CSelfInfoEditDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_DLG_SELF_INFO_EDIT, pParent)
{
	m_hBkGround = ::CreateSolidBrush(RGB(235, 235, 235));
	m_nPublicLevel = 0;
	m_pcomboProvince = new CUIAddrComboBox();
	m_pcomboCity = new CUIAddrComboBox();
	m_pcomboCounty = new CUIAddrComboBox();
	m_nLastReqSeq = 0;
}

CSelfInfoEditDlg::~CSelfInfoEditDlg()
{
	g_pProtocol->UnRegistCmd(m_hWnd);
	DeleteObject(m_hBkGround);

	if(::IsWindow(GetSafeHwnd()))
	{
		m_pcomboProvince->SendMessage(WM_NCDESTROY);
		m_pcomboCity->SendMessage(WM_NCDESTROY);
		m_pcomboCounty->SendMessage(WM_NCDESTROY);
		DestroyWindow();
	}

	if(m_pcomboProvince != NULL)
	{
		delete m_pcomboProvince;
		m_pcomboProvince = NULL;
	}
	if(m_pcomboCity != NULL)
	{
		delete m_pcomboCity;
		m_pcomboCity = NULL;
	}
	if(m_pcomboCounty != NULL)
	{
		delete m_pcomboCounty;
		m_pcomboCounty = NULL;
	}
}

void CSelfInfoEditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BTN_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_EDIT_NAME, m_edtName);
	DDX_Control(pDX, IDC_COMBO_SEX, m_comboSex);
	DDX_Control(pDX, IDC_DATETIMEPICKER_BIRTH, m_DateTimeBirth);
	DDX_Control(pDX, IDC_COMBO_CAREER, m_comboCareer);
	DDX_Control(pDX, IDC_EDIT_PHONE, m_edtPhone);
	DDX_Control(pDX, IDC_EDIT_TEL, m_edtTel);
	DDX_Control(pDX, IDC_EDIT_EMAIL, m_edtEmail);
	DDX_Radio(pDX, IDC_RADIO_PUBLIC, m_nPublicLevel);
	DDX_Control(pDX, IDC_COMBO_DATE_YEAR, m_comboDateYear);
	DDX_Control(pDX, IDC_COMBO_DATE_MONTH, m_comboDateMonth);
	DDX_Control(pDX, IDC_COMBO_DATE_DAY, m_comboDateDay);
	DDX_Control(pDX, IDC_RADIO_PUBLIC, m_radioPublic);
	DDX_Control(pDX, IDC_RADIO_PUBLIC_TO_FRIEND, m_radioPublicToFriend);
	DDX_Control(pDX, IDC_RADIO_PRIVATE, m_radioPrivate);
}


BEGIN_MESSAGE_MAP(CSelfInfoEditDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_CANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BTN_SAVE, OnBnClickedSave)
	ON_CBN_SELCHANGE(IDC_COMBO_PROVINCE, OnCbnSelchangeComboProvince)
	ON_CBN_SELCHANGE(IDC_COMBO_CITY, OnCbnSelchangeComboCity)
	ON_XT_MESSAGE()
	ON_WM_TIMER()
	ON_CBN_SELCHANGE(IDC_COMBO_DATE_YEAR, &CSelfInfoEditDlg::OnCbnSelchangeComboDateYear)
	ON_CBN_SELCHANGE(IDC_COMBO_DATE_MONTH, &CSelfInfoEditDlg::OnCbnSelchangeComboDateMonth)
	ON_CBN_SELCHANGE(IDC_COMBO_DATE_DAY, &CSelfInfoEditDlg::OnCbnSelchangeComboDateDay)
	ON_MESSAGE(WM_COMBO_ITEMSELECT, OnAddrComboSelChange)
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CSelfInfoEditDlg)
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoAck)
	ON_XT_COMMAND(CMD_USERINFO_MOD_ACK, OnUserInfoModAck)
END_XT_MESSAGE_MAP()

// CSelfInfoEditDlg ��Ϣ�������
BOOL CSelfInfoEditDlg::Create()
{
	BOOL bRlt = CDialog::Create(IDD_DLG_SELF_INFO_EDIT, m_pParentWnd);
	ShowWindow(SW_SHOW);

	return bRlt;
}

BOOL CSelfInfoEditDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���������ӿؼ������ÿؼ�����
	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
	while(hWndChild)  
	{  
		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
	}

	// ��ť
	m_btnSave.SetPng(IDB_PNG_GRAYBTN_S_HOVER, IDB_PNG_GRAYBTN_S_NORMAL, IDB_PNG_GRAYBTN_S_PRESSED, IDB_PNG_GRAYBTN_S_PRESSED);
	m_btnSave.SetTextColor(RGB(0, 0, 0));
	m_btnCancel.SetPng(IDB_PNG_GRAYBTN_S_HOVER, IDB_PNG_GRAYBTN_S_NORMAL, IDB_PNG_GRAYBTN_S_PRESSED, IDB_PNG_GRAYBTN_S_PRESSED);
	m_btnCancel.SetTextColor(RGB(0, 0, 0));

	// ���ñ߿���ɫ
	m_edtName.SetBorderColor(RGB(200, 200, 200));
	m_edtPhone.SetBorderColor(RGB(200, 200, 200));
	m_edtTel.SetBorderColor(RGB(200, 200, 200));
	m_edtEmail.SetBorderColor(RGB(200, 200, 200));
	m_comboSex.SetBorderColor(RGB(200, 200, 200));
	m_comboCareer.SetBorderColor(RGB(200, 200, 200));
	m_DateTimeBirth.SetBorderColor(RGB(200, 200, 200));

	CRect rcComSex;
	m_comboSex.GetWindowRect(&rcComSex);
	ScreenToClient(&rcComSex);

	m_pcomboProvince->Create(GetSafeHwnd(), _T("DUIAddrComboP"), UI_WNDSTYLE_CHILD, 0, rcComSex.left, rcComSex.bottom+3, 135, 28); 
	m_pcomboProvince->SetLabel("ʡ�ݣ�");
	m_pcomboProvince->ShowWindow(TRUE); 
	m_pcomboCity->Create(GetSafeHwnd(), _T("DUIAddrComboC"), UI_WNDSTYLE_CHILD, 0, rcComSex.left+136, rcComSex.bottom+3, 135, 28); 
	m_pcomboCity->SetLabel("���У�");
	m_pcomboCity->ShowWindow(TRUE); 
	m_pcomboCounty->Create(GetSafeHwnd(), _T("DUIAddrComboD"), UI_WNDSTYLE_CHILD, 0, rcComSex.left+272, rcComSex.bottom+3, 135, 28); 
	m_pcomboCounty->SetLabel("���أ�");
	m_pcomboCounty->ShowWindow(TRUE); 

	// ��Combo��������
	map<int, string> mapProvince;
	GetAllProvince(mapProvince);
	map<int, string>::iterator it = mapProvince.begin();
	for(; it != mapProvince.end(); ++it)
	{
		int nIndex = m_pcomboProvince->AddString(it->second.c_str());
	}
	m_pcomboProvince->SelectItem(0);

	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_USERINFO_MOD_ACK,m_hWnd);

	//����ʱ��
	COleDateTime MinTime(BURTH_START_YEAR,1,1,0,0,0);
	COleDateTime MaxTime(COleDateTime::GetCurrentTime());
	m_DateTimeBirth.SetRange(&MinTime,&MaxTime);
	int nYear = MinTime.GetYear();
	for ( ; nYear <= MaxTime.GetYear(); nYear++)
	{
		CString strYear = "";
		strYear.Format("%d��", nYear);
		m_comboDateYear.AddString(strYear);
	}
	m_comboDateYear.SetCurSel(nYear-MinTime.GetYear() - 1);

	int nMonth = 1;
	for( ; nMonth <= 12; nMonth++)
	{
		CString strMonth = "";
		strMonth.Format("%d��", nMonth);
		m_comboDateMonth.AddString(strMonth);
	}
	m_comboDateMonth.SetCurSel(MaxTime.GetMonth()-1);

	int nMaxYear = MaxTime.GetYear();
	int nMaxMonth = MaxTime.GetMonth();
	if(nMaxMonth + 1 == 13)
	{
		nMaxMonth = 1;
		nMaxYear += 1;
	}
	else
		nMaxMonth += 1;

	COleDateTime ThisMonthTime(nMaxYear, nMaxMonth, 1, 0, 0, 0);
	COleDateTime LastMonthTime(MaxTime.GetYear(), MaxTime.GetMonth(), 1, 0, 0, 0);
	COleDateTimeSpan SpanMonthTime = ThisMonthTime - LastMonthTime;
	int nMaxDays = SpanMonthTime.GetDays();
	int nDay = 1;
	for(; nDay <= nMaxDays; nDay++)
	{
		CString strDay = "";
		strDay.Format("%d��", nDay);
		m_comboDateDay.AddString(strDay);
	}
	m_comboDateDay.SetCurSel(MaxTime.GetDay()-1);

	m_comboDateYear.SetBorderColor(RGB(209,209,209));
	m_comboDateMonth.SetBorderColor(RGB(209,209,209));
	m_comboDateDay.SetBorderColor(RGB(209,209,209));
	
	ShowFriendInfo();

#ifdef BUSINESS_VESION
	GetDlgItem(IDC_STATIC_CAREER)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_COMBO_CAREER)->ShowWindow(SW_HIDE);
#endif
	return TRUE;
}

void CSelfInfoEditDlg::OnPaint()
{
	CDialog::OnPaint();

	CPaintDC dc(this);	

}

BOOL CSelfInfoEditDlg::OnEraseBkgnd(CDC* pDC)
{
	CDialog::OnEraseBkgnd(pDC);

	// ���ñ���ɫ
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient,RGB(235, 235, 235));	

	return FALSE;
}

// ���ÿؼ���ɫ
HBRUSH CSelfInfoEditDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBkGround;
	}

	return hbr;
}

// ����˱��水ť���򸸴��ڷ���Ϣ
void CSelfInfoEditDlg::OnBnClickedSave()
{
	UpdateData(TRUE);
	FRIEND_INFO user = g_ImUserInfo;

	CString str;
	m_edtName.GetWindowText(str);
	str.Trim();

	if ( str == "" )
	{
		CMessageBox::Prompt("�ǳƲ���Ϊ��!");
		m_edtName.SetFocus();
		return;
	}


	//����
	/*COleDateTime timeBirth;
	m_DateTimeBirth.GetTime(timeBirth);

	CString strDate = timeBirth.Format("%Y%m%d");

	COleDateTime  timeCur = COleDateTime::GetCurrentTime();
	COleDateTime  timeMin(1901,1,1,0,0,0);
	CString strCurDate = timeCur.Format("%Y%m%d");
	CString strMinDate = timeMin.Format("%Y%m%d");

	if(strDate.Compare(strCurDate) <= 0 && strDate.Compare(strMinDate) )
	{
		strncpy(user.birthday,(LPCTSTR)strDate,sizeof(user.birthday));
	}
	else
	{
		CMessageBox::Prompt("��Ч��������!");
		m_DateTimeBirth.SetFocus();
		return;
	}*/

	COleDateTime timeBirth(m_comboDateYear.GetCurSel()+BURTH_START_YEAR, m_comboDateMonth.GetCurSel()+1, m_comboDateDay.GetCurSel()+1,0,0,0);
	CString strDate = timeBirth.Format("%Y%m%d");

	COleDateTime  timeCur = COleDateTime::GetCurrentTime();
	COleDateTime  timeMin(1901,1,1,0,0,0);
	CString strCurDate = timeCur.Format("%Y%m%d");
	CString strMinDate = timeMin.Format("%Y%m%d");

	if(strDate.Compare(strCurDate) <= 0 && strDate.Compare(strMinDate) )
	{
		strncpy(user.birthday,(LPCTSTR)strDate,sizeof(user.birthday));
	}
	else
	{
		CMessageBox::Prompt("��Ч��������!");
		m_DateTimeBirth.SetFocus();
		return;
	}

	/*�����ʽ��֤*/
	/*
	CString strEmail,error;
	m_edtEmail.GetWindowText(strEmail);
	if ( !CheckEmail(strEmail,error) )
	{
		CMessageBox::Prompt(error);
		m_edtEmail.SetFocus();
		return;
	}
	*/
	
	//�ֻ����绰������
	m_edtPhone.GetWindowText(user.mobile, MAX_PHONE_LEN);
	m_edtEmail.GetWindowText(user.email, MAX_EMAIL_LEN);
	
	//�ǳ�
	m_edtName.GetWindowText(user.nickname, MAX_NICKNAME_LEN);

	//�Ա�
	user.sex = m_comboSex.GetCurSel();

	
	//��ҵ
	user.career = m_comboCareer.GetCurSel()+1;

	// ��ַ
	user.province = m_pcomboProvince->GetCurSel() + 1;
	user.city = m_pcomboCity->GetCurSel() + 1;
	user.country = m_pcomboCounty->GetCurSel() + 1;

	// ��������
	user.linkpop = 2 - m_nPublicLevel;

	g_pProtocol->SendReq((XT_USERINFO_MOD_REQ)user);
	m_nLastReqSeq = g_pProtocol->GetCurSendSequnceId();
}

// �����ȡ����ť���򸸴��ڷ���Ϣ
void CSelfInfoEditDlg::OnBnClickedCancel()
{
	::SendMessage(GetParent()->m_hWnd, WM_SELF_INFO_EDIT_DLG_MSG, IDC_BTN_CANCEL, 0);
}

void CSelfInfoEditDlg::OnCbnSelchangeComboProvince()
{
	m_pcomboCity->RemoveAll();

	// ��ȡѡ���ʡID
	int nPid = m_pcomboProvince->GetCurSel()+1;

	// �õ�ʡ�ݽṹ��
	map<int, string> mapCity;
	GetAllCity(nPid, mapCity);
	map<int, string>::iterator it = mapCity.begin();
	for(; it != mapCity.end(); ++it)
	{
		m_pcomboCity->AddString(it->second.c_str());
	}

	m_pcomboCity->SelectItem(0);
}

void CSelfInfoEditDlg::OnCbnSelchangeComboCity()
{
	m_pcomboCounty->RemoveAll();

	// ��ȡѡ���ʡID
	int nPid = m_pcomboProvince->GetCurSel()+1;

	// ��ȡѡ�����ID
	int nCid = m_pcomboCity->GetCurSel()+1;

	// �õ�ʡ�ݽṹ��
	map<int, string> mapDistrict;
	GetAllDistrict(nPid, nCid, mapDistrict);
	map<int, string>::iterator it = mapDistrict.begin();
	for(; it != mapDistrict.end(); ++it)
	{
		m_pcomboCounty->AddString(it->second.c_str());
	}

	m_pcomboCounty->SelectItem(0);
}

// ѡ��ʡ��
void CSelfInfoEditDlg::SelectProvince(int nProvinceID)
{
	m_pcomboProvince->SelectItem(nProvinceID-1);
}

// ѡ����
void CSelfInfoEditDlg::SelectCity(int nCityID)
{
	m_pcomboCity->SelectItem(nCityID-1);
}

// ѡ������
void CSelfInfoEditDlg::SelectDistrict(int nDistrictID)
{
	m_pcomboCounty->SelectItem(nDistrictID-1);
}

void CSelfInfoEditDlg::ShowFriendInfo()
{
	SetDlgItemText(IDC_EDIT_ID,g_ImUserInfo.GetUserName().c_str());
	SetDlgItemText(IDC_EDIT_NAME,g_ImUserInfo.nickname);
	//�Ա�
	m_comboSex.ResetContent();
	m_comboSex.AddString("Ů");
	m_comboSex.AddString("��");
	m_comboSex.SetCurSel(g_ImUserInfo.sex);

	//����
	int nY, nM, nD;
	COleDateTime timeBirth;
	sscanf(g_ImUserInfo.birthday, "%4d%2d%2d", &nY, &nM, &nD); 
	//nY = 1900;
	if(nY >= 1000 && nY <= 3000 
		&& nM >= 1 && nM <= 12
		&& nD >= 1 && nD <= 31) //�ж�ʱ���Ƿ�Ϸ�
	{
		timeBirth = COleDateTime(nY,nM,nD,0,0,0);
	}
	else
	{
		timeBirth = COleDateTime::GetCurrentTime();
	}
	//VERIFY(m_DateTimeBirth.SetTime(timeBirth));

	//ʡ���С���
	SelectProvince(g_ImUserInfo.province);
	SelectCity(g_ImUserInfo.city);
	SelectDistrict(g_ImUserInfo.country);

	//�ֻ����绰��email
	SetDlgItemText(IDC_EDIT_PHONE,g_ImUserInfo.mobile);
	//SetDlgItemText(IDC_EDIT_TEL,g_ImUserInfo.mobile);
	SetDlgItemText(IDC_EDIT_EMAIL,g_ImUserInfo.email);

	//�绰(����)
#ifdef _VER_INTERNAL_
	string strJob, strExt;
	SplitExtAndJob(g_ImUserInfo.flash_url, strExt, strJob);
	SetDlgItemText(IDC_EDIT_TEL,strExt.c_str());
#endif

	//��ҵ
	m_comboCareer.ResetContent();
	for(int i=0; i<10; i++)
	{
		m_comboCareer.AddString(g_cCareers[i]);
	}
	m_comboCareer.SetCurSel(g_ImUserInfo.career - 1);

	if(g_ImUserInfo.linkpop == 0)
	{
		m_nPublicLevel = 2;
	}
	else if(g_ImUserInfo.linkpop == 1)
	{
		m_nPublicLevel = 1;
	}
	else
	{
		m_nPublicLevel = 0;
	}
	UpdateData(FALSE);
}

// �޸ĸ�������ʱ��̨������Ϣ
void CSelfInfoEditDlg::OnUserInfoModAck(const char *data,WORD wRet)
{
	LPXT_USERINFO_MOD_ACK ack = (LPXT_USERINFO_MOD_ACK)data;
	if(wRet != 0)
	{
		CString strMsg;
		strMsg.Format("�޸ĸ�������ʧ�ܣ�%s", ack->error);
		CMessageBox::Model(GetSafeHwnd(),strMsg);		
	}
	else
	{
		SetTimer(TIMER_REFRESH,350,NULL);
	}
}

void CSelfInfoEditDlg::OnUserInfoAck(const char *data,WORD wRet)
{
	//ȷ��������Ӧ�������һ�η��������Ӧ��Ӧ�������֮ǰ������Ӧ������
	if( m_nLastReqSeq != g_pProtocol->GetLastAckSequenceId() )
	{
		return;
	}

	if ( wRet==0 )
	{
		LPXT_USERINFO_GET_ACK ack = (LPXT_USERINFO_GET_ACK)data;

		if ( ack->GetId()==g_ImUserInfo.GetId() )
		{   
			// ������ע��
			//memcpy(&g_ImUserInfo,ack,sizeof(XT_USER_INFO));
			UpdateUserFullInfo(&g_ImUserInfo,ack);
			//�༭֮������е��Լ�Ҳ��Ҫ����
			FRIEND_INFO* pFriend = FindFriend(ack->GetId());
			if(pFriend && pFriend != &g_ImUserInfo)
			{
				//memcpy(pFriend,ack,sizeof(XT_USER_INFO));
				UpdateUserFullInfo(pFriend,ack);
			}
			ShowFriendInfo();
		}
	}
	::SendMessage(GetParent()->m_hWnd, WM_SELF_INFO_EDIT_DLG_MSG, IDC_BTN_SAVE, 0);
}

void CSelfInfoEditDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_REFRESH:
		{
			KillTimer(nIDEvent);
			XT_USERINFO_GET_REQ req;
			req.fid = g_ImUserInfo.GetId();
			g_pProtocol->SendReq(req);
		}
		break;
	default:
		CDialog::OnTimer(nIDEvent);
	}
}


void CSelfInfoEditDlg::OnCbnSelchangeComboDateYear()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nCurrentYear = m_comboDateYear.GetCurSel() + BURTH_START_YEAR;
	int nCurrentMonth = m_comboDateMonth.GetCurSel() + 1;
	int nCurrentDay = m_comboDateDay.GetCurSel() + 1;

	COleDateTime MaxTime();
	COleDateTime ThisMonthTime(nCurrentYear, nCurrentMonth+1, 1, 0, 0, 0);
	COleDateTime LastMonthTime(nCurrentYear, nCurrentMonth, 1, 0, 0, 0);
	COleDateTimeSpan SpanMonthTime = ThisMonthTime - LastMonthTime;
	int nMaxDays = SpanMonthTime.GetDays();
	if(nCurrentDay > nMaxDays)
	{
		m_comboDateDay.SetCurSel(nMaxDays - 1);
	}
	while(m_comboDateDay.GetCount() > nMaxDays)
	{
		m_comboDateDay.DeleteString(m_comboDateDay.GetCount()-1);
	}
	while(m_comboDateDay.GetCount() < nMaxDays)
	{
		CString strDay = "";
		strDay.Format("%d��", m_comboDateDay.GetCount()+1);
		m_comboDateDay.AddString(strDay);
	}
}


void CSelfInfoEditDlg::OnCbnSelchangeComboDateMonth()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	int nCurrentYear = m_comboDateYear.GetCurSel() + BURTH_START_YEAR;
	int nCurrentMonth = m_comboDateMonth.GetCurSel() + 1;
	int nCurrentDay = m_comboDateDay.GetCurSel() + 1;

	COleDateTime MaxTime();
	COleDateTime ThisMonthTime(nCurrentYear, nCurrentMonth+1, 1, 0, 0, 0);
	COleDateTime LastMonthTime(nCurrentYear, nCurrentMonth, 1, 0, 0, 0);
	COleDateTimeSpan SpanMonthTime = ThisMonthTime - LastMonthTime;
	int nMaxDays = SpanMonthTime.GetDays();
	if(nCurrentDay > nMaxDays)
	{
		m_comboDateDay.SetCurSel(nMaxDays - 1);
	}
	while(m_comboDateDay.GetCount() > nMaxDays)
	{
		m_comboDateDay.DeleteString(m_comboDateDay.GetCount()-1);
	}
	while(m_comboDateDay.GetCount() < nMaxDays)
	{
		CString strDay = "";
		strDay.Format("%d��", m_comboDateDay.GetCount()+1);
		m_comboDateDay.AddString(strDay);
	}
}


void CSelfInfoEditDlg::OnCbnSelchangeComboDateDay()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
}

LRESULT CSelfInfoEditDlg::OnAddrComboSelChange(WPARAM wp, LPARAM lp)
{
	if(wp == (WPARAM)m_pcomboProvince)
	{
		OnCbnSelchangeComboProvince();
	}
	if(wp == (WPARAM)m_pcomboCity)
	{
		OnCbnSelchangeComboCity();
	}
	return 0;
}
