#include "stdafx.h"
#include "tone.h"
#include <MMSystem.h>

// 提示音
void PlayTone(EM_VOICE_TYPE vType, BOOL IsLoop /*= FALSE*/)
{
	if(g_LocalData.VoiceIsEnable(vType))
	{
		string strPath = g_LocalData.GetVoicePath(vType);
		DWORD dwPlayModel = 0;
		if(IsLoop)
		{
			dwPlayModel = SND_FILENAME | SND_ASYNC | SND_LOOP;
		}
		else
		{
			dwPlayModel = SND_FILENAME | SND_ASYNC;
		}
		PlaySound(strPath.c_str(),NULL,dwPlayModel);
	}
}

// 关闭提示音
void StopTone()
{
	PlaySound(NULL,NULL,SND_FILENAME | SND_ASYNC | SND_LOOP);
}	