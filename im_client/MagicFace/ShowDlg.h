#pragma once


// CShowDlg �Ի���

class CShowDlg : public CDialog
{
	DECLARE_DYNAMIC(CShowDlg)

public:
	CShowDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CShowDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_SHOW };

public:
	void ShowFrame(CWnd *pWnd);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

protected:
	CBitmap  m_bitmap;
public:
	afx_msg void OnPaint();
};
