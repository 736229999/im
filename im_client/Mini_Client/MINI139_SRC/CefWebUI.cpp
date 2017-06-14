#include "StdAfx.h"
#include "CefWebUI.h"
#include "cef_app.h"

class CCefWnd : public DuiLib::CWindowWnd
{
public:
	CCefWnd();
	~CCefWnd();
	void Init(CCefWebUI* pOwner);
	RECT CalPos();

	LPCTSTR GetWindowClassName() const;
	void OnFinalMessage(HWND hWnd);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void SetUrl(CCefWebUI *pOwner, const char *pUrl);
	void MoveWindow(RECT rc);
	void SetCallbackFun(CallBackFun p, DWORD dwDlg);
	void ShowWindow(bool bShow)
	{
		if(m_bInit)
		{
			HWND hWnd = m_cWebClient->GetBrowser()->GetWindowHandle();
			::ShowWindow(hWnd, bShow ? SW_SHOW : SW_HIDE);
		}
	}

	CCefWebUI* GetCefWebUI();
protected:
	CCefWebUI* m_pOwner;
	CefRefPtr<CWebClient>  m_cWebClient;
	BOOL m_bInit;
};

CCefWebUI* CCefWnd::GetCefWebUI()
{
	return m_pOwner;
}


CCefWnd::CCefWnd() : m_pOwner(NULL)
{
	m_bInit = FALSE;
	CefRefPtr<CWebClient> client(new CWebClient());
	m_cWebClient= client;
	m_cWebClient->SetCefWnd(this);
}

CCefWnd::~CCefWnd()
{
	m_cWebClient->Release();
	if(m_pOwner!=NULL)
	{
		m_pOwner->Invalidate();
		m_pOwner->m_pWindow = NULL;
	}
}

void CCefWnd::Init(CCefWebUI* pOwner)
{
	m_pOwner = pOwner;
	RECT rcPos = CalPos();

	CefWindowInfo info;
	RECT rectnew=rcPos;
	rectnew.top=rcPos.top+1;
	rectnew.bottom=rcPos.bottom-1;
	rectnew.left=rcPos.left+1;
	rectnew.right=rcPos.right-1;
	Create(m_pOwner->GetManager()->GetPaintWindow(), NULL, /*UI_WNDSTYLE_CHILD*/UI_CLASSSTYLE_CHILD, 0, rectnew);
	info.SetAsChild(GetHWND(),rectnew);
	m_bInit = TRUE;    
}

RECT CCefWnd::CalPos()
{
	DuiLib::CDuiRect rcPos = m_pOwner->GetPos();

	return rcPos;
}

LPCTSTR CCefWnd::GetWindowClassName() const
{
	return _T("CefWnd");
}

void CCefWnd::OnFinalMessage(HWND /*hWnd*/)
{
}

LRESULT CCefWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if(uMsg == WM_IS_NAVEGATE)
	{
		const char* pUrl = (char*)wParam;
		return m_pOwner->IsNavegate(pUrl);
	}
	if(uMsg == WM_RBUTTON)
	{
		return m_pOwner->Rbutton();
	}
	LRESULT lRt = CWindowWnd::HandleMessage(uMsg, wParam, lParam);
	return lRt;
}

void CCefWnd::SetUrl(CCefWebUI *pOwner, const char *pUrl)
{
	if(!m_bInit)
	{
		Init(pOwner);
	}

	//Sleep(300);
	CefRefPtr<CefBrowser>browser = m_cWebClient->GetBrowser();
	if(browser)
	{
		
		CefRefPtr<CefFrame> mf = browser->GetMainFrame();
		mf->LoadURL(pUrl);
	}
	else
	{
		RECT rcPos = CalPos();
		CefWindowInfo info;
		RECT rectnew=rcPos;
		rectnew.top=rcPos.top+1;
		rectnew.bottom=rcPos.bottom-1;
		rectnew.left=rcPos.left+1;
		rectnew.right=rcPos.right-1;
		info.SetAsChild(m_pOwner->GetManager()->GetPaintWindow(),rectnew);
		CefBrowserSettings browserSettings;
		CefBrowser::CreateBrowser(info, static_cast<CefRefPtr<CefClient> >(m_cWebClient),pUrl,browserSettings);
	}
}

void CCefWnd::MoveWindow(RECT rc)
{
	if(m_bInit)
	{
		CefRefPtr<CefBrowser>browser = m_cWebClient->GetBrowser();
		if(browser)
		{
			CefWindowHandle hwnd = browser->GetWindowHandle();
			::MoveWindow(hwnd,rc.left,rc.top,rc.right-rc.left,rc.bottom-rc.top, true); 
			browser->SetSize(PET_VIEW, rc.right-rc.left,rc.bottom-rc.top);

			CefRefPtr<CefFrame> mf = browser->GetMainFrame();
			//browser->SetSize(PET_VIEW, rc.right-rc.left,rc.bottom-rc.top);
		}
	}
}

void CCefWnd::SetCallbackFun(CallBackFun p, DWORD dwDlg)
{
	m_cWebClient->SetCallbackFun(p, dwDlg);
}

CCefWebUI::CCefWebUI(void)
{
	m_pWindow = new CCefWnd();
}

CCefWebUI::~CCefWebUI(void)
{
	if(m_pWindow != NULL && m_pWindow->GetCefWebUI() != NULL)
	{
		delete m_pWindow;
		m_pWindow = NULL;
	}
}

// 初始化
void CefInit()
{
	CefSettings cSettings;
	CefSettingsTraits::init(&cSettings);
	cSettings.multi_threaded_message_loop= true;
	CefRefPtr<CefApp> spApp;
	CefInitialize(cSettings, spApp);   
}

// 卸载
void CefUninit()
{
	CefShutdown();
}

void CCefWebUI::SetUrl(const char *pUrl)
{
	m_pWindow->SetUrl(this, pUrl);
}

bool CCefWebUI::IsNavegate(const char* pUrl)
{
	LRESULT bIsNavege = ::SendMessage(m_pParentWnd->GetHWND(), WM_IS_NAVEGATE, (DWORD)pUrl, NULL);
	return bIsNavege;
}

bool CCefWebUI::Rbutton()
{
	LRESULT bIsNavege = ::SendMessage(m_pParentWnd->GetHWND(), WM_RBUTTON, NULL, NULL);
	return bIsNavege;
}

void CCefWebUI::DoEvent(DuiLib::TEventUI& event)
{
	DuiLib::CControlUI::DoEvent(event);
}

void CCefWebUI::SetPos(RECT rc)
{
	DuiLib::CControlUI::SetPos(rc);
	if(m_pWindow != NULL)
	{
		m_pWindow->MoveWindow(rc);
	}
}


void CCefWebUI::SetParentWnd(DuiLib::WindowImplBase* pParentWnd)
{
	m_pParentWnd = pParentWnd;
}

//void CCefWebUI::SetCallbackFun(CallBackFun p, DWORD dwDlg)
//{
//	if(m_pWindow != NULL)
//	{
//		m_pWindow->SetCallbackFun(p, dwDlg);
//	}
//}

void CCefWebUI::SetInternVisible(bool bVisible /*= true*/) // 仅供内部调用，有些UI拥有窗口句柄，需要重写此函数
{
	if(m_pWindow != NULL)
	{
		m_pWindow->ShowWindow(bVisible);
	}
}