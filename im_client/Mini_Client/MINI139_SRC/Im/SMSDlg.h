#pragma once

#include "faceiconctrl.h"
#include "localdata.h"
// CSMSDlg 对话框

class CSMSDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CSMSDlg)
	DECLARE_HMG_SCROLLBAR()
	DECLARE_XT_MESSAGE_MAP()

public:
	CSMSDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSMSDlg();

public:
	static CSMSDlg * Show(FRIEND_INFO * pFriend);
	static void Close();

// 对话框数据
	enum { IDD = IDD_DIALOG_SMS };

public:
	void ShowRecvSMS(tagSMSItem si);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonSms();
	afx_msg void OnStnClickedStaticInfo();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnEnKillfocusEditSms();
	afx_msg void OnEnSetfocusEditSms();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonExport();
	afx_msg void OnBnClickedButtonClear();
	DECLARE_MESSAGE_MAP()

	void OnSMSAck(const char *data,WORD wRet);

	void Init();
	void SetStyle(int nStyle);

	void UpdateSMSList();

protected:
	static CSMSDlg * pInstance;

	CFlatEdit		m_edtSMS  ;
	CFlatEdit			m_edtSMSShow ;
	CHyperBtn		m_btnInfo;
	CBmpBtn			m_btnHistory;
	FRIEND_INFO		m_FriendInfo;
	CFaceIconCtrl   m_wndFaceCtrl;
	CBmpBtn			m_btnSend, m_btnClose;
	CFont			m_FontSMS;
	CXListCtrl		m_wndListSMS;
	SMS_LIST		m_smsList;

	bool			m_bRecvSMS		;
public:
	afx_msg void OnLvnItemchangedListSms(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHdnItemclickListSms(NMHDR *pNMHDR, LRESULT *pResult);
};
