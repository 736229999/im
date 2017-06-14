#pragma once

#include "stdafx.h"
#include "hmgskinpublic.h"
#include "coolscroll.h"
#include "detours.h"
#include "resource.h"

#ifndef TVS_NOHSCROLL
#define TVS_NOHSCROLL 0x8000
#endif

BOOL WINAPI HMGScrollbar_UninitializeApp(void);
BOOL WINAPI HMGScrollbar_InitializeApp(void);

static HDC			hdcSkin = NULL; //可能会存在GDI泄露，xuexiaofeng
static HBITMAP		hSkinBmp= NULL;
static HBITMAP		hOldSkinBmp = NULL;
typedef struct 
{
	int x, y;
	int width, height;

} CustomDrawTable;

//	Define a set of structures which describe
//	where-abouts the source "textures" are in the
//	custom draw bitmap. We need to know x, y, width and height
//	for each scrollbar segment.


CustomDrawTable cdt_horz_normal[] = 
{
	{ 55,  0, 10, 10 },	//left arrow  NORMAL
	{ 44,  0, 10, 10 }, //right arrow NORMAL
	{ 70,  0, 1,  10 }, //page left   NORMAL
	{ 70,  0, 1,  10 }, //page right  NORMAL
	
	{ -1, -1, -1, -1 },	//padding

	{ 44,  33,  3, 10 }, //horz thumb (left)
	{ 47,  33,  29, 10 }, //horz thumb (left)
	//{ 59,  32,  10, 15 }, //horz thumb (middle)
	//{ 69,  32,  9, 15 }, //horz thumb (middle)
	{ 76,  33,  3, 10 }, //horz thumb (right)
};

CustomDrawTable cdt_horz_hot[] = 
{
	{ 45,  11, 10, 10 },	//left arrow  NORMAL
	{ 55,  11, 10, 10 }, //right arrow NORMAL
	{ 70,  11, 1,  10 }, //page left   NORMAL
	{ 70,  11, 1,  10 }, //page right  NORMAL

	{ -1, -1, -1, -1 },	//padding

	{ 84,  32,  1, 15 }, //horz thumb (left)
	{ 85,  32,  9, 15 }, //horz thumb (left)
	{ 95,  32,  10, 15 }, //horz thumb (middle)
	{ 105,  32,  9, 15 }, //horz thumb (middle)
	{ 118,  32,  1, 15 }, //horz thumb (right)
};

CustomDrawTable cdt_horz_active[] = 
{
	{ 55,  22,  10, 10 },	//left arrow  NORMAL
	{ 44,  22, 10, 10 }, //right arrow NORMAL
	{ 70,  22, 1,  10 }, //page left   NORMAL
	{ 70,  22, 1,  10 }, //page right  NORMAL

	{ -1, -1, -1, -1 },	//padding

	{ 116,  33,  3, 10 }, //horz thumb (left)
	{ 119,  33,  29, 10 }, //horz thumb (left)
	//{ 95,  32,  10, 15 }, //horz thumb (middle)
	//{ 105,  32,  9, 15 }, //horz thumb (middle)
	{ 148,  33,  3, 10 }, //horz thumb (right)
};


CustomDrawTable cdt_vert_normal[] = 
{
	{ 0, 0,  10, 10 }, //up arrow   NORMAL
	{ 0, 11, 10, 10 }, //down arrow NORMAL
	{ 0, 22+3, 10, 1 }, //page up	 NORMAL
	{ 0, 22+3, 10, 1 }, //page down  NORMAL

	{ -1, -1, -1, -1 },	//padding

	{ 33, 0,  10, 3  },//{101, 1,  18, 1  }, //vert thumb (left)
	{ 33, 3,  10, 29  },//{101, 2,  18, 5  }, //vert thumb (left)
	//{ 39, 11,  10, 10  },//{ 101, 7,  18, 8  }, //vert thumb (middle)
	//{ 39, 21,  10, 10  },//{ 101, 15,  18, 5  }, //vert thumb (middle)
	{ 33, 32,  10, 3  },//{ 101, 20,  18, 1  }, //vert thumb (right)
};

CustomDrawTable cdt_vert_hot[] = 
{
	{ 16, 0,  15, 14 }, //up arrow   ACTIVE
	{ 16, 15, 14, 14 }, //down arrow ACTIVE
	{ 16,  30, 15, 1  }, //page up	 ACTIVE
	{ 16,  30, 15, 1  }, //page down  ACTIVE

	{ -1, -1, -1, -1 },	//padding

	{ 32, 21,  14, 9  }, //vert thumb (left)
	{ 32, 26,  14, 1  }, //vert thumb (middle)
	{ 32, 26,  14, 9  }, //vert thumb (right)
};

CustomDrawTable cdt_vert_active[] = 
{
	{ 22, 0,  10, 10 }, //up arrow   ACTIVE
	{ 22, 11, 10, 10 }, //down arrow ACTIVE
	{ 22, 22+3, 10, 1 }, //page up	 ACTIVE
	{ 22, 22+3, 10, 1 }, //page down  ACTIVE

	{ -1, -1, -1, -1 },	//padding

	{ 33, 36,  10, 3  },//{101, 1,  18, 1  }, //vert thumb (left)
	{ 33, 39,  10, 29  },//{101, 2,  18, 5  }, //vert thumb (left)
	//{ 39, 47,  10, 10  },//{ 101, 7,  18, 8  }, //vert thumb (middle)
	//{ 39, 57,  10, 10  },//{ 101, 15,  18, 5  }, //vert thumb (middle)
	{ 33, 68,  10, 3  },//{ 101, 20,  18, 1  }, //vert thumb (right)
};


void InitHMGScrollbar(void)
{
	hdcSkin  = CreateCompatibleDC(0);
	hSkinBmp = HMG_ResLoadBitmap(IDB_IM_SCROLLBAR);//::LoadBitmap(AfxFindResourceHandle(MAKEINTRESOURCE(IDB_IM_SCROLLBAR), RT_BITMAP), MAKEINTRESOURCE(IDB_IM_SCROLLBAR));//LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_IM_SCROLLBAR));
	hOldSkinBmp = (HBITMAP)SelectObject(hdcSkin, hSkinBmp);
	
	HMGScrollbar_InitializeApp();
}

void ReleaseHMGScrollbar()
{
	if( hdcSkin)
	{
		SelectObject(hdcSkin,hOldSkinBmp);
		DeleteDC(hdcSkin);
	}
	
	if( hSkinBmp )
	{
		DeleteObject(hSkinBmp);
	}

	HMGScrollbar_UninitializeApp();
}

void AttachHMGScrollbar(HWND hWnd)
{
	InitializeCoolSB(hWnd);
	
	CoolSB_SetStyle(hWnd, SB_VERT, CSBS_FLAT);

	CoolSB_SetSize(hWnd, SB_VERT, 10, 10);
	CoolSB_SetMinThumbSize(hWnd, SB_VERT, 10);


	CoolSB_SetStyle(hWnd, SB_HORZ, CSBS_FLAT);

	CoolSB_SetSize(hWnd, SB_HORZ, 10, 10);
	CoolSB_SetMinThumbSize(hWnd, SB_HORZ, 10);

	//long style  = GetWindowLong(hWnd ,GWL_STYLE);
	//style |= (TVS_NOHSCROLL);
	//SetWindowLong(hWnd ,GWL_STYLE,style);
}

LRESULT HandleCustomDraw(UINT ctrlid, NMCSBCUSTOMDRAW *nm)
{
	RECT *rc;
	CustomDrawTable *cdt;
	UINT code = NM_CUSTOMDRAW;

	UNREFERENCED_PARAMETER(ctrlid);

	if(nm->dwDrawStage == CDDS_PREPAINT)
	return CDRF_SKIPDEFAULT;
	
	//the sizing gripper in the bottom-right corner
	if(nm->nBar == SB_BOTH)	
	{
		RECT *rc = &nm->rect;
		
		StretchBlt(nm->hdc, rc->left, rc->top, rc->right-rc->left, rc->bottom-rc->top,
			hdcSkin, 100, 100, 18, 18, SRCCOPY);
				
		return CDRF_SKIPDEFAULT;
	}
	else if(nm->nBar == SB_HORZ)
	{
		rc = &nm->rect;

		if(nm->uState == CDIS_HOT)	
			cdt = &cdt_horz_hot[nm->uItem];
		else if(nm->uState == CDIS_SELECTED) 
			cdt = &cdt_horz_active[nm->uItem];
		else				   
			cdt = &cdt_horz_normal[nm->uItem];
		
		if(nm->uItem == HTSCROLL_THUMB)
		{
			StretchBlt(nm->hdc, rc->left,   rc->top, 3, rc->bottom-rc->top, hdcSkin, cdt->x, cdt->y, cdt->width, cdt->height, SRCCOPY);
			cdt++;
			StretchBlt(nm->hdc, rc->left+3, rc->top, rc->right-rc->left-6, rc->bottom-rc->top, hdcSkin, cdt->x, cdt->y, cdt->width, cdt->height, SRCCOPY);
			/*cdt++;
			StretchBlt(nm->hdc, rc->left+(rc->right-rc->left-10)/2, rc->top, 10, rc->bottom-rc->top, hdcSkin, cdt->x, cdt->y, cdt->width, cdt->height, SRCCOPY);
			cdt++;
			StretchBlt(nm->hdc, rc->left+(rc->right-rc->left-10)/2+10, rc->top, (rc->right-rc->left-10)/2, rc->bottom-rc->top, hdcSkin, cdt->x, cdt->y, cdt->width, cdt->height, SRCCOPY);*/
			cdt++;
			StretchBlt(nm->hdc, rc->right-3,   rc->top, 3, rc->bottom-rc->top, hdcSkin, cdt->x, cdt->y, cdt->width, cdt->height, SRCCOPY);
			return CDRF_SKIPDEFAULT;
		}

	}
	else if(nm->nBar == SB_VERT)
	{		
		rc = &nm->rect;

		if(nm->uState == CDIS_HOT)		
			cdt = &cdt_vert_hot[nm->uItem];
		else if(nm->uState == CDIS_SELECTED)  
			cdt = &cdt_vert_active[nm->uItem];
		else				    
			cdt = &cdt_vert_normal[nm->uItem];

		if(nm->uItem == HTSCROLL_THUMB)
		{
			StretchBlt(nm->hdc, rc->left, rc->top,   rc->right-rc->left, 3, hdcSkin, cdt->x, cdt->y, cdt->width, cdt->height, SRCCOPY);
			cdt++;
			StretchBlt(nm->hdc, rc->left, rc->top+3,   rc->right-rc->left, (rc->bottom-rc->top-3), hdcSkin, cdt->x, cdt->y, cdt->width, cdt->height, SRCCOPY);
			//cdt++;
			//StretchBlt(nm->hdc, rc->left, rc->top+((rc->bottom-rc->top)-10)/2,   rc->right-rc->left, 10, hdcSkin, cdt->x, cdt->y, cdt->width, cdt->height, SRCCOPY);
			//cdt++;
			//StretchBlt(nm->hdc, rc->left, rc->top+((rc->bottom-rc->top)-10)/2+10, rc->right-rc->left, ((rc->bottom-rc->top)-10)/2, hdcSkin, cdt->x, cdt->y, cdt->width, cdt->height, SRCCOPY);
			cdt++;			
			StretchBlt(nm->hdc, rc->left, rc->bottom-3, rc->right-rc->left, 3,hdcSkin, cdt->x, cdt->y, cdt->width, cdt->height, SRCCOPY);
			return CDRF_SKIPDEFAULT;
		}
	}
	else
	{
		return CDRF_DODEFAULT;
	}

	//normal bitmaps, use same code for HORZ and VERT
	StretchBlt(nm->hdc, rc->left, rc->top, rc->right-rc->left, rc->bottom-rc->top,
		hdcSkin, cdt->x, cdt->y, cdt->width, cdt->height, SRCCOPY);

	return CDRF_SKIPDEFAULT;

}



//////////////////////////////////////////////////////
DETOUR_TRAMPOLINE(BOOL WINAPI Detour_EnableScrollBar(HWND hwnd, int wSBflags, UINT wArrows), EnableScrollBar);
DETOUR_TRAMPOLINE(BOOL WINAPI Detour_GetScrollInfo	(HWND hwnd, int fnBar, LPSCROLLINFO lpsi), GetScrollInfo);
DETOUR_TRAMPOLINE(int  WINAPI Detour_GetScrollPos	(HWND hwnd, int nBar), GetScrollPos);
DETOUR_TRAMPOLINE(BOOL WINAPI Detour_GetScrollRange	(HWND hwnd, int nBar, LPINT lpMinPos, LPINT lpMaxPos), GetScrollRange);
DETOUR_TRAMPOLINE(int  WINAPI Detour_SetScrollInfo	(HWND hwnd, int fnBar, LPSCROLLINFO lpsi, BOOL fRedraw), SetScrollInfo);
DETOUR_TRAMPOLINE(int  WINAPI Detour_SetScrollPos	(HWND hwnd, int nBar, int nPos, BOOL fRedraw), SetScrollPos);
DETOUR_TRAMPOLINE(int  WINAPI Detour_SetScrollRange	(HWND hwnd, int nBar, int nMinPos, int nMaxPos, BOOL fRedraw), SetScrollRange);
DETOUR_TRAMPOLINE(BOOL WINAPI Detour_ShowScrollBar	(HWND hwnd, int wBar, BOOL fShow), ShowScrollBar);

static BOOL WINAPI Tramp_EnableScrollBar(HWND hwnd, int wSBflags, UINT wArrows)
{
	if(CoolSB_IsCoolScrollEnabled(hwnd))	
		return CoolSB_EnableScrollBar(hwnd, wSBflags, wArrows);
	else
		return Detour_EnableScrollBar(hwnd, wSBflags, wArrows);
}

static BOOL WINAPI Tramp_GetScrollInfo(HWND hwnd, int fnBar, LPSCROLLINFO lpsi)
{
	if(CoolSB_IsCoolScrollEnabled(hwnd))	
		return CoolSB_GetScrollInfo(hwnd, fnBar, lpsi);
	else
		return Detour_GetScrollInfo(hwnd, fnBar, lpsi);
}

static int	 WINAPI Tramp_GetScrollPos(HWND hwnd, int nBar)
{
	if(CoolSB_IsCoolScrollEnabled(hwnd))	
		return CoolSB_GetScrollPos(hwnd, nBar);
	else
		return Detour_GetScrollPos(hwnd, nBar);
}

static BOOL WINAPI Tramp_GetScrollRange(HWND hwnd, int nBar, LPINT lpMinPos, LPINT lpMaxPos)
{
	if(CoolSB_IsCoolScrollEnabled(hwnd))	
		return CoolSB_GetScrollRange(hwnd, nBar, lpMinPos, lpMaxPos);
	else
		return Detour_GetScrollRange(hwnd, nBar, lpMinPos, lpMaxPos);
}

static int	 WINAPI Tramp_SetScrollInfo(HWND hwnd, int fnBar, LPSCROLLINFO lpsi, BOOL fRedraw)
{
	if(CoolSB_IsCoolScrollEnabled(hwnd))	
		return CoolSB_SetScrollInfo(hwnd, fnBar, lpsi, fRedraw);
	else
		return Detour_SetScrollInfo(hwnd, fnBar, lpsi, fRedraw);
}

static int  WINAPI Tramp_SetScrollPos(HWND hwnd, int nBar, int nPos, BOOL fRedraw)
{
	if(CoolSB_IsCoolScrollEnabled(hwnd))	
		return CoolSB_SetScrollPos(hwnd, nBar, nPos, fRedraw);
	else
		return Detour_SetScrollPos(hwnd, nBar, nPos, fRedraw);
}

static int  WINAPI Tramp_SetScrollRange(HWND hwnd, int nBar, int nMinPos, int nMaxPos, BOOL fRedraw)
{
	if(CoolSB_IsCoolScrollEnabled(hwnd))	
		return CoolSB_SetScrollRange(hwnd, nBar, nMinPos, nMaxPos, fRedraw);
	else
		return Detour_SetScrollRange(hwnd, nBar, nMinPos, nMaxPos, fRedraw);
}

static BOOL WINAPI Tramp_ShowScrollBar		(HWND hwnd, int wBar, BOOL fShow)
{
	if(CoolSB_IsCoolScrollEnabled(hwnd))	
		return CoolSB_ShowScrollBar(hwnd, wBar, fShow);
	else
		return Detour_ShowScrollBar(hwnd, wBar, fShow);
}

BOOL WINAPI HMGScrollbar_InitializeApp(void)
{
	DWORD dwVersion = GetVersion();

	// Only available under Windows NT, 2000 and XP
	if(dwVersion < 0x80000000)
	{
		DetourFunctionWithTrampoline((PBYTE)Detour_EnableScrollBar, (PBYTE)Tramp_EnableScrollBar);
		DetourFunctionWithTrampoline((PBYTE)Detour_GetScrollInfo,   (PBYTE)Tramp_GetScrollInfo);
		DetourFunctionWithTrampoline((PBYTE)Detour_GetScrollPos,    (PBYTE)Tramp_GetScrollPos);
		DetourFunctionWithTrampoline((PBYTE)Detour_GetScrollRange,  (PBYTE)Tramp_GetScrollRange);
		DetourFunctionWithTrampoline((PBYTE)Detour_SetScrollInfo,   (PBYTE)Tramp_SetScrollInfo);
		DetourFunctionWithTrampoline((PBYTE)Detour_SetScrollPos,    (PBYTE)Tramp_SetScrollPos);
		DetourFunctionWithTrampoline((PBYTE)Detour_SetScrollRange,  (PBYTE)Tramp_SetScrollRange);
		DetourFunctionWithTrampoline((PBYTE)Detour_ShowScrollBar,   (PBYTE)Tramp_ShowScrollBar);

		// don't actually use this feature within coolsb yet, but we might need it
		CoolSB_SetESBProc(Detour_EnableScrollBar);

		return TRUE;
	}
	else
	{
		return FALSE;
	}	
}

BOOL WINAPI HMGScrollbar_UninitializeApp(void)
{
	DWORD dwVersion = GetVersion();

	// Only available under Windows NT, 2000 and XP
	if(dwVersion < 0x80000000)
	{
		DetourRemove((PBYTE)Detour_EnableScrollBar, (PBYTE)Tramp_EnableScrollBar);
		DetourRemove((PBYTE)Detour_GetScrollInfo,   (PBYTE)Tramp_GetScrollInfo);
		DetourRemove((PBYTE)Detour_GetScrollPos,    (PBYTE)Tramp_GetScrollPos);
		DetourRemove((PBYTE)Detour_GetScrollRange,  (PBYTE)Tramp_GetScrollRange);
		DetourRemove((PBYTE)Detour_SetScrollInfo,   (PBYTE)Tramp_SetScrollInfo);
		DetourRemove((PBYTE)Detour_SetScrollPos,    (PBYTE)Tramp_SetScrollPos);
		DetourRemove((PBYTE)Detour_SetScrollRange,  (PBYTE)Tramp_SetScrollRange);
		DetourRemove((PBYTE)Detour_ShowScrollBar,   (PBYTE)Tramp_ShowScrollBar);

		// don't actually use this feature within coolsb yet, but we might need it
		CoolSB_SetESBProc(EnableScrollBar);

		return TRUE;
	}
	else
	{
		return FALSE;
	}
}
