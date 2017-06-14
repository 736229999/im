#pragma once
#include "afxcmn.h"
#include "resource.h"
#include "AutoAcceptDlg.h"
#include "AutoReplyDlg.h"
#include "..\im\impublic.h"
#include "..\..\HmgSkin\HmgSkin\ColumnCtrl.h"


// CSysCommonAutoReply �Ի���

class CSysCommonAutoReply : public CDialog
{
	DECLARE_DYNAMIC(CSysCommonAutoReply)

public:
	CSysCommonAutoReply(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSysCommonAutoReply();

	// ��������
	void Save();

// �Ի�������
	enum { IDD = IDD_WIZARD_SYS_COMMON_AUTO_REPLY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
	// �˵���ѡ�Ÿı��¼�
	afx_msg LRESULT OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam);

	CColumnCtrl m_colCtrl;
	CFont m_font;
};
