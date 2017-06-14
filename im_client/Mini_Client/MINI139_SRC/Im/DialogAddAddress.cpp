// DialogAddAddress.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "DialogAddAddress.h"
#include ".\dialogaddaddress.h"
#include "LocalData.h"


// CDialogAddAddress 对话框
vector<CDialogAddAddress *> CDialogAddAddress::vecInstance ;

IMPLEMENT_DYNAMIC(CDialogAddAddress, CXSkinDlg)
CDialogAddAddress::CDialogAddAddress(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CDialogAddAddress::IDD, pParent)
{
}

CDialogAddAddress::~CDialogAddAddress()
{
	for ( int i=0; i<vecInstance.size(); i++ )
	{
		if ( vecInstance[i]==this )
		{
			vecInstance.erase(vecInstance.begin()+i);
			break;
		}
	}
}

void CDialogAddAddress::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_wndBtnClear);
	DDX_Control(pDX, IDOK, m_wndBtnOk);
	DDX_Control(pDX, IDCANCEL, m_wndBtnCancel);
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_wndBtnApply);
}


BEGIN_MESSAGE_MAP(CDialogAddAddress, CXSkinDlg)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
	ON_MESSAGE(WM_MODIFY_NOTIFY,OnModifyNotify)
END_MESSAGE_MAP()


// CDialogAddAddress 消息处理程序

BOOL CDialogAddAddress::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetDlgStyle(MINCLOSE);
	EnableHelp(true);
	// TODO:  在此添加额外的初始化
	CRect rectThis,rectPart;
	GetWindowRect(rectThis);
	GetDlgItem(IDD_DIALOG_ADD_ADDRESS_CROLL_PART)->GetWindowRect(rectPart);
	m_nTopAddressPart = rectPart.top - rectThis.top;
	m_nBottomAddressPart = rectThis.bottom - rectPart.bottom ;
	m_wndAddressPart.CreateFromCtrl(IDD_DIALOG_ADD_ADDRESS_CROLL_PART, this);


	if ( m_dialogType==Addr_Mofify )
	{
		SetWindowText("修改联系人资料");
		this->m_wndAddressPart.m_wndEditName.SetReadOnly(TRUE);
	}
	else
	if ( m_dialogType==Addr_Add )
	{
		SetWindowText("添加联系人");
		this->m_wndAddressPart.m_wndEditName.SetReadOnly(FALSE);
	}
	return TRUE;
}

void CDialogAddAddress::OnHelpClick()
{
	OpenHtmlPage("www.feiniu.com");
}

void CDialogAddAddress::Show(AddrInfoType type,const ContactPersonInfo &personInfo,const ContactGroupInfo &groupInfo,HWND hTabAddbookPage)
{
	if ( type==Addr_Add )
	{
		for ( int i=0; i<vecInstance.size(); i++ )
		{
			if ( vecInstance[i]->m_dialogType==Addr_Add )
			{
				vecInstance[i]->BringWindowToTop();
				return ;
			}
		}

		CDialogAddAddress *pDlg = new CDialogAddAddress(GetDesktopWindow());
		pDlg->m_dialogType = type;
		pDlg->Create(CDialogAddAddress::IDD,GetDesktopWindow());
		pDlg->ShowWindow(SW_SHOW);
		pDlg->BringWindowToTop();
		pDlg->m_hTabAddbookPage = hTabAddbookPage;
		pDlg->m_wndAddressPart.SetContactInfo(personInfo,groupInfo);

		vecInstance.push_back(pDlg);
	}
	else
	if ( type==Addr_Mofify )
	{
		for ( int i=0; i<vecInstance.size(); i++ )
		{
			if ( vecInstance[i]->m_dialogType==Addr_Mofify )
			{
				if ( vecInstance[i]->m_wndAddressPart.m_strName==personInfo.strName )
				{
					vecInstance[i]->BringWindowToTop();
					return ;
				}
			}
		}

		CDialogAddAddress *pDlg = new CDialogAddAddress(GetDesktopWindow());	
		pDlg->m_dialogType = type;
		pDlg->Create(CDialogAddAddress::IDD,GetDesktopWindow());
		pDlg->ShowWindow(SW_SHOW);
		pDlg->m_wndAddressPart.SetContactInfo(personInfo,groupInfo);
		pDlg->m_hTabAddbookPage = hTabAddbookPage;
		pDlg->BringWindowToTop();

		vecInstance.push_back(pDlg);
	}
}

void CDialogAddAddress::Close()
{
	vector<CDialogAddAddress *> vecTemp;

	for ( int i=0; i<vecInstance.size(); i++ )
	{
		vecTemp.push_back(vecInstance[i]);
	}

	for ( int i=0; i<vecTemp.size(); i++)
		vecTemp[i]->DestroyWindow();
}

void CDialogAddAddress::OnBnClickedOk()
{
	if ( Save() )
		DestroyWindow();
}

void CDialogAddAddress::OnBnClickedButtonApply()
{
	if ( Save() )
	{
		m_wndBtnApply.EnableWindow(FALSE);
		m_wndAddressPart.ResetModifyFlag();
	}
}

void CDialogAddAddress::OnBnClickedCancel()
{
	DestroyWindow();
}
 
void CDialogAddAddress::OnClose()
{
	DestroyWindow();
}

void CDialogAddAddress::OnDestroy()
{
	CXSkinDlg::OnDestroy();
	delete this;
}

void CDialogAddAddress::OnBnClickedButtonClear()
{
	// TODO: 在此添加控件通知处理程序代码
	m_wndAddressPart.Reset();
}

void CDialogAddAddress::OnSize(UINT nType, int cx, int cy)
{
	CXSkinDlg::OnSize(nType, cx, cy);
}

bool CDialogAddAddress::Save()
{
	bool bRet = false;

	if ( m_wndAddressPart.GetContactInfo(m_addressInfo.personInfo,m_addressInfo.groupInfo) )
	{
		bRet=true;
	}

	if ( m_dialogType==Addr_Add && bRet)
	{
		ADDRESS_BOOK_LIST listAddress;
		g_LocalData.LoadAddressBook(listAddress);

		ADDRESS_BOOK_LIST::iterator it;
		for ( it=listAddress.begin(); it!=listAddress.end(); it++ )
		{
			if ( (*it)->strName==m_addressInfo.personInfo.strName )
			{
				//MessageBox("您的通讯录中已经有\""+m_addressInfo.personInfo.strName+"\"的记录了！","温馨提示",MB_OK|MB_ICONINFORMATION);
				CMessageBox::Prompt("您的通讯录中已经有\""+m_addressInfo.personInfo.strName+"\"的记录了！","温馨提示");
				bRet =false;
			}
		}

		ClearAddressBookList(listAddress);
	}

	if ( bRet )
		::SendMessage(m_hTabAddbookPage,WM_ADDRESS_NOTIFY,(WPARAM)m_dialogType,(LPARAM)&m_addressInfo);

	return bRet;
}

LRESULT CDialogAddAddress::OnModifyNotify(WPARAM,LPARAM)
{
	m_wndBtnApply.EnableWindow(TRUE);
	return 0;
}
