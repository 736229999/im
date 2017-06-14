#pragma once
#include "afxwin.h"
#include "..\resource.h"
#include "SysSecurityPwd.h"
#include "SysSecurityVerify.h"
#include "SysSecurityMsglog.h"
#include "SysSecurityFileTransfer.h"
#include "..\im\impublic.h"
#include "..\..\SpaceManagement\public\public_controls\buttonext.h"
#include "..\..\HmgSkin\HmgSkin\ColumnCtrl.h"
// CSystemSettingSecurityDlg �Ի���

class CSystemSettingSecurityDlg : public CDialog
{
	DECLARE_DYNAMIC(CSystemSettingSecurityDlg)

public:
	CSystemSettingSecurityDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSystemSettingSecurityDlg();
	// ����Tabҳ��ʾ
	void ShowTab(int nTabIndex);
	// ��������
	void Save();

// �Ի�������
	enum { IDD = IDD_WIZARD_CLUSTER_SYS_SECURITY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	void OnOK()	{}
	void OnCancel() {}
	DECLARE_MESSAGE_MAP()
public:
	void  CreateSecurityDlg();
	afx_msg void OnDestroy();
private:
	CStatic m_ctlFrameSecurity;
	CSysSecurityPwd*    m_pSysSecPwd;
	CSysSecurityVerify* m_pSysSecVerify;
	CSysSecurityMsglog* m_pSysSecMsglog;
	CSysSecurityFileTransfer* m_pSysSecFileTransfer;

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// �˵���ѡ�Ÿı��¼�
	afx_msg LRESULT OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam);

	CColumnCtrl m_colSecuritySetting;
	CFont m_font;
	vector<CWnd*> m_vecpChildWnd;
};
