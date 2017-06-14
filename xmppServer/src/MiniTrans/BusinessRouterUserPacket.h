#pragma once
#include "BusinessExIMSrv.h"
#include "ClientProtocol.h"
#include "PublicType.h"

class CBusinessTransfer : public CBusinessExIMSrv
{
public:
	CBusinessTransfer(CRouterBusiness* pRouter);
	~CBusinessTransfer();

	virtual int DoWork();
};

class CBusinessTransferInd : public CBusinessExIMSrv
{
public:
	CBusinessTransferInd(CRouterBusiness* pRouter);
	~CBusinessTransferInd();

	virtual int DoWork();
};

class CBusinessTransferNotify : public CBusinessExIMSrv
{
public:
	CBusinessTransferNotify(CRouterBusiness* pRouter);
	~CBusinessTransferNotify();

	virtual int DoWork();
};

