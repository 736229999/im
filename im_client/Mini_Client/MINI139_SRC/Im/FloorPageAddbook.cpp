// im\TabPageCluster.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\floorpageaddbook.h"
#include "p2pSendmsgDlg.h"
#include "DuiP2PSendMsgDlg.h"
#include "MessageHandleWnd.h"
#include "..\FriendInfoDlg.h"
#include "ClusterCreateDlg.h"
#include "LocalData.h"
#include "xtpacket.h"
#include "DialogClusterAuth.h"
#include "DailogClusterCheck.h"
#include "FSDialog.h"
#include "MsgDlg.h"
#include "imaddresslistctrl.h"
#include "contactinvitedlg.h"
#include "OutlookContactSelDlg.h"
// CFloorPageAddbook 对话框


#define ID_ADDRESS_MODIFY		1001
#define ID_ADDRESS_ADD			1002
#define ID_MEMBER_DEL           1003
#define ID_GROUP_DEL            1004
#define ID_INVITE_MINI139       1005
#define ID_SENDMAIL	            1006
#define ID_INVITE_GROUP         1007

#define IDC_CLUSTER_LIST        10101

#define TIMER_MSG               0x01

IMPLEMENT_DYNAMIC(CFloorPageAddbook, CDialog)
IMPLEMENT_HMG_SCROLLBAR(CFloorPageAddbook)

static int iClusterBuildCount=0;

CFloorPageAddbook::CFloorPageAddbook(CWnd* pParent /*=NULL*/)
	: CDialog(CFloorPageAddbook::IDD, pParent)
{
	m_bSorted = false;
}

CFloorPageAddbook::~CFloorPageAddbook()
{

}

void CFloorPageAddbook::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_ADD_ADDR, m_wndBtnAdd);
	DDX_Control(pDX, IDC_BUTTON_MODIFY_ADDR, m_wndBtnModify);
	DDX_Control(pDX, IDC_BUTTON_DEL_ADDR, m_wndBtnDelete);
	DDX_Control(pDX, IDC_BUTTON_MYADDRESS, m_wndBtnSetMyInfo);
	DDX_Control(pDX, IDC_BUTTON_MINILINK, m_wndBtnMiniLink);
}


BEGIN_MESSAGE_MAP(CFloorPageAddbook, CDialog)
	ON_WM_SIZE()
	ON_MESSAGE(WM_CLUSTER_LIST_NOTIFY, OnGroupListNotify)
	ON_WM_DESTROY()
	SET_HMG_SCROLLBAR(IDC_CLUSTER_LIST)
	ON_WM_ERASEBKGND()
	ON_WM_INITMENUPOPUP()
	ON_WM_MEASUREITEM()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_ADD_ADDR, OnBnClickedButtonAddAddr)
	ON_MESSAGE(WM_ADDRESS_NOTIFY,OnAddressNotify)
	ON_COMMAND(ID_GROUP_DEL,OnGroupDel)
	ON_COMMAND(ID_MEMBER_DEL,OnMemberDel)
	ON_COMMAND(ID_ADDRESS_MODIFY,OnMemberModify)
	ON_COMMAND(ID_ADDRESS_ADD,OnMemberAdd)
	ON_COMMAND(ID_SENDMAIL,OnSendMail)
	ON_COMMAND(ID_INVITE,OnInvite)
	ON_COMMAND(ID_INVITE_GROUP, OnInviteGroup)
	ON_BN_CLICKED(IDC_BUTTON_DEL_ADDR, OnBnClickedButtonDelAddr)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY_ADDR, OnBnClickedButtonModifyAddr)
	ON_MESSAGE(WM_SAVECARD_NOTIFY,OnSaveCardNotify)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BUTTON_MINILINK, OnBnClickedButtonMinilink)
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CFloorPageAddbook 消息处理程序

BOOL CFloorPageAddbook::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rc(0,0,40,40);
	m_wndAddressList.Create(this,rc,WS_VISIBLE|WS_CHILD);
	//m_wndAddressList.SetImageList(&g_imgHead16);
	m_wndAddressList.SetImageList(&g_imgStatus35); 
	m_wndAddressList.SetDlgCtrlID(IDC_CLUSTER_LIST);

	this->m_wndBtnAdd.SetBitmap(IDB_BTN_ADD,IDB_BTN_ADD,RGB(0,255,0));
	this->m_wndBtnDelete.SetBitmap(IDB_BTN_DEL,IDB_BTN_DEL);
	this->m_wndBtnModify.SetBitmap(IDB_BTN_MODIFY,IDB_BTN_MODIFY);
	this->m_wndBtnSetMyInfo.SetBitmap(IDB_BITMAP_SYS_SET_NORMAL,IDB_BITMAP_SYS_SET_NORMAL);
	this->m_wndBtnMiniLink.SetBitmap(IDB_BTN_MINILINK,IDB_BTN_MINILINK);

	//m_wndBtnAdd.SetFlatStyle(CFlatButton::FS_OFFICE);
	//m_wndBtnDelete.SetFlatStyle(CFlatButton::FS_OFFICE);
	//m_wndBtnModify.SetFlatStyle(CFlatButton::FS_OFFICE);
	//m_wndBtnSetMyInfo.SetFlatStyle(CFlatButton::FS_OFFICE);
	//m_wndBtnMiniLink.SetFlatStyle(CFlatButton::FS_OFFICE);

	m_wndBtnAdd.SetToolTip("添加");
	m_wndBtnDelete.SetToolTip("删除");
	m_wndBtnModify.SetToolTip("修改");
	m_wndBtnSetMyInfo.SetToolTip("设置我的名片信息");
	m_wndBtnMiniLink.SetToolTip("获取 哞哞 下载链接");

	AttachHMGScrollbar(m_wndAddressList.m_hWnd);

	this->EnableToolTips(TRUE);

	return TRUE; 
}
void CFloorPageAddbook::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if( m_wndAddressList.m_hWnd )
	{
		m_wndAddressList.MoveWindow(0,23,cx,cy-23);
	}
}

//处理来自好友列表的鼠标右键
LRESULT CFloorPageAddbook::OnGroupListNotify(WPARAM wParam, LPARAM lParam)
{
	AL_ITEM_POS pos=m_wndAddressList.GetCurSel();

	int nGroup = pos.nGroup;
	int nItem  = pos.nItem;

	switch ( wParam )
	{
	case NM_AL_LBUTTONDBLCLICK:		
		if( nItem != -1 && nGroup!=-1)		//双击了一个有效的联系人
		{
			OnMemberModify();
		}

		break;	
	case NM_AL_RBUTTONDOWN:
	case NM_AL_LBUTTONDOWN:
		{
			if ( nGroup==0 )			
			{
				if ( m_wndBtnDelete.IsWindowEnabled() )
				{
					m_wndBtnDelete.EnableWindow(FALSE);
                    m_wndBtnDelete.Invalidate();
				}
			}
			else
			{
				if ( m_wndBtnDelete.IsWindowEnabled()==FALSE )
				{
					m_wndBtnDelete.EnableWindow(TRUE);
                    m_wndBtnDelete.Invalidate();
				}
			}
		}
		break;
	case NM_AL_RBUTTONUP:
		{
			CMenuEx menu,*popmenu;
			menu.LoadMenu(IDR_MENU_TEMPLATE);
			popmenu = menu.GetSubMenu(0);
			popmenu->RemoveMenu(0,MF_BYPOSITION);

			CPoint point;
			GetCursorPos(&point);

			if( nItem != -1)			//点在某个联系人上
			{
				ContactPersonInfo *pInfo =(ContactPersonInfo *)m_wndAddressList.GetItemData(nGroup,nItem);
				CString strInfo;

				if ( pInfo->nType==1 )//out look person
				{
					strInfo.Format("发送电子邮件...",pInfo->strName);
					popmenu->AppendMenu(MF_STRING,ID_SENDMAIL,strInfo);

					strInfo.Format("查看/编辑\"%s\"的信息",pInfo->strName);
					popmenu->AppendMenu(MF_STRING,ID_ADDRESS_MODIFY,strInfo);					

					popmenu->AppendMenu(MF_SEPARATOR,0,0);

					strInfo.Format("邀请\"%s\"一起使用IEC-iM...",pInfo->strName);
					popmenu->AppendMenu(MF_STRING,ID_INVITE,strInfo);
				}
				else
				{
					popmenu->AppendMenu(MF_STRING,ID_ADDRESS_ADD,"添加联系人\t(&A)");
					popmenu->AppendMenu(MF_STRING,ID_MEMBER_DEL,"删除联系人\t(&D)");
					popmenu->AppendMenu(MF_SEPARATOR,0,0);
					popmenu->AppendMenu(MF_STRING,ID_ADDRESS_MODIFY,"查看/编辑联系人\t(&M)");
				}
				
			}
			else
			if ( nGroup!=-1 )  //点到组节点上
			{
				if ( nGroup==0 )// out look 组
				{
					popmenu->AppendMenu(MF_STRING,ID_INVITE_GROUP,"邀请使用IEC-iM...");
				}
				else
				{
					popmenu->AppendMenu(MF_STRING,ID_ADDRESS_ADD,"添加联系人\t(&A)");
					popmenu->AppendMenu(MF_STRING,ID_GROUP_DEL,"删除组\t(&D)");
				}
			}
			
			//弹出右键菜单
			popmenu->TrackPopupMenu(TPM_LEFTBUTTON, point.x, point.y, this);
		}
		break;
	case NM_AL_DRAGDROP:
		{
			int nGroupDest = LOBYTE(lParam);	

			if ( nGroupDest==0 || nGroup==0 )
			{
				m_wndAddressList.SetCurSel(nGroup,nItem);
			}
			else
			{
				int nDestItem = m_wndAddressList.MoveItem(nGroup,nItem,nGroupDest);

				ContactPersonInfo *pInfo = (ContactPersonInfo *)m_wndAddressList.GetItemData(nGroupDest,nDestItem);
				pInfo->nGroupIndex = m_wndAddressList.GetGroupData(nGroupDest);

				g_LocalData.SaveAddressBook(m_listPerson);
				m_wndAddressList.SetCurSel(nGroupDest,nDestItem);
			}
		}

		break;
	case NM_AL_MAIL_CLICK:
		{
			AL_ITEM_POS * pos = (AL_ITEM_POS *)lParam;
			ContactPersonInfo *p = (ContactPersonInfo *)m_wndAddressList.GetItemData(pos->nGroup,pos->nItem);
			if ( p )
			{
				CContactInviteDlg::Show(p->strEmail,p->strName);
			}
		}
		break;
	}
	return 0;
}

void CFloorPageAddbook::OnDestroy()
{
	CDialog::OnDestroy();

	CDialogAddAddress::Close();

	m_wndAddressList.DeleteAllItems();
	ClearAddressBookList(m_listPerson);
}

BOOL CFloorPageAddbook::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CFloorPageAddbook::OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu)
{
	CDialog::OnInitMenuPopup(pPopupMenu, nIndex, bSysMenu);
	CMenuEx::InitPopupMenu(pPopupMenu,nIndex,bSysMenu);
}

void CFloorPageAddbook::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CDialog::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
	CMenuEx::MeasureItem(lpMeasureItemStruct);
}

void CFloorPageAddbook::LoadAddressBook()
{
	m_wndAddressList.DeleteAllItems();
	::ClearAddressBookList(m_listPerson);
	g_LocalData.LoadAddressBookGroupInfo(m_groupInfo);
	g_LocalData.LoadAddressBook(m_listPerson);

	UpdateAddressList();

	BOOL bVisible = IsWindowVisible();

	int n = m_wndAddressList.GetGroupCount();
	for ( int g=0; g<n; g++ )
	{
		int items = m_wndAddressList.GetItemCount(g);

		if ( items>0 )
		{
			if ( bVisible )
				QuickSort(g,0,items-1);

			m_wndAddressList.SetGroupState(g,ALS_EXPAND);
		}
	}

	m_bSorted = bVisible;
}

void CFloorPageAddbook::SaveAddressBook()
{
	g_LocalData.SaveAddressBook(m_listPerson);
	g_LocalData.SaveAddressBookGroupInfo(m_groupInfo);
}


void CFloorPageAddbook::OnBnClickedButtonAddAddr()
{
	OnMemberAdd();
}

LRESULT CFloorPageAddbook::OnAddressNotify(WPARAM wParam, LPARAM lParam)
{
	tagContactInfo *pInfo = (tagContactInfo *)lParam;

	if ( pInfo->personInfo.nType==0 )
	{
		m_groupInfo = pInfo->groupInfo;
		UpdateGroupInfo();
	}

	if ( wParam==(CDialogAddAddress::Addr_Add) )//add contact person info
	{
		ContactPersonInfo *pPersonInfo = new ContactPersonInfo();

		*pPersonInfo = pInfo->personInfo;
		m_listPerson.push_back(pPersonInfo);

		AddPerson2List(pPersonInfo,true);

		g_LocalData.SaveAddressBookGroupInfo(m_groupInfo);
		g_LocalData.SaveAddressBook(m_listPerson);
	}
	else
	if ( wParam==(CDialogAddAddress::Addr_Mofify) )//modify
	{
		int nGroup,nItem;

		if ( FindItem(pInfo->personInfo.strName,nGroup,nItem) )
		{
			ContactPersonInfo *p = (ContactPersonInfo *)m_wndAddressList.GetItemData(nGroup,nItem);
		
			int iOldGroup = p->nGroupIndex;
			int iNewGroup = pInfo->personInfo.nGroupIndex;

			*p = pInfo->personInfo;
			g_LocalData.SaveAddressBook(m_listPerson);
			
			if ( pInfo->personInfo.nType==0 && iOldGroup!=iNewGroup )
			{
				g_LocalData.SaveAddressBookGroupInfo(m_groupInfo);
				
				int nIndex = FindGroupData(iNewGroup);

				if ( nIndex!=-1 )
				{
					m_wndAddressList.MoveItem(nGroup,nItem,nIndex);
				}
			}
		}
	}

	return 0;
}

void CFloorPageAddbook::UpdateAddressList()
{
	m_wndAddressList.DeleteAllItems();
	
	int nOutlook = m_wndAddressList.AddGroup("Outlook地址簿");
	m_wndAddressList.SetGroupData(nOutlook,0xFFFE);

	for ( int i=0; i<m_groupInfo.count; i++ )
	{
		int n = m_wndAddressList.AddGroup(m_groupInfo.items[i].szName);
		m_wndAddressList.SetGroupData(n,m_groupInfo.items[i].iIndex);
	}

	ADDRESS_BOOK_LIST::iterator itAddress;

	for ( itAddress=m_listPerson.begin(); itAddress!=m_listPerson.end(); itAddress++)
	{
		ContactPersonInfo *p = (*itAddress);
		AddPerson2List(p,false);
	}

}

void CFloorPageAddbook::AddPerson2List(const ContactPersonInfo *pInfo, bool bSel)
{
	int iGroup = -1;
	int iCount = m_wndAddressList.GetGroupCount();


	for ( int i=0; i<iCount; i++ )
	{
		if ( pInfo->nType==1 )
		{
			/*if ( m_wndAddressList.GetGroupData(i)==pInfo->nGroupIndex )
			{
				iGroup=i;
				break;
			}*/
			iGroup=0;
			break;
		}
		else
		if ( pInfo->nType==0 )
		{
			if ( m_wndAddressList.GetGroupData(i)==pInfo->nGroupIndex )
			{
				iGroup=i;
				break;
			}
		}
	}
	

	if ( iGroup!=-1 )
	{
		int nItem=m_wndAddressList.AddItem(iGroup,pInfo->strName,0);
		m_wndAddressList.SetItemData(iGroup,nItem,(DWORD)pInfo);

		if ( bSel )
			m_wndAddressList.SetCurSel(iGroup,nItem);
	}
}

void CFloorPageAddbook::OnMemberDel()
{
	AL_ITEM_POS pos = m_wndAddressList.GetCurSel();

	if ( pos.nGroup!=-1 && pos.nItem!=-1 )
	{
		if ( CMessageBox::Model(GetSafeHwnd(),"您确信要删除吗?","确认",MB_YESNO)!=IDYES )
			return ;

		ADDRESS_BOOK_LIST::iterator it;

		for (it=m_listPerson.begin(); it!=m_listPerson.end(); it++ )
		{
			if ( m_wndAddressList.GetItemData(pos.nGroup,pos.nItem)==(DWORD)(*it) )
			{
				m_wndAddressList.DeleteItem(pos.nGroup,pos.nItem);
				m_listPerson.erase(it);
				break;
			}
		}

		g_LocalData.SaveAddressBook(m_listPerson);
	}
	else
	{
		//MessageBox("请选择一个联系人！","删除联系人",MB_OK|MB_ICONINFORMATION);
		CMessageBox::Prompt("请选择一个联系人！","删除联系人");
	}
}

void CFloorPageAddbook::OnGroupDel()
{
	AL_ITEM_POS pos = m_wndAddressList.GetCurSel();

	if ( pos.nGroup!=-1 && pos.nItem==-1 )
	{
		if ( m_wndAddressList.GetItemCount(pos.nGroup)==0 )
		{
			int iIndex = m_wndAddressList.GetGroupData(pos.nGroup);

			m_wndAddressList.DelGroup(pos.nGroup);
			m_groupInfo.Delete(iIndex);
			g_LocalData.SaveAddressBookGroupInfo(m_groupInfo);
		}
		else
		{
			//MessageBox("您只能删除为空的组！请将该组中所有的联系人移动到另一个组中！","删除组",MB_OK|MB_ICONINFORMATION);
			CMessageBox::Prompt("您只能删除为空的组！请将该组中所有的联系人移动到另一个组中！","删除组");
		}
	}
}

void CFloorPageAddbook::OnBnClickedButtonDelAddr()
{
	AL_ITEM_POS pos = m_wndAddressList.GetCurSel();

	if ( pos.nGroup!=-1 )
	{
		if ( pos.nItem==-1 )
			OnGroupDel();
		else
			OnMemberDel();
	}
}

void CFloorPageAddbook::OnMemberModify()
{
	AL_ITEM_POS pos = m_wndAddressList.GetCurSel();

	if ( pos.nGroup!=-1 && pos.nItem!=-1 )
	{
		ContactPersonInfo *p = (ContactPersonInfo *)m_wndAddressList.GetItemData(pos.nGroup,pos.nItem);
		CDialogAddAddress::Show(CDialogAddAddress::Addr_Mofify,*p,m_groupInfo,m_hWnd);
	}
	else
	{
		CMessageBox::Prompt("请选择一个您要编辑的联系人！");
	}
}

void CFloorPageAddbook::OnInviteGroup()
{
	if ( COutlookContactSelDlg::GetInstance()==NULL )
	{
		COutlookContactSelDlg *pDlg = COutlookContactSelDlg::Show();
		ADDRESS_BOOK_LIST::iterator it;
		for ( it=m_listPerson.begin(); it!=m_listPerson.end(); it++ )
		{
			ContactPersonInfo *p = (*it);

			if (p->nType==1 )
				pDlg->AddContact(p->strName,p->strEmail);
		}
	}
	else
	{
		COutlookContactSelDlg::Show();
	}
	
}

void CFloorPageAddbook::OnInvite()
{
	AL_ITEM_POS pos = m_wndAddressList.GetCurSel();
	ContactPersonInfo *p = (ContactPersonInfo *)m_wndAddressList.GetItemData(pos.nGroup,pos.nItem);
	if ( p && p->nType==1 )
	{
		CContactInviteDlg::Show(p->strEmail,p->strName);
	}
	
}

void CFloorPageAddbook::OnSendMail()
{
	AL_ITEM_POS pos = m_wndAddressList.GetCurSel();
	ContactPersonInfo *p = (ContactPersonInfo *)m_wndAddressList.GetItemData(pos.nGroup,pos.nItem);
	if ( p )
	{
		CString strMail;
		strMail.Format("mailto:%s",(char *)(LPCSTR)p->strEmail);
		ShellExecute(NULL,"",(LPCSTR)strMail,"","",SW_SHOW);
	}
	
}

void CFloorPageAddbook::OnBnClickedButtonModifyAddr()
{
	OnMemberModify();
}

void CFloorPageAddbook::UpdateGroupInfo()
{
	map<int,int> mapGroup;
	int nGroupCount = m_wndAddressList.GetGroupCount();

	for ( int i=0; i<nGroupCount; i++ )
	{
		mapGroup.insert(map<int,int>::value_type((int)m_wndAddressList.GetGroupData(i),i));
	}

	for ( int i=0; i<m_groupInfo.count; i++ )
	{
		map<int,int>::iterator it = mapGroup.find(m_groupInfo.items[i].iIndex);

		if ( it==mapGroup.end() )
		{
			int n=m_wndAddressList.AddGroup(m_groupInfo.items[i].szName);
			m_wndAddressList.SetGroupData(n,(DWORD)m_groupInfo.items[i].iIndex);
		}
	}
}

int CFloorPageAddbook::FindGroupData(int iData)
{
	int c=m_wndAddressList.GetGroupCount();
	for ( int i=0; i<c; i++ )
	{
		if ( m_wndAddressList.GetGroupData(i)==iData )
		{
			return i;
		}
	}

	return -1;
}

void CFloorPageAddbook::OnMemberAdd()
{
	ContactPersonInfo info;
	AL_ITEM_POS pos = m_wndAddressList.GetCurSel();

	if ( pos.nGroup!=-1 )
	{
		info.nGroupIndex = m_wndAddressList.GetGroupData(pos.nGroup);
	}

	CDialogAddAddress::Show(CDialogAddAddress::Addr_Add,info,m_groupInfo,m_hWnd);

}
bool CFloorPageAddbook::FindItem(CString strItem, int &nGroup, int &nItem)
{
	int nGroupCount = m_wndAddressList.GetGroupCount();
	for ( int i=0; i<nGroupCount; i++)
	{
		int nItemCount = m_wndAddressList.GetItemCount(i);
		for ( int j=0; j<nItemCount; j++ )
		{
			if ( strItem==m_wndAddressList.GetItemText(i,j) )
			{
				nGroup=i;
				nItem = j;
		
				return true;
			}
		}
	}

	return false;
}

LRESULT CFloorPageAddbook::OnSaveCardNotify(WPARAM wParam, LPARAM lParam)
{
	FRIEND_INFO *pFriend = (FRIEND_INFO *)lParam;

	CString strNickName = pFriend->nickname;
	strNickName.Trim();

	bool bFind = false;

	ADDRESS_BOOK_LIST::iterator it;
	for ( it=m_listPerson.begin(); it!=m_listPerson.end(); it++ )
	{
		if ( (*it)->strName==strNickName)
		{
			bFind = true;
			break;
		}
	}

	if ( bFind )
	{
		CDialogAddAddress::Show(CDialogAddAddress::Addr_Mofify, *(*it), m_groupInfo,m_hWnd);
	}
	else
	{
		ContactPersonInfo personInfo;

		personInfo.strName    = strNickName;
		personInfo.nSex       = pFriend->sex;
		personInfo.strAddress = pFriend->address;
        personInfo.strNote    = pFriend->description;
		personInfo.strEmail   = pFriend->email;
		personInfo.strPhone   = pFriend->mobile;
		personInfo.strBlog.Format("http://%s.feiniu.com",pFriend->GetUserName().c_str());
		CDialogAddAddress::Show(CDialogAddAddress::Addr_Add, personInfo, m_groupInfo,m_hWnd);
	}

	return 0;
}

void CFloorPageAddbook::QuickSort(int nGroup, int iLo, int iHi)
{
	//if ( iLo<iHi )
	//{
	//	CString strKey = ((ContactPersonInfo *)m_wndAddressList.GetItemData(nGroup,iLo))->strName;

	//	int lo = iLo;
	//	int hi = iHi;

	//	while (lo<hi)
	//	{
	//		ContactPersonInfo *fhi = (ContactPersonInfo *)m_wndAddressList.GetItemData(nGroup,hi);

	//		while ( lo<hi && fhi->strName>strKey )
	//		{
	//			--hi;
	//			fhi = (ContactPersonInfo *)m_wndAddressList.GetItemData(nGroup,hi);
	//		}

	//		m_wndAddressList.SwapItem(nGroup,lo,nGroup,hi);

	//		ContactPersonInfo *flo = (ContactPersonInfo *)m_wndAddressList.GetItemData(nGroup,lo);

	//		while ( lo<hi && flo->strName<strKey )
	//		{
	//			++lo;
	//			flo = (ContactPersonInfo *)m_wndAddressList.GetItemData(nGroup,lo);
	//		}

	//		m_wndAddressList.SwapItem(nGroup,lo,nGroup,hi);
	//	}

	//	QuickSort(nGroup,iLo,lo-1);
	//	QuickSort(nGroup,lo+1,iHi);
	//}
}


void CFloorPageAddbook::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if ( bShow && m_bSorted==false )
	{
		int n = m_wndAddressList.GetGroupCount();

		for ( int g=0; g<n; g++ )
		{
			int items = m_wndAddressList.GetItemCount(g);

			if ( items>0 )
			{
				QuickSort(g,0,items-1);
			}
		}

		m_bSorted = true;
	}
}

void CFloorPageAddbook::OnInviteAck(CString strName, CString strEmailAddress)
{
	int n=m_wndAddressList.GetItemCount(0);
	for ( int i=0; i<n; i++ )
	{
		ContactPersonInfo *p = (ContactPersonInfo *)m_wndAddressList.GetItemData(0,i);
		if ( p->strName==strName && p->strEmail==strEmailAddress )
		{
			p->nInvited=1;
			m_wndAddressList.InvalidateItem(0,i);

			break;
		}
	}
}

void CFloorPageAddbook::OnBnClickedButtonMinilink()
{
	char text[]= "http://iecim.feiniu.com";

	if(OpenClipboard())
	{
		HGLOBAL clipbuffer;
		char * buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, strlen(text)+1);
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, (LPCSTR)text);
		GlobalUnlock(clipbuffer);
		SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();

		ShowMessage("IEC-iM的下载地址 \"http://iecim.feiniu.com\" 已经复制到剪贴板了,\n你可以通过MSN 、QQ 或者 Email等方式发送给您的朋友(在编辑框中选择粘贴)，\n邀请大家一起来体验 Mini。");
	}
}

void CFloorPageAddbook::OnPaint()
{
	CPaintDC dc(this); 

	CRect rc;
	GetClientRect(&rc);
	rc.bottom=rc.top+23;
	CResBitmap bmp;
	bmp.LoadBitmap(IDB_HEADER_BACK);
	HorizStretchBlt(&dc,0,0,rc.Width(),&bmp);

}
