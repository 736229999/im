#pragma once

#include <afxinet.h>

#ifdef _DEBUG
#import "..\..\binDebug\UserInfo.dll" named_guids no_namespace
#else
#import "..\..\binRelease\UserInfo.dll" named_guids no_namespace
#endif

void MiniOpenUrl(CString strUrl);
void ShowArticle(char *username);
void ShowAlbum(char *username);

CString WebDataFrom2Str(int iFrom);
CString WebViewProp2Str(int iViewProp);//查看权限
