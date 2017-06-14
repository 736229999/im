#pragma once
#include "..\resource.h"
#include "..\im\impublic.h"
#include "..\..\..\DuiLib\UIlib.h"
////
class CSysCommonAudio : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UISecurityAudioCfg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("AudioSetting.xml");  } 
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	// 保存
	void Save();

private:
	    DuiLib::CComboUI *m_comboVoiceInput;
	    DuiLib::CComboUI *m_comboVoiceOutput;
	    DuiLib::CComboUI *m_comboCameraSel;

		DuiLib::CSliderUI *m_pSliderMicro;
		DuiLib::CSliderUI *m_pSliderVoice;
};


////class CUISlider : public DuiLib::WindowImplBase
////{
////public:
////	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UISlider");  }
////	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("Slider.xml");  }
////	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
////	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
////	LPCTSTR GetResourceID() const;
////	void SetThumbPos(int nThumbPos);
////	int GetThumbPos();
////};
////
////// CSysCommonAudio 对话框
////
////class CSysCommonAudio : public CDialog
////{
////	DECLARE_DYNAMIC(CSysCommonAudio)
////
////public:
////	CSysCommonAudio(CWnd* pParent = NULL);   // 标准构造函数
////	virtual ~CSysCommonAudio();
////
////// 对话框数据
////	enum { IDD = IDD_WIZARD_SYS_COMMON_AUDIO };
////
////	void Save();
////
////protected:
////	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
////	virtual BOOL OnInitDialog();
////	void OnOK()	{}
////	void OnCancel() {}
////	DECLARE_MESSAGE_MAP()
////public:
////	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
////	CPngBtn m_btnMicro;
////	CPngBtn m_btnVoice;
////
////private:
////	CUISlider	*m_pSliderMicro;
////	CUISlider	*m_pSliderVoice;
////
////	CGraphComboBox m_comboVoiceInput;
////	CGraphComboBox m_comboVoiceOutput;
////	CGraphComboBox m_comboCameraSel;
////};
