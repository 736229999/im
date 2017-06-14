// FriendInfoArchDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "FriendInfoArchDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "XmlAddress.h"
#include "Im\imfriendlistctrl.h"
#include "../../../Common/Common.h"
#include "MainFrame.h"


extern CString g_cCareers[];

// CFriendInfoArchDlg 对话框

IMPLEMENT_DYNAMIC(CFriendInfoArchDlg, CDialog)

CFriendInfoArchDlg::CFriendInfoArchDlg(FRIEND_INFO *pFriendInfo, CWnd* pParent)
	: CDialog(IDD_DLG_FRINDINFO_ARCH, pParent)
{
	m_pFriendInfo = pFriendInfo;
	m_hBkGround = ::CreateSolidBrush(RGB(235, 235, 235));
}

CFriendInfoArchDlg::~CFriendInfoArchDlg()
{
	DeleteObject(m_hBkGround);
}

void CFriendInfoArchDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_REMARK, m_editRemark);
	DDX_Control(pDX, IDC_COMBO_GROUP, m_comboGroup);
}


BEGIN_MESSAGE_MAP(CFriendInfoArchDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CFriendInfoArchDlg 消息处理程序
BOOL CFriendInfoArchDlg::Create()
{
	BOOL bRlt = CDialog::Create(IDD_DLG_FRINDINFO_ARCH, m_pParentWnd);
	ShowWindow(SW_SHOW);

	return bRlt;
}

BOOL CFriendInfoArchDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 遍历所有子控件，设置控件字体
	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
	while(hWndChild)  
	{  
		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
	}

	m_editRemark.SetBorderColor(RGB(200, 200, 200));
	m_comboGroup.SetBorderColor(RGB(200, 200, 200));

	m_editRemark.LimitText(8);

	if(CMainFrame::IsBuyer(m_pFriendInfo->GetId()))//用户ID大于5000000表示买家用户，不显示行业
	{
		GetDlgItem(IDC_STATIC_CAREER)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_STATIC_INDUSTRY_VALUE)->ShowWindow(SW_HIDE);
	}
	//如果好友不在好友列表中则无法更改备注名
	BOOL bFriend = FALSE;
	FRIEND_LIST::iterator it;
	for ( it=g_lsFriend.begin();it!=g_lsFriend.end(); it++ )
	{
		if ( (*it) == m_pFriendInfo )
		{
			bFriend = TRUE;
			break;
		}
	}
	if(!bFriend)
	{
		m_editRemark.EnableWindow(FALSE);
		m_comboGroup.EnableWindow(FALSE);
	}
	return TRUE;
}

void CFriendInfoArchDlg::OnPaint()
{
	CDialog::OnPaint();

	CPaintDC dc(this);	

}

BOOL CFriendInfoArchDlg::OnEraseBkgnd(CDC* pDC)
{
	CDialog::OnEraseBkgnd(pDC);

	// 设置背景色
	CRect rcClient;
	GetClientRect(&rcClient);
	pDC->FillSolidRect(rcClient,RGB(235, 235, 235));	

	return FALSE;
}

// 设置控件颜色
HBRUSH CFriendInfoArchDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBkGround;
	}

	return hbr;
}

string GetAddress(int nPid, int nCid, int nDid)
{
	// 合成地区
	map<int, string> mapP;
	GetAllProvince(mapP);
	map<int, string> mapC;
	map<int, string> mapD;
	string strAddress;
	int nP = 1;
	int nC = 1;
	if(nPid > 0)
	{
		strAddress = mapP[nPid];
		nP = nPid;
	}
	else
	{
		strAddress = mapP[nP];
	}
	strAddress += "  ";
	GetAllCity(nP, mapC);
	if(nCid > 0)
	{
		strAddress += mapC[nCid];
		nC = nCid;
	}
	else
	{
		strAddress += mapC[nC];
	}
	strAddress += "  ";
	GetAllDistrict(nP, nC, mapD);
	if(nDid > 0)
	{
		strAddress += mapD[nDid];
	}
	else
	{
		strAddress += mapD[1];
	}

	return strAddress;
}

// 更新用户信息
void CFriendInfoArchDlg::Refresh(FRIEND_INFO *pFriendInfo)
{
	m_pFriendInfo = pFriendInfo;
	SetDlgItemText(IDC_STATIC_NAME_VALUE, m_pFriendInfo->nickname);
	SetDlgItemText(IDC_STATIC_SEX_VALUE, SexToStr(m_pFriendInfo->sex));

	// 备注名
	m_editRemark.SetWindowText(m_pFriendInfo->comment_name);

	// 生日
	if(strcmp(m_pFriendInfo->birthday, "") != 0)
	{
		int nY, nM, nD;
		sscanf(m_pFriendInfo->birthday, "%4d%2d%2d", &nY, &nM, &nD); 
		CString strBirthday;
		strBirthday.Format("%d年%d月%d日", nY, nM, nD);
		SetDlgItemText(IDC_STATIC_BIRTHDAY, strBirthday);
	}
	
	// 电话
	//SetDlgItemText(IDC_STATIC_TEL_VALUE, g_ImUserInfo.);
	// 行业
	CString strCareer = "";
	if(m_pFriendInfo->career > 0 && m_pFriendInfo->career <= 10)
	{
		strCareer = g_cCareers[m_pFriendInfo->career-1];
	}
	SetDlgItemText(IDC_STATIC_INDUSTRY_VALUE, strCareer);

	//分组信息
	m_comboGroup.ResetContent();
	//飞牛网组织架构中的不允许移动
	if(m_pFriendInfo->group_index < FEINIU_GROUP_ID)
	{
		int nSel = 0;
		for(int i=0; i<g_GroupInfo.count; i++)
		{
			if(g_GroupInfo.group[i].group_index < FEINIU_GROUP_ID)
			{
				int nIndex = m_comboGroup.AddString(g_GroupInfo.group[i].name);
				m_comboGroup.SetItemData(nIndex, i);
				if(g_GroupInfo.group[i].group_index == m_pFriendInfo->group_index)
				{
					nSel = nIndex;
				}
			}			
		}
		m_comboGroup.SetCurSel(nSel);
	}
	else
	{	// 限定我的团队中的成员不能选择分组
		int nGroupItemIndex = g_GroupInfo.FindItemIndex(m_pFriendInfo->group_index);
		ASSERT(nGroupItemIndex != -1);
		if(nGroupItemIndex != -1)
		{
			m_comboGroup.AddString(g_GroupInfo.group[nGroupItemIndex].name);
			m_comboGroup.SetItemData(0, nGroupItemIndex);
			m_comboGroup.SetCurSel(0);
		}
	}

	// 私有
	if(m_pFriendInfo->linkpop == 0)
	{
		// 非陌生人或黑名单
		SetDlgItemText(IDC_STATIC_LOCAL_VALUE,"保密");
		SetDlgItemText(IDC_STATIC_MOBILE_VALUE,"保密");
		SetDlgItemText(IDC_STATIC_EMAIL_VALUE,"保密");
	}
	// 好友可见
	else if(m_pFriendInfo->linkpop == 1)
	{
		if(m_pFriendInfo->group_index != 1 && m_pFriendInfo->group_index != 2)
		{
			// 非陌生人或黑名单
			string strAddress = GetAddress(m_pFriendInfo->province, m_pFriendInfo->city, m_pFriendInfo->country);
			SetDlgItemText(IDC_STATIC_LOCAL_VALUE,strAddress.c_str());
			SetDlgItemText(IDC_STATIC_MOBILE_VALUE,m_pFriendInfo->mobile);
			SetDlgItemText(IDC_STATIC_EMAIL_VALUE,m_pFriendInfo->email);
			//电话(特殊)
			#ifdef _VER_INTERNAL_
			string strJob, strExt;
			SplitExtAndJob(m_pFriendInfo->flash_url, strExt, strJob);
			SetDlgItemText(IDC_STATIC_TEL_VALUE,strExt.c_str());
			#endif
		}
		else
		{
			SetDlgItemText(IDC_STATIC_LOCAL_VALUE,"保密");
			SetDlgItemText(IDC_STATIC_MOBILE_VALUE,"保密");
			SetDlgItemText(IDC_STATIC_EMAIL_VALUE,"保密");
		}
	}
	// 公开
	else
	{
		string strAddress = GetAddress(m_pFriendInfo->province, m_pFriendInfo->city, m_pFriendInfo->country);
		SetDlgItemText(IDC_STATIC_LOCAL_VALUE,strAddress.c_str());
		SetDlgItemText(IDC_STATIC_MOBILE_VALUE,m_pFriendInfo->mobile);
		SetDlgItemText(IDC_STATIC_EMAIL_VALUE,m_pFriendInfo->email);
	}
}

string CFriendInfoArchDlg::GetRemarkName()
{
	CString strRemarkName;
	m_editRemark.GetWindowText(strRemarkName);
	strRemarkName.Trim();
	return (LPCTSTR)strRemarkName;
}

int CFriendInfoArchDlg::GetGroupIndex()
{
	int nIndex = m_comboGroup.GetCurSel();
	return m_comboGroup.GetItemData(nIndex);
}
