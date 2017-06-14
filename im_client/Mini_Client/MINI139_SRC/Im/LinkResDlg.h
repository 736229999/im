// ----------------------------------------------------------------
// �ļ�:LinkResDlg.h
// �汾: 
// ����: xy
// ����: 2006-7-7
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
// 
// ----------------------------------------------------------------

#pragma once

#include "..\resource.h"
#include "afxwin.h"
#include "bkgFrame.h"
#include "resgdiobj.h"

class CLinkResDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CLinkResDlg)

protected:

	CLinkResDlg(CWnd* pMsg,CWnd* pParent = NULL);   // ��׼���캯��

public:

	virtual ~CLinkResDlg();

	enum { IDD = IDD_LINKRES };

public:

	static CLinkResDlg*	NewDlg(CWnd* pMsg,CWnd* pParent = NULL);

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

public:

	afx_msg void OnBnClickedOk();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL OnInitDialog();
	afx_msg void OnNcDestroy();
	afx_msg void OnPaint();

private:

	CWnd*			m_pWndMsg;
	CResBitmap		m_bmpLinkRes;
	HBRUSH			m_hBrushWhite;
	CBkgFrame		m_wndFrame;
	CXpButton		m_btnOk;
	CXpButton		m_btnCancel;
	CFlatEdit		m_edtResUrl;

public:

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
};
