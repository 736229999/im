#pragma  once

#include "BusinessExIMSrv.h"
#include "ClientProtocol.h"
#include "PublicType.h"

//Ⱥ��ʱ�ļ���ͼƬ�������ϴ�
class CBusinessFileUpload : public CBusinessExIMSrv
{
public:
	CBusinessFileUpload(CRouterBusiness* pRouter);

	virtual int DoWork();

private:


};

//Ⱥ��ʱ�ļ���ͼƬ����������
class CBusinessFileDownload : public CBusinessExIMSrv
{
public:
	CBusinessFileDownload(CRouterBusiness* pRouter);

	virtual int DoWork();

private:


};

