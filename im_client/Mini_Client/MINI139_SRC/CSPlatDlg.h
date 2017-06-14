#pragma once

#include "..\..\..\DuiLib\UIlib.h"
#include "im\impublic.h"
#include <vector>
#include "im\DuiP2PSendMsgDlg.h"
/*
class CCSPlatQueuDlg : public DuiLib::WindowImplBase
{
public:
	enum { 
		TIMER_SERCHEDIT_SETFOCUS=0x00,
	};

	CCSPlatQueuDlg();
	~CCSPlatQueuDlg();
	static std::tr1::shared_ptr<CCSPlatQueuDlg> Show(HWND hWnd, POINT pt, vector<int> vecWaitFriendID);
	static void CloseDlg();
	static std::tr1::shared_ptr<CCSPlatQueuDlg> GetInstance()	{	return pInstance;	}
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("CCSPlatQueuDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("CSPlatQueu.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	void Notify(DuiLib::TNotifyUI& msg);
	// ���ݺ���IDˢ���б�
	void UpdateList(std::vector<int> &vecQueu);
	// ��ӵȴ������еĺ���
	static void AddWaitFriend(int nFriendID);
	// ɾ���ȴ������еĺ���
	static void DelWaitFriend(int nFriendID);

protected:
	// ��ӵ�Ԫ
	void AddItem(int nFriendID);

	static std::tr1::shared_ptr<CCSPlatQueuDlg> pInstance;
	DuiLib::CListUI *m_pList;
};
*/

class CSearchListWnd;
class CDuiRecentList;
class CDuiFriendList;
class CDuiCSPlatDlg : public DuiLib::WindowImplBase
{
	DECLARE_XT_MESSAGE_MAP()

public:
	enum { 
		TIMER_SERCHEDIT_SETFOCUS=0x00,
	};

	CDuiCSPlatDlg();
	~CDuiCSPlatDlg();
	static std::tr1::shared_ptr<CDuiCSPlatDlg> Show();
	static void CloseDlg();
	static std::tr1::shared_ptr<CDuiCSPlatDlg> GetInstance()	{	return pInstance;	}
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("CSPlatDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("CSPlatDlg.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass);
	void Notify(DuiLib::TNotifyUI& msg);
	static void UpdateBk();
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);//����˵������Ϣ
	void ShowCounselingDlg();
	void ShowAllCounselDlg();
	// ���������ϵ���б�
	CDuiRecentList* GetRecentList()	{	return m_pRecentList;	}
	// ���غ����б�
	CDuiFriendList* GetFriendList()		{	return m_pFriendList;	}

	//������ѯ
	void AddFriendListElement(FRIEND_INFO* pFriend, bool bIsMessage = false, bool bIsTransfer = false, bool bIsRepeat = false, bool bIsReply = false, bool bIsRemind = false, bool bBtnMarkeing = false, bool bBtnMarked = false);
	void AddP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg);
	void SetFriendListElementTranPre(FRIEND_INFO* pFriend, bool bIsTranPre);
	void SetFriendListElement(FRIEND_INFO* pFriend, bool bIsReply, bool bIsTransfer, bool bIsRepeat, bool bIsMessage, bool bIsRemind, int nUnReadNum = 0);
	void MoveP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg);//������ѯ��ǰλ���ƶ������
	void ShowSelectP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlgSelect,int nCntUnRead = 0);
	void DeleteP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg);//��ѯ��ɾ������
	void MoveAllWindow(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlgCurrent);
	void UpdateFriendListElement(FRIEND_INFO* pFriend);
	BOOL IsCurrentShowDlg(CDuiP2PSendMsgDlg* pDlg);
	void OnFinalMessage(HWND hWnd);

	// ��ӵȴ������еĺ���
	static void AddWaitFriend(int nFriendID);
	static void UpdateWaitFriend();
	// ɾ���ȴ������еĺ���
	static void DelWaitFriend(int nFriendID);
	// ��յȴ����У��ٿͻ������µ�¼�ɹ������
	static void ClearWaitFriend();
	//���õ�һ��tab��ʾ�ҵ��Ŷ�
	void SetLayoutPageTeam();
	//tabҳѡ���û�
	void SetLayoutPageCust();
/*	
	// ��ȡ�ȴ��������ʼ�ĺ���
	static int GetFirstWaitBuyer();
*/
	void SetListCurselNull();//�򿪿ͷ����촰��ʱ�ͻ��б�����Ϊ-1
	void SetCustUnMsgVisable(bool bIsVisable);//��ʾ�����ؿͻ���ǩ�ĺ��
	void SetTeamUnMsgVisable(bool bIsVisable);//��ʾ�������ҵ��Ŷӱ�ǩ�ĺ��
	void SetFriendUnMsg(FRIEND_INFO* pFriend, bool bIsUnMsg);//�����ҵ��Ŷ��к��ѵĺ��
	BOOL IsPopMsgWarn(int &nUnRead);///�鿴���лỰ����δ����Ϣ
	BOOL IsCurrentPopMsgWarn(int &nUnRead,int nFriendID);///�鿴��ǰ�Ự����δ����Ϣ
	BOOL CheckUnSavedServiceLog();
	BOOL SelectUnCompletedServiceLog(FRIEND_INFO *pFriend);
	BOOL CheckBuyerTalkingExist();//����Ƿ�����ҺͿͷ�����Ĵ���
	int  CalcBuyerTalkingWnd();//������ҺͿͷ�����Ĵ�������
	// ��ȡ�ȴ����г���
	int GetWaiterCount()	{	return g_vecWaitFriendID.size();	}
	
	//�����С��
	CRect GetPreRc();//��ȡ���֮ǰ��λ��
	void SetPreRc(CDuiP2PSendMsgDlg* pDlg);//�������֮ǰ��λ��
	void SetAllWindowMax(BOOL bIsMax);//���д��ڶ�������������С��
	void UpdateFriendList(FRIEND_INFO *pFriend);
protected:
	// ͨ���ؼ��ֲ��Һ���
	void SearchFriend(const char *pKey, vector<int> &vecMeetFriend);
	// ���ҹؼ��ָı�
	void SearchTextChange(const char *pKey);
	void OnTimer(UINT nIDEvent);
	//������ʾ�����б�
	void UpdateTeamList();
	void OnSendMsg();
	void OnRecentListRButtonDown();
	void OnFriendInfo();
	void OnModfCommName();
	//����Ա����Ϣ
	void OnSendGroupMessage();
	void OnModFCNameAck(const char * data,WORD wRet);
	void ModifyName(XT_MOD_FRIEND_COMMNAME_ACK *pAck);
	// ���µȴ����б�ǩ
	void UpdateWaitLabel();

	static std::tr1::shared_ptr<CDuiCSPlatDlg> pInstance;

	std::tr1::shared_ptr<CSearchListWnd> m_pSearchListWnd;

	CDuiRecentList *m_pRecentList;
	CDuiFriendList *m_pFriendList;
	DuiLib::CTabLayoutUI *m_pLayoutPage;
	DuiLib::CListUI* m_pCounselList;
	DuiLib::CButtonUI* m_pBtnCust;
	
	DuiLib::CLabelUI* m_labelUnMsgCust;//�ͻ�δ����Ϣ���
	DuiLib::CLabelUI* m_labelUnMsgTeam;//�ҵ��Ŷ�δ����Ϣ���

	CRect m_preRc;//���֮ǰ��λ��
public:
	/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* m_pDlgSelectP2PSendMsgDlg;//��ǰѡ��������
	vector<FRIEND_INFO*> m_vecFriend;//����������ѯ�ĺ���
	vector<CDuiP2PSendMsgDlg*> m_vecP2PSendMsgDlg;//����������ѯ�������

	map<FRIEND_INFO*, int> m_mapFriendCount;//��ѯ�Ĵ����������ж��ظ���ѯ
	map<DuiLib::CDuiString,bool> m_MenuCheckInfo; //�жϲ˵���ǰѡ��Ĵ�������ȫ��
	BOOL m_bIsUpdate;

protected:
	static std::vector<int> g_vecWaitFriendID;		// �ȴ�����
};