#pragma once


#include "../Resource.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "..\..\..\HmgSkin\HmgSkin\CheckButton.h"

class CHistoryFindToolBar;

class CHistoryToolBar : public CDialog
{
	DECLARE_DYNAMIC(CHistoryToolBar)

public:
	CHistoryToolBar(CWnd* pHitory, CWnd* pParent = NULL);   // standard constructor
	virtual ~CHistoryToolBar();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	// Dialog Data
	enum { IDD = IDD_HISTORY_TOOL_BAR };

	enum PAGE_TPYE
	{
		PAGE_FIRST = 0,
		PAGE_LAST,
		PAGE_NEXT,
		PAGE_PREV
	};

public:
	afx_msg void OnBnClickedButtonSearch();
	afx_msg void OnBnClickedButtonHistoryFind();
	afx_msg void OnBnClickedButtonFirst();
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonLast();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	void				SetPage(INT32 iTotal, INT32 iCurrent);

public:
	CPngBtn    m_btnFirst, m_btnPrev, m_btnNext, m_btnLast;
	CCheckButton		m_btnSearch;
private:
	CWnd*				m_pHitory;

	CStatic				m_staticHistoryPage;
};