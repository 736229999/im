#include "..\..\..\DuiLib\UIlib.h"
#include "im\CustomToolBar.h"
#include "im\RichEditCtrlEx.h"
#include "im\SimpleFileTransSession.h"

class CGroupSendDlg : public DuiLib::WindowImplBase, public CSimpleFileTransSession
{
protected:	
	LRESULT OnXtMessage(WPARAM wParam, LPARAM lParam);
	LRESULT	OnXtResend(WPARAM wParam, LPARAM lParam);

public:
	CGroupSendDlg();
	~CGroupSendDlg();
	static std::tr1::shared_ptr<CGroupSendDlg> Show(vector<int> &vecFriendID);
	static void CloseDlg();
	static std::tr1::shared_ptr<CGroupSendDlg> GetInstance()	{	return pInstance;	}
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("GroupSendDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("GroupSendDlg.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass);
	void Notify(DuiLib::TNotifyUI& msg);
	static void UpdateBk();
	void SF_RecvPfileAck(const XT_MSG &msg);
	void SF_RecvPfilePacketAck(const XT_MSG &msg);

protected:
	// 刷新好友选择区
	void UpdateSel(vector<int> &vecFriendID);
	// 获取选择好友的ID
	void GetSelID(vector<int> &vecSelFriendID);
	afx_msg void OnBnClickedFontBar();
	afx_msg void OnFaceIconClicked();
	afx_msg void OnSendPicClicked();
	afx_msg void OnScreenCopy();
	afx_msg void OnScreenCopyOption();
	afx_msg LRESULT OnEmotionSelNotify(WPARAM wParam,LPARAM lParam);
	// 发送按钮
	void OnBnClickedBtnSend();
	void OnBnClickedBtnHotkey();
	void OnEnter();
	void OnCtrlenter();
	void SF_InitXtMsg(XT_MSG &msg, int nFriendID);
	void RefreshFriendStatus(int nFriendID);
	// 发送消息
	void SendMsg(int nFriendID, const char *pMsgData);

	virtual void SF_InitXtMsg(XT_MSG &msg){}
	virtual void SF_OnAbortFile(const char * file_name){}
	virtual void SF_OnRecvFileComplete(const char *file_name){}
	virtual void SF_OnRecvFileReq(char *file_name){}
	void SF_SendPfileReq(const IMAGE_FILE_STRUCT &is,int nPackets, std::tr1::shared_ptr<XT_MSG> &msgO, int &nType);

	static std::tr1::shared_ptr<CGroupSendDlg> pInstance;
	vector<DuiLib::COptionUI*> m_vecLbSelFriend;
	CP2PTalkToolBar m_wndTalkToolBar;
	CRichEditCtrlEx	m_reditSend;
};
