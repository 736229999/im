#pragma once

#include "../resource.h"
#include "../controls/SkinCtrl.h"

#include "impublic.h"
#include "buttonext.h"
// CSelectGroupDlg �Ի���

class CSelectGroupDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CSelectGroupDlg)

protected:
	CSelectGroupDlg(uint32 nFriendID,const CString & sFriendName, CWnd *pParent);   // ��׼���캯��

public:
	virtual ~CSelectGroupDlg();

// �Ի�������
	enum { IDD = IDD_SELECT_GROUP };

public:
	static void	NewDlg(uint32 nFriendID,const CString& sFriendName,HWND hwndIMPage);
	int		GetGroupIndex() { return m_nGroupIndex; }

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeGroup();
	afx_msg void OnBnClickedOk();
	afx_msg void OnNcDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()

public:
	CGraphComboBox m_cmbGroup;
	CTransparentLabel m_lblMsg;

	HWND            m_hwndIMPage;

protected:
	uint32			m_nFriendID;
	CString			m_sFriendName;
	int				m_nGroupIndex;

	CTransparentLabel m_lblTip;
	CBmpBtn m_btnOK;
};
