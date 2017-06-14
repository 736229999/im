#pragma once

#include "RichEditCtrlEx.h"
#include "improtocol.h"
#include "afxwin.h"
#include "ImageToolBar.h"
#include "clusterpanel.h"
#include "simplefiletranssession.h"
#include "customtoolbar.h"

#include "..\controls\skinctrl.h"
#include "buttonext.h"
#include "..\ClusterParamSetting.h"
#include "..\..\SpaceManagement\public\public_controls\buttonext.h"
#include "PopupHistoryDlg.h"

#define WM_CLOSETABCTRL_CLOSE (WM_USER+424) //tab�ؼ���Ϣ��¼�رյ���Ϣ

// CP2PSendMsgDlg �Ի���
class CClusterSendMsgDlg : public CXSkinDlg, public CSimpleFileTransSession
{
	DECLARE_DYNAMIC(CClusterSendMsgDlg)
	DECLARE_HMG_SCROLLBAR();

public:
	CClusterSendMsgDlg(CLUSTER_INFO *pClusterInfo, CWnd *pParent);   // ��׼���캯��
	virtual ~CClusterSendMsgDlg();

	enum { IDD = IDD_CLUSTER_SENDMSG };
	
	struct tagWaitImageFileStruct
	{
		char    szFile[255] ;
		DWORD   time_recv   ;

		tagWaitImageFileStruct()
		{
			memset(this,0,sizeof(tagWaitImageFileStruct));
		}
	};

public:
	DWORD GetActiveTickCount() { return m_dwTickCountActive; }

	void ProcessMsg(const XT_MSG & msg);
	void AddMsgTip(CString strTip,COLORREF clrText=RGB(122,122,122));
	bool RecvMagicFace(uint32 from_id, CString strFaceName, CString strFaceUrl);
	void ShowMagicFace(const char *face_name);

	void Download(const vector<DOWNLOAD_OBJ>& arrTask);
	afx_msg LRESULT OnSnapShot(WPARAM wParam,LPARAM lParam);
public:
	virtual BOOL DestroyWindow();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnHelpClick();
	afx_msg void OnFaceIconClicked();
	afx_msg void OnFontBoldClicked();
	afx_msg void OnFontItalicClicked();
	afx_msg void OnSendPicClicked();
	afx_msg void OnFontUnderLineClicked();
	afx_msg void OnBnClickedBtnColor();
	afx_msg void OnBnClickedBtnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnHistory();
	afx_msg void OnStnClickedStaticTitle();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedFontBar();
	afx_msg void OnBnClickedAction();
	afx_msg void OnScreenCopy();
	afx_msg void OnMagicFace();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnBnClickedBtnDiscuss();
	afx_msg void OnBnClickedBtnClusterAlubm();
	afx_msg void OnBnClickedBtnClusterHome();
	afx_msg void OnStnClickedImageTitle();
	afx_msg void OnButtonLink();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnUserLinkClick(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnUserLinkRBClick(WPARAM wParam,LPARAM lParam);
	//afx_msg LRESULT OnSetHeadTitle(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedBtnSendEx();

	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

	LRESULT OnEmotionSelNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnMagicFaceNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnMagicFaceClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnThreadNotify(WPARAM , LPARAM );
	LRESULT	OnStartDownloadFace(WPARAM w, LPARAM l);
	LRESULT OnDownloadFaceOK(WPARAM w, LPARAM l);
	LRESULT OnDownloadProcess(WPARAM w, LPARAM l);
	LRESULT OnInsertMsg(WPARAM w, LPARAM l);
	LRESULT OnCloseTabCtrlClose(WPARAM wParam,LPARAM lParam);
	LRESULT OnKeyDownReturn(WPARAM wParam,LPARAM lParam);

	void OnCMemberInd(const char *data, WORD wRet);
	void OnSetCMemberAck(const char *data,WORD wRet);
	void OnUserInfoGetAck(const char *data,WORD wRet);
	void OnUserInfoModified(const char *data,WORD wRet);
	void OnStatusModAck(const char *data,WORD wRet);
	void OnLoginOut(const char *data, WORD wRet);
	void OnClusterMembersAck(const char *data,WORD wRet);
	void OnStartUploadCPicAck(const char * data,WORD wRet);
	void OnUploadCPicAck(const char * data,WORD wRet);
	void OnDownloadCPicAck(const char * data,WORD wRet);
	// ������Ϣ
	void OnBnClickedBtnSend();

protected:
	virtual void SF_InitXtMsg(XT_MSG &msg);
	virtual void SF_OnRecvFileComplete(const char *file_name);
	virtual void SF_OnAbortFile(const char *file_name);
	virtual void SF_OnRecvFileReq(char *file_name);

	virtual void DrawCaptionBar();
	virtual void InitResource();

private:
	int  DoModal();
	void UpdateP2PPanel();
	void AdjustUI();
	void UpdateXFont(const XT_FONT &xf);
	void AddClusterTip();
	void AddTalkMsg(const XT_MSG &msg,bool isHistoryMsg);
	bool GetRedrawRgn(CRgn &rgn);

	void StopDownload(const CString& sFileID,bool bTerminate = true);
	void ProcTalkMsg(const CString & sMsg,CString& sShowMsg);
	static UINT DownloadPicProc( LPVOID pParam );
	void LoadLastTalkMsg();

	// ��ݶ�����Ϣ��Ӧ
	void OnCommandFastWordMenu(int id);
	afx_msg void OnBnClickedFastword();


private:
	struct _tagMsg
	{
		XT_MSG msg;
		int    type;
  
		bool operator < (const _tagMsg &o) const
		{
			return msg.message_sequence_id < o.msg.message_sequence_id;
		}
	};

	void PushMsg(const _tagMsg & msg);
	void PopMsg();

	list<_tagMsg>                    m_lstMsgRecv        ;

public:
	void UpdateMemberList(CLUSTER_INFO* pClusterInfo) 
	{
		if( m_wndClusterPanel.GetSafeHwnd() )
		{
			m_pClusterInfo = pClusterInfo;
			m_wndClusterPanel.UpdateClusterList();	
		}
	}

	void UpdateProfile(CLUSTER_INFO* pClusterInfo) 
	{
		if( m_wndClusterPanel.GetSafeHwnd() )
		{
			m_pClusterInfo = pClusterInfo;
			m_wndClusterPanel.UpdateClusterProfile(pClusterInfo);	
		}
	}
private:
	HICON				m_hIcon;
	CLUSTER_INFO		*m_pClusterInfo  ;

	CRichEditCtrlEx		m_reditSend		;
	CRichEditCtrlEx		m_reditHistory	;
	CSplitterControl	m_wndSplitter1	;
	CBmpBtn				m_btnHistory	;
	CBmpBtn				m_btnClose		;
	CSplitBtn			m_btnSend		;

	/*CFlatButton*/ 
	CBmpBtn             m_btnClusterCfg;
	CFlatButton			m_btnDiscuss;
	CFlatButton			m_btnAlbum;
	CFlatButton			m_btnHome;

	//CCaptionBar			m_captionBar	;
	CStatic				m_wndHide		;

	CP2PTalkToolBar		m_wndTalkToolBar;
	CSendMsgActionToolBar	m_wndActionToolBar;	// ����������
	CP2PToolBar			m_wndToolBar;


	CPopupHistoryDlg    m_wndPopupHistory;
	CClusterPanel		m_wndClusterPanel;

	int					m_nSplitterPos	;

	vector<tagWaitImageFileStruct>  m_vecWaitImageFiles	;
	map<CString,CString>            m_mapMagicFace		;
	DWORD						    m_dwTickCountActive	;
	vector<int>					    m_vecRecords        ;
	
	DWORD							m_dwProgressBar     ;
	HACCEL							m_hAccel;	// ���̼��ټ�

	CBitmap*			m_pBitmapHead	;
	CFaceIconCtrl		m_ctrlFaceIcon	;		//Ⱥͷ���

	uint32				m_uPopupHistoryWidth;	//�Ҳ������¼���ڿ��
	bool				m_bPreShowHistory;
	bool				m_bShowHistory;			//�Ƿ���ʾ�����¼����

	//enum DOWNLOAD_STATE{
	//	DS_DOWNLOADING = 0,
	//	DS_SUCCESS,
	//	DS_FAILED 
	//};
	//struct DOWNLOAD_OBJ{
	//	CString	sUrl;	// �������ļ�
	//	CString	sSaved;	// �����ļ������ַ
	//	HANDLE	handle;	// �߳̾��
	//	UINT	timer;	// �߳�����ʱ��
	//	UINT	state;	// 0.��������;1.���سɹ�;2.����ʧ��
	//};

	// ��ǰ���������ļ����̵߳��ֵ�
	map<CString,DOWNLOAD_OBJ*>	m_dicDownloadObj;

	HICON	  m_hBigIcon, m_hSmallIcon;
public:
	afx_msg void OnBnClickedBtnCfg();

	afx_msg void OnMenuViewClusterInfo();
	afx_msg void OnMenuClusterProfile();
	afx_msg void OnMenuClusterExit();
	afx_msg void OnMenuRecNotice();
	afx_msg void OnMenuRecOnly();
	afx_msg void OnMenuUnreadShow();
	afx_msg void OnMenuRejectMsg();
	afx_msg void OnBtnMsgLog();
	
	////Ⱥ��Ƭ�޸�
	void OnSetCMProfileAck(const char *data, WORD wRet);
	
	////Ⱥ���޸�Ⱥ��Ϣ
	void OnSetClusterInfoAck(const char *data,WORD wRet);

	////Ⱥ��Ƭ��ȡ
	void OnGetCMProfileAck(const char *data, WORD wRet);

	afx_msg void OnMove(int x, int y);

private:
	void OnBnClickedBtnHotkey();
	void OnEnter();
	void OnCtrlenter();

private:
	vector<string> m_vecFastWord;
public:
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
