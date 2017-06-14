#pragma once


// CTabWndCtrl 对话框



/*这个是竖着的TAB,    zhangmin*/
class CTabWndCtrl : public CDialog
{
	DECLARE_DYNAMIC(CTabWndCtrl)

public:
	CTabWndCtrl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabWndCtrl();

	struct tagTabInfo
	{
		CWnd *pPage;
		CString strToolTip;
		UINT nBitmap;
	};
// 对话框数据
	enum { IDD = IDD_DIALOG_TABWND };

public:
	int  AddTab(CWnd *pWnd,CString strToolTip,UINT nBitmap);
	void SetCurSel(int nSel);
	int  GetCurSel();
	CWnd * GetTabSheet(int nTab);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK() {}
	virtual void OnCancel() {}

	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

private:
	int TabHitTest(const CPoint &pt);

private:
	vector<tagTabInfo> m_vecTab ;
	int			   m_nCurSel;
	int            m_nHotTab;
	int            m_nPreHotTab;
	CRect		   m_rcTab  ;
	bool           m_bMoseOver;
	CToolTipCtrl   m_Tooltip ;	
	
public:
	virtual BOOL OnInitDialog();
};
