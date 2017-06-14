// im\DialogClusterAuth.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "DialogClusterAuth.h"
#include ".\dialogclusterauth.h"
#include "DuiP2PSendMsgDlg.h"

using namespace DuiLib;
// CDialogClusterAuth 对话框
#define MAX_CHAR_LEN 60
//map<uint32,void *> CDialogClusterAuth::m_mapClusterAuthId;
extern  void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);

CDialogClusterAuth::CDialogClusterAuth(uint32 cid, CWnd* pParent /*=NULL*/)
{
	m_uiClusterId = cid;
	//m_mapClusterAuthId.insert(map<uint32,void *>::value_type(cid,(void *)this));
}

CDialogClusterAuth::CDialogClusterAuth()
{

}
CDialogClusterAuth::~CDialogClusterAuth()
{
	/*map<uint32,void *>::iterator it;
	it = m_mapClusterAuthId.find(m_uiClusterId);
	if ( it!=m_mapClusterAuthId.end() )
	{
		m_mapClusterAuthId.erase(it);
	}*/
}

std::tr1::shared_ptr<CDialogClusterAuth> CDialogClusterAuth::pInstance = NULL;
std::tr1::shared_ptr<CDialogClusterAuth> CDialogClusterAuth::Show(uint32 cid)
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		BringWindowToTop(pInstance->GetHWND());
		pInstance->UpdateClusterInfo(cid);
	}
	else
	{
		pInstance = tr1::shared_ptr<CDialogClusterAuth>(new CDialogClusterAuth());
		pInstance->Create(GetDesktopWindow(), _T(""), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->UpdateClusterInfo(cid);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}
	return pInstance;
}

void CDialogClusterAuth::UpdateClusterInfo(uint32 cid)
{
	m_uiClusterId = cid;////更新群号
	m_pTabLayout->SelectItem(0); ////切换到0窗口
	m_pTabLayoutB->SelectItem(0);////切换到0窗口
}

void CDialogClusterAuth::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));

	SetIcon(IDR_MAINFRAME);

	//////////获取控件变量
	m_pBtnClose = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnclose"));

	m_pTabLayout = static_cast<DuiLib::CTabLayoutUI*>(m_PaintManager.FindControl("tabs"));
	m_pEditVerify  = static_cast<DuiLib::CRichEditUI*>(m_PaintManager.FindControl("verifyInfoEdit"));

	m_pEditShowInfo= static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl("ShowInfoEdit"));

	m_pTabLayoutB = static_cast<DuiLib::CTabLayoutUI*>(m_PaintManager.FindControl("tabsB"));
	m_pBtnAdd = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnAdd"));
	m_pBtnCancel  = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnCancel"));
   
	m_pBtnReAdd   = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnReAdd"));
    m_pBtnReCancel= static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnReCancel"));

	m_pEditVerify->SetLimitText(MAX_CHAR_LEN);
	m_pEditShowInfo->SetReadOnly(TRUE);
	UpdateBk();
}

// 消息
void CDialogClusterAuth::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender == m_pBtnClose)    ////关闭
		{
			Close(IDC_OK);
		}
		else if(msg.pSender == m_pBtnAdd)  ////入群
		{
			OnBtnAdd();
		}
		else if(msg.pSender == m_pBtnCancel)//取消
		{
			OnBnClickedCancel();
		}
		else if(msg.pSender == m_pBtnReAdd)  ////继续入群
		{
			OnBnClickedAddAgain();
		}
		else if(msg.pSender == m_pBtnReCancel)//取消
		{
			OnBnClickedCancel();
		}
	}
}


LRESULT CDialogClusterAuth::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}
	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CDialogClusterAuth::UpdateBk()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			::UpdateBk(background, pInstance->GetHWND());
		}
	}
}


void CDialogClusterAuth::OnBtnAdd()
{
	
		XT_CLUSTER_AUTH_REQ req;

		req.nAction = 0;
		req.nFamilyProp = PERM_FAMILY;
		req.nFamilyID = m_uiClusterId;
		req.nRequestID = g_ImUserInfo.GetId();
		req.nManagerID = 0;
		req.send_time  = time(NULL);
		req.nRequestID = g_ImUserInfo.GetId();

		CString strTxt = m_pEditVerify->GetText();
		sprintf(req.info,_T("%s"),strTxt);
		g_pProtocol->SendReq(req);

		if(m_pTabLayout->GetCurSel() == 0)
		{
			m_pTabLayout->SelectItem(1);////切换到1窗口
			m_pTabLayoutB->SelectItem(1);////切换到1窗口
		}
	
}

void CDialogClusterAuth::OnBnClickedCancel()
{
	Close(IDOK);
}

void CDialogClusterAuth::OnBnClickedAddAgain()
{
	// TODO: 在此添加控件通知处理程序代码
	pInstance->Close(IDOK);
}


void CDialogClusterAuth::OnBnClickedClose()
{
	// TODO: 在此添加控件通知处理程序代码
	pInstance->Close(IDOK);
}
	
////IMPLEMENT_DYNAMIC(CDialogClusterAuth, CXSkinDlg)
////IMPLEMENT_HMG_SCROLLBAR(CDialogClusterAuth)
////
////CDialogClusterAuth::CDialogClusterAuth(uint32 cid, CWnd* pParent /*=NULL*/)
////	: CXSkinDlg(CDialogClusterAuth::IDD, pParent)
////{
////	m_hBkGround = CreateSolidBrush(RGB(255,255,255));
////	m_uiClusterId = cid;
////	m_mapClusterAuthId.insert(map<uint32,void *>::value_type(cid,(void *)this));
////}
////
////CDialogClusterAuth::~CDialogClusterAuth()
////{
////	map<uint32,void *>::iterator it;
////	it = m_mapClusterAuthId.find(m_uiClusterId);
////
////	if ( it!=m_mapClusterAuthId.end() )
////		m_mapClusterAuthId.erase(it);
////	DeleteObject(m_hBkGround);
////}
////
////void CDialogClusterAuth::DoDataExchange(CDataExchange* pDX)
////{
////	CXSkinDlg::DoDataExchange(pDX);
////	DDX_Control(pDX, IDC_EDIT_AUTH, m_edtAuthInfo);
////	DDX_Control(pDX, IDC_OK,m_btnOk);
////	DDX_Control(pDX, IDC_CANCEL,m_btnCancel);
////	DDX_Control(pDX, IDC_ADD_AGAIN, m_btnAddAgain);
////	DDX_Control(pDX, IDC_CLOSE, m_btnClose);
////}
////
////
////BEGIN_MESSAGE_MAP(CDialogClusterAuth, CXSkinDlg)
////	ON_BN_CLICKED(IDC_OK, OnBtnOK)
////	ON_WM_CLOSE()
////	ON_WM_DESTROY()
////	ON_WM_CTLCOLOR()
////	ON_BN_CLICKED(IDC_CANCEL, OnBnClickedCancel)
////	SET_HMG_SCROLLBAR(IDC_EDIT_AUTH)
////	ON_WM_ERASEBKGND()
////	ON_BN_CLICKED(IDC_ADD_AGAIN, OnBnClickedAddAgain)
////	ON_BN_CLICKED(IDC_CLOSE, OnBnClickedClose)
////END_MESSAGE_MAP()
////
////
////// CDialogClusterAuth 消息处理程序
////
////void CDialogClusterAuth::OnBtnOK()
////{
////	XT_CLUSTER_AUTH_REQ req;
////
////	req.nAction = 0;
////	req.nFamilyProp = PERM_FAMILY;
////	req.nFamilyID = m_uiClusterId;
////	req.nRequestID = g_ImUserInfo.GetId();
////	req.nManagerID = 0;
////	req.send_time  = time(NULL);
////	req.nRequestID = g_ImUserInfo.GetId();
////	
////	GetDlgItemText(IDC_EDIT_AUTH,req.info,sizeof(req.info));
////	g_pProtocol->SendReq(req);
////	HideUI();//隐藏窗口
////	//DestroyWindow();
////}
////
////BOOL CDialogClusterAuth::OnInitDialog()
////{
////	CXSkinDlg::OnInitDialog();
////
////	SetDlgStyle(MINCLOSE);
////	/*SetWindowText("群验证");*/
////	SetWindowText("添加/查找");
////	InitDisplay();//初始化显示
////	/*CString str;
////	str.Format("群 %u",m_uiClusterId);
////	SetDlgItemText(IDC_STATIC_INFO,str);*/
////
////	m_edtAuthInfo.LimitText(/*99*/120);
////	//m_edtAuthInfo.SetWindowText("你好，同意我加入群好吗？");
////
////	AttachHMGScrollbar(m_edtAuthInfo.m_hWnd);
////
////	return TRUE;  
////}
////
////CDialogClusterAuth * CDialogClusterAuth::Show(uint32 cid)
////{
////	map<uint32,void *>::iterator it = m_mapClusterAuthId.find(cid);
////
////	CDialogClusterAuth *pDlg = NULL;
////
////	if ( it==m_mapClusterAuthId.end() )
////	{
////		pDlg = new CDialogClusterAuth(cid,GetDesktopWindow());
////		pDlg->Create(CDialogClusterAuth::IDD,GetDesktopWindow());
////		pDlg->ShowWindow(SW_SHOW);
////		m_mapClusterAuthId.insert(map<uint32,void *>::value_type(cid,(void *)pDlg));
////	}
////	else
////	{
////		pDlg = (CDialogClusterAuth *)it->second;
////
////		if (pDlg->IsIconic())
////			pDlg->ShowWindow(SW_RESTORE);
////
////		pDlg->BringWindowToTop();
////	}
////
////	return pDlg;
////}
////
////void CDialogClusterAuth::Close()
////{
////	map<uint32,void *>::iterator it;
////	vector<CDialogClusterAuth *> vecDlg;
////
////	for (it=m_mapClusterAuthId.begin(); it!=m_mapClusterAuthId.end(); it++ )
////	{
////		vecDlg.push_back((CDialogClusterAuth *)it->second);
////	}
////
////	for ( int i=0; i<vecDlg.size(); i++ )
////		vecDlg[i]->DestroyWindow();
////}
////
////void CDialogClusterAuth::OnClose()
////{
////	DestroyWindow();
////}
////
////void CDialogClusterAuth::OnDestroy()
////{
////	CXSkinDlg::OnDestroy();
////
////	delete this;
////}
////
////HBRUSH CDialogClusterAuth::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
////{
////	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);
////	if( nCtlColor==CTLCOLOR_STATIC )
////	{
////		pDC->SetBkMode(TRANSPARENT);
////		hbr = m_hBkGround;
////	}
////
////	return hbr;
////}
////
////void CDialogClusterAuth::OnBnClickedCancel()
////{
////	DestroyWindow();
////}
////
////BOOL CDialogClusterAuth::OnEraseBkgnd(CDC* pDC)
////{
////	BOOL bRt = CXSkinDlg::OnEraseBkgnd(pDC);
////	CRect rcFrame;
////	CWnd *pWnd = GetDlgItem(IDC_STATIC_LINE);
////
////	if ( pWnd )
////	{
////		pWnd->GetWindowRect(&rcFrame);
////		ScreenToClient(&rcFrame);
////
////		pDC->FillSolidRect(&rcFrame,RGB(255,255,255));
////		pDC->Draw3dRect(&rcFrame,RGB(0,122,255),RGB(0,122,255));
////	}
////	return bRt;
////}
////
////void CDialogClusterAuth::InitDisplay()
////{
////	((CStatic*)GetDlgItem(IDC_STATIC_LIMIT))->ShowWindow(SW_SHOW);
////	((CStatic*)GetDlgItem(IDC_STATIC_INFO))->ShowWindow(SW_SHOW);	
////	((CButton *)GetDlgItem(IDC_OK))->ShowWindow(SW_SHOW);	
////	((CButton *)GetDlgItem(IDC_CANCEL))->ShowWindow(SW_SHOW);
////
////	((CEdit*)GetDlgItem(IDC_EDIT_AUTH))->SetWindowText(_T(""));
////	((CButton *)GetDlgItem(IDC_ADD_AGAIN))->ShowWindow(SW_HIDE);
////	((CButton *)GetDlgItem(IDC_CLOSE))->ShowWindow(SW_HIDE);
////}
////
////void CDialogClusterAuth::HideUI()
////{
////	((CStatic*)GetDlgItem(IDC_STATIC_LIMIT))->ShowWindow(SW_HIDE);
////	((CStatic*)GetDlgItem(IDC_STATIC_INFO))->ShowWindow(SW_HIDE);	
////	((CButton *)GetDlgItem(IDC_OK))->ShowWindow(SW_HIDE);	
////	((CButton *)GetDlgItem(IDC_CANCEL))->ShowWindow(SW_HIDE);
////
////	CString strTxt = _T("待群主通过您的身份验证后，");
////	strTxt.AppendFormat(_T("\r\n"));
////	strTxt.AppendFormat(_T("就可以成功入群。"));
////	((CEdit*)GetDlgItem(IDC_EDIT_AUTH))->SetWindowText(strTxt);
////	((CButton *)GetDlgItem(IDC_ADD_AGAIN))->ShowWindow(SW_SHOW);
////	((CButton *)GetDlgItem(IDC_CLOSE))->ShowWindow(SW_SHOW);
////}
////
////void CDialogClusterAuth::OnBnClickedAddAgain()
////{
////	// TODO: 在此添加控件通知处理程序代码
////	InitDisplay();
////	DestroyWindow();
////	/*CFSDialog * pDlg = CFSDialog::Show();
////	pDlg->SetSearchType(FS_CLUSTER);*/
////}
////
////
////void CDialogClusterAuth::OnBnClickedClose()
////{
////	// TODO: 在此添加控件通知处理程序代码
////	InitDisplay();
////	DestroyWindow();
////}
