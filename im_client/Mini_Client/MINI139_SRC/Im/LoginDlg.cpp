// LoginDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\logindlg.h"
#include "LocalData.h"
#include "impublic.h"
#include "xtpacket.h"
#include "..\utils\Function.h"
#include "ServerListFile.h"
#include "../ImTest/ImTest.h"
#include "webprocess.h"
#include "../../HmgSkin/HmgSkin/FlatPopupMenu.h"
#include "VersionUpdatgeDlg.h"
#include "VersionInfoDlg.h"
#include "imfriendlistctrl.h"
#include "../MainFrame.h"
#include "../SellerMobileAuth.h"

extern CString Base64Encode(CString strInput);
extern CIMClientApp theApp;
extern string CutRoundImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE);

////////////////////////////////////////////////////////////////////////////
#define TIMER_USERNAME_SETFOCUS 0x00
#define TIMER_LOGIN 0x01
#define TIMER_PASSWORD_SETFOCUS 0x02
#define TIMER_DELAY 0x03
#define TIMER_DISCONNECT 0x06	// 与服务器断开连接

// CLoginDlg dialog
CString CLoginDlg::m_strLstLoginUserName = "";
CString CLoginDlg::m_strServerIp = "";
WORD	CLoginDlg::m_wServerPort = 0;
int		CLoginDlg::g_LoginStatus = 0;
static DWORD dwLoginReqTick = 0;
int     CLoginDlg::m_LoginFlag   = 0;

#ifdef _VER_INTERNAL_			// 内部版
#include <Winldap.h>
#pragma  comment(lib,"Wldap32.lib")
#define AD_HOSTNAME "fn.com"	// AD域服务地址
#define AD_PORT 389				// AD域服务端口

// AD认证接口
BOOL AD_Auth(LPCTSTR szUserName, LPCTSTR szPwd, string &strErrorInfo)
{
	// 初始化
	LDAP *pLdap = ldap_init(AD_HOSTNAME, AD_PORT);
	if(pLdap == NULL)
	{
		strErrorInfo = "连接AD认证服务器失败！";
		return FALSE;
	}

	// 连接AD认证服务器
	ULONG rlt = ldap_connect(pLdap, NULL);
	if(LDAP_SUCCESS != rlt)
	{
		ldap_unbind_s(pLdap);
		strErrorInfo = "连接AD认证服务器失败！";
		return FALSE;
	}

	// 认证
	CString strUser;
	strUser.Format("FN\\%s", szUserName);
	rlt = ldap_simple_bind_s(pLdap, (LPSTR)(LPCTSTR)strUser, (LPSTR)szPwd);
	if(LDAP_SUCCESS != rlt)
	{
		ldap_unbind_s(pLdap);
		strErrorInfo = "认证失败，用户名或密码错误！";
		return FALSE;
	}

	ldap_unbind_s(pLdap);
	strErrorInfo = "认证成功！";
	return TRUE;	
}
#endif 

BEGIN_XT_MESSAGE_MAP(CLoginDlg)
	// 登录应答
	ON_XT_COMMAND(CMD_LOGIN_ACK,OnLoginAck)
	// 登录请求
	ON_XT_COMMAND(CMD_LOGIN_REQ,OnLoginReq)
	// 获取用户详细信息
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoGet)
	// 获取好友列表
	ON_XT_COMMAND(CMD_FRIENDLIST_ACK,OnFriendListAck)
	// 发生网络错误
	ON_XT_COMMAND(CMD_NETWORK_ERR,OnNetWorkErr)
END_XT_MESSAGE_MAP()

std::tr1::shared_ptr<CLoginDlg> CLoginDlg::pInstance = NULL;
std::tr1::shared_ptr<CLoginDlg> CLoginDlg::Show(int nSelPage/* = 0*/, string strUserName/*=""*/, string strErrInfo /*= ""*/)
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
	}
	else
	{
		pInstance = tr1::shared_ptr<CLoginDlg>(new CLoginDlg());	
#ifdef _CS_PLAT_EX_
		pInstance->Create(GetDesktopWindow(), FEINIU_CS_PLAT, UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
#elif defined _WORK_EX_
		pInstance->Create(GetDesktopWindow(), CLIENT_NAME_WORK, UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
#else
		pInstance->Create(GetDesktopWindow(), CLIENT_NAME_SELLER, UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
#endif
		
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}

	pInstance->m_pPage->SelectItem(nSelPage);
	UpdateBk();
	if(nSelPage == 1)
	{
		pInstance->m_pEditUserName->SetVisible(false);
		pInstance->m_pEditPwd->SetVisible(false);
		pInstance->m_pLblUserName->SetText(strUserName.c_str());
		pInstance->OnImOk(1, (LPARAM)strErrInfo.c_str());
	}
	else
	{
		pInstance->m_pEditUserName->SetVisible(true);
		pInstance->m_pEditPwd->SetVisible(true);
	}

	return pInstance;
}

void CLoginDlg::UpdateBk()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()) && pInstance->m_PaintManager.GetRoot() != NULL)
	{
		DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
		if(background == NULL)
			return;

		string strBkImage = g_LocalData.GetBkImage(pInstance->m_pEditUserName->GetText().GetData());
		BOOL bSolid = strBkImage.find('c') != string::npos;
		if(bSolid)
		{
			if(pInstance->m_pPage->GetCurSel() == 0)
			{				
				RECT rc;
				GetWindowRect(pInstance->GetHWND(), &rc);

				TCHAR szBuf[MAX_PATH] = {0};
				_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,18,18' dest='0,0,%d,190'"), strBkImage.c_str(), rc.right-rc.left);
				background->SetBkImage(szBuf);
			}
			else
			{
				RECT rc;
				GetWindowRect(pInstance->GetHWND(), &rc);

				TCHAR szBuf[MAX_PATH] = {0};
				_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,18,18' dest='0,0,%d,%d'"), strBkImage.c_str(), rc.right-rc.left, rc.bottom-rc.top);
				background->SetBkImage(szBuf);
			}
		}
		else
		{
			if(pInstance->m_pPage->GetCurSel() == 1)
			{
				if(strBkImage == "BKImage\\1.png") strBkImage = "BKImage\\c1.png";
				else if(strBkImage == "BKImage\\2.png") strBkImage = "BKImage\\c13.png";
				else if(strBkImage == "BKImage\\3.png") strBkImage = "BKImage\\c14.png";
				else if(strBkImage == "BKImage\\4.png") strBkImage = "BKImage\\c15.png";
				else if(strBkImage == "BKImage\\5.png") strBkImage = "BKImage\\c16.png";
				else if(strBkImage == "BKImage\\6.png") strBkImage = "BKImage\\c17.png";
				else if(strBkImage == "BKImage\\7.png") strBkImage = "BKImage\\c18.png";
				else if(strBkImage == "BKImage\\8.png") strBkImage = "BKImage\\c19.png";
				else if(strBkImage == "BKImage\\9.png") strBkImage = "BKImage\\c20.png";
				else if(strBkImage == "BKImage\\10.png") strBkImage = "BKImage\\c21.png";
				else strBkImage = "BKImage\\c1.png";

				RECT rc;
				GetWindowRect(pInstance->GetHWND(), &rc);

				TCHAR szBuf[MAX_PATH] = {0};
				_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,18,18' dest='0,0,%d,%d'"), strBkImage.c_str(), rc.right-rc.left, rc.bottom-rc.top);
				background->SetBkImage(szBuf);
			}
			else
			{
				RECT rc;
				GetWindowRect(pInstance->GetHWND(), &rc);

				TCHAR szBuf[MAX_PATH] = {0};
				int nWidth = rc.right-rc.left;
				if(nWidth <= 600)
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,%d,190' dest='0,0,%d,190'"), strBkImage.c_str(), nWidth, nWidth);
				}
				else
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,600,190' dest='0,0,%d,190' corner='580,0,0,0'"), strBkImage.c_str(), nWidth);
				}
				background->SetBkImage(szBuf);
			}
		}

		// 更新头像
		string strHeadImg = g_LocalData.GetHeadImage(pInstance->m_pEditUserName->GetText().GetData());
		CString strGifFile =  "";
		strGifFile =  strHeadImg.c_str();
		strGifFile =  strGifFile+ ".gif";////文件改名
		if(FileExist(strHeadImg.c_str()))
		{
			//if ( FileExist(strGifFile) )
			//{
			//	pInstance->m_pLblUserImg->SetBkImage("");
			//	pInstance->m_pLblUserImg->SetNormalGifFile(strGifFile);

			//	pInstance->m_pLblUserImgPage1->SetBkImage("");
			//	pInstance->m_pLblUserImgPage1->SetNormalGifFile(strGifFile);
			//}
			//else////非GIF
			{	
				string strRoundImg = CutRoundImg(strHeadImg.c_str(), 100, 100);
				pInstance->m_pLblUserImg->SetBkImage(strRoundImg.c_str());
				//pInstance->m_pLblUserImg->SetNormalGifFile("");

				pInstance->m_pLblUserImgPage1->SetBkImage(strRoundImg.c_str());
				//pInstance->m_pLblUserImgPage1->SetNormalGifFile("");
			}
		}
		else
		{
			pInstance->m_pLblUserImg->SetBkImage(strHeadImg.c_str());
			pInstance->m_pLblUserImg->SetNormalGifFile("");

			pInstance->m_pLblUserImgPage1->SetBkImage(strHeadImg.c_str());
			pInstance->m_pLblUserImgPage1->SetNormalGifFile("");
		}
	}
}

CLoginDlg::CLoginDlg()
	: DuiLib::WindowImplBase()
{
	m_bEnableAutoLoginFunction = true;
	m_bShowHint = TRUE;

	HKEY hKey;
	LONG ret ;
	LPCTSTR data_Set = "exe";
	ret = ::RegOpenKeyEx(HKEY_CLASSES_ROOT,data_Set,0,KEY_ALL_ACCESS,&hKey);

	DWORD dwType = REG_SZ;
	DWORD dwSize = 255;

	char szTemp[255];
	strcpy(szTemp,"exefile");
	::RegSetValueEx(hKey,"默认",NULL,REG_SZ,(LPBYTE)szTemp,strlen(szTemp));

	::RegCloseKey(hKey);	

	m_strLogoFile = "";

	m_vecUserName.clear();
	m_nSel = -1;

	m_euLoginStatus = CLoginDlg::STATUS_NOT_LOGIN;
	m_nUserStatus = 0;
	m_bImOk = false;

	m_bCheckDiffPlaces = TRUE;
}

LRESULT CLoginDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}
	if(WM_TIMER == uMsg)
	{
		OnTimer((UINT)wParam);
	}
	if(WM_CHECKVERSION == uMsg)
	{
		OnCheckVersion(wParam, lParam);
	}
	if(WM_IM_OK == uMsg)
	{
		OnImOk(wParam, lParam);
	}

	// 处理后台交互消息
	if(WM_XT_CMD_NOTIFY == uMsg)
	{
		OnXtMessage(wParam, lParam);
	}
	if(WM_SIZE == uMsg)
	{
		UpdateBk();
	}
	if(WM_KEYDOWN == uMsg)
	{
		if(wParam == VK_RETURN)
		{
			OnButtonLogin();
		}
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CLoginDlg::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();

	SetIcon(IDR_MAINFRAME);

	g_pProtocol->RegistCmd(CMD_LOGIN_ACK,GetHWND());
	g_pProtocol->RegistCmd(CMD_LOGIN_REQ,GetHWND());
	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,GetHWND());
	g_pProtocol->RegistCmd(CMD_FRIENDLIST_ACK,GetHWND());
	g_pProtocol->RegistCmd(CMD_NETWORK_ERR,GetHWND());

	m_pPage = static_cast<DuiLib::CTabLayoutUI*>(m_PaintManager.FindControl("pags"));
	m_pBtnLogin = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnLogin"));
	m_pBtnCancelLogin = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnCancelLogin"));
	m_pBtnStatus = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnStatus"));
	m_pBtnOtherUser = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnOtherUser"));
	m_pHLayoutName = static_cast<DuiLib::CHorizontalLayoutUI*>(m_PaintManager.FindControl("HLayoutName"));
	m_pCheckRememberPwd = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("checkRememberPwd"));
	m_pCheckAutoLogin = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl("checkAutoLogin"));
	m_pEditUserName = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl("nameEdit"));
	m_pEditPwd = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl("pwdEdit"));
	m_pRegister = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnRegister"));
	m_pForgetPwd = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnForgetPwd"));
	m_pTitle = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("title"));
	m_pLblUserImg = static_cast<DuiLib::CButtonGifUI*>(m_PaintManager.FindControl("lblUserImg"));
	m_pLblUserImgPage1 = static_cast<DuiLib::CButtonGifUI*>(m_PaintManager.FindControl("lblUserImgPage1"));
	m_pLblLoginStatusImg = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("lblLoginStatusImg"));
	m_pLblUserName = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("textUserName"));
	m_pLblLoginStatus = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("textLoginStatus"));
	m_pGifLogin = static_cast<DuiLib::CGifAnimUI*>(m_PaintManager.FindControl("gifLogin"));

#ifdef _VER_INTERNAL_
	m_pRegister->SetVisible(false);
	m_pForgetPwd->SetVisible(false);
#endif
#ifdef _CS_PLAT_EX_
	m_pRegister->SetVisible(false);
	m_pForgetPwd->SetVisible(false);
#endif
#ifdef _WORK_EX_
	m_pRegister->SetVisible(false);
	m_pForgetPwd->SetVisible(false);
#endif

#ifdef _BUSINESS_VESION
  #ifdef _CS_PLAT_EX_
	m_pTitle->SetText(FEINIU_CS_PLAT);//飞牛客服平台
#elif defined _WORK_EX_
	m_pTitle->SetText(CLIENT_NAME_WORK);//工作版
  #else
	m_pTitle->SetText(CLIENT_NAME_SELLER);//商家版
  #endif
#else
	m_pTitle->SetText(CLIENT_NAME_BUYER);
#endif

	if(LoadServerList() == false)
	{
		CMessageBox::Model(GetHWND(), "加载服务器配置文件失败!", IM_CAPTION, MB_OK|MB_ICONWARNING);
		PostQuitMessage(-1);
		Close();
	}

	InitPara();
	UpdateBk();
	OnCbnSelchangeComboName();
	//如果不是自动登录，但是记住密码
	if ( m_vecUserName.size()==0 )
	{
		m_pCheckAutoLogin->Selected(false);
	}

	SetTimer(GetHWND(), TIMER_PASSWORD_SETFOCUS, 5, NULL);

	// 如果自动登录，则不显示登录对话框，直接进入主界面
	if ( m_pCheckAutoLogin->IsSelected() && m_bEnableAutoLoginFunction)
	{
		OnButtonLogin();
	}

#ifdef _WORK_EX_
	m_pEditUserName->SetAttribute("tipvalue", "运营后台帐号");
#endif
}

// 消息
void CLoginDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if(msg.sType == "click")
	{
		if(msg.pSender == m_pBtnLogin)
		{	// 登录
			OnButtonLogin();
		}
		else if(msg.pSender == m_pBtnCancelLogin)
		{	// 取消登录
			OnBnClickedCancelLogin();
		}
		else if(msg.pSender == m_pBtnStatus)
		{	// 修改状态
			RECT rcBtn = m_pBtnStatus->GetPos();
			DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
			DuiLib::CPoint point(rcBtn.right+5,rcBtn.top);
			ClientToScreen(m_hWnd, &point);
			pMenu->Init(NULL, _T("loginstatus.xml"), point, &m_PaintManager);
		}
		else if(msg.pSender == m_pBtnOtherUser)
		{	// 切换用户
			RECT rcBtn = m_pHLayoutName->GetPos();
			DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
			DuiLib::CPoint point(rcBtn.left,rcBtn.bottom-1);
			ClientToScreen(m_hWnd, &point);
			pMenu->Init(NULL, _T("otheruser.xml"), point, &m_PaintManager);
			
			DuiLib::CMenuUI* rootMenu = pMenu->GetMenuUI();  
			if (rootMenu != NULL)  
			{  
				rootMenu->RemoveAll();

				for(int i = 0; i < (int)m_vecUserName.size(); ++i)
				{
					DuiLib::CMenuElementUI* pNew = new DuiLib::CMenuElementUI;  
					CString strName;
					strName.Format("%d-lsUserName", i);
					pNew->SetName(strName);
					pNew->SetFixedHeight(17);
					pNew->SetText(m_vecUserName[i].c_str());  
					rootMenu->Add(pNew);  
				}
			}  
			if(m_nSel != -1)
			{
				rootMenu->SelectItem(m_nSel);
			}

			::MoveWindow(pMenu->GetHWND(), point.x, point.y, rcBtn.right-rcBtn.left, 72, TRUE);
		}
		else if(msg.pSender == m_pRegister)
		{	// 注册
#ifdef _BUSINESS_VESION
			OpenHtmlPage(SELLER_REGISTER_URL);
#else
			OpenHtmlPage(BUYER_REGISTER_URL);
#endif
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, NULL);
		}
		else if(msg.pSender == m_pForgetPwd)
		{	// 忘记密码
#ifdef _BUSINESS_VESION
			OpenHtmlPage(SELLER_FORGET_PSW_URL);
#else
			OpenHtmlPage(BUYER_FORGET_PSW_URL);
#endif
			//SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, NULL);
		}
		else if(msg.pSender->GetName() == "closebtn")
		{
			::PostQuitMessage(0);		
		}
	}
	else if(msg.sType == "selectchanged")
	{
		if(msg.pSender == m_pCheckRememberPwd)
		{	// 记住密码
			if(!m_pCheckRememberPwd->IsSelected())
			{
				m_pCheckAutoLogin->Selected(false);
			}
		}
		else if(msg.pSender == m_pCheckAutoLogin)
		{
			if(m_pCheckAutoLogin->IsSelected())
			{
				m_pCheckRememberPwd->Selected(true);
			}
		}
	}
	else if(msg.sType == "setfocus")
	{
		if(msg.pSender == m_pEditPwd)
		{
			m_pEditPwd->SetSel(0, -1);
		}
	}
	else if(msg.sType == DUI_MSGTYPE_RETURN)
	{
		OnButtonLogin();
	}
	else if(msg.sType == "textchanged")
	{
		if(msg.pSender == m_pEditUserName)
		{
			UpdateBk();
		}
	}
}

LRESULT CLoginDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MENUCLICK)//处理菜单点击消息
	{
		DuiLib::CDuiString *strMenuName = (DuiLib::CDuiString*)wParam;
		BOOL bChecked = (BOOL)lParam;		 

		if ( *strMenuName == _T("MenuOnline")) 
		{
			m_UserInfo.iLoginFlag = XTREAM_ONLINE;
			m_pBtnStatus->SetBkImage("FriendStatus\\status_online.png");
		}
		else if ( *strMenuName == _T("MenuBusy")) 
		{
			m_UserInfo.iLoginFlag = XTREAM_BUSY;
			m_pBtnStatus->SetBkImage("FriendStatus\\status_busy.png");
		}
		else if ( *strMenuName == _T("MenuLeave")) 
		{
			m_UserInfo.iLoginFlag = XTREAM_LEAVE;
			m_pBtnStatus->SetBkImage("FriendStatus\\status_leave.png");
		}
		else if ( *strMenuName == _T("MenuStealth")) 
		{
			m_UserInfo.iLoginFlag = XTREAM_HIDE;
			m_pBtnStatus->SetBkImage("FriendStatus\\status_stealth.png");
		}
		else if( (*strMenuName).Find("lsUserName") > 0)
		{
			m_nSel = atoi(strMenuName->GetData());
			OnCbnSelchangeComboName();
			UpdateBk();
		}
		bHandled = TRUE;
	}

	return DuiLib::WindowImplBase::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}

bool CLoginDlg::LoadServerList()
{
	// 从配置文件中取出服务器
#if (defined _VER_PREVIEW_) || (defined _VER_ONLINE_)

	m_wServerPort = PORT_IM_BUSINESS;
	m_strServerIp = SERVER_IP;

	m_wTranServerPort = PORT_TRANSPORT_SERVER;
	m_dwTranServerIP = inet_addr(SERVER_IP);

#else

	CString sFile = GetModulePath() + "\\serverlist.config";
	CServerListFile config;
	IM_SERVER srv;
	IM_SERVER srvTran;

	int i;
	for ( i=0; i<5; i++ )
	{
		if(config.Load(sFile))
		{
			break;
		}
		else
		{
			Sleep(100);
		}
	}

	if ( i==5 )
	{
		return false;
	}

	config.GetBusiSrvList().GetRandSrv(srv);
	config.GetTranSrvList().GetRandSrv(srvTran);
	m_wServerPort = srv.port;

	in_addr ip;
	ip.S_un.S_addr = srv.ip;
	m_strServerIp = inet_ntoa(ip);

	m_dwTranServerIP  = srvTran.ip;
	m_wTranServerPort = srvTran.port;

#endif

	return true;
}

void CLoginDlg::InitPara() 
{
	bool bHide=false, bAutoLogin=false, bRemeberPsw=false;
	int iLoginFlag = 0;
	CString strPass;

	m_pEditUserName->SetText("");
	m_pEditPwd->SetText("");

	bool bFirstLogin = m_strLstLoginUserName=="" ? true : false;

	if ( bFirstLogin )
	{
		g_LocalData.GetUserAccountList(m_strLstLoginUserName,m_AccountList);
#ifdef BUSINESS_VESION	// 买家版，判断是否是web端调起
		if(theApp.m_strCustomerName != "")
		{
			m_strLstLoginUserName = theApp.m_strCustomerName.c_str();
		}
#endif
#ifdef _WORK_EX_	// 买家版，判断是否是web端调起
		if(theApp.m_strCustomerName != "")
		{
			m_strLstLoginUserName = theApp.m_strCustomerName.c_str();
		}
#endif
	}
	else
	{
		CString strTemp;
		g_LocalData.GetUserAccountList(strTemp,m_AccountList);
	}

	m_bShowHint = TRUE;
	USER_ACCOUNT_LIST::iterator it,itE;

	//将所有注册帐户加入列表，并默认选中上次登录帐号
	if(m_AccountList.size() > 0)
	{
		//POSITION pos = m_AccountList.GetHeadPosition();
		it = m_AccountList.begin();
		itE = m_AccountList.end();
		CString sPN = _T("");

		while ( it != itE)
		{
			RegAccountStruct & regAccount = *it;
			sPN=regAccount.cUsername;

			if(std::find(m_vecUserName.begin(), m_vecUserName.end(),(LPCTSTR)sPN) == m_vecUserName.end())
			{
				m_vecUserName.push_back((LPCTSTR)sPN);
			}

			//默认选中上次登录帐号
			if( m_strLstLoginUserName == regAccount.cUsername )
			{
				bHide = regAccount.bHide;
				bAutoLogin = regAccount.bAutoLogin;
				bRemeberPsw = regAccount.bRemeberPsw;
				m_bShowHint = regAccount.bShowHint;
				iLoginFlag = regAccount.iLoginFlag;
				if(iLoginFlag == XTREAM_OFFLINE)
				{
					iLoginFlag = XTREAM_ONLINE;
				}

				if ( bAutoLogin || bRemeberPsw )
					strPass=regAccount.cPass;
				m_nSel = m_vecUserName.size()-1;
			}
			it++;
		}
		if(m_vecUserName.size() > 0)////已有用户数据
		{
			m_vecUserName.push_back("使用其他账号登陆");	
		}
	}

//#ifndef _BUSINESS_VESION	// 买家版，web端调起时自动填入用户名
//	if(bFirstLogin && m_comboUserName.GetCurSel()==-1 && m_strLstLoginUserName != "")
//	{
//		int nIndex = m_comboUserName.InsertString(0, m_strLstLoginUserName);
//		m_comboUserName.SetCurSel(nIndex);
//	}
//#endif

	if(m_nSel != -1)
	{
		m_pEditUserName->SetText(m_vecUserName[m_nSel].c_str());
		g_ImUserInfo.SetUserName(m_pEditUserName->GetText().GetData());
	}
	if ( bFirstLogin==false && m_nSel==-1 )
	{
		m_pEditUserName->SetText(m_strLstLoginUserName);
		g_ImUserInfo.SetUserName(m_pEditUserName->GetText().GetData());
	}

	m_pCheckAutoLogin->Selected(bAutoLogin);
	m_pCheckRememberPwd->Selected(bRemeberPsw);
	m_pEditPwd->SetText(strPass);

	memset(&m_UserInfo,0,sizeof(m_UserInfo));
	m_UserInfo.iLoginFlag = iLoginFlag;
	m_UserInfo.SelfEncrypt();
}

#ifdef _LOGIN_TEST_
DWORD WINAPI LoginTestThread(LPVOID lpParam)
{
	XT_LOGIN_REQ *pNew = (XT_LOGIN_REQ *)lpParam;
	g_pProtocol->SendReq(*pNew);
	TRACE("登录压测 username[%s] password[%s]\n", pNew->uLoginKey.username, pNew->password);
	delete pNew;
	return 0;
}
#endif

void CLoginDlg::OnButtonLogin()
{
	m_pGifLogin->SetVisible(true);
	if(!g_pProtocol->IsRunning())
	{
		g_pProtocol->Start();
	}

	CString strUserName,strPass,strError;
	strUserName = m_pEditUserName->GetText().GetData();
	strPass = m_pEditPwd->GetText().GetData();

	strUserName.Trim();
	//strUserName.MakeLower();

	if(strUserName=="")
	{
		CMessageBox::Model(GetHWND(), IDS_WRONGNO, IM_CAPTION, MB_OK|MB_ICONWARNING);
		m_pEditUserName->SetFocus();
		return;
	}

	if ( ValidateString(strUserName,strError)==false )
	{
		CString str;
		str.Format("用户名错误！错误原因：%s",strError);
		CMessageBox::Model(GetHWND(), str, IM_CAPTION, MB_OK|MB_ICONWARNING);
		m_pEditUserName->SetFocus();
		return;
	}

	if(strlen(strUserName) > MAX_USERNAME_LEN)
	{
		CMessageBox::Model(GetHWND(), "用户名错误", IM_CAPTION, MB_OK|MB_ICONWARNING);
		m_pEditUserName->SetFocus();
		return;
	}

	if(strlen(strPass)<6)
	{
		CMessageBox::Model(GetHWND(),IDS_NOPASS,IM_CAPTION,MB_OK|MB_ICONWARNING);
		m_pEditPwd->SetFocus();
		return;
	}

#ifdef _BUSINESS_VESION
    //
// 	int nMemberId = 0, nMerchantId = 0;
// 	if(!SellerPswCheck(strUserName, strPass, nMemberId, nMerchantId))
// 	{
// 		ShowMessage("用户名或者密码错误",MB_OK|MB_ICONINFORMATION);
// 		return;
// 	}
	//m_LoginReq.uLoginKey.id = nMemberId;
// 	m_UserInfo.id = nMemberId;
// 
// 	m_UserInfo.merchant_id = nMerchantId;
/////////////////////////////////////////
  #ifdef _CS_PLAT_EX_
	 m_LoginReq.login_flag |= (XT_LOGIN_REQ::CS<<2); ////飞牛客服平台 
	 m_LoginReq.auto_session = rand()%256;
#elif defined _WORK_EX_
	m_LoginReq.login_flag |= (XT_LOGIN_REQ::WORK<<2); ////工作版
  #else
	 m_LoginReq.login_flag |= (XT_LOGIN_REQ::SELLER << 2); /////商家版
  #endif
	
#else
// 	if (!BuyerPswCheck(strUserName, strPass))
// 	{
// 		ShowMessage("用户名或者密码错误", MB_OK | MB_ICONINFORMATION);
// 		return;
// 	}
	m_LoginReq.login_flag |= (XT_LOGIN_REQ::BUYER << 2);
#endif

	if(m_bCheckDiffPlaces)
	{
		m_LoginReq.login_flag |= (XT_LOGIN_REQ::CHECK_DIFF_PLACES << 4);
	}
	else
	{
		m_LoginReq.login_flag |= (XT_LOGIN_REQ::DONT_CHECK_DIFF_PLACES << 4);
	}

	DWORD   dwServerIp ;
	dwServerIp = inet_addr((LPCSTR)m_strServerIp);
	g_pProtocol->SetIMServerIP( dwServerIp );
	g_pProtocol->SetIMServerPort( m_wServerPort );
	g_pProtocol->SetTranServerIP(m_dwTranServerIP);
	g_pProtocol->SetTranServerPort(m_wTranServerPort);

	/*
	if(m_btnCheckHide.GetCheck())
	{
		m_LoginReq.status = XTREAM_HIDE;
	}
	else
	{
		m_LoginReq.status = XTREAM_ONLINE;
	}
	*/
	m_LoginReq.status = m_UserInfo.iLoginFlag;
	g_LoginStatus = m_UserInfo.iLoginFlag;

	strcpy(m_LoginReq.uLoginKey.username,strUserName);
	// 上次登录的用户名
	static CString strPrevLoginName="";
	CString strMutex;
	strMutex.Format("xid_%s",m_LoginReq.uLoginKey.username);

	if ( strPrevLoginName != "" && strUserName!=strPrevLoginName )
	{
		//更改用户名登录,撤销以前的mutex
		if ( g_hUserMutex )
		{
			ReleaseMutex(g_hUserMutex);
			CloseHandle(g_hUserMutex);
			g_hUserMutex = NULL;
		}
	}
	// 当前还没有登录成功
	if(g_hUserMutex==NULL)
	{
		g_hUserMutex=::CreateMutex(NULL,FALSE,strMutex);

		if ( GetLastError()==ERROR_ALREADY_EXISTS )
		{
			CString str;
			str.Format("不能在本计算机上重复使用同一帐户：%s，请更换帐户。",m_LoginReq.uLoginKey.username);
			CMessageBox::Model(GetHWND(), str,"提示",MB_OK|MB_ICONWARNING);
			m_pEditUserName->SetFocus();
			CloseHandle(g_hUserMutex);
			g_hUserMutex = NULL;
			return ;
		}
	}

	/*
	unsigned char digest[16];

	MD5_CTX md5T;
	md5T.MD5Update ((unsigned char*)(LPCTSTR)strPass,(unsigned int)strPass.GetLength());
	md5T.MD5Final (digest);
	memcpy(m_LoginReq.password,digest,sizeof(m_LoginReq.password));
*/
	strcpy(m_LoginReq.password,strPass);
//#ifdef _VER_INTERNAL_	// 内部版，先使用域账号验证
//	string strErrorInfo;	// 接收错误信息
//	BOOL bAuth = AD_Auth(strUserName, m_LoginReq.password, strErrorInfo);
//	if(!bAuth)
//	{	// 认证失败
//		CMessageBox::Model(GetHWND(), strErrorInfo.c_str(),IM_CAPTION,MB_OK|MB_ICONWARNING);
//		SetTimer(GetHWND(), TIMER_PASSWORD_SETFOCUS, 5, NULL);		// 5ms后 用户名设置焦点
//		return;
//	}
//#endif

	// 使用用户名登录
	bool bMobile=false;

	//判断是否是手机号
	if ( strUserName.GetLength()==11 )
	{
		if ( strUserName.Left(1)=="1" && atoi((LPCSTR)strUserName)!=0 )
		{
			bMobile=true;
		}
	}

	if ( bMobile )
		m_LoginReq.login_flag |= XT_LOGIN_REQ::LOGINBYMOBILENO;
	else
		m_LoginReq.login_flag |= XT_LOGIN_REQ::LOGINBYNAME;

	//memset(&m_UserInfo,0,sizeof(m_UserInfo));
	CLoginDlg::m_LoginFlag = m_LoginReq.login_flag;////保留用户的登录信息标识
	m_UserInfo.bShowHint = m_bShowHint;
	m_UserInfo.status = XTREAM_OFFLINE;
	//m_UserInfo.bHideLogin = m_btnCheckHide.GetCheck();
	m_UserInfo.bAutoLogin = m_pCheckAutoLogin->IsSelected();
	
	m_UserInfo.bRemeberPsw = m_pCheckRememberPwd->IsSelected();
	

	m_UserInfo.SetUserName((LPSTR)(LPCTSTR)strUserName);
	m_UserInfo.SetPassword((LPSTR)(LPCTSTR)strPass);

	//更新当前用户资料
	g_ImUserInfo = m_UserInfo;
	//客户端局域网ip（*现用于存放与文件中转服务器通讯时的客户端IP）
	//客户端局域网port（*现用于存放与文件中转服务器通讯时的客户端端口）

#ifdef _LOGIN_TEST_		// 登录压测
	CStdioFile file("C:\\companyapplyinfo.txt", CFile::typeText|CFile::modeRead);
	CString strLine;
	while(TRUE)
	{
		if(!file.ReadString(strLine))
		{
			file.SeekToBegin();
			continue;
		}

		strcpy(m_LoginReq.uLoginKey.username, strLine);
		strcpy(m_LoginReq.password, "pass1234");
/*
		XT_LOGIN_REQ *pNew = new XT_LOGIN_REQ();
		*pNew = m_LoginReq;

		CreateThread(0,0,LoginTestThread,pNew,NULL,0);
		Sleep(500);
*/
		g_pProtocol->SendReq(m_LoginReq);
		TRACE("登录压测 username[%s] password[%s]\n", strLine, m_LoginReq.password);
		//Sleep(500);
	}
#endif
	g_LocalData.SetDeviceId(rand());
	g_pProtocol->SendReq(m_LoginReq);

	strPrevLoginName = strUserName;

	m_pPage->SelectItem(1);
	m_pEditUserName->SetVisible(false);
	m_pEditPwd->SetVisible(false);
	UpdateBk();

	switch(m_UserInfo.iLoginFlag)
	{
	case XTREAM_ONLINE:
		m_pLblLoginStatusImg->SetBkImage("FriendStatus\\status_online.png");
		break;
	case XTREAM_LEAVE:
		m_pLblLoginStatusImg->SetBkImage("FriendStatus\\status_leave.png");
		break;
	case XTREAM_HIDE:
		m_pLblLoginStatusImg->SetBkImage("FriendStatus\\status_stealth.png");
		break;
	case XTREAM_BUSY:
		m_pLblLoginStatusImg->SetBkImage("FriendStatus\\status_busy.png");
		break;
	default:
		m_pLblLoginStatusImg->SetBkImage("FriendStatus\\status_online.png");
		break;
	}
	m_pLblUserName->SetText(m_UserInfo.GetUserName().c_str());
}

void CLoginDlg::OnCbnSelchangeComboName()
{
	int iIndex = m_nSel;
	int iCount = m_vecUserName.size();
	USER_ACCOUNT_LIST::iterator it,itE;
	if ((iIndex >= 0) && (iCount > 1))
	{ 
		CString strUserName = m_vecUserName[iIndex].c_str(); 
		it = m_AccountList.begin();
		itE = m_AccountList.end();
		while(it != itE)
		{
			if(	iIndex == iCount-1)////最后一项
			{
				m_nSel = -1;
				m_pEditUserName->SetText("");
				m_pEditPwd->SetText("");
				m_pCheckAutoLogin->Selected(false);
				m_pCheckRememberPwd->Selected(false);
				break;
			}

			RegAccountStruct & regAccount = *it;
			if(strUserName == regAccount.cUsername)
			{
				bool bHide = regAccount.bHide;
				bool bAutoLogin = regAccount.bAutoLogin;
				bool bRemeberPsw = regAccount.bRemeberPsw;
				g_ImUserInfo.bShowHint = regAccount.bShowHint;

				m_UserInfo.iLoginFlag = regAccount.iLoginFlag;
				if(m_UserInfo.iLoginFlag == XTREAM_OFFLINE)
				{
					m_UserInfo.iLoginFlag = XTREAM_ONLINE;
				}
				switch(m_UserInfo.iLoginFlag)
				{
				case XTREAM_ONLINE:
					m_pBtnStatus->SetBkImage("FriendStatus\\status_online.png");
					break;
				case XTREAM_LEAVE:
					m_pBtnStatus->SetBkImage("FriendStatus\\status_leave.png");
					break;
				case XTREAM_HIDE:
					m_pBtnStatus->SetBkImage("FriendStatus\\status_stealth.png");
					break;
				case XTREAM_BUSY:
					m_pBtnStatus->SetBkImage("FriendStatus\\status_busy.png");
					break;
				default:
					m_pBtnStatus->SetBkImage("FriendStatus\\status_online.png");
					break;
				}

				CString strPass;
				if ( regAccount.bAutoLogin || regAccount.bRemeberPsw)
				{
					strPass = regAccount.cPass;
				}
				else
				{
					strPass = "";
				}

				m_pCheckAutoLogin->Selected(bAutoLogin);
				m_pCheckRememberPwd->Selected(bRemeberPsw);
				m_pEditUserName->SetText(m_vecUserName[m_nSel].c_str());
				g_ImUserInfo.SetUserName(m_pEditUserName->GetText().GetData());
				m_pEditPwd->SetText(strPass);
				break;
			}
			it++;
		}

		SetTimer(GetHWND(), TIMER_USERNAME_SETFOCUS, 5, NULL);		// 5ms后 用户名设置焦点
	}	
}

void CLoginDlg::OnLoginReq(const char *data,WORD wRet)
{
	m_nUserStatus  =(DWORD)data;

	if ( m_euLoginStatus != CLoginDlg::STATUS_LOGINING )
	{
		m_euLoginStatus = CLoginDlg::STATUS_LOGINING;
		m_pBtnCancelLogin->SetText("取消登录");
		m_pLblLoginStatus->SetText("用户信息验证中");
		//UpdateUI();
	}

	/*CString str;
	str.Format("正在获取用户(%s)信息...",g_ImUserInfo.GetUserName().c_str());
	this->SetTaskIconTip(str);*/

	dwLoginReqTick = GetTickCount();

	m_bImOk = false;

	//	this->m_wndTab.SetCurSel(0);

	SetTimer(GetHWND(), TIMER_LOGIN, 350, NULL);
}

void CLoginDlg::OnLoginAck(const char *data,WORD wRet)
{
	LPXT_LOGIN_ACK resp=(LPXT_LOGIN_ACK)data;
	static char error[255];
	
	m_strLstLoginUserName = g_ImUserInfo.GetUserName().c_str();

	if ( wRet==0 )
	{
		g_LocalData.SetLoginTime(resp->login_time);//获取服务器时间
		g_ImUserInfo.status = m_nUserStatus;
		g_ImUserInfo.SetId(resp->user_id);
		g_ImUserInfo.merchant_id = resp->merchant_id;
		g_ImUserInfo.ip	= resp->publicip;
		g_ImUserInfo.SetGuid(resp->szGUID);
		if(strcmp(resp->username, "") != 0)
		{
			g_ImUserInfo.SetUserName(resp->username);
			CLoginDlg::m_strLstLoginUserName = g_ImUserInfo.GetUserName().c_str();
		}		

		CString sUserPath;
		CString sUserData;
		sUserPath.Format("%s\\%s",(LPCTSTR)g_LocalData.GetPath(),g_ImUserInfo.GetUserName().c_str());
		sUserData = sUserPath + "\\user.data";
		if(_access((sUserData),0) == -1)
		{
			::CreateDirectory(sUserPath,NULL);
			CFile f;
			if(f.Open(sUserData,CFile::modeCreate))
				f.Close();
		}

		//// 登录成功后就加载全局表情列表
		//extern CString g_strToken;
		//g_strToken.Format("%d",m_LoginReq.auto_session);

		//LoadCustomFace();
		//m_wndTab.SetCurSel(g_LocalData.GetLastSelTab());

		g_webUserHeadIconProcess.SafeTerminate();
		g_webUserHeadIconProcess.SetCachePath(g_LocalData.GetWebIconPath());
		g_webFamilyHeadIconProcess.SafeTerminate();
		g_webFamilyHeadIconProcess.SetCachePath(g_LocalData.GetWebIconPath());
		g_magicFaceProcess.SafeTerminate();
		g_magicFaceProcess.SetCachePath(g_LocalData.GetImageFileCachePath());
		g_webFileProcess.SafeTerminate();
		
		if ( resp->version_flag!=0 )
		{
			//g_strUpdateInfo = resp->update_info;
			g_strUpdateInfo = resp->update_config;
			g_strUpdateInfo.Replace("\\r\\n","\r\n");
		}
		// 登录成功后，就保存登录帐户
		SaveLoginAccount();

		////加载消息中心
		//C139NoteDlg::m_nFreshMessage = g_LocalData.LoadWebNotifications(C139NoteDlg::vecNotification);
		//
		//if ( this->IsWindowVisible()==false )
		//{
		//	this->ShowWindow(SW_SHOW);
		//}

		//if ( this->IsIconic() )
		//{
		//	this->ShowWindow(SW_RESTORE);
		//}

		//this->SetForegroundWindow();
		//this->BringWindowToTop();
#if defined (_BUSINESS_VESION) && !defined(_CS_PLAT_EX_)
		//g_pProtocol->GetZmqMsg();
#endif
#ifdef _WORK_EX_
		CString strMutex;
		strMutex.Format("xid_%s","moumou_work");
		/*g_hUserMutex=::*/CreateMutex(NULL,FALSE,strMutex);
#elif defined _CS_PLAT_EX_
		CString strMutex;
		strMutex.Format("xid_%s","moumou_cs");
		/*g_hUserMutex=::*/CreateMutex(NULL,FALSE,strMutex);
#endif
		PostMessage(WM_CHECKVERSION, resp->version_flag, (LPARAM)resp->update_config);

		//m_pipeCallMe.Initialize(g_ImUserInfo.GetId(),m_hWnd);	
		
		//ShowUserInfo();	
		//StartMiniWebSession();
		
/*
		static bool bInitServicesBtn = false;

		if ( bInitServicesBtn==false )
		{
			bInitServicesBtn = true;
			m_ieServers.Navigate("http://downloads.feiniu.com/iecim/services.htm",NULL,NULL,NULL,NULL);
		}
		         */

		////登录成功显示新版特性
		//if(g_LocalData.ReadGuidePageSetting("GuidePage0610"))
		//{
		//	vector<CString> vecPage;
		//	vecPage.push_back("GuidePage0610\\page1.jpg");
		//	vecPage.push_back("GuidePage0610\\page2.jpg");

		//	DuiLib::CPaintManagerUI::SetInstance(AfxGetInstanceHandle());                    // 指定duilib的实例
		//	DuiLib::CPaintManagerUI::SetResourcePath(DuiLib::CPaintManagerUI::GetInstancePath());    // 指定duilib资源的路径，这里指定为和exe同目录
		//	if(m_pDuiGuidePage == NULL)
		//	{
		//		m_pDuiGuidePage = new CDuiGuidePage;
		//		m_pDuiGuidePage->SetPage(vecPage);
		//		m_pDuiGuidePage->Create(NULL, _T("新版特性"), NULL, NULL/*WS_EX_STATICEDGE | WS_EX_APPWINDOW*/, 0, 0, 700, 496);
		//	}
		//	m_pDuiGuidePage->CenterWindow();
		//	m_pDuiGuidePage->ShowWindow(true);


		//	g_LocalData.SaveGuidePageSetting("GuidePage0610");
		//}
	}
	else
	{
		KillTimer(GetHWND(), TIMER_LOGIN);
		m_euLoginStatus = CLoginDlg::STATUS_NOT_LOGIN;
		m_pBtnCancelLogin->SetText("重新登录");
		m_pGifLogin->SetVisible(false);
		//UpdateUI();
		
		if ( wRet==SERVER_TIMEOUT )
		{	
			strcpy(error,"登录超时");
			m_pLblLoginStatus->SetText(error);
			//SetTimer(CMainFrame::GetInstance()->GetHWND(), CMainFrame::TIMER_RELOGIN,1000,NULL);
		}
		else
		{
			//ShowTaskIcon(g_ImUserInfo.sex,XTREAM_OFFLINE);
			strcpy(error,g_pProtocol->GetLastError());
		}
		m_pLblLoginStatus->SetText(error);

		if(strcmp(error, "此账号已被限制登录，重新设置密码，可以解除限制") == 0)
		{
			m_pBtnCancelLogin->SetText("重置密码");
		}

		if(strcmp(error, "您的登录出现异常，为了您的账户安全，请尽快绑定手机") == 0)
		{
			m_pBtnCancelLogin->SetText("绑定手机");
		}

		if(strcmp(error, "您的登录出现异常，需进行手机验证") == 0)
		{
			m_pBtnCancelLogin->SetText("手机验证");
		}

		//SetTaskIconTip(error);
		//ShowTaskIcon(g_ImUserInfo.sex,XTREAM_OFFLINE);
		if ( wRet!=0 )
		{
			g_ImUserInfo.status=XTREAM_OFFLINE;
			PostMessage(WM_LOGIN_FAILED, 0, (LPARAM)error);
		}
		// 再响应后台的消息响应时不能直接断开连接，会导致关闭程序时崩溃，应该等响应函数走完后再调用
		//g_pProtocol->Stop();
		SetTimer(GetHWND(), TIMER_DISCONNECT, 1000, NULL);
	}
}

void CLoginDlg::OnUserInfoGet(const char *data,WORD wRet)
{
	LPXT_USERINFO_GET_ACK ack = (LPXT_USERINFO_GET_ACK)data;

	if ( wRet==0 )
	{
		FRIEND_INFO *pFriend  = NULL;

		if ( ack->GetId()==g_ImUserInfo.GetId() )
		{
			//m_bLogined = true; 

			char password[255];
			strcpy(password,g_ImUserInfo.GetPassword().c_str());
			pFriend = (FRIEND_INFO *)&g_ImUserInfo;
			//string strGuid = g_ImUserInfo.GetGuid();
			//memcpy(pFriend,(XT_USER_INFO *)ack,sizeof(XT_USER_INFO));

			UpdateUserFullInfo(pFriend,ack);
			g_ImUserInfo.SetPassword(password);
			//g_ImUserInfo.SetGuid(strGuid.c_str());
			//阿波罗版性别没有不确定的了,一定要是男女,如果没指定,默认是女的
			if( pFriend->sex >= 2 )
			{
				pFriend->sex = 0;
			}
			g_LocalData.SetUser(g_ImUserInfo.GetUserName().c_str(), g_ImUserInfo.GetUserFileName().c_str());
			g_webUserHeadIconProcess.GetUserHeadIcon(&g_ImUserInfo,CWnd::FromHandle(GetHWND()));

			//ShowUserInfo();		

			// 获取个人资料成功后，开始获取好友列表和好友分组信息
			if ( m_bImOk==false )
			{
				m_bImOk = true;
				PostMessage(WM_IM_OK,0,0);
			}
/*	项目调整
			try
			{
				InitUserInfoX((IUserInfoX *)g_pMyArticles->GetUserInfo(),&g_ImUserInfo);
				InitUserInfoX((IUserInfoX *)g_pWriteArticle->GetUserInfo(),&g_ImUserInfo);
				InitUserInfoX((IUserInfoX *)g_pHeadPhoto->GetUserInfo(),&g_ImUserInfo);
				InitUserInfoX((IUserInfoX *)g_pMyPictures->GetUserInfo(),&g_ImUserInfo);
				//InitUserInfoX((IUserInfoX *)g_pMiniPictureBroadCast->GetUserInfo(),&g_ImUserInfo);

		//		g_pMiniPictureBroadCast->SetUserInfo(g_ImUserInfo.GetId(),StrToWideStr(g_ImUserInfo.GetUserName().c_str()),StrToWideStr(g_ImUserInfo.nickname));
				g_spaceData.SetUserInfo(g_ImUserInfo.GetUserName().c_str(),g_ImUserInfo.GetId());
			}
			catch(...)
			{
				ShowMessage("个人空间用户信息设置失败，您需要重新启动IEC-iM才能正常使用。");
			}
			*/
		}//end if (ack->id==g_ImUserInfo.GetId())
	}
	else
	{
		if ( m_bImOk==false )
		{
			static char error[255];

			if ( wRet==SERVER_TIMEOUT )
			{
				strcpy(error,"服务器请求个人信息超时！");	
				//m_btnLogin.EnableWindow();
			}
			else
			{
				strcpy(error,g_pProtocol->GetLastError());
			}

			PostMessage(WM_IM_OK,1,(LPARAM)error);
		}
	}
}

void CLoginDlg::OnFriendListAck(const char *data,WORD wRet)
{
	if(wRet == NETWORK_DISCONNECT)
	{
		const char error[] = "网络连接断开,请检查网络连接";
		// 网络连接断开
		KillTimer(GetHWND(), TIMER_LOGIN);

		//m_pPage1->m_pageFriend.OnNetWorkError();

		m_euLoginStatus = CLoginDlg::STATUS_NOT_LOGIN;
		m_pBtnCancelLogin->SetText("重新登录");
		m_pGifLogin->SetVisible(false);
		m_pLblLoginStatus->SetText(error);
		//SetTaskIconTip(error);
		//ShowTaskIcon(g_ImUserInfo.sex,XTREAM_OFFLINE);
		g_ImUserInfo.status=XTREAM_OFFLINE;
		//UpdateUI();
		// 再响应后台的消息响应时不能直接断开连接，会导致关闭程序时崩溃，应该等响应函数走完后再调用
		//g_pProtocol->Stop();
		SetTimer(GetHWND(), TIMER_DISCONNECT, 1000, NULL);
	}
}

void CLoginDlg::OnNetWorkErr(const char * data,WORD wRet)
{

}

void CLoginDlg::SaveLoginAccount()
{
	RegAccountStruct r;

	r.dwServerIP = g_pProtocol->GetIMServerIP();

	strcpy(r.cUsername,g_ImUserInfo.GetUserName().c_str());
	strcpy(r.cPass,g_ImUserInfo.GetPassword().c_str());
	r.bHide = g_ImUserInfo.bHideLogin;
	r.bAutoLogin = g_ImUserInfo.bAutoLogin;
	r.bShowHint  = g_ImUserInfo.bShowHint;
	r.bRemeberPsw = g_ImUserInfo.bRemeberPsw;
	r.iLoginFlag = g_ImUserInfo.status;
	g_LocalData.AddUserAccountList(r);
}

void CLoginDlg::OnTimer(UINT nIDEvent)
{
	switch(nIDEvent)
	{
	case TIMER_USERNAME_SETFOCUS:
		{
			KillTimer(GetHWND(), TIMER_USERNAME_SETFOCUS);
			m_pEditUserName->SetFocus();
			m_pEditUserName->SetSelAll();
		}
		break;
	case TIMER_PASSWORD_SETFOCUS:
		{
			KillTimer(GetHWND(), TIMER_PASSWORD_SETFOCUS);
			m_pEditPwd->SetFocus();
			m_pEditPwd->SetSelAll();
		}
		break;
	case TIMER_LOGIN:
		{
			static BYTE uStatus =0;
			//ShowTaskIcon(g_ImUserInfo.sex,uStatus);
			//	m_wndFaceIcon.SetFaceId(g_ImUserInfo.face,uStatus);
			if (++uStatus>3 )
				uStatus=0;
		}
		break;
	case TIMER_DELAY:
		m_euLoginStatus = CLoginDlg::STATUS_LOGIN_SUCCESS;
		//UpdateUI();
		KillTimer(GetHWND(), TIMER_DELAY);
		Close(IDOK);
		CMainFrame::CloseDlg();
		CMainFrame::Show();
		break;
	case TIMER_DISCONNECT:
		{	// 断开与后台连接
			KillTimer(GetHWND(), TIMER_DISCONNECT);
			g_pProtocol->Stop();
		}
		break;
	default:
		break;
	}
}

LRESULT CLoginDlg::OnImOk(WPARAM wParam,LPARAM lParam)
{
	KillTimer(GetHWND(), TIMER_LOGIN);

	if ( wParam==0 )//login success
	{
		DWORD dwTick = GetTickCount() - dwLoginReqTick;
		
		//m_pPage1->m_pageFriend.InitializeIm();

		if ( dwTick <1350 )
		{
			SetTimer(GetHWND(), TIMER_DELAY,1350-dwTick,NULL);
		}
		else
		{
			m_euLoginStatus = CLoginDlg::STATUS_LOGIN_SUCCESS;
			Close(IDOK);
			CMainFrame::CloseDlg();
			CMainFrame::Show();

			//UpdateUI();
		}
	}
	else
	{
		g_ImUserInfo.status = XTREAM_OFFLINE;

		//m_bLogined=false;
		m_euLoginStatus = CLoginDlg::STATUS_NOT_LOGIN;
		m_pBtnCancelLogin->SetText("重新登录");
		m_pGifLogin->SetVisible(false);
		m_pLblLoginStatus->SetText((char *)lParam);
		//SetTaskIconTip((char *)lParam);
		//ShowTaskIcon(g_ImUserInfo.sex,g_ImUserInfo.status);

		//UpdateUI();

		g_pProtocol->Stop();
	}

	return 0;
}

LRESULT CLoginDlg::OnCheckVersion(WPARAM w,LPARAM l)
{
	m_strVersionUrl = (LPCSTR)l;
	m_bIsNewVersion = (w == 0)?true:false;
	
	if ( w!=0 )
	{
		if ( w == 2 )// 2强制更新
		{
			/*
			XT_LOGOUT lg;
			lg.id = g_ImUserInfo.GetId();
			lg.authData = 0;
			g_pProtocol->SendReq(lg);
			*/
			g_pProtocol->Stop();


			//CVersionInfoDlg *pDlg = new CVersionInfoDlg(w, (LPCSTR)l, GetDesktopWindow());
			//pDlg->ShowWindow(SW_SHOW);

/*
			CVersionUpdatgeDlg *pDlg = new CVersionUpdatgeDlg(12,(LPCSTR)l);
			pDlg->ShowWindow(SW_SHOW);
*/		
			CVersionUpdateDlgDui::Show(12, (LPCSTR)l);
		}
		else  if ( w==1 )//自动后台更新 非强制更新
		{
			//if ( m_hMini139UpdateThread==NULL && g_strMini139UpdateFile=="" )
			//{
			//	DWORD dwThreadId=0;
			//	m_hMini139UpdateThread = CreateThread(NULL,0,
			//					(LPTHREAD_START_ROUTINE)Mini139AutoUpdateProc,
			//					  (LPVOID)l,
			//					  0,
			//					&dwThreadId);
			//}


			//已经忽略过了
			WORD wVKIgnore = 0;
			g_LocalData.GetSoftUpdateIgnoreConfig(wVKIgnore);
			//不提醒
			WORD nVK = 0;
			g_LocalData.GetSoftUpdateConfig(nVK);
			if(nVK != 0 ) //有更新时不要安装，但提醒我
			{
				//第一次提示
				if(wVKIgnore == 0)
				{
					CVersionInfoDlg *pDlg = new CVersionInfoDlg(3, (LPCSTR)l, CWnd::FromHandle(GetDesktopWindow()));
					pDlg->ShowWindow(SW_SHOW);
				}
			}
			//自动更新
			else
			{
				g_pProtocol->Stop();
				/*
				CVersionUpdatgeDlg *pDlg = new CVersionUpdatgeDlg(12,(LPCSTR)l);
				pDlg->ShowWindow(SW_SHOW);
				*/
				CVersionUpdateDlgDui::Show(12, (LPCSTR)l);
			}
			CString strParams;
			/*
			SetCurrentDirectory(g_LocalData.GetPath());
			strParams.Format("miniupdate.exe \"%s\" %u /s",(LPCSTR)l, CIMProtocol::s_nCurVer);

			WinExec(strParams,SW_HIDE);
			*/
			IM_LOG(strParams);
		}
		else if( w == 3) // 非强制更新
		{		
			/*
			CVersionInfoDlg *pDlg = new CVersionInfoDlg(w, (LPCSTR)l, GetDesktopWindow());
			pDlg->ShowWindow(SW_SHOW);
			*/
		}
	}

	if( w != 2 )
	{	
		// 获取自己的详细资料
		XT_USERINFO_GET_REQ req;
		req.fid = g_ImUserInfo.GetId();
		g_pProtocol->SendReq(req);

		// 获取在线客服的资料
		//req.fid = HELPER_ID;
		//g_pProtocol->SendReq(req);

		m_pLblLoginStatus->SetText("获取个人信息...");
		
		//StartHeartBeart();
		// 加载本地缓存的群列表
		//LoadLoacalClusterList();
	}
	return 1;
}

// 取消登录
void CLoginDlg::OnBnClickedCancelLogin()
{
	XT_LOGOUT req;
	req.id = g_ImUserInfo.GetId();
	req.authData = 0;
	g_pProtocol->SendReq(req);
	g_pProtocol->Stop();
	KillTimer(GetHWND(), TIMER_LOGIN);
	if (g_hUserMutex)
	{
		ReleaseMutex(g_hUserMutex);
		CloseHandle(g_hUserMutex);
		g_hUserMutex = NULL;
	}

	g_ImUserInfo.status = XTREAM_OFFLINE;

	//先去掉再次登录
	//if(m_pBtnCancelLogin->GetText() == "重新登录" && m_pLblLoginStatus->GetText() != "用户名或密码错误")
	//{
	//	OnButtonLogin();
	//	return;
	//}

	if(m_pBtnCancelLogin->GetText() == "重置密码")
	{	// 跳转重置密码网页	
#if (defined _VER_PREVIEW_) || (defined _VER_ONLINE_)
		OpenHtmlPage(RESET_PWD_URL); 
#else
		OpenHtmlPage(RESET_PWD_URL); 
#endif
		m_pBtnCancelLogin->SetText("登录");
	}

	if(m_pBtnCancelLogin->GetText() == "绑定手机")
	{	// 跳转绑定手机网页	
#if (defined _VER_PREVIEW_) || (defined _VER_ONLINE_)
		OpenHtmlPage(BIND_MOBIL_URL); 
#else
		OpenHtmlPage(BIND_MOBIL_URL); 
#endif
		m_pBtnCancelLogin->SetText("登录");
	}

	if(m_pBtnCancelLogin->GetText() == "手机验证")
	{	// 打开手机验证窗口	
		CSellerMobileAuthDlg::Show(GetHWND());
		m_pBtnCancelLogin->SetText("登录");
	}

	m_pPage->SelectItem(0);
	m_pEditUserName->SetVisible(true);
	m_pEditPwd->SetVisible(true);
	UpdateBk();
}

void CLoginDlg::OnFinalMessage(HWND hWnd)
{
	g_pProtocol->UnRegistCmd(hWnd);
	DuiLib::WindowImplBase::OnFinalMessage(hWnd);
}

////////////////////////////////////////////////////////////////////////////
std::string PostHttpPage(const std::string& hostName, const std::string& pathName, const std::string& postData)  
{  
	CInternetSession session("User-Agent");
	CString result;
	std::string ret;
	try  
	{  
		INTERNET_PORT nPort = 80;  
		DWORD dwRet = 0;	
		CHttpConnection* pServer = session.GetHttpConnection(hostName.c_str(), nPort);  
		CHttpFile* pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_POST, pathName.c_str());	 
		// 请求头	
		CString strHeaders = "Content-Type: application/x-www-form-urlencoded";
		//开始发送请求	  
		pFile->SendRequest(strHeaders,(LPVOID)postData.c_str(),postData.size());	
		pFile->QueryInfoStatusCode(dwRet); 	 
		if (dwRet == HTTP_STATUS_OK)  
		{	
			CString newline;	
			while(pFile->ReadString(newline))  
			{ 
				//循环读取每行内容  
				result += newline+"\r\n";  
			}	 
		}  
		delete pFile;  
		delete pServer;    
	}  
	catch (CInternetException* pEx)  
	{  
		//catch errors from WinInet  
		TCHAR pszError[200];	
		pEx->GetErrorMessage(pszError, 200);	  
		return ret;  
	}  
	session.Close();	

	ret = std::string((LPCTSTR)result);
	return ret;	
}	 


//模拟浏览器发送HTTP请求函数
std::string HttpRequest(char * lpHostName,short sPort,char * lpUrl,char * lpMethod,char * lpPostData,int nPostDataLen)
{
    HINTERNET hInternet,hConnect,hRequest;

    BOOL bRet;

    std::string strResponse;

    hInternet = (HINSTANCE)InternetOpen("User-Agent",INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
    if(!hInternet)
        goto Ret0;

    hConnect = (HINSTANCE)InternetConnect(hInternet,lpHostName,sPort,NULL,"HTTP/1.1",INTERNET_SERVICE_HTTP,0,0);
    if(!hConnect)
        goto Ret0;

    hRequest = (HINSTANCE)HttpOpenRequest(hConnect,lpMethod,lpUrl,"HTTP/1.1",NULL,NULL,INTERNET_FLAG_RELOAD,0);
    if(!hRequest)
        goto Ret0;

    //bRet = HttpAddRequestHeaders(hRequest,"Content-Type: application/x-www-form-urlencoded",Len(FORMHEADERS),HTTP_ADDREQ_FLAG_REPLACE | HTTP_ADDREQ_FLAG_ADD);
    //if(!bRet)
        //goto Ret0;

    bRet = HttpSendRequest(hRequest,NULL,0,lpPostData,nPostDataLen);
    while(TRUE)
    {
        char cReadBuffer[4096];
        unsigned long lNumberOfBytesRead;
        bRet = InternetReadFile(hRequest,cReadBuffer,sizeof(cReadBuffer) - 1,&lNumberOfBytesRead);
        if(!bRet || !lNumberOfBytesRead)
            break;
        cReadBuffer[lNumberOfBytesRead] = 0;
        strResponse = strResponse + cReadBuffer;
    }

 Ret0:
    if(hRequest)
        InternetCloseHandle(hRequest);
    if(hConnect)
        InternetCloseHandle(hConnect);
    if(hInternet)
        InternetCloseHandle(hInternet);

    return strResponse;
}

/*待下载文件的URL */
/*存放到本地的路径*/ 
bool Download(const CString& strFileURLInServer,const CString & strFileLocalFullPath)
{ 
	ASSERT(strFileURLInServer != ""); 
	ASSERT(strFileLocalFullPath != ""); 
	CInternetSession session; 
	CHttpConnection* pHttpConnection = NULL; 
	CHttpFile* pHttpFile = NULL; 
	CString strServer, strObject; 
	INTERNET_PORT wPort; 
	bool bReturn = false; 

	DWORD dwType; 
	const int nTimeOut = 2000; 
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); 	//重试之间的等待延时 
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); 			//重试次数 
	char* pszBuffer = NULL; 

	try 
	{ 
		AfxParseURL(strFileURLInServer, dwType, strServer, strObject, wPort); 
		pHttpConnection = session.GetHttpConnection(strServer, wPort); 
		pHttpFile = pHttpConnection->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject); 
		if(pHttpFile->SendRequest() == FALSE) 
		return false; 
		DWORD dwStateCode; 

		pHttpFile->QueryInfoStatusCode(dwStateCode); 
		if(dwStateCode == HTTP_STATUS_OK) 
		{ 
			HANDLE hFile = CreateFile(strFileLocalFullPath, GENERIC_WRITE, 
			FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 
			NULL); //创建本地文件 
			if(hFile == INVALID_HANDLE_VALUE) 
			{ 
				pHttpFile->Close(); 
				pHttpConnection->Close(); 
				session.Close(); 
				return false; 
			} 

			char szInfoBuffer[1000]; //返回消息 
			DWORD dwFileSize = 0; //文件长度 
			DWORD dwInfoBufferSize = sizeof(szInfoBuffer); 
			BOOL bResult = FALSE; 
			bResult = pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, 
			(void*)szInfoBuffer,&dwInfoBufferSize,NULL); 

			dwFileSize = atoi(szInfoBuffer); 
			const int BUFFER_LENGTH = 1024 * 10; 
			pszBuffer = new char[BUFFER_LENGTH]; //读取文件的缓冲 
			DWORD dwWrite, dwTotalWrite; 
			dwWrite = dwTotalWrite = 0; 
			UINT nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH); //读取服务器上数据 

			while(nRead > 0) 
			{ 
				WriteFile(hFile, pszBuffer, nRead, &dwWrite, NULL); //写到本地文件 
				dwTotalWrite += dwWrite; 
				nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH); 
			} 

			delete[]pszBuffer; 
			pszBuffer = NULL; 
			CloseHandle(hFile); 
			bReturn = true; 
		} 
	} 
	catch(CInternetException* e) 
	{ 
		TCHAR tszErrString[256]; 
		e->GetErrorMessage(tszErrString, strlen(tszErrString)); 
		TRACE(_T("Download XSL error! URL: %s,Error: %s"), strFileURLInServer, tszErrString); 
		e->Delete(); 
	} 
	catch(...) 
	{ 
	} 

	if(pszBuffer != NULL) 
	{ 
		delete[]pszBuffer; 
	} 
	if(pHttpFile != NULL) 
	{ 
		pHttpFile->Close(); 
		delete pHttpFile; 
	} 
	if(pHttpConnection != NULL) 
	{ 
		pHttpConnection->Close(); 
		delete pHttpConnection; 
	} 
	session.Close(); 
	return bReturn; 
} 