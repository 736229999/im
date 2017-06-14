// ----------------------------------------------------------------
// �ļ�:ClusterIconCtrl.h
// �汾: 
// ����: xy
// ����: 2006-6-8
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
// 
// ----------------------------------------------------------------

#pragma once


//------------------------------------------------------
// ���ͼ��ؼ�
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

	HCURSOR			m_hCursor ;	// ���
	CString			m_sUrl;		// ������ҳ����
	CResBitmap		m_bitmap;	// ����ͼ��
	CResBitmap		m_bmpVIP;	// VIP��־
	BOOL			m_bVIP;		// �Ƿ�ΪVIP
	CToolTipCtrl	m_toolTip;	// ��ʾ�ؼ�	

	CString			m_sLastTip;	// ��ʾ��Ϣ

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


