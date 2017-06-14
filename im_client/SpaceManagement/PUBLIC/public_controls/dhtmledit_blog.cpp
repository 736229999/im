// 计算机生成了由 Microsoft Visual C++ 创建的 IDispatch 包装类

// 注意: 不要修改此文件的内容。如果此类由
//  Microsoft Visual C++ 重新生成，您的修改将被改写。


#include "stdafx.h"
#include "dhtmledit_blog.h"
#include ".\dhtmledit_blog.h"

/////////////////////////////////////////////////////////////////////////////
// CDHtmlEditCtrl

IMPLEMENT_DYNCREATE(CDHtmlEditCtrl, CWnd)


BEGIN_MESSAGE_MAP(CDHtmlEditCtrl, CWnd)
	ON_WM_HELPINFO()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

BOOL CDHtmlEditCtrl::OnHelpInfo(HELPINFO* lpHelpInfo)
{
	ShellExecute(NULL,"open","http://iecim.feiniu.com/help/publish-article.html","","",SW_SHOW);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CDHtmlEditCtrl 属性

/////////////////////////////////////////////////////////////////////////////
// CDHtmlEditCtrl 操作

void CDHtmlEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
