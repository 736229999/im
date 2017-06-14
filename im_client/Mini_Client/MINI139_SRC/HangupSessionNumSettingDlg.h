#pragma once


// CHangupSessionNumSettingDlg 对话框

class CHangupSessionNumSettingDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CHangupSessionNumSettingDlg)

public:
	CHangupSessionNumSettingDlg(int nNum, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CHangupSessionNumSettingDlg();

	// 获取设置的自定义数目
	int GetNum()
	{
		return m_nNum;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	// 相应控件颜色绘制事件，解决控制背景色保持原有颜色的问题
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	CFlatEdit m_editHangupNum;
	CBmpBtn m_btnOK;
	CBmpBtn m_btnCancel;
	int m_nNum;
};
