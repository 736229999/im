#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"

#define WM_FASTWORD_GROUP_RENAME (WM_USER+1) //修改快捷短语组名

//快捷短语组名重命名
class CFastWordGroupRename;
class CFastWordGroupRename : public DuiLib::WindowImplBase
{
public:
	static std::tr1::shared_ptr<CFastWordGroupRename> NewDlg(HWND pDlg, CString strGroupName);
	static std::tr1::shared_ptr<CFastWordGroupRename> GetInstance()	{	return pInstance;	}
	static std::tr1::shared_ptr<CFastWordGroupRename> pInstance;
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("FastWordGroupRename");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("FastWordGroupRename.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	virtual void InitWindow();
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	void Notify(DuiLib::TNotifyUI& msg);
	static void UpdateBk();
	void SetParentHwnd(HWND pDlg);
	void SetChangedGroupName(CString strGroupName);//设置待编辑的组名
private:
	HWND m_parent;
	CString m_strGroupName;//待编辑的组名
public:
	CFastWordGroupRename(void);
	~CFastWordGroupRename(void);
};

