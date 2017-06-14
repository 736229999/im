#pragma once

// 个人设置，该设置界面只允许一个实例存在

#include "SAPage1.h"
#include "SAPage2.h"
#include "SAPage3.h"
#include "SAPage4.h"
#include "SAPage6.h"//Avatar 
#include "SAPage7.h"//个人空间管理 - 文章
#include "SAPageHotkey.h"
#include "SAPageBasicSet.h"
#include "SAPage5.h" //接收文件路径设置
#include "SAPageVedio.h"
#include "SAPageClusterMsgSet.h"

#include "IMProtocol.h"

#include "..\controls\FolderList.h"
#include "..\controls\skinctrl.h"
#include "buttonext.h"
//个人设置

class CSAConfig : public CXSkinDlg
{
	DECLARE_DYNAMIC(CSAConfig)
public:
	CSAConfig(CWnd* pParent = NULL);   // 标准构造函数

	virtual ~CSAConfig();

	static CSAConfig * Show(int nStartPage=-1);
	static void Close();
	static CSAConfig * pInstance;

	XT_USER_INFO m_UserInfo;

// 对话框数据
	enum { IDD = IDD_SA_CONFIG };

	void UpdateUserInfo();

public:
	void SetStartPage(int nPage);
	void ShowPage(int nPage);

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void OnHelpClick();
	afx_msg void OnBnClickedCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedFolder();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcDestroy();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	
	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

	void OnUserInfoModified(const char *data,WORD wRet);
	void OnLogOut(const char *data,WORD wRet);

private:
	virtual INT_PTR DoModal();
	void AddPage(CString strPage, CDialog *pPage,int iFolder);

	LRESULT OnUploadNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnFaceSelNotify(WPARAM wParam, LPARAM lParam);

	int m_nPreSelPage;
	int m_nStartPage;

	CFont		m_Font;
	HBRUSH		m_hBrushCaption ;
	HBRUSH		m_hBackGround;

	// 个人设置
	CSAPage1 *			m_pPage1;		// 基本资料
	CSAPage2 *			m_pPage2;		// 联系方式
	CSAPage6 *			m_pPage6;		// Avatar
	CSAPage7 *          m_pPage7;		// 个人空间管理

	// 系统设置
	CSAPage3 *			m_pPage3;		// 网络安全
	CSAPage4 *			m_pPage4;		// 登录设置
	CSAPageBasicSet *	m_pPageBasicSet;// 基本设置
	CSAPageHotkey *		m_pPageHotKey;	// 热键设置
	CSAPage5 *          m_pPage5;       // 文件传输
	CSAPageVedio *      m_pPageVedio    ;//

	CSAPageClusterMsgSet * m_pPageClusterMsgSet;	// 群消息设置

	CSAFrame			m_wndFrame;
	CFolderList			m_wndFolderList;	// 折叠目录窗口

	CBmpBtn				m_btnOk;
	CBmpBtn				m_btnCancel;

	CString				m_strInfo ;
};
