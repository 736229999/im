// im\MainToolBar.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\ImClient.h"
#include ".\maintoolbar.h"


// CMainToolBar �Ի���

IMPLEMENT_DYNAMIC(CMainToolBar, CDialog)
CMainToolBar::CMainToolBar(CWnd* pParent /*=NULL*/)
	: CDialog(CMainToolBar::IDD, pParent)
{
}

CMainToolBar::~CMainToolBar()
{
}

void CMainToolBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BTN_BIGHEAD,m_btnBigHead);
	DDX_Control(pDX,IDC_BTN_UPPICTURE,m_btnUploadPic);
	DDX_Control(pDX,IDC_BTN_WRITEARTICLE,m_btnWriteArticle);
	DDX_Control(pDX,IDC_BTN_MYPICTURE,m_btnMyPicture);
	DDX_Control(pDX,IDC_BTN_MYARTICLE,m_btnMyArticle);
	DDX_Control(pDX,IDC_BTN_MEDIA,m_btnMedia);
}


BEGIN_MESSAGE_MAP(CMainToolBar, CDialog)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CMainToolBar ��Ϣ�������

BOOL CMainToolBar::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc,IM_SPACE_COLOR);
	return FALSE;
}

BOOL CMainToolBar::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CMainToolBar::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnBigHead.SetBitmap(IDB_BTN_BIGHEAD,IDB_BTN_BIGHEAD,RGB(0,255,0));
	m_btnBigHead.SetToolTip("��ͷ��");

	m_btnUploadPic.SetBitmap(IDB_UPLOAD_PIC,IDB_UPLOAD_PIC,RGB(0,255,0));
	m_btnUploadPic.SetToolTip("�ϴ�ͼƬ");

	m_btnWriteArticle.SetBitmap(IDB_WRITEBLOG,IDB_WRITEBLOG,RGB(0,255,0));
	m_btnWriteArticle.SetToolTip("��������");

	m_btnMyPicture.SetBitmap(IDB_MY_PIC,IDB_MY_PIC,RGB(0,255,0));
	m_btnMyPicture.SetToolTip("�ҵ�ͼƬ");

	m_btnMyArticle.SetBitmap(IDB_MY_ARTICLE,IDB_MY_ARTICLE,RGB(0,255,0));
	m_btnMyArticle.SetToolTip("�ҵ�����");

	m_btnMedia.SetBitmap(IDB_BTN_MEDIA,IDB_BTN_MEDIA,RGB(0,255,0));
	m_btnMedia.SetToolTip("�ϴ����֡���Ƶ��������Ƶ����");

	return TRUE; 
}

LRESULT CMainToolBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
if ( message==WM_COMMAND )
	{
		switch(wParam)
		{
		case IDC_BTN_BIGHEAD:			
			g_pHeadPhoto->Show();
			break;
		case IDC_BTN_UPPICTURE:
			ShowUploadPicDlg((char*)g_ImUserInfo.GetUserName().c_str(),g_ImUserInfo.nickname,(char*)g_ImUserInfo.GetPassword().c_str(),g_ImUserInfo.GetId());	
			break;
		case IDC_BTN_WRITEARTICLE:
			g_pWriteArticle->Show();
			break;
		case IDC_BTN_MYPICTURE:
			g_pMyPictures->Show();
			break;
		case IDC_BTN_MYARTICLE:
			g_pMyArticles->Show();
			break;
		case IDC_BTN_MEDIA:
			{
				if(g_pWebConfig)
				{
					CString strUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(UPLOAD_MEDIA,StrToWideStr(g_ImUserInfo.GetUserName().c_str()));
					MiniOpenUrl(strUrl);
				}

			}
			break;
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}
