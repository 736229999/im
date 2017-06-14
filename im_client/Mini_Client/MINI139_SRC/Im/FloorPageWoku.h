#pragma once

#define WM_WOKUTOOL_CLICK      WM_USER+0x0C01

class CWokuToolWnd : public CWnd
{
	DECLARE_DYNAMIC(CWokuToolWnd)

public:
	CWokuToolWnd();
	virtual ~CWokuToolWnd();

public:
	BOOL Create(CWnd *pParentWnd,const CRect &rect, DWORD dwStyle=WS_VISIBLE|WS_CHILDWINDOW|WS_CLIPCHILDREN,UINT nID=0);
	int AddGroup(CString strGroup);
	int AddItem(int nGroup,CString strItem,UINT nBitmap,DWORD dwData, CString strToolTip);

	struct tagItem{
		int nGroup;
		int nItem;
	} ;

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();	
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnMouseWheel (UINT nFlags, short zDelta, CPoint pt);
	
	DECLARE_MESSAGE_MAP()

	void ShowToolTip(CString strTip);
	virtual void InitToolTip();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void HitTest(const CPoint &point, int &iGroup, int &iItem);
	void Draw(CDC *pDC);
	void ReDraw()
	{
		CDC *pDC = GetDC();
		if ( pDC )
		{
			Draw(pDC);
			ReleaseDC(pDC);
		}
	}

protected:
	void CalcScrollInfo();

protected:
	bool m_bMouseOver ;
	CToolTipCtrl m_ToolTip;	
	CString      m_strPreTip ;
	struct tagItemInfo
	{
		DWORD dwData;
		CRect rect;
		CString strItem;
		CString strToolTip;
		UINT  nBitmap;
	};

	struct tagGroupInfo
	{
		DWORD dwData;
		CString strGroup;
		vector<tagItemInfo> vecItem;
	};

	long m_OriginY;
	long m_nClientRealHeigh;

	tagItem m_HotItem, m_SelItem;

	vector<tagGroupInfo> m_vecGroup;

	CFont  m_fontBold, m_fontNormal;
};

// CFloorPageWoku 对话框

class CFloorPageWoku : public CDialog
{
	DECLARE_DYNAMIC(CFloorPageWoku)
	DECLARE_HMG_SCROLLBAR()

public:
	CFloorPageWoku(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFloorPageWoku();


	CWokuToolWnd m_wndTool;
// 对话框数据
	enum { IDD = IDD_FLOOR_WOKU };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK() {}
	virtual void OnCancel() {}
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	LRESULT OnWokuToolClick(WPARAM wParam,LPARAM lParam);

	DECLARE_MESSAGE_MAP()

};
