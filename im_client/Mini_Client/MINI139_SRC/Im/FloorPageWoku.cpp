// im\FloorPageWoku.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "FloorPageWoku.h"
#include ".\floorpagewoku.h"

#include "WebProcess.h"
#include "SAConfig.h"
#include "SelfFaceDlg.h"

#define IDC_WOKU_TOOL 1002

static const int WOKU_GROUP_CY=80;

IMPLEMENT_DYNAMIC(CWokuToolWnd, CWnd)
CWokuToolWnd::CWokuToolWnd()
{
	m_OriginY    = 0;
	m_nClientRealHeigh = 0;
	m_bMouseOver = false;
	m_HotItem.nGroup = m_HotItem.nItem=-1;

	m_fontBold.CreateFont(
		15,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Verdana");

	m_fontNormal.CreateFont(
		15,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"Verdana");
}

CWokuToolWnd::~CWokuToolWnd()
{
}

BEGIN_MESSAGE_MAP(CWokuToolWnd, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

BOOL CWokuToolWnd::Create(CWnd *pParentWnd,const CRect &rect, DWORD dwStyle/* =WS_VISIBLE|WS_BORDER|WS_CHILD */,UINT nID/* =0 */)
{
	BOOL bRet = CWnd::Create( AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW | CS_GLOBALCLASS),
		"woktoolpanel",
		dwStyle,
		rect,pParentWnd,
		nID);

	InitToolTip();
	return bRet;
}

// CWokuToolWnd 消息处理程序
BOOL CWokuToolWnd::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CWokuToolWnd::OnPaint()
{
	CPaintDC dc(this);
	Draw(&dc);
}


BOOL CWokuToolWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	OnVScroll(zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, 0, 0);	
	return TRUE;
}


void CWokuToolWnd::ShowToolTip(CString strTip)
{
	if ( strTip == m_strPreTip )
	{
		return ;
	}

	InitToolTip();
	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rc;
		GetClientRect(&rc);
		m_ToolTip.AddTool(this, strTip, rc, 1);
	} // if


	m_ToolTip.UpdateTipText(strTip ,this,1);

	m_strPreTip = strTip ;

	m_ToolTip.Activate(TRUE);
}

void CWokuToolWnd::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		m_ToolTip.Create(this,TTS_ALWAYSTIP);
		m_ToolTip.Activate(TRUE);
		m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 350);
	} 
} 

void CWokuToolWnd::Draw(CDC *pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int cy = rcClient.Height();
	int cx = rcClient.Width();

	CDC dcMem;
	CBitmap bmMem;
	dcMem.CreateCompatibleDC(pDC);
	bmMem.CreateCompatibleBitmap(pDC,cx,m_nClientRealHeigh);

	int nSavedMem = dcMem.SaveDC();

	dcMem.SelectObject(&bmMem);
	dcMem.FillSolidRect(0,0,cx,m_nClientRealHeigh,0xFFFFFF);

	dcMem.SelectObject(&m_fontBold);
	dcMem.SetBkMode(TRANSPARENT);

	int nTextCy   = dcMem.GetTextExtent("A").cy;

	int nGroups   = m_vecGroup.size();
	int nGroupTop = 0;

	CResBitmap bmpIcon1;
	bmpIcon1.LoadBitmap(IDB_ZONE_ICON1);

	for ( int nGroup=0; nGroup<nGroups; nGroup++ )
	{
		//draw Group title
		CRect rcGroupText;
		rcGroupText.left   = 0;
		rcGroupText.top    = nGroupTop;
		rcGroupText.right  = cx-2;
		rcGroupText.bottom = rcGroupText.top+nTextCy+4;

		dcMem.FillSolidRect(&rcGroupText,RGB(234,246,250));
		dcMem.Draw3dRect(&rcGroupText,RGB(221,244,250),RGB(221,244,250));

		DrawBitmap(&dcMem,rcGroupText.left+2,rcGroupText.top+3,&bmpIcon1,RGB(0,255,0));
		dcMem.SetTextColor(RGB(0,0,0));
		dcMem.SelectObject(&m_fontBold);
		dcMem.TextOut(rcGroupText.left+16,rcGroupText.top+1,m_vecGroup[nGroup].strGroup);

		//Draw Item
		int nItems    = m_vecGroup[nGroup].vecItem.size();

		int nItemTop  = nGroupTop + nTextCy+6;
		int nItemLeft = 20;

		int nTa = dcMem.SetTextAlign(TA_CENTER);

		for ( int nItem=0; nItem<nItems; nItem++ )
		{
			CResBitmap bmpItem;
			bmpItem.LoadBitmap(m_vecGroup[nGroup].vecItem[nItem].nBitmap);

			CRect & rcItem = m_vecGroup[nGroup].vecItem[nItem].rect;

			rcItem.left = nItemLeft;
			rcItem.top  = nItemTop;
			rcItem.right = nItemLeft+40;
			rcItem.bottom = nItemTop+41+nTextCy+2;
			rcItem.InflateRect(6,1);

			dcMem.SelectObject(&m_fontNormal);

			bool bSel=false;
			int  nSavedSelDC ;
			//设置选中的背景和字体颜色
			if ( nGroup==m_HotItem.nGroup && nItem==m_HotItem.nItem )
			{
				nSavedSelDC = dcMem.SaveDC();

				dcMem.FillSolidRect(&rcItem,RGB(233,244,255));
				dcMem.SetTextColor(RGB(244,100,100));

				bSel=true;
			}
			else
			{
				dcMem.SetTextColor(RGB(0,0,122));
			}

			DrawBitmap(&dcMem,nItemLeft,nItemTop,&bmpItem,RGB(0,255,0));
			dcMem.TextOut(nItemLeft+20,nItemTop+41,m_vecGroup[nGroup].vecItem[nItem].strItem);

			if ( bSel )
				dcMem.RestoreDC(nSavedSelDC);

			nItemLeft+= 65;
		}

		dcMem.SetTextAlign(nTa);

		nGroupTop += WOKU_GROUP_CY;
	}

	if ( m_nClientRealHeigh<cy )
	{
		pDC->FillSolidRect(0,m_nClientRealHeigh,cx,cy-m_nClientRealHeigh,0xFFFFFF);
	}

	pDC->BitBlt(0,0,cx,cy,&dcMem,0,m_OriginY,SRCCOPY);

	dcMem.RestoreDC(nSavedMem);
}

void CWokuToolWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (m_hWnd)
	{
		CalcScrollInfo();
		Invalidate();
	}
}

void CWokuToolWnd::HitTest(const CPoint &point, int &iGroup, int &iItem)
{
	CPoint pt;
	pt = point;
	pt.y += m_OriginY;

	int nGroups = m_vecGroup.size();

	for ( int nGroup=0; nGroup<nGroups; nGroup++ )
	{
		if ( pt.y<(nGroup+1)*WOKU_GROUP_CY )
		{
			int nItems = m_vecGroup[nGroup].vecItem.size();
			for ( int nItem=0; nItem<nItems; nItem++ )
			{
				if ( m_vecGroup[nGroup].vecItem[nItem].rect.PtInRect(pt) )
				{
					iGroup=nGroup;
					iItem = nItem;
					return ;
				}
			}
			break;
		}
	}

	iGroup=-1;
	iItem=-1;
}

void CWokuToolWnd::CalcScrollInfo()
{
	if (IsWindow(m_hWnd)) 
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		int nPreClientRealHeigh = m_nClientRealHeigh;
		m_nClientRealHeigh = m_vecGroup.size()*WOKU_GROUP_CY;

		SCROLLINFO si;
		memset(&si, 0, sizeof(si));
		si.cbSize = sizeof(si);
		si.fMask  = SIF_PAGE | SIF_RANGE | SIF_POS;
		si.nPage  = rcClient.Height();
		si.nMin = 0;
		si.nMax = m_nClientRealHeigh - 1;

		if (si.nPage >= si.nMax) 
		{
			m_OriginY = 0;
			ShowScrollBar(SB_VERT, FALSE);
		}
		else
		{
			if ( m_OriginY+si.nPage>m_nClientRealHeigh )
			{
				m_OriginY = m_nClientRealHeigh-si.nPage;
				if ( m_OriginY<0 )
					m_OriginY=0;

				si.nPos = m_OriginY;
			}
			ShowScrollBar(SB_VERT, TRUE);
			CWnd::SetScrollInfo(SB_VERT, &si);
		}
	}
}

void CWokuToolWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	int nGroup, nItem;
	HitTest(point,nGroup,nItem);

	if ( nGroup!=-1 && nItem!=-1 )
	{
		SetCursor(::LoadCursor(NULL,IDC_HAND));

		if ( m_HotItem.nGroup==m_HotItem.nItem && m_HotItem.nItem==-1 )
		{
			SetTimer(0x01,100,NULL);
		}

		if ( m_HotItem.nItem!=nItem || m_HotItem.nGroup!=nGroup )
		{
			m_HotItem.nItem  = nItem;
			m_HotItem.nGroup = nGroup;

			ShowToolTip(m_vecGroup[nGroup].vecItem[nItem].strToolTip);

			Invalidate();
		}
	}
	else
	{
		ShowToolTip("");
		SetCursor(::LoadCursor(NULL,IDC_ARROW));
	}

	CWnd::OnMouseMove(nFlags, point);
}

void CWokuToolWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	HitTest(point,m_SelItem.nGroup,m_SelItem.nItem);

	//int iSel , iFaceType;
	//HitTest(point,iFaceType,iSel);

	//if ( m_CurrentSel.nSel!=iSel || m_CurrentSel.nFacyType!=iFaceType )
	//{
	//	m_CurrentSel.nSel = iSel;
	//	m_CurrentSel.nFacyType = iFaceType;
	//	ReDraw();

	//	GetParent()->PostMessage(WM_COMMAND,GetDlgCtrlID(),0);
	//}
	CWnd::OnLButtonDown(nFlags, point);
}

void CWokuToolWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int iGroup,iItem;
	HitTest(point,iGroup,iItem);

	if ( iGroup==m_SelItem.nGroup && iItem==m_SelItem.nItem )
	{
		if ( iGroup!=-1 && iItem!=-1 )
           GetParent()->PostMessage(WM_WOKUTOOL_CLICK,(WPARAM)&m_SelItem,m_vecGroup[iGroup].vecItem[iItem].dwData);
	}

	CWnd::OnLButtonUp(nFlags, point);
}

void CWokuToolWnd::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		int nGroup, nItem;
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		HitTest(point,nGroup,nItem);

		if ( nGroup==-1 && nItem==-1 )
		{
			KillTimer(nIDEvent);
			m_HotItem.nItem=m_HotItem.nGroup=-1;
			Invalidate();
		}
	}
	CWnd::OnTimer(nIDEvent);
}

int CWokuToolWnd::AddGroup(CString strGroup)
{
	tagGroupInfo g;
	g.strGroup = strGroup;
	g.dwData   = 0;
	m_vecGroup.push_back(g);

	return m_vecGroup.size()-1;
}

int CWokuToolWnd::AddItem(int nGroup,CString strItem,UINT nBitmap,DWORD dwData, CString strToolTip)
{
	if ( nGroup>-1 && nGroup<m_vecGroup.size() )
	{
		tagItemInfo i;
		i.dwData  = dwData;
		i.nBitmap = nBitmap;
		i.strItem = strItem;
		i.strToolTip = strToolTip;
		m_vecGroup[nGroup].vecItem.push_back(i);

		return m_vecGroup[nGroup].vecItem.size()-1;
	}

	return -1;
}

void CWokuToolWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	SetFocus();

	switch (nSBCode) 
	{
	case SB_TOP:
		OnVScroll(SB_LINEUP, nPos, pScrollBar);
		break;
	case  SB_BOTTOM:
		OnVScroll(SB_LINEDOWN, nPos, pScrollBar);
		break;
	case SB_LINEUP:
		if (m_OriginY > 0) 
		{
			int nPos;
			m_OriginY -= 20;

			if (m_OriginY < 0) 
			{
				m_OriginY = 0;
				nPos = 0;
			}
			else
			{
				nPos = GetScrollPos(SB_VERT)-20;
			}

			CWnd::SetScrollPos(SB_VERT, nPos);

			Invalidate();
		}
		break;
	case SB_LINEDOWN:
		if (m_OriginY + rcClient.Height() < m_nClientRealHeigh) 
		{
			int nPos;

			m_OriginY += 20;

			if ( m_OriginY+rcClient.Height()>m_nClientRealHeigh )
			{
				nPos = m_nClientRealHeigh;
				m_OriginY = m_nClientRealHeigh-rcClient.Height();
			}
			else
			{
				nPos = GetScrollPos(SB_VERT) + 20;
			}


			CWnd::SetScrollPos(SB_VERT, nPos);

			Invalidate();
		}
		break;
	case SB_PAGEDOWN:
		if (m_OriginY + rcClient.Height() < m_nClientRealHeigh) 
		{
			CWnd::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + rcClient.Height());
			m_OriginY += rcClient.Height();

			Invalidate();
		}
		break;
	case SB_PAGEUP:
		if (m_OriginY > 0) 
		{
			CWnd::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - rcClient.Height());
			m_OriginY -= rcClient.Height();
			if (m_OriginY < 0) 
			{
				m_OriginY = 0;
			}

			Invalidate();
		}
		break;
	case SB_THUMBPOSITION:
		CWnd::SetScrollPos(SB_VERT, nPos);
		m_OriginY = m_OriginY / rcClient.Height() + nPos;		
		Invalidate();
		break;
	case SB_THUMBTRACK:
		if (nPos < m_nClientRealHeigh) 
		{
			m_OriginY = nPos;
			//m_OriginY += LINE_UP_CAP;
			Invalidate();
		}

		CWnd::SetScrollPos(SB_VERT, nPos);
		break;
	default:
		;
	}

	//	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

// CFloorPageWoku 对话框

IMPLEMENT_DYNAMIC(CFloorPageWoku, CDialog)
IMPLEMENT_HMG_SCROLLBAR(CFloorPageWoku)
CFloorPageWoku::CFloorPageWoku(CWnd* pParent /*=NULL*/)
	: CDialog(CFloorPageWoku::IDD, pParent)
{
}

CFloorPageWoku::~CFloorPageWoku()
{
}

void CFloorPageWoku::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFloorPageWoku, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	SET_HMG_SCROLLBAR(IDC_WOKU_TOOL)
	ON_MESSAGE(WM_WOKUTOOL_CLICK,OnWokuToolClick)
END_MESSAGE_MAP()


// CFloorPageWoku 消息处理程序

BOOL CFloorPageWoku::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CFloorPageWoku::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if ( m_wndTool.m_hWnd )
		m_wndTool.MoveWindow(0,0,cx,cy);
}

BOOL CFloorPageWoku::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect(0,20,100,100);
	m_wndTool.Create(this,rect,WS_VISIBLE|WS_CHILDWINDOW|WS_CLIPCHILDREN|WS_TABSTOP,IDC_WOKU_TOOL);
	
	int n ;

	n=m_wndTool.AddGroup("IECiM空间");
	m_wndTool.AddItem(n,"我的主页",IDB_ZONE_MYHOME,7,"打开并且进入你在IECiM上的个人主页");
	m_wndTool.AddItem(n,"我的帐户",IDB_ZONE_ACCOUNT,8,"在这里你可以管理你的金币、查看你的消费记录");
	m_wndTool.AddItem(n,"便利店",IDB_ZONE_SHOP,9,"购买闪秀装备、个人主页模版、专用魔法表情");

	n=m_wndTool.AddGroup("IECiM");
	m_wndTool.AddItem(n,"大头贴",IDB_ZONE_BIGHEAD,0,"用IECiM制作大头贴，很多好看、很酷的相框和背景，并且可以方便的上传到你的相册哦");
	m_wndTool.AddItem(n,"个人设置",IDB_ZONE_SETTING,1,"设置你在IEC-iM的登录密码、个人资料；设置图片备份目录；各种常规软件设置");
	m_wndTool.AddItem(n,"更改头像",IDB_ZONE_CHANGEHEAD,2,"更改你在IEC-iM上的头像");

	n = m_wndTool.AddGroup("图片");
	m_wndTool.AddItem(n,"上传图片",IDB_ZONE_UPLOAD_PICTURE,3,"将图片快速批量上传到你的相册中");
	m_wndTool.AddItem(n,"我的图片",IDB_ZONE_MYPICTURE,4,"查看你在IEC-iM上的所有图片、备份你在IEC-iM上的所有图片");
	//m_wndTool.AddItem(n,"图片主页",459,2);

	n = m_wndTool.AddGroup("文章");
	m_wndTool.AddItem(n,"发表文章",IDB_ZONE_WRITE_ARTICLE,5,"用IEC-iM写文章、可以像Word一样编辑、排版你的文章，还可以保存成草稿，一次写不完下次继续写");
	m_wndTool.AddItem(n,"我的文章",IDB_ZONE_MYARTICLE,6,"用IEC-iM管理你的文章、备份你在IEC-iM上上发表的文章");
//http://miniking.feiniu.com/media/download.php

	n = m_wndTool.AddGroup("上传音乐、视频、在线录像");
	m_wndTool.AddItem(n,"媒体中心",IDB_ZONE_MEDIA,10,"上传音乐、视频到个人空间， 在线视频制作");

	m_wndTool.SetFocus();

	AttachHMGScrollbar(m_wndTool.m_hWnd);

	return TRUE; 
}

LRESULT CFloorPageWoku::OnWokuToolClick(WPARAM wParam,LPARAM lParam)
{
	switch(lParam)
	{
	case 0://上传图片
		g_pHeadPhoto->Show();
		break;
	case 1://个人设置
		CSAConfig::Show();
		break;
	case 2://更改头像
		CSelfFaceDlg::Show();
		break;
	case 3:
		ShowUploadPicDlg((char*)g_ImUserInfo.GetUserName().c_str(),g_ImUserInfo.nickname,(char*)g_ImUserInfo.GetPassword().c_str(),g_ImUserInfo.GetId());	
		break;
	case 4:
		g_pMyPictures->Show();
		break;
	case 5:
		g_pWriteArticle->Show();
		break;
	case 6:
		g_pMyArticles->Show();
		break;
	case 7:
		{
			if( g_pWebConfig )
			{
				CString strUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(USER_HOME,StrToWideStr(g_ImUserInfo.GetUserName().c_str()));
				MiniOpenUrl(strUrl);
			}

		}
		break;
	case 8:
		{
			if(g_pWebConfig)
			{
				CString strUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(USER_ACCOUNT,StrToWideStr(g_ImUserInfo.GetUserName().c_str()));
				MiniOpenUrl(strUrl);
			}

		}
		break;
	case 9:
		{
			if(g_pWebConfig)
			{
				CString strUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(WOKU_SHOP,L"");
				MiniOpenUrl(strUrl);
			}

		}
		break;
	case 10:
		{
			if(g_pWebConfig)
			{
				CString strUrl = (LPCSTR)g_pWebConfig->GetWebPageUrl(UPLOAD_MEDIA,StrToWideStr(g_ImUserInfo.GetUserName().c_str()));
				MiniOpenUrl(strUrl);
			}

		}
		break;
	}
	return 0;
}

BOOL CWokuToolWnd::PreTranslateMessage(MSG* pMsg)
{
	if ( m_ToolTip.m_hWnd )
		m_ToolTip.RelayEvent(pMsg);

	return CWnd::PreTranslateMessage(pMsg);
}
