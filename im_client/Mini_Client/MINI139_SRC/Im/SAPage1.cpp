// SAPage1.cpp : 实现文件
//

#include "stdafx.h"
#include "..\imclient.h"
#include ".\sapage1.h"
#include "impublic.h"

#include "webprocess.h"
#include "SelfFaceDlg.h"

IMPLEMENT_DYNAMIC(CSAPage1, CSABasePage)
CSAPage1::CSAPage1(CWnd* pParent /*=NULL*/)
	: CSABasePage(CSAPage1::IDD, pParent)
{
	m_bFaceModified = false;
	Create(CSAPage1::IDD,pParent);
}

CSAPage1::~CSAPage1()
{
}

void CSAPage1::DoDataExchange(CDataExchange* pDX)
{
	CSABasePage::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_STATIC_FACE,m_wndFaceIconCtrl);
	DDX_Control(pDX,IDC_COMBO_SEX, m_comboSex);
	DDX_Control(pDX,IDC_EDIT_AGE, m_editAge);
	//DDX_Control(pDX,IDC_EDIT_CAREER,m_editCareer);
	DDX_Control(pDX,IDC_EDIT_NICKNAME,m_editNickName);
	DDX_Control(pDX,IDC_EDIT_USERNAME,m_editUserName);
	DDX_Control(pDX,IDC_EDIT_SELFINFO,m_editSelfInfo);
	DDX_Control(pDX, IDC_BUTTON_SELFACE, m_btnSelFace);
	DDX_Control(pDX, IDC_STATIC_LEVEL, m_userLevel);
}


BEGIN_MESSAGE_MAP(CSAPage1, CSABasePage)
	ON_BN_CLICKED(IDC_BUTTON_SELFACE, OnBnClickedBtnFacesel)
	ON_MESSAGE(WM_FACESEL_NOTIFY,OnFaceSelNotify)
	ON_CBN_SELCHANGE(IDC_COMBO_SEX, OnCbnSelchangeComboSex)
	ON_MESSAGE(WM_USER_HEAD_NOTIFY,OnWebIconDownload)
	ON_COMMAND(IDC_STATIC_FACE,OnBnClickedBtnFacesel)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CSAPage1 消息处理程序

void CSAPage1::OnBnClickedBtnFacesel()
{
	CSelfFaceDlg * pDlg = CSelfFaceDlg::Show();
	pDlg->m_hSAPageWnd  = m_hWnd;	
}


LRESULT CSAPage1::OnFaceSelNotify(WPARAM wParam,LPARAM lParam)
{
	if ( lParam==IDOK )
	{
		CString strNewUrl;

		CSelfFaceDlg *pDlg = CSelfFaceDlg::GetInstance();
		pDlg->GetFaceUrl(strNewUrl);

		CString s1 = m_strFaceUrl;
		s1.Trim();
		s1.MakeLower();

		CString s2 = strNewUrl;
		s2.Trim();
		s2.MakeLower();

		if ( s1!=s2 )
		{
			m_bFaceModified=true;
			m_bModified = true;
			m_strFaceUrl = strNewUrl;

			XT_USER_INFO info;
			info.SetId(g_ImUserInfo.GetId());
			strcpy(info.face_url,strNewUrl);

			g_webUserHeadIconProcess.GetUserHeadIcon(&info,this);
		}	
	}	

	return 1;
}

BOOL CSAPage1::OnInitDialog()
{
	CSABasePage::OnInitDialog();

	m_editAge.SetWindowText("0");
	m_comboSex.ResetContent();

	m_comboSex.AddString("女");
	m_comboSex.AddString("男");
	//m_comboSex.AddString("-");		//阿波罗版禁止了未确定项
    
	m_editAge.LimitText(3);
	m_editAge.EnablePaste(false);

	m_editNickName.LimitText(23);
	//m_editCareer.LimitText(59);
	m_editSelfInfo.LimitText(254);

	m_userLevel.SetOnlineTime(g_ImUserInfo.online_time);

	return TRUE; 
}

void CSAPage1::SetUserInfo(const XT_USER_INFO &info)
{
	m_editSelfInfo.SetWindowText(info.description);
	//m_wndFaceIconCtrl.SetFace(info.face);

	g_webUserHeadIconProcess.GetUserHeadIcon(&info,this);
	m_wndFaceIconCtrl.SetSex(info.sex,XTREAM_ONLINE);

	m_comboSex.SetCurSel(info.sex);

	m_editAge.SetWindowText(info.age==0 ? "" : IntToStr(info.age) );

	m_editAge.SetWindowText( UIntToStr(((XT_USER_INFO *)&info)->GetId()) );

	m_strFaceUrl = info.face_url;

	m_editUserName.SetWindowText(((XT_USER_INFO *)&info)->GetUserName().c_str());
	m_editNickName.SetWindowText(info.nickname);
//	m_editCareer.SetWindowText(info.career);
}

int CSAPage1::GetUserInfo(XT_USER_INFO &info)
{
	CString str;
	m_editNickName.GetWindowText(str);
	str.Trim();

	if ( str=="" )
	{
		//MessageBox("昵称不能为空!",IM_CAPTION,MB_OK|MB_ICONWARNING);
		CMessageBox::Prompt("昵称不能为空!");
		m_editNickName.SetFocus();
		return 0;
	}
	
	m_editNickName.GetWindowText(info.nickname,MAX_STR_LEN);
//	m_editCareer.GetWindowText(info.career,MAX_STR_LEN);
	m_editSelfInfo.GetWindowText(info.description,MAX_STR_LEN);

	info.sex = m_comboSex.GetCurSel();
	
	m_editAge.GetWindowText(str);
	info.age = atoi(str);
	strcpy(info.face_url,m_strFaceUrl);

	if ( info.age>255 )
		info.age=0;

//	this->m_bModified = false;

	return 1;
}

void CSAPage1::OnCbnSelchangeComboSex()
{
	//int nSel = this->m_comboSex.GetCurSel();

	//if ( nSel==0 )
	//	this->m_wndFaceIconCtrl.SetSex(0);
	//else
	//	this->m_wndFaceIconCtrl.SetSex(1);
}

LRESULT CSAPage1::OnWebIconDownload(WPARAM w,LPARAM l)
{
	char * szFileIcon = (char *)l;
	uint32 fid        = w;

	if ( w==g_ImUserInfo.GetId() )
	{
		if ( szFileIcon )
			this->m_wndFaceIconCtrl.SetFaceFile(szFileIcon,XTREAM_ONLINE);
	}

	return 0;
}

void CSAPage1::OnTimer(UINT nIDEvent)
{
	CSABasePage::OnTimer(nIDEvent);

	if ( nIDEvent==0x01 )
	{
		KillTimer(0x01);
		this->OnBnClickedBtnFacesel();
	}
}
