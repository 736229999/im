// ClusterMember.cpp : 实现文件
//

#include "stdafx.h"
#include "ClusterMember.h"
#include "afxdialogex.h"
#include "..\IMClient.h"
#include "..\Im\p2pinvitedlg.h"
#include "..\Im\impublic.h"
#include "MainFrame.h"
// CClusterMember 对话框

#define  TIMER_UPDATE_CLUSTER 1
IMPLEMENT_DYNAMIC(CClusterMember, CDialog)
IMPLEMENT_HMG_SCROLLBAR(CClusterMember)
enum PIC_INDEX
{
	PIC_FRIEND_OFF = 0,
	PIC_FRIEND     = 0,
	PIC_ADMIN      = 1,
	PIC_CREATOR    = 1,
};
CClusterMember::CClusterMember(CWnd* pParent /*=NULL*/)
	: CDialog(CClusterMember::IDD, pParent)
{
	m_row    = 0;
	m_column = 0;
	m_bChooseFlag = FALSE;
	m_bChooseMemberFlag = FALSE;
	m_pProgress		 = NULL;

	m_btnSetAdmin.SetTextColor(RGB(77, 149, 194));
	m_btnCancelAdmin.SetTextColor(RGB(77, 149, 194));
	m_btnDelMember.SetTextColor(RGB(77, 149, 194));
	m_btnModifyInfo.SetTextColor(RGB(77, 149, 194));
	m_btnAddMember.SetTextColor(RGB(77, 149, 194));
}

CClusterMember::~CClusterMember()
{
	CloseProgress();
}

void CClusterMember::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LISTCTL, m_edit);
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_BTN_SET_ADMIN, m_btnSetAdmin);
	DDX_Control(pDX, IDC_BTN_CANCEL_ADMIN, m_btnCancelAdmin);
	DDX_Control(pDX, IDC_BTN_DEL_MEMBER, m_btnDelMember);
	DDX_Control(pDX, IDC_BTN_MODIFY_INFO, m_btnModifyInfo);
	DDX_Control(pDX, IDC_BTN_ADD_MEMBER, m_btnAddMember);
}


BEGIN_MESSAGE_MAP(CClusterMember, CDialog)
	ON_EN_KILLFOCUS(IDC_EDIT_LISTCTL, &CClusterMember::OnEnKillfocusEditListctl)
    ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_SET_ADMIN, &CClusterMember::OnBnClickedBtnSetAdmin)
	ON_BN_CLICKED(IDC_BTN_CANCEL_ADMIN, &CClusterMember::OnBnClickedBtnCancelAdmin)
	ON_BN_CLICKED(IDC_BTN_DEL_MEMBER, &CClusterMember::OnBnClickedBtnDelMember)
	ON_BN_CLICKED(IDC_BTN_MODIFY_INFO, &CClusterMember::OnBnClickedBtnModifyInfo)
	ON_BN_CLICKED(IDC_BTN_ADD_MEMBER, &CClusterMember::OnBnClickedBtnAddMember)
	ON_WM_TIMER()
	ON_NOTIFY(NM_CLICK, IDC_LIST1, &CClusterMember::OnNMClickList1)
	SET_HMG_SCROLLBAR(IDC_LIST1)
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CClusterMember)
	/*ON_XT_COMMAND(CMD_GET_CINFO_ACK,OnGetClusterInfoAck)
	ON_XT_COMMAND(CMD_SET_CINFO_ACK,OnSetClusterInfoAck)
	ON_XT_COMMAND(CMD_GET_CMEMBERS_ACK,OnClusterMembersAck)
	ON_XT_COMMAND(CMD_CMEMBERS_IND,OnCMemberInd)*/
	ON_XT_COMMAND(CMD_SET_CMEMBERS_ACK,OnSetCMemberAck)
	//ON_XT_COMMAND(CMD_LOGOUT,OnLoginOut)
END_XT_MESSAGE_MAP()

// CClusterMember 消息处理程序
BOOL CClusterMember::OnInitDialog()
{
	CDialog::OnInitDialog();
	AttachHMGScrollbar(m_list.m_hWnd);

	// 遍历所有子控件，设置控件字体
	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
	while(hWndChild)  
	{  
		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
	}
	m_btnSetAdmin.SetFont(&g_font12);
	m_list.SetFont(&g_font12);

	////CreateListInfo(&m_clusterInfo);
	m_edit.LimitText(10);////群名片长度限制为10
	m_edit.SetBorderColor(RGB(255, 255, 255));
	m_list.SetBkColor(0XFFFFFF);
	g_pProtocol->RegistCmd(CMD_SET_CMEMBERS_ACK,m_hWnd);
	return TRUE;
}

void CClusterMember::CreateListInfo(CLUSTER_INFO  *pClusterInfo)
{
	m_clusterInfo = *pClusterInfo;
	LONG lStyle;
	CString strTxt = _T("");
	lStyle = GetWindowLong(m_list.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_list.m_hWnd, GWL_STYLE, lStyle);//设置style
	DWORD dwStyle = m_list.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT;//选中某行使整行高亮（只适用与report风格的listctrl）
	dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT/* | LVS_EX_SUBITEMIMAGES*/); //特别注意最后一个属性。); //设置扩展风格
	
	//m_imagelist.Create(IDB_CLUSTER_ADMIN, 16, 9, RGB(255,0,255));/////测试图标
	//m_imagelist.Create(IDB_CLUSTER_CREATOR, 16, 9, RGB(255,0,255));/////测试图标
	//m_imagelist.Remove(0);
	//m_imagelist.Remove(0);
	//m_list.SetImageList(&m_imagelist, LVSIL_SMALL);
	
	///////////添加图片////////////////////////////
	m_list.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);
	m_list.SetImageList(&g_imgStatus30,LVSIL_SMALL);
	
	////创建表头
	int admin_num = 0;
	for(int num = 0; num < pClusterInfo->Count(); num ++)
	{
		FRIEND_INFO *pFriend = &(pClusterInfo->members[num]);
		if(pFriend->cluster_property == 1)
			admin_num ++;
	}

	CString admin_str;
	admin_str.Format("管理员:%d/10人",admin_num);
	
	m_list.InsertColumn(0,_T(admin_str),LVCFMT_LEFT,100);
	m_list.InsertColumn(1,_T("账号"),LVCFMT_LEFT,90);
	m_list.InsertColumn(2,_T("群名片"),LVCFMT_LEFT,90);
	m_list.InsertColumn(3,_T("等级"),LVCFMT_LEFT,70);
	m_list.InsertColumn(4,_T("最后发言"),LVCFMT_LEFT,110);

	::SendMessage(m_list.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);
///////////////////////////////////////////////////////////////////////////////////
	CString str;
	str.Format("群 %s",pClusterInfo->szFamilyName);
	SetWindowText(str);
	   
//////////////////////////////////////////////////////////////////////////////////
	for ( int i=0; i<pClusterInfo->Count(); i++ )
	{
		////插入行
		m_list.InsertItem(i,_T(""));	

		FRIEND_INFO *pFriend = &pClusterInfo->members[i];	
		int nType =  pFriend->cluster_property;
	
		if  (nType == 2)
		{
			//m_list.SetItemText(i,0,_T("群主"));
			//m_list.SetItem(i,0,LVIF_TEXT|LVIF_IMAGE,_T(""),0, 0, 0,0);
			//m_list.SetItem(i,0,LVIF_TEXT|LVIF_IMAGE,_T("群主"),1, 0, 0,0);
			m_list.SetItemImage(i,0,PIC_CREATOR);
		}
		if  (nType == 1)
		{
			//m_list.SetItemText(i,0,_T("管理员"));
			//m_list.SetItem(i,0,LVIF_TEXT|LVIF_IMAGE,_T(""),0, 0, 0,0);
			//m_list.SetItem(i,0,LVIF_TEXT|LVIF_IMAGE,_T("管理员"),1, 0, 0,0);
			m_list.SetItemImage(i,0,PIC_ADMIN);
		}
			str = (CountInfoToStr(pFriend));/////本群的群成员账号
			m_list.SetItemText(i,1,str);	
		/////从群ID查找群的成员信息;
		m_list.SetItemText(i, 2, pFriend->profile_name);//////群名片
	   // m_list.SetItemText(i,3,_T("21"));
	   // m_list.SetItemText(i,4,_T("2014/7/24"));
	    m_edit.ShowWindow(SW_HIDE);

	}
}

void CClusterMember::ModifyProfileName()
{
	CRect rc;
	m_list.GetSubItemRect(m_row, m_column,LVIR_LABEL,rc);//取得子项的矩形
	rc.left+=8;
	rc.top+=10;
	rc.right+=8;
	rc.bottom+=9;
	m_edit.MoveWindow(&rc);//将编辑框移动到子项上面，覆盖在子项上
	char * ch=new char [128];
	m_list.GetItemText(m_row, m_column,(LPTSTR)ch,127);//取得子项的内容
	m_edit.SetWindowText((LPCTSTR)(LPTSTR)ch);//将子项的内容显示到编辑框中
	m_edit.ShowWindow(SW_SHOW);//显示编辑框
		
	m_edit.SetFocus();//使编辑框取得焦点
	m_edit.SetSel(0,-1);//使光标移到最后面			
	delete[] ch;
}

void CClusterMember::OnEnKillfocusEditListctl()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strTxt = _T("");
	m_edit.GetWindowText(strTxt);//取得编辑框的内容
	m_list.SetItemText(m_row,m_column,strTxt);//将该内容更新到CListCtrl中
	m_edit.ShowWindow(SW_HIDE);//隐藏编辑框
	FRIEND_INFO *pFriend = &(m_clusterInfo.members[m_row]);	////当前选中行的详细信息
	if(strcmp(pFriend->profile_name,strTxt) == 0)
		return;
	int strLen = strTxt.GetLength();
	if(strLen > 10)
	{
		CMessageBox::Model(GetSafeHwnd(),"群名片长度最大只能为10");
		return;
	}
	//if (strLen > 0)
	{
		memcpy(pFriend->profile_name, (void*)strTxt.GetBuffer(strLen), strLen);
		pFriend->profile_name[strLen] = '\0';
	}
	//else
	//{
	//	pFriend->profile_name[0] = '\0';
	//}
	///////更新本群中成员名片2015-03-03///////////////////////////////////////////////
	{
		//修改成员群名片
		XT_PROFILENAME_SET_REQ req;
		req.nFamilyID   = m_clusterInfo.nFamilyID;/////群ID
		req.nMemID = pFriend->GetId();         ////好友ID
		strcpy(req.ProfileName, pFriend->profile_name);/////群名片
		g_pProtocol->SendReq(req);
	}
}

HBRUSH CClusterMember::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CClusterMember::OnBnClickedBtnSetAdmin()
{
	// TODO: 在此添加控件通知处理程序代码
	///////////后台消息处理////////////////////////////////////////////////
	CString strTxt = _T("");
	if ( m_bChooseFlag )
	{
		FRIEND_INFO *pFriend = &(m_clusterInfo.members[m_row]);	////当前选中行的详细信息
		if ( pFriend->GetId()==g_ImUserInfo.GetId() )
		{
			ShowMessage("您是本群的最高长官!管理员是你的下属!");
			return ;
		}
		if ( pFriend->cluster_property>0 )
		{
			CString str;
			str.Format("%s 已经是本群的管理员了!",pFriend->nickname);
			ShowMessage(str);
			return ;
		}
		else
		{
			int admin_num = 0;
			for(int num = 0; num < m_clusterInfo.Count(); num ++)
			{
				FRIEND_INFO *pFriend = &(m_clusterInfo.members[num]);
				if(pFriend->cluster_property == 1)
					admin_num ++;
			}

			if(admin_num >= 10)
			{
				CMessageBox::Model(GetSafeHwnd(),_T("管理员已达到上限！"));
				return;
			}
			pFriend->cluster_property =  1;////修改属性
		    /*	m_list.SetItem(m_row, 0, LVIF_TEXT | LVIF_IMAGE, _T("管理员"), PIC_ADMIN, 0, 0, 0);*/
			m_list.SetItemImage(m_row,0,PIC_ADMIN);
		}
		XT_SET_CMEMBERS_REQ req;

		req.nFamilyID = m_clusterInfo.nFamilyID;
		req.nAction = CMEMBER_SET_ADMIN;//CMEMBER_ADD
		req.nFamilyProp = m_clusterInfo.nFamilyProp;
		req.nCount = 1;
		req.nMemID[0] = pFriend->GetId();
		g_pProtocol->SendReq(req);

		//this->LockOperation();
		//this->ShowProgress("正在设置管理员...");
		//////////////////////////////////////////////////////////////////////
	}
	else
	{
		CMessageBox::Model(GetSafeHwnd(),_T("请选择管理员！"));
	}
}


void CClusterMember::OnBnClickedBtnCancelAdmin()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_bChooseFlag)////选中ListCtrl某行
	{
		FRIEND_INFO *pFriend = &(m_clusterInfo.members[m_row]);	////当前选中行的详细信息
		if ( pFriend->GetId()==g_ImUserInfo.GetId() )
		{
			ShowMessage("您不能更改自己的权限哦!");
			return ;
		}
		else if ( pFriend->cluster_property!=1 )
		{
			CString str;
			str.Format("%s 不是本群的管理员!请选择管理员",pFriend->nickname);
			ShowMessage(str);
			return ;
		}
		else
		{
			pFriend->cluster_property = 0;////修改属性
			//m_list.SetItem(m_row, 0, LVIF_TEXT | LVIF_IMAGE, _T(""), PIC_FRIEND_OFF, 0, 0, 0);////取消权限
	  	    m_list.SetItemImage(m_row,0,PIC_FRIEND_OFF);
		}
		XT_SET_CMEMBERS_REQ req;

		req.nFamilyID = m_clusterInfo.nFamilyID;
		req.nAction = CMEMBER_CANCEL_ADMIN;//CMEMBER_ADD
		req.nFamilyProp = m_clusterInfo.nFamilyProp;
		req.nCount = 1;
		req.nMemID[0] = pFriend->GetId();
		g_pProtocol->SendReq(req);
		Sleep(50);
		//this->LockOperation();
		//this->ShowProgress("正在取消管理员资格...");
	}
	else
	{
		CMessageBox::Model(GetSafeHwnd(),_T("请选择管理员！"));
	}
}


void CClusterMember::OnBnClickedBtnDelMember()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_bChooseFlag)
    {
		CString sMsg;
		
		FRIEND_INFO *pFriend = &(m_clusterInfo.members[m_row]);	////当前选中行的详细信息
		if ( pFriend->GetId()==g_ImUserInfo.GetId() )
		{
			ShowMessage("您不能删除自己。");
			return ;
		}

		WORD nCount=m_list.GetItemCount();//总行数
		//// 如果成员小于规定个数,解散
		if(nCount<=MIN_CLUSTER_MEMBER)
		{
			sMsg.Format("群\"%s\"的成员个数如果少于%d个的话,将会被系统自动解散!确定要删除成员\"%s\"吗?",
				m_clusterInfo.szFamilyName,
				MIN_CLUSTER_MEMBER,
				pFriend->nickname);
			if ( CMessageBox::Model(GetSafeHwnd(),sMsg,IM_CAPTION,MB_OKCANCEL)==IDOK )
			{
				XT_CLUSTER_DISMISS_REQ req;
				req.nFamilyID = m_clusterInfo.nFamilyID;
				req.nFamilyProp = PERM_FAMILY;
				g_pProtocol->SendReq(req);

				//清空当前表项
				m_list.DeleteAllItems();

				// 删除主界面群列表对应的项
				::SendMessage(CMainFrame::GetInstance()->GetHWND(),WM_IMOPERATION,OPR_DEL_CLUSTER,(LPARAM)m_clusterInfo.nFamilyID);	// 发送此消息时会将本窗口关闭
				return;
			}
		}
		else
		{
			sMsg.Format("是否删除该用户");

			if (CMessageBox::Model(GetSafeHwnd(),sMsg,IM_CAPTION,MB_YESNO)==IDYES )
			{
				XT_SET_CMEMBERS_REQ req;

				req.nFamilyID = m_clusterInfo.nFamilyID;
				req.nAction     = CMEMBER_DEL;//
				req.nFamilyProp = m_clusterInfo.nFamilyProp;
				req.nCount = 1;
				req.nMemID[0] = pFriend->GetId();

				g_pProtocol->SendReq(req);	

				///清空当前选中表项
			   	m_list.DeleteItem(m_row);

				////this->LockOperation();
				////this->ShowProgress("删除中,请稍候...");
			}
		}
	}
}

void CClusterMember::OnBnClickedBtnAddMember()
{
	// TODO: 在此添加控件通知处理程序代码
	CClusterMemberSelectDlg dlg(&m_clusterInfo,this);

	if ( IDOK==dlg.DoModal() )
	{
		dlg.GetClusterInfo(m_clusterInfo);
		UpdateClusterInfo();
	}
}

void CClusterMember::UpdateClusterInfo()
{
	SetTimer(TIMER_UPDATE_CLUSTER,10,NULL);	
}

void CClusterMember::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==TIMER_UPDATE_CLUSTER )
	{
		KillTimer(nIDEvent);
		Real_UpdateClusterInfo();
	}
	CDialog::OnTimer(nIDEvent);
}

void CClusterMember::Real_UpdateClusterInfo()
{
	m_list.DeleteAllItems();
	int nItemCount = m_list.GetItemCount();//已有总列数
	CLUSTER_INFO  *pCluster = &this->m_clusterInfo;
	CString str;
	str.Format("群 %s",pCluster->szFamilyName);
	SetWindowText(str);
    ////////////////////////////////////////////////////
	int nType = pCluster->GetMemberType(g_ImUserInfo.GetId());

	for ( int i=0; i<pCluster->Count(); i++ )
	{
		FRIEND_INFO *pFriend = &pCluster->members[i];

		m_list.InsertItem(nItemCount+i,_T(""));	//尾部插入

		int nType =  pFriend->cluster_property;

		if  (nType == 2)
		{
			//m_list.SetItem(m_row, 0, LVIF_TEXT | LVIF_IMAGE, _T("群主"), PIC_CREATOR, 0, 0, 0);
		      m_list.SetItemImage(m_row,0,PIC_CREATOR);
		}
		else if  (nType == 1)
		{
			//m_list.SetItem(m_row, 0, LVIF_TEXT | LVIF_IMAGE, _T("管理员"), PIC_ADMIN, 0, 0, 0);
		    m_list.SetItemImage(m_row,0,PIC_ADMIN);
		}
		str = (UserInfoToStr(pFriend));/////本群的成员信息
		m_list.SetItemText(nItemCount+i,1,str);	

		/////从群ID查找群的信息;
		m_list.SetItemText(nItemCount + i, 2, pFriend->profile_name);///群名片
		/*m_list.SetItemText(nItemCount+i,3,_T("21"));
		m_list.SetItemText(nItemCount+i,4,_T("2014/7/24"));*/
		m_edit.ShowWindow(SW_HIDE);
	}

	//FRIEND_INFO *pCreator = pCluster->GetCreator();

	//if ( pCreator )
	//	m_edtCreator.SetWindowText(pCreator->nickname);
	//else
	//	m_edtCreator.SetWindowText(UIntToStr(pCluster->nCreatorID));

	//m_edtID.SetWindowText(UIntToStr(pCluster->nFamilyID));
	//m_edtIntro.SetWindowText(pCluster->szFamilyDesc);
	//m_edtName.SetWindowText(pCluster->szFamilyName);
	//if(g_pWebConfig )
	//{
	//	CString sUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(FAMILY_HOME,StrToWideStr(pCluster->szFamilyDomain));
	//	m_btnLinkHome.SetWindowText(sUrl);
	//}

	//m_btnLinkHome.SizeToContent();
	////m_combType.SetCurSel(pCluster->nFamilyClass);
	//ComboSelFamilyClass(&m_combType,pCluster->nFamilyClass);

	//CLUSTER_INFO *pRealCluster = FindClusterInfo(pCluster->nFamilyID);

	//if ( pRealCluster )
	//{
	//	m_btnCheckMagic.SetCheck(pRealCluster->bAutoShowMagic);
	//	m_btnCheckRefuse.SetCheck(pRealCluster->bRefused);
	//}

	//m_pBmpFamilyHead = GetClusterBitmap(pRealCluster,false);

	//SetClusterAuthRadioBtn(pCluster->nJoinFlag);

	//if ( nType==2 )
	//{
	//	m_btnSetmanage.EnableWindow(TRUE);
	//	m_btnDelmanage.EnableWindow(TRUE);
	//}
	//else
	//{
	//	m_btnSetmanage.EnableWindow(FALSE);
	//	m_btnDelmanage.EnableWindow(FALSE);
	//}

	//if ( nType==2 || nType==1 )//管理员和创建者
	//{
	//	m_edtName.SetReadOnly(FALSE);
	//	m_edtIntro.SetReadOnly(FALSE);
	//	//		m_edtBulitin.SetReadOnly(FALSE);

	//	m_btnRadio1.EnableWindow(TRUE);
	//	m_btnRadio2.EnableWindow(TRUE);
	//	m_btnRadio3.EnableWindow(TRUE);

	//	m_combType.EnableWindow(TRUE);
	//	m_btnAdvancedSetting.EnableWindow(TRUE);
	//	//m_btnFamilyHeadModify.ShowWindow(SW_SHOW);
	//}
	//else
	//{
	//	m_btnSetmembers.EnableWindow(FALSE);
	//	m_btnDelmember.EnableWindow(FALSE);
	//	m_btnRadio1.EnableWindow(FALSE);
	//	m_btnRadio2.EnableWindow(FALSE);
	//	m_btnRadio3.EnableWindow(FALSE);
	//	m_btnAdvancedSetting.EnableWindow(FALSE);

	//	//m_btnFamilyHeadModify.ShowWindow(SW_HIDE);
	//}

	//m_lstCluster.SortClusterList();
}


void CClusterMember::OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView=(NM_LISTVIEW*)pNMHDR;
	CRect rc;
	if(pNMListView->iItem!=-1)
	{
		m_bChooseFlag = TRUE;//选中了ListCtrl中的某一项
		m_row = pNMListView->iItem;//m_row为被选中行的行序号（int类型成员变量）
		m_column = 2;//m_column为被选中行的列序号（int类型成员变量）
		if(pNMListView->iItem!=-1)//选择第二列时，弹出修改框
		{
			m_bChooseMemberFlag = TRUE;
		}
		else
		{
			m_bChooseMemberFlag = FALSE;
		}
	}
	*pResult = 0;
}

void CClusterMember::OnBnClickedBtnModifyInfo()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_bChooseMemberFlag)////选中列
	{
		m_bChooseMemberFlag = FALSE;
		ModifyProfileName();
	}
	else
	{
		CMessageBox::Model(GetSafeHwnd(),_T("请选中【群名片】所在的列！"));
	}
}



void CClusterMember::ShowProgress(CString strTip)
{
	CloseProgress();

	m_pProgress = new CFormProgress3(m_hWnd,this);
	m_pProgress->SetTip(strTip);
	m_pProgress->CenterWindow(this);
	m_pProgress->ShowWindow(SW_SHOW);
}

void CClusterMember::CloseProgress()
{
	if ( m_pProgress )
	{
		m_pProgress->DestroyWindow();
		delete m_pProgress;
		m_pProgress=NULL;
	}
}


void CClusterMember::OnSetCMemberAck(const char *data,WORD wRet)
{
	LPXT_SET_CMEMBERS_ACK ack = (LPXT_SET_CMEMBERS_ACK)data;

	if ( wRet==0 && ack->nFamilyID==this->m_clusterInfo.nFamilyID && ack->nFamilyProp==PERM_FAMILY )
	{
		if ( ack->nAction==CMEMBER_SET_ADMIN )//设为管理员
		{
			RegUserOperation(0x00b5);

			//for ( int i=0; i<m_lstCluster.GetCount(); i++ )
			{
				FRIEND_INFO *pUser = &(m_clusterInfo.members[m_row]);	////当前选中行的详细信息
				if ( pUser->GetId()==ack->nMemberID )
				{
					pUser->cluster_property=1;
				    m_list.SetItemImage(m_row,0,PIC_ADMIN);
					m_list.Invalidate(TRUE);
					
				}
					//break;
			}
		}

		if ( ack->nAction==CMEMBER_CANCEL_ADMIN )//撤销管理员
		{
			RegUserOperation(0x00b4);

			//for ( int i=0; i<m_lstCluster.GetCount(); i++ )
			{
				FRIEND_INFO *pUser = &(m_clusterInfo.members[m_row]);	////当前选中行的详细信息
				if ( pUser->GetId()==ack->nMemberID )

				{
					pUser->cluster_property=0;
					m_list.SetItemImage(m_row,0,PIC_FRIEND_OFF);
					m_list.Invalidate(TRUE);
			
					//break;
				}
			}
		}

		if ( ack->nAction == CMEMBER_DEL )//删除成员
		{		
			RegUserOperation(0x00b2);

			//for ( int i=0; i<m_lstCluster.GetCount(); i++ )
			{
				FRIEND_INFO *pUser = &(m_clusterInfo.members[m_row]);	////当前选中行的详细信息
				if ( pUser->GetId()==ack->nMemberID )
				{
					//m_clusterInfo.DelMember(ack->nMemberID);
					m_list.DeleteItem(m_row);
					//m_lstCluster.ResetContent();

					for ( int i=0; i<m_clusterInfo.Count(); i++ )
					{
						FRIEND_INFO *pFriend = &m_clusterInfo.members[i];

						//int nItem = m_lstCluster.AddString(UserInfoToStr(pFriend));
						//m_lstCluster.SetItemData(nItem,(DWORD)pFriend);
					}

					////m_lstCluster.SortClusterList();
					//break;
				}
			}
		}
		if (ack->nAction == CMEMBER_PROFILE_MOD)//修改群名片
		{
		    RegUserOperation(0x00b1);

			FRIEND_INFO *pUser = &(m_clusterInfo.members[m_row]);	////当前选中行的详细信息
			if (pUser->GetId() == ack->nMemberID)
			{
				CString strTxt = _T("");
				m_edit.GetWindowText(strTxt);
				int strLen = strTxt.GetLength();
				if (strLen > 0)
				{
					memcpy(pUser->profile_name, (void*)strTxt.GetBuffer(strLen), strLen);
					pUser->profile_name[strLen] = '\0';
				}
				m_list.Invalidate(TRUE);
				//break;
			}
		}
	}

	//this->UnLockOperation();
	this->CloseProgress();
}


BOOL CClusterMember::PreTranslateMessage(MSG* pMsg)
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