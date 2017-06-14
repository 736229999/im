#pragma once

#include "im\IMProtocol.h"
// CSpecifySendDlg �Ի���

#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"
// ָ�����촰�ڸ������ڷ��͵���Ϣ
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
	// �����û�
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
	BOOL            m_bSearch;//�ж��Ƿ���Ҫ��ӦCMD_XTREAM_SEARCH_ACK
};

////class CSpecifySendDlg : public CXSkinDlg
////{
////	DECLARE_DYNAMIC(CSpecifySendDlg)
////
////public:
////	CSpecifySendDlg();   // ��׼���캯��
////	virtual ~CSpecifySendDlg();
////	BOOL OnInitDialog();
////	BOOL Create(CWnd* pParent);
////
////protected:
////	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
////	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
////	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
////	void OnOK();
////	void OnCancel();
////	DECLARE_MESSAGE_MAP()
////	DECLARE_XT_MESSAGE_MAP()
////	// �����û�
////	void OnSearchUser(const char *data, int nRetCode);
////
////private:
////	CBmpBtn m_btnOK;
////	CBmpBtn m_btnCancel;
////	CFlatEdit m_pEditName->;
////	BOOL            m_bSearch;//�ж��Ƿ���Ҫ��ӦCMD_XTREAM_SEARCH_ACK
////};
