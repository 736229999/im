// im\ClusterListCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\customlistctrl.h"
#include "MessageHandleWnd.h"
#include "..\FriendInfoDlg.h"
#include "localdata.h"
#include "webprocess.h"
#include "msgdlg.h"
#include "xtpacket.h"
#include "imclusterlistctrl.h"
#include "..\..\SpaceManagement\public\public_controls\ResGdiObj.h"
#include "..\..\HmgSkin\HmgSkin\FlatPopupMenu.h"
#include "AddBlackDlg.h"
//#include "..\..\..\..\im_client\HmgUploadPicture\stdafx.h"
#define CLS_COLOR 0x00FFFFFF//CLS_COLOR RGB(239, 244, 255)
// CClusterListCtrl
#define ID_SEND_P2PMSG   10001
#define ID_FRIEND_INFO   10002
#define ID_FRIEND_ALBUM  10003
#define ID_FRIEND_BLOG   10004
#define ID_FRIEND_MSG    10005
#define ID_FRIEND_ADD    10006
#define ID_CLUSTER_TALK  10007
#define ID_CLUSTER_INFO  10008
#define ID_CLUSTER_PAGE  10009
#define ID_CLUSTER_MSG   10010
#define ID_CLEAR_RECENT	 10011
#define ID_CLUSTER_FRIEND_REMOVE 10012
#define NULL_MSG_LOG     0
IMPLEMENT_DYNAMIC(CListCtrlBase, CListBox)

#include "ClustersettingDlg.h"

CListCtrlBase::CListCtrlBase()
{
	CResBitmap bmp;
	bmp.LoadBitmap(IDB_IM_HEAD20);

	m_ImageListHead.Create(20,20,ILC_COLOR24|ILC_MASK,0,1);
	m_ImageListHead.SetBkColor(RGB(255,255,255));
	m_ImageListHead.Add(&bmp,(CBitmap *)NULL);

	bmp.DeleteObject();
	/*
	bmp.LoadBitmap(IDB_IM_STATUS35);

	m_ImageListDefault.Create(35,35,ILC_COLOR24|ILC_MASK,0,1);
	m_ImageListDefault.SetBkColor(RGB(255,255,255));
	m_ImageListDefault.Add(&bmp,(CBitmap *)NULL);
*/
	m_nLastFindItem = -1;

	m_cxExtent = 0;
}

CListCtrlBase::~CListCtrlBase()
{
}


BEGIN_MESSAGE_MAP(CListCtrlBase, CListBox)
	ON_WM_MEASUREITEM()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FRIEND_INFO, OnFriendInfo)
	ON_COMMAND(ID_SEND_P2PMSG, OnP2PTalk)
	ON_COMMAND(ID_FRIEND_ALBUM, OnFriendAlbum)
	ON_COMMAND(ID_FRIEND_BLOG, OnFriendBlog)
	ON_COMMAND(ID_FRIEND_ADD, OnFriendAdd)
	ON_COMMAND(ID_FRIEND_MSG, OnFriendMsg)
	
	//ON_CONTROL_REFLECT(LBN_DBLCLK, OnLbnDblclk)
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
END_MESSAGE_MAP()

void CListCtrlBase::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CListBox::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
	CMenuEx::MeasureItem(lpMeasureItemStruct);
}


//void CListCtrlBase::OnLbnDblclk()
//{
//	int nSel = GetCurSel();
//	DblClick(nSel);
//}

void CListCtrlBase::DblClick(int nSel)
{
	if ( nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pUser = (FRIEND_INFO *)GetItemData(nSel);
		//CFriendInfoDlg::Show(pUser);

		/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pUser);

		if ( pDlg->GetHWND()==NULL )
			//pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
			pDlg = CDuiP2PSendMsgDlg::NewDlg(pUser,pmSingleTalk, GetDesktopWindow());

		if ( /*pDlg->IsWindowVisible()*/::IsWindowVisible(pDlg->GetHWND())==FALSE )
			pDlg->ShowWindow(/*SW_SHOW*/true);

		if (/*pDlg->IsIconic()*/::IsMinimized(pDlg->GetHWND()))
			pDlg->ShowWindow(/*SW_RESTORE*/true);

		/*pDlg->BringWindowToTop()*/::BringWindowToTop(pDlg->GetHWND());
	}
}

void CListCtrlBase::DrawBitmap(CDC *pDC,int x, int y, CBitmap *pBitmap, bool bBlackWhite)
{
	BITMAP bi;
	pBitmap->GetBitmap(&bi);


	CDC *pSafeDC = GetDC();

	CDC dcSrc ;
	dcSrc.CreateCompatibleDC(pSafeDC);
	CBitmap *pBitmapSave = dcSrc.SelectObject(pBitmap);


	CDC dcDraw;
	dcDraw.CreateCompatibleDC(pSafeDC);

	CBitmap bmpDraw;
	bmpDraw.CreateCompatibleBitmap(pSafeDC,bi.bmWidth,bi.bmHeight);

	CBitmap *pOldbmp = dcDraw.SelectObject(&bmpDraw);

	dcDraw.BitBlt(0,0,bi.bmWidth,bi.bmHeight,&dcSrc,0,0,SRCCOPY);

	if ( bBlackWhite==true )
	{
		BYTE     R,G,B;
		COLORREF clr  ;
		int      temp ;

		for ( int i=0;i<bi.bmWidth; i++ )
		{
			for ( int j=0;j<bi.bmHeight; j++ )
			{
				clr = dcDraw.GetPixel(i,j);

				R   = clr & 0xFF;
				G   = (clr & 0xFF00)>>8;
				B   = (clr & 0xFF0000)>>16;

				temp  =  ((int)R + (int)G + (int)B)/3;

				dcDraw.SetPixel(i,j,RGB(temp,temp,temp));
			}
		}
	}

	pDC->BitBlt(x,y,bi.bmWidth,bi.bmHeight,&dcDraw,0,0,SRCCOPY);

	dcSrc.SelectObject(pBitmapSave);
	dcDraw.SelectObject(pOldbmp);

	ReleaseDC(pSafeDC);
}

int CListCtrlBase::FindItem(int uid)
{
	int nC = GetCount();
	int i;
	FRIEND_INFO * pUser = NULL;
	for(i = 0;i < nC;i++)
	{
		pUser = (FRIEND_INFO *)GetItemData(i);
		if(pUser && pUser->GetId() == uid)
		{
			return i;
		}
	}
	return -1;
}

void CListCtrlBase::PopMenu(int nSel)
{
	if(nSel<0)
		return;
	CMenuEx menu, *popmenu;
	menu.LoadMenu(IDR_MENU_TEMPLATE);
	popmenu = menu.GetSubMenu(0);
	popmenu->RemoveMenu(0,MF_BYPOSITION);

	CString str;

	FRIEND_INFO *pUser = (FRIEND_INFO *)GetItemData(nSel);

	CPoint pt;
	GetCursorPos(&pt);

	if ( pUser->GetId()!=g_ImUserInfo.GetId() )
	{
		str.Format("与 \"%s\" 单独聊天",(pUser->nickname));
		popmenu->AppendMenu(MF_STRING,ID_SEND_P2PMSG,str);	

		popmenu->AppendMenu(MF_SEPARATOR);
		
		str.Format("查看 \"%s\" 的详细资料",(pUser->nickname));
        popmenu->AppendMenu(MF_STRING,ID_FRIEND_INFO,str);	
		
		/*str.Format("查看 \"%s\" 的主页",(pUser->nickname));
		popmenu->AppendMenu(MF_STRING,ID_FRIEND_BLOG,str);	

		str.Format("查看 \"%s\" 的相册",(pUser->nickname));
		popmenu->AppendMenu(MF_STRING,ID_FRIEND_ALBUM,str);	*/

		/*str.Format("将\"%s\"从本群中删除",(pUser->nickname));
		popmenu->AppendMenu(MF_STRING,ID_CLUSTER_FRIEND_REMOVE,str);*/

		if ( FindFriend(pUser->GetId()) )
		{
			popmenu->AppendMenu(MF_SEPARATOR);

			str.Format("和 \"%s\" 的聊天记录",(pUser->nickname));
			popmenu->AppendMenu(MF_STRING,ID_FRIEND_MSG,str);	
		}
		else
		{
			popmenu->AppendMenu(MF_SEPARATOR);
#ifndef _CS_PLAT_EX_
			str.Format("我要加 \"%s\" 为好友",(pUser->nickname));
			popmenu->AppendMenu(MF_STRING,ID_FRIEND_ADD,str);	
#endif
		}
	}
	else////查看自己资料
	{
		/////popmenu->AppendMenu(MF_STRING,ID_SEND_P2PMSG,"我的个人主页");	
		popmenu->AppendMenu(MF_STRING,ID_FRIEND_INFO,"查看我的资料");	
	}

	//popmenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);

	CFlatPopupMenu flatmenu;
	flatmenu.Create(AfxGetInstanceHandle());
	flatmenu.AppendMenu(popmenu);
	UINT id = flatmenu.Track(pt.x, pt.y, GetSafeHwnd(), true);
	// 相应选择菜单的消息相应
	DWORD wP = MAKEWPARAM(id, 0);
	SendMessage(WM_COMMAND, wP, 0);
}

void CListCtrlBase::OnRButtonUp(UINT nFlags, CPoint point)
{
	SCROLLINFO si;
	this->GetScrollInfo(SB_VERT,&si);

	int count = GetCount();

	if ( count>0 )
	{
		int nItemHeight = GetItemHeight(0);

		int nSel = 0;
		
		if ( si.nMax>0 )
			nSel = count*si.nPos/(float)si.nMax+point.y/nItemHeight;
		else
			nSel = point.y / nItemHeight;

		//SetCurSel(nSel);

		if ( nSel>-1 && nSel<count )
		{
			PopMenu(nSel);	
		}
		else
		{
			PopMenu(-1);
		}
	}
	else
	{
		PopMenu(-1);
	}

	CListBox::OnRButtonUp(nFlags, point);
}


void CListCtrlBase::DrawHeadIcon(CDC *pDC, FRIEND_INFO *pUser, CPoint pt, bool bSmall)
{
	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pDC);

	CResBitmap bmpTemp;

	bool bDrawLine=false;
	
	CBitmap *pOldbmp=NULL;

	if ( pUser->GetId()!=(uint32)-1 )
	{	
		CBitmap * pUserBitmap = ::GetUserBitmap(pUser, bSmall);
		if (pUserBitmap)
		{
			BITMAP bmp;
			pUserBitmap->GetBitmap(&bmp);
			if(bmp.bmWidth < 35)
				bmpTemp.CreateCompatibleBitmap(pDC,bmp.bmWidth, bmp.bmHeight);
			else
				bmpTemp.CreateCompatibleBitmap(pDC,35,35);

			pOldbmp = dcTemp.SelectObject(&bmpTemp);

			if ( pUser->status==XTREAM_OFFLINE )
				DrawBitmap(&dcTemp,0,0,pUserBitmap,true);
			else
			{
				DrawBitmap(&dcTemp,0,0,pUserBitmap,false);
/*先不画状态
				CDC *pDC1=&dcTemp;

				if (pUser->status==XTREAM_LEAVE)
				{	//draw status foot

					CDC dcTemp;
					dcTemp.CreateCompatibleDC(pDC);

					CResBitmap bmpTemp;

					int x, y, width, height;
					int top=0;


					x = 6;
					y = 11;

					width  = 13;
					height = 8;

					bmpTemp.LoadBitmap(IDB_STATUS_FOOT20);
					

					CBitmap *pbmpOld = dcTemp.SelectObject(&bmpTemp);

					pDC1->BitBlt(x,y,width,height,&dcTemp,0,top,SRCCOPY);

					dcTemp.SelectObject(pbmpOld);
				}
*/
				
			}

			//bDrawLine =true;
		}
		else
		{
			CBitmap bitmapTemp;
			
			if(bSmall)
				bitmapTemp.CreateCompatibleBitmap(pDC,24,24);
			else
			
				bitmapTemp.CreateCompatibleBitmap(pDC,35,35);
			pOldbmp=dcTemp.SelectObject(&bitmapTemp);

			//m_ImageListDefault.Draw(&dcTemp,GetDefaultImage(pUser->sex,pUser->status),CPoint(0,0),ILD_NORMAL);
			
			int nStatus = 0;
			if(pUser->GetId() == g_ImUserInfo.GetId())
				nStatus = 0;
			else
				nStatus = pUser->status;
			if(bSmall)
				g_imgStatus24.Draw(&dcTemp,GetDefaultImage(pUser->sex,/*pUser->status*/nStatus),CPoint(0,0),ILD_NORMAL);
			else			
				g_imgStatus35.Draw(&dcTemp,GetDefaultImage(pUser->sex,/*pUser->status*/nStatus),CPoint(0,0),ILD_NORMAL);
			

			//g_ImgFaceFrame.Draw(dcTemp, 0, 0, 35, 35);
			//g_ImgIcon.Draw(dcTemp, 0, 0, 35, 35);
		}

	}
	else
	{
		
		//bmpTemp.LoadBitmap(IDB_IM_CLUSTER);
		
		bmpTemp.CreateCompatibleBitmap(pDC,35,35);
		pOldbmp = dcTemp.SelectObject(&bmpTemp);

		//g_ImgFaceFrame.Draw(dcTemp, 0, 0, 35, 35);
		//g_ImgIconGroup.Draw(dcTemp, 0, 0, 35, 35);
		
		g_imgGroupStatus35.Draw(&dcTemp,1,CPoint(0,0),ILD_NORMAL);
	}

	if ( bDrawLine )
	{
		dcTemp.Draw3dRect(0,0,20,20,0xBBCCBB,0xBBCCBB);
	}
	if(bSmall)
		pDC->TransparentBlt(pt.x,pt.y,20,20,&dcTemp,0,0,20,20,RGB(255,255,255));
	else
		pDC->TransparentBlt(pt.x,pt.y,35,35,&dcTemp,0,0,35,35,RGB(255,255,255));

	if ( pOldbmp!=NULL )
		dcTemp.SelectObject(pOldbmp);
}

void CListCtrlBase::DrawBitmap(CDC *pDC,int x, int y, UINT nIDBmp)
{
	CResBitmap bmp;
	bmp.LoadBitmap(nIDBmp);

	BITMAP bi;
	bmp.GetBitmap(&bi);

	CDC dc;

	dc.CreateCompatibleDC(pDC);

	CBitmap *pOldbmp = dc.SelectObject(&bmp);

	TransparentBlt(pDC->m_hDC,x,y,bi.bmWidth,bi.bmHeight,dc.m_hDC,0,0,bi.bmWidth,bi.bmHeight,RGB(0,255,0));
//	pDC->BitBlt(x,y,bi.bmWidth,bi.bmHeight,&dc,0,0,SRCCOPY);
	dc.SelectObject(pOldbmp);
}

void CListCtrlBase::SwapItem(int i, int j)
{
	int count = this->GetCount();
	
	if ( i!=j )
	{
		DWORD dwI = GetItemData(i);
		DWORD dwJ = GetItemData(j);

		CString strI , strJ;
		GetText(i,strI);
		GetText(j,strJ);

		this->DeleteString(i);
		this->InsertString(i,strJ);
		this->SetItemData(i,dwJ);

		this->DeleteString(j);
		this->InsertString(j,strI);
		this->SetItemData(j,dwI);
	}
}

void CListCtrlBase::OnFriendAlbum()
{
	int nSel = GetCurSel();
	if (nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pFriend=(FRIEND_INFO *)GetItemData(nSel);
		ShowAlbum((char*)pFriend->GetUserName().c_str());
	}	
}

void CListCtrlBase::OnFriendAdd()
{
	int nSel = GetCurSel();
	if (nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pFriend=(FRIEND_INFO *)GetItemData(nSel);

		if ( pFriend )
		{
			CAddBlackDlg::Show(pFriend->GetId());
		}
	}
}

void CListCtrlBase::OnFriendMsg()
{
	int nSel = GetCurSel();
	if (nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pFriend=(FRIEND_INFO *)GetItemData(nSel);
		std::tr1::shared_ptr<CDuiMsgDlg> pDlg = CDuiMsgDlg::Show();
		pDlg->SetDefaultFriend(pFriend->GetId());
	}
}

void CListCtrlBase::OnFriendBlog()
{
	int nSel = GetCurSel();
	if (nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pFriend=(FRIEND_INFO *)GetItemData(nSel);
		ShowBlogSite((char*)pFriend->GetUserName().c_str());
	}
}

void CListCtrlBase::OnFriendInfo()
{
	int nSel = GetCurSel();
	if (nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pFriend=(FRIEND_INFO *)GetItemData(nSel);
		if(pFriend->GetId() == g_ImUserInfo.GetId())
		{
			CMainFrame::GetInstance()->OnBnClickedFaceIcon();
		}
		else
		{
			FRIEND_INFO* pGroupFriend = GetFriendInfo(pFriend->GetId());
			if(pGroupFriend != NULL)
				CFriendInfoDlg::NewDlg(pGroupFriend);
			else
				CFriendInfoDlg::NewDlg(pFriend);
		}
	}
}

void CListCtrlBase::OnP2PTalk()
{
	int nSel = GetCurSel();
	if (nSel>-1)
	{
		FRIEND_INFO *pFriend=(FRIEND_INFO *)GetItemData(nSel);

		if ( pFriend->GetId()!=g_ImUserInfo.GetId() )
		{
			/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *pDlg = g_pMessageHandleWnd->GetP2PSendMsgDlg(pFriend);

			if ( pDlg->GetHWND()==NULL )
				//pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
				pDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, GetDesktopWindow());

			if ( /*pDlg->IsWindowVisible()*/::IsWindowVisible(pDlg->GetHWND())==FALSE )
				pDlg->ShowWindow(/*SW_SHOW*/true);

			if (/*pDlg->IsIconic()*/::IsMinimized(pDlg->GetHWND()))
				pDlg->ShowWindow(/*SW_RESTORE*/true);

			/*pDlg->BringWindowToTop()*/::BringWindowToTop(pDlg->GetHWND());
		}
		else
		{
			ShowBlogSite((char*)pFriend->GetUserName().c_str());
		}
	}
}

void CListCtrlBase::OnRButtonDown(UINT nFlags, CPoint point)
{
	SCROLLINFO si;
	this->GetScrollInfo(SB_VERT,&si);
	int count = GetCount();

	if ( count>0 )
	{
		int nItemHeight = GetItemHeight(0);
		int nSel = 0;
		if ( si.nMax>0 )
			nSel = count*si.nPos/(float)si.nMax+point.y/nItemHeight;
		else
			nSel = point.y / nItemHeight;

		SetCurSel(nSel);
	}
	CListBox::OnRButtonDown(nFlags, point);
}

void CListCtrlBase::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nPos;
	if(nChar>=32 && nChar<127)
	{	
		// 从上次查询结果位置开始查询下一个用户
		nPos = FindUser(nChar);
		if(nPos >= 0)
		{
			SetCurSel(nPos);

			return;
		}
	}

	CListBox::OnKeyDown(nChar, nRepCnt, nFlags);
}


int CListCtrlBase::FindUser(char cC)
{
	int i;
	char c;
	FRIEND_INFO * pFriend;
	int nC = GetCount();

	if(m_nLastFindItem != -1)
	{
		i = m_nLastFindItem + 1;
	}
	else
	{
		m_nLastFindItem = 0;
		i = 0;
	}
	for(;i < nC;i++)
	{
		
		pFriend =(FRIEND_INFO*) GetItemData(i);
		c = GetFirstPinyin(pFriend->nickname);
		// 不区分大小写
		if(strnicmp(&cC,&c,1) == 0)
		{
			m_nLastFindItem = i;
			return i;
		}
	}
	if(m_nLastFindItem > nC)
		m_nLastFindItem = nC;
	for(i = 0;i < m_nLastFindItem;i++)
	{
		pFriend =(FRIEND_INFO*) GetItemData(i);
		c = GetFirstPinyin(pFriend->nickname);
		// 不区分大小写
		if(strnicmp(&cC,&c,1) == 0)
		{
			m_nLastFindItem = i;
			return i;
		}
		
	}
	m_nLastFindItem = -1;
	return -1;
}
void CListCtrlBase::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//CListBox::OnChar(nChar, nRepCnt, nFlags);
}

IMPLEMENT_DYNAMIC(CClusterListCtrl, CListCtrlBase)
CClusterListCtrl::CClusterListCtrl()
{
	m_bShowBorder = true;
	m_toolTip.Create(this,TTS_ALWAYSTIP);
	m_nLastToolTip=-1;
	m_bInvalidateLock = false;
}

CClusterListCtrl::~CClusterListCtrl()
{

}


BEGIN_MESSAGE_MAP(CClusterListCtrl, CListCtrlBase)
	ON_WM_MEASUREITEM()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_CLUSTER_FRIEND_REMOVE,OnFriendDel)
END_MESSAGE_MAP()



// 绘制族内成员
void CClusterListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	if(m_bInvalidateLock == TRUE)
	{
		return;
	}

	COLORREF clrSelText = RGB(255,0,0);
	COLORREF clrSelBack = RGB(236,236,236);

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	// draw focus rectangle when no items in listbox
	if (lpDIS->itemID != (UINT)-1)
	{
		int selChange   = lpDIS->itemAction & ODA_SELECT;
		int focusChange = lpDIS->itemAction & ODA_FOCUS;
		int drawEntire  = lpDIS->itemAction & ODA_DRAWENTIRE;

		int nSaveDC =  0;

		char szText[255];
		FRIEND_INFO *pFriend = NULL;
		
		try
		{
			nSaveDC = pDC->SaveDC();
			pFriend = (FRIEND_INFO *)lpDIS->itemData;

			if(*pFriend->profile_name != 0)
			{
				strcpy(szText,pFriend->profile_name);
			}

			else if(*pFriend->comment_name != 0)
			{
				strcpy(szText,pFriend->comment_name);
			}
			else
			{
				strcpy(szText,pFriend->nickname);
			}
		}
		catch(...)
		{
			ASSERT(FALSE);
			return ;
		}

		if (selChange || drawEntire)
		{
			BOOL sel = lpDIS->itemState & ODS_SELECTED;
			int nLen = CListBox::GetTextLen(lpDIS->itemID);

			// 得到绘制区域
			CRect rcItem = lpDIS->rcItem;
			rcItem.InflateRect(-1,-1);

			COLORREF textcolor = sel ? clrSelText : 0;
				COLORREF backgroundcolor = sel ? clrSelBack : CLS_COLOR;

			if ( pFriend->GetId()==g_ImUserInfo.GetId() )
			{
				textcolor = RGB(0,0,255);
				strcat(szText,"(我)");
			}
			else
			{
				if ( pFriend->status!=XTREAM_OFFLINE )
				{
					textcolor=0;
				}
				else
				{
					textcolor=RGB(0xAA,0xAA,0xAA);
				}
			}

			pDC->SetTextColor(textcolor);
			
			pDC->FillSolidRect(&rcItem,backgroundcolor);

			if ( sel )
			{					
				//pDC->Draw3dRect(&rcItem,RGB(107,133,216),RGB(107,133,216));
				pDC->Draw3dRect(&rcItem,clrSelBack,clrSelBack);
			}
			
			pDC->SetBkMode(TRANSPARENT);

			CSize sizeText = pDC->GetTextExtent(szText,strlen(szText));

			if ( sizeText.cy<5 ) 
				sizeText = pDC->GetTextExtent("A",1);
			
			CRect rcText = rcItem;
			rcText.left+=38;
			rcText.top+=(rcItem.Height()-sizeText.cy)/2+1;
			// 获得头像的位置
			CPoint ptFace(17,rcItem.top + (rcItem.Height()-20)/2 );

			if ( pFriend->cluster_property==CMEMBER_CREATOR ) //族长
			{
				DrawBitmap(pDC,1,ptFace.y+2,IDB_CLUSTER_CREATOR);
			}

			if ( pFriend->cluster_property==CMEMBER_ADMIN )//管理员
			{
				DrawBitmap(pDC,1,ptFace.y+2,IDB_CLUSTER_ADMIN);
			}

			DrawHeadIcon(pDC,pFriend,ptFace,true);

			//m_ImageListHead.Draw(pDC,MAKE_HEAD_INDEX(pFriend->face,pFriend->status),ptFace,ILD_NORMAL);


			pDC->DrawText(szText, strlen(szText), &rcText,DT_END_ELLIPSIS|DT_TOP|DT_LEFT);
			
			CSize size;
			size = pDC->GetOutputTextExtent(szText);
			int nScrollBarWidth = ::GetSystemMetrics(SM_CXVSCROLL);
			size.cx += nScrollBarWidth;	// in case of vertical scrollbar

			int cxExtent = (size.cx > m_cxExtent) ? size.cx : m_cxExtent;

			if (cxExtent > m_cxExtent)
			{
				m_cxExtent = cxExtent;
				SetHorizontalExtent(m_cxExtent+(m_cxExtent/32));
			}
		}

		pDC->RestoreDC(nSaveDC);
	}
}

void CClusterListCtrl::InvalidateItem(int nItem)
{
	if ( nItem>-1 && nItem<GetCount() )
	{
		CRect rcItem;
		this->GetItemRect(nItem,&rcItem);
		this->InvalidateRect(&rcItem);
	}
}

BOOL CClusterListCtrl::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(&rc,CLS_COLOR);

	CRect rcWnd;
	GetWindowRect(&rcWnd);

	rcWnd.right-=rcWnd.left;
	rcWnd.left = 0;
	rcWnd.bottom-=rcWnd.top;
	rcWnd.top = 0;

	CDC *pWndDC = GetWindowDC();

	if ( m_bShowBorder )
		pWndDC->Draw3dRect(&rcWnd,IM_LINE_COLOR,IM_LINE_COLOR);
	else
		pWndDC->Draw3dRect(&rcWnd,CLS_COLOR,CLS_COLOR);

	rcWnd.InflateRect(-1,-1);
	pWndDC->Draw3dRect(&rcWnd,CLS_COLOR,CLS_COLOR);

	ReleaseDC(pWndDC);

	return FALSE;
}

void CClusterListCtrl::PreSubclassWindow()
{
	CListCtrlBase::PreSubclassWindow();

	ModifyStyle(0, LBS_OWNERDRAWFIXED);
	this->SetItemHeight(0,24);
}

void CClusterListCtrl::OnNcPaint()
{
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CListCtrlBase::OnNcPaint()
}

void CClusterListCtrl::SortClusterList()
{
	SetInvalidateLock(TRUE);
	int i,j ;

	int nOnline=0, nSuperUser=0, nCount = this->GetCount() ;

	typedef FRIEND_INFO * LPFRIEND_INFO;
	LPFRIEND_INFO * pArray = new LPFRIEND_INFO[nCount];

	for ( i=0; i<nCount; i++ )
	{
		FRIEND_INFO *p1 = (FRIEND_INFO *)GetItemData(i);

		pArray[i]=p1;

		if ( p1->cluster_property!=0 && p1->status!=XTREAM_OFFLINE )
			nSuperUser++;

		if ( p1->status!=XTREAM_OFFLINE )
			nOnline++;	
	}

	this->ResetContent();

	for ( i=0; i<nCount-1; i++ )
	{
		FRIEND_INFO *p1 = pArray[i];

		for ( j=i+1; j<nCount; j++ )
		{
			FRIEND_INFO *p2 = pArray[j];

			if ( p1->status>p2->status )
			{
				FRIEND_INFO * p = pArray[j];
				pArray[j]=pArray[i];
				pArray[i]=p;
			}
		}
	}

	for ( i=0; i<nOnline-1; i++ )
	{
		FRIEND_INFO *p1 = pArray[i];
		
		for ( j=i+1; j<nOnline; j++ )
		{
			FRIEND_INFO *p2 = pArray[j];

			if ( p1->cluster_property<p2->cluster_property )
			{
				FRIEND_INFO * p = pArray[j];
				pArray[j]=pArray[i];
				pArray[i]=p;
			}
		}
	}

	for ( i=nSuperUser; i<nOnline-1; i++ )
	{
		FRIEND_INFO *p1 = pArray[i];
		
		for ( j=i+1; j<nOnline; j++ )
		{
			FRIEND_INFO *p2 = pArray[j];

			if ( strcmp(p1->nickname,p2->nickname)>0 )
			{
				FRIEND_INFO * p = pArray[j];
				pArray[j]=pArray[i];
				pArray[i]=p;
			}
		}
	}

	for ( i=nOnline; i<nCount-1; i++ )
	{
		FRIEND_INFO *p1 = pArray[i];
		
		for ( j=i+1; j<nCount; j++ )
		{
			FRIEND_INFO *p2 = pArray[j];

			if ( strcmp(p1->nickname,p2->nickname)>0 )
			{
				FRIEND_INFO * p = pArray[j];
				pArray[j]=pArray[i];
				pArray[i]=p;
			}
		}
	}

	for ( int i=0; i<nCount; i++ )
	{
		if(pArray[i]->comment_name[0] == 0)
		{
			int iItem=AddString(pArray[i]->nickname);
			this->SetItemData(iItem,(DWORD)pArray[i]);
		}
		else
		{
			int iItem=AddString(pArray[i]->comment_name);
			this->SetItemData(iItem,(DWORD)pArray[i]);
		}

	}

	delete [] pArray;
	KillTimer(TIMER_SORT);
	SetInvalidateLock(FALSE);

}

void CClusterListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	BOOL b;
	int nItem=ItemFromPoint(point,b);
	if(!b)
	{
		if(nItem!=m_nLastToolTip)
		{
			if(m_pCluster==NULL)
				return ;
			CString s;
			FRIEND_INFO * pUser= (FRIEND_INFO*)GetItemData(nItem);
			if(pUser==NULL)
				return;
			if(pUser->cluster_property==CMEMBER_CREATOR)
			{
				s="创建者";
			}
			else if(pUser->cluster_property==CMEMBER_ADMIN)
			{
				s="管理员";
			}
			else
			{
				s=pUser->nickname;
			}
			ShowToolTip(s);
			m_nLastToolTip=nItem;
		}
	}

	CListCtrlBase::OnMouseMove(nFlags, point);
}

BOOL CClusterListCtrl::PreTranslateMessage(MSG* pMsg)
{
	m_toolTip.RelayEvent(pMsg);
	return CListCtrlBase::PreTranslateMessage(pMsg);
}

void CClusterListCtrl::ShowToolTip(const CString& sToolTip)
{
	if(m_toolTip.GetToolCount()==0)
	{
		m_toolTip.AddTool(this,sToolTip);
		// 设置出现提示需要的时间长度
		m_toolTip.SetDelayTime(TTDT_INITIAL,100);
		// 设置提示持续时间
		m_toolTip.SetDelayTime(TTDT_AUTOPOP,10000);
	}
	m_toolTip.UpdateTipText(sToolTip,this ); 
}


void CClusterListCtrl::OnFriendDel()
{
	int nSel = GetCurSel();
	if (nSel>-1 && nSel<GetCount() )
	{
		////////////////////////////////////////////////////////////
		FRIEND_INFO *pFriend=(FRIEND_INFO *)GetItemData(nSel);
		CString sMsg = _T("");
		sMsg.Format("确定要删除群成员\"%s\"吗？",pFriend->nickname);

		if ( pFriend->GetId()==g_ImUserInfo.GetId() )
		{
			ShowMessage("您不能删除自己。");
			return ;
		}
		WORD nCount=GetCount();//总行数
		// 如果成员小于规定个数,解散
		if(nCount<=MIN_CLUSTER_MEMBER)
		{
			sMsg.Format("群\"%s\"的成员个数如果少于%d个的话,将会被系统自动解散!确定要删除成员\"%s\"吗?",
				m_pCluster->szFamilyName,
				MIN_CLUSTER_MEMBER,
				pFriend->nickname);
			if ( CMessageBox::Model(m_hWnd,sMsg,IM_CAPTION,MB_OKCANCEL)==IDOK )
			{
				XT_CLUSTER_DISMISS_REQ req;
				req.nFamilyID = m_pCluster->nFamilyID;
				req.nFamilyProp = PERM_FAMILY;
				g_pProtocol->SendReq(req);

				//清空当前列表
				ResetContent();
			   // 删除主界面群列表对应的项
				::SendMessage(CMainFrame::GetInstance()->GetHWND(), WM_IMOPERATION, OPR_DEL_CLUSTER,(LPARAM)m_pCluster->nFamilyID);
				// 退出
				OnClose();
			}
		}
		else
		{
			sMsg.Format("确定要删除群成员\"%s\"吗？",pFriend->nickname);

			if (CMessageBox::Model(GetSafeHwnd(),sMsg,IM_CAPTION,MB_YESNO)==IDYES )
			{
				XT_SET_CMEMBERS_REQ req;

				req.nFamilyID = m_pCluster->nFamilyID;
				req.nAction     = CMEMBER_DEL;//
				req.nFamilyProp = m_pCluster->nFamilyProp;
				req.nCount = 1;
				req.nMemID[0] = pFriend->GetId();
				g_pProtocol->SendReq(req);	
				/////在ClusterPanel.CPP中响应ACK操作
			}
		}
	}
}

/////刷新列表
void CClusterListCtrl::ImGetClusterList()
{
	XT_GET_CLIST_REQ req;

	req.nFamilyProp = PERM_FAMILY;
	req.nCount=0;
	g_pProtocol->SendReq(req);
}

void CClusterListCtrl::PopMenu(int nSel)
{
	if(nSel<0)
		return;
	CMenuEx menu, *popmenu;
	menu.LoadMenu(IDR_MENU_TEMPLATE);
	popmenu = menu.GetSubMenu(0);
	popmenu->RemoveMenu(0,MF_BYPOSITION);

	CString str;

	FRIEND_INFO *pUser = (FRIEND_INFO *)GetItemData(nSel);

	CPoint pt;
	GetCursorPos(&pt);

	if ( pUser->GetId()!=g_ImUserInfo.GetId() )
	{
		str.Format("与 \"%s\" 单独聊天",(pUser->nickname));
		popmenu->AppendMenu(MF_STRING,ID_SEND_P2PMSG,str);	

		popmenu->AppendMenu(MF_SEPARATOR);
		
		str.Format("查看 \"%s\" 的详细资料",(pUser->nickname));
        popmenu->AppendMenu(MF_STRING,ID_FRIEND_INFO,str);	
		
		/*str.Format("查看 \"%s\" 的主页",(pUser->nickname));
		popmenu->AppendMenu(MF_STRING,ID_FRIEND_BLOG,str);	

		str.Format("查看 \"%s\" 的相册",(pUser->nickname));
		popmenu->AppendMenu(MF_STRING,ID_FRIEND_ALBUM,str);	*/

		//组内群发不支持群成员删除
		if(m_pCluster->nFamilyProp != TEMP_FAMILY)
		{
			str.Format("将\"%s\"从本群中删除",(pUser->nickname));
			popmenu->AppendMenu(MF_STRING,ID_CLUSTER_FRIEND_REMOVE,str);
		}

		if ( FindFriend(pUser->GetId()) )
		{
			popmenu->AppendMenu(MF_SEPARATOR);

			str.Format("和 \"%s\" 的聊天记录",(pUser->nickname));
			popmenu->AppendMenu(MF_STRING,ID_FRIEND_MSG,str);	
		}
		else
		{
			popmenu->AppendMenu(MF_SEPARATOR);
#ifndef _CS_PLAT_EX_
			str.Format("我要加 \"%s\" 为好友",(pUser->nickname));
			popmenu->AppendMenu(MF_STRING,ID_FRIEND_ADD,str);	
#endif
		}
		

	}
	else////查看自己资料
	{
		/////popmenu->AppendMenu(MF_STRING,ID_SEND_P2PMSG,"我的个人主页");	
		popmenu->AppendMenu(MF_STRING,ID_FRIEND_INFO,"查看我的资料");	
	}

	//popmenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);

	CFlatPopupMenu flatmenu;
	flatmenu.Create(AfxGetInstanceHandle());
	flatmenu.AppendMenu(popmenu);
	UINT id = flatmenu.Track(pt.x, pt.y, GetSafeHwnd(), true);
	// 相应选择菜单的消息相应
	DWORD wP = MAKEWPARAM(id, 0);
	SendMessage(WM_COMMAND, wP, 0);
}

//最近联系人
IMPLEMENT_DYNAMIC(CRecentListCtrl, CListCtrlBase)

CRecentListCtrl::CRecentListCtrl()
{
	m_nMaxCount = MAX_RECENT_USER;
}

CRecentListCtrl::~CRecentListCtrl()
{
	ClearRecentList();	
}


BEGIN_MESSAGE_MAP(CRecentListCtrl, CListCtrlBase)
	ON_WM_DESTROY()
	ON_XT_MESSAGE()
	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_CLUSTER_TALK,OnClusterTalk)
	ON_COMMAND(ID_CLUSTER_INFO,OnClusterInfo)
	ON_COMMAND(ID_CLUSTER_PAGE,OnClusterPage)
	ON_COMMAND(ID_CLUSTER_MSG,OnClusterMsg)
	ON_CONTROL_REFLECT(LBN_DBLCLK, OnLbnDblclk)
	ON_COMMAND(ID_CLEAR_RECENT,OnClearRecent)
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CRecentListCtrl)
	// 好友详细资料获取
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoGet)
	// 好友状态改变通知
	ON_XT_COMMAND(CMD_FRIEND_STATUS_IND,OnUserStatusInd)
	ON_XT_COMMAND(CMD_STATUS_GET_ACK,OnStatusGetAck)
END_XT_MESSAGE_MAP()


void CRecentListCtrl::Load()
{
	ResetContent();
	ClearRecentList();

	g_LocalData.GetRecentFriends(m_flRecent);

	FRIEND_LIST::iterator it;
	int n=0;
	
	for ( it=m_flRecent.begin(); it!=m_flRecent.end(); it++ )
	{
		FRIEND_INFO *pUser = *it;

		InsertString(n,pUser->nickname);
		SetItemData(n,(DWORD)pUser);

		// 获取好友状态
		XT_STATUS_GET_REQ req;
		req.id = g_ImUserInfo.GetId();
		req.fid = pUser->GetId();
		g_pProtocol->SendReq(req);
	}
}

void CRecentListCtrl::Save()
{
	g_LocalData.SaveRecentFriends(m_flRecent);
}

void CRecentListCtrl::UpdateUserList()
{
	FRIEND_LIST::iterator it;

	bool bInvalidate=false;

	for ( it=m_flRecent.begin(); it!=m_flRecent.end(); it++ )
	{
		FRIEND_INFO *pUser= (*it);
		bool bDelItem=false;

		if (pUser->GetId()==(uint32)-1 )
		{
			tagCluster c;
			GetCluster(pUser,c);

			if ( ::FindClusterInfo(c.cid)==NULL )
			{
				bDelItem=true;
			}
		}
		else
		{
			FRIEND_INFO *pActive = FindFriend(pUser->GetId());

			if (pActive)
			{
				memcpy(pUser,pActive,sizeof(XT_USER_INFO));
				bInvalidate = true;
			}
			else
			{
				bDelItem = true;
			}
		}

		if ( bDelItem )
		{
			int count=GetCount();
			for ( int i=0; i<count; i++ )
			{
				if ( GetItemData(i)==(DWORD)pUser )
				{
					DeleteString(i);
					bInvalidate = true;
					break;
				}
			}
		}
	}

	if ( bInvalidate )
		Invalidate();

}

void CRecentListCtrl::UpdateUser(FRIEND_INFO *pUser)
{
	int nItem;
	FRIEND_INFO *pItem=NULL;

	if (FindUser(pUser->GetId(),&pItem,&nItem))
	{
		//DWORD dwTime = pItem->reserved;
        memcpy(pItem,pUser,sizeof(FRIEND_INFO));
		//pItem->reserved=dwTime;
		InvalidateItem(nItem);
	}
}

void CRecentListCtrl::UpdateCluster(CLUSTER_INFO *pCluster)
{
	int nItem;
	FRIEND_INFO *pItem=NULL;

	if (FindCluster(pCluster->nFamilyID,&pItem,&nItem))
	{
		tagCluster c;
		c.cid   = pCluster->nFamilyID;
		strcpy(c.name, pCluster->szFamilyName);
		strcpy(c.domain, pCluster->szFamilyDomain);

		SetCluster(pItem, c);

		InvalidateItem(nItem);
	}
}

void CRecentListCtrl::AddCluster(CLUSTER_INFO *pCluster)
{
	FRIEND_LIST::iterator it;

	int nItem;
	FRIEND_INFO *pItem=NULL;

	tagCluster c;
	c.cid             = pCluster->nFamilyID;
	strcpy(c.domain, pCluster->szFamilyDomain);
	strcpy(c.name,   pCluster->szFamilyName);


	bool bFind = FindCluster(pCluster->nFamilyID,&pItem,&nItem);
	int nC=0;
	if ( bFind==false )
	{
		// 获得当前联系人的个数
		nC = GetCount();
		ASSERT(m_nMaxCount>=0);
		if(nC >= m_nMaxCount)
		{
			FRIEND_INFO *pFriend = (FRIEND_INFO*)GetItemData(nC - 1);
			if(pFriend)
			{
				FRIEND_LIST::iterator it,itE;
				itE = m_flRecent.end();
				for(it = m_flRecent.begin();it != itE;it++)
				{
					if(*it == pFriend)
					{
						delete pFriend;
						m_flRecent.erase(it);
						break;
					}
				}
			}
			// 如果超过规定个数，则删除最后一个
			DeleteString(nC-1);

			//// 如果超过规定个数，则删除最后一个
			//
			//DeleteString(nC-1);
			//FRIEND_LIST::iterator it=m_flRecent.end();
			//it--;
			//if(*it)
			//{
			//	delete *it;
			//}
			//m_flRecent.erase(it);
			//m_flRecent.pop_back();	
		}

		FRIEND_INFO *pRecent = new FRIEND_INFO();
		pRecent->SelfEncrypt();
		pRecent->SetId((uint32)-1);
		pRecent->reserved = /*time(0)*/g_LocalData.GetTimeNow();


		SetCluster(pRecent, c);

		m_flRecent.push_back(pRecent);

		int nRecent=InsertString(0,c.name);
		SetItemData(nRecent,(DWORD)pRecent);
	}
	else
	{
		pItem->reserved = /*time(0)*/g_LocalData.GetTimeNow();

		SetCluster(pItem, c);
	
		if ( nItem==0 )
		{
			InvalidateItem(nItem);
		}
		else
		{
			this->DeleteString(nItem);
			InsertString(0,c.name);
			SetItemData(0,(DWORD)pItem);
		}
	}
}

void CRecentListCtrl::AddUser(FRIEND_INFO *pUser)
{
	FRIEND_LIST::iterator it;

	int nItem;
	FRIEND_INFO *pItem=NULL;
	int nC=0;

	bool bFind = FindUser(pUser->GetId(),&pItem,&nItem);

	if ( bFind==false )
	{
		// 获得当前联系人的个数
		nC = GetCount();
		ASSERT(m_nMaxCount>=0);
		if(nC >= m_nMaxCount)
		{
			FRIEND_INFO *pFriend = (FRIEND_INFO*)GetItemData(nC - 1);
			if(pFriend)
			{
				FRIEND_LIST::iterator it,itE;
				itE = m_flRecent.end();
				for(it = m_flRecent.begin();it != itE;it++)
				{
					if(*it == pFriend)
					{
						delete pFriend;
						m_flRecent.erase(it);
						break;
					}
				}
			}
			// 如果超过规定个数，则删除最后一个
			DeleteString(nC-1);
			
			/*
			it--;
			if(*it)
			{
				delete *it;
			}
			m_flRecent.erase(it);*/
			//m_flRecent.pop_back();	
		}
		FRIEND_INFO *pRecent = new FRIEND_INFO();
		memcpy(pRecent,pUser,sizeof(FRIEND_INFO));

		pRecent->reserved = /*time(0)*/g_LocalData.GetTimeNow();
		m_flRecent.push_back(pRecent);
		
		int nRecent=InsertString(0,pRecent->nickname);
		SetItemData(nRecent,(DWORD)pRecent);
	}
	else
	{
		memcpy(pItem,pUser,sizeof(FRIEND_INFO));
		pItem->reserved = /*time(0)*/g_LocalData.GetTimeNow();

		if ( nItem==0 )
		{
			InvalidateItem(nItem);
		}
		else
		{
			this->DeleteString(nItem);
			InsertString(0,pItem->nickname);
			SetItemData(0,(DWORD)pItem);
		}
	}
}

void CRecentListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	COLORREF clrSelBack = RGB(233,233,255);

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	// draw focus rectangle when no items in listbox
	if (lpDIS && lpDIS->itemID != (UINT)-1 && pDC )
	{
		int selChange   = lpDIS->itemAction & ODA_SELECT;
		int focusChange = lpDIS->itemAction & ODA_FOCUS;
		int drawEntire  = lpDIS->itemAction & ODA_DRAWENTIRE;

		int nSaveDC     = pDC->SaveDC();

		if (selChange || drawEntire)
		{
			BOOL sel = lpDIS->itemState & ODS_SELECTED;
			FRIEND_INFO *pFriend = (FRIEND_INFO *)lpDIS->itemData;

			char szText[255];

			if ( pFriend->GetId()!=(uint32)-1 )
			{
				strcpy(szText,pFriend->nickname);
			}
			else
			{
				tagCluster c;
				GetCluster(pFriend,c);
				/* strcpy(szText,c.name);*/
				/////增加群查找赋值/////////////////////////////
				CLUSTER_INFO *pCluster = FindClusterInfo(c.cid);
				if ( pCluster )
				{
					strcpy(szText,pCluster->szFamilyName);
				}
			}

			//好友昵称显示
			if ( strcmp(szText,"")==0 )
			{
				strcpy(szText,"-");
			}
			pDC->SetTextColor(0);
			pDC->FillSolidRect(&lpDIS->rcItem,RGB(255,255,255));
			if ( sel )
			{					
				pDC->FillSolidRect(&lpDIS->rcItem,RGB(233,222,255));
				pDC->Draw3dRect(&lpDIS->rcItem,RGB(107,133,216),RGB(107,133,216));
			}
			pDC->SetBkMode(TRANSPARENT);
			CSize sizeText = pDC->GetTextExtent(szText,strlen(szText));
			CRect rcText = lpDIS->rcItem;
			rcText.left += 50;
			rcText.top += ( ( lpDIS->rcItem.bottom - lpDIS->rcItem.top ) / 2 - sizeText.cy ) / 2 + 1;
			pDC->DrawText(szText, strlen(szText), &rcText,DT_END_ELLIPSIS|DT_TOP|DT_LEFT);

			//头像显示
			CPoint ptFace(3, lpDIS->rcItem.top + (lpDIS->rcItem.bottom - lpDIS->rcItem.top - 35) / 2);
			DrawHeadIcon(pDC,pFriend,ptFace,false);


			//最后一次聊天时间显示
			pDC->SetTextColor(RGB(122,122,122));
			CTime tm(pFriend->reserved);
			uint32 now = /*time(0)*/g_LocalData.GetTimeNow();
			if(now- pFriend->reserved < 24*60*60 && now- pFriend->reserved > 0)
			{
				sprintf(szText," %s",tm.Format("%H:%M"));
			}
			else if(now- pFriend->reserved > 24*60*60 && now- pFriend->reserved < 24*60*60*2)
			{
				sprintf(szText," %s",tm.Format("昨天"));
			}
			else
			{
				sprintf(szText," %s",tm.Format("%m-%d"));
			}
			//sprintf(szText," (最近联系时间 %s)",tm.Format("%Y-%m-%d %H:%M:%S"));
			rcText.left += sizeText.cx;
			rcText.left -= 2;
			pDC->DrawText(szText, strlen(szText), &rcText,DT_END_ELLIPSIS|DT_TOP|DT_RIGHT);


			//最后一次聊天记录显示
			MSG_LIST ml1;
			g_LocalData.GetLastMsgList(pFriend->GetId(),CDataManager::USER,ml1,1);
			POSITION pos = ml1.GetTailPosition();
			if(pos == NULL_MSG_LOG)//记录为空
			{
				return ;
			}
			sprintf(szText," %s", ml1.GetAt(pos).data);
			rcText = lpDIS->rcItem;
			rcText.left += 50;
			rcText.top += 24;
			pDC->DrawText(szText, strlen(szText), &rcText,DT_END_ELLIPSIS|DT_TOP|DT_LEFT);

		
			CSize size;
			size = pDC->GetOutputTextExtent(szText);
			int nScrollBarWidth = ::GetSystemMetrics(SM_CXVSCROLL);
			size.cx += nScrollBarWidth;	// in case of vertical scrollbar
			int cxExtent = (size.cx > m_cxExtent) ? size.cx : m_cxExtent;
			if (cxExtent > m_cxExtent)
			{
				m_cxExtent = cxExtent;
				SetHorizontalExtent(m_cxExtent+(m_cxExtent/32));
			}
			
		}

		pDC->RestoreDC(nSaveDC);
	}
}

void CRecentListCtrl::InvalidateItem(int nItem)
{
	CRect rcItem;
	GetItemRect(nItem,&rcItem);
	InvalidateRect(&rcItem);
}

void CRecentListCtrl::OnUserInfoGet(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_USERINFO_GET_ACK ack = (LPXT_USERINFO_GET_ACK)data;
		
		FRIEND_INFO *pUser=NULL;
		int nItem;
		FindUser(ack->GetId(),&pUser,&nItem);

		if ( pUser )
		{
			UpdateUserFullInfo(pUser,ack);
			FRIEND_INFO f= *pUser;
			//memcpy(&f,ack,sizeof(XT_USER_INFO));
			UpdateUser(&f);
		}
	}
}

void CRecentListCtrl::DblClick(int nSel)
{
	if ( nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pItem = (FRIEND_INFO *)GetItemData(nSel);

		if ( pItem->GetId()!=(uint32)-1 )
			CListCtrlBase::DblClick(nSel);
		else
		{
			tagCluster c;
			GetCluster(pItem,c);
			
			CLUSTER_INFO *pCluster = FindClusterInfo(c.cid);

			if ( pCluster )
			{
				CClusterSendMsgDlg *pDlg = g_pMessageHandleWnd->GetClusterSendMsgDlg(pCluster);

				if ( pDlg->m_hWnd==NULL )
					pDlg->Create(CClusterSendMsgDlg::IDD,GetDesktopWindow());

				pDlg->ShowWindow(SW_SHOW);

				if ( pDlg->IsIconic() )
					pDlg->ShowWindow(SW_RESTORE);

				pDlg->BringWindowToTop();
			}
        }
	}
}

void CRecentListCtrl::OnClusterInfo()
{
	int nSel = GetCurSel();

	if ( nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pItem = (FRIEND_INFO *)GetItemData(nSel);

		if ( pItem->GetId()==(uint32)-1 )
		{
            tagCluster c;
			GetCluster(pItem,c);
			
			CLUSTER_INFO *pCluster = FindClusterInfo(c.cid);

			if ( pCluster )
				CClusterSettingDlg::Show(pCluster);
		}
	}
}

void CRecentListCtrl::OnClusterTalk()
{
	int nSel = GetCurSel();

	if ( nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pItem = (FRIEND_INFO *)GetItemData(nSel);

		if ( pItem->GetId()==(uint32)-1 )
		{
            tagCluster c;
			GetCluster(pItem,c);
			
			CLUSTER_INFO *pCluster = FindClusterInfo(c.cid);

			if ( pCluster )
			{
				CClusterSendMsgDlg *pDlg = g_pMessageHandleWnd->GetClusterSendMsgDlg(pCluster);

				if ( pDlg->m_hWnd==NULL )
					pDlg->Create(CClusterSendMsgDlg::IDD,GetDesktopWindow());

				pDlg->ShowWindow(SW_SHOW);

				if ( pDlg->IsIconic() )
					pDlg->ShowWindow(SW_RESTORE);

				pDlg->BringWindowToTop();
			}
		}
	}
}

void CRecentListCtrl::OnClusterMsg()
{
	int nSel = GetCurSel();

	if ( nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pItem = (FRIEND_INFO *)GetItemData(nSel);

		if ( pItem->GetId()==(uint32)-1 )
		{
            tagCluster c;
			GetCluster(pItem,c);
			
			CLUSTER_INFO *pCluster = FindClusterInfo(c.cid);

			if ( pCluster )
			{
				std::tr1::shared_ptr<CDuiMsgDlg> pDlg = CDuiMsgDlg::Show();
				pDlg->SetDefaultCluster(pCluster->nFamilyID);
			}
		}
	}
}

void CRecentListCtrl::OnClusterPage()
{
	int nSel = GetCurSel();

	if ( nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pItem = (FRIEND_INFO *)GetItemData(nSel);

		if ( pItem->GetId()==(uint32)-1 )
		{
            tagCluster c;
			GetCluster(pItem,c);
			
			CLUSTER_INFO *pCluster = FindClusterInfo(c.cid);

			if ( pCluster && (g_pWebConfig != NULL))
			{
				MiniOpenUrl((LPCSTR)g_pWebConfig->GetWebPageUrl(FAMILY_HOME,StrToWideStr(pCluster->szFamilyDomain)));
			}
		}
	}
}

void CRecentListCtrl::PopMenu(int nSel)
{
	CPoint pt;
	GetCursorPos(&pt);
	if ( nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pItem = (FRIEND_INFO *)GetItemData(nSel);

		if ( pItem->GetId()!=(uint32)-1 )
			CListCtrlBase::PopMenu(nSel);
		else
		{
			CMenuEx menu, *popmenu;
			menu.LoadMenu(IDR_MENU_TEMPLATE);
			popmenu = menu.GetSubMenu(0);
			popmenu->RemoveMenu(0,MF_BYPOSITION);

			CString str;

			tagCluster c;
			GetCluster(pItem,c);

			CLUSTER_INFO *pCluster = FindClusterInfo(c.cid);

			if ( pCluster )
			{
				str.Format("进入群 \"%s\" 的聊天室",pCluster->szFamilyName);
				popmenu->AppendMenu(MF_STRING,ID_CLUSTER_TALK,str);	

				popmenu->AppendMenu(MF_SEPARATOR);

				str.Format("查看群 \"%s\" 资料",pCluster->szFamilyName);
				popmenu->AppendMenu(MF_STRING,ID_CLUSTER_INFO,str);	

				/*str.Format("查看群 \"%s\" 的主页",pCluster->szFamilyName);
				popmenu->AppendMenu(MF_STRING,ID_CLUSTER_PAGE,str);	
			
				popmenu->AppendMenu(MF_SEPARATOR);*/

				popmenu->AppendMenu(MF_STRING,ID_CLUSTER_MSG,"历史聊天记录");	
			
				popmenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
			}
		}
	}
	else
	{
		CMenuEx menu, *popmenu;
		menu.LoadMenu(IDR_MENU_TEMPLATE);
		popmenu = menu.GetSubMenu(0);
		popmenu->RemoveMenu(0,MF_BYPOSITION);
		popmenu->AppendMenu(MF_STRING,ID_CLEAR_RECENT,"清空列表");	
		popmenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
	}
}

void CRecentListCtrl::OnUserStatusInd(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_STATUS_IND ind =(LPXT_STATUS_IND)data;
		FRIEND_INFO *pFriend = NULL;

		int nItem;
		
		FindUser(ind->fid,&pFriend,&nItem);

		//如果是隐身的话，则状态图标和离开一样
		if ( ind->status==XTREAM_HIDE )
		{
			ind->status=XTREAM_OFFLINE;
		}

		if ( pFriend )
		{
			pFriend->ver    = ind->version;
			pFriend->status = ind->status;

			InvalidateItem(nItem);
		}
	}
}


void CRecentListCtrl::ClearRecentList()
{
	FRIEND_LIST::iterator it;

	for ( it=m_flRecent.begin(); it!=m_flRecent.end(); it++ )
	{
		delete (*it);
	}

	m_flRecent.clear();
}

void CRecentListCtrl::PreSubclassWindow()
{
	CListCtrlBase::PreSubclassWindow();

	ModifyStyle(0, LBS_OWNERDRAWFIXED);
	
	SetItemHeight(0,50);
	
	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,m_hWnd);
	g_pProtocol->RegistCmd(CMD_FRIEND_STATUS_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_STATUS_GET_ACK,m_hWnd);
}

void CRecentListCtrl::OnDestroy()
{
	g_pProtocol->UnRegistCmd(m_hWnd);
	g_LocalData.SaveRecentFriends(m_flRecent);
	CListCtrlBase::OnDestroy();
}

bool CRecentListCtrl::FindUser(uint32 fid, FRIEND_INFO **ppFriend, int *pnItem)
{
	int count=GetCount();

	for ( int i=0; i<count; i++ )
	{
		FRIEND_INFO *pUser=(FRIEND_INFO *)GetItemData(i);

		if ( pUser->GetId()==fid )
		{
			*pnItem   = i;
			*ppFriend = pUser;

			return true;
		}
	}

	return false;
}

bool CRecentListCtrl::FindCluster(uint32 cid, FRIEND_INFO **ppFriend, int *pnItem)
{
	int count=GetCount();

	for ( int i=0; i<count; i++ )
	{
		FRIEND_INFO *pUser=(FRIEND_INFO *)GetItemData(i);

		if ( pUser->GetId()==(uint32)-1 )
		{
			tagCluster c;
			GetCluster(pUser, c);

			if ( c.cid==cid )
			{
				*pnItem	   = i;
				*ppFriend  = pUser; 
				return true;
			}
		}
	}

	return false;
}

void CRecentListCtrl::GetCluster(FRIEND_INFO *pFriend, tagCluster & c)
{
	CRecvPacket rp(pFriend->description,sizeof(pFriend->description));
	rp>>c.cid>>c.domain>>c.name;
}


void CRecentListCtrl::SetCluster(FRIEND_INFO *pFriend, const tagCluster & c)
{
	CSendPacket sp(pFriend->description);
	sp<<c.cid<<c.domain<<c.name;
}

void CRecentListCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CListCtrlBase::OnKillFocus(pNewWnd);

	if ( GetCount()>0 )
		SetCurSel(-1);
}



void CRecentListCtrl::OnLbnDblclk()
{
	int nSel = GetCurSel();

	DblClick(nSel);
}


void CRecentListCtrl::OnClearRecent()
{
	ClearRecentList();
	int nC=GetCount();
	for(int i=0;i<nC;i++)
	{
		DeleteString(0);
	}

}



void CRecentListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	CListCtrlBase::OnLButtonUp(nFlags, point);
	UpdateUserList();
}

// 获取最近联系人
const FRIEND_LIST* CRecentListCtrl::GetRecentFriend()
{
	return &m_flRecent;
}

// 获取好友状态
void CRecentListCtrl::OnStatusGetAck(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_STATUS_GET_ACK ack = (LPXT_STATUS_GET_ACK)data;

		if ( ack )
		{
			FRIEND_INFO *pFriend = NULL;
			int nItem;
			FindUser(ack->id,&pFriend,&nItem);

			if ( pFriend )
			{
				if ( ack->status==XTREAM_HIDE )
				{
					ack->status=XTREAM_OFFLINE;
				}			
				pFriend->ip     = ack->ip;
				pFriend->port   = ack->port;
				pFriend->localIP = ack->lanip;
				pFriend->localPort = ack->lanport;
				pFriend->status = ack->status;
				pFriend->ver    = ack->ver;
				memcpy(pFriend->key,ack->p2pkey,16);
				pFriend->status = ack->status;

				InvalidateItem(nItem);
			}
		}
	}
}


IMPLEMENT_DYNAMIC(CSearchListCtrl, CListCtrlBase)
CSearchListCtrl::CSearchListCtrl()
{
	m_bShowBorder = true;
	m_toolTip.Create(this,TTS_ALWAYSTIP);
	m_nLastToolTip=-1;
	m_bInvalidateLock = false;
	m_dwCount = 0;
	m_bChange[0] = FALSE;
	m_bChange[1] = FALSE;
	m_bChange[2] = FALSE;
}

CSearchListCtrl::~CSearchListCtrl()
{
}
BEGIN_MESSAGE_MAP(CSearchListCtrl, CListCtrlBase)
	//ON_WM_MEASUREITEM()
	ON_WM_ERASEBKGND()
	//ON_WM_NCPAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_DRAWITEM()
	ON_CONTROL_REFLECT(LBN_DBLCLK, OnLbnDblclk)
	ON_COMMAND(ID_CLUSTER_TALK,OnClusterTalk)
	ON_COMMAND(ID_CLUSTER_INFO,OnClusterInfo)
	ON_COMMAND(ID_CLUSTER_MSG,OnClusterMsg)
	ON_COMMAND(ID_CLEAR_RECENT,OnClearRecent)
END_MESSAGE_MAP()

void CSearchListCtrl::InvalidateItem(int nItem)
{
	if ( nItem>-1 && nItem<GetCount() )
	{
		CRect rcItem;
		this->GetItemRect(nItem,&rcItem);
		this->InvalidateRect(&rcItem);
	}
}

void CSearchListCtrl::ShowToolTip(const CString& sToolTip)
{
	if(m_toolTip.GetToolCount()==0)
	{
		m_toolTip.AddTool(this,sToolTip);
		// 设置出现提示需要的时间长度
		m_toolTip.SetDelayTime(TTDT_INITIAL,100);
		// 设置提示持续时间
		m_toolTip.SetDelayTime(TTDT_AUTOPOP,10000);
	}
	m_toolTip.UpdateTipText(sToolTip,this ); 
}


void CSearchListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	BOOL b;
	int nItem=ItemFromPoint(point,b);
	if(!b)
	{
		if(nItem!=m_nLastToolTip)
		{
			if(m_pCluster==NULL)
				return ;
			CString s;
			FRIEND_INFO * pUser= (FRIEND_INFO*)GetItemData(nItem);
			if(pUser==NULL)
				return;
			if(pUser->cluster_property==CMEMBER_CREATOR)
			{
				s="创建者";
			}
			else if(pUser->cluster_property==CMEMBER_ADMIN)
			{
				s="管理员";
			}
			else
			{
				s=pUser->nickname;
			}
			ShowToolTip(s);
			m_nLastToolTip=nItem;
		}
	}

	CListCtrlBase::OnMouseMove(nFlags, point);
}


BOOL CSearchListCtrl::PreTranslateMessage(MSG* pMsg)
{
	m_toolTip.RelayEvent(pMsg);
	return CListCtrlBase::PreTranslateMessage(pMsg);
}

// 绘制族内成员
void CSearchListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	COLORREF clrSelBack = RGB(255,255,255);

	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	// draw focus rectangle when no items in listbox
	if (lpDIS && lpDIS->itemID != (UINT)-1 && pDC )
	{
		int selChange   = lpDIS->itemAction & ODA_SELECT;
		int focusChange = lpDIS->itemAction & ODA_FOCUS;
		int drawEntire  = lpDIS->itemAction & ODA_DRAWENTIRE;

		int nSaveDC     = pDC->SaveDC();

		if (selChange || drawEntire)
		{
			/*BOOL sel = lpDIS->itemState & ODS_SELECTED;*/
			FRIEND_INFO *pFriend = (FRIEND_INFO *)lpDIS->itemData;

			char szText[255];
			if(pFriend == NULL)
			{
				/*switch(m_dwCount %3)
				{
				case 0:
					strcpy(szText,"我的好友:");
					m_dwCount++;
					break;
				case 1:
					strcpy(szText,"群组成员:");
					m_dwCount++;
					break;
				case 2:
					strcpy(szText,"群:");
					m_dwCount++;
					break;
				}*/
				//return ;
			}
			else if ( pFriend->GetId()!=(uint32)-1 )
			{
				strcpy(szText,pFriend->nickname);
			}
			else
			{
				tagCluster c;
				GetCluster(pFriend,c);
				/////增加群查找赋值/////////////////////////////
				CLUSTER_INFO *pCluster = FindClusterInfo(c.cid);
				if ( pCluster )
				{
					strcpy(szText,pCluster->szFamilyName);
				}
			}

			//好友昵称显示
			if ( strcmp(szText,"")==0 )
			{
				strcpy(szText,"-");
			}
			pDC->SetTextColor(0);
			pDC->FillSolidRect(&lpDIS->rcItem,RGB(255,255,255));
			////if ( sel )
			{					
				pDC->FillSolidRect(&lpDIS->rcItem,RGB(233,222,255));
				pDC->Draw3dRect(&lpDIS->rcItem,RGB(107,133,216),RGB(107,133,216));
			}
			pDC->SetBkMode(TRANSPARENT);
			CSize sizeText = pDC->GetTextExtent(szText,strlen(szText));
			CRect rcText = lpDIS->rcItem;
			if(pFriend != NULL)
			{
				rcText.left += 50;
			}
			rcText.top += ( ( lpDIS->rcItem.bottom - lpDIS->rcItem.top ) / 2 - sizeText.cy ) / 2 + 1;
			pDC->DrawText(szText, strlen(szText), &rcText,DT_END_ELLIPSIS|DT_TOP|DT_LEFT);

			////头像显示
			if(pFriend != NULL)
			{
				CPoint ptFace(3, lpDIS->rcItem.top + (lpDIS->rcItem.bottom - lpDIS->rcItem.top - 35) / 2);
				DrawHeadIcon(pDC,pFriend,ptFace,false);
			}
		}
		pDC->RestoreDC(nSaveDC);
	}
}


BOOL CSearchListCtrl::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(&rc,CLS_COLOR);

	CRect rcWnd;
	GetWindowRect(&rcWnd);

	rcWnd.right-=rcWnd.left;
	rcWnd.left = 0;
	rcWnd.bottom-=rcWnd.top;
	rcWnd.top = 0;

	CDC *pWndDC = GetWindowDC();

	if ( m_bShowBorder )
		pWndDC->Draw3dRect(&rcWnd,IM_LINE_COLOR,IM_LINE_COLOR);
	else
		pWndDC->Draw3dRect(&rcWnd,CLS_COLOR,CLS_COLOR);

	rcWnd.InflateRect(-1,-1);
	pWndDC->Draw3dRect(&rcWnd,CLS_COLOR,CLS_COLOR);

	ReleaseDC(pWndDC);

	return FALSE;
}

void CSearchListCtrl::OnNcPaint()
{
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CListCtrlBase::OnNcPaint()
}

void CSearchListCtrl::PreSubclassWindow()
{
	CListCtrlBase::PreSubclassWindow();

	ModifyStyle(0, LBS_OWNERDRAWFIXED);

	SetItemHeight(0,35);
}

void CSearchListCtrl::OnLbnDblclk()
{
	int nSel = GetCurSel();

	DblClick(nSel);
}

void CSearchListCtrl::DblClick(int nSel)
{
	if ( nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pItem = (FRIEND_INFO *)GetItemData(nSel);
		if(pItem == NULL)
		{
			return ;
		}
		if ( pItem->GetId()!=(uint32)-1 )
			CListCtrlBase::DblClick(nSel);
		else
		{
			tagCluster c;
			GetCluster(pItem,c);

			CLUSTER_INFO *pCluster = FindClusterInfo(c.cid);

			if ( pCluster )
			{
				CClusterSendMsgDlg *pDlg = g_pMessageHandleWnd->GetClusterSendMsgDlg(pCluster);

				if ( pDlg->m_hWnd==NULL )
					pDlg->Create(CClusterSendMsgDlg::IDD,GetDesktopWindow());

				pDlg->ShowWindow(SW_SHOW);

				if ( pDlg->IsIconic() )
					pDlg->ShowWindow(SW_RESTORE);

				pDlg->BringWindowToTop();
			}
		}
	}
}


void CSearchListCtrl::GetCluster(FRIEND_INFO *pFriend, tagCluster & c)
{
	CRecvPacket rp(pFriend->description,sizeof(pFriend->description));
	rp>>c.cid>>c.domain>>c.name;
}

void CSearchListCtrl::PopMenu(int nSel)
{
	CPoint pt;
	GetCursorPos(&pt);
	if ( nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pItem = (FRIEND_INFO *)GetItemData(nSel);

		if ( pItem->GetId()!=(uint32)-1 )
			CListCtrlBase::PopMenu(nSel);
		else
		{
			CMenuEx menu, *popmenu;
			menu.LoadMenu(IDR_MENU_TEMPLATE);
			popmenu = menu.GetSubMenu(0);
			popmenu->RemoveMenu(0,MF_BYPOSITION);

			CString str;

			tagCluster c;
			GetCluster(pItem,c);

			CLUSTER_INFO *pCluster = FindClusterInfo(c.cid);

			if ( pCluster )
			{
				str.Format("进入群 \"%s\" 的聊天室",pCluster->szFamilyName);
				popmenu->AppendMenu(MF_STRING,ID_CLUSTER_TALK,str);	

				popmenu->AppendMenu(MF_SEPARATOR);

				str.Format("查看群 \"%s\" 资料",pCluster->szFamilyName);
				popmenu->AppendMenu(MF_STRING,ID_CLUSTER_INFO,str);	

				/*str.Format("查看群 \"%s\" 的主页",pCluster->szFamilyName);
				popmenu->AppendMenu(MF_STRING,ID_CLUSTER_PAGE,str);	
			
				popmenu->AppendMenu(MF_SEPARATOR);

				//popmenu->AppendMenu(MF_STRING,ID_CLUSTER_MSG,"历史聊天记录");*/	
			
				popmenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
			}
		}
	}
	else
	{
		CMenuEx menu, *popmenu;
		menu.LoadMenu(IDR_MENU_TEMPLATE);
		popmenu = menu.GetSubMenu(0);
		popmenu->RemoveMenu(0,MF_BYPOSITION);
		popmenu->AppendMenu(MF_STRING,ID_CLEAR_RECENT,"清空列表");	
		popmenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
	}
}

void CSearchListCtrl::SetCluster(FRIEND_INFO *pFriend, const tagCluster & c)
{
	CSendPacket sp(pFriend->description);
	sp<<c.cid<<c.domain<<c.name;
}


void CSearchListCtrl::AddCluster(CLUSTER_INFO *pCluster)
{
	FRIEND_LIST::iterator it;

	int nItem;
	FRIEND_INFO *pItem=NULL;

	tagCluster c;
	c.cid             = pCluster->nFamilyID;
	strcpy(c.domain, pCluster->szFamilyDomain);
	strcpy(c.name,   pCluster->szFamilyName);


	bool bFind = FindCluster(pCluster->nFamilyID,&pItem,&nItem);
	int nC=0;
	if ( bFind==false )
	{
		// 获得当前联系人的个数
		nC = GetCount();
		//ASSERT(m_nMaxCount>=0);
		if(nC >= 0)
		{
			FRIEND_INFO *pFriend = (FRIEND_INFO*)GetItemData(nC - 1);
			if(pFriend)
			{
				FRIEND_LIST::iterator it,itE;
				/*itE = m_flRecent.end();
				for(it = m_flRecent.begin();it != itE;it++)
				{
					if(*it == pFriend)
					{
						delete pFriend;
						m_flRecent.erase(it);
						break;
					}
				}*/
			}
			//DeleteString(nC-1);
		}

		FRIEND_INFO *pRecent = new FRIEND_INFO();
		pRecent->SelfEncrypt();
		pRecent->SetId((uint32)-1);
		pRecent->reserved = /*time(0)*/g_LocalData.GetTimeNow();


		SetCluster(pRecent, c);

		//m_flRecent.push_back(pRecent);
		//////////////////////////////////////////////////
		CString strShowText = _T("");
		strShowText.Format(_T("%s"),c.name);
		int nRecent=InsertString(0,strShowText);
		SetItemData(nRecent,(DWORD)pRecent); 
	}
	else
	{
		pItem->reserved = /*time(0)*/g_LocalData.GetTimeNow();

		SetCluster(pItem, c);

		if ( nItem==0 )
		{
			InvalidateItem(nItem);
		}
		else
		{
			this->DeleteString(nItem);
			InsertString(0,c.name);
			SetItemData(0,(DWORD)pItem);
		}
	}
}


bool CSearchListCtrl::FindCluster(uint32 cid, FRIEND_INFO **ppFriend, int *pnItem)
{
	int count=GetCount();

	for ( int i=0; i<count; i++ )
	{
		FRIEND_INFO *pUser=(FRIEND_INFO *)GetItemData(i);

		if ( pUser->GetId()==(uint32)-1 )
		{
			tagCluster c;
			GetCluster(pUser, c);

			if ( c.cid==cid )
			{
				*pnItem	   = i;
				*ppFriend  = pUser; 
				return true;
			}
		}
	}

	return false;
}


void CSearchListCtrl::OnClusterInfo()
{
	int nSel = GetCurSel();

	if ( nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pItem = (FRIEND_INFO *)GetItemData(nSel);

		if ( pItem->GetId()==(uint32)-1 )
		{
			tagCluster c;
			GetCluster(pItem,c);

			CLUSTER_INFO *pCluster = FindClusterInfo(c.cid);

			if ( pCluster )
				CClusterSettingDlg::Show(pCluster);
		}
	}
}



void CSearchListCtrl::OnClusterTalk()
{
	int nSel = GetCurSel();

	if ( nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pItem = (FRIEND_INFO *)GetItemData(nSel);

		if ( pItem->GetId()==(uint32)-1 )
		{
			tagCluster c;
			GetCluster(pItem,c);

			CLUSTER_INFO *pCluster = FindClusterInfo(c.cid);

			if ( pCluster )
			{
				CClusterSendMsgDlg *pDlg = g_pMessageHandleWnd->GetClusterSendMsgDlg(pCluster);

				if ( pDlg->m_hWnd==NULL )
					pDlg->Create(CClusterSendMsgDlg::IDD,GetDesktopWindow());

				pDlg->ShowWindow(SW_SHOW);

				if ( pDlg->IsIconic() )
					pDlg->ShowWindow(SW_RESTORE);

				pDlg->BringWindowToTop();
			}
		}
	}
}

void CSearchListCtrl::OnClusterMsg()
{
	int nSel = GetCurSel();

	if ( nSel>-1 && nSel<GetCount() )
	{
		FRIEND_INFO *pItem = (FRIEND_INFO *)GetItemData(nSel);

		if ( pItem->GetId()==(uint32)-1 )
		{
			tagCluster c;
			GetCluster(pItem,c);

			CLUSTER_INFO *pCluster = FindClusterInfo(c.cid);

			if ( pCluster )
			{
				std::tr1::shared_ptr<CDuiMsgDlg> pDlg = CDuiMsgDlg::Show();
				pDlg->SetDefaultCluster(pCluster->nFamilyID);
			}
		}
	}
}


void CSearchListCtrl::OnClearRecent()
{
	ResetContent();
}