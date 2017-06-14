#pragma once

#include "..\..\..\DuiLib\UIlib.h"
#include <vector>

#define WM_MSG_ITEMCLICK (WM_USER+256)		// 向父窗口发送点击消息

class CSearchListWnd : public DuiLib::WindowImplBase
{
public:
	CSearchListWnd(const std::vector<int> &vecFriendID);
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("SearchListWnd");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("SearchListWnd.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	virtual LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	virtual void InitWindow();
	void Notify(DuiLib::TNotifyUI& msg);
	// 刷新列表
	void Update(const std::vector<int> &vecFriendID);
	// 获取当前选择
	int GetCurSel();
	// 获取item关联的数据
	DWORD GetItemData(int nIndex);

protected:
	std::vector<int> m_vecFriendID;
	DuiLib::CListUI *m_pList;
	DuiLib::CTabLayoutUI *m_pTab;
};