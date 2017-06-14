#pragma once


// CHistoryFindToolBar dialog

#include "../Resource.h"

class CHistoryFindToolBar : public CDialog
{
	DECLARE_DYNAMIC(CHistoryFindToolBar)

public:
	CHistoryFindToolBar(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHistoryFindToolBar();

// Dialog Data
	enum { IDD = IDD_HISTORY_FIND_TOOL_BAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	CXpButton			m_btnFind;
	CFlatEdit			m_editKeyword;
	CGraphComboBox		m_comboRange;
};
