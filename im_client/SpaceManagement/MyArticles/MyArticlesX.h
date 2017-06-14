// MyArticlesX.h : CMyArticlesX µÄÉùÃ÷

#pragma once
#include "resource.h"       // Ö÷·ûºÅ

#include "MyArticles.h"

// CMyArticlesX

class ATL_NO_VTABLE CMyArticlesX : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMyArticlesX, &CLSID_MyArticlesX>,
	public IDispatchImpl<IMyArticlesX, &IID_IMyArticlesX, &LIBID_MyArticlesLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CMyArticlesX()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MYARTICLESX)


BEGIN_COM_MAP(CMyArticlesX)
	COM_INTERFACE_ENTRY(IMyArticlesX)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		//init gdi plus
		GdiplusStartupInput gdiplusStartupInput; 
		GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL); 

		InitResourceDLL(GetModulePath()+"Mini139_res.dll");
		InitHmgSkinControls(GetModulePath()+"HmgSkinRes.dll");

		g_spaceData.SetPath(GetModulePath());

		HRESULT hr = CoCreateInstance(CLSID_CUserInfoX,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IUserInfoX,
			(void **)&g_pUserInfo);//CreateInstance(CLSID_CUserInfoX);

		if ( FAILED(hr) )
		{
			g_pUserInfo = NULL;
		}

		g_pWebConfig.CreateInstance(CLSID_WebConfigX);

		return S_OK;
	}
	
	void FinalRelease() 
	{
		UnInitResourceDLL();
		UnInitHmgSkinControls();

		if ( g_pUserInfo!=NULL )
		{
			g_pUserInfo->Release();
			g_pUserInfo = NULL;
		}

		if ( g_pWebConfig!=NULL )
		{
			g_pWebConfig.Release();
			g_pWebConfig = NULL;
		}

		GdiplusShutdown(m_gdiplusToken);
	}

public:

	STDMETHOD(Show)(void);
	STDMETHOD(Close)(void);
	STDMETHOD(get_UserInfo)(LONG * pVal);

private:
	ULONG_PTR	     m_gdiplusToken;
public:
	STDMETHOD(SelectWriteArticleX)(LONG pWriteArticleX);
	STDMETHOD(OnUploadLocalArticle)(LONG nSavedID);
	STDMETHOD(UpdateWebArticleList)(LONG nArticleID);
	STDMETHOD(UpdateLocalArticleList)(LONG nSavedID, BSTR strArticleTitle);
	STDMETHOD(SetMessageWndHandle)(LONG hWnd);
};

OBJECT_ENTRY_AUTO(__uuidof(MyArticlesX), CMyArticlesX)
