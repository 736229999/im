#pragma once

#define FACE_SZ    40
#define FACE_SPACE 8
#define FACE_DEFAULT 1
#define FACE_HISTORY 2
#define LINE_UP_CAP  20
#define TEXT_CY    18

#define MAX_FACE_SHOW_SIZE 300

CString HistoryFaceUrl2WebUrl(CString strHistoryFaceUrl);


class CFaceWndCtrl : public CWnd
{
	DECLARE_DYNAMIC(CFaceWndCtrl)

public:
	CFaceWndCtrl();
	virtual ~CFaceWndCtrl();

public:
	BOOL Create(CWnd *pParentWnd,const CRect &rect, DWORD dwStyle=WS_VISIBLE|WS_CHILDWINDOW|WS_CLIPCHILDREN,UINT nID=0);
	void InitData();
	void ClearData();
	void GetSel(int &iType, CString &strUrl);
	void SetSel(int iType, CString strUrl);

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();	
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()
	
	LRESULT  OnWebFileNotify(WPARAM wParam, LPARAM lParam);
	LRESULT  OnThreadNotify(WPARAM wParam, LPARAM lParam);

	static  UINT LoadWokuheadProc(LPVOID lpParam);

	void Draw(CDC *pDC);
	void ReDraw() {
		CDC *p=GetDC();
		Draw(p);
		ReleaseDC(p);
	}
	void HitTest(const CPoint &point, int &iFaceType, int &iSel);

protected:
	void CalcScrollInfo();

protected:

	struct tagDefaultFace
	{
		int			nFaceId;
		char        szUrl[255];
		CRect       rcDraw;
		CBitmap  *  pBitmap;

		tagDefaultFace()
		{
			pBitmap = NULL;
		}
	};

	struct tagHistoryFace
	{
		char        szUrl[255];
		CRect       rcDraw;
		CBitmap  *  pBitmap;

		tagHistoryFace()
		{
			pBitmap = NULL;
		}
	};

	struct 
	{
		int nFacyType;
		int nSel ;

	} m_HotSel, m_CurrentSel;

	bool m_bMouseOver ;

	vector<tagDefaultFace *>  m_vecDefaultFace;//默认头像
	vector<tagHistoryFace *>  m_vecHistoryFace;//已经上传过的图片

	long m_OriginY;
	long m_nClientRealHeigh;

	HANDLE m_hWokuheadProc;

};

