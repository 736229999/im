/*====================================================================  
=  文件名:CDlgSCreenCtrlTool类                                    
=  修改日期:2015-5-5                                            
=  作者:wyx                  
=  作用:截屏画笔工具，用于绘制图形的边框大小和颜色
====================================================================*/ 
#pragma once

// CDlgSCreenCtrlColor 对话框
class CScreenCtrl;
class CDlgSCreenCtrlColor : public CDialog
{
	DECLARE_DYNAMIC(CDlgSCreenCtrlColor)

public:
	CDlgSCreenCtrlColor(CWnd* pParent = NULL, CScreenCtrl* pDlgSCreenCtrl = NULL);   // 标准构造函数
	virtual ~CDlgSCreenCtrlColor();

// 对话框数据
	enum { IDD = IDD_DIALOG_SCREEN_COLOR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	CStatic m_staticColor;//显示选中颜色的picture控件
	CPngBtn m_btnSmall;//边框宽度的大小为小
	CPngBtn m_btnNormal;//边框宽度的大小为中
	CPngBtn m_btnBig;//边框宽度的大小为大
protected:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedBtnSmall();
	afx_msg void OnBnClickedBtnNormal();
	afx_msg void OnBnClickedBtnBig();
	
public:
	void SetRect(int nPicRect, BOOL bIn);//设置鼠标是否在某个picture控件上，其他控件设置为未选中
	void DrawAllRectColor();//绘制所有picture控件上的颜色
	void DrawRectColor(DWORD dwColor);//绘制显示选中颜色的picture控件
	void DrawGrayLine();//绘制一条灰色的竖线，类似QQ

private:
	CScreenCtrl* m_pDlgSCreenCtrl;

	UINT m_vecPicId[16];//picture控件ID
	DWORD m_vecColor[16];//16种颜色，用于绘制picture控件
	BOOL m_bIsInRect[16];//判断鼠标在哪个picture控件
	BOOL m_bIsInRectOld[16];//判断鼠标上次在哪个picture控件
	int m_nColor;//鼠标选中的颜色编号
	int m_nSize;//鼠标选中的大小编号
};
