// im\P2PFileTransProgressBar.cpp : 实现文件
//

#include "stdafx.h"
#include "P2PFileTransProgressBar.h"
#include ".\p2pfiletransprogressbar.h"
#include "..\utils\Function.h"


// CP2PFileTransProgressBar

IMPLEMENT_DYNAMIC(CP2PFileTransProgressBar, CStatic)
CP2PFileTransProgressBar::CP2PFileTransProgressBar()
{
	m_nCurSel  = -1;
	m_nHotItem = -1;
	m_bCloseBtnDown    = false;
	m_bMouseOnTabBtns  = false;
	m_bMouseOnCloseBtn = false;
	m_bmpBtns.LoadBitmap(IDB_FILETRANS_TABBTN);
	m_bmpBtnClose.LoadBitmap(IDB_FILETRANS_BTN);
}

CP2PFileTransProgressBar::~CP2PFileTransProgressBar()
{
	int n=m_vecItems.size();
	for ( int i=0;i<n;i++)
	{
		delete m_vecItems[i];
	}
	m_vecItems.clear();
}


BEGIN_MESSAGE_MAP(CP2PFileTransProgressBar, CStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void  CP2PFileTransProgressBar::SetItemData(int nItem,DWORD dwData)
{
	if ( nItem>-1 && nItem<m_vecItems.size() )
	{
		m_vecItems[nItem]->dwData = dwData;
	}
}

BOOL CP2PFileTransProgressBar::GetItemInfo(int nItem,DWORD &dwUser, BOOL &bSendFile)
{
	if ( nItem>-1 && nItem<m_vecItems.size() )
	{
		dwUser=m_vecItems[nItem]->dwData;
		bSendFile=m_vecItems[nItem]->bSendFile;
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

DWORD CP2PFileTransProgressBar::GetItemData(int nItem)
{
	if ( nItem>-1 && nItem<m_vecItems.size() )
	{
		return m_vecItems[nItem]->dwData;
	}

	return 0;
}

DWORD CP2PFileTransProgressBar::AddItem(CString strFileName,long nFileSize,DWORD dwUser,BOOL bSendFile)
{
	CString s1,s2,s3;
	ExtractFilePath(strFileName,s1,s2,s3);

	strFileName = s2+s3;

	tagItem * pItem		= new tagItem();
	pItem->bSendFile	= bSendFile;
	pItem->dwData		= dwUser;
	if (nFileSize<1024)
	{
		pItem->strFileSize.Format("%d 字节",nFileSize);
	}
	else
	{
		nFileSize = nFileSize/1024;

		if (nFileSize<8192)
			pItem->strFileSize.Format("%d KB",nFileSize);
		else
			pItem->strFileSize.Format("%.2f MB",(float)nFileSize/1024.0);
	}

	pItem->strFileName	= strFileName;

	CWnd * pWnd    = CWnd::GetDesktopWindow();
	
	if ( pWnd )
	{
		CDC  * pDC	   = pWnd->GetDC();

		if ( pDC )
		{
			TCHAR temp[255];
			::GetTempPath(sizeof(temp)/sizeof(TCHAR),temp);

			CreateDirectory(temp,NULL);

			strcat(temp,pItem->strFileName);

			HANDLE hFile = CreateFile(temp,FILE_ALL_ACCESS,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_FLAG_OVERLAPPED,NULL);
			if ( hFile!=INVALID_HANDLE_VALUE )
			{
				CloseHandle(hFile);
			}

			SHFILEINFO sfi;
			memset(&sfi,0,sizeof(SHFILEINFO));
			HIMAGELIST hImageList =(HIMAGELIST)SHGetFileInfo(temp,FILE_ATTRIBUTE_NORMAL,&sfi,sizeof(sfi),SHGFI_SYSICONINDEX|SHGFI_LARGEICON);


			CDC dcTemp ;
			dcTemp.CreateCompatibleDC(pDC);

			int nSaved  = dcTemp.SaveDC();
			
			pItem->pFileBitmap = new CBitmap();
			pItem->pFileBitmap->CreateCompatibleBitmap(pDC,32,32);

			dcTemp.SelectObject(pItem->pFileBitmap);
			dcTemp.FillSolidRect(0,0,32,32,0xFFFFFF);

			if ( hImageList )
			{
				//::ImageList_Draw()
				ImageList_Draw(hImageList,sfi.iIcon,dcTemp.m_hDC,0,0,0);

			}

			dcTemp.RestoreDC(nSaved);
			pWnd->ReleaseDC(pDC);

			DeleteFile(temp);
		}
	}

	m_vecItems.push_back(pItem);
	m_nCurSel = m_vecItems.size()-1; 
	
	if ( m_hWnd )
	{
		Invalidate();
	}

	return m_nCurSel;
}

void  CP2PFileTransProgressBar::SetItemProgressPos(int nItem, int nPos)
{
	if ( nItem>-1 && nItem<m_vecItems.size() )
	{
		m_vecItems[nItem]->nProgressPos = nPos;

		if ( m_nCurSel==nItem )
		{
			Invalidate();
		}
	}
}

void  CP2PFileTransProgressBar::SetItemProgressTip(int nItem, CString strTip)
{
	if ( nItem>-1 && nItem<m_vecItems.size() )
	{
		m_vecItems[nItem]->strProgressTip = strTip;

		if ( m_nCurSel==nItem )
		{
			Invalidate();
		}
	}
}

int   CP2PFileTransProgressBar::FindItemByItemData(DWORD dwData,BOOL bSendFile)
{
	int n=m_vecItems.size();
	for ( int i=0; i<n; i++ )
	{
		if ( dwData==m_vecItems[i]->dwData )
		{
			return i;
		}
	}
	return -1;
}

void  CP2PFileTransProgressBar::DeleteItem(int nItem)
{
	if ( nItem>-1 && nItem<m_vecItems.size() )
	{
		tagItem * p = m_vecItems[nItem];
		m_vecItems.erase(m_vecItems.begin()+nItem);
		delete p;

		if ( m_nCurSel>=m_vecItems.size() )
		{
			m_nCurSel--;
		}
		
		if ( m_hWnd )
		{
			Invalidate();
		}
	}
}

void CP2PFileTransProgressBar::SetCurSel(int nSel)
{
	m_nCurSel = nSel;

	if ( m_nCurSel<0 || m_nCurSel>=m_vecItems.size() )
		m_nCurSel=0;

	if ( m_hWnd )
	{
		Invalidate();
	}
}

int  CP2PFileTransProgressBar::GetCurSel()
{
	return m_nCurSel;
}

void CP2PFileTransProgressBar::Draw(CDC *pClientDC)
{
	CRect rc;
	GetClientRect(&rc);

	int cx = rc.Width();
	int cy = rc.Height();

	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pClientDC);

	CBitmap bmpTemp;
	bmpTemp.CreateCompatibleBitmap(pClientDC,cx,cy);

	int nSavedTempDC = dcTemp.SaveDC();

	dcTemp.SelectObject(&bmpTemp);

	//begin draw
	dcTemp.FillSolidRect(&rc,0xFFFFFF);

	int nItems = m_vecItems.size();

	CDC dcBtn;
	dcBtn.CreateCompatibleDC(pClientDC);

	CBitmap *pOldbmp = dcBtn.SelectObject(&m_bmpBtns);

	dcTemp.SetBkMode(TRANSPARENT);
	dcTemp.SelectObject(&g_fontText);

	for ( int i=0; i<nItems; i++ )
	{
		tagItem *pItem		   = m_vecItems[i];
		pItem->rcTabBtn.left   = i*28+3;
		pItem->rcTabBtn.right  = pItem->rcTabBtn.left+24;
		pItem->rcTabBtn.top    = 3;
		pItem->rcTabBtn.bottom = pItem->rcTabBtn.top + 15;

		if ( m_nCurSel==i )
		{
			dcTemp.BitBlt(pItem->rcTabBtn.left,pItem->rcTabBtn.top,24,15,&dcBtn,0,0,SRCCOPY);
		}
		else
		if ( m_nHotItem==i )
		{
			dcTemp.BitBlt(pItem->rcTabBtn.left,pItem->rcTabBtn.top,24,15,&dcBtn,24,0,SRCCOPY);
		}
		else
		{
			dcTemp.BitBlt(pItem->rcTabBtn.left,pItem->rcTabBtn.top,24,15,&dcBtn,48,0,SRCCOPY);
		}

		CString strID;
		strID.Format("%d",i+1);

		if ( m_nHotItem==i )
		{
			dcTemp.SetTextColor(RGB(233,200,188));
		}
		else
		{
			dcTemp.SetTextColor(0);
		}

		int nTextLeft = pItem->rcTabBtn.left+(pItem->rcTabBtn.Width()-dcTemp.GetTextExtent(strID).cx)/2;
		dcTemp.TextOut(nTextLeft,pItem->rcTabBtn.top+2,strID);
	}	

	if ( nItems>0 && (m_nCurSel>-1 && m_nCurSel<nItems) )
	{
		tagItem * pItem = m_vecItems[m_nCurSel];		
		::DrawBitmap(&dcTemp,5,24,pItem->pFileBitmap,0);

		CString strTitle;

		strTitle.Format("%s %s (%s)",pItem->bSendFile? "发送:" : "接收:" ,pItem->strFileName,pItem->strFileSize);

		dcTemp.SetTextColor(0);
		dcTemp.TextOut(43,24,strTitle);

		int nBarLeft  = 43;
		int nBarTop   = 38;
		int nBarRight = cx-28;
		int nBarBottom = nBarTop+18;

		//dcTemp.FillSolidRect(nBarLeft,nBarTop,nBarRight-nBarLeft,nBarBottom-nBarTop,0xFF0000);
		CPen penBar(PS_SOLID,1,RGB(147,184,228));
		dcTemp.SelectObject(&penBar);
		dcTemp.SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));
		dcTemp.RoundRect(nBarLeft,nBarTop,nBarRight,nBarBottom,3,3);

		CRect rc1;
		rc1.SetRect(nBarLeft+2,nBarTop+2,nBarRight-2,nBarBottom-2);

		dcTemp.FillSolidRect(&rc1,RGB(222,231,254));

		CRect rc2 = rc1;

		rc2.right = rc2.left+(rc2.Width()*pItem->nProgressPos)/100.0;

		int nVar1 = rc2.Height()/2;//half height
		dcTemp.FillSolidRect(rc2.left,rc2.top,rc2.Width(),nVar1,RGB(195,216,255));
		dcTemp.FillSolidRect(rc2.left,rc2.top+nVar1,rc2.Width(),nVar1,RGB(174,209,255));

		dcTemp.Draw3dRect(&rc1,RGB(177,208,253),RGB(177,208,253));
		dcTemp.Draw3dRect(&rc2,RGB(130,182,247),RGB(130,182,247));

		CString strTip   = pItem->strProgressTip;
		CSize   sizeTip  = dcTemp.GetTextExtent(strTip);
		
		int nTipLeft = rc1.left + (rc1.Width()-sizeTip.cx)/2;
		//dcTemp.SetTextColor(0);
		//dcTemp.SetROP2(R2_NOT);
		//dcTemp.TextOut(nTipLeft,rc1.top,strTip);

		CDC dcTip;
		dcTip.CreateCompatibleDC(pClientDC);
		CBitmap bmpTip;
		bmpTip.CreateCompatibleBitmap(pClientDC,sizeTip.cx,sizeTip.cy);

		int nSavedDCTip = dcTip.SaveDC();

		dcTip.SetTextColor(RGB(122,233,0));

		dcTip.SelectObject(&bmpTip);
		dcTip.FillSolidRect(0,0,sizeTip.cx,sizeTip.cy,RGB(0,0,255));

		dcTemp.BitBlt(nTipLeft,rc1.top+1,sizeTip.cx,sizeTip.cy,&dcTip,0,0,SRCINVERT);

		dcTip.SelectObject(&g_fontText);
		dcTip.SetBkColor(RGB(0,0,255));
		dcTip.TextOut(0,0,strTip);

		dcTemp.BitBlt(nTipLeft,rc1.top+1,sizeTip.cx,sizeTip.cy,&dcTip,0,0,SRCINVERT);

		dcTip.RestoreDC(nSavedDCTip);
	}


	dcTemp.Draw3dRect(2,17,cx-4,45,RGB(233,233,233),RGB(233,233,233));

	m_rcBtnClose.left = cx-24;
	m_rcBtnClose.top  = 39;
	m_rcBtnClose.right = m_rcBtnClose.left+16;
	m_rcBtnClose.bottom = m_rcBtnClose.top+16;

	if ( m_bCloseBtnDown )
		DrawBitmap(&dcTemp,m_rcBtnClose.left+1,m_rcBtnClose.top+1,&m_bmpBtnClose,0);
	else
		DrawBitmap(&dcTemp,m_rcBtnClose.left,m_rcBtnClose.top,&m_bmpBtnClose,0);


	pClientDC->BitBlt(0,0,cx,cy,&dcTemp,0,0,SRCCOPY);

	dcBtn.SelectObject(pOldbmp);
	dcTemp.RestoreDC(nSavedTempDC);
}

void CP2PFileTransProgressBar::OnPaint()
{
	CPaintDC dc(this);
	Draw(&dc);
}

BOOL CP2PFileTransProgressBar::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CP2PFileTransProgressBar::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( m_rcBtnClose.PtInRect(point) )
	{
		if ( m_bMouseOnCloseBtn==false )
		{
			SetTimer(0x02,10,NULL);

			if ( m_nCurSel>-1 && m_nCurSel<m_vecItems.size() )
			{
				tagItem *pItem = m_vecItems[m_nCurSel];
				CString strToolTip;

				if ( pItem->bSendFile )
				{
					strToolTip.Format("点击这里取消发送文件:%s",(LPCSTR)pItem->strFileName);
				}
				else
				{
					strToolTip.Format("点击这里取消接收文件:%s",(LPCSTR)pItem->strFileName);
				}

				SetToolTip(strToolTip);
			}

			m_bMouseOnCloseBtn=true;
		}
	}
	else
	{
		SetToolTip("");

		if ( m_bMouseOnCloseBtn==true )
		{
			KillTimer(0x02);
			m_bMouseOnCloseBtn=false;
		}
	}

	int nHotItem = -1;
	int nItems   = m_vecItems.size();

	for ( int n=0; n<nItems; n++ )
	{
		if ( m_vecItems[n]->rcTabBtn.PtInRect(point) )
		{
			nHotItem=n;
			break;
		}
	}

	if ( nHotItem!=m_nHotItem )
	{
		if ( nHotItem==-1 )//not no tab btns
		{
			if ( m_bMouseOnTabBtns )
			{
				KillTimer(0x01);
				m_bMouseOnTabBtns = false;
			}
		}
		else
		{
			if ( m_bMouseOnTabBtns==false )
			{
				SetTimer(0x01,10,NULL);
				m_bMouseOnTabBtns = true;
			}
		}

		m_nHotItem = nHotItem;
		Invalidate();
	}

	CStatic::OnMouseMove(nFlags, point);
}

void CP2PFileTransProgressBar::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( m_rcBtnClose.PtInRect(point) )
	{
		if ( m_bCloseBtnDown==false )
		{
			m_bCloseBtnDown=true;
			Invalidate();
		}
	}

	if ( m_nHotItem!=-1 && m_nHotItem!=m_nCurSel )
	{
		m_nCurSel = m_nHotItem;
		Invalidate();
	}

	CStatic::OnLButtonDown(nFlags, point);
}

void CP2PFileTransProgressBar::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( m_bCloseBtnDown )
	{
		if ( m_rcBtnClose.PtInRect(point) )
		{
			m_bCloseBtnDown=false;
			Invalidate();

			NMHDR nmhr;
			nmhr.hwndFrom = m_hWnd;
			nmhr.code     = m_nCurSel;
			GetParent()->SendMessage(WM_NOTIFY,this->GetDlgCtrlID(),(LPARAM)&nmhr);
		}
	}

	CStatic::OnLButtonUp(nFlags, point);
}

void CP2PFileTransProgressBar::OnTimer(UINT nIDEvent)
{
	CPoint ptCursor ;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);

	switch(nIDEvent)
	{
	case 0x01://tab btns timer check
		{
			int nHotItem = -1;
			int nItems   = m_vecItems.size();

			for ( int n=0; n<nItems; n++ )
			{
				if ( m_vecItems[n]->rcTabBtn.PtInRect(ptCursor) )
				{
					nHotItem=n;
					break;
				}
			}

			if ( nHotItem==-1 )
			{
				KillTimer(0x01);
				m_nHotItem=-1;
				m_bMouseOnTabBtns = false;

				Invalidate();
			}
		}

		break;
	case 0x02:// close btn timer check
		{
			if ( m_rcBtnClose.PtInRect(ptCursor)==FALSE )
			{
				KillTimer(0x02);
				m_bMouseOnCloseBtn=false;
			}
		}
		break;
	}

	CStatic::OnTimer(nIDEvent);
}

void CP2PFileTransProgressBar::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		//m_ToolTip.Create(this);
		//m_ToolTip.Activate(FALSE);
		//m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
		m_ToolTip.Create( this , TTS_ALWAYSTIP ) ;
		m_ToolTip.Activate( TRUE ) ;
	} 
}

void CP2PFileTransProgressBar::SetToolTip(CString strTip)
{
	InitToolTip();
	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, strTip, rectBtn, 1);
	} // if

	m_ToolTip.UpdateTipText(strTip,this,1);

	m_ToolTip.Activate(TRUE);

}

void CP2PFileTransProgressBar::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	EnableToolTips(TRUE);
	//InitToolTip();
}

BOOL CP2PFileTransProgressBar::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_ToolTip.m_hWnd)
		m_ToolTip.RelayEvent(pMsg);

	return CStatic::PreTranslateMessage(pMsg);
}