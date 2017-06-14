// MyPicturesX.h : CMyPicturesX µÄÉùÃ÷

#pragma once
#include "resource.h"       // Ö÷·ûºÅ

#include "MyPictures.h"


// CMyPicturesX

class ATL_NO_VTABLE CMyPicturesX : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMyPicturesX, &CLSID_MyPicturesX>,
	public IDispatchImpl<IMyPicturesX, &IID_IMyPicturesX, &LIBID_MyPicturesLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CMyPicturesX()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_MYPICTURESX)


BEGIN_COM_MAP(CMyPicturesX)
	COM_INTERFACE_ENTRY(IMyPicturesX)
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

private:
	ULONG_PTR m_gdiplusToken;

public:
	STDMETHOD(Show)(void);
	STDMETHOD(Close)(void);
	STDMETHOD(get_UserInfo)(LONG* pVal);
	STDMETHOD(SelectHeadPhotoX)(LONG pHeadPhotoX);
	STDMETHOD(SetMessageWndHandle)(LONG hWnd);
};

OBJECT_ENTRY_AUTO(__uuidof(MyPicturesX), CMyPicturesX)
