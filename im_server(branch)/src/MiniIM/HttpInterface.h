#ifndef __HTTP_INTERFACE_H__
#define __HTTP_INTERFACE_H__

#include "PublicType.h"
#include "ClientProtocol.h"

// 获取最近联系商家
void HttpGetRecentSubMerchant(uint32 nUserID, int nMaxCount, XT_WEB_BUSI_LIST_ACK::BusiList* pListInfo,uint32& nRetCount);
#ifdef XMPP_BUILD
// 商家分流
void HttpSellerFlow(uint32 nUserID, int nUserLevel, uint32 nMerchantID, int nGroupID, int nRequestTime, XT_GET_SUB_ACCOUNT_ACK &ack);
#endif
#endif
