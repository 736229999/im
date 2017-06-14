#pragma once
#include "impublic.h"
#include "..\im\IMProtocol.h"
#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"

// CAddBlackDlg 对话框
/*添加好友成功，选择后分组，修改备注名*/
class CMainFrame;

class CAddBlackDlg : public DuiLib::WindowImplBase
{
	DECLARE_XT_MESSAGE_MAP()

public:
	static void Show(int nFriendID);
	static void UpdateAllBk();
	virtual ~CAddBlackDlg();

private:
	enum
	{
		TIMER_CHECK_SUCCESS = 0x01,
		TIMER_LOADDING = 0x02,
	};

	CAddBlackDlg(int nFriendID);

	virtual LPCTSTR    GetWindowClassName() const			{   return _T("CAddBlackWnd");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("AddFriSuccess.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	void OnFinalMessage(HWND hWnd);

	void OnOK();
	void OnClose();
    void OnBnClickedSendMsg();
	void OnUserInfoAck(const char *data, WORD wRet);
	void AddToComboBox();
	void OnTimer(UINT nIDEvent);
	void UpdateBk();

private:
	DuiLib::CLabelUI *m_plabelUser;
	DuiLib::CEditUI  *m_pEditName;
	DuiLib::CComboUI *m_pComboGroup;

	DuiLib::CButtonUI*m_pBtnClose;
	DuiLib::CButtonUI*m_pBtnOK;
	DuiLib::CButtonUI*m_pBtnSend;

	DuiLib::CTabLayoutUI *m_pTabPage;
	DuiLib::CLabelUI *m_pLbUserInfoLadding;
	DuiLib::CLabelUI *m_pLbUserImg;

	DuiLib::CHorizontalLayoutUI *m_pHLVeriInfo;
	DuiLib::CRichEditUI *m_pREVeriInfo;

	int m_nFriendID;
	FRIEND_INFO* m_pFriend;
	map<int, CString> m_mapGroupInfo;
	static map<int, std::tr1::shared_ptr<CAddBlackDlg>> g_mapDlg;
};

////class CMainFrame;
////class CAddBlackDlg : public CXSkinDlg
////{
////	DECLARE_DYNAMIC(CAddBlackDlg)
////protected:
////	LRESULT OnXtMessage(WPARAM wParam, LPARAM lParam);
////	LRESULT	OnXtResend(WPARAM wParam, LPARAM lParam);
////public:
////	CAddBlackDlg(FRIEND_INFO* p_Friend,CMainFrame* p_IMPage,CWnd* pParent = NULL);   // 标准构造函数
////	virtual ~CAddBlackDlg();
////
////// 对话框数据
////	enum { IDD = IDD_DIALOG_ADD_BLACKLIST };
////	void  AddToComboBox( );
////	void  ClearComboBox();
////protected:
////	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
////	afx_msg void OnBnClickedOk();
////	afx_msg void OnBnClickedCancel();	
////	afx_msg void OnBnClickedSendMsg();
////	afx_msg void OnPaint();
////	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
////	afx_msg void OnUserInfoAck(const char *data, WORD wRet);
////	virtual void PostNcDestroy();
////	BOOL OnInitDialog(); 
////
////private:
////	static CAddBlackDlg * pInstance;
////	FRIEND_INFO* pFriend;
////	CMainFrame*	pIMPage;
////	CBmpBtn	m_btnOK;
////	CBmpBtn	m_btnSend;
////
////	DECLARE_MESSAGE_MAP()
////
////public:
////
////	CResBitmap m_bmpLogo;
////	map<int, CString> m_mapGroupInfo;
////};
