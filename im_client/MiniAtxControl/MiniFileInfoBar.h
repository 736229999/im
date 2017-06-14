// MiniFileInfoBar.h : CMiniFileInfoBar 的声明
#pragma once
#include "resource.h"       // 主符号
#include <atlctl.h>

// IMiniFileInfoBar
[
	object,
	uuid(13109A5E-20CD-4517-823F-C372DA423455),
	dual,
	helpstring("IMiniFileInfoBar 接口"),
	pointer_default(unique)
]
__interface IMiniFileInfoBar : public IDispatch
{
	[id(1), helpstring("方法SetFileInfo")] HRESULT SetFileInfo([in] BSTR strFileName, [in] LONG nFileSize);
	[propget, id(2), helpstring("属性 OwnerData")] HRESULT OwnerData([out, retval] ULONG* pVal);
	[propput, id(2), helpstring("属性 OwnerData")] HRESULT OwnerData([in] ULONG newVal);
};


// CMiniFileInfoBar
[
	coclass,
	threading("apartment"),
	vi_progid("MiniAtxControl.MiniFileInfoBar"),
	progid("MiniAtxControl.MiniFileInfoBar.1"),
	version(1.0),
	uuid("A9DE5645-9879-4779-978F-1B01617020B9"),
	helpstring("MiniFileInfoBar Class"),
	support_error_info(IMiniFileInfoBar),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CMiniFileInfoBar : 
	public IMiniFileInfoBar,
	public IPersistStreamInitImpl<CMiniFileInfoBar>,
	public IOleControlImpl<CMiniFileInfoBar>,
	public IOleObjectImpl<CMiniFileInfoBar>,
	public IOleInPlaceActiveObjectImpl<CMiniFileInfoBar>,
	public IViewObjectExImpl<CMiniFileInfoBar>,
	public IOleInPlaceObjectWindowlessImpl<CMiniFileInfoBar>,
	public IPersistStorageImpl<CMiniFileInfoBar>,
	public ISpecifyPropertyPagesImpl<CMiniFileInfoBar>,
	public IQuickActivateImpl<CMiniFileInfoBar>,
	public IDataObjectImpl<CMiniFileInfoBar>,
	public IProvideClassInfo2Impl<&__uuidof(CMiniFileInfoBar), NULL>,
	public CComControl<CMiniFileInfoBar>
{
public:

	CMiniFileInfoBar()
	{
		m_hSysFont = CreateFont(
			12,                        // nHeight
			0,                         // nWidth
			0,                         // nEscapement
			0,                         // nOrientation
			FW_NORMAL,                  // nWeight
			FALSE,                     // bItalic
			FALSE,                     // bUnderline
			0,                         // cStrikeOut
			DEFAULT_CHARSET,           // nCharSet
			OUT_DEFAULT_PRECIS,        // nOutPrecision
			CLIP_DEFAULT_PRECIS,       // nClipPrecision
			DEFAULT_QUALITY,           // nQuality
			DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
			TEXT("宋体"));

		memset(m_szInfo,0,sizeof(m_szInfo));
		m_hBitmap = NULL;
	}


	~CMiniFileInfoBar()
	{
		if ( m_hBitmap )
		{
			DeleteObject(m_hBitmap);
			m_hBitmap=NULL;
		}
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CMiniFileInfoBar)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// 示例项
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CMiniFileInfoBar)
	CHAIN_MSG_MAP(CComControl<CMiniFileInfoBar>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// 处理程序原型: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IMiniFileInfoBar
public:
		HRESULT OnDraw(ATL_DRAWINFO& di)
		{
			RECT& rc  = *(RECT*)di.prcBounds;
			HDC & hdc = di.hdcDraw;

			int nSavedDC = SaveDC(hdc);

			HBRUSH bsBack    = ::CreateSolidBrush(RGB(255,255,255));

			SelectObject(hdc,m_hSysFont);

			SIZE szText;
			GetTextExtentPoint(hdc,m_szInfo,wcslen(m_szInfo),&szText);

			FillRect(hdc,&rc,bsBack);

			//draw file icon
			HDC hdcTemp =
				CreateCompatibleDC(hdc);

			int left = rc.left+5;

			{
				HGDIOBJ hOldbitmap = SelectObject(hdcTemp,m_hBitmap);
				BitBlt(hdc,left,rc.top+3,32,32,hdcTemp,0,0,SRCCOPY);
				SelectObject(hdcTemp,hOldbitmap);
				DeleteDC(hdcTemp);
			}


			//draw text
			SetTextColor(hdc,RGB(155,122,88));
			SetTextAlign(hdc,TA_LEFT|TA_TOP);
			TextOut(hdc,left, rc.bottom-szText.cy-3, m_szInfo, wcslen(m_szInfo) );

			RestoreDC(hdc,nSavedDC);

			DeleteObject(bsBack);

			return S_OK;
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		SIZE sz;
		sz.cy=40*26.4;
		sz.cx=40*26.4;

		SetExtent(DVASPECT_CONTENT,&sz);
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}
	STDMETHOD(SetFileInfo)(BSTR strFileName, LONG nFileSize);
	STDMETHOD(get_OwnerData)(ULONG* pVal);
	STDMETHOD(put_OwnerData)(ULONG newVal);

protected:
	ULONG		m_nOwnerData	;
	TCHAR       m_szInfo[255]	;
	HFONT       m_hSysFont		;
	HBITMAP     m_hBitmap		;
};

