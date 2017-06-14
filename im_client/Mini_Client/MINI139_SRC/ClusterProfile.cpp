// ClusterProfile.cpp : 实现文件
//

#include "stdafx.h"
#include "ClusterProfile.h"
#include "afxdialogex.h"
#include "..\IMClient.h"

// CClusterProfile 对话框

IMPLEMENT_DYNAMIC(CClusterProfile, CDialog)

CClusterProfile::CClusterProfile(CWnd* pParent /*=NULL*/)
	: CDialog(CClusterProfile::IDD, pParent)
{
	m_pCluster = NULL;
}

CClusterProfile::CClusterProfile(CLUSTER_INFO *pInfo, CWnd* pParent)
	: CDialog(CClusterProfile::IDD, pParent)
{	
	m_pCluster = pInfo;
}

CClusterProfile::~CClusterProfile()
{
}

void CClusterProfile::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_CLUSER_NICKNAME, m_edtClusterNickname);
}


BEGIN_MESSAGE_MAP(CClusterProfile, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_RADIO2, &CClusterProfile::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO_REC_POP, &CClusterProfile::OnBnClickedRadioRecPop)
	ON_BN_CLICKED(IDC_RADIO3, &CClusterProfile::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CClusterProfile::OnBnClickedRadio4)
	//ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CClusterProfile)
	//ON_XT_COMMAND(CMD_SET_PROFILE_NAME_ACK, OnGetCMProfileAck)
END_XT_MESSAGE_MAP()

BOOL CClusterProfile::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 遍历所有子控件，设置控件字体
	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
	while(hWndChild)  
	{  
		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
	}

	m_edtClusterNickname.SetBorderColor(RGB(200, 200, 200));
	m_edtClusterNickname.LimitText(5);/////设置为10个字符长度
    //SetTimer(0X01,10,NULL);
	switch(m_pCluster->radio)
	{
		case CLUSTER_INFO::RECV_SHOW:
			((CButton *)GetDlgItem(IDC_RADIO_REC_POP))->SetCheck(TRUE);
			break;
		case CLUSTER_INFO::RECV_NOSHOW:
			((CButton *)GetDlgItem(IDC_RADIO2))->SetCheck(TRUE);
			break;
		case CLUSTER_INFO::NORECV:
			((CButton *)GetDlgItem(IDC_RADIO3))->SetCheck(TRUE);
			break;
		case CLUSTER_INFO::SHOW_NUM:
			((CButton *)GetDlgItem(IDC_RADIO4))->SetCheck(TRUE);
			break;
		default:
			((CButton *)GetDlgItem(IDC_RADIO_REC_POP))->SetCheck(TRUE);
			break;
	}	
	//g_pProtocol->RegistCmd(CMD_SET_PROFILE_NAME_ACK, m_hWnd);
	OnUpdateUI(m_pCluster);

	return TRUE;
}

// CClusterProfile 消息处理程序
HBRUSH CClusterProfile::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		UINT nId = pWnd->GetDlgCtrlID();

		if ( nId==IDC_EDIT_USERNAME || nId==IDC_EDIT_NICKNAME || nId==IDC_EDIT_EMAIL )
		{
			
		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr = g_hWhiteBrush;
		}

		if ( nId==IDC_STATIC_TIP || nId==IDC_STATIC_TIP2 )
		{
			pDC->SetTextColor(RGB(74,123,211));
		}

	}

	if ( nCtlColor==CTLCOLOR_DLG  )
	{
		hbr = g_hWhiteBrush;
	}

	return hbr;
}

void CClusterProfile::OnBnClickedRadioRecPop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pCluster->radio = CLUSTER_INFO::RECV_SHOW;
	m_pCluster->bRefused = false;
	m_pCluster->nMsgNum = 0;
}

void CClusterProfile::OnBnClickedRadio2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pCluster->radio = CLUSTER_INFO::RECV_NOSHOW;
	m_pCluster->bRefused = true;
	m_pCluster->nMsgNum = 0;
}

void CClusterProfile::OnBnClickedRadio3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pCluster->radio = CLUSTER_INFO::NORECV;
	m_pCluster->bRefused = true;
	m_pCluster->nMsgNum = 0;
}

void CClusterProfile::OnBnClickedRadio4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pCluster->radio = CLUSTER_INFO::SHOW_NUM;
	m_pCluster->bRefused = true;
	m_pCluster->nMsgNum = 0;
}

void CClusterProfile::OnUpdateUI(CLUSTER_INFO *pCluster)
{
	m_pCluster = pCluster;

	// 获取群名片
	BOOL bFind = FALSE;
	for(int i = 0; i < m_pCluster->Count(); ++i)
	{
		FRIEND_INFO *pFriend = &m_pCluster->members[i];
		if(pFriend->GetId() == g_ImUserInfo.GetId())
		{
			m_edtClusterNickname.SetWindowText(pFriend->profile_name);
			bFind = TRUE;
			break;
		}
	}

	if(!bFind)
	{
		m_edtClusterNickname.SetWindowText("");
		m_edtClusterNickname.SetSel(0,-1);	
	}
    //m_edtClusterNickname.SetWindowText(m_pCluster->szFamilyFaceUrl);//群名片
}

//////修改本人的群名片
//void CClusterProfile::OnGetCMProfileAck(const char *data, WORD wRet)
//{
//
//	LPXT_PROFILENAME_SET_ACK ack = (LPXT_PROFILENAME_SET_ACK)data;
//	if (wRet == 0 && ack->nFamilyID == this->m_pCluster->nFamilyID)
//	{
//			RegUserOperation(0x00b1);
//			int nMemberCnt = m_pCluster->Count();
//			for (int j = 0; j<nMemberCnt; j++)
//			{
//				FRIEND_INFO *pUser = &m_pCluster->members[j];
//				if (pUser->id == ack->nMemID)//////关联为自己的ID
//				{
//					strcpy(pUser->profile_name, ack->ProfileName);/////群名片赋值
//					m_edtClusterNickname.SetWindowText(pUser->profile_name);//群名片
//					break;
//				}
//			}
//	}
//}


//void CClusterProfile::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	CString strTxt = _T("");
//	if ( nIDEvent==0X01 )
//	{
//		m_edtClusterNickname.GetWindowText(strTxt);
//		if ( strTxt=="" )
//		{
//			//m_pHintPass = new CHintDlg2(IDB_LOGIN_PASSHINT,rc.left,rc.top,this);
//			m_edtClusterNickname.SetWindowText("不超过10字");
//			m_edtClusterNickname.SetSel(0,-1);				
//		}			
//	}
//	CDialog::OnTimer(nIDEvent);
//}


BOOL CClusterProfile::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message==WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_RETURN:
			{
			}
			return TRUE;
			break;
		case VK_ESCAPE:
			return TRUE;
			break;
		default:
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

// 获取群片名
string CClusterProfile::GetProfileName()	
{
	CString str;
	m_edtClusterNickname.GetWindowText(str);
	return (LPCTSTR)str;
}