#pragma once

#include "faceiconctrl.h"
#include "facewndctrl.h"
#include "FormProgress.h"

#include <atlimage.h>

// CSelfFaceDlg 对话框
class CSelfFaceDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CSelfFaceDlg)
	DECLARE_XT_MESSAGE_MAP()

public:
	CSelfFaceDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelfFaceDlg();

	bool   GetFaceUrl(CString &strUrl);

// 对话框数据
	enum { IDD = IDD_DIALOG_SELFFACE };


	HWND    m_hSAPageWnd;

	static CSelfFaceDlg * Show();
	static CSelfFaceDlg * GetInstance() { return m_pInstance;	}
	static void Close();
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnHelpClick();
	virtual void OnOK() {}
	virtual void OnCancel() {}
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonPic();
	afx_msg void OnBnClickedButtonBighead();
	afx_msg void OnNcDestroy();
	afx_msg void OnClose();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	DECLARE_MESSAGE_MAP()

	BOOL AdjustImgSize(CImage& img,CString& strNewFile);

	LRESULT OnWebIconNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnCancelUpload(WPARAM wParam,LPARAM lParam);
	LRESULT OnThreadNotify(WPARAM wParam, LPARAM lParam);
	void	OnLogOut(const char *data, WORD wRet);

protected:
	CXpButton   m_btnBigHead;
	CXpButton   m_btnUpload;
	CXpButton   m_btnOk;
	CXpButton   m_btnCancel;

	struct {
		CString strFile;
		CSelfFaceDlg *pDlg;

	} m_thParam;

	CFaceWndCtrl	  m_wndFaceSelCtrl;
	CFaceIconCtrl	  m_wndFaceIconCtrl;
	CFormProgress3 *  m_pWndProgress;

protected:
	static CSelfFaceDlg* m_pInstance;
	HANDLE     m_hUploadFace;
	static UINT UploadFaceProc(LPVOID p);
	static UINT LoadWebHeadListProc(LPVOID param);
};
