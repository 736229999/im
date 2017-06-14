#pragma once
#include "XSkinDlg.h"
#include "..\im\IMProtocol.h"
#include <vector>
#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"

class CResultListPeopleUI;////������
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
	// ��ʾ�Ի���
	static std::tr1::shared_ptr<CNearCommCustDlg> Show();
	static void CloseDlg();
	static void UpdateBk();
	// ��ѯ�����ϵ�ͻ�
private:
	static std::tr1::shared_ptr<CNearCommCustDlg> pInstance;

	DuiLib::CButtonUI *m_pBtnMin;
	DuiLib::CButtonUI *m_pBtnClose;

	DuiLib::CButtonUI *m_pBtnSearch;
	DuiLib::CButtonUI *m_pBtnStartComm;
	DuiLib::CButtonUI *m_pBtnViewRecord;
	
	/////��ʼʱ��ͽ���ʱ��
	DuiLib::CDateTimeUI *m_pDateTimeBegin;
	DuiLib::CDateTimeUI *m_pDateTimeEnd;

	/////��ҳ��ť
	DuiLib::CButtonUI *m_pBtnFirst;
	DuiLib::CButtonUI *m_pBtnPre;
	DuiLib::CButtonUI *m_pBtnNext;
	DuiLib::CButtonUI *m_pBtnLast;

	
	DuiLib::CButtonUI *m_pBtnDealTime;////����ʱ��
	DuiLib::CEditUI   *m_pEditPageNo; ////��nҳ
	DuiLib::CLabelUI *m_pLabelPageAll;   ////0/1ҳ

	BOOL m_bSearch;
	void OnSearchUser(const char *data, int nRetCode);
	
protected:	
	afx_msg void OnBnClickedSearch();
	// ǰһҳ
	afx_msg void OnBnClickedPrevPage();
	// ��һҳ
	afx_msg void OnBnClickedNextPage();
	// ��һҳ
	afx_msg void OnBnClickedFirstPage();
	// ���һҳ
	afx_msg void OnBnClickedLastPage();
	// ����
	afx_msg void OnBnClickedStartComm();
	// �鿴�����¼
	afx_msg void OnBnClickedViewRecord();

//	// �б�������
	afx_msg void OnLvnColumnclickListOutput(NMHDR *pNMHDR, LRESULT *pResult);
	
	////��ť��ʾ
	void SetBtnShow(BOOL bShow);
	
	// ��ȡ�����ϵ�ͻ�
	void OnGetNearCommCust(const char *data, WORD wRet);

	// ��ȡ�ؼ�ָʾʱ��
	//CString GetTime(const CFlatDateTimeCtrl &data, const CFlatDateTimeCtrl &time);

	// ��ȡPageҳ��
	int GetPageNo();

	// ����Pageҳ
	void SetPageNo(int nPageNo);

	// ������ʾ�ͻ���Ϣ
	void ShowCustInfo();

	// �û��ı�������ʽ
	void OnChangeDirect();

	// ���³�ʼ�����ݣ�����ʼ��ֹʱ��ı�ʱ�����б���Ŀͻ����ݶ�Ҫ��գ����´ӷ�������ȡ
	void ReInitData();

	// �ӵ�ǰ�����л�ȡ�ͻ�����
	BOOL GetPageData(int nPageNo, vector<XT_COMMINFO_GET> &vecCustInfo);

	// �������̨������Ϣ����ȡ�ͻ�����Ŀ
	void GetCustCountFromServer();

	// ����̨������Ϣ������ǰҳ����
	void GetPageDataFromServer();

	// ����ͻ���Ϣ
	void OutputCustInfo(const vector<XT_COMMINFO_GET> &vecCustInfo);

	// ��ս�����ʾ�Ŀͻ���Ϣ
	void ClearCustInfo();

	// ������ҳ��
	void UpdatePageCount();

	// ��ȡ�б���ѡ��ĺ���
	FRIEND_INFO* GetSelFriend();

private:
	static std::tr1::shared_ptr<CNearCommCustDlg> m_staInstance;

private:
	
	CResultListPeopleUI *m_listOutput;
	int m_nPageCount;

	BOOL m_bDirect;		// �����Ϣ����ʽ��FALSE��ʾ�������ϵʱ��Ӵ�С����TRUE��ʾ�������ϵʱ���С��������
	int m_nPageIndex;	// ҳ����	

	// ����ÿһҳ�������ϵ����Ϣ
	map<int, vector<XT_COMMINFO_GET>> m_mapArrCommInfo0;		// �����ϵʱ�䰴�Ӵ�С����ʱ���ڴ�
	map<int, vector<XT_COMMINFO_GET>> m_mapArrCommInfo1;		// �����ϵʱ�䰴��С��������ʱ���ڴ�

	CString m_strBgnStartTime;									// ��ʼʱ��
	CString m_strBgnEndTime;									// ����ʱ��	

	int m_nCustCount;			// �ܿͻ���			
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
	//			/*| WS_HSCROLL | ES_AUTOHSCROLL*/  // �Զ�ˮƽ����
	//			| WS_VSCROLL | ES_AUTOVSCROLL, rc, CWnd::FromHandle(GetHWND()), 10002);
	//		pUI->Attach(m_timeBgn.GetSafeHwnd());          
	//		return pUI;
	//	}
	//}	
};

// ����б�
class CResultListPeopleUI : public DuiLib::CListUI
{
public:
	CResultListPeopleUI(DuiLib::CPaintManagerUI& paint_manager);
	~CResultListPeopleUI();
	void AddItem(LPCTSTR szName, LPCTSTR szEndTime, LPCTSTR szDealInfo, LPCTSTR szBackInfo);////�˺ţ���ϵʱ�䣬������Ϣ����ע
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
////	// ��ʾ�Ի���
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
////	// ��ѯ�����ϵ�ͻ�
////	afx_msg void OnBnClickedSearch();
////	// ǰһҳ
////	afx_msg void OnBnClickedPrevPage();
////	// ��һҳ
////	afx_msg void OnBnClickedNextPage();
////	// ��һҳ
////	afx_msg void OnBnClickedFirstPage();
////	// ���һҳ
////	afx_msg void OnBnClickedLastPage();
////	// ����
////	afx_msg void OnBnClickedStartComm();
////	// �鿴�����¼
////	afx_msg void OnBnClickedViewRecord();
////	// �б�������
////	afx_msg void OnLvnColumnclickListOutput(NMHDR *pNMHDR, LRESULT *pResult);
////
////	// ��ȡ�����ϵ�ͻ�
////	void OnGetNearCommCust(const char *data, WORD wRet);
////
////	// ��ȡ�ؼ�ָʾʱ��
////	CString GetTime(const CFlatDateTimeCtrl &data, const CFlatDateTimeCtrl &time);
////
////	// ��ȡPageҳ��
////	int GetPageNo();
////
////	// ����Pageҳ
////	void SetPageNo(int nPageNo);
////
////	// ������ʾ�ͻ���Ϣ
////	void ShowCustInfo();
////
////	// �û��ı�������ʽ
////	void OnChangeDirect();
////
////	// ���³�ʼ�����ݣ�����ʼ��ֹʱ��ı�ʱ�����б���Ŀͻ����ݶ�Ҫ��գ����´ӷ�������ȡ
////	void ReInitData();
////
////	// �ӵ�ǰ�����л�ȡ�ͻ�����
////	BOOL GetPageData(int nPageNo, vector<XT_COMMINFO_GET> &vecCustInfo);
////
////	// �������̨������Ϣ����ȡ�ͻ�����Ŀ
////	void GetCustCountFromServer();
////
////	// ����̨������Ϣ������ǰҳ����
////	void GetPageDataFromServer();
////
////	// ����ͻ���Ϣ
////	void OutputCustInfo(const vector<XT_COMMINFO_GET> &vecCustInfo);
////
////	// ��ս�����ʾ�Ŀͻ���Ϣ
////	void ClearCustInfo();
////
////	// ������ҳ��
////	void UpdatePageCount();
////
////	// ��ȡ�б���ѡ��ĺ���
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
////	BOOL m_bDirect;		// �����Ϣ����ʽ��FALSE��ʾ�������ϵʱ��Ӵ�С����TRUE��ʾ�������ϵʱ���С��������
////	int m_nPageIndex;	// ҳ����	
////
////	// ����ÿһҳ�������ϵ����Ϣ
////	map<int, vector<XT_COMMINFO_GET>> m_mapArrCommInfo0;		// �����ϵʱ�䰴�Ӵ�С����ʱ���ڴ�
////	map<int, vector<XT_COMMINFO_GET>> m_mapArrCommInfo1;		// �����ϵʱ�䰴��С��������ʱ���ڴ�
////
////	CString m_strBgnStartTime;									// ��ʼʱ��
////	CString m_strBgnEndTime;									// ����ʱ��	
////	
////	int m_nCustCount;			// �ܿͻ���						
////};
