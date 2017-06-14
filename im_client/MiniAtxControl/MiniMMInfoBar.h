// MiniMMInfoBar.h : CMiniMMInfoBar 的声明
#pragma once
#include "resource.h"       // 主符号
#include <atlctl.h>

//extern CComModule _Module;

// IMiniMMInfoBar
[
	object,
	uuid(2C3B0C2F-46D8-492E-A8B8-4347A7B857F7),
	dual,
	helpstring("IMiniMMInfoBar 接口"),
	pointer_default(unique)
]
__interface IMiniMMInfoBar : public IDispatch
{
	[propget, id(1), helpstring("属性 OwnerData")] HRESULT OwnerData([out, retval] LONG* pVal);
	[propput, id(1), helpstring("属性 OwnerData")] HRESULT OwnerData([in] LONG newVal);
	[propget, id(2), helpstring("属性 MMType")] HRESULT MMType([out, retval] BSTR* pVal);
	[propput, id(2), helpstring("属性 MMType")] HRESULT MMType([in] BSTR newVal);
};


// CMiniMMInfoBar
[
	coclass,
	threading("apartment"),
	vi_progid("MiniAtxControl.MiniMMInfoBar"),
	progid("MiniAtxControl.MiniMMInfoBar.1"),
	version(1.0),
	uuid("BD1D401A-73F3-473E-AE1F-03B142021BD7"),
	helpstring("MiniMMInfoBar Class"),
	support_error_info(IMiniMMInfoBar),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CMiniMMInfoBar : 
	public IMiniMMInfoBar,
	public IPersistStreamInitImpl<CMiniMMInfoBar>,
	public IOleControlImpl<CMiniMMInfoBar>,
	public IOleObjectImpl<CMiniMMInfoBar>,
	public IOleInPlaceActiveObjectImpl<CMiniMMInfoBar>,
	public IViewObjectExImpl<CMiniMMInfoBar>,
	public IOleInPlaceObjectWindowlessImpl<CMiniMMInfoBar>,
	public IPersistStorageImpl<CMiniMMInfoBar>,
	public ISpecifyPropertyPagesImpl<CMiniMMInfoBar>,
	public IQuickActivateImpl<CMiniMMInfoBar>,
	public IDataObjectImpl<CMiniMMInfoBar>,
	public IProvideClassInfo2Impl<&__uuidof(CMiniMMInfoBar), NULL>,
	public CComControl<CMiniMMInfoBar>
{
public:

	CMiniMMInfoBar()
	{
		m_strType    = L"";
		m_nOwnerData = 0;
		m_bAudio     = true;
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CMiniMMInfoBar)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// 示例项
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CMiniMMInfoBar)
	CHAIN_MSG_MAP(CComControl<CMiniMMInfoBar>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// 处理程序原型: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IMiniMMInfoBar
public:
		HRESULT OnDraw(ATL_DRAWINFO& di)
		{
			RECT& rc  = *(RECT*)di.prcBounds;
			HDC & hdc = di.hdcDraw;

			int nSavedDC = SaveDC(hdc);

			HBRUSH hbrWhite = CreateSolidBrush(0xFFFFFF);

			::FillRect(hdc,&rc,hbrWhite);

			HDC hdcMem      = ::CreateCompatibleDC(hdc);
			HBITMAP hbmpMem = NULL;
			
			if ( m_bAudio )
				hbmpMem = ::LoadBitmap( _AtlBaseModule.GetModuleInstance(),MAKEINTRESOURCE(IDB_AUDIO));
			else
				hbmpMem = ::LoadBitmap( _AtlBaseModule.GetModuleInstance(),MAKEINTRESOURCE(IDB_VEDIO));

			
			HBITMAP hOldbmp = (HBITMAP)::SelectObject(hdcMem,hbmpMem);

			int cx = rc.right - rc.left;
			int cy = rc.bottom - rc.top;

			int nLeft = rc.left+(cx-48)/2;
			int nTop  = rc.top+(cy-48)/2;

			BitBlt(hdc,nLeft,nTop,48,48,hdcMem,0,0,SRCCOPY);

			SelectObject(hdcMem,hOldbmp);
		
			RestoreDC(hdc,nSavedDC);
			DeleteObject(hbrWhite);

			return S_OK;
		}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		SIZE sz;
		sz.cy=48*26.4;
		sz.cx=48*26.4;

		SetExtent(DVASPECT_CONTENT,&sz);
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}
	STDMETHOD(get_OwnerData)(LONG* pVal);
	STDMETHOD(put_OwnerData)(LONG newVal);
	STDMETHOD(get_MMType)(BSTR* pVal);
	STDMETHOD(put_MMType)(BSTR newVal);

protected:
	LONG	  m_nOwnerData;
	CComBSTR  m_strType;
	bool      m_bAudio;

};

