/*====================================================================  
=  文件名:CScreenCtrl类                                    
=  修改日期:2015-5-5                                            
=  作者:wyx                  
=  作用:用于截屏，效果类似QQ
=  使用方法:
=  CScreenCtrl dlg(GetDesktopWindow());
=  dlg.m_strImageFileCachePath = g_LocalData.GetImageFileCachePath();
=  if ( IDOK==dlg.DoModal() )
=  {
=      CString strCacheFileName;
=      g_LocalData.SaveImageFileToCache(dlg.m_strImageFile,strCacheFileName);
=      CString strCacheImageFullPathName=g_LocalData.GetImageFileCachePath()+strCacheFileName;

=      DeleteFile(dlg.m_strImageFile);

=      this->m_reditSend.InsertImage(strCacheImageFullPathName,EMOTION_PIC_DATA());//发送到聊天框
}
====================================================================*/ 
#pragma once
#include <vector>
#include "RectTrackerEx.h"
#include "EditScreen.h"
// CScreenCtrl 对话框
class CRectTrackerEx;
class CDlgSCreenCtrlTool;
#include "GdiPlus.h"
using namespace Gdiplus;

#define WM_EDIT_PIC_SAVE WM_USER+1
#define WM_DRAW_RECTTRACKER WM_USER+2

typedef struct tARROWSTRUCT {
	int nWidth;
	float fTheta;
	bool bFill;
} ARROWSTRUCT;

typedef CArray< CBitmap*, CBitmap* > CBitmapArray;

class CScreenCtrl : public CDialog
{
	DECLARE_DYNAMIC(CScreenCtrl)

public:
	CScreenCtrl(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScreenCtrl();

	// 对话框数据
	enum { IDD = IDD_DIALOG_SCREEN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	LRESULT OnEditPicSave(WPARAM,LPARAM);
	LRESULT DrawRectTracker(WPARAM,LPARAM);

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMove(int x, int y);

public:
	HBITMAP CopyBitmap(HBITMAP hSourceHbitmap);//复制bmp图片
	void SetDrawColor(DWORD dwColor);//设置绘制图形的颜色
	void SetDrawSize(int nSize);//设置绘制图形的宽度
	void SetFontSize(int nSize);//设置文字编辑的大小
	void DonePic();//完成截屏
	void SavePic(CString strFilePath);//保存图片
	void Revoke();//撤销编辑
public:
	CString m_strImageFile;//保存的文件名
	CString m_strImageFileCachePath;//保存的文件路径

	BOOL m_bIsDrawRectangle;//判断是否为绘制矩形状态
	BOOL m_bIsDrawEllipse;//判断是否为绘制圆形状态
	BOOL m_bIsDrawArrow;//判断是否为绘制箭头状态
	BOOL m_bIsDrawEdit;//判断是否为编辑文字状态
	BOOL m_bEditFlag;//判断是否需要结束文字编辑，用于文字编辑状态时选择了其他编辑工具
private:
	unsigned long m_pGdiToken;//GDI+工具初始化

	CEditScreen m_edit;//文字编辑的编辑框

	CDlgSCreenCtrlTool* m_pDlgSCreenCtrlTool;//截图编辑工具

	CRectTrackerEx m_rectTracker;//绘制框选区域的边框
	BOOL m_bIsDrawBack;//判断是否已经绘制了背景
	CSize      m_szScreen;//当前屏幕的大小

	CBitmap    m_bitmapScreen;//当前屏幕图片
	CBitmap    m_bitmapBack;//当前屏幕背景图片，用于双缓存
	CBitmap    m_bitmapLast;//最后一次保存的图片，用于保存图片时去掉边框

	std::vector<CRect> m_vecRc;//按层级顺序存储当前系统所有的窗口
	CRect m_rect;//选择区域的边框
	CRect m_rectLast;//选择区域变化前的区域，用于计算需要刷新的区域
	CRect m_rectOld;//判断鼠标当前位置的窗口是否为上一次的窗口，用于刷新窗口捕获，防止闪屏

	CRect m_rectTool;
	CRect m_rectToolLast;

	BOOL m_bIsBtnDown;//判断鼠标是否按下，用于判断是否选择了区域
	BOOL m_bIsBtnDownFirst;//判断鼠标是否第一次按下，用于判断是拖动区域还是选择区域
	BOOL m_bIsMouseUp;//判断鼠标是否抬起，用于重新绘制选择的区域

	//鼠标按下的位置,用于拖动选择区域或改变选择区域的大小
	int m_nDownX;
	int m_nDownY;
	//鼠标移动的位置,用于拖动选择区域或改变选择区域的大小
	int m_nMoveX;
	int m_nMoveY;

	//选择区域变化时的三个区域刷新，防止闪屏
	CRect m_rectRefresh1;
	CRect m_rectRefresh2;
	CRect m_rectRefresh3;

	//选择区域变化时的三个区域刷新，防止闪屏
	CRect m_rectRefresh4;
	CRect m_rectRefresh5;
	CRect m_rectRefresh6;

	int m_nHandle;//判断当前鼠标状态，根据鼠标状态绘制鼠标样式

	CPoint m_ptStart;//绘制图形时的起始位置
	CPoint m_ptOld;//上一次绘制图形的位置，用于擦除上一次绘制的图形
	BOOL m_bMouseDown;//判断是否为绘制图形时鼠标按下

	CBitmapArray m_arrayBitmap;//存储需要绘制的图片，用于撤销编辑
	CBitmapArray m_arrayBitmapAll;//存储所有绘制的图片，用于释放内存

	//上一次绘制箭头的两个顶点，用于擦除上一次绘制的箭头
	CPoint m_pt1Old;
	CPoint m_pt2Old;

	DWORD m_dwDrawColor;//绘制颜色
	int m_nDrawSize;//绘制大小(宽度)
	int m_nFontSize;//文字大小

};