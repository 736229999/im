#pragma once

#include "sabasepage.h"
#include "shockwaveflash.h"
// CSAPage6 �Ի���

class CSAPage6 : public CSABasePage
{
	DECLARE_DYNAMIC(CSAPage6)

public:
	CSAPage6(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSAPage6();

	virtual void ResetModifyFlag();
// �Ի�������
	enum { IDD = IDD_SA_PAGE6 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	DECLARE_MESSAGE_MAP()

	LRESULT OnWebFlashNotify(WPARAM wParam, LPARAM lParam);

private:
	CXpButton		  m_btnRefresh;
	CShockwaveflash * m_pWndAvatar;
};
