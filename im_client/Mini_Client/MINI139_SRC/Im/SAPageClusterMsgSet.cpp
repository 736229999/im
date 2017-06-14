// im\SAPageClusterMsgSet.cpp : 实现文件
//

#include "stdafx.h"
#include "SAPageClusterMsgSet.h"


// CSAPageClusterMsgSet 对话框

IMPLEMENT_DYNAMIC(CSAPageClusterMsgSet, CSABasePage)
CSAPageClusterMsgSet::CSAPageClusterMsgSet(CWnd* pParent /*=NULL*/)
	: CSABasePage(CSAPageClusterMsgSet::IDD, pParent)
{
	Create(CSAPageClusterMsgSet::IDD,pParent);
	m_typeRecvMsg = RECV_AND_SHOW;
}

CSAPageClusterMsgSet::~CSAPageClusterMsgSet()
{
}

void CSAPageClusterMsgSet::DoDataExchange(CDataExchange* pDX)
{
	CSABasePage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSAPageClusterMsgSet, CSABasePage)
END_MESSAGE_MAP()


// CSAPageClusterMsgSet 消息处理程序
void CSAPageClusterMsgSet::SaveConfig()
{
	//CString sCfg;
	//sCfg.Format("%sdata\\%s\\Config.ini",(LPCTSTR)g_LocalData.GetPath(),g_ImUserInfo.GetUserName().c_str());
	//CString s;
	//// 是否
	//s.Format("%d",m_typeRecvMsg);
	//bRt=::WritePrivateProfileString(
	//	"ClusterMsgSet", 
	//	"RecvMsgType",
	//	s, 
	//	sCfg);
}

void CSAPageClusterMsgSet::LoadConfig()
{

}


