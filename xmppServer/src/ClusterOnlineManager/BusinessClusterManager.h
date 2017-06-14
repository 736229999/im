#pragma once

#include "BusinessExGSrv.h"


//��ѯ���߼���ȫ������
class CBusinessGetClusterInfo :
	public CBusinessExGSrv
{
public:
	CBusinessGetClusterInfo(CRouterBusiness* pRouter);

	virtual int DoWork();
};

//���ȫ�ּ�������
class CBusinessAddCluster :
	public CBusinessExGSrv
{
public:
	CBusinessAddCluster(CRouterBusiness* pRouter);

	virtual int DoWork();
};

//ɾ��ȫ�ּ�������
class CBusinessDelCluster :
	public CBusinessExGSrv
{
public:
	CBusinessDelCluster(CRouterBusiness* pRouter);

	virtual int DoWork();
};


//ĳ̨IM������ͣ��֪ͨ,��ʱ����Ҫ�����IM����������Ϣ
class CBusinessImServerShutdown :
	public CBusinessExGSrv
{
public:
	CBusinessImServerShutdown(CRouterBusiness* pRouter);

	virtual int DoWork();
};
