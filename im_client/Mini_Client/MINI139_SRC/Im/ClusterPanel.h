#pragma once
#include "afxcmn.h"
#include "CustomListCtrl.h"
#include "IMProtocol.h"
#include "..\controls\skinctrl.h"
#include "P2PInviteDlg.h"
#include "..\TabWndSearch.h"
#include "RichEditCtrlEx.h"
// CClusterPanel �Ի���
class CEditNoticeDlg;
class CClusterPanel : public CDialog
{
	DECLARE_DYNAMIC(CClusterPanel)
	DECLARE_HMG_SCROLLBAR()

public:
	CClusterPanel(CLUSTER_INFO * pCluster, CWnd* pParent );   // ��׼���캯��
	virtual ~CClusterPanel();

// �Ի�������
	enum { IDD = IDD_CLUSTER_PANEL };

public:
	void			UpdateClusterList();
	void            UpdateClusterProfile(CLUSTER_INFO* pClusterInfo);
	afx_msg void	ShowUserMenu(int uid);
	void			SelUser(int uid);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnLbnDblclkListCluster();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnStnClickedStaticSetting();
	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

	void InvalidateFamilyHead();
	void OnSetCMemberAck(const char *data,WORD wRet);
protected:
	CClusterListCtrl   m_wndList	;
	CLUSTER_INFO *     m_pCluster	;

	CHyperBtn          m_btnNotice;
	CTransparentLabel  m_wndInfo;
	CRichEditCtrlEx	   m_edtIntro;

	CBitmap       *    m_pBitmapFamilyFlag ;
	CResBitmap         m_bmpDefaultFamilyFlag;
	CHyperBtn m_btnAdd;
	DWORD m_nOnline;

	CPngBtn m_btnAddMember;
	CPngBtn m_btnSearchMember;


	CFlatEdit m_edtSearch;

	CTabWndSearch m_wndTabSearch;
	XT_FONT m_NoticeFont;

	CEditNoticeDlg *m_pEditNoticeDlg;

public:
	afx_msg void OnLbnSelchangeListCluster();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnRefresh();
	
	afx_msg void OnStnClickedStaticNotice();
	afx_msg void OnStnClickedStaticAdd();
	afx_msg void OnStnClickedStaticSearch();

	afx_msg void OnBnClickedBtnAdd();
	void OnBnClickedBtnSearch();
	
	void   UpdateNotice(LPCTSTR szNotice);

	afx_msg void OnEnSetfocusEditSearch();
	afx_msg void OnEnKillfocusEditSearch();

	// �༭�����Զ�����Ϣ
	afx_msg LRESULT OnEditNoticeMsg(WPARAM wp, LPARAM lp);

	// ����Ⱥ��������Ӧ��Ϣ
	void OnSetClusterInfoAck(const char *data,WORD wRet);

	// ��ȡȺ��Ϣ
	void OnClusterInfoAck(const char *data,WORD wRet);

	// ����ϵͳ��Ϣ
	void OnRecvSvrMsg(const char *data,WORD wRet);


	////�û�״̬�ı�ʱ����Ϣ
	////void OnStatusModAck(const char *data,WORD wRet);

	void OnFriendStatusChange(const char *data,WORD wRet);

	////Ⱥ��Ƭ�޸ĺ�Ⱥ�Ի����г�Ա��Ƭ��Ӧ�仯
	void OnGetCMProfileAck(const char *data, WORD wRet);

	////Ⱥ��Ϣ��������
	void OnSetClusterMsgRecAck(const char *data, WORD wRet);
	
	// ��������
	void OnDestroy();
	// �ƶ�����༭����
	void CClusterPanel::MoveNoticeDlg();
};
