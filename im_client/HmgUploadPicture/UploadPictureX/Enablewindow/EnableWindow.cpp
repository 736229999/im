// EnableWindow.cpp: implementation of the _EnableWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "..\..\stdafx.h"
#include "EnableWindow.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

_EnableWindow::_EnableWindow(HWND hWnd)
{
	m_hWnd = NULL;
	if(hWnd != NULL && ::IsWindow(hWnd))
	{
		m_hWnd = hWnd;
		::EnableWindow(m_hWnd, FALSE);	
	}
}

_EnableWindow::_EnableWindow(CWnd *pWnd)
{
	m_hWnd = NULL;

	if(pWnd != NULL && pWnd ->m_hWnd != NULL && ::IsWindow(pWnd->m_hWnd) )
	{
		m_hWnd = pWnd ->m_hWnd;
		::EnableWindow(m_hWnd, FALSE);
	}
}

_EnableWindow::_EnableWindow(const UINT nCtrlID, CWnd *pParentWnd)
{
	m_hWnd = NULL;
	if(pParentWnd != NULL &&  pParentWnd ->m_hWnd != NULL && ::IsWindow(pParentWnd->m_hWnd))
	{
		m_hWnd = ::GetDlgItem(pParentWnd ->m_hWnd, nCtrlID);
		::EnableWindow(m_hWnd, FALSE);
	}
}

_EnableWindow::~_EnableWindow()
{
	if(m_hWnd != NULL)
	{
		::EnableWindow(m_hWnd, TRUE);
		::InvalidateRect(m_hWnd, NULL, TRUE);
	}

	m_hWnd = NULL;
}
