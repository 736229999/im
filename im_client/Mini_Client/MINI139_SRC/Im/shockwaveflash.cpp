// ������������� Microsoft Visual C++ ������ IDispatch ��װ��

// ע��: ��Ҫ�޸Ĵ��ļ������ݡ����������
//  Microsoft Visual C++ �������ɣ������޸Ľ�����д��


#include "stdafx.h"
#include "shockwaveflash.h"
#include ".\shockwaveflash.h"

/////////////////////////////////////////////////////////////////////////////
// CShockwaveflash

IMPLEMENT_DYNCREATE(CShockwaveflash, CWnd)

/////////////////////////////////////////////////////////////////////////////
// CShockwaveflash ����

/////////////////////////////////////////////////////////////////////////////
// CShockwaveflash ����

BOOL CShockwaveflash::PreTranslateMessage(MSG* pMsg)
{
	if ( m_bEnableRightPopMenu==false )
	{
		if ( pMsg->message==WM_RBUTTONDOWN || pMsg->message==WM_RBUTTONUP )
			return TRUE;
	}

	return CWnd::PreTranslateMessage(pMsg);
}
