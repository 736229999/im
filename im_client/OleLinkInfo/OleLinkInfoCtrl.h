#pragma once
#include "GdiPlus.h"
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")
// OleLinkInfoCtrl.h : COleLinkInfoCtrl ActiveX 控件类的声明。


// COleLinkInfoCtrl : 有关实现的信息，请参阅 OleLinkInfoCtrl.cpp。
class COleLinkInfoCtrl : public COleControl
{
	DECLARE_DYNCREATE(COleLinkInfoCtrl)

// 构造函数
public:
	COleLinkInfoCtrl();

// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
	virtual BOOL IsInvokeAllowed(DISPID dispid);
private:
	CRect m_rcPic;
	CRect m_rcHead;
	CRect m_rcContent1;
	CRect m_rcContent2;
	CString m_strPicFilePath;
	CString m_strHead;
	CString m_strContent;
	CString m_strUrl;

	ULONG_PTR m_pGdiToken;

// 实现
protected:
	~COleLinkInfoCtrl();

	DECLARE_OLECREATE_EX(COleLinkInfoCtrl)    // 类工厂和 guid
	DECLARE_OLETYPELIB(COleLinkInfoCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(COleLinkInfoCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(COleLinkInfoCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
		dispidGetUrlText = 6L,
		dispidSetUrlText = 5L,
		dispidIsPointInLinkRect = 4L,
		dispidSetContentText = 3L,
		dispidSetHeadText = 2L,
		dispidSetPicFilePath = 1L
	};
protected:
	void SetPicFilePath(LPCTSTR strFilePath);
	void SetHeadText(LPCTSTR strHeadText);
	void SetContentText(LPCTSTR strContentText);
	LONG IsPointInLinkRect(LONG olex, LONG oley, LONG x, LONG y);
	void SetUrlText(LPCTSTR strUrlText);
	BSTR GetUrlText(void);
};

