// TitleEdit.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "WriteArticle.h"
#include "TitleEdit.h"
#include ".\titleedit.h"


// CTitleEdit

IMPLEMENT_DYNAMIC(CTitleEdit, CFlatEdit)
CTitleEdit::CTitleEdit()
{
}

CTitleEdit::~CTitleEdit()
{
}


BEGIN_MESSAGE_MAP(CTitleEdit, CFlatEdit)
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()



// CTitleEdit ��Ϣ�������


void CTitleEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString str;
	GetWindowText(str);
	if(str=="�ڴ��������")
	SetWindowText("");
	CFlatEdit::OnLButtonDown(nFlags, point);
}

LRESULT CTitleEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (message==WM_LBUTTONDOWN||message==WM_MBUTTONDOWN||message==WM_RBUTTONDOWN)
	{
		GetParent()->PostMessage(WM_LBUTTONDOWN,wParam,lParam);
	}

	return CFlatEdit::WindowProc(message, wParam, lParam);
}
