#pragma once

#include "controls\button.h"
#include "controls\skinctrl.h"
#include "controls\pictureex.h"

class CFormProgress : public CDialog
{
	DECLARE_DYNAMIC(CFormProgress)

public:
	CFormProgress(HWND hNotifyWnd ,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFormProgress();

	HWND   m_hNotifyWnd ;
	
// 对话框数据
	enum { IDD = IDD_DIALOG_PROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnCancel() {}
	virtual void OnOK() {}
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedCancel();


	DECLARE_MESSAGE_MAP()

private:
	CPictureEx     m_wndProgress;
	CFlatButton    m_btnCancel;

public:
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
};

class CFormProgress2 : public CDialog
{
	DECLARE_DYNAMIC(CFormProgress2)

public:
	CFormProgress2(CString strTip, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFormProgress2();

	
// 对话框数据
	enum { IDD = IDD_DIALOG_PROGRESS2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnCancel() {}
	virtual void OnOK() {}
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedCancel();


	DECLARE_MESSAGE_MAP()

private:
	CPictureEx        m_wndProgress  ;
	CString           m_strTip;

public:
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
};

class CFormProgress3 : public CDialog
{
	DECLARE_DYNAMIC(CFormProgress3)

public:
	CFormProgress3(HWND hNotifyWnd ,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFormProgress3();

	HWND   m_hNotifyWnd ;

	void   SetTip(CString str) { m_lblTip.SetWindowText(str,""); }
	
// 对话框数据
	enum { IDD = IDD_DIALOG_PROGRESS3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnCancel() {}
	virtual void OnOK() {}
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedCancel();


	DECLARE_MESSAGE_MAP()

private:
	CPictureEx          m_wndProgress ;
	CFlatButton			m_btnCancel   ;
	CTransparentLabel   m_lblTip  ;

public:
	afx_msg void OnDestroy();
	virtual BOOL OnInitDialog();
};
