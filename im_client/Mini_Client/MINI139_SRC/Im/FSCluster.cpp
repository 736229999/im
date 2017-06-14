#include "stdafx.h"
//#include "..\IMClient.h"
//#include "FSCluster.h"
//#include ".\fscluster.h"
//
//#define ID_FINDCLUSTER 10000
//// CFSSearchCluster 对话框
//
//IMPLEMENT_DYNAMIC(CFSSearchCluster, CDialog)
//CFSSearchCluster::CFSSearchCluster(CWnd * pWndMsg,CWnd* pParent /*=NULL*/)
//	: CDialog(CFSSearchCluster::IDD, pParent)
//{
//	m_hAccel = NULL;
//	m_pWndMsg = pWndMsg;
//	Create(IDD,pParent);
//}
//
//CFSSearchCluster::~CFSSearchCluster()
//{
//	if(m_hAccel)
//	{
//		DestroyAcceleratorTable(m_hAccel);
//	}
//}
//
//LRESULT CFSSearchCluster::OnGetDefID(WPARAM wp, LPARAM lp) 
//{
//	return MAKELONG(ID_FINDCLUSTER,DC_HASDEFID);// 
//} 
//
//void CFSSearchCluster::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX,IDC_EDIT_NAME,m_edtName);
//	DDX_Control(pDX,IDC_EDIT_ID, m_edtID);
//	DDX_Control(pDX,IDC_COMBO_TYPE, m_combType);
//}
//
//
//BEGIN_MESSAGE_MAP(CFSSearchCluster, CDialog)
//	ON_MESSAGE(DM_GETDEFID, OnGetDefID)
//	ON_COMMAND(ID_FINDCLUSTER,OnFindCluster)
//	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio)
//	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio)
//	ON_BN_CLICKED(IDC_RADIO3, OnBnClickedRadio)
//	ON_WM_CTLCOLOR()
//	ON_WM_ERASEBKGND()
//END_MESSAGE_MAP()
//
//
//void CFSSearchCluster::OnFindCluster()
//{
//	if(m_pWndMsg)
//	{
//		m_pWndMsg->SendMessage(WM_COMMAND,IDC_BTN_SEARCH,0);
//	}
//}
//// CFSSearchCluster 消息处理程序
//
//BOOL CFSSearchCluster::OnInitDialog()
//{
//	CDialog::OnInitDialog();
//
//	m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_ACC_FINDFRIEND)); 
//	ASSERT(m_hAccel);
//	CButton * pRadio1 = (CButton *)GetDlgItem(IDC_RADIO3);
//	pRadio1->SetCheck(1);
//
//	//ComboInitFamilyClass(&this->m_combType);
//	InitClusterType(&this->m_combType);///群分类
//
//	m_edtName.LimitText(19);
//	m_edtName.SetBorderColor(RGB(88,88,88));
//
//	m_edtID.SetBorderColor(RGB(88,88,88));
//	m_combType.SetCurSel(0);
//
//	OnBnClickedRadio();
//
//	GetDlgItem(IDC_STATIC_TIP)->SetFont(&g_font12B);
//
//	return TRUE;  
//}
//
//void CFSSearchCluster::OnBnClickedRadio()
//{
//	CButton *pRadio1 = (CButton *)GetDlgItem(IDC_RADIO1);
//	CButton *pRadio2 = (CButton *)GetDlgItem(IDC_RADIO2);
//	CButton *pRadio3 = (CButton *)GetDlgItem(IDC_RADIO3);
//
//	if ( pRadio1->GetCheck() )//按类型搜索
//	{
//		m_combType.ShowWindow(SW_SHOW);
//		m_edtID.ShowWindow(SW_HIDE);
//		m_edtName.ShowWindow(SW_HIDE);
//		SetDlgItemText(IDC_STATIC_TIP2,"请输入你要查找的群类型");
//	}
//	else
//	if ( pRadio2->GetCheck() )//按名称搜索
//	{
//		m_combType.ShowWindow(SW_HIDE);
//		m_edtID.ShowWindow(SW_HIDE);
//		m_edtName.ShowWindow(SW_SHOW);
//		SetDlgItemText(IDC_STATIC_TIP2,"请输入你要查找的群名称");
//	}
//	else//精确搜索，按ID
//	{
//		m_combType.ShowWindow(SW_HIDE);
//		m_edtID.ShowWindow(SW_SHOW);
//		m_edtName.ShowWindow(SW_HIDE);
//		SetDlgItemText(IDC_STATIC_TIP2,"请输入你要查找的群号");
//	}
//}
//
//bool CFSSearchCluster::GetSearchReq(XT_CLUSTER_SEARCH_REQ &req)
//{
//	CButton *pRadio1 = (CButton *)GetDlgItem(IDC_RADIO1);
//	CButton *pRadio2 = (CButton *)GetDlgItem(IDC_RADIO2);
//	CButton *pRadio3 = (CButton *)GetDlgItem(IDC_RADIO3);
//
//	CString str;
//
//	if ( pRadio1->GetCheck() )
//	{
//		req.search_type = CLUSTER_SEARCH_CLASS;
//		req.type_data.nFamilyClass = ComboGetFamilyClass(&m_combType);//(nSel==LB_ERR ? 0 : m_combType.GetItemData(nSel) )
//	}
//	else  if ( pRadio2->GetCheck() )
//	{
//		req.search_type = CLUSTER_SEARCH_NAME;
//
//		m_edtName.GetWindowText(str);
//		::TrimSpace2(str);
//
//		if ( str=="" )
//		{
//			//MessageBox("群名称不能为空!",IM_CAPTION,MB_OK|MB_ICONWARNING);
//			CMessageBox::Prompt("群名称不能为空!");
//			return false;
//		}
//
//		//CString error;
//		//if ( ValidateString(str,error)==false )
//		//{
//		//	CMessageBox::Prompt("群名错误！错误原因:"+error);
//		//	return false;
//		//}
//
//		strcpy(req.type_data.szFamilyName,str);
//	}
//	else
//	{
//		req.search_type = CLUSTER_SEARCH_ID;
//		m_edtID.GetWindowText(str);
//		str.Trim();
//
//		if (str=="")
//		{
//			//MessageBox("群号不能为空！",IM_CAPTION,MB_OK|MB_ICONWARNING);
//			CMessageBox::Prompt("群号不能为空！");
//			return false;
//		}
//
//		req.type_data.nFamilyID=atol((LPCSTR)str);
//	}
//
//	return true;
//}
//
//
//HBRUSH CFSSearchCluster::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	UINT nID = pWnd->GetDlgCtrlID();
//
//	if ( nCtlColor==CTLCOLOR_STATIC )
//	{
//		pDC->SetBkMode(TRANSPARENT);
//		hbr = g_hWhiteBrush;
///*
//		if ( nID==IDC_STATIC_TIP || nID==IDC_STATIC_TIP2 )
//		{
//			pDC->SetTextColor(RGB(74,123,211));
//		}
//		*/
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
//BOOL CFSSearchCluster::OnEraseBkgnd(CDC* pDC)
//{
//	CRect rc;
//	GetClientRect(rc);
//	pDC->FillSolidRect(&rc,0xFFFFFF);
//	return FALSE;
//}
