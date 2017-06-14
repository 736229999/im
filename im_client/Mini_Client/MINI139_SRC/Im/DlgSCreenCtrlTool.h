/*====================================================================  
=  文件名:CDlgSCreenCtrlTool类                                    
=  修改日期:2015-5-5                                            
=  作者:wyx                  
=  作用:截屏编辑工具，用于绘制矩形、圆形、箭头、文字编辑等
====================================================================*/ 
#pragma once
#include "afxwin.h"
#include "buttonext.h"
// CDlgSCreenCtrlTool 对话框
class CScreenCtrl;
class CDlgSCreenCtrlColor;
class CDlgSCreenCtrlTool : public CDialog
{
	DECLARE_DYNAMIC(CDlgSCreenCtrlTool)

public:
	CDlgSCreenCtrlTool(CWnd* pParent = NULL, CScreenCtrl* pScreenCtrl = NULL);   // 标准构造函数
	virtual ~CDlgSCreenCtrlTool();

	// 对话框数据
	enum { IDD = IDD_DIALOG_SCREEN_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
protected:
	CPngBtn m_btnRectangle;//绘制矩形
	CPngBtn m_btnEllipse;//绘制圆形
	CPngBtn m_btnArrow;//绘制箭头
	CPngBtn m_btnEdit;//文字编辑
	CPngBtn m_btnSave;//保存图片
	CPngBtn m_btnDone;//完成截屏
	CPngBtn m_btnClose;//关闭截屏
	CPngBtn m_btnRevoke;//撤销编辑
protected:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnRectangle();
	afx_msg void OnBnClickedBtnEllipse();
	afx_msg void OnBnClickedBtnArrow();
	afx_msg void OnBnClickedBtnSave();
	afx_msg void OnBnClickedBtnDone();
	afx_msg void OnBnClickedBtnClose();
	afx_msg void OnBnClickedBtnEdit();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnBnClickedBtnRevoke();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

public:
	void SetAllBtnUp();//设置所有按键抬起
	void ShowScreenColor();//画笔工具显示并跟随编辑工具

private:
	CDlgSCreenCtrlColor* m_pDlgSCreenCtrlColor;//画笔工具，用于绘制图形边框的宽度和颜色
	CScreenCtrl* m_pScreenCtrl;//截屏工具，用于在截屏功能中设置绘制模式
};
