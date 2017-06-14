#include "StdAfx.h"
#include "RichEditUtil.h"

#include "localdata.h"
#include "WebProcess.h"
#include "RichEditCtrlEx.h"
IRichEditOle* RichEdit_GetOleInterface(HWND hWnd)
{
	IRichEditOle *pRichEditOle = NULL;
	::SendMessage(hWnd, EM_GETOLEINTERFACE, 0, (LPARAM)&pRichEditOle);
	return pRichEditOle;
}

int RichEdit_GetWindowTextLength(HWND hWnd)
{
	return ::GetWindowTextLength(hWnd);
}

int RichEdit_GetWindowText(HWND hWnd, LPTSTR lpszStringBuf, int nMaxCount)
{
	return ::GetWindowText(hWnd, lpszStringBuf, nMaxCount);
}

int RichEdit_GetWindowText(HWND hWnd, tstring& strText)
{
	int nLength;
	TCHAR * pszText;

	nLength = RichEdit_GetWindowTextLength(hWnd);
	pszText = new TCHAR[nLength+1];
	if (NULL == pszText)
		return 0;
	memset(pszText, 0, (nLength+1)*sizeof(TCHAR));
	nLength = RichEdit_GetWindowText(hWnd, pszText, nLength+1);
	strText = pszText;
	delete []pszText;

	return nLength;
}

int RichEdit_GetTextRange(HWND hWnd, CHARRANGE * lpchrg, tstring& strText)
{
	strText = _T("");

	if (NULL == lpchrg || lpchrg->cpMax <= lpchrg->cpMin)
		return 0;

	LONG nLen = lpchrg->cpMax - lpchrg->cpMin;
	TCHAR * pText = new TCHAR[nLen+1];
	if (NULL == pText)
		return 0;

	memset(pText, 0, (nLen+1)*sizeof(TCHAR));

	TEXTRANGE tr = { 0 };
	tr.chrg = *lpchrg;
	tr.lpstrText = pText;
	int nRet = (int)::SendMessage(hWnd, EM_GETTEXTRANGE, 0, (LPARAM)&tr);

	strText = pText;
	delete []pText;

	return nRet;
}

DWORD RichEdit_GetDefaultCharFormat(HWND hWnd, CHARFORMAT& cf)
{
	cf.cbSize = sizeof(CHARFORMAT);
	return (DWORD)::SendMessage(hWnd, EM_GETCHARFORMAT, 0, (LPARAM)&cf);
}

BOOL RichEdit_SetDefaultCharFormat(HWND hWnd, CHARFORMAT& cf)
{
	cf.cbSize = sizeof(CHARFORMAT);
	return (BOOL)::SendMessage(hWnd, EM_SETCHARFORMAT, 0, (LPARAM)&cf);
}

DWORD RichEdit_GetSelectionCharFormat(HWND hWnd, CHARFORMAT& cf)
{
	cf.cbSize = sizeof(CHARFORMAT);
	return (DWORD)::SendMessage(hWnd, EM_GETCHARFORMAT, 1, (LPARAM)&cf);
}

BOOL RichEdit_SetSelectionCharFormat(HWND hWnd, CHARFORMAT& cf)
{
	cf.cbSize = sizeof(CHARFORMAT);
	return (BOOL)::SendMessage(hWnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf);
}

void RichEdit_ReplaceSel(HWND hWnd, LPCTSTR lpszNewText, BOOL bCanUndo/* = FALSE*/)
{
	::SendMessage(hWnd, EM_REPLACESEL, (WPARAM)bCanUndo, (LPARAM)lpszNewText);
}

void RichEdit_GetSel(HWND hWnd, LONG& nStartChar, LONG& nEndChar)
{
	CHARRANGE cr = { 0, 0 };
	::SendMessage(hWnd, EM_EXGETSEL, 0, (LPARAM)&cr);
	nStartChar = cr.cpMin;
	nEndChar = cr.cpMax;
}

int RichEdit_SetSel(HWND hWnd, LONG nStartChar, LONG nEndChar)
{
	CHARRANGE cr = { nStartChar, nEndChar };
	return (int)::SendMessage(hWnd, EM_EXSETSEL, 0, (LPARAM)&cr);
}

// 设置默认字体
void RichEdit_SetDefFont(HWND hWnd, LPCTSTR lpFontName, int nFontSize,
						 COLORREF clrText, BOOL bBold, BOOL bItalic, 
						 BOOL bUnderLine, BOOL bIsLink)
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(cf));
	RichEdit_GetDefaultCharFormat(hWnd, cf);

	if (lpFontName != NULL)					// 设置字体名称
	{
		cf.dwMask = cf.dwMask | CFM_FACE;
		_tcscpy(cf.szFaceName, lpFontName);
	}

	if (nFontSize > 0)						// 设置字体高度
	{
		cf.dwMask = cf.dwMask | CFM_SIZE;
		cf.yHeight = nFontSize * 20;
	}

	cf.dwMask |= CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_LINK;

	cf.crTextColor = clrText;				// 设置字体颜色
	cf.dwEffects &= ~CFE_AUTOCOLOR;

	if (bBold)								// 设置粗体
		cf.dwEffects |= CFE_BOLD;
	else
		cf.dwEffects &= ~CFE_BOLD;

	if (bItalic)							// 设置倾斜
		cf.dwEffects |= CFE_ITALIC;
	else
		cf.dwEffects &= ~CFE_ITALIC;

	if (bUnderLine)							// 设置下划线
		cf.dwEffects |= CFE_UNDERLINE;
	else
		cf.dwEffects &= ~CFE_UNDERLINE;

	if (bIsLink)							// 设置超链接
		cf.dwEffects |= CFE_LINK;
	else
		cf.dwEffects &= ~CFE_LINK;

	RichEdit_SetDefaultCharFormat(hWnd, cf);
}

// 设置默认字体名称和大小
void RichEdit_SetDefFont(HWND hWnd, LPCTSTR lpFontName, int nFontSize)
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(cf));
	RichEdit_GetDefaultCharFormat(hWnd, cf);

	if (lpFontName != NULL)					// 设置字体名称
	{
		cf.dwMask = cf.dwMask | CFM_FACE;
		_tcscpy(cf.szFaceName, lpFontName);
	}

	if (nFontSize > 0)						// 设置字体高度
	{
		cf.dwMask = cf.dwMask | CFM_SIZE;
		cf.yHeight = nFontSize * 20;
	}

	RichEdit_SetDefaultCharFormat(hWnd, cf);
}

// 设置默认字体颜色
void RichEdit_SetDefTextColor(HWND hWnd, COLORREF clrText)
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(cf));
	RichEdit_GetDefaultCharFormat(hWnd, cf);

	cf.dwMask = cf.dwMask | CFM_COLOR;
	cf.crTextColor = clrText;
	cf.dwEffects &= ~CFE_AUTOCOLOR;
	RichEdit_SetDefaultCharFormat(hWnd, cf);
}

// 设置默认超链接
void RichEdit_SetDefLinkText(HWND hWnd, BOOL bEnable)
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(cf));
	RichEdit_GetDefaultCharFormat(hWnd, cf);

	cf.dwMask = cf.dwMask | CFM_LINK;
	if (bEnable)							// 设置超链接
		cf.dwEffects |= CFE_LINK;
	else
		cf.dwEffects &= ~CFE_LINK;

	RichEdit_SetDefaultCharFormat(hWnd, cf);
}

// 设置字体
void RichEdit_SetFont(HWND hWnd, LPCTSTR lpFontName, int nFontSize,
					  COLORREF clrText, BOOL bBold, BOOL bItalic, 
					  BOOL bUnderLine, BOOL bIsLink)
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(cf));
	RichEdit_GetSelectionCharFormat(hWnd, cf);

	if (lpFontName != NULL)					// 设置字体名称
	{
		cf.dwMask = cf.dwMask | CFM_FACE;
		_tcscpy(cf.szFaceName, lpFontName);
	}

	if (nFontSize > 0)						// 设置字体高度
	{
		cf.dwMask = cf.dwMask | CFM_SIZE;
		cf.yHeight = nFontSize * 20;
	}

	cf.dwMask |= CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_LINK;

	cf.crTextColor = clrText;				// 设置字体颜色
	cf.dwEffects &= ~CFE_AUTOCOLOR;

	if (bBold)								// 设置粗体
		cf.dwEffects |= CFE_BOLD;
	else
		cf.dwEffects &= ~CFE_BOLD;

	if (bItalic)							// 设置倾斜
		cf.dwEffects |= CFE_ITALIC;
	else
		cf.dwEffects &= ~CFE_ITALIC;

	if (bUnderLine)							// 设置下划线
		cf.dwEffects |= CFE_UNDERLINE;
	else
		cf.dwEffects &= ~CFE_UNDERLINE;

	if (bIsLink)							// 设置超链接
		cf.dwEffects |= CFE_LINK;
	else
		cf.dwEffects &= ~CFE_LINK;

	RichEdit_SetSelectionCharFormat(hWnd, cf);
}

// 设置字体名称和大小
void RichEdit_SetFont(HWND hWnd, LPCTSTR lpFontName, int nFontSize)
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(cf));
	RichEdit_GetSelectionCharFormat(hWnd, cf);

	if (lpFontName != NULL)					// 设置字体名称
	{
		cf.dwMask = cf.dwMask | CFM_FACE;
		_tcscpy(cf.szFaceName, lpFontName);
	}

	if (nFontSize > 0)						// 设置字体高度
	{
		cf.dwMask = cf.dwMask | CFM_SIZE;
		cf.yHeight = nFontSize * 20;
	}

	RichEdit_SetSelectionCharFormat(hWnd, cf);
}

// 设置字体颜色
void RichEdit_SetTextColor(HWND hWnd, COLORREF clrText)
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(cf));
	RichEdit_GetSelectionCharFormat(hWnd, cf);

	cf.dwMask = cf.dwMask | CFM_COLOR;
	cf.crTextColor = clrText;
	cf.dwEffects &= ~CFE_AUTOCOLOR;

	RichEdit_SetSelectionCharFormat(hWnd, cf);
}

// 设置超链接
void RichEdit_SetLinkText(HWND hWnd, BOOL bEnable)
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(cf));
	RichEdit_GetSelectionCharFormat(hWnd, cf);

	cf.dwMask = cf.dwMask | CFM_LINK;
	if (bEnable)							// 设置超链接
		cf.dwEffects |= CFE_LINK;
	else
		cf.dwEffects &= ~CFE_LINK;

	RichEdit_SetSelectionCharFormat(hWnd, cf);
}

// 设置左缩进(单位:缇)
BOOL RichEdit_SetStartIndent(HWND hWnd, int nSize)
{
	PARAFORMAT2 pf2;
	memset(&pf2, 0, sizeof(pf2));
	pf2.cbSize = sizeof(PARAFORMAT2);
	pf2.dwMask = PFM_STARTINDENT;
	pf2.dxStartIndent = nSize;
	return (BOOL)::SendMessage(hWnd, EM_SETPARAFORMAT, 0, (LPARAM)&pf2);
}

// 插入表情图片
BOOL RichEdit_InsertFace(HWND hWnd, LPCTSTR lpszFileName, int nFaceId,
						 int nFaceIndex, COLORREF clrBg, BOOL bAutoScale, int nReservedWidth)
{
//	BSTR bstrFileName = NULL;
//	IRichEditOle * pRichEditOle = NULL;
//	IOleClientSite *pOleClientSite = NULL;
//	IImageOle* pImageOle = NULL;
//	IOleObject *pOleObject = NULL;
//	REOBJECT reobject = {0};
//	HRESULT hr = E_FAIL;
//
//	if (NULL == lpszFileName || NULL == *lpszFileName)
//		return FALSE;
//
//	TCHAR cProtocol[16] = {0};
//	_tcsncpy(cProtocol, lpszFileName, 7);
//	if ((_tcsicmp(cProtocol, _T("http://")) != 0) && ::GetFileAttributes(lpszFileName) == 0xFFFFFFFF)
//		return FALSE;
//
//	bstrFileName = ::SysAllocString((OLECHAR*)lpszFileName);
//	if (NULL == bstrFileName)
//		return FALSE;
//
//	pRichEditOle = RichEdit_GetOleInterface(hWnd);
//	if (NULL == pRichEditOle)
//		goto Ret0;
//
//	hr = ::CoCreateInstance(CLSID_ImageOle, NULL, 
//		CLSCTX_INPROC_SERVER, IID_IImageOle, (void**)&pImageOle);
//	if (FAILED(hr))
//		goto Ret0;
//
//	hr = pImageOle->QueryInterface(IID_IOleObject, (void **)&pOleObject);
//	if (FAILED(hr))
//		goto Ret0;
//
//	pRichEditOle->GetClientSite(&pOleClientSite);
//	if (NULL == pOleClientSite)
//		goto Ret0;
//
//	pOleObject->SetClientSite(pOleClientSite);
//
//	pImageOle->SetRichEditHwnd(hWnd);
//	pImageOle->SetTextServices(NULL);
//	pImageOle->SetTextHost(NULL);
//	pImageOle->SetFaceId(nFaceId);
//	pImageOle->SetFaceIndex(nFaceIndex);
//	pImageOle->SetBgColor(clrBg);
//	pImageOle->SetAutoScale(bAutoScale, nReservedWidth);
//	pImageOle->LoadFromFile(bstrFileName);
//
//	hr = ::OleSetContainedObject(pOleObject, TRUE);
//
//	reobject.cbStruct = sizeof(REOBJECT);
//	reobject.clsid    = CLSID_ImageOle;
//	reobject.cp       = REO_CP_SELECTION;
//	reobject.dvaspect = DVASPECT_CONTENT;
//	reobject.dwFlags  = REO_BELOWBASELINE;
//	reobject.poleobj  = pOleObject;
//	reobject.polesite = pOleClientSite;
//	reobject.dwUser   = 0;
//
//	hr = pRichEditOle->InsertObject(&reobject);
//
//Ret0:
//	if (pOleObject != NULL)
//		pOleObject->Release();
//
//	if (pImageOle != NULL)
//		pImageOle->Release();
//
//	if (pOleClientSite != NULL)
//		pOleClientSite->Release();
//
//	if (pRichEditOle != NULL)
//		pRichEditOle->Release();
//
//	if (bstrFileName != NULL)
//		::SysFreeString(bstrFileName);
//
//	return SUCCEEDED(hr);
	return FALSE;
}

// 获取文本
void RichEdit_GetText(HWND hWnd, tstring& strText)
{
//	REOBJECT reobject;
//	LONG nFaceId, nPos = 0;
//	tstring strOrgText, strTemp;
//
//	IRichEditOle * pRichEditOle = RichEdit_GetOleInterface(hWnd);
//	if (NULL == pRichEditOle)
//		return;
//
//	CHARRANGE chrg = {0, RichEdit_GetWindowTextLength(hWnd)};
//	RichEdit_GetTextRange(hWnd, &chrg, strOrgText);
//
//	for (LONG i = 0; i < (int)strOrgText.size(); i++)
//	{
//		memset(&reobject, 0, sizeof(REOBJECT));
//		reobject.cbStruct = sizeof(REOBJECT);
//		reobject.cp = i;
//		HRESULT hr = pRichEditOle->GetObject(REO_IOB_USE_CP, &reobject, REO_GETOBJ_POLEOBJ);
//		if (SUCCEEDED(hr))
//		{
//			if (reobject.cp > 0 && reobject.cp > nPos)
//			{
//				strTemp = strOrgText.substr(nPos, reobject.cp-nPos);
//				//Replace(strTemp, _T("/"), _T("//"));
//				strText += strTemp;
//			}
//			nPos = reobject.cp + 1;
//
//			if (NULL == reobject.poleobj)
//				continue;
//
//			if (CLSID_ImageOle == reobject.clsid)
//			{
//				IImageOle * pImageOle = NULL;
//				hr = reobject.poleobj->QueryInterface(__uuidof(IImageOle), (void**)&pImageOle);
//				if (SUCCEEDED(hr))
//				{
//					pImageOle->GetFaceId(&nFaceId);
//					if (nFaceId != -1)
//					{
//						TCHAR cBuf[32] = {0};
//						wsprintf(cBuf, _T("/f[\"%03d\"]"), nFaceId);
//						strText += cBuf;
//					}
//					else
//					{
//						strText += _T("/c[\"");
//						BSTR bstrFileName = NULL;
//						pImageOle->GetFileName(&bstrFileName);
//						//strText += bstrFileName;
//						::SysFreeString(bstrFileName);
//						strText += _T("\"]");
//					}
//					pImageOle->Release();
//				}
//			}
//			reobject.poleobj->Release();
//		}
//	}
//
//// 	int nCount = pRichEditOle->GetObjectCount();
//// 	for (int i = 0; i < nCount; i++)
//// 	{
//// 		memset(&reobject, 0, sizeof(REOBJECT));
//// 		reobject.cbStruct = sizeof(REOBJECT);
//// 		HRESULT hr = pRichEditOle->GetObject(i, &reobject, REO_GETOBJ_POLEOBJ);
//// 		if (hr == S_OK)
//// 		{
//// 			if (reobject.cp > 0 && reobject.cp > nPos)
//// 			{
//// 				strTemp = strOrgText.substr(nPos, reobject.cp-nPos);
//// 				Replace(strTemp, _T("/"), _T("//"));
//// 				strText += strTemp;
//// 			}
//// 			nPos = reobject.cp+1;
//// 
//// 			if (NULL == reobject.poleobj)
//// 				continue;
//// 
//// 			if (CLSID_ImageOle == reobject.clsid)
//// 			{
//// 				pImageOle = (IImageOle *)reobject.poleobj;
//// 				pImageOle = NULL;
//// 				reobject.poleobj->QueryInterface(__uuidof(IImageOle), (void**)&pImageOle);
//// 
//// 				pImageOle->GetFaceId(&nFaceId);
//// 				if (nFaceId != -1)
//// 				{
//// 					TCHAR cBuf[32] = {0};
//// 					wsprintf(cBuf, _T("/f[\"%03d\"]"), nFaceId);
//// 					strText += cBuf;
//// 				}
//// 				else
//// 				{
//// 					strText += _T("/c[\"");
//// 					BSTR bstrFileName = NULL;
//// 					pImageOle->GetFileName(&bstrFileName);
//// 					strText += bstrFileName;
//// 					::SysFreeString(bstrFileName);
//// 					strText += _T("\"]");
//// 				}
//// 				pImageOle->Release();
//// 			}
//// 
//// 			reobject.poleobj->Release();
//// 		}
//// 	}
//
//	if (nPos < (int)strOrgText.size())
//	{
//		strTemp = strOrgText.substr(nPos);
//		//Replace(strTemp, _T("/"), _T("//"));
//		strText += strTemp;
//	}
//
//	pRichEditOle->Release();
}

// 替换选中文本
void RichEdit_ReplaceSel(HWND hWnd, LPCTSTR lpszNewText, LPCTSTR lpFontName,
						 int nFontSize,	COLORREF clrText, BOOL bBold, BOOL bItalic, 
						 BOOL bUnderLine, BOOL bIsLink, int nStartIndent, BOOL bCanUndo/* = FALSE*/)
{
	long lStartChar = 0, lEndChar = 0;
	RichEdit_GetSel(hWnd, lStartChar, lEndChar);
	RichEdit_ReplaceSel(hWnd, lpszNewText, bCanUndo);
	lEndChar = lStartChar + _tcslen(lpszNewText);
	RichEdit_SetSel(hWnd, lStartChar, lEndChar);
	RichEdit_SetFont(hWnd, lpFontName, nFontSize, clrText, bBold, bItalic, bUnderLine, bIsLink);
	RichEdit_SetStartIndent(hWnd, nStartIndent);
	RichEdit_SetSel(hWnd, lEndChar, lEndChar);
}

//BOOL RichEdit_GetImageOle(HWND hWnd, POINT pt, IImageOle** pImageOle)
//{
//	IRichEditOle * pRichEditOle;
//	REOBJECT reobject;
//	HRESULT hr;
//	BOOL bRet = FALSE;
//
//	if (NULL == pImageOle)
//		return FALSE;
//
//	*pImageOle = NULL;
//
//	pRichEditOle = RichEdit_GetOleInterface(hWnd);
//	if (NULL == pRichEditOle)
//		return FALSE;
//
//	int nCount = pRichEditOle->GetObjectCount();
//	for (int i = 0; i < nCount; i++)
//	{
//		memset(&reobject, 0, sizeof(REOBJECT));
//		reobject.cbStruct = sizeof(REOBJECT);
//		hr = pRichEditOle->GetObject(i, &reobject, REO_GETOBJ_POLEOBJ);
//		if (S_OK == hr)
//		{
//			if (reobject.poleobj != NULL)
//			{
//				if (CLSID_ImageOle == reobject.clsid)
//				{
//					IImageOle * pImageOle2 = NULL;
//					hr = reobject.poleobj->QueryInterface(__uuidof(IImageOle), (void**)&pImageOle2);
//					reobject.poleobj->Release();
//					if (SUCCEEDED(hr))
//					{
//						RECT rcObject = {0};
//						hr = pImageOle2->GetObjectRect(&rcObject);
//						if (SUCCEEDED(hr))
//						{
//							if (::PtInRect(&rcObject, pt))
//							{
//								*pImageOle = pImageOle2;
//								bRet = TRUE;
//								break;
//							}
//						}
//						pImageOle2->Release();
//					}
//				}
//				else
//				{
//					reobject.poleobj->Release();
//				}
//			}
//		}
//	}
//
//	pRichEditOle->Release();
//
//	return bRet;
//}

IRichEditOle* RichEdit_GetOleInterface(ITextServices * pTextServices)
{
	IRichEditOle *pRichEditOle = NULL;
	pTextServices->TxSendMessage(EM_GETOLEINTERFACE, 0, (LPARAM)&pRichEditOle, NULL);
	return pRichEditOle;
}

int RichEdit_GetWindowTextLength(ITextServices * pTextServices)
{
	HRESULT lRes = 0;
	pTextServices->TxSendMessage(WM_GETTEXTLENGTH, 0, 0, &lRes);
	return (int)lRes;
}

int RichEdit_GetWindowText(ITextServices * pTextServices, LPTSTR lpszStringBuf, int nMaxCount)
{
	HRESULT lRes = 0;
	pTextServices->TxSendMessage(WM_GETTEXT, nMaxCount, (LPARAM)lpszStringBuf, &lRes);
	return (int)lRes;
}

int RichEdit_GetWindowText(ITextServices * pTextServices, tstring& strText)
{
	int nLength;
	TCHAR * pszText;

	nLength = RichEdit_GetWindowTextLength(pTextServices);
	pszText = new TCHAR[nLength+1];
	if (NULL == pszText)
		return 0;
	memset(pszText, 0, (nLength+1)*sizeof(TCHAR));
	nLength = RichEdit_GetWindowText(pTextServices, pszText, nLength+1);
	strText = pszText;
	delete []pszText;

	return nLength;
}

int RichEdit_GetTextRange(ITextServices * pTextServices, CHARRANGE * lpchrg, tstring& strText)
{
	strText = _T("");

	if (NULL == lpchrg || lpchrg->cpMax <= lpchrg->cpMin)
		return 0;

	LONG nLen = lpchrg->cpMax - lpchrg->cpMin;
	//TCHAR * pText = new TCHAR[nLen+1];
	//if (NULL == pText)
	//	return 0;

	//memset(pText, 0, (nLen+1)*sizeof(TCHAR));

	//TEXTRANGE tr = { 0 };
	//tr.chrg = *lpchrg;
	//tr.lpstrText = pText;

	//HRESULT lRes = 0;
	//pTextServices->TxSendMessage(EM_GETTEXTRANGE, 0, (LPARAM)&tr, &lRes);
	
	BSTR b;
	pTextServices->TxGetText(&b);

	_bstr_t bstr_t(b);

	strText = bstr_t;
	if(strText != "")
	{
		strText.replace(strText.length() - 1, 1, "");
	}
	//delete []pText;

	return 0/*(int)lRes*/;
}

DWORD RichEdit_GetDefaultCharFormat(ITextServices * pTextServices, CHARFORMAT& cf)
{
	cf.cbSize = sizeof(CHARFORMAT);
	HRESULT lRes = 0;
	pTextServices->TxSendMessage(EM_GETCHARFORMAT, 0, (LPARAM)&cf, &lRes);
	return (DWORD)lRes;
}

BOOL RichEdit_SetDefaultCharFormat(ITextServices * pTextServices, CHARFORMAT& cf)
{
	cf.cbSize = sizeof(CHARFORMAT);
	HRESULT lRes = 0;
	pTextServices->TxSendMessage(EM_SETCHARFORMAT, 0, (LPARAM)&cf, &lRes);
	return (BOOL)lRes;
}

DWORD RichEdit_GetSelectionCharFormat(ITextServices * pTextServices, CHARFORMAT& cf)
{
	cf.cbSize = sizeof(CHARFORMAT);
	LRESULT lRes = 0;
	pTextServices->TxSendMessage(EM_GETCHARFORMAT, 1, (LPARAM)&cf, &lRes);
	return (DWORD)lRes;
}

BOOL RichEdit_SetSelectionCharFormat(ITextServices * pTextServices, CHARFORMAT& cf)
{
	cf.cbSize = sizeof(CHARFORMAT);
	LRESULT lRes = 0;
	pTextServices->TxSendMessage(EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM)&cf, &lRes);
	return (BOOL)lRes;
}

void RichEdit_ReplaceSel(ITextServices * pTextServices, LPCTSTR lpszNewText, BOOL bCanUndo/* = FALSE*/)
{
	pTextServices->TxSendMessage(EM_REPLACESEL, (WPARAM)bCanUndo, (LPARAM)lpszNewText, NULL);
}

void RichEdit_GetSel(ITextServices * pTextServices, LONG& nStartChar, LONG& nEndChar)
{
	CHARRANGE cr = { 0, 0 };
	pTextServices->TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, NULL);
	nStartChar = cr.cpMin;
	nEndChar = cr.cpMax;
}

int RichEdit_SetSel(ITextServices * pTextServices, LONG nStartChar, LONG nEndChar)
{
	CHARRANGE cr = { nStartChar, nEndChar };
	HRESULT lRes = 0;
	pTextServices->TxSendMessage(EM_EXSETSEL, 0, (LPARAM)&cr, &lRes);
	return (int)lRes;
}

// 设置默认字体
void RichEdit_SetDefFont(ITextServices * pTextServices, LPCTSTR lpFontName,	
						 int nFontSize,	COLORREF clrText, BOOL bBold, 
						 BOOL bItalic, BOOL bUnderLine, BOOL bIsLink)
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(cf));
	RichEdit_GetDefaultCharFormat(pTextServices, cf);

	if (lpFontName != NULL)					// 设置字体名称
	{
		cf.dwMask = cf.dwMask | CFM_FACE;
		_tcscpy(cf.szFaceName, lpFontName);
	}

	if (nFontSize > 0)						// 设置字体高度
	{
		cf.dwMask = cf.dwMask | CFM_SIZE;
		cf.yHeight = nFontSize * 20;
	}

	cf.dwMask |= CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_LINK;

	cf.crTextColor = clrText;				// 设置字体颜色
	cf.dwEffects &= ~CFE_AUTOCOLOR;

	if (bBold)								// 设置粗体
		cf.dwEffects |= CFE_BOLD;
	else
		cf.dwEffects &= ~CFE_BOLD;

	if (bItalic)							// 设置倾斜
		cf.dwEffects |= CFE_ITALIC;
	else
		cf.dwEffects &= ~CFE_ITALIC;

	if (bUnderLine)							// 设置下划线
		cf.dwEffects |= CFE_UNDERLINE;
	else
		cf.dwEffects &= ~CFE_UNDERLINE;

	if (bIsLink)							// 设置超链接
		cf.dwEffects |= CFE_LINK;
	else
		cf.dwEffects &= ~CFE_LINK;

	RichEdit_SetDefaultCharFormat(pTextServices, cf);
}

// 设置字体
void RichEdit_SetFont(ITextServices * pTextServices, LPCTSTR lpFontName, int nFontSize,
					  COLORREF clrText, BOOL bBold, BOOL bItalic, BOOL bUnderLine, BOOL bIsLink)
{
	CHARFORMAT cf;
	memset(&cf, 0, sizeof(cf));
	RichEdit_GetSelectionCharFormat(pTextServices, cf);

	if (lpFontName != NULL)					// 设置字体名称
	{
		cf.dwMask = cf.dwMask | CFM_FACE;
		_tcscpy(cf.szFaceName, lpFontName);
	}

	if (nFontSize > 0)						// 设置字体高度
	{
		cf.dwMask = cf.dwMask | CFM_SIZE;
		cf.yHeight = nFontSize * 20;
	}

	cf.dwMask |= CFM_COLOR | CFM_BOLD | CFM_ITALIC | CFM_UNDERLINE | CFM_LINK;

	cf.crTextColor = clrText;				// 设置字体颜色
	cf.dwEffects &= ~CFE_AUTOCOLOR;

	if (bBold)								// 设置粗体
		cf.dwEffects |= CFE_BOLD;
	else
		cf.dwEffects &= ~CFE_BOLD;

	if (bItalic)							// 设置倾斜
		cf.dwEffects |= CFE_ITALIC;
	else
		cf.dwEffects &= ~CFE_ITALIC;

	if (bUnderLine)							// 设置下划线
		cf.dwEffects |= CFE_UNDERLINE;
	else
		cf.dwEffects &= ~CFE_UNDERLINE;

	if (bIsLink)							// 设置超链接
		cf.dwEffects |= CFE_LINK;
	else
		cf.dwEffects &= ~CFE_LINK;

	RichEdit_SetSelectionCharFormat(pTextServices, cf);
}

// 设置左缩进(单位:缇)
BOOL RichEdit_SetStartIndent(ITextServices * pTextServices, int nSize)
{
	PARAFORMAT2 pf2;
	memset(&pf2, 0, sizeof(pf2));
	pf2.cbSize = sizeof(PARAFORMAT2);
	pf2.dwMask = PFM_STARTINDENT;
	pf2.dxStartIndent = nSize;
	HRESULT lRes = 0;
	pTextServices->TxSendMessage(EM_SETPARAFORMAT, 0, (LPARAM)&pf2, &lRes);
	return (BOOL)lRes;
}

// 插入表情图片
BOOL RichEdit_InsertFace(ITextServices *pTextServices, DuiLib::CRichEditUI * pEdit, ITextHost *pTextHost,	
						 LPCTSTR lpszFileName, int nFaceId,	int nFaceIndex, 
						 COLORREF clrBg, BOOL bAutoScale, int nReservedWidth)
{
// 	BSTR bstrFileName = NULL;
// 	IRichEditOle * pRichEditOle = NULL;
// 	IOleClientSite *pOleClientSite = NULL;
// 	COleImage* pImageOle = NULL;
// 	IOleObject *pOleObject = NULL;
// 	REOBJECT reobject = {0};
// 	HRESULT hr = E_FAIL;
// 
// 	if (NULL == pTextServices || NULL == pTextHost ||
// 		NULL == lpszFileName || NULL == *lpszFileName)
// 		return FALSE;
// 
// 	TCHAR cProtocol[16] = {0};
// 	_tcsncpy(cProtocol, lpszFileName, 7);
// 	if ((_tcsicmp(cProtocol, _T("http://")) != 0) && ::GetFileAttributes(lpszFileName) == 0xFFFFFFFF)
// 		return FALSE;
// 	//lpszFileName = _T("E:\\1.png");
// 	CString str = lpszFileName;
// 
// 	bstrFileName = str.AllocSysString();
// 	//bstrFileName = ::SysAllocString((OLECHAR*)lpszFileName);
// 	//bstrFileName = ::SysAllocString();
// 	if (NULL == bstrFileName)
// 		return FALSE;
// 
// 	pTextServices->TxSendMessage(EM_GETOLEINTERFACE, 0, (LPARAM)&pRichEditOle, NULL);
// 	if (NULL == pRichEditOle)
// 		goto Ret0;
// 
// 	hr = ::CoCreateInstance(CLSID_OleImage, NULL, 
// 		CLSCTX_INPROC_SERVER, IID_IOleImage, (void**)&pImageOle);
// 	if (FAILED(hr))
// 		goto Ret0;
// 
// 	hr = pImageOle->QueryInterface(IID_IOleObject, (void **)&pOleObject);
// 	if (FAILED(hr))
// 		goto Ret0;
// 
// 	pRichEditOle->GetClientSite(&pOleClientSite);
// 	if (NULL == pOleClientSite)
// 		goto Ret0;
// 
// 	pOleObject->SetClientSite(pOleClientSite);
// 
// 	//pImageOle->SetTextServices(pTextServices);
// 	//pImageOle->SetTextHost(pTextHost);
// 	//pImageOle->SetFaceId(nFaceId);
// 	//pImageOle->SetFaceIndex(nFaceIndex);
// 	//pImageOle->SetBgColor(clrBg);
// 	//pImageOle->SetAutoScale(bAutoScale, nReservedWidth);
// 
// 	pImageOle->m_strImageName = lpszFileName;
// 
// 	// 加载图像
// 	pImageOle->LoadFromFile(/*lpszFileName*/_bstr_t(lpszFileName), pEdit, 400);
// 
// 	hr = ::OleSetContainedObject(pOleObject, TRUE);
// 
// 	reobject.cbStruct = sizeof(REOBJECT);
// 	reobject.clsid    = CLSID_ImageOle;
// 	reobject.cp       = REO_CP_SELECTION;
// 	reobject.dvaspect = DVASPECT_CONTENT;
// 	reobject.dwFlags  = REO_BELOWBASELINE;
// 	reobject.poleobj  = pOleObject;
// 	reobject.polesite = pOleClientSite;
// 	reobject.dwUser   = 0;
// 
// 	hr = pRichEditOle->InsertObject(&reobject);
// 
// Ret0:
// 	if (pOleObject != NULL)
// 		pOleObject->Release();
// 
// 	if (pImageOle != NULL)
// 		pImageOle->Release();
// 
// 	if (pOleClientSite != NULL)
// 		pOleClientSite->Release();
// 
// 	if (pRichEditOle != NULL)
// 		pRichEditOle->Release();
// 
// 	if (bstrFileName != NULL)
// 		::SysFreeString(bstrFileName);

	
	CComPtr<IStorage> spStorage;
	CComPtr<ILockBytes> spLockBytes;
	CComPtr<IOleClientSite> spOleClientSite;	
	COleImage* spOleImage;   //这不能用智能指针
	CComPtr<IOleObject> spOleObject;
	CLSID clsid;
	REOBJECT reobject;
	HRESULT hr = E_FAIL;

	do {

		// 创建LockBytes
		hr = CreateILockBytesOnHGlobal(NULL, TRUE, &spLockBytes);
		if (hr != S_OK) {
			break;
		}

		ASSERT(spLockBytes != NULL);

		// 创建Storage
		hr = StgCreateDocfileOnILockBytes(spLockBytes,
			STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, &spStorage);
		if (hr != S_OK) {
			break;
		}

		// 获取ClientSite
		IRichEditOle * pRichEditOle = RichEdit_GetOleInterface(pTextServices);
		hr = pRichEditOle->GetClientSite(&spOleClientSite);
		if (hr != S_OK) {
			break;
		}

		// 创建COleImage实例
		hr = CoCreateInstance(CLSID_OleImage, NULL, CLSCTX_INPROC, IID_IOleImage, (LPVOID*) &spOleImage);
		if (hr != S_OK) {
			break;
		}

		spOleImage->m_strImageName = lpszFileName;
		// 
		// 	// 加载图像
		hr = spOleImage->LoadFromFile(_bstr_t(lpszFileName), pEdit, 400);
		if (hr != S_OK) {
			break;
		}

		// 获取IOleObject接口
		hr = spOleImage->QueryInterface(IID_IOleObject, (LPVOID *) &spOleObject);
		if (hr != S_OK) {
			break;
		}
		spOleImage->Release();	/*QueryInterface会增加引用计数，调用后恢复一下*/	

		// 获取IOleObject的用户CLSID
		hr = spOleObject->GetUserClassID(&clsid);
		if (hr != S_OK) {
			break;
		}

		// 填充OLE对象属性
		ZeroMemory(&reobject, sizeof(REOBJECT));		
		reobject.cbStruct	= sizeof(REOBJECT);
		reobject.clsid		= clsid;
		reobject.cp			= REO_CP_SELECTION;
		reobject.dvaspect	= DVASPECT_CONTENT;
		reobject.dwFlags	= REO_BELOWBASELINE;
		reobject.poleobj	= spOleObject;
		reobject.polesite	= spOleClientSite;
		reobject.pstg		= spStorage;
		SIZEL sizel = {0};
		reobject.sizel = sizel;
		reobject.dwUser = -1;

		// 插入OLE对象
		hr = pRichEditOle->InsertObject(&reobject);
		if (hr != S_OK) {
			break;
		}

		// 通知OLE容器保证OLE对象被正确引用
		hr = OleSetContainedObject(spOleObject, TRUE);

		if(spOleObject)
			spOleObject.Release();	

		if(spLockBytes)
			spLockBytes.Release();

		if(spOleClientSite)
			spOleClientSite.Release();

		/*if(m_pRichEditOle)
		m_pRichEditOle.Release();*/

		//加入vec,为了释放清空
		//m_vecOleImage.push_back(spOleImage);

	} while (0);

	return SUCCEEDED(hr);
}

bool IsGifEmotion(CString str, int & id)
{
	//id = atoi((LPCSTR)str);
	id = GetGifIndex(str) + 1;
	if ( id > 0 && id < 60 )
		return true;
	else
		return false;

}

// 获取文本
void RichEdit_GetText(ITextServices * pTextServices, tstring& strText, IMAGE_FILE_LIST * ifl)
{
	if ( ifl )
		ifl->clear();

	REOBJECT reobject;
	LONG nFaceId, nPos = 0;
	tstring strOrgText, strTemp;

	IRichEditOle * pRichEditOle = RichEdit_GetOleInterface(pTextServices);
	if (NULL == pRichEditOle)
		return;

	CHARRANGE chrg = {0, RichEdit_GetWindowTextLength(pTextServices)};
	RichEdit_GetTextRange(pTextServices, &chrg, strOrgText);

	int nChar = 0;
	for (LONG i = 0; i < (int)strOrgText.size(); i++)
	{
		memset(&reobject, 0, sizeof(REOBJECT));
		reobject.cbStruct = sizeof(REOBJECT);
		reobject.cp = i - nChar;

		BYTE bChar = strOrgText[i];
		if(bChar > 127)
		{
			i++;
			nChar++;
		}

		HRESULT hr = pRichEditOle->GetObject(REO_IOB_USE_CP, &reobject, REO_GETOBJ_POLEOBJ);
		if (SUCCEEDED(hr))
		{
			if (reobject.cp > 0 && reobject.cp > nPos - nChar)
			{
				strTemp = strOrgText.substr(nPos, reobject.cp+nChar-nPos);
				//Replace(strTemp, _T("/"), _T("//"));
				strText += strTemp;
			}
			nPos = reobject.cp + 1 + nChar;

			if (NULL == reobject.poleobj)
				continue;

			if (CLSID_OleImage == reobject.clsid)
			{
				COleImage * pImageOle = NULL;
				hr = reobject.poleobj->QueryInterface(IID_IOleImage, (void**)&pImageOle);
				if (SUCCEEDED(hr))
				{
					//pImageOle->GetFaceId(&nFaceId);
					//if (nFaceId != -1)
					//{
					//	TCHAR cBuf[32] = {0};
					//	wsprintf(cBuf, _T("/f[\"%03d\"]"), nFaceId);
					//	strText += cBuf;
					//}
					//else
					//{
						strText += _T("<image:");
						BSTR bstrFileName = NULL;
						//bstrFileName = pImageOle->m_strImageName;

						//BSTR bstrTest = ::SysAllocString(bstrFileName);
						//_bstr_t bstr_t(bstrTest);
						//std::string stl = bstr_t;
						//SysFreeString(bstrTest );

						std::string stl = pImageOle->m_strImageName;
						
						CString strFileNameNew = stl.c_str();
						CString strPath,strFileName,strExt;
						ExtractFilePath(strFileNameNew,strPath,strFileName,strExt);
						strFileNameNew.Format("%s%s", strFileName, strExt);

						IMAGE_FILE_STRUCT image;
						image.file_session_id   = CRichEditCtrlEx::m_dwFileSessionId;
						image.file_size         = GetFileSize(bstrFileName);
						ASSERT(image.file_size > 0);
						sprintf(image.file_name, "%s%s", strFileName, strExt ); 

						if ( ifl )
						{
							bool find=false;

							IMAGE_FILE_LIST::iterator it;
							for ( it=ifl->begin(); it!=ifl->end(); it++ )
							{
								if (strcmp( (*it).file_name, image.file_name)==0 )
								{
									find=true;
								}
							}

							if ( find==false )
							{
								int nGifId = 0;
								if(!IsGifEmotion(image.file_name, nGifId))	// 排除系统表情
								{	
									CRichEditCtrlEx::m_dwFileSessionId++;
									ifl->push_back(image);
								}
							}
						}

						stl = strFileNameNew;
						strText = strText + stl;
						::SysFreeString(bstrFileName);
						strText += _T(">");
					//}
					pImageOle->Release();
				}
			}
			reobject.poleobj->Release();
		}
	}

	if (nPos < (int)strOrgText.size())
	{
		strTemp = strOrgText.substr(nPos);
		//Replace(strTemp, _T("/"), _T("//"));
		strText += strTemp;
	}

	pRichEditOle->Release();
}

// 替换选中文本
void RichEdit_ReplaceSel(ITextServices * pTextServices, LPCTSTR lpszNewText,	
						 LPCTSTR lpFontName, int nFontSize,	COLORREF clrText, 
						 BOOL bBold, BOOL bItalic, BOOL bUnderLine, BOOL bIsLink, 
						 int nStartIndent, BOOL bCanUndo/* = FALSE*/)
{
	long lStartChar = 0, lEndChar = 0;
	RichEdit_GetSel(pTextServices, lStartChar, lEndChar);
	RichEdit_ReplaceSel(pTextServices, lpszNewText, bCanUndo);
	lEndChar = lStartChar + _tcslen(lpszNewText);
	RichEdit_SetSel(pTextServices, lStartChar, lEndChar);
	RichEdit_SetFont(pTextServices, lpFontName, nFontSize, clrText, bBold, bItalic, bUnderLine, bIsLink);
	RichEdit_SetStartIndent(pTextServices, nStartIndent);
	RichEdit_SetSel(pTextServices, lEndChar, lEndChar);
}

// BOOL RichEdit_GetImageOle(ITextServices * pTextServices, POINT pt, IImageOle** pImageOle)
// {
// 	IRichEditOle * pRichEditOle;
// 	REOBJECT reobject;
// 	HRESULT hr;
// 	BOOL bRet = FALSE;
// 
// 	if (NULL == pImageOle)
// 		return FALSE;
// 
// 	*pImageOle = NULL;
// 
// 	pRichEditOle = RichEdit_GetOleInterface(pTextServices);
// 	if (NULL == pRichEditOle)
// 		return FALSE;
// 
// 	int nCount = pRichEditOle->GetObjectCount();
// 	for (int i = 0; i < nCount; i++)
// 	{
// 		memset(&reobject, 0, sizeof(REOBJECT));
// 		reobject.cbStruct = sizeof(REOBJECT);
// 		hr = pRichEditOle->GetObject(i, &reobject, REO_GETOBJ_POLEOBJ);
// 		if (S_OK == hr)
// 		{
// 			if (reobject.poleobj != NULL)
// 			{
// 				if (CLSID_ImageOle == reobject.clsid)
// 				{
// 					IImageOle * pImageOle2 = NULL;
// 					hr = reobject.poleobj->QueryInterface(__uuidof(IImageOle), (void**)&pImageOle2);
// 					reobject.poleobj->Release();
// 					if (SUCCEEDED(hr))
// 					{
// 						RECT rcObject = {0};
// 						hr = pImageOle2->GetObjectRect(&rcObject);
// 						if (SUCCEEDED(hr))
// 						{
// 							if (::PtInRect(&rcObject, pt))
// 							{
// 								*pImageOle = pImageOle2;
// 								bRet = TRUE;
// 								break;
// 							}
// 						}
// 						pImageOle2->Release();
// 					}
// 				}
// 				else
// 				{
// 					reobject.poleobj->Release();
// 				}
// 			}
// 		}
// 	}
// 
// 	pRichEditOle->Release();
// 
// 	return bRet;
// }

bool FindFirstEmotion(CString str , int &iEmotion, int & nEmBeg,int & nEmEnd, LPCTSTR* emotions)
{
	int count = sizeof(emotions)/sizeof(LPCSTR);

	iEmotion = -1;
	nEmBeg = -1;
	nEmEnd = -1;

	bool bRet=false;
	int i;
	int nBeg = str.Find("<image|",0);
	int nEnd;
	CString sEmotion;

	if(nBeg > -1)
	{
		nEnd = str.Find("/>",nBeg);
		if(nEnd <= nBeg)
		{
			return false;
		}
		sEmotion = str.Mid(nBeg + 7,nEnd - nBeg - 7);
	}
	//for (i = 0; i < count; ++i) 
	//{
	//	if(sEmotion == emotions[i])
	//	{
	//		nEmBeg = nBeg;
	//		nEmEnd = nEnd + 2;
	//		iEmotion = i;
	//		bRet = true;
	//		break;
	//	}		
	//} 
	return bRet;
}

bool FindFirstImage(CString str,CString &strImageFileName,int &iImageStart,int &iImageEnd)
{
	int n=str.Find("<image:",0);

	if ( n>-1 )
	{
		iImageStart=n;

		n=str.Find(">",iImageStart);

		if ( n>iImageStart )
		{
			iImageEnd=n+1;
			strImageFileName = str.Mid(iImageStart+7,iImageEnd-iImageStart-8);
			return true;
		}		
	}

	return false;
}

CString GetStoreInformation(CString strHttpConnection, CString strHttpFile)
{
	try
	{
		CInternetSession mysession;
		CHttpConnection *myconn=mysession.GetHttpConnection(strHttpConnection);
		CHttpFile *myfile=myconn->OpenRequest("GET",strHttpFile);
		myfile->SendRequest();
		CString mystr;
		CString tmp;
		while(myfile->ReadString(tmp))
		{
			mystr+=tmp;
		}
		myfile->Close();
		myconn->Close();
		mysession.Close();
		delete myfile;
		delete myconn;
		myfile=0;
		myconn=0;
		return mystr;
	}
	catch (CMemoryException* e)
	{
		return "";
	}
	catch (CFileException* e)
	{
		return "";
	}
	catch (CException* e)
	{
		return "";
	}
}

void InsertLinkInfo(ITextServices *pTextServices, CString strStoreUrl, CString strStoreLogoUrl, CString strStoreName, CString strStoreDesc)
{
	IStorage*  lpStorage  = NULL;//存储接口
	IOleObject*  lpOleObject  = NULL;//OLE对象
	LPLOCKBYTES  lpLockBytes  = NULL;//LOCKBYTE
	IOleClientSite* lpOleClientSite = NULL; 
	_DOleLinkInfo* iut = NULL;
	CLSID   clsid;
	REOBJECT  reobject;
	HRESULT   hr;
	IRichEditOle *lpRichEditOle = RichEdit_GetOleInterface(pTextServices);// = GetIRichEditOle();
	if(lpRichEditOle == NULL)
		return;
	//创建PolyCtl对象并获取接口HKEY_CLASSES_ROOT\Wow6432Node\CLSID
	//const GUID CDECL tlid = { 0x6289C187, 0x8A21, 0x4C65, { 0xA3, 0x7A, 0x30, 0xC5, 0x0C, 0xCB, 0x7D, 0x77} };
	//IID IID_DOleTest5 = { 0x693cc9bf, 0xb11e, 0x4ce6, { 0x82, 0x85, 0x8c, 0x0d, 0x8a, 0x49, 0x29, 0x8a} };
	//IUnknown* iu = m_o1.GetControllingUnknown(); 
	hr = ::CoCreateInstance(/*_tlid*/CLSID_OleLinkInfo,NULL,CLSCTX_INPROC,DIID__DOleLinkInfo/*IID_DOleTest10*/,(LPVOID*)&/*lpPolyCtl*/iut);
	//m_o1.m_pOuterUnknown = iu;
	if( /*lpPolyCtl*/iut == NULL )
	{
		return;
	}

	// USES_CONVERSION;
	BOOL bRet = TRUE;

	try{
		hr = /*lpPolyCtl->*/iut->QueryInterface(/*IID_DOleTest5/ *IID_DOleTest10* /,*/ /*(LPVOID *)*/&lpOleObject);//获得数据对象接口
		iut->Release();		/*QueryInterface会增加引用计数，调用后恢复一下*/	

		if( hr != S_OK ) 
			AfxThrowOleException(hr);
		hr = lpOleObject->GetUserClassID(&clsid);
		if ( hr != S_OK)
			AfxThrowOleException(hr);
		CString strLocalPath = strStoreLogoUrl.Right(strStoreLogoUrl.GetLength() - strStoreLogoUrl.ReverseFind('/') - 1);
		CString strDownloadFile = g_LocalData.GetImageFileCachePath()+strLocalPath;

		//图片改造
		CString strUrl = strStoreLogoUrl;
		if (strUrl.Find("http://") == -1)
		{
			strUrl = GetPicRandomUrl() + strUrl;
		}

		DownLoadWebFile(/*strStoreLogoUrl*/strUrl,strDownloadFile);
		LPCTSTR lpPic = (LPCSTR)strDownloadFile;

		iut->SetPicFilePath(lpPic);

		CString strHead = strStoreName;
		strHead.Trim();
		LPCTSTR lpHead = (LPCSTR)strHead;
		iut->SetHeadText(lpHead);

		CString strContent = strStoreDesc;
		strContent.Trim();
		LPCTSTR lpContent = (LPCSTR)strContent;
		iut->SetContentText(lpContent);

		LPCTSTR lpUrl = (LPCSTR)strStoreUrl;
		iut->SetUrlText(lpUrl);
		if( hr != S_OK ) 
			AfxThrowOleException(hr);

		hr = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);//创建LOCKBYTE对象
		if (hr != S_OK)
			AfxThrowOleException(hr);
		ASSERT(lpLockBytes != NULL);

		hr = ::StgCreateDocfileOnILockBytes(lpLockBytes,//创建复合文档
			STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &lpStorage);
		if (hr != S_OK)
		{
			VERIFY(lpLockBytes->Release() == 0);
			lpLockBytes = NULL;
			AfxThrowOleException(hr);
		}

		lpRichEditOle->GetClientSite(&lpOleClientSite);

		ZeroMemory(&reobject, sizeof(REOBJECT));//初始化一个对象  
		reobject.cbStruct = sizeof(REOBJECT);
		reobject.clsid  = clsid;
		reobject.cp   = REO_CP_SELECTION;
		reobject.dvaspect = DVASPECT_CONTENT;
		reobject.dwFlags = REO_BELOWBASELINE;
		reobject.dwUser   = ODT_LINK_INFO;
		reobject.poleobj = lpOleObject;
		reobject.polesite = lpOleClientSite;
		reobject.pstg  = lpStorage;
		reobject.sizel.cx = 8000;
		reobject.sizel.cy = 2800;

		hr = lpRichEditOle->InsertObject( &reobject );
		if (hr != S_OK)
			AfxThrowOleException(hr);
		OleSetContainedObject(lpOleObject,TRUE);
	}

	catch( COleException* e )
	{
		TRACE(_T("OleException code:%d"),e->m_sc);
		TCHAR szError[1024];
		e->GetErrorMessage(szError, 1024);
		e->Delete();
		bRet = FALSE;
	}
	catch(_com_error& e)
	{
		//CString strMsg;
		//strMsg.Format(_T("错误描述：%s\n错误消息：%s"), 
		//	(LPCTSTR)e.Description(),
		//	(LPCTSTR)e.ErrorMessage());
		bRet = FALSE;
	}

	// release the interface
	if( lpOleObject  != NULL )  lpOleObject->Release();
	if( lpOleClientSite != NULL ) lpOleClientSite->Release();
	if( lpStorage  != NULL ) lpStorage->Release();
}

BOOL RichEdit_InsertImage(DuiLib::CRichEditUI * pEdit, LPCTSTR lpszFileName, int nFaceId, int nFaceIndex)
{
	BOOL bRet = FALSE;

	if (NULL == pEdit || NULL == lpszFileName || NULL == *lpszFileName)
		return FALSE;

	ITextServices * pTextServices = pEdit->GetTextServices();
	ITextHost * pTextHost = pEdit->GetTextHost();
	if (pTextServices != NULL && pTextHost != NULL)
	{
		//if (pRichEdit == m_pRecvEdit)
		//	RichEdit_SetStartIndent(pTextServices, 300);
		bRet = RichEdit_InsertFace(pTextServices, pEdit, pTextHost, 
			lpszFileName, nFaceId, nFaceIndex, RGB(255,255,255), TRUE, 40);
	}

	if (pTextServices != NULL)
		pTextServices->Release();
	if (pTextHost != NULL)
		pTextHost->Release();
	return bRet;
}

void InsertWaitIcon(ITextServices* pTextServices, DuiLib::CRichEditUI* pEdit, LPCTSTR lpszPathName, IMAGE_FILE_STRUCT *pStruct)
{
	// 全部使用智能指针
	CComPtr<IStorage> spStorage;
	CComPtr<ILockBytes> spLockBytes;
	CComPtr<IOleClientSite> spOleClientSite;	
	COleImage* spOleImage;   //这不能用智能指针
	CComPtr<IOleObject> spOleObject;
	CLSID clsid;
	REOBJECT reobject;
	HRESULT hr = E_FAIL;

	do {

		// 创建LockBytes
		hr = CreateILockBytesOnHGlobal(NULL, TRUE, &spLockBytes);
		if (hr != S_OK) {
			break;
		}

		ASSERT(spLockBytes != NULL);

		// 创建Storage
		hr = StgCreateDocfileOnILockBytes(spLockBytes,
			STGM_SHARE_EXCLUSIVE | STGM_CREATE | STGM_READWRITE, 0, &spStorage);
		if (hr != S_OK) {
			break;
		}

		IRichEditOle *lpRichEditOle = RichEdit_GetOleInterface(pTextServices);

		// 获取ClientSite
		hr = lpRichEditOle->GetClientSite(&spOleClientSite);
		if (hr != S_OK) {
			break;
		}

		// 创建COleImage实例
		hr = CoCreateInstance(CLSID_OleImage, NULL, CLSCTX_INPROC, IID_IOleImage, (LPVOID*) &spOleImage);
		if (hr != S_OK) {
			break;
		}

		spOleImage->m_strImageName = (CString)lpszPathName;
		spOleImage->m_lOwnData = (long)pStruct;

		// 加载图像
		hr = spOleImage->LoadFromFile(_bstr_t(lpszPathName), pEdit, 400);
		if (hr != S_OK) {
			break;
		}

		// 获取IOleObject接口
		hr = spOleImage->QueryInterface(IID_IOleObject, (LPVOID *) &spOleObject);
		if (hr != S_OK) {
			break;
		}
		spOleImage->Release();	/*QueryInterface会增加引用计数，调用后恢复一下*/	

		// 获取IOleObject的用户CLSID
		hr = spOleObject->GetUserClassID(&clsid);
		if (hr != S_OK) {
			break;
		}

		// 填充OLE对象属性
		ZeroMemory(&reobject, sizeof(REOBJECT));		
		reobject.cbStruct	= sizeof(REOBJECT);
		reobject.clsid		= clsid;
		reobject.cp			= REO_CP_SELECTION;
		reobject.dvaspect	= DVASPECT_CONTENT;
		reobject.dwFlags	= REO_BELOWBASELINE;
		reobject.poleobj	= spOleObject;
		reobject.polesite	= spOleClientSite;
		reobject.pstg		= spStorage;
		SIZEL sizel = {0};
		reobject.sizel = sizel;
		reobject.dwUser = ODT_WAITICON;

		// 插入OLE对象
		hr = lpRichEditOle->InsertObject(&reobject);
		if (hr != S_OK) {
			break;
		}

		// 通知OLE容器保证OLE对象被正确引用
		hr = OleSetContainedObject(spOleObject, TRUE);

		if(spOleObject)
			spOleObject.Release();	

		if(spLockBytes)
			spLockBytes.Release();

		if(spOleClientSite)
			spOleClientSite.Release();

		//加入vec,为了释放清空
		//m_vecOleImage.push_back(spOleImage);

	} while (0);

}

void InsertGifEmotion(ITextServices* pTextServices, DuiLib::CRichEditUI* pEdit, int i)
{
	CString   strGif = g_LocalData.GetGifFacePath(i);

	if ( FileExist(strGif)==FALSE )
	{
		return ;
	}
	RichEdit_InsertImage(pEdit, (LPCTSTR)strGif, -1, -1);
	return;

	LPLOCKBYTES lpLockBytes = NULL;

	SCODE   sc ;
	HRESULT hr ;

	LPOLECLIENTSITE lpClientSite ;
#ifndef _XY_PICTURE
	IShowImagePtr	lpAnimator   ;
#else
	IXPicturePtr	lpAnimator   ;
#endif
	LPSTORAGE		lpStorage    ;
	LPOLEOBJECT		lpObject     ;

	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)
		AfxThrowOleException(sc);
	ASSERT(lpLockBytes != NULL);

	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &lpStorage);
	if (sc != S_OK)
	{
		VERIFY(lpLockBytes->Release() == 0);
		lpLockBytes = NULL;
		AfxThrowOleException(sc);
	}
	ASSERT(lpStorage != NULL);

	IRichEditOle *lpRichEditOle = RichEdit_GetOleInterface(pTextServices);

	lpRichEditOle->GetClientSite(&lpClientSite);

	ASSERT(lpClientSite != NULL);

	try
	{
#ifndef _XY_PICTURE
		hr = lpAnimator.CreateInstance(CLSID_ShowImage);
#else
		hr = lpAnimator.CreateInstance("XPictureMain.XPicture.1");
#endif
		if( FAILED(hr) )
			_com_issue_error(hr);
		//m_arrImageData.push_back(lpAnimator);

		BSTR bstr = strGif.AllocSysString();
#ifndef _XY_PICTURE
		hr = lpAnimator->LoadImageFile(bstr);
#else
		hr = lpAnimator->LoadImage(bstr);
#endif
		::SysFreeString(bstr);

		if( FAILED(hr) )
			_com_issue_error(hr);

		hr = lpAnimator.QueryInterface(IID_IOleObject, (void**)&lpObject);
		if( FAILED(hr) )
			_com_issue_error(hr);
//#ifndef _XY_PICTURE
//		lpAnimator->PutOwnData((LONG*)m_nArrayGifId[i]);
//#else
//		lpAnimator->SetUserData((LONG)m_nArrayGifId[i]);
//#endif
		OleSetContainedObject(lpObject, TRUE);

		//2 insert in 2 richedit, you need a struct of REOBJECT
		REOBJECT reobject;
		ZeroMemory(&reobject, sizeof(REOBJECT));

		reobject.cbStruct = sizeof(REOBJECT);
		CLSID clsid;
		sc = lpObject->GetUserClassID(&clsid);
		if (sc != S_OK)
			AfxThrowOleException(sc);

		reobject.clsid	= clsid;
		reobject.cp		= REO_CP_SELECTION;
		reobject.dvaspect	= DVASPECT_CONTENT;
		reobject.dwFlags	= REO_BELOWBASELINE | REO_INPLACEACTIVE ;
		reobject.dwUser		= ODT_GIF;
		reobject.poleobj	= lpObject;
		reobject.polesite	= lpClientSite;
		reobject.pstg		= lpStorage;

		SIZEL sizel={0,0};
		reobject.sizel = sizel;

		lpRichEditOle->InsertObject(&reobject);


		if(lpObject)
			lpObject->Release();	

		if(lpLockBytes)
			lpLockBytes->Release();

		if(lpClientSite)
			lpClientSite->Release();

		if(lpRichEditOle)
			lpRichEditOle->Release();

		if(lpStorage)
			lpStorage->Release();


	}
	catch(...)
	{
	}
}

void RichEdit_InsertLine(ITextServices * pTextServices)
{
	CResBitmap bmpLine;
	bmpLine.LoadBitmap(IDB_MSG_LINE);
	IRichEditOle *lpRichEditOle = RichEdit_GetOleInterface(pTextServices);
	CImageDataObject::InsertBitmap(lpRichEditOle,(HBITMAP)bmpLine.GetSafeHandle(),ODT_BITMAP);
}

void RichEdit_InsertText(ITextServices * pTextServices, LPCTSTR lpText, DuiLib::CRichEditUI* pEdit, bool bWaitImageFile, CStringList *pslWaitImageFile, HWND hWnd, bool bIsReditSend)
{
	RichEdit_SetSel(pTextServices, -1, -1);

	int iEmotion;
	int iImageStart,iImageEnd;
	int nEmotionBeg;
	int nEmotionEnd;

	CString strImageFileName;

	int nLinkStoreStart;
	int nLinkStoreEnd;
	int nLinkItemStart;
	int nLinkItemEnd;
	bool bFindEmotion, bFindImage, bIsLinkStore, bIsLinkItem;

	CHARRANGE cr;
	pEdit->GetSel(cr);

	CString text = lpText;
	while ( 1 )
	{
		bFindEmotion = false;
		bFindImage = false;
		bIsLinkStore = false;
		bIsLinkItem = false;

		nLinkStoreStart = text.Find(STORE_URL);
		if(nLinkStoreStart != -1)
			bIsLinkStore = bIsReditSend ? false : true;

		nLinkItemStart = text.Find(ITEM_URL);
		if(nLinkItemStart != -1)
			bIsLinkItem = bIsReditSend ? false : true;

		bFindEmotion = FindFirstEmotion(text,iEmotion,nEmotionBeg,nEmotionEnd, NULL);
		bFindImage   = FindFirstImage(text,strImageFileName,iImageStart,iImageEnd);

		if(!bFindEmotion && !bFindImage && !bIsLinkStore && !bIsLinkItem)
			break;

		if(nLinkStoreStart != -1)
		{
			nLinkStoreEnd = text.Find(".html") + 5;
			//CString strUrl = text.Mid(nLinkStoreStart, nLinkStoreEnd - nLinkStoreStart);
		}

		if(nLinkItemStart != -1)
		{
			CString strUrlSize = ITEM_URL;
			CString strItemRight = text.Right(text.GetLength() - strUrlSize.GetLength() - nLinkItemStart);
			CString strItem = "";
			while(strItemRight.GetAt(0) >= 0x30 && strItemRight.GetAt(0) <= 0x39)
			{
				strItem += strItemRight.GetAt(0);
				strItemRight.Delete(0);
			}
			nLinkItemEnd = nLinkItemStart + strUrlSize.GetLength() + strItem.GetLength();
		}
		


		if(max(nLinkStoreStart, nLinkItemStart) < max(nEmotionBeg, iImageStart) && (bIsLinkItem || bIsLinkStore))
		{
			bFindImage = false;
			bFindEmotion = false;
			if(nLinkStoreStart < nLinkItemStart && bIsLinkStore)
				bIsLinkItem = false;
			else if(nLinkStoreStart > nLinkItemStart && bIsLinkItem)
				bIsLinkStore = false;
		}
		else if(max(nLinkStoreStart, nLinkItemStart) > max(nEmotionBeg, iImageStart) && (bFindImage || bFindEmotion))
		{
			bIsLinkItem = false;
			bIsLinkStore = false;
			if(nEmotionBeg < iImageStart && bFindEmotion)
				bFindImage = false;
			else if(nEmotionBeg > iImageStart && bFindImage)
				bFindEmotion = false;
		}

		if(bIsLinkStore)
		{
			CString strUrlStore = text.Mid(nLinkStoreStart, nLinkStoreEnd - nLinkStoreStart);

			text = text.Mid(nLinkStoreEnd);

			CString strStoreFind = STORE_URL;
			int nFindStore = strUrlStore.Find(strStoreFind);
			if( nFindStore != -1)
			{
				int nFindLastDot = strUrlStore.ReverseFind('.');
				if(nFindLastDot != -1)
				{
					CString strStoreID = strUrlStore.Mid(nFindStore + strStoreFind.GetLength(), nFindLastDot - strStoreFind.GetLength() - nFindStore);
					if(/*m_bIsShowLinkInformation*/true)
					{
						CString strHttpConnectionStore = WEBIM_URL;
						CString strHttpFileStore = WEBIM_LINKINFO_STORE + strStoreID;
						CString strStoreResult = GetStoreInformation(strHttpConnectionStore, strHttpFileStore);

						Json::Reader jReader;
						Json::Value jvSellerInfo;

						std::string strStore = LPCSTR(strStoreResult);
						jReader.parse(strStore,jvSellerInfo);
						std::string storeSuccess = jvSellerInfo["success"].asString();	

						if(storeSuccess == "true")
						{
							std::string strStoreData = LPCSTR("data");
							Json::Value valueData = jvSellerInfo.get(strStoreData, jvSellerInfo);

							std::string strStoreInfo = LPCSTR("storeInfo");
							Json::Value valueInfo = valueData.get(strStoreInfo, valueData);

							std::string storeLogoUrl = valueInfo["storeLogoUrl"].asString();
							std::string storeName = valueInfo["storeName"].asString();
							std::string storeDesc = valueInfo["storeDesc"].asString();

							CString strStoreLogoUrl = storeLogoUrl.c_str();
							CString strStoreName = storeName.c_str();
							CString strStoreDesc = storeDesc.c_str();

							string strGbkStoreDesc = UTF8_Convert(strStoreDesc, FALSE);
							strStoreDesc = strGbkStoreDesc.c_str();
							string strGbkStoreName = UTF8_Convert(strStoreName, FALSE);
							strStoreName = strGbkStoreName.c_str();

							//去掉字体样式
							strStoreDesc.Replace("&nbsp;", "");
							strStoreDesc.Replace("<br />", "");
							strStoreDesc.Replace("&gt;", "");
							strStoreDesc.Replace("&lt;", "");
							strStoreDesc.Replace("\r", "");
							strStoreDesc.Replace("\n", "");
							strStoreDesc.Replace("</span>", "");
							strStoreDesc.Replace("</em>", "");
							strStoreDesc.Replace("</u>", "");
							strStoreDesc.Replace("</strong>", "");

							while( strStoreDesc.Find("<span") != -1)
							{
								CString str = strStoreDesc.Right(strStoreDesc.GetLength() - strStoreDesc.Find("<span"));
								if(str.Left(str.Find(">")) != -1)
								{
									str = str.Left(str.Find(">") + 1);
									strStoreDesc.Replace(str, "");
								}
								else
									strStoreDesc.Delete(strStoreDesc.Find("<span"));
							}
							while( strStoreDesc.Find("<em") != -1)
							{
								CString str = strStoreDesc.Right(strStoreDesc.GetLength() - strStoreDesc.Find("<em"));
								if(str.Left(str.Find(">")) != -1)
								{
									str = str.Left(str.Find(">") + 1);
									strStoreDesc.Replace(str, "");
								}
								else
									strStoreDesc.Delete(strStoreDesc.Find("<em"));
							}
							while( strStoreDesc.Find("<u") != -1)
							{
								CString str = strStoreDesc.Right(strStoreDesc.GetLength() - strStoreDesc.Find("<u"));
								if(str.Left(str.Find(">")) != -1)
								{
									str = str.Left(str.Find(">") + 1);
									strStoreDesc.Replace(str, "");
								}
								else
									strStoreDesc.Delete(strStoreDesc.Find("<u"));
							}
							while( strStoreDesc.Find("<strong") != -1)
							{
								CString str = strStoreDesc.Right(strStoreDesc.GetLength() - strStoreDesc.Find("<strong"));
								if(str.Left(str.Find(">")) != -1)
								{
									str = str.Left(str.Find(">") + 1);
									strStoreDesc.Replace(str, "");
								}
								else
									strStoreDesc.Delete(strStoreDesc.Find("<strong"));
							}
							while (strStoreDesc.Find("/>") != -1)
							{
								CString str = strStoreDesc.Left(strStoreDesc.Find("/>") + 2);
								if(str.ReverseFind('<') != -1)
								{
									str = str.Right(str.GetLength() - str.ReverseFind('<'));
									strStoreDesc.Replace(str, "");
								}
								else
								{
									strStoreDesc.Delete(strStoreDesc.Find("/>"), 2);
								}
							}
							while( strStoreDesc.Find("<div") != -1)
							{
								CString str = strStoreDesc.Right(strStoreDesc.GetLength() - strStoreDesc.Find("<div"));
								if(str.Left(str.Find(">")) != -1)
								{
									str = str.Left(str.Find(">") + 1);
									strStoreDesc.Replace(str, "");
								}
								else
									strStoreDesc.Delete(strStoreDesc.Find("<div"));
							}

							pEdit->ReplaceSel("\r", false);
							InsertLinkInfo(pTextServices, strUrlStore, strStoreLogoUrl, strStoreName, strStoreDesc);
							if(text != "")
								pEdit->ReplaceSel("\r", false);
						}
					}
				}
			}
		}
		else if(bIsLinkItem)
		{
			CString strUrlItem = text.Mid(nLinkItemStart, nLinkItemEnd - nLinkItemStart);
			text = text.Mid(nLinkItemEnd);

			CString strItemFind = ITEM_URL;
			int nFindItem = strUrlItem.Find(strItemFind);
			if( nFindItem != -1)
			{
				CString strItemID = strUrlItem.Right(strUrlItem.GetLength() - strItemFind.GetLength());
				if(/*m_bIsShowLinkInformation*/true)
				{
					CString strHttpConnectionStore = WEBIM_URL;
					CString strHttpFileStore = WEBIM_LINKINFO_ITEM + strItemID;
					CString strItemResult = GetStoreInformation(strHttpConnectionStore, strHttpFileStore);

					Json::Reader jReader;
					Json::Value jvSellerInfo;

					std::string strItem = LPCSTR(strItemResult);
					jReader.parse(strItem,jvSellerInfo);
					std::string storeSuccess = jvSellerInfo["success"].asString();	

					if(storeSuccess == "true")
					{
						std::string strItemData = LPCSTR("data");
						Json::Value valueData = jvSellerInfo.get(strItemData, jvSellerInfo);

						std::string strProductInfo = LPCSTR("goodsAsking");
						Json::Value valueInfo = valueData.get(strProductInfo, valueData);

						double dPrice = valueInfo["salePrice"].asDouble();
						std::string itemTitle = valueInfo["title"].asString();
						std::string itemPic = valueInfo["pic01"].asString();

						CString strItemPrice = "";
						strItemPrice.Format("价格：￥%lg元", dPrice);
						CString strItemTitle = itemTitle.c_str();
						CString strItemPic = itemPic.c_str();

						int nFindStyle = strItemTitle.Find("</");
						if(nFindStyle != -1)
						{
							CString strDescLeft = strItemTitle.Left(nFindStyle);
							CString strDescRight = strDescLeft.Right(strDescLeft.GetLength() - strDescLeft.ReverseFind('>') - 1);
							strItemTitle = strDescRight;
						}
						string strGbkStoreDesc = UTF8_Convert(strItemTitle, FALSE);
						strItemTitle = strGbkStoreDesc.c_str();
						//strItemTitle = Utf82Char(strItemTitle);

						pEdit->ReplaceSel("\r", false);
						InsertLinkInfo(pTextServices, strUrlItem, strItemPic, strItemTitle, strItemPrice);
						if(text != "")
							pEdit->ReplaceSel("\r", false);
					}
				}
			}
		}
		
		// 将自定义表情替换为插入图片
		else if ( bFindImage )
		{
			pEdit->ReplaceSel(text.Left(iImageStart), false);

			CString strFullImageFilePathName ;

			CString strTemp = strImageFileName;
			strTemp.MakeLower();

			int id = -1;
			bool bGifFace=false;

			if( strTemp == "noimage" )//如果为noimage则显示叉叉图片
			{
				CResBitmap bmp; 
				bmp.LoadBitmap(IDB_NO_IMAGE);
				//InsertBitmap((HBITMAP)bmp.m_hObject);
				IRichEditOle *lpRichEditOle = RichEdit_GetOleInterface(pTextServices);// = GetIRichEditOle();
				CImageDataObject::InsertBitmap(lpRichEditOle,(HBITMAP)bmp.m_hObject,ODT_BITMAP);
			}
			else
			{
				if ( strTemp == "noimage.gif" )
				{
					strFullImageFilePathName = GetModulePath()+"Image\\"+strImageFileName;
				}
				else if ( IsGifEmotion(strTemp,id) )
				{
					bGifFace = true;
					strFullImageFilePathName = g_LocalData.GetGifFacePath(id);
				}
				else
				{
					strFullImageFilePathName = g_LocalData.GetImageFileCachePath() + strImageFileName;
				}
				if ( _access(strFullImageFilePathName,0)!=-1 )
				{
					if ( bGifFace )
					{
						Richedit_InsertEmotion(pTextServices, pEdit, id);
					}
					else
					{
						RichEdit_InsertImage(pEdit, (LPCTSTR)strFullImageFilePathName, -1, -1);
						//IRichEditOle *lpRichEditOle = RichEdit_GetOleInterface(pTextServices);// = GetIRichEditOle();
						//CImageDataObject::InsertBitmap(lpRichEditOle,(HBITMAP)bmp.m_hObject,ODT_BITMAP);
					}
				}
				else
				{
					if ( bWaitImageFile )
					{
						IMAGE_FILE_STRUCT *pStruct = new IMAGE_FILE_STRUCT();
						sprintf(pStruct->file_name,strImageFileName);
						std::string strFile = pStruct->file_name;
							
						// 判断是否为网络图片
						if(strFile[0] != '{')
						{
							pStruct->image_type = 2;
							CString strHttpImg = pStruct->file_name;
							if (strHttpImg.Find("http://") == -1)
							{
								strHttpImg = GetPicRandomUrl() + strHttpImg;
							}
							sprintf(pStruct->file_name,strHttpImg);
						}
						/*if(strFile.find("http") != string::npos)
						{
						pStruct->image_type = 2;
						}*/
						//InsertWaitIcon(pStruct);	
						InsertWaitIcon(pTextServices, pEdit, g_LocalData.GetProgramPath() + "\\Emotion\\waitImage.gif", pStruct);
						//m_vecImageFileStruct.push_back(pStruct);
						if(pStruct->image_type == 2)
						{
							// 下载图片
							g_webFileProcess.GetWebFile((DWORD)pStruct, pStruct->file_name, hWnd);
						}

						if ( pslWaitImageFile )
						{
							pslWaitImageFile->AddTail(strImageFileName);
						}
					}
					else
					{
						IMAGE_FILE_STRUCT *pStruct = new IMAGE_FILE_STRUCT();
						sprintf(pStruct->file_name,strImageFileName);
						string strFile = pStruct->file_name;
						
						if(strFile[0] != '{')
						//if(strFile.find("http") != string::npos)
						{
							pStruct->image_type = 2;
							CString strHttpImg = pStruct->file_name;
							if (strHttpImg.Find("http://") == -1)
							{
								strHttpImg = GetPicRandomUrl() + strHttpImg;
							}
							sprintf(pStruct->file_name,strHttpImg);

							InsertWaitIcon(pTextServices, pEdit, g_LocalData.GetProgramPath() + "\\Emotion\\waitImage.gif", pStruct);
							//m_vecImageFileStruct.push_back(pStruct);
							// 下载图片
							g_webFileProcess.GetWebFile((DWORD)pStruct, pStruct->file_name, hWnd);

							if ( pslWaitImageFile )
							{
								pslWaitImageFile->AddTail(strImageFileName);
							}
						}
						else
						{
							delete pStruct;
							pStruct = NULL;

							CResBitmap bmp; 
							bmp.LoadBitmap(IDB_NO_IMAGE);
							//InsertBitmap((HBITMAP)bmp.m_hObject);		
							IRichEditOle *ole = RichEdit_GetOleInterface(pTextServices);
							CImageDataObject::InsertBitmap(ole,(HBITMAP)bmp.GetSafeHandle(),ODT_BITMAP);
						}
					}
				}
			}

			text = text.Mid(iImageEnd);
		}
		// 将系统表情替换为插入表情
		else if ( bFindEmotion )
		{
			pEdit->ReplaceSel(text.Left(nEmotionBeg), false);
			//InsertEmotion(iEmotion);
			text = text.Mid(nEmotionEnd);
		}
		else
		{
			break;
		}		
	}
	pEdit->ReplaceSel(text, false);

	//pTextServices->TxSendMessage(EM_REPLACESEL, (WPARAM) false, (LPARAM)lpText, 0); 
}

void RichEdit_InsertFileInfoBar(ITextServices * pTextServices, DWORD dwFileID, bool bSendFile, CString strFileName, long nFileSize)
{
	SCODE			sc ;
	HRESULT			hr ;
	LPLOCKBYTES		lpLockBytes = NULL;
	LPOLECLIENTSITE lpClientSite ;
	IMiniFileInfoBarPtr	lpFileInfo;
	LPSTORAGE		lpStorage    ;
	LPOLEOBJECT		lpObject     ;

	DWORD			dwBarIndex = -1;

	sc = ::CreateILockBytesOnHGlobal(NULL, TRUE, &lpLockBytes);
	if (sc != S_OK)
		AfxThrowOleException(sc);
	ASSERT(lpLockBytes != NULL);

	sc = ::StgCreateDocfileOnILockBytes(lpLockBytes,
		STGM_SHARE_EXCLUSIVE|STGM_CREATE|STGM_READWRITE, 0, &lpStorage);
	if (sc != S_OK)
	{
		VERIFY(lpLockBytes->Release() == 0);
		lpLockBytes = NULL;
		AfxThrowOleException(sc);
	}
	ASSERT(lpStorage != NULL);
	IRichEditOle *lpRichEditOle = RichEdit_GetOleInterface(pTextServices);;// = GetIRichEditOle();
	lpRichEditOle->GetClientSite(&lpClientSite);
	ASSERT(lpClientSite != NULL);
	try
	{
		hr = lpFileInfo.CreateInstance(CLSID_CMiniFileInfoBar);
		if( FAILED(hr) )
			_com_issue_error(hr);

		// 获取被嵌入的对象
		hr = lpFileInfo.QueryInterface(IID_IOleObject, (void**)&lpObject);
		lpObject->SetClientSite(lpClientSite);
		if( FAILED(hr) )
			_com_issue_error(hr);

		lpFileInfo->put_OwnerData(dwFileID);

		// 通知一个对象被嵌入容器中
		hr = OleSetContainedObject(lpObject, TRUE);

		//2 insert in 2 richedit, you need a struct of REOBJECT
		REOBJECT reobject;
		ZeroMemory(&reobject, sizeof(REOBJECT));
		reobject.cbStruct = sizeof(REOBJECT);
		CLSID clsid;
		// 获取被嵌入对象的类id
		sc = lpObject->GetUserClassID(&clsid);
		if (sc != S_OK)
			AfxThrowOleException(sc);

		reobject.clsid	= clsid;
		// 能被选择
		reobject.cp		= REO_CP_SELECTION;
		// 内容
		reobject.dvaspect	= DVASPECT_CONTENT;
		// 和文字同一水平线
		reobject.dwFlags	= REO_BELOWBASELINE| REO_INPLACEACTIVE ;

		reobject.dwUser		= bSendFile? ODT_SEND_FILE : ODT_RECV_FILE;
		// 嵌入对象
		reobject.poleobj	= lpObject;
		// 
		reobject.polesite	= lpClientSite;
		// 存储数据
		reobject.pstg		= lpStorage;

		SIZEL sizel={0,0};
		reobject.sizel = sizel;

		lpRichEditOle->InsertObject(&reobject);
		lpFileInfo->SetFileInfo(strFileName.AllocSysString(),nFileSize);
		lpFileInfo->put_OwnerData(dwFileID);

		if(lpObject)
			lpObject->Release();	
		if(lpLockBytes)
			lpLockBytes->Release();
		if(lpClientSite)
			lpClientSite->Release();
		if(lpRichEditOle)
			lpRichEditOle->Release();
		if(lpStorage)
			lpStorage->Release();
		/*if(lpProgress)
		lpProgress->Release();*/

	}
	catch(...)
	{
		TRACE("CRichEditCtrlEx插入文件出现异常!\n");
	}


//	BSTR bstrFileName = NULL;
//	IRichEditOle * pRichEditOle = NULL;
//	IOleClientSite *pOleClientSite = NULL;
//	IImageOle* pImageOle = NULL;
//	IOleObject *pOleObject = NULL;
//	REOBJECT reobject = {0};
//	HRESULT hr = E_FAIL;
//
//	if (NULL == pTextServices || NULL == pTextHost ||
//		NULL == lpszFileName || NULL == *lpszFileName)
//		return FALSE;
//
//	TCHAR cProtocol[16] = {0};
//	_tcsncpy(cProtocol, lpszFileName, 7);
//	if ((_tcsicmp(cProtocol, _T("http://")) != 0) && ::GetFileAttributes(lpszFileName) == 0xFFFFFFFF)
//		return FALSE;
//	//lpszFileName = _T("E:\\1.png");
//	CString str = lpszFileName;
//
//	bstrFileName = str.AllocSysString();
//	//bstrFileName = ::SysAllocString((OLECHAR*)lpszFileName);
//	//bstrFileName = ::SysAllocString();
//	if (NULL == bstrFileName)
//		return FALSE;
//
//	pTextServices->TxSendMessage(EM_GETOLEINTERFACE, 0, (LPARAM)&pRichEditOle, NULL);
//	if (NULL == pRichEditOle)
//		goto Ret0;
//
//	hr = ::CoCreateInstance(CLSID_ImageOle, NULL, 
//		CLSCTX_INPROC_SERVER, IID_IImageOle, (void**)&pImageOle);
//	if (FAILED(hr))
//		goto Ret0;
//
//	hr = pImageOle->QueryInterface(IID_IOleObject, (void **)&pOleObject);
//	if (FAILED(hr))
//		goto Ret0;
//
//	pRichEditOle->GetClientSite(&pOleClientSite);
//	if (NULL == pOleClientSite)
//		goto Ret0;
//
//	pOleObject->SetClientSite(pOleClientSite);
//
//	pImageOle->SetTextServices(pTextServices);
//	pImageOle->SetTextHost(pTextHost);
//	pImageOle->SetFaceId(nFaceId);
//	pImageOle->SetFaceIndex(nFaceIndex);
//	pImageOle->SetBgColor(clrBg);
//	pImageOle->SetAutoScale(bAutoScale, nReservedWidth);
//	pImageOle->LoadFromFile(bstrFileName);
//
//	hr = ::OleSetContainedObject(pOleObject, TRUE);
//
//	reobject.cbStruct = sizeof(REOBJECT);
//	reobject.clsid    = CLSID_ImageOle;
//	reobject.cp       = REO_CP_SELECTION;
//	reobject.dvaspect = DVASPECT_CONTENT;
//	reobject.dwFlags  = REO_BELOWBASELINE;
//	reobject.poleobj  = pOleObject;
//	reobject.polesite = pOleClientSite;
//	reobject.dwUser   = 0;
//
//	hr = pRichEditOle->InsertObject(&reobject);
//
//Ret0:
//	if (pOleObject != NULL)
//		pOleObject->Release();
//
//	if (pImageOle != NULL)
//		pImageOle->Release();
//
//	if (pOleClientSite != NULL)
//		pOleClientSite->Release();
//
//	if (pRichEditOle != NULL)
//		pRichEditOle->Release();
//
//	if (bstrFileName != NULL)
//		::SysFreeString(bstrFileName);
//
//	return SUCCEEDED(hr);
}

IMiniFileInfoBar* RichEdit_FindNearestFileInfoBar(ITextServices * pTextServices, int pos, OLE_DATA_TYPE &odtType , int & nBarPos )
{
	IRichEditOle *ole = NULL;
	pTextServices->TxSendMessage(EM_GETOLEINTERFACE, 0, (LPARAM)&ole, NULL);
	int n = ole->GetObjectCount();

	REOBJECT reo;
	reo.cbStruct = sizeof(reo);

	for (int i = n-1; i >=0; --i)
	{
		ole->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES);

		if ( reo.cp<pos && (reo.dwUser==ODT_RECV_FILE || reo.dwUser==ODT_SEND_FILE ) )
		{
			IMiniFileInfoBar *pImage = NULL;
			reo.poleobj->QueryInterface(IID_IMiniFileInfoBar,(void **)&pImage);	
			odtType = (OLE_DATA_TYPE)reo.dwUser;

			nBarPos = reo.cp;
			return pImage;
		}
	}

	return NULL;
}

void RichEdit_SetFileBarTip(DuiLib::CRichEditUI* pEdit, ITextServices * pTextServices, DWORD dwFileID, OLE_DATA_TYPE odtType, CString strTip, COLORREF clrTip)
{
	IRichEditOle *ole = NULL;
	pTextServices->TxSendMessage(EM_GETOLEINTERFACE, 0, (LPARAM)&ole, NULL);

	int n = ole->GetObjectCount();

	REOBJECT reo;
	reo.cbStruct = sizeof(reo);

	for (int i = 0; i <n; ++i)
	{
		ole->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES);

		if ( reo.dwUser==odtType )
		{
			IMiniFileInfoBar *pImage = NULL;
			reo.poleobj->QueryInterface(IID_IMiniFileInfoBar,(void **)&pImage);	
			odtType = (OLE_DATA_TYPE)reo.dwUser;

			if ( pImage && pImage->OwnerData==dwFileID )
			{
				CHARRANGE	crSave;
				CString		strSel;

				pTextServices->TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&crSave, NULL);
				RichEdit_SetSel(pTextServices, reo.cp+1,-1);

				/*CHARRANGE cr;
				cr.cpMin = cr.cpMax = 0;
				pTextServices->TxSendMessage(EM_EXGETSEL, 0, (LPARAM)&cr, 0);
				LPWSTR lpText = NULL;
				lpText = new WCHAR[cr.cpMax - cr.cpMin + 1];
				::ZeroMemory(lpText, (cr.cpMax - cr.cpMin + 1) * sizeof(WCHAR));

				tstring str;
				pTextServices->TxSendMessage(EM_GETSELTEXT, 0, (LPARAM)lpText, 0);
				strSel=lpText;*/
				strSel = pEdit->GetSelText();
				pTextServices->TxSendMessage(EM_EXSETSEL, 0, (LPARAM)&crSave, NULL);
				int pos1 = strSel.Find("\r",0);
				int pos2 = strSel.Find("\r",pos1+1);

				CString strSub = strSel.Mid(pos1+1,pos2-pos1-1);

				USES_CONVERSION;
				int nLen =wcslen(A2W((LPCSTR)strSub));
				RichEdit_SetSel(pTextServices, reo.cp+1+pos1+1,/*reo.cp+2+nLen*/-1);
				//ReplaceSel("");

				CResBitmap bmpLine;
				bmpLine.LoadBitmap(IDB_HINT_FLAG2);
				CImageDataObject::InsertBitmap(ole,(HBITMAP)bmpLine.GetSafeHandle(),ODT_BITMAP);

				RichEdit_SetStartIndent(pTextServices, 230);
				XT_FONT xf;

				xf.fontColor = clrTip;//RGB(233,222,255);

				CString strInsertTip = " "+strTip;

				pEdit->InsertText(-1, strInsertTip);

				nLen = wcslen(A2W((LPCSTR)strInsertTip));

				RichEdit_SetSel(pTextServices, reo.cp+1+pos1+1,/*reo.cp+3+nLen*/-1);
				RichEdit_SetFont(pTextServices, xf.fontName, xf.fontSize, xf.fontColor, xf.flags & XTF_BOLD, xf.flags & XTF_ITALIC, xf.flags & XTF_UNDERLINE, false);

				RichEdit_SetSel(pTextServices, -1,-1);

				pEdit->EndDown();
			}
		}
	}
}

void Richedit_InsertEmotion(ITextServices* pTextServices, DuiLib::CRichEditUI* pEdit, int nEmotion)
{
	CString strGif = g_LocalData.GetGifFacePath(nEmotion);

	if ( FileExist(strGif)==FALSE )
	{
		return ;
	}
	RichEdit_InsertImage(pEdit, strGif, -1, -1);
}

// 替换接收到的图片
void Richedit_ReplaceRecvImg(ITextServices* pTextServices, DuiLib::CRichEditUI* pEdit, IMAGE_FILE_STRUCT *pStruct, const char * file_name)
{
	IRichEditOle *ole = RichEdit_GetOleInterface(pTextServices);
	int n = ole->GetObjectCount();
	REOBJECT reo;
	reo.cbStruct = sizeof(reo);
	CString strNewFile = "";
	if(file_name != NULL && strcmp(file_name, "") != 0)
	{
		strNewFile = g_LocalData.GetImageFileCachePath() + GetFileNameFromPath(file_name);
		CopyFile(file_name, strNewFile, FALSE);
	}

	for (int i = 0; i < n; ++i)
	{
		ole->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES);

		if ( reo.dwUser == ODT_WAITICON )
		{
#ifndef _XY_PICTURE
			IShowImage *pImage = NULL;	
			reo.poleobj->QueryInterface(IID_IShowImage,(void **)&pImage);
#else
			IXPicture * pImage = NULL;
			reo.poleobj->QueryInterface(IID_IXPicture,(void **)&pImage);			
#endif

			//新的图片接口
			CComPtr<COleImage> spOleImage;
			reo.poleobj->QueryInterface(IID_IOleImage, (void **)&spOleImage);

			if(spOleImage)
			{
				IMAGE_FILE_STRUCT *pImage = (IMAGE_FILE_STRUCT *)(spOleImage->m_lOwnData);
				if ( pStruct == pImage )
				{
					CHARRANGE cr;
					pEdit->GetSel(cr);
					pEdit->SetSel(reo.cp,reo.cp+1);
					pEdit->ReplaceSel("", false);

					if(strNewFile != "")
						RichEdit_InsertImage(pEdit, strNewFile, -1, -1);
					else
					{	// 图片下载失败时，插入裂图
						CResBitmap bmp; 
						bmp.LoadBitmap(IDB_NO_IMAGE);
						CImageDataObject::InsertBitmap(ole,(HBITMAP)bmp.m_hObject,ODT_BITMAP);
					}					

					pEdit->SetSel(cr);

					pEdit->EndDown();
				}
				//是否需要删除?
				//delete spOleImage;
				//spOleImage = NULL;

			}

			if ( pImage )
			{
				DWORD dwOwnData;
#ifndef _XY_PICTURE
				dwOwnData = (LONG)pImage->GetOwnData();
#else
				pImage->GetUserData((LONG*)&dwOwnData);
#endif
				if (dwOwnData!=0 )
				{
					IMAGE_FILE_STRUCT *pImage = (IMAGE_FILE_STRUCT *)dwOwnData;

					if ( strcmp(pImage->file_name,file_name)==0 )
					{
						CHARRANGE cr;
						pEdit->GetSel(cr);
						pEdit->SetSel(reo.cp,reo.cp+1);
						pEdit->ReplaceSel("", false);
						RichEdit_InsertImage(pEdit, strNewFile, -1, -1);

						pEdit->SetSel(cr);

						pEdit->EndDown();
					}
				}				
			}	
		}
	}
}

void Richedit_UpdateGif(DuiLib::CRichEditUI* pEdit, ITextServices* pTextServices)
{

	REOBJECT reobject;
	LONG nFaceId, nPos = 0;
	tstring strOrgText, strTemp;

	IRichEditOle * pRichEditOle = RichEdit_GetOleInterface(pTextServices);
	if (NULL == pRichEditOle)
		return;

	for (LONG i = 0; i < pRichEditOle->GetObjectCount(); i++)
	{
		memset(&reobject, 0, sizeof(REOBJECT));
		reobject.cbStruct = sizeof(REOBJECT);
		reobject.cp = i;
		HRESULT hr = pRichEditOle->GetObject(i, &reobject, REO_GETOBJ_POLEOBJ);

		if (SUCCEEDED(hr))
		{
			//if (reobject.cp > 0 && reobject.cp > nPos)
			//{
			//	strTemp = strOrgText.substr(nPos, reobject.cp-nPos);
			//	//Replace(strTemp, _T("/"), _T("//"));
			//	strText += strTemp;
			//}
			nPos = reobject.cp + 1;

			if (NULL == reobject.poleobj)
				continue;

			if (CLSID_OleImage == reobject.clsid)
			{
				COleImage * pImageOle = NULL;
				hr = reobject.poleobj->QueryInterface(IID_IOleImage, (void**)&pImageOle);
				pImageOle->ChangeFrame();  
			}
		}
	}
}

size_t len_wchar2ansi(LPCTSTR pszText, size_t pnwcharLen)
{
	if (pnwcharLen == 0) 
	{
		return 0;
	}

	char* szText = 0;
	wchar_t* wszText = new wchar_t[pnwcharLen + 1];
	int nszLen;

	MultiByteToWideChar(CP_ACP, 0, pszText, -1, wszText, pnwcharLen);
	wszText[pnwcharLen] = 0;

	nszLen = WideCharToMultiByte(CP_ACP, 0, wszText, -1, szText, 0, NULL, NULL);
	szText = new char[nszLen + 1];
	WideCharToMultiByte(CP_ACP, 0, wszText, -1, szText, nszLen + 1, NULL, NULL);

	size_t nLen = strlen(szText);

	delete[] szText;
	delete[] wszText;

	return nLen;
}

CString Richedit_GetSelText(DuiLib::CRichEditUI* pEdit)
{
	ITextServices * pTextServices = pEdit->GetTextServices();
	CString str = pEdit->GetSelText();

	CString text ;//return value

	CHARRANGE cr;
	pEdit->GetSel(cr);

	IRichEditOle *ole = RichEdit_GetOleInterface(pTextServices);
	int n = ole->GetObjectCount();
	REOBJECT reo;
	reo.cbStruct = sizeof(reo);

	CString str2;
	str2 = str;
	int postext = 0, posObjects = 0;


	for (int i = 0; i < n; ++i)
	{
		ole->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES);
		//	text += str.Mid(postext,reo.cp-posObjects);

		if ( reo.cp<cr.cpMin || reo.cp>cr.cpMax )
		{
			continue;
		}

		if(reo.dwUser == ODT_LINK_INFO)
		{
			//bIsOleLinkInfo = TRUE;
			return "";
		}
		text += str.Mid(postext, len_wchar2ansi(str2, reo.cp - posObjects - cr.cpMin));

		//USES_CONVERSION;
// 		if ( reo.dwUser>=0 && reo.dwUser<sizeof(emotions)/sizeof(LPCTSTR) )
// 		{
// 			text += emotions[reo.dwUser];
// 		}
// 		else
// 		{
//#ifndef _XY_PICTURE
			IShowImage *pImage = NULL;		
			reo.poleobj->QueryInterface(IID_IShowImage,(void **)&pImage);
//#else
//			IXPicture * pImage = NULL;
//			reo.poleobj->QueryInterface(IID_IXPicture,(void **)&pImage);
//#endif

			CComPtr<COleImage> spOleImage;
			reo.poleobj->QueryInterface(IID_IOleImage, (void **)&spOleImage);


			CString strImageFile; 
			if(spOleImage)
			{
				strImageFile =	spOleImage->m_strImageName;
			}

			if( pImage)
			{
#ifndef _XY_PICTURE
				strImageFile.Format(CString((LPCSTR)pImage->GetImage()));  
				//strImageFile = W2A(pImage->GetImage());
#else
				BSTR bs;
				pImage->GetFileName(&bs);
				strImageFile = W2A(bs);
#endif
			}

			if ( !strImageFile.IsEmpty() ) 
			{
				CString strTemp;

				if (reo.dwUser == ODT_GIF )
				{
					IMAGE_FILE_STRUCT image;

					CString strPath,strFileName,strExt;
					ExtractFilePath(strImageFile,strPath,strFileName,strExt);

					image.file_session_id   = CRichEditCtrlEx::m_dwFileSessionId  ;
					image.file_size         = GetFileSize(strImageFile);

					sprintf(image.file_name, "%s%s", strFileName, strExt );      ;

					strTemp.Format("<image:%s%s>",strFileName,strExt);	
					//strTemp.Format("<image:%s>",strImageFile);
				}

				text += strTemp;
			}		

		//}

		posObjects = reo.cp+1-cr.cpMin;
		postext = len_wchar2ansi(str, reo.cp-cr.cpMin)+1;

		str2=str.Mid(postext);
		break;
	}

	text += str.Mid(postext);

	return text;
}

CString Richedit_GetLinkInfo(DuiLib::CRichEditUI* pEdit, ITextServices* pTextServices, int nX, int nY)
{
	
	IRichEditOle *ole = NULL;
	pTextServices->TxSendMessage(EM_GETOLEINTERFACE, 0, (LPARAM)&ole, NULL);
	int n = ole->GetObjectCount();

	REOBJECT reo;
	reo.cbStruct = sizeof(reo);

	for (int i = n-1; i >=0; --i)
	{
		ole->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES);

		if(reo.dwUser == ODT_LINK_INFO)
		{
			_DOleLinkInfo* iut = NULL;
			reo.poleobj->QueryInterface(DIID__DOleLinkInfo,(void **)&iut);

			CPoint pt = pEdit->GetCharPos( reo.cp );
			long x = nX;
			long y = nY;

			int nResult = iut->IsPointInLinkRect(pt.x, pt.y, x, y);
			if(nResult == 1)
			{
				//hCursor = LoadCursor(NULL,IDC_HAND);
				//if(hCursor)
				//{
				_bstr_t url = iut->GetUrlText();
				//m_strLinkInfoUrl = (LPCSTR)url;
				//SysFreeString(url);
				//m_bIsPointInLinkRect = TRUE;
				//SetCursor(hCursor);
				if( iut != NULL )	iut->Release();
				if( reo.poleobj     != NULL )  reo.poleobj->Release();
				if( reo.polesite     != NULL )  reo.polesite->Release();
				if( reo.pstg     != NULL )  reo.pstg->Release();
				return (LPCSTR)url;
				//}
			}
			if( iut     != NULL )	iut->Release();
			if( reo.poleobj     != NULL )  reo.poleobj->Release();
			if( reo.polesite     != NULL )  reo.polesite->Release();
			if( reo.pstg     != NULL )  reo.pstg->Release();
		}
	}
	return "";
//	IRichEditOle *ole = RichEdit_GetOleInterface(pTextServices);
//	int nOleCount = ole->GetObjectCount();
//
//	REOBJECT reo;
//	reo.cbStruct = sizeof(reo);
//
//	for(int nOle = 0; nOle < nOleCount; nOle++)
//	{
//		ole->GetObject(nOle, &reo, REO_GETOBJ_ALL_INTERFACES);
//
//		
//		if(reo.dwUser == ODT_LINK_INFO)
//		{
//			CPoint pt = pEdit->GetCharPos( reo.cp );
//			if(pt.x <= 0 && pt.y <= 0)
//				continue;
//			long x = nX;
//			long y = nY;
//			_DOleLinkInfo* iut = NULL;
//			reo.poleobj->QueryInterface(DIID__DOleLinkInfo,(void **)&iut);
//			return "";
//			int nResult = iut->IsPointInLinkRect(pt.x, pt.y, x, y);
//			if(nResult == 1)
//			{
//				//hCursor = LoadCursor(NULL,IDC_HAND);
//				//if(hCursor)
//				//{
//					_bstr_t url = iut->GetUrlText();
//					//m_strLinkInfoUrl = (LPCSTR)url;
//					//SysFreeString(url);
//					//m_bIsPointInLinkRect = TRUE;
//					//SetCursor(hCursor);
//// 					if( iut != NULL )	iut->Release();
//// 					if( reo.poleobj     != NULL )  reo.poleobj->Release();
//// 					if( reo.polesite     != NULL )  reo.polesite->Release();
//// 					if( reo.pstg     != NULL )  reo.pstg->Release();
//					return url;
//				//}
//			}
//// 			if( iut     != NULL )	iut->Release();
//// 			if( reo.poleobj     != NULL )  reo.poleobj->Release();
//// 			if( reo.polesite     != NULL )  reo.polesite->Release();
//// 			if( reo.pstg     != NULL )  reo.pstg->Release();
//		}
//	}
}

CString Richedit_GetSelImage(DuiLib::CRichEditUI* pEdit)
{
	ITextServices * pTextServices = pEdit->GetTextServices();
	CString str = pEdit->GetSelText();

	CString text ;//return value

	CHARRANGE cr;
	pEdit->GetSel(cr);

	IRichEditOle *ole = RichEdit_GetOleInterface(pTextServices);
	int n = ole->GetObjectCount();
	REOBJECT reo;
	reo.cbStruct = sizeof(reo);

	CString str2;
	str2 = str;
	int postext = 0, posObjects = 0;


	for (int i = 0; i < n; ++i)
	{
		ole->GetObject(i, &reo, REO_GETOBJ_ALL_INTERFACES);
		//	text += str.Mid(postext,reo.cp-posObjects);

		if ( reo.cp<cr.cpMin || reo.cp>cr.cpMax )
		{
			continue;
		}

		if(reo.dwUser == ODT_LINK_INFO)
		{
			//bIsOleLinkInfo = TRUE;
			return "";
		}
		text += str.Mid(postext, len_wchar2ansi(str2, reo.cp - posObjects - cr.cpMin));

		//USES_CONVERSION;
		// 		if ( reo.dwUser>=0 && reo.dwUser<sizeof(emotions)/sizeof(LPCTSTR) )
		// 		{
		// 			text += emotions[reo.dwUser];
		// 		}
		// 		else
		// 		{
		//#ifndef _XY_PICTURE
		IShowImage *pImage = NULL;		
		reo.poleobj->QueryInterface(IID_IShowImage,(void **)&pImage);
		//#else
		//			IXPicture * pImage = NULL;
		//			reo.poleobj->QueryInterface(IID_IXPicture,(void **)&pImage);
		//#endif

		CComPtr<COleImage> spOleImage;
		reo.poleobj->QueryInterface(IID_IOleImage, (void **)&spOleImage);


		CString strImageFile; 
		if(spOleImage)
		{
			strImageFile =	spOleImage->m_strImageName;
		}
		return strImageFile;
	}
	return "";
}

void Richedit_InsertBitmap(DuiLib::CRichEditUI* pEdit, HBITMAP hBitmap)
{
	try
	{
		ITextServices * pTextServices = pEdit->GetTextServices();
		IRichEditOle *lpRichEditOle = RichEdit_GetOleInterface(pTextServices);
		CImageDataObject::InsertBitmap(lpRichEditOle,
			hBitmap,ODT_BITMAP);
	}
	catch(...)
	{
		TRACE("\ninsertbitmap error\n");
	}
}