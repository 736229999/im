// EvalueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EditFastWordDlg.h"
#include "Resource.h"

#include <map>
using namespace std;
// CEvalueDlg dialog

using namespace DuiLib;

extern  void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
CEditFastWordDlg::CEditFastWordDlg(CString strText, CWnd* pParent /*=NULL*/)
{
	m_AutoReply.strText = strText;
}

void CEditFastWordDlg::InitWindow()
{
	UpdateBk();
	WindowImplBase::InitWindow();
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));

	CRect rc;
	if(m_reTextOnline.GetSafeHwnd() == NULL)
		m_reTextOnline.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | ES_WANTRETURN
		| WS_VSCROLL | ES_AUTOVSCROLL, rc, CWnd::FromHandle(GetHWND()), 10002);
	m_reTextOnline.SetLimitWordSize(400);//发送框限制输入400个字
	SetIcon(IDR_MAINFRAME);
	m_pLabelTitle =  static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("LabelTitle")));
}

void CEditFastWordDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);

	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender->GetName() == "btnOK")
		{
			OnOK();
		}
		else if(msg.pSender->GetName() == "btnCancel")
		{
			Close(IDCANCEL);
		}
	}
}

// 获取文本
CString CEditFastWordDlg::GetText()
{
	return m_AutoReply.strText;
}

// 设置标题
void CEditFastWordDlg::SetTitle(const char *pTitle)
{
	m_strTitle = pTitle;
	m_pLabelTitle->SetText(m_strTitle);
}

void CEditFastWordDlg::OnOK()
{
	m_reTextOnline.GetText(m_AutoReply.strText);
	Close(1);
}

void CEditFastWordDlg::UpdateBk()
{
	if(::IsWindow(GetHWND()) && m_PaintManager.GetRoot() != NULL)
	{
		DuiLib::CControlUI* background = m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			string strBkImage = g_LocalData.GetBkImage(g_ImUserInfo.GetUserName().c_str());
			BOOL bSolid = strBkImage.find('c') != string::npos;

			if(bSolid)
			{
				RECT rc;
				GetWindowRect(GetHWND(), &rc);

				TCHAR szBuf[MAX_PATH] = {0};
				_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,18,18'"), strBkImage.c_str());
				background->SetBkImage(szBuf);
			}
			else
			{
				RECT rc;
				GetWindowRect(GetHWND(), &rc);
				int nWidth = rc.right - rc.left;
				TCHAR szBuf[MAX_PATH] = {0};
				if(nWidth <= 600)
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,%d,28' dest='0,0,%d,28'"), strBkImage.c_str(), rc.right-rc.left, rc.right-rc.left);
				}
				else
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,600,28' dest='0,0,%d,28' corner='580,0,0,0'"), strBkImage.c_str(), nWidth);
				}
				background->SetBkImage(szBuf);
			}
		}
	}
}

////IMPLEMENT_DYNAMIC(CEditFastWordDlg, CXSkinDlg)
////
////CEditFastWordDlg::CEditFastWordDlg(CString strText, CWnd* pParent /*=NULL*/)
////	: CXSkinDlg(IDD_DLG_FAST_WORD, pParent)
////{
////	//////添加颜色设置///////////////
////	m_colorBackground = RGB(255, 255, 255);
////	m_colorBorder = RGB(255, 255, 255)/*CHAT_BKG_COLOR*/;
////	DeleteObject(m_hBkGround);
////	m_hBkGround = ::CreateSolidBrush(m_colorBackground);
////
////	SetDlgStyle(CXSkinDlg::CLOSE);
////
////	// 初始化
////	m_strText = strText;
////
////	m_strTitle = "新增快捷短语";
////}
////
////CEditFastWordDlg::~CEditFastWordDlg()
////{
////}
////
////void CEditFastWordDlg::DoDataExchange(CDataExchange* pDX)
////{
////	CXSkinDlg::DoDataExchange(pDX);
////	DDX_Control(pDX, IDC_RICHEDIT_TEXT, m_reText);
////	DDX_Control(pDX, IDOK, m_btnOK);
////	DDX_Control(pDX, IDCANCEL, m_btnCancel);
////}
////
////
////BEGIN_MESSAGE_MAP(CEditFastWordDlg, CXSkinDlg)
////	ON_WM_PAINT()
////END_MESSAGE_MAP()
////
////
////// CEvalueDlg message handlers
////BOOL CEditFastWordDlg::OnInitDialog()
////{
////	CXSkinDlg::OnInitDialog();
////
////	SetWindowText(m_strTitle);
////
////	m_reText.SetText(m_strText);
////	m_reText.SetXFont(g_ImFont);
////	m_reText.SetFocus();
////
////	return TRUE;
////}
////
////void CEditFastWordDlg::OnPaint()
////{
////	CPaintDC dc(this);
////	CXSkinDlg::DrawCaptionBar();
////	CXSkinDlg::DrawBorder();
////
////	CRect rc;
////	GetClientRect(&rc);
////
////	// 对话框底部的按钮栏
////	CRect rcBtn;
////	m_btnOK.GetWindowRect(&rcBtn);
////	ScreenToClient(&rcBtn);
////	rcBtn.left = 1;
////	rcBtn.right = rc.right - 1;
////	rcBtn.top = rcBtn.top - rcBtn.Height() / 3;
////	rcBtn.bottom = rc.bottom - 1;
////	dc.FillSolidRect(&rcBtn, RGB(222, 222, 222));
////}
////
////// 获取文本
////CString CEditFastWordDlg::GetText()
////{
////	return m_strText;
////}
////
////void CEditFastWordDlg::OnOK()
////{
////	m_reText.GetText(m_strText, NULL);
////	CXSkinDlg::OnOK();
////}
////
////// 设置标题
////void CEditFastWordDlg::SetTitle(const char *pTitle)
////{
////	m_strTitle = pTitle;
////	if(::IsWindow(GetSafeHwnd()))
////	{
////		SetWindowText(m_strTitle);
////	}
////}