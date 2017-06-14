#pragma once
#include "Im\impublic.h"

// CSelfInfoPreviewDlg �Ի���
#define WM_SELF_INFO_PREVIEW_DLG_MSG (WM_USER+152)		// �Զ���С�ģ�wpΪ��ťID
class CSelfInfoPreviewDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelfInfoPreviewDlg)

public:
	CSelfInfoPreviewDlg(CWnd* pParent);   // ��׼���캯��
	virtual ~CSelfInfoPreviewDlg();
	BOOL Create();
	// �����û���Ϣ
	void Refresh();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
