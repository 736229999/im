#pragma once

#include "IMTabWndCtrl.h"
#include "floorpagefriend.h"
#include "floorpagecluster.h"
#include "floorpageaddbook.h"
#include "floorpagerecent.h"
#include "improtocol.h"
#include "..\controls\Floor_Wnd_Ctrl\xfloorwndctl.h"

// CTabPage1 �Ի���

class CTabPage1 : public CDialog
{
	DECLARE_DYNAMIC(CTabPage1)
	DECLARE_HMG_SCROLLBAR()

public:
	CTabPage1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabPage1();

// �Ի�������
	enum { IDD = IDD_TAB_PAGE1 };

public:
//	CXFloorWndCtrl     m_wndFloorCtrl;
	CIMTabWndCtrl      m_wndIMTabCtrl;
	CFloorPageFriend   m_pageFriend;
	CFloorPageCluster  m_pageCluster;
	CFloorPageRecent   m_pageRecent;
	CFloorPageAddbook  m_pageAddbook;			//ͨѶ¼

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnOK() {}
	virtual void OnCancel() {}
	afx_msg void OnSize(UINT nType, int cx, int cy);	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcDestroy();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void OnLoginAck(const char *data,WORD wRet);

	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

};
