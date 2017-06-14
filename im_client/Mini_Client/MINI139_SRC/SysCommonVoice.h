#pragma once
#include "..\resource.h"
#include "..\im\impublic.h"
#include "..\..\HmgSkin\XListCtrl.h"
//#include "..\..\HmgSkin\HmgSkin\ButtonEx.h"
#include "..\im\LocalData.h"
#include <mmsystem.h>
#include "..\..\..\HmgSkin\HmgSkin\CheckButton.h"
// CSysCommonVoice 对话框
////

#include "..\..\..\DuiLib\UIlib.h"

class CSysCommonVoice : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UICommonVoiceCfg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("VoiceSetting.xml");  }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	void    Save();

	afx_msg void OnBnClickedBtnReceiveMsgVoice();
	afx_msg void OnBnClickedBtnFriendOnlineVoice();
	afx_msg void OnBnClickedBtnSysMsgVoice();
	afx_msg void OnBnClickedBtnShockScreenVoice();
	afx_msg void OnBnClickedBtnReceiveMsgBrowse();
	afx_msg void OnBnClickedBtnFriendOnlineBrowse();
	afx_msg void OnBnClickedBtnSysMsgBrowse();
	afx_msg void OnBnClickedBtnShockScreenBrowse();

private:

	DuiLib::CButtonUI *m_pBtnRecvOnce;  ////试听
	DuiLib::CButtonUI *m_pBtnOnline;    ////试听
	DuiLib::CButtonUI *m_pBtnSysMsg;    ////试听
	DuiLib::CButtonUI *m_pBtnShake;     ////试听

	DuiLib::COptionUI *m_pCheckRecvBtn; ////默认状态
	DuiLib::COptionUI *m_pCheckOnlineBtn; ////默认状态
	DuiLib::COptionUI *m_pCheckSysBtn; ////默认状态
	DuiLib::COptionUI *m_pCheckShakeBtn; ////默认状态

	DuiLib::CEditUI *m_pEditRecvPath;   ////路径填充
	DuiLib::CEditUI *m_pEditOnlinePath; ////路径填充
	DuiLib::CEditUI *m_pEditSysPath;    ////路径填充
	DuiLib::CEditUI *m_pEditShakePath;  ////路径填充

	DuiLib::CButtonUI *m_pBtnVoiceChose;  ////选择文件
	DuiLib::CButtonUI *m_pBtnOnlineChose; ////选择文件
	DuiLib::CButtonUI *m_pBtnSysChose;    ////选择文件
	DuiLib::CButtonUI *m_pBtnShakeChose;  ////选择文件
};
////class CSysCommonVoice : public CDialog
////{
////	DECLARE_DYNAMIC(CSysCommonVoice)
////
////public:
////	CSysCommonVoice(CWnd* pParent = NULL);   // 标准构造函数
////	virtual ~CSysCommonVoice();
////	void Save();
////
////// 对话框数据
////	enum { IDD = IDD_WIZARD_SYS_COMMON_VOICE_LOG };
////
////protected:
////	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
////	virtual BOOL OnInitDialog();
////	void OnOK()	{}
////	void OnCancel() {}
////	DECLARE_MESSAGE_MAP()
////public:
////	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
////	afx_msg void OnDestroy();
////	// 将背景刷为白色
////	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
////	// 绘制表格
////	afx_msg void OnPaint();
////	afx_msg void OnSize(UINT nType, int cx, int cy);
////
////	afx_msg void OnBnClickedBtnReceiveMsgVoice();
////	afx_msg void OnBnClickedBtnFriendOnlineVoice();
////	afx_msg void OnBnClickedBtnSysMsgVoice();
////	afx_msg void OnBnClickedBtnShockScreenVoice();
////	afx_msg void OnBnClickedBtnReceiveMsgBrowse();
////	afx_msg void OnBnClickedBtnFriendOnlineBrowse();
////	afx_msg void OnBnClickedBtnSysMsgBrowse();
////	afx_msg void OnBnClickedBtnShockScreenBrowse();
////
////	// 计算表格摆放位置数据
////	void CalcTablePosData();
////
////private:
////	// 绘制表格所需数据
////	CRect m_rcTable;
////	CPoint m_ptTop1, m_ptTop2, m_ptTop3;
////	CPoint m_ptBottom1, m_ptBottom2, m_ptBottom3;
////	CPoint m_ptLeft1, m_ptLeft2, m_ptLeft3, m_ptLeft4;
////	CPoint m_ptRight1, m_ptRight2, m_ptRight3, m_ptRight4;
////
////	// 表格cell
////	CRect m_arrRcCell[5][4];
////
////	// 试听按钮
////	CPngBtn m_btnVoice_ReceiveMsg, m_btnVoice_FriendOnline, m_btnVoice_SysMsg, m_btnVoice_ShockScreen;
////
////	// 状态栏
////	CCheckButton m_check_ReceiveMsg, m_check_FriendOnline, m_check_SysMsg, m_check_ShockScreen;
////
////	// 浏览文件按钮
////	CPngBtn m_btnBrowse_ReceiveMsg, m_btnBrowse_FriendOnline, m_btnBrowse_SysMsg, m_btnBrowse_ShockScreen;
////
////	// 声音文件路径
////	CFlatEdit m_pEditRecvPath->, m_pEditOnlinePath->, m_pEditSysPath->, m_edit_ShockScreen;
////};
