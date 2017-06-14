#pragma once
#include <comdef.h>//��ʼ��һ��com��
#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#include "GdiPlus.h"
using namespace Gdiplus;
#endif
#pragma comment(lib,"gdiplus.lib")
// CPictureCtrlClose �Ի���
class CPictureCtrl;
class CPictureCtrlClose : public CDialog
{
	DECLARE_DYNAMIC(CPictureCtrlClose)

public:
	CPictureCtrlClose(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPictureCtrlClose();

	// �Ի�������
	enum { IDD = IDD_DIALOG_PICTURE_CLOSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CPictureCtrl* m_pPictureCtrl;
	unsigned long m_pGdiToken;
	CBrush m_brush;
	BOOL m_bIsHover;
	void SetPos(CRect rc);
	void DrawClose(BOOL bIsHover);
	BOOL ImageFromIDResource(UINT nID, LPCTSTR sTR, Image * & pImg);
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
