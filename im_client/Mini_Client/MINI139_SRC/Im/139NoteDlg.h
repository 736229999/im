#pragma once
#include "buttonext.h"
#include "richeditctrlex.h"
#include "afxcmn.h"

class C139NoteEditCtrl : public CRichEditCtrl
{
	DECLARE_DYNAMIC(C139NoteEditCtrl)

public:
	C139NoteEditCtrl();
	virtual ~C139NoteEditCtrl();

public:
	void SetStartIndent(int nIndent);
	void InsertText( CString text,const XT_FONT& xf);
	void InsertText( CString text);
	void SetXFont(const XT_FONT &xf);
	void SetXFont(const XT_FONT &xf, long nStart, long nEnd);
	CHARRANGE InsertLink(CString strTextLink);
	void SetSenderID(uint32 sender_id, CString strNickName) { m_senderId = sender_id;
		m_strNickName = strNickName;
	}

protected:
	afx_msg void OnEnLink(NMHDR *pNMHDR, LRESULT *pResult);
	virtual void PreSubclassWindow();
	DECLARE_MESSAGE_MAP()

protected:
	XT_FONT * m_pXFont;
	uint32    m_senderId;
	CString   m_strNickName;
};


// C139NoteDlg 对话框

class C139NoteDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(C139NoteDlg)
	DECLARE_HMG_SCROLLBAR()

public:
	C139NoteDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~C139NoteDlg();

	static C139NoteDlg * pInstance;
// 对话框数据
	enum { IDD = IDD_DIALOG_139NOTE };

public:
	static void Show();
	static void Close();
	static void ResetContent();

	static vector<TWebNotificationInfo *> vecNotification;
	static int m_nFreshMessage;
	            
	void InitContent();
	void ShowNotification(const TWebNotificationInfo *info);
	void DisplaySysMsg(const TWebNotificationInfo *info);
	void DisplayMsgBox(const TWebNotificationInfo *info);
	void DisplayFriendMsg(const TWebNotificationInfo *info);
	void DisplayMainPageComment(const TWebNotificationInfo *info);
	void DisplayBlogContentComment(const TWebNotificationInfo *info);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK() {}
	virtual void OnCancel() {}
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedHistory();
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedPrev();
	afx_msg void OnListItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()

	LRESULT OnThreadNotify(WPARAM wParam, LPARAM lParam);
	void SetStyle(int nStyle/*0:带历史消息列表模式;1:不带历史消息模式*/);

protected:
	
	void InitList();

	C139NoteEditCtrl	m_reMessage;
	CBmpBtn		     	m_btnHistory;
	CBmpBtn				m_btnPrev;
	CBmpBtn				m_btnNext;
	CBmpBtn				m_btnClose;
	CXListCtrl			m_wndList;

public:
	afx_msg void OnNMRclickList1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg void OnLvnKeydownList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT nIDEvent);
};
