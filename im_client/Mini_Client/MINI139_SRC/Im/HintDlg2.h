#pragma once


// CHintDlg2 �Ի���

class CHintDlg2 : public CDialog
{
	DECLARE_DYNAMIC(CHintDlg2)

public:
	CHintDlg2(UINT nHintBtmap, int left, int top,CWnd *pParent);   // ��׼���캯��
	virtual ~CHintDlg2();

// �Ի�������
	enum { IDD = IDD_HINT_PASSDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CResBitmap m_bitmap;
	int     m_iLeft, m_iTop;
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};
