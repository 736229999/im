#pragma once

#include "controls\SplitterControl.h"
#include "controls\button.h"
#include "controls\FlatEdit.h"
#include "controls\GraphComboBox.h"

#include "ProgressCtrlEx.h"

#include "_HyperLink/StatLink.h"
#include "controls/StaticCtrlEx.h"


#include "XSkinDlg.h"

//	#define CDialog CXSkinDlg

class CUpLoadPhotoDlg;

// CPanelBaseDlg dialog

class CPanelBaseDlg : public CDialog
{
	DECLARE_DYNAMIC(CPanelBaseDlg)

public:
	CPanelBaseDlg(UINT IDD, CWnd* pParent = NULL);   // standard constructor
	virtual ~CPanelBaseDlg();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	CBrush m_br;
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void GetRepaintRgn(CRgn &rgn);
public:
	void RemoveRgnOfWnd(CRgn &rgn, CWnd *pWnd,int iOffset = 0);
protected:
	void OnKickIdle();
};


class CPanelDlg1 : public CPanelBaseDlg
{
	DECLARE_DYNAMIC(CPanelDlg1)

public:
	CPanelDlg1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPanelDlg1();

	// Dialog Data
	enum { IDD = IDD_DLG_PANEL1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CGraphComboBox m_wndComboAlbum;	// Œ“µƒœ‡≤·

	CGraphComboBox	/* m_FilePopCombo; */
		m_wndComboACL, m_wndComboTAG, m_wndComboLimitSize;
	
	CFlatEdit m_wndEditTag, m_wndEditAlbum;
public:
	afx_msg void OnCbnKillfocusDlgUploadAlbumCombo();
	afx_msg void OnCbnSetfocusDlgUploadAlbumCombo();
	afx_msg void OnCbnKillfocusDlgUploadTagCombo();
	afx_msg void OnCbnSetfocusDlgUploadTagCombo();
void GetRepaintRgn(CRgn &rgn);
public:
	afx_msg void OnCbnSelendokDlgUploadImageSize();

	CUpLoadPhotoDlg *m_pParentWnd;
public:
	BOOL	m_bEnableAlumTips;
protected:

};

class CPanelDlg2 : public CPanelBaseDlg
{
	DECLARE_DYNAMIC(CPanelDlg2)

public:
	CPanelDlg2(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPanelDlg2();

	// Dialog Data
	enum { IDD = IDD_DLG_PANEL2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CXpButton   m_BeginUpLoadButton;	
	CXpButton	m_wndResetInfo;
	CBmpBtn		m_wndBmpBtnINFO;
	CStaticLink m_wndStaticHyperLink;

	void GetRepaintRgn(CRgn &rgn);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


class CUploadOperationsDlg : public CPanelBaseDlg
{
	DECLARE_DYNAMIC(CUploadOperationsDlg)

public:
	CUploadOperationsDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUploadOperationsDlg();

	// Dialog Data
	enum { IDD = IDD_DLG_OPERATIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
public:
	CXpButton m_AddButton;
	CXpButton m_AddAllButton;
	CXpButton m_RemoveButton;
	CXpButton m_RemoveAllButton;
private:
	CBrush m_br;
	CString m_strSelectFileINFO;

	CStaticCtrlEx m_wndStaticPrompt1, m_wndStaticPrompt2, m_wndStaticPromptSelectInfo,
		m_wndStaticPromptSelectInfo2;
private:
	CString m_strText1, m_strText2;
public:
	CProgressCtrlEx	m_wndProgressCtrl;
	void SetSelectFileINFO(const CString & strText, const CString strText1 = "");
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
private:
	void OnBtnRemoveAll();
	void OnBtnRemove();
	void OnBtnAddAll();
	void OnBtnAdd();
public:
	CUpLoadPhotoDlg *m_pParent;
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	void OnDlgUpdateUI(CCmdUI *pCmdUI);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnDestroy();
public:
	void GetRepaintRgn(CRgn &rgn);
};


extern BOOL g_bEnableContinueOperation;