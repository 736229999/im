#pragma once
#include "Im\IMProtocol.h"
#include "..\..\..\HmgSkin\HmgSkin\RadioButton.h"
#include "..\..\..\DuiLib\Control\UIButtonGif.h"

// CSelfInfoEditDlg 对话框
#define WM_SELF_INFO_EDIT_DLG_MSG (WM_USER+362)
class CUIAddrComboBox;
class CSelfInfoEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelfInfoEditDlg)

public:
	CSelfInfoEditDlg(CWnd* pParent);   // 标准构造函数
	virtual ~CSelfInfoEditDlg();
	BOOL Create();
	void ShowFriendInfo();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedSave();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeComboProvince();
	afx_msg void OnCbnSelchangeComboCity();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	void OnOK()	{}
	void OnCancel() {}

	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

private:
	// 修改个人资料时后台返回消息
	void OnUserInfoModAck(const char *data,WORD wRet);
	// 选择省份
	void SelectProvince(int nProvinceID);
	// 选择市
	void SelectCity(int nCityID);
	// 选择区县
	void SelectDistrict(int nDistrictID);
	void OnUserInfoAck(const char *data,WORD wRet);
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (_tcsicmp(pstrClass, _T("ButtonGif")) == 0)
		{
			return	new DuiLib::CButtonGifUI();
		}
		return NULL;
	}
private:
	HBRUSH m_hBkGround;
	CPngBtn m_btnSave;
	CPngBtn m_btnCancel;

	CFlatEdit m_edtName;				//昵称
	CGraphComboBox  m_comboSex;			//性别
	CFlatDateTimeCtrl m_DateTimeBirth;	//生日
	CUIAddrComboBox  *m_pcomboProvince;	//省
	CUIAddrComboBox  *m_pcomboCity;		//市
	CUIAddrComboBox  *m_pcomboCounty;	//县
	CFlatEdit m_edtPhone;				//手机	
	CFlatEdit m_edtTel;					//电话	
	CFlatEdit m_edtEmail;				//邮箱
	CGraphComboBox  m_comboCareer;		//行业
	int m_nPublicLevel;					//公开等级
	CGraphComboBox  m_comboDateYear;		//年
	CGraphComboBox  m_comboDateMonth;		//年
	CGraphComboBox  m_comboDateDay;		//年
	CRadioButton m_radioPublic;
	CRadioButton m_radioPublicToFriend;
	CRadioButton m_radioPrivate;
	int m_nLastReqSeq;

public:
	afx_msg void OnCbnSelchangeComboDateYear();
	afx_msg void OnCbnSelchangeComboDateMonth();
	afx_msg void OnCbnSelchangeComboDateDay();
	afx_msg LRESULT OnAddrComboSelChange(WPARAM wp, LPARAM lp);
};
