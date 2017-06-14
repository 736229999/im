#pragma once

#include "FaceIconCtrl.h"

#include "userlevelctrl.h"
#include "buttonext.h"

#include "P2PUserPanel.h"
#include "..\RoundFaceCtrl.h"
#include "..\..\SpaceManagement\public\public_controls\ResGdiObj.h"

#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"


// CUserCardDlg 对话框

// 头像被点击的消息相应
#define WM_FACE_CLICKED (WM_USER + 784)


class CUserCardDlg : public DuiLib::WindowImplBase
{

	DECLARE_XT_MESSAGE_MAP()

public:
	static std::tr1::shared_ptr<CUserCardDlg> Show(FRIEND_INFO *pFriend, int nLeft, int nTop);

public:
	
#ifdef _VER_INTERNAL_
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UserCard");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("UserCardForInner.xml");    }/////内部版
#else
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UserCard");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("UserCard.xml");    }
#endif

	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	~CUserCardDlg();

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
	CWndShadow m_WndShadow;
	static std::tr1::shared_ptr<CUserCardDlg> pInstance;

	DuiLib::CButtonUI*m_pBtnclose;
    DuiLib::CButtonGifUI *m_pBtnPic;
	DuiLib::CLabelUI *m_pLabelNickName;
	DuiLib::CLabelUI *m_pLabelProfile;

#ifndef _VER_INTERNAL_
	DuiLib::CLabelUI *m_pLevel;
	DuiLib::CLabelUI *m_pLabelLevel;
    DuiLib::CLabelUI *m_pLabelSec;
	DuiLib::CLabelUI *m_pLabelThird;
#else
	DuiLib::CLabelUI *m_pLabelCellphone;
	DuiLib::CLabelUI *m_pLabelTel;
	DuiLib::CLabelUI *m_pLabelJob;
#endif

	FRIEND_INFO	 *  m_pFriendInfo;
////////////////////////////////////////////////////////////////

public:
	static void Refresh(FRIEND_INFO *pFriend);
	static void CloseWindow();
    void UpdateBk();
	static std::tr1::shared_ptr<CUserCardDlg> GetInstance()
	{
		return pInstance;
	}
	void UpdateFriendInfo(FRIEND_INFO *pFriend);
	void OnUserInfoAck(const char *data,WORD wRet);
	void SetLevelPic(int nLevel);
	int  CountLevel(int nTime,int&nNextDelta);
	// 点击头像区域
	afx_msg void OnBnClickedFaceIcon();
};
