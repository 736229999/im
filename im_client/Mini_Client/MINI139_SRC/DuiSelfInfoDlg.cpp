#include "StdAfx.h"
#include "DuiSelfInfoDlg.h"
#include "GdiPlus.h"
#include "Im\impublic.h"
#include "Im\WebProcess.h"
#include "../../../Common/Common.h"
#include "XmlAddress.h"
#include "DuiModHeadIconDlg.h"
#include "im\LocalData.h"

using namespace DuiLib;
#define BURTH_START_YEAR 1901

extern string CutRoundImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE);
extern CString g_cCareers[];

BEGIN_XT_MESSAGE_MAP(CDuiSelfInfoDlg)
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoAck)
	ON_XT_COMMAND(CMD_USERINFO_MOD_ACK, OnUserInfoModAck)
END_XT_MESSAGE_MAP()

CDuiSelfInfoDlg::CDuiSelfInfoDlg(void)
{
	m_nLastReqSeq = 0;
}

CDuiSelfInfoDlg::~CDuiSelfInfoDlg(void)
{

}

DuiLib::UILIB_RESOURCETYPE CDuiSelfInfoDlg::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LPCTSTR CDuiSelfInfoDlg::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

std::tr1::shared_ptr<CDuiSelfInfoDlg> CDuiSelfInfoDlg::pInstance = NULL;
std::tr1::shared_ptr<CDuiSelfInfoDlg> CDuiSelfInfoDlg::Show()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
	}
	else
	{
		pInstance = tr1::shared_ptr<CDuiSelfInfoDlg>(new CDuiSelfInfoDlg());
		pInstance->Create(GetDesktopWindow(), _T("个人资料"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}
	return pInstance;
}

void CDuiSelfInfoDlg::InitWindow() 
{
	SetIcon(IDR_MAINFRAME);
	UpdateBk();

	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_USERINFO_MOD_ACK,m_hWnd);
	m_pDuiModHeadDlg = NULL;

	m_btnHead = static_cast<CButtonGifUI*>(m_PaintManager.FindControl("btnhead"));
	m_btnEdit = static_cast<CButtonUI*>(m_PaintManager.FindControl("editSelf"));
	m_btnSave = static_cast<CButtonUI*>(m_PaintManager.FindControl("editSave"));
	m_btnCancel = static_cast<CButtonUI*>(m_PaintManager.FindControl("editCancel"));
	m_tabSelfInfo = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl("tabs"));
	m_comboSex = static_cast<CComboUI*>(m_PaintManager.FindControl("combosex"));

	DuiLib::CListLabelElementUI* pListElement1 = new DuiLib::CListLabelElementUI;
	pListElement1->SetAttribute("text", "女");
	pListElement1->SetTag(0);
	m_comboSex->Add(pListElement1);

	DuiLib::CListLabelElementUI* pListElement2 = new DuiLib::CListLabelElementUI;
	pListElement2->SetAttribute("text", "男");
	pListElement2->SetTag(1);
	m_comboSex->Add(pListElement2);

	m_comboProvince = static_cast<CComboUI*>(m_PaintManager.FindControl("comboprovince"));
	m_comboCity = static_cast<CComboUI*>(m_PaintManager.FindControl("combocity"));
	m_comboCountry = static_cast<CComboUI*>(m_PaintManager.FindControl("combocountry"));

	m_comboYear = static_cast<CComboUI*>(m_PaintManager.FindControl("comboyear"));
	m_comboMonth = static_cast<CComboUI*>(m_PaintManager.FindControl("combomonth"));
	m_comboDay = static_cast<CComboUI*>(m_PaintManager.FindControl("comboday"));
	
	//设置时间
	COleDateTime MinTime(BURTH_START_YEAR,1,1,0,0,0);
	COleDateTime MaxTime(COleDateTime::GetCurrentTime());
	//m_DateTimeBirth.SetRange(&MinTime,&MaxTime);
	int nYear = MinTime.GetYear();
	int nYearIndex = 0;
	for ( ; nYear <= MaxTime.GetYear(); nYear++)
	{
		CString strYear = "";
		strYear.Format("%d年", nYear);

		DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
		pListElement->SetAttribute("text", strYear);
		pListElement->SetTag(nYearIndex);
		m_comboYear->Add(pListElement);
		nYearIndex++;
	}
	m_comboYear->SelectItem(nYear-MinTime.GetYear() - 1);

	int nMonth = 1;
	for( ; nMonth <= 12; nMonth++)
	{
		CString strMonth = "";
		strMonth.Format("%d月", nMonth);

		DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
		pListElement->SetAttribute("text", strMonth);
		pListElement->SetTag(nMonth - 1);
		m_comboMonth->Add(pListElement);
	}
	m_comboMonth->SelectItem(MaxTime.GetMonth()-1);

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
		strDay.Format("%d日", nDay);

		DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
		pListElement->SetAttribute("text", strDay);
		pListElement->SetTag(nDay - 1);
		m_comboDay->Add(pListElement);
	}
	m_comboDay->SelectItem(MaxTime.GetDay()-1);

	m_comboCareer = static_cast<CComboUI*>(m_PaintManager.FindControl("combocareer"));
	static LPCSTR c_type [] = { 
		"食品","生鲜","数码","家电","日常生活",
		"母婴","图书音像","美妆","服饰箱包",
		"保健","兴趣爱好","工作交流","分享",
		"其他"};

		static int    c_id   [] = {
			101, 102, 103, 104,
			201, 202, 203, 204,
			301, 302, 303, 304,
			401	};

			int n = sizeof(c_id)/sizeof(int);

	for ( int i=0; i<n; i++ )
	{
		DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
		pListElement->SetAttribute("text", c_type[i]);
		pListElement->SetTag(c_id[i]);
		m_comboCareer->Add(pListElement);
	}

	//显示头像
	m_pVLhead = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("VLhead")));
	CString strFaceFile = g_webUserHeadIconProcess.GetFaceFile(&g_ImUserInfo);
    CString strGifFile = strFaceFile + ".gif";////转化GIF
	//CString strFaceFile = g_webUserHeadIconProcess.Url2File((LPTSTR)(LPCTSTR)g_ImUserInfo.face_url);
	string strImg = CutRoundImg(strFaceFile, 88, 88, FALSE);
	strFaceFile = strImg.c_str();
	BOOL bIsGif = FALSE;
	if ( /*m_FriendInfo.status!=XTREAM_OFFLINE &&*/ FileExist(strFaceFile) )
	{
		if(FileExist(strGifFile))
		{
			bIsGif = TRUE;
			//m_btnHead->SetBkImage("");
			//m_btnHead->SetNormalGifFile(strGifFile);
		}
		else
		{
			bIsGif = FALSE;
		 //   LPCSTR szHead = (LPSTR)(LPCSTR)strFaceFile;
			//m_btnHead->SetBkImage(szHead);
		}
	}
	
	m_pLabelNameHead = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("selfnamehead")));
	m_pLabelName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("selfname")));
	m_pLabelSelfSex = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("selfsex")));
	m_pLabelSelfcity = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("selfcity")));
	m_pLabelSelfbirth = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("selfbirth")));
	m_pLabelSelfcellphone = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("selfcellphone")));
	m_pLabelSelftelephone = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("selftelephone")));
	m_pLabelSelfmial = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("selfmial")));
	m_pLabelSelfcareer = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("selfcareer")));
	
	m_pEditName = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editname")));
	m_pEditName->SetMaxChar(20);
	m_pEditName->SetEnabled(false);
#ifdef _WORK_EX_
	m_btnHead->SetEnabled(false);
#endif
	m_pEditCellphone = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editcellphone")));
	m_pEditTelephone = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("edittelephone")));
	m_pEditMial = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editmial")));

	m_pEditCellphone->SetEnabled(false);
	m_pEditCellphone->SetBorderColor(0xFFDEDEDE);
	m_pEditTelephone->SetEnabled(false);
	m_pEditTelephone->SetBorderColor(0xFFDEDEDE);
	m_pEditMial->SetEnabled(false);
	m_pEditMial->SetBorderColor(0xFFDEDEDE);

	// 向Combo加载数据
	map<int, string> mapProvince;
	GetAllProvince(mapProvince);
	map<int, string>::iterator it = mapProvince.begin();
	int nIndexProvince = 0;
	for(; it != mapProvince.end(); ++it)
	{
		//m_comboProvince->AddString(it->second.c_str());

		DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
		pListElement->SetAttribute("text", it->second.c_str());
		pListElement->SetTag(nIndexProvince);
		m_comboProvince->Add(pListElement);
		nIndexProvince++;
	}

	m_pRadio0 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("radio0")));
	m_pRadio1 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("radio1")));
	m_pRadio2 = static_cast<COptionUI*>(m_PaintManager.FindControl(_T("radio2")));

	m_pLevel = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("level"));
	m_pLabelLevel = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("levelLabel"));
	m_pLabelSec   = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("levelSec"));
	m_pLabelThird = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("levelThird"));

	UpdateSelfInfo(bIsGif);

#ifdef _CS_PLAT_EX_  ///飞牛客服平台
	m_btnEdit->SetVisible(false); ////取消"编辑"和"等级"
	m_pLevel->SetVisible(false);
	m_pLabelLevel->SetVisible(false);
	m_pLabelSec->SetVisible(false);
	m_pLabelThird->SetVisible(false);
#endif

#ifdef _WORK_EX_
	m_pLevel->SetVisible(false);
	m_pLabelLevel->SetVisible(false);
	m_pLabelSec->SetVisible(false);
	m_pLabelThird->SetVisible(false);
#endif
}

void CDuiSelfInfoDlg::UpdateSelfInfo(BOOL bGifFlag)
{
	bGifFlag = FALSE;
	CString strFaceFile = g_webUserHeadIconProcess.GetFaceFile(&g_ImUserInfo);
	CString strGifFile = _T("");
	if(bGifFlag)
	{
       strGifFile = strFaceFile + ".gif";////文件改名
	   if ( !FileExist(strGifFile) )////不存在则创建
	   {
		   CopyFile(strFaceFile,strGifFile,FALSE);
	   }
	}
	else
	{
       string strImg = CutRoundImg(strFaceFile, 88, 88, FALSE);
	   strFaceFile = strImg.c_str();
	}

#ifndef _CS_PLAT_EX_  ///飞牛客服平台
	int nNextTime = 0;
	int nLevel = CountLevel(g_ImUserInfo.online_time,nNextTime);
	SetLevelPic(nLevel);////设置图片
	CString strMsg = _T("");
	strMsg.Format("当前在线时长为%d,下一次升级需要时间为%d",g_ImUserInfo.online_time,nNextTime);
	m_pLabelLevel->SetToolTip(strMsg);
	m_pLabelSec->SetToolTip(strMsg);
	m_pLabelThird->SetToolTip(strMsg);
#endif

	if ( /*m_FriendInfo.status!=XTREAM_OFFLINE &&*/ FileExist(strFaceFile))
	{
		if(FileExist(strGifFile))
		{
			m_btnHead->SetBkImage("");
			m_btnHead->SetNormalGifFile(strGifFile);
		}
		else
		{
			LPCSTR szHead = (LPSTR)(LPCSTR)strFaceFile;
			m_btnHead->SetBkImage(szHead);
			m_btnHead->SetNormalGifFile(_T(""));
		}
	}

	m_pLabelName->SetText(g_ImUserInfo.nickname);
	m_pLabelNameHead->SetText(g_ImUserInfo.nickname);

	m_pLabelSelfSex->SetText(SexToStr(g_ImUserInfo.sex));
	// 合成地区
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
	m_pLabelSelfcity->SetText(strAddress.c_str());
	// 生日
	if(strcmp(g_ImUserInfo.birthday, "") != 0)
	{
		int nY, nM, nD;
		sscanf(g_ImUserInfo.birthday, "%4d%2d%2d", &nY, &nM, &nD); 
		CString strBirthday;
		strBirthday.Format("%d年%d月%d日", nY, nM, nD);
		/*SetDlgItemText(IDC_STATIC_BIRTHDAY, strBirthday);*/
		m_pLabelSelfbirth->SetText(strBirthday);
	}	
	// 手机
	//SetDlgItemText(IDC_STATIC_MOBILE_VALUE, g_ImUserInfo.mobile);
	m_pLabelSelfcellphone->SetText(g_ImUserInfo.mobile);
	// 电话
	//SetDlgItemText(IDC_STATIC_TEL_VALUE, g_ImUserInfo.);
	//电话(特殊)
#ifdef _VER_INTERNAL_
	string strJob, strExt;
	SplitExtAndJob(g_ImUserInfo.flash_url, strExt, strJob);
	//SetDlgItemText(IDC_STATIC_TEL_VALUE,strExt.c_str());
	m_pLabelSelftelephone->SetText(strExt.c_str());
#endif
	// 邮箱
	//SetDlgItemText(IDC_STATIC_EMAIL_VALUE, g_ImUserInfo.email);
	m_pLabelSelfmial->SetText(g_ImUserInfo.email);
	// 行业
	CString strCareer = "";
	if(g_ImUserInfo.career > 0 && g_ImUserInfo.career < 10 + 1)
	{
		strCareer = g_cCareers[g_ImUserInfo.career-1];
	}
	//SetDlgItemText(IDC_STATIC_INDUSTRY_VALUE, strCareer);
	m_pLabelSelfcareer->SetText(strCareer);
}

void CDuiSelfInfoDlg::UpdateSelfEditInfo()
{
	m_pEditName->SetText(g_ImUserInfo.nickname);
	m_comboSex->SelectItem(g_ImUserInfo.sex);

	if(strcmp(g_ImUserInfo.birthday, "") == 0)
	{
		m_comboDay->SelectItem(0);
		m_comboMonth->SelectItem(0);
		m_comboYear->SelectItem(0);
	}
	else
	{
		//生日
		int nY, nM, nD;
		sscanf(g_ImUserInfo.birthday, "%4d%2d%2d", &nY, &nM, &nD); 

		if(nY != 0 && nM != 0 && nD !=0)
		{
			m_comboDay->SelectItem(nD - 1);
			m_comboMonth->SelectItem(nM - 1);
			m_comboYear->SelectItem(nY - BURTH_START_YEAR);
		}
		else
		{
			COleDateTime MaxTime(COleDateTime::GetCurrentTime());
			COleDateTime MinTime(BURTH_START_YEAR,1,1,0,0,0);
			m_comboDay->SelectItem(MaxTime.GetDay()-1);
			m_comboMonth->SelectItem(MaxTime.GetMonth()-1);
			m_comboYear->SelectItem(MaxTime.GetYear()-MinTime.GetYear());
		}
	}

	if(g_ImUserInfo.province == 0 && g_ImUserInfo.city == 0 && g_ImUserInfo.country == 0)
	{
		m_comboProvince->SelectItem(0);
		m_comboCity->SelectItem(0);
		m_comboCountry->SelectItem(0);
	}
	else
	{
		m_comboProvince->SelectItem(g_ImUserInfo.province-1);
		m_comboCity->SelectItem(g_ImUserInfo.city-1);
		m_comboCountry->SelectItem(g_ImUserInfo.country-1);
	}


	//手机、电话、email
	m_pEditCellphone->SetText(g_ImUserInfo.mobile);
	m_pEditMial->SetText(g_ImUserInfo.email);

	//电话(特殊)
#ifdef _VER_INTERNAL_
	string strJob, strExt;
	SplitExtAndJob(g_ImUserInfo.flash_url, strExt, strJob);
	//SetDlgItemText(IDC_EDIT_TEL,strExt.c_str());
	m_pEditTelephone->SetText(strExt.c_str());
#endif
	if(g_ImUserInfo.career == 0)
		m_comboCareer->SelectItem(0);
	else
		m_comboCareer->SelectItem(g_ImUserInfo.career - 1);

	if(g_ImUserInfo.linkpop == 0)
	{
		m_pRadio0->Selected(true);
		m_pRadio1->Selected(false);
		m_pRadio2->Selected(false);
	}
	else if(g_ImUserInfo.linkpop == 1)
	{
		m_pRadio0->Selected(false);
		m_pRadio1->Selected(true);
		m_pRadio2->Selected(false);
	}
	else
	{
		m_pRadio0->Selected(false);
		m_pRadio1->Selected(false);
		m_pRadio2->Selected(true);
	}
}

BOOL CDuiSelfInfoDlg::SaveSelfEditInfo()
{
	FRIEND_INFO user = g_ImUserInfo;

	CString str;
	str = m_pEditName->GetText();
	str.Trim();

	if ( str == "" )
	{
		CMessageBox::Prompt("昵称不能为空!");
		return FALSE;
	}


	//生日
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
		CMessageBox::Prompt("无效生日设置!");
		m_DateTimeBirth.SetFocus();
		return;
	}*/

	COleDateTime timeBirth(m_comboYear->GetCurSel()+BURTH_START_YEAR, m_comboMonth->GetCurSel()+1, m_comboDay->GetCurSel()+1,0,0,0);
	CString strDate = timeBirth.Format("%Y%m%d");

	COleDateTime  timeCur = COleDateTime::GetCurrentTime();
	COleDateTime  timeMin(1900,12,31,0,0,0);
	CString strCurDate = timeCur.Format("%Y%m%d");
	CString strMinDate = timeMin.Format("%Y%m%d");

	if(strDate.Compare(strCurDate) <= 0 && strDate.Compare(strMinDate) )
	{
		strncpy(user.birthday,(LPCTSTR)strDate,sizeof(user.birthday));
	}
	else
	{
		CMessageBox::Prompt("无效生日设置!");
		return FALSE;
	}

	/*邮箱格式验证*/
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
	
	//手机、电话、邮箱
	str = m_pEditCellphone->GetText();
	strncpy(user.mobile, (LPSTR)(LPCSTR)str, MAX_PHONE_LEN);

	str = m_pEditMial->GetText();
	strncpy(user.email, (LPSTR)(LPCSTR)str, MAX_EMAIL_LEN);
	
	//昵称
	str = m_pEditName->GetText();
	strncpy(user.nickname, (LPSTR)(LPCSTR)str, MAX_NICKNAME_LEN);

	//性别
	user.sex = m_comboSex->GetCurSel();

	
	//行业
	user.career = m_comboCareer->GetCurSel()+1;

	// 地址
	user.province = m_comboProvince->GetCurSel() + 1;
	user.city = m_comboCity->GetCurSel() + 1;
	user.country = m_comboCountry->GetCurSel() + 1;

	// 公开级别
	//user.linkpop = 2 - m_nPublicLevel;

	if(m_pRadio0->IsSelected())
	{
		user.linkpop = 0;
	}
	else if(m_pRadio1->IsSelected())
	{
		user.linkpop = 1;
	}
	else if(m_pRadio2->IsSelected())
	{
		user.linkpop = 2;
	}
	g_pProtocol->SendReq((XT_USERINFO_MOD_REQ)user);
	m_nLastReqSeq = g_pProtocol->GetCurSendSequnceId();
}

LRESULT CDuiSelfInfoDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	if(WM_XT_CMD_NOTIFY == uMsg)
	{
		OnXtMessage(wParam, lParam);
	}

	if(WM_SIZE == uMsg)
	{
		UpdateBk();
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CDuiSelfInfoDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender == m_btnHead)
		{
// 			if(m_pDuiModHeadDlg == NULL || m_pDuiModHeadDlg->GetHWND() == NULL)
// 			{
// 				m_pDuiModHeadDlg = new CDuiModHeadIconDlg();
// 				m_pDuiModHeadDlg->Create(GetDesktopWindow(), _T("修改头像"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
// 			}
// 			m_pDuiModHeadDlg->ShowWindow(true);
// 			m_pDuiModHeadDlg->CenterWindow();
// 			::BringWindowToTop(m_pDuiModHeadDlg->GetHWND());
			CDuiModHeadIconDlg::Show();
		}
		if(msg.pSender == m_btnEdit)
		{
			m_tabSelfInfo->SelectItem(1);

// 			m_comboSex->SelectItem(0);
// 
// 
// 			// 向Combo加载数据
// 			map<int, string> mapProvince;
// 			GetAllProvince(mapProvince);
// 			map<int, string>::iterator it = mapProvince.begin();
// 			int nIndexProvince = 0;
// 			for(; it != mapProvince.end(); ++it)
// 			{
// 				//m_comboProvince->AddString(it->second.c_str());
// 
//  				DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
//  				pListElement->SetAttribute("text", it->second.c_str());
//  				pListElement->SetTag(nIndexProvince);
//  				m_comboProvince->Add(pListElement);
//  				nIndexProvince++;
// 			}
// 			m_comboProvince->SelectItem(0);
// 
// 			//设置时间
// 			COleDateTime MinTime(BURTH_START_YEAR,1,1,0,0,0);
// 			COleDateTime MaxTime(COleDateTime::GetCurrentTime());
// 			m_comboYear->SelectItem(MaxTime.GetYear()-MinTime.GetYear());
// 			m_comboMonth->SelectItem(MaxTime.GetMonth()-1);
// 			m_comboDay->SelectItem(MaxTime.GetDay()-1);

			UpdateSelfEditInfo();
		}
		else if(msg.pSender == m_btnSave)
		{
			SaveSelfEditInfo();
			m_tabSelfInfo->SelectItem(0);
		}
		else if(msg.pSender == m_btnCancel)
		{
			m_tabSelfInfo->SelectItem(0);
		}
	}
	else if(msg.sType == DUI_MSGTYPE_ITEMSELECT)
	{
		if(msg.pSender == m_comboYear)
		{
			int nCurrentYear = m_comboYear->GetCurSel() + BURTH_START_YEAR;
			int nCurrentMonth = m_comboMonth->GetCurSel() + 1;
			int nCurrentDay = m_comboDay->GetCurSel() + 1;

			if(nCurrentMonth == 0)
				nCurrentMonth = 1;
			if(nCurrentDay == 0)
				nCurrentDay = 1;
			COleDateTime MaxTime();

			COleDateTime LastMonthTime(nCurrentYear, nCurrentMonth, 1, 0, 0, 0);
			int nThisYear = nCurrentYear;
			int nThisMonth = nCurrentMonth+1;
			if(nCurrentMonth == 12)
			{
				nThisYear = nThisYear + 1;
				nThisMonth = 1;
			}

			COleDateTime ThisMonthTime(nThisYear, nThisMonth, 1, 0, 0, 0);
			COleDateTimeSpan SpanMonthTime = ThisMonthTime - LastMonthTime;
			int nMaxDays = SpanMonthTime.GetDays();
			if(nCurrentDay > nMaxDays)
			{
				m_comboDay->SelectItem(nMaxDays - 1);
			}
			while(m_comboDay->GetCount() > nMaxDays)
			{
				m_comboDay->RemoveAt(m_comboDay->GetCount()-1);
			}
			while(m_comboDay->GetCount() < nMaxDays)
			{
				CString strDay = "";
				strDay.Format("%d日", m_comboDay->GetCount()+1);

				DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
				pListElement->SetAttribute("text", strDay);
				pListElement->SetTag(m_comboDay->GetCount());
				m_comboDay->Add(pListElement);
			}
		}
		else if(msg.pSender == m_comboMonth)
		{
			int nCurrentYear = m_comboYear->GetCurSel() + BURTH_START_YEAR;
			int nCurrentMonth = m_comboMonth->GetCurSel() + 1;
			int nCurrentDay = m_comboDay->GetCurSel() + 1;

			COleDateTime MaxTime();
			

			COleDateTime LastMonthTime(nCurrentYear, nCurrentMonth, 1, 0, 0, 0);
			int nThisYear = nCurrentYear;
			int nThisMonth = nCurrentMonth+1;
			if(nCurrentMonth == 12)
			{
				nThisYear = nThisYear + 1;
				nThisMonth = 1;
			}
			COleDateTime ThisMonthTime(nThisYear, nThisMonth, 1, 0, 0, 0);
			COleDateTimeSpan SpanMonthTime = ThisMonthTime - LastMonthTime;
			int nMaxDays = SpanMonthTime.GetDays();
			if(nCurrentDay > nMaxDays)
			{
				m_comboDay->SelectItem(nMaxDays - 1);
			}
			while(m_comboDay->GetCount() > nMaxDays)
			{
				m_comboDay->RemoveAt(m_comboDay->GetCount()-1);
			}
			while(m_comboDay->GetCount() < nMaxDays)
			{
				CString strDay = "";
				strDay.Format("%d日", m_comboDay->GetCount()+1);
				DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
				pListElement->SetAttribute("text", strDay);
				pListElement->SetTag(m_comboDay->GetCount());
				m_comboDay->Add(pListElement);
			}
		}
		else if(msg.pSender == m_comboDay)
		{

		}
		else if(msg.pSender == m_comboProvince)
		{
			m_comboCity->RemoveAll();

			// 获取选择的省ID
			int nPid = m_comboProvince->GetCurSel()+1;

			// 得到省份结构体
			map<int, string> mapCity;
			GetAllCity(nPid, mapCity);
			map<int, string>::iterator it = mapCity.begin();
			for(; it != mapCity.end(); ++it)
			{
				DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
				pListElement->SetAttribute("text", it->second.c_str());
				pListElement->SetTag(m_comboCity->GetCount());
				m_comboCity->Add(pListElement);
			}
			m_comboCity->SelectItem(0);
		}
		else if(msg.pSender == m_comboCity)
		{
			m_comboCountry->RemoveAll();

			// 获取选择的省ID
			int nPid = m_comboProvince->GetCurSel()+1;

			// 获取选择的市ID
			int nCid = m_comboCity->GetCurSel()+1;

			// 得到省份结构体
			map<int, string> mapDistrict;
			GetAllDistrict(nPid, nCid, mapDistrict);
			map<int, string>::iterator it = mapDistrict.begin();
			for(; it != mapDistrict.end(); ++it)
			{
				DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
				pListElement->SetAttribute("text", it->second.c_str());
				pListElement->SetTag(m_comboCountry->GetCount());
				m_comboCountry->Add(pListElement);
			}

			m_comboCountry->SelectItem(0);
		}
		else if(msg.pSender == m_comboCountry)
		{

		}

	}
}

void CDuiSelfInfoDlg::OnUserInfoAck(const char *data,WORD wRet)
{
	//确保回来的应答是最后一次发的请求对应的应答，如果是之前的请求应答则丢弃
	//if( m_nLastReqSeq != g_pProtocol->GetLastAckSequenceId() )
	//{
	//	return;
	//}

	if ( wRet==0 )
	{
		LPXT_USERINFO_GET_ACK ack = (LPXT_USERINFO_GET_ACK)data;

		if ( ack->GetId()==g_ImUserInfo.GetId() )
		{   
			// 更新头像
			//memcpy(&g_ImUserInfo,ack,sizeof(XT_USER_INFO));
			char password[255];
			strcpy(password,g_ImUserInfo.GetPassword().c_str());
			UpdateUserFullInfo(&g_ImUserInfo,ack);
			g_ImUserInfo.SetPassword(password);
			//CString strLocalPath = g_webUserHeadIconProcess.Url2File((LPTSTR)(LPCTSTR)g_ImUserInfo.face_url);
			//m_pFacePic->SetPicPath(strLocalPath);
			CString strFacceUrl = (char *)ack->face_url;
			BOOL bGifFlag = FALSE;
		    if(strFacceUrl.Find(".gif") != string::npos)////是否为gif显示
			{
				bGifFlag = TRUE;
			}
			UpdateSelfInfo(bGifFlag);
		}
	}
}

// 修改个人资料时后台返回消息
void CDuiSelfInfoDlg::OnUserInfoModAck(const char *data,WORD wRet)
{
	LPXT_USERINFO_MOD_ACK ack = (LPXT_USERINFO_MOD_ACK)data;
	if(wRet == 0)
	{
		//SetTimer(TIMER_REFRESH,350,NULL);
		Sleep(50);
		XT_USERINFO_GET_REQ req;
		req.fid = g_ImUserInfo.GetId();
		g_pProtocol->SendReq(req);
	}
}

void CDuiSelfInfoDlg::UpdateBk()
{
	if(::IsWindow(GetHWND()) && m_PaintManager.GetRoot() != NULL)
	{
		DuiLib::CControlUI* background = m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			string strBkImage = g_LocalData.GetBkImage(g_ImUserInfo.GetUserName().c_str());
			BOOL bSolid = strBkImage.find('c') != string::npos;

			if(bSolid)
			{
				RECT rc;
				GetWindowRect(GetHWND(), &rc);

				TCHAR szBuf[MAX_PATH] = {0};
				_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,18,18'"), strBkImage.c_str());
				background->SetBkImage(szBuf);
			}
			else
			{
				RECT rc;
				GetWindowRect(GetHWND(), &rc);
				int nWidth = rc.right - rc.left;
				TCHAR szBuf[MAX_PATH] = {0};
				if(nWidth <= 600)
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,%d,150' dest='0,0,%d,150'"), strBkImage.c_str(), rc.right-rc.left, rc.right-rc.left);
				}
				else
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,600,160' dest='0,0,%d,150' corner='580,0,0,0'"), strBkImage.c_str(), nWidth);
				}
				background->SetBkImage(szBuf);
			}
		}
	}
}

void CDuiSelfInfoDlg::SetLevelPic(int nLevel)
{
	switch(nLevel)
	{
	case 1:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetVisible(false);
			m_pLabelThird->SetVisible(false);
		}
		break;
	case 2:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetVisible(true);
			m_pLabelThird->SetVisible(false);
		}
		break;
	case 3:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelThird->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetVisible(true);
			m_pLabelThird->SetVisible(true);
		}
		break;
	case 4:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelSec->SetVisible(false);
			m_pLabelThird->SetVisible(false);
		}
		break;
	case 5:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelSec->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelSec->SetVisible(true);
			m_pLabelThird->SetVisible(false);
		}
		break;
	case 6:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelSec->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelThird->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelSec->SetVisible(true);
			m_pLabelThird->SetVisible(true);
		}
		break;
	default:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetVisible(false);
			m_pLabelThird->SetVisible(false);
		}
		break;
	}
}