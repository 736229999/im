#pragma once
#include "GdiPlus.h"
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")
// CPictureCtrlPic 对话框
class CPictureCtrl;
class CPictureCtrlPic : public CDialog
{
	DECLARE_DYNAMIC(CPictureCtrlPic)

public:
	CPictureCtrlPic(CWnd* pParent = NULL, CString strFilePath = _T(""));   // 标准构造函数
	virtual ~CPictureCtrlPic();

	// 对话框数据
	enum { IDD = IDD_DIALOG_PICTURE_PIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CString m_strFilePath;
	CPictureCtrl* m_pPictureCtrl;
	CRect m_rect;
	void SetPos(CRect rc);
	void SetNewPos(BOOL bUp);
	BOOL ShowJpgGif(CDC* pDC,CString strPath, int x, int y);
	void MoveNewPos(int x, int y);
	int m_nAddWidth;
	int m_nAddHight;
	int m_nTop;
	int m_nBottom;
	int m_nLeft;
	int m_nRight;
	int m_x;
	int m_y;
	BOOL m_bIsMove;
	ULONG_PTR m_pGdiToken;
public:
	void SetFile(CString& strFilePath);
public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
