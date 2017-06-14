#pragma once

#include "afxwin.h"

#include "richeditctrlex.h"

#include "afxcmn.h"

#include "localdata.h"
#include "improtocol.h"

#include "..\controls\skinctrl.h"
#include "..\..\..\SpaceManagement\public\public_controls\buttonext.h"
#include "CustomListCtrl.h"
#include "..\..\..\DuiLib\UIlib.h"
#include "..\..\..\HmgSkin\HmgSkin\CheckButton.h"

#define WM_MAINTAB_SEL_CHANGE	WM_USER+32	
class CUIMsgManageMainTab : public DuiLib::WindowImplBase
{
public:
	enum EN_SEL
	{
		en_null = -1,
		en_sel_friend = 0,
		en_sel_cluster = 1,
		en_sel_sysmsg = 2,
	};

	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UIMsgManageMainTab");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("MsgManageMainTab.xml");  }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	virtual void Notify(DuiLib::TNotifyUI& msg);
	void SetCurSel(EN_SEL en);
	EN_SEL GetCurSel();
	void SetSysMsgTabText(CString strText);
	virtual void InitWindow();

	DuiLib::CButtonUI* m_pBtnRead;
	DuiLib::CButtonUI* m_pBtnSubscriber;
};

class CDuiOrderTree : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("DuiOrderTree");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("OrderTree.xml");  }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	void SetOrderMsgRead(OrderMsg* pOrderMsg);
	void SetAllOrderMsgRead();
public:
	void LoadOrderMsg(int nPage = 0, BOOL bIsOrderTrade = TRUE);
	void LoadTranMsg(int nPage = 0);
	void Notify(DuiLib::TNotifyUI& msg);
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
private:
	std::map<DuiLib::CButtonUI*, DuiLib::CVerticalLayoutUI*> m_mapBtnOrderToVLOrderContent;
	std::map<DuiLib::CButtonUI*, DuiLib::CButtonUI*> m_mapBtnOrderToTime;
	std::vector<DuiLib::CButtonUI*> m_vecBtnOrderDetails;
	//virtual void Notify(DuiLib::TNotifyUI& msg);
};

#define WM_LABELLIST_SEL_CHANGE	WM_USER+33	
class CLabelListWnd : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UIMsgManageLabelList");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("MsgManageLabelList.xml");  }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	virtual void Notify(DuiLib::TNotifyUI& msg);
	virtual void InitWindow();
	int AddItem(const char *szItem);
	void RemoveAll();
	DWORD GetItemData(int nIndex);
	void SetItemData(int nIndex, DWORD data);
	int GetItemCount();
	void SetCurSel(int nIndex);
	int GetCurSel();
	DuiLib::CListContainerElementUI* GetItem(int nIndex);
	void SetBkColor(DWORD dwBkColor, DWORD dwItemBkColor, DWORD dwHotItemBkColor, DWORD dwSelectedItemBkColor);

private:
	DuiLib::CListUI *m_pList;
	vector<DWORD> m_vecData;
};


class CImgListUI : public DuiLib::CListUI
{
public:
	CImgListUI(DuiLib::CPaintManagerUI& paint_manager);
	~CImgListUI();
	bool Add(LPCTSTR szImgPath, LPCTSTR szText);
	void RemoveAll();

private:
	DuiLib::CPaintManagerUI& paint_manager_;
	DuiLib::CDialogBuilder m_dlgBuilder;
};



#define WM_IMGLIST_SEL_CHANGE	WM_USER+34	
class CImgListWnd : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UIMsgManageImgList");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("MsgManageImgList.xml");  }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	virtual void Notify(DuiLib::TNotifyUI& msg);
	virtual void InitWindow();
	int AddItem(LPCTSTR szImgPath, LPCTSTR szText);
	void RemoveAll();
	DWORD GetItemData(int nIndex);
	void SetItemData(int nIndex, DWORD data);
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass);
	int GetItemCount();
	void SetCurSel(int nIndex);
	int GetCurSel();

private:
	CImgListUI *m_pList;
	vector<DWORD> m_vecData;
};



enum FINDRANGE_ITEMTYPE{
	EM_IT_FRIEND,
	EM_IT_CLUSTER,
	EM_IT_ALL,
	EM_IT_1MONTH,
	EM_IT_3MONTH,
	EM_IT_1YEAR
};

struct FINDRANGE_ITEMDATA{
	INT		type;
	UINT	id;
};

// CMsgDlg 对话框

class CMsgDlg : public CDialog
{
	DECLARE_DYNAMIC(CMsgDlg);
	DECLARE_HMG_SCROLLBAR()

public:
	CMsgDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMsgDlg();

// 对话框数据
	enum { IDD = IDD_MSG_DIALOG };

public:
	void SetDefaultFriend(uint32 fid);
	void SetDefaultCluster(uint32 cid);
	void SetOrderMsgRead(OrderMsg* pOrderMsg);
	void SetAllOrderMsgRead();
	void SetOrderPage(int nOrderType);
	void SetSystemMsgPage();
	void ShowFirstPage();
	void SetSysMsgTabText();
	void AddOrderMsg();

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnNext();
	afx_msg void OnBnClickedBtnFirst();
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnBnClickedBtnLast();
	afx_msg void OnNcDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg LRESULT OnMainTabSelChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnList1SelChange(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnList2SelChange(WPARAM wParam, LPARAM lParam);
	
	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

	void OnLoginOut(const char *data, WORD wRet);
	void OnUserSearchAck(const char *data, int nRetCode);
	void OnClusterSearchAck(const char *data,WORD wRet);

	void FindMsg(int nId,CDataManager::MSG_TYPE idType,const CString& sKeyword,MSG_LIST &rstList,CTime tMsgBegin);
	// 设置查询范围复选框的当前选择项
	bool	SetRangeSel(FINDRANGE_ITEMDATA rit);

private:
	
	//调整界面布局
	void AdjustUI();
	void InitFriendTree();
	void InitClusterTree();
	void InitSystemTree();
	void InitComboRange();

	void AddP2PTalkMsg(const XT_MSG &msg);
	void AddSystemMsg(const SYSTEM_MSG &msg);
	void AddClusterSystemMsg(const CLUSTER_SYSTEM_MSG &msg);
	void InitMsg();
	void LoadSystemMessage();
	void LoadClusterSystemMessage();
	void LoadSystemMsgAll(const CString& sKeyword,MSG_LIST &rstList,CTime tMsgBegin);
	void ShowPage(int iPage);
	void AdjustPageBtn();
private:
	HICON		m_hIcon;

private:
#ifdef _CS_PLAT_EX_////飞牛客服工作台
	static const int m_nSysTypeCnt = 3;
#else
	static const int m_nSysTypeCnt = 6;
#endif

	static CString m_strSysMsgNames[m_nSysTypeCnt];
	int m_iMessageType ;//0 talk message, 1 system message

	/*页数*/
	int  m_iPage    ;
	int  m_iPageCount  ;

	CRichEditCtrlEx     m_edtReHistory;
	CPngBtn    m_btnFirst, m_btnPrev, m_btnNext, m_btnLast;

	MSG_LIST      m_mlRecord;
	SYSTEM_MSG_LIST m_systemMsgList;
	int m_nSearchFriendId;
	int m_nSearchClusterId;
	CLUSTER_SYSTEM_MSG_LIST m_clusterSystemMsgList;
	SYSTEM_MSG_LIST  m_mlSysMsgShow;    /*要显示的系统消息列表*/
	CLUSTER_SYSTEM_MSG_LIST m_mlClusterSysMsgShow;
	
	vector<FINDRANGE_ITEMDATA>	m_arrItemData;	// 复合框中记录的项数据
	BOOL   m_bSearch;//判断是否需要响应CMD_XTREAM_SEARCH_ACK
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	CPngBtn m_btnFind;
	CFlatEdit m_editKeyword;
	CGraphComboBox m_comboRange;
	CTransparentLabel m_lblRange;
	CTransparentLabel m_lblKeyword;
	afx_msg void OnBnClickedBtnFind();
	map<int,HTREEITEM>::iterator mapIt;
	afx_msg void OnBnClickedBtnFriend();
	afx_msg void OnBnClickedBtnCluster();
	afx_msg void OnBnClickedBtnSys();
	CCheckButton m_btnSearch;
	afx_msg void OnBnClickedBtnSearch();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	FRIEND_LIST		m_flRecent;

private:
	CUIMsgManageMainTab *m_pMainTab;
	CDuiOrderTree* m_pOrderTreeOrder;//交易消息
	CDuiOrderTree* m_pTranMsg;
	CLabelListWnd *m_pLabelList;
	CImgListWnd *m_pImgList;
	CRect m_rcRichEditSmall;
	CRect m_rcRichEditBig;
	HBRUSH m_hBkGround;
	COLORREF		m_colorBackground;
};

class CDuiMsgDlg : public DuiLib::WindowImplBase
{
public:
	CDuiMsgDlg();
	~CDuiMsgDlg();
	static std::tr1::shared_ptr<CDuiMsgDlg> CDuiMsgDlg::Show();
	static void UpdateBk();
	static void CloseDlg();
	
public:
	void SetDefaultFriend(uint32 fid);
	void SetDefaultCluster(uint32 cid);
	void SetOrderMsgRead(OrderMsg* pOrderMsg);
	void SetAllOrderMsgRead();
	void ShowOrderPage(int nOrderType);
	void ShowSystemMsgPage();
	void SetSysMsgTabOrderUnReadCount();
	void AddOrderMsg();
	static std::tr1::shared_ptr<CDuiMsgDlg> GetInstance()	{	return pInstance;	}

public:
	virtual LPCTSTR    GetWindowClassName() const   {   return _T("DuiMsgDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("MsgManage.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	virtual DuiLib::CControlUI* CreateControl(LPCTSTR pstrClassName);
	virtual void OnFinalMessage( HWND hWnd );
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);

private:
	CMsgDlg *m_pMsgDlg;
	static std::tr1::shared_ptr<CDuiMsgDlg> pInstance;
};
