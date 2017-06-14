#pragma once
#include "..\resource.h"
#include  "..\controls\skinctrl.h"
#include "afxwin.h"
#include "stdafx.h"
#include "..\im\impublic.h"
#include "ClusterParamSetting.h"


struct CLUSTER_INFO_CLASS_JOINFLAG
{
	uint16    nFamilyClass;			
	uint8     nJoinFlag;
};

// CDialogClusterManage 对话框
#define WM_CLUSTER_MANAGE_MSG  WM_USER + 0X0203
class CDialogClusterManage : public CDialog
{
	DECLARE_DYNAMIC(CDialogClusterManage)

public:
	CDialogClusterManage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogClusterManage();

// 对话框数据
	enum { IDD = IDC_DIALOG_CLUSETR_MANAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	static std::tr1::shared_ptr<CDialogClusterManage> Show();

private:
	static std::tr1::shared_ptr<CDialogClusterManage> pInstance;

public:
    afx_msg LRESULT OnSetClusterInfo(WPARAM wParam,LPARAM lParam);
    afx_msg void OnStnClickedStaticCluster();
   afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
protected:
	CHyperBtn m_btnClusterSetting;
	CLUSTER_INFO *     m_pCluster;
	CString m_strClusterInfo;
	CLUSTER_INFO_CLASS_JOINFLAG  m_ClusterInfoClassFlag;
};
