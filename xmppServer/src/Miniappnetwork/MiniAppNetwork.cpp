// MiniAppNetwork.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "./ZAppNetwork.h"
#include "../../include/MiniAppNetwork.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

// �����ѵ�����Ĺ��캯����
// �й��ඨ�����Ϣ������� MiniAppNetwork.h
CMiniAppNetwork::CMiniAppNetwork()
{ 
	m_pAppNetwork = new CZAppNetwork();
}

CMiniAppNetwork::~CMiniAppNetwork()
{
	delete (CZAppNetwork*)m_pAppNetwork; m_pAppNetwork = NULL;
}

/*
��������: �����û��ص������ͻص�����
��Ҫ����˵����pUserKey���û��ص�����
				pfnCallback���û��ص�����
�� �� ֵ��
*/ 
void CMiniAppNetwork::SetUserCallback(void * pUserKey,pfnAppNetCallback pfnCallback)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	pAppNetwork->SetUserCallback(pUserKey,pfnCallback);
}

/*
��������: ����Ӧ����������
��Ҫ����˵����addr��IP ��ַ
				prot��udp�����˿�
�� �� ֵ��0 �ɹ���< 0 ʧ��
*/ 
int CMiniAppNetwork::StartWork(const char * addr,uint16 port)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	return pAppNetwork->StartWork(addr,port);
}

LPONLINE_USER_INFO CMiniAppNetwork::FindOnlineUser(uint32 uid)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	return pAppNetwork->FindOnlineUser(uid);
}

/*
��������: �û���½
��Ҫ����˵����uid���û�ID
�� �� ֵ��
*/ 
int CMiniAppNetwork::UserLogin(uint32 uid,uint32 uip,uint16 uport,const char * pSessionKey ,const char * pP2PSessionKey,uint32 lanip,uint16 lanport, uint8 flag)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	return pAppNetwork->UserLogin(uid,uip,uport,pSessionKey,pP2PSessionKey,lanip,lanport,flag);
}

void CMiniAppNetwork::UserLogout(uint32 uid)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	pAppNetwork->UserLogout(uid);
}

/*
��������: �����½
��Ҫ����˵����fid������ID
�� �� ֵ��
*/ 
void CMiniAppNetwork::FamilyLogin(uint32 fid,uint8 ftype/* = PERM_FAMILY*/)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	pAppNetwork->FamilyLogin(fid);
}
void CMiniAppNetwork::FamilyLogout(uint32 fid,uint8 ftype/* = PERM_FAMILY*/)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	pAppNetwork->FamilyLogout(fid);
}


/*
��������: ��������
��Ҫ����˵����uid ��Ŀ���û�
				data�����͵�����
				size�����ݳ���
				retrys���ط�����
				interval���ط����
�� �� ֵ��0 �ɹ���< 0 ʧ��
*/ 
int CMiniAppNetwork::SendDataTo(const char * data,int size,int retrys ,uint16 interval,uint32 ip ,uint16 port,uint32 vlink)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	return pAppNetwork->SendDataToA(data,size,retrys,interval,ip,port,vlink);
}

//ֱ��ͨ������㷢������
int CMiniAppNetwork::SendDataTo(SendDataStruct & sendData)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	return pAppNetwork->SendDataToA(sendData);	
}

void CMiniAppNetwork::StopWork()
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	pAppNetwork->StopWork();
}

uint16 CMiniAppNetwork::GetPort() 
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	return pAppNetwork->GetPort();
}
uint32 CMiniAppNetwork::GetLocalIP()
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	return pAppNetwork->GetLocalIP();
}

CUdpNetTrans*  CMiniAppNetwork::GetSrvNetTrans()
{
	return ((CZAppNetwork *)m_pAppNetwork)->GetSrvNetTrans();
}
CUdpNetTrans*  CMiniAppNetwork::GetCntNetTrans()
{
	return ((CZAppNetwork *)m_pAppNetwork)->GetCntNetTrans();
}

void CMiniAppNetwork::AddImServer(uint32 srvid,uint32 srvip)
{
	((CZAppNetwork *)m_pAppNetwork)->AddImServer(srvid,srvip);
}

void CMiniAppNetwork::DelImServer(uint32 srvid)
{
	((CZAppNetwork *)m_pAppNetwork)->DelImServer(srvid);
}

int CMiniAppNetwork::UserType(uint32 id)
{
	return ((CZAppNetwork *)m_pAppNetwork)->UserType(id);
}