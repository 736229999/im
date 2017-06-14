#pragma once

#pragma once

#include "afxcmn.h"
#include "afxwin.h"

#include "explorer1.h"

#include "XSkinDlg.h"
#include "FormProgress.h"
// CMainDlg 对话框
void RemoveHtmlAttribute(CString &strHtml, CString strAttribute);

class CMainDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_MAIN };

public:
	static CMainDlg * Show();
	static void  Close();
	static CMainDlg * GetInstance() { return pInstance; }

public:
	void UpdateLocalArticleList(DWORD dwSavedID, CString strTitle);
	void UpdateWebArticleList(DWORD dwFocusArticleID);
	void OnUploadedLocalArticle(DWORD dwSavedID);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnOK() {}
	virtual void OnCancel() {}
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonPost();
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnBnClickedButtonEdit();
	afx_msg void OnListWebClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListWebDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListWebColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListWebItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListLocalClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnListLocalDblClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListLocalColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnListLocalItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonBackup();
	afx_msg void OnBnClickedButtonSetting();
	afx_msg void OnCbnSelchangeComboView();
	afx_msg void OnBnClickedCheckTop();
	afx_msg void OnStnClickedStaticDownload();
	afx_msg void OnNMRclickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	DECLARE_MESSAGE_MAP()

	LRESULT OnWebDataNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnCancelPubArticle(WPARAM wParam, LPARAM lParam);
	LRESULT OnThreadNotify(WPARAM wParam,LPARAM lParam);

	virtual void OnHelpClick();

protected:
	void    AdjustUI();
	void	GetRepaintRgn(CRgn &rgn);
	void	DrawTabBtn(CDC *pDC, const CRect &rcTab, const CString &sText, bool bBold);
	void    SetActiveTab(int iTab);
	void    ReDrawTabBtns();

protected:
	void    InitListCtrls();
	void    ShowStatusTip(CString str);
	void    PutArticlesToListWeb(vector<TWebArticleInfo *> &info );
	void    ShowWebArticle(const TWebArticleInfo * article);

protected:
	void	InitWebArticles();
	void	InitLocalArticles();
	void    UpdateToolState();

protected:
	void	SortList(CXListCtrl &wndList,int nColumn);
	void    SortList(CXListCtrl &wndList,int nColumn,bool bAscend);
	void	QuickSortList(CXListCtrl &wndList,int nColumnCount, int nColumn, int iLo, int iHi, bool bAscend);

protected:
	CExplorer1  m_wndExploreWeb;
	CExplorer1  m_wndExploreLocal;
	CXListCtrl  m_wndListWeb;
	CXListCtrl  m_wndListLocal;

	CRect       m_rcTabBtn1, m_rcTabBtn2;
	int         m_iTabBtn;

	CSplitterControl m_wndSplitter;
	int         m_nSplitterPos;

	CXpButton		m_btnNew;
	CXpButton		m_btnPost;
	CXpButton		m_btnBackup;
	CXpButton		m_btnSetting;
	CGraphComboBox	m_comboSearch;
	CGraphComboBox  m_comboView;
	CFlatEdit		m_edtKey;
	CXpButton		m_btnSearch;
	CFlatButton		m_btnEdit;
	CFlatButton		m_btnDelete;
	CButton			m_btnCheckTop;
	//	CBmpBtn2		m_btnUpdown;

	CImageList      m_ImageList;
	CString         m_strTip;

protected:
	static CMainDlg *		pInstance;

	CString					m_strDefaultFile;
	TUserWebArticleInfo		m_myArticles;
	TLocalSavedArticleInfo  m_currendLocalArticle;

	bool			m_bUpdatingMyAritles ;

	bool			m_bAttachTopSide;
	int				m_nPreSplitterPos;

protected:
	HANDLE			m_hThread;

	static UINT		PubArticleProc(LPVOID p);
	static UINT     WebArticleBackupProc(LPVOID p);

	CFormProgress3 * m_pFormProgress;
	DWORD			m_dwFocusWebArticleID;

public:
	DECLARE_EVENTSINK_MAP()
	void BeforeNavigate2ExplorerLocal(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel);
	void BeforeNavigate2ExplorerWeb(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel);
};
