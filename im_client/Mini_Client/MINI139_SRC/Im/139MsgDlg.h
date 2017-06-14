#pragma once
#include "buttonext.h"
#include "explorer1.h"


// C139MsgDlg 对话框

class C139MsgDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(C139MsgDlg)

public:
	C139MsgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~C139MsgDlg();

	static void Show(CString strMessage);
	static void Close();

// 对话框数据
	enum { IDD = IDD_DIALOG_139MSG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

protected:
	void SetBullitinLinks(CString strLinks);
	void ShowBullitin(int nIndex);

protected:
	CBmpBtn m_btnPrev;
	CBmpBtn m_btnNext;
	CBmpBtn m_btnClose;
	CExplorer1 m_wndExplore;
	CStatic    m_wndFrame;

	CStringList  m_slBullitinLink;
	int			 m_nCurrentLink;

	static C139MsgDlg * pInstance;
public:
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedNext();
	afx_msg void OnBnClickedPrev();
	DECLARE_EVENTSINK_MAP()
	void NavigateComplete2Explorer1(LPDISPATCH pDisp, VARIANT* URL);
};
