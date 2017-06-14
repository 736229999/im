/*====================================================================  
=  文件名:CSelCustomHeadIconDlg类                                    
=  修改日期:2015-6-10                                            
=  作者:wyx                  
=  作用:用于自定义头像上传
====================================================================*/ 
#pragma once
#include "GdiPlus.h"
#include "im/RectTrackerEx.h"
using namespace Gdiplus;
#pragma comment(lib,"gdiplus.lib")

// CSelCustomHeadIconDlg 对话框

class CSelCustomHeadIconDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CSelCustomHeadIconDlg)

public:
	CSelCustomHeadIconDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSelCustomHeadIconDlg();

// 对话框数据
	enum { IDD = IDD_DLG_SEL_CUSTOM_HEAD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnSelectpic();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	CPngBtn m_btnSelectPic;
public:
	ULONG_PTR m_pGdiToken;//gdi初始化
	CString m_strFilePath;//文件路径
	CStatic m_staticPic;//显示上传文件的区域
	int m_nTrackerRectWidth;//预览头像宽度，88
	int m_nTrackerRectHeight;//预览头像高度，88
	CRect m_rect;//上传文件的区域
	CRect m_rectPic;//上传文件的区域
	CRect m_rectLast;//选择区域变化前的区域，用于计算需要刷新的区域
	CRectTrackerEx m_rectTracker;//绘制框选区域的边框
	CBitmap m_bitmap;//背景图片，用于刷新背景
	int m_nHandle;//判断当前鼠标状态，根据鼠标状态绘制鼠标样式

	//鼠标移动的位置,用于拖动选择区域或改变选择区域的大小
	int m_nDownX;
	int m_nDownY;
	int m_nMoveX;
	int m_nMoveY;


	
};
