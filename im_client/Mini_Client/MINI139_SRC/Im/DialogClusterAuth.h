#pragma once

#include <map>
#include "afxwin.h"
#include "FSDialog.h"
#include "../im/RichEditCtrlEx.h"
#include "..\im\IMProtocol.h"
#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"
// CDialogClusterAuth 对话框
////
// 验证入群
class CDialogClusterAuth : public DuiLib::WindowImplBase
{
	DECLARE_XT_MESSAGE_MAP()

public:
	virtual ~CDialogClusterAuth();
	CDialogClusterAuth();
	CDialogClusterAuth(uint32 cid,CWnd* pParent = NULL);   // 标准构造函数

public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("ClusterAuthWnd");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("AddClusterVerify.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);

	static void UpdateBk();
	static std::tr1::shared_ptr<CDialogClusterAuth> GetInstance()	{	return pInstance;	}

	static std::tr1::shared_ptr<CDialogClusterAuth>  Show(uint32 cid);
	void  UpdateClusterInfo(uint32 cid);

	afx_msg void OnBtnAdd();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedAddAgain();
	afx_msg void OnBnClickedClose();

private:
	CWndShadow m_WndShadow;
	DuiLib::CButtonUI*m_pBtnClose;

	DuiLib::CTabLayoutUI *m_pTabLayout;
	DuiLib::CTabLayoutUI *m_pTabLayoutB;
	DuiLib::CRichEditUI  *m_pEditVerify;
	DuiLib::CEditUI  *m_pEditShowInfo;

	DuiLib::CButtonUI*m_pBtnAdd;
	DuiLib::CButtonUI*m_pBtnCancel;

	DuiLib::CButtonUI*m_pBtnReAdd;
	DuiLib::CButtonUI*m_pBtnReCancel;

	static std::tr1::shared_ptr<CDialogClusterAuth> pInstance;
	uint32  m_uiClusterId;
	///CRichEditCtrlEx m_reTextOnline;

/*	static  map<uint32,void *> m_mapClusterAuthId;	*/	
};

////class CDialogClusterAuth : public CXSkinDlg
////{
////	DECLARE_DYNAMIC(CDialogClusterAuth)
////	DECLARE_HMG_SCROLLBAR()
////
////public:
////	virtual ~CDialogClusterAuth();
////
////// 对话框数据
////	enum { IDD = IDC_DIALOG_CLUSETR_AUTH };
////
////public:
////	static CDialogClusterAuth * Show(uint32 cid);
////	static void Close();
////
////protected:
////	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
////	virtual void OnOK() {}
////	virtual void OnCancel() {}
////	virtual BOOL OnInitDialog();
////	afx_msg void OnClose();
////	afx_msg void OnDestroy();
////	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
////	afx_msg void OnBtnOK();
////	afx_msg void OnBnClickedCancel();
////
////	DECLARE_MESSAGE_MAP()
////
////private:
////	CDialogClusterAuth(uint32 cid,CWnd* pParent = NULL);   // 标准构造函数
////
////private:
////	uint32  m_uiClusterId;
////	static  map<uint32,void *> m_mapClusterAuthId;
////	CFlatEdit m_edtAuthInfo;
////	CBmpBtn m_btnOk,m_btnCancel;
////	CBmpBtn m_btnAddAgain,m_btnClose;
////	HBRUSH	m_hBkGround ;
////
////public:
////	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
////	void InitDisplay();
////	void HideUI();
////	afx_msg void OnBnClickedAddAgain();
////	afx_msg void OnBnClickedClose();
////
////};
