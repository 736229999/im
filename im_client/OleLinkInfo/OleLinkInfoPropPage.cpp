// OleLinkInfoPropPage.cpp : COleLinkInfoPropPage 属性页类的实现。

#include "stdafx.h"
#include "OleLinkInfo.h"
#include "OleLinkInfoPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(COleLinkInfoPropPage, COlePropertyPage)



// 消息映射

BEGIN_MESSAGE_MAP(COleLinkInfoPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// 初始化类工厂和 guid

IMPLEMENT_OLECREATE_EX(COleLinkInfoPropPage, "OLELINKINFO.OleLinkInfoPropPage.1",
	0x2ddf36cf, 0x8cf, 0x47b6, 0x84, 0x25, 0xd0, 0xb, 0x1d, 0x8, 0x50, 0x43)



// COleLinkInfoPropPage::COleLinkInfoPropPageFactory::UpdateRegistry -
// 添加或移除 COleLinkInfoPropPage 的系统注册表项

BOOL COleLinkInfoPropPage::COleLinkInfoPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_OLELINKINFO_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// COleLinkInfoPropPage::COleLinkInfoPropPage - 构造函数

COleLinkInfoPropPage::COleLinkInfoPropPage() :
	COlePropertyPage(IDD, IDS_OLELINKINFO_PPG_CAPTION)
{
}



// COleLinkInfoPropPage::DoDataExchange - 在页和属性间移动数据

void COleLinkInfoPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// COleLinkInfoPropPage 消息处理程序
