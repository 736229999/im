#pragma once
#include "stdafx.h"
#include "improtocol.h"

#include "ClusterPage1.h"
#include "ClusterPage2.h"
#include "ClusterPage3.h"
#include "..\IMClient.h"
#include "..\DialogClusterManage.h"//包含头文件的方式
#include "..\..\SpaceManagement\public\public_controls\ResGdiObj.h"

#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"

/////////////////////////////////////////////////////////////////////////////


class CCreateClusterDlg : public DuiLib::WindowImplBase
{
	DECLARE_XT_MESSAGE_MAP()

public:
	static std::tr1::shared_ptr<CCreateClusterDlg> Show();

public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("CreateCluster");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("CreateCluster.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	// 消息
	void Notify(DuiLib::TNotifyUI& msg);
	static void UpdateBk();

private:
	void OnBnClickedOK();

	void OnCreateAck(const char *data,int nRetCode);

private:
	CWndShadow m_WndShadow;
	static std::tr1::shared_ptr<CCreateClusterDlg> pInstance;

	DuiLib::CEditUI *m_pEditClusterName;
	DuiLib::CComboUI *m_pComboType;
	DuiLib::CRichEditUI *m_pREditBriefInfo;
	DuiLib::COptionUI *m_pRedioJoinLevel_allowall;
	DuiLib::COptionUI *m_pRedioJoinLevel_needverify;
	DuiLib::COptionUI *m_pRedioJoinLevel_disablejoin;

	DuiLib::CButtonUI *m_pBtnOK;
	DuiLib::CButtonUI *m_pBtnCancel;

	int m_nClusterType;
	int m_nLevel;
};
//
//
//// // CClusterCreateDlg 对话框 
//class CClusterCreateDlg : public CXSkinDlg
//{
//	// Construction
//public:
//	CClusterCreateDlg(CWnd* pParent = NULL);   // standard constructor
//	virtual ~CClusterCreateDlg();
//
//public:
//	// Dialog Data
//	enum { IDD = IDD_WIZARD_CLUSTER };
//
//public:
//	static CClusterCreateDlg * pInstance;
//
//	CBmpBtn m_btnCancel;
//	CBmpBtn m_btnPrev;
//	CBmpBtn m_btnNext;
//
//protected:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
//	virtual BOOL OnInitDialog();
//	virtual void OnHelpClick();
//
//	afx_msg void OnBnClickedNext();
//	afx_msg void OnBnClickedPrev();
//	afx_msg void OnBnClickedCancel();
//	afx_msg void OnDestroy();
//	afx_msg void OnTimer(UINT nIDEvent);
//	afx_msg void OnClose();
//	afx_msg void OnPaint();
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//
//	DECLARE_MESSAGE_MAP()
//	DECLARE_XT_MESSAGE_MAP()
//
//	void UpdatePage();
//	void InitPara();
//
//	void OnCreateAck(const char *data,int nRetCode);
//	void OnSetCMemberAck(const char *data, int nRetCode);
//	void OnUserFamilyFlagAck(const char *data, WORD wRet);
//
//protected:
//
//	CStatic			m_wndFrame;
//	int				m_nNextPos;//创建群分包，下一次位置
//	vector<int>		m_vecPos;
//	int				m_nStep;	
//
//	int				m_nCluster;	// 新创建群的id
//	
//	CLUSTER_INFO	m_clusterInfo;
//	CResBitmap		m_bmpLogo;
//	CClusterPage1 *	m_pPage1;
//	CClusterPage2 *	m_pPage2;
//	CClusterPage3 *	m_pPage3;
//	DWORD			m_nSetCMembersSeq;
//	CString			m_strTip;
//};
//
