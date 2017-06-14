#include "stdafx.h"
#include "GroupSendDlg.h"
#include "GroupSendAddOtherDlg.h"
#include "im\WebProcess.h"
#include "im\EmotionDlg.h"
#include "im\LocalData.h"
#include "im\ImageFileDialog.h"
#include "im\MessageHandleWnd.h"
#include "im\ScreenCtrl.h"
#include "im\ImageFileDialog.h"
#include "im\impublic.h"
#include "..\HmgSkin\HmgSkin\FlatPopupMenu.h"
#include "MainFrame.h"
#include "DuiMfRecentList.h"
#include "im\xtpacket.h"
#ifdef _CS_PLAT_EX_
#include "CSPlatDlg.h"
#endif

extern CWebUserHeadIconProcess g_webUserHeadIconProcess;
extern CMessageHandleWnd *g_pMessageHandleWnd;
extern string CutRoundImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE);
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
// 上传图片到图片空间
extern BOOL upLoadPic(const char* szPic, string &strURL);

BEGIN_XT_MESSAGE_MAP(CGroupSendDlg)
END_XT_MESSAGE_MAP()

CGroupSendDlg::CGroupSendDlg()
	: m_wndTalkToolBar(&m_reditSend, &g_ImFont, CWnd::FromHandle(GetHWND()))
{
	
}

CGroupSendDlg::~CGroupSendDlg()
{
}

std::tr1::shared_ptr<CGroupSendDlg> CGroupSendDlg::pInstance = NULL;
std::tr1::shared_ptr<CGroupSendDlg> CGroupSendDlg::Show(vector<int> &vecFriendID)
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
		pInstance->UpdateSel(vecFriendID);
	}
	else
	{
		pInstance = std::tr1::shared_ptr<CGroupSendDlg>(new CGroupSendDlg());
		pInstance->Create(GetDesktopWindow(), _T("群发即时消息"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
		pInstance->UpdateSel(vecFriendID);
	}

	return pInstance;
}

void CGroupSendDlg::CloseDlg()
{
	CGroupSendAddOtherDlg::CloseDlg();
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		pInstance->Close(IDOK);
	}
}

DuiLib::UILIB_RESOURCETYPE CGroupSendDlg::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LRESULT CGroupSendDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(uMsg == WM_OK)
	{
		UpdateSel(CGroupSendAddOtherDlg::g_SelFriendID);
	}
	else if (uMsg == WM_MENUCLICK)  
	{ 
		DuiLib::CDuiString *strMenuName = (DuiLib::CDuiString*)wParam;
		if(*strMenuName == "MenuDel")
		{
			vector<int> vecSelFriendID;
			GetSelID(vecSelFriendID);
			for(int i = 0; i < (int)m_vecLbSelFriend.size(); ++i)
			{
				if(m_vecLbSelFriend[i]->IsSelected())
				{
					int nID = m_vecLbSelFriend[i]->GetTag();
					vector<int>::iterator it = std::find(vecSelFriendID.begin(), vecSelFriendID.end(), nID);
					if(it != vecSelFriendID.end())
					{
						vecSelFriendID.erase(it);
					}

					// 好友列表取消选中
					std::tr1::shared_ptr<CGroupSendAddOtherDlg> pIn = CGroupSendAddOtherDlg::GetInstance();
					if(pIn != NULL && pIn->GetHWND() != NULL)
					{
						pIn->CheckFriend(nID, FALSE);
					}
					break;
				}
			}
			UpdateSel(vecSelFriendID);
		}
	}
	else if(WM_KEYDOWN == uMsg)
	{
		switch(  wParam  )  
		{  
		case 0x0D:
			{
				HOTKEY_CONFIG *cfg=g_LocalData.GetHotKeyConfig();

				if ( cfg->sendMsgKey==SMK_ENTER  )
				{
					if ( !KEYDOWN(VK_CONTROL) )
						OnBnClickedBtnSend();
					else
					{
						if (m_reditSend.IsFocused() )
							m_reditSend.InsertText("\n");
					}
				}
				else if(cfg->sendMsgKey==SMK_CTRLENTER )
				{
					if  ( KEYDOWN(VK_CONTROL) )
						OnBnClickedBtnSend();
					else
					{
						if (m_reditSend.IsFocused() )
							m_reditSend.InsertText("\n");
					}
				}
			}
			return TRUE;
			break;
		default:
			break;
		}  
	}
	else if(WM_EMOTIONSEL_NOTIFY == uMsg)
	{
		OnEmotionSelNotify(wParam, lParam);
	}
	else if(WM_MOUSEMOVE == uMsg)
	{
		POINT pt = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		DuiLib::CButtonUI* pBtnScreenTool = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentool")));
		CRect rcTool = pBtnScreenTool->GetPos();
		DuiLib::CButtonUI* pBtnScreenToolConfig = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentoolconfig")));
		CRect rcConfig = pBtnScreenToolConfig->GetPos();
		if(rcTool.PtInRect(pt))
		{
			DuiLib::CButtonUI* pBtnScreenToolConfig = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentoolconfig")));
			pBtnScreenToolConfig->SetNormalImage("P2PSendMsgDlgTool\\screenshots_choose_hover.png");
		}
		else if(rcConfig.PtInRect(pt))
		{
			DuiLib::CButtonUI* pBtnScreenTool = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentool")));
			pBtnScreenTool->SetNormalImage("P2PSendMsgDlgTool\\screenshots_hover.png");
		}
		else
		{
			DuiLib::CButtonUI* pBtnScreenToolConfig = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentoolconfig")));
			pBtnScreenToolConfig->SetNormalImage("P2PSendMsgDlgTool\\screenshots_choose_normal.png");

			DuiLib::CButtonUI* pBtnScreenTool = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentool")));
			pBtnScreenTool->SetNormalImage("P2PSendMsgDlgTool\\screenshots_normal.png");
		}
	}
	else if(WM_XT_CMD_NOTIFY == uMsg)
	{
		OnXtMessage(wParam, lParam);
	}
	else if(WM_KEYDOWN_RETURN == uMsg)
	{
		return 2;
	}
	else if(WM_SNAPSHOT == uMsg)
	{
		OnScreenCopy();
	}
	else if(WM_SYSCOMMAND)
	{
		if(wParam == SC_MINIMIZE)
		{
			if(CGroupSendAddOtherDlg::GetInstance() != NULL && CGroupSendAddOtherDlg::GetInstance()->GetHWND() != NULL)
			{
				CGroupSendAddOtherDlg::GetInstance()->SendMessage(WM_SYSCOMMAND,SC_MINIMIZE,NULL);
			}
		}
		else if(wParam == SC_RESTORE)
		{
			if(CGroupSendAddOtherDlg::GetInstance() != NULL && CGroupSendAddOtherDlg::GetInstance()->GetHWND() != NULL)
			{
				CGroupSendAddOtherDlg::GetInstance()->SendMessage(WM_SYSCOMMAND,SC_RESTORE,NULL);
			}
		}
	}
	return DuiLib::WindowImplBase::HandleMessage(uMsg, wParam, lParam);
}

LPCTSTR CGroupSendDlg::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CGroupSendDlg::InitWindow()
{
	DuiLib::WindowImplBase::InitWindow();
	SetIcon(IDR_MAINFRAME);

	m_reditSend.SetEmotionImageList(&g_imgEmotionList);
	m_reditSend.SetXFont(g_ImFont);
	m_reditSend.SetStartIndent(80);
	m_reditSend.SetFocus();
	m_reditSend.DragAcceptFiles(TRUE);

	UpdateBk();
}

DuiLib::CControlUI* CGroupSendDlg::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("RichEditSendWnd")) == 0)
	{
		CWndUI  *pUI  = new CWndUI;      
		RECT rc;
		rc.left = 0;
		rc.right = 0;
		rc.top = 0;
		rc.bottom = 0;
		m_reditSend.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | ES_WANTRETURN
			| WS_VSCROLL | ES_AUTOVSCROLL, rc, CWnd::FromHandle(GetHWND()), 10002);
		m_reditSend.SetLimitWordSize(400);//发送框限制输入400个字
		pUI->Attach(m_reditSend.GetSafeHwnd());         
		return pUI;
	}

	if (_tcsicmp(pstrClass, _T("TalkBarWnd")) == 0)
	{
		CWndUI  *pUI  = new CWndUI;      
		RECT rc;
		rc.left = 0;
		rc.right = 0;
		rc.top = 0;
		rc.bottom = 0;

		m_wndTalkToolBar.Create(CP2PTalkToolBar::IDD, CWnd::FromHandle(GetHWND()));
		m_wndTalkToolBar.MoveWindow(&rc);
		pUI->Attach(m_wndTalkToolBar.GetSafeHwnd());           
		return pUI;
	}
	return NULL;
}

void CGroupSendDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if(msg.sType == "click")
	{
		if(msg.pSender->GetName() == "btnAddOther")
		{
			// 计算添加好友窗口弹出的位置
			int x, y; 
			x = GetSystemMetrics(SM_CXSCREEN); //屏幕宽度 
			y = GetSystemMetrics(SM_CYSCREEN); //屏幕高度  

			CRect rc;
			::GetWindowRect(GetHWND(), &rc);

			CPoint pt;
			if(rc.right + 230 + 2< x)
			{
				pt.x = rc.right + 2;
				pt.y = rc.top;
			}
			else if(rc.left > 230 + 2)
			{
				pt.x = rc.left - 230 - 2;
				pt.y = rc.top;
			}
			else
			{
				pt.x = rc.right - 230;
				pt.y = rc.top;
			}

			vector<int> vecSelFriendID;
			GetSelID(vecSelFriendID);
			CGroupSendAddOtherDlg::Show(GetHWND(), vecSelFriendID, pt);
		}
		else if(msg.pSender->GetName() == _T("btnfont")) 
		{
			OnBnClickedFontBar();
		}
		else if(msg.pSender->GetName() == _T("btnemotion")) 
		{
			OnFaceIconClicked();
		}	
		else if(msg.pSender->GetName() == _T("btnsendPic"))
		{
			OnSendPicClicked();
		}
		else if(msg.pSender->GetName() == _T("btnscreentool"))
		{
			OnScreenCopy();
		}
		else if(msg.pSender->GetName() == _T("btnscreentoolconfig"))
		{
			OnScreenCopyOption();
		}
		else if(msg.pSender->GetName() == _T("btnsend")) 
		{
			OnBnClickedBtnSend();
		}
		else if(msg.pSender->GetName() == _T("btnsendconfig")) 
		{
			OnBnClickedBtnHotkey();
		}
		else if(msg.pSender->GetName() == _T("closebtn"))
		{	
			if(CGroupSendAddOtherDlg::GetInstance() != NULL && CGroupSendAddOtherDlg::GetInstance()->GetHWND() != NULL)
				CGroupSendAddOtherDlg::GetInstance()->Close();
		}
	}
	else if(msg.sType == DUI_MSGTYPE_RBUTTONDOWN)
	{
		vector<DuiLib::COptionUI*>::iterator it = std::find(m_vecLbSelFriend.begin(), m_vecLbSelFriend.end(), msg.pSender);
		if(it != m_vecLbSelFriend.end())
		{	// 弹出右键菜单
			(*it)->Selected(true);
			
			DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
			CPoint point;
			GetCursorPos(&point);
			DuiLib::CPoint duiPoint(point.x,point.y);
			pMenu->Init(NULL, _T("MenuDel.xml"), point, &m_PaintManager);
		}
	}
}

void CGroupSendDlg::UpdateBk()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			::UpdateBk(background, pInstance->GetHWND());
		}
	}
}

// 刷新好友选择区
void CGroupSendDlg::UpdateSel(vector<int> &vecFriendID)
{
	// 清空原有控件
	DuiLib::CVerticalLayoutUI *pVertFriendArea = static_cast<DuiLib::CVerticalLayoutUI*>(m_PaintManager.FindControl("vlSel"));
	pVertFriendArea->RemoveAll();
	m_vecLbSelFriend.clear();
	CRect rcVert = pVertFriendArea->GetPos();
	int nVertWidth = 470;

	// 添加控件
	DuiLib::CHorizontalLayoutUI *pHori = new DuiLib::CHorizontalLayoutUI();
	pHori->SetFixedHeight(30);
	pVertFriendArea->Add(pHori);
	for(int i = 0; i < (int)vecFriendID.size(); ++i)
	{
		FRIEND_INFO *pFriend = FindFriend(vecFriendID[i]);
		if(pFriend != NULL)
		{
			DuiLib::COptionUI *pLb = new DuiLib::COptionUI();
			pLb->SetTag(pFriend->GetId());
			pHori->Add(pLb);
			pLb->SetFixedHeight(30);
			pLb->SetTag((DWORD)vecFriendID[i]);
			pLb->SetNormalImage("SysBtn\\white.png");
			pLb->SetSelectedImage("SysBtn\\grayEA.png");
			pLb->SetGroup("gs");

			CString strFile = g_webUserHeadIconProcess.GetFaceFile(pFriend);
			if ( FileExist(strFile) )
			{
				BOOL bOnline = TRUE;
				if(pFriend->GetId() == g_ImUserInfo.GetId())
				{
					bOnline = TRUE;
				}
				else if(pFriend->status == XTREAM_OFFLINE || pFriend->status == XTREAM_HIDE)
				{
					bOnline = FALSE;
				}
				string strHeadIcon = CutRoundImg(strFile, 26, 26, !bOnline);
				CString strBk;
				strBk.Format("file='%s' dest='2,2,28,28'", strHeadIcon.c_str());
				pLb->SetForeImage(strBk);
				pLb->SetSelectedForedImage(strBk);
			}

			CString strName;
			strName.Format("%s;", UserInfoToStr(pFriend));
			pLb->SetText(strName);
			pLb->SetAttribute("align", "left,vcenter");
			pLb->SetAttribute("textpadding", "30,0,0,0");
			int nWidth = pLb->CalcRealWidth();
			pLb->SetFixedWidth(nWidth);

			if(m_vecLbSelFriend.size() == 0)
			{
				CRect rc;
				rc.left = 0;
				rc.top = 0; 
				rc.right = nWidth; 
				rc.bottom = 30;
				pLb->SetPos(rc);
			}
			else
			{
				CRect rcLast= m_vecLbSelFriend[m_vecLbSelFriend.size()-1]->GetPos();
				if(nVertWidth - rcLast.right >= pLb->GetFixedWidth())
				{	// 能够容下
					CRect rc;
					rc.left = rcLast.right; 
					rc.top = rcLast.top; 
					rc.right = rcLast.right+pLb->GetFixedWidth(); 
					rc.bottom = rcLast.top+pLb->GetFixedHeight();
					pLb->SetPos(rc);
				}
				else
				{	// 容不下，另起一行
					pHori->SetAutoDestroy(false);
					pHori->Remove(pLb);
					pHori->SetAutoDestroy(true);

					pHori = new DuiLib::CHorizontalLayoutUI();
					pHori->SetFixedHeight(30);
					pVertFriendArea->Add(pHori);
					pHori->Add(pLb);

					CRect rc;
					rc.left = 0; 
					rc.top = rcLast.bottom; 
					rc.right = pLb->GetFixedWidth(); 
					rc.bottom = rcLast.bottom+pLb->GetFixedHeight();
					pLb->SetPos(rc);
				}
			}

			m_vecLbSelFriend.push_back(pLb);
		}
	}
}

// 获取选择好友的ID
void CGroupSendDlg::GetSelID(vector<int> &vecSelFriendID)
{
	vecSelFriendID.clear();
	for(int i = 0; i < (int)m_vecLbSelFriend.size(); ++i)
	{
		vecSelFriendID.push_back(m_vecLbSelFriend[i]->GetTag());
	}
}

void CGroupSendDlg::OnBnClickedFontBar()
{
	DuiLib::CHorizontalLayoutUI* pHlTalkBar = static_cast<DuiLib::CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("hltalkbar")));
	if(pHlTalkBar->IsVisible())
	{
		pHlTalkBar->SetVisible(false);
	}
	else
	{
		pHlTalkBar->SetVisible(true);
	}
}

void CGroupSendDlg::OnFaceIconClicked()
{
	DuiLib::CPoint pt;
	GetCursorPos(&pt);

	CEmotionDlg *dlg = new CEmotionDlg(pt.x,pt.y,&g_imgEmotionList,CWnd::FromHandle(GetHWND()));
	dlg->m_hEmotionSelWnd = m_hWnd;
	dlg->ShowWindow(SW_SHOW);	
}

void CGroupSendDlg::OnSendPicClicked()
{
	CImageFileDialog dlg("","",OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,CWnd::FromHandle(GetHWND()));

	if ( IDOK==dlg.DoModal() )
	{
		if ( dlg.GetSelFileSize()<1024 )
		{
			CString strCacheFileName;
			g_LocalData.SaveImageFileToCache(dlg.GetPathName(),strCacheFileName);
			CString strCacheImageFullPathName=g_LocalData.GetImageFileCachePath()+strCacheFileName;

			//m_reditSend.InsertImage(strCacheImageFullPathName,EMOTION_PIC_DATA());
			m_reditSend.InsertImage((LPCTSTR)strCacheImageFullPathName);
			m_reditSend.SetFocus();
		}
		else
		{
			ShowMessage("对不起，您只能发送小于1M的图片！",MB_ICONINFORMATION|MB_OK);
		}
	}
}

void CGroupSendDlg::OnScreenCopy()
{	

	HOTKEY_CONFIG *cfg;
	cfg = g_LocalData.GetHotKeyConfig();

	if(cfg->bScreenCopyHideWindow)
	{
		this->ShowWindow(SW_HIDE);
	}

	if(g_pMessageHandleWnd->GetScreenCtrl() == NULL)
	{
		g_pMessageHandleWnd->NewScreenCtrl();
		if(IDOK == g_pMessageHandleWnd->GetScreenCtrl()->DoModal())
		{
			CString strCacheFileName;
			g_LocalData.SaveImageFileToCache(g_pMessageHandleWnd->GetScreenCtrl()->m_strImageFile,strCacheFileName);
			CString strCacheImageFullPathName=g_LocalData.GetImageFileCachePath()+strCacheFileName;

			DeleteFile(g_pMessageHandleWnd->GetScreenCtrl()->m_strImageFile);

			//m_reditSend.InsertImage(strCacheImageFullPathName,EMOTION_PIC_DATA());
			m_reditSend.InsertImage((LPCTSTR)strCacheImageFullPathName);

			g_pMessageHandleWnd->DeleteScreenCtrl();
		}
		else
		{
			g_pMessageHandleWnd->DeleteScreenCtrl();
		}
	}

	if(cfg->bScreenCopyHideWindow)
	{
		this->ShowWindow(SW_SHOW);
	}
}

void CGroupSendDlg::OnScreenCopyOption()
{
	DuiLib::CButtonUI* pBtnScreenCopyConfig = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl(_T("btnscreentoolconfig")));
	RECT rc = pBtnScreenCopyConfig->GetPos();
	RECT rcParent;
	::GetWindowRect(*this, &rcParent);
	rc.left += rcParent.left;
	rc.top += rcParent.top;
	rc.right += rcParent.left;
	rc.bottom += rcParent.top;

	DuiLib::CPoint p;
	int	id;

	// create it
	CFlatPopupMenu menu;
	HMODULE hModule = GetModuleHandle("Mini139_res.dll"); 
	menu.AddImg(hModule, IDB_PNG_CHOOSE, "PNG");
	menu.Create(AfxGetInstanceHandle());

	// add some items
	HOTKEY_CONFIG *cfg;
	cfg = g_LocalData.GetHotKeyConfig();

	if(cfg->bScreenCopyHideWindow == SMK_ENTER)
	{
		menu.AppendItem(0,"截屏时显示当前窗口",1,0);
		menu.AppendItem(0,"截屏时隐藏当前窗口",2);
	}
	else
	{
		menu.AppendItem(0,"截屏时显示当前窗口",1);
		menu.AppendItem(0,"截屏时隐藏当前窗口",2,0);
	}

	// track it
	p.x = rc.right - 24;
	p.y = rc.bottom + 2;

	id=menu.Track(p.x,p.y,*this,true);


	cfg = g_LocalData.GetHotKeyConfig();
	switch(id)
	{
	case 1:
		cfg->bScreenCopyHideWindow = false;
		break;
	case 2:
		cfg->bScreenCopyHideWindow = true;
		break;
	default:
		break;
	}
}

LRESULT CGroupSendDlg::OnEmotionSelNotify(WPARAM wParam,LPARAM lParam)
{
	CUSTOM_EMOTION * pFace = (CUSTOM_EMOTION * )wParam;
	m_reditSend.InsertFace(pFace,(int)lParam);
	m_reditSend.SetFocus();
	return 1;
}

void CGroupSendDlg::OnBnClickedBtnSend()
{
	DuiLib::COptionUI* pCheckSendOnline = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl(_T("checkSendOnline")));
	BOOL bSendOnline = pCheckSendOnline->IsSelected();

	CString text;
	IMAGE_FILE_LIST il;
	IMAGE_FILE_LIST::iterator it;

	m_reditSend.GetText(text,&il);
	m_reditSend.ClearEmotionPicData();

	if ( text.IsEmpty() )
	{
		CMessageBox::Prompt("消息不能为空", IM_CAPTION, MB_OK, GetHWND(), TRUE);
		return ;
	}

	if ( text.GetLength()>800 )
	{
		CMessageBox::Prompt("你发送消息内容超长，请分条发送。", IM_CAPTION, MB_OK, GetHWND(), TRUE);
		return ;
	}

	if ( FindForbiddenWords(text) )
	{
		m_reditSend.ClearMsg();
		m_reditSend.SetFocus();
		CMessageBox::Prompt("你发送的消息中有限制用语，消息发送失败！", IM_CAPTION, MB_OK, GetHWND(), TRUE);
		return ;
	}

	if ( text.GetLength()==0 )
	{
		m_reditSend.SetFocus();
		return ;
	}

	vector<int> vecID;
	GetSelID(vecID);
	if(vecID.empty())
	{
		CMessageBox::Prompt("请选择接收消息的好友", IM_CAPTION, MB_OK, GetHWND(), TRUE);
		return;
	}

	text = "【群发】 "  + text;

	if(il.size() != 0)
	{
		CString strData = text;
		for ( it=il.begin(); it!=il.end(); it++ )
		{
			CString strPicFile = g_LocalData.GetImageFileCachePath() + it->file_name;
			string strUrl;
			if(upLoadPic(strPicFile, strUrl))
			{	// 上传成功，替换url
				strData.Replace(it->file_name, strUrl.c_str());	
			}
			else
			{	// 上传失败，替换为错误图片
				strData.Replace(it->file_name, "noimage");					
			}
		}
		if(strData.GetLength() > 800)
		{
			CMessageBox::Prompt("你发送消息内容超长，请分条发送。", IM_CAPTION, MB_OK, GetHWND(), TRUE);
			return;
		}

		text = strData;
	}

	for(int i = 0; i < (int)vecID.size(); ++i)
	{
		SendMsg(vecID[i], text);
	}

	m_reditSend.ClearMsg();
	m_reditSend.SetFocus();

	CMessageBox::Prompt("消息已经发送成功", "发送成功", MB_OK, GetHWND(), TRUE);
}

void CGroupSendDlg::SF_InitXtMsg(XT_MSG &msg, int nFriendID)
{
	memset(&msg,0,sizeof(msg));

	msg.ver         = XTREAM_CLIENT_VERSION;
	msg.from_id     = g_ImUserInfo.GetId();

	msg.fontColor   = g_ImFont.fontColor;
	msg.fontSize    = g_ImFont.fontSize;
	msg.fontStyle   = g_ImFont.flags;
	strcpy(msg.fontName,g_ImFont.fontName);
	strcpy(msg.from_nickname,g_ImUserInfo.nickname);

	msg.send_time   = g_LocalData.GetTimeNow();

	FRIEND_INFO *pFriend = FindFriend(nFriendID);
	if(pFriend != NULL)
	{
		msg.to_id		= pFriend->GetId();
		msg.dest_ip		= pFriend->ip;
		msg.dest_port	= pFriend->port;
		msg.msg_type    = 0;		
	}
}

void CGroupSendDlg::OnBnClickedBtnHotkey()
{
	DuiLib::CButtonUI* pBtnSendConfig = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl(_T("btnsendconfig")));
	RECT rc = pBtnSendConfig->GetPos();
	RECT rcParent;
	::GetWindowRect(*this, &rcParent);
	rc.left += rcParent.left;
	rc.top += rcParent.top;
	rc.right += rcParent.left;
	rc.bottom += rcParent.top;

	DuiLib::CPoint p;
	int	id;

	// create it
	CFlatPopupMenu menu;
	HMODULE hModule = GetModuleHandle("Mini139_res.dll"); 
	menu.AddImg(hModule, IDB_PNG_CHOOSE, "PNG");
	menu.Create(AfxGetInstanceHandle());

	// add some items
	HOTKEY_CONFIG *cfg;
	cfg=g_LocalData.GetHotKeyConfig();

	if(cfg->sendMsgKey == SMK_ENTER)
	{
		menu.AppendItem(0,"按Enter键发送消息",1,0);
		menu.AppendItem(0,"按Ctrl+Enter键发送消息",2);
	}
	else
	{
		menu.AppendItem(0,"按Enter键发送消息",1);
		menu.AppendItem(0,"按Ctrl+Enter键发送消息",2,0);
	}

	// track it

	p.x=rc.right - 24;
	p.y=rc.bottom+2;

	id=menu.Track(p.x,p.y,*this,true);

	// 选择结果
	switch (id)
	{
	case 1:
		{
			OnEnter();
		}
		break;
	case 2:
		{
			OnCtrlenter();
		}
		break;
	default:
		break;
	}
}

void CGroupSendDlg::OnEnter()
{
	// TODO: 在此添加命令处理程序代码
	HOTKEY_CONFIG *cfg;
	cfg=g_LocalData.GetHotKeyConfig();
	cfg->sendMsgKey=SMK_ENTER;
}


void CGroupSendDlg::OnCtrlenter()
{
	// TODO: 在此添加命令处理程序代码
	HOTKEY_CONFIG *cfg;
	cfg=g_LocalData.GetHotKeyConfig();
	cfg->sendMsgKey=SMK_CTRLENTER;
}

void CGroupSendDlg::RefreshFriendStatus(int nFriendID)
{
	XT_STATUS_GET_REQ req;
	req.fid = nFriendID;
	req.id = g_ImUserInfo.GetId();
	g_pProtocol->SendReq(req);
}

void CGroupSendDlg::SF_RecvPfileAck(const XT_MSG &msg)
{
	string strMsgData = "";
	CRecvPacket rp((char *)msg.data,sizeof(msg.data));
	uint32 file_id;
	uint8  ret;
	char szUrl[1024] = {0};		// 当此张图片已经传输过时，会收到图片的url，未传输过时，url为空
	rp>>ret>>file_id>>szUrl;

	if ( ret ) 
	{
		if(ret == 2)
		{
			TRACE("接收方已经有该图片，放弃接收该图片\n");
			// 判断图片是否已传输过
			SEND_FILE_STRUCT_NEW *pSend = SF_FindSendFile(file_id);
			if(pSend != NULL && strcmp(szUrl, "") != 0)
			{
				pSend->ReplaceUrl(szUrl);
				if(pSend->IsReplaceAll())
				{
					strMsgData = pSend->pMsg->data;
					SF_RemoveSendFile(file_id);
				}
			}
		}
		else if(ret == 1)
			TRACE("接收方在初始化图片文件时发生错误,放弃接受图片\n");
		SF_RemoveSendFile(file_id);
	}
	else
	{
		XT_MSG file_packet;
		SF_InitXtMsg(file_packet, msg.from_id);
	
		file_packet.data_type   = 3;//

		SEND_FILE_STRUCT_NEW *pSend = SF_FindSendFile(file_id);

		if ( pSend && pSend->file_id==file_id && pSend->fileObject==NULL )
		{
			static char buffer[FILE_PACKET_LEN];

			CString strSendFile ;
			strSendFile.Format("%s%s",g_LocalData.GetImageFileCachePath(),pSend->file_name);

			pSend->fileObject=new CStdioFile();

			int nRead  = 0;
			int nCount = pSend->window_size;

			if ( pSend->fileObject->Open(strSendFile,CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone) )
			{
				while( (nRead=pSend->fileObject->Read(buffer,sizeof(buffer)))>0 )
				{					
					CSendPacket spMsg(file_packet.data);

					spMsg<<file_id<<pSend->last_packet_id<<pSend->last_file_pointer;
					spMsg.WriteData(buffer,nRead);

					pSend->last_packet_id++;
					pSend->last_file_pointer+=nRead ;
					pSend->send_size+=nRead;
					file_packet.data_len=spMsg.GetSize();
					file_packet.recv_flag = m_nType;

					file_packet.recv_flag = m_nType;
					g_pProtocol->SendP2PMsg(file_packet,m_nType);

					nCount--;

					if ( nCount==0 )
					{
						break;
					}
				}
			}
		}
	}

	if(!strMsgData.empty())
	{
		vector<int> vecID;
		GetSelID(vecID);

		for(int i = 0; i < (int)vecID.size(); ++i)
		{
			SendMsg(vecID[i], strMsgData.c_str());
		}
	}
}

void CGroupSendDlg::SF_RecvPfilePacketAck(const XT_MSG &msg)
{
	string strMsgData = "";

	CRecvPacket rp((char *)msg.data,sizeof(msg.data));
	uint32  file_id;
	rp>>file_id;
	SEND_FILE_STRUCT_NEW *pSend=SF_FindSendFile(file_id);

	if ( pSend && pSend->fileObject )
	{
		char buffer[FILE_PACKET_LEN];

		pSend->ack_count++;

		DWORD dwTick = GetTickCount();

		int nPackets=0;

		if ( pSend->last_ack_tick == 0 )
		{
			nPackets = 1;
		}
		else
		{
			if ( (dwTick-pSend->last_ack_tick)>1000 )
			{
				pSend->window_size--;
				pSend->ack_count=0;
				nPackets=0;
			}
			else
			{
				if ( pSend->ack_count>=pSend->window_size )
				{
					nPackets=5;
					pSend->window_size+=5;
					pSend->ack_count=0;
				}
				else
				{
					nPackets=1;
				}
			}
		}

		pSend->last_ack_tick=dwTick;
		nPackets = 1;

		for ( int n=0; n<nPackets; n++ )
		{
			int nRead=pSend->fileObject->Read(buffer,FILE_PACKET_LEN);

			if ( nRead>0 )
			{
				XT_MSG file_packet;
				SF_InitXtMsg(file_packet, msg.from_id);
				file_packet.data_type=3;
				// 通讯方式
				file_packet.recv_flag = m_nType;

				CSendPacket spMsg(file_packet.data);
				spMsg<<file_id<<pSend->last_packet_id<<pSend->last_file_pointer;
				spMsg.WriteData(buffer,nRead);

				pSend->last_packet_id++;
				pSend->last_file_pointer+=nRead ;
				pSend->send_size+=nRead;

				file_packet.data_len=spMsg.GetSize();

				g_pProtocol->SendP2PMsg(file_packet,m_nType);
			}

			char		file_name[255] = {0};
			uint32		file_size = 0;
			uint32		last_packet_id = 0;
			char		szUrl[128] = {0};
			uint32		last_file_pointer = 0;
			rp>>file_size>>file_name>>last_packet_id>>last_file_pointer>>szUrl;

			if (/*pSend->send_size==pSend->file_size*/strcmp(szUrl, "") != 0)
			{
				pSend->ReplaceUrl(szUrl);
				if(pSend->IsReplaceAll())
				{
					strMsgData = pSend->pMsg->data;
				}

				SF_RemoveSendFile(pSend->file_id);
				break;
			}
		}
	}

	if(!strMsgData.empty())
	{
		vector<int> vecID;
		GetSelID(vecID);

		for(int i = 0; i < (int)vecID.size(); ++i)
		{
			SendMsg(vecID[i], strMsgData.c_str());
		}
	}
}

// 发送消息
void CGroupSendDlg::SendMsg(int nFriendID, const char *pMsgData)
{
	DuiLib::COptionUI* pCheckSendOnline = static_cast<DuiLib::COptionUI*>(m_PaintManager.FindControl(_T("checkSendOnline")));
	BOOL bSendOnline = pCheckSendOnline->IsSelected();

	FRIEND_INFO *pFriend = FindFriend(nFriendID);
	if(pFriend == NULL)
	{
		return;
	}

	if(bSendOnline && (pFriend->status != XTREAM_ONLINE && pFriend->status != XTREAM_LEAVE && pFriend->status != XTREAM_BUSY))	
	{
		return;
	}

	XT_MSG msg;
	SF_InitXtMsg(msg, nFriendID);
	msg.data_type   = 0;//即时聊天消息

	strcpy(msg.data, pMsgData);
	msg.data_len=strlen(msg.data)+1;

	XT_MSG msgShow = msg;

	int  type = CIMProtocol::SMT_SERVER;
	// 如果没有获取过好友状态，则获取
	if( pFriend->ip == 0)
	{	
		// 在允许的次数内重试获取好友最新的ip
		if(pFriend->p2pRetry > 0)
		{
			pFriend->connFlag = FRIEND_INFO::NONE;
			pFriend->p2pRetry--;
			// 获取当前好友的ip和端口
			RefreshFriendStatus(pFriend->GetId());
		}
		else
		{
			// 无法重试了，则使用服务器中转
			pFriend->connFlag = FRIEND_INFO::SERVER;
		}
		//}
		// 通过服务器中转
		type = CIMProtocol::SMT_SERVER;
	}
	else 
	{
		// 好友状态已经获取，可以开始p2p测试
		if( pFriend->connFlag == FRIEND_INFO::NONE ||
			pFriend->connFlag == FRIEND_INFO::P2P_FAILED
			)
		{
			XT_P2PTEST_REQ req;
			// 如果公网ip相同，则可能在同一个内网，尝试内网连接
			if(pFriend->ip == g_ImUserInfo.ip)
			{
				pFriend->connFlag = FRIEND_INFO::TRY_LAN;
				req.dest_ip = pFriend->localIP;
				req.dest_port = pFriend->localPort;
				req.ipType = XT_P2PTEST_REQ::LAN;
			}
			else
			{
				pFriend->connFlag = FRIEND_INFO::TRY_P2P;
				req.dest_ip = pFriend->ip;
				req.dest_port = pFriend->port;
				req.ipType = XT_P2PTEST_REQ::P2P;	
			}
			req.id = g_ImUserInfo.GetId();
			req.toid = pFriend->GetId();
			// 发送p2p测试包
			g_pProtocol->SendP2pTestPacket(req);
		}

		type = CIMProtocol::SMT_SERVER;
	}

	g_pProtocol->SendP2PMsg(msg,type);

	strcpy(msgShow.from_nickname,g_ImUserInfo.nickname);
	g_LocalData.SaveTalkMsg(msgShow,pFriend->GetId());
	CMainFrame::GetInstance()->GetRecentList()->AddUser(pFriend);
	RECENTLIST_OP(AddUser(pFriend))
	RegUserOperation(0x00c4);
}

void CGroupSendDlg::SF_SendPfileReq(const IMAGE_FILE_STRUCT &is,int nPackets, std::tr1::shared_ptr<XT_MSG> &msgO, int &nType)
{
	XT_MSG msg;
	SF_InitXtMsg(msg, msgO->to_id);

	msg.data_type  = 1;//

	CSendPacket spMsg(msg.data);
	spMsg<<is.file_session_id<<is.file_size<<is.file_name;

#ifdef _DEBUG
	// 	CString sPrompt;
	// 	int nP = is.file_size/FILE_PACKET_LEN;
	// 	if(is.file_size%FILE_PACKET_LEN)
	// 		nP++;
	// 	sPrompt.Format("%s(%d)%dbytes分为%d包",is.file_name,is.file_session_id,is.file_size,nP);
	// 	ShowMessage(sPrompt);
#endif
	msg.data_len = spMsg.GetSize();

	SEND_FILE_STRUCT_NEW *pSend = new SEND_FILE_STRUCT_NEW();

	pSend->file_id   = is.file_session_id;
	pSend->file_size = is.file_size;
	pSend->window_size = nPackets;

	pSend->send_size = 0;
	sprintf(pSend->file_name,is.file_name);

	// 保存原有消息体，当所有图片都上传到图片空间后，消息体发送出去
	pSend->pMsg = msgO;
	pSend->nType = nType;

	m_vecSendFiles.push_back(pSend);

	g_pProtocol->SendP2PMsg(msg,m_nType);
}