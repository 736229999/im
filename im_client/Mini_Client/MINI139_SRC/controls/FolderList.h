#pragma once

#include "resgdiobj.h"
#include <vector>
using namespace std;

/////////////////////////////////////////////////////////////////////////////
// CFolderList window

enum FOLDER_STATE
{
	FOLDER_EXPAND,
		FOLDER_CLOSE,
};

enum FOLDER_STYLE
{
	FS_CHECK=0x01,
	FS_IMAGE=0x02,
};

class CFolderList : public CWnd
{
// Construction
public:
	
	struct tagItem
	{
		DWORD dwData;
		bool isMouseOver;
		bool isSelected;
		bool isChecked ;
		int  iImageIndex;
		CString strText;
		CRect rcItem;
		CRect rcCheckBox ;

		tagItem()
		{
			isSelected  = false;
			isMouseOver = false;
			isChecked   = false;
			iImageIndex = -1;
			dwData=0;
		}
	};

	struct tagFolder
	{
		FOLDER_STATE state;
		bool isMouseOver;
		CString title;
		vector<tagItem *> vecItem;	
		CRect rcBar;

		tagFolder()
		{
			state=FOLDER_EXPAND;
			title="title";
			isMouseOver=false;
		}
	};

	CFolderList();
	virtual ~CFolderList();

	// Implementation
public:
	void ClearAll();
	int AddFolder(CString strTitle,FOLDER_STATE state = FOLDER_EXPAND);
	int GetCurSel();
	void SetCurSel(int nIndex);
	BOOL Create(CWnd *pParentWnd,const CRect &rect, DWORD dwStyle=WS_VISIBLE|WS_CHILDWINDOW|WS_CLIPCHILDREN,UINT nID=0);
	void SetItemData(int nIndex,DWORD dwData);
	int AddItem(CString strText, int iFolder, bool bCheck=false, int iImageIndex=-1, bool bTail=true);
	DWORD GetItemData(int nIndex);
	bool GetItemCheck(int nIndex);
	void SetItemCheck(int nIndex, bool bCheck=true);
	void SetItemImage(int nIndex, int iImageIndex);
	void SetFolderListStyle(DWORD dwStyle) { m_dwStyle = dwStyle; }
	void SetImageList(CImageList *pImageList) { m_pImageList=pImageList; }
	void SetItemIndent(int nIndent) { m_nItemIndent=nIndent; }
	CString GetItemText(int nIndex);

	inline int GetFolderCount() { return m_vecFolder.size() ; };
	inline int GetItemCount() { return m_vecItem.size(); };
	void CalcScrollInfo();
	void RecalcLayout();


protected:
	virtual void PreSubclassWindow();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel (UINT nFlags, short zDelta, CPoint pt);

	DECLARE_MESSAGE_MAP()
private:
	void DrawFolder(const tagFolder * folder,CDC *pDC,const CRect rc);
	int GetItemIndex(tagItem *pItem);

private:
	int m_nItemHeight;
	int m_nTextHeight;

	int m_nCurSel;
	vector<tagFolder *> m_vecFolder;
	vector<tagItem *> m_vecItem;

	CPoint m_point;
	CFont m_NormalFont,m_BoldFont;

	CResBitmap   m_bmpCheck, m_bmpUnCheck;
	int  m_nItemIndent;

private:
	DWORD        m_dwStyle;
	CImageList * m_pImageList;
	int          m_nOriginY  ;
	int			 m_nClientRealHeigh ;
};
