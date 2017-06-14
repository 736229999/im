#pragma once

#include "BusinessExGSrv.h"


//查询在线家族全局资料
class CBusinessGetClusterInfo :
	public CBusinessExGSrv
{
public:
	CBusinessGetClusterInfo(CRouterBusiness* pRouter);

	virtual int DoWork();
};

//添加全局家族资料
class CBusinessAddCluster :
	public CBusinessExGSrv
{
public:
	CBusinessAddCluster(CRouterBusiness* pRouter);

	virtual int DoWork();
};

//删除全局家族资料
class CBusinessDelCluster :
	public CBusinessExGSrv
{
public:
	CBusinessDelCluster(CRouterBusiness* pRouter);

	virtual int DoWork();
};


//某台IM服务器停机通知,这时候需要清除该IM服务器的信息
class CBusinessImServerShutdown :
	public CBusinessExGSrv
{
public:
	CBusinessImServerShutdown(CRouterBusiness* pRouter);

	virtual int DoWork();
};
