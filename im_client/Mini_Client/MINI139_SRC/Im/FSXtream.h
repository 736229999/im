//#pragma once
//
//#include "..\resource.h"
//#include "..\controls\skinctrl.h"
//// CFSSearchXtream 对话框
//
//class CFSSearchXtream : public CDialog
//{
//	DECLARE_DYNAMIC(CFSSearchXtream)
//
//public:
//	CFSSearchXtream(CWnd * pWndMsg,CWnd* pParent = NULL);   // 标准构造函数
//	virtual ~CFSSearchXtream();
//
//	bool GetSearchReq(XT_XTREAM_SEARCH_REQ &req);
//
//	CSAFrame *pFrame;
//
//// 对话框数据
//	enum { IDD = IDD_FS_XTREAM };
//
//protected:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//	virtual BOOL OnInitDialog();
//	afx_msg void OnBnClickedRadio();
//	LRESULT OnGetDefID(WPARAM wp, LPARAM lp) ;
//	void	OnFindFriend();
//
//	DECLARE_MESSAGE_MAP()
//
//private:
//	CString		m_strEmail;
//	CString		m_strNickName;
//	CString		m_strUserName;
//	CWnd *		m_pWndMsg;
//	HACCEL		m_hAccel;
//
//	CFlatEdit	 m_edtUserName, m_edtEmail, m_edtNickName;
//
//	CButton m_btnRadio1,m_btnRadio2;
//
//	HBRUSH   m_hBrushBackGround;
//	HBRUSH   m_hBrushDisable   ;
//public:
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	CFlatEdit m_edtID;
//};
