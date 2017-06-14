#pragma once


// CShowDlg 对话框

class CShowDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowDlg)

public:
	CShowDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CShowDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_SHOW };

public:
	void ShowFrame(CWnd *pWnd);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

protected:
	CBitmap  m_bitmap;
public:
	afx_msg void OnPaint();
};
