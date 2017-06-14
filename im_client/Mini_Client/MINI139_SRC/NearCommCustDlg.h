#pragma once
#include "XSkinDlg.h"
#include "..\im\IMProtocol.h"
#include <vector>
#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"

class CResultListPeopleUI;////声明类
//

class CNearCommCustDlg : public DuiLib::WindowImplBase
{
		DECLARE_XT_MESSAGE_MAP()

public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("NearCommCustDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("NearCommCust.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass);
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	void    Notify(DuiLib::TNotifyUI& msg);
	// 显示对话框
	static std::tr1::shared_ptr<CNearCommCustDlg> Show();
	static void CloseDlg();
	static void UpdateBk();
	// 查询最近联系客户
private:
	static std::tr1::shared_ptr<CNearCommCustDlg> pInstance;

	DuiLib::CButtonUI *m_pBtnMin;
	DuiLib::CButtonUI *m_pBtnClose;

	DuiLib::CButtonUI *m_pBtnSearch;
	DuiLib::CButtonUI *m_pBtnStartComm;
	DuiLib::CButtonUI *m_pBtnViewRecord;
	
	/////开始时间和结束时间
	DuiLib::CDateTimeUI *m_pDateTimeBegin;
	DuiLib::CDateTimeUI *m_pDateTimeEnd;

	/////翻页按钮
	DuiLib::CButtonUI *m_pBtnFirst;
	DuiLib::CButtonUI *m_pBtnPre;
	DuiLib::CButtonUI *m_pBtnNext;
	DuiLib::CButtonUI *m_pBtnLast;

	
	DuiLib::CButtonUI *m_pBtnDealTime;////交易时间
	DuiLib::CEditUI   *m_pEditPageNo; ////第n页
	DuiLib::CLabelUI *m_pLabelPageAll;   ////0/1页

	BOOL m_bSearch;
	void OnSearchUser(const char *data, int nRetCode);
	
protected:	
	afx_msg void OnBnClickedSearch();
	// 前一页
	afx_msg void OnBnClickedPrevPage();
	// 后一页
	afx_msg void OnBnClickedNextPage();
	// 第一页
	afx_msg void OnBnClickedFirstPage();
	// 最后一页
	afx_msg void OnBnClickedLastPage();
	// 聊天
	afx_msg void OnBnClickedStartComm();
	// 查看聊天记录
	afx_msg void OnBnClickedViewRecord();

//	// 列表栏排序
	afx_msg void OnLvnColumnclickListOutput(NMHDR *pNMHDR, LRESULT *pResult);
	
	////按钮显示
	void SetBtnShow(BOOL bShow);
	
	// 获取最近联系客户
	void OnGetNearCommCust(const char *data, WORD wRet);

	// 获取控件指示时间
	//CString GetTime(const CFlatDateTimeCtrl &data, const CFlatDateTimeCtrl &time);

	// 获取Page页码
	int GetPageNo();

	// 设置Page页
	void SetPageNo(int nPageNo);

	// 界面显示客户信息
	void ShowCustInfo();

	// 用户改变了排序方式
	void OnChangeDirect();

	// 重新初始化数据，当起始结止时间改变时，所有保存的客户数据都要清空，重新从服务器获取
	void ReInitData();

	// 从当前缓存中获取客户数据
	BOOL GetPageData(int nPageNo, vector<XT_COMMINFO_GET> &vecCustInfo);

	// 给服务后台发送消息，获取客户总数目
	void GetCustCountFromServer();

	// 给后台发送消息，请求当前页数据
	void GetPageDataFromServer();

	// 输出客户信息
	void OutputCustInfo(const vector<XT_COMMINFO_GET> &vecCustInfo);

	// 清空界面显示的客户信息
	void ClearCustInfo();

	// 更新总页数
	void UpdatePageCount();

	// 获取列表栏选择的好友
	FRIEND_INFO* GetSelFriend();

private:
	static std::tr1::shared_ptr<CNearCommCustDlg> m_staInstance;

private:
	
	CResultListPeopleUI *m_listOutput;
	int m_nPageCount;

	BOOL m_bDirect;		// 买家信息排序方式（FALSE表示按最后联系时间从大到小排序，TRUE表示按最后联系时间从小到大排序）
	int m_nPageIndex;	// 页索引	

	// 保存每一页的最近联系人信息
	map<int, vector<XT_COMMINFO_GET>> m_mapArrCommInfo0;		// 最后联系时间按从大到小排序时存于此
	map<int, vector<XT_COMMINFO_GET>> m_mapArrCommInfo1;		// 最后联系时间按从小到大排序时存于此

	CString m_strBgnStartTime;									// 开始时间
	CString m_strBgnEndTime;									// 结束时间	

	int m_nCustCount;			// 总客户数			
	int m_nLineChosed;

	//virtual DuiLib::CControlUI* CreateControl(LPCTSTR pstrClassName)
	//{
	//	if (_tcsicmp(pstrClassName, _T("TalkBarWnd")) == 0)
	//	{
	//		CDateUI  *pUI  = new CDateUI;      
	//		RECT rc;
	//		rc.left = 0;
	//		rc.right = 0;
	//		rc.top = 0;
	//		rc.bottom = 0;
	//		m_timeBgn.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | ES_WANTRETURN
	//			/*| WS_HSCROLL | ES_AUTOHSCROLL*/  // 自动水平滚动
	//			| WS_VSCROLL | ES_AUTOVSCROLL, rc, CWnd::FromHandle(GetHWND()), 10002);
	//		pUI->Attach(m_timeBgn.GetSafeHwnd());          
	//		return pUI;
	//	}
	//}	
};

// 结果列表
class CResultListPeopleUI : public DuiLib::CListUI
{
public:
	CResultListPeopleUI(DuiLib::CPaintManagerUI& paint_manager);
	~CResultListPeopleUI();
	void AddItem(LPCTSTR szName, LPCTSTR szEndTime, LPCTSTR szDealInfo, LPCTSTR szBackInfo);////账号，联系时间，交易信息，备注
	void RemoveAll();

private:
	DuiLib::CPaintManagerUI& paint_manager_;

};


// CNearCommCustDlg dialog
////
////class CNearCommCustDlg : public CXSkinDlg
////{
////	DECLARE_DYNAMIC(CNearCommCustDlg)
////	DECLARE_HMG_SCROLLBAR()
////
////public:
////	CNearCommCustDlg(CWnd* pParent = NULL);   // standard constructor
////	virtual ~CNearCommCustDlg();
////
////	// 显示对话框
////	static void Show();
////
////protected:
////	BOOL OnInitDialog();
////	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
////	virtual void PostNcDestroy();
////
////	DECLARE_MESSAGE_MAP()
////	DECLARE_XT_MESSAGE_MAP()
////	afx_msg void OnPaint(); 
////	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
////
////	// 查询最近联系客户
////	afx_msg void OnBnClickedSearch();
////	// 前一页
////	afx_msg void OnBnClickedPrevPage();
////	// 后一页
////	afx_msg void OnBnClickedNextPage();
////	// 第一页
////	afx_msg void OnBnClickedFirstPage();
////	// 最后一页
////	afx_msg void OnBnClickedLastPage();
////	// 聊天
////	afx_msg void OnBnClickedStartComm();
////	// 查看聊天记录
////	afx_msg void OnBnClickedViewRecord();
////	// 列表栏排序
////	afx_msg void OnLvnColumnclickListOutput(NMHDR *pNMHDR, LRESULT *pResult);
////
////	// 获取最近联系客户
////	void OnGetNearCommCust(const char *data, WORD wRet);
////
////	// 获取控件指示时间
////	CString GetTime(const CFlatDateTimeCtrl &data, const CFlatDateTimeCtrl &time);
////
////	// 获取Page页码
////	int GetPageNo();
////
////	// 设置Page页
////	void SetPageNo(int nPageNo);
////
////	// 界面显示客户信息
////	void ShowCustInfo();
////
////	// 用户改变了排序方式
////	void OnChangeDirect();
////
////	// 重新初始化数据，当起始结止时间改变时，所有保存的客户数据都要清空，重新从服务器获取
////	void ReInitData();
////
////	// 从当前缓存中获取客户数据
////	BOOL GetPageData(int nPageNo, vector<XT_COMMINFO_GET> &vecCustInfo);
////
////	// 给服务后台发送消息，获取客户总数目
////	void GetCustCountFromServer();
////
////	// 给后台发送消息，请求当前页数据
////	void GetPageDataFromServer();
////
////	// 输出客户信息
////	void OutputCustInfo(const vector<XT_COMMINFO_GET> &vecCustInfo);
////
////	// 清空界面显示的客户信息
////	void ClearCustInfo();
////
////	// 更新总页数
////	void UpdatePageCount();
////
////	// 获取列表栏选择的好友
////	FRIEND_INFO* GetSelFriend();
////
////private:
////	static std::tr1::shared_ptr<CNearCommCustDlg> m_staInstance;
////
////private:
////	CFlatDateTimeCtrl m_dataBgn;
////	CFlatDateTimeCtrl m_timeBgn;
////	CFlatDateTimeCtrl m_dataEnd;
////	CFlatDateTimeCtrl m_timeEnd;
////	CBmpBtn m_btnSearch;
////	CBmpBtn m_btnStartComm;
////	CBmpBtn m_btnViewChatLog;
////	CFlatEdit m_editPageNo;
////	CXListCtrl m_listOutput;
////	int m_nPageCount;
////	CPngBtn m_btnFirstPage;
////	CPngBtn m_btnPrevPage;
////	CPngBtn m_btnNextPage;
////	CPngBtn m_btnLastPage;
////
////	HBRUSH	m_hBkSelfGround;
////
////	BOOL m_bDirect;		// 买家信息排序方式（FALSE表示按最后联系时间从大到小排序，TRUE表示按最后联系时间从小到大排序）
////	int m_nPageIndex;	// 页索引	
////
////	// 保存每一页的最近联系人信息
////	map<int, vector<XT_COMMINFO_GET>> m_mapArrCommInfo0;		// 最后联系时间按从大到小排序时存于此
////	map<int, vector<XT_COMMINFO_GET>> m_mapArrCommInfo1;		// 最后联系时间按从小到大排序时存于此
////
////	CString m_strBgnStartTime;									// 开始时间
////	CString m_strBgnEndTime;									// 结束时间	
////	
////	int m_nCustCount;			// 总客户数						
////};
