//#pragma once
//
//// CFSSearchCluster 对话框
//
//class CFSSearchCluster : public CDialog
//{
//	DECLARE_DYNAMIC(CFSSearchCluster)
//public:
//
//	CFSSearchCluster(CWnd * pWndMsg,CWnd* pParent = NULL);   // 标准构造函数
//	virtual ~CFSSearchCluster();
//
//// 对话框数据
//	enum { IDD = IDD_FS_CLUSTER };
//
//	bool GetSearchReq(XT_CLUSTER_SEARCH_REQ &req);
//
//protected:
//	afx_msg void OnBnClickedRadio();
//	virtual BOOL OnInitDialog();
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//	virtual void OnOK() {}
//	virtual void OnCancel() {}
//	LRESULT OnGetDefID(WPARAM wp, LPARAM lp) ;
//	void	OnFindCluster();
//	DECLARE_MESSAGE_MAP()
//	CWnd *		m_pWndMsg;
//	CFlatEdit  m_edtName;
//	CFlatEdit  m_edtID;
//	CGraphComboBox m_combType;
//	HACCEL		m_hAccel;
//public:
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//};
