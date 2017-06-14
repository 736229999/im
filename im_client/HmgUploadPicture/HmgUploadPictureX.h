// HmgUploadPictureX.h : CHmgUploadPictureX ������

#pragma once
#include "resource.h"       // ������

#include "HmgUploadPicture.h"
#include "UploadPictureX\uploadphotodlg.h"

extern CUpLoadPhotoDlg *g_dlg ;

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
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
