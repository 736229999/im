#pragma once

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

// CCloseTabCtrl
#include <vector>

// ��Ϣ
// itemѡ����ı���¼�����һ������Ϊthisָ�룬�ڶ���������λ�ֽ�Ϊԭ��ѡ�����λ�ֽ�Ϊ��ѡ����
#define WM_CLOSETABCTRL_SEL_CHANGE (WM_USER+423)		
// ���˹رհ�ť�򸸴��ڷ���WM_CLOSETABCTRL��Ϣ
#define WM_CLOSETABCTRL_CLOSE (WM_USER+424)

class AFX_EXT_CLASS CCloseTabCtrl : public CStatic
{
	DECLARE_DYNAMIC(CCloseTabCtrl)

public:
	CCloseTabCtrl();
	virtual ~CCloseTabCtrl();
	// �����Ŀ
	int AddItem(CString strItemText);
	// ������ɫ
	void SetBackgroundColor(COLORREF clrBackgound);
	void SetCurSel(int nSel);
	int GetCurSel();

	// ���ù�������
	BOOL SetItemData(int nIndex, DWORD dwData);
	// ��ȡ��������
	BOOL GetItemData(int nIndex, DWORD &dwData);

	// ���ñ߿���ʽ
	void SetFrameModol(BYTE nModol)
	{
		m_nModol = nModol;
	}

protected:
	DECLARE_MESSAGE_MAP()

	BOOL OnEraseBkgnd(CDC* pDC);
	void OnPaint();	
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual void PreSubclassWindow();

	// ����ÿ��item���������
	void CalcCoord();

private:
	// ��Ŀ��Ϣ
	struct STRU_ITEM
	{
		CString strText;	// �ı�
		int nWidth;			// �ı����
		int nHeight;		// �ı��߶�

		CRect rcItem;
		CRect rcCloseImg;	// �رհ�ť��λ��	

		DWORD dwData;		// ��������	
	};

private:
	CFont m_font;
	COLORREF m_clrBackground;		// ����ɫ
	COLORREF m_clrTextNormal;		// �ı���ɫ
	std::vector<STRU_ITEM> m_vecItem;

	int m_nCurSel;

	BOOL m_bTracking;
	int m_nPreSel;
	BYTE m_nModol;				// �߿���ʽ
};


