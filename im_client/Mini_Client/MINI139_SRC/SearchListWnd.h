#pragma once

#include "..\..\..\DuiLib\UIlib.h"
#include <vector>

#define WM_MSG_ITEMCLICK (WM_USER+256)		// �򸸴��ڷ��͵����Ϣ

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
	// ˢ���б�
	void Update(const std::vector<int> &vecFriendID);
	// ��ȡ��ǰѡ��
	int GetCurSel();
	// ��ȡitem����������
	DWORD GetItemData(int nIndex);

protected:
	std::vector<int> m_vecFriendID;
	DuiLib::CListUI *m_pList;
	DuiLib::CTabLayoutUI *m_pTab;
};