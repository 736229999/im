/************************************************************************ 
 文件描述：动态负载业务，客户端获取服务器状态列表
 
 创建人： Zouyf, 2006-6-29 
 
 修改记录：  
            
************************************************************************/

#pragma once
#include "BusinessExIMSrv.h"

class CBusiGetSrvStatusLst : public CBusinessExIMSrv
{
public:
	CBusiGetSrvStatusLst(CRouterBusiness* pRouter);
	virtual ~CBusiGetSrvStatusLst(void);

	virtual int DoWork();
};

