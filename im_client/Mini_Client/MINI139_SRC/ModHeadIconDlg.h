#pragma once
#include "..\HmgSkin\HmgSkin\ColumnCtrl.h"
#include "clientprotocol.h"
#include "im/improtocol.h"

#define HEAD_TYPE_DEFULT 1
#define HEAD_TYPE_CUSTOM 2
// 自定义消息
#define WM_MOD_HEAD_DLG_MSG (WM_USER+412)
#define WM_PIC_PREVIEW_MSG (WM_USER+413)
#include <map>
#include <string>

// CModHeadIconDlg 对话框
class CSelDefHeadIconDlg;
class CRountFaceCtrl;
class CSelCustomHeadIconDlg;
class CModHeadIconDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CModHeadIconDlg)
	DECLARE_HMG_SCROLLBAR()

public:
	CModHeadIconDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CModHeadIconDlg();
	virtual BOOL OnInitDialog();
	void OnOK();
	void OnCancel();
	// 创建
	BOOL Create(CWnd *pParentWnd);
	// 获取选择的头像的url
	string GetHeadUrl();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// 选择默认头像的窗口的自定义消息响应
	afx_msg LRESULT OnSelDefHeadDlgMsg(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnPicPreviewMsg(WPARAM wP, LPARAM lP);
	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

private:
	CPngBtn m_btnOK;
	CPngBtn m_btnCancel;

	CColumnCtrl m_MainTab;
	CFont m_font;

	CRect m_rcTab;
	CRect m_rcBottonArea;
	CRect m_rcWorkArea;
	CRect m_rcSep;			// 工作区与预览区之间的分割线区域
	CRect m_rcPreview;

	CSelDefHeadIconDlg *m_pSelDefHeadDlg;
	CSelCustomHeadIconDlg *m_pSelCustomHeadDlg;
	CRountFaceCtrl *m_pPreviewPic;

	std::map<std::string, std::string> m_mapDefHeadLocal_Url;

	IMAGE_FILE_STRUCT m_imageFile;
	vector<SEND_FILE_STRUCT *>  m_vecSendFiles;
	CString m_strFilePath;
	int m_nHeadType;
public:
	afx_msg LRESULT OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam);
	void OnHeadFileUploadAck(const char *data,WORD wRet);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
