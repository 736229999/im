#pragma once
#include "Im\impublic.h"

// CSelfInfoPreviewDlg 对话框
#define WM_SELF_INFO_PREVIEW_DLG_MSG (WM_USER+152)		// 自定义小心，wp为按钮ID
class CSelfInfoPreviewDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelfInfoPreviewDlg)

public:
	CSelfInfoPreviewDlg(CWnd* pParent);   // 标准构造函数
	virtual ~CSelfInfoPreviewDlg();
	BOOL Create();
	// 更新用户信息
	void Refresh();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedEdit();
	void OnOK()	{}
	void OnCancel() {}

	DECLARE_MESSAGE_MAP()

private:
	HBRUSH m_hBkGround;
	CPngBtn m_btnEdit;
};
