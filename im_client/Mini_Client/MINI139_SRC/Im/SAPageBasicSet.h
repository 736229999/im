#pragma once

#include "SABasePage.h"
#include "afxwin.h"


class CSAPageBasicSet : public CSABasePage
{
	DECLARE_DYNAMIC(CSAPageBasicSet)

public:
	CSAPageBasicSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSAPageBasicSet();

	void	SaveConfig();

// �Ի�������
	enum { IDD = IDD_SAPAGE_BASICSET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CButton m_btnCheckMagic;
	CButton m_btnCheckMiniPictureBroadCast;
	virtual BOOL OnInitDialog();
	//CButton m_btnFriendPic;
	//CButton m_btnFriendArt;
};
