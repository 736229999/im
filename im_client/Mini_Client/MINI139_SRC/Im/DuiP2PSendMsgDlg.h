#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"
#include "P2PSendMsgDlg.h"
#include "DuiP2PSendMsgDlg.h"
#include "afxwin.h"
#include "RichEditCtrlEx.h"
#include "improtocol.h"

#include "ImageToolBar.h"
#include "P2PUserPanel.h"
#include "P2PUserList.h"
#include "simplefiletranssession.h"
#include "advancedfiletranssession.h"
#include "..\mmtalk\mmtalker.h"
#include "customtoolbar.h"
#include "PopupHistoryDlg.h"

#include "..\controls\skinctrl.h"
#include "P2PFileTransProgressBar.h"
#include "UserIPWnd.h"
#include "buttonext.h"

#include <vector>
#include "explorer1.h"
#include "faceiconctrl.h"
#include "..\..\SpaceManagement\public\public_controls\buttonext.h"
#include "..\RoundFaceCtrl.h"
#include "..\CefWebUI.h"

#define TIMER_FASTWORDSELECTMENU 4003 //��ݶ��ﶨʱ�������ڵ�����ݶ���˵�֮�������ʧȥ�����ٴλ�ý���
#define TIMER_REDITSEND_TEXT_CHANGE 4004 //������ж����ݸı�Ķ�ʱ��������"/"֮���������Ŀ�ݶ���˵����ű仯
#define TIMER_SCREEN_COPY 4005 //��0.1��֮�����
#define TIMER_UPDATEURL 4006 //��0.5��֮��ˢ���Ҳ�ҳ
#define TIMER_EDIT_ENDDOWN 4007 //��ʾ��ʷ��Ϣ���϶������·�
#define FRM_TIMER_ID 4008 //gifͼƬ�ػ���
#define TIMER_GETFRIEND_INFO 4009 //��ȡ�û���Ϣ

////////����С�����///////////////////////////////////////
#define UNSAVED_SERVICELOG       0 //δ�������С��
#define SAVED_SERVICELOG         1 //�ѱ������С��
#define UNCOMPLETED_SERVICELOG   2 //���治�����ķ���С��
#define IS_BUYER_TRAN                 3 //�Ѿ�ת�ӵ����
///////////////////////////////////////////////////////////

// ���촰�ڸ�����Ϣ
struct StruExtraInfo
{
	string strProductID;
	string strOrderSeq;
};

class CCustSerTranDlg;
class CDuiP2PSendMsgDlg : public DuiLib::WindowImplBase, public CAdvancedFileTransSession, public CSimpleFileTransSession
{
	DECLARE_XT_MESSAGE_MAP()
	DECLARE_HMG_SCROLLBAR()
public:
	CDuiP2PSendMsgDlg(StruExtraInfo struExtInfo = StruExtraInfo());
	~CDuiP2PSendMsgDlg();
	BOOL CallbackNagive(DWORD dwThis, char *p);
	void ServiceLogWorn(DWORD dwStatus);
public:
	virtual LPCTSTR    GetWindowClassName() const   {   return _T("P2PSendMsgDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("P2PSendMsgDlg.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();

	void Notify(DuiLib::TNotifyUI& msg);
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	virtual LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

	virtual DuiLib::CControlUI* CreateControl(LPCTSTR pstrClassName)
	{
		//if (_tcsicmp(pstrClassName, _T("RichEditSendWnd")) == 0)
		//{
		//	CWndUI  *pUI  = new CWndUI;      
		//	RECT rc;
		//	rc.left = 0;
		//	rc.right = 0;
		//	rc.top = 0;
		//	rc.bottom = 0;
		//	m_reditSend.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | ES_WANTRETURN
		//		| WS_VSCROLL | ES_AUTOVSCROLL, rc, CWnd::FromHandle(GetHWND()), 10002);
		//	m_reditSend.SetLimitWordSize(400);//���Ϳ���������400����
		//	pUI->Attach(m_reditSend.GetSafeHwnd());         
		//	return pUI;
		//}

		//if (_tcsicmp(pstrClassName, _T("RichEditHistoryWnd")) == 0)
		//{
		//	CWndUI  *pUI  = new CWndUI;      

		//	RECT rc;
		//	rc.left = 0;
		//	rc.right = 400;
		//	rc.top = 0;
		//	rc.bottom = 249;
		//	m_reditHistory.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | ES_WANTRETURN | ES_READONLY
		//		| WS_VSCROLL | ES_AUTOVSCROLL, rc, CWnd::FromHandle(GetHWND()), 10001);

		//	pUI->Attach(m_reditHistory.GetSafeHwnd());           
		//	return pUI;
		//}

		if (_tcsicmp(pstrClassName, _T("TalkBarWnd")) == 0)
		{
			CWndUI  *pUI  = new CWndUI;      
			RECT rc;
			rc.left = 0;
			rc.right = 0;
			rc.top = 0;
			rc.bottom = 0;

			if(m_pwndTalkToolBar == NULL)
				m_pwndTalkToolBar = new CP2PTalkToolBar(m_reditSend, &g_ImFont, CWnd::FromHandle(GetHWND()));

			m_pwndTalkToolBar->Create(CP2PTalkToolBar::IDD, CWnd::FromHandle(GetHWND()));
			m_pwndTalkToolBar->MoveWindow(&rc);
			pUI->Attach(m_pwndTalkToolBar->GetSafeHwnd());           
			return pUI;
		}

		if (_tcsicmp(pstrClassName, _T("HistoryWnd")) == 0)
		{
			CWndUI  *pUI  = new CWndUI;      
			RECT rc;
			rc.left = 0;
			rc.right = 0;
			rc.top = 0;
			rc.bottom = 0;

			m_wndPopupHistory.Create(CPopupHistoryDlg::IDD, CWnd::FromHandle(GetHWND()));
			m_wndPopupHistory.MoveWindow(&rc);
			pUI->Attach(m_wndPopupHistory.GetSafeHwnd());          
			return pUI;
		}

		if (_tcsicmp(pstrClassName, _T("wkeWebkit")) == 0)
		{
			return new CCefWebUI();
		}

		if (_tcsicmp(pstrClassName, _T("ButtonGif")) == 0)
		{
			return	new DuiLib::CButtonGifUI();
		}
		return NULL;
	}
public:
	friend class CMessageHandleWnd;

public:
	struct tagWaitImageFileStruct
	{
		char    szFile[255] ;
		char	type		;	// 0.����ͼƬ��1.����ͼƬ
		DWORD   time_recv   ;
		tagWaitImageFileStruct()
		{
			memset(this,0,sizeof(tagWaitImageFileStruct));
		}
	};

	TP2PMode GetP2PMsgMode() { return m_pmP2PMode;	}

	static /*std::tr1::shared_ptr<*/CDuiP2PSendMsgDlg*/*>*/ NewDlg(FRIEND_INFO* pFriend, TP2PMode msgMode, CWnd *pParent, StruExtraInfo struExtInfo = StruExtraInfo(), int nUnReadNum = 0);
	void SetExtInfo(StruExtraInfo struExtInfo);
	void SetFriend(FRIEND_INFO *pFriend, TP2PMode msgMode, CWnd *pParent,int nUnReadNum = 0);

	void UpdateFriendInfo(FRIEND_INFO *pFriend);

	// ��ʾ���������ְ�ť
	void ShowOrHideScoreBtn(BOOL bShow);

	//�رմ���ʱ���͹ر���Ϣ
	void SendMsgDlgNotify();

	//���Ϳ����ý���
	void SetEditFocus();
public:

	friend UINT DownloadPicProc( LPVOID pParam );
	static UINT UpdateUrlThread( LPVOID pParam );

public:
	virtual BOOL DestroyWindow();
	DWORD GetActiveTickCount() { return m_dwTickCountActive; }

	void SetHangupState(bool bIsVisable);
	void HangupCancel();
	void ProcessMsg(const XT_MSG & msg);
	void UpdateCluserInfo(uint32 cid);
	void SaveClusterMsg(const XT_MSG &msg);

	bool RecvMagicFace(uint32 friend_id, CString strFaceName, CString strFaceUrl);
	void RecvSendFileReq(const char *data, WORD wRet);
	void ShowMagicFace(const char *face_name);

	CLUSTER_INFO * GetClusterInfo() { return &m_clusterInfo;}
	FRIEND_INFO * GetFriendInfo() { return &m_FriendInfo; }

	// ������ʱȺ��
	void	CreateTempCluster(const CLUSTER_INFO& ci,int nGroupIndex);
	// ���ص�ǰ��ʱȺ�Ĵ����Ӧ�����id������ǰΪ���Ա��ʱȺ�Ĵ���ʱ����Ч��
	int		GetTempGroupTalkIndex() const
	{ return m_nTempGroupTalkIndex; };

	// ��ʼ��������
	void Download(const vector<DOWNLOAD_OBJ>& arrTask);
	void ChangeHeadStatus(FRIEND_INFO *pFriend);
#ifdef _BUSINESS_VESION
	void AddTranMsg(uint32 fromId, const char *pOrderId, const char *pReason);
#endif	
	void UpdateBk();
	void InitPos();

	void UpdateUrl();// ����url
	void UpdateFriendState();// �����û�����״̬����Ϊ����
#ifdef _CS_PLAT_EX_
	// �����Ƿ��Ѿ�ת��
	BOOL GetIsTran();//�û��Ƿ��Ѿ���ת��
	void UpdateIsTran();//�û�ת��״̬��ԭ
	BOOL IsMonitorOn(int nGroupIndex);//�ж��û����ڵ����Ƿ���Ա����
	void SetToolEnabled(bool bIsEnable);
#endif

protected:
	//virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnHelpClick();

	afx_msg void OnSendFile();
	afx_msg void OnInvite();
	afx_msg void OnFaceIconClicked();
	afx_msg void OnSendPicClicked();
	afx_msg void OnBnClickedFontBar();
	afx_msg void OnMagicFace();
	afx_msg void OnLinkResClicked();
	afx_msg void OnScreenCopy();
	afx_msg void OnScreenCopyEx();
	afx_msg void OnScreenCopyOption();
	afx_msg void OnBnClickedBtnSendEx();
	afx_msg void OnBnClickedBtnHistory();
	afx_msg void OnBnClickedButtonShake();
	afx_msg void OnUpdateFontStyle(CCmdUI *pCmdUI);
	afx_msg void OnBnClickedBtnCard();
	afx_msg void OnBnClickedBtnVisit();
	void OnFinalMessage(HWND hWnd);
	afx_msg void OnBnClickedBtnFold();
	afx_msg void OnAccIp();
	afx_msg void OnBnClickedButtonHistory();

	// ���������ۡ���ť
	afx_msg void OnBnClickedButtonScore();
	afx_msg LRESULT OnSnapShot(WPARAM wParam,LPARAM lParam);

	LRESULT OnEmotionSelNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnMagicFaceNotify(WPARAM wParam ,LPARAM lParam);
	LRESULT OnQueryPeerOnline(WPARAM wParam, LPARAM lParam);
	LRESULT OnMagicFaceClick(WPARAM wParam,LPARAM lParam);
	LRESULT	OnStartDownloadFace(WPARAM w, LPARAM l);
	LRESULT OnDownloadFaceOK(WPARAM w, LPARAM l);
	LRESULT OnDownloadProcess(WPARAM w, LPARAM l);
	LRESULT OnInsertMsg(WPARAM w, LPARAM l);
	LRESULT OnUserDragDropFile(WPARAM w,LPARAM l);
	LRESULT OnThreadNotify(WPARAM wParam,LPARAM lParam);

	LRESULT OnRecvFileOK(WPARAM wParam,LPARAM lParam);
	LRESULT OnRecvFileFailed(WPARAM wParam, LPARAM lParam);
	LRESULT OnSendFileOK(WPARAM wParam,LPARAM lParam);
	LRESULT OnSendFileFailed(WPARAM wParam,LPARAM lParam);

	LRESULT OnCloseTabCtrlClose(WPARAM wParam,LPARAM lParam);

	void OnFriendStatusChange(const char *data,WORD wRet);
	void OnFriendOffLine(const char *data,WORD wRet);
	void OnGetClusterMember(const char *data,WORD wRet);
	void OnCMemberInd(const char *data, WORD wRet);
	void OnClusterDismissInd(const char *data,WORD wRet);
	void OnStatusGetAck(const char *data,WORD wRet);
	void OnStatusModAck(const char *data,WORD wRet);
	void OnLogOut(const char *data,WORD wRet);
	void OnFriendInfoGet(const char *data, WORD wRet);
	// ��ʼ�ϴ�ȺͼƬӦ��
	void OnStartUploadCPicAck(const char * data,WORD wRet);
	// �ϴ�ȺͼƬӦ��
	void OnUploadCPicAck(const char * data,WORD wRet);
	// ����ȺͼƬӦ��
	void OnDownloadCPicAck(const char * data,WORD wRet);
	// ��ʱȺ������Ӧ��
	void OnCreateTempClusterAck(const char *data,WORD wRet);
	// ��ʱȺ��Ա������Ӧ��
	void OnTempCMembersSetAck(const char *data,WORD wRet);
	// ��ȡ���ѻ�������
	void OnGetFriendBasicInfoAck(const char *data,WORD wRet);
	// ��ȡת��֮ǰ��30�������¼
	void OnWebMoreMsgAck(const char *data,WORD wRet);
#ifdef _CS_PLAT_EX_
	// ���ܻ�ȡ�ͷ���ǰ�������
	void OnMonitorSetBuyerInd(const char *data,WORD wRet);
	//���ܻ�ȡ�ͷ��Ӵ������
	void OnMonitorStartAck(const char *data,WORD wRet);
	// ���ܻ�ȡ�ͷ��Ķ���������
	void OnMonitorStartInd(const char *data,WORD wRet);
	// ����ȡ����ؿͷ�Ӧ��
	void OnMonitorEndAck(const char *data,WORD wRet);
	// �����յ�ȡ����ؿͷ���Ϣ
	void OnMonitorEndInd(const char *data,WORD wRet);
	//���ܲ���ỰӦ��
	void OnMonitorInsertChatAck(const char *data,WORD wRet);
	// �ͷ�����Ҷ�Ҫ��ȡ���ܲ���ĻỰ
	void OnMonitorInsertChatInd(const char *data,WORD wRet);
	//���ܽ��ֻỰӦ��
	void OnMonitorTranChatAck(const char *data,WORD wRet);
	//�ͷ�������յ����ܽ��ջỰ
	void OnMonitorTranChatInd(const char *data,WORD wRet);
	//void OnGetCSIDFromUserID(const char *data,WORD wRet);
#endif
	// ������ʱȺ��������
	void SetClusterMember(uint32 clusterId);
	// p2p����Ӧ��
	void OnP2PTestAck(const char * data, WORD wRet);
	// ���Ͱ�ť
	void OnBnClickedBtnSend();

#ifdef _BUSINESS_VESION
	void OnGetNearCommCustRemarkAck(const char *data,WORD wRet);
	void OnUpdateNearCommCustRemarkAck(const char *data,WORD wRet);
	// �յ��ͷ�ת��ȷ�ϰ�
	void OnRecvCustSerTranAckInd(const char * data, WORD wRet);
#endif	

protected: //virtual derived from CFileSession
	virtual void SF_InitXtMsg(XT_MSG &msg);
	virtual void SF_OnAbortFile(const char *file_name);
	virtual void SF_OnRecvFileComplete(const char *file_name);
	virtual void SF_OnRecvFileReq(char *file_name);
	virtual void SF_OnRecvFilePacket(char * file_name, int file_id );

	virtual void AF_OnRecvSpeedNotify(uint32 id,int nBPS,float fPercent);
	virtual void AF_OnSendSpeedNotify(uint32 id,int nBPS,float fPercent);
	virtual void AF_OnSendOK(uint32 id);
	virtual void AF_OnDownloadOK(uint32 id);	
	virtual void AF_OnDownloadFailed(uint32 id,FILE_TRAN_ERR err) ;	
	virtual void AF_OnSendFailed(uint32 id,FILE_TRAN_ERR err);
	virtual void AF_OnRecvAbortTranReq(const P2P_ABORT_TRAN_REQ & req);
	virtual void AF_OnSendFileReturn(uint32 fileId,bool bAccept);
	virtual void AF_OnSendStatusNotify(uint32  id,TASK_STATUS status);
	virtual void AF_OnRecvStatusNotify(uint32 id,TASK_STATUS status);
	virtual bool AF_CanTrans(uint32  id);
	virtual void OnConnectTalkOk(bool bOk);
	virtual void OnAcceptTalkOk(bool bOk);

public:
	void UpdateSignature();
	void OnRecvServerAck(const char *data,WORD wRet);
	// ֪ͨ����˴���һ�����촰��
	void NotifyServer_OpenedMsgDlg();
	void UpdateFastWordInfo();//ˢ�����п�ݶ���
	void AddTalkMsg(const XT_MSG &msg,bool isHistoryMsg,int nUnReadNum = 0);
	void InsertFileInfoBar(DWORD dwFileID,bool bSendFile,CString strFileName, long nFileSize);
	void AddTranFromMsg(const XT_MSG &msg, CString strFromName, CString strToName);
	BOOL CloseAllDlg();//�رմ��ڣ������رմ��ڵ���ʾ��
	void SetWindowMax(BOOL bIsMax);//���ô�����󻯻�����С��
	void SetCsBuyerOffline();
	void RbuttonDownWeb();
	BOOL CanTran();
	void TranCuster(int nTranId);
	void AddTranRefuse(string strFromName);
	void SetTranPre(bool bIsTranPre);
	void InsertMsg(int nServiceId, int nCusterId);
#ifdef _CS_PLAT_EX_
	// ���ֻỰ�󣬴򿪿ͻ����촰��
	static unsigned __stdcall ThreadTranChat(void * pParam);
	int GetMonitorBuyerId();
	// �򿪿ͻ����촰��
	LRESULT OnOpenTranChatMsgDlg(WPARAM w,LPARAM l);
	void CloseMonitor();
	void SetHistoryText(CString strText);
#endif
protected:
	void ShowDlgMax(BOOL bIsShowMax);//��󻯻�����С��
	void ShowDlgTitle();
	void LoadLastTalkMsg(int nUnReadNum = 0);
	void AdjustUI();
	void ShowFileTransInfoBar(bool bShow);
	void UpdateP2PPanel();
	void DrawControlLine();
	int  InitFontToolBar();
	void AddMsgTip(CString strTip,COLORREF clrText=RGB(122,122,122));
	void AddFileOpenTip();

	void AddCMemberTip(XT_USER_INFO *user, int action/*1add, 2delete*/);
	void AddP2PTip();

	bool GetRedrawRgn(CRgn & rgn)		;
	void SetBtnFolderBitmap();

	void RefreshFriendStatus();
	//------------------------------------------------------
	// �������͵���Ϣ���ʽת��Ϊ��Ҫ������ʾ����Ϣ����Ҫ���͵���Ϣ
	//------------------------------------------------------
	void ProcTalkMsg(const CString & sMsg,CString& sShowMsg);
	// ֹͣ����ĳ���ļ�,����ָ���Ƿ�ǿ����ֹ�����߳�
	void StopDownload(const CString& sFileID,bool bTerminate = true);

	void LayoutUserWindow(DuiLib::CPoint& ptUserPanel, DuiLib::CSize& szUserPanel, DuiLib::CPoint& ptHistory, DuiLib::CSize& szHistory);
	void LayoutFileWindow(DuiLib::CPoint& ptSend, DuiLib::CSize& szSend, DuiLib::CPoint& ptHistory, DuiLib::CSize& szHistory,
		DuiLib::CPoint& ptSplitter, DuiLib::CSize& szSplitter, DuiLib::CPoint& ptToolBar, DuiLib::CSize& szToolBar);
	void LayoutButton(int nLeft, int nTop, DuiLib::CSize& szBtn, int nClientCx);

	virtual		void          InitResource();

	// ��ݶ�����Ϣ��Ӧ
	afx_msg void OnCommandFastWordMenu(int id);
	// ����˿�ݶ��ﰴť
	afx_msg void OnBnClickedFastword(DuiLib::TNotifyUI& msg);
#ifdef _BUSINESS_VESION
	// �˺Ź���
	afx_msg void OnBnClickedBtnHangup();
	// �ͷ�ת��
	afx_msg void OnBnClickedBtnCustserTran();
#endif

private:
	struct _tagMsg
	{
		XT_MSG msg;
		int    type;

		bool operator < (const _tagMsg &o) const
		{
			return msg.send_time < o.msg.send_time;
		}
	};

	void PushMsg(const _tagMsg & msg);
	void PopMsg();

	list<_tagMsg>                    m_lstMsgRecv        ;

private:
	HICON				m_hIcon;
	FRIEND_INFO		m_FriendInfo	;

	CBitmap*			m_pBitmapHead	;
	DuiLib::CRichEditUI*m_reditSend		;
	DuiLib::CRichEditUI*m_reditHistory	;
	CSplitterControl	m_wndSplitter1	;
	CBmpBtn				m_btnHistory	;
	CBmpBtn				m_btnClose		;
	CSplitBtn			m_btnSend		;
	CBmpBtn             m_btnFolder     ;

	CPngBtn				m_btnSendFile	;
	CPopupHistoryDlg	m_wndPopupHistory	;		//�Ҳ������¼����

	CCefWebUI*		m_wndAD;
	//DuiLib::CWebBrowserUI* m_wndAD;
	CString				m_strAdUrl		;
	XT_FONT			    m_fontLast		; //ѡ���ݶ���֮ǰ������
	BOOL                m_bIsFontLast;
	CHARRANGE	        m_crTran;//�ͷ�ת����ʾ��richedit�е�λ��
public:
#ifdef _BUSINESS_VESION
	CString				m_strNewComnCustRemarkClient;		//�����ϵ�ͻ��Ҳ౸ע(����)
	CString				m_strNewComnCustRemarkServer;		//�����ϵ�ͻ��Ҳ౸ע(�����)
#endif 
private:
	//CP2PUserPanel       *m_pwndUserPanel  ;
	CP2PUserList        m_wndUserList   ;
	CP2PTalkToolBar		*m_pwndTalkToolBar;

	uint32				m_uPopupHistoryWidth;	//�Ҳ������¼���ڿ��
	int					m_nSplitterPos	;
	int					m_nPreDlgHeight ;
	TP2PMode			m_pmP2PMode , m_pmPreP2PMode ;
	CLUSTER_INFO		m_clusterInfo   ;
	uint32				m_uiClusterId   ;
	FIFO<XT_MSG>        m_fifoMsg       ;
	uint32              m_uiAddFriendId ;
	bool                m_bClusterInit  ;
	bool				m_bGroupDestoryed ;
	bool				m_bDlgInit       ;
	bool				m_bUpdateP2PPanel;

	bool				m_bPreShowHistory;
	bool				m_bShowHistory;			//�Ƿ���ʾ�����¼����
	int 				m_nWinShakeCnt;
	int					m_nLastShakeTime;

	CToolTipCtrl        m_toolTip;

	vector<tagWaitImageFileStruct> m_vecWaitImageFiles;
	CImageBack          m_imageTitle;

	CStatic			 m_wndHide			 ;
	CP2PToolBar      m_wndToolBar		 ;
	CRect            m_rcPre             ;

	map<CString,CString>    m_mapMagicFace ;
	DWORD            m_dwTickCountActive   ;
	vector<int>      m_vecRecords          ;	
	map<DuiLib::CDuiString,bool> m_MenuCheckInfo;

	// ���ټ����
	HACCEL			m_hAccel;
	// ������ʱȺ�ĵ����к�
	uint32			m_wSeq;
	// ������ʱ���ԱȺ��ʱ���Ӧ�����
	int				m_nTempGroupTalkIndex;


	// ��ǰ���������ļ����̵߳��ֵ�
	map<CString,DOWNLOAD_OBJ*>	m_dicDownloadObj;

	DWORD			m_dwProgressBar ;

	bool            m_bClosePanel ;

	HICON			m_hBigIcon, m_hSmallIcon;

	CUserIPWnd		m_lblIP;

	CP2PFileTransProgressBar  m_wndP2PFileTransProgressBar;

	uint32 m_screenCopyTick;
	DuiLib::CButtonGifUI* m_pHead;//ͷ������
	DuiLib::CTreeNodeUI* m_pSelectGroupNode;//��ǰѡ�еĿ�ݶ�����
	DuiLib::CTreeNodeUI* m_pSelectFastWordNode;//��ǰѡ�еĿ�ݶ���
	DuiLib::CMenuWnd* m_pMenuFastWord;//��ݶ���������õ����Ĳ˵�
	DuiLib::CMenuWnd* m_pMenuFastWordNode;//��ݶ�������õ����Ĳ˵�
	DuiLib::CMenuWnd* m_pMenuMsgRButton;//��ʷ��Ϣ����Ҽ��˵�
	DuiLib::CMenuWnd* m_pMenuWebRButton;//�Ҳ�ҳ���Ҽ��˵�
	BOOL m_bIsFirstFastWordMenu;//�ж��Ƿ��һ�ε�����ݶ���˵�����Ϊ�����˵��ͻ�ʧȥ���㣬ʧȥ����ͻ�رղ˵������Ե�һ�ε����˵�ʧȥ����ʱ���ر�
	CString m_strLastReditSendText;//��������ݸı�֮ǰ������
	map<CString, BOOL> m_mapExpandNode;//���ڼ�¼֮ǰ�򿪵Ŀ�ݶ�����

	DuiLib::CLabelUI* m_pLabelHangupOn;//�ֶ�����ť����״̬
	DuiLib::CLabelUI* m_pLabelHangupOff;//�ֶ�����ť�ǹ���״̬

	DuiLib::CButtonUI* m_pBtnSend;
	DuiLib::CButtonUI* m_pBtnSendConfig;
	DuiLib::CTabLayoutUI* m_pTabRight;
#ifdef _CS_PLAT_EX_
	DuiLib::CButtonUI* m_pBtnMonitorOn;
	DuiLib::CButtonUI* m_pBtnMonitorOff;
	DuiLib::CButtonUI* m_pBtnInsertMsg;
	DuiLib::CButtonUI* m_pBtnTranMsg;

	int m_nIdMonitorBuyer;//����ؿͷ������
	BOOL m_bIsMonitor;//�Ƿ���
	BOOL m_bIsInsertMsg;//�Ƿ����Ự
	map<uint32, vector<XT_MSG>>	m_mapHistoryMsg;	// ��ؿ�ʼ�󣬱���ؿͷ���������������������Ϣ�����ڴ�	
	BOOL m_bIsTran;//�ж��Ƿ��Ѿ��ͷ�ת��
	BOOL m_bIsBuyerNone;//�ж��Ƿ��Ѿ����������ں���ҵ�����
#endif
	BOOL m_bCanTran;//�ж��Ƿ����ת��
	BOOL m_bIsGetFriendInfo;//�ж��Ƿ��ȡ�˺�����Ϣ
	CString m_strLinkInfo;
	CString m_strLinkInfoPre;
	int m_nInsertBuyerId;
private:
	//���������ļ�����֮ǰ��UI����
	struct tagP2PFile
	{
		CString strFile;
		long    nSize;
		long    nID;
	};

	struct tagWaitRecvFile
	{
		long    nFileId;
		long    nSize;
		long    nStartPosition;
		CString strLocalFile;
	};

	UINT	m_dwIpTick;

	uint32  m_nPeerTransServerIp;
	uint16  m_nPeerTransServerPort;

	long  m_nSendFileSequence;

	HINSTANCE hInstance;

	vector<tagP2PFile> m_vecSendFile;
	vector<tagP2PFile> m_vecRecvFile;

	vector<tagP2PFile> m_vecWaitSendFile;
	vector<tagWaitRecvFile> m_vecWaitRecvFile;

	struct 
	{
		uint32 mmID;
		uint8  mmType;
		uint8  nAccept;	

	}  m_mmWaitInfo;

	bool			   m_bMMStartTalk, m_bMMWaitTalk;

	void P2PSendFile(CString strFile);
	void P2PSendFileReplay(long nFileId,bool bRecv);

	LRESULT OnRichEditLinkClilck(WPARAM wParam, LPARAM lParam);
	LRESULT OnRButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	void OnBnClickedBtnHotkey();
	void OnEnter();
	void OnCtrlenter();
	void OnBnClickedBtnAdd();

private:
	vector<string> m_vecFastWord;
	CPngBtn m_btnHangup;				// ����ť
	CPngBtn m_btnAddMember;             //��ӳ�Ա��ť
	CString m_strGroupName;
	CPngBtn m_btnCustSerTran;			// ���Ұ�ͷ�ת�ӹ���	
#ifdef _VER_INTERNAL_
	CInternalFriendInfoDlg *m_pInterNalFriendInfo;
#endif
	StruExtraInfo m_struExtInfo;		// ������Ϣ
public:
	void NavigateComplete2WebAd(LPDISPATCH pDisp, VARIANT* URL);
	void MM_InitXtMsg(XT_MSG &msg);
#ifdef _BUSINESS_VESION
	CCustSerTranDlg *m_pCustSerTranDlg;
#endif
private:
	////uint32 m_nCSID;
	BOOL m_bRet;
};

