#pragma once

#include <afxtempl.h>
#include "FloorPageObject.h"
#include "TLabelComboBox.h"

#define ID_LABELCOMBO	0x3E81
#define ID_LABEL		0x3E82

class CFloorLabelObject;

typedef CArray<CFloorPageObject*, CFloorPageObject*> CArrayPage;

class CXFloorWndCtrl : public CWnd
{
	friend class CFloorPageObject;
	friend class CFloorObject;
	friend class CFloorLevelObject;

public:
	CXFloorWndCtrl();
	~CXFloorWndCtrl();

public:
	BOOL Create(CWnd *pParentWnd,const CRect &rect, DWORD dwStyle=WS_VISIBLE|WS_CHILDWINDOW|WS_CLIPCHILDREN,UINT nID=0x5670);

	BOOL ActivatePage(LPCTSTR lpszPageName);
	BOOL ActivatePage(int nIndex);
	BOOL DeletePage(LPCTSTR lpszPageName);
	BOOL DeletePage(int nIndex);
	void RecalcLayout();
	BOOL PullDownPages(int nIndex);
	BOOL PullUpPages(int nIndex);
	BOOL AttachWindow(short nIndex, CWnd * pWnd) ;
	BOOL SetPageName(short nIndex, LPCTSTR lpszPageName) ;
	void Init();

	int  GetCurSel();

protected:
	void PrepareChangeStyle();

	virtual CFloorPageObject* NewPage(BOOL bAsPage, LPCTSTR lpszPageName);
	virtual void Move(int nIndex, BOOL bDown = TRUE);
	int PtrPageToIndex(CFloorPageObject* pPage);

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

protected:
	void OnAsPageChanged();
	void OnPagesChanged();

	void StyleAs(BOOL bAsFloor);
	short GetHeight(short nIndex);
	void SetHeight(short nIndex, short nNewValue);
	long GetUserData(short nIndex);
	void SetUserData(short nIndex, long nNewValue);

public:
	virtual int Lookup(CFloorLabelObject* pLabelObject);
	virtual CTLabelComboBox* GetLabelControl();
	virtual CComboBox* GetComboBox();
	static UINT m_nMessageChange;
	virtual CFloorPageObject* GetPageFromPoint(CPoint point);
	virtual CRect GetRectClient();
	virtual CFloorPageObject*  GetActivePage();
	static BOOL RegClassFloorWnd();
	virtual void OnActivatePage(CFloorPageObject* pPage);
	virtual CFloorPageObject* AddPage(LPCTSTR lpszPageName, int* pIndex = NULL);
	int GetCountPage();
	virtual CFloorPageObject* GetPage(LPCTSTR lpszPageName);
	virtual CFloorPageObject* GetPage(int nIndex);
	CBitmap* Preview(CWnd * pWnd);

private:
	CRect  ChangeHeightLabel(CSize s);

	CRect  m_rectClient;
	static CRect	m_rtBorder;
	static CFloorPageObject * m_pFocusPage;

	int m_nDYLabel, m_nDYCombo;
	CTLabelComboBox		m_lbPage;
	CComboBox			m_cbPage;
	CFloorPageObject *  m_pActivePage;
	CArrayPage       *  m_pArPage;
	BOOL				m_bAnimation;
	long				m_nSleep;
	BOOL				m_bSign;
	CString				m_sPages;
	BOOL				m_bAsPage;

	COLORREF   m_colorBorder, m_colorBkGnd;
};