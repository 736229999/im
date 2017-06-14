#pragma once
#include "afxwin.h"
#include "SABasePage.h"
#include "..\MMTalk\mmvideocap.h"

class CSAPageVedio : public CSABasePage
{
	DECLARE_DYNAMIC(CSAPageVedio)

public:
	CSAPageVedio(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSAPageVedio();

	// 对话框数据
	enum { IDD = IDD_SA_PAGE_VIDEO };

public:
	int  SaveVedioSetting(); 

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonPath();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	DECLARE_MESSAGE_MAP()

protected:
	CMMVideoCap	   m_videoCap;
	bool		   m_bInitVedioList;
	CGraphComboBox m_comboVedioList;
public:
	afx_msg void OnCbnSelchangeComboVedio();
};