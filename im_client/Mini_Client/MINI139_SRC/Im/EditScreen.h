/*====================================================================  
=  �ļ���:CEditScreen��                                    
=  �޸�����:2015-5-5                                            
=  ����:wyx                  
=  ����:���ڽ����������ֱ༭
====================================================================*/ 
#pragma once

// CEditScreen
#define WM_EDIT_PIC_SAVE WM_USER+1
#define WM_DRAW_RECTTRACKER WM_USER+2
class CEditScreen : public CEdit
{
	DECLARE_DYNAMIC(CEditScreen)

public:
	CEditScreen();
	virtual ~CEditScreen();

protected:
	//{{AFX_MSG(CEditTrans)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg	void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChange();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	BOOL m_bIsPaint;//�ж��Ƿ���Ҫ���ƣ����ֵ������½ǵı�Ե���޷���������
	CBitmap* m_bitmapLast;//���Ƶı���ͼƬ
	BOOL m_bIsDrawRectangle;//�ж��Ƿ�������߱߿�
	BOOL m_bIsSavePicture;//�ж��Ƿ���Ҫ����ͼƬ
public:
	void SetParentRect(CRect rcParent);//���ý�������

private:
	DWORD m_dwTextColor;//������ɫ
	CBrush m_brush;//͸����ˢ
	CRect m_rcParent;//�����ڽ�������
	CRect m_rect;//�߿��������ڶ�̬�������߱߿�
};


