#pragma once

// OleLinkInfoPropPage.h : COleLinkInfoPropPage ����ҳ���������


// COleLinkInfoPropPage : �й�ʵ�ֵ���Ϣ������� OleLinkInfoPropPage.cpp��

class COleLinkInfoPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(COleLinkInfoPropPage)
	DECLARE_OLECREATE_EX(COleLinkInfoPropPage)

// ���캯��
public:
	COleLinkInfoPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_OLELINKINFO };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
};

