#pragma once
#include <atlimage.h>
#include "ScrollBarEx.h"

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
#define WM_COLUMNCTRL_SEL_CHANGE (WM_USER+521)		

// CColumnCtrl
#include <vector>

class AFX_EXT_CLASS CColumnCtrl : public CStatic
{
	DECLARE_DYNAMIC(CColumnCtrl)

public:
	// ��Ŀ���з�ʽ
	enum ENDIRE
	{
		EN_X_DIRE = 0,		// ����	
		EN_Y_DIRE			// ����	
	};

	CColumnCtrl();
	virtual ~CColumnCtrl();
	// �������з�ʽ
	void SetDirectMode(ENDIRE enDireMode);
	// �����Ŀ(����bmpͼƬ��pngͼƬ��֧��)
	int AddItem(CString strItemText, CString strImgPath = "", BOOL bRraw = FALSE);
	// ��������
	void SetFont(CFont *pfont);
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

	// �Ƿ�����»���
	void DrawUnderline(BOOL bDraw)
	{
		m_bDrawUnderline = bDraw;
	}

	// ɾ��������Ŀ
	void RemoveAllItems();

	void SetCurSel(int nSel);
	int GetCurSel();

	// ���ñ߿���ʽ
	void SetFrameModol(BYTE nModol)
	{
		m_nModol = nModol;
	}

	// ��ȡ�ı�����
	CString GetItemText(int nIndex);

	// ����ITEM�ĸ߶ȣ�ֻ����������ʱ����Ч
	void SetItemHeight(int nItemHeight);

	// ����ITEM�Ŀ�ȣ�ֻ�ں�������ʱ����Ч
	void SetItemWidth(int nItemWidth);

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
	afx_msg LRESULT OnScrollBarMsg(WPARAM wParam, LPARAM lParam);

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
			strImgPath = "";
			bHasUnderline = FALSE;
		}
	};

private:
	CFont *m_pfont;
	ENDIRE m_enDire;				// ��Ŀ���з�ʽ��Ĭ�Ϻ���
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

	BOOL m_bDrawUnderline;			// item��ѡ��ʱ���ı��·��Ƿ�����»���
	BYTE m_nModol;					// �߿���ʽ

	CScrollBarEx m_vScroll;			// ��ֱ��������ֻ�õ��б���������ʱ�ų���
	CRect rt_mvScroll;				// ��ֱ��������������

	double m_dPrecent;				// �����������İٷְ�
	int m_nScrollHeight;			// �����������ĸ߶�
};


