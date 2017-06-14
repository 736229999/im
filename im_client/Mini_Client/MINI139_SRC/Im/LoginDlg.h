#pragma once

#include "impublic.h"
#include "localdata.h"
#include "afxwin.h"
#include "hintdlg2.h"
#include "buttonext.h"
#include "..\..\SpaceManagement\public\public_controls\buttonext.h"
#include "..\..\..\HmgSkin\HmgSkin\CheckButton.h"


#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"


class CLoginDlg : public DuiLib::WindowImplBase
{
	//DECLARE_XT_MESSAGE_MAP()
protected:
	LRESULT OnXtMessage(WPARAM wParam, LPARAM lParam);
	LRESULT	OnXtResend(WPARAM wParam, LPARAM lParam);

public:
	enum LOGIN_STATUS
	{
		STATUS_NOT_LOGIN=0,
		STATUS_LOGINING,
		STATUS_LOGIN_SUCCESS,
	};

	static std::tr1::shared_ptr<CLoginDlg> Show(int nSelPage = 0, string strUserName="", string strErrInfo = "");
	static void UpdateBk();
	
public:
	CLoginDlg();
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("LoginDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("LoginDlg.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	//virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	// 消息
	void Notify(DuiLib::TNotifyUI& msg);
	void OnFinalMessage( HWND hWnd );

private:
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	bool LoadServerList();
	void InitPara();
	
	void OnCbnSelchangeComboName();
	void SaveLoginAccount();

	void OnLoginReq(const char *data,WORD wRet);
	void OnLoginAck(const char *data,WORD wRet);
	void OnUserInfoGet(const char *data,WORD wRet);
	void OnFriendListAck(const char *data,WORD wRet);
	void OnNetWorkErr(const char * data,WORD wRet);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnImOk(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnCheckVersion(WPARAM w,LPARAM l);
	afx_msg void OnButtonLogin();
	afx_msg void OnBnClickedCancelLogin();
	////此处加载自定义控件
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (_tcsicmp(pstrClass, _T("ButtonGif")) == 0)
		{
			return	new DuiLib::CButtonGifUI();
		}
		return NULL;
	}
public:
	XT_LOGIN_REQ		m_LoginReq;

	static CString		m_strServerIp	;
	static WORD			m_wServerPort;
	static CString	    m_strLstLoginUserName  ;
	static int			g_LoginStatus;
	static int	        m_LoginFlag  ;

private:
	bool	m_bEnableAutoLoginFunction;
	USER_ACCOUNT_LIST  m_AccountList  ;
	BOOL m_bShowHint;
	CString			m_strLogoFile;
	DWORD			m_dwTranServerIP;
	WORD			m_wTranServerPort;

	USER_INFO       m_UserInfo;	

	// 用户用户名下拉列表的数据
	std::vector<std::string> m_vecUserName;
	int m_nSel;

	LOGIN_STATUS m_euLoginStatus	; //0:not logined, 1 logining.., 2:logined
	int m_nUserStatus;
	bool m_bImOk;

	CString	m_strVersionUrl;
	bool m_bIsNewVersion;

private:
	static std::tr1::shared_ptr<CLoginDlg> pInstance;

	DuiLib::CTabLayoutUI *m_pPage;
	DuiLib::CButtonUI *m_pBtnLogin;
	DuiLib::CButtonUI *m_pBtnCancelLogin;
	DuiLib::CButtonUI *m_pBtnStatus;
	DuiLib::CButtonUI *m_pBtnOtherUser;
	DuiLib::CHorizontalLayoutUI *m_pHLayoutName;
	DuiLib::COptionUI *m_pCheckRememberPwd;
	DuiLib::COptionUI *m_pCheckAutoLogin;
	DuiLib::CEditUI *m_pEditUserName;
	DuiLib::CEditUI *m_pEditPwd;
	DuiLib::CButtonUI *m_pRegister;
	DuiLib::CButtonUI *m_pForgetPwd;
	DuiLib::CLabelUI *m_pTitle;
	DuiLib::CButtonGifUI *m_pLblUserImg;
	DuiLib::CButtonGifUI *m_pLblUserImgPage1;
	DuiLib::CLabelUI *m_pLblLoginStatusImg;
	DuiLib::CLabelUI *m_pLblUserName;
	DuiLib::CLabelUI *m_pLblLoginStatus;
	DuiLib::CGifAnimUI *m_pGifLogin;

	BOOL m_bCheckDiffPlaces;		// 是否校验异地登录
};