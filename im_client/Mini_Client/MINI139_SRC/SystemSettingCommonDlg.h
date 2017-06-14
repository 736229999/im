#pragma once
#include "afxwin.h"
#include "..\resource.h"
#include "SysCommonBasic.h"
#include "SysCommonMsg.h"
#include "SysCommonVoice.h"
#include "SysCommonAudio.h"
#include "SysCommonFile.h"
#include "SysCommonSoftwareUpdate.h"
#include "SysCommonAutoReply.h"
#include "SysCommonFastWord.h"
#include "..\im\impublic.h"
#include "..\..\HmgSkin\HmgSkin\ColumnCtrl.h"

// CSystemSettingCommonDlg 对话框

class CSystemSettingCommonDlg : public CDialog
{
	DECLARE_DYNAMIC(CSystemSettingCommonDlg)

public:
	CSystemSettingCommonDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSystemSettingCommonDlg();
	// 控制Tab页显示
	void ShowTab(int nTabIndex);

	// 保存配置
	void Save();

// 对话框数据
	enum { IDD = IDD_WIZARD_CLUSTER_SYS_COMMON };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	void OnOK()	{}
	void OnCancel() {}
	DECLARE_MESSAGE_MAP()
private:	
	void  CreateCommonDlg();
	CStatic m_ctlFrameCommon;
	CSysCommonBasic  *m_pSysComBasic;
	CSysCommonMsg    *m_pSysComMsg;
	CSysCommonVoice  *m_pSysComVoice;
	CSysCommonAudio  *m_pSysComAudio;
	CSysCommonFile   *m_pSysComFile;
	CSysCommonSoftwareUpdate  *m_pSysSoftUpdate;
	CSysCommonAutoReply  *m_pSysAutoReply;
	//CSysCommonFastWord    *m_pSysFastWord;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	// 菜单栏选着改变事件
	afx_msg LRESULT OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam);
	CColumnCtrl m_colBasicSetting;
	CFont m_font;
	vector<CWnd*> m_vecpChildWnd;
};
