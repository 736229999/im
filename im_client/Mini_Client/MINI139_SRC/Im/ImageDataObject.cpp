//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ImageDataObject.h"
#include <Richole.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Static member functions
//////////////////////////////////////////////////////////////////////

void CImageDataObject::InsertBitmap(IRichEditOle* pRichEditOle, HBITMAP hBitmap, DWORD dwData)
{
	SCODE sc;

	CImageDataObject *pods = new CImageDataObject;
	LPDATAOBJECT lpDataObject;
	pods->QueryInterface(IID_IDataObject, (void **)&lpDataObject);
	pods->SetBitmap(hBitmap);
	
	IOleClientSite *pOleClientSite;	
	pRichEditOle->GetClientSite(&pOleClientSite);

	IStorage *pStorage;	

	LPLOCKBYTES lpLockBytes = NULL;
	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)
		AfxThrowOleException(sc);
	ASSERT(lpLockBytes != NULL);
	// 创建一个IStorage文件对象
	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &pStorage);
	if (sc != S_OK)
	{
		VERIFY(lpLockBytes->Release() == 0);
		lpLockBytes = NULL;
		AfxThrowOleException(sc);
	}
	ASSERT(pStorage != NULL);

	IOleObject *pOleObject; 

	pOleObject = pods->GetOleObject(pOleClientSite, pStorage);//

	if(pOleObject == NULL)
		return ;

	REOBJECT reobject;
	ZeroMemory(&reobject, sizeof(REOBJECT));
	reobject.cbStruct = sizeof(REOBJECT);
	
	CLSID clsid;
	sc = pOleObject->GetUserClassID(&clsid);
	if (sc != S_OK)
		AfxThrowOleException(sc);

	reobject.clsid = clsid;
	reobject.cp = REO_CP_SELECTION;
	reobject.dvaspect = DVASPECT_CONTENT;
	reobject.dwFlags	= REO_BELOWBASELINE| REO_INPLACEACTIVE ;
	reobject.poleobj  = pOleObject;//(LPOLEOBJECT)pPic;//
	reobject.polesite = pOleClientSite;
	reobject.pstg   = pStorage;
	reobject.dwUser = dwData;


	pRichEditOle->InsertObject(&reobject);

	pOleObject->Release();
	pOleClientSite->Release();
	pStorage->Release();
	lpDataObject->Release();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

void CImageDataObject::SetBitmap(HBITMAP hBitmap)
{
	ASSERT(hBitmap);

	STGMEDIUM stgm;
	stgm.tymed = TYMED_GDI;					// Storage medium = HBITMAP handle		
	stgm.hBitmap = hBitmap;
	stgm.pUnkForRelease = NULL;				// Use ReleaseStgMedium

	FORMATETC fm;
	fm.cfFormat = CF_BITMAP;				// Clipboard format = CF_BITMAP
	fm.ptd = NULL;							// Target Device = Screen
	fm.dwAspect = DVASPECT_CONTENT;			// Level of detail = Full content
	fm.lindex = -1;							// Index = Not applicaple
	fm.tymed = TYMED_GDI;					// Storage medium = HBITMAP handle

	SetData(&fm, &stgm, TRUE);		
}

IOleObject *CImageDataObject::GetOleObject(IOleClientSite *pOleClientSite, IStorage *pStorage)
{
	ASSERT(m_stgmed.hBitmap);

	SCODE sc;
	IOleObject *pOleObject = NULL;
	sc = ::OleCreateStaticFromData(this, IID_IOleObject, OLERENDER_FORMAT, 
			&m_fromat, pOleClientSite, pStorage, (void **)&pOleObject);
	if (sc != S_OK)
	{
		TRACE("get ole object error!\n");
		//AfxThrowOleException(sc);
	}
	return pOleObject;
}