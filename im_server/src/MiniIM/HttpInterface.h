#ifndef __HTTP_INTERFACE_H__
#define __HTTP_INTERFACE_H__

#include "PublicType.h"
#include "ClientProtocol.h"

// 获取最近联系商家
void HttpGetRecentSubMerchant(uint32 nUserID, int nMaxCount, XT_WEB_BUSI_LIST_ACK::BusiList* pListInfo,uint32& nRetCount);

#endif
