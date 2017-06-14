#pragma once

#include "sabasepage.h"
// CSAPage6 �Ի���

class CSAPage7 : public CSABasePage
{
	DECLARE_DYNAMIC(CSAPage7)

public:
	CSAPage7(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSAPage7();

// �Ի�������
	enum { IDD = IDD_SA_PAGE7 };

public:
	int  SaveZoneSetting(); 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CXpButton    m_btnPath;
	CFlatEdit    m_edtPath;
public:
	afx_msg void OnBnClickedButtonPath();
};
