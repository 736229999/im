// ----------------------------------------------------------------
// 文件:ClusterIconCtrl.h
// 版本: 
// 作者: xy
// 日期: 2006-6-8
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once


//------------------------------------------------------
// 族徽图标控件
//------------------------------------------------------
#include "resgdiobj.h"

class CClusterIconCtrl : public CStatic
{
	DECLARE_DYNAMIC(CClusterIconCtrl)

public:

	CClusterIconCtrl();

	void		SetUrl(const CString& sUrl)
	{ m_sUrl = sUrl; };

	void		SetVIP(BOOL b)
	{ m_bVIP = b; };

	virtual ~CClusterIconCtrl();

	void		ShowToolTip(const CString& strTip);

protected:

	HCURSOR			m_hCursor ;	// 鼠标
	CString			m_sUrl;		// 家族网页链接
	CResBitmap		m_bitmap;	// 家族图标
	CResBitmap		m_bmpVIP;	// VIP标志
	BOOL			m_bVIP;		// 是否为VIP
	CToolTipCtrl	m_toolTip;	// 提示控件	

	CString			m_sLastTip;	// 提示信息

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};


