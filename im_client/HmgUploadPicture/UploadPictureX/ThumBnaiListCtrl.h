#pragma once

#include "PubHeader.h"
#include "ThumBnaiListCtrl.h"

typedef	enum 
{
	EM_ROTATE_LEFT = 0,
	EM_ROTATE_RIGHT
}_EM_ROTATE;

//	#define USE_LISTCTRL_GETDISPINFO

//	#define _USE_CUSTOME_DRAWITEM_

#include "InPlaceTextTip.h"

#define USE_INPLACE_TEXTTIP_CTRL 1	

#include <afxcmn.h>
// CThumBnaiListCtrl
class CThumBnaiListCtrl : public CListCtrl
{ 
	DECLARE_DYNAMIC(CThumBnaiListCtrl)

public:
	CThumBnaiListCtrl();
	virtual ~CThumBnaiListCtrl();
public:
	FileInfoList  m_FileList;
protected:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()
protected:
	void DrawTool(int nItem, CPoint point);
	void Rotate(const int nIndex, const _EM_ROTATE em);
public:
	void Initilization();
	void Clear();
	bool LoadFolder(const CString & strFolder);
	void AddFileList(FileInfoList * pFileList, const int nMaxSideSize = 0);
	void AddFile(FILEINFO fileInfo);

	bool MatchExtension(CString file);
	BOOL DeleteItem(int nItem);
	//验证图片文件
	bool IsImageGDIPLUSValid(CString filepath);
	//获取文件CLSID，用以保存旋转后的图片
	int GetEncoderClsid(const WCHAR *format, CLSID *pClsid);

	//获取文件信息
	FILEINFO * GetFileData(DWORD dwFileID);
	//绘制已添加图标
	void DrawFileFlag(const int nItem, BYTE bFlag);
	//移除已添加图标
	//void RemoveSelFlag(DWORD dwFileID);
private:
	int SetFileFlag(const int nItemIndex, const BYTE bFlag/*0 normal, 1 sel, 2 uploaded*/);
public:
	int SetAllFileFlag(const BYTE bFlag);
	int	SetFileFlagEx(const DWORD dwFileID, const BYTE bFlag);
	int SetFileFlagEx(FILEINFO *_pFileInfo, const BYTE bFlag);
	int  FindFileIndex(DWORD dwFileId);
private:
	void StartLoadThread(void);
	//生成缩略图线程
	static UINT RunLoadThumbnailThread( LPVOID lpParam );
public:
	//结束线程
	void TerminateLoadThread(void);
private:
	HICON			m_hSelIcon, m_hUpdIcon	;

	BOOL			m_bRedraw			;

	bool			m_bRunning		;

	HANDLE			m_hThreadLoad	;

	bool			m_bMouseOver	;
	
	int				m_nPreHoverItem ;

	int             m_nHoverItem    ;

	bool            m_bStopFlag     ;

	bool            m_bAppendFlag   ;

	FileInfoList    m_AddFileList   ;

	//缩略图列表
	CImageList		m_ImageListThumb	;

	CBitmap			m_LeftBitmap, m_LeftBitmap2, m_RightBitmap, m_RightBitmap2, m_BigBitmap, m_BigBitmap2;
public:
	BOOL	m_bDraging;
	BOOL	PreCreateWindow(CREATESTRUCT& cs);
protected:
	int m_nFlags;	// 为0 上面的这个listctrl, 1则是添加到下面listctrl，也就是需要上传的
	CToolTipCtrl m_wndToolTipCtrl;
protected:
	CInPlaceTextTip	m_wndToolTipCtrl2;

	void ClickedInListCtrl();
	LRESULT  OnMsgOfListCtrlTextTipLBUTTONDOWN(WPARAM wParam, LPARAM lParam);
public:
	void ShowToolTipText(const CString & strToolTipText, const UINT nFlags, const CRect &rcTitle = CRect(0,0,0,0));
protected:
	void OnListCtrlGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult);
private:
	int m_nToopTipTextFlags;	// 0 为图片提示 1 为文字处的提示, 2 向左, 3 向右, 4 放大
public:
	void RunLoadThumbnailThread_run();
	void SelectItem(const int nItem);
	void SelectItem(FileInfoList & _lstFileInfo);
	void DeselectItem(const int nItem = -1);
private:
	void OnListCtrlGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult);
#ifdef _USE_CUSTOME_DRAWITEM_
public:
	void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
;
#endif // _USE_CUSTOME_DRAWITEM_
public:
	CWnd *m_pParentWnd;
	void RedrawPics(const int nMaxSideSize);
	BOOL PreTranslateMessage(MSG* pMsg);
	int GetCurSel(void);

private:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};

