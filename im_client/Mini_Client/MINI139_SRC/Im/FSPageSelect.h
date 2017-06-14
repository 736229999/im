//#pragma once
//#include "afxcmn.h"
//#include "FSXtream.h"
//#include "FSCluster.h"
//#include "..\controls\skinctrl.h"
//
//enum FS_TYPE 
//{
//	FS_XTREAM  = 0,
//	FS_CLUSTER = 1, 
//};
//
//// CFSPageSelect 对话框
//
//class CFSPageSelect : public CDialog
//{
//	DECLARE_DYNAMIC(CFSPageSelect)
//
//public:
//	CFSPageSelect(CWnd * pWndMsg,CWnd* pParent);   // 标准构造函数
//	virtual ~CFSPageSelect();
//
//// 对话框数据
//	enum { IDD = IDD_FS_SELECT };
//
//	FS_TYPE GetSearchType();
//	void SetSearchType(FS_TYPE type);
//
//	bool GetSearchReq(XT_XTREAM_SEARCH_REQ &req);
//	bool GetSearchReq(XT_CLUSTER_SEARCH_REQ &req);
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//	virtual BOOL OnInitDialog();
//	virtual void OnOK() {}
//	virtual void OnCancel() {}
//	afx_msg void OnDestroy();
//	afx_msg void OnSize(UINT nType, int cx, int cy);
//	afx_msg void OnBnClickedRadio();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//
//	DECLARE_MESSAGE_MAP()
//
//private:
//	FS_TYPE  m_fsType;
//
//	CSAFrame *pFrame;
//	CWnd * m_pWndMsg;
//
//	CFSSearchXtream *  m_pFSXtream;
//	CFSSearchCluster * m_pFSCluster;
//};
