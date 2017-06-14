#pragma once

#include <string>
#include <richedit.h>
#include <richole.h>
#include <textserv.h>
#include "Utils.h"
//#include "..\IImageOle.h"
#include "richeditctrlex.h"
#include "ImageDataObject.h"

#if defined(UNICODE) || defined(_UNICODE)
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif

IRichEditOle* RichEdit_GetOleInterface(HWND hWnd);
int RichEdit_GetWindowTextLength(HWND hWnd);
int RichEdit_GetWindowText(HWND hWnd, LPTSTR lpszStringBuf, int nMaxCount);
int RichEdit_GetWindowText(HWND hWnd, tstring& strText);
int RichEdit_GetTextRange(HWND hWnd, CHARRANGE * lpchrg, tstring& strText);
DWORD RichEdit_GetDefaultCharFormat(HWND hWnd, CHARFORMAT& cf);
BOOL RichEdit_SetDefaultCharFormat(HWND hWnd, CHARFORMAT& cf);
DWORD RichEdit_GetSelectionCharFormat(HWND hWnd, CHARFORMAT& cf);
BOOL RichEdit_SetSelectionCharFormat(HWND hWnd, CHARFORMAT& cf);
void RichEdit_ReplaceSel(HWND hWnd, LPCTSTR lpszNewText, BOOL bCanUndo = FALSE);
void RichEdit_GetSel(HWND hWnd, LONG& nStartChar, LONG& nEndChar);
int RichEdit_SetSel(HWND hWnd, LONG nStartChar, LONG nEndChar);

void RichEdit_SetDefFont(HWND hWnd, LPCTSTR lpFontName, int nFontSize,	// ����Ĭ������
						 COLORREF clrText, BOOL bBold, BOOL bItalic, 
						 BOOL bUnderLine, BOOL bIsLink);
void RichEdit_SetDefFont(HWND hWnd, LPCTSTR lpFontName, int nFontSize);	// ����Ĭ���������ƺʹ�С
void RichEdit_SetDefTextColor(HWND hWnd, COLORREF clrText);				// ����Ĭ��������ɫ
void RichEdit_SetDefLinkText(HWND hWnd, BOOL bEnable);					// ����Ĭ�ϳ�����
void RichEdit_SetFont(HWND hWnd, LPCTSTR lpFontName, int nFontSize,		// ��������
					  COLORREF clrText, BOOL bBold, BOOL bItalic, 
					  BOOL bUnderLine, BOOL bIsLink);	
void RichEdit_SetFont(HWND hWnd, LPCTSTR lpFontName, int nFontSize);	// �����������ƺʹ�С
void RichEdit_SetTextColor(HWND hWnd, COLORREF clrText);				// ����������ɫ
void RichEdit_SetLinkText(HWND hWnd, BOOL bEnable);						// ���ó�����
BOOL RichEdit_SetStartIndent(HWND hWnd, int nSize);						// ����������(��λ:�)
BOOL RichEdit_InsertFace(HWND hWnd, LPCTSTR lpszFileName, int nFaceId,	// �������ͼƬ
						 int nFaceIndex, COLORREF clrBg, BOOL bAutoScale, int nReservedWidth);
void RichEdit_GetText(HWND hWnd, tstring& strText);						// ��ȡ�ı�
void RichEdit_ReplaceSel(HWND hWnd, LPCTSTR lpszNewText, LPCTSTR lpFontName,	// �滻ѡ���ı�
						 int nFontSize,	COLORREF clrText, BOOL bBold, BOOL bItalic, 
						 BOOL bUnderLine, BOOL bIsLink, int nStartIndent, BOOL bCanUndo = FALSE);
//BOOL RichEdit_GetImageOle(HWND hWnd, POINT pt, IImageOle** pImageOle);


IRichEditOle* RichEdit_GetOleInterface(ITextServices * pTextServices);
int RichEdit_GetWindowTextLength(ITextServices * pTextServices);
int RichEdit_GetWindowText(ITextServices * pTextServices, LPTSTR lpszStringBuf, int nMaxCount);
int RichEdit_GetWindowText(ITextServices * pTextServices, tstring& strText);
int RichEdit_GetTextRange(ITextServices * pTextServices, CHARRANGE * lpchrg, tstring& strText);
DWORD RichEdit_GetDefaultCharFormat(ITextServices * pTextServices, CHARFORMAT& cf);
BOOL RichEdit_SetDefaultCharFormat(ITextServices * pTextServices, CHARFORMAT& cf);
DWORD RichEdit_GetSelectionCharFormat(ITextServices * pTextServices, CHARFORMAT& cf);
BOOL RichEdit_SetSelectionCharFormat(ITextServices * pTextServices, CHARFORMAT& cf);
void RichEdit_ReplaceSel(ITextServices * pTextServices, LPCTSTR lpszNewText, BOOL bCanUndo = FALSE);
void RichEdit_GetSel(ITextServices * pTextServices, LONG& nStartChar, LONG& nEndChar);
int RichEdit_SetSel(ITextServices * pTextServices, LONG nStartChar, LONG nEndChar);

void RichEdit_SetDefFont(ITextServices * pTextServices, LPCTSTR lpFontName,		// ����Ĭ������
						 int nFontSize,	COLORREF clrText, BOOL bBold, 
						 BOOL bItalic, BOOL bUnderLine, BOOL bIsLink);
void RichEdit_SetFont(ITextServices * pTextServices, LPCTSTR lpFontName, int nFontSize,	// ��������
					  COLORREF clrText, BOOL bBold, BOOL bItalic, BOOL bUnderLine, BOOL bIsLink);
BOOL RichEdit_SetStartIndent(ITextServices * pTextServices, int nSize);			// ����������(��λ:�)
BOOL RichEdit_InsertFace(ITextServices *pTextServices, DuiLib::CRichEditUI * pEdit, ITextHost *pTextHost,	// �������ͼƬ
						 LPCTSTR lpszFileName, int nFaceId,	int nFaceIndex, 
						 COLORREF clrBg, BOOL bAutoScale, int nReservedWidth);
void RichEdit_GetText(ITextServices * pTextServices, tstring& strText, IMAGE_FILE_LIST * ifl);			// ��ȡ�ı�
void RichEdit_ReplaceSel(ITextServices * pTextServices, LPCTSTR lpszNewText,	// �滻ѡ���ı�
						 LPCTSTR lpFontName, int nFontSize,	COLORREF clrText, 
						 BOOL bBold, BOOL bItalic, BOOL bUnderLine, BOOL bIsLink, 
						 int nStartIndent, BOOL bCanUndo = FALSE);
//BOOL RichEdit_GetImageOle(ITextServices * pTextServices, POINT pt, IImageOle** pImageOle);
void RichEdit_InsertText(ITextServices * pTextServices, LPCTSTR lpText, DuiLib::CRichEditUI* pEdit, bool bWaitImageFile = false, CStringList *pslWaitImageFile = NULL, HWND hWnd = NULL, bool bIsReditSend = false);
void RichEdit_InsertFileInfoBar(ITextServices * pTextServices, DWORD dwFileID, bool bSendFile, CString strFileName, long nFileSize);
IMiniFileInfoBar* RichEdit_FindNearestFileInfoBar(ITextServices * pTextServices, int pos, OLE_DATA_TYPE &odtType , int & nBarPos );
void RichEdit_SetFileBarTip(DuiLib::CRichEditUI* pEdit, ITextServices * pTextServices, DWORD dwFileID, OLE_DATA_TYPE odtType, CString strTip, COLORREF clrTip=RGB(133,122,255));

void RichEdit_InsertLine(ITextServices * pTextServices);
void Richedit_InsertEmotion(ITextServices* pTextServices, DuiLib::CRichEditUI* pEdit, int nEmotion);
void Richedit_ReplaceRecvImg(ITextServices* pTextServices, DuiLib::CRichEditUI* pEdit, IMAGE_FILE_STRUCT *pStruct, const char * file_name);
BOOL RichEdit_InsertImage(DuiLib::CRichEditUI * pEdit, LPCTSTR lpszFileName, int nFaceId, int nFaceIndex);
void Richedit_UpdateGif(DuiLib::CRichEditUI* pEdit, ITextServices* pTextServices);
CString Richedit_GetSelText(DuiLib::CRichEditUI* pEdit);
CString Richedit_GetLinkInfo(DuiLib::CRichEditUI* pEdit, ITextServices* pTextServices, int nX, int nY);
CString Richedit_GetSelImage(DuiLib::CRichEditUI* pEdit);
void Richedit_InsertBitmap(DuiLib::CRichEditUI* pEdit, HBITMAP hBitmap);