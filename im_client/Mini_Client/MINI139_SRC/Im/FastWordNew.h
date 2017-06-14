#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"
#include "customtoolbar.h"
#include "DuiP2PSendMsgDlg.h"

//��ݶ����½���༭
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
	void UpdateCombo();//��������д��combo
	BOOL AddFastWord();//��ӿ�ݶ���
	BOOL AddFastWordGroup(CString strGroup);//�����
	void SetType(int nType);//��������ӻ��Ǳ༭��ݶ���
	void SetGroupName(CString strGroupName);//���༭�Ŀ�ݶ�����
	void SetFastWordIndex(int nFastWordIndex);//���༭�Ŀ�ݶ���
	BOOL EditFastWord();//�༭��ݶ���
	void UpdateInterface();//���Ϊ�༭��ݶ�������ʾѡ�п�ݶ������Ϣ
private:
	HWND m_parent;
	int m_nType;//��ݶ��� 1Ϊ��ӣ�2Ϊ�༭
	XT_FONT m_font;//���õ�����
	CRichEditCtrlEx m_reditSend;//��ݶ���RichEdit
	CP2PTalkToolBar m_wndTalkToolBar;//����������ʽ
	DuiLib::CListUI* m_pComboGroup;//�������combo
	DuiLib::CEditUI* m_editFastWordNum;//���༭�Ŀ�ݱ���
	int m_nFastWordIndex;//���༭�Ŀ�ݶ���
	CString m_strGroupName;//���༭�Ŀ�ݶ�����
	CString m_strFastWordNum;//���༭�Ŀ�ݱ���
	virtual DuiLib::CControlUI* CreateControl(LPCTSTR pstrClassName)
	{
		//RichEdit����RichEditSendWnd
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
		//������ʽ����TalkBarWnd
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
			strcpy(m_font.fontName, "΢���ź�");

			if(m_reditSend.GetSafeHwnd() == NULL)
				m_reditSend.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | ES_WANTRETURN
				| WS_VSCROLL | ES_AUTOVSCROLL, rc, CWnd::FromHandle(GetHWND()), 10005);
			m_reditSend.SetLimitWordSize(400);//���Ϳ���������400����
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