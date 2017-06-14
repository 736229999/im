#pragma once

#include "..\..\Mini_Client\Mini139_res\resource.h"
//×ÊÔ´DLLº¯Êý
bool		InitResourceDLL(CString strDLLFile);
void		UnInitResourceDLL();

HINSTANCE   GetResDLLHandle();

HBITMAP		ResLoadBitmap(UINT nIdResource);
HICON       ResLoadIcon(UINT nIdResource);
HCURSOR     ResLoadCursor(UINT nIdResource);

CString     ResLoadHtmlResource(UINT nIdResource);

class CResBitmap : public CBitmap
{
public:
	CResBitmap(void);
	virtual ~CResBitmap(void);

public:
	BOOL LoadBitmap(UINT nIDResource);
	BOOL Attach(HGDIOBJ hObject);
	HGDIOBJ Detach();
	//operator CBitmap *() const;
	//operator = (CBitmap *p);

private:
    HGDIOBJ   m_hResObj;
};
