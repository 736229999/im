#pragma once
#include "..\resource.h"
#include "..\im\impublic.h"
#include "..\..\HmgSkin\XListCtrl.h"
//#include "..\..\HmgSkin\HmgSkin\ButtonEx.h"
#include "..\im\LocalData.h"
#include <mmsystem.h>
#include "..\..\..\HmgSkin\HmgSkin\CheckButton.h"
// CSysCommonVoice �Ի���
////

#include "..\..\..\DuiLib\UIlib.h"

class CSysCommonVoice : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UICommonVoiceCfg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("VoiceSetting.xml");  }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	void    Save();

	afx_msg void OnBnClickedBtnReceiveMsgVoice();
	afx_msg void OnBnClickedBtnFriendOnlineVoice();
	afx_msg void OnBnClickedBtnSysMsgVoice();
	afx_msg void OnBnClickedBtnShockScreenVoice();
	afx_msg void OnBnClickedBtnReceiveMsgBrowse();
	afx_msg void OnBnClickedBtnFriendOnlineBrowse();
	afx_msg void OnBnClickedBtnSysMsgBrowse();
	afx_msg void OnBnClickedBtnShockScreenBrowse();

private:

	DuiLib::CButtonUI *m_pBtnRecvOnce;  ////����
	DuiLib::CButtonUI *m_pBtnOnline;    ////����
	DuiLib::CButtonUI *m_pBtnSysMsg;    ////����
	DuiLib::CButtonUI *m_pBtnShake;     ////����

	DuiLib::COptionUI *m_pCheckRecvBtn; ////Ĭ��״̬
	DuiLib::COptionUI *m_pCheckOnlineBtn; ////Ĭ��״̬
	DuiLib::COptionUI *m_pCheckSysBtn; ////Ĭ��״̬
	DuiLib::COptionUI *m_pCheckShakeBtn; ////Ĭ��״̬

	DuiLib::CEditUI *m_pEditRecvPath;   ////·�����
	DuiLib::CEditUI *m_pEditOnlinePath; ////·�����
	DuiLib::CEditUI *m_pEditSysPath;    ////·�����
	DuiLib::CEditUI *m_pEditShakePath;  ////·�����

	DuiLib::CButtonUI *m_pBtnVoiceChose;  ////ѡ���ļ�
	DuiLib::CButtonUI *m_pBtnOnlineChose; ////ѡ���ļ�
	DuiLib::CButtonUI *m_pBtnSysChose;    ////ѡ���ļ�
	DuiLib::CButtonUI *m_pBtnShakeChose;  ////ѡ���ļ�
};
////class CSysCommonVoice : public CDialog
////{
////	DECLARE_DYNAMIC(CSysCommonVoice)
////
////public:
////	CSysCommonVoice(CWnd* pParent = NULL);   // ��׼���캯��
////	virtual ~CSysCommonVoice();
////	void Save();
////
////// �Ի�������
////	enum { IDD = IDD_WIZARD_SYS_COMMON_VOICE_LOG };
////
////protected:
////	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
////	virtual BOOL OnInitDialog();
////	void OnOK()	{}
////	void OnCancel() {}
////	DECLARE_MESSAGE_MAP()
////public:
////	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
////	afx_msg void OnDestroy();
////	// ������ˢΪ��ɫ
////	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
////	// ���Ʊ��
////	afx_msg void OnPaint();
////	afx_msg void OnSize(UINT nType, int cx, int cy);
////
////	afx_msg void OnBnClickedBtnReceiveMsgVoice();
////	afx_msg void OnBnClickedBtnFriendOnlineVoice();
////	afx_msg void OnBnClickedBtnSysMsgVoice();
////	afx_msg void OnBnClickedBtnShockScreenVoice();
////	afx_msg void OnBnClickedBtnReceiveMsgBrowse();
////	afx_msg void OnBnClickedBtnFriendOnlineBrowse();
////	afx_msg void OnBnClickedBtnSysMsgBrowse();
////	afx_msg void OnBnClickedBtnShockScreenBrowse();
////
////	// ������ڷ�λ������
////	void CalcTablePosData();
////
////private:
////	// ���Ʊ����������
////	CRect m_rcTable;
////	CPoint m_ptTop1, m_ptTop2, m_ptTop3;
////	CPoint m_ptBottom1, m_ptBottom2, m_ptBottom3;
////	CPoint m_ptLeft1, m_ptLeft2, m_ptLeft3, m_ptLeft4;
////	CPoint m_ptRight1, m_ptRight2, m_ptRight3, m_ptRight4;
////
////	// ���cell
////	CRect m_arrRcCell[5][4];
////
////	// ������ť
////	CPngBtn m_btnVoice_ReceiveMsg, m_btnVoice_FriendOnline, m_btnVoice_SysMsg, m_btnVoice_ShockScreen;
////
////	// ״̬��
////	CCheckButton m_check_ReceiveMsg, m_check_FriendOnline, m_check_SysMsg, m_check_ShockScreen;
////
////	// ����ļ���ť
////	CPngBtn m_btnBrowse_ReceiveMsg, m_btnBrowse_FriendOnline, m_btnBrowse_SysMsg, m_btnBrowse_ShockScreen;
////
////	// �����ļ�·��
////	CFlatEdit m_pEditRecvPath->, m_pEditOnlinePath->, m_pEditSysPath->, m_edit_ShockScreen;
////};
