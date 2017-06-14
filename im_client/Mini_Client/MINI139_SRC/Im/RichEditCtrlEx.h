#pragma once

#include <afxole.h>
#include "impublic.h"
#include "Richole.h"
#include <set>
//#define _XY_PICTURE
#ifndef _XY_PICTURE
#import "..\ImageJk.dll" named_guids no_namespace
#else

#import "xpicture.dll" 
using namespace XPictureMain;
#endif
#ifdef _DEBUG
	#import "..\binDebug\MiniAtxControl.dll" named_guids no_namespace
	#import "..\binDebug\OleLinkInfo.ocx" named_guids no_namespace
#else
	#import "..\binRelease\MiniAtxControl.dll" named_guids no_namespace
	#import "..\binRelease\OleLinkInfo.ocx" named_guids no_namespace
#endif

#include "../OleImage.h"

#define WM_KEYDOWN_RETURN (WM_USER+105)
#define WM_FASTWORD_MENU (WM_USER+106) //输入框弹出快捷短语菜单
#define WM_FASTWORD_MENU_HIDE (WM_USER+107) //输入框隐藏快捷短语菜单
#define WM_KEYDOWN_UPDOWN (WM_USER+108) //输入框按上下键，暂时未实现
/************************************************************************ 
* 
* 文件名：richeditctrlex.h 
* 
* 文件描述：图文混排编辑控件
* 
* 创建人： 薛小峰,2005年8月7日
* 
* 版本号：1.0 
* 
* 修改记录： 
* 
************************************************************************/ 

#define WM_USERLINK_CLICK	(WM_USER + 3000)
#define WM_USERLINK_RBCLICK	(WM_USER + 3001)

enum EMOTION_PIC_TYPE{
	EPT_SYS_BMP,	// 系统位图表情
	EPT_SYS_GIF,	// 系统gif表情
	EPT_CUSTOM,		// 自定义表情
	EPT_WEB			// web表情
};


//OLE 对象及数据类型
enum OLE_DATA_TYPE
{
	ODT_GIF =-1,
	ODT_BITMAP = -2,
	ODT_WEB_EMOTION = -3,
	ODT_WAITICON = -4,
	ODT_PROGRESS_BAR = -5,
	ODT_SEND_FILE = -6,
	ODT_RECV_FILE = -7,
	ODT_CANCEL_FILE = -8,//已取消的发送文件或者接收文件
	ODT_SEND_VEDIO = -9,
	ODT_RECV_VEDIO = -10,
	ODT_CANCEL_VEDIO = -11,
	ODT_SEND_AUDIO = -12,
	ODT_RECV_AUDIO = -13,
	ODT_CANCEL_AUDIO = -14,
	ODT_LINK_INFO = -15,
};//

struct EMOTION_PIC_DATA{
	int			nType;	
	CString		sData;	
	EMOTION_PIC_DATA()
	{
		nType = EPT_CUSTOM;
	}
};

//struct CUSTOM_EMOTION;



#define RN_FILE_ACCEPT	 0 // 接收
#define RN_FILE_SAVEAS	 1 // 另存为
#define RN_FILE_REFUSE   2
#define RN_FILE_CANCEL   3
#define RN_VEDIO_ACCEPT  4
#define RN_VEDIO_REFUSE   5
#define RN_VEDIO_CANCEL   6
#define RN_AUDIO_ACCEPT  7
#define RN_AUDIO_REFUSE  8
#define RN_AUDIO_CANCEL  9
#define RN_VEDIO_SETTING 10
#define RN_FILETRANS_SETTING 11

#define EDIT_TYPE_SENDMSG 0
#define EDIT_TYPE_AUTOACCEPT 1
struct USER_LINK
{
	uint32		id;
	CHARRANGE	range;
	CString		sName;
};

struct FRIEND_CLUSTER_LINK
{
	int       nType;//1:好友 2:群
	CHARRANGE range;//区域
	int       nId;//ID
	char      szName[255];
	FRIEND_INFO* friendInfo;
	XT_CLUSTER_INFO* clusterInfo;
	FRIEND_CLUSTER_LINK()
	{
		nType = 0;
		range.cpMax = 0;
		range.cpMin = 0;
		nId = 0;
		memset(szName, 0, sizeof(szName));
		friendInfo = NULL;
		clusterInfo = NULL;
	}
};

class CUSTOM_EMOTION;
class CPictureCtrl;
class CRichEditCtrlEx : public CRichEditCtrl
{
	DECLARE_DYNAMIC(CRichEditCtrlEx)

public:
	
	CRichEditCtrlEx();
	
	virtual ~CRichEditCtrlEx();

public:
	
	static LPCTSTR emotions[];
	
	static LPCTSTR emotion_tips[];
	
	static CHARFORMAT2 XFont2CharFormat(const XT_FONT &xf);
	
	static XT_FONT CharFormat2XCFont(const CHARFORMAT2 &cf);
	
	const static int emotion_count;
	
	//------------------------------------------------------
	// 插入一自定义表情
	//------------------------------------------------------
	void   InsertFace(const CUSTOM_EMOTION * face,int nParam);

public:
	vector<IMAGE_FILE_STRUCT *> m_vecImageFileStruct;
	//------------------------------------------------------
	// 手动清除表情图片中的额外信息数据
	//------------------------------------------------------
	void	ClearEmotionPicData()
	{ m_arrPicData.clear(); };

	virtual BOOL Create( DWORD in_dwStyle, const RECT& in_rcRect, CWnd* in_pParentWnd, UINT in_nID ) ;

public:
	//设置转义符
	void EnableZYF(bool bEnable) { m_bZYF=bEnable;
	}
	bool IsEnableZYF() { return m_bZYF;
	}
	CString Utf82Char(CString strUtf8);
	void InsertUserLink(const char * szUser,uint32 id);
	void SetStartIndent(int nIndent);
	void SetEmotionImageList(CImageList * pImageEmotionList);
	void InsertText( CString text,const XT_FONT& xf);
	void InsertText( CString text,bool bWaitImageFile=false, CStringList *pslWaitImageFile=NULL);
	void InsertGifEmotion(int i);
	void InsertEmotion( int i );
	void InsertBitmap(HBITMAP hBitmap);
	void InsertImage(CString strPicPath,EMOTION_PIC_DATA& data);//旧插入图片接口
	HRESULT InsertImage(LPCTSTR lpszPathName);    //新插入图片接口，支持gif、png等
	void InsertLine();
	void InsertWaitIcon(IMAGE_FILE_STRUCT *pStruct);
	void InsertWaitIcon(LPCTSTR lpszPathName, IMAGE_FILE_STRUCT *pStruct);

	void OnRecvImageFile(const char *file_name);
	void OnAbortImageFile(const char *file_name);

	XT_FONT GetXFont();
	void SetXFont(const XT_FONT &xf);
	void SetXFont(const XT_FONT &xf, long nStart, long nEnd);
	bool ReplaceFileRecvImage(LPCTSTR sPathName,int nFileSessionId);
	
	CHARRANGE InsertLink( CString strTextLink);
	void InsertFriendClusterLink(FRIEND_CLUSTER_LINK FriendClusterLinkInfo);
	void ClearFriendClusterLink();
	void GetMsgFormat( XT_FONT & mft );
	void ScrollToBottom( );
	void SetText( LPCTSTR text );

	CString GetText( CString & text , IMAGE_FILE_LIST * ifl=NULL );
	CString GetSelTextEx(BOOL& bIsOleLinkInfo);

	void SetParagraphBulleted();
	bool ParagraphIsBulleted();

	void ClearMsg( );
	bool IsFocused() { return m_bFocused; }

	// download emotion progress bar
	DWORD InsertProgressBar();
	DWORD FindProgressBar();
	void SetProgressBarPos(DWORD dwBar,int nPos);
	void SetProgressBarTip(DWORD dwBar,CString strTip);
	void SetMMBarTip(DWORD dwData, OLE_DATA_TYPE odtType, CString strTip, COLORREF clrTip);
	void SetFileBarTip(DWORD dwFileID, OLE_DATA_TYPE odtType, CString strTip, COLORREF clrTip=RGB(133,122,255));
	//
	void InsertMMInfoBar(DWORD dwMMID, bool bRequest,CString strMMType, CString strFriendNickName);
	void InsertFileInfoBar(DWORD dwFileID,bool bSendFile,CString strFileName, long nFileSize);
	IMiniFileInfoBar * FindNearestFileInfoBar(int pos, OLE_DATA_TYPE &odtType, int & nBarPos);
	IMiniMMInfoBar * FindNearestMMInfoBar(int pos, OLE_DATA_TYPE &odtType, int & nBarPos);

	void SetLimitWordSize(int nSize);//设置字数限制
	void SetEditType(int nType);
	void SetShowFastWordMenu(BOOL bIsShow);//设置是否显示快捷短语菜单,用于父窗口调用
	void SetShowLinkInformation(BOOL bIsShow = FALSE);//设置是否显示链接信息,用于父窗口调用
	void SendFastWordSelectMenu();//发送弹出快捷短语菜单的消息
	CString GetStoreInformation(CString strHttpConnection, CString strHttpFile);
	void InsertLinkInfo(CString strStoreUrl, CString strStoreLogoUrl, CString strStoreName, CString strStoreDesc);
protected:
	int PointToUserLink(CPoint point);
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void OnDropFiles(HDROP hDrop);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLink(NMHDR *pNMHDR, LRESULT *result);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg LRESULT OnIMEChar(WPARAM wParm,LPARAM lParam);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg LRESULT OnWebFileNotify(WPARAM wParam, LPARAM lParam);
	afx_msg void OnNcDestroy();
	DECLARE_MESSAGE_MAP()

	void ProcessEmotion();
	void OnAdd2Emotion(void);
	void OnCancelZYF();
	void OnSaveAs(void);
	void OnCopy(void);
	void OnCut(void);
	void OnPaste(void);

	bool IsGifEmotion(CString str,int &id);
	bool FindFirstImage(CString str,CString &strImageFileName,int &iImageStart,int &iImageEnd);
	bool FindFirstEmotion( CString str , int &iEmotion, int & nEmBeg,int & nEmEnd );
	bool FindFirstInputSysEmo(CString str , int &iEmotion, int & nEmBeg,int & nEmEnd );
	bool FindFirstCustomEmotion( CString str,int & nGroup,int &iEmotion, int &iPos);
	void SetHintText(CString &strTitle);
	HBITMAP GetStaticFace( int i);
	// 替换接收到的图片
	void ReplaceRecvImg(IMAGE_FILE_STRUCT *pStruct, const char * file_name);

public:	
	vector<FRIEND_CLUSTER_LINK> m_vecFriendClusterLink;
	static DWORD    m_dwFileSessionId	;//m_dwFileSessionId属性改为公有，其他地方也可以累加
private:

	CImageList *	m_pImageEmotionList	;
	XT_FONT    *	m_pXFont			;

	bool	        m_bFocused			;
		

	vector<HBITMAP> m_vecStaticFace		;

	long			m_nArrayGifId[100]	;
	vector<EMOTION_PIC_DATA>
					m_arrPicData		;
	vector<USER_LINK>	
					m_arrName;
	
	bool			m_bCapture ;

	bool			m_bZYF;			// 是否处理转义符号
#ifndef _XY_PICTURE
	vector<IShowImagePtr>	
					m_arrImages;
#else
	vector<IXPicturePtr>
					m_arrImages;
#endif
	//vector<COleImage*>   	m_vecOleImage;
	DWORD			m_dwBarIndex  ;

	bool			m_bPromptOffline;	// 是否提示好友不在线，确定是否使用图片的快捷键
	int				m_nPreEmotion;		// 上次否定的快捷键对应图片id

	CComPtr<IRichEditOle> m_pRichEditOle;

	int m_nLimitWordSize;//设置的字数限制
	int m_nEditType;//edit类型，用于判断是否使用公共的字体
	CString m_strEditOld;//用于存储最后一次满200字的字符串
	long m_nStartCharOld;//用于获取光标最后所在的位置
	long m_nEndCharOld;//用于获取光标最后所在的位置

	CPictureCtrl* m_pPictureCtrl;
	BOOL m_bIsShowFastWordMenu;//是否显示快捷短语菜单,用于父窗口调用
	BOOL m_bIsShowLinkInformation;//是否显示链接信息
	BOOL m_bIsPointInLinkRect;//判断鼠标是否在链接信息上面
	CString m_strLinkInfoUrl;//链接信息的地址

public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	virtual BOOL DestroyWindow();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnEnChange();
	int FindFirstObject(int cpMin, int nObjectCount);
	afx_msg void OnSelAll();
	afx_msg void OnClear();
	// 响应WM_CREATE
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
