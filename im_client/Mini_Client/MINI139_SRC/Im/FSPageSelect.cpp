// FSPageSelect.cpp : 实现文件
//

#include "stdafx.h"
//#include "..\IMClient.h"
//#include "FSPageSelect.h"
//#include ".\fspageselect.h"
//
//
//// CFSPageSelect 对话框
//
//IMPLEMENT_DYNAMIC(CFSPageSelect, CDialog)
//CFSPageSelect::CFSPageSelect(CWnd* pWndMsg,CWnd* pParent)
//	: CDialog(CFSPageSelect::IDD, pParent)
//{
//	m_pFSXtream  = NULL;
//	m_pFSCluster = NULL;
//	m_fsType     = FS_XTREAM;
//
//	m_pWndMsg = pWndMsg;
//
//	Create(IDD,pParent);
//}
//
//CFSPageSelect::~CFSPageSelect()
//{
//	delete m_pFSXtream;
//	delete m_pFSCluster;
//}
//
//void CFSPageSelect::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//}
//
//BEGIN_MESSAGE_MAP(CFSPageSelect, CDialog)
//	ON_WM_DESTROY()
//	ON_WM_SIZE()
//	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio)
//	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio)
//	ON_WM_CTLCOLOR()
//	ON_WM_ERASEBKGND()
//END_MESSAGE_MAP()
//
//FS_TYPE CFSPageSelect::GetSearchType()
//{
//	return m_fsType;
//}
//// CFSPageSelect 消息处理程序
//void CFSPageSelect::SetSearchType(FS_TYPE type)
//{
//	CButton * pRadio1 = (CButton *)GetDlgItem(IDC_RADIO1);
//	CButton * pRadio2 = (CButton *)GetDlgItem(IDC_RADIO2);
//
//	if ( type==FS_XTREAM )
//	{
//		pRadio1->SetCheck(1);
//		pRadio2->SetCheck(0);
//	}
//	else
//	{
//		pRadio2->SetCheck(1);
//		pRadio1->SetCheck(0);
//	}
//
//	OnBnClickedRadio();
//}
//void CFSPageSelect::OnDestroy()
//{
//	CDialog::OnDestroy();
//
//	m_pFSXtream->DestroyWindow();
//	m_pFSCluster->DestroyWindow();
//}
//
//BOOL CFSPageSelect::OnInitDialog()
//{
//	CDialog::OnInitDialog();
//
//	//pFrame = (CSAFrame *)GetParent();
//
//	CRect rcTab;
//	GetClientRect(&rcTab);
//	//rcTab.top+=35;
//
//	m_pFSXtream = new CFSSearchXtream(m_pWndMsg,this);
//	m_pFSXtream->MoveWindow(&rcTab);
//	m_pFSXtream->ShowWindow(SW_SHOW);
//
//	m_pFSCluster = new CFSSearchCluster(m_pWndMsg,this);
//	m_pFSCluster->MoveWindow(&rcTab);
//	m_pFSCluster->ShowWindow(SW_HIDE);
//
//	CButton * pRadio1 = (CButton *)GetDlgItem(IDC_RADIO1);
//	pRadio1->SetCheck(1);
//	//pFrame->SetWindowText("查找联系人");
//
//	return TRUE;  
//}
//
//void CFSPageSelect::OnSize(UINT nType, int cx, int cy)
//{
//	CDialog::OnSize(nType, cx, cy);
//
//	if ( m_pFSCluster && m_pFSCluster->m_hWnd )
//	{
//		m_pFSCluster->MoveWindow(0,12,cx,cy-10);
//	}
//
//	if ( m_pFSXtream && m_pFSXtream->m_hWnd )
//	{
//		m_pFSXtream->MoveWindow(0,12,cx,cy-10);
//	}
//}
//
//void CFSPageSelect::OnBnClickedRadio()
//{
//	CButton * pRadio1 = (CButton *)GetDlgItem(IDC_RADIO1);
//	
//	if ( pRadio1->GetCheck() )
//	{
//		m_fsType = FS_XTREAM;
//
//		if ( !m_pFSXtream->IsWindowVisible() )
//		{
//			m_pFSXtream->ShowWindow(SW_SHOW);
//			m_pFSCluster->ShowWindow(SW_HIDE);
//		}
//
//		//pFrame->SetWindowText("查找联系人");
//		//pFrame->Invalidate(this);
//	}
//	else
//	{
//		m_fsType = FS_CLUSTER;
//
//		if ( !m_pFSCluster->IsWindowVisible() )
//		{
//			m_pFSCluster->ShowWindow(SW_SHOW);
//			m_pFSXtream->ShowWindow(SW_HIDE);
//		}
//
//		//pFrame->SetWindowText("查找群");
//		//pFrame->Invalidate(this);
//	}
//}
//
//bool CFSPageSelect::GetSearchReq(XT_CLUSTER_SEARCH_REQ &req)
//{
//	return m_pFSCluster->GetSearchReq(req);
//}
//
//bool CFSPageSelect::GetSearchReq(XT_XTREAM_SEARCH_REQ &req)
//{
//	return m_pFSXtream->GetSearchReq(req);
//}
//
//HBRUSH CFSPageSelect::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	if ( nCtlColor==CTLCOLOR_STATIC )
//	{
//		pDC->SetBkMode(TRANSPARENT);
//		hbr = g_hWhiteBrush;
//	}
//
//	if ( nCtlColor==CTLCOLOR_DLG  )
//	{
//		hbr = g_hWhiteBrush;
//	}
//
//	return hbr;
//}
//
//BOOL CFSPageSelect::OnEraseBkgnd(CDC* pDC)
//{
//	CRect rc;
//	GetClientRect(rc);
//	pDC->FillSolidRect(&rc,0xFFFFFF);
//
//	return FALSE;
//}
