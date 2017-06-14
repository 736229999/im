#pragma once

#include "sabasepage.h"
// CSAPage6 对话框

class CSAPage7 : public CSABasePage
{
	DECLARE_DYNAMIC(CSAPage7)

public:
	CSAPage7(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSAPage7();

// 对话框数据
	enum { IDD = IDD_SA_PAGE7 };

public:
	int  SaveZoneSetting(); 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CXpButton    m_btnPath;
	CFlatEdit    m_edtPath;
public:
	afx_msg void OnBnClickedButtonPath();
};
