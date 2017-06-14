#pragma once

#include "dhtmledit_blog.h""
// CBlogEditToolBar �Ի���

class CBlogEditToolBar : public CDialog
{
	DECLARE_DYNAMIC(CBlogEditToolBar)

public:
	CBlogEditToolBar(CDHtmlEditCtrl *pDHtmlEditCtrl, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBlogEditToolBar();

// �Ի�������
	enum { IDD = IDD_BLOGTOOL_BAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK() {}
	virtual void OnCancel() {}
	DECLARE_MESSAGE_MAP()

private:
	CDHtmlEditCtrl * m_pDHtmlEditCtrl;
	CGraphComboBox   m_comboFontName;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeComboFontname();
	afx_msg void OnBnClickedButtonU();
	afx_msg void OnBnClickedButtonB();
};
