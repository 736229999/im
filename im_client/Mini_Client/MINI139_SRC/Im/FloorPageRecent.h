#pragma once

#include "customlistctrl.h"
// CFloorPageRecent �Ի���

class CFloorPageRecent : public CDialog
{
	DECLARE_DYNAMIC(CFloorPageRecent)

public:
	CFloorPageRecent(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFloorPageRecent();

// �Ի�������
	enum { IDD = IDD_FLOOR_RECENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK() {}
	virtual void OnCancel() {}
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonClear();
	DECLARE_MESSAGE_MAP()

protected:
//	CFlatButton  m_btnClear;
	CRecentListCtrl    m_wndRecentList;

};
