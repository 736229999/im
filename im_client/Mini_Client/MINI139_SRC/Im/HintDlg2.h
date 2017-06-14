#pragma once


// CHintDlg2 对话框

class CHintDlg2 : public CDialog
{
	DECLARE_DYNAMIC(CHintDlg2)

public:
	CHintDlg2(UINT nHintBtmap, int left, int top,CWnd *pParent);   // 标准构造函数
	virtual ~CHintDlg2();

// 对话框数据
	enum { IDD = IDD_HINT_PASSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CResBitmap m_bitmap;
	int     m_iLeft, m_iTop;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};
