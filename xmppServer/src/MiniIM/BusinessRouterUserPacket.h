#pragma once
#include "BusinessExIMSrv.h"
#include "ClientProtocol.h"
#include "PublicType.h"


class CBusinessRouterPacket : public CBusinessExIMSrv
{
public:
	CBusinessRouterPacket(CRouterBusiness* pRouter);
	~CBusinessRouterPacket();

	virtual int DoWork();
private:

};

class CBusinessRouterAckPacket : public CBusinessExIMSrv
{
public:
	CBusinessRouterAckPacket(CRouterBusiness* pRouter);
	~CBusinessRouterAckPacket();

	virtual int DoWork();
private:

};
class CBusinessRouterUserPacket : public CBusinessExIMSrv
{
public:
	CBusinessRouterUserPacket(CRouterBusiness* pRouter);
	~CBusinessRouterUserPacket();

	virtual int DoWork();
private:

};

class CBusinessRouterUserAckPacket : public CBusinessExIMSrv
{
public:
	CBusinessRouterUserAckPacket(CRouterBusiness* pRouter);
	~CBusinessRouterUserAckPacket();

	virtual int DoWork();
private:

};

class CBusinessReqTransFile : public CBusinessExIMSrv
{
public:
	CBusinessReqTransFile(CRouterBusiness* pRouter);
	~CBusinessReqTransFile();

	virtual int DoWork();

};


class CBusinessAckTransFile : public CBusinessExIMSrv
{
public:
	CBusinessAckTransFile(CRouterBusiness* pRouter);
	~CBusinessAckTransFile();

	virtual int DoWork();

};

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
class CBusinessReqCusterTran : public CBusinessExIMSrv
{
public:
	CBusinessReqCusterTran(CRouterBusiness* pRouter);
	~CBusinessReqCusterTran();

	virtual int DoWork();
};
class CBusinessAckCusterTran : public CBusinessExIMSrv
{
public:
	CBusinessAckCusterTran(CRouterBusiness* pRouter);
	~CBusinessAckCusterTran();

	virtual int DoWork();
};
