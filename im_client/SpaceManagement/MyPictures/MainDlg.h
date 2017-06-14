#pragma once


#include "formprogress.h"
#include "albumfolder.h"
// CMainDlg 对话框


template <class T>
class FIFO : public list<T>
{
public:
	inline bool Pop(T& data)
	{
		if ( size()>0 ) 
		{
			data = front();
			pop_front();
			return true;
		}
		return false;
	}

	inline bool Push(const T& data,bool bTail=true)
	{
		if ( bTail )
			push_back(data);
		else
			push_front(data);

		return true;
	}

	inline long  GetCount()
	{
		return size();
	}

	inline void Clear()
	{
		clear();
	}

	T operator [] (int n)
	{
		ASSERT( n>=0 || n<size() );

		list<T>::iterator it;
		it=begin();
		while (n-->0)
			it++;
		return *it;
	}
};

class CMainDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CMainDlg) 

public:
	CMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_MYPICTURE };

public:
	static CMainDlg * Show();
	static void  Close();
	static CMainDlg * GetInstance() { return pInstance; }

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK() {}
	virtual void OnCancel() {}	
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedButtonLeft();
	afx_msg void OnBnClickedButtonRight();
	afx_msg void OnClose();
	afx_msg void OnNcDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnCbnSelchangeComboView();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedButtonLeft2();
	afx_msg void OnBnClickedButtonRight2();
	afx_msg void OnEnChangeEditDescription();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonSetting();
	afx_msg void OnBnClickedButtonPic();
	afx_msg void OnBnClickedButtonBigphoto();
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnBnClickedButtonDeletePic();
	afx_msg void OnBnClickedButtonDeleteAlbum();
	afx_msg void OnBnClickedButtonCopyUrl();
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnStnClickedStaticDownload();	
	afx_msg void OnBnClickedAlbumFolder();
	DECLARE_MESSAGE_MAP()

	LRESULT OnWebDataNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnWebPictureNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnThreadNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnCancelBackup(WPARAM wParam, LPARAM lParam);

	virtual void OnHelpClick();

	void OnPictureSaveAs();
	void OnPictureOpen();
	void UpdateWindowText(CString s);


protected:
	void	AdjustUI();
	void	DrawPicture(CDC *pDC);
	void	DrawList(CDC *pDC);
	void	ReDrawList();
	void    ReDrawPicture();
	void    ReDraw();
	int		HitTest(const CPoint & point);
	void	GetRepaintRgn(CRgn &rgn);
	void	ShowPicture(int iPicturePosition);
	void    SetLabelPictureNum(int n);
	void    UpdateButtonState();
	CBitmap * FindBitmap(uint32 pic_id);
	void      AddBitmap(uint32 pic_id, CBitmap *pBitmap);
	void	 UpdateAlbumNum();
	CString  GetAlbumName(uint32 album_id);

protected:
	void    ShowAlbum(uint32 album_id);
	void    InitPictures();
	void	SafeTerminateThreads();
	void    ClearBitmaps();
	int	    FindPicturePosition(uint32 pic_id);

protected:
	static UINT LoadBitmapProc(LPVOID p);
	static UINT LoadFocusPictureProc(LPVOID p);
	static UINT WebPictureExportProc(LPVOID p);

	struct     tagExportThreadParam
	{
		int     iPictureIndex;
		int     iThreadIndex;
		char    szCache[255];
		char    szLocal[255];
		HANDLE  hThread;
	};

	HANDLE     m_hExportThreads[5];
	int        m_iExportCursor;
	int		   m_iExportedCount;
	CString    m_strBackupPath;

protected:
	struct tagThreadParam
	{
		HANDLE hThread		;
		uint32 pic_id		;
		uint32 album_id		;
		DWORD  dwCreateDate	;
		char   szFile[255]	;

		tagThreadParam()
		{
			memset(this,0,sizeof(tagThreadParam));
		}
	};

	TUserAlbumInfo			  m_myAlbumInfo;

	map<uint32,CBitmap *>     m_mapBitmaps;//map<picture id, CBitmap *>
	vector<TWebPictureInfo *> m_vecBitmaps;
	CCriticalSection	 m_secBitmaps;

	vector<HANDLE>		m_vecThreads;
	HANDLE              m_hLoadFocusPic;
	CCriticalSection	m_secThreads;

	int                 m_iPicturePosition;
	CResBitmap          m_bmpPicFrame;
	CResBitmap          m_bmpPicPig;
	CResBitmap          m_bmpError;

	Bitmap			*   m_pFocusPicture;
	CString             m_strDescription;
	uint32              m_nFocusPictureID;
	CString				m_strFocusPictureFile;
	int					m_iHotPicture ;
	uint32				m_nAlbumID;

protected:
	static CMainDlg *  pInstance;

protected:
	CColumnBtn			m_btnLeft;
	CColumnBtn			m_btnRight;
	CColumnBtn			m_btnBigLeft;
	CColumnBtn			m_btnBigRight;
	CXSliderCtrl		m_wndSlider;

	CRect				m_PictureRect;
	CRect				m_ListRect;

	CAlbumFolder	    m_wndAlbumFolder;
	CXpButton			m_btnPic;
	CXpButton			m_btnBigHead;
	CXpButton			m_btnRefresh;
	CXpButton			m_btnExport;
	CXpButton			m_btnSetting;
	CFlatEdit			m_edtDescription;
	CGraphComboBox		m_comboView;
	CXpButton			m_btnSave;
	CXpButton           m_btnDeletePic, m_btnDeleteAlbum;

protected:
	TUserWebPictureInfo  m_myPictures;
	bool				m_bPictureTitleModified;
	bool				m_bPictureViewModified;
	bool				m_bUpdatingMyPictures;

	bool				m_bWaittingFlag;
	bool				m_bModified ;
	bool				m_bMultiSel ;
	vector<DWORD>		m_vecMulSelPics;//vector <picture id>
	int					m_nDownloadThreadRef;

	FIFO<TWebPictureInfo *> m_fifoPics;

	CFormProgress3  *   m_pFormProgress3;
	CFormProgress2  *   m_pFormProgress2;

	struct tagDelPicInfo
	{
		CString        strPicIDs;
		CMainDlg *pDlg;
	};

	static UINT DeletePicProc(LPVOID param);
	static UINT LoadAlbumListProc(LPVOID param);

	HANDLE m_hDeletePicProc, m_hLoadAlbumListProc;

	CFont	   m_fontText;

};
