

#include "stdafx.h"
#include "SelectGroupDlg.h"
#include "impublic.h"

// CSelectGroupDlg 对话框

IMPLEMENT_DYNAMIC(CSelectGroupDlg, CXSkinDlg)

CSelectGroupDlg::CSelectGroupDlg(uint32 nFriendID,const CString & sFriendName, CWnd *pParent)
	: CXSkinDlg(CSelectGroupDlg::IDD, pParent)
{

	m_nGroupIndex = 0;
	m_nFriendID = nFriendID;
	m_sFriendName = sFriendName;
	
	Create(CSelectGroupDlg::IDD,pParent);
}

CSelectGroupDlg::~CSelectGroupDlg()
{
}

void CSelectGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GROUP, m_cmbGroup);
	DDX_Control(pDX, IDC_MSG, m_lblMsg);
	DDX_Control(pDX, IDC_STATIC_TIP, m_lblTip);
	DDX_Control(pDX, IDOK, m_btnOK);
}


BEGIN_MESSAGE_MAP(CSelectGroupDlg, CXSkinDlg)
	ON_CBN_SELCHANGE(IDC_GROUP, OnCbnSelchangeGroup)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_WM_NCDESTROY()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CSelectGroupDlg 消息处理程序

BOOL CSelectGroupDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();
	SetDlgStyle(MINCLOSE);
	int nC=g_GroupInfo.count;
	int i;
	int nIdx=0;
	for(i=0;i<nC;i++)
	{
		if ( g_GroupInfo.group[i].group_index!=1 &&
			g_GroupInfo.group[i].group_index!=2 &&	//过滤掉陌生人和黑名单，xuexiaofeng
			g_GroupInfo.group[i].group_index < MY_TEAM_GROUP_ID)	// 过滤掉我的团队分组
		{
			nIdx=m_cmbGroup.InsertString(-1,g_GroupInfo.group[i].name);
			m_cmbGroup.SetItemData(nIdx,g_GroupInfo.group[i].group_index);
		}
	}
	if(nC>0)
		m_cmbGroup.SetCurSel(0);
	CString sMsg;
	sMsg.Format("为\"%s\"选择一个组，谢谢。",m_sFriendName);
	m_lblMsg.SetWindowText(sMsg,"");

	SetWindowText("好友分组");

	return TRUE;  
}

void CSelectGroupDlg::NewDlg(uint32 nFriendID,const CString &sFriendName, HWND hwndIMPage)
{
	CSelectGroupDlg *pGroup = new CSelectGroupDlg(nFriendID,sFriendName,GetDesktopWindow()); 
	pGroup->m_hwndIMPage = hwndIMPage;

	pGroup->CenterWindow(GetDesktopWindow());
	pGroup->ShowWindow(SW_MINIMIZE);
	pGroup->ShowWindow(SW_SHOW);
	pGroup->ShowWindow(SW_RESTORE);
}

void CSelectGroupDlg::OnCbnSelchangeGroup()
{
	int nIdx;
	nIdx=m_cmbGroup.GetCurSel();
	if(nIdx<0)
	{
		m_nGroupIndex=0;
		return;
	}
	
	m_nGroupIndex=(int)m_cmbGroup.GetItemData(nIdx);
}




void CSelectGroupDlg::OnBnClickedOk()
{
	// 如果选择的是我的好友，则默认不用改变组
	if(m_nGroupIndex != 0)
		::SendMessage(m_hwndIMPage,WM_CHANGE_FRIENDGROUP,(WPARAM)m_nFriendID,(LPARAM)m_nGroupIndex);
	DestroyWindow();
}

void CSelectGroupDlg::OnNcDestroy()
{
	CXSkinDlg::OnNcDestroy();

	delete this;
}



BOOL CSelectGroupDlg::OnEraseBkgnd(CDC* pDC)
{
	BOOL bRet = CXSkinDlg::OnEraseBkgnd(pDC);
	
	CRect rcLine;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_LINE);

	if ( pWnd && pWnd->m_hWnd )
	{
		pWnd->GetWindowRect(&rcLine);
		ScreenToClient(&rcLine);

		CBrush brush;
		brush.Attach(::g_hWhiteBrush);
		CPen   pen(PS_SOLID,1,IM_LINE_COLOR);

		CBrush *pOldbrush = pDC->SelectObject(&brush);
		CPen   *pOldpen   = pDC->SelectObject(&pen);

		pDC->Rectangle(&rcLine);
		brush.Detach();

		pDC->SelectObject(pOldbrush);
		pDC->SelectObject(pOldpen);
	}

	return bRet;
}
