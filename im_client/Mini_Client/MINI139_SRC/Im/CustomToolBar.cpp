// CustomToolBar.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "CustomToolBar.h"
#include ".\customtoolbar.h"
#include "Localdata.h"
#include "emotiondlg.h"
#include "linkresDlg.h"
#include "WebProcess.h"
#include "RichEditUtil.h"
#define TIMER_DOWNLOAD_FACE 0x1001

IMPLEMENT_DYNAMIC(CP2PTalkToolBar, CDialog)
CP2PTalkToolBar::CP2PTalkToolBar(CRichEditCtrlEx *pEditor, XT_FONT *pFont, CWnd* pParent /*=NULL*/)
	: CDialog(CP2PTalkToolBar::IDD, pParent)
{
	m_pFont = pFont;
	m_pRichEdit = pEditor;
	m_pRichEditDui = NULL;
}

CP2PTalkToolBar::CP2PTalkToolBar(DuiLib::CRichEditUI *pEditor, XT_FONT *pFont, CWnd* pParent /*=NULL*/)
	: CDialog(CP2PTalkToolBar::IDD, pParent)
{
	m_pFont = pFont;
	m_pRichEdit = NULL;
	m_pRichEditDui = pEditor;
}

CP2PTalkToolBar::~CP2PTalkToolBar()
{
}

void CP2PTalkToolBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_COMBO_FONTNAME,m_comboFontName);
	DDX_Control(pDX,IDC_COMBO_FONTSIZE,m_comboFontSize);
	DDX_Control(pDX,IDC_BUTTON_U, m_btnUnderline);
	DDX_Control(pDX,IDC_BUTTON_B, m_btnBold);
	DDX_Control(pDX,IDC_BUTTON_I, m_btnItalic);
//	DDX_Control(pDX,IDC_BUTTON_F, m_btnFace);
	//DDX_Control(pDX,IDC_BUTTON_PIC, m_btnPic);
	DDX_Control(pDX,IDC_BUTTON_C, m_btnColor);
}


BEGIN_MESSAGE_MAP(CP2PTalkToolBar, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_FONTNAME, OnCbnSelchangeComboFontname)
	ON_CBN_SELCHANGE(IDC_COMBO_FONTSIZE, OnCbnSelchangeComboFontsize)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_C,OnBnClickedBtnColor)
	ON_BN_CLICKED(IDC_BUTTON_I,OnFontItalicClicked)
	ON_BN_CLICKED(IDC_BUTTON_U,OnFontUnderLineClicked)
	ON_BN_CLICKED(IDC_BUTTON_B,OnFontBoldClicked)
END_MESSAGE_MAP()


// CP2PTalkToolBar 消息处理程序

BOOL CP2PTalkToolBar::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	vector<string> *pvecSysFontName = GetAllSysFontName();
	for(int i = 0; i != pvecSysFontName->size(); ++i)
	{
		m_comboFontName.AddString((*pvecSysFontName)[i].c_str());
	}

	int i,count;
	for (  i=8; i<25; i++ )
	{
		CString str;
		str.Format("%d",i);
		m_comboFontSize.AddString(str);
	}

	count = m_comboFontName.GetCount();

	m_comboFontName.SetCurSel(0);
	m_comboFontName.SetBorderColor(COMBOBOX_BORDER_NORMAL);

	for ( i=0; i<count; i++ )
	{
		CString strItem;
		m_comboFontName.GetLBText(i,strItem);

		if ( strItem==m_pFont->fontName )
		{
			m_comboFontName.SetCurSel(i);
			break;
		}
	}

	m_comboFontSize.SetCurSel(m_pFont->fontSize-8);
	m_comboFontSize.SetBorderColor(COMBOBOX_BORDER_NORMAL);

	m_btnColor.SetPic(IDB_PNG_FONT_COLOR_PRESSED, IDB_PNG_FONT_COLOR_HOVER, IDB_PNG_FONT_COLOR_PRESSED, IDB_PNG_FONT_COLOR_NORMAL, GetModuleHandle("Mini139_res.dll"));
	m_btnBold.SetPic(IDB_PNG_FONT_BOLD_PRESSED, IDB_PNG_FONT_BOLD_HOVER, IDB_PNG_FONT_BOLD_PRESSED, IDB_PNG_FONT_BOLD_NORMAL, GetModuleHandle("Mini139_res.dll"));
	m_btnUnderline.SetPic(IDB_PNG_FONT_UNDERLINE_PRESSED, IDB_PNG_FONT_UNDERLINE_HOVER, IDB_PNG_FONT_UNDERLINE_PRESSED, IDB_PNG_FONT_UNDERLINE_NORMAL, GetModuleHandle("Mini139_res.dll"));
	m_btnItalic.SetPic(IDB_PNG_FONT_ITALICS_PRESSED, IDB_PNG_FONT_ITALICS_HOVER, IDB_PNG_FONT_ITALICS_PRESSED, IDB_PNG_FONT_ITALICS_NORMAL, GetModuleHandle("Mini139_res.dll"));

	CRect rcBtn;
	m_btnColor.GetWindowRect(&rcBtn);
	ScreenToClient(&rcBtn);
	m_btnColor.MoveWindow(rcBtn.left, rcBtn.top, 22, 22);

	m_btnBold.GetWindowRect(&rcBtn);
	ScreenToClient(&rcBtn);
	m_btnBold.MoveWindow(rcBtn.left, rcBtn.top, 22, 22);

	m_btnUnderline.GetWindowRect(&rcBtn);
	ScreenToClient(&rcBtn);
	m_btnUnderline.MoveWindow(rcBtn.left, rcBtn.top, 22, 22);

	m_btnItalic.GetWindowRect(&rcBtn);
	ScreenToClient(&rcBtn);
	m_btnItalic.MoveWindow(rcBtn.left, rcBtn.top, 22, 22);

	m_btnColor.SetToolTip("颜色");

	m_comboFontName.SetFont(&g_font12);
	m_comboFontSize.SetFont(&g_font12);

	UpdateFontBtnTips();

	return TRUE; 
}

void CP2PTalkToolBar::SetRichEdit(DuiLib::CRichEditUI *pEditor)
{
	m_pRichEditDui = pEditor;
}

void CP2PTalkToolBar::SetRichEdit(CRichEditCtrlEx *pEditor)
{
	m_pRichEdit = pEditor;
}

void CP2PTalkToolBar::UpdateFontBtnTips()
{
	if ( m_pFont->flags & XTF_BOLD )
	{
		m_btnBold.SetToolTip("撤消粗体");
		m_btnBold.SetCheck(1);
	}
	else
	{
		m_btnBold.SetToolTip("字体加粗");
		m_btnBold.SetCheck(0);
	}

	if ( m_pFont->flags & XTF_ITALIC )
	{
		m_btnItalic.SetToolTip("撤消斜体");
		m_btnItalic.SetCheck(1);
	}
	else
	{
		m_btnItalic.SetToolTip("斜体");
		m_btnItalic.SetCheck(0);
	}

	if ( m_pFont->flags & XTF_UNDERLINE )
	{
		m_btnUnderline.SetToolTip("撤消下划线");
		m_btnUnderline.SetCheck(1);
	}
	else
	{
		m_btnUnderline.SetToolTip("下划线");
		m_btnUnderline.SetCheck(0);
	}
}

void CP2PTalkToolBar::OnCbnSelchangeComboFontname()
{
	m_comboFontName.GetLBText(
			m_comboFontName.GetCurSel(),
			m_pFont->fontName);

	//m_pEditor->SetXFont(*m_pFont);
	//m_pEditor->SetFocus();
	if(m_pRichEditDui != NULL)
	{
		//m_pRichEditDui->SetFont(m_pFont->fontName, m_pFont->fontSize, m_pFont->flags & XTF_BOLD, m_pFont->flags & XTF_UNDERLINE, m_pFont->flags & XTF_ITALIC);
		m_pRichEditDui->SetFocus();
		ITextServices* pTextServices = m_pRichEditDui->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_pFont->fontName, m_pFont->fontSize, m_pFont->fontColor, m_pFont->flags & XTF_BOLD, m_pFont->flags & XTF_ITALIC, m_pFont->flags & XTF_UNDERLINE, false);
	}
	else
	{
		m_pRichEdit->SetXFont(*m_pFont);
		m_pRichEdit->SetFocus();
	}
}

void CP2PTalkToolBar::OnCbnSelchangeComboFontsize()
{
	m_pFont->fontSize = m_comboFontSize.GetCurSel()+8;

	//m_pEditor->SetXFont(*m_pFont);
	//m_pEditor->SetFocus();
	if(m_pRichEditDui != NULL)
	{
		//m_pRichEditDui->SetFont(m_pFont->fontName, m_pFont->fontSize, m_pFont->flags & XTF_BOLD, m_pFont->flags & XTF_UNDERLINE, m_pFont->flags & XTF_ITALIC);
		m_pRichEditDui->SetFocus();
		ITextServices* pTextServices = m_pRichEditDui->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_pFont->fontName, m_pFont->fontSize, m_pFont->fontColor, m_pFont->flags & XTF_BOLD, m_pFont->flags & XTF_ITALIC, m_pFont->flags & XTF_UNDERLINE, false);
	}
	else
	{
		m_pRichEdit->SetXFont(*m_pFont);
		m_pRichEdit->SetFocus();
	}
}

BOOL CP2PTalkToolBar::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(&rc,CHAT_BOTTOM_COLOR);
	//CPen pen(PS_SOLID, 0, CHAT_BKG_COLOR);
	//CPen *pOldPen = pDC->SelectObject(&pen);
	//pDC->MoveTo(rc.left, rc.top);
	//pDC->LineTo(rc.right, rc.top);
	//pDC->SelectObject(pOldPen);

	return FALSE;
}

LRESULT CP2PTalkToolBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( message==WM_COMMAND )
	{
		::PostMessage(GetParent()->m_hWnd,
			message,wParam,lParam);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CP2PTalkToolBar::OnFontBoldClicked()
{
	DWORD dwOld = m_pFont->flags & XTF_BOLD;

	if ( dwOld )
	{
		m_pFont->flags &= ~XTF_BOLD;
	}
	else
	{
		m_pFont->flags |= XTF_BOLD;
	}

	//m_pEditor->SetXFont(*m_pFont);
	//m_pEditor->SetFocus();
	if(m_pRichEditDui != NULL)
	{
		//m_pRichEditDui->SetFont(m_pFont->fontName, m_pFont->fontSize, m_pFont->flags & XTF_BOLD, m_pFont->flags & XTF_UNDERLINE, m_pFont->flags & XTF_ITALIC);
		m_pRichEditDui->SetFocus();
		ITextServices* pTextServices = m_pRichEditDui->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_pFont->fontName, m_pFont->fontSize, m_pFont->fontColor, m_pFont->flags & XTF_BOLD, m_pFont->flags & XTF_ITALIC, m_pFont->flags & XTF_UNDERLINE, false);
	}
	else
	{
		m_pRichEdit->SetXFont(*m_pFont);
		m_pRichEdit->SetFocus();
	}

	UpdateFontBtnTips();
}

void CP2PTalkToolBar::OnFontItalicClicked()
{
	DWORD dwOld = m_pFont->flags & XTF_ITALIC;

	if ( dwOld )
	{
		m_pFont->flags &= ~XTF_ITALIC;
	}
	else
	{
		m_pFont->flags |= XTF_ITALIC;
	}

	//m_pEditor->SetXFont(*m_pFont);
	//m_pEditor->SetFocus();
	if(m_pRichEditDui != NULL)
	{
		//m_pRichEditDui->SetFont(m_pFont->fontName, m_pFont->fontSize, m_pFont->flags & XTF_BOLD, m_pFont->flags & XTF_UNDERLINE, m_pFont->flags & XTF_ITALIC);
		m_pRichEditDui->SetFocus();
		ITextServices* pTextServices = m_pRichEditDui->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_pFont->fontName, m_pFont->fontSize, m_pFont->fontColor, m_pFont->flags & XTF_BOLD, m_pFont->flags & XTF_ITALIC, m_pFont->flags & XTF_UNDERLINE, false);
	}
	else
	{
		m_pRichEdit->SetXFont(*m_pFont);
		m_pRichEdit->SetFocus();
	}

	UpdateFontBtnTips();
}


void CP2PTalkToolBar::OnFontUnderLineClicked()
{
	DWORD dwOld = m_pFont->flags & XTF_UNDERLINE;

	if ( dwOld )
	{
		m_pFont->flags &= ~XTF_UNDERLINE;
	}
	else
	{
		m_pFont->flags |= XTF_UNDERLINE;
	}

	//m_pEditor->SetXFont(*m_pFont);
	//m_pEditor->SetFocus();
	if(m_pRichEditDui != NULL)
	{
		//m_pRichEditDui->SetFont(m_pFont->fontName, m_pFont->fontSize, m_pFont->flags & XTF_BOLD, m_pFont->flags & XTF_UNDERLINE, m_pFont->flags & XTF_ITALIC);
		m_pRichEditDui->SetFocus();
		ITextServices* pTextServices = m_pRichEditDui->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_pFont->fontName, m_pFont->fontSize, m_pFont->fontColor, m_pFont->flags & XTF_BOLD, m_pFont->flags & XTF_ITALIC, m_pFont->flags & XTF_UNDERLINE, false);
	}
	else
	{
		m_pRichEdit->SetXFont(*m_pFont);
		m_pRichEdit->SetFocus();
	}

	UpdateFontBtnTips();
}

void CP2PTalkToolBar::OnBnClickedBtnColor()
{
	CColorDialog dlg(m_pFont->fontColor,0,GetParent());

	if ( dlg.DoModal()==IDOK )
	{
		m_pFont->fontColor = dlg.GetColor();
	}

	//m_pEditor->SetXFont(*m_pFont);
	//m_pEditor->SetFocus(); 
	if(m_pRichEditDui != NULL)
	{
		//m_pRichEditDui->SetFont(m_pFont->fontName, m_pFont->fontSize, m_pFont->flags & XTF_BOLD, m_pFont->flags & XTF_UNDERLINE, m_pFont->flags & XTF_ITALIC);
		m_pRichEditDui->SetFocus();
		ITextServices* pTextServices = m_pRichEditDui->GetTextServices();
		RichEdit_SetDefFont(pTextServices, m_pFont->fontName, m_pFont->fontSize, m_pFont->fontColor, m_pFont->flags & XTF_BOLD, m_pFont->flags & XTF_ITALIC, m_pFont->flags & XTF_UNDERLINE, false);
	}
	else
	{
		m_pRichEdit->SetXFont(*m_pFont);
		m_pRichEdit->SetFocus();
	}
	m_btnColor.SetCheck(0);
}
// D:\BlogClient_2\Im\CustomToolBar.cpp : 实现文件
//

// CP2PToolBar 对话框

IMPLEMENT_DYNAMIC(CP2PToolBar, CDialog)
CP2PToolBar::CP2PToolBar(CWnd* pParent /*=NULL*/)
	: CDialog(CP2PToolBar::IDD, pParent)
{
}

CP2PToolBar::~CP2PToolBar()
{
}

void CP2PToolBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BUTTON_FONT,m_btnFont);
	DDX_Control(pDX,IDC_BUTTON_FACE,m_btnFace);
	DDX_Control(pDX,IDC_BUTTON_PIC,m_btnPic);
	DDX_Control(pDX,IDC_BUTTON_SCRN,m_btnScreen);
	DDX_Control(pDX, IDC_BUTTON_SHAKE, m_btnShake);
	DDX_Control(pDX, IDC_BUTTON_SCORE, m_btnScore);
	DDX_Control(pDX, IDC_BUTTON_HISTORY, m_btnHistory);
	DDX_Control(pDX, IDC_BUTTON_FASTWORD, m_btnFastword);
}


BEGIN_MESSAGE_MAP(CP2PToolBar, CDialog)
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_FONT, OnBnClickedButtonFont)
	ON_BN_CLICKED(IDC_BUTTON_ACTION, OnBnClickedButtonAction)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LINK, OnBnClickedButtonLink)
	ON_WM_SIZE()
END_MESSAGE_MAP()

LRESULT CP2PToolBar::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( message==WM_COMMAND )
	{
		::PostMessage(GetParent()->m_hWnd,
			message,wParam,lParam);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

// CP2PToolBar 消息处理程序

BOOL CP2PToolBar::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnFont.SetBitmap(IDB_TALK_FONT_HOT,IDB_TALK_FONT_NORMAL,IDB_TALK_FONT_DOWN,IDB_TALK_FONT_DISABLE);
	m_btnFont.SetToolTip("设置字体颜色和格式");

	m_btnFace.SetBitmap(IDB_TALK_EMOTION_HOT,IDB_TALK_EMOTION_NORMAL,IDB_TALK_EMOTION_DOWN,IDB_TALK_EMOTION_DISABLE);
	m_btnFace.SetToolTip("表情");

	m_btnPic.SetBitmap(IDB_TALK_SENDPIC_HOT,IDB_TALK_SENDPIC_NORMAL,IDB_TALK_SENDPIC_DOWN,IDB_TALK_SENDPIC_DISABLE);
	m_btnPic.SetToolTip("发送图片");


	
	m_btnScreen.SetBitmap(IDB_PNG_SCREEN_NORM,IDB_PNG_SCREEN_PREESED,IDB_PNG_SCREEN_HOVER);
	m_btnScreen.SetBitmapSplit(IDB_PNG_SCREEN_OPTION_NORM,IDB_PNG_SCREEN_OPTION_PRESSED,IDB_PNG_SCREEN_OPTION_HOVER);
	m_btnScreen.SetTogether(true);
	m_btnScreen.SetToolTip("截图");

	m_btnShake.SetBitmap(IDB_BTN_VIBRATION_HOT,IDB_BTN_VIBRATION_NORMAL,IDB_BTN_VIBRATION_DOWN,IDB_BTN_VIBRATION_DISABLE);
	m_btnShake.SetToolTip("抖屏");


	m_btnScore.SetBitmap(IDB_BTN_SCORE_HOT,IDB_BTN_SCORE_NORMAL,IDB_BTN_SCORE_DOWN,IDB_BTN_SCORE_DISABLE);
#ifdef _BUSINESS_VESION
	m_btnScore.SetToolTip("推送服务评价表");
#else
	m_btnScore.ShowWindow(SW_HIDE);

	CRect rcBtn;
	m_btnScore.GetWindowRect(&rcBtn);
	m_btnFastword.MoveWindow(rcBtn);

#endif


	m_btnHistory.SetBitmap(IDB_BITMAP_HISTORY_RECORD,IDB_BITMAP_HISTORY_RECORD,RGB(0,255,0));

	//m_btnHistory.SetBitmap(IDB_BTN_HISTORY_NORMAL,IDB_BTN_HISTORY_NORMAL,RGB(0,255,0));
	m_btnHistory.SetFlatStyle(CFlatButton::FS_OFFICE, CFlatButton::AS_LEFT);
	m_btnHistory.SetToolTip("消息记录");
	
	m_btnFastword.SetPng(IDB_PNG_QUICK_HOVER, IDB_PNG_QUICK_NORM, IDB_PNG_QUICK_PRESSED, IDB_PNG_QUICK_NORM);
	m_btnFastword.SetToolTip("快捷短语");

	return TRUE;
}

BOOL CP2PToolBar::OnEraseBkgnd(CDC* pDC)
{
	//设置背景色为聊天框的颜色
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(&rc,RGB(255, 255, 255));
	//CPen pen(PS_SOLID, 0, RGB(222, 222, 222));
	//CPen *pOldPen = pDC->SelectObject(&pen);
	//pDC->MoveTo(rc.left, rc.top);
	//pDC->LineTo(rc.right, rc.top);
	//pDC->SelectObject(pOldPen);

/*
	CResBitmap bmpTemp;
	bmpTemp.LoadBitmap(IDB_P2PTOOLFONT_BACK);

	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pDC);

	CBitmap *pOldbmp = dcTemp.SelectObject(&bmpTemp);

	BITMAP bi;
	CRect  rc;

	bmpTemp.GetBitmap(&bi);
	GetClientRect(rc);
	pDC->StretchBlt(0,0,rc.Width(),rc.Height(),&dcTemp,0,0,bi.bmWidth,bi.bmHeight,SRCCOPY);

	dcTemp.SelectObject(pOldbmp);*/

	return FALSE;
}

void CP2PToolBar::OnBnClickedButtonFont()
{
	bool bDown=m_btnFont.IsDown();
	m_btnFont.SetDown(!bDown);
}



void CP2PToolBar::OnBnClickedButtonAction()
{
	//m_btnAction.SetDown(!m_btnAction.IsDown());
}

bool CP2PToolBar::StartDownloadFace()
{
	// 当前有下载线程
	if(m_download.IsDownloading())
	{
		return false;	
	}
	else
	{
		// 检测下载线程的进度
		SetTimer(TIMER_DOWNLOAD_FACE,CHECK_TIME,NULL);
		CString sCache=g_LocalData.GetImageFileCachePath();
		vector<string> sExistFiles;
		GetExistWebFiles(sExistFiles);
		// 开始下载
		if(g_pWebConfig)
		{
			m_download.DownloadFace(g_pWebConfig->GetWebPageUrl(MINI_WEB_FACE_PATH,L""),sCache,sExistFiles);
		}

		m_nCount=0;
		m_nTime=0;

		return true;
	}
}

void CP2PToolBar::DownloadFaceOK(bool bDeleteCanche)
{
	const CFaceFileList *  pFaceList=&(m_download.GetFaceList());
	if(pFaceList==NULL)
		return ;
	if(pFaceList->size()==0)
		return;
	// 首先获得自定义表情配置文件
	CString strCustomFacePath = g_LocalData.GetCustomFacePath();
	CString strCustomConfigFile = strCustomFacePath+"customface.cfg.xml";
	CStdioFile file;
	bool bOpenFlag;
	// 删除配置文件
	DeleteFile(strCustomConfigFile);
	// 重建配置文件
	bOpenFlag = file.Open(strCustomConfigFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText);
	CUSTOM_EMOTION	face;
	CString				strFaceFileName;
	CString				sFileID;
	int nS=pFaceList->size();
	FACE_INFO fi;
	int i;
	for( i = 0;i < nS; i++)
	{
		fi=(*pFaceList)[i];
		sFileID=fi.sFile.c_str();
		// 验证该表情是否下载过
		if(IsWebFaceExist(sFileID))
		{
			continue;
		};
		face.type=1;
		// 设置id名称
		strncpy(face.szID,sFileID,sizeof(face.szID)-1);
		// 设置快捷键
		strncpy(face.szShortCut,fi.sShortCut.c_str(),sizeof(face.szShortCut)-1);	
		strncpy(face.szTip,fi.sTip.c_str(),sizeof(face.szTip)-1);
		strncpy(face.szUrl,fi.sUrl.c_str(),sizeof(face.szUrl)-1);// 设置URL
		// 获得全名
		sFileID=pFaceList->GetFaceFullPath(i).c_str();
		g_LocalData.SaveImageFileToCustomFace(sFileID,strFaceFileName);
		// 删除缓存文件
		if(bDeleteCanche)
		{
			DeleteFile(sFileID);
		}
		// 设置文件名
		sprintf(face.szFile,strFaceFileName);
		// 添加到默认组中去
		g_customEmotionDB[0].push_back(face);
	}
	// 保存配置文件
	if ( bOpenFlag )
	{
		WriteCustomEmotionConfig(file);	
	}
	file.Close();
}

void CP2PToolBar::OnTimer(UINT nIDEvent)
{
	if(nIDEvent==TIMER_DOWNLOAD_FACE)
	{
		static int nState=0;
		nState++;
		nState%=5;	// 5帧动画
// 		switch(nState)
// 		{
// 		case 0:
// 			m_btnFace.SetBitmap(IDB_TALK_EMOTION,IDB_TALK_EMOTION);
// 			break;
// 		case 1:
// 			m_btnFace.SetBitmap(IDB_TALK_EMOTION1,IDB_TALK_EMOTION1);
// 			break;
// 		case 2:
// 			m_btnFace.SetBitmap(IDB_TALK_EMOTION2,IDB_TALK_EMOTION2);
// 			break;
// 		case 3:
// 			m_btnFace.SetBitmap(IDB_TALK_EMOTION3,IDB_TALK_EMOTION3);
// 			break;
// 		default:
// 			m_btnFace.SetBitmap(IDB_TALK_EMOTION4,IDB_TALK_EMOTION4);
// 			;
// 		}
		// 下载结束
		if(!m_download.IsDownloading())
		{
			static CString sErrMsg = "";
			m_nTime=0;
			m_nCount=0;
			KillTimer(TIMER_DOWNLOAD_FACE);
			//m_btnFace.SetBitmap(IDB_TALK_EMOTION,IDB_TALK_EMOTION);
			CWnd* pParent=GetParent();
			// 判断是否有错误
			if(m_download.HasDownloadErr())
			{
				// 下载表情文件失败
				// 保存已经下载的文件
				DownloadFaceOK(true);
				sErrMsg = m_download.GetLastErrMsg();
				pParent->SendMessage(WM_DOWNLOADFACE_OK,1,(LPARAM)(LPCTSTR)sErrMsg);
			}
			else
			{
				DownloadFaceOK(true);
				pParent->SendMessage(WM_DOWNLOADFACE_OK,0,(LPARAM)m_download.GetFaceList().size());
			}
		}
		// 正在下载
		else
		{
			if(m_nCount==m_download.GetFaceList().size())
			{
				// 判断当前下载文件是否超时
				m_nTime+=CHECK_TIME;
				if(m_nTime>=MAX_DOWNLOAD_TIME)
				{
					KillTimer(TIMER_DOWNLOAD_FACE);
					CWnd* pParent=GetParent();
					// 下载超时,停止下载
					m_download.StopDownload();
					// 保存已经下载的文件
					DownloadFaceOK(true);
					pParent->SendMessage(WM_DOWNLOADFACE_OK,1,(LPARAM)"下载表情文件超时");
				}
			}
			else
			{
				// 间隔一定时间才发送下载信息
				static int nCount=0;
				nCount++;
				nCount=nCount%10;	// 2秒
				if(nCount!=0)
					return;
				CWnd* pParent=GetParent();
				m_nCount=m_download.GetFaceList().size();
				m_nTime=0;
				int nTask=m_download.GetTask();
				int nProcess=m_download.GetProcess();
				if(nTask!=0)
				{
					pParent->SendMessage(WM_DOWNLOADPROCESS,(WPARAM)nTask,(LPARAM)nProcess);
				}
			}
		}
		
	}

	CDialog::OnTimer(nIDEvent);
}

BOOL CP2PTalkToolBar::PreTranslateMessage(MSG* pMsg)
{
	if  (  pMsg->message  ==  WM_KEYDOWN  )  
	{  
		switch(  pMsg->wParam  )  
		{  
		case 0x0D:
		case VK_ESCAPE:
			return TRUE;
		default:
			;
		}  
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CP2PToolBar::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类	
	if  (  pMsg->message  ==  WM_KEYDOWN  )  
	{  
		switch(  pMsg->wParam  )  
		{  
		case 0x0D:
		case VK_ESCAPE:
			return TRUE;
		default:
			;
		}  
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CP2PToolBar::OnBnClickedButtonLink()
{
	
}

void CP2PToolBar::StopDownloadFace()
{
	m_download.StopDownload();
	// 保存已经下载的表情
	DownloadFaceOK(true);
};

void CP2PToolBar::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	if (m_btnHistory.m_hWnd != NULL) 
	{
		CRect rcHistory;
		m_btnHistory.GetClientRect(rcHistory);
		rcHistory.left += cx - rcHistory.right;
		rcHistory.right = cx;
		rcHistory.top += cy - rcHistory.bottom - 2;
		rcHistory.bottom = cy - 2;
		m_btnHistory.MoveWindow(rcHistory);
	}
}

// 显示或隐藏评分按钮
void CP2PToolBar::ShowOrHideScoreBtn(BOOL bShow)
{
	if (IsWindow(GetSafeHwnd()))
	{
		m_btnScore.ShowWindow(bShow ? SW_SHOW : SW_HIDE);

		if(!bShow)
		{	// 隐藏时，移动快捷短语图标
			CRect rcBtn;
			m_btnScore.GetWindowRect(&rcBtn);
			ScreenToClient(&rcBtn);
			m_btnFastword.MoveWindow(&rcBtn);
		}
	}
}

void CP2PToolBar::HideScoreShakeBtn()
{
	if (IsWindow(GetSafeHwnd()))
	{
		CRect rcBtn;
		m_btnShake.GetWindowRect(&rcBtn);
		ScreenToClient(&rcBtn);
		m_btnFastword.MoveWindow(&rcBtn);
		m_btnShake.ShowWindow(SW_HIDE);
		m_btnScore.ShowWindow(SW_HIDE);
	}
}
