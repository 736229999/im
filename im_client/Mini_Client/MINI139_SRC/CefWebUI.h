#pragma once

#include "..\..\DuiLib\UIlib.h"
#include "WebClient.h"

#define WM_IS_NAVEGATE               WM_USER+0x1117     //�������
#define WM_RBUTTON                   WM_USER+0x1118     //�Ҽ��˵�

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
	virtual void SetInternVisible(bool bVisible = true); // �����ڲ����ã���ЩUIӵ�д��ھ������Ҫ��д�˺���
	CCefWnd *m_pWindow;
	DuiLib::WindowImplBase* m_pParentWnd;
};

// ��ʼ��
void CefInit();
// ж��
void CefUninit();