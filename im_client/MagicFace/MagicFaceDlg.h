// MagicFaceDlg.h : 头文件
//

#pragma once
#include "shockwaveflash1.h"
#include "showdlg.h"

// CMagicFaceDlg 对话框
class CMagicFaceDlg : public CDialog
{
// 构造
public:
	CMagicFaceDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MAGICFACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

protected:
	void OnProgressShockwaveflash1(long percentDone);
	void FSCommandShockwaveflash1(LPCTSTR command, LPCTSTR args);
	void FlashCallShockwaveflash1(LPCTSTR request);
	void OnReadyStateChangeShockwaveflash1(long newState);
	DECLARE_EVENTSINK_MAP()

protected:
	CShockwaveflash1 m_FlashOcx;
	CShowDlg  *      m_pDlgShow;


public:
	afx_msg void OnDestroy();
};
