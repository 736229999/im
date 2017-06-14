#pragma once

#include "PubHeader.h"
#include "shelltree.h"
#include "previewdlg.h"
#include "ThumBnaiListCtrl.h"
#include "ThumBnaiUpListCtrl.h"

#include "XSkinDlg.h"
#include "formprogress.h"

#include "controls\SplitterControl.h"
#include "controls\button.h"
#include "controls\FlatEdit.h"
#include "controls\GraphComboBox.h"

#include "ProgressCtrlEx.h"

#include "_HyperLink/StatLink.h"
#include "controls/StaticCtrlEx.h"

#include "UploadOperationsDlg.h"
#include "PanelBaseDlg.h"

#include "SyncCtrl.h"
#include "..\Resource.h"

#if 1
	#define _USE_XSKING_DLG
#endif

#ifndef _USE_XSKING_DLG
#define CXSkinDlg CDialog
#endif

#include "PubHeader.h"
#include "..\configinfo.h"

class CUpLoadPhotoDlg : public CXSkinDlg
{
	class _auto_enable_ui_
	{
	public:
		_auto_enable_ui_(CUpLoadPhotoDlg *pThis)	{m_pDlg = pThis; m_pDlg ->EnableDlgUI(FALSE);}
		~_auto_enable_ui_(){if(m_pDlg != NULL) m_pDlg ->EnableDlgUI(TRUE);}
	private:
		CUpLoadPhotoDlg *m_pDlg;
	};
	DECLARE_DYNAMIC(CUpLoadPhotoDlg)

public:
	CUpLoadPhotoDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUpLoadPhotoDlg();
	//放大预览图片窗口。
// 对话框数据
	enum { IDD = IDD_UPLOADPHOTO_DIALOG };

public:
	void SetParamInfo(const CString & strParam);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnHelpClick();

private:
	CStatic m_wndSplitterLeft,	m_wndSplitterRight;
	
	CBrush m_br;
	//树型目录控件
	CShellTree m_wndDirTree;
	// 选择的目录
	CString m_strSelectDir;
	// 本地的图片文件列表
	CThumBnaiListCtrl m_wndFileListCtrl;
	// 待上传的文件列表显示
	CThumBnaiUpListCtrl m_wndUpLoadFileListCtrl;
	
	CStatic m_strDescription;
private:
	CString version;
	bool m_bSetSize;
public:
	CUploadOperationsDlg m_wndUploadOperationsDlg;
private:
	CStatic m_wndGroupBox1, m_wndGroupBox3;
	CStatic m_wndGroupBox2;
	// 实现
protected:
	HICON m_hIcon;
	CImageList		m_ImageList;

	CFormProgress3	*   m_pwndProgressBar  ;

	//生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	LRESULT OnLoadFinished(WPARAM wParam,LPARAM lParam);
	LRESULT OnCancelUpload(WPARAM wParam,LPARAM lParam);
	LRESULT OnThreadNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnPreview(WPARAM wParam, LPARAM lParam);
	LRESULT OnDestroyPreview(WPARAM wParam, LPARAM lParam);

	void GetRepaintRgn(CRgn &rgn);
public:
	//上传文件操作

	afx_msg void OnTvnSelchangedDirTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnDeleteitemDirTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemexpandingDirTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedAdduploadButton();
	afx_msg void OnBnClickedAddalluploadbutton();
	afx_msg void OnBnClickedCancelButton();
	afx_msg void OnBnClickedUploadfileButton();
	afx_msg void OnBnClickedRemoveuploadfileButton();
	afx_msg void OnBnClickedRemovealluploadfilesButton();
	afx_msg void OnNMDblclkFileList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCbnSelchangeTagCombo();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnAlbum();

	void RemoveSelFile(FILEINFO *pInfo);
	void AddSelFile(FILEINFO *pInfo);
	void AddUploadedFile(FILEINFO *pInfo);
	BYTE GetFileFlag(FILEINFO *pInfo);
	bool CheckFileSize(const CString & strFileName, ULONGLONG ulFileSize);
	void ShowFileMessage();
	void RemoveUploadedFile();
public:
	int	ReplaceSelFile(const CString & strOldFilePathName, const CString & strNewFilePathName);
public:
	//CFlatEdit m_FileDsp;
//	CFlatEdit m_EditWindow;		// Not use
	
	CXpButton m_btnMyAlbum;
		
	CPanelDlg1 m_wndPanelDlg1;
	CPanelDlg2 m_wndPanelDlg2;
	
	CStaticCtrlEx m_wndStaticTreeCtrlPrompt;
	CStaticCtrlEx m_wndStaticListCtrlPrompt;
protected:
	virtual void OnCancel();
	virtual void OnOK() {}
	
	void GetLastUploadPicFolder(CString & strFolder);
	void SaveLastUploadPicFolder(const CString & strFolder);
protected:
	
	static UINT InitProc(LPVOID pParam);
	void DoInit();
	
	void    StartUpload();

public:
	// save full file path
    CStringList     m_strUploadFilelst;
	_lstUpFileINFO	m_lstUpLoadFleINFO;
protected:
	void ResizePostion();
	void OnBtnHyperLink();
private:

//	CString             m_strError         ;
	HANDLE				m_hThreadUpload    ;
	HANDLE				m_hThreadInit      ;
	// 为上传文件的一些属性

	
	map<CString, BOOL>	m_mapSelFile;
	CSyncCriticalSection m_SyncMapSelFile;

	map<CString, BOOL>  m_mapUpdFile;

public:
	afx_msg void OnFileListCtrlBeginDrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnUploadListCtrlBeginDrag(NMHDR *pNMHDR, LRESULT *pResult);
private:
	CListCtrl* m_pDragList;		//Which ListCtrl we are dragging FROM
	CListCtrl* m_pDropList;		//Which ListCtrl we are dropping ON
	CImageList* m_pDragImage;	//For creating and managing the drag-image
	BOOL		m_bDragging;	//T during a drag operation
	int			m_nDragIndex;	//Index of selected item in the List we are dragging FROM
	int			m_nDropIndex;	//Index at which to drop item in the List we are dropping ON
	CWnd*		m_pDropWnd;		//Pointer to window we are dropping on (will be cast to CListCtrl* type)
private:	
	int StartDragDrop(NM_LISTVIEW* pNMListView, CThumBnaiListCtrl & ListCtrl);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
private:
	void DropItemOnList(CListCtrl* pDragList, CListCtrl* pDropList);
public:
	afx_msg void OnUploadListCtrlBeginLabelEdit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnUploadListCtrlEndLabelEdit(NMHDR *pNMHDR, LRESULT *pResult);
protected:

	BOOL IsNotExistInUploadFileLst(const CString & strFilePathName);
private:
	UINT m_nTimerID;
	HCURSOR m_hHyperLinkCursor;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
private:
	_CURSOR_TYPE m_CursorType;
	HCURSOR m_hNoDragCursor, m_hHandleCursor;
private:
	int get_max_side_size();

	void OnBtnResetInfo(void);
	afx_msg void OnNMDblclkUploadfileList(NMHDR *pNMHDR, LRESULT *pResult);
	int GetSpaceFromServer(void);
	int GetAlbumListFromServer(void);
	int GetTagListFromServer(void);
	int CreateNewAlbum(const CString & strAlbumName);
private:
	void SetCurPos(const float nPos);
	void SetCurPos2(const float nPos);
private:
	BOOL	m_bInitDialog;
	void	LoadFolder(const CString & strFolder);
	CString ParseAlbumName(const CString & strAlbumName);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:	
	void RedrawUploadFileListPics(const int nMaxSideSize);
public:
	BOOL	m_bLoadFolder;
private:
	BOOL	m_bValidateSucceed;
	afx_msg void OnClose();
private:
	BOOL	m_bStopUploadThread;
public:
	void	EnableDlgUI(const BOOL bEnable = TRUE);
	void	MiniOpenUrl(const CString & strUrl);
	void	ShowAlbum(LPCSTR lpszUserName, int nAlbumID = 0);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
private:
	CString m_strMiniServerAddr;
	CString m_strIMServiceAddr;
public:
	void OpenHtmlPage(const CString & strUrl);
	afx_msg LRESULT BringToTop(WPARAM wParam, LPARAM lParam);
//	int AddUploadedFile(CStringList & strFileLst);
};



