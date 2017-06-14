// OleImage.h : COleImage ������
#pragma once
#include "resource.h"       // ������
#include <atlctl.h>
#include "Mini139_src_i.h"
#include "_IOleImageEvents_CP.h"
#include "..\..\..\DuiLib\UIlib.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif

using namespace ATL;



// COleImage
class ATL_NO_VTABLE COleImage :
	public CComObjectRootEx<CComSingleThreadModel>,
	public IDispatchImpl<IOleImage, &IID_IOleImage, &LIBID_Mini139_srcLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IOleControlImpl<COleImage>,
	public IOleObjectImpl<COleImage>,
	public IOleInPlaceActiveObjectImpl<COleImage>,
	public IViewObjectExImpl<COleImage>,
	public IOleInPlaceObjectWindowlessImpl<COleImage>,
	public ISupportErrorInfo,
	public IConnectionPointContainerImpl<COleImage>,
	public CProxy_IOleImageEvents<COleImage>,
	public IQuickActivateImpl<COleImage>,
#ifndef _WIN32_WCE
	public IDataObjectImpl<COleImage>,
#endif
	public IProvideClassInfo2Impl<&CLSID_OleImage, &__uuidof(_IOleImageEvents), &LIBID_Mini139_srcLib>,
	public CComCoClass<COleImage, &CLSID_OleImage>,
	public CComControl<COleImage>
{
public:


	COleImage()
	{
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE |
	OLEMISC_CANTLINKINSIDE |
	OLEMISC_INSIDEOUT |
	OLEMISC_ACTIVATEWHENVISIBLE |
	OLEMISC_SETCLIENTSITEFIRST
)

DECLARE_REGISTRY_RESOURCEID(IDR_OLEIMAGE)


BEGIN_COM_MAP(COleImage)
	COM_INTERFACE_ENTRY(IOleImage)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IViewObjectEx)
	COM_INTERFACE_ENTRY(IViewObject2)
	COM_INTERFACE_ENTRY(IViewObject)
	COM_INTERFACE_ENTRY(IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceObject)
	COM_INTERFACE_ENTRY2(IOleWindow, IOleInPlaceObjectWindowless)
	COM_INTERFACE_ENTRY(IOleInPlaceActiveObject)
	COM_INTERFACE_ENTRY(IOleControl)
	COM_INTERFACE_ENTRY(IOleObject)
	COM_INTERFACE_ENTRY(ISupportErrorInfo)
	COM_INTERFACE_ENTRY(IConnectionPointContainer)
	COM_INTERFACE_ENTRY(IQuickActivate)
#ifndef _WIN32_WCE
	COM_INTERFACE_ENTRY(IDataObject)
#endif
	COM_INTERFACE_ENTRY(IProvideClassInfo)
	COM_INTERFACE_ENTRY(IProvideClassInfo2)
END_COM_MAP()

BEGIN_PROP_MAP(COleImage)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// ʾ����
	// PROP_ENTRY_TYPE("������", dispid, clsid, vtType)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()

BEGIN_CONNECTION_POINT_MAP(COleImage)
	CONNECTION_POINT_ENTRY(__uuidof(_IOleImageEvents))
END_CONNECTION_POINT_MAP()

BEGIN_MSG_MAP(COleImage)
	CHAIN_MSG_MAP(CComControl<COleImage>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// �������ԭ��:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// ISupportsErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		static const IID* const arr[] =
		{
			&IID_IOleImage,
		};

		for (int i=0; i<sizeof(arr)/sizeof(arr[0]); i++)
		{
			if (InlineIsEqualGUID(*arr[i], riid))
				return S_OK;
		}
		return S_FALSE;
	}

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// ��С��֡��ʱ
#define MIN_FRM_DELAY				100

private:
	CDC *m_pDCs; // ÿ֡��DC
	CBitmap *m_pBitmaps; // ÿһ֡��λͼ
	CBitmap **m_ppOldBmps; // ����ÿһ֡DC�ɵ�λͼ
	int *m_pFrmIndexes; // ÿ��100�����Ӧ��֡����
	int m_nFrmCount; // ��֡��
	int m_nTimeCount; // 100���������������GIF����֡�������ܵ��ӳ���1�룬�����ֵΪ10
	volatile int m_iPrevFrm; // ǰһ֡����ʱ������ʱ�����ж��Ƿ���Ҫ�Ļ�֡
	volatile int m_iCurTime; // ��ǰʱ�䣨���ڼ���100���룩
	CRichEditCtrl *m_pContainer; // ��������ָ��
	DuiLib::CRichEditUI* m_pContainerDui;
	static DWORD m_dwOleObjectOffset; // IOleObject�ӿ�ָ�����COleImage��ʼ��ַ��ƫ����
// IOleImage

public:
	CString m_strImageName;
	long m_lOwnData;
	

	/*
	HRESULT OnDraw(ATL_DRAWINFO& di)
	{
		RECT& rc = *(RECT*)di.prcBounds;
		// ��������������Ϊ di.prcBounds ָ���ľ���
		HRGN hRgnOld = NULL;
		if (GetClipRgn(di.hdcDraw, hRgnOld) != 1)
			hRgnOld = NULL;
		bool bSelectOldRgn = false;

		HRGN hRgnNew = CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);

		if (hRgnNew != NULL)
		{
			bSelectOldRgn = (SelectClipRgn(di.hdcDraw, hRgnNew) != ERROR);
		}

		Rectangle(di.hdcDraw, rc.left, rc.top, rc.right, rc.bottom);
		SetTextAlign(di.hdcDraw, TA_CENTER|TA_BASELINE);
		LPCTSTR pszText = _T("OleImage");
#ifndef _WIN32_WCE
		TextOut(di.hdcDraw,
			(rc.left + rc.right) / 2,
			(rc.top + rc.bottom) / 2,
			pszText,
			lstrlen(pszText));
#else
		ExtTextOut(di.hdcDraw,
			(rc.left + rc.right) / 2,
			(rc.top + rc.bottom) / 2,
			ETO_OPAQUE,
			NULL,
			pszText,
			ATL::lstrlen(pszText),
			NULL);
#endif

		if (bSelectOldRgn)
			SelectClipRgn(di.hdcDraw, hRgnOld);

		return S_OK;
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}
	*/

	
	// ʵ������һ���ᱻ���õĺ���
	HRESULT FinalConstruct();

	// ɾ��ʵ��ǰһ���ᱻ���õĺ���
	void FinalRelease();

	// ����IOleObject�ӿ�ָ����COleImage���е�ƫ����
	static DWORD GetIOleObjectOffset(void);

	// ����IOleObjec�ӿ�ָ��õ�COleImage��ָ�룬��ʵ���Ǽ�ȥǰ���ƫ����
	static COleImage *FromOleObject(IOleObject *lpOleObject);

	// ���ļ����أ���Ҳ���������һ����IStream���صĺ���������ֻ����ʾ����
	HRESULT LoadFromFile(BSTR lpszPathName, CRichEditCtrl *pContainer, UINT nMaxWidth = 0);

	HRESULT LoadFromFile(BSTR lpszPathName, DuiLib::CRichEditUI *pContainer, UINT nMaxWidth);

	// ���ƺ���
	HRESULT OnDraw(ATL_DRAWINFO& di);

	// ����֡
	void ChangeFrame();

	// �������һ��Ҫ��д�����Բ����κ����飬�����д��˫�������ͼ��ʱ��ASSERTʧ��
	STDMETHOD(DoVerb)(
		_In_ LONG /* iVerb */, 
		_In_opt_ LPMSG /* pMsg */, 
		_Inout_ IOleClientSite* /* pActiveSite */, 
		_In_ LONG /* lindex */,
		_In_ HWND /* hwndParent */, 
		_In_ LPCRECT /* lprcPosRect */)
	{
		return S_OK;
	}
};

OBJECT_ENTRY_AUTO(__uuidof(OleImage), COleImage)
