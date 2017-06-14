#pragma once


// CPopupHistoryDlg dialog
#include "../Resource.h"
#include "afxcmn.h"
#include "RichEditCtrlEx.h"
#include "HistoryFindToolBar.h"
#include "HistoryToolBar.h"
#include "LocalData.h"

#include "..\..\..\SpaceManagement\public\public_controls\buttonext.h"
#include "..\controls\skinctrl.h"
#include "..\..\..\HmgSkin\HmgSkin\CloseTabCtrl.h"
#include "..\..\..\HmgSkin\HmgSkin\ColumnCtrl.h"
#include "..\..\..\hmgskin\export\include\xbutton.h"

#define WM_CLOSETABCTRL_CLOSE (WM_USER+424) //tab�ؼ���Ϣ��¼�رյ���Ϣ

class CPopupHistoryDlg : public CDialog
{
	DECLARE_DYNAMIC(CPopupHistoryDlg)
	DECLARE_HMG_SCROLLBAR()

public:
	CPopupHistoryDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPopupHistoryDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// Dialog Data
	enum { IDD = IDD_DIALOG_HISTORY };

	enum SelTabCtrl
	{
		SEL_INFO = 0,
		SEL_HISTORY,
	};

	enum SelHistoryTabCtrl
	{
		SEL_ALL = 0,
		SEL_PIC,
		SEL_FILE,
	};

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	void OnPaint();
	// tab��ѡ�Ÿı��¼�
	afx_msg LRESULT OnTabBtnCtrlSelChange(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnFind();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	afx_msg LRESULT ShowFindToolBar(WPARAM, LPARAM);
	afx_msg LRESULT ShowHistoryFirst(WPARAM, LPARAM);
	//afx_msg LRESULT ShowHistoryPrev(WPARAM, LPARAM);
	//afx_msg LRESULT ShowHistoryNext(WPARAM, LPARAM);
	//afx_msg LRESULT ShowHistoryLast(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()

public:
	LRESULT OnCloseTabCtrlClose(WPARAM wParam,LPARAM lParam);
public:
	void		SetFriedInfo(FRIEND_INFO* fi);
	void		SetClusterInfo(CLUSTER_INFO* ci);
	void		LoadAllMsg();

public:
	void        UpdateEditRect(CRect &rcParent);
private:
	void		InitUserInfoTab();
	void		InitHistoryTab();
	void		AdjustUI();
	void		AdjustFindToolBar();					//����"ȫ��"�����¼�༭��Ͳ��ҹ�����
	void 		AdjustBottomToolBar(INT32 iCurPage, INT32 iAllPage);
	/*ֻ��ȫ����Ϣ�Ƿ�ҳ��ʾ��ͼƬ���ļ�����һҳ��ʾ*/
	void 		MsgAllToPage(INT32 iCurPage, INT32 iAllPage, const MSG_LIST &mlRecord);				//"����"�����¼ת��iPageҳ
	void		AddP2PTalkMsg(const XT_MSG &msg);
	void        ShowTalkMsgAll(const XT_MSG &msg);
	void        ShowTalkMsgPic(const XT_MSG &msg);
	void        ShowTalkMsgFile(const XT_MSG &msg);
	void 		MsgPicFileUpdate();
	void 		UpdateSearchMsg(const CString& sKeyword, CTime tMsgBegin);
private:
	
	//CHistoryFindToolBar	m_wndFindToolBar;				//���ҹ�����
	/*���ҹ�����*/
	CPngBtn m_btnFind;
	CFlatEdit m_editKeyword;
	CGraphComboBox m_comboRange;
	CTransparentLabel m_lblRange;
	CTransparentLabel m_lblKeyword;
	
	
	CHistoryToolBar		m_wndToolBar;					//�ײ���ť����

	CRichEditCtrlEx		m_editHistoryAll;				//����TAB�ؼ��еġ�ȫ���������¼
	CRichEditCtrlEx		m_editHistoryPic;				//����TAB�ؼ��еġ�ͼƬ�������¼
	CRichEditCtrlEx		m_editHistoryFile;				//����TAB�ؼ��еġ��ļ��������¼

	bool				m_bIsSearch;
	bool				m_bIsFindToolBarOn;				//���ҹ������Ƿ���
	bool				m_bIsSearchMsg;

	UINT32				m_nToolBarHight;
	UINT32				m_nToolBarWidth;
	UINT32				m_nFindToolBarHight;

	FRIEND_INFO*		m_pFriendInfo;
	CLUSTER_INFO*		m_pClusterInfo;

	MSG_LIST			m_mlRecordAll;					//��ȫ���������¼���еļ�¼
	INT32				m_iPageAll;						//��ȫ���������¼��ĵ�ǰҳ��
	INT32				m_iPageAllTotal;				//��ȫ���������¼���ҳ��

	MSG_LIST			m_mlRecordSearch;				//�����ҡ������¼���еļ�¼
	INT32				m_iPageSearch;
	INT32				m_iPageSearchTotal;

	bool				m_bInitial;

	CCloseTabCtrl		m_closeTab;
	CColumnCtrl			m_colCtrl;
	HBRUSH m_hBkGround;
	CFont m_font;

public:
	afx_msg void OnBnClickedBtnRet();
};
 
