#pragma once
#include "improtocol.pb.h"

#define MAX_SEARCH_FRINED_RETURN_NUM 50

#ifdef IM_USE_CACHE



int CMemcacheQueryUserStatusi( CRouterBusiness* m_pRouter,
		const uint32 *idList,uint8 &m_nReturnCount,
		GSRV_GET_USER_STATUS_ACK::tagUserStatus *userStatus);

int CMemcacheQueryUserStatusi(CRouterBusiness* m_pRouter,
				const uint32 *idList,uint8 &m_nReturnCount,
				XT_XTREAM_SEARCH_ACK::tagFriendInfo  *FriendInfo);

int CMemcacheQueryUserStatusi(CGlobalBusi* m_pGlobalBusi,
				const uint32 *idList,uint8 &m_nReturnCount,
				GSRV_GET_USER_STATUS_ACK::tagUserStatus *userStatus);
#ifndef IM_USE_IMAPI
#include "ClientService.h"

void CMemcacheUpdateConsultCSInfo(CRouterBusiness* m_pRouter,ClientService *clientService);

void CMemcacheUpdateConsultCSInfo(CRouterBusiness* m_pRouter,CSinfo_t userInfo);
#endif
int CalcQueueLength(int QueueOrigin,int row,int col);

int GetCSSubAccount_x(CRouterBusiness* m_pRouter,uint32 TeamID,uint32 buyer,vector<uint32> ids,uint32 &id);

void GetSubAccountInfo(CRouterBusiness* m_pRouter,uint32 id,SubAccountInf_t &Subinfo);

bool SortQueueMethod(const CSQList_t &x,const CSQList_t &y);

int PushQueueSubAccountToKafka(CRouterBusiness* m_pRouter,uint32 teamID);

int PushQueueToKafka(CRouterBusiness* m_pRouter,uint32 teamID,uint32 buyerId,uint32 id);

void SetWebContext(CRouterBusiness* m_pRouter,uint32 toid,uint32 csid,uint32 teamid);

void CsCustOper(CRouterBusiness* m_pRouter, CS_UPDATE_OPER op, uint32 csid, uint32 custid);

void CheckAndCleanQueue(CRouterBusiness* m_pRouter,tagGSRV_GET_ONLINE_CSQINFO_ACK::CSQinfo &qinfo);

void CheckAndCleanQueue(CRouterBusiness* m_pRouter,vector<CSQList_t> &m_Customerlist);

bool CheckQueueUserState(CRouterBusiness* m_pRouter,uint32 userid,bool &state);

#endif
