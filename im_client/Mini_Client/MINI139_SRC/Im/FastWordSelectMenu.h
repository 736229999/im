#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"
#include "LocalData.h"

#define WM_FASTWORD_MENU_HIDE (WM_USER+107) //��ݶ���˵�����
#define WM_FASTWORD_MENU_SELECT (WM_USER+119) //������ݶ���˵�
#define WM_FASTWORD_MENU_SELECT_FIRST (WM_USER+120) //��ݱ������ݶ���˵��е���ȫһ�²���û���ظ���ֱ��д�������

//��ݼ�������ݶ���˵�
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
	void SetFastWordSelectMenu(vector<FastWordGroupInfo> vecFastWordGroupInfo, CString strSearch);//����������ַ����ҿ�ݶ���,��ʾ���б���
private:
	HWND m_parent;
	DuiLib::CListUI* m_listFastWordSelect;//���п�ݶ����б�
public:
	CFastWordSelectMenu(void);
	~CFastWordSelectMenu(void);
};