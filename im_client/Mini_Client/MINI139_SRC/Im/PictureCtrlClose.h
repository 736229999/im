#pragma once
#include <comdef.h>//初始化一下com口
#ifndef ULONG_PTR
#define ULONG_PTR unsigned long*
#include "GdiPlus.h"
using namespace Gdiplus;
#endif
#pragma comment(lib,"gdiplus.lib")
// CPictureCtrlClose 对话框
class CPictureCtrl;
class CPictureCtrlClose : public CDialog
{
	DECLARE_DYNAMIC(CPictureCtrlClose)

public:
	CPictureCtrlClose(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPictureCtrlClose();

	// 对话框数据
	enum { IDD = IDD_DIALOG_PICTURE_CLOSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
