// TitleEdit.cpp : 实现文件
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



// CTitleEdit 消息处理程序


void CTitleEdit::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString str;
	GetWindowText(str);
	if(str=="在此输入标题")
	SetWindowText("");
	CFlatEdit::OnLButtonDown(nFlags, point);
}

LRESULT CTitleEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message==WM_LBUTTONDOWN||message==WM_MBUTTONDOWN||message==WM_RBUTTONDOWN)
	{
		GetParent()->PostMessage(WM_LBUTTONDOWN,wParam,lParam);
	}

	return CFlatEdit::WindowProc(message, wParam, lParam);
}
