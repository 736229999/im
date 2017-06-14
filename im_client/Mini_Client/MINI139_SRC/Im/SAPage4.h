#pragma once

//�������á�- ��¼����
#include "SABasePage.h"
#include "impublic.h"
#include "afxwin.h"

class CSAPage4 : public CSABasePage 
{
	DECLARE_DYNAMIC(CSAPage4)

public:
	CSAPage4(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSAPage4();

	CGraphComboBox  m_comboImServer;
	CButton  m_btnCheckHide,m_btnCheckAutoLogin,m_btnCheckAutoStart;
// �Ի�������
	enum { IDD = IDD_SA_PAGE4 };

public:
	int SaveLoginSetting();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboImserver();
	
	DECLARE_MESSAGE_MAP()
	
public:
	CButton m_btnCheckHint;
};
