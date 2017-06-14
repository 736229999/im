// MiniProgressBar.h : CMiniProgressBar 的声明
#pragma once
#include "resource.h"       // 主符号
#include <atlctl.h>

// IMiniProgressBar
[
	object,
	uuid(A987526D-19AF-4857-B12A-7A645F646597),
	dual,
	helpstring("IMiniProgressBar 接口"),
	pointer_default(unique)
]
__interface IMiniProgressBar : public IDispatch
{
	[id(1), helpstring("方法SetPos")] HRESULT SetPos(LONG nPos);
	[id(2), helpstring("方法SetTip")] HRESULT SetTip(BSTR strTip);
	[propget, id(3), helpstring("属性 OwnerData")] HRESULT OwnerData([out, retval] ULONG* pVal);
	[propput, id(3), helpstring("属性 OwnerData")] HRESULT OwnerData([in] ULONG newVal);
};


// CMiniProgressBar
[
	coclass,
	threading("apartment"),
	vi_progid("MiniAtxControl.MiniProgressBar"),
	progid("MiniAtxControl.MiniProgressBar.1"),
	version(1.0),
	uuid("F727C052-933D-4B31-9AC2-6DB4AA183171"),
	helpstring("MiniProgressBar Class"),
	support_error_info(IMiniProgressBar),
	registration_script("control.rgs")
]
class ATL_NO_VTABLE CMiniProgressBar : 
	public IMiniProgressBar,
	public IPersistStreamInitImpl<CMiniProgressBar>,
	public IOleControlImpl<CMiniProgressBar>,
	public IOleObjectImpl<CMiniProgressBar>,
	public IOleInPlaceActiveObjectImpl<CMiniProgressBar>,
	public IViewObjectExImpl<CMiniProgressBar>,
	public IOleInPlaceObjectWindowlessImpl<CMiniProgressBar>,
	public IPersistStorageImpl<CMiniProgressBar>,
	public ISpecifyPropertyPagesImpl<CMiniProgressBar>,
	public IQuickActivateImpl<CMiniProgressBar>,
	public IDataObjectImpl<CMiniProgressBar>,
	public IProvideClassInfo2Impl<&__uuidof(CMiniProgressBar), NULL>,
	public CComControl<CMiniProgressBar>
{
public:

	CMiniProgressBar()
	{
		memset(m_szTip,0,sizeof(m_szTip));
		m_nOwnerData = 0;
		m_nPos       = 0;
	}

DECLARE_OLEMISC_STATUS(OLEMISC_RECOMPOSEONRESIZE | 
	OLEMISC_CANTLINKINSIDE | 
	OLEMISC_INSIDEOUT | 
	OLEMISC_ACTIVATEWHENVISIBLE | 
	OLEMISC_SETCLIENTSITEFIRST
)


BEGIN_PROP_MAP(CMiniProgressBar)
	PROP_DATA_ENTRY("_cx", m_sizeExtent.cx, VT_UI4)
	PROP_DATA_ENTRY("_cy", m_sizeExtent.cy, VT_UI4)
	// 示例项
	// PROP_ENTRY("Property Description", dispid, clsid)
	// PROP_PAGE(CLSID_StockColorPage)
END_PROP_MAP()


BEGIN_MSG_MAP(CMiniProgressBar)
	CHAIN_MSG_MAP(CComControl<CMiniProgressBar>)
	DEFAULT_REFLECTION_HANDLER()
END_MSG_MAP()
// 处理程序原型: 
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

// IViewObjectEx
	DECLARE_VIEW_STATUS(VIEWSTATUS_SOLIDBKGND | VIEWSTATUS_OPAQUE)

// IMiniProgressBar
public:
		HRESULT OnDraw(ATL_DRAWINFO& di)
		{
			RECT& rc  = *(RECT*)di.prcBounds;
			HDC & hdc = di.hdcDraw;

			int nSavedDC = SaveDC(hdc);

			HPEN penBorder   = ::CreatePen(PS_SOLID,1,RGB(0,0,0));
			HBRUSH bsBack    = ::CreateSolidBrush(RGB(255,255,255));
			HBRUSH bsPos     = ::CreateSolidBrush(RGB(0,233,0));
			HBRUSH bsRemain  = ::CreateSolidBrush(RGB(255,233,244));

			HFONT hFont      = ::CreateFont(12, 
				0,
				0, 
				90,
				FW_NORMAL,
				0,
				0,
				0,
				DEFAULT_CHARSET,
				OUT_TT_ONLY_PRECIS,
				CLIP_DEFAULT_PRECIS,
				DRAFT_QUALITY,
				(DEFAULT_PITCH|FF_MODERN),
				TEXT("宋体"));

			SelectObject(hdc,hFont);

			TEXTMETRIC tm;
			GetTextMetrics(hdc,&tm);

			RECT rcBar   = rc;
			rcBar.top    = rc.top + tm.tmHeight+8;
			rcBar.bottom = rcBar.top+12;
			rcBar.left   = rc.left+5;
			rcBar.right  = rc.right-5;

			FillRect(hdc,&rc,bsBack);

			SetTextColor(hdc,RGB(155,122,88));
			SetTextAlign(hdc,TA_LEFT|TA_TOP);
			TextOut(hdc,rc.left + 6, rc.top+2, (LPCTSTR)m_szTip, wcslen(m_szTip) );

			FillRect(hdc,&rcBar,bsRemain);

			RECT rcPos  = rcBar;
			rcPos.right = rcPos.left + ( rcBar.right-rcBar.left )*(float)m_nPos/100.0;

			FillRect(hdc,&rcPos,bsPos);

			SelectObject(hdc,(HGDIOBJ)GetStockObject(NULL_BRUSH));
			SelectObject(hdc,penBorder);

			Rectangle(hdc,rcBar.left-1,rcBar.top-1,rcBar.right+1,rcBar.bottom+1);

			RestoreDC(hdc,nSavedDC);

			DeleteObject(penBorder);
			DeleteObject(bsBack);
			DeleteObject(bsPos);
			DeleteObject(bsRemain);
			DeleteObject(hFont);

			return S_OK;
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		SIZE sz;
		sz.cy=40*26.4;
		sz.cx=220*26.4;

		SetExtent(DVASPECT_CONTENT,&sz);
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}
	STDMETHOD(SetPos)(LONG nPos);
	STDMETHOD(SetTip)(BSTR strTip);
	STDMETHOD(get_OwnerData)(ULONG* pVal);
	STDMETHOD(put_OwnerData)(ULONG newVal);

protected:
	ULONG		m_nOwnerData;
	SHORT		m_nPos;
	TCHAR       m_szTip[255];

};

