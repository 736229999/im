#pragma once


// CHangupSessionNumSettingDlg �Ի���

class CHangupSessionNumSettingDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CHangupSessionNumSettingDlg)

public:
	CHangupSessionNumSettingDlg(int nNum, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CHangupSessionNumSettingDlg();

	// ��ȡ���õ��Զ�����Ŀ
	int GetNum()
	{
		return m_nNum;
	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK();
	BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();
	// ��Ӧ�ؼ���ɫ�����¼���������Ʊ���ɫ����ԭ����ɫ������
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

private:
	CFlatEdit m_editHangupNum;
	CBmpBtn m_btnOK;
	CBmpBtn m_btnCancel;
	int m_nNum;
};
