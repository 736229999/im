#include "stdafx.h"
#include "SkinPickerDialog.h"
#include "Im\LoginDlg.h"
#include "MainFrame.h"
#include "im/MessageHandleWnd.h"
#include "im/ClusterCreateDlg.h"
#include "im/FSDialog.h"
#include "NearCommCustDlg.h"
#include "im/MsgDlg.h"
#include "DuiSelfInfoDlg.h"
#include "SystemSettingDlg.h"
#include "FriendInfoDlg.h"
#include "SpecifySendDlg.h"
#include "EditAutoReplyDlg.h"
#include "im/UserCardDlg.h"
#include "CExitWarning.h"
#include "im/DuiCounsel.h"
#include "im/ModfCommNameDlg.h"
#include "im/AddBlackDlg.h"
#include "im/DialogClusterAuth.h"
#include "im/P2PSendMsgDlgCloseHint.h"
#include "im/DuiCounselNone.h"
#include "CExitMsgWarning.h"
#include "GroupSendDlg.h"
#include "GroupSendAddOtherDlg.h"
#include "im/MessageBox.h"
#include "DuiModHeadIconDlg.h"
#include "CModPSW.h"
#include "im/ClusterInfoDlg.h"
#include "im/DuiOrderNotice.h"
#include "im/DuiOrderSubscibe.h"
#ifdef _CS_PLAT_EX_
#include "CSPlatDlg.h"
#endif
#include "SellerMobileAuth.h"
using namespace DuiLib;
extern CMessageHandleWnd *g_pMessageHandleWnd;

void UpdateBk(DuiLib::CControlUI* background, HWND hWnd)
{
	string strBkImage = g_LocalData.GetBkImage(g_ImUserInfo.GetUserName().c_str());
	BOOL bSolid = strBkImage.find('c') != string::npos;
	if(!bSolid)
	{
		if(strBkImage == "BKImage\\1.png")	strBkImage  = "BKImage\\c1.png";
		else if(strBkImage == "BKImage\\2.png") strBkImage = "BKImage\\c13.png";
		else if(strBkImage == "BKImage\\3.png") strBkImage = "BKImage\\c14.png";
		else if(strBkImage == "BKImage\\4.png") strBkImage = "BKImage\\c15.png";
		else if(strBkImage == "BKImage\\5.png") strBkImage = "BKImage\\c16.png";
		else if(strBkImage == "BKImage\\6.png") strBkImage = "BKImage\\c17.png";
		else if(strBkImage == "BKImage\\7.png") strBkImage = "BKImage\\c18.png";
		else if(strBkImage == "BKImage\\8.png") strBkImage = "BKImage\\c19.png";
		else if(strBkImage == "BKImage\\9.png") strBkImage = "BKImage\\c20.png";
		else if(strBkImage == "BKImage\\10.png") strBkImage = "BKImage\\c21.png";
		else strBkImage = "BKImage\\c1.png";
	}
	RECT rc;
	GetWindowRect(hWnd, &rc);

	TCHAR szBuf[MAX_PATH] = {0};
	_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,18,18' dest='0,0,%d,26'"), strBkImage.c_str(), rc.right-rc.left);
	background->SetBkImage(szBuf);
}

std::tr1::shared_ptr<CSkinPickerDialog> CSkinPickerDialog::pInstance = NULL;
std::tr1::shared_ptr<CSkinPickerDialog> CSkinPickerDialog::Show()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
	}
	else
	{
		pInstance = std::tr1::shared_ptr<CSkinPickerDialog>(new CSkinPickerDialog());
		pInstance->Create(GetDesktopWindow(), _T("一键换肤"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}

	return pInstance;
}

void CSkinPickerDialog::OnFinalMessage(HWND /*hWnd*/) 
{ 
	return;
}

void CSkinPickerDialog::InitWindow()
{
	WindowImplBase::InitWindow();
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));
	/////
	UpdateBk();
    SetForeImg();
	
	/*m_pBtnRestore = static_cast<CButtonUI*>(m_PaintManager.FindControl(kBtnRestore));
	m_pBtnListTrans = static_cast<CButtonUI*>(m_PaintManager.FindControl(kBtnListTrans));
	m_pOptSkinTrans = static_cast<COptionUI*>(m_PaintManager.FindControl(kOptSkinTrans));
	m_pBtnSkinTrans = static_cast<CButtonUI*>(m_PaintManager.FindControl(kBtnSkinTrans));
	m_pBtnPattet = static_cast<CButtonUI*>(m_PaintManager.FindControl(kBtnPattet));
	m_pBtnCustom = static_cast<CButtonUI*>(m_PaintManager.FindControl(kBtnCustom));

	SetBkColor(m_pMainWnd->GetBkColor());
	SetBkImage(m_pMainWnd->GetBkImage());
	CDuiString szValue;
	szValue.SmallFormat(_T("%d%%"),m_pMainWnd->GetListTrans()* 100 / 255 );
	m_pBtnListTrans->SetText(szValue);
	m_pBtnListTrans->SetTag(m_pMainWnd->GetListTrans());

	szValue.SmallFormat(_T("%d%%"),m_pMainWnd->GetSkinTrans() * 100 / 150);
	m_pBtnSkinTrans->SetText(szValue);
	m_pBtnSkinTrans->SetTag(m_pMainWnd->GetSkinTrans());

	m_pOptSkinTrans->Selected(m_pMainWnd->GetIsSkinTrans());*/

}

CControlUI* CSkinPickerDialog::CreateControl(LPCTSTR pstrClass) 
{
	if (_tcsicmp(pstrClass, "SkinPikerPictureItem") == 0)
		return	new CSkinPikerPictureItemUI();

	return NULL;
}
//
//DWORD CSkinPickerDialog::GetBkColor() const
//{
//	CControlUI* background = m_pm.FindControl(kWndBackGround);
//	if (background != NULL)
//		return background->GetBkColor();
//	return 0;
//}
//
//void CSkinPickerDialog::SetBkColor(DWORD dwBackColor)
//{
//	CControlUI* background = m_pm.FindControl(kWndBackGround);
//	if (background != NULL)
//	{
//		background->SetBkImage(_T(""));
//		background->SetBkColor(dwBackColor);
//		background->NeedUpdate();
//	}
//
//}
//
void CSkinPickerDialog::UpdateBk()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		CControlUI* background = pInstance->m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			::UpdateBk(background, pInstance->GetHWND());
		}

	}
}
//
//LPCTSTR CSkinPickerDialog::GetBkImage()
//{
//	CControlUI* background = m_pm.FindControl(kWndBackGround);
//	if (background != NULL)
//		return background->GetBkImage();
//	return _T("");
//}

void CSkinPickerDialog::Notify(TNotifyUI& msg)
{
	if( msg.sType == _T("click") ) 
	{
		/*if( msg.pSender->GetName() == _T("closebtn")) {
		Close(IDOK);
		return; 
		}
		else if (msg.pSender->GetName() == kBtnListTrans)
		{
		CPopupWidget* pWidget = new CPopupWidget;
		CPoint point;
		RECT rc	= m_pBtnListTrans->GetPos();
		point.x = rc.left;
		point.y = rc.top;			
		pWidget->Init(_T("xmls\\Skin\\ListTrans.xml"), m_hWnd, m_pMainWnd->GetMainWndPaintManager(), point, CHANGE_LIST_TRANS, m_pBtnListTrans);

		return;
		}
		else if (msg.pSender->GetName() == kBtnSkinTrans)
		{
		CPopupWidget* pWidget = new CPopupWidget;
		CPoint point;
		RECT rc	= m_pBtnSkinTrans->GetPos();
		point.x = rc.left;
		point.y = rc.top;			
		pWidget->Init(_T("xmls\\Skin\\ListTrans.xml"), m_hWnd, m_pMainWnd->GetMainWndPaintManager(), point, CHANGE_MAINWND_TRANS, m_pBtnSkinTrans);

		return;
		}
		else if (msg.pSender->GetName() == kBtnCustom)
		{
		vector<CDuiString> names;
		if (CFileUtil::OpenFile(L"JEPG Files(*.jpg)\0*.jpg\0BMP Files\0*.bmp\0PNG Files\0*.png\0\0",GetHWND(),names, false))
		{
		SetBkImage(names[0]);
		m_pMainWnd->SetBkImage(names[0]);
		}	

		}
		else */
		if (msg.pSender->GetName().Find(kSkinPickerPicutreItem) == 0)
		{

			////清空默认选择的有颜色
			m_pSkinPicItem->SetForeImage("");
			////
			CSkinPikerPictureItemUI * pItem = static_cast<CSkinPikerPictureItemUI*>(msg.pSender);
			TCHAR szBuf[MAX_PATH] = {0};
			map<CSkinPikerPictureItemUI*,CString>::iterator it;
			if(m_mapDlg.find(pItem) != m_mapDlg.end()) ////选择过
			{
				it = m_mapDlg.begin();
				for(; it != m_mapDlg.end(); ++it)
				{
					it->first->EraseAttribute(_T("foreimage"),_T("")); //// 取消上次选中tips
				}
				pItem->SetAttribute(_T("foreimage"),szBuf);  //// 设置选中tips
			}
			else /////没选择过
			{
		 	    it = m_mapDlg.begin();
				for(; it != m_mapDlg.end(); ++it)
				{
					it->first->EraseAttribute(_T("foreimage"),_T("")); //// 取消上次选中tips
				} 
			   	pItem->SetAttribute(_T("foreimage"),szBuf);  //// 设置选中tips  
				m_mapDlg[pItem] = pItem->GetChooseTips();
			}

			CDuiString strBkImage = pItem->GetBkImage();
			strBkImage.Replace(_T("small"), _T(""));
			g_LocalData.SetBkImage(g_ImUserInfo.GetUserName().c_str(), strBkImage.GetData());
			UpdateBk();
			

			CLoginDlg::UpdateBk();
			CMainFrame::UpdateBk();
			CCreateClusterDlg::UpdateBk();
			CAddFriendOrClusterDlg::UpdateBk();
			CNearCommCustDlg::UpdateBk();
			CDuiMsgDlg::UpdateBk();
			CSystemSettingDlg::UpdateBk();
			CSpecifySendDlg::UpdateBk();
			CExitWarningDlg::UpdateBk();
			CDuiCounsel::UpdateBk();
			CModfCommNameDlg::UpdateBk();
			CAddBlackDlg::UpdateAllBk();
			CDialogClusterAuth::UpdateBk();
			CP2PSendMsgDlgCloseHint::UpdateBk();
			CDuiCounselNone::UpdateBk();
			CFriendInfoDlg::UpdateAllBk();
			CExitMsgWarningDlg::UpdateBk();
			CGroupSendDlg::UpdateBk();
			CGroupSendAddOtherDlg::UpdateBk();
			CMessageBox::UpdateAllBk();
			CDuiModHeadIconDlg::UpdateBk();
			CSellerMobileAuthDlg::UpdateBk();
			CDuiOrderNotice::UpdateBk();
			CDuiOrderSubscibe::UpdateBk();
		 #ifdef _CS_PLAT_EX_////飞牛客服工作台
			CModPassWordDlg::UpdateBk();
         #endif
			CClusterInfoDlg::UpdateBk();
#ifdef _CS_PLAT_EX_
			CDuiCSPlatDlg::UpdateBk();
#endif			

			static std::tr1::shared_ptr<CUserCardDlg> pUserTips = CUserCardDlg::GetInstance();
			if(pUserTips != NULL)
			{
				pUserTips->UpdateBk();
			}

		/*	static std::tr1::shared_ptr<CFriendInfoDlg> pFriInfoDlg = CFriendInfoDlg::GetInstance();
			if(pFriInfoDlg != NULL)
			{
				pFriInfoDlg->UpdateBk();
			}*/

			static std::tr1::shared_ptr<CDuiSelfInfoDlg> pDuiSelfInfoDlg = CDuiSelfInfoDlg::GetInstance();
			if(pDuiSelfInfoDlg != NULL)
			{
				pDuiSelfInfoDlg->UpdateBk();
			}
			g_pMessageHandleWnd->UpdateBk();
			/*m_pMainWnd->SetBkImage(strBkImage);*/
		}
		else if(msg.pSender->GetName() == "minbtn")
		{
			SendMessage(WM_SYSCOMMAND, SC_MINIMIZE, 0); 
		}
		else if(msg.pSender->GetName() == "closebtn")
		{
			if(m_mapDlg.size()>0)
			{
				m_mapDlg.clear();
			}
			Close(IDOK);
		}
		/*else if (msg.pSender->GetName() == kBtnRestore)
		{
			m_pMainWnd->LoadDefaultState();
			SetBkColor(m_pMainWnd->GetBkColor());
			SetBkImage(m_pMainWnd->GetBkImage());

			m_pBtnListTrans->SetTag(kMainWndDefaultListTrans);
			m_pBtnListTrans->SetText(_T("30%"));

			m_pBtnSkinTrans->SetEnabled(false);
			m_pMainWnd->SetIsSkinTrans(false);
			m_pBtnSkinTrans->SetText(_T("0%"));
			m_pBtnSkinTrans->SetTag(0);
			CenterWindow();
		}
		else if (msg.pSender->GetName() == kBtnPattet)
		{
			CPalletDialog *pPalletDlg = new CPalletDialog(_T("xmls\\PalletDialog.xml"),this);
			pPalletDlg->Create(m_hWnd,_T("Redran调色板"), UI_WNDSTYLE_DIALOG, WS_EX_WINDOWEDGE);
			pPalletDlg->CenterWindow();
			if(pPalletDlg->ShowModal() == IDOK)
			{
				SetBkColor(pPalletDlg->GetSelectColor());
				m_pMainWnd->SetBkColor(pPalletDlg->GetSelectColor());
			}			
			delete	pPalletDlg;
		}*/
		

	}
	/*else if(msg.sType == _T("selectchanged"))
	{
	if( msg.pSender->GetName() == kOfficialSkin) {
	m_pSkinTab->SelectItem(0);
	}
	else if( msg.pSender->GetName() == kMySkin) {
	m_pSkinTab->SelectItem(1);
	}
	else if( msg.pSender->GetName() == kOptSkinTrans) {
	if(static_cast<COptionUI*>(msg.pSender)->IsSelected())
	{
	m_pBtnSkinTrans->SetEnabled(true);
	m_pMainWnd->SetIsSkinTrans(true);
	}
	else
	{
	m_pBtnSkinTrans->SetEnabled(false);
	m_pMainWnd->SetIsSkinTrans(false);
	m_pBtnSkinTrans->SetText(_T("0%"));
	m_pBtnSkinTrans->SetTag(0);
	}
	}
	}*/

}

LRESULT CSkinPickerDialog::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	//// 处理后台交互消息
	//if(WM_XT_CMD_NOTIFY == uMsg)z
	//{
	//	OnXtMessage(wParam, lParam);
	//}

	return WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void  CSkinPickerDialog::SetForeImg()
{
	TCHAR szBuf[MAX_PATH] = {0};
	string strForeImg = "SysBtn\\icon_choose.png";
	string strBkImage = g_LocalData.GetBkImage(g_ImUserInfo.GetUserName().c_str());
	BOOL bSolid = strBkImage.find('c') != string::npos;
	if(!bSolid)
	{
		if(strBkImage == "BKImage\\1.png")
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem1"));
		}
		else if(strBkImage == "BKImage\\2.png")
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem2"));
		}
		else if(strBkImage == "BKImage\\3.png")	
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem3"));
		}
		else if(strBkImage == "BKImage\\4.png") 
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem4"));
		}
		else if(strBkImage == "BKImage\\5.png") 
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem5"));
		}
		else if(strBkImage == "BKImage\\6.png")
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem6"));
		}
		else if(strBkImage == "BKImage\\7.png")
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem7"));
		}
		else if(strBkImage == "BKImage\\8.png") 
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem8"));
		}
		else if(strBkImage == "BKImage\\9.png")
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem9"));
		}
		else if(strBkImage == "BKImage\\10.png")
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem10"));
		}
		else
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem1"));
		}	
		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' dest='70,70,86,86'"), strForeImg.c_str());
	}	
	else////纯色
	{
		if(strBkImage == "BKImage\\c1.png")
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem11"));
		}
		else if(strBkImage == "BKImage\\c2.png")
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem12"));
		}
		else if(strBkImage == "BKImage\\c3.png")	
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem13"));
		}
		else if(strBkImage == "BKImage\\c4.png") 
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem14"));
		}
		else if(strBkImage == "BKImage\\c5.png") 
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem15"));
		}
		else if(strBkImage == "BKImage\\c6.png")
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem16"));
		}
		else if(strBkImage == "BKImage\\c7.png")
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem17"));
		}
		else if(strBkImage == "BKImage\\c8.png") 
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem18"));
		}
		else if(strBkImage == "BKImage\\c9.png")
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem19"));
		}
		else if(strBkImage == "BKImage\\c10.png")
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem20"));
		}
		else if(strBkImage == "BKImage\\c11.png")
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem21"));
		}
		else if(strBkImage == "BKImage\\c12.png")
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem22"));
		}
		else
		{
			m_pSkinPicItem = static_cast<CSkinPikerPictureItemUI*>(m_PaintManager.FindControl("SkinPictureItem11"));
		}	
		_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' dest='17,17,33,33'"), strForeImg.c_str());
	}

	m_pSkinPicItem->SetForeImage(szBuf);
}
