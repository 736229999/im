#pragma once

#include "sabasepage.h"
#include "afxcmn.h"

// CSAPageHotkey �Ի���

class CSAPageHotkey : public CSABasePage
{
	DECLARE_DYNAMIC(CSAPageHotkey)

public:
	CSAPageHotkey(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSAPageHotkey();

// �Ի�������
	enum { IDD = IDD_SAPAGE_HOTKEY };

	void	SaveConfig();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
