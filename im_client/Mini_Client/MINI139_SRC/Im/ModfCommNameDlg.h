#pragma once
#include "impublic.h"
#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"

class CMainFrame;
class CModfCommNameDlg : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("ModfCommNameWnd");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("BackNameMod.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	static void UpdateBk();
	static std::tr1::shared_ptr<CModfCommNameDlg> GetInstance()	{	return pInstance;	}

	virtual ~CModfCommNameDlg();
	CModfCommNameDlg();
	void OnOK();
	void OnCancel();
	static std::tr1::shared_ptr<CModfCommNameDlg> Show(FRIEND_INFO* pFriend);
	void  UpdateFriendInfo(FRIEND_INFO *pFriend);

private:
	CWndShadow m_WndShadow;

	DuiLib::CEditUI *m_pEditName;

	DuiLib::CButtonUI*m_pBtnClose;
	DuiLib::CButtonUI*m_pBtnMin;
	DuiLib::CButtonUI*m_pBtnOK;
	DuiLib::CButtonUI*m_pBtnCancel;
		
	static std::tr1::shared_ptr<CModfCommNameDlg> pInstance;
	FRIEND_INFO*	m_pFriend;
};


//#include "..\resource.h"
//#include "impublic.h"
//
//class CMainFrame;
//
////------------------------------------------------------
//// 修改用户备注名称对话框
////------------------------------------------------------
//
//class CModfCommNameDlg : public CXSkinDlg
//{
//	DECLARE_DYNAMIC(CModfCommNameDlg)
//protected:
//
//	CModfCommNameDlg(CMainFrame *pIMPage,FRIEND_INFO* pFriend, CWnd* pParent = NULL);   // 标准构造函数
//
//public:
//
//	virtual ~CModfCommNameDlg();
//
//// 对话框数据
//	enum { IDD = IDD_MODF_COMMNAME };
//
//	static CModfCommNameDlg*	NewDlg(CMainFrame* pIMPage,FRIEND_INFO* pFriend,CWnd* pParent = NULL);
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//
//	DECLARE_MESSAGE_MAP()
//public:
//	afx_msg void OnBnClickedOk();
//
//protected:
//
//	CMainFrame*	m_pIMPage;		// IM窗体
//	//uint32			m_nFriendID;	// 被修改备注名称的好友ID
//	CBmpBtn			m_btnOK;
//	CBmpBtn			m_btnCancel;
//	CFlatEdit		m_edtCommName;
//	FRIEND_INFO*	m_pFriend;
//
//public:
//	afx_msg void OnNcDestroy();
//	virtual BOOL OnInitDialog();
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
//};
