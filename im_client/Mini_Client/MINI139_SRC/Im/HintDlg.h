#pragma once

#include "impublic.h"
#include "..\resource.h"
#include "..\RoundFaceCtrl.h"

class CHintDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CHintDlg)

public:
	CHintDlg(CWnd* pParent, int iHintType /*0提示信息带超级链接,1提示信息不带超级链接*/);   // 标准构造函数
	virtual ~CHintDlg();

// 对话框数据
	enum { IDD=IDD_DIALOG_HINT };
	enum { 
			UP_TIMER   = 0x01, 
			SHOW_TIMER = 0x02,
			DOWN_TIMER = 0x03, 
			MOUSE_TIMER = 0x04,
		};
	enum {
		LINKT_UNKNOWN=0,
		LINKT_FRIEND,
		LINKT_CLUSTER,
	};
	
public:
	
	static void Show(FRIEND_INFO * pFriend,CString strTitle, CString strHint, HWND hCmdWnd);
	static void Show(CLUSTER_INFO * pCluster,CString sTitle,CString sHint,HWND hCmdWnd);
	static void Show(CString strHint, int iHintType=1/*0: with link click, 1:without hint click*/,
						              HWND hCmdWnd=NULL,
									  bool bAutoSink=true);
	static void Show(CString strTitle,CString strHint, int iHintType=1/*0: with link click, 1:without hint click*/,
						              HWND hCmdWnd=NULL,
									  bool bAutoSink=true);
public:
	void SetCmdWnd(HWND hNotify, UINT uCmd);	
	void SetFriendInfo(FRIEND_INFO * pFriend) ;
	void SetClusterInfo(CLUSTER_INFO* pCluster);
	void SetHintText(CString strTitle,CString strHint);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnNcDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

private:
	virtual int DoModal();
	void FlipBackGround(CDC *pDC, CDC * pSafeDC);
	void DrawBitmap(CDC *pDC,int x, int y, UINT nIDBmp);

private:
	int		m_nCy;

	bool	m_bMouseOverHint ;
	bool    m_bMouseOverClose;

	FRIEND_INFO *	m_pFriend;
	CLUSTER_INFO *	m_pCluster;
	int				m_nLinkType;	// 0.链接对象未知；1.当前链接对象为好友；2.当前链接对象为群
	CBitmap *		m_bitmapFace;
	CString			m_strTitle;
	CString			m_strHint;

	COLORREF      m_crHintText ;

	CRect		  m_rcFace;
	CRect		  m_rcHint;

	bool		  m_bNotifySended;

	HWND		  m_hWndNotify;
	UINT		  m_uMessageID;

	int           m_iActiveProcessed;

	CRect	      m_rcClose;

	int           m_iHintType ;//0提示信息带超级链接,1提示信息不带超级链接

	bool          m_bAutoSink ;


	static unsigned int  HintID;
	static vector<CHintDlg *>  m_vecInstance;
	bool          m_bDrawBorder;
	HBRUSH m_hBkGround;
	CRountFaceCtrl *m_pFaceCtrl;
public:
	afx_msg void OnPaint();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
};

