/************************************************************************ 
 �ļ���������̬����ҵ�񣬿ͻ��˻�ȡ������״̬�б�
 
 �����ˣ� Zouyf, 2006-6-29 
 
 �޸ļ�¼��  
            
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

