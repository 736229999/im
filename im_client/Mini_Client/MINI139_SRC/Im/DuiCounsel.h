/*====================================================================  
=  �ļ���:CDuiCounsel��                                    
=  �޸�����:2015-6-10                                            
=  ����:wyx                  
=  ����:��������ѯ����Ϣչʾ 
====================================================================*/ 
#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\DuiLib\Core\UIManager.h"
#include "LocalData.h"
#include "P2PSendMsgDlg.h"
#include "DuiP2PSendMsgDlg.h"
class CDuiCounsel :
	public DuiLib::WindowImplBase
{
public:
	/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* m_pDlgSelectP2PSendMsgDlg;//��ǰѡ��������
	vector<FRIEND_INFO*> m_vecFriend;//����������ѯ�ĺ���
	vector</*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg*> m_vecP2PSendMsgDlg;//����������ѯ�������
	map<FRIEND_INFO*, int> m_mapFriendCount;//��ѯ�Ĵ����������ж��ظ���ѯ
	//vector<DuiLib::CListContainerElementUI*> m_vecListElement;//���е�Ԫ�����ڲ鿴��������ȫ������ѯ
	int m_nRemindCount;//δ����Ϣ��
	map<DuiLib::CDuiString,bool> m_MenuCheckInfo; //�жϲ˵���ǰѡ��Ĵ�������ȫ��
	BOOL m_bIsUpdate;
	CRect m_preRc;
public:
	virtual LPCTSTR    GetWindowClassName() const   {   return _T("DUIMainFrame");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("DuiCounsel.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();//��ʼ��
	void Notify(DuiLib::TNotifyUI& msg);//��Ϣ��Ӧ

	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);//����˵������Ϣ
	static void UpdateBk();
	static std::tr1::shared_ptr<CDuiCounsel> Show();
	static std::tr1::shared_ptr<CDuiCounsel> GetInstance();
	DuiLib::CButtonUI* pBtnRemind;////δ������

private:
	static std::tr1::shared_ptr<CDuiCounsel> pInstance;
	DuiLib::CListUI* m_pList;
public:
	//���һ����ѯ�ĺ���
	void AddFriendListElement(FRIEND_INFO* pFriend, bool bIsMessage = false, bool bIsTransfer = false, bool bIsRepeat = false, bool bIsReply = false, bool bIsRemind = false, bool bBtnMarkeing = false, bool bBtnMarked = false);
	//������ѯ���ѵı��
	void SetFriendListElement(FRIEND_INFO* pFriend, bool bIsReply, bool bIsTransfer, bool bIsRepeat, bool bIsMessage, bool bIsRemind,int nUnReadNum = 0);	
	void AddP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg);//���������ж�Ϊ�ظ������
	void DeleteP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg);//��ѯ��ɾ������
	void ShowSelectP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlgSelect,int nCntUnRead = 0);//��ʾ��ǰ�����
	void MoveP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg);//������ѯ��ǰλ���ƶ������
	void ShowAllCounselDlg();//��ʾ������ѯ����
	void ShowCounselingDlg();//��ʾ���������ѯ����
	void MoveAllWindow(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlgCurrent);//һ�������ı��С��λ�������������֮�ı�
	void SetAllWindowMax(BOOL bIsMax);//���д��ڶ����
	BOOL IsPopMsgWarn(int &nUnRead);
	BOOL IsCurrentShowDlg(CDuiP2PSendMsgDlg* pDlg);
	void UpdateFriendListElement(FRIEND_INFO* pFriend);
	CRect GetPreRc();//��ȡ���֮ǰ��λ��
	void SetPreRc(CDuiP2PSendMsgDlg* pDlg);//�������֮ǰ��λ��
};

