// ������������� Microsoft Visual C++ ������ IDispatch ��װ��

// ע��: ��Ҫ�޸Ĵ��ļ������ݡ����������
//  Microsoft Visual C++ �������ɣ������޸Ľ�����д��


#include "stdafx.h"
#include "explorer1.h"
#include ".\explorer1.h"

/////////////////////////////////////////////////////////////////////////////
// CExplorer1

IMPLEMENT_DYNCREATE(CExplorer1, CWnd)

BEGIN_MESSAGE_MAP(CExplorer1, CWnd)
	ON_WM_HELPINFO()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CExplorer1::OnHelpInfo(HELPINFO* lpHelpInfo)
{
	if ( m_strHelpUrl!="" )
		ShellExecute(NULL,"open",m_strHelpUrl,"","",SW_SHOW);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CExplorer1 ����

/////////////////////////////////////////////////////////////////////////////
// CExplorer1 ����

void CExplorer1::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd::OnRButtonDown(nFlags, point);
}

LRESULT CExplorer1::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	return CWnd::WindowProc(message, wParam, lParam);
}

BOOL CExplorer1::PreTranslateMessage(MSG* pMsg)
{
	if ( m_bEnableRButton==false && (pMsg->message==WM_RBUTTONDOWN || pMsg->message==WM_RBUTTONUP) )
	{
		return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}
