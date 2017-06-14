#pragma once

#include <map>
using namespace std;

#include "Business.h"
#include "PacketBuffer.h"
#include "DataBaseLib.h"
#include "GlobalBusi.h"

//����ĳ������Э�����ĸ�ҵ��ģ����д������
class CDatabaseGW;

class CRouterBusiness
{
public:
	CRouterBusiness(int nType);
	~CRouterBusiness(void);

	//ע��һ��Э�����������ĸ�ҵ���ദ��
	bool Register( CBusiness* pBusiness, int nBusinessID);

	void RoutOne();

	//����Ҫ·�ɵİ����У�·�ɵİ����Ӹö�����ȡ��
	void SetPacketBuf(CFIFOPacketBuffer* pPacketBuf) { m_pPacketBuf = pPacketBuf ;	}

	//���û�ȡ���ݿ�ӿ�
	void SetDataBase(CDataBaseMini* pDataBaseMini,CDataBaseWeb* pDataBaseWeb,CDataBaseFamily* pDataBaseFamily)
	{ 
		m_pDataBaseMini = pDataBaseMini; 
		m_pDataBaseWeb = pDataBaseWeb ;
		m_pDataBaseFamily = pDataBaseFamily;
	}

	//�õ�mini�Լ������ݿ�
	CDataBaseMini* GetDataBaseMini()  { return m_pDataBaseMini;	}
	//�õ�mini����վ���õ����ݿ�
	CDataBaseWeb*  GetDataBaseWeb()  { return m_pDataBaseWeb;	}
	//�õ��������ݿ�����
	CDataBaseFamily* GetDataBaseFamily() { return m_pDataBaseFamily; }

	//�õ�ȫ����Ϣ��ѯ������
	CGlobalBusi* GetGlobalBusi()	{ return m_pGlobalBusi;  }
	//�õ�ȫ������Ⱥ��ѯ������
	CGlobalBusi* GetGlobalFamilyBusi() { return m_pGlobalFamilyBusi;}

	
	//����ȫ����Ϣ��ѯ������
	void SetGlobalBusi(CGlobalBusi* pGlobalBusi,CGlobalBusi* pGlobalFamilyBusi) 
	{
		m_pGlobalBusi		= pGlobalBusi;
		m_pGlobalFamilyBusi = pGlobalFamilyBusi;
	}

	SendDataStruct& GetSendData() { return m_SendDataStruct;	}

private:

	int				m_nType;		//·�ɰ�������
	map<int,CBusiness*>	 m_mapBusiness;
	CSyncCritical		m_Critical;
	CFIFOPacketBuffer*	m_pPacketBuf;
	BYTE				m_PacketBuf[sizeof(RecvDataStruct)];	//��ʱ�ַ���

	CDataBaseMini*		m_pDataBaseMini;
	CDataBaseWeb*		m_pDataBaseWeb;
	CDataBaseFamily*	m_pDataBaseFamily;

	CGlobalBusi*		m_pGlobalBusi;
	CGlobalBusi*		m_pGlobalFamilyBusi;

	SendDataStruct		m_SendDataStruct;			//ҵ���͵İ��ṹ
};

