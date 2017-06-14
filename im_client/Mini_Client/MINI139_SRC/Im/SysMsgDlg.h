#pragma once

#include "richeditctrlex.h"
#include "improtocol.h"
#include "buttonext.h"
#include <map>
using namespace std;

class CSysMsgDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CSysMsgDlg)
	DECLARE_HMG_SCROLLBAR()

public:
	virtual ~CSysMsgDlg();

	static void ShowSysMsg(time_t pub_time, CString strMsg);

// 对话框数据
	enum { IDD = IDD_DIALOG_SYSMSG };

protected:
	CSysMsgDlg(CString strMsg, CWnd* pParent = NULL);   // 标准构造函数
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK() { DestroyWindow(); }
	virtual void OnCancel() { DestroyWindow(); }
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnNcDestroy();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

	void OnLogOut(const char *data, WORD wRet);

private:
	CRichEditCtrlEx    m_editor;
	CBmpBtn            m_btnClose;
	CString            m_strMsg;
	time_t			   m_t;
	static map<time_t, CSysMsgDlg *> mapInstance;
};
