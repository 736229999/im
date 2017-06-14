#pragma once

#include "Resource.h"

#include "afxwin.h"
#include "afxcmn.h"

#include "imagetoolbar.h"
#include "graphcheckcombobox.h"
#include "dhtmledit_blog.h"
#include "buttonext.h"
#include "ToolBarEx.h"
#include "MenuTitleBar.h"
#include "TitleEdit.h"
#include "OpenArticlesDlg.h"
#include "SpaceTypeDef.h"

#include <vector>
#include <map>
#include "buttonext.h"
#include "hyperbtn.h"
#include "dhtmledit_blog.h"
using namespace std;

// CWriteBlogPanel 对话框

class CWriteBlogPanel : public CDialog
{
	DECLARE_DYNAMIC(CWriteBlogPanel)

public:
	CWriteBlogPanel(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWriteBlogPanel();

// 对话框数据
	enum { IDD = IDD_WRITE_BLOG_PANEL };

public:
	CGraphCheckComboBox	m_comboDir;
	CGraphComboBox		m_comboTag;
	CGraphComboBox		m_comboView;
	CStatic				m_btnLink139Blog,m_btnLinkMyZone;
	CButton				m_btnCheckTop;

	CEdit				m_editDir;
	CButton				m_btnCreateDir;
	CHyperBtn			m_btnLocal1,m_btnLocal2,m_btnLocal3;
	CBmpBtn				m_btnDel1,m_btnDel2,m_btnDel3;
	CHyperBtn			m_btnHome, m_btnMore;
	CStatic				m_staticNickName;
	CStatic				m_staticTag,m_staticAppArts,m_staticViewType;
	CHyperBtn			m_btnInsertLink, m_btnInsertLocal,m_btnInsertWeb,m_btnInsertWoku;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnOK() {}
	virtual void OnCancel() {}
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnDropdownComboDir();
	afx_msg void OnCbnDropdownComboTag();
	afx_msg void OnStnClickedStaticLink2();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

protected:
	CBitmap * m_pBmpHead;
public:
	
};


class CWriteBlogDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CWriteBlogDlg)

public:
	CWriteBlogDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWriteBlogDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_WRITEBLOG };

public:
	enum EditArticleType {
		FreshArticle = 0x01,
		LocalSavedArticle ,
		WebArticle,
	};

	void EditLocalSavedArticle(const TLocalSavedArticleInfo &info, DWORD dwSavedID);
	void EditWebArticle(const TWebArticleInfo &info);
	void NewArticle();
	void ResetArticle();
	//void PubLocalSavedArticle(const LocalSavedArticleInfo &article,DWORD dwSavedID);

protected:
	EditArticleType  m_articleType;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	//virtual void OnOK() {}
	//virtual void OnCancel() {}
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnNcDestroy();
	afx_msg void OnCbnSelchangeComboFontsize();
	afx_msg void OnCbnSelchangeComboFontname();
	afx_msg void OnFontBold();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFontItalic();
	afx_msg void OnBnClickedButtonWebPic();
	afx_msg void OnBnClickedMyArtilce();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedBtnDel1();
	afx_msg void OnBnClickedBtnDel2();
	afx_msg void OnBnClickedBtnDel3();	
	void OnUpdateBold(CCmdUI *pCmdUI);
	void OnUpdateItalic(CCmdUI *pCmdUI);
	void OnUpdateUnderLine(CCmdUI *pCmdUI);
	void OnFontUnderLine();
	void OnFontColor();
	void OnFormatNumber();
	void OnFormatBullets();
	void OnFormatAlignLeft();
	void OnFormatAlignCenter();
	void OnFormatAlignRight();
	void OnInsertEmotion();//插入表情
	void OnInsertLink();//插入链接
	afx_msg void OnBnClickedButtonReset();
	afx_msg void OnBnClickedButtonPost();
	afx_msg void OnBnClickedButtonLocalfile();
	afx_msg void OnCbnSelchangeComboMytag();
	afx_msg void OnCbnKillfocusComboDir();
	afx_msg void OnCbnSetfocusComboDir();
	afx_msg void OnCbnKillfocusComboTag();
	afx_msg void OnCbnSetfocusComboTag();	
	afx_msg void OnBnClickedButton139pic();
	afx_msg void OnCbnSelchangeComboDir();
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnStnClickedStaticLocal1();
	afx_msg void OnStnClickedStaticLocal2();
	afx_msg void OnStnClickedStaticLocal3();
	void onmouseupDhtmleditBlog();	
	void onkeydownDhtmleditBlog();	
	void onkeypressDhtmleditBlog();
	void onkeyupDhtmleditBlog();	
	virtual void OnOK() {}
	virtual void OnCancel() {}

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

	
	afx_msg void OnCreateDir();
	virtual void OnHelpClick();
	virtual void PreSubclassWindow();

	LRESULT OnThreadNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnCancelUpload(WPARAM wParam, LPARAM lParam);
	LRESULT OnWebFaceNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnWebPicNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnCloseDlg(WPARAM,LPARAM);

protected:
	void	AdjustUI();
	void	GetRepaintRgn(CRgn &rgn);
	int		InitBlogToolBar();
	void	CreateFontNameBox();
	void	CreateFontSizeBox();
	int    InitMainMenuBar();
	int    InitToolButtons();
	int    InitSetToolBar();
	void   InvalidateButton();

	void    FocusEditor();

protected:
	void	UpdateArticle(BOOL bUpdated=TRUE);
	void    SaveArticleToLocal();

protected:
	//CString ProcessWebFace(CString strDocument);
	bool	CheckLog(CString &sError);
	void	InitMyTagList();
	void	InitDirList();
	void    InitLocalArticle();

protected:

	DWORD   m_dwLocal1,m_dwLocal2,m_dwLocal3;

	TUserBookInfo       m_bookInfo;

	struct tagArticle
	{
		CString strContent	;
		CString strTitle	;
		CString strTag		;
		int     iViewType	;
		int     iTopFlag	;

		vector<TBookInfo> vecBook;
		
		tagArticle()
		{
			strContent = "";
			strTitle   = "";
			strTag     = "";

			iViewType = 0;
			iTopFlag  = 0;
		}

	} m_articleInfo, m_articleInfo2;

	void    InitArticleParams();
	bool    IsArticleModified();

protected:
	HANDLE	m_hContent;
	bool    m_bInitBookCombo;
	static UINT LoadArticleContent(LPVOID p);

protected:
	static CWriteBlogDlg * pInstance;

	static UINT PubLogProc(LPVOID param);
	static UINT InitProc(LPVOID param);
	static UINT InitArticleBookProc(LPVOID param);
	LRESULT OnWebDataNotify(WPARAM wParam,LPARAM lParam);

	CTitleEdit			m_edtTitle;
	CImageToolBar       m_toolBlog;
	CImageToolBar       m_toolSet;
	CDHtmlEditCtrl		* m_pEditBlog;
	CGraphComboBox      m_comboFontName;
	CGraphComboBox      m_comboFontSize;

	CFlatButton			m_buttonNew,m_buttonOpen,m_buttonSave,m_buttonPost,m_buttonMyArts;
	CFlatButton			m_btnInsertWeb,m_btnInsertLink,m_btnInsertLocal,m_btnInsertWoku;

	vector<TLocalSavedArticleInfo>  m_LocalSavedArticles;	
	
	CToolBarEx			m_imgToolbar;
	CMenuTitleBar		m_menuTitleBar;

	CFont				m_Font,m_FontTitle,m_FontTitle1;


	bool                m_bFontBold;
	bool                m_bFontItalic;
	bool                m_bFontUnderLine ;

	DWORD				m_dwSavedID;
	DWORD				m_dwArticleID;

	TArticleBookInfo	m_articleBook;

	CStatic				m_wndHide;

	static map<CString,CString> 			m_mapFile      ;//map<local,web>
	static map<CString,long>                m_mapFileSize  ;//map<local,filesize>


	CString             m_strPubLogErrorInfo ;
	CWnd *              m_pWndProgress   ;

	HANDLE              m_hThreadPub     ;
	HANDLE              m_hThreadInit    ;
	HANDLE			    m_hThreadArticleBook;

	CWriteBlogPanel     m_wndBlogPanel  ;
	CString             m_strDir;

public:
	afx_msg void OnMenuExit();
	void onmousedownDhtmleditBlog();
//	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

	afx_msg void OnBnClickedButtonPost1();
	afx_msg void OnBnClickedButtonTop();
	afx_msg void OnStnClickedButtonWebpic();
	afx_msg void OnStnClickedButtonLocalfile();
	afx_msg void OnStnClickedStaticLink3();
//	afx_msg void OnCbnDropdownComboTag();
	afx_msg void OnNewArticles();
	afx_msg void OnStnClickedStaticArts();
	afx_msg void OnStnClickedLink();
	afx_msg void OnStnClickedStaticWebarts();

	afx_msg void OnMenuNewArticle();
	
	afx_msg void OnMenuSave();
	afx_msg void OnPicLink();
	afx_msg void OnInsertLocalPic();
	afx_msg void OnInsertWebPic();
	afx_msg void OnViewArticle();
	afx_msg void OnViewWebArticles();
	afx_msg void OnStnClickedStaticMore();
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	
	afx_msg void OnStnClickedStaticViewspace();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	
	afx_msg void OnOpen();
	afx_msg void OnBnClickedButtonMyarticle();
	afx_msg void OnStnClickedStaticLocalfile();
	afx_msg void OnStnClickedStaticWeb();
	afx_msg void OnStnClickedStaticLink();
	afx_msg void OnCut();
	afx_msg void OnRedo();
	afx_msg void OnUndo();
	afx_msg void OnSelall();
	afx_msg void OnPaste();
	afx_msg void OnCopy();
	
	afx_msg void OnViewMyarticles();
	afx_msg void OnViewWebarticles();
};
