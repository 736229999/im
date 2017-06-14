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
	// ��Ϣ
	void Notify(DuiLib::TNotifyUI& msg);

	// ѡ��������
	void SetSearchType(FS_TYPE enType);
	static void UpdateBk();

private:
	// ���Һ���
	void SearchFriend();
	// ����Ⱥ
	void SearchCluster();
	// ��Ϊ����
	void AddFiend();
	// ����Ⱥ
	void AddCluster();

	void OnFriendSearchAck(const char *data,WORD wRet);
	void OnClusterSearchAck(const char *data,WORD wRet);

	// ˢ��ҳ�水ť״̬
	void RefreshPageBtn();

	// ���ң���ҳ��ť���ʱ���ң�
	BOOL Search();

	// �鿴��ϵ������
	void ViewFriendInfo(int nIndex);

	// �鿴Ⱥ����
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

	DuiLib::CButtonUI *m_pBtnPrePage;	// ��һҳ
	DuiLib::CButtonUI *m_pBtnFirstPage;	// ��һҳ
	DuiLib::CButtonUI *m_pBtnNextPage;	// ��һҳ
	DuiLib::CButtonUI *m_pBtnLastPage;	// ���һҳ

	DuiLib::CButtonUI *m_pBtnPrePageC;	// ��һҳ
	DuiLib::CButtonUI *m_pBtnNextPageC;	// ��һҳ

	DuiLib::CEditUI *m_pEditClusterSearchValue;

	int m_nCurPageNo;			// ��ǰҳ����
	int m_nPageCount;			// ��ҳ��

	HWND m_hDlgWnd;			// ����һ��HWND���Ա㴰�ڹرջ�������ر�ʱ�ܹ��ɹ�ע����̨����Э��

	int m_nLastReqSeq;

	static std::tr1::shared_ptr<CAddFriendOrClusterDlg> pInstance;
	map<uint32,DWORD>     m_mapReqTick;	
};

// ����б�
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
//// CFSDialog �Ի���  
//class CFSDialog : public CXSkinDlg
//{
//	DECLARE_DYNAMIC(CFSDialog)
//
//public:
//	virtual ~CFSDialog();
//
//// �Ի�������
//	enum { IDD = IDD_FRIEND_SEARCH };
//
//public:
//	static std::tr1::shared_ptr<CFSDialog> Show();
//	void SetSearchType(FS_TYPE type);
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
//	CFSDialog(CWnd* pParent = NULL);   // ��׼���캯��
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
//	BOOL            m_bSearch;//�ж��Ƿ���Ҫ��ӦCMD_XTREAM_SEARCH_ACK
//	
//public:
//	CTabCtrl m_ctlTabGroup;
//	afx_msg void OnTcnSelchangeTabGroup(NMHDR *pNMHDR, LRESULT *pResult);
//	void  InitTabCtrl();
//};
