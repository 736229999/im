#pragma once
#include "resource.h"


// CArtSetDlg 对话框

class CArtSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CArtSetDlg)

public:
	CArtSetDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CArtSetDlg();

// 对话框数据
	enum { IDD = IDD_ARTS_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
