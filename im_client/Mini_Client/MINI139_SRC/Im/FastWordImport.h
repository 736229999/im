#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"
#include "customtoolbar.h"
#include "DuiP2PSendMsgDlg.h"

//快捷短语导入
class CFastWordImport;
class CFastWordImport : public DuiLib::WindowImplBase
{
public:
	static std::tr1::shared_ptr<CFastWordImport> NewDlg(HWND pDlg);
	static std::tr1::shared_ptr<CFastWordImport> GetInstance()	{	return pInstance;	}
	static std::tr1::shared_ptr<CFastWordImport> pInstance;
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("FastWordImport");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("FastWordImport.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	virtual void InitWindow();
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	void Notify(DuiLib::TNotifyUI& msg);
	static void UpdateBk();
	void SetParentHwnd(HWND pDlg);
	void ExportTemplate(CString strFileName);//导出快捷短语模版
	BOOL ImportExcel(CString strFileName);//导入Excel
	BOOL ImportCsv(CString strFileName);//导入Csv
private:
	HWND m_parent;
	XT_FONT m_font;//导出的字体
	CRichEditCtrlEx		m_reditSend;//用于初始化m_font
	CP2PTalkToolBar  m_wndTalkToolBar;//设置字体样式

	DuiLib::COptionUI *m_pRedioExcel;
	DuiLib::COptionUI *m_pRedioCsv;
	DuiLib::COptionUI *m_pRedioClear;

	//字体样式放入TalkBarWnd
	virtual DuiLib::CControlUI* CreateControl(LPCTSTR pstrClassName)
	{
		if (_tcsicmp(pstrClassName, _T("TalkBarWnd")) == 0)
		{
			CWndUI  *pUI  = new CWndUI;      
			RECT rc;
			rc.left = 0;
			rc.right = 0;
			rc.top = 0;
			rc.bottom = 0;

			m_font.fontSize    = 12;
			m_font.fontColor   = 0;
			m_font.flags       = 0;
			strcpy(m_font.fontName, "微软雅黑");

			m_reditSend.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | ES_WANTRETURN
				| WS_VSCROLL | ES_AUTOVSCROLL, rc, CWnd::FromHandle(GetHWND()), 10002);

			m_wndTalkToolBar.Create(CP2PTalkToolBar::IDD, CWnd::FromHandle(GetHWND()));
			m_wndTalkToolBar.MoveWindow(&rc);
			pUI->Attach(m_wndTalkToolBar.GetSafeHwnd());           
			return pUI;
		}
		return NULL;
	}
public:
	CFastWordImport(void);
	~CFastWordImport(void);
};

