#pragma once
#include "resource.h"
#include "Im\IMProtocol.h"
#include "Im\CustomListCtrl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>
#include "..\..\..\DuiLib\UIlib.h"
using namespace std;

class CSearchListUI;
class CPeopleSearchList : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("SearchListUI");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("PeopleSearchList.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass);
	void AddGroup(LPCTSTR szText, int nHeight = 32);
	void AddFriend(LPCTSTR szImgPath, LPCTSTR szText, LPCTSTR szRemarks, DWORD tag, BOOL bUser = TRUE, int nHeight = 32);
	void RemoveAll();
	int  GetCount();
	void SetCurSel(int nIndex);
	void Notify(DuiLib::TNotifyUI& msg);
	void SelPage(int nIndex);
	void SetTempSessionBtnText(LPCTSTR pText);
	// ����ѡ��������촰��
	void OpenCharWnd();
	// �Ƿ���ʾ��ʱ�Ự��ť
	void ShowTempSessionBtn(BOOL bShow);

public:
	int m_nLastReqSeq;

private:
	CSearchListUI *m_pSearchList;
	DuiLib::CTabLayoutUI *m_pTab;
	DuiLib::CButtonUI *m_pBtnTempSession;
};

class CSearchListUI : public DuiLib::CListUI
{
public:
	CSearchListUI(DuiLib::CPaintManagerUI& paint_manager);
	~CSearchListUI();
	bool AddGroup(LPCTSTR szText, int nHeight = 32);
	bool AddFriend(LPCTSTR szImgPath, LPCTSTR szText, LPCTSTR szRemarks, DWORD tag, BOOL bUser = TRUE, int nHeight = 32);

private:
	DuiLib::CPaintManagerUI& paint_manager_;
};

// CTabWndSearch �Ի���
#define   WM_SHOW_SEARCH_RESULT  (WM_USER + 2102)
class CTabWndSearch : public CDialog
{
	DECLARE_DYNAMIC(CTabWndSearch)

public:
	CTabWndSearch(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabWndSearch();
	// ��Ϣ·��
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// �Ի�������
	enum { IDD = IDD_DIALOG_TAB_SEARCH };

	// �����촰��
	void OpenCharWnd();

	// �Ƿ���ʾ��ʱ�Ự��ť
	void ShowTempSessionBtn(BOOL bShow);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	
	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()
	// �����û�
	void OnSearchUser(const char *data, int nRetCode);
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);

public:
	// �����б�����Ŀ
	void ShowSearchList(CString &str);
	void UpdateSearchList(CLUSTER_INFO * pCluster,CString strItem);
	HWND GetListHWnd()	{	return m_PeopleSearchList.GetHWND();	}

private:
	CPeopleSearchList m_PeopleSearchList;

public:
	BOOL  IsKeyMatch(CString strKey, FRIEND_INFO *pFriend,CLUSTER_INFO *pCluster);
	static BOOL IsIncludeChinese(const char *str);
	BOOL  ShowSearchRes(CString strKey,FRIEND_INFO *pFriend,CLUSTER_INFO *pCluster);
	BOOL  KeySearch(CString strKey,CString strName,FRIEND_INFO *pFriend,CLUSTER_INFO *pCluster);
	static BOOL KeySearch(CString strKey,CString strName);
	static vector<CString> GetLetter(CString strText);
	static CString  FindLetter(int nCode);

	map<CLUSTER_INFO*,CString > m_mapGroup;    ////Ⱥ����
	map<FRIEND_INFO* ,CString > m_mapCluster;  ////Ⱥ�г�Ա
	map<FRIEND_INFO* ,CString > m_mapTeamFriend;   ////�������Ա
	map<FRIEND_INFO* ,CString > m_mapFriend;   ////�������Ա
};
