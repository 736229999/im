#pragma once

#include "sabasepage.h"
#include "afxcmn.h"

// CSAPageHotkey 对话框

class CSAPageHotkey : public CSABasePage
{
	DECLARE_DYNAMIC(CSAPageHotkey)

public:
	CSAPageHotkey(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSAPageHotkey();

// 对话框数据
	enum { IDD = IDD_SAPAGE_HOTKEY };

	void	SaveConfig();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CHotKeyCtrl m_hotkey;
//	afx_msg void OnBnSetfocusHotkeyCustom();
//	afx_msg void OnBnSetfocusHotkeyDefault();
	afx_msg void OnBnClickedHotkeyCustom();
	afx_msg void OnBnClickedHotkeyDefault();
	afx_msg void OnBnClickedEnter();
};
