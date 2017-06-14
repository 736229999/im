#pragma once

#include "XSkinDlg.h"
// CUserGuideDlg 对话框
#include "resgdiobj.h"

class CUserGuideDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CUserGuideDlg)

public:
	CUserGuideDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserGuideDlg();

	static void Show();
	static void Close();

// 对话框数据
	enum { IDD = IDD_DIALOG_USERGUIDE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnOK() { DestroyWindow();	}
	virtual void OnCancel() { DestroyWindow();	}
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnStnClickedStatic1();
	afx_msg void OnStnClickedStatic2();
	afx_msg void OnStnClickedStatic3();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

protected:
	CHyperBtn m_btn1;
	CHyperBtn m_btn2;
	CHyperBtn m_btn3;
	
	static CUserGuideDlg * pInstance;

	CResBitmap   m_bmpDlg;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
