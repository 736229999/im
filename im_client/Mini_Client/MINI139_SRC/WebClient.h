#pragma once
#include <cef_client.h>
#include "..\..\DuiLib\UIlib.h"
#include "CefWebUI.h"
typedef BOOL (*CallBackFun)(DWORD dwDlg, char *p); // Ϊ�ص�������������������Ϊ CallBackFun������Ϊchar *p
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
	//��ҳ�������ڻص��ӿ�  
	virtual bool OnBeforePopup(CefRefPtr<CefBrowser> parentBrowser,
								const CefPopupFeatures& popupFeatures,
								CefWindowInfo& windowInfo,
								const CefString& url,
								CefRefPtr<CefClient>& client,
								CefBrowserSettings& settings);

	void SetCefWnd(DuiLib::CWindowWnd* pCefWnd);

	//�˵�����  
	//virtual bool OnBeforeMenu(CefRefPtr<CefBrowser> browser, const CefMenuInfo& menuInfo);
	virtual bool OnBeforeMenu(CefRefPtr<CefBrowser> browser,
		const CefMenuInfo& menuInfo) { return true; }
	virtual CefRefPtr<CefMenuHandler> GetMenuHandler();

	// ���CEF��SP�麯��
	IMPLEMENT_REFCOUNTING(CWebClient);
	IMPLEMENT_LOCKING(CWebClient);

private:
	CallBackFun m_fun;
	DWORD m_dwDlg;			// ������ָ��
};