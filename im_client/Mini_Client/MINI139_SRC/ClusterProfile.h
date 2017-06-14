#pragma once
#include "..\resource.h"
#include  "..\controls\skinctrl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "..\Im\impublic.h"
#include "..\Im\formprogress.h"
#include "MainFrame.h"
// CClusterProfile 对话框

class CClusterProfile : public CDialog
{
	DECLARE_DYNAMIC(CClusterProfile)

public:
	CClusterProfile(CWnd* pParent = NULL);   // 标准构造函数
	CClusterProfile(CLUSTER_INFO *pInfo, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClusterProfile();
	std::string GetProfileName();	// 获取群片名

// 对话框数据
	enum { IDD = IDD_CLUSTER_PROFILE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedRadio2();	
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnBnClickedRadioRecPop();
	void OnUpdateUI(CLUSTER_INFO *pCluster);
	//void OnGetCMProfileAck(const char *data, WORD wRet);

private:
	CLUSTER_INFO *m_pCluster;
	CFlatEdit m_edtClusterNickname;

public:
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL PreTranslateMessage(MSG* pMsg);
};
