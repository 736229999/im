#pragma once
#include "stdafx.h"
#include "resource.h"

// CButtonEx
#define  WM_BN_CLICK                WM_USER+0x1113

class CButtonEx : public CButton
{
	DECLARE_DYNAMIC(CButtonEx)

public:
	CButtonEx();
	CButtonEx( int nItem, int nSubItem, CRect rect, HWND hParent );
	virtual ~CButtonEx();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClicked();
	int m_inItem;
	int m_inSubItem;
	CRect m_rect;
	HWND m_hParent;
	BOOL bEnable;
};


