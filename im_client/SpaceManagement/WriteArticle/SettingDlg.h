#pragma once
#include "resource.h"
#include "graphcombobox.h"


// CSettingDlg �Ի���

class CSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSettingDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
