#pragma once

#include "..\..\DuiLib\UIlib.h"
#include "WebClient.h"

#define WM_IS_NAVEGATE               WM_USER+0x1117     //传输完成
#define WM_RBUTTON                   WM_USER+0x1118     //右键菜单

class CCefWnd;
class CCefWebUI : public DuiLib::CControlUI
{
	friend class CCefWnd;
public:
	CCefWebUI(void);
	~CCefWebUI(void);
	void SetUrl(const char *pUrl);
	bool IsNavegate(const char* pUrl);
	bool Rbutton();
	virtual void SetPos(RECT rc);
	void DoEvent(DuiLib::TEventUI& event);
	void SetParentWnd(DuiLib::WindowImplBase* pParentWnd);
	//void SetCallbackFun(CallBackFun p, DWORD dwDlg);
protected:
	virtual void SetInternVisible(bool bVisible = true); // 仅供内部调用，有些UI拥有窗口句柄，需要重写此函数
	CCefWnd *m_pWindow;
	DuiLib::WindowImplBase* m_pParentWnd;
};

// 初始化
void CefInit();
// 卸载
void CefUninit();