// OleLinkInfoPropPage.cpp : COleLinkInfoPropPage ����ҳ���ʵ�֡�

#include "stdafx.h"
#include "OleLinkInfo.h"
#include "OleLinkInfoPropPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


IMPLEMENT_DYNCREATE(COleLinkInfoPropPage, COlePropertyPage)



// ��Ϣӳ��

BEGIN_MESSAGE_MAP(COleLinkInfoPropPage, COlePropertyPage)
END_MESSAGE_MAP()



// ��ʼ���๤���� guid

IMPLEMENT_OLECREATE_EX(COleLinkInfoPropPage, "OLELINKINFO.OleLinkInfoPropPage.1",
	0x2ddf36cf, 0x8cf, 0x47b6, 0x84, 0x25, 0xd0, 0xb, 0x1d, 0x8, 0x50, 0x43)



// COleLinkInfoPropPage::COleLinkInfoPropPageFactory::UpdateRegistry -
// ��ӻ��Ƴ� COleLinkInfoPropPage ��ϵͳע�����

BOOL COleLinkInfoPropPage::COleLinkInfoPropPageFactory::UpdateRegistry(BOOL bRegister)
{
	if (bRegister)
		return AfxOleRegisterPropertyPageClass(AfxGetInstanceHandle(),
			m_clsid, IDS_OLELINKINFO_PPG);
	else
		return AfxOleUnregisterClass(m_clsid, NULL);
}



// COleLinkInfoPropPage::COleLinkInfoPropPage - ���캯��

COleLinkInfoPropPage::COleLinkInfoPropPage() :
	COlePropertyPage(IDD, IDS_OLELINKINFO_PPG_CAPTION)
{
}



// COleLinkInfoPropPage::DoDataExchange - ��ҳ�����Լ��ƶ�����

void COleLinkInfoPropPage::DoDataExchange(CDataExchange* pDX)
{
	DDP_PostProcessing(pDX);
}



// COleLinkInfoPropPage ��Ϣ�������
