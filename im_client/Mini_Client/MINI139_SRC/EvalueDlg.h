#pragma once
#include "XSkinDlg.h"
#include "clientprotocol.h"
// CEvalueDlg dialog

class CEvalueDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CEvalueDlg)

public:
	CEvalueDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEvalueDlg();

	// 获取评价等级
	CString GetEvalueGrade();

	// 获取服务等级
	EN_EVALUA_GRADE GetEvalueGradeEn();

	// 获取建议
	CString GetSuggest()	{ return m_strSuggest; }

protected:
	BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void OnPaint();
	// 相应控件颜色绘制事件，解决控制背景色保持原有颜色的问题
	HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	// 点击提交按钮
	void OnBnClickedSubmit();

	DECLARE_MESSAGE_MAP()

private:
	CFlatEdit m_editSuggest;
	CBmpBtn m_btnSubmit;
	int m_nEvalueGrade;				// 评价等级
	CString m_strSuggest;			// 建议	
};
