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

#define TIMER_FASTWORDSELECTMENU 4003 //快捷短语定时器，用于弹出快捷短语菜单之后输入框失去焦点再次获得焦点
#define TIMER_REDITSEND_TEXT_CHANGE 4004 //输入框判断内容改变的定时器，用于"/"之后输入中文快捷短语菜单跟着变化
#define TIMER_SCREEN_COPY 4005 //过0.1秒之后截屏
#define TIMER_UPDATEURL 4006 //过0.5秒之后刷新右侧页
#define TIMER_EDIT_ENDDOWN 4007 //显示历史消息后拖动到最下方
#define FRM_TIMER_ID 4008 //gif图片重绘间隔
#define TIMER_GETFRIEND_INFO 4009 //获取用户信息

////////服务小记相关///////////////////////////////////////
#define UNSAVED_SERVICELOG       0 //未保存服务小记
#define SAVED_SERVICELOG         1 //已保存服务小记
#define UNCOMPLETED_SERVICELOG   2 //保存不完整的服务小记
#define IS_BUYER_TRAN                 3 //已经转接的买家
///////////////////////////////////////////////////////////

// 聊天窗口附加信息
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
		//	m_reditSend.SetLimitWordSize(400);//发送框限制输入400个字
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
		char	type		;	// 0.本地图片，1.网络图片
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

	// 显示或隐藏评分按钮
	void ShowOrHideScoreBtn(BOOL bShow);

	//关闭窗口时发送关闭消息
	void SendMsgDlgNotify();

	//发送框设置焦点
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

	// 建立临时群聊
	void	CreateTempCluster(const CLUSTER_INFO& ci,int nGroupIndex);
	// 返回当前临时群聊窗体对应的组的id（当当前为组成员临时群聊窗体时才有效）
	int		GetTempGroupTalkIndex() const
	{ return m_nTempGroupTalkIndex; };

	// 开始下载任务
	void Download(const vector<DOWNLOAD_OBJ>& arrTask);
	void ChangeHeadStatus(FRIEND_INFO *pFriend);
#ifdef _BUSINESS_VESION
	void AddTranMsg(uint32 fromId, const char *pOrderId, const char *pReason);
#endif	
	void UpdateBk();
	void InitPos();

	void UpdateUrl();// 更新url
	void UpdateFriendState();// 更新用户在线状态设置为在线
#ifdef _CS_PLAT_EX_
	// 设置是否已经转接
	BOOL GetIsTran();//用户是否已经被转接
	void UpdateIsTran();//用户转接状态还原
	BOOL IsMonitorOn(int nGroupIndex);//判断用户所在的组是否可以被监控
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

	// “推送评价”按钮
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
	// 开始上传群图片应答
	void OnStartUploadCPicAck(const char * data,WORD wRet);
	// 上传群图片应答
	void OnUploadCPicAck(const char * data,WORD wRet);
	// 下载群图片应答
	void OnDownloadCPicAck(const char * data,WORD wRet);
	// 临时群建立的应答
	void OnCreateTempClusterAck(const char *data,WORD wRet);
	// 临时群成员建立的应答
	void OnTempCMembersSetAck(const char *data,WORD wRet);
	// 获取好友基本资料
	void OnGetFriendBasicInfoAck(const char *data,WORD wRet);
	// 获取转接之前的30条聊天记录
	void OnWebMoreMsgAck(const char *data,WORD wRet);
#ifdef _CS_PLAT_EX_
	// 主管获取客服当前聊天买家
	void OnMonitorSetBuyerInd(const char *data,WORD wRet);
	//主管获取客服接待的买家
	void OnMonitorStartAck(const char *data,WORD wRet);
	// 主管获取客服的动作，聊天
	void OnMonitorStartInd(const char *data,WORD wRet);
	// 主管取消监控客服应答
	void OnMonitorEndAck(const char *data,WORD wRet);
	// 主管收到取消监控客服消息
	void OnMonitorEndInd(const char *data,WORD wRet);
	//主管插入会话应答
	void OnMonitorInsertChatAck(const char *data,WORD wRet);
	// 客服和买家都要获取主管插入的会话
	void OnMonitorInsertChatInd(const char *data,WORD wRet);
	//主管接手会话应答
	void OnMonitorTranChatAck(const char *data,WORD wRet);
	//客服和买家收到主管接收会话
	void OnMonitorTranChatInd(const char *data,WORD wRet);
	//void OnGetCSIDFromUserID(const char *data,WORD wRet);
#endif
	// 发送临时群建立请求
	void SetClusterMember(uint32 clusterId);
	// p2p测试应答
	void OnP2PTestAck(const char * data, WORD wRet);
	// 发送按钮
	void OnBnClickedBtnSend();

#ifdef _BUSINESS_VESION
	void OnGetNearCommCustRemarkAck(const char *data,WORD wRet);
	void OnUpdateNearCommCustRemarkAck(const char *data,WORD wRet);
	// 收到客服转接确认包
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
	// 通知服务端打开了一个聊天窗口
	void NotifyServer_OpenedMsgDlg();
	void UpdateFastWordInfo();//刷新所有快捷短语
	void AddTalkMsg(const XT_MSG &msg,bool isHistoryMsg,int nUnReadNum = 0);
	void InsertFileInfoBar(DWORD dwFileID,bool bSendFile,CString strFileName, long nFileSize);
	void AddTranFromMsg(const XT_MSG &msg, CString strFromName, CString strToName);
	BOOL CloseAllDlg();//关闭窗口，包括关闭窗口的提示框
	void SetWindowMax(BOOL bIsMax);//设置窗口最大化或者最小化
	void SetCsBuyerOffline();
	void RbuttonDownWeb();
	BOOL CanTran();
	void TranCuster(int nTranId);
	void AddTranRefuse(string strFromName);
	void SetTranPre(bool bIsTranPre);
	void InsertMsg(int nServiceId, int nCusterId);
#ifdef _CS_PLAT_EX_
	// 接手会话后，打开客户聊天窗口
	static unsigned __stdcall ThreadTranChat(void * pParam);
	int GetMonitorBuyerId();
	// 打开客户聊天窗口
	LRESULT OnOpenTranChatMsgDlg(WPARAM w,LPARAM l);
	void CloseMonitor();
	void SetHistoryText(CString strText);
#endif
protected:
	void ShowDlgMax(BOOL bIsShowMax);//最大化或者最小化
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
	// 将待发送的消息表达式转换为需要本地显示的消息和需要发送的消息
	//------------------------------------------------------
	void ProcTalkMsg(const CString & sMsg,CString& sShowMsg);
	// 停止下载某个文件,可以指定是否强行终止下载线程
	void StopDownload(const CString& sFileID,bool bTerminate = true);

	void LayoutUserWindow(DuiLib::CPoint& ptUserPanel, DuiLib::CSize& szUserPanel, DuiLib::CPoint& ptHistory, DuiLib::CSize& szHistory);
	void LayoutFileWindow(DuiLib::CPoint& ptSend, DuiLib::CSize& szSend, DuiLib::CPoint& ptHistory, DuiLib::CSize& szHistory,
		DuiLib::CPoint& ptSplitter, DuiLib::CSize& szSplitter, DuiLib::CPoint& ptToolBar, DuiLib::CSize& szToolBar);
	void LayoutButton(int nLeft, int nTop, DuiLib::CSize& szBtn, int nClientCx);

	virtual		void          InitResource();

	// 快捷短语消息相应
	afx_msg void OnCommandFastWordMenu(int id);
	// 点击了快捷短语按钮
	afx_msg void OnBnClickedFastword(DuiLib::TNotifyUI& msg);
#ifdef _BUSINESS_VESION
	// 账号挂起
	afx_msg void OnBnClickedBtnHangup();
	// 客服转接
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
	CPopupHistoryDlg	m_wndPopupHistory	;		//右侧聊天记录窗口

	CCefWebUI*		m_wndAD;
	//DuiLib::CWebBrowserUI* m_wndAD;
	CString				m_strAdUrl		;
	XT_FONT			    m_fontLast		; //选择快捷短语之前的字体
	BOOL                m_bIsFontLast;
	CHARRANGE	        m_crTran;//客服转接提示在richedit中的位置
public:
#ifdef _BUSINESS_VESION
	CString				m_strNewComnCustRemarkClient;		//最近联系客户右侧备注(本地)
	CString				m_strNewComnCustRemarkServer;		//最近联系客户右侧备注(服务端)
#endif 
private:
	//CP2PUserPanel       *m_pwndUserPanel  ;
	CP2PUserList        m_wndUserList   ;
	CP2PTalkToolBar		*m_pwndTalkToolBar;

	uint32				m_uPopupHistoryWidth;	//右侧聊天记录窗口宽度
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
	bool				m_bShowHistory;			//是否显示聊天记录窗口
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

	// 加速键句柄
	HACCEL			m_hAccel;
	// 建立临时群聊的序列号
	uint32			m_wSeq;
	// 建立临时组成员群聊时候对应的组号
	int				m_nTempGroupTalkIndex;


	// 当前正在下载文件的线程的字典
	map<CString,DOWNLOAD_OBJ*>	m_dicDownloadObj;

	DWORD			m_dwProgressBar ;

	bool            m_bClosePanel ;

	HICON			m_hBigIcon, m_hSmallIcon;

	CUserIPWnd		m_lblIP;

	CP2PFileTransProgressBar  m_wndP2PFileTransProgressBar;

	uint32 m_screenCopyTick;
	DuiLib::CButtonGifUI* m_pHead;//头像设置
	DuiLib::CTreeNodeUI* m_pSelectGroupNode;//当前选中的快捷短语组
	DuiLib::CTreeNodeUI* m_pSelectFastWordNode;//当前选中的快捷短语
	DuiLib::CMenuWnd* m_pMenuFastWord;//快捷短语组的设置弹出的菜单
	DuiLib::CMenuWnd* m_pMenuFastWordNode;//快捷短语的设置弹出的菜单
	DuiLib::CMenuWnd* m_pMenuMsgRButton;//历史消息框的右键菜单
	DuiLib::CMenuWnd* m_pMenuWebRButton;//右侧页的右键菜单
	BOOL m_bIsFirstFastWordMenu;//判断是否第一次弹出快捷短语菜单，因为弹出菜单就会失去焦点，失去焦点就会关闭菜单，所以第一次弹出菜单失去焦点时不关闭
	CString m_strLastReditSendText;//输入框内容改变之前的内容
	map<CString, BOOL> m_mapExpandNode;//用于记录之前打开的快捷短语组

	DuiLib::CLabelUI* m_pLabelHangupOn;//手动挂起按钮挂起状态
	DuiLib::CLabelUI* m_pLabelHangupOff;//手动挂起按钮非挂起状态

	DuiLib::CButtonUI* m_pBtnSend;
	DuiLib::CButtonUI* m_pBtnSendConfig;
	DuiLib::CTabLayoutUI* m_pTabRight;
#ifdef _CS_PLAT_EX_
	DuiLib::CButtonUI* m_pBtnMonitorOn;
	DuiLib::CButtonUI* m_pBtnMonitorOff;
	DuiLib::CButtonUI* m_pBtnInsertMsg;
	DuiLib::CButtonUI* m_pBtnTranMsg;

	int m_nIdMonitorBuyer;//被监控客服的买家
	BOOL m_bIsMonitor;//是否监控
	BOOL m_bIsInsertMsg;//是否插入会话
	map<uint32, vector<XT_MSG>>	m_mapHistoryMsg;	// 监控开始后，被监控客服与买家聊天的所有聊天消息保存于此	
	BOOL m_bIsTran;//判断是否已经客服转接
	BOOL m_bIsBuyerNone;//判断是否已经弹出不存在和买家的聊天
#endif
	BOOL m_bCanTran;//判断是否可以转接
	BOOL m_bIsGetFriendInfo;//判断是否获取了好友信息
	CString m_strLinkInfo;
	CString m_strLinkInfoPre;
	int m_nInsertBuyerId;
private:
	//用来管理文件传输之前的UI交互
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
	CPngBtn m_btnHangup;				// 挂起按钮
	CPngBtn m_btnAddMember;             //添加成员按钮
	CString m_strGroupName;
	CPngBtn m_btnCustSerTran;			// 卖家版客服转接功能	
#ifdef _VER_INTERNAL_
	CInternalFriendInfoDlg *m_pInterNalFriendInfo;
#endif
	StruExtraInfo m_struExtInfo;		// 附加信息
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

