#include "stdafx.h"
#include "ConnBusi.h"
#include "xtpacket.h"
#include "Log.h"
#include "ConnBusiControl.h"

CConnBusi::CConnBusi(CConnBusiControl * pConnBusiControl)
{
	assert(pConnBusiControl != NULL);
	m_pConnBusiControl = pConnBusiControl;
}

CConnBusi::~CConnBusi(void)
{

}

void CConnBusi::BusiRequest(const LPSERVER_REGISTER lpReq)
{
	SendDataStruct sendData;
	CSendPacket sendPacket(sendData.data);
	
	XT_HEAD head;
	m_pConnBusiControl->InitHead(&head,CMD_SERVER_REGISTER,sizeof(SERVER_REGISTER));

	sendPacket.SetXtHead(head);
	sendPacket.WritePacket((const char *)lpReq,sizeof(SERVER_REGISTER));
	sendData.data_len = sendPacket.GetSize();
	
	m_pConnBusiControl->SendRequestData(sendData);
}

void CConnBusi::BusiRequest(const LPSERVER_UNREGISTER lpReq)
{
	SendDataStruct sendData;
	CSendPacket sendPacket(sendData.data);
	
	XT_HEAD head;
	m_pConnBusiControl->InitHead(&head,CMD_SERVER_UNREGISTER,sizeof(SERVER_UNREGISTER));

	sendPacket.SetXtHead(head);
	sendPacket.WritePacket((const char *)lpReq,sizeof(SERVER_UNREGISTER));
	sendData.data_len = sendPacket.GetSize();

	m_pConnBusiControl->SendRequestData(sendData);
}
void CConnBusi::BusiRequest(const LPSERVER_STATUS_REPORT lpReq)
{
	SendDataStruct sendData;
	CSendPacket sendPacket(sendData.data);
	
	XT_HEAD head;
	m_pConnBusiControl->InitHead(&head,CMD_SERVER_STATUS_REPORT,sizeof(SERVER_STATUS_REPORT));

	sendPacket.SetXtHead(head);
	sendPacket.WritePacket((const char *)lpReq,sizeof(SERVER_STATUS_REPORT));
	sendData.data_len = sendPacket.GetSize();

	m_pConnBusiControl->SendRequestData(sendData);
}
void CConnBusi::BusiRequest(const LPSERVER_STATUSLIST_REQ lpReq)
{
	SendDataStruct sendData;
	CSendPacket sendPacket(sendData.data);
	
	XT_HEAD head;
	m_pConnBusiControl->InitHead(&head,CMD_SERVER_STATUSLIST_REQ,sizeof(SERVER_STATUSLIST_REQ));

	sendPacket.SetXtHead(head);
	sendPacket.WritePacket((const char *)lpReq,sizeof(SERVER_STATUSLIST_REQ));
	sendData.data_len = sendPacket.GetSize();

	m_pConnBusiControl->SendRequestData(sendData);
}

