#include "StdAfx.h"
#include "WebClient.h"
#include "im\impublic.h"

void CWebClient::OnAfterCreated(CefRefPtr<CefBrowser> browser)
{
	m_Browser = browser;
}

bool CWebClient::OnBeforeBrowse(CefRefPtr<CefBrowser> browser,
								CefRefPtr<CefFrame> frame,
								CefRefPtr<CefRequest> request,
								NavType navType,
								bool isRedirect) 
{ 
	CefString strUrl = request->GetURL();
	string strUrlNew = strUrl.ToString();
	const char* pUrl = strUrlNew.c_str();
	if(::SendMessage(m_pCefWnd->GetHWND(), WM_IS_NAVEGATE, (DWORD)pUrl, NULL))
	{
		return true;
	}
	return false; 
}

void CWebClient::SetCefWnd(DuiLib::CWindowWnd* pCefWnd)
{
	m_pCefWnd = pCefWnd;
}
void CWebClient::SetCallbackFun(CallBackFun p, DWORD dwDlg)
{
	m_dwDlg = dwDlg;
	m_fun = p;
}

//****************************************************  
//网页生命周期回调接口  
bool CWebClient::OnBeforePopup(CefRefPtr<CefBrowser> parentBrowser,
							const CefPopupFeatures& popupFeatures,
							CefWindowInfo& windowInfo,
							const CefString& url,
							CefRefPtr<CefClient>& client,
							CefBrowserSettings& settings) 
{	// 禁止使用cef弹出新的浏览器窗口
	CString strUrl = url.ToString().c_str();
	if(strUrl.Find(RIGHT_URL_SELLER) == -1 && strUrl.Find(RIGHT_URL) == -1 && strUrl.Find("http://im.feiniu.com") == -1)
	{
		OpenHtmlPage(strUrl);
	}
	return true;	
}	

//bool CWebClient::OnBeforeMenu(CefRefPtr<CefBrowser> browser, const CefMenuInfo& menuInfo)
//{
//	//menuInfo.Clear(CefMenuInfoTraits);
//	return true;
//}

CefRefPtr<CefMenuHandler> CWebClient::GetMenuHandler()
{
	::SendMessage(m_pCefWnd->GetHWND(), WM_RBUTTON, NULL, NULL);
	return this; 
}