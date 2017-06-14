#pragma once

class CRountFaceCtrl : public CStatic
{
	DECLARE_DYNAMIC(CRountFaceCtrl)

public:
	CRountFaceCtrl(UINT idRountNormal, UINT idRoundSel, LPCTSTR szFacePath = NULL);
	virtual ~CRountFaceCtrl();
	// ����
	BOOL Create(CWnd *pParent, UINT id);
	// ����ͼƬ
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
	// ת��Ϊ�Ҷ�ͼ
	void TurnGray();

private:
	CImage m_imgRoundNormal;
	CImage m_imgRoundSel;
	CImage m_imgFace;
	BOOL m_bSel;

	BOOL m_bTracking;
	TRACKMOUSEEVENT m_TMS; 
	int m_nSize;			// ��ʾͼƬ��С

	CBitmap m_bitmap;
	BOOL m_bSaveBackground;
	HCURSOR  m_hCursor ;
	BOOL m_bHintFace;//��ʾ��Ϣ�õ�ͷ��
	CString m_strOldHeadFile;
public:
	void SetBitmapHint(BOOL bStatic);//����Ϊ��ʾ��Ϣ��ͷ��
	void SetSaveBackground(BOOL bSaveBackground);//�����Ƿ���Ҫ��ȡ�Ӵ������ڸǵĸ���������ˢ�±���
};


