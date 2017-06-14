#pragma once
#include "afxcmn.h"
#include "resource.h"
#include "AutoAcceptDlg.h"
#include "AutoReplyDlg.h"
#include "..\im\impublic.h"
#include "..\..\HmgSkin\HmgSkin\ColumnCtrl.h"


// CSysCommonAutoReply 对话框

class CSysCommonAutoReply : public CDialog
{
	DECLARE_DYNAMIC(CSysCommonAutoReply)

public:
	CSysCommonAutoReply(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSysCommonAutoReply();

	// 保存配置
	void Save();

// 对话框数据
	enum { IDD = IDD_WIZARD_SYS_COMMON_AUTO_REPLY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	void OnOK()	{}
	void OnCancel() {}
	DECLARE_MESSAGE_MAP()

public:
	void  InitTabCtrl();
	CAutoAcceptDlg  *m_AutoAcptDlg;
	CUIAutoReply	*m_pAutoReply;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// 菜单栏选着改变事件
	afx_msg LRESULT OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam);

	CColumnCtrl m_colCtrl;
	CFont m_font;
};
