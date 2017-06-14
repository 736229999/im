#pragma once
#include <cef_client.h>
#include "..\..\DuiLib\UIlib.h"
#include "CefWebUI.h"
typedef BOOL (*CallBackFun)(DWORD dwDlg, char *p); // 为回调函数命名，类型命名为 CallBackFun，参数为char *p
class CWebClient : public CefClient, 
				   public CefLifeSpanHandler, 
				   public CefRequestHandler,
				   public CefMenuHandler
{
protected:
	CefRefPtr<CefBrowser> m_Browser;
	DuiLib::CWindowWnd* m_pCefWnd;

public:
	CWebClient(void)
	{
		m_fun = NULL;
		m_dwDlg = 0;
	}

	virtual ~CWebClient(void){};

	void SetCallbackFun(CallBackFun p, DWORD dwDlg);

	CefRefPtr<CefBrowser> GetBrowser() { return m_Browser; }


	virtual CefRefPtr<CefLifeSpanHandler> GetLifeSpanHandler() OVERRIDE
		{ return this; }
	virtual CefRefPtr<CefRequestHandler> GetRequestHandler() OVERRIDE
		{ return this; }


	virtual void OnAfterCreated(CefRefPtr<CefBrowser> browser) OVERRIDE;

	virtual bool OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
								CefRefPtr<CefFrame> frame,
								CefRefPtr<CefRequest> request,
								NavType navType,
								bool isRedirect);

	//****************************************************  
	//网页生命周期回调接口  
	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> parentBrowser,
								const CefPopupFeatures& popupFeatures,
								CefWindowInfo& windowInfo,
								const CefString& url,
								CefRefPtr<CefClient>& client,
								CefBrowserSettings& settings);

	void SetCefWnd(DuiLib::CWindowWnd* pCefWnd);

	//菜单处理  
	//virtual bool OnBeforeMenu(CefRefPtr<CefBrowser> browser, const CefMenuInfo& menuInfo);
	virtual bool OnBeforeMenu(CefRefPtr<CefBrowser> browser,
		const CefMenuInfo& menuInfo) { return true; }
	virtual CefRefPtr<CefMenuHandler> GetMenuHandler();

	// 添加CEF的SP虚函数
	IMPLEMENT_REFCOUNTING(CWebClient);
	IMPLEMENT_LOCKING(CWebClient);

private:
	CallBackFun m_fun;
	DWORD m_dwDlg;			// 主窗口指针
};