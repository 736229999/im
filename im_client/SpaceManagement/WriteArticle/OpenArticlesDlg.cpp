// OpenArticlesDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteArticle.h"
#include "OpenArticlesDlg.h"
#include ".\openarticlesdlg.h"
#include "webinterfacewrapper.h"
#include "WebDataProcess.h"
// COpenArticlesDlg 对话框
extern bool CompareArticleDate(const TWebArticleInfo *p1, const TWebArticleInfo *p2);
//bool CompareArticleDate(const TWebArticleInfo *p1, const TWebArticleInfo *p2)
//{
//	if (p1->dwCreateDate>p2->dwCreateDate)
//		return true;
//	else
//		return false;
//}

IMPLEMENT_DYNAMIC(COpenArticlesDlg, CDialog)
COpenArticlesDlg::COpenArticlesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COpenArticlesDlg::IDD, pParent),
m_myArticles(g_pUserInfo->GetUserID())
{
}

COpenArticlesDlg::~COpenArticlesDlg()
{
}

void COpenArticlesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_LIST1,m_wndListBox);
	DDX_Control(pDX, IDC_BUTTON_REFRESH, m_btnRefresh);
}


BEGIN_MESSAGE_MAP(COpenArticlesDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_LIST1, OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_PCANCLE, OnBnClickedButtonPcancle)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnBnClickedButtonRefresh)
	ON_MESSAGE(WM_WEBDATA_NOTIFY,OnWebDataNotify)
END_MESSAGE_MAP()


// End of CreateImageList

BOOL COpenArticlesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
   
	CRect  rcClient;
	GetClientRect(&rcClient);
	CRect  rcListBox;
	m_wndListBox.GetWindowRect(&rcListBox);
	ScreenToClient(&rcListBox);

	m_wndListArticles.Create(this,CRect(rcListBox.right+5,42,rcClient.Width()-10,rcClient.Height()-45),WS_VISIBLE|WS_CHILDWINDOW|WS_CLIPCHILDREN,1234);
	m_wndListArticles.ShowWindow(SW_SHOW);

	CResBitmap bmp;
	bmp.LoadBitmap(IDB_ARTICLE_SEL);

	m_ImageList.Create(48,48,ILC_COLOR24|ILC_MASK,0,1);
	m_ImageList.Add(&bmp,RGB(0,255,0));

	bmp.DeleteObject();

	m_wndListBox.SetImageList(&m_ImageList);
	m_wndListBox.AddString(_T("           草稿"), 0);
	m_wndListBox.AddString(_T("最近发表的文章"), 1);
	m_wndListBox.SetCurSel(1);

	g_spaceData.LoadUserWebArticles(g_pUserInfo->GetUserID(),m_myArticles);
	sort(m_myArticles.vecArticles.begin(),m_myArticles.vecArticles.end(),CompareArticleDate);

	OnLbnSelchangeList1();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void COpenArticlesDlg::OnLbnSelchangeList1()
{
	int nSel = m_wndListBox.GetCurSel();

	m_wndListArticles.RemoveAllItems();

	if ( nSel==0 )//草稿
	{
		TSavedArticleList al;
		g_spaceData.LoadMySavedArticleList(al);

		int n = al.GetCount();

		for ( int i=0; i<n; i++ )
		{
			TSavedArticleItem item = al.GetAt( al.FindIndex(i) );

			TLocalSavedArticleInfo info;
			g_spaceData.LoadMySavedArticle(info,item.dwSavedID);

			m_wndListArticles.AddItem(info.strTitle,info.strCreateDate,item.dwSavedID);
		}
	}
	else
	{
		int n = m_myArticles.vecArticles.size();

		for ( int i=0; i<n; i++ )
		{
			TWebArticleInfo *pArticle = m_myArticles.vecArticles[i];
			m_wndListArticles.AddItem(pArticle->szTitle,pArticle->szCreateDate,pArticle->dwArticleID);		
		}
	}

	m_wndListArticles.SetCurSel(0);
}

void COpenArticlesDlg::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_wndListBox.GetCurSel();
	if ( nSel==0 )
	{
		int nItem = m_wndListArticles.GetCurSel();
		if ( nItem!=-1 )
		{
			DWORD dwSavedID = m_wndListArticles.GetItemData(nItem);
			g_spaceData.DeleteSavedArticle(dwSavedID);
			m_wndListArticles.DeleteItem(nItem);
		}
	}
	else
	{
		int nItem = m_wndListArticles.GetCurSel();
		//if ( nItem!=-1 )
		//{
		//	DWORD dwArticleID = m_wndListArticles.GetItemData(nItem);
		//	CWebInterfaceWrapper obj;
		//	CString strError;
		//	obj.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());
		//	if ( obj.DeleteArticle(dwArticleID,strError) )
		//	{
		//	
		//		m_wndListArticles.DeleteItem(nItem);
		//	}
		//	else
		//	{
		//	}
		//}
	}
}

void COpenArticlesDlg::OnBnClickedButtonOk()
{
	// TODO: 在此添加控件通知处理程序代码
	int nSel = m_wndListBox.GetCurSel();

	if ( nSel==0 )
	{
		int nItem = m_wndListArticles.GetCurSel();
		if ( nItem!=-1 )
		{
			m_nArticleType  = 0;
			m_dwArticleID   = m_wndListArticles.GetItemData(nItem);
		}
	}
	else
	if ( nSel==1 )
	{
		int nItem = m_wndListArticles.GetCurSel();
		if ( nItem!=-1 )
		{
			m_nArticleType  = 1;
			m_dwArticleID   = m_wndListArticles.GetItemData(nItem);
		}
	}

	CDialog::OnOK();
}

void COpenArticlesDlg::OnBnClickedButtonPcancle()
{
	CDialog::OnCancel();
}

LRESULT COpenArticlesDlg::OnWebDataNotify(WPARAM wParam, LPARAM lParam)
{
	CWebDataProcess::tagThreadResult * result = (CWebDataProcess::tagThreadResult *)lParam;

	m_myArticles.Lock();

	if ( wParam == 1)//文章列表
	{
		vector<TWebArticleInfo *> info;
		CString strServerDate;
		ParaseWebArticles(result->strWebResult,info,strServerDate);

		strncpy(m_myArticles.szLastUpdateDate,(LPCSTR)strServerDate,sizeof(m_myArticles.szLastUpdateDate)-1);

		int n = info.size();

		for ( int i=0; i<n; i++ )
		{
			int index = m_myArticles.FindArticle(info[i]->dwArticleID);
			if ( index!=-1 )
			{
				delete m_myArticles.vecArticles[index];
				m_myArticles.vecArticles[index] = info[i];	
			}
			else
			{
				m_myArticles.vecArticles.push_back(info[i]);
			}
		}

		sort(m_myArticles.vecArticles.begin(),m_myArticles.vecArticles.end(),CompareArticleDate);
		g_spaceData.SaveUserWebArticles(m_myArticles);		

	}

	m_myArticles.Unlock();

	OnLbnSelchangeList1();

	m_btnRefresh.EnableWindow(TRUE);

	return 0;
}


void COpenArticlesDlg::OnBnClickedButtonRefresh()
{
	m_btnRefresh.EnableWindow(FALSE);
	g_webDataProcess.GetWebArticleList(UIntToStr(g_pUserInfo->GetUserID()),m_myArticles.szLastUpdateDate,"",this);   
}
