#pragma once


// CUploadingDlg 对话框

#include "afxwin.h"
#include "afxcmn.h"

#include "..\configinfo.h"

class CUploadingDlg : public CDialog
{
	DECLARE_DYNAMIC(CUploadingDlg)
public:
	CString m_strWindowText;
	_lstUpFileINFO & m_lstUpLoadFleINFO;
public:
	CUploadingDlg(_lstUpFileINFO & lstUpFileINFO, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUploadingDlg();

// 对话框数据
	enum { IDD = IDD_DLG_UPLOADING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);



	LRESULT OnMsgOfUploadingThread(WPARAM wParam, LPARAM lParam);

protected:
	void	OnUploadingCancel();
	static UINT UploadThreadProc( LPVOID pParam );
	int		UploadFile_run(void);
	BOOL	CompressFileIfNeed(tagUpFileInfo * pFile, const int nMaxSideSize);
	bool	UploadFile(const tagUpFileInfo & _UpFileINFO, CString & strError);		

private:

	HWND	m_hWndCurPicName, m_hWndProgressText;
	HWND	m_hWndProgressCtrl;

	BOOL	m_bStopUploadingThread;
	HICON	m_hIcon;

	_CURSOR_TYPE m_CursorType;

	CString m_strProgressText;
	CString m_strCurPicName;
	CProgressCtrl m_wndProgressCtrl;

};
