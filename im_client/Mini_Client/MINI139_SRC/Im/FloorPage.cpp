// im\FloorPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "FloorPage.h"
#include "MsgObj.h"
#include "EmotionDlg.h"
#include "LocalData.h"

// CFloorPage �Ի���

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
	// ���ж���ЩͼƬ�����Ƿ�������Զ��������
	size_t nC = arrImg.size();
	if(nC == 0)
		return false;
	arrShow.resize(nC);
	
	EMOTION_ID id;
	size_t i;
	for( i = 0;i < nC;i++)
	{
		// ��ѯ���ر�������Ƿ��и����ӵĶ���
		id = FindFaceUseUrl(arrImg[i].c_str());
		// ���ر�����û�и�ͼƬ����Ҫ����
		if( id.nGroup == -1)
		{
			// �Ȼ�ø��ļ���md5��
			GetMD5WebFileName(arrImg[i].c_str(),sMD5);		
			// �жϸ��ļ��Ƿ��Ѿ�����
			if(FileExist(g_LocalData.GetImageCanchePath()+"\\"+sMD5))
			{
				// ���ļ��Ѿ�����,��ֱ��ʹ�������ļ�����ʾ
				arrShow[i] = sMD5;
			}
			else
			{
				arrShow[i] = sMD5;
				// ��������
				DOWNLOAD_OBJ param;
				param.handle = NULL;
				param.sSaved = g_LocalData.GetImageCanchePath()+"\\"+sMD5;
				param.sUrl = arrImg[i].c_str();
				param.timer = 0;
				param.state = DS_DOWNLOADING;
				arrDownloadTask.push_back(param);
			}
		}
		// �����и��ļ����棬����Ҫ����
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

// CFloorPage ��Ϣ�������
