#pragma once

#ifdef _IM_UNIT_TEST

#include "../Resource.h"
#include <memory>
#include <thread>
#include <chrono>
#include <vector>
#include <cstdint>
#include "boost/asio.hpp"
#include "boost/noncopyable.hpp"
#include "afxwin.h"
#include "afxcmn.h"

// CDialogTest dialog

class CTestDialog : public CDialogEx
{
	DECLARE_DYNAMIC(CTestDialog)

public:
	CTestDialog(CWnd* pParent = NULL, bool auto_test = false);   // standard constructor
	virtual ~CTestDialog();

// Dialog Data
	enum { IDD = IDD_DIALOG_TEST };

	enum : UINT_PTR 
	{ 
		UPDATE_TIMER = 1000,
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonBegin();
	afx_msg void OnBnClickedButtonEnd();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonConfig();

	LRESULT FinishTesting(WPARAM w, LPARAM l);
	LRESULT SendCount(WPARAM w, LPARAM l);

private:
	void		AutoTest();
	void		Cleanup();
	void		TestThread(); 
	void		EnableItems();
	void		DisableItems();

	void		UpdateShowTime();

private:
	CListCtrl						send_cnt_list_;

	int32_t							message_length_;
	int32_t							message_interval_;

	bool							auto_test_;
public:
	afx_msg void OnClose();
};

#endif