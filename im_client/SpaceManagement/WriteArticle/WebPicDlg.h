#pragma once

// CWebPicDlg 对话框
class CWebPicDlg : public CDialog
{
	DECLARE_DYNAMIC(CWebPicDlg)

public:
	virtual ~CWebPicDlg();

public:
	static void Show(int x, int y, HWND hWndSel);
	static void Destory();
	// 对话框数据
	enum { IDD = IDD_DIALOG_WEBPIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnNcDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedButtonRight();
	afx_msg void OnBnClickedButtonLeft();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()

	LRESULT OnWebDataNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnWebPictureNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnThreadNotify(WPARAM wParam, LPARAM lParam);

protected:
	CWebPicDlg(CWnd* pParent = NULL);   // 标准构造函数
	static CWebPicDlg *pInstance ;

protected:
	void	InitPictures();
	void	AdjustUI();
	void	SafeTerminateThreads();
	void	ClearBitmaps();
	void	DrawList(CDC *pDC);
	void	ReDrawList() ;
	void	UpdateButtonState();
	int		HitTest(const CPoint & point);
	void	InitToolTip();
	void	ShowToolTip( CString strTip);
	
protected:
	static UINT LoadBitmapProc(LPVOID p);

protected:

	struct tagThreadParam
	{
		HANDLE hThread		;
		uint32 pic_id		;
		DWORD  dwCreateDate	;
		char   szFile[255]	;

		tagThreadParam()
		{
			memset(this,0,sizeof(tagThreadParam));
		}
	};

	struct tagPicture
	{
		uint32		pic_id			;
		DWORD       dwCreateDate	;
		CBitmap *	pBitmap			;

		tagPicture()
		{
			pBitmap = NULL;
		}
	};

	vector<tagPicture>  m_vecBitmaps;
	::CCriticalSection  m_secBitmaps;
	vector<HANDLE>		m_vecThreads;
	::CCriticalSection	m_secThreads;

	DWORD				m_nFocusPictureID;
	int                 m_iPicturePosition;
	int					m_iHotPicture;
	CResBitmap          m_bmpPicFrame;
	CResBitmap          m_bmpPicPig;
	CResBitmap          m_bmpError;

	CColumnBtn m_btnLeft,	m_btnRight;
	CRect					m_ListRect;
	TUserWebPictureInfo		m_myPictures;
	FIFO<TWebPictureInfo *>	m_fifoPics;

	bool					m_bWaittingFlag;
	int						m_nDownloadThreadRef;

	CToolTipCtrl	m_ToolTip		;

	HWND			m_hPicSelWnd	;
	CXSliderCtrl	m_wndSlider		;
};
