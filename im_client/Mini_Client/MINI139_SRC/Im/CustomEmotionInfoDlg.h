#pragma once


// CCustomEmotionInfoDlg 对话框

class CCustomEmotionInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CCustomEmotionInfoDlg)

public:
	CCustomEmotionInfoDlg(CString strShortCut, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCustomEmotionInfoDlg();

	bool IsInfoNextTime() { return m_bInfoNextTime; }
// 对话框数据
	enum { IDD = IDD_DIALOG_CUSTOMEMOTION_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CString m_strShortCut;
	bool    m_bInfoNextTime ;

public:
	afx_msg void OnBnClickedCheck1();
};
