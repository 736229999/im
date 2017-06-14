#pragma once

#include "FaceIconCtrl.h"
#include "afxwin.h"
#include "IMProtocol.h"

#include "..\controls\skinctrl.h"
#include "buttonext.h"
#include "shockwaveflash.h"
#include "explorer1.h"
// CP2PUserPanel 对话框

class CUserPictureWnd : public CButton
{
public:
	CUserPictureWnd();
	~CUserPictureWnd()
	{
		Clear();
	}

public:
	void AddBitmap(CBitmap * pBitmap, CString strTip, DWORD dwData);
	int  GetCurrentItem() { return m_nHotItem; }
	DWORD GetItemData(int nItem) { return m_vecItem[nItem]->dwData; }
	void Clear();

protected:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC *pDC);
	DECLARE_MESSAGE_MAP()

	void ShowToolTip(CString strTip);
	void InitToolTip();
	int  HitTest(CPoint pt);

protected:
	CToolTipCtrl m_ToolTip;	
	CString      m_strPreTip ;
	bool		 m_bOver;
	int			 m_nHotItem;

protected:
	struct tagItem
	{
		CBitmap * pBitmap;
		CString   strTip;
		CRect	  rcItem;
		DWORD     dwData;

		static bool Compare(tagItem *p1, tagItem *p2)
		{
			if ( p1->dwData>p2->dwData )
				return true;
			else
				return false;
		}
	};

	vector<tagItem *> m_vecItem;	

public:
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

class CP2PUserPanel : public CDialog
{
	DECLARE_DYNAMIC(CP2PUserPanel)
	DECLARE_HMG_SCROLLBAR()

public:
	CP2PUserPanel(FRIEND_INFO * pFriend, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CP2PUserPanel();


	//CExplorer1		m_wndAvatar;
	//CStatic			m_wndAvatarFrame;
	void	AdjustUI();
// 对话框数据
	enum { IDD = IDD_P2PUSER_INFO };

	void UpdateFriendInfo();
	void SetFriendInfo(FRIEND_INFO *pFriend) { m_pFriendInfo=pFriend;}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//virtual void OnOK() {}
	//virtual void OnCancel() {}
	afx_msg void OnStnClickedStaticFriend();
	afx_msg void OnBnClickedButtonMore();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnAddfriend();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

	LRESULT OnWebFlashNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnWebDataNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnWebPictureNotify(WPARAM wParam,LPARAM lParam);
	void OnUserInfoModified(const char *data,WORD wRet);

protected:
	CHyperBtn	m_btnArticle;

	FRIEND_INFO  *	m_pFriendInfo	;
	CHyperBtn		m_btnAddFriend	;

	CShockwaveflash * m_pWndAvatar  ;
	uint32			m_userId        ;

	int				m_nInitWebDataOk;

	TWebArticleInfo * m_pLastestArticle;
	vector<TWebPictureInfo *> m_vecLastestPicture;
public:
	afx_msg void OnStnClickedStaticArt();
	CUserPictureWnd m_wndUserPic;
	afx_msg void OnBnClickedButtonPic();
};
