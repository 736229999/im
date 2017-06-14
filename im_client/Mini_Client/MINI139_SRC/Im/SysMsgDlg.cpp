// im\SysMsgDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "SysMsgDlg.h"
#include ".\sysmsgdlg.h"


// CSysMsgDlg 对话框
map<time_t, CSysMsgDlg *> CSysMsgDlg::mapInstance;

IMPLEMENT_DYNAMIC(CSysMsgDlg, CXSkinDlg)
IMPLEMENT_HMG_SCROLLBAR(CSysMsgDlg)

CSysMsgDlg::CSysMsgDlg(CString strMsg,CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CSysMsgDlg::IDD, pParent)
{
	m_strMsg = strMsg;
	Create(IDD,pParent);
}

CSysMsgDlg::~CSysMsgDlg()
{
}

void CSysMsgDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_EDIT_MSG,m_editor);
	DDX_Control(pDX,IDOK,m_btnClose);
}


BEGIN_MESSAGE_MAP(CSysMsgDlg, CXSkinDlg)
	ON_WM_PAINT()
	ON_WM_CLOSE()
	ON_WM_NCDESTROY()
	ON_XT_MESSAGE()
	SET_HMG_SCROLLBAR(IDC_EDIT_MSG)
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CSysMsgDlg)
	ON_XT_COMMAND(CMD_LOGOUT,OnLogOut)
END_XT_MESSAGE_MAP()

void CSysMsgDlg::ShowSysMsg(time_t pub_time, CString strMsg)
{
	map<time_t, CSysMsgDlg *>::iterator it = mapInstance.find(pub_time);

	if ( it==mapInstance.end() )
	{
		CSysMsgDlg *p = new CSysMsgDlg(strMsg,GetDesktopWindow());
		p->ShowWindow(SW_SHOW);
		p->BringWindowToTop();
		p->m_t = pub_time;
		mapInstance.insert(map<time_t,CSysMsgDlg*>::value_type(pub_time,p));
	}
	else
	{
		it->second->BringWindowToTop();
	}
}

BOOL CSysMsgDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetWindowText("系统消息");
	SetDlgStyle(CLOSE);

	AttachHMGScrollbar(m_editor.m_hWnd);

	m_editor.InsertText(m_strMsg);
	XT_FONT xf;
	xf.fontColor = RGB(0,84,166);
	m_editor.SetXFont(xf);

	m_btnClose.SetFocus();

	g_pProtocol->RegistCmd(CMD_LOGOUT,m_hWnd);

	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	return TRUE;  
}

void CSysMsgDlg::OnPaint()
{
	CPaintDC dc(this);
	CXSkinDlg::OnPaint();

	if ( m_editor.m_hWnd )
	{
		CRect rc;
		m_editor.GetWindowRect(&rc);
		ScreenToClient(&rc);
		rc.InflateRect(1,1);
		rc.right++;
		dc.Draw3dRect(&rc,IM_LINE_COLOR,IM_LINE_COLOR);
	}
}

void CSysMsgDlg::OnClose()
{
	DestroyWindow();
}

void CSysMsgDlg::OnNcDestroy()
{
	CXSkinDlg::OnNcDestroy();

	map<time_t,CSysMsgDlg *>::iterator it = mapInstance.find(m_t);

	if ( it!=mapInstance.end() )
		mapInstance.erase(it);

	delete this;
}

void CSysMsgDlg::OnLogOut(const char *data, WORD wRet)
{
	g_pProtocol->UnRegistCmd(m_hWnd);
	DestroyWindow();
}
