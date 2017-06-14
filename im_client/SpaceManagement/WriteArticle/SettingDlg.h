#pragma once
#include "resource.h"
#include "graphcombobox.h"


// CSettingDlg 对话框

class CSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSettingDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CGraphComboBox m_tagComboBox;
	CGraphComboBox m_comBoBoxView;
	//CGraphComboBox m_comboBoxArticles;
	

	CGraphComboBox m_comboBoxArts;
//	afx_msg void OnCbnSelchangeComboTag();
	virtual BOOL OnInitDialog();
};
