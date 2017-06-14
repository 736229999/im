// ArtSetDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "WriteArticle.h"
#include "ArtSetDlg.h"


// CArtSetDlg 对话框

IMPLEMENT_DYNAMIC(CArtSetDlg, CDialog)
CArtSetDlg::CArtSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CArtSetDlg::IDD, pParent)
{
}

CArtSetDlg::~CArtSetDlg()
{
}

void CArtSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CArtSetDlg, CDialog)
END_MESSAGE_MAP()


// CArtSetDlg 消息处理程序
