#pragma once

#include <map>
using namespace std;

#include "Business.h"
#include "PacketBuffer.h"
#include "DataBaseLib.h"
#include "GlobalBusi.h"

//调度某个具体协议由哪个业务模块进行处理的类
class CDatabaseGW;

class CRouterBusiness
{
public:
	CRouterBusiness(int nType);
	~CRouterBusiness(void);

	//注册一个协议命令字由哪个业务类处理
	bool Register( CBusiness* pBusiness, int nBusinessID);

	void RoutOne();

	//设置要路由的包队列，路由的包将从该队列中取出
	void SetPacketBuf(CFIFOPacketBuffer* pPacketBuf) { m_pPacketBuf = pPacketBuf ;	}

	//设置获取数据库接口
	void SetDataBase(CDataBaseMini* pDataBaseMini,CDataBaseWeb* pDataBaseWeb,CDataBaseFamily* pDataBaseFamily)
	{ 
		m_pDataBaseMini = pDataBaseMini; 
		m_pDataBaseWeb = pDataBaseWeb ;
		m_pDataBaseFamily = pDataBaseFamily;
	}

	//得到mini自己的数据库
	CDataBaseMini* GetDataBaseMini()  { return m_pDataBaseMini;	}
	//得到mini与网站共用的数据库
	CDataBaseWeb*  GetDataBaseWeb()  { return m_pDataBaseWeb;	}
	//得到家族数据库链接
	CDataBaseFamily* GetDataBaseFamily() { return m_pDataBaseFamily; }

	//得到全局信息查询服务器
	CGlobalBusi* GetGlobalBusi()	{ return m_pGlobalBusi;  }
	//得到全局在线群查询服务器
	CGlobalBusi* GetGlobalFamilyBusi() { return m_pGlobalFamilyBusi;}

	
	//设置全局信息查询服务器
	void SetGlobalBusi(CGlobalBusi* pGlobalBusi,CGlobalBusi* pGlobalFamilyBusi) 
	{
		m_pGlobalBusi		= pGlobalBusi;
		m_pGlobalFamilyBusi = pGlobalFamilyBusi;
	}

	SendDataStruct& GetSendData() { return m_SendDataStruct;	}

private:

	int				m_nType;		//路由包的类型
	map<int,CBusiness*>	 m_mapBusiness;
	CSyncCritical		m_Critical;
	CFIFOPacketBuffer*	m_pPacketBuf;
	BYTE				m_PacketBuf[sizeof(RecvDataStruct)];	//临时分发包

	CDataBaseMini*		m_pDataBaseMini;
	CDataBaseWeb*		m_pDataBaseWeb;
	CDataBaseFamily*	m_pDataBaseFamily;

	CGlobalBusi*		m_pGlobalBusi;
	CGlobalBusi*		m_pGlobalFamilyBusi;

	SendDataStruct		m_SendDataStruct;			//业务发送的包结构
};

