#pragma once


// CCustomEmotionInfoDlg �Ի���

class CCustomEmotionInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CCustomEmotionInfoDlg)

public:
	CCustomEmotionInfoDlg(CString strShortCut, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCustomEmotionInfoDlg();

	bool IsInfoNextTime() { return m_bInfoNextTime; }
// �Ի�������
	enum { IDD = IDD_DIALOG_CUSTOMEMOTION_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

private:
	CString m_strShortCut;
	bool    m_bInfoNextTime ;

public:
	afx_msg void OnBnClickedCheck1();
};
