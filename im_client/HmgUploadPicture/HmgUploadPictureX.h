// HmgUploadPictureX.h : CHmgUploadPictureX 的声明

#pragma once
#include "resource.h"       // 主符号

#include "HmgUploadPicture.h"
#include "UploadPictureX\uploadphotodlg.h"

extern CUpLoadPhotoDlg *g_dlg ;

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif



// CHmgUploadPictureX

class ATL_NO_VTABLE CHmgUploadPictureX :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CHmgUploadPictureX, &CLSID_HmgUploadPictureX>,
	public IDispatchImpl<IHmgUploadPictureX, &IID_IHmgUploadPictureX, &LIBID_HmgUploadPictureLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CHmgUploadPictureX()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_HMGUPLOADPICTUREX)


BEGIN_COM_MAP(CHmgUploadPictureX)
	COM_INTERFACE_ENTRY(IHmgUploadPictureX)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:

public:
	STDMETHOD(QueryVersion)(BSTR strVersion, BSTR* retVal);
	STDMETHOD(UploadPicShellExt)(BSTR strParamInfo);
	STDMETHOD(BringToTop)(void);
	STDMETHOD(ShowUploadDlg)(BSTR strParam);
};

OBJECT_ENTRY_AUTO(__uuidof(HmgUploadPictureX), CHmgUploadPictureX)
