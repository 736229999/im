// im\FloorPage.cpp : 实现文件
//

#include "stdafx.h"
#include "FloorPage.h"
#include "MsgObj.h"
#include "EmotionDlg.h"
#include "LocalData.h"

// CFloorPage 对话框

IMPLEMENT_DYNAMIC(CFloorPage, CDialog)
CFloorPage::CFloorPage(UINT nIDTemplate,CWnd* pParent /*=NULL*/)
	: CDialog(nIDTemplate, pParent)
{
}
CFloorPage::~CFloorPage()
{
}

void CFloorPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CFloorPage, CDialog)
END_MESSAGE_MAP()



bool CFloorPage::TranslateMsg(const CString & sMsg,CString & sShowMsg,vector<DOWNLOAD_OBJ> & arrDownloadTask)
{
	arrDownloadTask.clear();
	CMsgObj msgObj;
	CString sMD5;
	CString sFile;
	vector<string> arrImg;
	vector<string> arrShow;
	msgObj.Load(sMsg);
	msgObj.GetWebImage(arrImg);
	// 先判断这些图片链接是否存在于自定义表情中
	size_t nC = arrImg.size();
	if(nC == 0)
		return false;
	arrShow.resize(nC);
	
	EMOTION_ID id;
	size_t i;
	for( i = 0;i < nC;i++)
	{
		// 查询本地表情库中是否有该链接的定义
		id = FindFaceUseUrl(arrImg[i].c_str());
		// 本地表情中没有该图片，需要下载
		if( id.nGroup == -1)
		{
			// 先获得该文件的md5名
			GetMD5WebFileName(arrImg[i].c_str(),sMD5);		
			// 判断该文件是否已经下载
			if(FileExist(g_LocalData.GetImageCanchePath()+"\\"+sMD5))
			{
				// 该文件已经下载,则直接使用它的文件名显示
				arrShow[i] = sMD5;
			}
			else
			{
				arrShow[i] = sMD5;
				// 下载任务
				DOWNLOAD_OBJ param;
				param.handle = NULL;
				param.sSaved = g_LocalData.GetImageCanchePath()+"\\"+sMD5;
				param.sUrl = arrImg[i].c_str();
				param.timer = 0;
				param.state = DS_DOWNLOADING;
				arrDownloadTask.push_back(param);
			}
		}
		// 本地有该文件缓存，不需要下载
		else
		{
			sFile = g_LocalData.GetImageCanchePath() +"\\"+ g_customEmotionDB[id.nGroup][id.nItem].szFile;
			if(!FileExist(sFile))
			{
				CopyFile(g_LocalData.GetCustomFacePath()+"\\" + g_customEmotionDB[id.nGroup][id.nItem].szFile,sFile,false);
			}
			arrShow[i] = g_customEmotionDB[id.nGroup][id.nItem].szFile;
		}
	}
	msgObj.ReplaceWebImage(arrShow);
	sShowMsg = msgObj.ToMsg().c_str();
	return true;
}

// CFloorPage 消息处理程序
