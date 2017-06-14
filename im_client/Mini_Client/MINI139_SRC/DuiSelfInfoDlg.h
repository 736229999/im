#pragma once
#include "..\..\..\DuiLib\UIlib.h"
#include "Im\IMProtocol.h"

class CDuiModHeadIconDlg;
class CDuiSelfInfoDlg : public DuiLib::WindowImplBase
{
	DECLARE_XT_MESSAGE_MAP()
public:
	CDuiSelfInfoDlg(void);
	~CDuiSelfInfoDlg(void);
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("SelfInfoDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("SelfInfoDlg.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	//DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass);
	//void AddGroup(LPCTSTR szText);
	//void AddFriend(LPCTSTR szImgPath, LPCTSTR szText, unsigned int id, DWORD dwTextColor = 0x2E2E2E);
	void Notify(DuiLib::TNotifyUI& msg);
	void UpdateBk();
	void SetLevelPic(int nLevel);
	static std::tr1::shared_ptr<CDuiSelfInfoDlg> Show();
	static std::tr1::shared_ptr<CDuiSelfInfoDlg> GetInstance()	{	return pInstance;	}
	////此处加载自定义控件
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (_tcsicmp(pstrClass, _T("ButtonGif")) == 0)
		{
			return	new DuiLib::CButtonGifUI();
		}
		return NULL;
	}
private:
	static std::tr1::shared_ptr<CDuiSelfInfoDlg> pInstance;
public:
	DuiLib::CButtonGifUI* m_btnHead;
	DuiLib::CButtonUI* m_btnEdit;
	DuiLib::CButtonUI* m_btnSave;
	DuiLib::CButtonUI* m_btnCancel;
	DuiLib::CTabLayoutUI* m_tabSelfInfo;
	DuiLib::CComboUI *m_comboSex;
	DuiLib::CComboUI *m_comboProvince;
	DuiLib::CComboUI *m_comboCity;
	DuiLib::CComboUI *m_comboCountry;
	DuiLib::CComboUI *m_comboYear;
	DuiLib::CComboUI *m_comboMonth;
	DuiLib::CComboUI *m_comboDay;
	DuiLib::CComboUI *m_comboCareer;

	DuiLib::CLabelUI* m_pLabelNameHead;
	DuiLib::CLabelUI* m_pLabelName;
	DuiLib::CLabelUI* m_pLabelSelfSex;
	DuiLib::CLabelUI* m_pLabelSelfcity;
	DuiLib::CLabelUI* m_pLabelSelfbirth;
	DuiLib::CLabelUI* m_pLabelSelfcellphone;
	DuiLib::CLabelUI* m_pLabelSelftelephone;
	DuiLib::CLabelUI* m_pLabelSelfmial;
	DuiLib::CLabelUI* m_pLabelSelfcareer;

	DuiLib::CLabelUI *m_pLevel;
	DuiLib::CLabelUI *m_pLabelLevel;
	DuiLib::CLabelUI *m_pLabelSec;
	DuiLib::CLabelUI *m_pLabelThird;
	
	void UpdateSelfInfo(BOOL bGifFlag = FALSE);
	void UpdateSelfEditInfo();
	BOOL SaveSelfEditInfo();

	DuiLib::CEditUI* m_pEditName;
	DuiLib::CEditUI* m_pEditCellphone;//editcellphone;
	DuiLib::CEditUI* m_pEditTelephone;//edittelephone;
	DuiLib::CEditUI* m_pEditMial;//editmial;

	DuiLib::COptionUI *m_pRadio0;    ////完全保密
	DuiLib::COptionUI *m_pRadio1;    ////好友可见
	DuiLib::COptionUI *m_pRadio2;    ////完全公开

	DuiLib::CVerticalLayoutUI* m_pVLhead;
	void OnUserInfoModAck(const char *data,WORD wRet);
	void OnUserInfoAck(const char *data,WORD wRet);
	CDuiModHeadIconDlg *m_pDuiModHeadDlg;
	int m_nLastReqSeq;
};

