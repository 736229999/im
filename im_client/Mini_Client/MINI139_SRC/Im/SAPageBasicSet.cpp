// im\SAPageBasicSet.cpp : 实现文件
//

#include "stdafx.h"

#include "SAPageBasicSet.h"
#include "Localdata.h"



// CSAPageBasicSet 对话框

IMPLEMENT_DYNAMIC(CSAPageBasicSet, CSABasePage)
CSAPageBasicSet::CSAPageBasicSet(CWnd* pParent /*=NULL*/)
	: CSABasePage(CSAPageBasicSet::IDD, pParent)
{
	Create(CSAPageBasicSet::IDD,pParent);
}

CSAPageBasicSet::~CSAPageBasicSet()
{
}

void CSAPageBasicSet::DoDataExchange(CDataExchange* pDX)
{
	CSABasePage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_MAGIC, m_btnCheckMagic);
	DDX_Control(pDX, IDC_CHECK_MINI_PICBROADCAST,m_btnCheckMiniPictureBroadCast);
	//DDX_Control(pDX, IDC_CHECK_FRD_PIC, m_btnFriendPic);
	//DDX_Control(pDX, IDC_CHECK_FRD_ART, m_btnFriendArt);
}


BEGIN_MESSAGE_MAP(CSAPageBasicSet, CSABasePage)
END_MESSAGE_MAP()


// CSAPageBasicSet 消息处理程序

void CSAPageBasicSet::SaveConfig()
{

	BASIC_SYSTEM_SETTING *cfg;
	cfg=g_LocalData.GetBasicSystemSetting();

	cfg->bPlayMagicFace = m_btnCheckMagic.GetCheck();
	cfg->bShowMiniPictureBroadCast = m_btnCheckMiniPictureBroadCast.GetCheck();
	//cfg.bShowFriendPicWnd = m_btnFriendPic.GetCheck();
	//cfg.bShowFriendArtWnd = m_btnFriendArt.GetCheck();
}



BOOL CSAPageBasicSet::OnInitDialog()
{
	CSABasePage::OnInitDialog();

	BASIC_SYSTEM_SETTING *cfg;
	cfg=g_LocalData.GetBasicSystemSetting();
	m_btnCheckMagic.SetCheck(cfg->bPlayMagicFace);
	m_btnCheckMiniPictureBroadCast.SetCheck(cfg->bShowMiniPictureBroadCast);
	
	//m_btnFriendPic.SetCheck(cfg.bShowFriendPicWnd);
	//m_btnFriendArt.SetCheck(cfg.bShowFriendArtWnd);

	return TRUE;  
}
