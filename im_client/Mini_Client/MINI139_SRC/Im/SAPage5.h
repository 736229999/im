#pragma once

#include "sabasepage.h"
#include "afxwin.h"
// CSAPage5 对话框

class CSAPage5 : public CSABasePage
{
	DECLARE_DYNAMIC(CSAPage5)

public:
	CSAPage5(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSAPage5();

// 对话框数据
	enum { IDD = IDD_SA_PAGE5 };

public:
	int  SaveUserRecvFolder(); 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPath();

	DECLARE_MESSAGE_MAP()

private:
	CXpButton    m_btnPath;
	CFlatEdit    m_edtPath;
};
