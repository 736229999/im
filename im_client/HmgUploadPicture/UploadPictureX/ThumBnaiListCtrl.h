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
	//��֤ͼƬ�ļ�
	bool IsImageGDIPLUSValid(CString filepath);
	//��ȡ�ļ�CLSID�����Ա�����ת���ͼƬ
	int GetEncoderClsid(const WCHAR *format, CLSID *pClsid);

	//��ȡ�ļ���Ϣ
	FILEINFO * GetFileData(DWORD dwFileID);
	//���������ͼ��
	void DrawFileFlag(const int nItem, BYTE bFlag);
	//�Ƴ������ͼ��
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
	//��������ͼ�߳�
	static UINT RunLoadThumbnailThread( LPVOID lpParam );
public:
	//�����߳�
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

	//����ͼ�б�
	CImageList		m_ImageListThumb	;

	CBitmap			m_LeftBitmap, m_LeftBitmap2, m_RightBitmap, m_RightBitmap2, m_BigBitmap, m_BigBitmap2;
public:
	BOOL	m_bDraging;
	BOOL	PreCreateWindow(CREATESTRUCT& cs);
protected:
	int m_nFlags;	// Ϊ0 ��������listctrl, 1������ӵ�����listctrl��Ҳ������Ҫ�ϴ���
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
	int m_nToopTipTextFlags;	// 0 ΪͼƬ��ʾ 1 Ϊ���ִ�����ʾ, 2 ����, 3 ����, 4 �Ŵ�
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

