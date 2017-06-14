#pragma once

// CMenuCtrl
#include <vector>

// �ؼ��߿����
#ifndef FRAME_DRAW_NULL
#define FRAME_DRAW_NULL 0x00
#endif
#ifndef FRAME_DRAW_LEFT
#define FRAME_DRAW_LEFT 0x01
#endif
#ifndef FRAME_DRAW_RIGHT
#define FRAME_DRAW_RIGHT 0x02
#endif
#ifndef FRAME_DRAW_TOP
#define FRAME_DRAW_TOP 0x04
#endif
#ifndef FRAME_DRAW_BOTTOM
#define FRAME_DRAW_BOTTOM 0x08
#endif
#ifndef FRAME_DRAW_ALL
#define FRAME_DRAW_ALL 0x0F
#endif

// ��Ϣ
// itemѡ����ı���¼�����һ������Ϊthisָ�룬�ڶ���������λ�ֽ�Ϊԭ��ѡ�����λ�ֽ�Ϊ��ѡ����
#define WM_MENUCTRL_SEL_CHANGE (WM_USER+421)		

class AFX_EXT_CLASS CMenuCtrl : public CStatic
{
	DECLARE_DYNAMIC(CMenuCtrl)

public:
	CMenuCtrl();
	virtual ~CMenuCtrl();
	// ���ñ߿���ʽ
	void SetFrameModol(BYTE nModol);
	// �����Ŀ
	int AddItem(CString strItemText);
	// ��������
	void SetFont(CFont *pfont);
	// ������ɫ
	void SetColor(COLORREF clrBackgound, COLORREF clrTextNormal, COLORREF clrTextSel);
	void SetCurSel(int nSel);
	int GetCurSel();

protected:
	DECLARE_MESSAGE_MAP()

	BOOL OnEraseBkgnd(CDC* pDC);
	void OnPaint();	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	virtual void PreSubclassWindow();

private:
	// ��Ŀ��Ϣ
	struct STRU_ITEM
	{
		CString strText;	// �ı�
		int nWidth;			// �ı����
		int nHeight;		// �ı��߶�

		CRect rcItem;
		CRect rcSelItem;
	};

private:
	CFont *m_pfont;
	COLORREF m_clrBackground;	// ����ɫ
	COLORREF m_clrTextNormal;	// �ı���ɫ
	COLORREF m_clrTextSel;		// �ı�ѡ��ʱ����ɫ
	BYTE m_nModol;				// �߿���ʽ
	std::vector<STRU_ITEM> m_vecItem;

	int m_nCurSel;
	int m_nMoveSel;

	BOOL m_bTracking;
	TRACKMOUSEEVENT m_TMS; 
	int m_nPreSel;
};


