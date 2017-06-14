#pragma once

// OleLinkInfoPropPage.h : COleLinkInfoPropPage 属性页类的声明。


// COleLinkInfoPropPage : 有关实现的信息，请参阅 OleLinkInfoPropPage.cpp。

class COleLinkInfoPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(COleLinkInfoPropPage)
	DECLARE_OLECREATE_EX(COleLinkInfoPropPage)

// 构造函数
public:
	COleLinkInfoPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_OLELINKINFO };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
};

