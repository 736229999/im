#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"
#include "customtoolbar.h"
#include "DuiP2PSendMsgDlg.h"

//快捷短语新建或编辑
class CFastWordNew;
class CFastWordNew : public DuiLib::WindowImplBase
{
public:
	static std::tr1::shared_ptr<CFastWordNew> NewDlg(HWND pDlg, int nType = 1, CString strGroupName = "", int nFastWordIndex = 0);
	static std::tr1::shared_ptr<CFastWordNew> GetInstance()	{	return pInstance;	}
	static std::tr1::shared_ptr<CFastWordNew> pInstance;
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("FastWordNew");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("FastWordNew.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	virtual void InitWindow();

	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	void Notify(DuiLib::TNotifyUI& msg);
	static void UpdateBk();
	void SetParentHwnd(HWND pDlg);
	void UpdateCombo();//所有组名写入combo
	BOOL AddFastWord();//添加快捷短语
	BOOL AddFastWordGroup(CString strGroup);//添加组
	void SetType(int nType);//设置是添加还是编辑快捷短语
	void SetGroupName(CString strGroupName);//待编辑的快捷短语组
	void SetFastWordIndex(int nFastWordIndex);//待编辑的快捷短语
	BOOL EditFastWord();//编辑快捷短语
	void UpdateInterface();//如果为编辑快捷短语则显示选中快捷短语的信息
private:
	HWND m_parent;
	int m_nType;//快捷短语 1为添加，2为编辑
	XT_FONT m_font;//设置的字体
	CRichEditCtrlEx m_reditSend;//快捷短语RichEdit
	CP2PTalkToolBar m_wndTalkToolBar;//设置字体样式
	DuiLib::CListUI* m_pComboGroup;//所有组的combo
	DuiLib::CEditUI* m_editFastWordNum;//待编辑的快捷编码
	int m_nFastWordIndex;//待编辑的快捷短语
	CString m_strGroupName;//待编辑的快捷短语组
	CString m_strFastWordNum;//待编辑的快捷编码
	virtual DuiLib::CControlUI* CreateControl(LPCTSTR pstrClassName)
	{
		//RichEdit放入RichEditSendWnd
		if (_tcsicmp(pstrClassName, _T("RichEditSendWnd")) == 0)
		{
			CWndUI  *pUI  = new CWndUI;      
			RECT rc;
			rc.left = 0;
			rc.right = 0;
			rc.top = 0;
			rc.bottom = 0;

			pUI->Attach(m_reditSend.GetSafeHwnd());
			return pUI;
		}
		//字体样式放入TalkBarWnd
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

			if(m_reditSend.GetSafeHwnd() == NULL)
				m_reditSend.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | ES_WANTRETURN
				| WS_VSCROLL | ES_AUTOVSCROLL, rc, CWnd::FromHandle(GetHWND()), 10005);
			m_reditSend.SetLimitWordSize(400);//发送框限制输入400个字
			m_wndTalkToolBar.Create(CP2PTalkToolBar::IDD, CWnd::FromHandle(GetHWND()));
			m_wndTalkToolBar.MoveWindow(&rc);
			pUI->Attach(m_wndTalkToolBar.GetSafeHwnd());           
			return pUI;
		}
		return NULL;
	}
public:
	CFastWordNew(void);
	~CFastWordNew(void);
};