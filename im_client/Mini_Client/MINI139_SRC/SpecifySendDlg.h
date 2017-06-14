#pragma once

#include "im\IMProtocol.h"
// CSpecifySendDlg 对话框

#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"
// 指定聊天窗口给父窗口发送的消息
#define WM_SPEC_SEND_DLG_MSG (WM_USER+635)
////

class CSpecifySendDlg : public DuiLib::WindowImplBase
{
	DECLARE_XT_MESSAGE_MAP()

public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("SpecifiedSendWnd");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("SpecifiedSend.xml");    }
	static std::tr1::shared_ptr<CSpecifySendDlg> GetInstance()	{	return pInstance;	}
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);

	static std::tr1::shared_ptr<CSpecifySendDlg> Show();
	static void CloseDlg();
	static void UpdateBk();
	// 查找用户
	void OnSearchUser(const char *data, int nRetCode);

	void OnOK();
	void OnCancel();

private:
	CWndShadow m_WndShadow;
	static std::tr1::shared_ptr<CSpecifySendDlg> pInstance;

	DuiLib::CEditUI *m_pEditName;

	DuiLib::CButtonUI*m_pBtnClose;
	DuiLib::CButtonUI*m_pBtnOK;
	DuiLib::CButtonUI*m_pBtnCancel;
	BOOL            m_bSearch;//判断是否需要响应CMD_XTREAM_SEARCH_ACK
};

////class CSpecifySendDlg : public CXSkinDlg
////{
////	DECLARE_DYNAMIC(CSpecifySendDlg)
////
////public:
////	CSpecifySendDlg();   // 标准构造函数
////	virtual ~CSpecifySendDlg();
////	BOOL OnInitDialog();
////	BOOL Create(CWnd* pParent);
////
////protected:
////	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
////	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
////	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
////	void OnOK();
////	void OnCancel();
////	DECLARE_MESSAGE_MAP()
////	DECLARE_XT_MESSAGE_MAP()
////	// 查找用户
////	void OnSearchUser(const char *data, int nRetCode);
////
////private:
////	CBmpBtn m_btnOK;
////	CBmpBtn m_btnCancel;
////	CFlatEdit m_pEditName->;
////	BOOL            m_bSearch;//判断是否需要响应CMD_XTREAM_SEARCH_ACK
////};
