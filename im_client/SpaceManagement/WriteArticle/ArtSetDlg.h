#pragma once
#include "resource.h"


// CArtSetDlg �Ի���

class CArtSetDlg : public CDialog
{
	DECLARE_DYNAMIC(CArtSetDlg)

public:
	CArtSetDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CArtSetDlg();

// �Ի�������
	enum { IDD = IDD_ARTS_SET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
