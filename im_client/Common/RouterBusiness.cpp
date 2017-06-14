#include "StdAfx.h"
#include "PublicType.h"
#include "Log.h"
#include ".\routerbusiness.h"
#include "RouterProtocol.h"

#pragma comment(lib,"../../lib/DataBaseLib.lib")

CRouterBusiness::CRouterBusiness(int nType)
{
	m_pPacketBuf = NULL;
	m_pDataBaseMini = NULL;
	m_pDataBaseWeb	= NULL;
	m_pGlobalBusi	= NULL;
	m_pGlobalFamilyBusi = NULL;
	m_nType = nType;
	CFIFOPacketBuffer* pPacketBufferArray = CRouterProtocol::GetInstance()->GetPacketBufferArray();
	SetPacketBuf(&(pPacketBufferArray[m_nType]));
}

CRouterBusiness::~CRouterBusiness(void)
{
}

bool CRouterBusiness::Register( CBusiness* pBusiness, int nBusinessID)
{
	assert(pBusiness);

	typedef pair <int, CBusiness*> Int_Pair;

	m_mapBusiness.insert( Int_Pair( nBusinessID, pBusiness ) );

	CRouterProtocol::GetInstance()->Register(m_nType,nBusinessID);

	return true;
}


void CRouterBusiness::RoutOne()
{
	int nDataLength = 0;
	XT_HEAD* pHead = NULL;
	map <int, CBusiness*> :: const_iterator BusiIter;
	CBusiness*	pBusiness = NULL;

	m_pPacketBuf->Pop(m_PacketBuf,nDataLength);

	if( nDataLength < sizeof(XT_HEAD))
	{
		CLog::Log("CRouterBusiness","���ݰ�û��һ����ͷ��С��������Size:%d",nDataLength);
		return;
	}

	//�����ܴ���ð���ҵ����
	pHead = (XT_HEAD*)(((RecvDataStruct*)m_PacketBuf)->data) ;
	BusiIter = m_mapBusiness.find(pHead->cmd);
	if ( BusiIter == m_mapBusiness.end() )
	{
		CLog::Log("CRouterBusiness","û�ҵ���ӦЭ��Ĵ���ģ�飬Э��ID��%d",pHead->cmd);
		return;
	}
	pBusiness = BusiIter->second;

	//��ʼҵ����

	CLog::Log(typeid(*pBusiness).name(),CLog::TYPE_DEBUG,"���յ��û�\'%d\' ����(0x%.4x)",
		pHead->sid, pHead->cmd);


	pBusiness->SetRowPacket(m_PacketBuf,nDataLength);
	pBusiness->DoWork();
	if( pBusiness->IsNeedReply())
	{
		pBusiness->SendReply();
		
	}
	pBusiness->Reset();	//�����Ա㴦����һ��ҵ��



}