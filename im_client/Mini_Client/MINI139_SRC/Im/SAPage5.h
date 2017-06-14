#pragma once

#include "sabasepage.h"
#include "afxwin.h"
// CSAPage5 �Ի���

class CSAPage5 : public CSABasePage
{
	DECLARE_DYNAMIC(CSAPage5)

public:
	CSAPage5(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSAPage5();

// �Ի�������
	enum { IDD = IDD_SA_PAGE5 };

public:
	int  SaveUserRecvFolder(); 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPath();

	DECLARE_MESSAGE_MAP()

private:
	CXpButton    m_btnPath;
	CFlatEdit    m_edtPath;
};
