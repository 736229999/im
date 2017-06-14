#pragma once
#include "Im\UserLevelCtrl.h"
#include "Im\IMProtocol.h"
#include "..\HmgSkin\HmgSkin\ColumnCtrl.h"
// CFriendInfoDlg 对话框
#include "..\..\..\..\Common\WndShadow.h"
#include "..\..\..\DuiLib\UIlib.h"
#include "map"

class CFriendInfoDlg : public DuiLib::WindowImplBase
{
	DECLARE_XT_MESSAGE_MAP()

public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UIFriendInfo");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("FriendInfo.xml");  }  
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	void    Notify(DuiLib::TNotifyUI& msg);
	afx_msg void OnBnClickedSendMsg();
	void OnClose();
	void OnUserInfoAck(const char *data, WORD wRet);
	void OnModFCNameAck(const char *data, WORD wRet);

	string GetRemarkName();
	int GetGroupIndex();

	// 更新用户信息
	void Refresh(FRIEND_INFO *pFriendInfo);

	// 显示对话框
	~CFriendInfoDlg();
    CFriendInfoDlg(FRIEND_INFO *pFriendInfo, CWnd* pParent = NULL);   // 标准构造函数
	//static /*std::tr1::shared_ptr<CFriendInfoDlg> */ CFriendInfoDlg* Show(FRIEND_INFO* pGroupFriend);
	int CountLevel(int nTime,int&nNextDelta);
	void SetLevelPic(int nLevel);

	// 更新所有背景
	static void UpdateAllBk();
    void UpdateBk();
	static void CloseAll();
	//static std::tr1::shared_ptr<CFriendInfoDlg> GetInstance()	{	return pInstance;	}
	static tr1::shared_ptr<CFriendInfoDlg> NewDlg(FRIEND_INFO* pGroupFriend);
private:

	////此处加载自定义控件
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (_tcsicmp(pstrClass, _T("ButtonGif")) == 0)
		{
			return	new DuiLib::CButtonGifUI();
		}
		return NULL;
	}
	/////
	DuiLib::CButtonUI*m_pBtnClose;       //关闭
	DuiLib::CButtonGifUI *m_pLabelHeadPhoto; //头像
	DuiLib::CLabelUI *m_pLabelnickName;  //昵称
	DuiLib::CLabelUI *m_pLevel;          //等级 
	DuiLib::CLabelUI *m_pLabelLevel;     //等级 
	DuiLib::CLabelUI *m_pLabelSec;       //等级 
	DuiLib::CLabelUI *m_pLabelThird;     //等级 
	DuiLib::CButtonUI *btnViewRecord;    //会话 
	
	/////
	DuiLib::CLabelUI *m_pLabellabelName; //姓名
	DuiLib::CEditUI  *m_pEditBackName;   //备注 
	DuiLib::CComboUI *m_pComboFriendGroup;//好友分组 
	
	DuiLib::CLabelUI *m_pLabelSex; 
	DuiLib::CLabelUI *m_pLabelAddr;    

	DuiLib::CLabelUI *m_pLabelBirthDay;  

	DuiLib::CLabelUI *m_pLabelCellPhone; 
	DuiLib::CLabelUI *m_pLabelTel;    
	DuiLib::CLabelUI *m_pLabelMail;  

	DuiLib::CLabelUI *m_pLlabelCareer;   ////职业
	DuiLib::CLabelUI *m_pLlabelCareerName;///名称

private:
    CWndShadow m_WndShadow;
	FRIEND_INFO *m_pFriendInfo;
	/*static std::tr1::shared_ptr<CFriendInfoDlg> pInstance;*/
	/*static CFriendInfoDlg *pInstance;*/
    map<uint32,CFriendInfoDlg *> m_mapFriendInfoDlg;

	static map<int, std::tr1::shared_ptr<CFriendInfoDlg>> g_mapDlg;
};

////class CRountFaceCtrl;
////class CFriendInfoArchDlg;
////class CFriendInfoDlg : public CXSkinDlg
////{
////	DECLARE_DYNAMIC(CFriendInfoDlg)
////
////public:
////	CFriendInfoDlg(FRIEND_INFO *pFriendInfo, CWnd* pParent = NULL);   // 标准构造函数
////	virtual ~CFriendInfoDlg();
////	static void Show(FRIEND_INFO *pFriendInfo);
////
////protected:
////	BOOL Create();
////	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
////	virtual BOOL OnInitDialog();
////	virtual void PostNcDestroy();
////	afx_msg void OnPaint();
////	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
////	// 设置控件颜色
////	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
////	void OnUserInfoAck(const char *data, WORD wRet);
////	void OnModFCNameAck(const char *data, WORD wRet);
////	
////	afx_msg void OnBnClickedSendMsg();
////	afx_msg void OnClose();
////	DECLARE_MESSAGE_MAP()
////	DECLARE_XT_MESSAGE_MAP()
////
////private:
////	CRountFaceCtrl *m_pFacePic;
////	HBRUSH m_hBkCaptionBar;
////	CUserLevelCtrl m_staLevel;		// 等级
////	FRIEND_INFO *m_pFriendInfo;
////	static std::tr1::shared_ptr<CFriendInfoDlg> pInstance;
////	CColumnCtrl m_MainTab;
////	CFont m_font;
////	CPngBtn m_btnSendMsg, m_btnEnterShop;
////	CFriendInfoArchDlg *m_pArchDlg;
////};
