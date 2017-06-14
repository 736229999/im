#pragma once

#include "faceiconctrl.h"
#include "facewndctrl.h"
#include "FormProgress.h"

#include <atlimage.h>

// CFamilyFaceDlg 对话框
class CFamilyFaceDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CFamilyFaceDlg)
	DECLARE_XT_MESSAGE_MAP()

public:
	CFamilyFaceDlg(uint32 nFamilyID,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFamilyFaceDlg();

	bool   GetFaceInfo(uint16 &face, CString &strUrl);

	// 对话框数据
	enum { IDD = IDD_DIALOG_FAMILYFACE };


	static void CloseWin();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnHelpClick();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonPic();
	DECLARE_MESSAGE_MAP()

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
		CFamilyFaceDlg *pDlg;

	} m_thParam;

	CFaceWndCtrl  m_wndFaceSelCtrl;
	CFaceIconCtrl	  m_wndFaceIconCtrl;

	CFormProgress3 *   m_pWndProgress;

protected:
	static CFamilyFaceDlg* m_pInstance;
	HANDLE     m_hUploadFace;
	static UINT UploadFaceProc(LPVOID p);

	uint32       m_nFamilyID;


protected:
	BOOL AdjustImgSize(CImage& img,CString& strNewFile);
};
