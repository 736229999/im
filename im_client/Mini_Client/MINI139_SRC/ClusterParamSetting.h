#pragma once
#include "xskindlg.h"
#include "..\resource.h"
#include  "..\controls\skinctrl.h"
#include "ClusterInfo.h"
#include "ClusterMember.h"
#include "ClusterProfile.h"
#include "ClusterSecurity.h"
#include "afxwin.h"
#include  "..\Im\impublic.h"
#include  "..\Im\IMProtocol.h"
#include  "..\Im\ImageFileDialog.h"
#include  "..\Im\FamilyFaceDlg.h"
#include "..\..\HmgSkin\HmgSkin\ColumnCtrl.h"
#include "RoundFaceCtrl.h"
// CClusterParamSetting 对话框

typedef struct 
{
	CString strFile;
	CFamilyFaceDlg *pDlg;
}  ThParam;

extern ThParam m_thParam;

class CRountFaceCtrl;
class CModHeadIconDlg;
class CClusterParamSetting : public CXSkinDlg
{
	DECLARE_DYNAMIC(CClusterParamSetting)

public:
	CClusterParamSetting(CLUSTER_INFO *pInfo, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClusterParamSetting();

// 对话框数据
	enum { IDD = IDD_DIALOG_CLUSTER_SETTING };
private:
	static CClusterParamSetting * pInstance;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	void PostNcDestroy();
	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CClusterInfo     *m_pClusterInfo;
	CClusterMember   *m_pClusterMember;
	CClusterProfile  *m_pClusterProfile;
	CClusterSecurity *m_pClusterSecurity;
	BOOL  m_bSecurityCfg;
	afx_msg void OnBnClickedBtnClusterinfo();
	afx_msg void OnBnClickedBtnClustermember();
	afx_msg void OnBnClickedBtnProfile();
	afx_msg void OnBnClickedBtnSecurity();
	void  CreateDialogBox(CLUSTER_INFO *pInfo);
	static CClusterParamSetting *Show(CLUSTER_INFO *pCluster);
	static void DestroyDlg();
public:
	//static void  Show(CLUSTER_INFO *pCluster);
	afx_msg void OnBnClickedClose();
	afx_msg void OnBnClickedBtnPhoto();
	// 修改头像窗口的自定义消息
	afx_msg LRESULT OnModHeadDlgMsg(WPARAM wP, LPARAM lP);
	void UpdateClusterInfo(CLUSTER_INFO *pCluster);
	BOOL AdjustImgSize(CImage& img,CString& strNewFile);
	void SetBitmap();

	void  OnSetClusterInfoAck(const char *data,WORD wRet);
public:
	CBmpBtn m_btnPhoto;
	CBmpBtn m_btnOK;
	CPngBtn m_btnCancel;
	CLUSTER_INFO *m_clusterInfo;
	CComboBox *m_pComboBox;
	afx_msg void OnPaint();
	XT_SET_CINFO_REQ    m_cInfoReq;
	CColumnCtrl m_colMainTab;
	CFont m_font;
	CRountFaceCtrl *m_pFacePic;

	//HBRUSH			m_hBkGround;
	//COLORREF		m_colorBackground;
	//COLORREF		m_colorBorder;
	afx_msg void OnBnClickedBtnOk();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// 菜单栏选着改变事件
	afx_msg LRESULT OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam);
	void OnSetCMemberAck(const char *data, WORD wRet);
	void OnGetCMProfileAck(const char *data, WORD wRet);
	BOOL PreTranslateMessage(MSG* pMsg);

private:
	CModHeadIconDlg *m_pModHeadDlg;
};
