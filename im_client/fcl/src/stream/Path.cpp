/******************************************************************
** 文件名:	Path.cpp
** 版  权:	(C)  
** 创建人:	T
** 日  期:	2007-5-15   
** 描  述:
**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
********************************************************************/

#include "stdafx.h"
#include ".\stream\Path.h"
#	include <windows.h>
#	include <direct.h>

BEGIN_NAMESPACE_FCL


char g_workDir[STR_LEN] = {0};



FCL_DECL const char* getWorkDir()
{
	if (g_workDir[0] != (char)0)
		return g_workDir;

	if (GetModuleFileName(NULL, g_workDir, STR_LEN - 1))
	{
		CPath filename(g_workDir);
		lstrcpyn(g_workDir, filename.getParentDir().c_str(), STR_LEN - 1);
	}


	return g_workDir;
}

FCL_DECL const char* resetWorkDir(const char* newDir)
{
	const char* oldDir = getWorkDir();
	if (!newDir || newDir[0] == '\0')
		return oldDir;

	strncpy(g_workDir, newDir, STR_LEN - 1);
	return g_workDir;
}

END_NAMESPACE
