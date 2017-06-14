#pragma once

#include <map>
using namespace std;

#include "SyncCritical.h"
#include "PacketBuffer.h"
#include "PublicType.h"

//���ڵ���һ���ض�Э�������ĸ����͵Ķ��У�����Э��Ĳ�ͬ�ŵ���ͬ
//�Ĵ�������У��ж�Э�������ĸ����еĻ��������ǿ���Э�鴦���Ƿ�
//�����ݿ��������ʱ���


class CRouterProtocol
{
public:

	~CRouterProtocol(void);

	static CRouterProtocol* GetInstance();

	//ע��һ��Э���������ദ�����
	//nProtocolID--�������ID
	//nBusinessID--Э��ID
	bool Register(int nProtocolID,uint16 nBusinessID);

	//·��һ���������ʵĴ������
	void RouterOnePacket(const RecvDataStruct* pRecvData);

	CFIFOPacketBuffer* GetPacketBufferArray()  { return m_pPacketBufferArray;}

	void SetPacketBufferArray(CFIFOPacketBuffer* pPacketBufferArray)
	{
		m_pPacketBufferArray = pPacketBufferArray;
	}

protected:
		CRouterProtocol(void);
private:
	static CRouterProtocol* m_pInstance;


	map<uint16,int>		m_mapProtocol;
	CSyncCritical		m_Critical;


	CFIFOPacketBuffer*	m_pPacketBufferArray;
	
};

