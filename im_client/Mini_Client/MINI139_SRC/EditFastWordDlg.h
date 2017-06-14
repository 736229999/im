#pragma once
#include "XSkinDlg.h"
#include "../im/RichEditCtrlEx.h"
#include "../Im/LocalData.h"
// CEvalueDlg dialog

#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"


class CEditFastWordDlg : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UIFastWord");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("FastWordAdd.xml");  }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	virtual void Notify(DuiLib::TNotifyUI& msg);
	virtual void InitWindow();
	CEditFastWordDlg(CString strText,CWnd* pParent = NULL);   // standard constructor
	// 获取文本
	CString GetText();

	// 获取字体
	XT_FONT GetFont();

	// 设置标题
	void SetTitle(const char *pTitle);

	void OnOK();
	void UpdateBk();

public:
	CRichEditCtrlEx m_reTextOnline;
	AUTO_REPLY m_AutoReply;
private:
	CWndShadow m_WndShadow;
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
			if(m_reTextOnline.GetSafeHwnd() == NULL)
				m_reTextOnline.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | ES_WANTRETURN
					| WS_VSCROLL | ES_AUTOVSCROLL, rc, CWnd::FromHandle(GetHWND()), 10002);
			pUI->Attach(m_reTextOnline.GetSafeHwnd());    
			return pUI;
		}
	}
};

////class CEditFastWordDlg : public CXSkinDlg
////{
////	DECLARE_DYNAMIC(CEditFastWordDlg)
////
////public:
////	CEditFastWordDlg(CString strText, CWnd* pParent = NULL);   // standard constructor
////	virtual ~CEditFastWordDlg();
////
////	// 获取文本
////	CString GetText();
////
////	// 设置标题
////	void SetTitle(const char *pTitle);
////
////protected:
////	BOOL OnInitDialog();
////	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
////	void OnPaint();
////	void OnOK();
////
////	DECLARE_MESSAGE_MAP()
////
////private:
////	CRichEditCtrlEx m_reText;
////	CBmpBtn m_btnOK;
////	CBmpBtn m_btnCancel;
////	CString m_strText;
////	CString m_strTitle;		// 标题
////};