#pragma once

#include "SABasePage.h"
#include "impublic.h"
// �������á�����ϵ��ʽ
class CSAPage2 : public CSABasePage
{
	DECLARE_DYNAMIC(CSAPage2)

public:
	CSAPage2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSAPage2();

	CFlatEdit	m_editMobile,m_editAddress,m_editEmail;
	CHyperBtn	m_btnBind;

// �Ի�������
	enum { IDD = IDD_SA_PAGE2 };

public:
	void SetUserInfo(const XT_USER_INFO &info);
	int  GetUserInfo(XT_USER_INFO &info);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

    
	
public:
	afx_msg void OnStnClickedStaticBind();
	afx_msg void OnBnClickedRadioPublic();
};
