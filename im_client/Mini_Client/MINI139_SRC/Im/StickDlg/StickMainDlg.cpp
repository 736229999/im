// StickMainDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "StickMainDlg.h"
#include ".\stickmaindlg.h"



#define HORI_MAGNETISM	10	// ˮƽ����
#define VERT_MAGNETISM	10	// ��ֱ������ϲ��Ӵ���Ĵ���
#define VERT_MAIN_MAGN	40	// ��ֱ�����������Ĵ���

#define TIMER_MOVEWINDOW	0x100

IMPLEMENT_DYNAMIC(CStickMainDlg, _BaseStickMainDlg)


BOOL CStickMainDlg::m_bDragFullWindows = TRUE;



INT GetDragFullWindows()
{
	// �����йص� hKEY, �ڲ�ѯ����ʱҪ�رա�
	HKEY hKEY; 
	LPCTSTR data_Set= "Control Panel\\Desktop";
	//����·�� data_Set ��ص� hKEY����һ������Ϊ�������ƣ��ڶ���������
	//��ʾҪ���ʵļ���λ�ã���������������Ϊ0��KEY_READ��ʾ�Բ�ѯ�ķ�ʽ��
	//����ע���hKEY�򱣴�˺������򿪵ļ��ľ����
	long ret0=(::RegOpenKeyEx(HKEY_CURRENT_USER,data_Set, 0, KEY_READ, &hKEY));
	if(ret0 != ERROR_SUCCESS)	//����޷���hKEY������ֹ�����ִ��
	{
		return -1;
	}
	// ��ѯ�йص����� (�û����� owner_Get)��
	BYTE	owner_Get[10];
	DWORD	type_1 = REG_SZ ; 
	DWORD	cbData_1 = 10; 
	//hKEYΪ�ղ�RegOpenKeyEx()�������򿪵ļ��ľ������DragFullWindows�塣
	//��ʾҪ�� ѯ�ļ�ֵ����type_1��ʾ��ѯ���ݵ����ͣ�owner_Get��������
	//��ѯ�����ݣ�cbData_1��ʾԤ���õ����ݳ��ȡ�
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
	// �ж���������
	GetWindowRect(&rctMain);
	nWidth = rctMain.Width();
	// ��Ļ�ϵ�ʣ��ƽ�����
	nWidth = (nScreenX - nWidth) / 2;
	
	// �����������滹�󣬾�Ĭ�Ϸ����м�
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
	// �������ʾ�Ĵ��岻���ڣ��򷵻�
	if(itWnd == m_lstWnds.end())
		return -1;
	STICK_WND * pSWnd = &(*itWnd);
	
	CRect rct;
	//
	// ���Ҫ��������,������ͬ�������һ�����Ӵ�������ʾ
	//
	if(nCmdShow == SSW_ATTACH)
	{	
		if(AttachWnd(itWnd,&rct))
			pSub->MoveWindow(&rct);
	}
	//
	// ���Ҫ��С�����壬��Ҫ����С���󽫸ô���֮�µ�������������ȥ
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
		// ����
		DetachWnd(&(*itWnd));
		// ��ֱ��������ʱ������Ҫ�ƶ�
		pRct->top += nY;
		pRct->bottom += nY; 
		// ������η���ʱ����λ��
		ptDown = pt;	 
		return true;
	}
	// ˮƽ��������������������Է�Χ�����
	int nX = pt.x - ptDown.x;
	CRect rct;
	itWnd->GetRect(rct);
	if(abs(nX) > HORI_MAGNETISM)
	{
		// ����
		DetachWnd(&(*itWnd));
		// ˮƽ��������ʱ������Ҫ�ƶ�
		pRct->left += nX;
		pRct->right += nX; 
		pRct->top += nY;
		pRct->bottom += nY;
		// ������η���ʱ����λ��
		ptDown = pt;	
		return true;
	}
	else
	{
		*pRct = rct;
	}
	return false;
}

bool CStickMainDlg::SubDetachSubTest(	StickDlgIt itWnd,	// ����
										LPRECT pRct,		// ��λ��
										 CPoint pt,			// ��ǰ���λ��
										CPoint& ptDown)		// ��갴��ʱ��λ��
{
	// �ϲ��Ӵ���
	STICK_WND * pUpSWnd;
	pUpSWnd = itWnd->pWndTop;
	// ����ڴ�ֱ�����϶��ľ���
	int nY = pt.y - ptDown.y;
	int nX = pt.x - ptDown.x;
	CRect rct;
	itWnd->GetRect(rct);

	if(pUpSWnd != NULL)
	{
		if(abs(nY) > VERT_MAGNETISM )
		{
			// ����
			pUpSWnd->pWndBottom = NULL;
			itWnd->pWndTop = NULL;
			itWnd->nShowState = STICK_WND::DETACH;
			// ��ֱ��������ʱ������Ҫ�ƶ�
			pRct->top += nY;
			pRct->bottom += nY; 
			pRct->left += nX;
			pRct->right += nX;
			// ������η���ʱ����λ��
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
	// �����ǰ�Ӵ����ϲ��Ѿ��������Ӵ��壬�򷵻�
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
			// �ڴ����б��м���Ӵ���Ĳ����ж�
			continue;
		}
		if(it->pWnd == itWnd->pWnd)
		{
			// �����Լ�
			continue;
		}
		if(it->IsHide())
		{
			// ���ش��岻������
			continue;
		}
		it->GetRect(rctLast);
		// �����봰���·�֮�䴹ֱ���
		nYB = pRct->top - rctLast.bottom;
		// �����봰���Ϸ�֮�䴹ֱ���
		nYT = rctLast.top - pRct->bottom ;
		// ˮƽ�е��Ƿ����Ӵ��崹ֱ��Χ��
		if(nCenterX > rctLast.left && nCenterX < rctLast.right) 
		{
			// �ж��Ƿ��ڸô���ų���Χ��

			// ���Ӵ����·���ֱ����
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
			// ���Ӵ����Ϸ���ֱ����
			else if( it->pWndTop == NULL && 
					 itWnd->pWndBottom == NULL &&
					 it->IsAttach() == false &&
					 abs(nYT) < VERT_MAGNETISM)
			{
				// �ô�������Ƿ�����״̬

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
	// ��ֹ��������
	ASSERT(  itWnd->pWndTop != itWnd->pWndBottom);
	itTopWnd->pWndBottom = &(*itWnd);
	// ��ֹ��������
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
			// �Ӵ����ϲ��������Ӵ���Ĳ����ж�
			continue;
		}
		if(it->IsAttach())
		{
			// �Ѿ�ճ����������Ĳ����ж�
			continue;
		}
		if(it->IsHide())
		{
			// ���ش��岻������
			continue;
		}
		it->GetRect(rctTop);
		rctOld = rctTop;
		// ���������Ӵ���֮�䴹ֱ�������
		nY = pRct->top - rctTop.top;
		// ������������Ӵ���֮��ˮƽ�������
		nX = pRct->left - rctTop.right;
		// �ж��Ƿ��ڸô���ų���Χ��
		
		if(abs(nX) < HORI_MAGNETISM)
		{// ˮƽ����	
			int nHeight = pRct->bottom - pRct->top;
			int nWidth = pRct->right - pRct->left;
			if(abs(nY) < VERT_MAIN_MAGN)
			{// ��ֱ����
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
			// �Ӵ����ϲ��������Ӵ���Ĳ����ж�
			continue;
		}
		if(it->IsAttach())
		{
			// �Ѿ�ճ����������Ĳ����ж�
			continue;
		}
		if(it->IsHide())
		{
			// ���ش��岻������
			continue;
		}
		it->GetRect(rctTop);
		rctOld = rctTop;
		// ���������Ӵ���֮�䴹ֱ�������
		nY = pRct->top - rctTop.top;
		// �������ұ����Ӵ���֮��ˮƽ�������
		nX = pRct->right - rctTop.left;
		// �ж��Ƿ��ڸô���ų���Χ��

		if(abs(nX) < HORI_MAGNETISM)
		{// ˮƽ����	
			int nHeight = pRct->bottom - pRct->top;
			int nWidth = pRct->right - pRct->left;
			if(abs(nY) < VERT_MAIN_MAGN)
			{// ��ֱ����
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
	// ���ж��Ƿ������ͣ��
	//

	// �Ӵ�������������ߵ�ˮƽ�������
	nX = rctMain.left - pRct->right;
	if(pRct->bottom > rctMain.top && pRct->top < rctMain.bottom)
	{
		if( abs(nX) < HORI_MAGNETISM )
		{// ��������ˮƽ�ų���
			if(abs(pRct->top - rctMain.top) < VERT_MAIN_MAGN)
			{// ˮƽ����ʹ�ֱ���붼�ﵽ������Χ������
				itWnd->nDirect = STICK_WND::LEFT;
				AttachWnd(itWnd,pRct);
				ptDown = pt;
				return true;
			}
			// ���û��ճס�����壬������������ܽ��������俿���������Ա�
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
				// ������η���ʱ����λ��
				ptDown = pt;
			}
		}
	}

	//
	// �ж��Ƿ����ұ�ͣ��
	//

	// �Ӵ������������ұߵ�ˮƽ�������
	nX = pRct->left - rctMain.right;
	if(pRct->bottom > rctMain.top && pRct->top < rctMain.bottom)
	{
		if( abs(nX) < HORI_MAGNETISM )
		{// ��������ˮƽ�ų���
			if(abs(pRct->top - rctMain.top) < VERT_MAIN_MAGN)
			{// ˮƽ����ʹ�ֱ���붼�ﵽ������Χ������
				itWnd->nDirect = STICK_WND::RIGHT;
				AttachWnd(itWnd,pRct);
				ptDown = pt;
				return true;
			}
			// ���û��ճס�����壬������������ܽ��������俿���������Ա�
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
				// ������η���ʱ����λ��
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

// �����Ӵ����ƶ��¼�
void CStickMainDlg::OnMovingSubWnd(CStickDlg * pSub,LPRECT pRct,CPoint& ptDown)
{
	ASSERT(pSub);
	if(pSub == NULL)
		return;
	StickDlgIt itWnd = FindWnd(pSub);
	StickDlgIt itLastWnd;
	// ������ƶ��Ĵ��岻���ڣ��򷵻�
	if(itWnd == m_lstWnds.end())
		return ;

	CRect rct;
	pSub->GetWindowRect(&rct);
	CPoint pt;
	GetCursorPos(&pt);

	// �����ǰ�Ѿ�����,���ж��Ƿ�����������
	if(itWnd->IsAttach())
	{
		STICK_WND * pTopWnd = NULL;
		pTopWnd = IsTopSub(&(*itWnd));
		if(pTopWnd)
		{// ������������ԵĶ����Ӵ�����ж��Ƿ�����������
			SubDetachMainTest(itWnd,pRct,pt,ptDown);
		}
		else
		{// �����ж��Ƿ���Ӵ������
			SubDetachSubTest(itWnd,pRct,pt,ptDown);
		}
	}
	// �����ǰ�Ѿ�����,���ж������������Ӵ��廹�Ǵ������Ӵ������
	else
	{
		if(itWnd->pWndTop)
		{
			// �����ǰ�Ӵ��嶥�����Ӵ������жϷ���
			SubDetachSubTest(itWnd,pRct,pt,ptDown);

		}
		else
		{
			// ���ж��Ƿ������������Ӵ��壬���ж��Ƿ�������������
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
	
	// ���Ӵ����±ߵĸ����Ӵ���Ҳ��֮�ƶ�
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
	// ������ƶ��Ĵ��岻���ڣ��򷵻�
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
		// ����Ѿ����룬��Ӱ��������Ĵ�С 
		SizingSubWnd(&(*itWnd),pRct);
		itWnd->pWnd->MoveWindow(pRct);
		return;
	}

	//
	// * ���������Ӱ�� *
	//
	// ��ʱ�������ҷ�����Ӵ���
	// ������ı��С�Ĵ���������������Сʱ��������Ҳ��Ҫ�������ƶ�
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
		// �ƶ�������
		MoveWindow(&rctMain);
	}


	//
	//	* �������Ӵ����Ӱ�� *
	//
	// [�ݿ������ҷ�����Ӵ���]�ı䴰����ʱ,���������������
	// ͬ���ı���,ʹ֮һ�´�С���ڸı䴰��߶�ʱ����Ҫע��,
	// ��Ҫ��֤�Ӵ��嶥���߶Ⱥ�������̶��ȸߣ��������������
	// ���Ӵ���߶ȵı仯���ϸ������ơ�
	// 

	// �෴����
	int nOppsiDirect = itWnd->OppsiDirect();
	StickDlgIt itFirst = FindFirstAttachWnd(nOppsiDirect);
	if(itFirst != m_lstWnds.end())
	{
		// �෴������Ӵ��屣�ֿ��䣬��������һ���ƶ�
		MoveWnd(&(*itFirst),0,nDeltaX);
	}

	// �ı䱾�����С
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
	// ������ƶ��Ĵ��岻���ڣ��򷵻�
	if(itWnd == m_lstWnds.end())
		return ;

	CRect rct,rctOther;
	CWnd* pWndOther = NULL;
	pSub->GetWindowRect(&rct);

	if(itWnd->IsDetach())
	{
		if(m_bDragFullWindows)
		{
			// ����Ѿ����룬��Ӱ��������Ĵ�С 
			SizingSubWnd(&(*itWnd),pRct);
		}
		return;
	}

	

	//
	// * ���������Ӱ�� *
	//
	// ��ʱ�������ҷ�����Ӵ���
	// ������ı��С�Ĵ���������������Сʱ��������Ҳ��Ҫ�������ƶ�
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
			// �ƶ�������
			MoveWindow(&rctMain);
		}
	}
	

	//
	//	* �������Ӵ����Ӱ�� *
	//
	// [�ݿ������ҷ�����Ӵ���]�ı䴰����ʱ,���������������
	// ͬ���ı���,ʹ֮һ�´�С���ڸı䴰��߶�ʱ����Ҫע��,
	// ��Ҫ��֤�Ӵ��嶥���߶Ⱥ�������̶��ȸߣ��������������
	// ���Ӵ���߶ȵı仯���ϸ������ơ�
	// 

	// �෴����
	int nOppsiDirect = itWnd->OppsiDirect();
	StickDlgIt itFirst = FindFirstAttachWnd(nOppsiDirect);
	if(itFirst != m_lstWnds.end())
	{
		if(m_bDragFullWindows)
		{
			// �෴������Ӵ��屣�ֿ��䣬��������һ���ƶ�
			MoveWnd(&(*itFirst),0,nDeltaX);
		}
		
	}
	
	// �ı䱾�����С
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
	// �¸߶�
	int nNewHeight = pRct->bottom - pRct->top;
	// �߶ȱ仯����=�¸߶�-�ɸ߶�
	int nDeltaH = nNewHeight - rctOld.Height();
	// ����������������֮�ı�
	STICK_WND * pUp = pSWnd->pWndTop;
	STICK_WND * pDown = pSWnd->pWndBottom;

	// �ָ������߶�
	pRct->top = rctOld.top;
	// �ײ�Ϊ�¶���+�¸߶�
	pRct->bottom = pRct->top + nNewHeight;

	// ��(���ı��С�Ĵ���)֮�ϵĴ��岻�������ƶ�
	// ����ͬ���
	size_t nC = 0;
	while(pUp)
	{
		pUp->GetRect(rctOther);
		// ���ֿ��һ��
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
		//  ��(���ı��С�Ĵ���)֮�µĴ����������¸���
		pDown->GetRect(rctOther);
		// ���ֿ��һ��
		rctOther.left = pRct->left;
		rctOther.right = pRct->right;
		// ���¸���
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
	// ����Ѿ�Ϊ����״̬�򷵻�
	if(itWnd->IsAttach())
		return false;
	StickDlgIt it,itEnd;
	// �������Ա����һ����������
	STICK_WND * pLastWnd;
	
	CRect rct,rctMain;
	CWnd * pWnd = NULL;
	STICK_WND wnd;
	int nTop = 0;
	int nLeft = 0;
	int nWidth = 0;
	int nHeight = 0;
	//
	// ���Ҫ��������,������ͬλ�����һ�������Ӵ���������
	//
	pLastWnd = FindLastAttachWnd(itWnd->nDirect);

	// ��ǰû�п��ӵ��Ӵ��壬��ʾ���������Ա�
	if(pLastWnd == NULL)
	{
		GetWindowRect(&rctMain);
		pWnd = (*itWnd);
		pWnd->GetWindowRect(&rct);
		// Ҫ��������ɼ�
		if(IsVisible())
		{
			GetWindowRect(&rctMain);
			switch(itWnd->nDirect)
			{
			case STICK_WND::RIGHT:	// ͣ����������
				nTop = rctMain.top;
				nLeft = rctMain.right;
				break;
			case STICK_WND::LEFT:	// ͣ����������
				nTop = rctMain.top;
				nLeft = rctMain.left - rct.Width();
				break;
			//case STICK_WND::UP:		// ͣ����������
			//	nLeft = rctMain.left;
			//	nTop = rctMain.top - rct.Height();
			//	break;
			//case STICK_WND::DOWN:	// ͣ����������
			//	nLeft = rctMain.left;
			//	nTop = rctMain.bottom;
			//	break;
			};
			
			// ��߲���
			nWidth = rct.Width();
			nHeight = rct.Height();

			// �������ֵ
			pRect->left = nLeft;
			pRect->top = nTop;
			pRect->right = nLeft + nWidth;
			pRect->bottom = nTop + nHeight;

		}
		// ��������������򰴴���ԭ����С��ʾ
		else
		{
			// �������ֵ
			*pRect = rct;
		}
		// �������¼�Ƶ���ǰ
		itWnd->nShowState = STICK_WND::ATTACH;
		// �Ϸ��޴���
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
		// ���һ�����Ӵ���ķ�Χ
		pLastWnd->GetRect(rct);
		nTop = rct.bottom;
		nLeft = rct.left;
		// ��ȱ���һ��
		nWidth = rct.Width();		
		// �߶ȱ��ֲ���
		pWnd->GetWindowRect(&rct);
		nHeight = rct.Height();
		// �������ֵ
		pRect->left = nLeft;
		pRect->top = nTop;
		pRect->right = nLeft + nWidth;
		pRect->bottom = nTop + nHeight;
		
		itWnd->nShowState = STICK_WND::ATTACH;
		// ��������
		itWnd->pWndTop = pLastWnd;
		pLastWnd->pWndBottom = &(*itWnd);
		// �����Լ����Լ�����
		ASSERT(pLastWnd != pLastWnd->pWndBottom);
	}

	// 
	//itWnd->pWnd->SetParent(this);
	//

	// �������������Ӵ����λ�ú�״̬
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
//	// ����Ѿ�Ϊ����״̬�򷵻�
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
		// ����ǰ���Ӵ����µĿ��Ӵ��������ƶ�
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
	// ����������
	if(itWnd->pWndTop)
	{
		itWnd->pWndTop->pWndBottom = itWnd->pWndBottom;
	}
	if(itWnd->pWndBottom)
	{
		itWnd->pWndBottom->pWndTop = itWnd->pWndTop;
	}
	// ���´����б�
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
	// �ô����µ������Ӵ���ҲҪ������
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
	// ������������ԱߵĶ����Ӵ���,�����
	DelTopSub(pSWnd);
}

void CStickMainDlg::HideWnd(StickDlgIt itWnd)
{
	// ����Ѿ�Ϊ����״̬�򷵻�
	if(itWnd->nShowState == STICK_WND::HIDE)
		return;
	STICK_WND * pNextWnd;
	int nDelta = 0;
	CRect rctHide;
	itWnd->GetRect(rctHide);
	nDelta = rctHide.Height();
	itWnd->nShowState = STICK_WND::HIDE;
	
	// ����ǰ���Ӵ����µĿ��Ӵ��������ƶ�
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

	// ��������
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
		// ����Ϊ��
		rct.OffsetRect(nDeltaHori,nDeltaVert);
		// ��ͬ���
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


// CStickMainDlg ��Ϣ�������

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
			// �ƶ��ұߵ��Ӵ���
			MoveWnd(m_pRightTopSub,
				nDeltaY,
				nDeltaX
				);
		}
		else
		{
			// �����ұ��Ƿ�ճ���Ӵ���
			MainAttachRightTest(&rct);
		}

		if(m_pLeftTopSub && (nDeltaY != 0 || nDeltaX != 0))
		{
			// �ƶ���ߵ��Ӵ���
			MoveWnd(m_pLeftTopSub,
				nDeltaY,
				nDeltaX
				);

		}
		else
		{
			// ��������Ƿ�ճ���Ӵ���
			MainAttachLeftTest(&rct);

		}
		// ��ʼ�µ��ƶ�
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
			// �ƶ��ұߵ��Ӵ���
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
			// �����ұ��Ƿ�ճ���Ӵ���
			MainAttachRightTest(pRect);
		}
	}
	if(m_pLeftTopSub && (nDeltaY != 0 || nDeltaX != 0))
	{
		if(m_bDragFullWindows)
		{
			// �ƶ���ߵ��Ӵ���
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
			// ��������Ƿ�ճ���Ӵ���
			MainAttachLeftTest(pRect);
		}
	}
	
	
	if(m_bMoveStart == FALSE)
	{
		// �տ�ʼ�ƶ�
		m_bMoveStart = TRUE;
		// ��¼��ʱ�Ĵ����С
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
		// �������������ұ仯����
		nDeltaRight = rct.right - m_rctLastSize.right;
		// �������������ϱ仯����
		nDeltaTop = rct.top - m_rctLastSize.top;
		StickDlgIt itLeft,itRight;
		itRight = FindFirstAttachWnd(STICK_WND::RIGHT);
		if(itRight != m_lstWnds.end())
		{
			MoveWnd(&(*itRight),nDeltaTop,nDeltaRight);
		}
		// ��������������仯����
		nDeltaLeft = rct.left - m_rctLastSize.left;
		itLeft = FindFirstAttachWnd(STICK_WND::LEFT);
		if(itLeft != m_lstWnds.end())
		{
			MoveWnd(&(*itLeft),nDeltaTop,nDeltaLeft);

		}
		// ��ʼ�µ�Sizing
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
	// �������������ұ仯����
	nDeltaRight = pRect->right - rct.right;
	// �������������ϱ仯����
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

	// ��������������仯����
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
	// ������ƶ��Ĵ��岻���ڣ��򷵻�
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
