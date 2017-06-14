// IMiniPictureBroadCastX.h : CIMiniPictureBroadCastX µÄÉùÃ÷

#pragma once
#include "resource.h"       // Ö÷·ûºÅ

#include "PictureLoadX.h"


// CIMiniPictureBroadCastX

class ATL_NO_VTABLE CIMiniPictureBroadCastX : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CIMiniPictureBroadCastX, &CLSID_IMiniPictureBroadCastX>,
	public IDispatchImpl<IIMiniPictureBroadCastX, &IID_IIMiniPictureBroadCastX, &LIBID_PictureLoadXLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CIMiniPictureBroadCastX()
	{
		g_nUserID = 0;
		g_strNickName = "";
		g_strUserName = "";
	}

DECLARE_REGISTRY_RESOURCEID(IDR_IMINIPICTUREBROADCASTX)


BEGIN_COM_MAP(CIMiniPictureBroadCastX)
	COM_INTERFACE_ENTRY(IIMiniPictureBroadCastX)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		GdiplusStartupInput gdiplusStartupInput; 
		GdiplusStartup(&m_gdiplusToken, &gdiplusStartupInput, NULL); 

		InitResourceDLL(GetModulePath()+"Mini139_res.dll");
		
		g_spaceData.SetPath(GetModulePath());

		return S_OK;
	}
	
	void FinalRelease() 
	{
		UnInitResourceDLL();	
		GdiplusShutdown(m_gdiplusToken);
	}

public:

	STDMETHOD(Show)(BSTR strCachePath);
	STDMETHOD(Close)(void);
	STDMETHOD(SetUserInfo)(LONG nUserID, BSTR strUserName, BSTR strNickName);
	STDMETHOD(AddUser)(long nUserID);
	STDMETHOD(SetWindowTop)(BOOL bWndTop);
	STDMETHOD(SetInterval)(int nInterval);
	STDMETHOD(SetMessageWndHandle)(LONG hWnd);
	STDMETHOD(EmptyPictures)();
protected:
	ULONG_PTR   m_gdiplusToken;

};

OBJECT_ENTRY_AUTO(__uuidof(IMiniPictureBroadCastX), CIMiniPictureBroadCastX)
