/*====================================================================  
=  �ļ���:CPictureCtrl��                                    
=  �޸�����:2015-3-31                                            
=  ����:wyx                  
=  ����:����ͼƬ�Ŵ�Ч������QQ
=  ʹ�÷���:
=	CPictureCtrl* pPictureCtrl = new CPictureCtrl(this, strPicPath);
=	pPictureCtrl->Create(CPictureCtrl::IDD, this);
=	pPictureCtrl->SetPos();
=	pPictureCtrl->ShowWindow(SW_SHOW);
====================================================================*/ 
#pragma once
#include "afxwin.h"
#include <comdef.h>//��ʼ��һ��com��
#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#endif
#include "GdiPlus.h"
using namespace Gdiplus;

#pragma comment(lib,"gdiplus.lib")

// CPictureCtrl �Ի���
class CPictureCtrlPic;
class CPictureCtrlClose;
class CPictureCtrl : public CDialog
{
	DECLARE_DYNAMIC(CPictureCtrl)

public:
	CPictureCtrl(CWnd* pParent = NULL, CString strFilePath = _T(""));   // ��׼���캯��
	virtual ~CPictureCtrl();

	// �Ի�������
	enum { IDD = IDD_DIALOG_PICTURE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
public:
	CBrush m_brush;
	CString m_strFilePath;
	CStatic m_staticPicture;
	HBITMAP m_hBmp;
	HCURSOR m_hCursor;
	CPictureCtrlPic* m_pPictureCtrlPic;
	CPictureCtrlClose* m_pPictureCtrlClose;
	int m_x;
	int m_y;
	CRect m_rcClose;
	BOOL m_bIsBig;
	int m_downX;
	int m_downY;
	int m_moveX;
	int m_moveY;
	CRect m_rcPic;
public:
	void SetFile(CString& strFilePath);
	BOOL ShowJpgGif(CDC* pDC,CString strPath, int x, int y);
	void CloseAll();
	void SetPos();
	void SetCloseRect(CRect rc);
	afx_msg void OnPaint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
