#include "stdafx.h"
#include "Resource.h"
#include "FriendMsgManageDlg.h"
#include "./im/impublic.h"

using namespace std;

BOOL operator==(STRU_GROUP_FRIEND &gf, int groupIndex)
{
	return groupIndex == gf.pGroup->group_index;
}

IMPLEMENT_DYNAMIC(CMsgManage_Friend_Dlg, CDialog)

CMsgManage_Friend_Dlg::CMsgManage_Friend_Dlg(CWnd* pParent)   // 标准构造函数
	: CDialog(IDD_DLG_MSGMANAGE_FRIEND, pParent)
{
	m_font.CreateFont(
		17,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("微软雅黑"));

	// tab设置
	m_colFriendGroup.SetDirectMode(CColumnCtrl::EN_Y_DIRE);
	m_colFriendGroup.SetFont(&m_font);
	m_colFriendGroup.SetBackgroundColor(RGB(219,221,222), RGB(240,240,241), RGB(206, 211, 214));
	m_colFriendGroup.SetItemTextColor(RGB(0,0,0), RGB(0,0,0), RGB(0,0,0));
	m_colFriendGroup.SetFrameModol(FRAME_DRAW_NULL);
	m_colFriendGroup.SetItemHeight(27);

	m_colFriend.SetDirectMode(CColumnCtrl::EN_Y_DIRE);
	m_colFriend.SetFont(&m_font);
	m_colFriend.SetBackgroundColor( RGB(240,240,241), RGB(255,255,255), RGB(225, 230, 235));
	m_colFriend.SetItemTextColor(RGB(0,0,0), RGB(0,0,0), RGB(0,0,0));
	m_colFriend.SetFrameModol(FRAME_DRAW_NULL);
	m_colFriend.SetItemHeight(27);

	InitGF();
}

CMsgManage_Friend_Dlg::~CMsgManage_Friend_Dlg()
{

}

void CMsgManage_Friend_Dlg::DoDataExchange(CDataExchange* pDX)    // DDX/DDV 支持
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FRIENDGROUP, m_colFriendGroup);
	DDX_Control(pDX, IDC_STATIC_FRIEND, m_colFriend);
	DDX_Control(pDX, IDC_RE_HISTORY, m_edtReHistory);
}

BEGIN_MESSAGE_MAP(CMsgManage_Friend_Dlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_COLUMNCTRL_SEL_CHANGE, OnColumnctrlSelChange)
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CMsgManage_Friend_Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	for ( int i=0; i<m_vecGF.size(); i++ )
	{
		STRU_GROUP_FRIEND &gf = m_vecGF[i];
		m_colFriendGroup.AddItem(gf.pGroup->name, "", i == m_vecGF.size()-1);
	}
	m_colFriendGroup.SetCurSel(0);

	return TRUE;
}

// 菜单栏选着改变事件
LRESULT CMsgManage_Friend_Dlg::OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam)
{
	CColumnCtrl *pCtrl = (CColumnCtrl*)wParam;
	if(pCtrl == &m_colFriendGroup)
	{
		int nCurSel = HIWORD(lParam);
		if(nCurSel >= 0 && nCurSel < m_vecGF.size())
		{
			m_colFriend.RemoveAllItems();
			STRU_GROUP_FRIEND &gf = m_vecGF[nCurSel];
			for(int i = 0; i < gf.vecpFriend.size(); ++i)
			{
				FRIEND_INFO* &fi = gf.vecpFriend[i];
				m_colFriend.AddItem(fi->nickname, "", i == gf.vecpFriend.size()-1);
			}
			m_colFriend.SetCurSel(0);
		}
	}

	if(pCtrl == &m_colFriend)
	{
		int nCurSelGroup = m_colFriendGroup.GetCurSel();
		int nCurSelFriend = HIWORD(lParam);
		if(nCurSelGroup >= 0 && nCurSelGroup < m_vecGF.size()
			&& nCurSelFriend >= 0 && nCurSelFriend < m_vecGF[nCurSelGroup].vecpFriend.size())
		{
			FRIEND_INFO* &pFriend = m_vecGF[nCurSelGroup].vecpFriend[nCurSelFriend];
			m_mlRecord.RemoveAll();
			g_LocalData.GetMsgList(pFriend->GetId(),CDataManager::USER,m_mlRecord);

			//// 更新查询范围复选框的选项
			//FINDRANGE_ITEMDATA rit;
			//rit.type=EM_IT_FRIEND;
			//rit.id=pFriend->id;
			//SetRangeSel(rit);
			//if(!m_bFlag)
			//{
			//	m_bFlag = TRUE;
			//	InitMsg();
			//}
		}
		
	}

	return 0;
}

HBRUSH CMsgManage_Friend_Dlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_DLG  )
	{
		hbr = g_hWhiteBrush;
	}

	return hbr;
}

// 初始化组--成员信息
void CMsgManage_Friend_Dlg::InitGF()
{
	for(int i=0; i<g_GroupInfo.count; i++)
	{
		STRU_GROUP_FRIEND gf;
		gf.pGroup = g_GroupInfo.group+i;
		m_vecGF.push_back(gf);
	}

	FRIEND_LIST::iterator it = g_lsFriend.begin();
	for(; it != g_lsFriend.end(); ++it)
	{
		FRIEND_INFO *fi = *it;
		vector<STRU_GROUP_FRIEND>::iterator find_it = find(m_vecGF.begin(), m_vecGF.end(), fi->group_index);
		if(find_it != m_vecGF.end())
		{
			find_it->vecpFriend.push_back(fi);
		}
	}
}

void CMsgManage_Friend_Dlg::OnPaint()
{
	//CDialog::OnPaint();
	m_colFriendGroup.Invalidate(TRUE);
	m_colFriend.Invalidate(TRUE);

	CPaintDC dc(this);

	// 对话框底部的按钮栏
	CRect rcBar;
	GetDlgItem(IDC_STATIC_BTN_BAR)->GetWindowRect(&rcBar);
	ScreenToClient(&rcBar);
	rcBar.left -= 1;
	rcBar.right += 1;
	dc.FillSolidRect(&rcBar, RGB(240, 240, 241));
}