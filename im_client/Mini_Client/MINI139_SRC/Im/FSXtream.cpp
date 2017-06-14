//// im\FSPage1.cpp : 实现文件
////
//
#include "stdafx.h"
//#include "..\IMClient.h"
//#include ".\fsxtream.h"
//
//
//
//// CFSSearchXtream 对话框
//#define ID_FRIENDSERCH 10000
//IMPLEMENT_DYNAMIC(CFSSearchXtream, CDialog)
//CFSSearchXtream::CFSSearchXtream(CWnd * pWndMsg,CWnd* pParent /*=NULL*/)
//	: CDialog(CFSSearchXtream::IDD, pParent)
//	, m_strUserName(_T(""))
//	, m_strEmail(_T(""))
//	, m_strNickName(_T(""))
//{
//	m_hAccel = NULL;
//	m_pWndMsg = pWndMsg;
//	Create(IDD,pParent);
//	m_hBrushBackGround = CreateSolidBrush(RGB(248,248,248));
//	m_hBrushDisable = CreateSolidBrush(RGB(200,200,200));
//}
//
//CFSSearchXtream::~CFSSearchXtream()
//{
//	DeleteObject(m_hBrushBackGround);
//	DeleteObject(m_hBrushDisable);
//	if(m_hAccel)
//	{
//		 DestroyAcceleratorTable(m_hAccel);
//	}
//}
//
//void CFSSearchXtream::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX,IDC_RADIO1,m_btnRadio1);
//	DDX_Control(pDX,IDC_RADIO2,m_btnRadio2);
//	DDX_Text(pDX,IDC_EDIT_USERNAME,m_strUserName);
//	DDX_Text(pDX,IDC_EDIT_NICKNAME,m_strNickName);
//	DDX_Text(pDX,IDC_EDIT_EMAIL,m_strEmail);
//	DDX_Control(pDX, IDC_EDIT_ID, m_edtID);
//}
//
//
//BEGIN_MESSAGE_MAP(CFSSearchXtream, CDialog)
//	ON_COMMAND(ID_FRIENDSERCH,OnFindFriend)
//	ON_MESSAGE(DM_GETDEFID, OnGetDefID)
//	ON_BN_CLICKED(IDC_RADIO1, OnBnClickedRadio)
//	ON_BN_CLICKED(IDC_RADIO2, OnBnClickedRadio)
//	ON_WM_CTLCOLOR()
//	ON_WM_ERASEBKGND()
//END_MESSAGE_MAP()
//
//void CFSSearchXtream::OnFindFriend()
//{
//	if(m_pWndMsg)
//	{
//		m_pWndMsg->SendMessage(WM_COMMAND,IDC_BTN_SEARCH,0);
//	}
//}
//
//LRESULT CFSSearchXtream::OnGetDefID(WPARAM wp, LPARAM lp) 
//{
//	return MAKELONG(ID_FRIENDSERCH ,DC_HASDEFID);// 
//} 
//
//// CFSSearchXtream 消息处理程序
//
//void CFSSearchXtream::OnBnClickedRadio()
//{
//	bool bCheck = m_btnRadio2.GetCheck();
//
//	GetDlgItem(IDC_EDIT_USERNAME)->ShowWindow(bCheck);
//	GetDlgItem(IDC_STATIC_NAME)->ShowWindow(bCheck);	
//	
//
//}
//
//BOOL CFSSearchXtream::OnInitDialog()
//{
//	CDialog::OnInitDialog();
//	m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_ACC_FINDFRIEND)); 
//	ASSERT(m_hAccel);
//
//	m_edtUserName.SubclassWindow(GetDlgItem(IDC_EDIT_USERNAME)->m_hWnd);
//	m_edtUserName.LimitText(30);
//	m_edtUserName.SetBorderColor(RGB(88,88,88));
//
//	m_edtEmail.SubclassWindow(GetDlgItem(IDC_EDIT_EMAIL)->m_hWnd);
//	m_edtEmail.LimitText(60);
//
//	m_edtNickName.SubclassWindow(GetDlgItem(IDC_EDIT_NICKNAME)->m_hWnd);
//	m_edtNickName.LimitText(30);
//
//	m_edtNickName.SetBorderColor(RGB(88,88,88));
//
//	m_btnRadio2.SetCheck(1);
//
//	GetDlgItem(IDC_STATIC_TIP)->SetFont(&g_font12B);
//	GetDlgItem(IDC_STATIC_TIP2)->SetFont(&g_font12B);
//
//	return TRUE; 
//}
//
//bool CFSSearchXtream::GetSearchReq(XT_XTREAM_SEARCH_REQ &req)
//{
//	if (m_btnRadio1.GetCheck())
//	{
//		//看谁在线上
//		req.search_type = XTREAM_SEARCH_ONLINE;
//	}
//	else
//	{
//		req.search_type = XTREAM_SEARCH_PRECISION;
//	}
//
//	req.count		= 0;//不受限制
//
//	if ( req.search_type==XTREAM_SEARCH_PRECISION )
//	{
//		if ( UpdateData() )
//		{
//			CString error;
//			CString sId;
//
//			m_strEmail.Trim();
//			m_strUserName.Trim();
//			m_strNickName.Trim();
//			m_edtID.GetWindowText(sId);
//			sId.Trim();
//				
//			req.id = atoi(sId);
//			strcpy(req.user_email,m_strEmail);
//			strcpy(req.user_name,m_strUserName);
//			strcpy(req.nick_name,m_strNickName);
//
//			if ( m_strNickName=="" && m_strEmail=="" && sId == "")
//			{
//				if ( m_strUserName=="" )
//				{
//					//MessageBox("请至少输入至少一个查找条件！",IM_CAPTION,MB_OK|MB_ICONINFORMATION);
//					CMessageBox::Prompt("请输入至少一个查找条件！");
//					m_edtUserName.SetFocus();
//
//					return false;
//				}
//				else
//				{
//					if ( ValidateString(m_strUserName,error)==false )
//					{
//						CMessageBox::Prompt("用户名错误！错误原因:"+error);
//						m_edtUserName.SetFocus();
//						return false;		
//					}
//
//					return true;//只根据名称查找
//				}
//			}
//			
//			if ( m_strEmail!="" )
//			{
//				if ( CheckEmail(m_strEmail,error)==false )
//				{
//					//MessageBox(error,IM_CAPTION,MB_OK|MB_ICONINFORMATION);
//					CMessageBox::Prompt("Email地址错误，错误原因:"+error);
//					m_edtEmail.SetFocus();
//					return false;
//				}
//
//				if ( ValidateString(m_strEmail,error)==false )
//				{ 
//					CMessageBox::Prompt("Email地址错误，错误原因:"+error);
//					m_edtEmail.SetFocus();
//					return false;
//				}
//			}
//
//			if ( m_strNickName!="" )
//			{
//				/*if ( ValidateString(m_strNickName,error)==false )
//				{ 
//					CMessageBox::Prompt("昵称错误，错误原因:"+error);
//					m_edtNickName.SetFocus();
//					return false;
//				}*/
//			}
//
//			return true;
//		}
//		else
//		{
//			return false;
//		}
//	}
//
//	return true;
//}
//
//BOOL CFSSearchXtream::PreTranslateMessage(MSG* pMsg)
//{
//	//if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST) 
//	//{	
//	//	/*if (m_hAccel && ::TranslateAccelerator(m_hWnd, m_hAccel, pMsg)) 
//	//		return TRUE; */
//	//} 
//	if (pMsg->message == WM_KEYDOWN)
//	{
//		switch (pMsg->wParam)
//		{
//		case VK_ESCAPE:
//		//case 0x0D:
//			return TRUE;
//		default:
//			break;
//		}
//	}
//
//	return CDialog::PreTranslateMessage(pMsg);
//}
//
//HBRUSH CFSSearchXtream::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//	
//	if ( nCtlColor==CTLCOLOR_STATIC )
//	{
//		UINT nId = pWnd->GetDlgCtrlID();
//
//		if ( nId==IDC_EDIT_USERNAME || nId==IDC_EDIT_NICKNAME || nId==IDC_EDIT_EMAIL )
//		{
//			
//		}
//		else
//		{
//			pDC->SetBkMode(TRANSPARENT);
//			hbr = g_hWhiteBrush;
//		}
///*
//		if ( nId==IDC_STATIC_TIP || nId==IDC_STATIC_TIP2 )
//		{
//			pDC->SetTextColor(RGB(74,123,211));
//		}
//*/
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
//BOOL CFSSearchXtream::OnEraseBkgnd(CDC* pDC)
//{
//	CRect rc;
//	GetClientRect(rc);
//	pDC->FillSolidRect(&rc,0xFFFFFF);
//	return FALSE;
//}
