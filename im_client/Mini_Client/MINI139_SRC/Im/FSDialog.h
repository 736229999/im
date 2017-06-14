#pragma once

#include "impublic.h"
#include "..\resource.h"

#include "fsPageResult.h"
#include "FSPageSelect.h"

#include  "..\controls\skinctrl.h"

#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"

enum FS_TYPE 
{
	FS_XTREAM  = 0,
	FS_CLUSTER = 1, 
};

class CResultListUI;
class CAddFriendOrClusterDlg : public DuiLib::WindowImplBase
{
	DECLARE_XT_MESSAGE_MAP()

public:
	static std::tr1::shared_ptr<CAddFriendOrClusterDlg> Show();
	static void CloseDlg();

public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("AddFriendOrCluster");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("AddFriendOrCluster.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass);
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	// 消息
	void Notify(DuiLib::TNotifyUI& msg);

	// 选择搜索项
	void SetSearchType(FS_TYPE enType);
	static void UpdateBk();

private:
	// 查找好友
	void SearchFriend();
	// 查找群
	void SearchCluster();
	// 加为好友
	void AddFiend();
	// 加入群
	void AddCluster();

	void OnFriendSearchAck(const char *data,WORD wRet);
	void OnClusterSearchAck(const char *data,WORD wRet);

	// 刷新页面按钮状态
	void RefreshPageBtn();

	// 查找（分页按钮点击时查找）
	BOOL Search();

	// 查看联系人资料
	void ViewFriendInfo(int nIndex);

	// 查看群资料
	void ViewClusterInfo(int nIndex);

private:
	CWndShadow m_WndShadow;
	CResultListUI *m_pResultList;
	DuiLib::CEditUI	*m_pEditFriendName;
	DuiLib::CLabelUI *m_pLabelPageNo;
	DuiLib::CEditUI *m_pEditclusterSearchValue;
	CResultListUI *m_pResultCList;
	DuiLib::CButtonUI *m_pBtnBack;
	DuiLib::CButtonUI *m_pBtnNext;
	DuiLib::CButtonUI *m_pClose;
	DuiLib::CTabLayoutUI *m_pTabLayout;
	DuiLib::COptionUI *m_pOptionXtream;
	DuiLib::COptionUI *m_pOptionCluster;
	DuiLib::COptionUI *m_pOptionFriendPreciseSearch;
	DuiLib::COptionUI *m_pOptionClusterID;
	DuiLib::CLabelUI *m_pLabelFriendName;
	DuiLib::CLabelUI *m_pLabelPageCount;
	DuiLib::CLabelUI *m_pLabelPageNoC;

	DuiLib::CButtonUI *m_pBtnPrePage;	// 上一页
	DuiLib::CButtonUI *m_pBtnFirstPage;	// 第一页
	DuiLib::CButtonUI *m_pBtnNextPage;	// 下一页
	DuiLib::CButtonUI *m_pBtnLastPage;	// 最后一页

	DuiLib::CButtonUI *m_pBtnPrePageC;	// 上一页
	DuiLib::CButtonUI *m_pBtnNextPageC;	// 下一页

	DuiLib::CEditUI *m_pEditClusterSearchValue;

	int m_nCurPageNo;			// 当前页索引
	int m_nPageCount;			// 总页数

	HWND m_hDlgWnd;			// 保存一份HWND，以便窗口关闭或非正常关闭时能够成功注销后台交互协议

	int m_nLastReqSeq;

	static std::tr1::shared_ptr<CAddFriendOrClusterDlg> pInstance;
	map<uint32,DWORD>     m_mapReqTick;	
};

// 结果列表
class CResultListUI : public DuiLib::CListUI
{
public:
	CResultListUI(DuiLib::CPaintManagerUI& paint_manager);
	~CResultListUI();
	void SetFsType(FS_TYPE enType)	{	m_enType = enType;	}
	void AddItem(LPCTSTR szName, LPCTSTR szNickName, LPCTSTR szSex, DWORD tag);
	void RemoveAll();

private:
	DuiLib::CPaintManagerUI& paint_manager_;
	FS_TYPE m_enType;
};

//
//// CFSDialog 对话框  
//class CFSDialog : public CXSkinDlg
//{
//	DECLARE_DYNAMIC(CFSDialog)
//
//public:
//	virtual ~CFSDialog();
//
//// 对话框数据
//	enum { IDD = IDD_FRIEND_SEARCH };
//
//public:
//	static std::tr1::shared_ptr<CFSDialog> Show();
//	void SetSearchType(FS_TYPE type);
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	virtual BOOL OnInitDialog();
//	virtual void OnHelpClick();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnNcDestroy();
//	afx_msg void OnBnClickedBtnSearch();
//	afx_msg void OnBnClickedBtnAdd();
//	afx_msg void OnBnClickedBtnPrev();
//	afx_msg void OnBnClickedBtnClose();
//	afx_msg void OnTimer(UINT nIDEvent);
//	afx_msg void OnClose();
//	afx_msg void OnPaint();
//	LRESULT OnGetDefID(WPARAM wp, LPARAM lp) ;
//
//	DECLARE_MESSAGE_MAP()
//	DECLARE_XT_MESSAGE_MAP()
//
//	void ShowPage(int nPage);
//	void OnBeginSearch(const char *data,WORD wRet);
//	void OnEndSearch(const char *data,WORD wRet);
//
//private:
//	CFSDialog(CWnd* pParent = NULL);   // 标准构造函数
//
//	static std::tr1::shared_ptr<CFSDialog> pInstance;
//	      
//	CBmpBtn       m_btnSearch ;
//	CBmpBtn       m_btnAdd    ;
//	CBmpBtn       m_btnClose  ;
//	CBmpBtn       m_btnPrev   ;
//
//	CSAFrame    	 m_wndPage;
//	CFSPageSelect  * m_pPageSelect;
//	CFSPageResult  * m_pPageResult;
//
//	int			m_nPage ;
//	FS_TYPE     m_fsType; //0 xtream search, 1 cluster search
//
//	map<uint32,DWORD>     m_mapReqTick    ;	
//
//	CResBitmap		m_bmpLogo;
//	int				m_nDefault;	
//	HACCEL			m_hAccel;
//	BOOL            m_bSearch;//判断是否需要响应CMD_XTREAM_SEARCH_ACK
//	
//public:
//	CTabCtrl m_ctlTabGroup;
//	afx_msg void OnTcnSelchangeTabGroup(NMHDR *pNMHDR, LRESULT *pResult);
//	void  InitTabCtrl();
//};
