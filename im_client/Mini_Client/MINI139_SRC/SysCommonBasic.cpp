// SysCommonBasic.cpp : 实现文件
//

#include "stdafx.h"
#include "SysCommonBasic.h"
#include "afxdialogex.h"
using namespace DuiLib;

CString GetRegisterValue(HKEY hkey, LPCTSTR Rgspath, LPCTSTR ValueName, DWORD type)
{
	CString str=_T("");
	unsigned char path[255] ;
	HKEY hKEY;   
	LONG ret = RegOpenKeyEx(hkey, Rgspath, 0, KEY_READ, &hKEY); 

	if(ret != ERROR_SUCCESS)   
	{   
		RegCloseKey(hKEY);   
		return str;   
	}   
	// 读取键值内容    
	DWORD dwInfoSize = 255;   
	ret = RegQueryValueEx(hKEY, ValueName, NULL, &type, (BYTE*)path, &dwInfoSize);   
	if(ret!=ERROR_SUCCESS)  
	{      
		RegCloseKey(hKEY);   
		return str;   
	}
	return (char*)path;
}


void CSysCommonBasic::InitWindow()
{
	WindowImplBase::InitWindow();
	m_pCheckStartRun = static_cast<COptionUI*>(m_PaintManager.FindControl("StartRun"));
	m_pCheckExitConfirm = static_cast<COptionUI*>(m_PaintManager.FindControl("ExitConfirm"));
	m_pCheckEndgeHide = static_cast<COptionUI*>(m_PaintManager.FindControl("EndgeHide"));
	m_pCheckFrontShow = static_cast<COptionUI*>(m_PaintManager.FindControl("FrontShow"));

	///////初始化设置///////////////////
	WORD nWarn;
	g_LocalData.GetExitWarnConfig(nWarn); //退出时告警
	if(nWarn!=0)
	{
		m_pCheckExitConfirm->Selected(TRUE);
	}
	
	WORD nTop;
	g_LocalData.GetTopShowConfig(nTop); //最前显示
	if(nTop!=0)
	{
	    m_pCheckFrontShow->Selected(TRUE);
	}
	
	WORD nHide;
	g_LocalData.GetHideWinConfig(nHide); //边缘隐藏
	if(nHide!=0)
	{
		m_pCheckEndgeHide->Selected(TRUE);
	}
	
	// 获取注册表的值
	string strKeyName = "moumou";
#ifdef _BUSINESS_VESION
	strKeyName = "moumou_business";
#elif _VER_INTERNAL_
	strKeyName = "moumou_internal";
#endif
#ifdef _CS_PLAT_EX_    
	strKeyName = "moumou_cs";
#endif
	CString strMoumoupath = GetRegisterValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", strKeyName.c_str(), REG_SZ);
	
	m_pCheckStartRun->Selected(!strMoumoupath.IsEmpty());
}


// 消息
void CSysCommonBasic::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
}


void CSysCommonBasic::SaveLoginSetting()
{
	///////设置注册表自动启动///////////////////////////////
	    /*	UpdateData(TRUE);*/
		RegAccountStruct r;
		HKEY hKey = NULL;
		LONG ret ;
		LPCTSTR data_Set = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
		ret = ::RegOpenKeyEx(HKEY_CURRENT_USER,data_Set,0,KEY_SET_VALUE,&hKey);
	
		DWORD dwType = REG_SZ;
		DWORD dwSize = 255;
	
		string strKeyName = "moumou";
#ifdef _BUSINESS_VESION
		strKeyName = "moumou_business";
#elif _VER_INTERNAL_
		strKeyName = "moumou_internal";
#endif
#ifdef _CS_PLAT_EX_    
		strKeyName = "moumou_cs";
#endif
		if ( m_pCheckStartRun->IsSelected())
		{
			char szTemp[255];
			GetModuleFileName(NULL,szTemp,255);
			::RegSetValueEx(hKey,strKeyName.c_str(),NULL,REG_SZ,(LPBYTE)szTemp,strlen(szTemp));
		}
		else
		{
			::RegDeleteValue(hKey,strKeyName.c_str());
	
		}
		::RegCloseKey(hKey);	
	
		WORD nWarn = 0;
	//////退出页面时告警/////////////////////////////////////
		if ( m_pCheckExitConfirm->IsSelected() )
		{
			nWarn = 1;
			g_LocalData.SaveExitWarnConfig(nWarn);
		}
		else
		{
			nWarn = 0;
			g_LocalData.SaveExitWarnConfig(nWarn);
		}
	//////设置最前端显示/////////////////////////////////////
		CRect rcMy;
		WORD nTop = 0;
		::GetWindowRect(CMainFrame::GetInstance()->GetHWND(),rcMy);//得到当前的窗口位置
		if ( m_pCheckFrontShow->IsSelected())
		{
			////设为总在最前
			SetWindowPos(CMainFrame::GetInstance()->GetHWND(), CWnd::wndTopMost.GetSafeHwnd(),rcMy.left,rcMy.top,rcMy.Size().cx,rcMy.Size().cy,SWP_SHOWWINDOW);
		    nTop = 1;
			g_LocalData.SaveTopShowConfig(nTop);
		}
		else
		{
			////取消总在最前
			SetWindowPos(CMainFrame::GetInstance()->GetHWND(), CWnd::wndNoTopMost.GetSafeHwnd(),rcMy.left,rcMy.top,rcMy.Size().cx,rcMy.Size().cy,SWP_SHOWWINDOW);
			nTop = 0;
			g_LocalData.SaveTopShowConfig(nTop);
		}
		
		///////主窗口位于边缘处是否隐藏//////////////////////////////////////
		WORD nHide = 0;
		if ( m_pCheckEndgeHide->IsSelected())
		{
			SetTimer(CMainFrame::GetInstance()->GetHWND(), TIMER_HIDE,AUOT_CHECK_TIME,NULL);
			nHide = 1;
			g_LocalData.SaveHideWinConfig(nHide);//是否自动隐藏
			
		}
		else
		{
			KillTimer(CMainFrame::GetInstance()->GetHWND(), TIMER_HIDE);
			nHide = 0;
			g_LocalData.SaveHideWinConfig(nHide);//是否自动隐藏
			
		}
	
}



// CSysCommonBasic 对话框
////////
////////IMPLEMENT_DYNAMIC(CSysCommonBasic, CDialog)
////////
////////CSysCommonBasic::CSysCommonBasic(CWnd* pParent /*=NULL*/)
////////	: CDialog(CSysCommonBasic::IDD, pParent)
////////{
////////	m_bAutoRun = FALSE;
////////}
////////
////////CSysCommonBasic::~CSysCommonBasic()
////////{
////////}
////////
////////void CSysCommonBasic::DoDataExchange(CDataExchange* pDX)
////////{
////////	CDialog::DoDataExchange(pDX);
////////	DDX_Control(pDX, IDC_CHECK_AUTORUN, m_btnCheckAutoStart);
////////	DDX_Control(pDX, IDC_CHECK_HIDE, m_btnCheckHide);
////////	DDX_Control(pDX, IDC_CHECK_TOP, m_btnCheckShowTop);
////////	DDX_Control(pDX, IDC_CHECK_EXIT, m_btnCheckExitWarn);
////////	DDX_Check(pDX, IDC_CHECK_AUTORUN, m_bAutoRun);
////////}
////////
////////
////////BEGIN_MESSAGE_MAP(CSysCommonBasic, CDialog)
////////	ON_WM_CTLCOLOR()
////////	ON_BN_CLICKED(IDC_CHECK_EXIT, &CSysCommonBasic::OnBnClickedCheckExit)
////////	ON_BN_CLICKED(IDC_CHECK_AUTORUN, &CSysCommonBasic::OnBnClickedCheckAutorun)
////////	ON_BN_CLICKED(IDC_CHECK_TOP, &CSysCommonBasic::OnBnClickedCheckTop)
////////END_MESSAGE_MAP()
////////
////////
////////CString GetRegisterValue(HKEY hkey, LPCTSTR Rgspath, LPCTSTR ValueName, DWORD type)
////////{
////////	CString str=_T("");
////////	unsigned char path[255] ;
////////	HKEY hKEY;   
////////	LONG ret = RegOpenKeyEx(hkey, Rgspath, 0, KEY_READ, &hKEY); 
////////
////////	if(ret != ERROR_SUCCESS)   
////////	{   
////////		RegCloseKey(hKEY);   
////////		return str;   
////////	}   
////////	// 读取键值内容    
////////	DWORD dwInfoSize = 255;   
////////	ret = RegQueryValueEx(hKEY, ValueName, NULL, &type, (BYTE*)path, &dwInfoSize);   
////////	if(ret!=ERROR_SUCCESS)  
////////	{      
////////		RegCloseKey(hKEY);   
////////		return str;   
////////	}
////////
////////	return (char*)path;
////////}
////////
////////
////////// CSysCommonBasic 消息处理程序
////////BOOL CSysCommonBasic::OnInitDialog()
////////{
////////	CDialog::OnInitDialog();
////////
////////	// 遍历所有子控件，设置控件字体
////////	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
////////	while(hWndChild)  
////////	{  
////////		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
////////		if(GetDlgItem(IDC_STATIC_LOGIN)->GetSafeHwnd() == hWndChild 
////////			|| GetDlgItem(IDC_STATIC_MAINFORM)->GetSafeHwnd() == hWndChild )
////////		{
////////			::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12B.GetSafeHandle(), TRUE);
////////		}
////////		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
////////	}
////////
////////	WORD nWarn;
////////	g_LocalData.GetExitWarnConfig(nWarn); //退出告警
////////	if(nWarn!=0)
////////	{
////////		m_btnCheckExitWarn.SetCheck(TRUE);
////////	}
////////
////////	WORD nTop;
////////	g_LocalData.GetTopShowConfig(nTop); //最前显示
////////	if(nTop!=0)
////////	{
////////       m_btnCheckShowTop.SetCheck(TRUE);
////////	}
////////
////////	WORD nHide;
////////	g_LocalData.GetHideWinConfig(nHide); //最前显示
////////	if(nHide!=0)
////////	{
////////	    m_btnCheckHide.SetCheck(TRUE);
////////	}
////////
////////	// 获取注册表的值
////////	string strKeyName = "moumou";
////////#ifdef _BUSINESS_VESION
////////	strKeyName = "moumou_business";
////////#endif
////////	CString strMoumoupath = GetRegisterValue(HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", strKeyName.c_str(), REG_SZ);
////////
////////	m_btnCheckAutoStart.SetCheck(!strMoumoupath.IsEmpty());
////////
////////	return TRUE;
////////}
////////
////////HBRUSH CSysCommonBasic::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
////////{
////////	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
////////
////////	if ( nCtlColor==CTLCOLOR_STATIC )
////////	{
////////		UINT nId = pWnd->GetDlgCtrlID();
////////
////////		if ( nId==IDC_EDIT_USERNAME || nId==IDC_EDIT_NICKNAME || nId==IDC_EDIT_EMAIL )
////////		{
////////
////////		}
////////		else
////////		{
////////			pDC->SetBkMode(TRANSPARENT);
////////			hbr = g_hWhiteBrush;
////////		}
////////
////////		if ( nId==IDC_STATIC_GROUP )
////////		{
////////			pDC->SetTextColor(RGB(255,0,0));//（255,0，0） //(74,123,211)
////////		}
////////		else
////////		{
////////			pDC->SetTextColor(0x2e2e2e);
////////		}
////////	}
////////
////////	if ( nCtlColor==CTLCOLOR_DLG  )
////////	{
////////		hbr = g_hWhiteBrush;
////////	}
////////
////////	return hbr;
////////}
////////
////////
////////void CSysCommonBasic::OnBnClickedCheckExit()
////////{
////////	// TODO: 在此添加控件通知处理程序代码
////////}
////////
////////
////////void CSysCommonBasic::OnBnClickedCheckAutorun()
////////{
////////	// TODO: 在此添加控件通知处理程序代码
////////}
////////
////////int CSysCommonBasic::SaveLoginSetting()
////////{
/////////////////设置注册表自动启动///////////////////////////////
////////	UpdateData(TRUE);
////////
////////	RegAccountStruct r;
////////	HKEY hKey = NULL;
////////	LONG ret ;
////////	LPCTSTR data_Set = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
////////	ret = ::RegOpenKeyEx(HKEY_CURRENT_USER,data_Set,0,KEY_SET_VALUE,&hKey);
////////
////////	DWORD dwType = REG_SZ;
////////	DWORD dwSize = 255;
////////
////////	string strKeyName = "moumou";
////////#ifdef _BUSINESS_VESION
////////	strKeyName = "moumou_business";
////////#endif
////////
////////	if ( m_bAutoRun )
////////	{
////////		m_btnCheckAutoStart.SetCheck(TRUE);
////////		char szTemp[255];
////////		GetModuleFileName(NULL,szTemp,255);
////////		::RegSetValueEx(hKey,strKeyName.c_str(),NULL,REG_SZ,(LPBYTE)szTemp,strlen(szTemp));
////////	}
////////	else
////////	{
////////		::RegDeleteValue(hKey,strKeyName.c_str());
////////
////////	}
////////	::RegCloseKey(hKey);	
////////
////////	WORD nWarn = 0;
////////////////退出页面时告警/////////////////////////////////////
////////	if ( m_btnCheckExitWarn.GetCheck() )
////////	{
////////		nWarn = 1;
////////		g_LocalData.SaveExitWarnConfig(nWarn);
////////	}
////////	else
////////	{
////////		nWarn = 0;
////////		g_LocalData.SaveExitWarnConfig(nWarn);
////////	}
////////////////设置最前端显示/////////////////////////////////////
////////    CMainFrame *pMain=(CMainFrame*)AfxGetMainWnd();	
////////	CRect rcMy;
////////	WORD nTop = 0;
////////	::GetWindowRect(pMain->m_hWnd,rcMy);//得到当前的窗口位置
////////	if ( m_btnCheckShowTop.GetCheck() )
////////	{
////////		//设为总在最前
////////		pMain->SetWindowPos(&wndTopMost,rcMy.left,rcMy.top,rcMy.Size().cx,rcMy.Size().cy,SWP_SHOWWINDOW);
////////	    nTop = 1;
////////		g_LocalData.SaveTopShowConfig(nTop);
////////	}
////////	else
////////	{
////////		//取消总在最前
////////		pMain->SetWindowPos(&wndNoTopMost,rcMy.left,rcMy.top,rcMy.Size().cx,rcMy.Size().cy,SWP_SHOWWINDOW);
////////		nTop = 0;
////////		g_LocalData.SaveTopShowConfig(nTop);
////////	}
////////	
////////	/////////主窗口位于边缘处是否隐藏//////////////////////////////////////
////////	WORD nHide = 0;
////////	if ( m_btnCheckHide.GetCheck() )
////////	{
////////		pMain->SetTimer(TIMER_HIDE,AUOT_CHECK_TIME,NULL);
////////		nHide = 1;
////////		g_LocalData.SaveHideWinConfig(nHide);//是否自动隐藏
////////		
////////	}
////////	else
////////	{
////////		pMain->KillTimer(TIMER_HIDE);
////////		nHide = 0;
////////		g_LocalData.SaveHideWinConfig(nHide);//是否自动隐藏
////////		
////////	}
////////
////////	return 1;
////////}
////////
////////
////////void CSysCommonBasic::OnBnClickedCheckTop()
////////{
////////	// TODO: 在此添加控件通知处理程序代码
////////}
