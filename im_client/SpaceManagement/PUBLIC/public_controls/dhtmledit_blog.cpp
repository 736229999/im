// ������������� Microsoft Visual C++ ������ IDispatch ��װ��

// ע��: ��Ҫ�޸Ĵ��ļ������ݡ����������
//  Microsoft Visual C++ �������ɣ������޸Ľ�����д��


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
// CDHtmlEditCtrl ����

/////////////////////////////////////////////////////////////////////////////
// CDHtmlEditCtrl ����

void CDHtmlEditCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}
