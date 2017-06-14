#pragma once

#include "SABasePage.h"
#include "impublic.h"
// 个人设置　－联系方式
class CSAPage2 : public CSABasePage
{
	DECLARE_DYNAMIC(CSAPage2)

public:
	CSAPage2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSAPage2();

	CFlatEdit	m_editMobile,m_editAddress,m_editEmail;
	CHyperBtn	m_btnBind;

// 对话框数据
	enum { IDD = IDD_SA_PAGE2 };

public:
	void SetUserInfo(const XT_USER_INFO &info);
	int  GetUserInfo(XT_USER_INFO &info);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

    
	
public:
	afx_msg void OnStnClickedStaticBind();
	afx_msg void OnBnClickedRadioPublic();
};
