#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"
#include "LocalData.h"

#define WM_FASTWORD_MENU_HIDE (WM_USER+107) //快捷短语菜单隐藏
#define WM_FASTWORD_MENU_SELECT (WM_USER+119) //弹出快捷短语菜单
#define WM_FASTWORD_MENU_SELECT_FIRST (WM_USER+120) //快捷编码与快捷短语菜单中的完全一致并且没有重复则直接写入输入框

//快捷键弹出快捷短语菜单
class CFastWordSelectMenu;
class CFastWordSelectMenu : public DuiLib::WindowImplBase
{
public:
	static std::tr1::shared_ptr<CFastWordSelectMenu> NewDlg(HWND pDlg);
	static std::tr1::shared_ptr<CFastWordSelectMenu> GetInstance()	{	return pInstance;	}
	static std::tr1::shared_ptr<CFastWordSelectMenu> pInstance;
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("FastWordSelectMenu");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("FastWordSelectMenu.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	virtual void InitWindow();
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	void Notify(DuiLib::TNotifyUI& msg);
	void SetParentHwnd(HWND pDlg);
	void SetFastWordSelectMenu(vector<FastWordGroupInfo> vecFastWordGroupInfo, CString strSearch);//根据输入的字符查找快捷短语,显示到列表中
private:
	HWND m_parent;
	DuiLib::CListUI* m_listFastWordSelect;//所有快捷短语列表
public:
	CFastWordSelectMenu(void);
	~CFastWordSelectMenu(void);
};