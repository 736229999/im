#pragma once

class CRountFaceCtrl : public CStatic
{
	DECLARE_DYNAMIC(CRountFaceCtrl)

public:
	CRountFaceCtrl(UINT idRountNormal, UINT idRoundSel, LPCTSTR szFacePath = NULL);
	virtual ~CRountFaceCtrl();
	// 创建
	BOOL Create(CWnd *pParent, UINT id);
	// 设置图片
	void SetPicPath(LPCTSTR szPicPath, BOOL bOnline = TRUE);
	void SetBitmap(CBitmap* pBitmap, CString strFilePath);
protected:
	virtual void PreSubclassWindow();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	BOOL CRountFaceCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);

	DECLARE_MESSAGE_MAP()

private:
	// 转换为灰度图
	void TurnGray();

private:
	CImage m_imgRoundNormal;
	CImage m_imgRoundSel;
	CImage m_imgFace;
	BOOL m_bSel;

	BOOL m_bTracking;
	TRACKMOUSEEVENT m_TMS; 
	int m_nSize;			// 显示图片大小

	CBitmap m_bitmap;
	BOOL m_bSaveBackground;
	HCURSOR  m_hCursor ;
	BOOL m_bHintFace;//提示消息用的头像
	CString m_strOldHeadFile;
public:
	void SetBitmapHint(BOOL bStatic);//设置为提示消息的头像
	void SetSaveBackground(BOOL bSaveBackground);//设置是否需要截取子窗口所掩盖的父窗口区域，刷新背景
};


