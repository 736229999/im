#pragma once
#include "resource.h"
#include "..\im\impublic.h"
#include "afxwin.h"

#include "..\..\..\DuiLib\UIlib.h"

class CSysCommonFastWord : public DuiLib::WindowImplBase
{
public:
	CSysCommonFastWord();   // 标准构造函数
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UIFastWord");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("autoreply.xml");  }
	virtual void InitWindow();
	virtual void OnClick(DuiLib::TNotifyUI& msg);
	virtual void Notify(DuiLib::TNotifyUI& msg);
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	// 保存
	void Save();
	void UpdateBk();

private:
	void AddAutoReply(const char *szAutoReply);
	void OnBnClickedBtnAdd();
	void OnBnClickedBtnMod();
	void OnBnClickedBtnDel();

private:
	DuiLib::CListUI *m_pList;
	DuiLib::CButtonUI *m_pBtnAdd;
	DuiLib::CButtonUI *m_pBtnEdit;
	DuiLib::CButtonUI *m_pBtnDel;
	DuiLib::CButtonUI *m_pBtnUp;
	DuiLib::CButtonUI *m_pBtnDown;
	vector<string> m_vecFastWord;
};

//
//// CSysCommonFastWord 对话框
//
//class CSysCommonFastWord : public CDialog
//{
//	DECLARE_DYNAMIC(CSysCommonFastWord)
//
//public:
//	CSysCommonFastWord(CWnd* pParent = NULL);   // 标准构造函数
//	virtual ~CSysCommonFastWord();
//
//	// 保存
//	void Save();
//
//// 对话框数据
//	enum { IDD = IDD_WIZARD_SYS_COMMON_FAST_WORD };
//
//protected:
//	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
//	virtual BOOL OnInitDialog();
//	void OnOK()	{}
//	void OnCancel() {}
//	DECLARE_MESSAGE_MAP()
//public:
//	CFlatButton m_btnAdd;
//	CFlatButton m_btnModify;
//	CFlatButton m_btnDel;
//	CFlatButton m_btnUp;
//	CFlatButton m_btnDown;
//	CListBox m_ctlList;
//	afx_msg void OnBnClickedBtnAdd();
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnBnClickedBtnModify();
//	afx_msg void OnBnClickedBtnDel();
//	afx_msg void OnBnClickedBtnUp();
//	afx_msg void OnBnClickedBtnDown();
//
//private:
//	vector<string> m_vecFastWord;
//};
