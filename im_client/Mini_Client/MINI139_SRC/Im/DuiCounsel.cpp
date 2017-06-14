#include "StdAfx.h"
#include "DuiCounsel.h"
#include "DuiCounselNone.h"
#include "..\CExitMsgWarning.h"
using namespace DuiLib;

extern map<FRIEND_INFO *,UINT>  g_mapMsgUnRead;////δ����Ϣ�ĺ���ID��δ������
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);

DuiLib::UILIB_RESOURCETYPE CDuiCounsel::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

LPCTSTR CDuiCounsel::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CDuiCounsel::InitWindow()
{
	m_pDlgSelectP2PSendMsgDlg = NULL;
	m_nRemindCount = 1;
	m_MenuCheckInfo[_T("MenuAll")] = true;
	m_bIsUpdate = FALSE;
	m_preRc = 0;
	m_pList = static_cast<DuiLib::CListUI*>(m_PaintManager.FindControl(_T("ListDemo1")));
	UpdateBk();
}

std::tr1::shared_ptr<CDuiCounsel> CDuiCounsel::pInstance = NULL;
std::tr1::shared_ptr<CDuiCounsel> CDuiCounsel::Show()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
	}
	else
	{
		pInstance = tr1::shared_ptr<CDuiCounsel>(new CDuiCounsel());
		pInstance->Create(GetDesktopWindow(), _T("DuiCounsel"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW, 0, 0, 210, 342);
		//pInstance->Create(GetDesktopWindow(), _T("��ѯ��"), NULL, NULL);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}

	return pInstance;
}

std::tr1::shared_ptr<CDuiCounsel> CDuiCounsel::GetInstance()
{	
	if(pInstance != NULL && !::IsWindow(pInstance->GetHWND()))
		return NULL;

	return pInstance;	
}

void CDuiCounsel::UpdateBk()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			string strBkImage = g_LocalData.GetBkImage(g_ImUserInfo.GetUserName().c_str());
			BOOL bSolid = strBkImage.find('c') != string::npos;
			if(!bSolid)
			{
				if(strBkImage == "BKImage\\1.png") strBkImage = "BKImage\\c1.png";
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
			GetWindowRect(pInstance->GetHWND(), &rc);

			TCHAR szBuf[MAX_PATH] = {0};
			_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,18,18' dest='0,0,%d,%d'"), strBkImage.c_str(), rc.right-rc.left, rc.bottom-rc.top);
			background->SetBkImage(szBuf);

			DuiLib::CListUI* pList = static_cast<DuiLib::CListUI*>(pInstance->m_PaintManager.FindControl(_T("ListDemo1")));
			if(pList != NULL)
			{
				for (int nElement = 0; nElement < pList->GetCount(); nElement++)
				{
					DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)pList->GetItemAt(nElement);
					DuiLib::CControlUI* pSelectItemColor = static_cast<DuiLib::CControlUI*>(pInstance->m_PaintManager.FindSubControlByName(pListElement, _T("selectitem")));
					if(!pListElement->IsSelected())
						pSelectItemColor->SetBkImage(szBuf);
					DuiLib::CControlUI* pLineItem = static_cast<DuiLib::CControlUI*>(pInstance->m_PaintManager.FindSubControlByName(pListElement, _T("lineitem")));
					pLineItem->SetBkImage(szBuf);
				}
			}
		}
	}
}

void CDuiCounsel::Notify(DuiLib::TNotifyUI& msg) 
{
	//������˵�,�ѻ����µĲ˵��ؼ�
	//DuiLib::CListUI* pListCombo = static_cast<DuiLib::CListUI*>(m_PaintManager.FindControl(_T("ListDemo2")));
	//if(pListCombo->IsVisible())
	//{
	//	if(msg.ptMouse.x < pListCombo->GetPos().left || 
	//		msg.ptMouse.x > pListCombo->GetPos().right ||
	//		msg.ptMouse.y < pListCombo->GetPos().top ||
	//		msg.ptMouse.y > pListCombo->GetPos().bottom)
	//		pListCombo->SetVisible(false);
	//	DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)pListCombo->GetItemAt( pListCombo->GetCurSel() );
	//	if(msg.ptMouse.x > pListElement->GetPos().left && 
	//		msg.ptMouse.x < pListElement->GetPos().right &&
	//		msg.ptMouse.y > pListElement->GetPos().top &&
	//		msg.ptMouse.y < pListElement->GetPos().bottom)
	//		pListCombo->SetVisible(false);
	//}
	if(msg.sType == _T("itemselect"))//ѡ����ѯ���еĺ���
	{
		if( m_pList == (DuiLib::CListUI *)msg.pSender )
		{
			int index = ((DuiLib::CListUI *)msg.pSender)->GetCurSel();
			if(g_mapMsgUnRead.size()>0)////�ϴ���δ������Ϣ
			{
                ShowSelectP2PSendMsgDlg(m_vecP2PSendMsgDlg[index],1);
			}
			else
			{
				ShowSelectP2PSendMsgDlg(m_vecP2PSendMsgDlg[index]);////�ֶ��򿪵���Ϣ
			}
		}
		//if(pListCombo == (DuiLib::CListUI *)msg.pSender)
		//{
		//	int index = ((DuiLib::CListUI *)msg.pSender)->GetCurSel();
		//	pListCombo->SetVisible(false);
		//	if(index == 0)
		//	{
		//		ShowAllCounselDlg();
		//		//ShowSelectP2PSendMsgDlg(m_vecP2PSendMsgDlg[index]);
		//	}
		//	else if(index == 1)
		//	{
		//		ShowCounselingDlg();
		//		//ShowSelectP2PSendMsgDlg(m_vecP2PSendMsgDlgCounseling[index]);
		//	}
		//}
	}
	else if(msg.sType == _T("click")) //�����ѯ�����˵�
	{
		if(msg.pSender->GetName() == _T("Label1")) 
		{
			//CMessageBox::Model(NULL, _T("�����ѯ"), _T("�����ѯ"), NULL);
 			//RECT rc = msg.pSender->GetPos();
 			//int nWidth = rc.right - rc.left;
 			//int nHeight = rc.bottom - rc.top;
 			//rc.left += msg.ptMouse.x;
 			//rc.top += msg.ptMouse.y;
 			//rc.right = rc.left + nWidth;
 			//rc.bottom = rc.top + 50;
 			//pListCombo->SetPos(rc);
 			//pListCombo->SetVisible(true);

			//������˵�
			DuiLib::CMenuWnd* pMenu = new DuiLib::CMenuWnd();
			DuiLib::CPoint point = msg.ptMouse;
			ClientToScreen(m_hWnd, &point);
			pMenu->Init(NULL, _T("DuiCounselMenu.xml"), point, &m_PaintManager, &m_MenuCheckInfo);
		}
		else if(msg.pSender->GetName() == _T("btnMarkeing")) //����˰���֮����ʾ����
		{
			DuiLib::CListContainerElementUI* pListElement = NULL;
			pListElement = (DuiLib::CListContainerElementUI*)msg.pSender->GetParent();//((DuiLib::CListUI *)msg.pSender->GetParent())->GetItemAt(index);

			DuiLib::CButtonUI* pBtnMarkeing = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarkeing")));
			DuiLib::CButtonUI* pBtnMarked = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarked")));

			pBtnMarkeing->SetVisible(false);
			pBtnMarked->SetVisible(true);
		}
		else if(msg.pSender->GetName() == _T("btnMarked")) //����˺���֮����ʾ����
		{
			DuiLib::CListContainerElementUI* pListElement = NULL;
			pListElement = (DuiLib::CListContainerElementUI*)msg.pSender->GetParent();//((DuiLib::CListUI *)msg.pSender->GetParent())->GetItemAt(index);

			DuiLib::CButtonUI* pBtnMarkeing = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarkeing")));
			DuiLib::CButtonUI* pBtnMarked = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarked")));
			pBtnMarkeing->SetVisible(true);
			pBtnMarked->SetVisible(false);
		}
		else if(msg.pSender->GetName() == _T("btnClose")) //����ر�ͼ��ر�һ����ѯ��������
		{
			CMainFrame::GetInstance()->LockDlg();////��ס�Ի���
			DuiLib::CLabelUI* pListElement = NULL;
			pListElement = (DuiLib::CLabelUI*)msg.pSender->GetParent();
		    pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
			if(pBtnRemind->IsVisible())////�ر�ѡ�е�һ�������,��δ����Ϣ
			{
				std::tr1::shared_ptr<CExitMsgWarningDlg> pWnd = CExitMsgWarningDlg::Show(CLOSE_TALK,GetHWND());
				pWnd->ShowModal();
				if(pWnd->m_bUnReadMsgShow)///ȷ�Ϲر�		
				{
					DuiLib::CControlUI* pC = pListElement->GetParent();
					DuiLib::CControlUI* pC2 = pC->GetParent();
					DuiLib::CControlUI* pC3 = pC2->GetParent();
					DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC3;
					int nIndex = pElement->GetIndex();

					CString strTxt = pBtnRemind->GetText();////��ȡ���ζԻ���δ����Ϣ��¼��д����ʱlog;
		        	int nUnReadCnt = _ttoi(strTxt);
					FRIEND_INFO* pFriend = m_vecFriend[nIndex];
					CMainFrame::GetInstance()->SaveUnReadMsgLog(pFriend,nUnReadCnt,0,FALSE);////����δ����Ϣ����д�ļ�
					m_vecP2PSendMsgDlg[nIndex]->SendMessage(WM_CLOSE_ONE);
					CMainFrame::GetInstance()->ReleaseDlg();////�򿪶Ի���
				}	
				else 
				{
				    CMainFrame::GetInstance()->ReleaseDlg();////�򿪶Ի���
					return;
				}
			}
			else ////��ǰ������δ����Ϣ��ֱ�ӹر�
			{
			   //////////////////////////////////////////////////////
				DuiLib::CControlUI* pC = pListElement->GetParent();
				DuiLib::CControlUI* pC2 = pC->GetParent();
				DuiLib::CControlUI* pC3 = pC2->GetParent();
				DuiLib::CListContainerElementUI* pElement = (DuiLib::CListContainerElementUI*)pC3;
				int nIndex = pElement->GetIndex();

				/////��δ����Ϣ
				FRIEND_INFO* pFriend = m_vecFriend[nIndex];
				map<FRIEND_INFO *,UINT>::iterator it;
				it = g_mapMsgUnRead.find(pFriend);
				if ( it!=g_mapMsgUnRead.end() )
				{
					g_mapMsgUnRead.erase(it);////����ϴα���ĵ�ǰ���ڵ�δ����Ϣ
				}
				/////////////////
				m_vecP2PSendMsgDlg[nIndex]->SendMessage(WM_CLOSE_ONE);
			    CMainFrame::GetInstance()->ReleaseDlg();////�򿪶Ի���
			}
		
		}
	}
}

LRESULT CDuiCounsel::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if (uMsg == WM_MENUCLICK)//����˵������Ϣ
	{
		CDuiString *strMenuName = (CDuiString*)wParam;
		BOOL bChecked = (BOOL)lParam;		 

		if ( *strMenuName == _T("MenuAll")) //��ʾȫ����ѯ��
		{
			ShowAllCounselDlg();
			m_MenuCheckInfo[_T("MenuAll")] = true;
			m_MenuCheckInfo[_T("MenuPorblem")] = false;
		}
		else if ( *strMenuName == _T("MenuPorblem")) //��ʾ��������ѯ��
		{
			BOOL bIsExist = FALSE;
			for(int nElement = 0; nElement < /*m_vecListElement.size()*/m_pList->GetCount(); nElement++)
			{
				DuiLib::CListContainerElementUI* pListElement = /*m_vecListElement[nElement];*/(DuiLib::CListContainerElementUI*)m_pList->GetItemAt(nElement);
				DuiLib::CButtonUI* pBtnMarked = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarked")));
				if(pBtnMarked->IsVisible())
				{
					bIsExist = TRUE;
					break;
				}
			}
			if(!bIsExist)
			{
				CDuiCounselNone::NewDlg();
				m_MenuCheckInfo[_T("MenuAll")] = true;
				m_MenuCheckInfo[_T("MenuPorblem")] = false;
			}
			else
			{
				ShowCounselingDlg();
				m_MenuCheckInfo[_T("MenuAll")] = false;
				m_MenuCheckInfo[_T("MenuPorblem")] = true;
			}
		}

		//delete strMenuName;
	}
	else if (uMsg == WM_LBUTTONDOWN)//�����ѯ�����������ö�
	{
		CRect rcCounsel;
		::GetWindowRect(*this, &rcCounsel);

		//û��ѡ����ѯ����Ĭ��ѡ�е�һ����ѯ��
		
		if(m_vecP2PSendMsgDlg.size() > 0)
		{
			/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg = NULL; 
			BOOL bIsDlgShow = FALSE;
			if(m_pList->GetCurSel() == -1)
			{
				for(int nDlg = 0; nDlg < m_vecP2PSendMsgDlg.size(); nDlg++)
				{
					pDlg = m_vecP2PSendMsgDlg[nDlg];
					if(::IsWindowVisible(pDlg->GetHWND()))
					{
						bIsDlgShow = TRUE;
						::BringWindowToTop(pDlg->GetHWND());
						break;
					}
				}
				if(!bIsDlgShow)
				{
					pDlg = m_vecP2PSendMsgDlg[0];
				}
			}
			else if( m_pList->GetCurSel() >= 0 )
			{
				pDlg = m_vecP2PSendMsgDlg[m_pList->GetCurSel()];
			}
			//if(pDlg != NULL)
			//{
			//	CRect rcDlg;
			//	/*pDlg->*/::GetWindowRect(pDlg->GetHWND(), rcDlg);
			//	pDlg->ShowWindow(SW_SHOW);
			//	::SetWindowPos(pDlg->GetHWND(),*this, rcCounsel.right, rcCounsel.top, rcDlg.Width(), rcDlg.Height(),SWP_SHOWWINDOW);
			//	/*pDlg->*/::MoveWindow(pDlg->GetHWND(), rcCounsel.right, rcCounsel.top, rcDlg.Width(), rcDlg.Height(), TRUE);
			//}
		}
	}
	else if ( WM_SIZE == uMsg)
	{
		UpdateBk();
	}

	bHandled = false;
	return 0;
}

void CDuiCounsel::AddFriendListElement(FRIEND_INFO* pFriend, bool bIsReply, bool bIsTransfer, bool bIsRepeat, bool bIsMessage, bool bIsRemind, bool bBtnMarkeing, bool bBtnMarked)
{
	for(int nFriend = 0; nFriend < m_vecFriend.size(); nFriend++)
	{
		if(m_vecFriend[nFriend] == pFriend)
			return;
	}

	m_vecFriend.push_back(pFriend);

	DuiLib::CDuiString str;
	DuiLib::CListContainerElementUI* pListElement = NULL;
	DuiLib::CDialogBuilder builder;
	pListElement = static_cast<DuiLib::CListContainerElementUI*>(builder.Create(_T("DuiCounselListElement.xml"), (UINT)0, NULL));
	
	//�����ѯ��Ԫʱ����ߺ��������ɫ
	DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
	CString strBkImage = background->GetBkImage();
	DuiLib::CControlUI* pSelectItemColor = static_cast<DuiLib::CControlUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("selectitem")));
	pSelectItemColor->SetBkImage(strBkImage);
	pSelectItemColor->SetBkColor(NULL);
	DuiLib::CControlUI* pLineItem = static_cast<DuiLib::CControlUI*>(pInstance->m_PaintManager.FindSubControlByName(pListElement, _T("lineitem")));
	pLineItem->SetBkImage(strBkImage);

	DuiLib::CButtonUI* pBtnClose = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnClose")));
	pBtnClose->SetVisible(false);
	vector<DuiLib::CButtonUI*> vecMousemoveBtn;
	vecMousemoveBtn.push_back(pBtnClose);
	pListElement->SetMousemoveBtnVisible(vecMousemoveBtn);

	DuiLib::CButtonUI* pBtnMessage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMessage")));
	DuiLib::CButtonUI* pBtnRepeat = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRepeat")));
	DuiLib::CButtonUI* pBtnReply = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnReply")));
	/*DuiLib::CButtonUI* */pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
	DuiLib::CButtonUI* pBtnTransfer = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnTransfer")));
	DuiLib::CButtonUI* pBtnMarkeing = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarkeing")));
	DuiLib::CButtonUI* pBtnMarked = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarked")));

	pBtnMessage->SetVisible(bIsMessage);
	pBtnTransfer->SetVisible(bIsTransfer);
	pBtnRepeat->SetVisible(bIsRepeat);
	pBtnReply->SetVisible(bIsReply);
	pBtnRemind->SetVisible(bIsRemind);
	
	pBtnMarkeing->SetVisible(false);
	pBtnMarked->SetVisible(false);

	pBtnMarkeing->SetVisible(true);//��һ�������ʾ����
	m_pList->Add(pListElement);
	//m_vecListElement.push_back(pListElement);

	m_PaintManager.SetDefaultFont("΢���ź�", 12, false, false, false);
	DuiLib::CLabelUI* nick_name = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("nickname")));
	m_PaintManager.AddFont("΢���ź�", 12, true, false, false);

	if (nick_name != NULL)
	{
		str = /*pFriend->nickname;*/UserInfoToStr(pFriend);

		//����18��������ʾ...
		if(str.GetLength() >= 16)
		{
			BYTE bChar = str.GetAt(12);
			if(bChar > 127)
				str = str.Left(12) + "...";
			else
				str = str.Left(13) + "...";
		}
		nick_name->SetFont(0);
		nick_name->SetText(str);
	}

	DuiLib::CLabelUI* historyMsg = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("historyMsg")));
	DuiLib::CLabelUI* historyTime = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("historyTime")));
	MSG_LIST ml1;
	
	g_LocalData.GetLastMsgList(pFriend->GetId(),CDataManager::USER,ml1,5);

	POSITION pos=ml1.GetTailPosition();

	if(pos != NULL)
	{
		XT_MSG msg = ml1.GetAt(pos);

		str = msg.data;
		//�����ͼƬ����ʾ[ͼƬ]
		while(str.Find("<image:",0) != -1)
		{
			int nStart = str.Find("<image:",0);
			int nEnd = str.Find(">",0);
			str.Replace(str.Mid(nStart, nEnd - nStart + 1), "[ͼƬ]");
		}
		//����18��������ʾ...
		if(str.GetLength() >= 16)
		{
			BYTE bChar = str.GetAt(12);
			if(bChar > 127)
				str = str.Left(12) + "...";
			else
				str = str.Left(13) + "...";
		}
		str.Format("  %s", str);
		if (historyMsg != NULL)
		{
			historyMsg->SetText(str);
		}
		//��ʾʱ�� ʱ:��
		CTime  tm(msg.send_time);
		str=tm.Format("%H:%M");
		if (historyTime != NULL)
		{
			historyTime->SetText(str);
		}
	}
	m_mapFriendCount[pFriend] = 1;

	if(m_MenuCheckInfo[_T("MenuPorblem")])
	{
		pListElement->SetVisible(false);
	}
}

void CDuiCounsel::SetFriendListElement(FRIEND_INFO* pFriend, bool bIsReply, bool bIsTransfer, bool bIsRepeat, bool bIsMessage, bool bIsRemind,int nUnReadNum)
{
	int nIndex = -1;
	for(int nFriend = 0; nFriend < m_vecFriend.size(); nFriend++)
	{
		if(m_vecFriend[nFriend] == pFriend)
		{
			nIndex = nFriend;
			break;
		}
	}
	if(nIndex == -1)
	{
		return;
	}

	if(nUnReadNum > 0)
	{
		m_mapFriendCount[pFriend] = nUnReadNum + 1;
	}

	//���Ϊ��ǰ�������ң�δ����Ϣ���ۼ�
	if(m_vecP2PSendMsgDlg.size() <= 1 || m_pDlgSelectP2PSendMsgDlg->GetFriendInfo()->GetId() == pFriend->GetId())
	{
		bIsRemind = false;
		m_mapFriendCount[pFriend] = 1;
	}

	DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pList->GetItemAt(nIndex);

	//if(m_pList->GetCount() == 1)
	//{
	//	m_pList->SelectItem(0);
	//}

	if(nIndex != 0)
	{
		for(int nElement = nIndex; nElement > 0; nElement--)
		{
			m_pList->SetItemIndex(((DuiLib::CListContainerElementUI*)m_pList->GetItemAt(nElement)), nElement-1);
		}
		
		//pList->SetItemIndex(pListElement, 0);

		FRIEND_INFO* pFriendChange = m_vecFriend[nIndex];
		vector<FRIEND_INFO*>::iterator itrFriendDel = m_vecFriend.begin();
		for(; itrFriendDel != m_vecFriend.end(); itrFriendDel++)
		{
			if(*itrFriendDel == pFriendChange)
			{
				m_vecFriend.erase(itrFriendDel);
				break;
			}
		}
		vector<FRIEND_INFO*>::iterator itrFriendInsert = m_vecFriend.begin();
		m_vecFriend.insert(itrFriendInsert, pFriendChange);////������ǰ

		/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlgChange = m_vecP2PSendMsgDlg[nIndex];
		vector</*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg*>::iterator itrDlgDel = m_vecP2PSendMsgDlg.begin();
		for(; itrDlgDel != m_vecP2PSendMsgDlg.end(); itrDlgDel++)
		{
			if(*itrDlgDel == pDlgChange)
			{
				m_vecP2PSendMsgDlg.erase(itrDlgDel);
				break;
			}
		}
		vector</*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg*>::iterator itrDlgInsert = m_vecP2PSendMsgDlg.begin();
		m_vecP2PSendMsgDlg.insert(itrDlgInsert, pDlgChange);////�Ի���������ǰ
	}

	if(!bIsReply && !bIsTransfer && !bIsRepeat && !bIsMessage&& !bIsRemind&&nUnReadNum==0)
	{
		DuiLib::CButtonUI* pBtnReply = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnReply")));
		pBtnReply->SetVisible(false);
		return;
	}

	DuiLib::CDuiString str;

	//�ж��Ƿ�Ϊ�ظ���ѯ
	//if(!bIsTransfer)
	//{
	//	BOOL bExist = FALSE;
	//	map<FRIEND_INFO*, int>::iterator itr = m_mapFriendCount.begin();
	//	for(; itr != m_mapFriendCount.end(); itr++)
	//	{
	//		if(itr->first == pFriend)
	//		{
	//			itr->second++;
	//			bExist = TRUE;
	//			//bIsRepeat = true;
	//			break;
	//		}	
	//	}
	//}


	DuiLib::CButtonUI* pBtnMessage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMessage")));
	DuiLib::CButtonUI* pBtnRepeat = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRepeat")));
	DuiLib::CButtonUI* pBtnReply = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnReply")));
	DuiLib::CButtonUI* pBtnTransfer = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnTransfer")));
	/*DuiLib::CButtonUI* */pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));

	pBtnMessage->SetVisible(bIsMessage);
	pBtnTransfer->SetVisible(bIsTransfer);
	pBtnRepeat->SetVisible(bIsRepeat);
	//�ж��ǲ��ǻظ�,����ǣ���δ����Ϣ����
	if(bIsReply /*&& m_mapFriendCount[pFriend] > 1*/)
	{
		pBtnReply->SetVisible(true);
		pBtnRepeat->SetVisible(false);
	}
	else
	{
		if(nUnReadNum>0)
		{
			pBtnRemind->SetVisible(true);
			CString strTxt = _T("");
			strTxt.Format(_T("%d"),nUnReadNum);
			pBtnRemind->SetText(strTxt);
	        pListElement->SetDrawBkColor(0xFFFEE8C0);
		}
		else
		{
			pBtnReply->SetVisible(false);
		}
	}

	//�ж��Ƿ�Ϊ�ظ���ѯ
	if(!bIsTransfer && !bIsReply)
	{
		BOOL bExist = FALSE;
		MSG_LIST mlRecordAll;
		g_LocalData.GetMsgList(pFriend->GetId(), CDataManager::USER, mlRecordAll);
		int nCountAll = mlRecordAll.GetCount();

		for(int nCount = nCountAll-1; nCount > 0; nCount--)
		{
			POSITION pos = mlRecordAll.FindIndex(nCount);
			ASSERT(pos);
			if(pos == NULL)
				break;
			XT_MSG &msg = mlRecordAll.GetAt(pos);
			if(msg.from_id == pFriend->GetId())
			{
				POSITION posLast = mlRecordAll.FindIndex(nCountAll-1);
				XT_MSG &msgLast = mlRecordAll.GetAt(posLast);

				uint32 tm = msgLast.send_time - msg.send_time;
				if(tm < 172800)//����һ����¼�Ա����������������Ϊ�ظ���ѯ
				{
					bIsRepeat = true;
					break;
				}
				break;
			}
		}
	}
	pBtnRepeat->SetVisible(bIsRepeat);
	if(nUnReadNum==0)////����δ����Ϣ
	{
		//δ����Ϣ���99��
		pBtnRemind->SetVisible(bIsRemind);
		CString strCount;
		if(m_mapFriendCount[pFriend] >= 99)
			strCount = "99+";
		else
			strCount.Format("%d", m_mapFriendCount[pFriend]++);
		pBtnRemind->SetText(strCount);

		DuiLib::CControlUI* pListItem = static_cast<DuiLib::CControlUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("listitem")));
		pListElement->SetDrawBkColor(0xFFFEE8C0);
		if(!pBtnRemind->IsVisible())///���ɼ�
		{
			m_mapFriendCount[pFriend] = 1;
			pListElement->SetDrawBkColor(NULL);
		}
		//����һ������ʾ��������ʾ��������ɫû�и���ȫ����λ��
		if((DuiLib::CListContainerElementUI*)m_pList->GetItemAt(m_pList->GetCurSel()) != pListElement)
		{
			pListElement->SetVisible(false);
			pListElement->SetVisible(true);
		}
	}
	DuiLib::CLabelUI* nick_name = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("nickname")));
	m_PaintManager.SetDefaultFont("΢���ź�", 12, false, false, false);//Ĭ������
	m_PaintManager.AddFont("΢���ź�", 12, true, false, false);//�������
	
	if (nick_name != NULL)
	{
		str = /*pFriend->nickname;*/UserInfoToStr(pFriend);

		//����18��������ʾ...
		if(str.GetLength() >= 16)
		{
			BYTE bChar = str.GetAt(12);
			if(bChar > 127)
				str = str.Left(12) + "...";
			else
				str = str.Left(13) + "...";
		}
		nick_name->SetFont(0);
		nick_name->SetText(str);
	}

	DuiLib::CLabelUI* historyMsg = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("historyMsg")));
	DuiLib::CLabelUI* historyTime = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("historyTime")));
	MSG_LIST ml1;
	// ��ȡ�û������¼
	g_LocalData.GetLastMsgList(pFriend->GetId(),CDataManager::USER,ml1,5);

	POSITION pos=ml1.GetTailPosition();

	if(pos != NULL)
	{
		XT_MSG msg = ml1.GetAt(pos);

		str = msg.data;

		while(str.Find("<image:",0) != -1)
		{
			int nStart = str.Find("<image:",0);
			int nEnd = str.Find(">",0);
			str.Replace(str.Mid(nStart, nEnd - nStart + 1), "[ͼƬ]");
		}
		if(str.GetLength() >= 16)
		{
			BYTE bChar = str.GetAt(12);
			if(bChar > 127)
				str = str.Left(12) + "...";
			else
				str = str.Left(13) + "...";
		}
		str.Format("  %s", str);
		if (historyMsg != NULL)
		{
			historyMsg->SetText(str);
		}
		//�ж��������ʷ��¼���һ����¼��ʱ�䳬��5�����ж�Ϊ��������ʱ�յ�����Ϣ����ʾ���Ա��
		CTime  tmSendTime(msg.send_time);
		CTime  tmCurrent = /*CTime::GetCurrentTime()*/g_LocalData.GetTimeNow();
		CTimeSpan tm = tmCurrent - tmSendTime;
		if(tm > 30 && !bIsTransfer && nUnReadNum == 0)
		{
			pBtnMessage->SetVisible(true);
		}
		str=tmSendTime.Format("%H:%M");
		if (historyTime != NULL)
		{
			historyTime->SetText(str);
		}
	}
#ifdef _BUSINESS_VESION
	CMainFrame::GetInstance()->Hangup();
#endif
}

void CDuiCounsel::UpdateFriendListElement(FRIEND_INFO* pFriend)
{
	int nIndex = -1;
	for(int nFriend = 0; nFriend < m_vecFriend.size(); nFriend++)
	{
		if(m_vecFriend[nFriend]->GetId() == pFriend->GetId())
		{
			m_vecFriend[nFriend] = pFriend;
			nIndex = nFriend;
			break;
		}
	}
	if(nIndex == -1)
	{
		return;
	}
	DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pList->GetItemAt(nIndex);

	DuiLib::CLabelUI* nick_name = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("nickname")));
	m_PaintManager.SetDefaultFont("΢���ź�", 12, false, false, false);//Ĭ������
	m_PaintManager.AddFont("΢���ź�", 12, true, false, false);//�������

	DuiLib::CDuiString str;
	if (nick_name != NULL)
	{
		str = /*pFriend->nickname;*/UserInfoToStr(pFriend);

		//����18��������ʾ...
		if(str.GetLength() >= 16)
		{
			BYTE bChar = str.GetAt(12);
			if(bChar > 127)
				str = str.Left(12) + "...";
			else
				str = str.Left(13) + "...";
		}
		nick_name->SetFont(0);
		nick_name->SetText(str);
	}
}

void CDuiCounsel::DeleteP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg)
{
	vector</*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg*>::iterator itr = m_vecP2PSendMsgDlg.begin();
	vector<FRIEND_INFO*>::iterator itrFriend = m_vecFriend.begin();
	//vector<DuiLib::CListContainerElementUI*>::iterator itrElement = m_vecListElement.begin();
	for( int nDlg = 0;
		itr != m_vecP2PSendMsgDlg.end(), nDlg < m_vecP2PSendMsgDlg.size(), itrFriend != m_vecFriend.end() /*, itrElement != m_vecListElement.end()*/;
		itr++, itrFriend++/*,itrElement++*/, nDlg++)
	{
		if(*itr == pDlg)
		{
			/*BOOL bExist = FALSE;
			map<FRIEND_INFO*, int>::iterator itrCount = m_mapFriendCount.begin();
			for(; itrCount != m_mapFriendCount.end(); itrCount++)
			{
				if(itrCount->first == *itrFriend)
				{
					itrCount->second++;
					bExist = TRUE;
					break;
				}
			}
			if(!bExist)
			{
				m_mapFriendCount.insert(make_pair(*itrFriend, 1));
			}*/

			m_vecP2PSendMsgDlg.erase(itr);
			m_vecFriend.erase(itrFriend);
			//m_vecListElement.erase(itrElement);
			m_pList->RemoveAt(nDlg);
			//�ر�һ����ѯ����ʾ��һ����ѯ��
			if(/*pDlg != NULL && pDlg->IsIconic() &&*/ nDlg != 0)
			{
				m_bIsUpdate = TRUE;
				m_pList->SelectItem(nDlg-1);
				//m_vecP2PSendMsgDlg[nDlg-1]->ShowWindow(SW_SHOW/*NORMAL*/);
				//m_vecP2PSendMsgDlg[nDlg-1]->CenterWindow();
			}
			else
			{
				// 				if(m_vecListElement.size() > 0)
				// 				{
				// 					m_bIsUpdate = TRUE;
				// 					pList->SelectItem(nDlg);
				// 				}
			}

			if(m_pList->GetCount() == 0)
			{
				ShowWindow(false);
			}
			break;
		}
	}
}
void CDuiCounsel::AddP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg)
{
	//�ж�������ظ�
	for(int nDlg = 0; nDlg < m_vecP2PSendMsgDlg.size(); nDlg++)
	{
		if(m_vecP2PSendMsgDlg[nDlg] == pDlg)
			return;
	}
	m_vecP2PSendMsgDlg.push_back(pDlg);
}

void CDuiCounsel::MoveP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg)
{
	CRect rcCounsel;
	::GetWindowRect(GetHWND(), &rcCounsel);
	
	CRect rcDlg;
	/*pDlg->*/::GetWindowRect(pDlg->GetHWND(), rcDlg);
	/*pDlg->*/::MoveWindow(pDlg->GetHWND(), rcCounsel.right, rcCounsel.top, rcDlg.Width(), rcDlg.Height(), TRUE);
}

void CDuiCounsel::ShowSelectP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlgSelect,int nCntUnRead)
{
	for(int nDlg = 0; nDlg < m_vecP2PSendMsgDlg.size(); nDlg++)
	{
		//δѡ�е����������
		/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg = m_vecP2PSendMsgDlg[nDlg];
		if(pDlgSelect != pDlg)
		{
			DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
			CString strBkImage = background->GetBkImage();

			::ShowWindow(pDlg->GetHWND(), SW_HIDE);
			DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pList->GetItemAt(nDlg);
			DuiLib::CControlUI* pSelectItemColor = static_cast<DuiLib::CControlUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("selectitem")));
			
			pSelectItemColor->SetBkImage(strBkImage);
			pSelectItemColor->SetBkColor(NULL);
			
			DuiLib::CControlUI* pLineItem = static_cast<DuiLib::CControlUI*>(pInstance->m_PaintManager.FindSubControlByName(pListElement, _T("lineitem")));
			pLineItem->SetBkImage(strBkImage);
		}
		else
		{
			BOOL bIsIconic = FALSE;

			for(int nDlgIconic = 0; nDlgIconic < m_vecP2PSendMsgDlg.size(); nDlgIconic++)
			{
				if(/*m_vecP2PSendMsgDlg[nDlgIconic]->IsIconic()*/::IsMinimized(m_vecP2PSendMsgDlg[nDlgIconic]->GetHWND()))
				{
					bIsIconic = TRUE;
					::ShowWindow(m_vecP2PSendMsgDlg[nDlgIconic]->GetHWND(), SW_RESTORE);
					/*m_vecP2PSendMsgDlg[nDlgIconic]->*/::BringWindowToTop(m_vecP2PSendMsgDlg[nDlgIconic]->GetHWND());
					break;
				}
			}

			CRect rcCounsel;
			::GetWindowRect(*this, &rcCounsel);
			CRect rcDlg;
			/*pDlg->*/::GetWindowRect(pDlg->GetHWND(), rcDlg);
			pDlg->ShowWindow(SW_SHOW);
			/*pDlg->*/::MoveWindow(pDlg->GetHWND(), rcCounsel.right, rcCounsel.top, rcDlg.Width(), rcDlg.Height(), TRUE);
			SetTimer(pDlg->GetHWND(), TIMER_EDIT_ENDDOWN, 100, NULL);
			m_pDlgSelectP2PSendMsgDlg = pDlg;
			//pList->SelectItem(nDlg);

			DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pList->GetItemAt(nDlg);
			if(m_vecP2PSendMsgDlg.size() == 0)
			{
				if(CDuiCounsel::GetInstance() != NULL && ::IsWindow(CDuiCounsel::GetInstance()->GetHWND()))
				{
					CDuiCounsel::GetInstance()->Close();////�ر���ѯ��
				}
				return;
			}

			DuiLib::CDuiString str;

			DuiLib::CButtonUI* pBtnMessage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMessage")));
			DuiLib::CButtonUI* pBtnRepeat = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRepeat")));
			DuiLib::CButtonUI* pBtnReply = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnReply")));
			DuiLib::CButtonUI* pBtnTransfer = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnTransfer")));
			/*DuiLib::CButtonUI* */pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));

			DuiLib::CControlUI* pSelectItemColor = static_cast<DuiLib::CControlUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("selectitem")));
			DWORD dwColor = 0xFFCF4C5A;
			pSelectItemColor->SetBkImage(_T(""));
			pSelectItemColor->SetBkColor(dwColor);

			DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
			CString strBkImage = background->GetBkImage();
			DuiLib::CControlUI* pLineItem = static_cast<DuiLib::CControlUI*>(pInstance->m_PaintManager.FindSubControlByName(pListElement, _T("lineitem")));
			pLineItem->SetBkImage(strBkImage);

			if(m_bIsUpdate)
			{
				m_bIsUpdate = FALSE;
			}
			else
			{
				if(nCntUnRead == 0)////û��δ����Ϣ
				{
					pBtnMessage->SetVisible(false);
					pBtnTransfer->SetVisible(false);
					pBtnRepeat->SetVisible(false);
					pBtnRemind->SetVisible(false);
					
					DuiLib::CControlUI* pListItem = static_cast<DuiLib::CControlUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("listitem")));
					pListElement->SetDrawBkColor(NULL);
					//m_nRemindCount = 1;
					//ѡ�����촰��ʱ��Ϣ����Ϊ1
					m_mapFriendCount[m_vecFriend[nDlg] ] = 1;

#ifdef _BUSINESS_VESION
					CMainFrame::GetInstance()->Hangup();
#endif
				}
				

				// 				map<FRIEND_INFO*, int>::iterator itrCount = m_mapFriendCount.begin();
				// 				for(; itrCount != m_mapFriendCount.end(); itrCount++)
				// 				{
				// 					if(itrCount->first == m_vecFriend[nDlg])
				// 					{
				// 						m_mapFriendCount.erase(itrCount);
				// 						break;
				// 					}
				// 				}
			}
			if(m_pList->GetCurSel() != nDlg)
			{
				m_bIsUpdate = TRUE;
				m_pList->SelectItem(nDlg);
			}
		}
	}
}

void CDuiCounsel::ShowAllCounselDlg()
{
	for(int nElement = 0; nElement < /*m_vecListElement.size()*/m_pList->GetCount(); nElement++)
	{
		DuiLib::CListContainerElementUI* pListElement = /*m_vecListElement[nElement]*/(DuiLib::CListContainerElementUI*)m_pList->GetItemAt( nElement );
		pListElement->SetVisible(true);
	}
	//û��ѡ����ѯ����Ĭ��ѡ�е�һ����ѯ��
	if( m_pList->GetCurSel() == -1 && m_vecP2PSendMsgDlg.size() > 0 )
	{
		for(int nDlg = 0; nDlg < m_vecP2PSendMsgDlg.size(); nDlg++)
		{
			if(::IsWindowVisible(m_vecP2PSendMsgDlg[nDlg]->GetHWND()))
			{
				m_bIsUpdate = TRUE;
				m_pList->SelectItem(nDlg);
			}
		}
	}
}

void CDuiCounsel::ShowCounselingDlg()
{
	for(int nElement = 0; nElement < /*m_vecListElement.size()*/m_pList->GetCount(); nElement++)
	{
		DuiLib::CListContainerElementUI* pListElement = /*m_vecListElement[nElement];*/(DuiLib::CListContainerElementUI*)m_pList->GetItemAt(nElement);
		
		DuiLib::CButtonUI* pBtnMessage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMessage")));
		DuiLib::CButtonUI* pBtnRepeat = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRepeat")));
		DuiLib::CButtonUI* pBtnReply = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnReply")));
		/*DuiLib::CButtonUI* */pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
		DuiLib::CButtonUI* pBtnTransfer = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnTransfer")));
		DuiLib::CButtonUI* pBtnMarked = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnMarked")));

		//���ԡ��ظ���ѯ��δ����Ϣ��ת����Ϣ�������ʾ������
		if(!(/*pBtnMessage->IsVisible() || pBtnRepeat->IsVisible() || / *pBtnReply->IsVisible()* /pBtnRemind->IsVisible() || pBtnTransfer->IsVisible() ||*/ pBtnMarked->IsVisible()))
			pListElement->SetVisible(false);
	}
	DuiLib::CListContainerElementUI* pListSelect = (DuiLib::CListContainerElementUI*)m_pList->GetItemAt( m_pList->GetCurSel() );
	
	if( pListSelect == NULL )
	{
		for(int nElement = 0; nElement < m_pList->GetCount(); nElement++)
		{
			DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pList->GetItemAt(nElement);//m_vecListElement[nElement];
			if(pListElement->IsVisible())
			{
				m_bIsUpdate = TRUE;
				ShowSelectP2PSendMsgDlg(m_vecP2PSendMsgDlg[nElement]);
				break;
			}
		}
	}
}

void CDuiCounsel::MoveAllWindow(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlgCurrent)
{
	//���д��ڴ�С�仯����ֹѡ�����������ʱ��С��ԭ
	for(int nDlg = 0; nDlg < m_vecP2PSendMsgDlg.size(); nDlg++)
	{
		/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg = m_vecP2PSendMsgDlg[nDlg];
		if(pDlgCurrent != pDlg)
		{
			CRect rcCurrent;
			/*pDlgCurrent->*/::GetWindowRect(pDlgCurrent->GetHWND(), &rcCurrent);
			/*pDlg->*/::MoveWindow(pDlg->GetHWND(), rcCurrent.left, rcCurrent.top, rcCurrent.Width(), rcCurrent.Height(), TRUE);
		}
	}
}

//���д��ڶ����
void CDuiCounsel::SetAllWindowMax(BOOL bIsMax)
{
	for(int nDlg = 0; nDlg < m_vecP2PSendMsgDlg.size(); nDlg++)
	{
		/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg = m_vecP2PSendMsgDlg[nDlg];
		pDlg->SetWindowMax(bIsMax);
	}
}

////�ر��������촰��,�鿴����δ����Ϣ
BOOL CDuiCounsel::IsPopMsgWarn(int &nUnRead)
{	
	BOOL bUnReadMsg = FALSE;
	nUnRead = 0;
	for(int nElement = 0; nElement <m_pList->GetCount(); nElement++)
	{
		DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pList->GetItemAt(nElement);
	    pBtnRemind = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("btnRemind")));
		if(pBtnRemind->IsVisible())
		{
			bUnReadMsg = TRUE;
			nUnRead++;
			
			CString strTxt = pBtnRemind->GetText();////��ȡ���ζԻ���δ����Ϣ��¼��д����ʱlog;
			int nUnReadCnt = _ttoi(strTxt);
			FRIEND_INFO* pFriend = m_vecFriend[nElement];
			g_mapMsgUnRead[pFriend] = nUnReadCnt;////�洢��ǰ�����δ����Ϣ
		}
	}
	if(!bUnReadMsg)////��ǰ���д��ھ�û��δ����Ϣ
	{
		g_mapMsgUnRead.clear();////ɾ����ʶ
	}
	return bUnReadMsg;
}
	
//�жϴ���Ĵ��ں͵�ǰѡ�еĴ����Ƿ�һ��
BOOL CDuiCounsel::IsCurrentShowDlg(CDuiP2PSendMsgDlg* pDlg)
{
	if(m_pList != NULL)
	{
		for (int nElement = 0; nElement < m_pList->GetCount(); nElement++)
		{
			DuiLib::CListContainerElementUI* pListElement = (DuiLib::CListContainerElementUI*)m_pList->GetItemAt(nElement);
			if(pListElement->IsSelected() && m_vecP2PSendMsgDlg[nElement] == pDlg)
				return TRUE;
		}
	}
	return FALSE;
}

//��ȡ���֮ǰ��λ��
CRect CDuiCounsel::GetPreRc()
{
	return m_preRc;
}

//�������֮ǰ��λ��
void CDuiCounsel::SetPreRc(CDuiP2PSendMsgDlg* pDlg)
{
	CRect rc;
	if(pDlg != NULL)
	{
		
		::GetWindowRect(pDlg->GetHWND(), &rc);
	}
	m_preRc = rc;
}