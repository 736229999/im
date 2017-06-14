// ----------------------------------------------------------------
// 文件:SendMsgActionDlg.h
// 版本: 
// 作者: xy
// 日期: 2006-6-13
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once

#include "../resource.h"
#include "../controls/SkinCtrl.h"
#include "ActionTextReader.h"
#include "buttonext.h"

class CSendMsgActionToolBar : public CDialog
{
	DECLARE_DYNAMIC(CSendMsgActionToolBar)

public:
	CSendMsgActionToolBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSendMsgActionToolBar();

	enum { IDD = IDD_SENDMSG_ACTION };

	//------------------------------------------------------
	// 获得动作数据
	//------------------------------------------------------
	BOOL	GetActionData(ACTION_DATA& ad);
	//------------------------------------------------------
	// 添加一个聊天对象
	//------------------------------------------------------
	void	AddObject(const CString& sObject,int id);
	//------------------------------------------------------
	// 显示提示消息
	//------------------------------------------------------
	void	ShowToolTip(const CString& strTip);
	//------------------------------------------------------
	// 获得当前选择的聊天对象
	//------------------------------------------------------
	CString	GetCurSelObject();
	//------------------------------------------------------
	// 将动作置为无
	//------------------------------------------------------
	void	ResetAction();
	//------------------------------------------------------
	// 将聊天对象设置为无
	//------------------------------------------------------
	void	ResetObject();


protected:

	virtual void DoDataExchange(CDataExchange* pDX); 
	//------------------------------------------------------
	// 初始化动作列表
	//------------------------------------------------------
	void		InitActions();

	DECLARE_MESSAGE_MAP()

protected:

	vector<ACTION_ITEM>	m_actionList;	// 动作脚本列表
	CActionTextReader	m_reader;		// 动作脚本解析器
	CFlatButton			m_btnEditAction;// 编辑动作脚本按钮
	CFlatButton			m_btnLoadAction;// 加载动作脚本按钮
	CToolTipCtrl		m_toolTip;		// 提示控件	
	CString				m_sLastTip;		// 上次的提示信息
	CGraphComboBox		m_cmbObject;	// 聊天对象下拉菜单
	CGraphComboBox		m_cmbAction;	// 动作下拉菜单
	CTransparentLabel	m_lblMsg;		// 提示标签

public:

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnEditaction();
	afx_msg void OnBnClickedBtnLoadaction();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnCbnSelchangeCmbActions();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

};
