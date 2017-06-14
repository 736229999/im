#pragma once

#include "SABasePage.h"
#include "impublic.h"
//�������á�- ���簲ȫ

class CSAPage3 : public CSABasePage
{
	DECLARE_DYNAMIC(CSAPage3)

public:
	CSAPage3(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSAPage3();

// �Ի�������
	enum { IDD = IDD_SA_PAGE3 };

public:
	virtual void ResetModifyFlag();

public:
	void SetUserInfo(const XT_USER_INFO &info);
	int  GetUserInfo(XT_USER_INFO &info);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckPswd();
	CFlatEdit m_edtPass1,m_edtPass2,m_edtPass3;
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeEditPass2();
	afx_msg void OnEnChangeEditPass3();
};
