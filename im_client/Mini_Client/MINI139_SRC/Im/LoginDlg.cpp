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
#define TIMER_DISCONNECT 0x06	// ��������Ͽ�����

// CLoginDlg dialog
CString CLoginDlg::m_strLstLoginUserName = "";
CString CLoginDlg::m_strServerIp = "";
WORD	CLoginDlg::m_wServerPort = 0;
int		CLoginDlg::g_LoginStatus = 0;
static DWORD dwLoginReqTick = 0;
int     CLoginDlg::m_LoginFlag   = 0;

#ifdef _VER_INTERNAL_			// �ڲ���
#include <Winldap.h>
#pragma  comment(lib,"Wldap32.lib")
#define AD_HOSTNAME "fn.com"	// AD������ַ
#define AD_PORT 389				// AD�����˿�

// AD��֤�ӿ�
BOOL AD_Auth(LPCTSTR szUserName, LPCTSTR szPwd, string &strErrorInfo)
{
	// ��ʼ��
	LDAP *pLdap = ldap_init(AD_HOSTNAME, AD_PORT);
	if(pLdap == NULL)
	{
		strErrorInfo = "����AD��֤������ʧ�ܣ�";
		return FALSE;
	}

	// ����AD��֤������
	ULONG rlt = ldap_connect(pLdap, NULL);
	if(LDAP_SUCCESS != rlt)
	{
		ldap_unbind_s(pLdap);
		strErrorInfo = "����AD��֤������ʧ�ܣ�";
		return FALSE;
	}

	// ��֤
	CString strUser;
	strUser.Format("FN\\%s", szUserName);
	rlt = ldap_simple_bind_s(pLdap, (LPSTR)(LPCTSTR)strUser, (LPSTR)szPwd);
	if(LDAP_SUCCESS != rlt)
	{
		ldap_unbind_s(pLdap);
		strErrorInfo = "��֤ʧ�ܣ��û������������";
		return FALSE;
	}

	ldap_unbind_s(pLdap);
	strErrorInfo = "��֤�ɹ���";
	return TRUE;	
}
#endif 

BEGIN_XT_MESSAGE_MAP(CLoginDlg)
	// ��¼Ӧ��
	ON_XT_COMMAND(CMD_LOGIN_ACK,OnLoginAck)
	// ��¼����
	ON_XT_COMMAND(CMD_LOGIN_REQ,OnLoginReq)
	// ��ȡ�û���ϸ��Ϣ
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoGet)
	// ��ȡ�����б�
	ON_XT_COMMAND(CMD_FRIENDLIST_ACK,OnFriendListAck)
	// �����������
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

		// ����ͷ��
		string strHeadImg = g_LocalData.GetHeadImage(pInstance->m_pEditUserName->GetText().GetData());
		CString strGifFile =  "";
		strGifFile =  strHeadImg.c_str();
		strGifFile =  strGifFile+ ".gif";////�ļ�����
		if(FileExist(strHeadImg.c_str()))
		{
			//if ( FileExist(strGifFile) )
			//{
			//	pInstance->m_pLblUserImg->SetBkImage("");
			//	pInstance->m_pLblUserImg->SetNormalGifFile(strGifFile);

			//	pInstance->m_pLblUserImgPage1->SetBkImage("");
			//	pInstance->m_pLblUserImgPage1->SetNormalGifFile(strGifFile);
			//}
			//else////��GIF
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
	::RegSetValueEx(hKey,"Ĭ��",NULL,REG_SZ,(LPBYTE)szTemp,strlen(szTemp));

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
	// ��ֹ�������
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

	// �����̨������Ϣ
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
	m_pTitle->SetText(FEINIU_CS_PLAT);//��ţ�ͷ�ƽ̨
#elif defined _WORK_EX_
	m_pTitle->SetText(CLIENT_NAME_WORK);//������
  #else
	m_pTitle->SetText(CLIENT_NAME_SELLER);//�̼Ұ�
  #endif
#else
	m_pTitle->SetText(CLIENT_NAME_BUYER);
#endif

	if(LoadServerList() == false)
	{
		CMessageBox::Model(GetHWND(), "���ط����������ļ�ʧ��!", IM_CAPTION, MB_OK|MB_ICONWARNING);
		PostQuitMessage(-1);
		Close();
	}

	InitPara();
	UpdateBk();
	OnCbnSelchangeComboName();
	//��������Զ���¼�����Ǽ�ס����
	if ( m_vecUserName.size()==0 )
	{
		m_pCheckAutoLogin->Selected(false);
	}

	SetTimer(GetHWND(), TIMER_PASSWORD_SETFOCUS, 5, NULL);

	// ����Զ���¼������ʾ��¼�Ի���ֱ�ӽ���������
	if ( m_pCheckAutoLogin->IsSelected() && m_bEnableAutoLoginFunction)
	{
		OnButtonLogin();
	}

#ifdef _WORK_EX_
	m_pEditUserName->SetAttribute("tipvalue", "��Ӫ��̨�ʺ�");
#endif
}

// ��Ϣ
void CLoginDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if(msg.sType == "click")
	{
		if(msg.pSender == m_pBtnLogin)
		{	// ��¼
			OnButtonLogin();
		}
		else if(msg.pSender == m_pBtnCancelLogin)
		{	// ȡ����¼
			OnBnClickedCancelLogin();
		}
		else if(msg.pSender == m_pBtnStatus)
		{	// �޸�״̬
			RECT rcBtn = m_pBtnStatus->GetPos();
			DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
			DuiLib::CPoint point(rcBtn.right+5,rcBtn.top);
			ClientToScreen(m_hWnd, &point);
			pMenu->Init(NULL, _T("loginstatus.xml"), point, &m_PaintManager);
		}
		else if(msg.pSender == m_pBtnOtherUser)
		{	// �л��û�
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
		{	// ע��
#ifdef _BUSINESS_VESION
			OpenHtmlPage(SELLER_REGISTER_URL);
#else
			OpenHtmlPage(BUYER_REGISTER_URL);
#endif
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, NULL);
		}
		else if(msg.pSender == m_pForgetPwd)
		{	// ��������
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
		{	// ��ס����
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
	if (uMsg == WM_MENUCLICK)//����˵������Ϣ
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
	// �������ļ���ȡ��������
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
#ifdef BUSINESS_VESION	// ��Ұ棬�ж��Ƿ���web�˵���
		if(theApp.m_strCustomerName != "")
		{
			m_strLstLoginUserName = theApp.m_strCustomerName.c_str();
		}
#endif
#ifdef _WORK_EX_	// ��Ұ棬�ж��Ƿ���web�˵���
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

	//������ע���ʻ������б���Ĭ��ѡ���ϴε�¼�ʺ�
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

			//Ĭ��ѡ���ϴε�¼�ʺ�
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
		if(m_vecUserName.size() > 0)////�����û�����
		{
			m_vecUserName.push_back("ʹ�������˺ŵ�½");	
		}
	}

//#ifndef _BUSINESS_VESION	// ��Ұ棬web�˵���ʱ�Զ������û���
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
	TRACE("��¼ѹ�� username[%s] password[%s]\n", pNew->uLoginKey.username, pNew->password);
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
		str.Format("�û������󣡴���ԭ��%s",strError);
		CMessageBox::Model(GetHWND(), str, IM_CAPTION, MB_OK|MB_ICONWARNING);
		m_pEditUserName->SetFocus();
		return;
	}

	if(strlen(strUserName) > MAX_USERNAME_LEN)
	{
		CMessageBox::Model(GetHWND(), "�û�������", IM_CAPTION, MB_OK|MB_ICONWARNING);
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
// 		ShowMessage("�û��������������",MB_OK|MB_ICONINFORMATION);
// 		return;
// 	}
	//m_LoginReq.uLoginKey.id = nMemberId;
// 	m_UserInfo.id = nMemberId;
// 
// 	m_UserInfo.merchant_id = nMerchantId;
/////////////////////////////////////////
  #ifdef _CS_PLAT_EX_
	 m_LoginReq.login_flag |= (XT_LOGIN_REQ::CS<<2); ////��ţ�ͷ�ƽ̨ 
	 m_LoginReq.auto_session = rand()%256;
#elif defined _WORK_EX_
	m_LoginReq.login_flag |= (XT_LOGIN_REQ::WORK<<2); ////������
  #else
	 m_LoginReq.login_flag |= (XT_LOGIN_REQ::SELLER << 2); /////�̼Ұ�
  #endif
	
#else
// 	if (!BuyerPswCheck(strUserName, strPass))
// 	{
// 		ShowMessage("�û��������������", MB_OK | MB_ICONINFORMATION);
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
	// �ϴε�¼���û���
	static CString strPrevLoginName="";
	CString strMutex;
	strMutex.Format("xid_%s",m_LoginReq.uLoginKey.username);

	if ( strPrevLoginName != "" && strUserName!=strPrevLoginName )
	{
		//�����û�����¼,������ǰ��mutex
		if ( g_hUserMutex )
		{
			ReleaseMutex(g_hUserMutex);
			CloseHandle(g_hUserMutex);
			g_hUserMutex = NULL;
		}
	}
	// ��ǰ��û�е�¼�ɹ�
	if(g_hUserMutex==NULL)
	{
		g_hUserMutex=::CreateMutex(NULL,FALSE,strMutex);

		if ( GetLastError()==ERROR_ALREADY_EXISTS )
		{
			CString str;
			str.Format("�����ڱ���������ظ�ʹ��ͬһ�ʻ���%s��������ʻ���",m_LoginReq.uLoginKey.username);
			CMessageBox::Model(GetHWND(), str,"��ʾ",MB_OK|MB_ICONWARNING);
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
//#ifdef _VER_INTERNAL_	// �ڲ��棬��ʹ�����˺���֤
//	string strErrorInfo;	// ���մ�����Ϣ
//	BOOL bAuth = AD_Auth(strUserName, m_LoginReq.password, strErrorInfo);
//	if(!bAuth)
//	{	// ��֤ʧ��
//		CMessageBox::Model(GetHWND(), strErrorInfo.c_str(),IM_CAPTION,MB_OK|MB_ICONWARNING);
//		SetTimer(GetHWND(), TIMER_PASSWORD_SETFOCUS, 5, NULL);		// 5ms�� �û������ý���
//		return;
//	}
//#endif

	// ʹ���û�����¼
	bool bMobile=false;

	//�ж��Ƿ����ֻ���
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
	CLoginDlg::m_LoginFlag = m_LoginReq.login_flag;////�����û��ĵ�¼��Ϣ��ʶ
	m_UserInfo.bShowHint = m_bShowHint;
	m_UserInfo.status = XTREAM_OFFLINE;
	//m_UserInfo.bHideLogin = m_btnCheckHide.GetCheck();
	m_UserInfo.bAutoLogin = m_pCheckAutoLogin->IsSelected();
	
	m_UserInfo.bRemeberPsw = m_pCheckRememberPwd->IsSelected();
	

	m_UserInfo.SetUserName((LPSTR)(LPCTSTR)strUserName);
	m_UserInfo.SetPassword((LPSTR)(LPCTSTR)strPass);

	//���µ�ǰ�û�����
	g_ImUserInfo = m_UserInfo;
	//�ͻ��˾�����ip��*�����ڴ�����ļ���ת������ͨѶʱ�Ŀͻ���IP��
	//�ͻ��˾�����port��*�����ڴ�����ļ���ת������ͨѶʱ�Ŀͻ��˶˿ڣ�

#ifdef _LOGIN_TEST_		// ��¼ѹ��
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
		TRACE("��¼ѹ�� username[%s] password[%s]\n", strLine, m_LoginReq.password);
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
			if(	iIndex == iCount-1)////���һ��
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

		SetTimer(GetHWND(), TIMER_USERNAME_SETFOCUS, 5, NULL);		// 5ms�� �û������ý���
	}	
}

void CLoginDlg::OnLoginReq(const char *data,WORD wRet)
{
	m_nUserStatus  =(DWORD)data;

	if ( m_euLoginStatus != CLoginDlg::STATUS_LOGINING )
	{
		m_euLoginStatus = CLoginDlg::STATUS_LOGINING;
		m_pBtnCancelLogin->SetText("ȡ����¼");
		m_pLblLoginStatus->SetText("�û���Ϣ��֤��");
		//UpdateUI();
	}

	/*CString str;
	str.Format("���ڻ�ȡ�û�(%s)��Ϣ...",g_ImUserInfo.GetUserName().c_str());
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
		g_LocalData.SetLoginTime(resp->login_time);//��ȡ������ʱ��
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

		//// ��¼�ɹ���ͼ���ȫ�ֱ����б�
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
		// ��¼�ɹ��󣬾ͱ����¼�ʻ�
		SaveLoginAccount();

		////������Ϣ����
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

		////��¼�ɹ���ʾ�°�����
		//if(g_LocalData.ReadGuidePageSetting("GuidePage0610"))
		//{
		//	vector<CString> vecPage;
		//	vecPage.push_back("GuidePage0610\\page1.jpg");
		//	vecPage.push_back("GuidePage0610\\page2.jpg");

		//	DuiLib::CPaintManagerUI::SetInstance(AfxGetInstanceHandle());                    // ָ��duilib��ʵ��
		//	DuiLib::CPaintManagerUI::SetResourcePath(DuiLib::CPaintManagerUI::GetInstancePath());    // ָ��duilib��Դ��·��������ָ��Ϊ��exeͬĿ¼
		//	if(m_pDuiGuidePage == NULL)
		//	{
		//		m_pDuiGuidePage = new CDuiGuidePage;
		//		m_pDuiGuidePage->SetPage(vecPage);
		//		m_pDuiGuidePage->Create(NULL, _T("�°�����"), NULL, NULL/*WS_EX_STATICEDGE | WS_EX_APPWINDOW*/, 0, 0, 700, 496);
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
		m_pBtnCancelLogin->SetText("���µ�¼");
		m_pGifLogin->SetVisible(false);
		//UpdateUI();
		
		if ( wRet==SERVER_TIMEOUT )
		{	
			strcpy(error,"��¼��ʱ");
			m_pLblLoginStatus->SetText(error);
			//SetTimer(CMainFrame::GetInstance()->GetHWND(), CMainFrame::TIMER_RELOGIN,1000,NULL);
		}
		else
		{
			//ShowTaskIcon(g_ImUserInfo.sex,XTREAM_OFFLINE);
			strcpy(error,g_pProtocol->GetLastError());
		}
		m_pLblLoginStatus->SetText(error);

		if(strcmp(error, "���˺��ѱ����Ƶ�¼�������������룬���Խ������") == 0)
		{
			m_pBtnCancelLogin->SetText("��������");
		}

		if(strcmp(error, "���ĵ�¼�����쳣��Ϊ�������˻���ȫ���뾡����ֻ�") == 0)
		{
			m_pBtnCancelLogin->SetText("���ֻ�");
		}

		if(strcmp(error, "���ĵ�¼�����쳣��������ֻ���֤") == 0)
		{
			m_pBtnCancelLogin->SetText("�ֻ���֤");
		}

		//SetTaskIconTip(error);
		//ShowTaskIcon(g_ImUserInfo.sex,XTREAM_OFFLINE);
		if ( wRet!=0 )
		{
			g_ImUserInfo.status=XTREAM_OFFLINE;
			PostMessage(WM_LOGIN_FAILED, 0, (LPARAM)error);
		}
		// ����Ӧ��̨����Ϣ��Ӧʱ����ֱ�ӶϿ����ӣ��ᵼ�¹رճ���ʱ������Ӧ�õ���Ӧ����������ٵ���
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
			//�����ް��Ա�û�в�ȷ������,һ��Ҫ����Ů,���ûָ��,Ĭ����Ů��
			if( pFriend->sex >= 2 )
			{
				pFriend->sex = 0;
			}
			g_LocalData.SetUser(g_ImUserInfo.GetUserName().c_str(), g_ImUserInfo.GetUserFileName().c_str());
			g_webUserHeadIconProcess.GetUserHeadIcon(&g_ImUserInfo,CWnd::FromHandle(GetHWND()));

			//ShowUserInfo();		

			// ��ȡ�������ϳɹ��󣬿�ʼ��ȡ�����б�ͺ��ѷ�����Ϣ
			if ( m_bImOk==false )
			{
				m_bImOk = true;
				PostMessage(WM_IM_OK,0,0);
			}
/*	��Ŀ����
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
				ShowMessage("���˿ռ��û���Ϣ����ʧ�ܣ�����Ҫ��������IEC-iM��������ʹ�á�");
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
				strcpy(error,"���������������Ϣ��ʱ��");	
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
		const char error[] = "�������ӶϿ�,������������";
		// �������ӶϿ�
		KillTimer(GetHWND(), TIMER_LOGIN);

		//m_pPage1->m_pageFriend.OnNetWorkError();

		m_euLoginStatus = CLoginDlg::STATUS_NOT_LOGIN;
		m_pBtnCancelLogin->SetText("���µ�¼");
		m_pGifLogin->SetVisible(false);
		m_pLblLoginStatus->SetText(error);
		//SetTaskIconTip(error);
		//ShowTaskIcon(g_ImUserInfo.sex,XTREAM_OFFLINE);
		g_ImUserInfo.status=XTREAM_OFFLINE;
		//UpdateUI();
		// ����Ӧ��̨����Ϣ��Ӧʱ����ֱ�ӶϿ����ӣ��ᵼ�¹رճ���ʱ������Ӧ�õ���Ӧ����������ٵ���
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
		{	// �Ͽ����̨����
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
		m_pBtnCancelLogin->SetText("���µ�¼");
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
		if ( w == 2 )// 2ǿ�Ƹ���
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
		else  if ( w==1 )//�Զ���̨���� ��ǿ�Ƹ���
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


			//�Ѿ����Թ���
			WORD wVKIgnore = 0;
			g_LocalData.GetSoftUpdateIgnoreConfig(wVKIgnore);
			//������
			WORD nVK = 0;
			g_LocalData.GetSoftUpdateConfig(nVK);
			if(nVK != 0 ) //�и���ʱ��Ҫ��װ����������
			{
				//��һ����ʾ
				if(wVKIgnore == 0)
				{
					CVersionInfoDlg *pDlg = new CVersionInfoDlg(3, (LPCSTR)l, CWnd::FromHandle(GetDesktopWindow()));
					pDlg->ShowWindow(SW_SHOW);
				}
			}
			//�Զ�����
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
		else if( w == 3) // ��ǿ�Ƹ���
		{		
			/*
			CVersionInfoDlg *pDlg = new CVersionInfoDlg(w, (LPCSTR)l, GetDesktopWindow());
			pDlg->ShowWindow(SW_SHOW);
			*/
		}
	}

	if( w != 2 )
	{	
		// ��ȡ�Լ�����ϸ����
		XT_USERINFO_GET_REQ req;
		req.fid = g_ImUserInfo.GetId();
		g_pProtocol->SendReq(req);

		// ��ȡ���߿ͷ�������
		//req.fid = HELPER_ID;
		//g_pProtocol->SendReq(req);

		m_pLblLoginStatus->SetText("��ȡ������Ϣ...");
		
		//StartHeartBeart();
		// ���ر��ػ����Ⱥ�б�
		//LoadLoacalClusterList();
	}
	return 1;
}

// ȡ����¼
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

	//��ȥ���ٴε�¼
	//if(m_pBtnCancelLogin->GetText() == "���µ�¼" && m_pLblLoginStatus->GetText() != "�û������������")
	//{
	//	OnButtonLogin();
	//	return;
	//}

	if(m_pBtnCancelLogin->GetText() == "��������")
	{	// ��ת����������ҳ	
#if (defined _VER_PREVIEW_) || (defined _VER_ONLINE_)
		OpenHtmlPage(RESET_PWD_URL); 
#else
		OpenHtmlPage(RESET_PWD_URL); 
#endif
		m_pBtnCancelLogin->SetText("��¼");
	}

	if(m_pBtnCancelLogin->GetText() == "���ֻ�")
	{	// ��ת���ֻ���ҳ	
#if (defined _VER_PREVIEW_) || (defined _VER_ONLINE_)
		OpenHtmlPage(BIND_MOBIL_URL); 
#else
		OpenHtmlPage(BIND_MOBIL_URL); 
#endif
		m_pBtnCancelLogin->SetText("��¼");
	}

	if(m_pBtnCancelLogin->GetText() == "�ֻ���֤")
	{	// ���ֻ���֤����	
		CSellerMobileAuthDlg::Show(GetHWND());
		m_pBtnCancelLogin->SetText("��¼");
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
		// ����ͷ	
		CString strHeaders = "Content-Type: application/x-www-form-urlencoded";
		//��ʼ��������	  
		pFile->SendRequest(strHeaders,(LPVOID)postData.c_str(),postData.size());	
		pFile->QueryInfoStatusCode(dwRet); 	 
		if (dwRet == HTTP_STATUS_OK)  
		{	
			CString newline;	
			while(pFile->ReadString(newline))  
			{ 
				//ѭ����ȡÿ������  
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


//ģ�����������HTTP������
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

/*�������ļ���URL */
/*��ŵ����ص�·��*/ 
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
	session.SetOption(INTERNET_OPTION_CONNECT_TIMEOUT, nTimeOut); 	//����֮��ĵȴ���ʱ 
	session.SetOption(INTERNET_OPTION_CONNECT_RETRIES, 1); 			//���Դ��� 
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
			NULL); //���������ļ� 
			if(hFile == INVALID_HANDLE_VALUE) 
			{ 
				pHttpFile->Close(); 
				pHttpConnection->Close(); 
				session.Close(); 
				return false; 
			} 

			char szInfoBuffer[1000]; //������Ϣ 
			DWORD dwFileSize = 0; //�ļ����� 
			DWORD dwInfoBufferSize = sizeof(szInfoBuffer); 
			BOOL bResult = FALSE; 
			bResult = pHttpFile->QueryInfo(HTTP_QUERY_CONTENT_LENGTH, 
			(void*)szInfoBuffer,&dwInfoBufferSize,NULL); 

			dwFileSize = atoi(szInfoBuffer); 
			const int BUFFER_LENGTH = 1024 * 10; 
			pszBuffer = new char[BUFFER_LENGTH]; //��ȡ�ļ��Ļ��� 
			DWORD dwWrite, dwTotalWrite; 
			dwWrite = dwTotalWrite = 0; 
			UINT nRead = pHttpFile->Read(pszBuffer, BUFFER_LENGTH); //��ȡ������������ 

			while(nRead > 0) 
			{ 
				WriteFile(hFile, pszBuffer, nRead, &dwWrite, NULL); //д�������ļ� 
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