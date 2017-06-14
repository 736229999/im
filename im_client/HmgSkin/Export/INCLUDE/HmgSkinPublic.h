#ifndef _HMG_SKIN_PUBLIC_H_
#define _HMG_SKIN_PUBLIC_H_

#include <vector>
using namespace std;

#include <gdiplus.h>
using namespace Gdiplus;

#include <afxtempl.h>
#include <afxcmn.h>
#include <afxdtctl.h>

#ifdef _HMGSKIN_EXPORT
	#define DLL_DECLSPEC  __declspec(dllexport)
#else
	#define DLL_DECLSPEC  __declspec(dllimport)
#endif

#ifdef __cplusplus
  extern "C" {
#endif
		//输出全局变量及类＼结构
#ifndef IM_SPACE_COLOR
#define IM_SPACE_COLOR			RGB(0x4d,0x95,0xc2)
#define IM_LINE_COLOR			RGB(255,255,255)
#endif

#ifndef TVS_NOHSCROLL
#define TVS_NOHSCROLL 0x8000
#endif

typedef struct
{
	NMHDR	hdr;
	DWORD   dwDrawStage;
	HDC		hdc;
	RECT	rect;
	UINT	uItem;
	UINT	uState;
	UINT	nBar;

} NMCSBCUSTOMDRAW;

typedef struct
{
	NMHDR	hdr;
	RECT	rect;
	POINT	pt;
	UINT	uCmdId;
	UINT	uState;
	int		nBar;

} NMCOOLBUTMSG;

#define DECLARE_HMG_SCROLLBAR()\
	protected:\
	void OnHMGSBCustomDraw(UINT id,  NMHDR * pHdr, LRESULT* result );\

#define IMPLEMENT_HMG_SCROLLBAR(theClass)\
	void theClass::OnHMGSBCustomDraw(UINT id,  NMHDR * pHdr, LRESULT* result )\
	{\
	UINT idCtrl = pHdr->code;\
	if (pHdr->code == NM_COOLSB_CUSTOMDRAW)\
	*result = HandleCustomDraw(idCtrl, (NMCSBCUSTOMDRAW *)pHdr);\
	}\

#define SET_HMG_SCROLLBAR(nID)  ON_NOTIFY_RANGE(NM_COOLSB_CUSTOMDRAW,nID,nID,OnHMGSBCustomDraw)

DLL_DECLSPEC bool InitHmgSkinControls(CString strResourceDLL);
DLL_DECLSPEC void UnInitHmgSkinControls();
DLL_DECLSPEC HINSTANCE GetHMGResourceDLLHandle();
DLL_DECLSPEC HBITMAP   HMG_ResLoadBitmap(UINT nIdResource);
DLL_DECLSPEC HICON     HMG_ResLoadIcon(UINT nIdResource);
DLL_DECLSPEC HCURSOR   HMG_ResLoadCursor(UINT nIdResource);

DLL_DECLSPEC void AttachHMGScrollbar(HWND hWnd);
DLL_DECLSPEC LRESULT HandleCustomDraw(UINT, NMCSBCUSTOMDRAW *);

#define NM_COOLSB_CUSTOMDRAW (0-0xfffU)

#ifdef __cplusplus
		}
#endif

#endif