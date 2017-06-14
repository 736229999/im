#pragma once

#include "resgdiobj.h"
// CP2PFileTransProgressBar

class CP2PFileTransProgressBar : public CStatic
{
	DECLARE_DYNAMIC(CP2PFileTransProgressBar)

public: 
	CP2PFileTransProgressBar();
	virtual ~CP2PFileTransProgressBar();
   
public:
	void  SetItemData(int nItem,DWORD dwData);
	DWORD GetItemData(int nItem);
	BOOL  GetItemInfo(int nItem,DWORD &dwUser, BOOL &bSendFile);
	DWORD AddItem(CString strFileName,long nFileSize,DWORD dwUser,BOOL bSendFile);
	void  SetItemProgressPos(int nItem, int nPos);
	void  SetItemProgressTip(int nItem, CString strTip);
	int   FindItemByItemData(DWORD dwData,BOOL bSendFile);
	void  DeleteItem(int nItem);
	void  SetCurSel(int nSel);
	int   GetCurSel();
	int   GetItemCount() { return m_vecItems.size(); }

protected:
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnPaint();	
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()

protected:
	void Draw(CDC *pClientDC);
	void SetToolTip(CString strTip);
	void InitToolTip();

protected:
	struct tagItem
	{
		CString     strFileName     ;
		CString		strProgressTip	;
		CString		strFileSize		;
		int			nProgressPos    ;
		BOOL		bSendFile		;
		DWORD		dwData			;
		CBitmap *	pFileBitmap		;

		CRect		rcTabBtn		;

		tagItem()
		{
			pFileBitmap  = NULL		;
			nProgressPos = 0		;     
			rcTabBtn.SetRect(0,0,0,0);
		}

		~tagItem()
		{
			if ( pFileBitmap )
			{
				delete pFileBitmap;
				pFileBitmap = NULL;
			}
		}
	};

    int						m_nCurSel	;
	int						m_nHotItem	;
	bool					m_bCloseBtnDown;
	vector<tagItem *>		m_vecItems	;
	CRect					m_rcBtnClose;
	CResBitmap				m_bmpBtns   ;
	CResBitmap				m_bmpBtnClose;

	bool					m_bMouseOnTabBtns;
	bool					m_bMouseOnCloseBtn;

	
	CToolTipCtrl m_ToolTip;

};


