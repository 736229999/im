// ----------------------------------------------------------------
// �ļ�:SendMsgActionDlg.h
// �汾: 
// ����: xy
// ����: 2006-6-13
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
// 
// ----------------------------------------------------------------

#pragma once

#include "../resource.h"
#include "../controls/SkinCtrl.h"
#include "ActionTextReader.h"
#include "buttonext.h"

class CSendMsgActionToolBar : public CDialog
{
	DECLARE_DYNAMIC(CSendMsgActionToolBar)

public:
	CSendMsgActionToolBar(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSendMsgActionToolBar();

	enum { IDD = IDD_SENDMSG_ACTION };

	//------------------------------------------------------
	// ��ö�������
	//------------------------------------------------------
	BOOL	GetActionData(ACTION_DATA& ad);
	//------------------------------------------------------
	// ���һ���������
	//------------------------------------------------------
	void	AddObject(const CString& sObject,int id);
	//------------------------------------------------------
	// ��ʾ��ʾ��Ϣ
	//------------------------------------------------------
	void	ShowToolTip(const CString& strTip);
	//------------------------------------------------------
	// ��õ�ǰѡ����������
	//------------------------------------------------------
	CString	GetCurSelObject();
	//------------------------------------------------------
	// ��������Ϊ��
	//------------------------------------------------------
	void	ResetAction();
	//------------------------------------------------------
	// �������������Ϊ��
	//------------------------------------------------------
	void	ResetObject();


protected:

	virtual void DoDataExchange(CDataExchange* pDX); 
	//------------------------------------------------------
	// ��ʼ�������б�
	//------------------------------------------------------
	void		InitActions();

	DECLARE_MESSAGE_MAP()

protected:

	vector<ACTION_ITEM>	m_actionList;	// �����ű��б�
	CActionTextReader	m_reader;		// �����ű�������
	CFlatButton			m_btnEditAction;// �༭�����ű���ť
	CFlatButton			m_btnLoadAction;// ���ض����ű���ť
	CToolTipCtrl		m_toolTip;		// ��ʾ�ؼ�	
	CString				m_sLastTip;		// �ϴε���ʾ��Ϣ
	CGraphComboBox		m_cmbObject;	// ������������˵�
	CGraphComboBox		m_cmbAction;	// ���������˵�
	CTransparentLabel	m_lblMsg;		// ��ʾ��ǩ

public:

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnEditaction();
	afx_msg void OnBnClickedBtnLoadaction();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeCmbActions();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

};
