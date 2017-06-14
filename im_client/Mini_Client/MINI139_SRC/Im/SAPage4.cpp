// im\SAPage4.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "SAPage4.h"
#include "improtocol.h"
#include "localdata.h"
#include "logindlg.h"

// 登录设置 对话框

IMPLEMENT_DYNAMIC(CSAPage4, CSABasePage)
CSAPage4::CSAPage4(CWnd* pParent /*=NULL*/)
	: CSABasePage(CSAPage4::IDD, pParent)
{
	Create(CSAPage4::IDD,pParent);
}

CSAPage4::~CSAPage4()
{
}

void CSAPage4::DoDataExchange(CDataExchange* pDX)
{
	CSABasePage::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_COMBO_IMSERVER,m_comboImServer);
	DDX_Control(pDX,IDC_CHECK_HIDE,m_btnCheckHide);
	DDX_Control(pDX,IDC_CHECK_LOGIN,m_btnCheckAutoLogin);
	DDX_Control(pDX,IDC_CHECK_AUTOSTART,m_btnCheckAutoStart);
	DDX_Control(pDX, IDC_CHECK_FRIEND, m_btnCheckHint);
}


BEGIN_MESSAGE_MAP(CSAPage4, CSABasePage)
	ON_CBN_SELCHANGE(IDC_COMBO_IMSERVER, OnCbnSelchangeComboImserver)
END_MESSAGE_MAP()


// CSAPage4 消息处理程序

BOOL CSAPage4::OnInitDialog()
{
	CSABasePage::OnInitDialog();

	DWORD dwImServerIP = g_pProtocol->GetIMServerIP();
	in_addr in;
	in.S_un.S_addr = ntohl(dwImServerIP);
	CString sImServerIp = inet_ntoa(in);

    CStringList sl;
	g_LocalData.GetImServerList(sl);

	m_comboImServer.ResetContent();

	for ( int i=0; i<sl.GetCount(); i++ )
	{
		POSITION pos=sl.FindIndex(i);
		m_comboImServer.AddString(sl.GetAt(pos));
	}

	m_comboImServer.SetWindowText(sImServerIp);

	USER_ACCOUNT_LIST ul;
	USER_ACCOUNT_LIST::iterator it,itE;
	CString strLstUserName;

	g_LocalData.GetUserAccountList(strLstUserName,ul);

	

	//POSITION pos = ul.GetHeadPosition();
	it = ul.begin();
	itE = ul.end();
	while (it != itE )
	{
		RegAccountStruct & regAccount = *it;
		if ( strcmp(regAccount.cUsername,g_ImUserInfo.GetUserName().c_str())==0 )
		{
			m_btnCheckHide.SetCheck(regAccount.bHide);
			m_btnCheckAutoLogin.SetCheck(regAccount.bAutoLogin);
			break;
		}
		it++;
	}
//
//#if _DEBUG
//	if ( g_ImUserInfo.status!=XTREAM_OFFLINE )
//	{
//		m_comboImServer.EnableWindow(FALSE);
//		GetDlgItem(IDC_STATIC2)->ShowWindow(SW_SHOW);
//	}
//#else
//	m_comboImServer.ShowWindow(SW_HIDE);
//	GetDlgItem(IDC_STATIC2)->ShowWindow(SW_HIDE);
//	GetDlgItem(IDC_STATIC1)->ShowWindow(SW_HIDE);
//#endif

	HKEY hKey;
	LONG ret ;
	LPCTSTR data_Set = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_ALL_ACCESS,&hKey);
	
	DWORD dwType = REG_SZ;
	DWORD dwSize = 255;
	char  szValue[255];

	ret = ::RegQueryValueEx(hKey,"Mini",NULL,&dwType,(LPBYTE)szValue,&dwSize);

	bool bAutoStart = false;

	if ( ret==ERROR_SUCCESS )
	{
		CString strValue=szValue;
		strValue.Trim();
		strValue.MakeLower();

		char szTemp[255];
		GetModuleFileName(NULL,szTemp,255);

		CString strTemp=szTemp;
		strTemp.MakeLower();
		strTemp.Trim();

		if ( strTemp!=strValue)
		{
			::RegSetValueEx(hKey,"Mini",NULL,REG_SZ,(LPBYTE)szTemp,strlen(szTemp));
		}

		bAutoStart=true;
	}

	::RegCloseKey(hKey);

	if ( bAutoStart )
	{
		m_btnCheckAutoStart.SetCheck(1);
	}

	if ( g_ImUserInfo.bShowHint )
		m_btnCheckHint.SetCheck(0);
	else
		m_btnCheckHint.SetCheck(1);//禁止提示信息

	return TRUE; 
}

int CSAPage4::SaveLoginSetting()
{
	RegAccountStruct r;
	//CString sIp;
	//DWORD   dwIp;
	//m_comboImServer.GetWindowText(sIp);

	//dwIp = htonl(inet_addr((LPCSTR)sIp));

	//if ( dwIp==0 || dwIp==0xFFFFFFFF)
	//{
	//	//MessageBox("不正确的IP地址格式！ 正确的IP地址例如: 127.0.0.1",IM_CAPTION,MB_OK|MB_ICONWARNING);
	//	CMessageBox::Prompt("不正确的IP地址格式！ 正确的IP地址例如: 127.0.0.1");
	//	m_comboImServer.SetFocus();
	//	m_comboImServer.SetEditSel(0,-1);

	//	return 0;
	//}

	r.dwServerIP = 0;// dwIp;
	sprintf(r.cUsername,g_ImUserInfo.GetUserName().c_str());
	strcpy(r.cPass,g_ImUserInfo.GetPassword().c_str());
	r.bHide = m_btnCheckHide.GetCheck();
	r.bAutoLogin = m_btnCheckAutoLogin.GetCheck();
	r.bShowHint  = !m_btnCheckHint.GetCheck();

	g_ImUserInfo.bShowHint  = r.bShowHint;
	g_ImUserInfo.bAutoLogin = r.bAutoLogin;
	g_ImUserInfo.bHideLogin = r.bHide;

	//if ( CLoginDlg::m_strServerIp!=sIp )
	//{
	//	if ( g_ImUserInfo.status==XTREAM_OFFLINE )
	//	{
 //   		CLoginDlg::m_strServerIp=sIp;
	//		g_pProtocol->SetIMServerIP(r.dwServerIP);
	//	}
	//}
	//
	g_LocalData.AddUserAccountList(r);

	HKEY hKey;
	LONG ret ;
	LPCTSTR data_Set = "Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	ret = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,data_Set,0,KEY_ALL_ACCESS,&hKey);
		
	DWORD dwType = REG_SZ;
	DWORD dwSize = 255;

	if ( m_btnCheckAutoStart.GetCheck() )
	{
		char szTemp[255];
		GetModuleFileName(NULL,szTemp,255);
		::RegSetValueEx(hKey,"Mini",NULL,REG_SZ,(LPBYTE)szTemp,strlen(szTemp));
	}
	else
	{
		::RegDeleteValue(hKey,"Mini");
		
	}
	
	::RegCloseKey(hKey);	

	this->m_bModified = false;

	return 1;
}

void CSAPage4::OnCbnSelchangeComboImserver()
{
	CString sImServerIp;
	CString strWindowText;
	m_comboImServer.GetWindowText(strWindowText);

	int find=m_comboImServer.FindString(0,strWindowText);

	m_comboImServer.GetLBText(
		m_comboImServer.GetCurSel(),
		sImServerIp );

	if (find==CB_ERR)
	{
		m_comboImServer.AddString(strWindowText);
	}
}
