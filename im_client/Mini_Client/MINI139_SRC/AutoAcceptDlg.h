#pragma once
#include "resource.h"
#include "..\im\impublic.h"
#include "../im/RichEditCtrlEx.h"
#include "../im/CustomToolBar.h"
#include "../im/LocalData.h"
// CAutoAcceptDlg �Ի���
//
#include "..\..\..\DuiLib\UIlib.h"


class CAutoAcceptDlg : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UIAutoAccept");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("autoaccept.xml");  }
	virtual void InitWindow();
	//virtual void OnClick(DuiLib::TNotifyUI& msg);
	//virtual void Notify(DuiLib::TNotifyUI& msg);
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	// ����
	CAutoAcceptDlg();
	void Save();
	vector<AUTO_REPLY> m_vecItem;

	// ��ȡ�Զ��Ӵ�������ߣ�
	void GetAutoAccept_Online(AUTO_REPLY &online);
	
	// ��ȡ�Զ��Ӵ�������ߣ�
	void GetAutoAccept_Offline(AUTO_REPLY &offline);

private:
	CP2PTalkToolBar m_pfontOnline;
	CRichEditCtrlEx m_reTextOnline;

	CP2PTalkToolBar m_pfontOffline;
	CRichEditCtrlEx m_reTextOffline;


	//CP2PTalkToolBar m_wndTalkToolBar;
	//CRichEditCtrlEx m_reTextOnline;
	//CRichEditCtrlEx /*m_reditSend*/;

	AUTO_REPLY m_AutoAcceptOnline;
	AUTO_REPLY m_AutoAcceptOffline;


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
				/*| WS_HSCROLL | ES_AUTOHSCROLL*/  // �Զ�ˮƽ����
				| WS_VSCROLL | ES_AUTOVSCROLL, rc, CWnd::FromHandle(GetHWND()), 10002);
			pUI->Attach(m_reTextOnline.GetSafeHwnd());          
			return pUI;
		}

		if (_tcsicmp(pstrClassName, _T("RichEditOffWnd")) == 0)////���߿�
		{
			CWndUI  *pUI  = new CWndUI;      
			RECT rc;
			rc.left = 0;
			rc.right = 0;
			rc.top = 0;
			rc.bottom = 0;

			m_reTextOffline.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | ES_WANTRETURN
				/*| WS_HSCROLL | ES_AUTOHSCROLL*/  // �Զ�ˮƽ����
				| WS_VSCROLL | ES_AUTOVSCROLL, rc, CWnd::FromHandle(GetHWND()), 10003);
			pUI->Attach(m_reTextOffline.GetSafeHwnd());          
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

			g_LocalData.GetAutoAccept_Online(m_AutoAcceptOnline);
			m_pfontOnline.SetFont(&(m_AutoAcceptOnline.font));////���ñ�����������
			m_pfontOnline.Create(CP2PTalkToolBar::IDD, CWnd::FromHandle(GetHWND()));
			m_pfontOnline.MoveWindow(&rc);
			pUI->Attach(m_pfontOnline.GetSafeHwnd());     
			return pUI;
		}

		if (_tcsicmp(pstrClassName, _T("TalkOffWnd")) == 0)////����
		{
			CWndUI  *pUI  = new CWndUI;      
			RECT rc;
			rc.left = 0;
			rc.right = 0;
			rc.top = 0;
			rc.bottom = 0;

			g_LocalData.GetAutoAccept_Offline(m_AutoAcceptOffline);
			m_pfontOffline.SetFont(&(m_AutoAcceptOffline.font));////���ñ�����������
			m_pfontOffline.Create(CP2PTalkToolBar::IDD, CWnd::FromHandle(GetHWND()));
			m_pfontOffline.MoveWindow(&rc);
			pUI->Attach(m_pfontOffline.GetSafeHwnd());           
			return pUI;
		}

		return NULL;
	}

};

//class CAutoAcceptDlg : public CDialog
//{
//	DECLARE_DYNAMIC(CAutoAcceptDlg)
//
//public:
//	CAutoAcceptDlg(CWnd* pParent = NULL);   // ��׼���캯��
//	virtual ~CAutoAcceptDlg();
//
//	// ��ȡ�Զ��Ӵ�������ߣ�
//	void GetAutoAccept_Online(AUTO_REPLY &online);
//
//	// ��ȡ�Զ��Ӵ�������ߣ�
//	void GetAutoAccept_Offline(AUTO_REPLY &offline);
//
//// �Ի�������
//	enum { IDD = IDD_WIZARD_AUTO_ACCEPT };
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
//	void OnOK()	{}
//	void OnCancel() {}
//	BOOL OnInitDialog();
//
//	DECLARE_MESSAGE_MAP()
//public:
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnSize(UINT nType, int cx, int cy);
//
//private:
//	CP2PTalkToolBar *m_pfontOnline;
//	CRichEditCtrlEx m_reTextOnline;
//
//	CP2PTalkToolBar *m_pfontOffline;
//	CRichEditCtrlEx m_reTextOffline;
//
//	AUTO_REPLY m_AutoAcceptOnline;
//	AUTO_REPLY m_AutoAcceptOffline;
//
//	HBRUSH m_hBkBrush;
//};
