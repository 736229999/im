#pragma once
#include <atlimage.h>

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
#define WM_TABBTNCTRL_SEL_CHANGE (WM_USER+523)		

// CTabBtnCtrl
#include <vector>

class AFX_EXT_CLASS CTabBtnCtrl : public CStatic
{
	DECLARE_DYNAMIC(CTabBtnCtrl)

public:
	CTabBtnCtrl();
	virtual ~CTabBtnCtrl();
	// �����Ŀ(����bmpͼƬ��pngͼƬ��֧��)
	int AddItem(CString strItemText, CString strImgPath = "", BOOL bRraw = FALSE);
	// ������ɫ
	void SetBackgroundColor(COLORREF clrBackgroundNormal, COLORREF clrBackgroundSel, COLORREF clrBackgroundMouseMove)
	{
		m_clrBackgroundNormal = clrBackgroundNormal;
		m_clrBackgroundSel = clrBackgroundSel;
		m_clrBackgroundMouseMove = clrBackgroundMouseMove;
	}
	// �����ı���ɫ
	void SetItemTextColor(COLORREF clrTextNormal, COLORREF clrTextSel, COLORREF clrTextMouseMove)
	{
		m_clrTextNormal = clrTextNormal;
		m_clrTextSel = clrTextSel;
		m_clrTextMouseMove = clrTextMouseMove;
	}

	// ɾ��������Ŀ
	void RemoveAllItems();

	void SetCurSel(int nSel);
	int GetCurSel();

	// ��ȡ�ı�����
	CString GetItemText(int nIndex);

	// ����ITEM�ĸ߶ȣ�ֻ����������ʱ����Ч
	void SetItemHeight(int nItemHeight);

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

	// ����ÿ��item���������
	void CalcCoord();

private:
	// ��Ŀ����
	struct STRU_ITEMPRO
	{
		CString strText;			// �ı�
		CRect rcText;				// �ı�����λ��

		CString strImgPath;			// ͼƬ·��
		CImage img;					// λͼ
		CPoint ptImg;				// λͼ��������

		CRect rcRange;				// ��Ŀ��������

		BOOL bHasUnderline;			// �Ƿ����»���
		CPoint ptBgn;				// �»�����ʼλ��
		CPoint ptEnd;				// �»��߽���λ��

		STRU_ITEMPRO()
		{
			strText = "";
		}
	};

private:
	CFont m_font;
	int m_nItemHeight;				// ITEM�ĸ߶ȣ�ֻ����������ʱ����Ч
	int m_nItemWidth;				// ��Ŀ���
	COLORREF m_clrBackgroundNormal;	// ����ɫ
	COLORREF m_clrBackgroundSel;	// ѡ��ʱ�ı���ɫ
	COLORREF m_clrBackgroundMouseMove;	// ����ƶ�ʱitem����ɫ

	COLORREF m_clrTextNormal;		// �ı���ɫ
	COLORREF m_clrTextSel;			// ��Ŀѡ��ʱ�ı�����ɫ
	COLORREF m_clrTextMouseMove;	// ����ƶ�����Ŀ���ı�����ɫ	

	std::vector<STRU_ITEMPRO> m_vecItem;
	int m_nCurSel;
	int m_nMoveSel;

	BOOL m_bTracking;
	TRACKMOUSEEVENT m_TMS; 
	int m_nPreSel;
};


