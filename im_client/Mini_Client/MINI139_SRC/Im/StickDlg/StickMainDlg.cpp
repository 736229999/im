// StickMainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "StickMainDlg.h"
#include ".\stickmaindlg.h"



#define HORI_MAGNETISM	10	// 水平磁性
#define VERT_MAGNETISM	10	// 垂直方向和上层子窗体的磁性
#define VERT_MAIN_MAGN	40	// 垂直方向和主窗体的磁性

#define TIMER_MOVEWINDOW	0x100

IMPLEMENT_DYNAMIC(CStickMainDlg, _BaseStickMainDlg)


BOOL CStickMainDlg::m_bDragFullWindows = TRUE;



INT GetDragFullWindows()
{
	// 定义有关的 hKEY, 在查询结束时要关闭。
	HKEY hKEY; 
	LPCTSTR data_Set= "Control Panel\\Desktop";
	//打开与路径 data_Set 相关的 hKEY，第一个参数为根键名称，第二个参数表。
	//表示要访问的键的位置，第三个参数必须为0，KEY_READ表示以查询的方式。
	//访问注册表，hKEY则保存此函数所打开的键的句柄。
	long ret0=(::RegOpenKeyEx(HKEY_CURRENT_USER,data_Set, 0, KEY_READ, &hKEY));
	if(ret0 != ERROR_SUCCESS)	//如果无法打开hKEY，则终止程序的执行
	{
		return -1;
	}
	// 查询有关的数据 (用户姓名 owner_Get)。
	BYTE	owner_Get[10];
	DWORD	type_1 = REG_SZ ; 
	DWORD	cbData_1 = 10; 
	//hKEY为刚才RegOpenKeyEx()函数所打开的键的句柄，″DragFullWindows″。
	//表示要查 询的键值名，type_1表示查询数据的类型，owner_Get保存所。
	//查询的数据，cbData_1表示预设置的数据长度。
	long ret1=::RegQueryValueEx(
		hKEY, 
		"DragFullWindows",
		NULL,
		&type_1, 
		owner_Get,
		&cbData_1
		);
	
	if(ret1 != ERROR_SUCCESS)
	{
		ret1 = RegCloseKey(hKEY);
		return -1;
	}
	ret1 = RegCloseKey(hKEY);
	if(strcmp("0",(const char *)owner_Get) == 0)
	{
		return 0;
	}
	return 1;
}

CStickMainDlg::CStickMainDlg(UINT nIDTemplate, CWnd * pParent /* = NULL */)
	: _BaseStickMainDlg(nIDTemplate,pParent),m_rctLastMove(0,0,10,10)
{
	
	m_bDragFullWindows = GetDragFullWindows();
	m_pLeftTopSub = NULL;
	m_pRightTopSub = NULL;
	m_bMoveStart = FALSE;
	m_bSizeStart = FALSE;
	m_rctLastSize = m_rctLastMove;
}


CStickMainDlg::~CStickMainDlg()
{
}

void CStickMainDlg::DoDataExchange(CDataExchange* pDX)
{
	_BaseStickMainDlg::DoDataExchange(pDX);
}


int CStickMainDlg::PlaceSubWnd(CStickDlg *pSub,PLACE_STICK_WND nCmdPlace)
{
	if(nCmdPlace == PSW_AUTO)
	{
		// 
	}
	else if(nCmdPlace == PSW_LEFT)
	{
		//
	}
	else if(nCmdPlace == PSW_RIGHT)
	{
		//

	}
	return -1;
}

bool CStickMainDlg::AutoAttachWnd(StickDlgIt itWnd,LPRECT pRct)
{
	CRect rctMain;
	int nWidth;
	int nScreenX,nScreenY;
	nScreenX = ::GetSystemMetrics(SM_CXSCREEN);
	// 判断吸附方向
	GetWindowRect(&rctMain);
	nWidth = rctMain.Width();
	// 屏幕上的剩余平均宽度
	nWidth = (nScreenX - nWidth) / 2;
	
	// 如果窗体比桌面还大，就默认放在中间
	if(nWidth < 10)
	{
		itWnd->nDirect = STICK_WND::AUTO;
		CRect rtSub;
		int nW,nH;
		itWnd->GetRect(rtSub);
		nScreenY = ::GetSystemMetrics(SM_CYSCREEN);
		nW = rtSub.Width();
		nH = rtSub.Height();
		pRct->top = (nScreenY - nH)/2;
		pRct->bottom = pRct->top + nH;
		pRct->left = (nScreenX - nW)/2;
		pRct->right = pRct->left + nW;
		return true;
	}
	if(rctMain.left > nWidth )
	{
		itWnd->nDirect = STICK_WND::LEFT;
	}
	else
	{
		itWnd->nDirect = STICK_WND::RIGHT;
	};
	if(AttachWnd(itWnd,pRct))
	{
		return true;
	}
	
	return false;
}

int CStickMainDlg::ShowSubWnd(CStickDlg * pSub,SHOW_STICK_WND nCmdShow)
{
	ASSERT(pSub);
	if(pSub == NULL)
		return -1;
	StickDlgIt itWnd = FindWnd(pSub);
	// 如果被显示的窗体不存在，则返回
	if(itWnd == m_lstWnds.end())
		return -1;
	STICK_WND * pSWnd = &(*itWnd);
	
	CRect rct;
	//
	// 如果要吸附窗体,则在相同方向最后一个可视窗体下显示
	//
	if(nCmdShow == SSW_ATTACH)
	{	
		if(AttachWnd(itWnd,&rct))
			pSub->MoveWindow(&rct);
	}
	//
	// 如果要最小化窗体，需要在最小化后将该窗体之下的其他窗体移上去
	//
	else if(nCmdShow == SSW_HIDE)
	{
		HideWnd(itWnd);
	}
	else if(nCmdShow == SSW_CLOSE)
	{
		CloseWnd(itWnd);
	}
	else if(nCmdShow == SSW_DETACH)
	{
		DetachWnd(pSWnd);
	}
	else if(nCmdShow == SSW_AUTO)
	{
		if(itWnd->IsAttach() )
		{
			if ( itWnd->IsWindowOutOfScreen() )
				DetachWnd(pSWnd);

			if(AttachWnd(itWnd,&rct))
				pSub->MoveWindow(&rct);
		}
		else
		{
			HideWnd(itWnd);
			if(AutoAttachWnd(itWnd,&rct))
				pSub->MoveWindow(&rct);
		}
		//else
		//{
		//	pSub->PlaceWnd(PSW_AUTO);
		//}
	}
	return 0;
}

bool CStickMainDlg::SubDetachMainTest(	StickDlgIt itWnd,
										LPRECT pRct, 
										CPoint pt, 
										CPoint& ptDown )
{

	int nY = pt.y - ptDown.y;
	if(abs(nY) > VERT_MAIN_MAGN)
	{
		// 分离
		DetachWnd(&(*itWnd));
		// 垂直方向分离的时候窗体需要移动
		pRct->top += nY;
		pRct->bottom += nY; 
		// 更新这次分离时鼠标的位置
		ptDown = pt;	 
		return true;
	}
	// 水平方向和主窗体间隔超过磁性范围则分离
	int nX = pt.x - ptDown.x;
	CRect rct;
	itWnd->GetRect(rct);
	if(abs(nX) > HORI_MAGNETISM)
	{
		// 分离
		DetachWnd(&(*itWnd));
		// 水平方向分离的时候窗体需要移动
		pRct->left += nX;
		pRct->right += nX; 
		pRct->top += nY;
		pRct->bottom += nY;
		// 更新这次分离时鼠标的位置
		ptDown = pt;	
		return true;
	}
	else
	{
		*pRct = rct;
	}
	return false;
}

bool CStickMainDlg::SubDetachSubTest(	StickDlgIt itWnd,	// 窗体
										LPRECT pRct,		// 新位置
										 CPoint pt,			// 当前鼠标位置
										CPoint& ptDown)		// 鼠标按下时的位置
{
	// 上层子窗体
	STICK_WND * pUpSWnd;
	pUpSWnd = itWnd->pWndTop;
	// 鼠标在垂直方向拖动的距离
	int nY = pt.y - ptDown.y;
	int nX = pt.x - ptDown.x;
	CRect rct;
	itWnd->GetRect(rct);

	if(pUpSWnd != NULL)
	{
		if(abs(nY) > VERT_MAGNETISM )
		{
			// 分离
			pUpSWnd->pWndBottom = NULL;
			itWnd->pWndTop = NULL;
			itWnd->nShowState = STICK_WND::DETACH;
			// 垂直方向分离的时候窗体需要移动
			pRct->top += nY;
			pRct->bottom += nY; 
			pRct->left += nX;
			pRct->right += nX;
			// 更新这次分离时鼠标的位置
			ptDown = pt;	 
			return true;
		}
		else
		{
			*pRct = rct;
		}
	}
	return false;
}

int CStickMainDlg::SubAttachSubTest(StickDlgIt itWnd, 
									 LPRECT pRct, 
									 CPoint pt, 
									 CPoint & ptDown )
{
	// 如果当前子窗体上部已经有其他子窗体，则返回
	if(itWnd->pWndTop != NULL)
		return ATTACH_NONE;

	CRect rctLast;
	StickDlgIt it,itEnd;
	it = m_lstWnds.begin();
	itEnd = m_lstWnds.end();
	
	int nCenterX ;
	nCenterX = (pRct->left + pRct->right) / 2;
	int nYB = 0;
	int nYT = 0;


	for(;it != itEnd;it++)
	{
		if(it->pWndBottom && it->pWndTop)
		{
			// 在窗体列表中间的子窗体的不做判断
			continue;
		}
		if(it->pWnd == itWnd->pWnd)
		{
			// 过滤自己
			continue;
		}
		if(it->IsHide())
		{
			// 隐藏窗体不做处理
			continue;
		}
		it->GetRect(rctLast);
		// 窗体与窗体下方之间垂直间距
		nYB = pRct->top - rctLast.bottom;
		// 窗体与窗体上方之间垂直间距
		nYT = rctLast.top - pRct->bottom ;
		// 水平中点是否在子窗体垂直范围内
		if(nCenterX > rctLast.left && nCenterX < rctLast.right) 
		{
			// 判断是否在该窗体磁场范围内

			// 与子窗体下方垂直距离
			if(it->pWndBottom == NULL && abs(nYB) < VERT_MAGNETISM )
			{	
				pRct->top -= nYB;
				pRct->bottom -= nYB;
				pRct->left = rctLast.left;
				pRct->right = rctLast.right;
				ptDown = pt;
				AttachSubWnd(itWnd,it);
				return ATTACH_BOTTOM;
			}
			// 与子窗体上方垂直距离
			else if( it->pWndTop == NULL && 
					 itWnd->pWndBottom == NULL &&
					 it->IsAttach() == false &&
					 abs(nYT) < VERT_MAGNETISM)
			{
				// 该窗体必须是非吸附状态

				pRct->top += nYT;
				pRct->bottom += nYT;
				pRct->left = rctLast.left;
				pRct->right = rctLast.right;
				ptDown = pt;
				AttachSubWnd(it,itWnd);
				return ATTACH_TOP;
			}
		}
		
	}
	return ATTACH_NONE;
}

void CStickMainDlg::AttachSubWnd(StickDlgIt itWnd,StickDlgIt itTopWnd)
{
	

	itWnd->nDirect = itTopWnd->nDirect;
	itWnd->nShowState = itTopWnd->nShowState;
	itWnd->pWndTop = &(*itTopWnd);
	// 防止自身链接
	ASSERT(  itWnd->pWndTop != itWnd->pWndBottom);
	itTopWnd->pWndBottom = &(*itWnd);
	// 防止自身链接
	ASSERT(  itTopWnd->pWndTop != itTopWnd->pWndBottom);
	//itWnd->pWnd->BringWindowToTop();
	itWnd->pWnd->SetActiveWindow();

	//// 
	//itWnd->pWnd->SetParent(itTopWnd->pWnd);
	////

	STICK_WND * pSWnd = itWnd->pWndBottom;
	size_t nC = 0;
	while(pSWnd)
	{
		pSWnd->nDirect = itTopWnd->nDirect;
		pSWnd->nShowState = itTopWnd->nShowState;
		//pSWnd->pWnd->BringWindowToTop();
		pSWnd->pWnd->SetActiveWindow();
		nC++;
		if(nC > m_lstWnds.size())
		{
			ASSERT(0);
			pSWnd->pWndBottom = NULL;
			break;
		}
		pSWnd = pSWnd->pWndBottom;
	}
}


bool CStickMainDlg::MainAttachLeftTest(LPRECT pRct)
{
	if(IsVisible() == FALSE)
		return FALSE;

	StickDlgIt it,itEnd;
	it = m_lstWnds.begin();
	itEnd = m_lstWnds.end();
	CRect rctTop;
	CRect rctOld;
	CRect rctMain;
	int nY = 0;
	int nX = 0;

	for(;it != itEnd;it++)
	{
		if(it->pWndTop)
		{
			// 子窗体上部有其他子窗体的不做判断
			continue;
		}
		if(it->IsAttach())
		{
			// 已经粘贴在主窗体的不做判断
			continue;
		}
		if(it->IsHide())
		{
			// 隐藏窗体不做处理
			continue;
		}
		it->GetRect(rctTop);
		rctOld = rctTop;
		// 主窗体与子窗体之间垂直间隔距离
		nY = pRct->top - rctTop.top;
		// 主窗体左边与子窗体之间水平间隔距离
		nX = pRct->left - rctTop.right;
		// 判断是否在该窗体磁场范围内
		
		if(abs(nX) < HORI_MAGNETISM)
		{// 水平距离	
			int nHeight = pRct->bottom - pRct->top;
			int nWidth = pRct->right - pRct->left;
			if(abs(nY) < VERT_MAIN_MAGN)
			{// 垂直距离
				GetWindowRect(&rctMain);
				it->nDirect = STICK_WND::LEFT;
				AttachWnd(it,&rctTop);
				nX = (rctTop.left - rctOld.left) + (pRct->left - rctMain.left);
				nY = (rctTop.top - rctOld.top) + (pRct->top - rctMain.top);
				MoveWnd(&(*it),nY,nX);
				return true;
			}
		}
	}
	return false;
}

bool CStickMainDlg::MainAttachRightTest(LPRECT pRct)
{
	if(IsVisible() == FALSE)
		return FALSE;
	StickDlgIt it,itEnd;
	it = m_lstWnds.begin();
	itEnd = m_lstWnds.end();
	CRect rctTop;
	CRect rctOld;
	CRect rctMain;
	int nY = 0;
	int nX = 0;

	for(;it != itEnd;it++)
	{
		if(it->pWndTop)
		{
			// 子窗体上部有其他子窗体的不做判断
			continue;
		}
		if(it->IsAttach())
		{
			// 已经粘贴在主窗体的不做判断
			continue;
		}
		if(it->IsHide())
		{
			// 隐藏窗体不做处理
			continue;
		}
		it->GetRect(rctTop);
		rctOld = rctTop;
		// 主窗体与子窗体之间垂直间隔距离
		nY = pRct->top - rctTop.top;
		// 主窗体右边与子窗体之间水平间隔距离
		nX = pRct->right - rctTop.left;
		// 判断是否在该窗体磁场范围内

		if(abs(nX) < HORI_MAGNETISM)
		{// 水平距离	
			int nHeight = pRct->bottom - pRct->top;
			int nWidth = pRct->right - pRct->left;
			if(abs(nY) < VERT_MAIN_MAGN)
			{// 垂直距离
				GetWindowRect(&rctMain);
				//MoveWnd(&(*it),nY,nX);
				it->nDirect = STICK_WND::RIGHT;
				AttachWnd(it,&rctTop);
				nX = (rctTop.left - rctOld.left) + (pRct->left - rctMain.left);
				nY = (rctTop.top - rctOld.top) + (pRct->top - rctMain.top);
				MoveWnd(&(*it),nY,nX);
				return true;
			}
		}
	}
	return false;
}

bool CStickMainDlg::SubAttachMainTest(StickDlgIt itWnd, 
									  LPRECT pRct, 
									  CPoint pt, 
									  CPoint & ptDown )
{
	/*
					 ______
     ___________    | sub1 |   
	|			|   |______|	
	|			|     /|\        
	|	main	|   ___|____  
	|           |/__| sub2 |  
	|___________|\	|______|
	
	*/
	int nCenterX ;
	nCenterX = (pRct->left + pRct->right) / 2;
	CRect rctMain;
	GetWindowRect(&rctMain);
	int nX;

	//
	// 先判断是否在左边停靠
	//

	// 子窗体与主窗体左边的水平间隔距离
	nX = rctMain.left - pRct->right;
	if(pRct->bottom > rctMain.top && pRct->top < rctMain.bottom)
	{
		if( abs(nX) < HORI_MAGNETISM )
		{// 在主窗体水平磁场内
			if(abs(pRct->top - rctMain.top) < VERT_MAIN_MAGN)
			{// 水平距离和垂直距离都达到磁力范围才吸附
				itWnd->nDirect = STICK_WND::LEFT;
				AttachWnd(itWnd,pRct);
				ptDown = pt;
				return true;
			}
			// 如果没有粘住主窗体，但是离主窗体很近，则让其靠在主窗体旁边
			int nX1 = pt.x - ptDown.x;
			if(abs(nX1) < HORI_MAGNETISM)
			{
				pRct->left += nX;
				pRct->right += nX;	
				return true;
			}	
			else
			{
				nX1 = (nX1 > 0) ? HORI_MAGNETISM : -HORI_MAGNETISM;
				pRct->left += nX1;
				pRct->right += nX1; 
				// 更新这次分离时鼠标的位置
				ptDown = pt;
			}
		}
	}

	//
	// 判断是否在右边停靠
	//

	// 子窗体与主窗体右边的水平间隔距离
	nX = pRct->left - rctMain.right;
	if(pRct->bottom > rctMain.top && pRct->top < rctMain.bottom)
	{
		if( abs(nX) < HORI_MAGNETISM )
		{// 在主窗体水平磁场内
			if(abs(pRct->top - rctMain.top) < VERT_MAIN_MAGN)
			{// 水平距离和垂直距离都达到磁力范围才吸附
				itWnd->nDirect = STICK_WND::RIGHT;
				AttachWnd(itWnd,pRct);
				ptDown = pt;
				return true;
			}
			// 如果没有粘住主窗体，但是离主窗体很近，则让其靠在主窗体旁边
			int nX1 = pt.x - ptDown.x;
			if(abs(nX1) < HORI_MAGNETISM)
			{
				pRct->left -= nX;
				pRct->right -= nX;	
				return true;
			}	
			else
			{
				nX1 = (nX1 > 0) ? HORI_MAGNETISM : -HORI_MAGNETISM;
				pRct->left += nX1;
				pRct->right += nX1; 
				// 更新这次分离时鼠标的位置
				ptDown = pt;
			}
		}
	}
	return false;
}


void CStickMainDlg::OnMoveSubWnd(CStickDlg * pSub)
{
	if(m_bDragFullWindows || (pSub->m_bMoveStart == FALSE))
		return;
	ASSERT(pSub);
	if(pSub == NULL)
		return;

}

// 处理子窗体移动事件
void CStickMainDlg::OnMovingSubWnd(CStickDlg * pSub,LPRECT pRct,CPoint& ptDown)
{
	ASSERT(pSub);
	if(pSub == NULL)
		return;
	StickDlgIt itWnd = FindWnd(pSub);
	StickDlgIt itLastWnd;
	// 如果被移动的窗体不存在，则返回
	if(itWnd == m_lstWnds.end())
		return ;

	CRect rct;
	pSub->GetWindowRect(&rct);
	CPoint pt;
	GetCursorPos(&pt);

	// 如果当前已经吸附,则判断是否从主窗体分离
	if(itWnd->IsAttach())
	{
		STICK_WND * pTopWnd = NULL;
		pTopWnd = IsTopSub(&(*itWnd));
		if(pTopWnd)
		{// 如果是主窗体旁的顶层子窗体才判断是否从主窗体分离
			SubDetachMainTest(itWnd,pRct,pt,ptDown);
		}
		else
		{// 否则判断是否从子窗体分离
			SubDetachSubTest(itWnd,pRct,pt,ptDown);
		}
	}
	// 如果当前已经分离,则判断是吸附其他子窗体还是从其他子窗体分离
	else
	{
		if(itWnd->pWndTop)
		{
			// 如果当前子窗体顶部有子窗体则判断分离
			SubDetachSubTest(itWnd,pRct,pt,ptDown);

		}
		else
		{
			// 先判断是否吸附到其他子窗体，再判断是否吸附到主窗体
			int type = SubAttachSubTest(itWnd,pRct,pt,ptDown);
			if( type == ATTACH_NONE)
			{
				SubAttachMainTest(itWnd,pRct,pt,ptDown);
			}
			else if(type == ATTACH_TOP)
			{
				return;
			}
		}	
	
	}
	
	// 该子窗体下边的附属子窗体也随之移动
	MoveWnd( &(*itWnd),pRct->top - rct.top,pRct->left - rct.left);
}

void CStickMainDlg::OnSizeSubWnd(CStickDlg * pSub)
{

	if(m_bDragFullWindows || (pSub->m_bSizeStart == FALSE))
		return;

	ASSERT(pSub);
	if(pSub == NULL)
		return;
	StickDlgIt itWnd = FindWnd(pSub);
	// 如果被移动的窗体不存在，则返回
	if(itWnd == m_lstWnds.end())
		return ;

	CRect rctOther;
	CRect rctNew;
	CRect rct = pSub->GetRectBeforeSize();
	CWnd* pWndOther = NULL;
	pSub->GetWindowRect(&rctNew);
	LPRECT pRct = &rctNew;
	if(itWnd->IsDetach())
	{
		// 如果已经剥离，则不影响主窗体的大小 
		SizingSubWnd(&(*itWnd),pRct);
		itWnd->pWnd->MoveWindow(pRct);
		return;
	}

	//
	// * 对主窗体的影响 *
	//
	// 暂时考虑左右方向的子窗体
	// 如果被改变大小的窗体宽度左右扩大缩小时，主窗体也需要向左右移动
	//

	int nDeltaX = 0;
	int nDeltaY = 0;
	CRect rctMain;
	GetWindowRect(&rctMain);


	if(itWnd->nDirect == STICK_WND::RIGHT)
	{
		nDeltaX = pRct->left - rct.left ;
	}
	else if(itWnd->nDirect == STICK_WND::LEFT)
	{
		nDeltaX = pRct->right - rct.right;
	}
	rctMain.left += nDeltaX;
	rctMain.right += nDeltaX;
	if(!(nDeltaX == 0 && nDeltaY == 0))
	{
		// 移动主窗体
		MoveWindow(&rctMain);
	}


	//
	//	* 对其他子窗体的影响 *
	//
	// [暂考虑左右方向的子窗体]改变窗体宽度时,其他吸附窗体和它
	// 同步改变宽度,使之一致大小。在改变窗体高度时就需要注意,
	// 需要保证子窗体顶部高度和主窗体固定等高，而其他窗体根据
	// 该子窗体高度的变化来上浮或下移。
	// 

	// 相反方向
	int nOppsiDirect = itWnd->OppsiDirect();
	StickDlgIt itFirst = FindFirstAttachWnd(nOppsiDirect);
	if(itFirst != m_lstWnds.end())
	{
		// 相反方向的子窗体保持宽不变，和主窗体一块移动
		MoveWnd(&(*itFirst),0,nDeltaX);
	}

	// 改变本窗体大小
	SizingSubWnd(&(*itWnd),pRct);

}

void CStickMainDlg::OnSizingSubWnd(CStickDlg * pSub,LPRECT pRct)
{

	if(m_bDragFullWindows == FALSE)
		return;
	
	ASSERT(pSub);
	if(pSub == NULL)
		return;
	StickDlgIt itWnd = FindWnd(pSub);
	// 如果被移动的窗体不存在，则返回
	if(itWnd == m_lstWnds.end())
		return ;

	CRect rct,rctOther;
	CWnd* pWndOther = NULL;
	pSub->GetWindowRect(&rct);

	if(itWnd->IsDetach())
	{
		if(m_bDragFullWindows)
		{
			// 如果已经剥离，则不影响主窗体的大小 
			SizingSubWnd(&(*itWnd),pRct);
		}
		return;
	}

	

	//
	// * 对主窗体的影响 *
	//
	// 暂时考虑左右方向的子窗体
	// 如果被改变大小的窗体宽度左右扩大缩小时，主窗体也需要向左右移动
	//
	int nDeltaX = 0;
	if(itWnd->nDirect == STICK_WND::RIGHT)
	{
		nDeltaX = pRct->left - rct.left ;
	}
	else if(itWnd->nDirect == STICK_WND::LEFT)
	{
		nDeltaX = pRct->right - rct.right;
	}
	if(m_bDragFullWindows)
	{
		int nDeltaY = 0;
		CRect rctMain;
		GetWindowRect(rctMain);
		rctMain.left += nDeltaX;
		rctMain.right += nDeltaX;
		if(!(nDeltaX == 0 && nDeltaY == 0))
		{
			// 移动主窗体
			MoveWindow(&rctMain);
		}
	}
	

	//
	//	* 对其他子窗体的影响 *
	//
	// [暂考虑左右方向的子窗体]改变窗体宽度时,其他吸附窗体和它
	// 同步改变宽度,使之一致大小。在改变窗体高度时就需要注意,
	// 需要保证子窗体顶部高度和主窗体固定等高，而其他窗体根据
	// 该子窗体高度的变化来上浮或下移。
	// 

	// 相反方向
	int nOppsiDirect = itWnd->OppsiDirect();
	StickDlgIt itFirst = FindFirstAttachWnd(nOppsiDirect);
	if(itFirst != m_lstWnds.end())
	{
		if(m_bDragFullWindows)
		{
			// 相反方向的子窗体保持宽不变，和主窗体一块移动
			MoveWnd(&(*itFirst),0,nDeltaX);
		}
		
	}
	
	// 改变本窗体大小
	SizingSubWnd(&(*itWnd),pRct);
	
	
}


void CStickMainDlg::SizingSubWnd(STICK_WND * pSWnd,LPRECT pRct)
{
	ASSERT(pSWnd);
	CRect rctOld,rctOther;
	if(m_bDragFullWindows)
		pSWnd->GetRect(rctOld);
	else
		rctOld = pSWnd->pWnd->GetRectBeforeSize();
	// 新高度
	int nNewHeight = pRct->bottom - pRct->top;
	// 高度变化增量=新高度-旧高度
	int nDeltaH = nNewHeight - rctOld.Height();
	// 其他吸附窗体宽度随之改变
	STICK_WND * pUp = pSWnd->pWndTop;
	STICK_WND * pDown = pSWnd->pWndBottom;

	// 恢复顶部高度
	pRct->top = rctOld.top;
	// 底部为新顶部+新高度
	pRct->bottom = pRct->top + nNewHeight;

	// 在(被改变大小的窗体)之上的窗体不做上下移动
	// 仅等同宽度
	size_t nC = 0;
	while(pUp)
	{
		pUp->GetRect(rctOther);
		// 保持宽度一致
		rctOther.left = pRct->left;
		rctOther.right = pRct->right;
		(*pUp)->MoveWindow(rctOther);
		pUp = pUp->pWndTop;
		nC++;
		if(nC > m_lstWnds.size())
		{
			ASSERT(0);
			break;
		}

	}
	nC = 0;
	while(pDown)
	{
		//  在(被改变大小的窗体)之下的窗体需做上下浮动
		pDown->GetRect(rctOther);
		// 保持宽度一致
		rctOther.left = pRct->left;
		rctOther.right = pRct->right;
		// 上下浮动
		rctOther.top += nDeltaH;
		rctOther.bottom += nDeltaH;
		(*pDown)->MoveWindow(rctOther);
		pDown = pDown->pWndBottom;
		nC++;
		if(nC > m_lstWnds.size())
		{
			ASSERT(0);
			break;
		}
	}
}

StickDlgIt CStickMainDlg::FindWnd(CStickDlg * pWnd)
{
	StickDlgIt it,itEnd;
	itEnd = m_lstWnds.end();
	for(it = m_lstWnds.begin();it != itEnd; it++)
	{
		if(*it == pWnd)
			return it;
	}
	return it;
}



STICK_WND * CStickMainDlg::FindLastAttachWnd(int nDirect)
{
	STICK_WND * pFind = NULL;
	STICK_WND * pTop = NULL;
	switch (nDirect)
	{
	case STICK_WND::LEFT:
		pTop = m_pLeftTopSub;
		break;
	case STICK_WND::RIGHT:
		pTop = m_pRightTopSub;
		break;
	}
	if(pTop == NULL)
		return NULL;
	pFind = pTop;
	size_t nC = 0;
	while(pFind->pWndBottom)
	{
		pFind = pFind->pWndBottom;
		nC++;
		if(nC > m_lstWnds.size())
		{
			ASSERT(0);
			break;
		}
	}
	return pFind;
	/*StickDlgIt it,itEnd,itFind;
	it = m_lstWnds.begin();
	itEnd = m_lstWnds.end();
	itFind = itEnd;
	for(;it != itEnd; it++)
	{
		if( it->IsAttach() &&
			it->nDirect == nDirect &&
			it->pWndBottom == NULL
		  )
		{
			itFind = it;
		}
		
	}*/
	//return itFind;
}

StickDlgIt CStickMainDlg::FindFirstAttachWnd(int nDirect)
{
	StickDlgIt it,itEnd,itFind;
	it = m_lstWnds.begin();
	itEnd = m_lstWnds.end();
	itFind = itEnd;
	for(;it != itEnd; it++)
	{
		if( it->IsAttach() &&
			it->nDirect == nDirect &&
			it->pWndTop == NULL
			)
		{
			itFind = it;
		}

	}
	return itFind;
}

bool CStickMainDlg::AttachWnd(StickDlgIt itWnd,LPRECT pRect)
{
	ASSERT(pRect);
	// 如果已经为吸附状态则返回
	if(itWnd->IsAttach())
		return false;
	StickDlgIt it,itEnd;
	// 主窗体旁边最后一个吸附窗体
	STICK_WND * pLastWnd;
	
	CRect rct,rctMain;
	CWnd * pWnd = NULL;
	STICK_WND wnd;
	int nTop = 0;
	int nLeft = 0;
	int nWidth = 0;
	int nHeight = 0;
	//
	// 如果要吸附窗体,则在相同位置最后一个可视子窗体下吸附
	//
	pLastWnd = FindLastAttachWnd(itWnd->nDirect);

	// 当前没有可视的子窗体，显示在主窗体旁边
	if(pLastWnd == NULL)
	{
		GetWindowRect(&rctMain);
		pWnd = (*itWnd);
		pWnd->GetWindowRect(&rct);
		// 要求主窗体可见
		if(IsVisible())
		{
			GetWindowRect(&rctMain);
			switch(itWnd->nDirect)
			{
			case STICK_WND::RIGHT:	// 停靠主窗体右
				nTop = rctMain.top;
				nLeft = rctMain.right;
				break;
			case STICK_WND::LEFT:	// 停靠主窗体左
				nTop = rctMain.top;
				nLeft = rctMain.left - rct.Width();
				break;
			//case STICK_WND::UP:		// 停靠主窗体上
			//	nLeft = rctMain.left;
			//	nTop = rctMain.top - rct.Height();
			//	break;
			//case STICK_WND::DOWN:	// 停靠主窗体下
			//	nLeft = rctMain.left;
			//	nTop = rctMain.bottom;
			//	break;
			};
			
			// 宽高不变
			nWidth = rct.Width();
			nHeight = rct.Height();

			// 返回输出值
			pRect->left = nLeft;
			pRect->top = nTop;
			pRect->right = nLeft + nWidth;
			pRect->bottom = nTop + nHeight;

		}
		// 如果主窗体隐藏则按窗体原来大小显示
		else
		{
			// 返回输出值
			*pRect = rct;
		}
		// 将窗体记录移到最前
		itWnd->nShowState = STICK_WND::ATTACH;
		// 上方无窗体
		itWnd->pWndTop = NULL;
		
		switch (itWnd->nDirect)
		{
		case STICK_WND::LEFT:
			m_pLeftTopSub = &(*itWnd);
			break;
		case STICK_WND::RIGHT:
			m_pRightTopSub = &(*itWnd);
			break;
		};
	}
	else
	{
		pWnd = *itWnd;
		// 最后一个可视窗体的范围
		pLastWnd->GetRect(rct);
		nTop = rct.bottom;
		nLeft = rct.left;
		// 宽度保持一致
		nWidth = rct.Width();		
		// 高度保持不变
		pWnd->GetWindowRect(&rct);
		nHeight = rct.Height();
		// 返回输出值
		pRect->left = nLeft;
		pRect->top = nTop;
		pRect->right = nLeft + nWidth;
		pRect->bottom = nTop + nHeight;
		
		itWnd->nShowState = STICK_WND::ATTACH;
		// 更新链接
		itWnd->pWndTop = pLastWnd;
		pLastWnd->pWndBottom = &(*itWnd);
		// 不能自己和自己相连
		ASSERT(pLastWnd != pLastWnd->pWndBottom);
	}

	// 
	//itWnd->pWnd->SetParent(this);
	//

	// 更改其下其他子窗体的位置和状态
	STICK_WND * pDown = NULL;
	pDown = itWnd->pWndBottom;
	size_t nC = 0;
	while(pDown)
	{
		pDown->nDirect = itWnd->nDirect;
		pDown->nShowState = STICK_WND::ATTACH;
		pDown->pWnd->BringWindowToTop();
		nC++;
		if(nC > m_lstWnds.size())
		{
			ASSERT(0);
			pDown->pWndBottom = NULL;
			break;
		}
		pDown = pDown->pWndBottom;
	}
	return true;
}

//void CStickMainDlg::MinimizeWnd(StickDlgIt itWnd)
//{
//	// 如果已经为隐藏状态则返回
//	if(itWnd->IsAttach() == false)
//		return;
//	StickDlgIt it,itEnd;
//	itEnd = m_lstWnd.end();
//	CWnd * pWnd = *itWnd;
//	//pWnd->ShowWindow(SW_MINIMIZE);
//}

void CStickMainDlg::CloseWnd(StickDlgIt itWnd)
{
	if(itWnd->IsAttach())
	{
		int nDelta = 0;
		CRect rctHide;
		itWnd->GetRect(rctHide);
		nDelta = rctHide.Height();
		// 将当前可视窗体下的可视窗体向上移动
		if(itWnd->pWndBottom)
		{
			MoveWnd(itWnd->pWndBottom,-nDelta,0);
		}
	}

	if(IsTopSub(&(*itWnd)))
	{
		STICK_WND * pNewTop = itWnd->pWndBottom;
		switch(itWnd->nDirect)
		{
		case STICK_WND::RIGHT:
			m_pRightTopSub = pNewTop;
			break;
		case STICK_WND::LEFT:
			m_pLeftTopSub = pNewTop;
			break;
		default:
			ASSERT(0);
			break;
		}
	}
	// 更新子链接
	if(itWnd->pWndTop)
	{
		itWnd->pWndTop->pWndBottom = itWnd->pWndBottom;
	}
	if(itWnd->pWndBottom)
	{
		itWnd->pWndBottom->pWndTop = itWnd->pWndTop;
	}
	// 更新窗体列表
	m_lstWnds.erase(itWnd);
}

void CStickMainDlg::MainDetach()
{

	if(m_pLeftTopSub)
	{
		DetachWnd(m_pLeftTopSub);
	}
	if(m_pRightTopSub)
	{
		DetachWnd(m_pRightTopSub);
	}
}

void CStickMainDlg::DetachWnd(STICK_WND * pSWnd)
{
	ASSERT(pSWnd);
	if(pSWnd == NULL)
		return;
	if(pSWnd->IsAttach() == false)
	{
		return;	
	}

	STICK_WND * pNextWnd;
	CRect rctHide;
	pSWnd->GetRect(rctHide);
	pSWnd->nShowState = STICK_WND::DETACH;
	
	if(pSWnd->pWndTop)
	{
		pSWnd->pWndTop->pWndBottom = NULL;
		pSWnd->pWndTop = NULL;
	}
	size_t nC = 0;
	// 该窗体下的其他子窗体也要被剥离
	pNextWnd = pSWnd->pWndBottom;
	while(pNextWnd)
	{
		pNextWnd->nShowState = STICK_WND::DETACH;
		pNextWnd = pNextWnd->pWndBottom;
		if(nC > m_lstWnds.size())
		{
			ASSERT(0);
			break;
		}
		nC++;
	}
	// 如果是主窗体旁边的顶部子窗体,则更新
	DelTopSub(pSWnd);
}

void CStickMainDlg::HideWnd(StickDlgIt itWnd)
{
	// 如果已经为隐藏状态则返回
	if(itWnd->nShowState == STICK_WND::HIDE)
		return;
	STICK_WND * pNextWnd;
	int nDelta = 0;
	CRect rctHide;
	itWnd->GetRect(rctHide);
	nDelta = rctHide.Height();
	itWnd->nShowState = STICK_WND::HIDE;
	
	// 将当前可视窗体下的可视窗体向上移动
	pNextWnd = itWnd->pWndBottom;
	if(pNextWnd)
	{
		MoveWnd(pNextWnd,-nDelta,0);
	}
	if(IsTopSub(&(*itWnd)))
	{
		STICK_WND * pNewTop = itWnd->pWndBottom;
		switch(itWnd->nDirect)
		{
		case STICK_WND::RIGHT:
			m_pRightTopSub = pNewTop;
			break;
		case STICK_WND::LEFT:
			m_pLeftTopSub = pNewTop;
			break;
		}
	}

	// 更新链接
	if(itWnd->pWndTop)
	{
		itWnd->pWndTop->pWndBottom = itWnd->pWndBottom;
	}
	if(itWnd->pWndBottom)
	{
		itWnd->pWndBottom->pWndTop = itWnd->pWndTop;
	}
	itWnd->pWndTop = NULL;
	itWnd->pWndBottom = NULL;
}

void CStickMainDlg::MoveWnd(STICK_WND *pSWnd,int nDeltaVert,int nDeltaHori)
{
	CRect rct;
	int nW ;
	if(pSWnd)
	{
		pSWnd->GetRect(rct);
		nW = rct.Width();
	}
	size_t nC = 0;
	while(pSWnd)
	{
		pSWnd->GetRect(rct);
		// 向上为负
		rct.OffsetRect(nDeltaHori,nDeltaVert);
		// 相同宽度
		rct.right = rct.left + nW;
		//TRACE("MoveWnd:%d,%d\n",nDeltaHori,nDeltaVert);
		pSWnd->pWnd->MoveWindow(&rct);
		if(pSWnd->pWndBottom == pSWnd)
		{
			ASSERT(0);
			pSWnd->pWndBottom = NULL;
			break;
		}
		pSWnd = pSWnd->pWndBottom;
		nC++;
		if(nC > m_lstWnds.size())
		{
			ASSERT(0);
			break;
		}
	}
}





BEGIN_MESSAGE_MAP(CStickMainDlg, _BaseStickMainDlg)
	ON_WM_MOVE()
	ON_WM_MOVING()
	ON_WM_SIZE()
	ON_WM_SIZING()
//	ON_WM_TIMER()
ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CStickMainDlg 消息处理程序

void CStickMainDlg::OnMove(int x, int y)
{
	if(m_bDragFullWindows == FALSE && IsVisible() && m_bMoveStart)
	{
		CRect rct;
		GetWindowRect(&rct);
		int nDeltaX;
		int nDeltaY;
		nDeltaY = rct.top - m_rctLastMove.top;
		nDeltaX = rct.left - m_rctLastMove.left;
		if(m_pRightTopSub && (nDeltaY != 0 || nDeltaX != 0))
		{
			// 移动右边的子窗体
			MoveWnd(m_pRightTopSub,
				nDeltaY,
				nDeltaX
				);
		}
		else
		{
			// 测试右边是否粘上子窗体
			MainAttachRightTest(&rct);
		}

		if(m_pLeftTopSub && (nDeltaY != 0 || nDeltaX != 0))
		{
			// 移动左边的子窗体
			MoveWnd(m_pLeftTopSub,
				nDeltaY,
				nDeltaX
				);

		}
		else
		{
			// 测试左边是否粘上子窗体
			MainAttachLeftTest(&rct);

		}
		// 开始新的移动
		m_bMoveStart = FALSE;
	}
	_BaseStickMainDlg::OnMove(x, y);
}

void CStickMainDlg::OnMoving(UINT fwSide, LPRECT pRect)
{
	if(IsVisible()== FALSE)
		return;

	CRect rct;
	GetWindowRect(&rct);
	int nDeltaX;
	int nDeltaY;
	nDeltaY = pRect->top - rct.top;
	nDeltaX = pRect->left - rct.left;
	
	if(m_pRightTopSub && (nDeltaY != 0 || nDeltaX != 0))
	{
		if(m_bDragFullWindows)
		{
			// 移动右边的子窗体
			MoveWnd(m_pRightTopSub,
				nDeltaY,
				nDeltaX
			);
		}	
	}
	else
	{
		if(m_bDragFullWindows)
		{
			// 测试右边是否粘上子窗体
			MainAttachRightTest(pRect);
		}
	}
	if(m_pLeftTopSub && (nDeltaY != 0 || nDeltaX != 0))
	{
		if(m_bDragFullWindows)
		{
			// 移动左边的子窗体
			MoveWnd(m_pLeftTopSub,
				nDeltaY,
				nDeltaX
				);
		}
	}
	else
	{
		if(m_bDragFullWindows)
		{
			// 测试左边是否粘上子窗体
			MainAttachLeftTest(pRect);
		}
	}
	
	
	if(m_bMoveStart == FALSE)
	{
		// 刚开始移动
		m_bMoveStart = TRUE;
		// 记录此时的窗体大小
		m_rctLastMove = rct;
	}
	_BaseStickMainDlg::OnMoving(fwSide, pRect);
}

void CStickMainDlg::OnSize(UINT nType, int cx, int cy)
{
	if(m_bDragFullWindows == FALSE && m_bSizeStart && IsVisible())
	{
		int nDeltaRight;
		int nDeltaTop;
		int nDeltaLeft;
		CRect rct;
		GetWindowRect(&rct);
		// 计算主窗体向右变化距离
		nDeltaRight = rct.right - m_rctLastSize.right;
		// 计算主窗体向上变化距离
		nDeltaTop = rct.top - m_rctLastSize.top;
		StickDlgIt itLeft,itRight;
		itRight = FindFirstAttachWnd(STICK_WND::RIGHT);
		if(itRight != m_lstWnds.end())
		{
			MoveWnd(&(*itRight),nDeltaTop,nDeltaRight);
		}
		// 计算主窗体向左变化距离
		nDeltaLeft = rct.left - m_rctLastSize.left;
		itLeft = FindFirstAttachWnd(STICK_WND::LEFT);
		if(itLeft != m_lstWnds.end())
		{
			MoveWnd(&(*itLeft),nDeltaTop,nDeltaLeft);

		}
		// 开始新的Sizing
		m_bSizeStart = FALSE;
	}
	_BaseStickMainDlg::OnSize(nType, cx, cy);
	
}

void CStickMainDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	if(IsVisible() == FALSE)
		return;
	int nDeltaRight;
	int nDeltaTop;
	int nDeltaLeft;
	CRect rct;
	GetWindowRect(&rct);
	// 计算主窗体向右变化距离
	nDeltaRight = pRect->right - rct.right;
	// 计算主窗体向上变化距离
	nDeltaTop = pRect->top - rct.top;
	StickDlgIt itLeft,itRight;

	if(nDeltaTop != 0 || nDeltaRight != 0)
	{
		if(m_bDragFullWindows)
		{
			itRight = FindFirstAttachWnd(STICK_WND::RIGHT);
			if(itRight != m_lstWnds.end())
			{
				MoveWnd(&(*itRight),nDeltaTop,nDeltaRight);
			}
		}
	}

	// 计算主窗体向左变化距离
	nDeltaLeft = pRect->left - rct.left;

	if(nDeltaTop != 0 || nDeltaLeft != 0)
	{
		if(m_bDragFullWindows)
		{
			itLeft = FindFirstAttachWnd(STICK_WND::LEFT);
			if(itLeft != m_lstWnds.end())
			{
				MoveWnd(&(*itLeft),nDeltaTop,nDeltaLeft);
			}
		}
	}
	if(m_bSizeStart == FALSE)
	{
		m_rctLastSize = rct;
		m_bSizeStart = TRUE;
	}
	
	_BaseStickMainDlg::OnSizing(fwSide, pRect);
}





void CStickMainDlg::OnActiveSub(CStickDlg * pSub)
{
	ASSERT(pSub);
	StickDlgIt itWnd = FindWnd(pSub);
	// 如果被移动的窗体不存在，则返回
	if(itWnd == m_lstWnds.end())
		return ;
	OnActiveSub(&(*itWnd));
	
}

void CStickMainDlg::OnActiveSub(STICK_WND * pStickWnd)
{
	ASSERT(pStickWnd);
	if ( pStickWnd==NULL )
		return ;

	STICK_WND * pSWnd = pStickWnd->pWndBottom;
	size_t nC = 0;
	while(pSWnd)
	{
		pSWnd->pWnd->BringWindowToTop();
		//pSWnd->pWnd->SetActiveWindow();
		nC++;
		if(nC > m_lstWnds.size())
		{
			ASSERT(0);
			pSWnd->pWndBottom = NULL;
			break;
		}
		pSWnd = pSWnd->pWndBottom;

	}
	pStickWnd->pWnd->BringWindowToTop();
}
void CStickMainDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CPoint pt;
	CRect rct;
	GetCursorPos(&pt);
	GetWindowRect(&rct);
	if(nState == WA_CLICKACTIVE)
	{
	/*	if(pt.y - rct.top < TITLE_BAR_CY)
		{
			if(m_pLeftTopSub)
				OnActiveSub(m_pLeftTopSub);
			if(m_pRightTopSub)
				OnActiveSub(m_pRightTopSub);
		}
		BringWindowToTop();*/
	}
	else if(nState == WA_ACTIVE )
	{
		//static int i = 0;
		//i++;
		//TRACE("main active%d\n",i);

		//if(bMinimized && IsVisible())
		//{
			

			/*if(IsVisible())
			{
				if ( CFriendPictureEntryDlg::GetInstance() )
					CFriendPictureEntryDlg::GetInstance()->BringWindowToTop();

				if ( CFriendLogEntryDlg::GetInstance() )
					CFriendLogEntryDlg::GetInstance()->BringWindowToTop();
			}
			else
			{
				if(m_pLeftTopSub)
					OnActiveSub(m_pLeftTopSub);
				if(m_pRightTopSub)
					OnActiveSub(m_pRightTopSub);
			}*/
			

		//	BringWindowToTop();
		//}
		
	}
	_BaseStickMainDlg::OnActivate(nState, pWndOther, bMinimized);
	
}
