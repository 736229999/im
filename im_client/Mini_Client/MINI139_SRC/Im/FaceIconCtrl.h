#pragma once

#include "impublic.h"
// CFaceIconCtrl

class CFaceIconCtrl : public CStatic
{
	DECLARE_DYNAMIC(CFaceIconCtrl)

public:
	CFaceIconCtrl();
	virtual ~CFaceIconCtrl();

//	void SetFace(int face, int status=0);
	void SetSex(int sex, int status=0);
	void SetFaceFile(CString strFile,int status);
	void SetUserInfo(const FRIEND_INFO *pUser);
	void SetCursor(HCURSOR hCursor) { m_hCursor = hCursor ; }//
	//	 SetClassLong(m_hWnd,    // window handle 
	//	 GCL_HCURSOR,		// change cursor 
	//	(LONG)m_hCursor);	// new cursor 
	//}

	void SetToolTip(const CString& sTip)
	{ m_sTip = sTip; };

	enum TFaceType
	{
		fDefault,//系统自定义
		fUser,//用户定义
	};

protected:
	void ShowToolTip(const CString& strTip);
	virtual void PreSubclassWindow();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

private:
	int		m_nStatus ;
	int		m_nSex	  ;

	int		m_nUserId;

	CBitmap * m_bitmap;


	bool m_bMouseOver ;
	bool m_bMouseDown ;

	TFaceType  m_faceType ;

	HCURSOR  m_hCursor ;

	CToolTipCtrl	m_toolTip;		// 提示控件	
	CString			m_sTip;			// 提示信息
	CString			m_sLastTip;		// 上次的提示信息
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
//	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};


