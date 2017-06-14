#pragma once
#include "xskindlg.h"
#include "..\resource.h"
#include "..\im\LocalData.h"
#include "MainFrame.h"
#include "..\..\..\DuiLib\UIlib.h"
#include "..\..\..\Common\WndShadow.h"

#define CLOSE_TALK        0 //�ر�δ������
#define CLOSE_MUL_TALK    1 //�ر����л��߹رյ�ǰ����
#define EXIT_APP          2 //�˳�����ر�δ����Ϣ
#define MOD_USER          3 //�����û�,���δ����Ϣ
#define LOGIN_AGAIN       4 //�ٴε�¼����ʾδ����Ϣ
#define CLOSE_ALL         5 //ȫ���ر�

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
	void SaveCloseTalkCfg(int nCode = CLOSE_TALK);///�ر�һ���Ի�
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