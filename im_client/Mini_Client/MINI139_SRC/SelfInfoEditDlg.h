#pragma once
#include "Im\IMProtocol.h"
#include "..\..\..\HmgSkin\HmgSkin\RadioButton.h"
#include "..\..\..\DuiLib\Control\UIButtonGif.h"

// CSelfInfoEditDlg �Ի���
#define WM_SELF_INFO_EDIT_DLG_MSG (WM_USER+362)
class CUIAddrComboBox;
class CSelfInfoEditDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelfInfoEditDlg)

public:
	CSelfInfoEditDlg(CWnd* pParent);   // ��׼���캯��
	virtual ~CSelfInfoEditDlg();
	BOOL Create();
	void ShowFriendInfo();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
	// �޸ĸ�������ʱ��̨������Ϣ
	void OnUserInfoModAck(const char *data,WORD wRet);
	// ѡ��ʡ��
	void SelectProvince(int nProvinceID);
	// ѡ����
	void SelectCity(int nCityID);
	// ѡ������
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

	CFlatEdit m_edtName;				//�ǳ�
	CGraphComboBox  m_comboSex;			//�Ա�
	CFlatDateTimeCtrl m_DateTimeBirth;	//����
	CUIAddrComboBox  *m_pcomboProvince;	//ʡ
	CUIAddrComboBox  *m_pcomboCity;		//��
	CUIAddrComboBox  *m_pcomboCounty;	//��
	CFlatEdit m_edtPhone;				//�ֻ�	
	CFlatEdit m_edtTel;					//�绰	
	CFlatEdit m_edtEmail;				//����
	CGraphComboBox  m_comboCareer;		//��ҵ
	int m_nPublicLevel;					//�����ȼ�
	CGraphComboBox  m_comboDateYear;		//��
	CGraphComboBox  m_comboDateMonth;		//��
	CGraphComboBox  m_comboDateDay;		//��
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
