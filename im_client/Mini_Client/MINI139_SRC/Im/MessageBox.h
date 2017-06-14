// ----------------------------------------------------------------
// 文件:MessageBox.h
// 版本: 
// 作者: xy
// 日期: 2006-6-27
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once

#include "..\..\..\..\DuiLib\UIlib.h"
#include <map>
#define WM_MSGBOX (WM_USER+888)		// 只有当是非模太对话框时才发送此消息给父窗口

class CMessageBox : public DuiLib::WindowImplBase
{
public:
	// 只有是模态对话框时返回值才有效
	static int Prompt(LPCTSTR lpszText, 
		LPCTSTR lpszCaption = NULL,
		UINT nType = MB_OK,
		HWND hParent = NULL,
		BOOL bModel = FALSE);

	// 弹出一个模态对话框
	static int Model(HWND hWnd, LPCTSTR lpszText, LPCTSTR lpszCaption = NULL, UINT nType = MB_OK);

	int GetDlgID() {	return m_nID;	}

	// 更新所有背景
	static void UpdateAllBk();

private:
	CMessageBox(const char *pTitle, const char *pText, UINT nType, HWND hParent, BOOL bModel);
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("MessageBox");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("MessageBox.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	// 创建一个按钮
	DuiLib::CButtonUI* NewBtn(const char *pName, const char *pText, BOOL bBkBlue=TRUE);
	// 创建一个占位符
	DuiLib::CLabelUI* NewLabel(int nWidth=10);
	void Notify(DuiLib::TNotifyUI& msg);
	// 刷新背景
	void UpdateBk();
	
private:
	static std::map<int, std::tr1::shared_ptr<CMessageBox>> g_mapDlg;
	int m_nID;
	string m_strTitle;
	string m_strText;
	UINT m_nType;
	HWND m_hParent;
	BOOL m_bModel;
};