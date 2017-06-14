#pragma once
#include "xskindlg.h"
#include "..\resource.h"
#include "..\im\LocalData.h"
#include "MainFrame.h"
#include "..\..\..\DuiLib\UIlib.h"
#include "..\..\..\Common\WndShadow.h"

#define CLOSE_TALK        0 //关闭未读窗口
#define CLOSE_MUL_TALK    1 //关闭所有或者关闭当前窗口
#define EXIT_APP          2 //退出程序关闭未读消息
#define MOD_USER          3 //更改用户,检测未读消息
#define LOGIN_AGAIN       4 //再次登录，提示未读消息
#define CLOSE_ALL         5 //全部关闭

class CExitMsgWarningDlg : public DuiLib::WindowImplBase
{
public:
	static std::tr1::shared_ptr<CExitMsgWarningDlg> Show(int nTag,HWND wnd);
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("ExitMsgWarning");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("ExitMsgWarning.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	void  Notify(DuiLib::TNotifyUI& msg);
	static void  UpdateBk();
	void  IsExitWarningShow();
	void  ReSetWindow(int nTag);
	void  SetParentHwnd(HWND pDlg);
	void  SetWindowTop();
	HWND  m_parent;
	BOOL  m_bUnReadMsgShow;


private:
	void SaveCloseTalkCfg(int nCode = CLOSE_TALK);///关闭一个对话
	void OnBnClickedBtnExit();
	void SaveMsgUnReadCfg();

private:
	WORD m_wTag;
	CWndShadow m_WndShadow;
	static std::tr1::shared_ptr<CExitMsgWarningDlg> pInstance;

	DuiLib::CButtonUI *m_pBtnClose;
	DuiLib::CLabelUI  *m_pLabelTitle;
	//DuiLib::CLabelUI *m_pLabelIcon;
	DuiLib::CLabelUI *m_pLabelWarn;
	DuiLib::CLabelUI *m_pLabelHide;
	
	DuiLib::CTabLayoutUI *m_pTabLayoutA;
	//DuiLib::CTabLayoutUI *m_pTabLayoutB;
	DuiLib::COptionUI *m_pCheckMsgBuf;
	DuiLib::CLabelUI  *m_pLabelExit;
	DuiLib::CButtonUI  *m_pBtnAdmin;
	DuiLib::CButtonUI  *m_pBtnCancel;
};