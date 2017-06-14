#pragma once


// CVersionInfoDlg �Ի���
#include "VersionUpdatgeDlg.h"

class CVersionInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CVersionInfoDlg)
	DECLARE_HMG_SCROLLBAR()

public:
	CVersionInfoDlg(int nVersonFlag, CString strVersionInfoUrl, CWnd* pParent );   // ��׼���캯��
	virtual ~CVersionInfoDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_VERSION_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()

private:
	int  m_nVersionFlag;//0����,1���Բ�������,2ǿ������

	CString			m_strTip;
	CString			m_strVersionInfoUrl;
	CFlatEdit		m_edtInfo;
public:
	afx_msg void OnPaint();
};
