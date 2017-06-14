#pragma once
#include "Im\UserLevelCtrl.h"
#include "Im\IMProtocol.h"

// CSelfInfoDlg 对话框
class CSelfInfoPreviewDlg;
class CRountFaceCtrl;
class CSelfInfoEditDlg;
class CModHeadIconDlg;
class CSelfInfoDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CSelfInfoDlg)

public:
	CSelfInfoDlg(CWnd* pParent);   // 标准构造函数
	virtual ~CSelfInfoDlg();
	BOOL Create();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	// 设置控件颜色
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LRESULT OnSelMsg_PreviewDlg(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnSelMsg_EditDlg(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnModHeadDlgMsg(WPARAM wP, LPARAM lP);
	afx_msg void OnBnClickedFace();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	// 修改个人资料时后台返回消息
	void OnUserInfoModAck(const char *data,WORD wRet);
	void OnUserInfoAck(const char *data,WORD wRet);
	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

private:
	CSelfInfoPreviewDlg *m_pPreViewDlg;
	CSelfInfoEditDlg *m_pEditDlg;
	CModHeadIconDlg *m_pModHeadDlg; 
	CRountFaceCtrl *m_pFacePic;
	HBRUSH m_hBkCaptionBar;
	CUserLevelCtrl m_staLevel;		// 等级
};
