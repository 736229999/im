#pragma once
#include "XSkinDlg.h"
#include "../im/RichEditCtrlEx.h"
#include "../im/CustomToolBar.h"
#include "../im/impublic.h"
#include "../im/LocalData.h"
//
#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"


class CEditAutoReplyDlg : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UIAutoReply");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("autoreplyAdd.xml");  }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	virtual void Notify(DuiLib::TNotifyUI& msg);
	virtual void InitWindow();
	CEditAutoReplyDlg(CString strText, const XT_FONT &font, CWnd* pParent = NULL);   // standard constructor

	// 获取文本
	CString GetText();
	
	// 获取字体
	XT_FONT GetFont();

	// 设置标题
	void SetTitle(const char *pTitle);

	void OnOK();
	void UpdateBk();
	
public:
	CP2PTalkToolBar m_pfontOnline;
	CRichEditCtrlEx m_reTextOnline;
	AUTO_REPLY m_AutoReply;
private:
	CWndShadow m_WndShadow;
	XT_FONT m_font;
	CString m_strText;
	CString m_strTitle;	
	DuiLib::CLabelUI *m_pLabelTitle;
	virtual DuiLib::CControlUI* CreateControl(LPCTSTR pstrClassName)
	{
		if (_tcsicmp(pstrClassName, _T("RichEditSendWnd")) == 0)
		{
			CWndUI  *pUI  = new CWndUI;      
			RECT rc;
			rc.left = 0;
			rc.right = 0;
			rc.top = 0;
			rc.bottom = 0;
			m_reTextOnline.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | ES_WANTRETURN
				/*| WS_HSCROLL | ES_AUTOHSCROLL*/  // 自动水平滚动
				| WS_VSCROLL | ES_AUTOVSCROLL, rc, CWnd::FromHandle(GetHWND()), 10002);
			pUI->Attach(m_reTextOnline.GetSafeHwnd());          
			return pUI;
		}

		if (_tcsicmp(pstrClassName, _T("TalkBarWnd")) == 0)
		{
			CWndUI  *pUI  = new CWndUI;      
			RECT rc;
			rc.left = 0;
			rc.right = 0;
			rc.top = 0;
			rc.bottom = 0;
				
			m_pfontOnline.SetFont(&(m_AutoReply.font));////设置保存的字体参数
			m_pfontOnline.Create(CP2PTalkToolBar::IDD, CWnd::FromHandle(GetHWND()));
			m_pfontOnline.MoveWindow(&rc);
			pUI->Attach(m_pfontOnline.GetSafeHwnd());     
			return pUI;
		}
	}
};

//class CEditAutoReplyDlg : public CXSkinDlg
//{
//	DECLARE_DYNAMIC(CEditAutoReplyDlg)
//
//public:
//	CEditAutoReplyDlg(CString strText, const XT_FONT &font, CWnd* pParent = NULL);   // standard constructor
//	virtual ~CEditAutoReplyDlg();
//
//	// 获取文本
//	CString GetText();
//
//	// 获取字体
//	XT_FONT GetFont();
//
//	// 设置标题
//	void SetTitle(const char *pTitle);
//
//protected:
//	BOOL OnInitDialog();
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//	void OnPaint();
//	void OnOK();
//
//	DECLARE_MESSAGE_MAP()
//
//private:
//	CRichEditCtrlEx m_reText;
//	CP2PTalkToolBar *m_pFontBar;
//	XT_FONT m_font;
//	CBmpBtn m_btnOK;
//	CBmpBtn m_btnCancel;
//	CString m_strText;
//	CString m_strTitle;				// 标题
//};