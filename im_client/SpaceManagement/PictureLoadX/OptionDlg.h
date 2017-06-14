#pragma once


// COptionDlg �Ի���

class COptionDlg : public CDialog
{
	DECLARE_DYNAMIC(COptionDlg)

public:
	COptionDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~COptionDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_OPTION };
protected: 
	BOOL m_bAlwaysTopMainDlg;
	int m_nIntervalMainDlg;
	BOOL m_bEmptyPictures;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void PreSubclassWindow();

	DECLARE_MESSAGE_MAP()
protected:
	afx_msg void OnBnClickedButtonCancle();
	afx_msg void OnBnClickedButtonOk();
	afx_msg void OnDeltaposSpinInterval(NMHDR *pNMHDR, LRESULT *pResult);
public:	
	BOOL  IsAlwaysTop();
	int   GetInterval();
	BOOL  IsEmptyPictures();
	
	virtual BOOL OnInitDialog();
	
};
