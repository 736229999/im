#pragma once

#include "afxwin.h"
#include "RichEditCtrlEx.h"
#include "improtocol.h"

#include "ImageToolBar.h"
#include "P2PUserPanel.h"
#include "P2PUserList.h"
#include "P2PAudioPanel.h"
#include "P2PVedioPanel.h"
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

using namespace std;


#define TIMER_DOWNLOADOBJ	2001
#define TIMER_DWN_CPIC		3001
#define TIME_DWN_CPIC		300
#define DOWNLOAD_TIMEROUT	(10*1000)	//10 s

#define TIMER_P2P_STATUS	1001
#define TIMER_CLIPBOARD     1002
#define TIMER_WIN_SHAKE     4001

#define TIMER_SHOW_USERCARD	4002	// 显示用户资料

#define WM_CLOSETABCTRL_CLOSE (WM_USER+424) //tab控件消息记录关闭的消息

#define WM_CLOSE_ALL (WM_USER+425)
#define WM_CLOSE_ONE (WM_USER+426)

enum TP2PMode {	   
	pmNone = 0,
	pmSingleTalk, 
	pmMultiTalk,
	pmAudio,
	pmVedio,
};


#define WM_FILERECV_OK     WM_USER+0x0D01
#define WM_FILERECV_FAILED WM_USER+0x0D02
#define WM_FILESEND_OK     WM_USER+0x0D03
#define WM_FILESEND_FAILED WM_USER+0x0D04


//}FILE_INFO;

#ifdef _VER_INTERNAL_
class CInternalFriendInfoDlg;
#endif
class CFlatPopupMenu;
// CP2PSendMsgDlg 对话框
class CP2PSendMsgDlg : public CXSkinDlg, public CSimpleFileTransSession, public CAdvancedFileTransSession, public CMMTalker
{
	DECLARE_DYNAMIC(CP2PSendMsgDlg)
	DECLARE_HMG_SCROLLBAR()

	friend class CMessageHandleWnd;

public:
	CP2PSendMsgDlg(FRIEND_INFO *pFriend, TP2PMode msgMode, CWnd *pParent, const char *szProductID = NULL);   // 标准构造函数
	virtual ~CP2PSendMsgDlg();

	enum { IDD = IDD_P2P_SENDMSG };

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


	void UpdateFriendInfo(FRIEND_INFO *pFriend);

	// 显示或隐藏评分按钮
	void ShowOrHideScoreBtn(BOOL bShow);

	DECLARE_EVENTSINK_MAP() 

public:

	friend UINT DownloadPicProc( LPVOID pParam );

public:
	virtual BOOL DestroyWindow();
	DWORD GetActiveTickCount() { return m_dwTickCountActive; }

	void OnP2pMsgTimeOut();
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

public:
	afx_msg void OnBnClickedBtnAudio();
	afx_msg void OnBnClickedBtnVedio();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void OnHelpClick();

	void OnTimer(UINT nIDEvent);

	afx_msg void OnSendFile();
	afx_msg void OnInvite();
	afx_msg void OnFaceIconClicked();
	//afx_msg void OnFontBoldClicked();
	//afx_msg void OnFontItalicClicked();
	//afx_msg void OnFontUnderLineClicked();
	afx_msg void OnSendPicClicked();
	//afx_msg void OnBnClickedBtnColor();
	afx_msg void OnBnClickedBtnClose();
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
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnPaint();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnBnClickedBtnFold();
	afx_msg void OnAccIp();
	afx_msg void OnBnClickedButtonHistory();

	// “推送评价”按钮
	afx_msg void OnBnClickedButtonScore();
	afx_msg LRESULT OnSnapShot(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

	LRESULT OnEmotionSelNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnMagicFaceNotify(WPARAM wParam ,LPARAM lParam);
	LRESULT OnQueryPeerOnline(WPARAM wParam, LPARAM lParam);
	LRESULT OnMagicFaceClick(WPARAM wParam,LPARAM lParam);
	LRESULT	OnStartDownloadFace(WPARAM w, LPARAM l);
	LRESULT OnDownloadFaceOK(WPARAM w, LPARAM l);
	LRESULT OnDownloadProcess(WPARAM w, LPARAM l);
	LRESULT OnInsertMsg(WPARAM w, LPARAM l);
	LRESULT OnUserDragDropFile(WPARAM w,LPARAM l);
	LRESULT OnP2PVedioNotify(WPARAM w,LPARAM l);
	LRESULT OnP2PAudioNotify(WPARAM w,LPARAM l);
	LRESULT OnP2PNetNotify(WPARAM wParam,LPARAM lParam);
	LRESULT OnThreadNotify(WPARAM wParam,LPARAM lParam);

	LRESULT OnRecvFileOK(WPARAM wParam,LPARAM lParam);
	LRESULT OnRecvFileFailed(WPARAM wParam, LPARAM lParam);
	LRESULT OnSendFileOK(WPARAM wParam,LPARAM lParam);
	LRESULT OnSendFileFailed(WPARAM wParam,LPARAM lParam);

	LRESULT OnCloseTabCtrlClose(WPARAM wParam,LPARAM lParam);
	LRESULT OnCloseAll(WPARAM wParam,LPARAM lParam);
	LRESULT OnCloseOne(WPARAM wParam,LPARAM lParam);
	LRESULT OnKeyDownReturn(WPARAM wParam,LPARAM lParam);

	void OnFriendStatusChange(const char *data,WORD wRet);
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

protected:	
	static void  OnRecvVedioData(BYTE * pBmpData,DWORD dwUser);

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
	/*virtual  void  AF_OnRecvServerAck(const char *data,WORD wRet);*/
	void MM_InitXtMsg(XT_MSG &msg);
	void MM_Reply(int mmType,uint32 mmID,bool bAccept);
	void MM_SendRequest(int mmType);//0 vedio, 1 audio
	void MM_CancelRequest(int mmType,uint32 mmID);
	void MM_Stop();

public:
	void MM_OnRecvCancel(const XT_MSG &msg);
	void MM_OnRecvRequest(const XT_MSG &msg);
	void MM_OnReply(const XT_MSG &msg);
	void MM_OnStop(const XT_MSG &msg);
	void UpdateSignature();
	void OnRecvServerAck(const char *data,WORD wRet);
	// 通知服务端打开了一个聊天窗口
	void NotifyServer_OpenedMsgDlg();
	//void OnRecvFileAck(const char *data,WORD wRet);
	//void OnRecvSendFileAck(const char *data,WORD wRet);

protected:
	int  DoModal();
	void ShowDlgTitle();
	void LoadLastTalkMsg();
	void AdjustUI();
	void ShowFileTransInfoBar(bool bShow);
	void UpdateP2PPanel();
	void AddTalkMsg(const XT_MSG &msg,bool isHistoryMsg);
	void DrawControlLine();
	int  InitFontToolBar();
	void AddMsgTip(CString strTip,COLORREF clrText=RGB(122,122,122));
	void AddVedioSettingTip();

	virtual void DrawCaptionBar();

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

	void LayoutUserWindow(CPoint& ptUserPanel, CSize& szUserPanel, CPoint& ptHistory, CSize& szHistory);
	void LayoutFileWindow(CPoint& ptSend, CSize& szSend, CPoint& ptHistory, CSize& szHistory,
		CPoint& ptSplitter, CSize& szSplitter, CPoint& ptToolBar, CSize& szToolBar);
	void LayoutButton(int nLeft, int nTop, CSize& szBtn, int nClientCx);

	virtual		void          InitResource();

	// 快捷短语消息相应
	afx_msg void OnCommandFastWordMenu(int id);
	// 点击了快捷短语按钮
	afx_msg void OnBnClickedFastword();
#ifdef _BUSINESS_VESION
	// 账号挂起
	afx_msg void OnBnClickedBtnHangup();
	// 客服转接
	afx_msg void OnBnClickedBtnCustserTran();
#endif
	// 处理鼠标移动消息，当鼠标移动到头像区域时，弹出好友资料窗口
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

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
	FRIEND_INFO			m_FriendInfo	;

	CBitmap*			m_pBitmapHead	;
	CRountFaceCtrl		m_ctrlFaceIcon	;
	CRichEditCtrlEx		m_reditSend		;
	CRichEditCtrlEx		m_reditHistory	;
	CSplitterControl	m_wndSplitter1	;
	CBmpBtn				m_btnHistory	;
	CBmpBtn				m_btnClose		;
	CSplitBtn			m_btnSend		;
	CBmpBtn             m_btnFolder     ;

	CPngBtn				m_btnSendFile	;
	CPngBtn				m_btnAudio      ;
	CPngBtn				m_btnVedio      ;
	//CImageToolBar		m_wndFontTool	;
	CPopupHistoryDlg	m_wndPopupHistory	;		//右侧聊天记录窗口

	CExplorer1			m_wndAD			;		//广告栏
	CString				m_strAdUrl		;
#ifdef _BUSINESS_VESION
	CString				m_strNewComnCustRemarkClient;		//最近联系客户右侧备注(本地)
	CString				m_strNewComnCustRemarkServer;		//最近联系客户右侧备注(服务端)
#endif 
	CP2PUserPanel       m_wndUserPanel  ;
	CP2PUserList        m_wndUserList   ;
	CP2PAudioPanel	    m_wndAudioPanel ;
	CP2PVedioPanel      m_wndVedioPanel ;

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
	CP2PTalkToolBar  m_wndTalkToolBar	 ;
	CP2PToolBar      m_wndToolBar		 ;
	CRect            m_rcPre             ;

	map<CString,CString>    m_mapMagicFace ;
	DWORD            m_dwTickCountActive   ;
	vector<int>      m_vecRecords          ;	

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

private:
	//用来管理文件传输之前的UI交互
	struct tagP2PFile
	{
		CString strFile;
		long    nSize;
		long    nID;
	};

	struct tagMMInfo
	{
		int    nMMType; //0vedio, 1 audio
		long   nMMID;
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
	vector<tagMMInfo>  m_vecSendMMInfo;
	vector<tagMMInfo>  m_vecRecvMMInfo;

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

private:
	void OnBnClickedBtnHotkey();
	void OnEnter();
	void OnCtrlenter();
	void OnBnClickedBtnAdd();
	// 打开好友聊天窗口的其他链接
	void BeforeNavigate2WebAd(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel);

private:
	vector<string> m_vecFastWord;
	CPngBtn m_btnHangup;				// 挂起按钮
	CPngBtn m_btnAddMember;             //添加成员按钮
	CString m_strGroupName;
	CPngBtn m_btnCustSerTran;			// 卖家版客服转接功能	
#ifdef _VER_INTERNAL_
	CInternalFriendInfoDlg *m_pInterNalFriendInfo;
#endif
	string m_strProductID;				// 产品ID
public:
	void NavigateComplete2WebAd(LPDISPATCH pDisp, VARIANT* URL);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nHitTest, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
