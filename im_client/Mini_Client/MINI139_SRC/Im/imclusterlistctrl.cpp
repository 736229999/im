#include "stdafx.h"
#include <afxwin.h>
#include <afxcmn.h>

#include "impublic.h"

#pragma warning (disable: 4018)

#define TIMER_MOUSEMOVE  0x01
#define TIMER_LOADING    0x02
#define TIMER_FLASH		 0x03

#define ROOT_BAR_CY      26  /*我创建的群*/

#define LIST_START_X      10  /*我创建的群*/
#define GROUP_BAR_CY	 45  /*飞牛网(武汉)*/
#define TOOL_BTN_SIZE	 17
#define TOOL_BTN_SPACE   8
#define GRP_BTN_W 12
#define SPACE_X 8



static CString  g_strDot="";
static int      g_iDotCount=0;

void ShowMessage(CString strMessage,UINT nType/* =MB_OK|MB_ICONINFORMATION */);

#define ItemExistsAsserte(nGroup, nIndex) \
	_ASSERTE(nGroup >= 0 && nGroup < m_vecRoot[posGroup.nRoot]->vecGroup.size() && \
		nIndex >= 0 && nIndex < m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->vecItem.size());

/////////////////////////////////////////////////////////////////////////////
// CIMClusterListCtrl
#include "imclusterlistctrl.h"
#include "..\Resource.h"
#include ".\imclusterlistctrl.h"

int GetOnlineCount(CL_GROUP_INFO *group)
{
	int n=0;
		for ( int iFriend=0; iFriend<group->vecItem.size(); iFriend++)
		{
			FRIEND_INFO *pFriend = (FRIEND_INFO *)group->vecItem[iFriend]->dwData;
			if ( pFriend && pFriend->status!=XTREAM_OFFLINE )
			{
				n++;
			}
		}
	return n;
}

CIMClusterListCtrl::CIMClusterListCtrl()
{
	m_nWidth = 0;
	m_nHeight = 0;
	m_bRedraw = true;

	m_nItemHeight = 18;

	m_clrBkGround	   = RGB(255,255,255);//0xFFD49E;//
	/*
	m_Font.CreateFont(12, 
		0,
		0, 
		90,
		FW_NORMAL,
		0,
		0,
		0,
		DEFAULT_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY,
		(DEFAULT_PITCH|FF_MODERN),
		"宋体");
*/
	m_OriginY = 0;
	m_nClientRealHeigh = 0;

	m_hCursor = ::LoadCursor(NULL,IDC_ARROW);
	m_bMouseDown = false;

	m_bMouseOnGroupIcon     = false ;
	m_bMouseOnHome			= false ;
	m_bMouseOnRefuse		= false ;	
	//m_bMouseOnAlbum			= false ;
	//m_bMouseOnArticle		= false ;
	//m_bMouseOnWriteArticle	= false ;
	//m_bMouseOnUploadPic		= false ;
	//m_bMouseOnSetting		= false ;
	//m_bMouseOnParty			= false ;
	m_bMouseOnExpand        = false ;
	m_bMouseOnTitle         = false ;

	m_bBtnGroupIconDown     = false ;
	m_bBtnHomeDown			= false ;
	m_bBtnRefuseDown		= false ;
	//m_bBtnArticleDown		= false ;
	//m_bBtnAlbumDown			= false ;
	//m_bBtnWriteArticleDown  = false ;
	//m_bBtnUploadPicDown     = false ;
	//m_bBtnSettingDown		= false ;
	//m_bBtnPartyDown			= false ;	

	m_bmpFamilyHead.LoadBitmap(IDB_FAMILY_HEAD);
	m_bmpGroupTool.LoadBitmap(IDB_FAMILY_GROUPTOOL);
	m_bmpGroupStatus.LoadBitmap(IDB_FAMILY_GROUP_STATUS);


	m_bInvalidateLock = false;
	m_bMouseOver = false;


}


CIMClusterListCtrl::~CIMClusterListCtrl()
{
	DeleteAllItems();
}

BEGIN_MESSAGE_MAP(CIMClusterListCtrl, CIMListCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_VSCROLL()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIMClusterListCtrl message handlers
BOOL CIMClusterListCtrl::Create(CWnd *pParentWnd,const CRect &rect, DWORD dwStyle/* =WS_VISIBLE|WS_BORDER|WS_CHILD */,UINT nID/* =0 */)
{
	return CIMListCtrl::Create( AfxRegisterWndClass(CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW),
						"xTreamTreeClass",
						dwStyle,
						rect,pParentWnd,
						nID);
}

void CIMClusterListCtrl::ClearGroup(CL_GROUP_POS posGroup)
{
	vector<_CL_ITEM_INFO *> & items   = m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->vecItem;

	int count = items.size();

	for (int i=0;i<count; i++ )
	{
		delete items[i];
	}

	items.clear();
	InvalidateList();
}

int CIMClusterListCtrl::AddRoot(CString strRoot)
{
	CL_ROOT_INFO  *pR = new CL_ROOT_INFO();
	pR->strTitle = strRoot;
	m_vecRoot.push_back(pR);
	return m_vecRoot.size()-1;
}

void CIMClusterListCtrl::DelGroup(CL_GROUP_POS posGroup)
{
	vector<CL_GROUP_INFO *> & groups  = m_vecRoot[posGroup.nRoot]->vecGroup; 
	vector<_CL_ITEM_INFO *> & items   = m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->vecItem;

	int count = items.size();

	for ( int i=0; i<count; i++ )
	{
		delete items[i];
	}

	items.clear();

	delete groups[posGroup.nGroup];
	groups.erase(groups.begin()+posGroup.nGroup);

	InvalidateList();
}

void CIMClusterListCtrl::UpdateGroupImage(CL_GROUP_POS posGroup)
{
	if ( posGroup.IsNull()==false )
	{
		CLUSTER_INFO * pCluster = (CLUSTER_INFO *)m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->dwData;

		if ( pCluster )
		{
			m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->pBitmap40 = GetClusterBitmap(pCluster,false);
			m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->pBitmap20 = GetClusterBitmap(pCluster,true);
			InvalidateList();
		}
	}
}

CL_GROUP_POS CIMClusterListCtrl::AddGroup(int nRoot, CString strTitle)
{
	CL_GROUP_POS posGroup;

	if ( nRoot>-1 && nRoot<m_vecRoot.size() )
	{
		CL_GROUP_INFO * Group = new CL_GROUP_INFO();

		strncpy(Group->szTitle,strTitle,sizeof(Group->szTitle) - 1);
		Group->clrText = RGB(0,0,180);
		Group->dwData = 0;
		
		m_vecRoot[nRoot]->vecGroup.push_back(Group);

		posGroup.nRoot  = nRoot;
		posGroup.nGroup = m_vecRoot[nRoot]->vecGroup.size()-1;

		InvalidateList();
	}

	return posGroup;
}

void CIMClusterListCtrl::SetGroupTextColor(CL_GROUP_POS posGroup, COLORREF clrText)
{
	if (posGroup.IsNull()==false)
	{
		m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->clrText = clrText;
		InvalidateList();
	}
}

void CIMClusterListCtrl::SetGroupState(CL_GROUP_POS posGroup, CLS_STATE state)
{
	if ( posGroup.IsNull()==false )
	{
		m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->state = state;
		InvalidateList();
	}
}

void CIMClusterListCtrl::SetGroupName(CL_GROUP_POS posGroup, CString strTitle)
{
	if ( posGroup.IsNull()==false )
	{
		strTitle.Trim();
		strncpy(m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->szTitle,strTitle,sizeof(m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->szTitle) - 1);	
		InvalidateList();
	}
}

CString CIMClusterListCtrl::GetGroupName(CL_GROUP_POS posGroup)
{
	if ( posGroup.IsNull()==false )
	{
		return m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->szTitle;
	}
	else
	{
		return "";
	}
}

void CIMClusterListCtrl::DeleteItem(CL_ITEM_POS posItem)
{
	if ( posItem.IsNull()==false )
	{
		delete m_vecRoot[posItem.nRoot]->vecGroup[posItem.nGroup]->vecItem[posItem.nItem];

		m_vecRoot[posItem.nRoot]->vecGroup[posItem.nGroup]->vecItem.erase(
			m_vecRoot[posItem.nRoot]->vecGroup[posItem.nGroup]->vecItem.begin()
			+ posItem.nItem);

		if ( m_hWnd )
		{
			InvalidateList();
		}
	}
}

void CIMClusterListCtrl::DeleteAllItems()
{
	for ( int n=0; n<m_vecRoot.size(); n++ )
	{
		for (int i = 0; i < m_vecRoot[n]->vecGroup.size(); i++)
		{
			delete m_vecRoot[n]->vecGroup[i];
		}
		m_vecRoot[n]->vecGroup.clear();
		delete m_vecRoot[n];
	}

	m_vecRoot.clear();

	m_OriginY = 0;
	m_nClientRealHeigh = 0;

	m_posItemSelected.nGroup = -1;
	m_posItemSelected.nItem = -1;
	m_posItemSelected.nRoot = -1;
	CalcScrollInfo();
	InvalidateList();
}

CL_ITEM_POS CIMClusterListCtrl::AddItem(CL_GROUP_POS posGroup, CString strText, DWORD dwData)
{
	CL_ITEM_POS posItem;

	if ( posGroup.IsNull()==false )
	{
		CL_ITEM_INFO  * item = new CL_ITEM_INFO();

		strncpy(item->szText,strText,sizeof(item->szText)-1);	
		item->dwData  = dwData;

		m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->vecItem.push_back(item);
		InvalidateList();

		posItem.nRoot  = posGroup.nRoot;
		posItem.nGroup = posGroup.nGroup;
		posItem.nItem  = m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->vecItem.size()-1;
	}

	return posItem;
}

void CIMClusterListCtrl::SetItem(CL_ITEM_POS posItem, CString strText, DWORD dwData /* = 0 */)
{
	if ( posItem.IsNull()==false )
	{
		CL_ITEM_INFO * Item = m_vecRoot[posItem.nRoot]->vecGroup[posItem.nGroup]->vecItem[posItem.nItem];
		strncpy(Item->szText,strText,sizeof(Item->szText) - 1);	
		Item->dwData = dwData;
		
		InvalidateList();
	}
}

void CIMClusterListCtrl::SwapGroup(CL_GROUP_POS pos1, CL_GROUP_POS pos2)
{
	if ( pos1==pos2 || pos1.IsNull() || pos2.IsNull() )
	{
		return ;
	}
	else
	{
		CL_GROUP_INFO * p1 = m_vecRoot[pos1.nRoot]->vecGroup[pos1.nGroup];
		CL_GROUP_INFO * p2 = m_vecRoot[pos2.nRoot]->vecGroup[pos2.nGroup];

		m_vecRoot[pos1.nRoot]->vecGroup[pos1.nGroup] = p2;
		m_vecRoot[pos2.nRoot]->vecGroup[pos2.nGroup] = p1;

		InvalidateList();
	}
}

void CIMClusterListCtrl::SwapItem(CL_ITEM_POS pos1, CL_ITEM_POS pos2)
{
	if ( pos1==pos2 || pos1.IsNull() || pos2.IsNull() )
	{
		return ;
	}
	else
	{
		CL_ITEM_INFO * item = m_vecRoot[pos1.nRoot]->vecGroup[pos1.nGroup]->vecItem[pos1.nItem];
		m_vecRoot[pos1.nRoot]->vecGroup[pos1.nGroup]->vecItem[pos1.nItem] = m_vecRoot[pos2.nRoot]->vecGroup[pos2.nGroup]->vecItem[pos2.nItem];
		m_vecRoot[pos2.nRoot]->vecGroup[pos2.nGroup]->vecItem[pos2.nItem] = item;

		InvalidateList();
	}
}

void CIMClusterListCtrl::SetItemText(CL_ITEM_POS posItem, CString strText)
{
	if ( posItem.IsNull()==false )
	{
		strncpy(m_vecRoot[posItem.nRoot]->vecGroup[posItem.nGroup]->vecItem[posItem.nItem]->szText,strText,
			sizeof(m_vecRoot[posItem.nRoot]->vecGroup[posItem.nGroup]->vecItem[posItem.nItem]->szText) - 1);
		InvalidateList();
	}
}

CString CIMClusterListCtrl::GetItemText(CL_ITEM_POS posItem)
{	
	if ( posItem.IsNull()==false )
        return m_vecRoot[posItem.nRoot]->vecGroup[posItem.nGroup]->vecItem[posItem.nItem]->szText;
	else
		return "";
}

void CIMClusterListCtrl::SetCurSel(CL_ITEM_POS posItem)
{
	if ( posItem.IsNull() )
	{
		return ;
	}

	m_posItemSelected = posItem;

	if ( m_vecRoot[posItem.nRoot]->vecGroup[posItem.nGroup]->state!=CLS_EXPAND )
	{
		m_vecRoot[posItem.nRoot]->vecGroup[posItem.nGroup]->state = CLS_EXPAND;
	}

    if ( m_hWnd )
	{
		//OnPaint();
		RecalcLayout();

		//if ( nItem<0 )
		//	m_OriginY = m_vecRoot[posItem.nRoot]->vecGroup[posItem.nGroup]->rcBar.top;
		//else
		//	m_OriginY = m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->vecItem[nItem]->rcItem.top;

		//InvalidateList();
		//CalcScrollInfo();
	}
}

void CIMClusterListCtrl::ClearCurSel()
{
	m_posItemSelected.nGroup = -1;
	m_posItemSelected.nItem = -1;
	m_posItemSelected.nRoot = -1;
}

CL_GROUP_POS CIMClusterListCtrl::FindCluster(int nClusterId)
{
	CL_GROUP_POS posSel;

	int nRoot = m_vecRoot.size();
	for ( int n=0; n<nRoot; n++ )
	{
		vector<CL_GROUP_INFO *> & groups = m_vecRoot[n]->vecGroup;

		int nCGroup = groups.size();
		int i;
		CLUSTER_INFO * pInfo;
		for(i = 0;i < nCGroup;i++)
		{
			pInfo = (CLUSTER_INFO *)groups[i]->dwData;
			if(pInfo)
			{
				if(pInfo->nFamilyID == nClusterId)
				{
					posSel.nRoot = n;
					posSel.nGroup = i;
					return posSel;
				}
			}
		}
	}

	return posSel;
}

CL_ITEM_POS CIMClusterListCtrl::GetCurSel()
{
	return m_posItemSelected;
}

DWORD CIMClusterListCtrl::GetItemData(CL_ITEM_POS posItem)
{
	if ( posItem.IsNull()==false )
	{
		vector<CL_GROUP_INFO *> & vecGroup = m_vecRoot[posItem.nRoot]->vecGroup;

		if ( posItem.nGroup>-1 && posItem.nGroup<vecGroup.size() )
		{
			if ( posItem.nItem>-1 && posItem.nItem<vecGroup[posItem.nGroup]->vecItem.size() )
				return vecGroup[posItem.nGroup]->vecItem[posItem.nItem]->dwData;
		}
	}
	
	return 0;
}

void CIMClusterListCtrl::SetItemData(CL_ITEM_POS posItem, DWORD dwData)
{
	if ( posItem.IsNull()==false )
	{
		vector<CL_GROUP_INFO *> & vecGroup = m_vecRoot[posItem.nRoot]->vecGroup;

		if ( posItem.nGroup>-1 &&  posItem.nGroup<vecGroup.size() )
		{
			if ( posItem.nItem>-1 && posItem.nItem<vecGroup[posItem.nGroup]->vecItem.size() )
				vecGroup[posItem.nGroup]->vecItem[posItem.nItem]->dwData = dwData;
		}
	}
}

void CIMClusterListCtrl::SetItemTextColor(CL_ITEM_POS posItem, COLORREF clrText)
{
	if ( posItem.IsNull()==false )
	{
		FRIEND_INFO *pf=(FRIEND_INFO *)m_vecRoot[posItem.nRoot]->vecGroup[posItem.nGroup]->vecItem[posItem.nItem]->dwData;
		m_vecRoot[posItem.nRoot]->vecGroup[posItem.nGroup]->vecItem[posItem.nItem]->clrText = clrText;

		InvalidateList();
	}
}

int CIMClusterListCtrl::MoveItem(CL_ITEM_POS posItem, CL_GROUP_POS posGroup)
{
	if( posGroup.IsNull() || posItem.IsNull() )
	{
		return 0;
	}

	CL_ITEM_INFO * item = m_vecRoot[posItem.nRoot]->vecGroup[posItem.nGroup]->vecItem[posItem.nItem];
	m_vecRoot[posItem.nRoot]->vecGroup[posItem.nGroup]->vecItem.erase( 
		m_vecRoot[posItem.nRoot]->vecGroup[posItem.nGroup]->vecItem.begin()+posItem.nItem );

	m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->vecItem.push_back(item);
	m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->state = CLS_EXPAND;

	m_posItemSelected.nRoot = posGroup.nRoot;
	m_posItemSelected.nGroup = posGroup.nGroup;
	m_posItemSelected.nItem = m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->vecItem.size()-1;
	
	InvalidateList();

	return 1;
}

// 只确定了各组项的大概范围位置,组内细节还没有计算
void CIMClusterListCtrl::RecalcLayout()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int nPreHeight = 0;	
	int nRoot      = m_vecRoot.size();

	for ( int n=0; n<nRoot; n++ )
	{
		CRect & rcRootBar = m_vecRoot[n]->rcBar;

		rcRootBar.top    = nPreHeight;
		rcRootBar.bottom = rcRootBar.top + ROOT_BAR_CY;
		rcRootBar.left = LIST_START_X;
		rcRootBar.right = rcClient.right-2;

		nPreHeight += ROOT_BAR_CY;

		if ( m_vecRoot[n]->state == CLS_EXPAND )
		{
			vector<CL_GROUP_INFO *> & vecGroup = m_vecRoot[n]->vecGroup;

			for (int iGroup = 0; iGroup < vecGroup.size(); iGroup++)
			{
				CRect rcGroup;
				
				rcGroup.left   = LIST_START_X;
				rcGroup.top    = nPreHeight ;
				rcGroup.right  = rcClient.right - 2;
				rcGroup.bottom = rcGroup.top + GROUP_BAR_CY;

				int  nItems  = vecGroup[iGroup]->vecItem.size();
				BOOL bExpand = (vecGroup[iGroup]->state == CLS_EXPAND);  // 永远false，哈哈
				// 如果当前组为展开,则计算高度时需要加上组内各项高度

				nPreHeight = rcGroup.bottom;

				if ( bExpand )
				{
					if ( nItems>0 )
						nPreHeight = nPreHeight + nItems*m_nItemHeight+12;
				}

				vecGroup[iGroup]->rcBar = rcGroup;
			}
		}		
	}
	
	if (m_nClientRealHeigh != nPreHeight) 
	{
		// 如果客户端实际高度超过显示范围高度则需要改变滑动条的长度
		m_nClientRealHeigh = max(nPreHeight, rcClient.Height());
		CalcScrollInfo();
	}	
	
	if (m_nClientRealHeigh - m_OriginY < rcClient.Height()) 
	{
		m_OriginY = m_nClientRealHeigh - rcClient.Height();
	}
	
	if (m_OriginY < 0) 
	{
		m_OriginY = 0;
	}
}

void CIMClusterListCtrl::OnPaint() 
{
	CPaintDC dc(this);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CRect rcClient;
	// 获取客户区域大小
	GetClientRect(&rcClient);
	// 调整画布的大小
	if(m_nHeight != rcClient.Height() || m_nWidth != rcClient.Width())
	{
		m_nHeight = rcClient.Height();
		m_nWidth = rcClient.Width();
		m_canvas.DeleteObject();
		m_canvas.CreateCompatibleBitmap(&dc, m_nWidth, m_nHeight);
		m_bRedraw = true;
	}
	// 选择画布
	CBitmap* pOldBmp = memDC.SelectObject(&m_canvas);
	// 绘图
	if(m_bRedraw)
	{
		CBrush brushBack(m_clrBkGround);
		memDC.FillRect(&rcClient, &brushBack);

		CFont* pFont = memDC.SelectObject(&g_font12);
		// 计算列表内各项的范围大小
		RecalcLayout();
		int nRootCount = m_vecRoot.size();

		for (int nRoot = 0; nRoot < nRootCount; nRoot++)
		{		
			DrawRoot(nRoot, &memDC);
		}

		memDC.SelectObject(pFont);
		m_bRedraw = false;
	}

	dc.BitBlt(0, 0, rcClient.Width(), rcClient.Height(), &memDC, 0, 0, SRCCOPY);
	memDC.SelectObject(pOldBmp);
}

//void CIMClusterListCtrl::DrawGroupBar(CDC *pDC,CRect rcBar, CPoint ptCursor,bool bSelected, bool bExpand)
//{
	//bool bHover = rcBar.PtInRect(ptCursor);

	//CBitmap *pBitmapBar = NULL;

	//if (bExpand)
	//	pBitmapBar = &m_bmpGroupBarOpen;
	//else
	//if (bHover)
	//	pBitmapBar = &m_bmpGroupBarHover;
	//else
	//	pBitmapBar = &m_bmpGroupBarNormal;

	//CDC dcMem;
	//dcMem.CreateCompatibleDC(pDC);

	//BITMAP bi;
	//pBitmapBar->GetBitmap(&bi);

	//CBitmap *pOld = dcMem.SelectObject(pBitmapBar);

	//pDC->TransparentBlt(rcBar.left,rcBar.top,10,bi.bmHeight,&dcMem,0,0,10,bi.bmHeight,RGB(0,255,0));
	//pDC->StretchBlt(10,rcBar.top,rcBar.Width()-15,bi.bmHeight,&dcMem,10,0,25,bi.bmHeight,SRCCOPY);
	//pDC->TransparentBlt(rcBar.right-10,rcBar.top,10,bi.bmHeight,&dcMem,bi.bmWidth-10,0,10,bi.bmHeight,RGB(0,255,0));

	//dcMem.SelectObject(pOld);
//}

void CIMClusterListCtrl::DrawRoot(int nRoot, CDC *pDC)
{
	int nSavedDC = pDC->SaveDC();
	pDC->SetWindowOrg(0,m_OriginY);

	CL_ROOT_INFO *Root = m_vecRoot[nRoot];
	int nGroup = Root->vecGroup.size();

	Root->rcButton.left = Root->rcBar.left;
	Root->rcButton.top = Root->rcBar.top + (ROOT_BAR_CY-16)/2;
	Root->rcButton.right = Root->rcButton.left + GRP_BTN_W;
	Root->rcButton.bottom = Root->rcButton.top + GRP_BTN_W;

	if ( Root->state==CLS_EXPAND )
		m_ButtonImages.Draw(pDC,0,Root->rcButton.TopLeft(),ILD_NORMAL);
	else
		m_ButtonImages.Draw(pDC,1,Root->rcButton.TopLeft(),ILD_NORMAL);

	Root->rcText.left = Root->rcBar.left + GRP_BTN_W + SPACE_X;
	Root->rcText.top  = Root->rcBar.top + (ROOT_BAR_CY-m_nTextHeight)/2;


	CString strRoot;
	strRoot.Format("%s (%d)",Root->strTitle,Root->vecGroup.size() );
	CSize szText = pDC->GetTextExtent(strRoot);
	Root->rcText.right = Root->rcText.left+szText.cx;
	Root->rcText.bottom = Root->rcText.top+szText.cy;

/*不画框
	if ( nRoot==m_posItemSelected.nRoot && (m_posItemSelected.nItem==-1 && m_posItemSelected.nGroup==-1 ))
	{
		CRect rcSelText = Root->rcText;
		rcSelText.InflateRect(1,1);

		CBrush   brushBack(RGB(247,247,247));
		CPen     pen(PS_SOLID,1,RGB(233,233,233));
		CBrush * pOldBrush   = pDC->SelectObject(&brushBack);
		CPen *   pOldPen     = pDC->SelectObject(&pen);

		pDC->Rectangle(rcSelText);

		pDC->SelectObject(pOldPen);
		pDC->SelectObject(pOldBrush);
	}
*/
	if ( Root->state==CLS_EXPAND )
	{
		/*  不画省略号
		if ( nGroup>0 )
		{
			CPen penDot;
			//penDot.CreatePen(PS_SOLID,1,RGB(0,0,0));
			LOGBRUSH logBrush;
			logBrush.lbStyle = BS_SOLID;
			logBrush.lbColor = RGB(0,0,0);
			penDot.CreatePen(PS_DOT|PS_GEOMETRIC|PS_JOIN_ROUND, 1, &logBrush);

			pDC->SelectObject(&penDot);

			int nLineLeft = Root->rcButton.left+8;
			pDC->MoveTo(nLineLeft,Root->rcButton.bottom);
			pDC->LineTo(nLineLeft,Root->vecGroup[nGroup-1]->rcBar.top+GROUP_BAR_CY/2+1);

			for ( int iGroup=0; iGroup<nGroup; iGroup++ )
			{
				int nLineTop = Root->vecGroup[iGroup]->rcBar.top+GROUP_BAR_CY/2;
				pDC->MoveTo(nLineLeft+2,nLineTop);
				pDC->LineTo(nLineLeft+10,nLineTop);
			}
		}
		*/
		for ( int n=0; n<nGroup; n++ )
		{
			DrawGroup(nRoot,n,pDC);
		}	

		pDC->TextOut(Root->rcText.left,Root->rcText.top,strRoot);
	}
	else//判断是否有群消息
	{
		if ( nGroup>0 )
		{
			for ( int n=0; n<nGroup; n++ )
			{
				CL_GROUP_INFO *Group = Root->vecGroup[n];

				if ( Group->bFlash )
				{
					int i = ( Group->iFlash % 3 );

					if ( i==1 )
						pDC->SetTextColor(RGB(0,255,0));
					else
					if ( i==2 )
						pDC->SetTextColor(RGB(255,255,0));

					break;
				}
			}
		}

		pDC->TextOut(Root->rcText.left,Root->rcText.top,strRoot);
	}

	pDC->RestoreDC(nSavedDC);
}

void CIMClusterListCtrl::DrawGroup(int nRoot, int nGroup, CDC *pDC )
{	
	POINT   ptCursor;
	GetCursorPos(&ptCursor);
	ScreenToClient(&ptCursor);
	ptCursor.y+=m_OriginY;

	CL_GROUP_INFO * Group = m_vecRoot[nRoot]->vecGroup[nGroup];
	// 得到当前列表控件的区域
	CRect rcView;
	GetClientRect(&rcView);
	rcView.top+=m_OriginY;
	rcView.bottom+=m_OriginY;
	// 该组的高度
	int nGroupHeight;
	nGroupHeight = Group->vecItem.size() * m_nItemHeight;
	// 如果当前组在可视区域之下则不显示
	if(Group->rcBar.top > rcView.bottom )
	{
		return;
	}
	// 如果当前组在可视区域之上则不显示
	else if(Group->rcBar.bottom + nGroupHeight < rcView.top)
	{
		return;
	}

	int nSavedDC = pDC->SaveDC();
	pDC->SetWindowOrg(0,m_OriginY);

	const int nLeftMargin = 15;	
	const int nTextTop    = ( m_nItemHeight-m_nTextHeight)/2;
	// 获得组的范围
	CRect rcBar = Group->rcBar;
	//rcBar.left+=2;
	//rcBar.top +=2;

	pDC->SetBkMode(TRANSPARENT);
	pDC->SelectObject(&g_font12);
		
	/*

	CRect & rcBtnExpand = Group->rcBtnExpand;

	rcBtnExpand.left   = rcBar.left+2;
	rcBtnExpand.top    = rcBar.top+(GROUP_BAR_CY-16)/2;
	rcBtnExpand.right  = rcBtnExpand.left+15;
	rcBtnExpand.bottom = rcBtnExpand.top+15;

	if ( Group->state==CLS_EXPAND )
		m_ButtonImages.Draw(pDC,0,rcBtnExpand.TopLeft(),ILD_NORMAL);
	else
		m_ButtonImages.Draw(pDC,1,rcBtnExpand.TopLeft(),ILD_NORMAL);
*/

	CString strGroup ;
	int nOnlineCount = GetOnlineCount(Group);
	int nTotal = Group->vecItem.size();
	if(nTotal == 0)
		strGroup = Group->szTitle;
	else
		strGroup.Format("%s (%d/%d)",Group->szTitle,nOnlineCount,nTotal);

	if ( strGroup=="" )
	{
		strGroup="-";
	}

	CSize  szGroupText =pDC->GetTextExtent(strGroup);
	CRect  rcGroupText;
	// 向右移动27
	rcGroupText.left    = rcBar.left + GRP_BTN_W + SPACE_X*2 + 45;
	rcGroupText.top     = rcBar.top + nTextTop;
	rcGroupText.right   = rcGroupText.left + szGroupText.cx;

	if ( Group->bLoadingMembers )//画...
	{
		if ( rcGroupText.right> ( rcBar.right-80 ) )
		{
			rcGroupText.right = rcBar.right-80;
		}
	}
	else
	{
		if ( rcGroupText.right> ( rcBar.right-50 ) )
		{
			rcGroupText.right = rcBar.right-50;
		}
	}

	rcGroupText.bottom  = rcGroupText.top + szGroupText.cy;

	CLUSTER_INFO *pCluster = (CLUSTER_INFO *)Group->dwData;

	CBrush brushBack(RGB(248,248,248));
	CPen   pen(PS_SOLID,1,RGB(233,233,233));

	pDC->SelectObject(&pen);
	pDC->SelectObject(&brushBack);

	rcGroupText.InflateRect(1,1);

	bool bSelectedGroup = (nRoot==m_posItemSelected.nRoot && nGroup==m_posItemSelected.nGroup);

	if ( bSelectedGroup && m_posItemSelected.nItem==-1)
	{
		CRect rcFill = Group->rcBar;
		rcFill.left = Group->rcBar.left + 2;
		pDC->FillSolidRect(rcFill,RGB(233,233,233));
	}
	// 设置获得的组名文本所在位置范围

	Group->rcText = rcGroupText;
	rcGroupText.top +=1;
	rcGroupText.left+=2;

	// 绘制组名文字
	if ( Group->bFlash )
	{
		int i = ( Group->iFlash % 3 );

		if ( i==1 )
			pDC->SetTextColor(RGB(0,255,0));
		else
		if ( i==2 )
			pDC->SetTextColor(RGB(255,255,0));
	}
	else
	{
		if ( Group->state==CLS_EXPAND || rcBar.PtInRect(ptCursor) )
		{
			if ( pCluster->nCreatorID==g_ImUserInfo.GetId() )
			{
				pDC->SetTextColor(RGB(255,0,0));
			}
			else
				pDC->SetTextColor(RGB(0,112,188));
		}
		else
		{
			if ( pCluster->nCreatorID == g_ImUserInfo.GetId() )
			{
				pDC->SetTextColor(RGB(255,0,0));
			}
			else
			pDC->SetTextColor(RGB(84,84,84));
		}

		if ( rcGroupText.PtInRect(ptCursor) )
		{
			pDC->SetTextColor(RGB(0,0,255));
			pDC->Draw3dRect(rcGroupText.left,rcGroupText.bottom-1,rcGroupText.right-rcGroupText.left,1,RGB(0,0,255),RGB(0,0,255));
		}
	}

	pDC->DrawText(strGroup, &rcGroupText, DT_TOP|DT_LEFT|DT_END_ELLIPSIS);

	if(pCluster->radio == CLUSTER_INFO::SHOW_NUM && pCluster->nMsgNum > 0)
	{	// 绘制未读消息数
		CImage imgMsgNum;
		LoadImageFromResource(&imgMsgNum, IDB_PNG_MSG_NUM_S, _T("PNG"));
		CRect rcImg;
		rcImg.left = rcGroupText.right + 5;
		rcImg.top = Group->rcBar.top + (Group->rcBar.Height()-imgMsgNum.GetHeight())/2;
		rcImg.right = rcImg.left + imgMsgNum.GetWidth();
		rcImg.bottom = rcImg.top + imgMsgNum.GetHeight();
		imgMsgNum.Draw(pDC->m_hDC, rcImg.left, rcImg.top);

		CString strNum;
		if(pCluster->nMsgNum < 99)
			strNum.Format("%d", pCluster->nMsgNum);
		else
			strNum = "99";
		CFont *pOldfont=pDC->SelectObject(&g_font8);
		pDC->SetTextColor(RGB(255,255,255));
		pDC->DrawText(strNum, &rcImg, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		pDC->SelectObject(pOldfont);
	}

	//群icon
	Group->rcFace.left   = rcBar.left + GRP_BTN_W + SPACE_X;
	Group->rcFace.top    = rcBar.top ;
	Group->rcFace.right  = Group->rcFace.left + 45;
	Group->rcFace.bottom = rcBar.bottom;

	//SetStretchBltMode(pDC->m_hDC,STRETCH_HALFTONE); 

	//g_ImgIconGroup.TransparentBlt(pDC->m_hDC,Group->rcFace.left, Group->rcFace.top,45,45,RGB(255,255,255));


	g_ImgFaceFrame.Draw(pDC->m_hDC, Group->rcFace.left, Group->rcFace.top,45,45);

	g_ImgIconGroup.Draw(pDC->m_hDC, Group->rcFace.left, Group->rcFace.top,45,45);

/*
	CBitmap * pBmpGroupHead = NULL;

	if ( Group->pBitmap20==NULL )
		pBmpGroupHead = &m_bmpFamilyHead;
	else
		pBmpGroupHead = Group->pBitmap20;

	DrawBitmap(pDC,Group->rcFace.left,Group->rcFace.top,pBmpGroupHead);
*/


	//g_imgStatus24.Draw(pDC,1,CPoint(Group->rcFace.left,Group->rcFace.top),ILD_NORMAL);
	//pDC->Draw3dRect(&Group->rcFace,RGB(200,201,203),RGB(200,201,203));

	//POINT pt;
	
	//高级设置
	//CRect & rcSetting = Group->rcBtnSetting;

	//rcSetting.left   = rcGroupText.left-1;
	//rcSetting.top    = rcBar.bottom - 24;
	//rcSetting.right  = rcSetting.left+ TOOL_BTN_SIZE;
	//rcSetting.bottom = rcSetting.top + TOOL_BTN_SIZE;

	//DrawBitmap(pDC,rcSetting.left,rcSetting.top,&m_bmpGroupTool,0,0,TOOL_BTN_SIZE,TOOL_BTN_SIZE,RGB(0,255,0));
	//DrawPtInRect(ptCursor,rcSetting,pDC,0,m_bMouseDown);

	//大杂烩
	//CRect & rcParty = Group->rcBtnParty;

	//rcParty.left   = rcSetting.right+TOOL_BTN_SPACE;
	//rcParty.top    = rcSetting.top;
	//rcParty.right  = rcParty.left+ TOOL_BTN_SIZE;
	//rcParty.bottom = rcParty.top + TOOL_BTN_SIZE;

	//DrawBitmap(pDC,rcParty.left,rcParty.top,&m_bmpGroupTool,TOOL_BTN_SIZE*8,0,TOOL_BTN_SIZE,TOOL_BTN_SIZE,RGB(0,255,0));
	//DrawPtInRect(ptCursor,rcParty,pDC,0,m_bMouseDown);

	//群相册
	//CRect & rcAlbum = Group->rcBtnAlbum;

	//rcAlbum.left   = rcParty.right+TOOL_BTN_SPACE;
	//rcAlbum.top    = rcParty.top;
	//rcAlbum.right  = rcAlbum.left+ TOOL_BTN_SIZE;
	//rcAlbum.bottom = rcAlbum.top + TOOL_BTN_SIZE;

	//DrawBitmap(pDC,rcAlbum.left,rcParty.top,&m_bmpGroupTool,TOOL_BTN_SIZE*6,0,TOOL_BTN_SIZE,TOOL_BTN_SIZE,RGB(0,255,0));
	//DrawPtInRect(ptCursor,rcAlbum,pDC,0,m_bMouseDown);

	//阻止群
	/*
	CRect & rcRefuse = Group->rcBtnRefuse;

	rcRefuse.left   = rcGroupText.right + 1;
	rcRefuse.top    = rcBar.top + 3;
	rcRefuse.right  = rcRefuse.left+ TOOL_BTN_SIZE;
	rcRefuse.bottom = rcRefuse.top + TOOL_BTN_SIZE;

	if ( pCluster->bRefused )
		DrawBitmap(pDC,rcRefuse.left,rcRefuse.top,&m_bmpGroupTool,TOOL_BTN_SIZE*3,0,TOOL_BTN_SIZE,TOOL_BTN_SIZE,RGB(0,255,0));
	else	
		DrawBitmap(pDC,rcRefuse.left,rcRefuse.top,&m_bmpGroupTool,TOOL_BTN_SIZE*2,0,TOOL_BTN_SIZE,TOOL_BTN_SIZE,RGB(0,255,0));

	DrawPtInRect(ptCursor,rcRefuse,pDC,0,m_bMouseDown);
*/
		
	//画家在群列表的...
	/*
	if ( Group->bLoadingMembers )//画...
	{
		pDC->SetTextColor(0xAA00AA);
		CString str = " 加载"+g_strDot;
		CSize sz = pDC->GetTextExtent(str);
		CRect rc = rcGroupText;
		rc.left  = 60+1;
		rc.right = rcBar.right-3;
		if ( rc.right>rc.left )
			pDC->DrawText(str,&rc, DT_TOP|DT_LEFT|DT_END_ELLIPSIS);
	}
*/
	//CRect & rcBtnHome  = Group->rcBtnHome;
	// 博客主页按钮放在最右边
	/*rcBtnHome.left   = rcBar.right-20;
	rcBtnHome.top	 = rcBar.bottom-21;
	rcBtnHome.right  = rcBtnHome.left+16;
	rcBtnHome.bottom = rcBtnHome.top+16;

	rcBtnHome.InflateRect(1,1);
	DrawBitmap(pDC,rcBtnHome.left,rcBtnHome.top,IDB_FRIEND_BLOG);
	DrawPtInRect(ptCursor,rcBtnHome,pDC,0,m_bMouseDown);*/

	// 绘制组博客按钮
	//DrawBitmap(pDC,rcBtnHome.left,rcBtnHome.top,&m_bmpGroupTool,TOOL_BTN_SIZE*7,0,TOOL_BTN_SIZE,TOOL_BTN_SIZE,RGB(0,255,0));
	//CRect rcTemp = rcBtnHome;
	//rcTemp.InflateRect(1,1);
	//rcTemp.right+=2;
	//DrawPtInRect(ptCursor,rcTemp,pDC,0,m_bMouseDown);

	// 如果该组被展开，则绘制其成员
	//PRD需求，不用画
	/*
	if (Group->state==CLS_EXPAND)
	{
		int nItems = Group->vecItem.size();

		if ( nItems>0 )
		{
			CPen penDot;
			//penDot.CreatePen(PS_SOLID,1,RGB(0,0,0));
			LOGBRUSH logBrush;
			logBrush.lbStyle = BS_SOLID;
			logBrush.lbColor = RGB(0,0,0);
			penDot.CreatePen(PS_DOT|PS_GEOMETRIC|PS_JOIN_ROUND, 1, &logBrush);
			CPen *pOldpen = pDC->SelectObject(&penDot);

			int nLineLeft = Group->rcBtnExpand.left+8;
			int nLineTop  = Group->rcBtnExpand.bottom;

			pDC->MoveTo(nLineLeft,nLineTop);
			pDC->LineTo(nLineLeft,Group->rcBar.bottom+nItems*m_nItemHeight-m_nItemHeight/2+4);

			for ( int n=0; n<nItems; n++ )
			{
				int nTop = Group->rcBar.bottom+4+n*m_nItemHeight+m_nItemHeight/2;
				pDC->MoveTo(nLineLeft+2,nTop);
				pDC->LineTo(nLineLeft+13,nTop);
			}

			pDC->SelectObject(pOldpen);
		}

		for ( int ii=0; ii<nItems; ii++ )
		{
			CL_ITEM_INFO * pItem = Group->vecItem[ii];
			FRIEND_INFO  * pUser = (FRIEND_INFO *)pItem->dwData;

			bool bSelItem = false;

			pItem->rcItem.left  = rcBar.left + nLeftMargin ;
			pItem->rcItem.top   = rcBar.bottom + ii * m_nItemHeight+4,
			pItem->rcItem.right = rcBar.right - 2, 
			pItem->rcItem.bottom=pItem->rcItem.top + m_nItemHeight;
			//
			// 判断该项是否在可视范围内
			//
			if(pItem->rcItem.bottom < rcView.top)
				continue;	// 在可视范围之上则不绘制
			else if(pItem->rcItem.top > rcView.bottom)
				break;		// 在可视范围之下则不绘制,之后的项也一定在可视范围之下

			if ( bSelectedGroup && ii==m_posItemSelected.nItem)
			{
				CRect rcFill = pItem->rcItem;
				rcFill.left += 15;
				pDC->FillSolidRect(&rcFill,RGB(233,233,233));
			}

			// 开始绘制头像
			pt.x = pItem->rcItem.left+18, pt.y = pItem->rcItem.top+3;

			CRect rcIcon(pt.x, pt.y, pt.x + HEAD_SZ, pt.y + HEAD_SZ);
			CRect rcIconBorder = rcIcon;

			rcIcon.InflateRect(1,1);

			if ( pUser->cluster_property!=0 ) //族长
			{
				rcIcon.left -= 22;
			}
			
			pItem->rcIcon = rcIcon;

			CBitmap * pBmpHead  = pBmpHead = GetUserBitmap(pUser,true);;

			if ( pBmpHead )
			{
				if( pUser->status!=XTREAM_OFFLINE)
				{
					::DrawBitmap(pDC,pt.x,pt.y,pBmpHead,0);
				}
				else
				{
					// 黑白
					DrawBitmap(pDC,pt.x,pt.y,pBmpHead,true);
				}

				
				pDC->Draw3dRect(&rcIconBorder,0xBBCCBB,0xBBCCBB);
			}
			else
			{
				if ( bSelItem )
					g_imgStatus52.Draw(pDC,GetDefaultImage(pUser->sex,pUser->status),pt,ILD_TRANSPARENT);
				else
					g_imgStatus35.Draw(pDC,GetDefaultImage(pUser->sex,pUser->status),pt,ILD_TRANSPARENT);
			}


			if ( pUser->cluster_property==CMEMBER_CREATOR ) //族长
			{
				DrawBitmap(pDC,pItem->rcItem.left,pt.y+1,IDB_CLUSTER_CREATOR);
			}

			if ( pUser->cluster_property==CMEMBER_ADMIN )//管理员
			{
				DrawBitmap(pDC,pItem->rcItem.left,pt.y+1,IDB_CLUSTER_ADMIN);
			}

			pt.x = pItem->rcItem.right-20, pt.y=pItem->rcItem.top+(m_nItemHeight-16)/2;

			pDC->SetTextColor(pItem->clrText);

			CSize  size1 =pDC->GetTextExtent(pItem->szText);

			CRect  rcItemText;

			rcItemText.left = pItem->rcItem.left + 60;

			rcItemText.top    = pItem->rcItem.top + nTextTop;
			rcItemText.bottom = rcItemText.top  + size1.cy;
			rcItemText.right  = rcItemText.left + size1.cx;

			pItem->rcText = rcItemText;

			if ( pUser->status==XTREAM_OFFLINE )
				pDC->SetTextColor(0xAAAAAA);
			
			pDC->DrawText(pItem->szText,strlen(pItem->szText),&rcItemText,DT_TOP|DT_LEFT|DT_END_ELLIPSIS);
		}
		
	}
	*/
	pDC->RestoreDC(nSavedDC);	
}

BOOL CIMClusterListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	/*CRect rcClient;
	//GetClientRect(&rcClient);
	//pDC->FillSolidRect(0, 0, rcClient.Width(), m_nClientRealHeigh, RGB(255, 255, 255));
	//return 1;

	BYTE r1 = GetRValue(m_clrBackBegin),
		 g1 = GetGValue(m_clrBackBegin),
		 b1 = GetBValue(m_clrBackBegin);
	
    BYTE r2 = GetRValue(m_clrBackEnd),
		 g2 = GetGValue(m_clrBackEnd),
		 b2 = GetBValue(m_clrBackEnd);
	// 渐变背景
	for (int nRow = rcClient.left; nRow <= rcClient.Height(); nRow++)
	{
		int r,g,b;
		r = r1 + (nRow * (r2-r1) / rcClient.Width());
		g = g1 + (nRow * (g2-g1) / rcClient.Width());
		b = b1 + (nRow * (b2-b1) / rcClient.Width());

		pDC->FillSolidRect(rcClient.left, nRow + m_OriginY, rcClient.Width(), 1, RGB(r, g, b));
	}
	
	//////////////////////////////////////////////////////////////////////////	
	// 立体边缘
	CPen pen(PS_SOLID, 1, RGB(255, 255, 255));
	CPen* pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(0, m_OriginY - 1);
	pDC->LineTo(rcClient.Width(), m_OriginY - 1); // 上线
	pDC->MoveTo(0, m_OriginY - 1);
	//pDC->LineTo(0, rcClient.Height() + m_OriginY - 1); // 左线
	
	CPen pen1(PS_SOLID, 1, RGB(54, 108, 222));
	pDC->SelectObject(&pen1);
	pDC->MoveTo(1, rcClient.Height() + m_OriginY - 1);
	pDC->LineTo(rcClient.Width() - 1, rcClient.Height() + m_OriginY - 1); // 下线
	if (m_nClientRealHeigh <= rcClient.Height()) 
	{
		//pDC->LineTo(rcClient.Width() - 1, -1 + m_OriginY - 1); // 右线
	}
	
	pDC->SelectObject(pOldPen); */

	return FALSE;
}

void CIMClusterListCtrl::PreSubclassWindow() 
{
	CDC *pDC = GetDC();

	CFont *pOldfont=pDC->SelectObject(&g_font12);

	m_nTextHeight =pDC->GetTextExtent("H我").cy;

	// 确定单项高度为文本或规定最大高度+5
	m_nItemHeight = ( m_nTextHeight>HEAD_SZ ? m_nTextHeight+5 : (HEAD_SZ+5) );
	//m_nItemHeight = 50;
	pDC->SelectObject(pOldfont);

	ReleaseDC(pDC);

	this->EnableToolTips(TRUE);

	CIMListCtrl::PreSubclassWindow();
}

void CIMClusterListCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	static int nPreOverGroup = -1;

	CL_ITEM_POS posSelItem;

	HitTest(point,posSelItem);

	if ( posSelItem.nRoot==-1 )
		return ;

	bool bInvalidate = false;

	if ( posSelItem.nGroup!=nPreOverGroup )
	{
		bInvalidate = true;
		nPreOverGroup = posSelItem.nGroup;
	}

	if ( m_bMouseOver==false )
	{
		SetTimer(TIMER_MOUSEMOVE,20,NULL);
		m_bMouseOver = true;
	}

	// 获得绝对坐标
	CPoint    ptReal(point.x,point.y+m_OriginY);
	CString   sTip="";
	HCURSOR   hCursor = LoadCursor(NULL,IDC_ARROW);	

	if ( posSelItem.nItem>-1 )
	{
		CL_ITEM_INFO *pItem = m_vecRoot[posSelItem.nRoot]->vecGroup[posSelItem.nGroup]->vecItem[posSelItem.nItem];

		bool bMouseOnFace = pItem->rcIcon.PtInRect(ptReal);

		if ( bMouseOnFace )
		{
			FRIEND_INFO *pFriend=(FRIEND_INFO *)pItem->dwData;

			if(pFriend->cluster_property==CMEMBER_CREATOR)
			{
				sTip="创建者";
			}
			else if(pFriend->cluster_property==CMEMBER_ADMIN)
			{
				sTip="管理员";
			}
			else
			{
				sTip.Format("%s 的头像",pFriend->nickname);
			}
		}			
	}
	else
	if ( posSelItem.nGroup>-1 )
	{
		CL_GROUP_INFO* pGroup   = m_vecRoot[posSelItem.nRoot]->vecGroup[posSelItem.nGroup];
		CLUSTER_INFO * pCluster = (CLUSTER_INFO *)pGroup->dwData;

		CPoint ptReal = point;
		ptReal.y += m_OriginY;

		if ( pGroup->rcFace.PtInRect(ptReal) )
		{
			if (pGroup->bLoadingMembers)
			{
				sTip.Format("正在获取群　\"%s\" 的成员列表...",pCluster->szFamilyName );
			}
			else
			{
				// 如果该组信息被阻止，给出提示
				if(pCluster->bRefused)
				{
					sTip="群聊天消息被阻止";
					bInvalidate=true;
				}
			}
		}

		//// 鼠标滑过标题文字

		bool bMouseOnTitle       = pGroup->rcText.PtInRect(ptReal);

		if ( bMouseOnTitle )
		{
			hCursor=LoadCursor(NULL,IDC_HAND);

			sTip = "点击这里进入群聊天室";

			if ( m_bMouseOnTitle==false )
			{
				bInvalidate = true;
			}
		}
		else
		{
			if ( m_bMouseOnTitle==true )
			{
				bInvalidate = true;
			}
		}

		m_bMouseOnTitle = bMouseOnTitle;

		////鼠标滑过族徽
		bool bMouseOnGroupIcon  = pGroup->rcFace.PtInRect(ptReal);

		if ( bMouseOnGroupIcon )
		{
			if ( m_bMouseOnGroupIcon==false )
			{
				bInvalidate = true;
			}
		}
		else
		{
			if ( m_bMouseOnGroupIcon==true )
			{
				bInvalidate = true;
			}
		}

		m_bMouseOnGroupIcon = bMouseOnGroupIcon;


		////鼠标滑过高级设置
		//bool bMouseOnSetting  = pGroup->rcBtnSetting.PtInRect(ptReal);

		//if ( bMouseOnSetting )
		//{
		//	hCursor=LoadCursor(NULL,IDC_HAND);

		//	sTip = "群设置";

		//	if ( m_bMouseOnSetting==false )
		//	{
		//		bInvalidate = true;
		//	}
		//}
		//else
		//{
		//	if ( m_bMouseOnSetting==true )
		//	{
		//		bInvalidate = true;
		//	}
		//}

		//m_bMouseOnSetting = bMouseOnSetting;

		////鼠标滑过大杂烩
		//bool bMouseOnParty  = pGroup->rcBtnParty.PtInRect(ptReal);

		//if ( bMouseOnParty )
		//{
		//	hCursor=LoadCursor(NULL,IDC_HAND);

		//	sTip = "进入群讨论区";

		//	if ( m_bMouseOnParty==false )
		//	{
		//		bInvalidate = true;
		//	}
		//}
		//else
		//{
		//	if ( m_bMouseOnParty==true )
		//	{
		//		bInvalidate = true;
		//	}
		//}

		//m_bMouseOnParty = bMouseOnParty;

		////鼠标滑过相册
		//bool bMouseOnAlbum  = pGroup->rcBtnAlbum.PtInRect(ptReal);

		//if ( bMouseOnAlbum )
		//{
		//	hCursor=LoadCursor(NULL,IDC_HAND);

		//	sTip = "群相册";

		//	if ( m_bMouseOnAlbum==false )
		//	{
		//		bInvalidate = true;
		//	}
		//}
		//else
		//{
		//	if ( m_bMouseOnAlbum==true )
		//	{
		//		bInvalidate = true;
		//	}
		//}

		//m_bMouseOnAlbum = bMouseOnAlbum;

		
		////鼠标滑过群阻止
		bool bMouseOnRefuse  = pGroup->rcBtnRefuse.PtInRect(ptReal);

		if ( bMouseOnRefuse )
		{
			hCursor=LoadCursor(NULL,IDC_HAND);
			
			if(pCluster->bRefused)
				sTip="单击解除阻止";
			else
				sTip="单击阻止接收消息";

			if ( m_bMouseOnRefuse==false )
			{
				bInvalidate = true;
			}
		}
		else
		{
			if ( m_bMouseOnRefuse==true )
			{
				bInvalidate = true;
			}
		}

		m_bMouseOnRefuse = bMouseOnRefuse;

		////鼠标滑过+ -
		bool bMouseOnExpand  = pGroup->rcBtnExpand.PtInRect(ptReal);

		if ( bMouseOnExpand )
		{
			hCursor=LoadCursor(NULL,IDC_HAND);

			if ( pGroup->state==CLS_EXPAND )
			{
				sTip="点击隐藏群成员列表";
			}
			else
			{
				sTip="点击展开群成员列表";
			}

			if ( m_bMouseOnExpand==false )
			{
				bInvalidate = true;
			}
		}
		else
		{
			if ( m_bMouseOnExpand==true )
			{
				bInvalidate = true;
			}
		}

		m_bMouseOnExpand = bMouseOnExpand;

		//鼠标滑过群主页
		bool bMouseOnHome  = pGroup->rcBtnHome.PtInRect(ptReal);

		/*if ( bMouseOnHome )
		{
		hCursor=LoadCursor(NULL,IDC_HAND);

		sTip = "点击查看群主页";

		if ( m_bMouseOnHome==false )
		{
		bInvalidate = true;
		}
		}
		else
		{
		if ( m_bMouseOnHome==true )
		{
		bInvalidate = true;
		}
		}*/

		m_bMouseOnHome = bMouseOnHome;
	}
	else
	{
		bool bMouseOnRoot = m_vecRoot[posSelItem.nRoot]->rcButton.PtInRect(ptReal);
		if ( bMouseOnRoot )
		{
			hCursor=LoadCursor(NULL,IDC_HAND);
			if ( m_vecRoot[posSelItem.nRoot]->state==CLS_EXPAND )
			{
				sTip.Format("点击关闭%s",m_vecRoot[posSelItem.nRoot]->strTitle);
			}
			else
			{
				sTip.Format("点击展开%s",m_vecRoot[posSelItem.nRoot]->strTitle);
			}
		}
	}

	ShowToolTip(sTip);

	SetCursor(hCursor);

	if (bInvalidate)
	{
		InvalidateList();
	}

	
	CIMListCtrl::OnMouseMove(nFlags,point);
}

void CIMClusterListCtrl::InvalidateGroup( CL_GROUP_POS posGroup )
{
	if ( posGroup.IsNull()==false )
	{
		CRect rcBar = m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->rcBar;

		rcBar.top    -= m_OriginY  ;
		rcBar.bottom -= m_OriginY  ;

		m_bRedraw   = true;

		InvalidateRect(&rcBar,TRUE);
	}
}

void CIMClusterListCtrl::InvalidateItem(CL_ITEM_POS posItem, bool bErase /* = TRUE */)
{
	if ( posItem.IsNull()==false )
	{
		CRect rcItem = (posItem.nItem >= 0 ? m_vecRoot[posItem.nRoot]->vecGroup[posItem.nGroup]->vecItem[posItem.nItem]->rcItem : 
			m_vecRoot[posItem.nRoot]->vecGroup[posItem.nGroup]->rcBar);

		rcItem.OffsetRect(0, -m_OriginY);
		
		CRect rcClient;
		GetClientRect(&rcClient);
		if (rcItem.top >= rcClient.top || rcItem.bottom <= rcClient.bottom) 
		{
			InvalidateRect(&rcItem, bErase);
		}
	}
}

void CIMClusterListCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	CL_ITEM_POS posHot;
	HitTest(point, posHot);

	m_posItemSelected = posHot;

	InvalidateList();


	::SendMessage( GetParent()->m_hWnd,
		WM_CLUSTER_LIST_NOTIFY,
		NM_CL_RBUTTONDOWN,
		(LPARAM)&posHot);

	CIMListCtrl::OnRButtonDown(nFlags, point);
}

void CIMClusterListCtrl::OnRButtonUp(UINT nFlags, CPoint point)
{
	CL_ITEM_POS posHot;
	HitTest(point, posHot);

	::SendMessage( GetParent()->m_hWnd,
		WM_CLUSTER_LIST_NOTIFY,
		NM_CL_RBUTTONUP,
		(LPARAM)&posHot);

	m_bMouseDown=false;

	CIMListCtrl::OnRButtonUp(nFlags, point);
}

void CIMClusterListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	HitTest(point, m_posItemSelected);

	m_bMouseDown = true;

	if ( m_posItemSelected.nGroup>-1 )
	{
		CL_GROUP_INFO *pGroup=m_vecRoot[m_posItemSelected.nRoot]->vecGroup[m_posItemSelected.nGroup];
		m_bBtnHomeDown    = pGroup->rcBtnHome.PtInRect(point);
		m_bBtnRefuseDown  = pGroup->rcBtnRefuse.PtInRect(point);
	}

	InvalidateList();
	SetFocus();

	::PostMessage( GetParent()->m_hWnd,
				   WM_CLUSTER_LIST_NOTIFY,
				   NM_CL_LBUTTONDOWN,
				   0);

	SetCapture();

	CIMListCtrl::OnLButtonDown(nFlags, point);
}

void CIMClusterListCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CL_ITEM_POS posHot;
	HitTest(point, posHot);

	point.y += this->m_OriginY;

	bool bClickButton	= false;

	if ( posHot==m_posItemSelected)
	{
		if(posHot.nGroup>-1)
	{	
		CL_GROUP_INFO *pGroup	= m_vecRoot[posHot.nRoot]->vecGroup[posHot.nGroup];

		bool bMouseOnHome		= pGroup->rcBtnHome.PtInRect(point);
		bool bMouseOnRefuse		= pGroup->rcBtnRefuse.PtInRect(point);
		//bool bMouseOnSetting    = pGroup->rcBtnSetting.PtInRect(point);
		//bool bMouseOnAlbum      = pGroup->rcBtnAlbum.PtInRect(point);
		//bool bMouseOnParty      = pGroup->rcBtnParty.PtInRect(point);
		bool bMouseOnTitle      = pGroup->rcText.PtInRect(point);

		if (bMouseOnTitle && m_bMouseOnTitle )
		{
			::PostMessage( GetParent()->m_hWnd,
				WM_CLUSTER_LIST_NOTIFY,
				NM_CL_TITLE_CLICK,
				(LPARAM)pGroup);
		}

		if(bMouseOnHome && m_bMouseOnHome )
		{
			::PostMessage( GetParent()->m_hWnd,
				WM_CLUSTER_LIST_NOTIFY,
				NM_CL_HOME_CLICK,
				(LPARAM)pGroup);
		}

		if(bMouseOnRefuse && m_bMouseOnRefuse )
		{
			::PostMessage( GetParent()->m_hWnd,
				WM_CLUSTER_LIST_NOTIFY,
				NM_CL_REFUSE_CLICK,
				(LPARAM)pGroup);
		}
		//if(bMouseOnSetting && m_bMouseOnSetting )
		//{
		//	::PostMessage( GetParent()->m_hWnd,
		//		WM_CLUSTER_LIST_NOTIFY,
		//		NM_CL_SETTING_CLICK,
		//		(LPARAM)pGroup);
		//}
		//if(bMouseOnAlbum && m_bMouseOnAlbum )
		//{
		//	::PostMessage( GetParent()->m_hWnd,
		//		WM_CLUSTER_LIST_NOTIFY,
		//		NM_CL_ALBUM_CLICK,
		//		(LPARAM)pGroup);
		//}
		//if(bMouseOnParty && m_bMouseOnParty )
		//{
		//	::PostMessage( GetParent()->m_hWnd,
		//		WM_CLUSTER_LIST_NOTIFY,
		//		NM_CL_PARTY_CLICK,
		//		(LPARAM)pGroup);
		//}

		if ( posHot==m_posItemSelected )
		{
			if (posHot.nItem == -1 ) // 点击了分组栏，需要收放分组成员
			{
				if ( pGroup->rcBtnExpand.PtInRect(point) )
				{
					m_vecRoot[posHot.nRoot]->vecGroup[posHot.nGroup]->state = m_vecRoot[posHot.nRoot]->vecGroup[posHot.nGroup]->state == CLS_EXPAND ? CLS_CLOSE : CLS_EXPAND;
					InvalidateList();
					bClickButton = true;
					if ( m_vecRoot[posHot.nRoot]->vecGroup[posHot.nGroup]->state==CLS_EXPAND )
					{
						::SendMessage( GetParent()->m_hWnd,
							WM_CLUSTER_LIST_NOTIFY,
							NM_CL_EXPAND,
							(LPARAM)&posHot);
					}
				}
			}
		}
	}
		else
		if(posHot.nRoot>-1)
		{
			bool bMouseOnButton = m_vecRoot[posHot.nRoot]->rcButton.PtInRect(point);
			if ( bMouseOnButton )
			{
				if ( m_vecRoot[posHot.nRoot]->state==CLS_EXPAND )
					m_vecRoot[posHot.nRoot]->state=CLS_CLOSE;
				else
					m_vecRoot[posHot.nRoot]->state=CLS_EXPAND;
			}
		}
			
		if ( bClickButton==false )
		{
			::SendMessage( GetParent()->m_hWnd,
					WM_CLUSTER_LIST_NOTIFY,
					NM_CL_LBUTTONUP,
					(LPARAM)&posHot);
		}
	}

	::ReleaseCapture();

	m_bMouseDown		= false;
	m_bBtnHomeDown		= false;
	m_bBtnRefuseDown	= false;

	InvalidateList();

	CIMListCtrl::OnLButtonUp(nFlags, point);
}

void CIMClusterListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	CL_ITEM_POS posHot;
	HitTest(point, posHot);

	if ( posHot.nGroup>-1 && posHot.nItem==-1 )
	{
		CL_GROUP_INFO *pGroup = m_vecRoot[posHot.nRoot]->vecGroup[posHot.nGroup];

		point.y += this->m_OriginY;

		if ( !pGroup->rcBtnExpand.PtInRect(point) )
		{
			::SendMessage( GetParent()->m_hWnd,
				WM_CLUSTER_LIST_NOTIFY,
				NM_CL_LBUTTONDBLCLICK,
				(LPARAM)&posHot);
		}
	}
	else
	{
		if ( posHot.nRoot!=-1 && posHot.nItem==-1 && posHot.nGroup==-1 )
		{
			if(m_vecRoot[posHot.nRoot]->state==CLS_EXPAND)
				m_vecRoot[posHot.nRoot]->state=CLS_CLOSE;
			else
				m_vecRoot[posHot.nRoot]->state=CLS_EXPAND;

			RecalcLayout();
		}
		::SendMessage( GetParent()->m_hWnd,
			WM_CLUSTER_LIST_NOTIFY,
			NM_CL_LBUTTONDBLCLICK,
			(LPARAM)&posHot);
	}

	InvalidateList();

	CIMListCtrl::OnLButtonDblClk(nFlags,point);
}

void CIMClusterListCtrl::HitTest(const POINT& pt, CL_ITEM_POS &posSel)
{
	posSel.nItem = posSel.nGroup = posSel.nRoot = -1;

	CPoint point(pt);
	point.y += m_OriginY;

	int nRoot = m_vecRoot.size();

	for ( int iRoot=0; iRoot<nRoot; iRoot++ )
	{
		if ( m_vecRoot[iRoot]->rcBar.PtInRect(point) )
		{
			posSel.nRoot = iRoot;
			return ;
		}

		if ( m_vecRoot[iRoot]->state==CLS_CLOSE )
			continue;

		int nGroup = m_vecRoot[iRoot]->vecGroup.size();

		for (int iGroup = 0; iGroup < nGroup; iGroup++)
		{
			CL_GROUP_INFO *pGroup= m_vecRoot[iRoot]->vecGroup[iGroup];
			
			if (pGroup->rcBar.PtInRect(point))
			{
				posSel.nRoot  = iRoot;
				posSel.nGroup = iGroup;
				return;
			}
			
			if (pGroup->state == CLS_EXPAND)
			{
				for (int i = 0; i < pGroup->vecItem.size(); i++)
				{
					CL_ITEM_INFO *pItem= pGroup->vecItem[i];
					
					if (pItem->rcItem.PtInRect(point))
					{
						posSel.nRoot  = iRoot;
						posSel.nGroup = iGroup;
						posSel.nItem  = i;
						return ;
					}
				}
			}
		}
	}
}

int CIMClusterListCtrl::GetItemCount(CL_GROUP_POS posGroup)
{
	return 	m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->vecItem.size();
}

void CIMClusterListCtrl::SetGroupData(CL_GROUP_POS posGroup,DWORD dwData)
{
	if ( posGroup.IsNull()==false )
		m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->dwData=dwData;
}

CL_GROUP_INFO * CIMClusterListCtrl::GetGroupInfo(CL_GROUP_POS posGroup)
{
	if ( posGroup.IsNull()==false )
		return m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup];
	else
		return NULL;
}

DWORD CIMClusterListCtrl::GetGroupData(CL_GROUP_POS posGroup)
{
	if ( posGroup.IsNull()==false )
		return 	m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->dwData;
	else
		return 0;
}

int CIMClusterListCtrl::GetGroupCount(int nRoot)
{
	return m_vecRoot[nRoot]->vecGroup.size();
}

int CIMClusterListCtrl::GetRootCount()
{
	return m_vecRoot.size();
}

int CIMClusterListCtrl::GetGroupCount()
{
	int n=0;
	for ( int i=0; i<m_vecRoot.size(); i++ )
		n+=m_vecRoot[i]->vecGroup.size();

	return n;
}

BOOL CIMClusterListCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	OnVScroll(zDelta > 0 ? SB_LINEUP : SB_LINEDOWN, 0, 0);	
	return TRUE;
}

void CIMClusterListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
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
			CIMListCtrl::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - m_nItemHeight);
			m_OriginY -= m_nItemHeight;
			if (m_OriginY < 0) 
			{
				m_OriginY = 0;
			}
			
			InvalidateList();
		}
		break;
	case SB_LINEDOWN:
		if (m_OriginY + rcClient.Height() < m_nClientRealHeigh) 
		{
			CIMListCtrl::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + m_nItemHeight);
			m_OriginY += m_nItemHeight;
			
			InvalidateList();
		}
		break;
	case SB_PAGEDOWN:
		if (m_OriginY + rcClient.Height() < m_nClientRealHeigh) 
		{
			CIMListCtrl::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + rcClient.Height());
			m_OriginY += rcClient.Height();

			InvalidateList();
		}
		break;
	case SB_PAGEUP:
		if (m_OriginY > 0) 
		{
			CIMListCtrl::SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - rcClient.Height());
			m_OriginY -= rcClient.Height();
			if (m_OriginY < 0) 
			{
				m_OriginY = 0;
			}

			InvalidateList();
		}
		break;
	case SB_THUMBPOSITION:
		CIMListCtrl::SetScrollPos(SB_VERT, nPos);
		m_OriginY = m_OriginY / rcClient.Height() + nPos;		
		InvalidateList();
		break;
	case SB_THUMBTRACK:
		if (nPos < m_nClientRealHeigh) 
		{
			m_OriginY = nPos;
			//m_OriginY += m_nItemHeight;
			InvalidateList();
		}
		
		CIMListCtrl::SetScrollPos(SB_VERT, nPos);
		break;
	default:
		;
	}
	
	//CIMListCtrl::ShowScrollBar(SB_VERT, FALSE);
}

int CIMClusterListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{	
	m_ButtonImages.Create(12, 12, ILC_MASK | ILC_COLOR24, 2, 1);
	HBITMAP hbmp = ResLoadBitmap(IDB_GROUPLIST_BUTTON);//LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_GROUPLIST_BUTTON));
	ImageList_AddMasked(m_ButtonImages.m_hImageList, hbmp, RGB(0, 255, 0));
	DeleteObject((HGDIOBJ)hbmp);	


	//m_ToolTip.SetDelayTime(1000);

	return 0;
}

void CIMClusterListCtrl::CalcScrollInfo()
{
	if (IsWindow(m_hWnd)) 
	{
		CRect rcClient;
		GetClientRect(&rcClient);
		SCROLLINFO si;
		memset(&si, 0, sizeof(si));
		si.cbSize = sizeof(si);
		si.fMask  = SIF_PAGE | SIF_RANGE;
		si.nPage  = rcClient.Height();
		si.nMin = 0;
		si.nMax = m_nClientRealHeigh - 1;
		if (si.nPage >= si.nMax) 
		{
			ShowScrollBar(SB_VERT, FALSE);
		}
		else
		{
			CIMListCtrl::SetScrollInfo(SB_VERT, &si);
		}
	}
}

void CIMClusterListCtrl::InvalidateList()
{
	// 设置重绘标志，强制重绘
	m_bRedraw = true;
	
	if ( IsWindow(m_hWnd) && m_bInvalidateLock==false )
	{
		
		Invalidate();
	}
}

void CIMClusterListCtrl::OnTimer(UINT nIDEvent)
{
	switch ( nIDEvent )
	{
	case TIMER_LOADING:
		{
			g_strDot += ".";

			if (g_iDotCount++>2)
			{
				g_iDotCount  = 0  ;
				g_strDot     = "" ;
			}
			
			for ( int nRoot=0; nRoot<m_vecRoot.size(); nRoot++ )
			{
				if ( m_vecRoot[nRoot]->state==CLS_EXPAND )
				{
					int nGroup = m_vecRoot[nRoot]->vecGroup.size();

					for ( int i=0; i<nGroup; i++ )
					{
						CL_GROUP_INFO *pGroup = m_vecRoot[nRoot]->vecGroup[i];

						if ( pGroup->bLoadingMembers )
						{
							CRect rcBar = pGroup->rcBar;
							rcBar.top    -= m_OriginY  ;
							rcBar.bottom -= m_OriginY  ;
							m_bRedraw=true;
							InvalidateRect(&rcBar);
						}
					}		
				}
		}
		}
		break;
	case TIMER_FLASH:
		{
			for ( int nRoot=0; nRoot<m_vecRoot.size(); nRoot++ )
			{
				BOOL bRootExpand = (m_vecRoot[nRoot]->state==CLS_EXPAND);
		
				int nGroup = m_vecRoot[nRoot]->vecGroup.size();

				for ( int i=0; i<nGroup; i++ )
				{
					CL_GROUP_INFO *pGroup = m_vecRoot[nRoot]->vecGroup[i];

					if ( pGroup->bFlash )
					{
						pGroup->iFlash++;
						if ( bRootExpand )
						{
							CRect rcBar = pGroup->rcBar;
							rcBar.top    -= m_OriginY  ;
							rcBar.bottom -= m_OriginY  ;
							m_bRedraw=true;
							InvalidateRect(&rcBar);
						}
						else
						{
							break;
						}
					}
				}
						
				if ( bRootExpand==FALSE )
				{
					CRect rcBar = m_vecRoot[nRoot]->rcBar;
					rcBar.top    -= m_OriginY  ;
					rcBar.bottom -= m_OriginY  ;
					m_bRedraw=true;
					InvalidateRect(&rcBar);
				}
			}
		}
		break;
	case TIMER_MOUSEMOVE:
		{
			CPoint ptCursor;
			CRect  rcClient;

			GetCursorPos(&ptCursor);
			GetWindowRect(&rcClient);

			if ( rcClient.PtInRect(ptCursor)==FALSE )
			{
				m_bMouseOver=false;
				InvalidateList();
				KillTimer(TIMER_MOUSEMOVE);
			}
		}
		break;
	}

	CIMListCtrl::OnTimer(nIDEvent);
}

void CIMClusterListCtrl::AddFlashFlag(CL_GROUP_POS posGroup)
{
	if ( posGroup.IsNull()==false )
	{
		int  nCount = m_vecRoot[posGroup.nRoot]->vecGroup.size();
		bool bFlag  = false;

		for ( int i=0; i<nCount; i++ )
		{
			if ( m_vecRoot[posGroup.nRoot]->vecGroup[i]->bFlash )
			{
				bFlag=true;
				break;
			}
		}

		if ( bFlag==false )
		{
			KillTimer(TIMER_FLASH);
			SetTimer(TIMER_FLASH, 355, NULL );
		}

		m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->bFlash = true;
	}
}

void CIMClusterListCtrl::AddLoadFlag(CL_GROUP_POS posGroup)
{
	if ( posGroup.IsNull()==false )
	{
		int  nCount = m_vecRoot[posGroup.nRoot]->vecGroup.size();
		bool bFlag  = false;

		for ( int i=0; i<nCount; i++ )
		{
			if ( m_vecRoot[posGroup.nRoot]->vecGroup[i]->bLoadingMembers )
			{
				bFlag=true;
				break;
			}
		}

		if ( bFlag==false )
		{
			KillTimer(TIMER_LOADING);
			SetTimer(TIMER_LOADING, 355, NULL );
			InvalidateGroup(posGroup);
		}

		m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->bLoadingMembers = true;
	}
}

void CIMClusterListCtrl::RemoveFlashFlag(CL_GROUP_POS posGroup)
{
	if ( posGroup.IsNull()==false )
	{
		m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->bFlash = false;

		int  nCount = m_vecRoot[posGroup.nRoot]->vecGroup.size();
		bool bFlag  = false;

		for ( int i=0; i<nCount; i++ )
		{
			if ( m_vecRoot[posGroup.nRoot]->vecGroup[i]->bFlash )
			{
				bFlag=true;
				break;
			}
		}

		if ( bFlag==false )
		{
			KillTimer(TIMER_FLASH);
			InvalidateGroup(posGroup);
		}
	}
}

void CIMClusterListCtrl::RemoveLoadFlag(CL_GROUP_POS posGroup)
{
	if ( posGroup.IsNull()==false )
	{
		m_vecRoot[posGroup.nRoot]->vecGroup[posGroup.nGroup]->bLoadingMembers = false;

		int  nCount = m_vecRoot[posGroup.nRoot]->vecGroup.size();
		bool bFlag  = false;

		for ( int i=0; i<nCount; i++ )
		{
			if ( m_vecRoot[posGroup.nRoot]->vecGroup[i]->bLoadingMembers )
			{
				bFlag=true;
				break;
			}
		}

		if ( bFlag==false )
		{
			::g_strDot = "";
			::g_iDotCount = 0;

			KillTimer(TIMER_LOADING);
		}
	}
}
