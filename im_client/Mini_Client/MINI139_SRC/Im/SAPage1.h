#pragma once

#include "SABasePage.h"
#include "FaceIconCtrl.h"
#include "impublic.h"
#include "userlevelctrl.h"

// �������á�����������
class CSAPage1 : public CSABasePage
{
	DECLARE_DYNAMIC(CSAPage1)

public:
	CSAPage1(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSAPage1();

	CXpButton	   m_btnSelFace;
	CFaceIconCtrl  m_wndFaceIconCtrl;
	CGraphComboBox m_comboSex;
	CFlatEdit	   m_editAge;
	CFlatEdit	   m_editNickName, m_editUserName;
	//CFlatEdit	   m_editCareer;
	CFlatEdit	   m_editSelfInfo;

// �Ի�������
	enum { IDD = IDD_SA_PAGE1 };

public:
	void SetUserInfo(const XT_USER_INFO &info);
	int  GetUserInfo(XT_USER_INFO &info);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnFacesel();
	afx_msg void OnCbnSelchangeComboSex();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()

	LRESULT OnWebIconDownload(WPARAM w,LPARAM l);
	LRESULT OnFaceSelNotify(WPARAM wParam,LPARAM lParam);
	
	bool m_bFaceModified;
	CString m_strFaceUrl;
	CUserLevelCtrl m_userLevel;
};
