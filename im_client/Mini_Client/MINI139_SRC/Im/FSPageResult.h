//#pragma once
//
//#include "..\resource.h"
//#include "afxcmn.h"
//#include "..\..\HmgSkin\XListCtrl.h"
//// CFSPageResult 对话框
//
//
//class CFSPageResult : public CDialog
//{
//	DECLARE_DYNAMIC(CFSPageResult)
////	DECLARE_XTREAM_SCROLLBAR()
//
//public:
//	// 在查询之前设置界面信息
//	void	PreFind();
//
//	CFSPageResult(CWnd* pParent = NULL);   // 标准构造函数
//	virtual ~CFSPageResult();
//
//	enum RESULT_TYPE { RT_XTREAM, RT_CLUSTER, };
//
//	FRIEND_INFO * GetSelUser();
//	XT_CLUSTER_INFO * GetSelCluster();
//
//	int			GetResultCount();
//
//	void SetSearchReq(const XT_CLUSTER_SEARCH_REQ &req);
//	void SetSearchReq(const XT_XTREAM_SEARCH_REQ &req);
//
//// 对话框数据
//	enum { IDD = IDD_FS_RESULT };
//
//protected:
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//	virtual BOOL OnInitDialog();
//	afx_msg void OnStnClickedStaticUserinfo();
//	afx_msg void OnStnClickedStaticPrev();
//	afx_msg void OnStnClickedStaticNext();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnNcDestroy();
//
//	DECLARE_MESSAGE_MAP()
//	DECLARE_XT_MESSAGE_MAP()
//
//	void OnFriendSearchAck(const char *data,WORD wRet);
//	void OnClusterSearchAck(const char *data,WORD wRet);
//
//private:
//	struct tagXtreamPage
//	{
//
//		void	AddFriend(FRIEND_INFO* pFriend)
//		{ ASSERT(pFriend); vecResults.push_back(pFriend); dicID[pFriend->GetId()] = true; };
//
//		bool	IsIDExist(uint32 id) const
//		{ return (dicID.find(id) != dicID.end()); };
//
//		int		Size() const
//		{ return vecResults.size(); };
//
//		const vector<FRIEND_INFO *> GetResult() const
//		{ return vecResults; };
//
//		~tagXtreamPage()
//		{
//			for ( int i=0; i<vecResults.size(); i++ )
//			{
//				delete (FRIEND_INFO *)vecResults[i];
//			}
//			vecResults.clear();
//		}
//
//		int						next_pos;
//	protected:
//		map<uint32,bool>		dicID;
//		vector<FRIEND_INFO *>	vecResults;
//		
//	};
//
//	struct tagClusterPage
//	{
//		bool	IsIDExist(uint32 id) const
//		{ return (dicID.find(id) != dicID.end()); };
//
//		void	AddFamily(XT_CLUSTER_INFO* pFamily)
//		{ ASSERT(pFamily); vecResults.push_back(pFamily); dicID[pFamily->nFamilyID] = true; };
//
//		int		Size() const
//		{ return vecResults.size(); };
//
//		const vector<XT_CLUSTER_INFO *> GetResult() const
//		{ return vecResults; };
//
//		~tagClusterPage()
//		{
//			for ( int i=0; i<vecResults.size(); i++ )
//			{
//				XT_CLUSTER_INFO* pClusterInfo;
//				pClusterInfo = vecResults[i];
//				delete pClusterInfo;
//			}
//			vecResults.clear();
//		}
//		int							next_pos;
//	protected:
//		vector<XT_CLUSTER_INFO *>	vecResults;
//		map<uint32,bool>			dicID;
//	};
//
//private:
//	void ClearXtreamPages();
//	void ClearClusterPages();
//	void ShowResult(tagXtreamPage *pPage);
//	void ShowResult(tagClusterPage *pPage);
//
//private:
//	XT_XTREAM_SEARCH_REQ   m_xtreamReq;
//	XT_CLUSTER_SEARCH_REQ  m_clusterReq;
//
//	//查找结果
//	vector<tagXtreamPage *> m_vecXtreamPages;
//	vector<tagClusterPage *> m_vecClusterPages;
//
//	tagXtreamPage * m_pCurXtreamPage;
//	tagClusterPage * m_pCurClusterPage;
//
//	HBRUSH   m_hBrushBackGround;
//	HBRUSH   m_hBrushDisable   ;
//
//private:
//	//CListCtrl m_wndListCluster;
//	//CListCtrl m_wndListFriend;
//
//	CXListCtrl m_wndListCluster;
//	CXListCtrl m_wndListFriend;
//	CHyperBtn m_btnNext,m_btnPre,m_btnUserInfo;
//
//private:
//	RESULT_TYPE   m_resultType;
//	
//public:
//	afx_msg void OnPaint();
//	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
//	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
//
//	void SetLastReqSet(uint32 seq) { m_nLastReqSeq = seq ;	}
//
//private:
//	uint32		  m_nLastReqSeq;		//最后一次请求的seq号
//
//public:
//	afx_msg void OnLvnItemchangedListCluster(NMHDR *pNMHDR, LRESULT *pResult);
//};
