#pragma once
#include <vector>

#define SEO_SELCOLOR  RGB(150,233,180)
#define SEO_NORMALCOLOR RGB(255,255,255)
#define SEO_REGISTEDCOLOR RGB(255,233,222)
#define SEO_FOCUSCOLOR RGB(188,188,100)

#define SEO_CMD_REGIST  1
#define SEO_CMD_SELCHANGE 2

#define WM_SEOLIST_NOTIFY WM_USER+1

#include <vector>
using namespace std;

class CSEOListCtrl: public CWnd
{
	DECLARE_DYNAMIC(CSEOListCtrl)

public:
	CSEOListCtrl();
	virtual ~CSEOListCtrl();

	struct tagItem
	{
		CString strTitle;
		CString strDate;

		DWORD   dwData;
		CRect   rcItem;

		tagItem()
		{
			dwData = 0;
		}
	};

public:
	BOOL Create(CWnd *pParentWnd,const CRect &rect, DWORD dwStyle=WS_VISIBLE|WS_CHILDWINDOW|WS_CLIPCHILDREN,UINT nID=0);

	void AddItem(CString strRegistName,CString strDate, DWORD dwData=0);
	DWORD GetItemData(int nItem);
	void  SetCurSel(int nSel);
	void  SetFocusItem(int nItem);
	int   GetCurSel();
	int   GetItemCount();
	void  RemoveAllItems();
	void  DeleteItem(int nCurSel);

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

protected:
	void DrawPtInRect(const CPoint &ptCursor,const CRect & rcRect,CDC *pDC,int nStyle, bool bMouseDown);
	void DrawItem(int nItem,tagItem &item,CDC *pDC, CRect rcDraw);
protected:
	void Draw(CDC *pDC, CRect rcDraw);
	void RecalcLayout();
	void CalcScrollInfo();
	
	void ShowItemTip(int nItem);
	void ShowToolTip(CString strTip);
	virtual void InitToolTip();

	int HitTest(CPoint pt);

protected:
	CToolTipCtrl m_ToolTip;	
	CString      m_strPreTip ;
	bool		 m_bMouseDown;
	vector<tagItem>  m_vecItems;
	int              m_nCurSel;
	int				 m_nFocusItem;
	int				 m_nFirstVisibleItem;
	int				 m_nLastVisibleItem;
	int				 m_nHotItem;
	long             m_nClientRealHeigh;
	long             m_nScrollStep;
	double			 m_fScrollPercent;
	long			 m_OriginY;
	HCURSOR          m_hCursor;

	CFont            m_Font,m_Font1;

public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
