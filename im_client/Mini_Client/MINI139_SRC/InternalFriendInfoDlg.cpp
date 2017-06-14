// InternalFriendInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "InternalFriendInfoDlg.h"
#include "afxdialogex.h"
#include "im/impublic.h"
#include "RoundFaceCtrl.h"
#include "../../../Common/Common.h"
#include "Im/WebProcess.h"


extern CWebUserHeadIconProcess g_webUserHeadIconProcess;

// CInternalFriendInfoDlg �Ի���

IMPLEMENT_DYNAMIC(CInternalFriendInfoDlg, CDialog)

CInternalFriendInfoDlg::CInternalFriendInfoDlg(FRIEND_INFO *pFriendInfo, CWnd* pParent /*=NULL*/)
	: CDialog(CInternalFriendInfoDlg::IDD, pParent)
	, m_pFriendInfo(pFriendInfo)
{
	m_hBkGround = ::CreateSolidBrush(RGB(255, 255, 255));
	m_pFacePic = new CRountFaceCtrl(IDB_PNG_BIG_BROUND_NORMAL, IDB_PNG_BIG_BROUND_NORMAL);
}

CInternalFriendInfoDlg::~CInternalFriendInfoDlg()
{
	if(g_pProtocol != NULL)
	{
		g_pProtocol->UnRegistCmd(m_hWnd);
	}

	DeleteObject(m_hBkGround);
	if(m_pFacePic != NULL)
	{
		if(::IsWindow(m_pFacePic->GetSafeHwnd()))
		{
			m_pFacePic->DestroyWindow();
		}
		delete m_pFacePic;
		m_pFacePic = NULL;
	}
}

void CInternalFriendInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CInternalFriendInfoDlg, CDialog)
	ON_XT_MESSAGE()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CInternalFriendInfoDlg)
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoAck)
END_XT_MESSAGE_MAP()


// CInternalFriendInfoDlg ��Ϣ�������
BOOL CInternalFriendInfoDlg::PreTranslateMessage(MSG* pMsg)
{
	if  ( pMsg->message  ==  WM_KEYDOWN  )  
	{  
		switch(  pMsg->wParam  )  
		{  
		case VK_ESCAPE:
		case VK_RETURN:
			return TRUE;
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CInternalFriendInfoDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,m_hWnd);

	// ���������ӿؼ������ÿؼ�����
	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
	while(hWndChild)  
	{  
		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
	}

	// ���ţ�ͨ����������
	vector<string> vecGroupName;
	int nGroupIndex = m_pFriendInfo->group_index;
	while(TRUE)
	{
		int nIndex = g_GroupInfo.FindItemIndex(nGroupIndex);
		if( nIndex != -1 )
		{
			vecGroupName.push_back(g_GroupInfo.group[nIndex].name);
			nGroupIndex = g_GroupInfo.group[nIndex].group_father;
		}
		else
		{
			break;
		}
	};
	string strDept;
	for(int i = vecGroupName.size()-1; i >= 0; --i )
	{
		strDept += vecGroupName[i];
		if(i != 0)
		{
			strDept += "/";
		}
	}
	GetDlgItem(IDC_STATIC_DEPART)->SetWindowText(strDept.c_str());

	// ͷ��
	m_pFacePic->Create(this, 50001);
	CRect rcPic;
	m_pFacePic->GetWindowRect(&rcPic);
	ScreenToClient(&rcPic);
	CRect rcPicNew;
	rcPicNew.left = 10;
	rcPicNew.right = rcPicNew.left + rcPic.Width();
	rcPicNew.top = 10;
	rcPicNew.bottom = rcPicNew.top + rcPic.Height();
	m_pFacePic->MoveWindow(&rcPicNew);
	m_pFacePic->ShowWindow(SW_SHOW);
	CString strLocalPath = g_webUserHeadIconProcess.Url2File((LPTSTR)(m_pFriendInfo->face_url));
	m_pFacePic->SetPicPath(strLocalPath);

	// ��ȡ������ϸ��Ϣ
	if(strcmp(m_pFriendInfo->flash_url, "") == 0)
	{
		XT_USERINFO_GET_REQ req;
		req.fid = m_pFriendInfo->GetId();
		g_pProtocol->SendReq(req);
	}
	else
	{
		UpdateUserInfo();
	}

	return TRUE;
}

void CInternalFriendInfoDlg::OnPaint()
{
	CDialog::OnPaint();

	CPaintDC dc(this);	

}

BOOL CInternalFriendInfoDlg::OnEraseBkgnd(CDC* pDC)
{
	CDialog::OnEraseBkgnd(pDC);

	// ���ñ���ɫ
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient,RGB(255, 255, 255));	

	return FALSE;
}

// ���ÿؼ���ɫ
HBRUSH CInternalFriendInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBkGround;
	}

	return hbr;
}

void CInternalFriendInfoDlg::OnUserInfoAck(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_USERINFO_GET_ACK ack = (LPXT_USERINFO_GET_ACK)data;

		if ( ack->GetId()==m_pFriendInfo->GetId() )
		{   
			// ����ͷ��
			CString strLocalPath = g_webUserHeadIconProcess.Url2File((LPTSTR)(LPCTSTR)m_pFriendInfo->face_url);
			m_pFacePic->SetPicPath(strLocalPath);
			CString sCommName=m_pFriendInfo->comment_name;
			memcpy(m_pFriendInfo,(XT_USER_INFO *)ack,sizeof(XT_USER_INFO));
			strncpy(m_pFriendInfo->comment_name,sCommName,sizeof(m_pFriendInfo->comment_name)-1);
			
			UpdateUserInfo();
		}
	}
}

// ���½����û���Ϣ
void CInternalFriendInfoDlg::UpdateUserInfo()
{
	// ������ְ�񣬷ֻ���
	string strJob, strExt;
	SplitExtAndJob(m_pFriendInfo->flash_url, strExt, strJob);
	// ְ��
	GetDlgItem(IDC_STATIC_JOB)->SetWindowText(strJob.c_str());

	// �ֻ���
	GetDlgItem(IDC_STATIC_EXT)->SetWindowText(strExt.c_str());

	// �û���
	GetDlgItem(IDC_STATIC_NAME)->SetWindowText(m_pFriendInfo->nickname);

	// �绰
	GetDlgItem(IDC_STATIC_MOBILE)->SetWindowText(m_pFriendInfo->mobile);
}