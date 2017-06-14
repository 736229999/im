// WriteArticleX.h : CWriteArticleX µÄÉùÃ÷

#pragma once
#include "resource.h"       // Ö÷·ûºÅ

#include "WriteArticle.h"


// CWriteArticleX

class ATL_NO_VTABLE CWriteArticleX : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWriteArticleX, &CLSID_WriteArticleX>,
	public IDispatchImpl<IWriteArticleX, &IID_IWriteArticleX, &LIBID_WriteArticleLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CWriteArticleX()
	{
		g_pWebConfig = NULL;
		g_pUserInfo  = NULL;
	}

DECLARE_REGISTRY_RESOURCEID(IDR_WRITEARTICLEX)


BEGIN_COM_MAP(CWriteArticleX)
	COM_INTERFACE_ENTRY(IWriteArticleX)
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
	STDMETHOD(Show)();
	STDMETHOD(Close)(void);
	STDMETHOD(get_UserInfo)(LONG* pVal);
	STDMETHOD(SelectMyArticlesX)(LONG pMyArticlesX);

private:
	ULONG_PTR	 m_gdiplusToken;

public:
	STDMETHOD(NewArticle)(void);
	STDMETHOD(EditLocalSavedArticle)(LONG pLocalSaveArticleInfo, LONG nSavedID);
	STDMETHOD(EditWebArticle)(LONG pWebArticleInfo);
};

OBJECT_ENTRY_AUTO(__uuidof(WriteArticleX), CWriteArticleX)
