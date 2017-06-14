#pragma once
#include "GdiPlus.h"
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")
// OleLinkInfoCtrl.h : COleLinkInfoCtrl ActiveX �ؼ����������


// COleLinkInfoCtrl : �й�ʵ�ֵ���Ϣ������� OleLinkInfoCtrl.cpp��
class COleLinkInfoCtrl : public COleControl
{
	DECLARE_DYNCREATE(COleLinkInfoCtrl)

// ���캯��
public:
	COleLinkInfoCtrl();

// ��д
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

// ʵ��
protected:
	~COleLinkInfoCtrl();

	DECLARE_OLECREATE_EX(COleLinkInfoCtrl)    // �๤���� guid
	DECLARE_OLETYPELIB(COleLinkInfoCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(COleLinkInfoCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(COleLinkInfoCtrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
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

