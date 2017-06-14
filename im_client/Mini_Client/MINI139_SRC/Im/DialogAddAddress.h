#pragma once

#include "FormViewCtrl.h"
#include "personinfo.h"
#include "FloorPageAddbook.h"
#include "XSkinDlg.h"
#include "buttonext.h"

#define WM_ADDRESS_NOTIFY		WM_USER + 101



struct tagContactInfo
{
	ContactGroupInfo  groupInfo;
	ContactPersonInfo personInfo;
};


class CDialogAddAddress : public CXSkinDlg
{
	DECLARE_DYNAMIC(CDialogAddAddress)

public:
	virtual ~CDialogAddAddress();

// 对话框数据
	enum { IDD = IDD_DIALOG_ADD_ADDRESS };	
	enum AddrInfoType { Addr_Add=0,Addr_Mofify,};

public:
	static void Show(AddrInfoType type, const ContactPersonInfo &personInfo, const ContactGroupInfo &groupInfo, HWND hTabAddbookPage);
	static void Close();

protected:
	CDialogAddAddress(CWnd* pParent = NULL);   // 标准构造函数

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnHelpClick();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()

	LRESULT OnModifyNotify(WPARAM,LPARAM);

protected:
	bool  Save();

protected:
	CFormViewCtrl		m_wndAddressPart;

	int					m_nTopAddressPart;
	int					m_nBottomAddressPart;

	tagContactInfo		m_addressInfo;
	AddrInfoType		m_dialogType;

	CBmpBtn		m_wndBtnClear;
	CBmpBtn		m_wndBtnOk;
	CBmpBtn		m_wndBtnCancel;
	CBmpBtn		m_wndBtnApply;

	HWND		m_hTabAddbookPage;

	static vector<CDialogAddAddress *> vecInstance;
};
