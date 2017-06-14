#pragma once

#include <vector>
#include <string>

// 自定义消息
#define WM_SELDEFHEADDLG_MSG (WM_USER+741)
// 消息种类
enum en_selhead_msg_type		
{
	en_selchange = 0,
};

// CSelDefHeadIconDlg 对话框
class CRountFaceCtrl;
class CSelDefHeadIconDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelDefHeadIconDlg)

public:
	CSelDefHeadIconDlg(const std::vector<std::string> &vecHeadPath);   // 标准构造函数
	virtual ~CSelDefHeadIconDlg();
	BOOL Create(CWnd *pParent);
	BOOL OnInitDialog();
	string GetSelHead()	{	return m_strHeadPath_CurSel;	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// 图标控件被点击的消息相应
	afx_msg void OnBnClickedHeadIcon(UINT uid);
	DECLARE_MESSAGE_MAP()

private:
	std::vector<std::string> m_vecHeadPath;
	std::vector<CRountFaceCtrl*> m_vecpFaceCtrl;
	HBRUSH m_hBkGround;
	std::string m_strHeadPath_CurSel;				// 当前选择的头像路径
};
