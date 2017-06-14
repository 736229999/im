#pragma once


// CFloorPageIM 对话框

class CIMTabWndCtrl : public CDialog
{
	DECLARE_DYNAMIC(CIMTabWndCtrl)

public:
	CIMTabWndCtrl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CIMTabWndCtrl();

	struct tagTabInfo
	{
		CWnd *pPage;
		CString strTabText;
		CRect   rcTab;
		UINT nBitmap;
	};
// 对话框数据
	enum { IDD = IDD_DIALOG_TABWND };

public:
	int  AddTab(CWnd *pWnd,CString strTabText, UINT nBitmap);
	void SetCurSel(int nSel);
	int  GetCurSel();
	CWnd * GetTabSheet(int nTab);


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK() {}
	virtual void OnCancel() {}
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonGuide();
	void ShowToolTip(const CString& strTip);
	BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()

protected:
	int TabHitTest(const CPoint &pt);

private:
	vector<tagTabInfo> m_vecTab ;
	int			   m_nCurSel;
	int            m_nHotTab;
	int            m_nPreHotTab;
	CRect		   m_rcTab  ;
	bool           m_bMouseOver;

	CResBitmap     m_bmpTabHot, m_bmpTabNormal;
	
	CResBitmap     m_bmpRecentHot, m_bmpRecentNormal;
	CResBitmap     m_bmpFriendHot, m_bmpFriendNormal;
	CResBitmap	   m_bmpGroupHot, m_bmpGroupNormal;

	
	CToolTipCtrl	m_toolTip;
	CString m_sLastTip,m_sTip;

	
	CHyperBtn		m_btnGuide;		//注册

};
